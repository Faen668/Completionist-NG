#pragma once

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

// Drawing helpers shared by the theme ornaments
namespace CompletionistUI::DecorDraw
{

	//---------------------------------------------------
	//-- Decor Draw ( With Alpha ) ----------------------
	//---------------------------------------------------

	inline ImU32 WithAlpha(ImU32 a_colour, float a_alpha) {

		const auto alpha = static_cast<ImU32>(((a_colour >> IM_COL32_A_SHIFT) & 0xFF) * std::clamp(a_alpha, 0.0f, 1.0f));
		return (a_colour & ~IM_COL32_A_MASK) | (alpha << IM_COL32_A_SHIFT);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Shade ) ---------------------------
	//---------------------------------------------------

	// Darker or lighter, keeps alpha
	inline ImU32 Shade(ImU32 a_colour, float a_factor) {

		const auto channel = [&](int a_shift) {
			const float value = static_cast<float>((a_colour >> a_shift) & 0xFF) * a_factor;
			return static_cast<ImU32>(std::clamp(value, 0.0f, 255.0f)) << a_shift;
		};
		return channel(IM_COL32_R_SHIFT) | channel(IM_COL32_G_SHIFT) | channel(IM_COL32_B_SHIFT) | (a_colour & IM_COL32_A_MASK);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Random ) --------------------------
	//---------------------------------------------------

	// Same sequence every frame, so the decoration stays put
	struct Random
	{
		std::uint32_t state;

		float Next() {

			state ^= state << 13;
			state ^= state >> 17;
			state ^= state << 5;
			return static_cast<float>(state & 0xFFFFFF) / static_cast<float>(0x1000000);
		}

		float Range(float a_low, float a_high) { return a_low + (a_high - a_low) * Next(); }
	};

	//---------------------------------------------------
	//-- Decor Draw ( Gradient Quad ) -------------------
	//---------------------------------------------------

	// Four corners, four colours, for light shafts and haze
	inline void GradientQuad(ImDrawList* a_drawList, const ImVec2 (&a_points)[4], const ImU32 (&a_colours)[4]) {

		const auto uv = ImGui::GetFontTexUvWhitePixel();
		a_drawList->PrimReserve(6, 4);
		const auto base = static_cast<ImDrawIdx>(a_drawList->_VtxCurrentIdx);
		for (int i = 0; i < 4; ++i) {
			a_drawList->PrimWriteVtx(a_points[i], uv, a_colours[i]);
		}
		a_drawList->PrimWriteIdx(base);
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 1));
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 2));
		a_drawList->PrimWriteIdx(base);
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 2));
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 3));
	}

	//---------------------------------------------------
	//-- Decor Draw ( Soft Spot ) -----------------------
	//---------------------------------------------------

	// Round glow, rings fading out
	inline void SoftSpot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		constexpr int rings = 8;
		for (int ring = rings; ring > 0; --ring) {
			a_drawList->AddCircleFilled(a_centre, a_radius * static_cast<float>(ring) / rings, a_colour, 32);
		}
	}

	//---------------------------------------------------
	//-- Decor Draw ( Edge Shadow ) ---------------------
	//---------------------------------------------------

	// Darkens a rect towards its edges
	inline void EdgeShadow(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_fade, ImU32 a_dark) {

		const ImU32 clear = WithAlpha(a_dark, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + a_fade), a_dark, a_dark, clear, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - a_fade), a_max, clear, clear, a_dark, a_dark);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + a_fade, a_max.y), a_dark, clear, clear, a_dark);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_max.x - a_fade, a_min.y), a_max, clear, a_dark, a_dark, clear);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Edge Weight ) ---------------------
	//---------------------------------------------------

	// 1 at a rect's edge, falling to 0 a_band in, so panel dressing clears behind the text
	inline float EdgeWeight(ImVec2 a_point, ImVec2 a_min, ImVec2 a_max, float a_band) {

		const float distance = std::min({ a_point.x - a_min.x, a_max.x - a_point.x, a_point.y - a_min.y, a_max.y - a_point.y });
		const float t = 1.0f - std::clamp(distance / a_band, 0.0f, 1.0f);
		return t * t;
	}
}

namespace CompletionistUI::DecorDraw
{
	//---------------------------------------------------
	//-- Decor Draw ( Perk Curve ) ----------------------
	//---------------------------------------------------

