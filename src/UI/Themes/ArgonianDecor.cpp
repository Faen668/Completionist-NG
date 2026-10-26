#include "UI/Themes/ArgonianDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace CompletionistUI::ArgonianDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Xanmeer stone, Hist sap, mangrove roots, Black Marsh water
		constexpr ImU32 stone = IM_COL32(59, 65, 52, 255);
		constexpr ImU32 stoneLight = IM_COL32(92, 102, 80, 255);
		constexpr ImU32 stoneDark = IM_COL32(26, 30, 22, 255);
		constexpr ImU32 sap = IM_COL32(227, 164, 60, 255);
		constexpr ImU32 root = IM_COL32(34, 27, 16, 255);
		constexpr ImU32 hangingMoss = IM_COL32(90, 112, 62, 190);
		constexpr ImU32 water = IM_COL32(150, 170, 128, 255);
		constexpr ImU32 mist = IM_COL32(170, 188, 150, 255);

		//---------------------------------------------------
		//-- Argonian Decor ( Glyph ) -----------------------
		//---------------------------------------------------

		// Carving on a stone block: step pyramid, spiral or a stepped band
		void Glyph(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, int a_kind, ImU32 a_colour) {

			const float stroke = std::max(1.0f, a_size * 0.12f);
			switch (a_kind) {
			case 0:
				for (int step = 0; step < 3; ++step) {
					const float half = a_size * (0.5f - step * 0.15f);
					const float y = a_centre.y + a_size * (0.35f - step * 0.25f);
					a_drawList->AddLine(ImVec2(a_centre.x - half, y), ImVec2(a_centre.x + half, y), a_colour, stroke);
				}
				break;
			case 1:
				a_drawList->PathArcTo(a_centre, a_size * 0.4f, 0.0f, pi * 1.6f, 12);
				a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
				a_drawList->PathArcTo(a_centre, a_size * 0.18f, pi, pi * 2.4f, 8);
				a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
				break;
			default: {
				const float  u = a_size * 0.15f;
				const ImVec2 steps[] = {
					ImVec2(a_centre.x - u * 3.0f, a_centre.y + u), ImVec2(a_centre.x - u * 2.0f, a_centre.y + u), ImVec2(a_centre.x - u * 2.0f, a_centre.y),
					ImVec2(a_centre.x - u, a_centre.y), ImVec2(a_centre.x - u, a_centre.y - u), ImVec2(a_centre.x + u, a_centre.y - u),
					ImVec2(a_centre.x + u, a_centre.y), ImVec2(a_centre.x + u * 2.0f, a_centre.y), ImVec2(a_centre.x + u * 2.0f, a_centre.y + u), ImVec2(a_centre.x + u * 3.0f, a_centre.y + u),
				};
				a_drawList->AddPolyline(steps, 10, a_colour, ImDrawFlags_None, stroke);
				break;
			}
			}
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Stone Band ) ------------------
		//---------------------------------------------------

		// One side of the frame: blocks with joints and the odd carving
		void StoneBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, Random& a_random) {

			a_drawList->AddRectFilled(a_min, a_max, stone);
			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			float       along = 0.0f;
			while (along < length) {
				const float block = depth * a_random.Range(2.2f, 4.5f);
				const float end = std::min(length, along + block);
				const auto  at = [&](float a_along, float a_across) {
					return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
				};
				// Weathering: each block a little lighter or darker
				const float tone = a_random.Range(0.85f, 1.15f);
				a_drawList->AddRectFilled(at(along + 1.0f, 1.0f), at(end - 1.0f, depth - 1.0f), Shade(stone, tone));
				a_drawList->AddLine(at(along + 1.0f, 1.5f), at(end - 1.0f, 1.5f), WithAlpha(stoneLight, 0.7f));
				a_drawList->AddLine(at(end, 0.0f), at(end, depth), stoneDark, 1.5f);
				if (a_random.Next() < 0.45f && end - along > depth * 1.5f) {
					Glyph(a_drawList, at((along + end) * 0.5f, depth * 0.5f), depth * 0.55f, static_cast<int>(a_random.Range(0.0f, 2.99f)), Shade(stone, 0.55f));
				}
				// Lichen
				if (a_random.Next() < 0.35f) {
					const auto centre = at(a_random.Range(along, end), a_random.Range(0.0f, depth));
					for (int dot = 0; dot < 10; ++dot) {
						const float angle = a_random.Range(0.0f, 2.0f * pi);
						const float reach = depth * 0.7f * std::sqrt(a_random.Next());
						a_drawList->AddCircleFilled(ImVec2(centre.x + std::cos(angle) * reach, centre.y + std::sin(angle) * reach * 0.6f), depth * a_random.Range(0.06f, 0.14f), IM_COL32(96, 118, 62, static_cast<int>(a_random.Range(90.0f, 180.0f))), 6);
					}
				}
				along = end;
			}
			a_drawList->AddRect(a_min, a_max, stoneDark, 0.0f, 0, 1.0f);
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Step Pyramid ) ----------------
		//---------------------------------------------------

		// Xanmeer in the frame's corners, a bead of sap on the top step
		void StepPyramid(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			for (int step = 0; step < 3; ++step) {
				const float half = a_size * (1.0f - step * 0.28f);
				const ImVec2 min(a_centre.x - half, a_centre.y - half);
				const ImVec2 max(a_centre.x + half, a_centre.y + half);
				a_drawList->AddRectFilled(min, max, Shade(stone, 0.9f + step * 0.15f));
				a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), stoneLight);
				a_drawList->AddRect(min, max, stoneDark, 0.0f, 0, 1.5f);
			}
			a_drawList->AddCircleFilled(a_centre, a_size * 0.12f, WithAlpha(sap, 0.85f), 8);
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Root ) ------------------------
		//---------------------------------------------------

		// Twisting root, thick at the start, a_angle is its heading; rootlets grow to a_outward's side only
		void Root(ImDrawList* a_drawList, ImVec2 a_start, float a_angle, float a_length, float a_thickness, float a_outward, Random& a_random) {

			constexpr int segments = 18;
			ImVec2        point = a_start;
			float         heading = a_angle;
			const float   step = a_length / segments;
			for (int i = 0; i < segments; ++i) {
				const float t = static_cast<float>(i) / segments;
				heading = a_angle + std::sin(t * pi * 5.0f + a_random.Range(-0.3f, 0.3f)) * 0.12f;
				const ImVec2 next(point.x + std::cos(heading) * step, point.y + std::sin(heading) * step);
				const float  width = std::max(1.0f, a_thickness * (1.0f - t * 0.6f));
				a_drawList->AddLine(point, next, root, width);
				a_drawList->AddLine(ImVec2(point.x, point.y - width * 0.3f), ImVec2(next.x, next.y - width * 0.3f), WithAlpha(IM_COL32(78, 68, 48, 255), 0.6f), std::max(1.0f, width * 0.25f));
				// Rootlets
				if (a_random.Next() < 0.25f && t < 0.8f) {
					const float off = heading + a_outward * a_random.Range(0.6f, 1.2f);
					a_drawList->AddLine(next, ImVec2(next.x + std::cos(off) * step * 1.6f, next.y + std::sin(off) * step * 1.6f), root, std::max(1.0f, width * 0.35f));
				}
				point = next;
			}
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Moss Clump ) ------------------
		//---------------------------------------------------

		// Strands of swamp moss hanging from the top edge
		void MossClump(ImDrawList* a_drawList, ImVec2 a_top, float a_length, Random& a_random) {

			const int strands = static_cast<int>(a_random.Range(6.0f, 11.0f));
			for (int strand = 0; strand < strands; ++strand) {
				const float x = a_top.x + a_random.Range(-a_length * 0.25f, a_length * 0.25f);
				const float drop = a_length * a_random.Range(0.5f, 1.0f);
				const float sway = a_random.Range(1.0f, 3.0f);
				const float phase = a_random.Range(0.0f, 2.0f * pi);
				constexpr int segments = 10;
				for (int i = 0; i < segments; ++i) {
					const float t0 = static_cast<float>(i) / segments;
					const float t1 = static_cast<float>(i + 1) / segments;
					const ImVec2 from(x + std::sin(phase + t0 * 5.0f) * sway * t0 * 2.0f, a_top.y + drop * t0);
					const ImVec2 to(x + std::sin(phase + t1 * 5.0f) * sway * t1 * 2.0f, a_top.y + drop * t1);
					a_drawList->AddLine(from, to, WithAlpha(hangingMoss, 1.0f - t0 * 0.7f), 1.6f - t0);
				}
			}
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Hist Branch ) -----------------
		//---------------------------------------------------

		// The Hist, a faint shape behind everything; gnarled, curving branches
		void HistBranch(ImDrawList* a_drawList, ImVec2 a_from, float a_angle, float a_length, float a_width, int a_depth, Random& a_random, ImU32 a_colour, ImVector<ImVec2>& a_tips) {

			const float  bendA = a_random.Range(-0.45f, 0.45f);
			const float  bendB = a_random.Range(-0.45f, 0.45f);
			const ImVec2 to(a_from.x + std::cos(a_angle) * a_length, a_from.y + std::sin(a_angle) * a_length);
			const ImVec2 c1(a_from.x + std::cos(a_angle + bendA) * a_length * 0.35f, a_from.y + std::sin(a_angle + bendA) * a_length * 0.35f);
			const ImVec2 c2(to.x - std::cos(a_angle + bendB) * a_length * 0.35f, to.y - std::sin(a_angle + bendB) * a_length * 0.35f);
			// Tapering: thick at the base, thinner along the branch
			constexpr int segments = 10;
			ImVec2        previous = a_from;
			for (int i = 1; i <= segments; ++i) {
				const float  t = static_cast<float>(i) / segments;
				const float  u = 1.0f - t;
				const ImVec2 point(u * u * u * a_from.x + 3.0f * u * u * t * c1.x + 3.0f * u * t * t * c2.x + t * t * t * to.x, u * u * u * a_from.y + 3.0f * u * u * t * c1.y + 3.0f * u * t * t * c2.y + t * t * t * to.y);
				a_drawList->AddLine(previous, point, a_colour, std::max(1.0f, a_width * (1.0f - t * 0.4f)));
				previous = point;
			}
			if (a_depth == 0) {
				a_tips.push_back(to);
				return;
			}
			const int children = a_depth > 3 ? 2 : 3;
			for (int child = 0; child < children; ++child) {
				const float spread = (static_cast<float>(child) - (children - 1) * 0.5f) * a_random.Range(0.5f, 0.85f);
				HistBranch(a_drawList, to, a_angle + spread + a_random.Range(-0.15f, 0.15f), a_length * a_random.Range(0.58f, 0.76f), a_width * 0.6f, a_depth - 1, a_random, a_colour, a_tips);
			}
		}

		//---------------------------------------------------
		//-- Argonian Decor ( Swamp ) -----------------------
		//---------------------------------------------------

		// Backdrop: dark marsh water, mist lying over it, the Hist, sap spores and bubbles
		void Swamp(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			// Murk rising from below
			const ImU32 murk = IM_COL32(44, 52, 30, 80);
			a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - height * 0.45f), a_max, WithAlpha(murk, 0.0f), WithAlpha(murk, 0.0f), murk, murk);

			// Mist in soft bands over the water
			for (int band = 0; band < 4; ++band) {
				const float y = a_min.y + height * (0.3f + 0.17f * band);
				const float half = size * (1.2f + 0.5f * band);
				const ImU32 thick = WithAlpha(mist, 0.035f + 0.01f * band);
				const ImU32 clear = WithAlpha(mist, 0.0f);
				a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, y - half), ImVec2(a_max.x, y), clear, clear, thick, thick);
				a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, y), ImVec2(a_max.x, y + half), thick, thick, clear, clear);
			}

			// The Hist, only in the main window; small panels stay clear
			Random           random{ 0x4157u };
			ImVector<ImVec2> tips;
			if (height > size * 20.0f) {
				const ImU32  tree = IM_COL32(44, 54, 32, 52);
				const ImVec2 trunk(a_min.x + width * 0.68f, a_max.y + size);
				HistBranch(a_drawList, trunk, -pi * 0.5f, height * 0.3f, size * 2.2f, 5, random, tree, tips);
				// Roots spreading into the water
				for (const float angle : { 0.25f, 0.6f, pi - 0.6f, pi - 0.25f }) {
					HistBranch(a_drawList, ImVec2(trunk.x, trunk.y - height * 0.08f), angle, height * 0.12f, size * 1.1f, 1, random, tree, tips);
				}
				for (const auto& tip : tips) {
					SoftSpot(a_drawList, tip, size * random.Range(1.0f, 2.0f), IM_COL32(110, 120, 70, 2));
				}
			}

			// Spores of Hist sap drifting
			const int spores = std::clamp(static_cast<int>(width * height / (size * size * 60.0f)), 4, 26);
			for (int i = 0; i < spores; ++i) {
				ImVec2 at;
				if (i < static_cast<int>(tips.size()) && random.Next() < 0.7f) {
					const auto& tip = tips[i];
					at = ImVec2(tip.x + random.Range(-2.0f, 2.0f) * size, tip.y + random.Range(-2.0f, 2.0f) * size);
				} else {
					at = ImVec2(a_min.x + width * random.Range(0.03f, 0.97f), a_min.y + height * random.Range(0.1f, 0.95f));
				}
				const float glow = random.Range(0.4f, 1.0f);
				SoftSpot(a_drawList, at, size * 0.55f * glow, WithAlpha(sap, 0.03f * glow));
				a_drawList->AddCircleFilled(at, std::max(1.0f, size * 0.07f * glow), WithAlpha(sap, 0.55f * glow), 8);
			}

			// Bubbles, more towards the bottom
			const int bubbles = std::clamp(static_cast<int>(width * height / (size * size * 34.0f)), 8, 46);
			for (int i = 0; i < bubbles; ++i) {
				const float v = std::sqrt(random.Next());
				const ImVec2 at(a_min.x + width * random.Range(0.02f, 0.98f), a_min.y + height * (0.3f + v * 0.68f));
				const float  radius = size * random.Range(0.08f, 0.26f);
				a_drawList->AddCircle(at, radius, WithAlpha(water, random.Range(0.08f, 0.2f)), 12, 1.0f);
				a_drawList->AddCircleFilled(ImVec2(at.x - radius * 0.35f, at.y - radius * 0.35f), std::max(0.8f, radius * 0.22f), WithAlpha(water, 0.18f), 6);
			}

			// Ripples on the surface
			for (int i = 0; i < 6; ++i) {
				const ImVec2 at(a_min.x + width * random.Range(0.1f, 0.9f), a_min.y + height * random.Range(0.72f, 0.95f));
				const float  radius = size * random.Range(1.5f, 3.5f);
				for (int ring = 1; ring <= 3; ++ring) {
					a_drawList->AddEllipse(at, ImVec2(radius * ring * 0.5f, radius * ring * 0.14f), WithAlpha(water, 0.07f / ring), 0.0f, 32, 1.0f);
				}
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.16f, IM_COL32(4, 6, 2, 210));
			a_drawList->PopClipRect();
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Scale ) -----------------------
	//---------------------------------------------------

	// In place of the diamond: a scale, rounded point down
	void Scale(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		constexpr int steps = 7;
		ImVec2        points[steps * 3 + 1];
		int           count = 0;
		const float   r = a_radius * 1.05f;
		// Arched top, left to right
		for (int i = 0; i <= steps; ++i) {
			const float x = -1.0f + 2.0f * static_cast<float>(i) / steps;
			points[count++] = ImVec2(a_centre.x + x * r * 0.8f, a_centre.y - r * (0.55f + 0.3f * (1.0f - x * x)));
		}
		// Right side curving down to the point, then the left side back up
		for (int i = 1; i <= steps; ++i) {
			const float t = static_cast<float>(i) / steps;
			const float u = 1.0f - t;
			const float x = u * u * 0.8f + 2.0f * u * t * 0.8f;
			const float y = u * u * -0.55f + 2.0f * u * t * 0.45f + t * t * 1.0f;
			points[count++] = ImVec2(a_centre.x + x * r, a_centre.y + y * r);
		}
		for (int i = steps - 1; i >= 1; --i) {
			const float t = static_cast<float>(i) / steps;
			const float u = 1.0f - t;
			const float x = u * u * 0.8f + 2.0f * u * t * 0.8f;
			const float y = u * u * -0.55f + 2.0f * u * t * 0.45f + t * t * 1.0f;
			points[count++] = ImVec2(a_centre.x - x * r, a_centre.y + y * r);
		}
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(points, count, a_colour);
			if (a_radius > 4.0f) {
				a_drawList->AddLine(ImVec2(a_centre.x, a_centre.y - r * 0.6f), ImVec2(a_centre.x, a_centre.y + r * 0.7f), Shade(a_colour, 0.6f), std::max(1.0f, r * 0.08f));
			}
		} else {
			a_drawList->AddPolyline(points, count, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.2f));
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Egg ) -------------------------
	//---------------------------------------------------

	// Marker: an egg outline, filling in when found
	void Egg(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const auto shape = [&](float a_scale, ImVec2* a_points) {
			constexpr int steps = 24;
			const float   ry = a_size * 0.42f * a_scale;
			const float   rx = a_size * 0.3f * a_scale;
			for (int i = 0; i < steps; ++i) {
				const float angle = 2.0f * pi * static_cast<float>(i) / steps;
				const float y = -std::cos(angle);
				a_points[i] = ImVec2(a_centre.x + std::sin(angle) * rx * (1.0f + 0.14f * y), a_centre.y + y * ry);
			}
			return steps;
		};
		ImVec2 points[24];
		const int count = shape(1.0f, points);
		a_drawList->AddPolyline(points, count, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.07f));
		if (a_fill > 0.0f) {
			ImVec2 inner[24];
			shape(0.35f + 0.65f * a_fill, inner);
			a_drawList->AddConvexPolyFilled(inner, count, a_colour);
			if (a_fill >= 1.0f && a_size > 12.0f) {
				// Speckles
				const ImU32 speckle = Shade(a_colour, 0.55f);
				a_drawList->AddCircleFilled(ImVec2(a_centre.x - a_size * 0.08f, a_centre.y - a_size * 0.12f), a_size * 0.045f, speckle, 6);
				a_drawList->AddCircleFilled(ImVec2(a_centre.x + a_size * 0.1f, a_centre.y + a_size * 0.05f), a_size * 0.035f, speckle, 6);
				a_drawList->AddCircleFilled(ImVec2(a_centre.x - a_size * 0.04f, a_centre.y + a_size * 0.2f), a_size * 0.03f, speckle, 6);
			}
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Eye ) -------------------------
	//---------------------------------------------------

	// Reptile eye: amber iris, slit pupil
	void Eye(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		constexpr int steps = 10;
		const float   w = a_radius * 2.4f;
		const float   h = a_radius * 1.15f;
		ImVec2        outline[steps * 2];
		for (int i = 0; i < steps; ++i) {
			const float t = -1.0f + 2.0f * static_cast<float>(i) / (steps - 1);
			outline[i] = ImVec2(a_centre.x + t * w, a_centre.y - h * (1.0f - t * t));
			outline[steps * 2 - 1 - i] = ImVec2(a_centre.x + t * w, a_centre.y + h * (1.0f - t * t));
		}
		SoftSpot(a_drawList, a_centre, h * 2.2f, WithAlpha(sap, 0.03f));
		a_drawList->AddConvexPolyFilled(outline, steps * 2, IM_COL32(12, 16, 10, 230));
		a_drawList->AddCircleFilled(a_centre, h * 0.82f, sap, 16);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x, a_centre.y - h * 0.15f), h * 0.5f, Shade(sap, 1.15f), 12);
		const ImVec2 pupil[4] = {
			ImVec2(a_centre.x, a_centre.y - h * 0.78f),
			ImVec2(a_centre.x + h * 0.16f, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + h * 0.78f),
			ImVec2(a_centre.x - h * 0.16f, a_centre.y),
		};
		a_drawList->AddConvexPolyFilled(pupil, 4, IM_COL32(8, 6, 4, 255));
		a_drawList->AddPolyline(outline, steps * 2, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.18f));
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Leaf ) ------------------------
	//---------------------------------------------------

	// A Hist leaf: long and pointed, a vein down the middle, lying flat
	void Leaf(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		constexpr int steps = 8;
		const float   length = a_radius * 2.4f;
		const float   width = a_radius * 0.9f;
		ImVec2        outline[steps * 2];
		for (int i = 0; i < steps; ++i) {
			const float t = static_cast<float>(i) / (steps - 1);
			const float x = a_centre.x - length * 0.5f + length * t;
			const float half = width * 0.5f * std::sin(pi * std::pow(t, 0.75f));
			outline[i] = ImVec2(x, a_centre.y - half);
			outline[steps * 2 - 1 - i] = ImVec2(x, a_centre.y + half);
		}
		a_drawList->AddConvexPolyFilled(outline, steps * 2, a_colour);
		a_drawList->AddLine(ImVec2(a_centre.x - length * 0.45f, a_centre.y), ImVec2(a_centre.x + length * 0.4f, a_centre.y), Shade(a_colour, 0.55f), std::max(1.0f, a_radius * 0.12f));
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Ridge ) -----------------------
	//---------------------------------------------------

	// Spined ridge in place of a rule, spines growing towards the middle
	void Ridge(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		Decor::FadedLine(a_drawList, a_from, a_to, a_colour, std::max(1.0f, a_size * 0.1f));
		const float spacing = a_size * 0.7f;
		for (float x = a_from.x + spacing * 0.5f; x < a_to.x - spacing * 0.25f; x += spacing) {
			const float t = (x - a_from.x) / length;
			const float grow = std::sin(t * pi);
			if (grow < 0.15f) {
				continue;
			}
			const float tall = a_size * 0.5f * grow;
			const float base = spacing * 0.36f;
			a_drawList->AddTriangleFilled(ImVec2(x - base, a_from.y), ImVec2(x + base * 0.7f, a_from.y - tall), ImVec2(x + base, a_from.y), WithAlpha(a_colour, 0.35f + 0.65f * grow));
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Ridge(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Ridge(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Leaf(a_drawList, ImVec2(midX, a_from.y), gap * 0.32f, a_colour);
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// Xanmeer stone frame over the swamp, roots below, moss hanging above
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);
		const ImU32 accent = ImGui::GetColorU32(Themes::GetPalette().accent);

		Swamp(a_drawList, a_min, a_max);

		Random random{ 0x7A11u };
		StoneBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, random);
		StoneBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, random);
		StoneBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, random);
		StoneBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, random);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(accent, 0.3f), 0.0f, 0, 1.0f);

		// Moss hanging from the top, clear of the title; small panels keep their top clear
		Random moss{ 0x3055u };
		if (compact >= 0.8f) {
			for (const float at : { 0.08f, 0.17f, 0.27f, 0.36f, 0.64f, 0.73f, 0.83f, 0.92f }) {
				MossClump(a_drawList, ImVec2(a_min.x + width * (at + moss.Range(-0.02f, 0.02f)), a_min.y + band), size * 1.6f * compact * moss.Range(0.7f, 1.2f), moss);
			}
		}
		for (const float at : { 0.2f, 0.45f, 0.7f }) {
			MossClump(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + height * at), size * 1.2f * compact, moss);
			MossClump(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + height * at), size * 1.2f * compact, moss);
		}

		// Mangrove roots creeping out of the bottom corners and hanging over the top ones
		Random roots{ 0x2007u };
		const float thick = band * 0.7f;
		Root(a_drawList, ImVec2(a_min.x + band, a_max.y - band * 0.55f), 0.0f, width * 0.16f, thick, 1.0f, roots);
		Root(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band), -pi * 0.5f, height * 0.24f, thick, -1.0f, roots);
		Root(a_drawList, ImVec2(a_max.x - band, a_max.y - band * 0.55f), pi, width * 0.16f, thick, -1.0f, roots);
		Root(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band), -pi * 0.5f, height * 0.24f, thick, 1.0f, roots);
		if (compact >= 0.8f) {
			Root(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band), pi * 0.5f, height * 0.16f, thick * 0.8f, 1.0f, roots);
			Root(a_drawList, ImVec2(a_min.x + band, a_min.y + band * 0.5f), 0.0f, width * 0.07f, thick * 0.7f, 1.0f, roots);
			Root(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band), pi * 0.5f, height * 0.16f, thick * 0.8f, -1.0f, roots);
			Root(a_drawList, ImVec2(a_max.x - band, a_min.y + band * 0.5f), pi, width * 0.07f, thick * 0.7f, -1.0f, roots);
		}

		const float pyramid = band * 1.25f;
		StepPyramid(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), pyramid);
		StepPyramid(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), pyramid);
		StepPyramid(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), pyramid);
		StepPyramid(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), pyramid);
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: mist and water light below, the Hist in the empty corner with its eye in the trunk
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}

		// Murk rising from the bottom edge
		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(water, 0.0f), WithAlpha(water, 0.0f), WithAlpha(water, 0.07f), WithAlpha(water, 0.07f));

		// Water light rippling along the bottom
		Random random{ static_cast<std::uint32_t>(width) * 17u + 3u };
		for (int line = 0; line < 4; ++line) {
			const float y = a_max.y - glow * random.Range(0.15f, 0.85f);
			const float phase = random.Range(0.0f, 2.0f * pi);
			const float wave = size * random.Range(0.12f, 0.25f);
			ImVec2      previous(a_min.x, y + std::sin(phase) * wave);
			for (float x = a_min.x + size * 0.5f; x <= a_max.x; x += size * 0.5f) {
				const float  t = (x - a_min.x) / width;
				const ImVec2 point(x, y + std::sin(phase + t * 14.0f) * wave);
				a_drawList->AddLine(previous, point, WithAlpha(water, 0.1f * std::sin(t * pi)), 1.2f);
				previous = point;
			}
		}

		// The Hist, large and faint, in the lower right; its sap glows and one slit eye watches from the trunk
		if (a_emblem) {
			const float      radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2     centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const ImU32      faint = WithAlpha(IM_COL32(150, 170, 110, 255), 0.14f);
			Random           tree{ 0x4157u };
			ImVector<ImVec2> tips;
			SoftSpot(a_drawList, ImVec2(centre.x, centre.y + radius * 0.35f), radius * 0.6f, WithAlpha(sap, 0.012f));
			a_drawList->PushClipRect(ImVec2(centre.x - radius * 1.2f, centre.y - radius * 1.1f), ImVec2(centre.x + radius * 1.2f, centre.y + radius), true);
			HistBranch(a_drawList, ImVec2(centre.x, centre.y + radius * 0.75f), -pi * 0.5f, radius * 0.6f, radius * 0.12f, 4, tree, faint, tips);
			for (const float angle : { 0.35f, pi - 0.35f }) {
				HistBranch(a_drawList, ImVec2(centre.x, centre.y + radius * 0.7f), angle, radius * 0.3f, radius * 0.06f, 1, tree, faint, tips);
			}
			a_drawList->PopClipRect();
			for (const auto& tip : tips) {
				a_drawList->AddCircleFilled(tip, std::max(1.5f, radius * 0.025f), WithAlpha(sap, 0.3f), 8);
			}
			Eye(a_drawList, ImVec2(centre.x, centre.y + radius * 0.42f), radius * 0.08f, WithAlpha(sap, 0.45f));
		}
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// Carved stone brackets on the panel's corners
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.24f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx, outer.y + sy);
			// Two stone bars meeting in a square block
			a_drawList->AddRectFilled(ImVec2(std::min(corner.x, corner.x + sx * arm), std::min(corner.y, corner.y + sy * thick)), ImVec2(std::max(corner.x, corner.x + sx * arm), std::max(corner.y, corner.y + sy * thick)), Shade(stone, 1.6f));
			a_drawList->AddRectFilled(ImVec2(std::min(corner.x, corner.x + sx * thick), std::min(corner.y, corner.y + sy * arm)), ImVec2(std::max(corner.x, corner.x + sx * thick), std::max(corner.y, corner.y + sy * arm)), Shade(stone, 1.6f));
			const float  block = thick * 1.6f;
			const ImVec2 blockMin(std::min(corner.x, corner.x + sx * block), std::min(corner.y, corner.y + sy * block));
			const ImVec2 blockMax(std::max(corner.x, corner.x + sx * block), std::max(corner.y, corner.y + sy * block));
			a_drawList->AddRectFilled(blockMin, blockMax, Shade(stone, 1.9f));
			a_drawList->AddRect(blockMin, blockMax, stoneDark, 0.0f, 0, 1.0f);
			a_drawList->AddLine(ImVec2(blockMin.x, blockMin.y + 1.0f), ImVec2(blockMax.x, blockMin.y + 1.0f), stoneLight);
			a_drawList->AddCircleFilled(ImVec2((blockMin.x + blockMax.x) * 0.5f, (blockMin.y + blockMax.y) * 0.5f), block * 0.18f, sap, 8);
		}
	}
}

