#include "UI/Themes/ImperialDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::ImperialDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// White marble and its veins, polished bronze, Legion crimson
		constexpr ImU32 marble = IM_COL32(210, 206, 198, 255);
		constexpr ImU32 marbleShade = IM_COL32(160, 156, 150, 255);
		constexpr ImU32 vein = IM_COL32(116, 114, 112, 255);
		constexpr ImU32 bronze = IM_COL32(178, 124, 70, 255);
		constexpr ImU32 bronzeLight = IM_COL32(234, 188, 128, 255);
		constexpr ImU32 bronzeDeep = IM_COL32(104, 68, 36, 255);
		constexpr ImU32 crimson = IM_COL32(150, 32, 38, 255);
		constexpr ImU32 darkMarble = IM_COL32(28, 26, 26, 255);
		constexpr ImU32 daylight = IM_COL32(255, 236, 200, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Fade At ) ---------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Leaf ) ------------------------
		//---------------------------------------------------

		// One laurel leaf, pointing along a_angle from its stem
		void Leaf(ImDrawList* a_drawList, ImVec2 a_stem, float a_length, float a_angle, ImU32 a_colour) {

			const ImVec2 centre(a_stem.x + std::cos(a_angle) * a_length * 0.5f, a_stem.y + std::sin(a_angle) * a_length * 0.5f);
			a_drawList->AddEllipseFilled(centre, ImVec2(a_length * 0.5f, a_length * 0.2f), a_colour, a_angle, 10);
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Veins ) -----------------------
		//---------------------------------------------------

		// A vein wandering through marble
		void Vein(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_step, int a_count, Random& a_random, ImU32 a_colour, float a_wander) {

			std::vector<ImVec2> points{ a_from };
			ImVec2              at = a_from;
			for (int i = 0; i < a_count; ++i) {
				at = ImVec2(at.x + a_step.x + a_random.Range(-a_wander, a_wander), at.y + a_step.y + a_random.Range(-a_wander, a_wander));
				points.push_back(at);
			}
			a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), a_colour, ImDrawFlags_None, 1.0f);
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Marble Wall ) -----------------
		//---------------------------------------------------

		// Backdrop: dark polished marble, pale veins running through it, daylight from above
		void MarbleWall(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(46, 42, 40, 70), IM_COL32(46, 42, 40, 70), IM_COL32(12, 11, 11, 70), IM_COL32(12, 11, 11, 70) });

			// Slabs, and the veins across them
			Random      random{ 0x1A9Eu };
			const float slab = size * 9.0f;
			for (float x = a_min.x + slab; x < a_max.x; x += slab) {
				a_drawList->AddLine(ImVec2(x, a_min.y), ImVec2(x, a_max.y), WithAlpha(IM_COL32(0, 0, 0, 255), 0.35f), 1.5f);
				a_drawList->AddLine(ImVec2(x + 1.5f, a_min.y), ImVec2(x + 1.5f, a_max.y), WithAlpha(marble, 0.035f), 1.0f);
			}
			const int veins = std::clamp(static_cast<int>(width * height / (size * size * 40.0f)), 6, 60);
			for (int i = 0; i < veins; ++i) {
				const ImVec2 from(a_min.x + width * random.Next(), a_min.y + height * random.Next() - size * 4.0f);
				Vein(a_drawList, from, ImVec2(size * random.Range(0.4f, 1.0f), size * random.Range(0.6f, 1.2f)), 10, random, WithAlpha(marble, random.Range(0.03f, 0.07f)), size * 0.5f);
			}

			// Daylight falling from high windows
			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_min.y - size * 2.0f), std::max(width * 0.4f, size * 10.0f), WithAlpha(daylight, 0.01f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 4, 4, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Marble Band ) -----------------
		//---------------------------------------------------

		// One side of the frame: veined marble blocks between strips of bronze, a bronze stud on each joint
		void MarbleBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};

			const float trim = std::max(1.5f, depth * 0.13f);
			const float inner = depth - trim;
			Random      random{ a_seed };
			a_drawList->AddRectFilled(a_min, a_max, marbleShade);
			a_drawList->PushClipRect(ImVec2(std::min(at(0.0f, trim).x, at(length, inner).x), std::min(at(0.0f, trim).y, at(length, inner).y)),
				ImVec2(std::max(at(0.0f, trim).x, at(length, inner).x), std::max(at(0.0f, trim).y, at(length, inner).y)), true);
			float along = 0.0f;
			while (along < length) {
				const float block = depth * random.Range(5.0f, 7.0f);
				const float end = std::min(length, along + block);
				a_drawList->AddRectFilled(at(along + 1.0f, trim), at(end - 1.0f, inner), Shade(marble, random.Range(0.92f, 1.02f)));
				// Two or three veins running at a slant
				for (int v = 0; v < 3; ++v) {
					const float start = along + (end - along) * random.Range(0.0f, 0.8f);
					std::vector<ImVec2> points;
					float               x = start;
					for (float across = trim; across <= inner + 1.0f; across += (inner - trim) / 4.0f) {
						points.push_back(at(x, across));
						x += depth * random.Range(0.1f, 0.6f);
					}
					a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), WithAlpha(vein, random.Range(0.35f, 0.7f)), ImDrawFlags_None, 1.0f);
				}
				a_drawList->AddLine(at(along + 1.0f, trim + 0.5f), at(end - 1.0f, trim + 0.5f), WithAlpha(IM_COL32(255, 255, 255, 255), 0.5f), 1.0f);
				along = end;
				if (along < length) {
					a_drawList->AddLine(at(along, trim), at(along, inner), WithAlpha(vein, 0.9f), 1.0f);
				}
			}
			a_drawList->PopClipRect();

			for (const auto& [from, to] : { std::pair{ 0.0f, trim }, std::pair{ inner, depth } }) {
				a_drawList->AddRectFilled(at(0.0f, from), at(length, to), bronze);
				a_drawList->AddLine(at(0.0f, from + 0.5f), at(length, from + 0.5f), WithAlpha(bronzeLight, 0.8f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Capital ) ---------------------
		//---------------------------------------------------

		// A bronze capital at each corner, its two volutes curling out
		void Capital(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, bronze);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), bronzeLight, 1.5f);
			a_drawList->AddLine(ImVec2(min.x + 1.0f, min.y), ImVec2(min.x + 1.0f, max.y), WithAlpha(bronzeLight, 0.7f), 1.0f);
			const float inset = a_size * 0.2f;
			a_drawList->AddRectFilled(ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), crimson);
			// Volutes in the upper corners of the crimson
			for (const float side : { -1.0f, 1.0f }) {
				const ImVec2 eye(a_centre.x + side * a_size * 0.38f, a_centre.y - a_size * 0.15f);
				for (int turn = 0; turn < 10; ++turn) {
					const float t = static_cast<float>(turn) / 9.0f;
					const float radius = a_size * 0.34f * (1.0f - t * 0.75f);
					const float angle = side * t * pi * 2.2f - pi * 0.5f;
					a_drawList->PathLineTo(ImVec2(eye.x + std::cos(angle) * radius, eye.y + std::sin(angle) * radius));
				}
				a_drawList->PathStroke(bronzeLight, ImDrawFlags_None, std::max(1.0f, a_size * 0.1f));
			}
			a_drawList->AddLine(ImVec2(min.x + inset, a_centre.y + a_size * 0.35f), ImVec2(max.x - inset, a_centre.y + a_size * 0.35f), bronzeLight, std::max(1.0f, a_size * 0.08f));
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Crest ) -----------------------
		//---------------------------------------------------

		// A crimson seal ringed in bronze, a laurel wreath round it
		void Crest(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

			Wreath(a_drawList, a_centre, a_radius, bronzeLight);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.62f, bronze, 24);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.5f, crimson, 24);
			a_drawList->AddCircle(a_centre, a_radius * 0.62f, bronzeLight, 24, 1.0f);
			Stud(a_drawList, a_centre, a_radius * 0.24f, bronzeLight, true);
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Stud ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: a squared bronze stud, lit on its upper faces
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

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
		a_drawList->AddCircleFilled(a_centre, a_radius * 0.3f, WithAlpha(IM_COL32(255, 244, 220, 255), 0.4f * Alpha(a_colour)), 8);
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Wreath ) ----------------------
	//---------------------------------------------------

	// A laurel wreath: two branches of leaves meeting at the top, open at the bottom
	void Wreath(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const float leaf = a_radius * 0.42f;
		for (const float side : { -1.0f, 1.0f }) {
			for (int i = 0; i < 6; ++i) {
				const float  t = static_cast<float>(i) / 5.0f;
				const float  angle = pi * 0.5f + side * (pi * 0.2f + t * pi * 0.68f);
				const ImVec2 stem(a_centre.x + std::cos(angle) * a_radius * 0.8f, a_centre.y + std::sin(angle) * a_radius * 0.8f);
				const float  along = angle + side * pi * 0.5f;	// the way the branch grows
				Leaf(a_drawList, stem, leaf, along - side * 0.5f, a_colour);
				Leaf(a_drawList, stem, leaf * 0.85f, along + side * 0.9f, a_colour);
			}
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Marker ) ----------------------
	//---------------------------------------------------

	// Needed / found: the Legion's tall rounded shield, its boss and the bar across it; filled crimson when found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  half = a_size * 0.25f;
		const float  height = a_size * 0.37f;
		const ImVec2 min(a_centre.x - half, a_centre.y - height);
		const ImVec2 max(a_centre.x + half, a_centre.y + height);
		const float  rounding = half * 0.55f;
		const float  stroke = std::max(1.0f, a_size * 0.07f);
		if (a_fill > 0.0f) {
			a_drawList->AddRectFilled(min, max, WithAlpha(a_colour, a_fill), rounding);
		}
		a_drawList->AddRect(min, max, a_colour, rounding, 0, stroke);
		// Filled, the bar and boss are cut dark into it; empty, they're drawn in its colour
		const ImU32 detail = a_fill > 0.0f ? WithAlpha(darkMarble, 0.3f + 0.6f * a_fill) : a_colour;
		a_drawList->AddLine(ImVec2(min.x + stroke, a_centre.y), ImVec2(max.x - stroke, a_centre.y), detail, stroke);
		a_drawList->AddCircleFilled(a_centre, half * 0.42f, detail, 12);
		if (a_fill > 0.0f) {
			a_drawList->AddCircleFilled(a_centre, half * 0.22f, WithAlpha(a_colour, a_fill), 10);
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Garland ) ---------------------
	//---------------------------------------------------

	// In place of a rule: a laurel garland, pairs of leaves along a stem, fading at the ends
	void Garland(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float leaf = a_size * 0.42f;
		const float unit = a_size * 0.55f;
		const float stroke = std::max(1.0f, a_size * 0.06f);
		for (float x = a_from.x; x < a_to.x; x += unit * 0.5f) {
			const float end = std::min(a_to.x, x + unit * 0.5f);
			a_drawList->AddLine(ImVec2(x, a_from.y), ImVec2(end, a_from.y), WithAlpha(a_colour, FadeAt((x + end) * 0.5f, a_from.x, length) * 0.8f), stroke);
		}
		// Single leaves, above and below the stem in turn, all growing towards the middle
		const float middle = (a_from.x + a_to.x) * 0.5f;
		const float spacing = unit * 0.5f;
		const int   count = static_cast<int>(length / spacing);
		const float start = a_from.x + (length - static_cast<float>(count) * spacing) * 0.5f;
		for (int i = 0; i <= count; ++i) {
			const float x = start + static_cast<float>(i) * spacing;
			const float fade = FadeAt(x, a_from.x, length);
			if (fade < 0.05f) {
				continue;
			}
			const float grow = x < middle ? 0.0f : pi;
			const float side = i % 2 == 0 ? -1.0f : 1.0f;
			Leaf(a_drawList, ImVec2(x, a_from.y), leaf, grow + side * (x < middle ? 0.5f : -0.5f), WithAlpha(a_colour, fade));
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Garland(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Garland(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Wreath(a_drawList, ImVec2(midX, a_from.y), gap * 0.5f, a_colour);
		a_drawList->AddCircleFilled(ImVec2(midX, a_from.y), gap * 0.14f, a_colour, 10);
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// Veined marble bound in bronze, bronze capitals on the corners, a wreathed crimson seal on the top beam
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		MarbleWall(a_drawList, a_min, a_max);

		MarbleBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0x1E01u);
		MarbleBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0x1E02u);
		MarbleBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0x1E03u);
		MarbleBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0x1E04u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(bronze, 0.3f), 0.0f, 0, 1.0f);

		const float corner = band * 0.95f;
		Capital(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		Capital(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		Capital(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		Capital(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);

		// Only on the main window, inside its top edge so none of it is clipped
		if (compact >= 0.8f) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			Crest(a_drawList, ImVec2(midX, a_min.y + band * 1.05f + 2.0f), band * 1.05f);
			Capital(a_drawList, ImVec2(midX, a_max.y - band * 0.5f), corner);
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: marble veins at the edges clearing towards the text, daylight from above, a laurel wreath in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(daylight, 0.04f), WithAlpha(daylight, 0.04f), WithAlpha(daylight, 0.0f), WithAlpha(daylight, 0.0f));

		// Veins, drawn in short lengths so each fades by how near the edge it is
		Random    random{ static_cast<std::uint32_t>(width) * 29u + static_cast<std::uint32_t>(height) };
		const int veins = std::clamp(static_cast<int>(width * height / (size * size * 8.0f)), 6, 120);
		a_drawList->PushClipRect(a_min, a_max, true);
		for (int i = 0; i < veins; ++i) {
			ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			for (int step = 0; step < 8; ++step) {
				const ImVec2 next(at.x + size * random.Range(0.1f, 0.9f), at.y + size * random.Range(0.3f, 1.0f));
				const float  weight = EdgeWeight(ImVec2((at.x + next.x) * 0.5f, (at.y + next.y) * 0.5f), a_min, a_max, band);
				if (weight > 0.03f) {
					a_drawList->AddLine(at, next, WithAlpha(marble, weight * 0.12f), 1.0f);
				}
				at = next;
			}
		}
		a_drawList->PopClipRect();

		// A laurel wreath, large and faint, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.4f, width * 0.15f);
			const ImVec2 centre(a_max.x - radius * 1.3f, a_max.y - radius * 1.1f);
			Wreath(a_drawList, centre, radius, WithAlpha(bronzeLight, 0.09f));
			a_drawList->AddCircle(centre, radius * 0.45f, WithAlpha(bronzeLight, 0.08f), 32, std::max(2.0f, radius * 0.05f));
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// Bronze brackets on the panel's corners, a crimson stud on the point
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.18f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, bronze, ImDrawFlags_None, thick);
			a_drawList->AddLine(ImVec2(corner.x, corner.y - sy * thick * 0.35f), ImVec2(corner.x + sx * arm, corner.y - sy * thick * 0.35f), WithAlpha(bronzeLight, 0.7f), 1.0f);
			for (const auto& end : { points[0], points[2] }) {
				a_drawList->AddCircleFilled(end, thick * 0.55f, bronzeLight, 8);
			}
			const ImVec2 stud(corner.x + sx * thick * 1.4f, corner.y + sy * thick * 1.4f);
			Stud(a_drawList, stud, thick * 1.1f, crimson, true);
			Stud(a_drawList, stud, thick * 1.1f, bronzeLight, false);
		}
	}
}