	// A perk tree link: leaves one perk and reaches the next level, so branches read left to right
	inline std::vector<ImVec2> PerkCurve(ImVec2 a_from, ImVec2 a_to, float a_step) {

		const float reach = (a_to.x - a_from.x) * 0.5f;
		const ImVec2 c1(a_from.x + reach, a_from.y);
		const ImVec2 c2(a_to.x - reach, a_to.y);
		const float length = std::hypot(a_to.x - a_from.x, a_to.y - a_from.y);
		const int   steps = std::clamp(static_cast<int>(length / std::max(1.0f, a_step)), 2, 96);

		std::vector<ImVec2> points;
		points.reserve(static_cast<std::size_t>(steps) + 1);
		for (int i = 0; i <= steps; ++i) {
			const float t = static_cast<float>(i) / steps;
			const float u = 1.0f - t;
			const float a = u * u * u, b = 3.0f * u * u * t, c = 3.0f * u * t * t, d = t * t * t;
			points.emplace_back(a * a_from.x + b * c1.x + c * c2.x + d * a_to.x, a * a_from.y + b * c1.y + c * c2.y + d * a_to.y);
		}
		return points;
	}

	//---------------------------------------------------
	//-- Decor Draw ( Normal At ) -----------------------
	//---------------------------------------------------

	// Unit normal of a polyline at a point, for strokes that sit either side of it
	inline ImVec2 NormalAt(const std::vector<ImVec2>& a_points, std::size_t a_index) {

		const auto& a = a_points[a_index == 0 ? 0 : a_index - 1];
		const auto& b = a_points[std::min(a_index + 1, a_points.size() - 1)];
		const float dx = b.x - a.x;
		const float dy = b.y - a.y;
		const float length = std::max(0.001f, std::hypot(dx, dy));
		return ImVec2(-dy / length, dx / length);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Offset Path ) ---------------------
	//---------------------------------------------------

	inline std::vector<ImVec2> OffsetPath(const std::vector<ImVec2>& a_points, float a_offset) {

		std::vector<ImVec2> out;
		out.reserve(a_points.size());
		for (std::size_t i = 0; i < a_points.size(); ++i) {
			const auto normal = NormalAt(a_points, i);
			out.emplace_back(a_points[i].x + normal.x * a_offset, a_points[i].y + normal.y * a_offset);
		}
		return out;
	}
}

// Shading and baking for the themes with fine painted surfaces
namespace CompletionistUI::DecorDraw
{
	//---------------------------------------------------
	//-- Decor Draw ( Mix ) -------------------------
	//---------------------------------------------------

	inline ImU32 Mix(ImU32 a_from, ImU32 a_to, float a_t) {

		const float t = std::clamp(a_t, 0.0f, 1.0f);
		const auto  channel = [&](int a_shift) {
			const float from = static_cast<float>((a_from >> a_shift) & 0xFF);
			const float to = static_cast<float>((a_to >> a_shift) & 0xFF);
			return static_cast<ImU32>(from + (to - from) * t) << a_shift;
		};
		return channel(IM_COL32_R_SHIFT) | channel(IM_COL32_G_SHIFT) | channel(IM_COL32_B_SHIFT) | channel(IM_COL32_A_SHIFT);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Noise ) -----------------------
	//---------------------------------------------------

	inline float Hash(int a_x, int a_y) {

		auto h = static_cast<std::uint32_t>(a_x) * 374761393u + static_cast<std::uint32_t>(a_y) * 668265263u;
		h = (h ^ (h >> 13)) * 1274126177u;
		return static_cast<float>((h ^ (h >> 16)) & 0xFFFF) / 65535.0f;
	}

	inline float ValueNoise(float a_x, float a_y) {

		const int   x = static_cast<int>(std::floor(a_x));
		const int   y = static_cast<int>(std::floor(a_y));
		const float fx = a_x - static_cast<float>(x);
		const float fy = a_y - static_cast<float>(y);
		const float u = fx * fx * (3.0f - 2.0f * fx);
		const float v = fy * fy * (3.0f - 2.0f * fy);
		const float top = Hash(x, y) + (Hash(x + 1, y) - Hash(x, y)) * u;
		const float bottom = Hash(x, y + 1) + (Hash(x + 1, y + 1) - Hash(x, y + 1)) * u;
		return top + (bottom - top) * v;
	}

