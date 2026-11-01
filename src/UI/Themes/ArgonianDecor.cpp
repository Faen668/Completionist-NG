#include "UI/Themes/ArgonianDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::ArgonianDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Scaled hide, Xanmeer stone, jade glyphs, black water and the Hist's amber
		constexpr ImU32 hide = IM_COL32(48, 92, 90, 255);
		constexpr ImU32 hideLight = IM_COL32(112, 168, 158, 255);
		constexpr ImU32 hideDark = IM_COL32(16, 36, 38, 255);
		constexpr ImU32 stone = IM_COL32(86, 104, 96, 255);
		constexpr ImU32 stoneLight = IM_COL32(140, 158, 146, 255);
		constexpr ImU32 stoneDark = IM_COL32(34, 46, 44, 255);
		constexpr ImU32 jade = IM_COL32(96, 196, 170, 255);
		constexpr ImU32 water = IM_COL32(150, 210, 214, 255);
		constexpr ImU32 amber = IM_COL32(238, 168, 52, 255);
		constexpr ImU32 amberLight = IM_COL32(255, 222, 140, 255);
		constexpr ImU32 amberDeep = IM_COL32(150, 74, 14, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Fade At ) ---------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Drop Outline ) ----------------
		//---------------------------------------------------

		// A drop of sap: round below, drawn to a point above
		std::vector<ImVec2> DropOutline(ImVec2 a_centre, float a_radius) {

			std::vector<ImVec2> outline;
			const ImVec2 belly(a_centre.x, a_centre.y + a_radius * 0.35f);
			const ImVec2 tip(a_centre.x, a_centre.y - a_radius * 1.25f);
			outline.push_back(tip);
			constexpr int steps = 14;
			for (int i = 0; i <= steps; ++i) {
				const float angle = -pi * 0.15f + static_cast<float>(i) / steps * pi * 1.3f;
				outline.emplace_back(belly.x + std::cos(angle) * a_radius, belly.y + std::sin(angle) * a_radius);
			}
			return outline;
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Rain ) ------------------------
		//---------------------------------------------------

		// Backdrop: black marsh water under rain, rings spreading where the drops fall
		void Rain(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(14, 34, 38, 70), IM_COL32(14, 34, 38, 70), IM_COL32(4, 12, 14, 70), IM_COL32(4, 12, 14, 70) });

			Random random{ 0xA59Au };
			// Rings on the water, flattened by the angle, two or three to each drop
			const int rings = std::clamp(static_cast<int>(width * height / (size * size * 18.0f)), 8, 90);
			for (int i = 0; i < rings; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  radius = size * random.Range(0.8f, 2.4f);
				const int    count = 2 + static_cast<int>(random.Next() * 2.0f);
				for (int ring = 0; ring < count; ++ring) {
					const float r = radius * (1.0f - 0.32f * static_cast<float>(ring));
					a_drawList->AddEllipse(at, ImVec2(r, r * 0.32f), WithAlpha(water, 0.05f + 0.02f * static_cast<float>(ring)), 0.0f, 24, 1.0f);
				}
			}
			// Rain, slanting
			const int drops = std::clamp(static_cast<int>(width * height / (size * size * 2.5f)), 30, 500);
			for (int i = 0; i < drops; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  length = size * random.Range(0.5f, 1.1f);
				a_drawList->AddLine(at, ImVec2(at.x - length * 0.25f, at.y + length), WithAlpha(water, random.Range(0.04f, 0.09f)), 1.0f);
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(2, 6, 8, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Scale Band ) ------------------
		//---------------------------------------------------

		// One side of the frame: rows of overlapping scales, as on a Saxhleel's back, each lit along its rim
		void ScaleBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, hideDark);
			a_drawList->PushClipRect(a_min, a_max, true);

			Random      random{ a_seed };
			const float scale = depth * 0.55f;
			// Three rows, the outer first so the inner ones lie over them
			for (int row = 0; row < 3; ++row) {
				const float across = depth * (0.18f + 0.32f * static_cast<float>(row));
				const float offset = row % 2 == 0 ? 0.0f : scale * 0.5f;
				for (float along = -scale + offset; along < length + scale; along += scale) {
					const ImVec2 centre = at(along, across);
					const ImU32  face = Shade(hide, random.Range(0.8f, 1.15f));
					a_drawList->AddCircleFilled(centre, scale * 0.62f, face, 12);
					// The rim, lit on the side facing in
					const float start = a_horizontal ? 0.15f * pi : -0.35f * pi;
					a_drawList->PathArcTo(centre, scale * 0.58f, start, start + pi * 0.7f, 8);
					a_drawList->PathStroke(WithAlpha(hideLight, 0.75f), ImDrawFlags_None, 1.0f);
				}
			}
			a_drawList->PopClipRect();
			a_drawList->AddRect(a_min, a_max, WithAlpha(hideDark, 0.9f), 0.0f, 0, 1.0f);
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Xanmeer ) ---------------------
		//---------------------------------------------------

		// A Xanmeer: the stepped stone pyramid of Black Marsh, glyphs cut along each tier, a dark door at its foot
		void Xanmeer(ImDrawList* a_drawList, ImVec2 a_base, float a_half, float a_height, float a_alpha) {

			constexpr int tiers = 4;
			const float   rise = a_height / tiers;
			for (int tier = 0; tier < tiers; ++tier) {
				const float  half = a_half * (1.0f - 0.2f * static_cast<float>(tier));
				const ImVec2 min(a_base.x - half, a_base.y - rise * static_cast<float>(tier + 1));
				const ImVec2 max(a_base.x + half, a_base.y - rise * static_cast<float>(tier));
				a_drawList->AddRectFilled(min, max, WithAlpha(Shade(stone, 1.0f - 0.05f * static_cast<float>(tier)), a_alpha));
				a_drawList->AddLine(ImVec2(min.x, min.y + 0.5f), ImVec2(max.x, min.y + 0.5f), WithAlpha(stoneLight, a_alpha), 1.0f);
				a_drawList->AddLine(ImVec2(min.x, max.y - 0.5f), ImVec2(max.x, max.y - 0.5f), WithAlpha(stoneDark, a_alpha), 1.0f);
				// A band of glyphs: short strokes in jade, a step in every third
				const float middle = (min.y + max.y) * 0.5f;
				for (float x = min.x + rise * 0.6f; x < max.x - rise * 0.6f; x += rise * 0.7f) {
					a_drawList->AddLine(ImVec2(x, middle - rise * 0.18f), ImVec2(x, middle + rise * 0.18f), WithAlpha(jade, 0.6f * a_alpha), 1.0f);
					a_drawList->AddLine(ImVec2(x, middle - rise * 0.18f), ImVec2(x + rise * 0.25f, middle - rise * 0.18f), WithAlpha(jade, 0.6f * a_alpha), 1.0f);
				}
			}
			// The door, and a bowl of Hist sap burning on the summit
			const float door = rise * 0.7f;
			a_drawList->AddRectFilled(ImVec2(a_base.x - door * 0.5f, a_base.y - rise * 0.95f), ImVec2(a_base.x + door * 0.5f, a_base.y), WithAlpha(stoneDark, a_alpha));
			const ImVec2 summit(a_base.x, a_base.y - a_height);
			a_drawList->AddRectFilled(ImVec2(summit.x - rise * 0.5f, summit.y - rise * 0.25f), ImVec2(summit.x + rise * 0.5f, summit.y), WithAlpha(stoneDark, a_alpha));
			SoftSpot(a_drawList, ImVec2(summit.x, summit.y - rise * 0.4f), rise * 0.9f, WithAlpha(amber, 0.08f * a_alpha));
			a_drawList->AddCircleFilled(ImVec2(summit.x, summit.y - rise * 0.35f), rise * 0.22f, WithAlpha(amber, a_alpha), 10);
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Stair Corner ) ----------------
		//---------------------------------------------------

		// A corner cut in steps, as a Xanmeer's profile; a_sx and a_sy point into the corner's panel
		void StairCorner(ImDrawList* a_drawList, ImVec2 a_corner, float a_arm, float a_sx, float a_sy, ImU32 a_fill, ImU32 a_edge) {

			constexpr int steps = 3;
			const float   step = a_arm / steps;
			std::vector<ImVec2> points{ a_corner, ImVec2(a_corner.x + a_sx * a_arm, a_corner.y) };
			for (int k = 0; k < steps; ++k) {
				const float x = a_arm - step * static_cast<float>(k);
				const float y = step * static_cast<float>(k + 1);
				points.emplace_back(a_corner.x + a_sx * x, a_corner.y + a_sy * y);
				points.emplace_back(a_corner.x + a_sx * (x - step), a_corner.y + a_sy * y);
			}
			for (const auto& point : points) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(a_fill);
			std::vector<ImVec2> edge(points.begin() + 1, points.end());
			a_drawList->AddPolyline(edge.data(), static_cast<int>(edge.size()), a_edge, ImDrawFlags_None, 1.0f);
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Scale ) -----------------------
	//---------------------------------------------------

	// In place of the diamond: one scale, round-backed and pointed, lit along its rim
	void Scale(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		std::vector<ImVec2> outline;
		constexpr int steps = 10;
		for (int i = 0; i <= steps; ++i) {
			const float angle = pi + static_cast<float>(i) / steps * pi;
			outline.emplace_back(a_centre.x + std::cos(angle) * a_radius * 0.85f, a_centre.y + std::sin(angle) * a_radius * 0.75f);
		}
		outline.emplace_back(a_centre.x, a_centre.y + a_radius * 1.05f);
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(outline.data(), static_cast<int>(outline.size()), a_colour);
			a_drawList->PathArcTo(a_centre, a_radius * 0.55f, pi * 1.15f, pi * 1.6f, 6);
			a_drawList->PathStroke(WithAlpha(IM_COL32(255, 255, 255, 255), 0.4f * Alpha(a_colour)), ImDrawFlags_None, 1.0f);
			return;
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Drop ) ------------------------
	//---------------------------------------------------

	// Needed / found: a drop of Hist sap, filling with amber as it's found
	void Drop(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.24f;
		const auto  outline = DropOutline(ImVec2(a_centre.x, a_centre.y + a_size * 0.04f), radius);
		if (a_fill > 0.0f) {
			a_drawList->AddConvexPolyFilled(outline.data(), static_cast<int>(outline.size()), WithAlpha(a_colour, a_fill));
			a_drawList->AddCircleFilled(ImVec2(a_centre.x - radius * 0.35f, a_centre.y + radius * 0.2f), radius * 0.22f, WithAlpha(IM_COL32(255, 248, 220, 255), 0.7f * a_fill), 8);
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.07f));
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Eye ) -------------------------
	//---------------------------------------------------

	// A Saxhleel's eye: amber, darker at the rim, split by a slit pupil, set in a ring of stone
	void Eye(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

		a_drawList->AddCircleFilled(a_centre, a_radius, stoneDark, 20);
		a_drawList->AddCircle(a_centre, a_radius, stone, 20, std::max(1.0f, a_radius * 0.14f));
		const float iris = a_radius * 0.78f;
		a_drawList->AddCircleFilled(a_centre, iris, amberDeep, 20);
		a_drawList->AddCircleFilled(a_centre, iris * 0.82f, amber, 20);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x, a_centre.y - iris * 0.2f), iris * 0.45f, WithAlpha(amberLight, 0.6f), 14);
		a_drawList->AddEllipseFilled(a_centre, ImVec2(iris * 0.16f, iris * 0.86f), IM_COL32(8, 6, 4, 255), 0.0f, 14);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x + iris * 0.32f, a_centre.y - iris * 0.36f), iris * 0.12f, WithAlpha(IM_COL32(255, 255, 255, 255), 0.75f), 8);
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Glyph ) -----------------------
	//---------------------------------------------------

	// A Xanmeer in miniature: three steps cut in jade
	void Glyph(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const float rise = a_radius * 0.55f;
		for (int tier = 0; tier < 3; ++tier) {
			const float half = a_radius * (1.0f - 0.32f * static_cast<float>(tier));
			const float bottom = a_centre.y + a_radius * 0.8f - rise * static_cast<float>(tier);
			a_drawList->AddRectFilled(ImVec2(a_centre.x - half, bottom - rise * 0.78f), ImVec2(a_centre.x + half, bottom), a_colour);
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Scales ) ----------------------
	//---------------------------------------------------

	// In place of a rule: a row of scales hanging from a line, fading at the ends
	void Scales(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float radius = a_size * 0.2f;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		const int   count = static_cast<int>(length / (radius * 2.0f));
		const float start = a_from.x + (length - static_cast<float>(count) * radius * 2.0f) * 0.5f;
		for (int i = 0; i < count; ++i) {
			const float  x = start + radius * (2.0f * static_cast<float>(i) + 1.0f);
			const float  fade = FadeAt(x, a_from.x, length);
			const ImVec2 centre(x, a_from.y - radius * 0.5f);
			a_drawList->PathArcTo(centre, radius, 0.0f, pi, 8);
			a_drawList->PathStroke(WithAlpha(a_colour, fade), ImDrawFlags_None, stroke);
			a_drawList->AddLine(ImVec2(x - radius, a_from.y - radius * 0.5f), ImVec2(x + radius, a_from.y - radius * 0.5f), WithAlpha(a_colour, fade * 0.6f), 1.0f);
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Scales(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Scales(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Eye(a_drawList, ImVec2(midX, a_from.y), gap * 0.42f);
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// Scaled hide all round, an amber eye watching from each corner, a Xanmeer rising over the top
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Rain(a_drawList, a_min, a_max);

		// Behind the window, so only its upper tiers show above it
		if (compact >= 0.8f) {
			Xanmeer(ImGui::GetBackgroundDrawList(), ImVec2((a_min.x + a_max.x) * 0.5f, a_min.y + band), std::min(width * 0.12f, size * 8.0f), band * 4.4f, 1.0f);
		}

		ScaleBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0xA201u);
		ScaleBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0xA202u);
		ScaleBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0xA203u);
		ScaleBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0xA204u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(jade, 0.22f), 0.0f, 0, 1.0f);

		const float eye = band * 0.85f;
		for (const auto& corner : { ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f) }) {
			Eye(a_drawList, corner, eye);
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: ripples and rain at the edges clearing towards the text, a Xanmeer in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.35f, size * 6.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(jade, 0.0f), WithAlpha(jade, 0.0f), WithAlpha(jade, 0.05f), WithAlpha(jade, 0.05f));

		Random random{ static_cast<std::uint32_t>(width) * 43u + static_cast<std::uint32_t>(height) };
		a_drawList->PushClipRect(a_min, a_max, true);
		const int rings = std::clamp(static_cast<int>(width * height / (size * size * 10.0f)), 4, 60);
		for (int i = 0; i < rings; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.05f) {
				continue;
			}
			const float radius = size * random.Range(0.6f, 1.6f);
			for (int ring = 0; ring < 2; ++ring) {
				const float r = radius * (1.0f - 0.4f * static_cast<float>(ring));
				a_drawList->AddEllipse(at, ImVec2(r, r * 0.32f), WithAlpha(water, weight * 0.12f), 0.0f, 20, 1.0f);
			}
		}
		const int drops = std::clamp(static_cast<int>(width * height / (size * size * 1.2f)), 20, 600);
		for (int i = 0; i < drops; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight > 0.05f) {
				const float length = size * random.Range(0.4f, 0.8f);
				a_drawList->AddLine(at, ImVec2(at.x - length * 0.25f, at.y + length), WithAlpha(water, weight * 0.12f), 1.0f);
			}
		}
		a_drawList->PopClipRect();

		// A Xanmeer, large and faint, in the lower right
		if (a_emblem) {
			const float half = std::min(height * 0.42f, width * 0.15f);
			Xanmeer(a_drawList, ImVec2(a_max.x - half * 1.3f, a_max.y - size * 0.4f), half, half * 1.1f, 0.12f);
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// Each corner cut in steps like a Xanmeer, a jade glyph on the outer step
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx, outer.y + sy);
			StairCorner(a_drawList, corner, arm, sx, sy, WithAlpha(stone, 0.85f), stoneLight);
			a_drawList->AddRectFilled(ImVec2(corner.x + sx * arm * 0.12f - 1.5f, corner.y + sy * arm * 0.12f - 1.5f), ImVec2(corner.x + sx * arm * 0.12f + 1.5f, corner.y + sy * arm * 0.12f + 1.5f), jade);
			a_drawList->AddCircleFilled(ImVec2(corner.x + sx * arm * 0.3f, corner.y + sy * arm * 0.3f), std::max(1.5f, size * 0.1f), amber, 8);
		}
	}
}

