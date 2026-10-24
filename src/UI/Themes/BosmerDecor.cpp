#include "UI/Themes/BosmerDecor.hpp"
#include "UI/Core/Themes.hpp"
#include "UI/Themes/DecorDraw.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>

namespace CompletionistUI::BosmerDecor
{
	using namespace DecorDraw;

	namespace
	{
		//---------------------------------------------------
		//-- Bosmer Decor ( Leaf Colour ) -------------------
		//---------------------------------------------------

		// Deep greens with the odd dead or blood-dark leaf
		ImU32 LeafColour(Random& a_random, float a_alpha) {

			constexpr ImU32 colours[] = {
				IM_COL32(56, 80, 40, 255),
				IM_COL32(72, 98, 48, 255),
				IM_COL32(44, 64, 36, 255),
				IM_COL32(86, 106, 56, 255),
				IM_COL32(36, 54, 32, 255),
				IM_COL32(118, 80, 42, 255),
				IM_COL32(104, 34, 30, 255),
			};
			const float pick = a_random.Next();
			const auto  index = pick < 0.88f ? static_cast<std::size_t>(pick / 0.88f * 5.0f) : 5 + (pick > 0.95f ? 1 : 0);
			return WithAlpha(colours[std::min<std::size_t>(index, 6)], a_alpha);
		}

		//---------------------------------------------------
		//-- Bosmer Decor ( Bark ) --------------------------
		//---------------------------------------------------

		// Thick bark band between two rects, grain running along it
		void Bark(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_thickness) {

			const ImU32 bark = IM_COL32(38, 28, 20, 255);
			const ImU32 light = IM_COL32(68, 52, 36, 255);
			const ImU32 dark = IM_COL32(16, 12, 9, 255);
			const float t = a_thickness;
			a_drawList->AddRectFilled(a_min, ImVec2(a_max.x, a_min.y + t), bark);
			a_drawList->AddRectFilled(ImVec2(a_min.x, a_max.y - t), a_max, bark);
			a_drawList->AddRectFilled(ImVec2(a_min.x, a_min.y + t), ImVec2(a_min.x + t, a_max.y - t), bark);
			a_drawList->AddRectFilled(ImVec2(a_max.x - t, a_min.y + t), ImVec2(a_max.x, a_max.y - t), bark);

			// Grain streaks, long along the edge, short across it
			Random     random{ 0x5EED1u };
			const auto streaks = [&](ImVec2 a_from, ImVec2 a_to, bool a_horizontal) {
				const float length = a_horizontal ? a_to.x - a_from.x : a_to.y - a_from.y;
				const int   count = static_cast<int>(length / (t * 1.6f));
				for (int i = 0; i < count; ++i) {
					const float along = random.Range(0.0f, length);
					const float span = random.Range(t * 1.5f, t * 6.0f);
					const float across = random.Range(t * 0.15f, t * 0.85f);
					const ImU32 colour = random.Next() < 0.6f ? dark : light;
					const float width = random.Range(1.0f, std::max(1.2f, t * 0.16f));
					if (a_horizontal) {
						const float x = a_from.x + along;
						a_drawList->AddLine(ImVec2(x, a_from.y + across), ImVec2(std::min(x + span, a_to.x), a_from.y + across + random.Range(-0.6f, 0.6f)), colour, width);
					} else {
						const float y = a_from.y + along;
						a_drawList->AddLine(ImVec2(a_from.x + across, y), ImVec2(a_from.x + across + random.Range(-0.6f, 0.6f), std::min(y + span, a_to.y)), colour, width);
					}
				}
			};
			streaks(a_min, ImVec2(a_max.x, a_min.y), true);
			streaks(ImVec2(a_min.x, a_max.y - t), ImVec2(a_max.x, a_max.y), true);
			streaks(ImVec2(a_min.x, a_min.y + t), ImVec2(a_min.x, a_max.y - t), false);
			streaks(ImVec2(a_max.x - t, a_min.y + t), ImVec2(a_max.x, a_max.y - t), false);

			a_drawList->AddRect(a_min, a_max, dark, 0.0f, 0, 1.0f);
			a_drawList->AddRect(ImVec2(a_min.x + t, a_min.y + t), ImVec2(a_max.x - t, a_max.y - t), dark, 0.0f, 0, 1.0f);
		}