	// Around 0.5, roughly 0 to 1
	inline float Fbm(float a_x, float a_y, int a_octaves) {

		float total = 0.0f;
		float weight = 0.5f;
		for (int octave = 0; octave < a_octaves; ++octave) {
			total += ValueNoise(a_x, a_y) * weight;
			a_x = a_x * 2.03f + 17.0f;
			a_y = a_y * 2.03f + 9.0f;
			weight *= 0.5f;
		}
		return total / (1.0f - std::pow(0.5f, static_cast<float>(a_octaves)));
	}

	inline float Smooth(float a_t) {

		const float t = std::clamp(a_t, 0.0f, 1.0f);
		return t * t * (3.0f - 2.0f * t);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Baked ) -----------------------
	//---------------------------------------------------

	// The paper and the etchings are far too fine to work out every frame, and they never change:
	// each is drawn once and its triangles reused until the window moves or resizes
	struct BakedArt
	{
		std::uint32_t          id{ 0 };
		ImVec2                 min;
		ImVec2                 max;
		float                  size{ 0.0f };
		ImVec2                 uv;
		std::uint32_t          lastUsed{ 0 };
		std::vector<ImDrawVert> vertices;
		std::vector<ImDrawIdx>  indices;
	};

	inline void Blit(ImDrawList* a_drawList, const BakedArt& a_art, ImVec2 a_offset = ImVec2(0.0f, 0.0f)) {

		const auto vertexCount = static_cast<int>(a_art.vertices.size());
		const auto indexCount = static_cast<int>(a_art.indices.size());
		if (vertexCount == 0) {
			return;
		}
		a_drawList->PrimReserve(indexCount, vertexCount);
		const auto base = static_cast<ImDrawIdx>(a_drawList->_VtxCurrentIdx);
		std::memcpy(a_drawList->_VtxWritePtr, a_art.vertices.data(), a_art.vertices.size() * sizeof(ImDrawVert));
		if (a_offset.x != 0.0f || a_offset.y != 0.0f) {
			for (int i = 0; i < vertexCount; ++i) {
				a_drawList->_VtxWritePtr[i].pos.x += a_offset.x;
				a_drawList->_VtxWritePtr[i].pos.y += a_offset.y;
			}
		}
		a_drawList->_VtxWritePtr += vertexCount;
		for (const auto index : a_art.indices) {
			*a_drawList->_IdxWritePtr++ = static_cast<ImDrawIdx>(base + index);
		}
		a_drawList->_VtxCurrentIdx += static_cast<unsigned int>(vertexCount);
	}

	// Paints into a_art's triangles rather than a draw list; false if it was too big to keep
	template <class Paint>
	bool Record(ImDrawList* a_drawList, BakedArt& a_art, Paint&& a_paint) {

		ImDrawList scratch(ImGui::GetDrawListSharedData());
		scratch._ResetForNewFrame();
		scratch.Flags = a_drawList->Flags & ~ImDrawListFlags_AllowVtxOffset;
		scratch.PushTextureID(a_drawList->_CmdHeader.TextureId);
		scratch.PushClipRectFullScreen();
		a_paint(&scratch);
		if (scratch.VtxBuffer.Size >= 60000) {
			return false;
		}
		a_art.vertices.assign(scratch.VtxBuffer.Data, scratch.VtxBuffer.Data + scratch.VtxBuffer.Size);
		a_art.indices.assign(scratch.IdxBuffer.Data, scratch.IdxBuffer.Data + scratch.IdxBuffer.Size);
		return true;
	}

	inline std::vector<BakedArt>& BakedCache() {

		static std::vector<BakedArt> cache;
		return cache;
	}

	template <class Paint>
	void Baked(ImDrawList* a_drawList, std::uint32_t a_id, ImVec2 a_min, ImVec2 a_max, Paint&& a_paint) {

		auto&       cache = BakedCache();
		static auto clock = std::uint32_t{ 0 };
		++clock;

		const float size = ImGui::GetFontSize();
		const auto  uv = ImGui::GetFontTexUvWhitePixel();
		const auto  same = [](ImVec2 a, ImVec2 b) { return a.x == b.x && a.y == b.y; };
		for (auto& art : cache) {
			if (art.id == a_id && art.size == size && same(art.min, a_min) && same(art.max, a_max) && same(art.uv, uv)) {
				art.lastUsed = clock;
				Blit(a_drawList, art);
				return;
			}
		}

		// Too big for 16-bit indices in one go: draw it directly, the list splits it as needed
		BakedArt art{ a_id, a_min, a_max, size, uv, clock };
		if (!Record(a_drawList, art, a_paint)) {
			a_paint(a_drawList);
			return;
		}
		Blit(a_drawList, art);

		if (cache.size() >= 24) {
			const auto oldest = std::ranges::min_element(cache, {}, &BakedArt::lastUsed);
			*oldest = std::move(art);
		} else {
			cache.push_back(std::move(art));
		}
	}

