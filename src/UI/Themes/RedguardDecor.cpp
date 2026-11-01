#include "UI/Themes/RedguardDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::RedguardDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Sandstone, the plaster between, brass and turquoise tile
		constexpr ImU32 sand = IM_COL32(194, 158, 110, 255);
		constexpr ImU32 sandLight = IM_COL32(234, 206, 158, 255);
		constexpr ImU32 sandDark = IM_COL32(118, 90, 58, 255);
		constexpr ImU32 joint = IM_COL32(64, 46, 30, 255);
		constexpr ImU32 brass = IM_COL32(202, 158, 74, 255);
		constexpr ImU32 brassLight = IM_COL32(246, 214, 144, 255);
		constexpr ImU32 turquoise = IM_COL32(58, 164, 158, 255);
		constexpr ImU32 turquoiseLight = IM_COL32(132, 216, 206, 255);
		constexpr ImU32 plaster = IM_COL32(36, 24, 17, 255);
		constexpr ImU32 sun = IM_COL32(255, 196, 120, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Fade At ) ---------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Sword ) -----------------------
		//---------------------------------------------------

		// A curved Yokudan sword, hilt at a_hilt, the blade sweeping up and out; a_facing is -1 or 1
		void Sword(ImDrawList* a_drawList, ImVec2 a_hilt, float a_length, float a_facing, ImU32 a_colour) {

			const ImVec2 p0 = a_hilt;
			const ImVec2 p1(a_hilt.x + a_facing * a_length * 0.15f, a_hilt.y - a_length * 0.6f);
			const ImVec2 p2(a_hilt.x + a_facing * a_length * 0.55f, a_hilt.y - a_length * 0.92f);
			constexpr int steps = 14;
			std::vector<ImVec2> spine;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / steps;
				const float u = 1.0f - t;
				spine.emplace_back(u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x, u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y);
			}
			// The blade widens a little towards the point, then cuts back to it
			const float width = a_length * 0.07f;
			std::vector<ImVec2> outline;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / steps;
				const float wide = t < 0.85f ? width * (0.8f + t * 0.4f) : width * 1.14f * (1.0f - t) / 0.15f;
				const auto  normal = NormalAt(spine, static_cast<std::size_t>(i));
				outline.emplace_back(spine[i].x + normal.x * wide * a_facing, spine[i].y + normal.y * wide * a_facing);
			}
			for (int i = steps; i >= 0; --i) {
				outline.push_back(spine[i]);
			}
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(a_colour);
			// Guard, grip and pommel
			const auto   normal = NormalAt(spine, 0);
			const ImVec2 along(-normal.y, normal.x);
			a_drawList->AddLine(ImVec2(a_hilt.x - normal.x * width * 2.0f, a_hilt.y - normal.y * width * 2.0f), ImVec2(a_hilt.x + normal.x * width * 2.0f, a_hilt.y + normal.y * width * 2.0f), a_colour, std::max(1.5f, width * 0.6f));
			const ImVec2 pommel(a_hilt.x - along.x * a_length * 0.18f, a_hilt.y - along.y * a_length * 0.18f);
			a_drawList->AddLine(a_hilt, pommel, a_colour, std::max(1.5f, width * 0.8f));
			a_drawList->AddCircleFilled(pommel, std::max(1.5f, width * 0.7f), a_colour, 10);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Plaster ) ---------------------
		//---------------------------------------------------

		// Backdrop: sun-baked plaster, mottled and finely cracked, the desert sun beating down from above
		void Plaster(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(60, 40, 26, 70), IM_COL32(60, 40, 26, 70), IM_COL32(18, 12, 8, 70), IM_COL32(18, 12, 8, 70) });

			Random    random{ 0x5E47u };
			const int blotches = std::clamp(static_cast<int>(width * height / (size * size * 6.0f)), 20, 260);
			for (int i = 0; i < blotches; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const bool   light = random.Next() < 0.5f;
				a_drawList->AddCircleFilled(at, size * random.Range(0.8f, 3.0f), WithAlpha(light ? sandDark : IM_COL32(0, 0, 0, 255), light ? 0.035f : 0.05f), 16);
			}

			// Hairline cracks, branching once or twice
			const int cracks = std::clamp(static_cast<int>(width * height / (size * size * 60.0f)), 4, 40);
			for (int i = 0; i < cracks; ++i) {
				ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				float  angle = random.Range(0.0f, pi * 2.0f);
				for (int step = 0; step < 7; ++step) {
					angle += random.Range(-0.7f, 0.7f);
					const ImVec2 next(at.x + std::cos(angle) * size * 0.7f, at.y + std::sin(angle) * size * 0.7f);
					a_drawList->AddLine(at, next, WithAlpha(IM_COL32(0, 0, 0, 255), 0.25f), 1.0f);
					if (random.Next() < 0.2f) {
						const float  fork = angle + random.Range(0.6f, 1.2f) * (random.Next() < 0.5f ? -1.0f : 1.0f);
						a_drawList->AddLine(next, ImVec2(next.x + std::cos(fork) * size * 0.6f, next.y + std::sin(fork) * size * 0.6f), WithAlpha(IM_COL32(0, 0, 0, 255), 0.2f), 1.0f);
					}
					at = next;
				}
			}

			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_min.y - size * 2.0f), std::max(width * 0.45f, size * 10.0f), WithAlpha(sun, 0.012f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(6, 4, 2, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Sandstone Band ) --------------
		//---------------------------------------------------

		// One side of the frame: wind-worn sandstone blocks carved with a zigzag band, brass edging, turquoise tiles set in
		void SandstoneBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, joint);

			const float trim = std::max(1.5f, depth * 0.11f);
			const float inner = depth - trim;
			Random      random{ a_seed };
			float       along = 0.0f;
			while (along < length) {
				const float block = depth * random.Range(2.2f, 3.2f);
				const float end = std::min(length, along + block);
				a_drawList->AddRectFilled(at(along + 1.0f, trim + 0.5f), at(end - 1.0f, inner - 0.5f), Shade(sand, random.Range(0.86f, 1.06f)), depth * 0.12f);
				a_drawList->AddLine(at(along + 2.0f, trim + 1.5f), at(end - 2.0f, trim + 1.5f), WithAlpha(sandLight, 0.6f), 1.0f);
				// Grit worn into the face
				for (int pit = 0; pit < 4; ++pit) {
					a_drawList->AddCircleFilled(at(along + (end - along) * random.Range(0.1f, 0.9f), trim + (inner - trim) * random.Range(0.2f, 0.8f)), random.Range(0.5f, 1.1f), WithAlpha(sandDark, 0.7f), 5);
				}
				along = end;
			}

			// The carved zigzag running the length of it
			const float middle = depth * 0.5f;
			const float rise = (inner - trim) * 0.22f;
			const float step = depth * 0.45f;
			std::vector<ImVec2> zigzag;
			for (float x = 0.0f; x <= length; x += step) {
				zigzag.push_back(at(x, middle + ((static_cast<int>(x / step) % 2 == 0) ? -rise : rise)));
			}
			a_drawList->AddPolyline(zigzag.data(), static_cast<int>(zigzag.size()), WithAlpha(sandDark, 0.9f), ImDrawFlags_None, std::max(1.0f, depth * 0.09f));

			for (const auto& [from, to] : { std::pair{ 0.0f, trim }, std::pair{ inner, depth } }) {
				a_drawList->AddRectFilled(at(0.0f, from), at(length, to), brass);
				a_drawList->AddLine(at(0.0f, from + 0.5f), at(length, from + 0.5f), WithAlpha(brassLight, 0.8f), 1.0f);
			}

			// Turquoise tiles set into the stone now and then
			for (float x = depth * 6.0f; x < length - depth * 5.0f; x += depth * 15.0f) {
				const ImVec2 centre = at(x, middle);
				Tile(a_drawList, centre, depth * 0.36f, turquoise, true);
				Tile(a_drawList, centre, depth * 0.36f, brass, false);
			}
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Corner Block ) ----------------
		//---------------------------------------------------

		// A sandstone block at each corner, a brass-set turquoise star in it
		void CornerBlock(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, brass, a_size * 0.12f);
			a_drawList->AddRectFilled(ImVec2(min.x + 1.5f, min.y + 1.5f), ImVec2(max.x - 1.5f, max.y - 1.5f), sand, a_size * 0.1f);
			a_drawList->AddLine(ImVec2(min.x + 2.0f, min.y + 2.5f), ImVec2(max.x - 2.0f, min.y + 2.5f), WithAlpha(sandLight, 0.8f), 1.0f);
			Star(a_drawList, a_centre, a_size * 0.72f, brass, true);
			Star(a_drawList, a_centre, a_size * 0.5f, turquoise, true);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Arch Outline ) ----------------
		//---------------------------------------------------

		// A pointed arch, as over the doorways of Sentinel and Hammerfell
		std::vector<ImVec2> ArchOutline(ImVec2 a_centre, float a_half, float a_height) {

			const float bottom = a_centre.y + a_height;
			const float spring = a_centre.y + a_height * 0.1f;
			std::vector<ImVec2> outline{ ImVec2(a_centre.x - a_half, bottom), ImVec2(a_centre.x - a_half, spring) };
			constexpr int steps = 10;
			for (int side = 0; side < 2; ++side) {
				for (int i = 1; i <= steps; ++i) {
					const float  t = side == 0 ? static_cast<float>(i) / steps : 1.0f - static_cast<float>(i) / steps;
					const float  sign = side == 0 ? -1.0f : 1.0f;
					const ImVec2 from(a_centre.x + sign * a_half, spring);
					const ImVec2 control(a_centre.x + sign * a_half, a_centre.y - a_height * 0.55f);
					const ImVec2 apex(a_centre.x, a_centre.y - a_height);
					const float  u = 1.0f - t;
					outline.emplace_back(u * u * from.x + 2.0f * u * t * control.x + t * t * apex.x, u * u * from.y + 2.0f * u * t * control.y + t * t * apex.y);
				}
			}
			outline.emplace_back(a_centre.x + a_half, spring);
			outline.emplace_back(a_centre.x + a_half, bottom);
			return outline;
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Crest ) -----------------------
		//---------------------------------------------------

		// A pointed-arch plaque edged in brass, turquoise tile inside, a brass star at its heart
		void Crest(ImDrawList* a_drawList, ImVec2 a_centre, float a_half, float a_height) {

			const auto edge = ArchOutline(a_centre, a_half, a_height);
			const auto stone = ArchOutline(ImVec2(a_centre.x, a_centre.y + 1.0f), a_half - 2.0f, a_height - 2.0f);
			const auto tile = ArchOutline(ImVec2(a_centre.x, a_centre.y + a_height * 0.16f), a_half * 0.62f, a_height * 0.7f);
			for (const auto& [points, colour] : { std::pair{ &edge, brass }, std::pair{ &stone, sand }, std::pair{ &tile, turquoise } }) {
				for (const auto& point : *points) {
					a_drawList->PathLineTo(point);
				}
				a_drawList->PathFillConcave(colour);
			}
			a_drawList->AddPolyline(tile.data(), static_cast<int>(tile.size()), brass, ImDrawFlags_Closed, 1.0f);
			Star(a_drawList, ImVec2(a_centre.x, a_centre.y + a_height * 0.3f), a_half * 0.36f, brassLight, true);
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Tile ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: a glazed tile, lit on its upper faces
	void Tile(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
			return;
		}
		a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(IM_COL32(255, 255, 240, 255), 0.3f * Alpha(a_colour)));
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Star ) ------------------------
	//---------------------------------------------------

	// The eight-pointed star of Yokudan tilework: two squares turned across each other
	void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		ImVec2 squares[2][4];
		for (int square = 0; square < 2; ++square) {
			for (int i = 0; i < 4; ++i) {
				const float angle = static_cast<float>(i) * pi * 0.5f + (square == 0 ? 0.0f : pi * 0.25f);
				squares[square][i] = ImVec2(a_centre.x + std::cos(angle) * a_radius, a_centre.y + std::sin(angle) * a_radius);
			}
		}
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(squares[0], 4, a_colour);
			a_drawList->AddConvexPolyFilled(squares[1], 4, a_colour);
			return;
		}
		// The outline only, round the points
		ImVec2 outline[16];
		for (int i = 0; i < 16; ++i) {
			const float angle = static_cast<float>(i) * pi / 8.0f;
			const float radius = i % 2 == 0 ? a_radius : a_radius * 0.77f;
			outline[i] = ImVec2(a_centre.x + std::cos(angle) * radius, a_centre.y + std::sin(angle) * radius);
		}
		a_drawList->AddPolyline(outline, 16, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.14f));
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Marker ) ----------------------
	//---------------------------------------------------

	// Needed / found: a brass oil lamp, lit when found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  s = a_size;
		const ImVec2 bowl(a_centre.x - s * 0.06f, a_centre.y + s * 0.1f);
		const ImVec2 tip(a_centre.x + s * 0.4f, a_centre.y + s * 0.02f);
		std::vector<ImVec2> outline{ ImVec2(bowl.x - s * 0.26f, bowl.y), tip, ImVec2(tip.x - s * 0.1f, bowl.y + s * 0.05f) };
		constexpr int steps = 10;
		for (int i = 0; i <= steps; ++i) {
			const float angle = static_cast<float>(i) / steps * pi;
			outline.emplace_back(bowl.x + std::cos(angle) * s * 0.26f, bowl.y + std::sin(angle) * s * 0.15f);
		}
		const float stroke = std::max(1.0f, s * 0.07f);
		if (a_fill > 0.0f) {
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(a_colour, a_fill));
			// The flame at the spout
			const ImVec2 flame(tip.x - s * 0.02f, tip.y - s * 0.06f);
			const float  grow = 0.4f + 0.6f * a_fill;
			const ImVec2 points[] = { ImVec2(flame.x, flame.y - s * 0.26f * grow), ImVec2(flame.x + s * 0.07f * grow, flame.y - s * 0.04f), ImVec2(flame.x, flame.y + s * 0.03f), ImVec2(flame.x - s * 0.07f * grow, flame.y - s * 0.04f) };
			a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(a_colour, a_fill));
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, stroke);
		// The lid and the handle
		a_drawList->PathArcTo(ImVec2(bowl.x, bowl.y), s * 0.1f, pi, pi * 2.0f, 8);
		a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
		a_drawList->PathArcTo(ImVec2(bowl.x - s * 0.3f, bowl.y + s * 0.04f), s * 0.07f, pi * 0.5f, pi * 1.5f, 8);
		a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Carving ) ---------------------
	//---------------------------------------------------

	// In place of a rule: a carved zigzag between two fine lines, fading at the ends
	void Carving(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float half = a_size * 0.22f;
		const float step = a_size * 0.3f;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		bool        up = true;
		for (float x = a_from.x; x < a_to.x; x += step, up = !up) {
			const float end = std::min(a_to.x, x + step);
			const float fade = FadeAt((x + end) * 0.5f, a_from.x, length);
			const ImU32 colour = WithAlpha(a_colour, fade);
			a_drawList->AddLine(ImVec2(x, a_from.y - half), ImVec2(end, a_from.y - half), WithAlpha(a_colour, fade * 0.6f), 1.0f);
			a_drawList->AddLine(ImVec2(x, a_from.y + half), ImVec2(end, a_from.y + half), WithAlpha(a_colour, fade * 0.6f), 1.0f);
			a_drawList->AddLine(ImVec2(x, a_from.y + (up ? half : -half) * 0.6f), ImVec2(end, a_from.y + (up ? -half : half) * 0.6f), colour, stroke);
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Carving(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Carving(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Star(a_drawList, ImVec2(midX, a_from.y), gap * 0.45f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// Carved sandstone edged in brass, starred blocks on the corners, a pointed arch over the middle
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Plaster(a_drawList, a_min, a_max);

		SandstoneBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0x5D01u);
		SandstoneBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0x5D02u);
		SandstoneBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0x5D03u);
		SandstoneBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0x5D04u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(brass, 0.25f), 0.0f, 0, 1.0f);

		const float corner = band * 0.95f;
		CornerBlock(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerBlock(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerBlock(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		CornerBlock(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);

		// Only on the main window: the arch rises behind it from the top beam, so it can stand tall without covering the title
		if (compact >= 0.8f) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			Crest(ImGui::GetBackgroundDrawList(), ImVec2(midX, a_min.y - band * 1.1f), band * 2.2f, band * 2.5f);
			CornerBlock(a_drawList, ImVec2(midX, a_max.y - band * 0.5f), corner);
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: cracked plaster at the edges clearing towards the text, sunlight from above, crossed swords in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(sun, 0.05f), WithAlpha(sun, 0.05f), WithAlpha(sun, 0.0f), WithAlpha(sun, 0.0f));

		// Cracks and grit, strongest at the edges
		Random    random{ static_cast<std::uint32_t>(width) * 61u + static_cast<std::uint32_t>(height) };
		const int cracks = std::clamp(static_cast<int>(width * height / (size * size * 6.0f)), 6, 160);
		a_drawList->PushClipRect(a_min, a_max, true);
		for (int i = 0; i < cracks; ++i) {
			ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			float  angle = random.Range(0.0f, pi * 2.0f);
			for (int step = 0; step < 4; ++step) {
				angle += random.Range(-0.8f, 0.8f);
				const ImVec2 next(at.x + std::cos(angle) * size * 0.5f, at.y + std::sin(angle) * size * 0.5f);
				const float  weight = EdgeWeight(ImVec2((at.x + next.x) * 0.5f, (at.y + next.y) * 0.5f), a_min, a_max, band);
				if (weight > 0.03f) {
					a_drawList->AddLine(at, next, WithAlpha(IM_COL32(0, 0, 0, 255), weight * 0.4f), 1.0f);
				}
				at = next;
			}
		}
		const int grains = std::clamp(static_cast<int>(width * height / (size * size * 0.6f)), 40, 1400);
		for (int i = 0; i < grains; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight > 0.02f) {
				a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.3f), WithAlpha(random.Next() < 0.5f ? sandDark : IM_COL32(0, 0, 0, 255), weight * 0.3f), 5);
			}
		}
		a_drawList->PopClipRect();

		// Two curved swords crossed, large and faint, in the lower right
		if (a_emblem) {
			const float  length = std::min(height * 0.85f, width * 0.3f);
			const ImVec2 centre(a_max.x - length * 0.6f, a_max.y - length * 0.12f);
			Sword(a_drawList, ImVec2(centre.x - length * 0.25f, centre.y), length, 1.0f, WithAlpha(brassLight, 0.08f));
			Sword(a_drawList, ImVec2(centre.x + length * 0.25f, centre.y), length, -1.0f, WithAlpha(brassLight, 0.08f));
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// Brass brackets on the panel's corners, a turquoise star on the point
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.17f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, brass, ImDrawFlags_None, thick);
			a_drawList->AddLine(ImVec2(corner.x, corner.y - sy * thick * 0.35f), ImVec2(corner.x + sx * arm, corner.y - sy * thick * 0.35f), WithAlpha(brassLight, 0.7f), 1.0f);
			for (const auto& end : { points[0], points[2] }) {
				Tile(a_drawList, end, thick * 0.8f, brassLight, true);
			}
			const ImVec2 star(corner.x + sx * thick * 1.5f, corner.y + sy * thick * 1.5f);
			Star(a_drawList, star, thick * 1.4f, brass, true);
			Star(a_drawList, star, thick * 0.95f, turquoise, true);
		}
	}
}

namespace CompletionistUI::RedguardDecor
{
	//---------------------------------------------------
	//-- Redguard Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Stars: whole when taken, outlined when next, a bare tile when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Star(a_drawList, a_centre, a_radius * 1.1f, a_colour, true);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.3f, WithAlpha(plaster, 0.8f), 12);
			return;
		case PerkState::kNext:
			Star(a_drawList, a_centre, a_radius * 1.0f, plaster, true);
			Star(a_drawList, a_centre, a_radius * 1.0f, a_colour, false);
			return;
		default:
			Tile(a_drawList, a_centre, a_radius * 0.42f, plaster, true);
			Tile(a_drawList, a_centre, a_radius * 0.42f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// A brass rod inlaid with turquoise where the perks are taken, a fine line where they're not
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, 3.0f);
		if (!a_lit) {
			a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		const float bore = std::max(2.5f, a_radius * 0.26f);
		a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, bore);
		a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), turquoiseLight, ImDrawFlags_None, std::max(1.0f, bore * 0.3f));
	}
}