		//---------------------------------------------------
		//-- Bosmer Decor ( Moss ) --------------------------
		//---------------------------------------------------

		// Patches of moss creeping over the bark
		void Moss(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_thickness) {

			Random      random{ 0x3055u };
			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const int   patches = static_cast<int>((width + height) * 2.0f / (a_thickness * 14.0f));
			for (int i = 0; i < patches; ++i) {
				// Somewhere on the band, any side
				const float along = random.Range(0.0f, (width + height) * 2.0f);
				const float across = random.Range(0.0f, a_thickness);
				ImVec2      centre;
				if (along < width) {
					centre = ImVec2(a_min.x + along, a_min.y + across);
				} else if (along < width * 2.0f) {
					centre = ImVec2(a_min.x + along - width, a_max.y - across);
				} else if (along < width * 2.0f + height) {
					centre = ImVec2(a_min.x + across, a_min.y + along - width * 2.0f);
				} else {
					centre = ImVec2(a_max.x - across, a_min.y + along - width * 2.0f - height);
				}
				const float spread = a_thickness * random.Range(0.8f, 2.2f);
				const int   dots = static_cast<int>(random.Range(8.0f, 22.0f));
				for (int dot = 0; dot < dots; ++dot) {
					const float angle = random.Range(0.0f, 2.0f * std::numbers::pi_v<float>);
					const float reach = spread * std::sqrt(random.Next());
					const ImU32 colour = IM_COL32(static_cast<int>(random.Range(60.0f, 110.0f)), static_cast<int>(random.Range(95.0f, 140.0f)), static_cast<int>(random.Range(40.0f, 60.0f)), static_cast<int>(random.Range(120.0f, 230.0f)));
					a_drawList->AddCircleFilled(ImVec2(centre.x + std::cos(angle) * reach, centre.y + std::sin(angle) * reach * 0.6f), a_thickness * random.Range(0.09f, 0.2f), colour, 6);
				}
			}
		}

		//---------------------------------------------------
		//-- Bosmer Decor ( Leaf Cluster ) ------------------
		//---------------------------------------------------

		// Leaves fanned out from a corner along its two edges, a_angle points into the window
		void LeafCluster(ImDrawList* a_drawList, ImVec2 a_corner, float a_size, float a_angle, unsigned a_seed) {

			Random random{ a_seed };
			for (int i = 0; i < 16; ++i) {
				const float side = i % 2 == 0 ? 1.0f : -1.0f;
				const float spread = side * random.Range(0.45f, 0.95f);
				const float length = a_size * random.Range(0.7f, 1.25f) * (1.0f - std::abs(spread) * 0.25f);
				const ImVec2 base(a_corner.x + std::cos(a_angle + spread) * a_size * 0.12f, a_corner.y + std::sin(a_angle + spread) * a_size * 0.12f);
				// Back leaves darker, for depth
				const float depth = i < 8 ? 0.6f : 1.0f;
				Leaf(a_drawList, base, length * (i < 8 ? 1.15f : 1.0f), a_angle + spread, Shade(LeafColour(random, 1.0f), depth), true);
			}
			// Berries
			for (int i = 0; i < 2; ++i) {
				const float angle = a_angle + random.Range(-0.6f, 0.6f);
				const float reach = a_size * random.Range(0.18f, 0.35f);
				const ImVec2 centre(a_corner.x + std::cos(angle) * reach, a_corner.y + std::sin(angle) * reach);
				a_drawList->AddCircleFilled(centre, a_size * 0.07f, IM_COL32(96, 14, 20, 255));
				a_drawList->AddCircleFilled(ImVec2(centre.x - a_size * 0.022f, centre.y - a_size * 0.022f), a_size * 0.022f, IM_COL32(190, 90, 90, 110));
			}
		}
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Leaf ) --------------------------
	//---------------------------------------------------

