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
		//-- Imperial Decor ( Tesserae ) --------------------
		//---------------------------------------------------

		// A strip of mosaic tiles, a_rows deep, laid along one edge; a_ring picks out one row in crimson
		void Tesserae(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_along, ImVec2 a_inward, float a_length, int a_rows, float a_tile, int a_ring, Random& a_random) {

			const int count = static_cast<int>(a_length / a_tile);
			for (int row = 0; row < a_rows; ++row) {
				for (int i = 0; i < count; ++i) {
					const float  along = (static_cast<float>(i) + 0.5f) * a_tile + a_random.Range(-0.08f, 0.08f) * a_tile;
					const float  across = (static_cast<float>(row) + 0.5f) * a_tile + a_random.Range(-0.08f, 0.08f) * a_tile;
					const ImVec2 centre(a_from.x + a_along.x * along + a_inward.x * across, a_from.y + a_along.y * along + a_inward.y * across);
					const float  half = a_tile * 0.4f;
					const ImU32  colour = row == a_ring ? WithAlpha(crimson, a_random.Range(0.35f, 0.5f)) : WithAlpha(marble, a_random.Range(0.05f, 0.1f));
					a_drawList->AddRectFilled(ImVec2(centre.x - half, centre.y - half), ImVec2(centre.x + half, centre.y + half), colour);
				}
			}
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Mosaic ) ----------------------
		//---------------------------------------------------

		// Backdrop: a dark floor laid with a mosaic border of small tiles, a crimson row running through it
		void Mosaic(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_inset) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(46, 42, 40, 70), IM_COL32(46, 42, 40, 70), IM_COL32(12, 11, 11, 70), IM_COL32(12, 11, 11, 70) });

			Random      random{ 0x1A9Eu };
			const float tile = std::max(3.0f, size * 0.32f);
			const ImVec2 min(a_min.x + a_inset, a_min.y + a_inset);
			const ImVec2 max(a_max.x - a_inset, a_max.y - a_inset);
			const float  innerWidth = max.x - min.x;
			const float  innerHeight = max.y - min.y;
			Tesserae(a_drawList, min, ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f), innerWidth, 3, tile, 1, random);
			Tesserae(a_drawList, ImVec2(min.x, max.y), ImVec2(1.0f, 0.0f), ImVec2(0.0f, -1.0f), innerWidth, 3, tile, 1, random);
			Tesserae(a_drawList, ImVec2(min.x, min.y + tile * 3.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), innerHeight - tile * 6.0f, 3, tile, 1, random);
			Tesserae(a_drawList, ImVec2(max.x, min.y + tile * 3.0f), ImVec2(0.0f, 1.0f), ImVec2(-1.0f, 0.0f), innerHeight - tile * 6.0f, 3, tile, 1, random);

			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_min.y - size * 2.0f), std::max(width * 0.4f, size * 10.0f), WithAlpha(daylight, 0.01f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 4, 4, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Column ) ----------------------
		//---------------------------------------------------

		// A fluted marble column shaft, lit down its left side
		void Column(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const ImU32 light = marble;
			const ImU32 shade = marbleShade;
			const float middle = a_min.x + width * 0.4f;
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(middle, a_max.y), shade, light, light, shade);
			a_drawList->AddRectFilledMultiColor(ImVec2(middle, a_min.y), a_max, light, Shade(shade, 0.8f), Shade(shade, 0.8f), light);
			// The flutes
			for (int flute = 1; flute <= 3; ++flute) {
				const float x = a_min.x + width * static_cast<float>(flute) / 4.0f;
				a_drawList->AddLine(ImVec2(x, a_min.y), ImVec2(x, a_max.y), WithAlpha(vein, 0.75f), 1.0f);
				a_drawList->AddLine(ImVec2(x + 1.0f, a_min.y), ImVec2(x + 1.0f, a_max.y), WithAlpha(IM_COL32(255, 255, 255, 255), 0.35f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Capital ) ---------------------
		//---------------------------------------------------

		// An Ionic capital: a slab wider than the shaft, a volute curling down at each end
		void Capital(ImDrawList* a_drawList, ImVec2 a_centre, float a_half, float a_height) {

			const ImVec2 min(a_centre.x - a_half, a_centre.y - a_height * 0.5f);
			const ImVec2 max(a_centre.x + a_half, a_centre.y + a_height * 0.5f);
			a_drawList->AddRectFilled(ImVec2(min.x + a_height * 0.3f, min.y), ImVec2(max.x - a_height * 0.3f, max.y), marble);
			a_drawList->AddLine(ImVec2(min.x, min.y + 0.5f), ImVec2(max.x, min.y + 0.5f), bronzeLight, std::max(1.5f, a_height * 0.14f));
			for (const float side : { -1.0f, 1.0f }) {
				const ImVec2 eye(a_centre.x + side * (a_half - a_height * 0.42f), a_centre.y + a_height * 0.08f);
				a_drawList->AddCircleFilled(eye, a_height * 0.44f, marble, 16);
				for (int turn = 0; turn < 14; ++turn) {
					const float t = static_cast<float>(turn) / 13.0f;
					const float radius = a_height * 0.4f * (1.0f - t * 0.8f);
					const float angle = -pi * 0.5f + side * t * pi * 2.4f;
					a_drawList->PathLineTo(ImVec2(eye.x + std::cos(angle) * radius, eye.y + std::sin(angle) * radius));
				}
				a_drawList->PathStroke(bronze, ImDrawFlags_None, std::max(1.0f, a_height * 0.1f));
			}
			a_drawList->AddLine(ImVec2(min.x + a_height * 0.4f, max.y), ImVec2(max.x - a_height * 0.4f, max.y), marbleShade, 1.0f);
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Entablature ) -----------------
		//---------------------------------------------------

		// The beam the columns carry: a plain architrave, a row of dentils, the bronze-capped cornice
		void Entablature(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float height = a_max.y - a_min.y;
			a_drawList->AddRectFilled(a_min, a_max, marble);
			const float cornice = height * 0.3f;
			a_drawList->AddRectFilled(a_min, ImVec2(a_max.x, a_min.y + cornice), bronze);
			a_drawList->AddLine(ImVec2(a_min.x, a_min.y + 0.5f), ImVec2(a_max.x, a_min.y + 0.5f), bronzeLight, 1.0f);
			// Dentils: small blocks under the cornice, the gaps between them in shadow
			const float top = a_min.y + cornice;
			const float bottom = top + height * 0.32f;
			const float tooth = std::max(3.0f, height * 0.36f);
			a_drawList->AddRectFilled(ImVec2(a_min.x, top), ImVec2(a_max.x, bottom), marbleShade);
			for (float x = a_min.x + tooth * 0.3f; x + tooth * 0.6f < a_max.x; x += tooth) {
				a_drawList->AddRectFilled(ImVec2(x, top), ImVec2(x + tooth * 0.62f, bottom), marble);
			}
			a_drawList->AddLine(ImVec2(a_min.x, bottom + 0.5f), ImVec2(a_max.x, bottom + 0.5f), vein, 1.0f);
			a_drawList->AddLine(ImVec2(a_min.x, a_max.y - 0.5f), ImVec2(a_max.x, a_max.y - 0.5f), marbleShade, 1.0f);
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Steps ) -----------------------
		//---------------------------------------------------

		// Marble steps the temple stands on, each lit along its edge
		void Steps(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, int a_count, float a_spread) {

			const float rise = (a_max.y - a_min.y) / static_cast<float>(a_count);
			for (int step = 0; step < a_count; ++step) {
				const float out = a_spread * static_cast<float>(step);
				const ImVec2 min(a_min.x - out, a_min.y + rise * static_cast<float>(step));
				const ImVec2 max(a_max.x + out, min.y + rise);
				a_drawList->AddRectFilled(min, max, Shade(marble, 1.0f - 0.06f * static_cast<float>(step)));
				a_drawList->AddLine(ImVec2(min.x, min.y + 0.5f), ImVec2(max.x, min.y + 0.5f), WithAlpha(IM_COL32(255, 255, 255, 255), 0.6f), 1.0f);
				a_drawList->AddLine(ImVec2(min.x, max.y - 0.5f), ImVec2(max.x, max.y - 0.5f), marbleShade, 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Imperial Decor ( Pediment ) --------------------
		//---------------------------------------------------

		// The temple's gable: a marble triangle under a bronze cornice, the wreathed seal of the Empire in its face
		void Pediment(ImDrawList* a_drawList, ImVec2 a_base, float a_half, float a_height) {

			const ImVec2 left(a_base.x - a_half, a_base.y);
			const ImVec2 right(a_base.x + a_half, a_base.y);
			const ImVec2 apex(a_base.x, a_base.y - a_height);
			a_drawList->AddTriangleFilled(ImVec2(left.x - a_height * 0.12f, left.y + 1.0f), ImVec2(apex.x, apex.y - a_height * 0.12f), ImVec2(right.x + a_height * 0.12f, right.y + 1.0f), bronze);
			a_drawList->AddTriangleFilled(left, apex, right, marble);
			// The recessed face
			const float inset = a_height * 0.18f;
			a_drawList->AddTriangleFilled(ImVec2(left.x + inset * 2.2f, left.y - inset * 0.5f), ImVec2(apex.x, apex.y + inset * 1.3f), ImVec2(right.x - inset * 2.2f, right.y - inset * 0.5f), marbleShade);
			a_drawList->AddLine(ImVec2(left.x - a_height * 0.12f, left.y), ImVec2(apex.x, apex.y - a_height * 0.12f), bronzeLight, 1.0f);
			const ImVec2 seal(a_base.x, a_base.y - a_height * 0.5f);
			const float  radius = a_height * 0.24f;
			Wreath(a_drawList, seal, radius, bronze);
			a_drawList->AddCircleFilled(seal, radius * 0.55f, crimson, 20);
			a_drawList->AddCircle(seal, radius * 0.55f, bronzeLight, 20, 1.0f);
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

	// A temple front: fluted columns up the sides under Ionic capitals, the entablature across the top,
	// marble steps at the foot, and on the main window a pediment rising over it all
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);
		const bool  full = compact >= 0.8f;

		Mosaic(a_drawList, a_min, a_max, band + size * 0.4f);

		// Behind the window, so only what stands out past its edges shows: the pediment, the capitals' ends, the lower steps
		auto*       behind = ImGui::GetBackgroundDrawList();
		const float capitalHalf = band * 1.15f;
		const float capitalHeight = band * 0.85f;
		const float beam = band * 1.15f;
		if (full) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			Pediment(behind, ImVec2(midX, a_min.y + band * 0.5f), std::min(width * 0.16f, size * 11.0f), band * 4.4f);
			Steps(behind, ImVec2(a_min.x - band * 0.2f, a_max.y - band * 0.5f), ImVec2(a_max.x + band * 0.2f, a_max.y + band * 0.9f), 2, band * 0.35f);
		}

		// The steps along the foot
		Steps(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, 2, 0.0f);

		// The columns, standing on the steps and under the beam
		const float shaftTop = a_min.y + beam + capitalHeight * 0.8f;
		const float shaftBottom = a_max.y - band * 1.3f;
		const float shaft = band * 0.95f;
		for (const float x : { a_min.x + band * 0.5f, a_max.x - band * 0.5f }) {
			Column(a_drawList, ImVec2(x - shaft * 0.5f, shaftTop), ImVec2(x + shaft * 0.5f, shaftBottom));
			// A plinth under each
			a_drawList->AddRectFilled(ImVec2(x - shaft * 0.7f, shaftBottom), ImVec2(x + shaft * 0.7f, a_max.y - band), marble);
			a_drawList->AddLine(ImVec2(x - shaft * 0.7f, shaftBottom + 0.5f), ImVec2(x + shaft * 0.7f, shaftBottom + 0.5f), marbleShade, 1.0f);
			for (auto* list : { behind, a_drawList }) {
				Capital(list, ImVec2(x, a_min.y + beam + capitalHeight * 0.5f), capitalHalf, capitalHeight);
			}
		}

		Entablature(a_drawList, a_min, ImVec2(a_max.x, a_min.y + beam));
	}

	//---------------------------------------------------
	//-- Imperial Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: a mosaic edge of small tiles fading in towards the text, daylight from above, a laurel wreath in the empty corner
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

		// Loose tiles, thick at the edges and thinning towards the middle
		Random      random{ static_cast<std::uint32_t>(width) * 29u + static_cast<std::uint32_t>(height) };
		const float tile = std::max(3.0f, size * 0.3f);
		const int   tiles = std::clamp(static_cast<int>(width * height / (size * size * 0.9f)), 40, 1600);
		for (int i = 0; i < tiles; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.05f) {
				continue;
			}
			const ImVec2 snapped(a_min.x + std::floor((at.x - a_min.x) / tile) * tile, a_min.y + std::floor((at.y - a_min.y) / tile) * tile);
			a_drawList->AddRectFilled(ImVec2(snapped.x + 1.0f, snapped.y + 1.0f), ImVec2(snapped.x + tile - 1.0f, snapped.y + tile - 1.0f), WithAlpha(random.Next() < 0.15f ? crimson : marble, weight * 0.12f));
		}

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

	// A sprig of laurel tucked into each corner, a bronze stud at its stem
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float leaf = size * 0.55f;
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 stem(outer.x + sx * size * 0.35f, outer.y + sy * size * 0.35f);
			// Leaves running out along both edges from the corner
			for (int i = 0; i < 3; ++i) {
				const float along = size * (0.35f + 0.42f * static_cast<float>(i));
				const float angleX = sx > 0.0f ? 0.0f : pi;
				const float angleY = sy > 0.0f ? pi * 0.5f : -pi * 0.5f;
				Leaf(a_drawList, ImVec2(stem.x + sx * along, stem.y), leaf * (1.0f - 0.15f * static_cast<float>(i)), angleX + sx * sy * 0.45f, bronze);
				Leaf(a_drawList, ImVec2(stem.x, stem.y + sy * along), leaf * (1.0f - 0.15f * static_cast<float>(i)), angleY - sx * sy * 0.45f, bronze);
			}
			a_drawList->AddLine(stem, ImVec2(stem.x + sx * size * 1.3f, stem.y), WithAlpha(bronze, 0.8f), 1.0f);
			a_drawList->AddLine(stem, ImVec2(stem.x, stem.y + sy * size * 1.3f), WithAlpha(bronze, 0.8f), 1.0f);
			Stud(a_drawList, stem, size * 0.22f, bronzeLight, true);
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