	//---------------------------------------------------
	//-- Decor Draw ( Mesh ) ------------------------
	//---------------------------------------------------

	// A grid of soft colours, for shading no flat fill can give
	template <class Shader>
	void Mesh(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_cellX, float a_cellY, Shader&& a_shader) {

		const int columns = std::max(1, static_cast<int>(std::ceil((a_max.x - a_min.x) / a_cellX)));
		const int rows = std::max(1, static_cast<int>(std::ceil((a_max.y - a_min.y) / a_cellY)));
		const auto uv = ImGui::GetFontTexUvWhitePixel();
		const int  stride = columns + 1;

		// In rows that a 16-bit index can always reach
		constexpr int chunk = 120;
		for (int start = 0; start < rows; start += chunk) {
			const int count = std::min(chunk, rows - start);
			a_drawList->PrimReserve(columns * count * 6, stride * (count + 1));
			const auto base = a_drawList->_VtxCurrentIdx;
			for (int row = 0; row <= count; ++row) {
				const float y = a_min.y + (a_max.y - a_min.y) * static_cast<float>(start + row) / static_cast<float>(rows);
				for (int column = 0; column <= columns; ++column) {
					const float x = a_min.x + (a_max.x - a_min.x) * static_cast<float>(column) / static_cast<float>(columns);
					a_drawList->PrimWriteVtx(ImVec2(x, y), uv, a_shader(x, y));
				}
			}
			for (int row = 0; row < count; ++row) {
				for (int column = 0; column < columns; ++column) {
					const auto corner = static_cast<ImDrawIdx>(base + static_cast<unsigned int>(row * stride + column));
					const auto next = static_cast<ImDrawIdx>(corner + stride);
					a_drawList->PrimWriteIdx(corner);
					a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(corner + 1));
					a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_drawList->PrimWriteIdx(corner);
					a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_drawList->PrimWriteIdx(next);
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Decor Draw ( Spline ) ----------------------
	//---------------------------------------------------

	// Catmull-Rom through the points, closed or not, so a handful of points give a hand-drawn curve
	inline std::vector<ImVec2> Spline(const ImVec2* a_points, int a_count, bool a_closed, int a_steps = 5) {

		std::vector<ImVec2> out;
		out.reserve(static_cast<std::size_t>(a_count * a_steps + 1));
		const auto point = [&](int a_index) {
			return a_closed ? a_points[(a_index + a_count) % a_count] : a_points[std::clamp(a_index, 0, a_count - 1)];
		};
		const int spans = a_closed ? a_count : a_count - 1;
		for (int i = 0; i < spans; ++i) {
			const auto p0 = point(i - 1);
			const auto p1 = point(i);
			const auto p2 = point(i + 1);
			const auto p3 = point(i + 2);
			for (int step = 0; step < a_steps; ++step) {
				const float t = static_cast<float>(step) / static_cast<float>(a_steps);
				const float t2 = t * t;
				const float t3 = t2 * t;
				out.emplace_back(
					0.5f * (2.0f * p1.x + (p2.x - p0.x) * t + (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 + (3.0f * p1.x - p0.x - 3.0f * p2.x + p3.x) * t3),
					0.5f * (2.0f * p1.y + (p2.y - p0.y) * t + (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 + (3.0f * p1.y - p0.y - 3.0f * p2.y + p3.y) * t3));
			}
		}
		if (!a_closed) {
			out.push_back(a_points[a_count - 1]);
		}
		return out;
	}

	// Always wound the same way, or a mirrored shape fills wrongly
	inline void FillShape(ImDrawList* a_drawList, const std::vector<ImVec2>& a_points, ImU32 a_colour) {

		float area = 0.0f;
		for (std::size_t i = 0; i < a_points.size(); ++i) {
			const auto& next = a_points[(i + 1) % a_points.size()];
			area += a_points[i].x * next.y - next.x * a_points[i].y;
		}
		if (area > 0.0f) {
			for (const auto& point : a_points) {
				a_drawList->PathLineTo(point);
			}
		} else {
			for (auto point = a_points.rbegin(); point != a_points.rend(); ++point) {
				a_drawList->PathLineTo(*point);
			}
		}
		a_drawList->PathFillConcave(a_colour);
	}
}