namespace CompletionistUI::ArgonianDecor
{
	//---------------------------------------------------
	//-- Argonian Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Drops of sap: amber when taken, an empty drop when next, a bare scale when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.06f));
			Drop(a_drawList, a_centre, a_radius * 3.0f, a_colour, 1.0f);
			return;
		case PerkState::kNext:
			{
				const auto outline = DropOutline(ImVec2(a_centre.x, a_centre.y + a_radius * 0.12f), a_radius * 0.72f);
				a_drawList->AddConvexPolyFilled(outline.data(), static_cast<int>(outline.size()), hideDark);
				Drop(a_drawList, a_centre, a_radius * 3.0f, a_colour, 0.0f);
			}
			return;
		default:
			Scale(a_drawList, a_centre, a_radius * 0.45f, hideDark, true);
			Scale(a_drawList, a_centre, a_radius * 0.45f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// A stream winding between the perks, its two banks drawn where they're taken
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, 2.0f);
		// The stream meanders a little either side of the plain curve
		std::vector<ImVec2> stream;
		float               travelled = 0.0f;
		const float         wander = std::max(1.5f, a_radius * 0.3f);
		for (std::size_t i = 0; i < curve.size(); ++i) {
			if (i > 0) {
				travelled += std::hypot(curve[i].x - curve[i - 1].x, curve[i].y - curve[i - 1].y);
			}
			const float t = static_cast<float>(i) / static_cast<float>(curve.size() - 1);
			const auto  normal = NormalAt(curve, i);
			const float offset = std::sin(travelled / (a_radius * 1.4f)) * wander * std::sin(t * pi);
			stream.emplace_back(curve[i].x + normal.x * offset, curve[i].y + normal.y * offset);
		}
		if (!a_lit) {
			a_drawList->AddPolyline(stream.data(), static_cast<int>(stream.size()), a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		const float wide = std::max(1.5f, a_radius * 0.18f);
		for (const float side : { -1.0f, 1.0f }) {
			const auto bank = OffsetPath(stream, side * wide);
			a_drawList->AddPolyline(bank.data(), static_cast<int>(bank.size()), a_colour, ImDrawFlags_None, 1.0f);
		}
		a_drawList->AddPolyline(stream.data(), static_cast<int>(stream.size()), WithAlpha(water, 0.35f), ImDrawFlags_None, std::max(1.0f, wide * 0.8f));
	}
}