namespace CompletionistUI::ImperialDecor
{
	//---------------------------------------------------
	//-- Imperial Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Legion shields: filled when taken, an empty shield when next, a bronze stud when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Marker(a_drawList, a_centre, a_radius * 2.8f, a_colour, 1.0f);
			return;
		case PerkState::kNext:
			{
				const float half = a_radius * 2.7f * 0.25f;
				const float height = a_radius * 2.7f * 0.37f;
				a_drawList->AddRectFilled(ImVec2(a_centre.x - half, a_centre.y - height), ImVec2(a_centre.x + half, a_centre.y + height), darkMarble, half * 0.55f);
				Marker(a_drawList, a_centre, a_radius * 2.7f, a_colour, 0.0f);
			}
			return;
		default:
			Stud(a_drawList, a_centre, a_radius * 0.42f, darkMarble, true);
			Stud(a_drawList, a_centre, a_radius * 0.42f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// A bronze rail between the perks, laurel leaves along it where they're taken
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, 3.0f);
		if (!a_lit) {
			a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, std::max(1.5f, a_radius * 0.14f));
		const float leaf = std::max(4.0f, a_radius * 0.55f);
		float       travelled = 0.0f;
		float       next = leaf * 1.5f;
		for (std::size_t i = 1; i < curve.size(); ++i) {
			travelled += std::hypot(curve[i].x - curve[i - 1].x, curve[i].y - curve[i - 1].y);
			if (travelled >= next && i + 2 < curve.size()) {
				const float angle = std::atan2(curve[i].y - curve[i - 1].y, curve[i].x - curve[i - 1].x);
				Leaf(a_drawList, curve[i], leaf, angle - 2.4f, a_colour);
				Leaf(a_drawList, curve[i], leaf, angle + 2.4f, a_colour);
				next += leaf * 1.6f;
			}
		}
	}
}