	// Almond leaf from its stalk, a_angle is the way the tip points
	void Leaf(ImDrawList* a_drawList, ImVec2 a_base, float a_length, float a_angle, ImU32 a_colour, bool a_filled, bool a_vein) {

		constexpr int steps = 8;
		const float   dx = std::cos(a_angle);
		const float   dy = std::sin(a_angle);
		const float   width = a_length * 0.34f;
		const auto    at = [&](float a_along, float a_side) {
			return ImVec2(a_base.x + dx * a_along - dy * a_side, a_base.y + dy * a_along + dx * a_side);
		};

		ImVec2 points[steps * 2];
		for (int i = 0; i < steps; ++i) {
			const float t = static_cast<float>(i + 1) / (steps + 1);
			const float half = width * 0.5f * std::sin(std::numbers::pi_v<float> * std::pow(t, 0.8f));
			points[i] = at(a_length * t, half);
			points[steps * 2 - 1 - i] = at(a_length * t, -half);
		}
		ImVec2 outline[steps * 2 + 2];
		outline[0] = a_base;
		std::copy(std::begin(points), std::begin(points) + steps, outline + 1);
		outline[steps + 1] = at(a_length, 0.0f);
		std::copy(std::begin(points) + steps, std::end(points), outline + steps + 2);

		const float stroke = std::max(1.0f, a_length * 0.07f);
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(outline, steps * 2 + 2, a_colour);
		} else {
			a_drawList->AddPolyline(outline, steps * 2 + 2, a_colour, ImDrawFlags_Closed, stroke);
		}
		if (a_vein) {
			const ImU32 vein = a_filled ? Shade(a_colour, 0.55f) : a_colour;
			a_drawList->AddLine(a_base, at(a_length * 0.82f, 0.0f), vein, std::max(1.0f, stroke * 0.7f));
		}
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Sprig ) -------------------------
	//---------------------------------------------------

	// Three leaves from one stalk
	void Sprig(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, float a_angle, ImU32 a_colour) {

		const ImVec2 base(a_centre.x - std::cos(a_angle) * a_radius * 0.8f, a_centre.y - std::sin(a_angle) * a_radius * 0.8f);
		Leaf(a_drawList, base, a_radius * 1.3f, a_angle - 0.95f, Shade(a_colour, 0.8f), true, a_radius > 5.0f);
		Leaf(a_drawList, base, a_radius * 1.3f, a_angle + 0.95f, Shade(a_colour, 0.8f), true, a_radius > 5.0f);
		Leaf(a_drawList, base, a_radius * 1.9f, a_angle, a_colour, true, a_radius > 5.0f);
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Vine ) --------------------------
	//---------------------------------------------------

	// Wavy stem with leaves either side, thinning out at the far end
	void Vine(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_stem, ImU32 a_leaves, float a_size, unsigned a_seed) {

		const float length = std::hypot(a_to.x - a_from.x, a_to.y - a_from.y);
		if (length < a_size) {
			return;
		}
		const float dx = (a_to.x - a_from.x) / length;
		const float dy = (a_to.y - a_from.y) / length;
		const float wave = a_size * 0.16f;
		const float period = a_size * 3.2f;
		const auto  at = [&](float a_along) {
			const float side = std::sin(a_along / period * 2.0f * std::numbers::pi_v<float>) * wave;
			return ImVec2(a_from.x + dx * a_along - dy * side, a_from.y + dy * a_along + dx * side);
		};

		// Stem, fading at the end
		const int   segments = std::max(2, static_cast<int>(length / (a_size * 0.3f)));
		const float stroke = std::max(1.0f, a_size * 0.09f);
		for (int i = 0; i < segments; ++i) {
			const float t = static_cast<float>(i) / segments;
			const float fade = 1.0f - std::pow(t, 2.0f);
			a_drawList->AddLine(at(length * t), at(length * (t + 1.0f / segments)), WithAlpha(a_stem, fade), stroke * (0.6f + 0.4f * fade));
		}

		// Thorns along the stem
		const float thorn = a_size * 0.16f;
		for (float along = a_size * 0.6f; along < length * 0.85f; along += a_size * 0.75f) {
			const float t = along / length;
			const float side = std::fmod(along, a_size * 1.5f) < a_size * 0.75f ? 1.0f : -1.0f;
			const auto  base = at(along);
			const ImVec2 tip(base.x - dy * side * thorn + dx * thorn * 0.6f, base.y + dx * side * thorn + dy * thorn * 0.6f);
			a_drawList->AddTriangleFilled(ImVec2(base.x - dx * thorn * 0.35f, base.y - dy * thorn * 0.35f), ImVec2(base.x + dx * thorn * 0.35f, base.y + dy * thorn * 0.35f), tip, WithAlpha(Shade(a_stem, 1.3f), 1.0f - t));
		}

		// Leaves on alternate sides, with a tendril now and then
		Random      random{ a_seed * 2654435761u + 1u };
		const float spacing = a_size * 1.15f;
		float       side = 1.0f;
		const float angle = std::atan2(dy, dx);
		for (float along = a_size * 0.4f; along < length - a_size * 0.3f; along += spacing * random.Range(0.8f, 1.2f)) {
			const float t = along / length;
			const float grow = std::max(0.25f, 1.0f - t * 0.75f);
			const auto  base = at(along);
			auto        colour = random.Next() < 0.15f ? LeafColour(random, 1.0f) : Shade(a_leaves, random.Range(0.55f, 0.9f));
			colour = WithAlpha(colour, std::min(1.0f, 1.25f - t));
			Leaf(a_drawList, base, a_size * random.Range(0.75f, 1.05f) * grow, angle + side * random.Range(0.75f, 1.05f), colour, true, a_size > 6.0f);
			if (random.Next() < 0.2f) {
				const float curl = a_size * 0.28f * grow;
				const ImVec2 out(base.x - dy * side * curl * 1.4f, base.y + dx * side * curl * 1.4f);
				a_drawList->PathArcTo(out, curl, angle - side * 1.6f, angle + side * 2.6f, 10);
				a_drawList->PathStroke(WithAlpha(a_stem, 1.0f - t), ImDrawFlags_None, std::max(1.0f, stroke * 0.6f));
			}
			side = -side;
		}
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Foliage ) -----------------------
	//---------------------------------------------------

	// Forest floor backdrop: light through the canopy, leaves thick at the edges
	void Foliage(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		a_drawList->PushClipRect(a_min, a_max, true);

		// Faint cold moonlight from above
		Random rays{ 0x5A11u };
		for (int i = 0; i < 3; ++i) {
			const float x = a_min.x + width * rays.Range(0.15f, 0.7f);
			const float top = width * rays.Range(0.02f, 0.05f);
			const float bottom = top * rays.Range(2.0f, 3.0f);
			const float drift = height * 0.35f;
			const ImU32 strong = IM_COL32(150, 180, 175, static_cast<int>(rays.Range(4.0f, 7.0f)));
			const ImU32 clear = IM_COL32(150, 180, 175, 0);
			const ImVec2 points[4] = { ImVec2(x, a_min.y), ImVec2(x + top, a_min.y), ImVec2(x + top + drift + bottom, a_max.y), ImVec2(x + drift, a_max.y) };
			GradientQuad(a_drawList, points, { strong, strong, clear, clear });
		}

		// Mist pooling on the forest floor
		const ImU32 mist = IM_COL32(120, 150, 140, 22);
		const ImU32 mistClear = IM_COL32(120, 150, 140, 0);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - height * 0.3f), a_max, mistClear, mistClear, mist, mist);
		Random wisps{ 0xD4BB1Eu };
		for (int i = 0; i < 7; ++i) {
			const ImVec2 centre(a_min.x + width * wisps.Range(0.05f, 0.95f), a_max.y - height * wisps.Range(0.02f, 0.22f));
			SoftSpot(a_drawList, centre, size * wisps.Range(4.0f, 9.0f), IM_COL32(120, 150, 140, 3));
		}

		// Leaf litter, thicker towards the edges
		Random     random{ 0x1EAFu };
		const int  tries = std::clamp(static_cast<int>(width * height / (size * size * 1.6f)), 40, 1400);
		const auto closeness = [&](float a_value) { return std::pow(1.0f - std::min(1.0f, a_value), 3.0f); };
		for (int i = 0; i < tries; ++i) {
			const float u = random.Next();
			const float v = random.Next();
			const float keep = random.Next();
			const float angle = random.Range(0.0f, 2.0f * std::numbers::pi_v<float>);
			const float scale = random.Range(0.7f, 2.4f);
			const float shade = random.Range(0.14f, 0.34f);
			const float edge = std::max({ closeness(u * 5.0f), closeness((1.0f - u) * 5.0f), closeness(v * 4.0f), closeness((1.0f - v) * 3.0f) });
			const float chance = 0.04f + edge * 0.9f;
			if (keep > chance) {
				continue;
			}
			const ImVec2 base(a_min.x + u * width, a_min.y + v * height);
			const auto   colour = Shade(LeafColour(random, random.Range(0.35f, 0.75f)), shade + edge * 0.25f);
			Leaf(a_drawList, base, size * scale, angle, colour, true, scale > 1.4f);
		}

		// Shadow under the canopy at the edges
		const float  fade = std::min(width, height) * 0.16f;
		const ImU32  dark = IM_COL32(2, 4, 2, 210);
		const ImU32  clear = IM_COL32(2, 4, 2, 0);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + fade), dark, dark, clear, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - fade), a_max, clear, clear, dark, dark);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + fade, a_max.y), dark, clear, clear, dark);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_max.x - fade, a_min.y), a_max, clear, dark, dark, clear);

		a_drawList->PopClipRect();
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Diamond ) -----------------------
	//---------------------------------------------------

	// In place of the diamond: a leaf, tip to the top right
	void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		constexpr float angle = -std::numbers::pi_v<float> * 0.25f;
		const float     reach = a_radius * 1.15f;
		Leaf(a_drawList, ImVec2(a_centre.x - std::cos(angle) * reach, a_centre.y - std::sin(angle) * reach), reach * 2.0f, angle, a_colour, a_filled, a_radius > 4.0f);
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// Leaf outline, filling in when found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		constexpr float angle = -std::numbers::pi_v<float> * 0.25f;
		const float     length = a_size * 0.86f;
		const ImVec2    base(a_centre.x - std::cos(angle) * length * 0.5f, a_centre.y - std::sin(angle) * length * 0.5f);
		Leaf(a_drawList, base, length, angle, a_colour, false, false);
		if (a_fill > 0.0f) {
			const float  inner = length * (0.35f + 0.65f * a_fill);
			const ImVec2 start(a_centre.x - std::cos(angle) * inner * 0.5f, a_centre.y - std::sin(angle) * inner * 0.5f);
			Leaf(a_drawList, start, inner, angle, a_colour, true, false);
		}
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.6f;
		const ImU32 leaves = ImGui::GetColorU32(Themes::GetPalette().accent);
		Vine(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, leaves, gap * 0.9f, 11u);
		Vine(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, leaves, gap * 0.9f, 23u);
		Sprig(a_drawList, ImVec2(midX, a_from.y), gap * 0.5f, -std::numbers::pi_v<float> * 0.5f, leaves);
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Frame ) -------------------------
	//---------------------------------------------------

	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		// Bark frame over the forest floor, ivy and leaves at the corners
		const float size = ImGui::GetFontSize();
		const ImU32 leaves = ImGui::GetColorU32(Themes::GetPalette().accent);
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		// Smaller panels, like the cell scanner's, get lighter trim
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float bark = size * 0.4f * std::max(0.6f, compact);
		Foliage(a_drawList, a_min, a_max);
		Bark(a_drawList, a_min, a_max, bark);
		Moss(a_drawList, a_min, a_max, bark);
		const float edge = bark + 1.0f;
		a_drawList->AddRect(ImVec2(a_min.x + edge, a_min.y + edge), ImVec2(a_max.x - edge, a_max.y - edge), WithAlpha(leaves, 0.35f), 0.0f, 0, 1.0f);

		const float run = std::min(width, height) * 0.22f;
		const float mid = edge * 0.5f;
		constexpr float pi = std::numbers::pi_v<float>;
		Vine(a_drawList, ImVec2(a_min.x + mid, a_min.y + mid), ImVec2(a_min.x + run * 1.6f, a_min.y + mid), IM_COL32(70, 56, 36, 255), leaves, size * 0.62f, 101u);
		Vine(a_drawList, ImVec2(a_max.x - mid, a_min.y + mid), ImVec2(a_max.x - run * 1.6f, a_min.y + mid), IM_COL32(70, 56, 36, 255), leaves, size * 0.62f, 202u);
		Vine(a_drawList, ImVec2(a_min.x + mid, a_min.y + mid), ImVec2(a_min.x + mid, a_min.y + run), IM_COL32(70, 56, 36, 255), leaves, size * 0.55f, 303u);
		Vine(a_drawList, ImVec2(a_max.x - mid, a_min.y + mid), ImVec2(a_max.x - mid, a_min.y + run), IM_COL32(70, 56, 36, 255), leaves, size * 0.55f, 404u);
		Vine(a_drawList, ImVec2(a_min.x + mid, a_max.y - mid), ImVec2(a_min.x + run, a_max.y - mid), IM_COL32(70, 56, 36, 255), leaves, size * 0.5f, 505u);
		Vine(a_drawList, ImVec2(a_max.x - mid, a_max.y - mid), ImVec2(a_max.x - run, a_max.y - mid), IM_COL32(70, 56, 36, 255), leaves, size * 0.5f, 606u);

		const float cluster = size * 1.8f * compact;
		LeafCluster(a_drawList, ImVec2(a_min.x + mid, a_min.y + mid), cluster, pi * 0.25f, 0xA11CEu);
		LeafCluster(a_drawList, ImVec2(a_max.x - mid, a_min.y + mid), cluster, pi * 0.75f, 0xB0B5u);
		LeafCluster(a_drawList, ImVec2(a_max.x - mid, a_max.y - mid), cluster * 0.85f, -pi * 0.75f, 0xC0FFEEu);
		LeafCluster(a_drawList, ImVec2(a_min.x + mid, a_max.y - mid), cluster * 0.85f, -pi * 0.25f, 0xD00Du);
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: wood grain at the edges, leaves caught in the corners, mist below, a leaf in a ring of thorns
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		constexpr float pi = std::numbers::pi_v<float>;
		const float     band = std::min(width, height) * 0.3f + size * 2.0f;
		const ImU32     accent = ImGui::GetColorU32(Themes::GetPalette().accent);

		// Mist along the bottom
		const float glow = std::min(height * 0.3f, size * 5.0f);
		const ImU32 mist = IM_COL32(120, 150, 140, 255);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(mist, 0.0f), WithAlpha(mist, 0.0f), WithAlpha(mist, 0.07f), WithAlpha(mist, 0.07f));

		// Wood grain running across, fading towards the middle
		Random random{ static_cast<std::uint32_t>(width) * 19u + 11u };
		for (float y = a_min.y + size * 0.3f; y < a_max.y; y += size * 0.32f) {
			const float phase = random.Range(0.0f, 2.0f * pi);
			const float bend = size * random.Range(0.05f, 0.2f);
			ImVec2      previous(a_min.x, y + std::sin(phase) * bend);
			for (float x = a_min.x + size * 0.6f; x <= a_max.x + 0.5f; x += size * 0.6f) {
				const ImVec2 point(std::min(x, a_max.x), y + std::sin(phase + (x - a_min.x) / (size * 6.0f)) * bend);
				const float  weight = EdgeWeight(ImVec2((previous.x + point.x) * 0.5f, y), a_min, a_max, band);
				if (weight > 0.03f) {
					a_drawList->AddLine(previous, point, WithAlpha(IM_COL32(110, 84, 54, 255), 0.12f * weight), 1.0f);
				}
				previous = point;
			}
		}

		// A few leaves fallen into the corners
		for (const auto& corner : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = corner.x == a_min.x ? 1.0f : -1.0f;
			const float sy = corner.y == a_min.y ? 1.0f : -1.0f;
			for (int i = 0; i < 3; ++i) {
				const ImVec2 base(corner.x + sx * size * random.Range(0.4f, 2.4f), corner.y + sy * size * random.Range(0.4f, 1.6f));
				Leaf(a_drawList, base, size * random.Range(0.8f, 1.4f), random.Range(0.0f, 2.0f * pi), Shade(LeafColour(random, 0.35f), 0.6f), true, false);
			}
		}

		// A leaf in a ring of thorns, large and faint, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const ImU32  faint = WithAlpha(accent, 0.1f);
			a_drawList->AddCircle(centre, radius * 1.05f, faint, 64, std::max(1.0f, radius * 0.03f));
			for (int i = 0; i < 18; ++i) {
				const float  angle = 2.0f * pi * static_cast<float>(i) / 18.0f;
				const ImVec2 base(centre.x + std::cos(angle) * radius * 1.05f, centre.y + std::sin(angle) * radius * 1.05f);
				const ImVec2 tip(centre.x + std::cos(angle + 0.12f) * radius * 1.2f, centre.y + std::sin(angle + 0.12f) * radius * 1.2f);
				const ImVec2 back(centre.x + std::cos(angle - 0.08f) * radius * 1.05f, centre.y + std::sin(angle - 0.08f) * radius * 1.05f);
				a_drawList->AddTriangleFilled(back, base, tip, faint);
			}
			Leaf(a_drawList, ImVec2(centre.x - radius * 0.55f, centre.y + radius * 0.55f), radius * 1.55f, -pi * 0.25f, WithAlpha(accent, 0.08f), true, true);
		}
	}

	//---------------------------------------------------
	//-- Bosmer Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Twigs bent round the corners, a leaf at each
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(2.0f, size * 0.16f);
		const ImU32 twig = IM_COL32(70, 54, 36, 255);
		const ImU32 accent = ImGui::GetColorU32(Themes::GetPalette().accent);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			a_drawList->PathLineTo(ImVec2(corner.x + sx * arm, corner.y + sy * thick * 0.3f));
			a_drawList->PathBezierQuadraticCurveTo(corner, ImVec2(corner.x + sx * thick * 0.3f, corner.y + sy * arm), 8);
			a_drawList->PathStroke(twig, ImDrawFlags_None, thick);
			const float angle = std::atan2(sy, sx);
			Leaf(a_drawList, ImVec2(corner.x + sx * arm * 0.85f, corner.y), size * 0.55f, angle - sx * sy * 0.6f, Shade(accent, 0.8f), true, false);
			Leaf(a_drawList, ImVec2(corner.x, corner.y + sy * arm * 0.85f), size * 0.55f, angle + sx * sy * 0.6f, Shade(accent, 0.7f), true, false);
		}
	}
}
