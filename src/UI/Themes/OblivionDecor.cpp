#include "UI/Themes/OblivionDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <numbers>
#include <utility>
#include <vector>

namespace CompletionistUI::OblivionDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Old parchment, the pen's ink, worn bronze, the banners' crimson, the dark oak of the bands
		constexpr ImU32 parchLight = IM_COL32(255, 246, 216, 255);
		constexpr ImU32 parchShade = IM_COL32(120, 84, 44, 255);
		constexpr ImU32 parchCard = IM_COL32(236, 220, 184, 255);
		constexpr ImU32 ink = IM_COL32(64, 44, 26, 255);
		constexpr ImU32 gold = IM_COL32(190, 142, 76, 255);
		constexpr ImU32 goldLight = IM_COL32(246, 220, 156, 255);
		constexpr ImU32 goldDeep = IM_COL32(116, 78, 38, 255);
		constexpr ImU32 bronzeDark = IM_COL32(50, 30, 16, 255);
		constexpr ImU32 crimson = IM_COL32(138, 34, 24, 255);
		constexpr ImU32 crimsonLight = IM_COL32(178, 60, 38, 255);
		constexpr ImU32 crimsonDeep = IM_COL32(84, 16, 12, 255);
		constexpr ImU32 oak = IM_COL32(92, 48, 30, 255);
		constexpr ImU32 oakDark = IM_COL32(52, 24, 15, 255);
		constexpr ImU32 ivory = IM_COL32(240, 226, 196, 255);

		// The window's frame, as last drawn, for the bands that fill it
		ImVec2 frameMin;
		ImVec2 frameMax;
		float  frameRail = 0.0f;

		//---------------------------------------------------
		//-- Oblivion Decor ( Mix ) -------------------------
		//---------------------------------------------------

		ImU32 Mix(ImU32 a_from, ImU32 a_to, float a_t) {

			const float t = std::clamp(a_t, 0.0f, 1.0f);
			const auto  channel = [&](int a_shift) {
				const float from = static_cast<float>((a_from >> a_shift) & 0xFF);
				const float to = static_cast<float>((a_to >> a_shift) & 0xFF);
				return static_cast<ImU32>(from + (to - from) * t) << a_shift;
			};
			return channel(IM_COL32_R_SHIFT) | channel(IM_COL32_G_SHIFT) | channel(IM_COL32_B_SHIFT) | channel(IM_COL32_A_SHIFT);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Noise ) -----------------------
		//---------------------------------------------------

		float Hash(int a_x, int a_y) {

			auto h = static_cast<std::uint32_t>(a_x) * 374761393u + static_cast<std::uint32_t>(a_y) * 668265263u;
			h = (h ^ (h >> 13)) * 1274126177u;
			return static_cast<float>((h ^ (h >> 16)) & 0xFFFF) / 65535.0f;
		}

		float ValueNoise(float a_x, float a_y) {

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
		float Fbm(float a_x, float a_y, int a_octaves) {

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

		float Smooth(float a_t) {

			const float t = std::clamp(a_t, 0.0f, 1.0f);
			return t * t * (3.0f - 2.0f * t);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Baked ) -----------------------
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

		void Blit(ImDrawList* a_drawList, const BakedArt& a_art) {

			const auto vertexCount = static_cast<int>(a_art.vertices.size());
			const auto indexCount = static_cast<int>(a_art.indices.size());
			if (vertexCount == 0) {
				return;
			}
			a_drawList->PrimReserve(indexCount, vertexCount);
			const auto base = static_cast<ImDrawIdx>(a_drawList->_VtxCurrentIdx);
			std::memcpy(a_drawList->_VtxWritePtr, a_art.vertices.data(), a_art.vertices.size() * sizeof(ImDrawVert));
			a_drawList->_VtxWritePtr += vertexCount;
			for (const auto index : a_art.indices) {
				*a_drawList->_IdxWritePtr++ = static_cast<ImDrawIdx>(base + index);
			}
			a_drawList->_VtxCurrentIdx += static_cast<unsigned int>(vertexCount);
		}

		template <class Paint>
		void Baked(ImDrawList* a_drawList, std::uint32_t a_id, ImVec2 a_min, ImVec2 a_max, Paint&& a_paint) {

			static std::vector<BakedArt> cache;
			static std::uint32_t         clock = 0;
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

			ImDrawList scratch(ImGui::GetDrawListSharedData());
			scratch._ResetForNewFrame();
			scratch.Flags = a_drawList->Flags & ~ImDrawListFlags_AllowVtxOffset;
			scratch.PushTextureID(a_drawList->_CmdHeader.TextureId);
			scratch.PushClipRectFullScreen();
			a_paint(&scratch);

			// Too big for 16-bit indices in one go: draw it directly, the list splits it as needed
			if (scratch.VtxBuffer.Size >= 60000) {
				a_paint(a_drawList);
				return;
			}

			BakedArt art{ a_id, a_min, a_max, size, uv, clock };
			art.vertices.assign(scratch.VtxBuffer.Data, scratch.VtxBuffer.Data + scratch.VtxBuffer.Size);
			art.indices.assign(scratch.IdxBuffer.Data, scratch.IdxBuffer.Data + scratch.IdxBuffer.Size);
			Blit(a_drawList, art);

			if (cache.size() >= 24) {
				const auto oldest = std::ranges::min_element(cache, {}, &BakedArt::lastUsed);
				*oldest = std::move(art);
			} else {
				cache.push_back(std::move(art));
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Mesh ) ------------------------
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
		//-- Oblivion Decor ( Parchment ) -------------------
		//---------------------------------------------------

		// Old paper: broad stains, foxing, darker towards its edges
		void ParchmentTone(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_edge, std::uint32_t a_seed) {

			const float size = ImGui::GetFontSize();
			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float offset = static_cast<float>(a_seed % 97u);
			const float band = std::min(width, height) * 0.22f + size * 2.0f;
			// No finer than a low UI scale can afford
			const float cell = std::max(size * 0.55f, std::sqrt(width * height / 22000.0f));
			Mesh(a_drawList, a_min, a_max, cell, cell, [&](float a_x, float a_y) {
				const float broad = Fbm(a_x / (size * 14.0f) + offset, a_y / (size * 10.0f), 3);
				const float fine = Fbm(a_x / (size * 1.5f), a_y / (size * 1.5f) + offset, 3);
				const float tone = (broad - 0.5f) * 0.36f + (fine - 0.5f) * 0.55f;
				const float edge = EdgeWeight(ImVec2(a_x, a_y), a_min, a_max, band) * a_edge;
				const float dark = std::max(0.0f, -tone) * 0.42f + edge * 0.34f;
				if (dark > tone * 0.3f) {
					return WithAlpha(parchShade, dark);
				}
				return WithAlpha(parchLight, tone * 0.3f);
			});

		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Parchment Grain ) -------------
		//---------------------------------------------------

		// The tooth of the sheet and its fibres
		void ParchmentGrain(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, std::uint32_t a_seed) {

			const float size = ImGui::GetFontSize();
			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			Random      random{ a_seed * 2654435761u + 7u };
			const int  spots = std::clamp(static_cast<int>(width * height / (size * size * 0.05f)), 80, 9000);
			for (int i = 0; i < spots; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  across = size * random.Range(0.05f, 0.14f);
				const float  down = size * random.Range(0.05f, 0.14f);
				const bool   light = random.Next() < 0.5f;
				a_drawList->AddRectFilled(at, ImVec2(at.x + across, at.y + down), WithAlpha(light ? parchLight : parchShade, random.Range(0.05f, light ? 0.2f : 0.16f)));
			}
			// Fibres, faint
			const int fibres = std::clamp(static_cast<int>(width * height / (size * size * 0.8f)), 30, 1200);
			for (int i = 0; i < fibres; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  angle = random.Range(0.0f, pi);
				const float  length = size * random.Range(0.15f, 0.5f);
				a_drawList->AddLine(at, ImVec2(at.x + std::cos(angle) * length, at.y + std::sin(angle) * length), WithAlpha(random.Next() < 0.5f ? parchShade : parchLight, random.Range(0.04f, 0.1f)), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Oak ) -------------------------
		//---------------------------------------------------

		// Dark oak, its grain drawn out along its length
		ImU32 OakAt(float a_x, float a_y, float a_size) {

			const float grain = Fbm(a_x / (a_size * 5.5f), a_y / (a_size * 0.34f), 4);
			const float knots = Fbm(a_x / (a_size * 2.6f) + 40.0f, a_y / (a_size * 1.4f), 3);
			return Mix(oakDark, oak, std::clamp((grain - 0.32f) * 1.9f + (knots - 0.5f) * 0.6f, 0.0f, 1.2f));
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Rail ) ------------------------
		//---------------------------------------------------

		// One length of worn bronze moulding: dark edges, a rubbed bright ridge, nicks and verdigris shadow
		void Rail(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, std::uint32_t a_seed) {

			const bool  horizontal = a_max.x - a_min.x >= a_max.y - a_min.y;
			const float length = horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) { return horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along); };

			a_drawList->AddRectFilled(a_min, a_max, bronzeDark);
			Random random{ a_seed };
			float  along = 0.0f;
			while (along < length) {
				const float step = depth * random.Range(0.25f, 0.8f);
				const float end = std::min(length, along + step);
				const float wear = Fbm((along + static_cast<float>(a_seed % 500u)) / (depth * 9.0f), 0.3f, 4);
				const ImU32 body = Mix(goldDeep, gold, Smooth((wear - 0.25f) * 2.2f) * random.Range(0.85f, 1.0f));
				a_drawList->AddRectFilled(at(along, depth * 0.12f), at(end, depth * 0.88f), body);
				const ImU32 ridge = Mix(gold, goldLight, Smooth((wear - 0.3f) * 2.4f) * random.Range(0.75f, 1.0f));
				a_drawList->AddRectFilled(at(along, depth * 0.26f), at(end, depth * 0.5f), ridge);
				a_drawList->AddRectFilled(at(along, depth * 0.6f), at(end, depth * 0.74f), WithAlpha(bronzeDark, random.Range(0.3f, 0.55f)));
				// Pits and rubbed flecks in the casting
				if (random.Next() < 0.28f) {
					const float across = random.Range(0.14f, 0.7f);
					a_drawList->AddRectFilled(at(along, depth * across), at(end, depth * (across + 0.14f)), WithAlpha(random.Next() < 0.6f ? bronzeDark : goldLight, random.Range(0.25f, 0.6f)));
				}
				along = end;
			}
			if (horizontal) {
				a_drawList->AddLine(ImVec2(a_min.x, a_min.y + 0.5f), ImVec2(a_max.x, a_min.y + 0.5f), WithAlpha(goldLight, 0.3f), 1.0f);
			} else {
				a_drawList->AddLine(ImVec2(a_min.x + 0.5f, a_min.y), ImVec2(a_min.x + 0.5f, a_max.y), WithAlpha(goldLight, 0.3f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Spline ) ----------------------
		//---------------------------------------------------

		// Catmull-Rom through the points, closed or not, so a handful of points give a hand-drawn curve
		std::vector<ImVec2> Spline(const ImVec2* a_points, int a_count, bool a_closed, int a_steps = 5) {

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
		void FillShape(ImDrawList* a_drawList, const std::vector<ImVec2>& a_points, ImU32 a_colour) {

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

		//---------------------------------------------------
		//-- Oblivion Decor ( Leaf ) ------------------------
		//---------------------------------------------------

		// One bronze acanthus leaf: dark under, bright along its rib
		void Leaf(ImDrawList* a_drawList, ImVec2 a_root, float a_angle, float a_length, float a_width, ImU32 a_body, ImU32 a_rib) {

			const float cosine = std::cos(a_angle);
			const float sine = std::sin(a_angle);
			const auto  at = [&](float a_along, float a_across) {
				return ImVec2(a_root.x + cosine * a_along * a_length - sine * a_across * a_width, a_root.y + sine * a_along * a_length + cosine * a_across * a_width);
			};
			const ImVec2 outline[] = { at(0.0f, 0.0f), at(0.34f, 0.5f), at(0.74f, 0.42f), at(1.0f, 0.0f), at(0.74f, -0.42f), at(0.34f, -0.5f) };
			const auto   shape = Spline(outline, 6, true, 3);
			FillShape(a_drawList, shape, a_body);
			a_drawList->AddLine(at(0.06f, 0.0f), at(0.88f, 0.0f), a_rib, std::max(1.0f, a_width * 0.14f));
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Corner Boss ) -----------------
		//---------------------------------------------------

		// A cast bronze flower on the corner, leaves running out along both rails
		void CornerBoss(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, float a_sx, float a_sy) {

			const auto petal = [&](float a_angle, float a_length, float a_width, float a_shade) {
				Leaf(a_drawList, a_centre, a_angle, a_length * 1.08f, a_width * 1.2f, bronzeDark, bronzeDark);
				Leaf(a_drawList, a_centre, a_angle, a_length, a_width, Shade(gold, a_shade), goldLight);
			};
			const float across = a_sx > 0.0f ? 0.0f : pi;
			const float down = a_sy > 0.0f ? pi * 0.5f : -pi * 0.5f;
			// Leaves along the rails, from the far end in
			for (const float scale : { 2.3f, 1.6f }) {
				petal(across + a_sx * a_sy * 0.13f, a_radius * scale, a_radius * 0.46f, scale > 2.0f ? 0.72f : 0.95f);
				petal(down - a_sx * a_sy * 0.13f, a_radius * scale, a_radius * 0.46f, scale > 2.0f ? 0.72f : 0.95f);
			}
			// The flower, turned to the corner
			const float outward = std::atan2(-a_sy, -a_sx);
			for (const float turn : { -1.15f, 1.15f, -0.58f, 0.58f, 0.0f }) {
				petal(outward + turn, a_radius * (turn == 0.0f ? 0.72f : 0.68f), a_radius * 0.56f, 0.86f + 0.2f * (1.0f - std::abs(turn)));
			}
			for (const float turn : { -2.1f, 2.1f }) {
				petal(outward + turn, a_radius * 0.82f, a_radius * 0.6f, 0.78f);
			}
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.27f, bronzeDark, 16);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.21f, gold, 16);
			a_drawList->AddCircleFilled(ImVec2(a_centre.x - a_radius * 0.04f, a_centre.y - a_radius * 0.05f), a_radius * 0.11f, goldLight, 12);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Sprig ) -----------------------
		//---------------------------------------------------

		// A laurel sprig, as a mapmaker draws beside a title
		void Sprig(ImDrawList* a_drawList, ImVec2 a_root, float a_length, float a_direction, ImU32 a_stem, ImU32 a_leaf, ImU32 a_rib) {

			const auto at = [&](float a_x, float a_y) { return ImVec2(a_root.x + a_x * a_length * a_direction, a_root.y + a_y * a_length); };
			const ImVec2 stem[] = { at(0.0f, 0.06f), at(0.25f, 0.13f), at(0.52f, 0.02f), at(0.78f, -0.02f), at(1.0f, 0.06f) };
			const auto   curve = Spline(stem, 5, false, 6);
			a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_stem, ImDrawFlags_None, std::max(1.0f, a_length * 0.018f));

			const float leafLength = a_length * 0.22f;
			const float leafWidth = a_length * 0.09f;
			const auto  leaf = [&](float a_along, float a_angle, float a_scale) {
				const auto&  point = curve[static_cast<std::size_t>(a_along * static_cast<float>(curve.size() - 1))];
				const float  angle = a_direction > 0.0f ? a_angle : pi - a_angle;
				Leaf(a_drawList, point, angle, leafLength * a_scale, leafWidth * a_scale, a_leaf, a_rib);
			};
			leaf(0.12f, -0.9f, 0.8f);
			leaf(0.22f, 0.8f, 0.7f);
			leaf(0.4f, -1.05f, 1.0f);
			leaf(0.52f, 0.7f, 0.85f);
			leaf(0.68f, -0.95f, 0.9f);
			leaf(0.8f, 0.55f, 0.75f);
			leaf(0.98f, -0.25f, 0.8f);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Etching ) ---------------------
		//---------------------------------------------------

		// A pen and ink drawing: x from a_origin, y up from it, in units. Each stroke's ink fades towards the
		// edges of a_fadeMin to a_fadeMax, so a drawing melts into the paper rather than ending
		struct Etching
		{
			ImDrawList* list;
			ImVec2      origin;
			float       unit;
			float       ink;	// overall strength
			ImVec2      fadeMin;
			ImVec2      fadeMax;
			float       fadeBand;
			Random      random;

			[[nodiscard]] ImVec2 At(float a_x, float a_y) const { return ImVec2(origin.x + a_x * unit, origin.y - a_y * unit); }

			[[nodiscard]] float Fade(ImVec2 a_point) const {

				const float left = Smooth((a_point.x - fadeMin.x) / fadeBand);
				const float right = Smooth((fadeMax.x - a_point.x) / fadeBand);
				const float top = Smooth((a_point.y - fadeMin.y) / (fadeBand * 0.5f));
				return left * right * top * (a_point.y > fadeMax.y ? 0.0f : 1.0f);
			}

			[[nodiscard]] float Pen() const { return std::max(0.7f, unit * 0.045f); }

			void Stroke(float a_x0, float a_y0, float a_x1, float a_y1, float a_alpha, float a_width = 1.0f) {

				const float wobble = unit * 0.03f;
				const ImVec2 from(At(a_x0, a_y0).x + random.Range(-wobble, wobble), At(a_x0, a_y0).y + random.Range(-wobble, wobble));
				const ImVec2 to(At(a_x1, a_y1).x + random.Range(-wobble, wobble), At(a_x1, a_y1).y + random.Range(-wobble, wobble));
				const float alpha = a_alpha * ink * Fade(ImVec2((from.x + to.x) * 0.5f, (from.y + to.y) * 0.5f));
				if (alpha > 0.01f) {
					list->AddLine(from, to, WithAlpha(OblivionDecor::ink, alpha), Pen() * a_width);
				}
			}

			void Wash(std::initializer_list<ImVec2> a_units, float a_alpha) {

				ImVec2 centre(0.0f, 0.0f);
				for (const auto& point : a_units) {
					const auto at = At(point.x, point.y);
					list->PathLineTo(at);
					centre.x += at.x;
					centre.y += at.y;
				}
				centre.x /= static_cast<float>(a_units.size());
				centre.y /= static_cast<float>(a_units.size());
				const float alpha = a_alpha * ink * Fade(centre);
				if (alpha > 0.003f) {
					list->PathFillConcave(WithAlpha(OblivionDecor::ink, alpha));
				} else {
					list->PathClear();
				}
			}

			// Soft shading laid in one direction, as a pen hatches
			void Hatch(float a_x0, float a_y0, float a_x1, float a_y1, float a_gap, float a_slope, float a_alpha) {

				const float gap = std::max(a_gap, 1.8f / unit);
				for (float x = a_x0; x < a_x1; x += gap) {
					if (random.Next() < 0.12f) {
						continue;
					}
					const float y0 = a_y0 + (a_y1 - a_y0) * random.Range(0.0f, 0.08f);
					const float y1 = a_y1 - (a_y1 - a_y0) * random.Range(0.0f, 0.1f);
					Stroke(x, y0, x + a_slope * (y1 - y0), y1, a_alpha * random.Range(0.7f, 1.0f), 0.8f);
				}
			}

			// Teeth along a wall's top
			void Merlons(float a_x0, float a_x1, float a_y, float a_height, float a_alpha) {

				const float tooth = std::max(a_height * 0.8f, 2.4f / unit);
				for (float x = a_x0; x + tooth * 0.6f < a_x1; x += tooth * 1.5f) {
					Wash({ ImVec2(x, a_y), ImVec2(x + tooth * 0.75f, a_y), ImVec2(x + tooth * 0.75f, a_y + a_height), ImVec2(x, a_y + a_height) }, a_alpha * 0.25f);
					Stroke(x, a_y, x, a_y + a_height, a_alpha);
					Stroke(x, a_y + a_height, x + tooth * 0.75f, a_y + a_height, a_alpha);
					Stroke(x + tooth * 0.75f, a_y + a_height, x + tooth * 0.75f, a_y, a_alpha);
				}
			}

			// A length of wall: lit from the left, coursed stone, shadow under its top
			void Wall(float a_x0, float a_y0, float a_x1, float a_y1, float a_alpha, bool a_teeth = true) {

				Wash({ ImVec2(a_x0, a_y0), ImVec2(a_x1, a_y0), ImVec2(a_x1, a_y1), ImVec2(a_x0, a_y1) }, a_alpha * 0.22f);
				Stroke(a_x0, a_y1, a_x1, a_y1, a_alpha);
				Stroke(a_x0, a_y0, a_x0, a_y1, a_alpha * 0.8f);
				Stroke(a_x1, a_y0, a_x1, a_y1, a_alpha);
				Hatch(a_x0 + (a_x1 - a_x0) * 0.5f, a_y0, a_x1, a_y1, 0.14f, 0.0f, a_alpha * 0.35f);
				const float course = std::max(0.26f, 2.4f / unit);
				for (float y = a_y0 + course; y < a_y1 - course * 0.5f; y += course) {
					if (random.Next() < 0.55f) {
						const float x = random.Range(a_x0, a_x1);
						Stroke(x, y, std::min(a_x1, x + random.Range(0.2f, 0.8f)), y, a_alpha * 0.4f, 0.8f);
					}
				}
				Stroke(a_x0, a_y1 - course * 0.35f, a_x1, a_y1 - course * 0.35f, a_alpha * 0.45f, 0.8f);
				if (a_teeth) {
					Merlons(a_x0, a_x1, a_y1, std::max(0.18f, 1.6f / unit), a_alpha);
				}
			}

			// A round tower: lines close together towards its shadowed side, so it reads as a drum
			void Drum(float a_x, float a_radius, float a_y0, float a_y1, float a_alpha, int a_roof) {

				Wash({ ImVec2(a_x - a_radius, a_y0), ImVec2(a_x + a_radius, a_y0), ImVec2(a_x + a_radius, a_y1), ImVec2(a_x - a_radius, a_y1) }, a_alpha * 0.2f);
				Stroke(a_x - a_radius, a_y0, a_x - a_radius, a_y1, a_alpha * 0.8f);
				Stroke(a_x + a_radius, a_y0, a_x + a_radius, a_y1, a_alpha * 1.1f);
				for (float t = 0.05f; t < 0.95f; t += 0.14f) {
					const float x = a_x + a_radius * std::sin((t - 0.08f) * pi * 0.5f);
					if (x > a_x - a_radius * 0.1f) {
						Stroke(x, a_y0, x, a_y1 - (a_y1 - a_y0) * random.Range(0.0f, 0.12f), a_alpha * (0.2f + t * 0.5f), 0.8f);
					}
				}
				const float slit = (a_y1 - a_y0) * 0.14f;
				Stroke(a_x - a_radius * 0.2f, a_y1 - slit * 2.0f, a_x - a_radius * 0.2f, a_y1 - slit, a_alpha, 1.3f);
				switch (a_roof) {
				case 1:	// Corbelled and toothed
					Wash({ ImVec2(a_x - a_radius * 1.2f, a_y1), ImVec2(a_x + a_radius * 1.2f, a_y1), ImVec2(a_x + a_radius * 1.2f, a_y1 + 0.2f), ImVec2(a_x - a_radius * 1.2f, a_y1 + 0.2f) }, a_alpha * 0.3f);
					Stroke(a_x - a_radius * 1.2f, a_y1 + 0.2f, a_x + a_radius * 1.2f, a_y1 + 0.2f, a_alpha);
					Stroke(a_x - a_radius, a_y1, a_x + a_radius, a_y1, a_alpha * 0.7f);
					Merlons(a_x - a_radius * 1.2f, a_x + a_radius * 1.2f, a_y1 + 0.2f, std::max(0.17f, 1.6f / unit), a_alpha);
					break;
				case 2:	// Pointed
					Wash({ ImVec2(a_x - a_radius * 1.15f, a_y1), ImVec2(a_x + a_radius * 1.15f, a_y1), ImVec2(a_x, a_y1 + a_radius * 2.2f) }, a_alpha * 0.45f);
					Stroke(a_x - a_radius * 1.15f, a_y1, a_x, a_y1 + a_radius * 2.2f, a_alpha);
					Stroke(a_x + a_radius * 1.15f, a_y1, a_x, a_y1 + a_radius * 2.2f, a_alpha);
					Stroke(a_x - a_radius * 1.15f, a_y1, a_x + a_radius * 1.15f, a_y1, a_alpha);
					break;
				default:
					break;
				}
			}

			// A row of open arches
			void Arcade(float a_x0, float a_x1, float a_y0, float a_height, int a_count, float a_alpha) {

				const float step = (a_x1 - a_x0) / static_cast<float>(a_count);
				for (int i = 0; i < a_count; ++i) {
					const float left = a_x0 + step * (static_cast<float>(i) + 0.18f);
					const float right = a_x0 + step * (static_cast<float>(i) + 0.82f);
					const float mid = (left + right) * 0.5f;
					const float spring = a_y0 + a_height * 0.62f;
					Wash({ ImVec2(left, a_y0), ImVec2(right, a_y0), ImVec2(right, spring), ImVec2(mid, a_y0 + a_height), ImVec2(left, spring) }, a_alpha * 0.55f);
					Stroke(left, a_y0, left, spring, a_alpha);
					Stroke(right, a_y0, right, spring, a_alpha);
					Stroke(left, spring, mid, a_y0 + a_height, a_alpha);
					Stroke(mid, a_y0 + a_height, right, spring, a_alpha);
				}
			}

			// A ridge of far hills, hatched down its near side
			void Ridge(float a_x0, float a_x1, float a_base, float a_height, float a_alpha, float a_seed) {

				const float step = std::max(0.35f, 3.0f / unit);
				float       lastY = 0.0f;
				bool        started = false;
				for (float x = a_x0; x <= a_x1; x += step) {
					const float t = (x - a_x0) / (a_x1 - a_x0);
					const float swell = std::sin(t * pi);
					const float y = a_base + a_height * swell * (0.35f + Fbm(x * 0.28f + a_seed, a_seed, 3) * 1.1f);
					if (started) {
						Stroke(x - step, lastY, x, y, a_alpha, 0.9f);
						if (random.Next() < 0.7f) {
							const float length = (y - a_base) * random.Range(0.2f, 0.7f);
							Stroke(x, y - 0.02f, x + length * 0.5f, y - length, a_alpha * 0.32f, 0.7f);
						}
						Wash({ ImVec2(x - step, a_base), ImVec2(x, a_base), ImVec2(x, y), ImVec2(x - step, lastY) }, a_alpha * 0.1f);
					}
					lastY = y;
					started = true;
				}
			}

			// Foliage: loops of the pen, thick on the underside
			void Tree(float a_x, float a_y, float a_radius, float a_alpha, bool a_trunk = true) {

				if (a_trunk) {
					Stroke(a_x, a_y, a_x + a_radius * 0.04f, a_y + a_radius * 0.9f, a_alpha, 1.4f);
					Stroke(a_x + a_radius * 0.08f, a_y, a_x + a_radius * 0.12f, a_y + a_radius * 0.7f, a_alpha * 0.8f, 1.0f);
				}
				const float crown = a_y + a_radius * 1.1f;
				const int   clumps = 6 + static_cast<int>(a_radius * 3.0f);
				for (int i = 0; i < clumps; ++i) {
					const float angle = random.Range(0.0f, pi * 2.0f);
					const float reach = a_radius * 0.8f * std::sqrt(random.Next());
					const float cx = a_x + std::cos(angle) * reach;
					const float cy = crown + std::sin(angle) * reach * 0.8f;
					const float blob = a_radius * random.Range(0.3f, 0.48f);
					const float under = Smooth(0.5f - (cy - crown) / (a_radius * 1.6f));
					const auto  at = At(cx, cy);
					list->AddCircleFilled(at, blob * unit, WithAlpha(OblivionDecor::ink, a_alpha * ink * Fade(at) * (0.06f + under * 0.12f)), 10);
				}
				// Leaves as short ticks of the pen, crowded towards the shaded underside
				const int ticks = static_cast<int>(a_radius * a_radius * 60.0f) + 10;
				for (int i = 0; i < ticks; ++i) {
					const float angle = random.Range(0.0f, pi * 2.0f);
					const float reach = a_radius * std::sqrt(random.Next()) * 1.08f;
					const float cx = a_x + std::cos(angle) * reach;
					const float cy = crown + std::sin(angle) * reach * 0.85f;
					const float under = 0.5f - (cy - crown) / (a_radius * 1.7f);
					if (random.Next() > under + 0.35f) {
						continue;
					}
					const float turn = random.Range(0.0f, pi);
					const float length = std::max(0.08f, 1.4f / unit) * random.Range(0.8f, 1.6f);
					Stroke(cx, cy, cx + std::cos(turn) * length, cy + std::sin(turn) * length, a_alpha * (0.45f + under * 0.55f), 0.8f);
				}
			}

			// Grass and scrub along the ground
			void Ground(float a_x0, float a_x1, float a_y, float a_alpha, int a_tufts) {

				for (int i = 0; i < a_tufts; ++i) {
					const float x = random.Range(a_x0, a_x1);
					const float y = a_y + random.Range(-0.1f, 0.25f);
					const float length = random.Range(0.25f, 0.9f);
					Stroke(x, y, x + length, y + random.Range(-0.03f, 0.03f), a_alpha * random.Range(0.3f, 0.8f), 0.8f);
					if (random.Next() < 0.4f) {
						Stroke(x, y, x - 0.05f, y + random.Range(0.1f, 0.3f), a_alpha * 0.7f, 0.8f);
					}
				}
			}
		};

		//---------------------------------------------------
		//-- Oblivion Decor ( White-Gold ) ------------------
		//---------------------------------------------------

		// The Imperial City from the lake: its ring of walls, the arena's arches, White-Gold Tower over all
		void ImperialCity(Etching& a_pen, float a_x) {

			constexpr float strong = 0.9f;
			a_pen.Ridge(a_x - 19.0f, a_x - 5.0f, 0.1f, 2.1f, 0.5f, 3.7f);
			a_pen.Ridge(a_x + 6.0f, a_x + 19.0f, 0.1f, 1.8f, 0.5f, 8.1f);

			// Far side of the ring
			a_pen.Wall(a_x - 8.2f, 1.1f, a_x + 8.2f, 2.5f, strong * 0.6f);
			for (const float x : { -7.4f, -4.9f, 4.7f, 7.3f }) {
				a_pen.Drum(a_x + x, 0.42f, 1.8f, 3.05f, strong * 0.65f, 1);
			}

			// White-Gold Tower: a tapering shaft, its collar, the crown above
			const float towerBase = 2.6f;
			const float towerTop = 6.9f;
			const float baseHalf = 0.68f;
			const float topHalf = 0.33f;
			a_pen.Wash({ ImVec2(a_x - baseHalf, towerBase), ImVec2(a_x + baseHalf, towerBase), ImVec2(a_x + topHalf, towerTop), ImVec2(a_x - topHalf, towerTop) }, 0.24f);
			a_pen.Stroke(a_x - baseHalf, towerBase, a_x - topHalf, towerTop, strong);
			a_pen.Stroke(a_x + baseHalf, towerBase, a_x + topHalf, towerTop, strong * 1.15f, 1.2f);
			for (float t = 0.1f; t < 1.0f; t += 0.12f) {
				const float offset = baseHalf * std::sin((t - 0.08f) * pi * 0.5f);
				const float top = offset * topHalf / baseHalf;
				a_pen.Stroke(a_x + offset, towerBase, a_x + top, towerTop - 0.05f, 0.25f + t * 0.55f, 0.8f);
			}
			// Bands and windows up the shaft
			for (float y = 3.3f; y < 6.4f; y += 0.72f) {
				const float t = (y - towerBase) / (towerTop - towerBase);
				const float half = baseHalf + (topHalf - baseHalf) * t;
				a_pen.Stroke(a_x - half, y, a_x + half, y - 0.02f, 0.65f, 0.9f);
				a_pen.Stroke(a_x - half * 0.3f, y + 0.18f, a_x - half * 0.3f, y + 0.46f, 0.9f, 1.3f);
			}
			// The great collar
			a_pen.Wash({ ImVec2(a_x - 0.34f, 6.9f), ImVec2(a_x + 0.34f, 6.9f), ImVec2(a_x + 0.68f, 7.25f), ImVec2(a_x - 0.68f, 7.25f) }, 0.4f);
			a_pen.Stroke(a_x - 0.34f, 6.9f, a_x - 0.68f, 7.25f, strong);
			a_pen.Stroke(a_x + 0.34f, 6.9f, a_x + 0.68f, 7.25f, strong);
			a_pen.Stroke(a_x - 0.68f, 7.25f, a_x + 0.68f, 7.25f, strong);
			a_pen.Merlons(a_x - 0.68f, a_x + 0.68f, 7.25f, 0.17f, strong);
			// The crown, narrowing to its pinnacle
			a_pen.Wash({ ImVec2(a_x - 0.22f, 7.3f), ImVec2(a_x + 0.22f, 7.3f), ImVec2(a_x + 0.11f, 8.25f), ImVec2(a_x - 0.11f, 8.25f) }, 0.32f);
			a_pen.Stroke(a_x - 0.22f, 7.3f, a_x - 0.11f, 8.25f, strong);
			a_pen.Stroke(a_x + 0.22f, 7.3f, a_x + 0.11f, 8.25f, strong);
			a_pen.Stroke(a_x, 8.25f, a_x, 8.85f, strong);

			// Palace and arena: great drums of arches round its foot
			a_pen.Wall(a_x - 3.6f, 1.2f, a_x + 3.6f, 3.2f, strong * 0.9f);
			a_pen.Arcade(a_x - 3.3f, a_x + 3.3f, 1.65f, 1.05f, 11, 0.7f);
			a_pen.Drum(a_x - 3.2f, 0.5f, 1.3f, 3.75f, strong, 1);
			a_pen.Drum(a_x + 3.2f, 0.5f, 1.3f, 3.75f, strong, 1);
			a_pen.Drum(a_x - 1.3f, 0.3f, 2.7f, 3.8f, strong * 0.9f, 2);
			a_pen.Drum(a_x + 1.3f, 0.3f, 2.7f, 3.8f, strong * 0.9f, 2);

			// The outer wall and its towers, nearest
			a_pen.Wall(a_x - 13.0f, 0.1f, a_x + 13.0f, 1.25f, strong);
			a_pen.Arcade(a_x - 2.2f, a_x + 2.2f, 0.1f, 0.85f, 6, 0.8f);
			for (const float x : { -12.8f, -9.6f, -6.4f, -2.9f, 2.9f, 6.2f, 9.5f, 12.8f }) {
				a_pen.Drum(a_x + x, 0.56f, 0.1f, 2.1f + (std::abs(x) < 7.0f ? 0.35f : 0.0f), strong, x == -9.6f || x == 9.5f ? 2 : 1);
			}
			a_pen.Ground(a_x - 18.0f, a_x + 18.0f, 0.1f, 0.5f, 90);
			for (const float x : { -15.5f, -14.2f, 14.1f, 15.8f, 17.0f }) {
				a_pen.Tree(a_x + x, 0.0f, a_pen.random.Range(0.55f, 0.85f), 0.65f, false);
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Ayleid Ruin ) -----------------
		//---------------------------------------------------

		// One broken pointed arch of Ayleid stone, its shafts grown thin
		void BrokenArch(Etching& a_pen, float a_x, float a_y, float a_half, float a_height, float a_alpha, bool a_broken) {

			const float pier = a_half * 0.42f;
			const float spring = a_y + a_height * 0.56f;
			for (const float side : { -1.0f, 1.0f }) {
				const float outer = a_x + side * a_half;
				const float inner = a_x + side * (a_half - pier);
				a_pen.Wash({ ImVec2(outer, a_y), ImVec2(inner, a_y), ImVec2(inner, spring), ImVec2(outer, spring) }, a_alpha * (side > 0.0f ? 0.6f : 0.35f));
				a_pen.Stroke(outer, a_y, outer, spring, a_alpha);
				a_pen.Stroke(inner, a_y, inner, spring, a_alpha);
				a_pen.Hatch(a_x + side * (a_half - pier) - (side > 0.0f ? 0.0f : pier), a_y, a_x + side * (a_half - pier) + (side > 0.0f ? pier : 0.0f), spring, 0.11f, 0.0f, a_alpha * 0.45f);
				for (float y = a_y + 0.45f; y < spring; y += 0.48f) {
					a_pen.Stroke(outer, y, inner, y + 0.03f, a_alpha * 0.7f, 0.8f);
					const float joint = (outer + inner) * 0.5f + a_pen.random.Range(-pier * 0.2f, pier * 0.2f);
					a_pen.Stroke(joint, y, joint, y + 0.48f, a_alpha * 0.5f, 0.8f);
				}
			}
			// Voussoirs up each side to the point
			const float apex = a_y + a_height;
			constexpr int stones = 7;
			const auto    archAt = [&](float a_side, float a_t, float a_inset) {
				const float t = a_t;
				const float width = (a_half - a_inset) * (1.0f - t * t);
				return ImVec2(a_x + a_side * width, spring + (apex - a_inset * 0.8f - spring) * std::sin(t * pi * 0.5f));
			};
			for (const float side : { -1.0f, 1.0f }) {
				const int count = a_broken && side > 0.0f ? stones - 3 : stones;
				for (int i = 0; i < count; ++i) {
					const float t0 = static_cast<float>(i) / stones;
					const float t1 = static_cast<float>(i + 1) / stones;
					const auto  outer0 = archAt(side, t0, 0.0f);
					const auto  outer1 = archAt(side, t1, 0.0f);
					const auto  inner0 = archAt(side, t0, pier);
					const auto  inner1 = archAt(side, t1, pier);
					a_pen.Wash({ outer0, outer1, inner1, inner0 }, a_alpha * (side > 0.0f ? 0.6f : 0.42f));
					a_pen.Stroke(outer0.x, outer0.y, outer1.x, outer1.y, a_alpha);
					a_pen.Stroke(inner0.x, inner0.y, inner1.x, inner1.y, a_alpha);
					a_pen.Stroke(outer1.x, outer1.y, inner1.x, inner1.y, a_alpha * 0.8f, 0.8f);
				}
			}
		}

		// An Ayleid ruin in the Cyrodiil hills, trees to one side
		void AyleidRuin(Etching& a_pen, float a_x) {

			a_pen.Ridge(a_x - 15.0f, a_x + 11.0f, 0.1f, 2.7f, 0.42f, 2.2f);
			a_pen.Ridge(a_x - 9.0f, a_x + 15.0f, 0.1f, 1.7f, 0.5f, 6.4f);

			BrokenArch(a_pen, a_x, 0.15f, 2.1f, 7.0f, 0.95f, false);
			BrokenArch(a_pen, a_x + 4.2f, 0.15f, 1.5f, 4.6f, 0.85f, true);
			// Fallen stones and stumps of a colonnade
			for (const auto& stump : { ImVec2(-4.0f, 2.0f), ImVec2(-3.0f, 1.0f), ImVec2(6.6f, 1.7f), ImVec2(7.5f, 0.8f) }) {
				a_pen.Wall(a_x + stump.x, 0.15f, a_x + stump.x + 0.5f, 0.15f + stump.y, 0.85f, false);
			}
			a_pen.Tree(a_x - 7.0f, 0.1f, 1.5f, 0.85f);
			a_pen.Tree(a_x - 9.2f, 0.1f, 1.05f, 0.7f);
			a_pen.Tree(a_x + 9.6f, 0.1f, 0.9f, 0.7f);
			a_pen.Ground(a_x - 13.0f, a_x + 12.0f, 0.1f, 0.6f, 70);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Hill Castle ) -----------------
		//---------------------------------------------------

		// A county's castle high on its hill, a tall keep over the curtain wall, woods down the slope
		void HillCastle(Etching& a_pen, float a_x) {

			constexpr float slope = 0.48f;
			const float     hilltop = 3.1f;
			const auto      hillAt = [&](float a_at) { return std::min(hilltop, std::max(0.0f, (a_at + 13.0f) * slope * Smooth((a_at + 13.0f) / 8.0f))); };

			a_pen.Ridge(a_x - 22.0f, a_x - 8.0f, 0.0f, 1.3f, 0.35f, 5.3f);

			// The hill
			const float step = 0.5f;
			for (float x = -13.0f; x < 5.5f; x += step) {
				a_pen.Stroke(a_x + x, hillAt(x), a_x + x + step, hillAt(x + step), 0.55f, 0.8f);
				a_pen.Wash({ ImVec2(a_x + x, 0.0f), ImVec2(a_x + x + step, 0.0f), ImVec2(a_x + x + step, hillAt(x + step)), ImVec2(a_x + x, hillAt(x)) }, 0.07f);
			}

			// Keep and chapel tower behind the wall
			a_pen.Wall(a_x + 0.9f, hilltop + 0.9f, a_x + 2.7f, hilltop + 4.4f, 0.95f);
			a_pen.Stroke(a_x + 1.4f, hilltop + 3.3f, a_x + 1.4f, hilltop + 3.9f, 0.9f, 1.5f);
			a_pen.Stroke(a_x + 2.1f, hilltop + 2.3f, a_x + 2.1f, hilltop + 2.8f, 0.9f, 1.5f);
			a_pen.Drum(a_x + 1.25f, 0.48f, hilltop + 4.4f, hilltop + 5.5f, 0.95f, 2);
			a_pen.Wall(a_x - 3.6f, hilltop + 0.3f, a_x + 0.8f, hilltop + 1.9f, 0.8f);
			a_pen.Wall(a_x + 2.7f, hilltop + 0.3f, a_x + 5.2f, hilltop + 1.5f, 0.8f);

			// Curtain wall and its great bastion
			a_pen.Wall(a_x - 5.6f, hillAt(-5.6f) - 0.3f, a_x - 3.0f, hilltop + 1.0f, 0.9f);
			a_pen.Wall(a_x - 3.0f, hilltop - 0.25f, a_x + 5.0f, hilltop + 0.9f, 0.9f);
			a_pen.Drum(a_x - 5.7f, 0.95f, hillAt(-5.7f) - 0.3f, hilltop + 1.55f, 1.0f, 1);
			a_pen.Drum(a_x + 4.7f, 0.5f, hilltop - 0.3f, hilltop + 1.9f, 0.85f, 1);

			// Woods down the hill, thicker at the foot
			for (const auto& tree : { ImVec2(-11.2f, 0.65f), ImVec2(-9.8f, 0.85f), ImVec2(-8.4f, 1.0f), ImVec2(-7.3f, 0.75f), ImVec2(-10.7f, 0.55f), ImVec2(-3.1f, 0.85f), ImVec2(-1.8f, 1.0f), ImVec2(-0.3f, 0.9f), ImVec2(1.5f, 1.05f), ImVec2(3.3f, 0.85f) }) {
				a_pen.Tree(a_x + tree.x, std::max(0.0f, hillAt(tree.x) - 1.1f), tree.y, 0.7f, false);
			}
			a_pen.Ground(a_x - 15.0f, a_x + 5.5f, 0.1f, 0.5f, 60);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Roadside Ruin ) ---------------
		//---------------------------------------------------

		// The double arch at the foot of the sidebar, tree and scrub about it
		void RoadsideRuin(Etching& a_pen, float a_x) {

			a_pen.Ridge(a_x - 11.0f, a_x + 11.0f, 0.1f, 1.7f, 0.35f, 4.4f);
			BrokenArch(a_pen, a_x - 0.2f, 0.2f, 1.6f, 5.4f, 0.8f, false);
			BrokenArch(a_pen, a_x + 2.9f, 0.2f, 1.5f, 4.6f, 0.75f, true);
			a_pen.Wall(a_x - 2.9f, 0.2f, a_x - 2.4f, 1.8f, 0.7f, false);
			a_pen.Wall(a_x + 5.0f, 0.2f, a_x + 5.5f, 1.2f, 0.7f, false);
			a_pen.Tree(a_x - 6.2f, 0.1f, 1.5f, 0.75f);
			a_pen.Tree(a_x - 3.7f, 0.1f, 0.8f, 0.6f, false);
			a_pen.Tree(a_x + 6.4f, 0.1f, 0.85f, 0.65f, false);
			a_pen.Ground(a_x - 9.0f, a_x + 9.0f, 0.1f, 0.5f, 40);
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Seal ) ------------------------
		//---------------------------------------------------

		// The Oblivion seal: a flat serifed bar, two crescents tapering to points that almost meet, a diamond in the eye they
		// leave. a_unit is the banner's width, which the shape was measured against
		void Seal(ImDrawList* a_drawList, ImVec2 a_centre, float a_unit, ImU32 a_colour, std::uint32_t a_seed) {

			const auto at = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * a_unit, a_centre.y + a_y * a_unit); };

			// One side, top middle round to the eye's apex; each run is smoothed alone so the serif and the horn's tip stay sharp
			const ImVec2 topEdge[] = { ImVec2(0.0f, -0.365f), ImVec2(0.14f, -0.385f), ImVec2(0.28f, -0.41f) };
			const ImVec2 outerEdge[] = { ImVec2(0.235f, -0.35f), ImVec2(0.278f, -0.15f), ImVec2(0.275f, -0.05f), ImVec2(0.24f, 0.1f), ImVec2(0.195f, 0.26f), ImVec2(0.125f, 0.36f), ImVec2(0.062f, 0.424f) };
			const ImVec2 innerEdge[] = { ImVec2(0.062f, 0.424f), ImVec2(0.1f, 0.33f), ImVec2(0.135f, 0.22f), ImVec2(0.16f, 0.07f), ImVec2(0.165f, -0.06f), ImVec2(0.13f, -0.2f), ImVec2(0.075f, -0.29f), ImVec2(0.0f, -0.33f) };

			std::vector<ImVec2> half;
			const auto          add = [&](const auto& a_run) {
				for (const auto& point : Spline(a_run, static_cast<int>(std::size(a_run)), false, 4)) {
					if (half.empty() || half.back().x != point.x || half.back().y != point.y) {
						half.push_back(point);
					}
				}
			};
			add(topEdge);
			add(outerEdge);
			add(innerEdge);

			// Each side its own shape, lapping a little over the middle so no seam shows: a single outline
			// through the two near-touching points doesn't fill reliably
			for (const float side : { -1.0f, 1.0f }) {
				std::vector<ImVec2> shape;
				shape.reserve(half.size() + 2);
				shape.push_back(at(-side * 0.012f, -0.365f));
				for (const auto& point : half) {
					shape.push_back(at(side * point.x, point.y));
				}
				shape.push_back(at(-side * 0.012f, -0.33f));
				FillShape(a_drawList, shape, a_colour);
			}

			const ImVec2 diamond[4] = { at(0.0f, -0.145f), at(0.09f, -0.04f), at(0.0f, 0.065f), at(-0.09f, -0.04f) };
			a_drawList->AddConvexPolyFilled(diamond, 4, a_colour);

			// Worn through in places, as painted cloth goes
			Random random{ a_seed * 7919u + 3u };
			for (int i = 0; i < 16; ++i) {
				const float  side = random.Next() < 0.5f ? -1.0f : 1.0f;
				const float  t = random.Next();
				const ImVec2 speck = at(side * (0.218f - t * 0.02f), -0.28f + t * 0.42f);
				a_drawList->AddCircleFilled(speck, a_unit * random.Range(0.004f, 0.01f), WithAlpha(crimsonDeep, random.Range(0.15f, 0.35f)), 5);
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Banner ) ----------------------
		//---------------------------------------------------

		// A worn brick-red banner hanging from a rolled sleeve between two acorn finials, the seal painted on it
		void WarBanner(ImDrawList* a_drawList, float a_midX, float a_top, float a_width, float a_height, std::uint32_t a_seed) {

			constexpr ImU32 brick = IM_COL32(144, 54, 34, 255);
			constexpr ImU32 brickLight = IM_COL32(166, 72, 44, 255);
			constexpr ImU32 brickDark = IM_COL32(98, 30, 20, 255);
			constexpr ImU32 thread = IM_COL32(232, 186, 138, 255);
			constexpr ImU32 sealCream = IM_COL32(216, 202, 176, 255);

			const float half = a_width * 0.5f;
			const float sleeve = a_width * 0.19f;
			const float clothTop = a_top + sleeve * 0.85f;
			const float tip = a_top + a_height;
			const float shoulder = a_top + a_height * 0.625f;

			// The tail curves in, a shield's foot rather than a straight chevron
			const auto bottomAt = [&](float a_x, float a_halfWidth, float a_tip) {
				const float u = std::clamp(1.0f - std::abs(a_x - a_midX) / a_halfWidth, 0.0f, 1.0f);
				return shoulder + (a_tip - shoulder) * std::pow(u, 0.68f);
			};
			const auto outlinePath = [&](float a_inset, float a_top0) {
				const float side = half - a_inset;
				const float point = tip - a_inset * 1.5f;
				std::vector<ImVec2> path;
				path.emplace_back(a_midX - side, a_top0);
				constexpr int steps = 14;
				for (int step = 0; step <= steps; ++step) {
					const float x = a_midX - side + side * static_cast<float>(step) / steps;
					path.emplace_back(x, bottomAt(x, side, point));
				}
				for (int step = steps - 1; step >= 0; --step) {
					const float x = a_midX + side - side * static_cast<float>(step) / steps;
					path.emplace_back(x, bottomAt(x, side, point));
				}
				path.emplace_back(a_midX + side, a_top0);
				return path;
			};

			// Its shadow, soft, on the paper
			auto shadow = outlinePath(0.0f, clothTop);
			for (const auto& [offset, alpha] : { std::pair{ a_width * 0.1f, 0.1f }, std::pair{ a_width * 0.06f, 0.14f }, std::pair{ a_width * 0.03f, 0.18f } }) {
				std::vector<ImVec2> moved;
				moved.reserve(shadow.size());
				for (const auto& point : shadow) {
					moved.emplace_back(point.x + offset, point.y + offset * 0.7f);
				}
				FillShape(a_drawList, moved, WithAlpha(IM_COL32(30, 14, 6, 255), alpha));
			}

			// The cloth: mottled and faded, darker at the hems, a pull either side from where it's sewn to the sleeve
			const float seed = static_cast<float>(a_seed);
			const auto  clothAt = [&](float a_x, float a_y) {
				const float mottle = Fbm(a_x / (a_width * 0.2f) + seed, a_y / (a_width * 0.2f), 4);
				const float fine = Fbm(a_x / (a_width * 0.05f), a_y / (a_width * 0.05f) + seed, 2);
				const float u = (a_x - a_midX) / half;
				const float v = (a_y - clothTop) / (tip - clothTop);
				const float pullAt = 0.03f + (1.0f - std::abs(u)) * 0.2f;
				const float pull = std::exp(-(v - pullAt) * (v - pullAt) / 0.0012f) * Smooth((std::abs(u) - 0.08f) * 4.0f);
				const float hem = std::pow(std::abs(u), 4.0f) * 0.5f + Smooth((v - 0.8f) * 5.0f) * 0.12f;
				const float tone = (mottle - 0.5f) * 1.7f + (fine - 0.5f) * 0.5f + pull * 0.7f - hem;
				return tone >= 0.0f ? Mix(brick, brickLight, tone * 1.6f) : Mix(brick, brickDark, -tone * 1.5f);
			};
			constexpr int strips = 24;
			constexpr int rows = 18;
			const float   step = a_width / strips;
			for (int strip = 0; strip < strips; ++strip) {
				const float left = a_midX - half + step * static_cast<float>(strip);
				const float right = left + step;
				const float leftEnd = bottomAt(left, half, tip);
				const float rightEnd = bottomAt(right, half, tip);
				for (int row = 0; row < rows; ++row) {
					const float t0 = static_cast<float>(row) / rows;
					const float t1 = static_cast<float>(row + 1) / rows;
					const ImVec2 points[4] = { ImVec2(left, clothTop + (leftEnd - clothTop) * t0), ImVec2(right, clothTop + (rightEnd - clothTop) * t0), ImVec2(right, clothTop + (rightEnd - clothTop) * t1), ImVec2(left, clothTop + (leftEnd - clothTop) * t1) };
					GradientQuad(a_drawList, points, { clothAt(points[0].x, points[0].y), clothAt(points[1].x, points[1].y), clothAt(points[2].x, points[2].y), clothAt(points[3].x, points[3].y) });
				}
			}

			// Hem: a dark edge, then two pale lines of stitching running round it
			const auto drawLine = [&](const std::vector<ImVec2>& a_path, ImU32 a_colour, float a_thickness) {
				a_drawList->AddPolyline(a_path.data(), static_cast<int>(a_path.size()), a_colour, ImDrawFlags_None, a_thickness);
			};
			drawLine(outlinePath(0.0f, clothTop), WithAlpha(bronzeDark, 0.75f), std::max(1.0f, a_width * 0.018f));
			drawLine(outlinePath(a_width * 0.022f, clothTop), WithAlpha(thread, 0.72f), std::max(1.0f, a_width * 0.014f));
			drawLine(outlinePath(a_width * 0.075f, clothTop + a_width * 0.02f), WithAlpha(thread, 0.85f), std::max(1.0f, a_width * 0.016f));

			Seal(a_drawList, ImVec2(a_midX, a_top + a_height * 0.477f), a_width, sealCream, a_seed);

			// The sleeve the cloth is rolled over, lit along its top
			const float  reach = half + a_width * 0.03f;
			const ImVec2 sleeveMin(a_midX - reach, a_top);
			const ImVec2 sleeveMax(a_midX + reach, a_top + sleeve);
			const ImU32  sleeveTop = Mix(brickLight, goldLight, 0.42f);
			const ImU32  sleeveMid = Mix(brickLight, gold, 0.3f);
			const ImU32  sleeveBottom = Mix(brick, bronzeDark, 0.4f);
			const float  sleeveMidY = a_top + sleeve * 0.4f;
			a_drawList->AddRectFilledMultiColor(sleeveMin, ImVec2(sleeveMax.x, sleeveMidY), sleeveTop, sleeveTop, sleeveMid, sleeveMid);
			a_drawList->AddRectFilledMultiColor(ImVec2(sleeveMin.x, sleeveMidY), sleeveMax, sleeveMid, sleeveMid, sleeveBottom, sleeveBottom);
			a_drawList->AddRect(sleeveMin, sleeveMax, WithAlpha(bronzeDark, 0.8f), a_width * 0.02f);
			a_drawList->AddLine(ImVec2(sleeveMin.x + 1.0f, a_top + sleeve * 0.22f), ImVec2(sleeveMax.x - 1.0f, a_top + sleeve * 0.22f), WithAlpha(goldLight, 0.4f), std::max(1.0f, a_width * 0.012f));

			// A bronze collar at each end of the sleeve and an acorn finial past it
			const float axisY = a_top + sleeve * 0.5f;
			for (const float side : { -1.0f, 1.0f }) {
				const float base = a_midX + side * reach;
				const auto  at = [&](float a_along, float a_across) { return ImVec2(base + side * a_along * a_width, axisY + a_across * a_width); };

				a_drawList->AddRectFilled(ImVec2(std::min(at(-0.03f, 0.0f).x, at(0.015f, 0.0f).x), a_top - a_width * 0.01f), ImVec2(std::max(at(-0.03f, 0.0f).x, at(0.015f, 0.0f).x), a_top + sleeve + a_width * 0.01f), bronzeDark);
				a_drawList->AddRectFilled(ImVec2(std::min(at(-0.022f, 0.0f).x, at(0.007f, 0.0f).x), a_top), ImVec2(std::max(at(-0.022f, 0.0f).x, at(0.007f, 0.0f).x), a_top + sleeve), gold);

				a_drawList->AddRectFilled(ImVec2(std::min(at(0.015f, 0.0f).x, at(0.075f, 0.0f).x), axisY - a_width * 0.035f), ImVec2(std::max(at(0.015f, 0.0f).x, at(0.075f, 0.0f).x), axisY + a_width * 0.035f), goldDeep);

				// Round at the neck, drawn out to a point
				const ImVec2 acorn[] = { at(0.065f, 0.0f), at(0.085f, -0.06f), at(0.145f, -0.082f), at(0.2f, -0.055f), at(0.275f, 0.0f), at(0.2f, 0.055f), at(0.145f, 0.082f), at(0.085f, 0.06f) };
				const auto   shape = Spline(acorn, 8, true, 3);
				std::vector<ImVec2> edge;
				edge.reserve(shape.size());
				const ImVec2 centre = at(0.16f, 0.0f);
				for (const auto& point : shape) {
					edge.emplace_back(centre.x + (point.x - centre.x) * 1.14f, centre.y + (point.y - centre.y) * 1.14f);
				}
				FillShape(a_drawList, edge, bronzeDark);
				FillShape(a_drawList, shape, gold);
				a_drawList->AddCircleFilled(at(0.125f, -0.026f), a_width * 0.026f, goldLight, 8);
				a_drawList->AddLine(at(0.1f, 0.042f), at(0.21f, 0.026f), WithAlpha(goldDeep, 0.8f), std::max(1.0f, a_width * 0.012f));
			}
		}

		//---------------------------------------------------
		//-- Oblivion Decor ( Dark Corner ) -----------------
		//---------------------------------------------------

		// The oak showing through the worn page behind the buttons, melting into the parchment
		void WornOak(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_left) {

			const float size = ImGui::GetFontSize();
			const float width = a_max.x - a_min.x;
			Mesh(a_drawList, a_min, a_max, size * 0.7f, size * 0.32f, [&](float a_x, float a_y) {
				const float along = a_left ? (a_x - a_min.x) / width : (a_max.x - a_x) / width;
				const float edge = Fbm(a_x / (size * 3.0f), a_y / (size * 3.5f) + 11.0f, 3);
				const float keep = Smooth((1.0f - along * 1.15f + (edge - 0.5f) * 0.5f) * 2.1f);
				return WithAlpha(OakAt(a_x, a_y, size), keep);
			});
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Stud ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: a cut gold lozenge, or its outline in ink
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.18f));
			return;
		}
		const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(goldLight, 0.55f * alpha));
		a_drawList->AddTriangleFilled(points[1], points[2], a_centre, WithAlpha(bronzeDark, 0.4f * alpha));
		a_drawList->AddPolyline(points, 4, WithAlpha(bronzeDark, 0.7f * alpha), ImDrawFlags_Closed, 1.0f);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Marker ) ----------------------
	//---------------------------------------------------

	// Needed / found: a sun ring in ink, a crimson wax seal in its gold ring once found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.34f;
		const float stroke = std::max(1.0f, a_size * 0.06f);
		if (a_fill < 1.0f) {
			const ImU32 line = WithAlpha(a_colour, 1.0f - a_fill * 0.7f);
			a_drawList->AddCircle(a_centre, radius * 0.72f, line, 16, stroke);
			a_drawList->AddCircle(a_centre, radius * 0.3f, line, 10, stroke * 0.8f);
			for (int ray = 0; ray < 8; ++ray) {
				const float angle = static_cast<float>(ray) * pi * 0.25f;
				const float cosine = std::cos(angle);
				const float sine = std::sin(angle);
				a_drawList->AddLine(ImVec2(a_centre.x + cosine * radius * 0.78f, a_centre.y + sine * radius * 0.78f), ImVec2(a_centre.x + cosine * radius * 1.12f, a_centre.y + sine * radius * 1.12f), line, stroke);
			}
		}
		if (a_fill > 0.0f) {
			a_drawList->AddCircleFilled(a_centre, radius * 1.02f, WithAlpha(goldDeep, a_fill), 18);
			a_drawList->AddCircleFilled(a_centre, radius * 0.82f, WithAlpha(a_colour, a_fill), 18);
			a_drawList->AddCircleFilled(ImVec2(a_centre.x - radius * 0.2f, a_centre.y - radius * 0.22f), radius * 0.28f, WithAlpha(goldLight, a_fill * 0.5f), 10);
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Rosette ) ---------------------
	//---------------------------------------------------

	// The small crimson boss set in a panel's top edge
	void Rosette(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

		const auto lozenge = [&](float a_scale, ImU32 a_fill) {
			const float r = a_radius * a_scale;
			const ImVec2 points[4] = { ImVec2(a_centre.x, a_centre.y - r), ImVec2(a_centre.x + r, a_centre.y), ImVec2(a_centre.x, a_centre.y + r), ImVec2(a_centre.x - r, a_centre.y) };
			a_drawList->AddConvexPolyFilled(points, 4, a_fill);
		};
		lozenge(1.0f, bronzeDark);
		lozenge(0.9f, gold);
		lozenge(0.72f, crimsonDeep);
		lozenge(0.62f, crimson);
		for (int petal = 0; petal < 4; ++petal) {
			const float angle = static_cast<float>(petal) * pi * 0.5f + pi * 0.25f;
			a_drawList->AddCircleFilled(ImVec2(a_centre.x + std::cos(angle) * a_radius * 0.18f, a_centre.y + std::sin(angle) * a_radius * 0.18f), a_radius * 0.14f, goldLight, 6);
		}
		a_drawList->AddCircleFilled(a_centre, a_radius * 0.08f, crimsonDeep, 6);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Rule ) ------------------------
	//---------------------------------------------------

	// A pen line with a small lozenge at its end, thinning as it goes
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize();
		const float direction = a_to.x >= a_from.x ? 1.0f : -1.0f;
		if ((a_to.x - a_from.x) * direction < size * 1.2f) {
			return;
		}
		const float radius = size * 0.2f;
		const ImVec2 end(a_to.x - direction * radius, a_to.y);
		a_drawList->AddLine(a_from, ImVec2(end.x - direction * radius, end.y), a_colour, 1.0f);
		Stud(a_drawList, end, radius, a_colour, false);
		a_drawList->AddCircleFilled(end, radius * 0.25f, a_colour, 6);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		// A plain ruled line: the panels under it carry the ornament
		const float midX = (a_from.x + a_to.x) * 0.5f;
		const ImU32 clear = WithAlpha(a_colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_from.x, a_from.y), ImVec2(midX, a_from.y + 1.0f), clear, a_colour, a_colour, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(midX, a_from.y), ImVec2(a_to.x, a_from.y + 1.0f), a_colour, clear, clear, a_colour);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// A bronze rail round an old sheet of parchment, a cast flower at each corner
	void Frame(ImDrawList* a_drawList, ImVec2 a_inMin, ImVec2 a_inMax) {

		const float size = ImGui::GetFontSize();
		// Out to the window's edge, so no strip of bare paper shows outside the rail
		const float  bleed = size * 0.4f;
		const ImVec2 a_min(a_inMin.x - bleed, a_inMin.y - bleed);
		const ImVec2 a_max(a_inMax.x + bleed, a_inMax.y + bleed);
		const float  width = a_max.x - a_min.x;
		const float  height = a_max.y - a_min.y;
		const float  compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float  rail = size * 0.56f * std::max(0.65f, compact);

		frameMin = a_min;
		frameMax = a_max;
		frameRail = rail;

		Baked(a_drawList, 0x0B11u, a_min, a_max, [&](ImDrawList* a_list) { ParchmentTone(a_list, a_min, a_max, 0.95f, 0xB1u); });
		Baked(a_drawList, 0x0B12u, a_min, a_max, [&](ImDrawList* a_list) { ParchmentGrain(a_list, a_min, a_max, 0xB1u); });
		Baked(a_drawList, 0x0B10u, a_min, a_max, [&](ImDrawList* a_list) {
			// The moulding's shadow on the page
			const float shade = size * 0.45f;
			const ImU32 dark = WithAlpha(parchShade, 0.5f);
			const ImU32 clear = WithAlpha(parchShade, 0.0f);
			a_list->AddRectFilledMultiColor(ImVec2(a_min.x + rail, a_min.y + rail), ImVec2(a_max.x - rail, a_min.y + rail + shade), dark, dark, clear, clear);
			a_list->AddRectFilledMultiColor(ImVec2(a_min.x + rail, a_max.y - rail - shade), ImVec2(a_max.x - rail, a_max.y - rail), clear, clear, dark, dark);
			a_list->AddRectFilledMultiColor(ImVec2(a_min.x + rail, a_min.y + rail), ImVec2(a_min.x + rail + shade, a_max.y - rail), dark, clear, clear, dark);
			a_list->AddRectFilledMultiColor(ImVec2(a_max.x - rail - shade, a_min.y + rail), ImVec2(a_max.x - rail, a_max.y - rail), clear, dark, dark, clear);

			Rail(a_list, a_min, ImVec2(a_max.x, a_min.y + rail), 0xB201u);
			Rail(a_list, ImVec2(a_min.x, a_max.y - rail), a_max, 0xB202u);
			Rail(a_list, ImVec2(a_min.x, a_min.y + rail), ImVec2(a_min.x + rail, a_max.y - rail), 0xB203u);
			Rail(a_list, ImVec2(a_max.x - rail, a_min.y + rail), ImVec2(a_max.x, a_max.y - rail), 0xB204u);

			const float boss = size * 1.2f * std::clamp(std::min(width, height) / (size * 46.0f), 0.5f, 1.0f);
			const float in = boss * 0.62f;
			CornerBoss(a_list, ImVec2(a_min.x + in, a_min.y + in), boss, 1.0f, 1.0f);
			CornerBoss(a_list, ImVec2(a_max.x - in, a_min.y + in), boss, -1.0f, 1.0f);
			CornerBoss(a_list, ImVec2(a_max.x - in, a_max.y - in), boss, -1.0f, -1.0f);
			CornerBoss(a_list, ImVec2(a_min.x + in, a_max.y - in), boss, 1.0f, -1.0f);
		});
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Header Band ) -----------------
	//---------------------------------------------------

	// The head of the sheet: toned darker, oak at its corners, the Imperial City and an Ayleid ruin drawn either side,
	// two banners hung from the rail, laurel by the title
	void HeaderBand(ImDrawList* a_drawList, float a_bottom, ImVec2 a_titleMin, ImVec2 a_titleMax) {

		const float size = ImGui::GetFontSize();
		const ImVec2 min(frameMin.x + frameRail, frameMin.y + frameRail);
		const ImVec2 max(frameMax.x - frameRail, a_bottom);
		const float  width = max.x - min.x;
		const float  height = max.y - min.y;
		if (width < size * 20.0f || height < size * 2.0f) {
			return;
		}

		const float midX = (min.x + max.x) * 0.5f;
		const float titleHalf = (a_titleMax.x - a_titleMin.x) * 0.5f;
		// Long and narrow, as the banners hang in the old Imperial halls
		const float bannerHeight = height + frameRail * 0.65f + size * 0.75f;
		const float bannerWidth = bannerHeight / 2.1f;
		const float reach = titleHalf + size * 5.6f + bannerWidth * 0.85f;
		const float bannerOffset = std::clamp(width * 0.165f, reach, std::max(reach, width * 0.36f));
		const bool  banners = bannerOffset + bannerWidth < width * 0.5f - size * 7.0f;

		Baked(a_drawList, 0x0B20u, min, max, [&](ImDrawList* a_list) {
			// Toned a shade darker than the page, deepest at its top
			const ImU32 top = WithAlpha(parchShade, 0.42f);
			const ImU32 bottom = WithAlpha(parchShade, 0.18f);
			a_list->AddRectFilledMultiColor(min, max, top, top, bottom, bottom);
			Random random{ 0xB301u };
			const int spots = static_cast<int>(width * height / (size * size * 0.4f));
			for (int i = 0; i < std::min(spots, 700); ++i) {
				const ImVec2 at(min.x + width * random.Next(), min.y + height * random.Next());
				a_list->AddCircleFilled(at, size * random.Range(0.08f, 0.3f), WithAlpha(random.Next() < 0.5f ? parchShade : parchLight, random.Range(0.02f, 0.08f)), 6);
			}

			// The etchings, between the buttons and the banners
			const float unit = height / 8.8f;
			const float leftEnd = banners ? midX - bannerOffset - bannerWidth * 0.85f : midX - titleHalf - size * 5.0f;
			const float rightStart = banners ? midX + bannerOffset + bannerWidth * 0.85f : midX + titleHalf + size * 5.0f;
			Etching ruin{ a_list, ImVec2(0.0f, max.y - size * 0.16f), unit, 0.72f, ImVec2(min.x + size * 3.5f, min.y), ImVec2(leftEnd, max.y), size * 3.5f, Random{ 0xB302u } };
			ruin.origin.x = min.x + (leftEnd - min.x) * 0.66f;
			AyleidRuin(ruin, 0.0f);
			Etching city{ a_list, ImVec2(0.0f, max.y - size * 0.16f), unit, 0.8f, ImVec2(rightStart, min.y), ImVec2(max.x - size * 3.5f, max.y), size * 3.5f, Random{ 0xB303u } };
			city.origin.x = rightStart + (max.x - rightStart) * 0.46f;
			ImperialCity(city, 0.0f);

			// Oak showing through round the buttons
			const float wornWidth = std::min(width * 0.26f, size * 15.0f);
			WornOak(a_list, min, ImVec2(min.x + wornWidth, max.y), true);
			WornOak(a_list, ImVec2(max.x - wornWidth, min.y), max, false);

			// Its foot: a thin rail across the sheet
			const float foot = std::max(3.0f, size * 0.22f);
			Rail(a_list, ImVec2(min.x, max.y - foot), max, 0xB304u);
			const ImU32 dark = WithAlpha(parchShade, 0.45f);
			const ImU32 clear = WithAlpha(parchShade, 0.0f);
			a_list->AddRectFilledMultiColor(ImVec2(min.x, max.y), ImVec2(max.x, max.y + size * 0.4f), dark, dark, clear, clear);

			// The frame's flowers again, over the oak
			const float boss = size * 1.2f;
			const float in = boss * 0.62f;
			CornerBoss(a_list, ImVec2(frameMin.x + in, frameMin.y + in), boss, 1.0f, 1.0f);
			CornerBoss(a_list, ImVec2(frameMax.x - in, frameMin.y + in), boss, -1.0f, 1.0f);

			if (banners) {
				WarBanner(a_list, midX - bannerOffset, frameMin.y + frameRail * 0.35f, bannerWidth, bannerHeight, 11u);
				WarBanner(a_list, midX + bannerOffset, frameMin.y + frameRail * 0.35f, bannerWidth, bannerHeight, 29u);
			}

			// Laurel either side of the title
			const float sprig = size * 4.8f;
			const float sprigY = (a_titleMin.y + a_titleMax.y) * 0.5f;
			const ImU32 stem = goldDeep;
			const ImU32 leaf = Mix(goldDeep, gold, 0.55f);
			Sprig(a_list, ImVec2(a_titleMin.x - size * 0.7f, sprigY), sprig, -1.0f, stem, leaf, WithAlpha(bronzeDark, 0.6f));
			Sprig(a_list, ImVec2(a_titleMax.x + size * 0.7f, sprigY), sprig, 1.0f, stem, leaf, WithAlpha(bronzeDark, 0.6f));
		});
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Footer Band ) -----------------
	//---------------------------------------------------

	// The sheet's foot: a strip of dark oak under a thin rail, gold laurel at each end
	void FooterBand(ImDrawList* a_drawList, float a_top) {

		const float size = ImGui::GetFontSize();
		const ImVec2 min(frameMin.x + frameRail, a_top);
		const ImVec2 max(frameMax.x - frameRail, frameMax.y - frameRail);
		if (max.y <= min.y) {
			return;
		}
		Baked(a_drawList, 0x0B30u, min, max, [&](ImDrawList* a_list) {
			Mesh(a_list, min, max, size * 0.7f, size * 0.22f, [&](float a_x, float a_y) { return OakAt(a_x, a_y, size); });
			const float foot = std::max(3.0f, size * 0.22f);
			Rail(a_list, ImVec2(min.x, min.y - foot), ImVec2(max.x, min.y), 0xB401u);
			const ImU32 dark = WithAlpha(IM_COL32(16, 6, 2, 255), 0.55f);
			const ImU32 clear = WithAlpha(IM_COL32(16, 6, 2, 255), 0.0f);
			a_list->AddRectFilledMultiColor(min, ImVec2(max.x, min.y + size * 0.5f), dark, dark, clear, clear);
			a_list->AddRectFilledMultiColor(ImVec2(min.x, max.y - size * 0.4f), max, clear, clear, dark, dark);

			const float midY = (min.y + max.y) * 0.5f;
			const float sprig = std::min((max.x - min.x) * 0.1f, size * 5.5f);
			Sprig(a_list, ImVec2(min.x + size * 2.0f, midY), sprig, 1.0f, gold, Mix(goldDeep, gold, 0.8f), WithAlpha(goldLight, 0.8f));
			Sprig(a_list, ImVec2(max.x - size * 2.0f, midY), sprig, -1.0f, gold, Mix(goldDeep, gold, 0.8f), WithAlpha(goldLight, 0.8f));

			const float boss = size * 1.2f;
			const float in = boss * 0.62f;
			CornerBoss(a_list, ImVec2(frameMax.x - in, frameMax.y - in), boss, -1.0f, -1.0f);
			CornerBoss(a_list, ImVec2(frameMin.x + in, frameMax.y - in), boss, 1.0f, -1.0f);
		});
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Sidebar Etching ) -------------
	//---------------------------------------------------

	void SidebarEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		if (width < size * 8.0f || a_max.y - a_min.y < size * 8.0f) {
			return;
		}
		Baked(a_drawList, 0x0B40u, a_min, a_max, [&](ImDrawList* a_list) {
			const float unit = width / 22.0f;
			Etching pen{ a_list, ImVec2(a_min.x + width * 0.52f, a_max.y), unit, 0.26f, ImVec2(a_min.x, a_max.y - unit * 12.0f), a_max, size * 1.4f, Random{ 0xB501u } };
			RoadsideRuin(pen, 0.0f);
		});
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: a sheet laid on the page, its edges browned. A detail panel has the castle and far hills drawn on it.
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		Baked(a_drawList, a_emblem ? 0x0B51u : 0x0B50u, a_min, a_max, [&](ImDrawList* a_list) {
			const float band = std::min(width, height) * 0.25f + size * 1.5f;
			Mesh(a_list, a_min, a_max, size * 1.2f, size * 1.2f, [&](float a_x, float a_y) {
				const float stain = Fbm(a_x / (size * 7.0f) + 50.0f, a_y / (size * 5.0f), 3);
				const float edge = EdgeWeight(ImVec2(a_x, a_y), a_min, a_max, band);
				return WithAlpha(parchShade, 0.03f + edge * 0.24f + std::max(0.0f, stain - 0.52f) * 0.14f);
			});
			Random     random{ static_cast<std::uint32_t>(width) * 41u + static_cast<std::uint32_t>(height) };
			const int  spots = std::clamp(static_cast<int>(width * height / (size * size * 0.6f)), 20, 900);
			for (int i = 0; i < spots; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				a_list->AddCircleFilled(at, size * random.Range(0.06f, 0.24f), WithAlpha(random.Next() < 0.5f ? parchShade : parchLight, random.Range(0.02f, 0.08f)), 6);
			}
		});
		if (a_emblem) {
			PanelEtching(a_drawList, a_min, a_max, 1.0f);
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Panel Etching ) ---------------
	//---------------------------------------------------

	void PanelEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_baseline) {

		const float width = a_max.x - a_min.x;
		const float size = ImGui::GetFontSize();
		const float ground = a_min.y + (a_max.y - a_min.y) * a_baseline - size * 0.25f;
		const float room = ground - a_min.y;
		if (room < size * 4.0f || width < size * 22.0f) {
			return;
		}
		Baked(a_drawList, a_baseline < 1.0f ? 0x0B53u : 0x0B52u, a_min, a_max, [&](ImDrawList* a_list) {
			const float unit = std::min(room / 9.4f, size * 0.62f);
			Etching castle{ a_list, ImVec2(a_max.x - unit * 7.0f, ground), unit, 0.42f, ImVec2(a_max.x - unit * 28.0f, a_min.y), ImVec2(a_max.x - size * 0.3f, ground), size * 3.0f, Random{ 0xB601u } };
			HillCastle(castle, 0.0f);
			if (a_baseline < 1.0f && width > size * 48.0f) {
				Etching hills{ a_list, ImVec2(a_min.x + unit * 11.0f, ground), unit * 0.82f, 0.22f, a_min, ImVec2(a_min.x + unit * 24.0f, ground), size * 3.0f, Random{ 0xB602u } };
				AyleidRuin(hills, 0.0f);
			}
		});
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// Bronze spandrels with a scrolled tip, as the corners of an illuminated page
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.5f;
		const float thick = std::max(2.0f, size * 0.12f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const auto   at = [&](float a_x, float a_y) { return ImVec2(corner.x + sx * a_x * arm, corner.y + sy * a_y * arm); };

			const ImVec2 webbing[] = { at(0.0f, 0.0f), at(0.82f, 0.0f), at(0.5f, 0.1f), at(0.26f, 0.26f), at(0.1f, 0.5f), at(0.0f, 0.82f) };
			FillShape(a_drawList, Spline(webbing, 6, true, 3), WithAlpha(goldDeep, 0.42f));

			const ImVec2 edge[3] = { at(1.0f, 0.0f), at(0.0f, 0.0f), at(0.0f, 1.0f) };
			a_drawList->AddPolyline(edge, 3, bronzeDark, ImDrawFlags_None, thick + 1.0f);
			a_drawList->AddPolyline(edge, 3, gold, ImDrawFlags_None, thick);
			a_drawList->AddLine(at(0.02f, -0.015f), at(1.0f, -0.015f), WithAlpha(goldLight, 0.8f), 1.0f);

			const ImVec2 scrollA[] = { at(0.92f, 0.04f), at(0.56f, 0.16f), at(0.33f, 0.34f), at(0.16f, 0.56f), at(0.04f, 0.92f) };
			const auto   curve = Spline(scrollA, 5, false, 4);
			a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), WithAlpha(goldDeep, 0.95f), ImDrawFlags_None, std::max(1.0f, thick * 0.55f));
			a_drawList->AddCircle(at(0.3f, 0.3f), arm * 0.1f, goldDeep, 10, std::max(1.0f, thick * 0.45f));
			a_drawList->AddCircleFilled(at(0.3f, 0.3f), arm * 0.045f, gold, 6);
			for (const auto& tip : { at(1.0f, 0.0f), at(0.0f, 1.0f) }) {
				a_drawList->AddCircleFilled(tip, thick * 0.85f, goldDeep, 8);
			}
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Plaque ) ----------------------
	//---------------------------------------------------

	// A raised card of paler parchment, its shadow on the sheet
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed) {

		const float size = ImGui::GetFontSize();
		const float round = size * 0.14f;
		const float drop = a_pressed ? 0.0f : size * 0.09f;
		a_drawList->AddRectFilled(ImVec2(a_min.x + drop * 0.4f, a_min.y + drop), ImVec2(a_max.x + drop, a_max.y + drop * 1.6f), WithAlpha(IM_COL32(40, 22, 8, 255), 0.4f), round);
		const ImU32 top = Mix(parchCard, goldLight, a_glow * 0.5f);
		const ImU32 bottom = Mix(Shade(parchCard, 0.84f), gold, a_glow * 0.25f);
		a_drawList->AddRectFilledMultiColor(a_min, a_max, a_pressed ? bottom : top, a_pressed ? bottom : top, a_pressed ? top : bottom, a_pressed ? top : bottom);
		a_drawList->AddRect(a_min, a_max, Mix(goldDeep, crimson, a_glow * 0.5f), round, 0, std::max(1.0f, size * 0.07f));
		a_drawList->AddRect(ImVec2(a_min.x + 2.0f, a_min.y + 2.0f), ImVec2(a_max.x - 2.0f, a_max.y - 2.0f), WithAlpha(goldDeep, 0.3f), round * 0.5f);
		a_drawList->AddLine(ImVec2(a_min.x + 3.0f, a_min.y + 1.5f), ImVec2(a_max.x - 3.0f, a_min.y + 1.5f), WithAlpha(parchLight, 0.75f), 1.0f);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Nav Plaque ) ------------------
	//---------------------------------------------------

	// The chosen sidebar row: a pointed strip of banner crimson, bound in gold
	void NavPlaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float height = a_max.y - a_min.y;
		const float point = height * 0.22f;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const ImVec2 outline[6] = { ImVec2(a_min.x, midY), ImVec2(a_min.x + point, a_min.y), ImVec2(a_max.x - point, a_min.y), ImVec2(a_max.x, midY), ImVec2(a_max.x - point, a_max.y), ImVec2(a_min.x + point, a_max.y) };
		const ImU32 upper = WithAlpha(crimsonLight, a_alpha);
		const ImU32 middle = WithAlpha(crimson, a_alpha);
		const ImU32 lower = WithAlpha(crimsonDeep, a_alpha);
		const ImVec2 topHalf[4] = { outline[0], outline[1], outline[2], outline[3] };
		const ImVec2 bottomHalf[4] = { outline[0], outline[3], outline[4], outline[5] };
		GradientQuad(a_drawList, topHalf, { middle, upper, upper, middle });
		GradientQuad(a_drawList, bottomHalf, { middle, middle, lower, lower });
		a_drawList->AddPolyline(outline, 6, WithAlpha(gold, a_alpha), ImDrawFlags_Closed, std::max(1.0f, ImGui::GetFontSize() * 0.07f));
		a_drawList->AddLine(ImVec2(a_min.x + point, a_min.y + 1.5f), ImVec2(a_max.x - point, a_min.y + 1.5f), WithAlpha(goldLight, 0.35f * a_alpha), 1.0f);
		Stud(a_drawList, ImVec2(a_min.x + point * 0.75f, midY), height * 0.1f, WithAlpha(goldLight, a_alpha), true);
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Highlight ) -------------------
	//---------------------------------------------------

	// A chosen row: one stroke of an amber brush, dry at its end
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		constexpr ImU32 amber = IM_COL32(226, 168, 92, 255);
		const float height = a_max.y - a_min.y;
		const float width = a_max.x - a_min.x;
		Random      random{ static_cast<std::uint32_t>(a_min.y) * 31u + 77u };
		constexpr int bristles = 7;
		for (int bristle = 0; bristle < bristles; ++bristle) {
			const float top = a_min.y + height * static_cast<float>(bristle) / bristles;
			const float bottom = a_min.y + height * static_cast<float>(bristle + 1) / bristles;
			const float across = std::sin((static_cast<float>(bristle) + 0.5f) / bristles * pi);
			const float length = width * (0.72f + 0.26f * across) * random.Range(0.92f, 1.0f);
			const float core = a_alpha * (0.18f + 0.3f * across);
			const ImU32 strong = WithAlpha(amber, core);
			const ImU32 clear = WithAlpha(amber, 0.0f);
			const float start = a_min.x + width * 0.02f * random.Next();
			a_drawList->AddRectFilledMultiColor(ImVec2(start, top), ImVec2(start + length * 0.55f, bottom), strong, strong, strong, strong);
			a_drawList->AddRectFilledMultiColor(ImVec2(start + length * 0.55f, top), ImVec2(start + length, bottom), strong, clear, clear, strong);
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Pad Cap ) ---------------------
	//---------------------------------------------------

	// The ivory key under a controller hint
	void PadCap(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_rounding) {

		const float drop = std::max(1.0f, ImGui::GetFontSize() * 0.07f);
		a_drawList->AddRectFilled(ImVec2(a_min.x, a_min.y + drop), ImVec2(a_max.x, a_max.y + drop), WithAlpha(IM_COL32(16, 6, 2, 255), 0.7f), a_rounding);
		a_drawList->AddRectFilled(a_min, a_max, ivory, a_rounding);
	}
}

