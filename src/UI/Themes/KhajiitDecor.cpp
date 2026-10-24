#include "UI/Themes/KhajiitDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace CompletionistUI::KhajiitDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Rug dyes, brass, the two moons
		constexpr ImU32 rugRed = IM_COL32(100, 28, 34, 255);
		constexpr ImU32 rugDark = IM_COL32(38, 12, 16, 255);
		constexpr ImU32 saffron = IM_COL32(196, 134, 56, 255);
		constexpr ImU32 indigo = IM_COL32(52, 46, 104, 255);
		constexpr ImU32 cream = IM_COL32(226, 206, 168, 255);
		constexpr ImU32 brass = IM_COL32(204, 150, 70, 255);
		constexpr ImU32 brassDark = IM_COL32(110, 74, 34, 255);
		constexpr ImU32 lamplight = IM_COL32(246, 176, 84, 255);
		constexpr ImU32 masser = IM_COL32(226, 124, 84, 255);
		constexpr ImU32 secunda = IM_COL32(214, 222, 238, 255);

		//---------------------------------------------------
		//-- Khajiit Decor ( Sparkle ) ----------------------
		//---------------------------------------------------

		// Four-point glint, for bright stars and moon sugar
		void Sparkle(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

			const ImU32 clear = WithAlpha(a_colour, 0.0f);
			const float thin = std::max(1.0f, a_size * 0.12f);
			const ImVec2 left[4] = { ImVec2(a_centre.x - a_size, a_centre.y), ImVec2(a_centre.x, a_centre.y - thin), ImVec2(a_centre.x + a_size, a_centre.y), ImVec2(a_centre.x, a_centre.y + thin) };
			const ImVec2 up[4] = { ImVec2(a_centre.x, a_centre.y - a_size), ImVec2(a_centre.x + thin, a_centre.y), ImVec2(a_centre.x, a_centre.y + a_size), ImVec2(a_centre.x - thin, a_centre.y) };
			GradientQuad(a_drawList, left, { clear, a_colour, clear, a_colour });
			GradientQuad(a_drawList, up, { clear, a_colour, clear, a_colour });
			a_drawList->AddCircleFilled(a_centre, thin * 1.2f, a_colour, 8);
		}

		//---------------------------------------------------
		//-- Khajiit Decor ( Dune ) -------------------------
		//---------------------------------------------------

		// A ridge of sand from the bottom up, lit along its crest
		void Dune(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_rise, float a_seed, ImU32 a_fill, ImU32 a_rim) {

			const float width = a_max.x - a_min.x;
			const auto  crest = [&](float a_x) {
				const float u = (a_x - a_min.x) / width;
				const float wave = 0.55f * std::sin(u * 5.1f + a_seed) + 0.3f * std::sin(u * 11.3f + a_seed * 2.1f) + 0.15f * std::sin(u * 23.0f + a_seed * 0.7f);
				return a_max.y - a_rise * (0.6f + 0.4f * wave);
			};
			const int   steps = std::max(16, static_cast<int>(width / 12.0f));
			const float step = width / steps;
			for (int i = 0; i < steps; ++i) {
				const float  x0 = a_min.x + step * i;
				const float  x1 = x0 + step;
				const ImVec2 quad[4] = { ImVec2(x0, crest(x0)), ImVec2(x1, crest(x1)), ImVec2(x1, a_max.y), ImVec2(x0, a_max.y) };
				a_drawList->AddConvexPolyFilled(quad, 4, a_fill);
				a_drawList->AddLine(quad[0], quad[1], a_rim, 1.2f);
			}
		}

		//---------------------------------------------------
		//-- Khajiit Decor ( Night ) ------------------------
		//---------------------------------------------------

		// Backdrop: night sky, the moons, stars, dunes and moon sugar
		void Night(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const bool  main = height > size * 20.0f;
			a_drawList->PushClipRect(a_min, a_max, true);

			// Indigo above, warmer plum towards the sand
			const ImVec2 sky[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, sky, { IM_COL32(26, 22, 60, 110), IM_COL32(26, 22, 60, 110), IM_COL32(52, 22, 40, 90), IM_COL32(52, 22, 40, 90) });

			// Stars, thinning towards the horizon
			Random     random{ 0x5EEDu };
			const int  stars = std::clamp(static_cast<int>(width * height / (size * size * 3.0f)), 20, 420);
			for (int i = 0; i < stars; ++i) {
				const float u = random.Next();
				const float v = random.Next() * random.Next();
				const float bright = random.Range(0.15f, 0.7f) * (1.0f - v);
				const ImVec2 at(a_min.x + u * width, a_min.y + v * height * 0.85f);
				a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.3f), WithAlpha(secunda, bright), 6);
			}
			for (int i = 0; i < (main ? 9 : 3); ++i) {
				const ImVec2 at(a_min.x + width * random.Range(0.05f, 0.95f), a_min.y + height * random.Range(0.04f, 0.45f));
				Sparkle(a_drawList, at, size * random.Range(0.3f, 0.55f), WithAlpha(secunda, random.Range(0.35f, 0.6f)));
			}

			// Masser and Secunda, faint behind the window
			if (main) {
				const float  span = std::min(width, height);
				const ImVec2 masserAt(a_min.x + width * 0.86f, a_min.y + height * 0.7f);
				const float  masserRadius = span * 0.14f;
				SoftSpot(a_drawList, masserAt, masserRadius * 2.2f, WithAlpha(masser, 0.01f));
				a_drawList->AddCircleFilled(masserAt, masserRadius, WithAlpha(masser, 0.07f), 64);
				Moon(a_drawList, masserAt, masserRadius, WithAlpha(masser, 0.08f), 0.62f);
				Random craters{ 0xC4A7u };
				for (int i = 0; i < 9; ++i) {
					const float angle = craters.Range(0.0f, 2.0f * pi);
					const float reach = masserRadius * 0.75f * std::sqrt(craters.Next());
					a_drawList->AddCircleFilled(ImVec2(masserAt.x + std::cos(angle) * reach, masserAt.y + std::sin(angle) * reach), masserRadius * craters.Range(0.06f, 0.16f), IM_COL32(60, 20, 20, 14), 16);
				}
				const ImVec2 secundaAt(a_min.x + width * 0.71f, a_min.y + height * 0.8f);
				const float  secundaRadius = span * 0.045f;
				SoftSpot(a_drawList, secundaAt, secundaRadius * 2.6f, WithAlpha(secunda, 0.01f));
				a_drawList->AddCircleFilled(secundaAt, secundaRadius, WithAlpha(secunda, 0.08f), 40);
				Moon(a_drawList, secundaAt, secundaRadius, WithAlpha(secunda, 0.08f), 0.7f);
			}

			// Dunes, back to front
			const float rise = height * (main ? 0.16f : 0.12f);
			Dune(a_drawList, a_min, a_max, rise, 0.4f, IM_COL32(40, 24, 42, 190), IM_COL32(190, 120, 96, 60));
			Dune(a_drawList, a_min, a_max, rise * 0.68f, 2.3f, IM_COL32(30, 18, 30, 215), IM_COL32(204, 132, 92, 70));
			Dune(a_drawList, a_min, a_max, rise * 0.4f, 4.1f, IM_COL32(20, 12, 20, 235), IM_COL32(214, 146, 96, 84));

			// Moon sugar glinting in the sand
			for (int i = 0; i < (main ? 14 : 4); ++i) {
				const ImVec2 at(a_min.x + width * random.Range(0.03f, 0.97f), a_max.y - rise * random.Range(0.05f, 0.45f));
				Sparkle(a_drawList, at, size * random.Range(0.12f, 0.24f), WithAlpha(IM_COL32(255, 236, 190, 255), random.Range(0.3f, 0.6f)));
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.14f, IM_COL32(6, 4, 12, 190));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Khajiit Decor ( Rug Band ) ---------------------
		//---------------------------------------------------

		// One side of the frame: a woven kilim strip
		void RugBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, rugRed);

			// Weave: fine ticks across the strip
			for (float along = 0.0f; along < length; along += 3.0f) {
				a_drawList->AddLine(at(along, 0.0f), at(along, depth), IM_COL32(0, 0, 0, 34), 1.0f);
			}

			// Saffron stripes along both edges
			for (const float across : { depth * 0.16f, depth * 0.84f }) {
				a_drawList->AddLine(at(0.0f, across), at(length, across), saffron, std::max(1.0f, depth * 0.08f));
			}

			// Diamonds down the middle, indigo and cream turn about, with a zigzag between
			const float spacing = depth * 1.3f;
			const float half = depth * 0.26f;
			int         index = 0;
			for (float along = spacing * 0.5f; along < length; along += spacing, ++index) {
				const ImVec2 centre = at(along, depth * 0.5f);
				const ImVec2 points[4] = { ImVec2(centre.x, centre.y - half), ImVec2(centre.x + half, centre.y), ImVec2(centre.x, centre.y + half), ImVec2(centre.x - half, centre.y) };
				a_drawList->AddConvexPolyFilled(points, 4, index % 2 == 0 ? indigo : cream);
				a_drawList->AddPolyline(points, 4, saffron, ImDrawFlags_Closed, 1.0f);
				const float zig = depth * 0.12f;
				const ImVec2 a = at(along + half * 1.4f, depth * 0.5f - zig);
				const ImVec2 b = at(along + spacing * 0.5f, depth * 0.5f + zig);
				const ImVec2 c = at(along + spacing - half * 1.4f, depth * 0.5f - zig);
				if (along + spacing - half * 1.4f < length) {
					a_drawList->AddLine(a, b, WithAlpha(cream, 0.6f), 1.0f);
					a_drawList->AddLine(b, c, WithAlpha(cream, 0.6f), 1.0f);
				}
			}
			a_drawList->AddRect(a_min, a_max, rugDark, 0.0f, 0, 1.0f);
		}

		//---------------------------------------------------
		//-- Khajiit Decor ( Medallion ) --------------------
		//---------------------------------------------------

		// Brass roundel in the frame's corners, a crescent on it
		void Medallion(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

			a_drawList->AddCircleFilled(a_centre, a_radius, brassDark, 24);
			a_drawList->AddCircle(a_centre, a_radius, brass, 24, std::max(1.0f, a_radius * 0.14f));
			a_drawList->AddCircle(a_centre, a_radius * 0.72f, WithAlpha(brass, 0.6f), 24, 1.0f);
			Crescent(a_drawList, a_centre, a_radius * 0.5f, cream, true);
		}

		//---------------------------------------------------
		//-- Khajiit Decor ( Lantern ) ----------------------
		//---------------------------------------------------

		// Brass lantern hanging on a chain, glowing
		void Lantern(ImDrawList* a_drawList, ImVec2 a_top, float a_chain, float a_size) {

			// Chain links
			const float link = a_size * 0.16f;
			int         index = 0;
			for (float y = a_top.y; y < a_top.y + a_chain; y += link * 1.4f, ++index) {
				if (index % 2 == 0) {
					a_drawList->AddEllipse(ImVec2(a_top.x, y + link * 0.7f), ImVec2(link * 0.35f, link * 0.7f), brass, 0.0f, 10, 1.0f);
				} else {
					a_drawList->AddLine(ImVec2(a_top.x, y), ImVec2(a_top.x, y + link * 1.4f), brass, 1.5f);
				}
			}
			const float top = a_top.y + a_chain;
			const float w = a_size * 0.36f;
			const float h = a_size * 0.62f;
			const float midY = top + a_size * 0.2f + h * 0.5f;
			SoftSpot(a_drawList, ImVec2(a_top.x, midY), a_size * 2.4f, WithAlpha(lamplight, 0.016f));

			// Domed cap and finial
			a_drawList->AddTriangleFilled(ImVec2(a_top.x - w * 1.15f, top + a_size * 0.2f), ImVec2(a_top.x, top - a_size * 0.02f), ImVec2(a_top.x + w * 1.15f, top + a_size * 0.2f), brass);
			// Glass with a lattice
			const ImVec2 glass[6] = {
				ImVec2(a_top.x - w * 0.8f, top + a_size * 0.2f),
				ImVec2(a_top.x + w * 0.8f, top + a_size * 0.2f),
				ImVec2(a_top.x + w, midY),
				ImVec2(a_top.x + w * 0.7f, top + a_size * 0.2f + h),
				ImVec2(a_top.x - w * 0.7f, top + a_size * 0.2f + h),
				ImVec2(a_top.x - w, midY),
			};
			a_drawList->AddConvexPolyFilled(glass, 6, WithAlpha(lamplight, 0.85f));
			a_drawList->AddCircleFilled(ImVec2(a_top.x, midY), w * 0.45f, IM_COL32(255, 228, 160, 255), 12);
			a_drawList->AddLine(ImVec2(a_top.x, top + a_size * 0.2f), ImVec2(a_top.x, top + a_size * 0.2f + h), brassDark, 1.5f);
			a_drawList->AddLine(glass[5], glass[2], brassDark, 1.5f);
			a_drawList->AddPolyline(glass, 6, brass, ImDrawFlags_Closed, 1.5f);
			a_drawList->AddRectFilled(ImVec2(a_top.x - w * 0.75f, top + a_size * 0.2f + h), ImVec2(a_top.x + w * 0.75f, top + a_size * 0.28f + h), brass);
			a_drawList->AddTriangleFilled(ImVec2(a_top.x - w * 0.3f, top + a_size * 0.28f + h), ImVec2(a_top.x + w * 0.3f, top + a_size * 0.28f + h), ImVec2(a_top.x, top + a_size * 0.45f + h), brass);
		}
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Moon ) -------------------------
	//---------------------------------------------------

	// The lit part of a moon, a_phase 0 new to 1 full, lit from the right
	void Moon(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, float a_phase) {

		if (a_phase <= 0.0f) {
			return;
		}
		// Limb down the right side, terminator back up; a crescent is concave, a gibbous moon convex
		constexpr int steps = 24;
		const float   squeeze = 1.0f - 2.0f * std::min(a_phase, 1.0f);
		for (int i = 0; i <= steps; ++i) {
			const float angle = -pi * 0.5f + pi * static_cast<float>(i) / steps;
			a_drawList->PathLineTo(ImVec2(a_centre.x + std::cos(angle) * a_radius, a_centre.y + std::sin(angle) * a_radius));
		}
		for (int i = steps - 1; i >= 1; --i) {
			const float angle = -pi * 0.5f + pi * static_cast<float>(i) / steps;
			a_drawList->PathLineTo(ImVec2(a_centre.x + std::cos(angle) * a_radius * squeeze, a_centre.y + std::sin(angle) * a_radius));
		}
		if (a_phase >= 0.5f) {
			a_drawList->PathFillConvex(a_colour);
		} else {
			a_drawList->PathFillConcave(a_colour);
		}
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Crescent ) ---------------------
	//---------------------------------------------------

	// In place of the diamond: a crescent moon
	void Crescent(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float r = a_radius * 1.1f;
		if (a_filled) {
			Moon(a_drawList, a_centre, r, a_colour, 0.32f);
			return;
		}
		constexpr int steps = 14;
		ImVec2        outline[steps * 2 + 2];
		for (int i = 0; i <= steps; ++i) {
			const float angle = -pi * 0.5f + pi * static_cast<float>(i) / steps;
			outline[i] = ImVec2(a_centre.x + std::cos(angle) * r, a_centre.y + std::sin(angle) * r);
			outline[steps * 2 + 1 - i] = ImVec2(a_centre.x + std::cos(angle) * r * 0.36f, a_centre.y + std::sin(angle) * r);
		}
		a_drawList->AddPolyline(outline, steps * 2 + 2, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.2f));
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Phase ) ------------------------
	//---------------------------------------------------

	// Marker: new moon when needed, waxing to full when found
	void Phase(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.36f;
		a_drawList->AddCircle(a_centre, radius, a_colour, 24, std::max(1.0f, a_size * 0.07f));
		if (a_fill > 0.0f) {
			Moon(a_drawList, a_centre, radius * 0.78f, a_colour, a_fill);
		}
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Twin Moons ) -------------------
	//---------------------------------------------------

	// Masser's crescent with Secunda beside it
	void TwinMoons(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

		SoftSpot(a_drawList, a_centre, a_radius * 2.0f, WithAlpha(masser, 0.025f));
		Moon(a_drawList, ImVec2(a_centre.x - a_radius * 0.2f, a_centre.y), a_radius, masser, 0.34f);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x + a_radius * 0.55f, a_centre.y - a_radius * 0.55f), a_radius * 0.32f, secunda, 16);
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Thread ) -----------------------
	//---------------------------------------------------

	// Woven zigzag in place of a rule, beads at intervals, fading at the ends
	void Thread(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float amplitude = a_size * 0.16f;
		const float period = a_size * 0.7f;
		int         index = 0;
		for (float x = a_from.x; x < a_to.x; x += period * 0.5f, ++index) {
			const float t = (x - a_from.x) / length;
			const float next = std::min(a_to.x, x + period * 0.5f);
			const float fade = std::pow(std::sin(std::clamp(t, 0.0f, 1.0f) * pi), 0.6f);
			const float y0 = a_from.y + (index % 2 == 0 ? -amplitude : amplitude);
			const float y1 = a_from.y + (index % 2 == 0 ? amplitude : -amplitude);
			a_drawList->AddLine(ImVec2(x, y0), ImVec2(next, y1), WithAlpha(a_colour, fade), 1.2f);
			if (index % 8 == 4 && fade > 0.3f) {
				const float  bead = a_size * 0.13f;
				const ImVec2 points[4] = { ImVec2(next, a_from.y - bead * 1.6f), ImVec2(next + bead, a_from.y), ImVec2(next, a_from.y + bead * 1.6f), ImVec2(next - bead, a_from.y) };
				a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(a_colour, fade));
			}
		}
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Divider ) ----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Thread(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Thread(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		TwinMoons(a_drawList, ImVec2(midX, a_from.y), gap * 0.42f);
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Frame ) ------------------------
	//---------------------------------------------------

	// Woven rug border over the night desert, brass at the corners, lanterns above
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.78f * std::max(0.6f, compact);

		Night(a_drawList, a_min, a_max);

		RugBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true);
		RugBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true);
		RugBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false);
		RugBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(brass, 0.35f), 0.0f, 0, 1.0f);

		// Lanterns in the main window only, clear of the buttons and the title
		if (compact >= 0.8f) {
			for (const float at : { 0.2f, 0.8f }) {
				Lantern(a_drawList, ImVec2(a_min.x + width * at, a_min.y + band), size * 0.5f, size * 1.7f);
			}
		}

		const float medallion = band * 0.95f;
		Medallion(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), medallion);
		Medallion(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), medallion);
		Medallion(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), medallion);
		Medallion(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), medallion);
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Panel ) ------------------------
	//---------------------------------------------------

	// Inside a panel: a woven lattice at the edges, lamplight below, the moons in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		// Lamplight from below
		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(lamplight, 0.0f), WithAlpha(lamplight, 0.0f), WithAlpha(lamplight, 0.07f), WithAlpha(lamplight, 0.07f));

		// Diamond lattice, like the rug's weave, fading towards the middle
		const float cell = size * 1.4f;
		for (float y = a_min.y; y < a_max.y; y += cell) {
			for (float x = a_min.x; x < a_max.x; x += cell) {
				const ImVec2 centre(x + cell * 0.5f, y + cell * 0.5f);
				const float  weight = EdgeWeight(centre, a_min, a_max, band);
				if (weight < 0.03f) {
					continue;
				}
				const ImVec2 points[4] = { ImVec2(centre.x, y), ImVec2(x + cell, centre.y), ImVec2(centre.x, y + cell), ImVec2(x, centre.y) };
				a_drawList->AddPolyline(points, 4, WithAlpha(saffron, 0.12f * weight), ImDrawFlags_Closed, 1.0f);
				a_drawList->AddCircleFilled(centre, 1.2f, WithAlpha(cream, 0.18f * weight), 6);
			}
		}

		// Stars caught in the corners
		Random random{ static_cast<std::uint32_t>(width) * 13u + 5u };
		for (int i = 0; i < 6; ++i) {
			const ImVec2 at(a_min.x + width * (i % 2 == 0 ? random.Range(0.01f, 0.12f) : random.Range(0.88f, 0.99f)), a_min.y + height * random.Range(0.05f, 0.95f));
			Sparkle(a_drawList, at, size * random.Range(0.15f, 0.3f), WithAlpha(secunda, random.Range(0.25f, 0.45f)));
		}

		// Masser and Secunda in a ring of stars, large and faint, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			a_drawList->AddCircle(centre, radius * 1.1f, WithAlpha(brass, 0.12f), 64, std::max(1.0f, radius * 0.025f));
			for (int i = 0; i < 16; ++i) {
				const float angle = 2.0f * pi * static_cast<float>(i) / 16.0f;
				a_drawList->AddCircleFilled(ImVec2(centre.x + std::cos(angle) * radius * 1.1f, centre.y + std::sin(angle) * radius * 1.1f), std::max(1.5f, radius * (i % 4 == 0 ? 0.04f : 0.02f)), WithAlpha(cream, 0.14f), 8);
			}
			Moon(a_drawList, ImVec2(centre.x - radius * 0.12f, centre.y + radius * 0.08f), radius * 0.72f, WithAlpha(masser, 0.12f), 0.34f);
			a_drawList->AddCircleFilled(ImVec2(centre.x + radius * 0.42f, centre.y - radius * 0.4f), radius * 0.22f, WithAlpha(secunda, 0.1f), 32);
		}
	}

	//---------------------------------------------------
	//-- Khajiit Decor ( Panel Corners ) ----------------
	//---------------------------------------------------

	// Brass corner fittings, a scroll at the end of each arm
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(2.0f, size * 0.14f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, brassDark, ImDrawFlags_None, thick + 2.0f);
			a_drawList->AddPolyline(points, 3, brass, ImDrawFlags_None, thick);
			// Scrolls curling in at the ends
			const float curl = size * 0.18f;
			a_drawList->AddCircle(ImVec2(corner.x + sx * arm, corner.y + sy * curl), curl, brass, 12, std::max(1.0f, thick * 0.6f));
			a_drawList->AddCircle(ImVec2(corner.x + sx * curl, corner.y + sy * arm), curl, brass, 12, std::max(1.0f, thick * 0.6f));
			Crescent(a_drawList, ImVec2(corner.x + sx * size * 0.42f, corner.y + sy * size * 0.42f), size * 0.16f, brass, true);
		}
	}
}