namespace CompletionistUI::ArgonianDecor
{
	//---------------------------------------------------
	//-- Argonian Decor ( Perk Node ) ----------------
	//---------------------------------------------------

	// Hist sap: a full egg when taken, an empty shell when next, a dull scale when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		if (a_state == PerkState::kLocked) {
			Scale(a_drawList, a_centre, a_radius * 0.55f, a_colour, false);
			return;
		}
		Egg(a_drawList, a_centre, a_radius * 2.4f, a_colour, a_state == PerkState::kTaken ? 1.0f : 0.0f);
	}

	//---------------------------------------------------
	//-- Argonian Decor ( Perk Link ) ----------------
	//---------------------------------------------------

	// Roots winding through the marsh, swollen with sap between taken perks
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		auto        points = PerkCurve(a_from, a_to, a_radius * 0.4f);
		const float wave = a_radius * 0.18f;
		for (std::size_t i = 1; i + 1 < points.size(); ++i) {
			const auto  normal = NormalAt(points, i);
			const float t = static_cast<float>(i) / points.size();
			const float side = std::sin(t * 3.14159f * 3.0f) * wave;
			points[i].x += normal.x * side;
			points[i].y += normal.y * side;
		}
		a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), a_colour, ImDrawFlags_None, std::max(1.0f, a_radius * (a_lit ? 0.22f : 0.1f)));
	}
}