namespace CompletionistUI::OblivionDecor
{
	//---------------------------------------------------
	//-- Oblivion Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Wax seals on the page: pressed in crimson when taken, a ruled ring when next, a pen dot when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			a_drawList->AddCircleFilled(a_centre, a_radius, goldDeep, 20);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.84f, a_colour, 20);
			a_drawList->AddCircle(a_centre, a_radius * 0.58f, WithAlpha(goldLight, 0.6f), 16, std::max(1.0f, a_radius * 0.08f));
			Stud(a_drawList, a_centre, a_radius * 0.3f, goldLight, true);
			return;
		case PerkState::kNext:
			// Over the lines, so they end at its rim
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.85f, parchCard, 20);
			a_drawList->AddCircle(a_centre, a_radius * 0.85f, a_colour, 20, std::max(1.2f, a_radius * 0.13f));
			a_drawList->AddCircle(a_centre, a_radius * 0.55f, WithAlpha(a_colour, 0.6f), 14, 1.0f);
			return;
		default:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.45f, parchCard, 12);
			a_drawList->AddCircle(a_centre, a_radius * 0.45f, a_colour, 12, 1.0f);
			return;
		}
	}

	//---------------------------------------------------
	//-- Oblivion Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// Routes across a map: a firm line where they're taken, a dashed one where they aren't
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto points = PerkCurve(a_from, a_to, std::max(2.0f, a_radius * 0.35f));
		if (a_lit) {
			a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), a_colour, ImDrawFlags_None, std::max(1.5f, a_radius * 0.2f));
			return;
		}
		for (std::size_t i = 0; i + 1 < points.size(); i += 3) {
			a_drawList->AddLine(points[i], points[std::min(i + 1, points.size() - 1)], a_colour, 1.2f);
		}
	}
}
