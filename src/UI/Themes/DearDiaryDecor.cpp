#include "UI/Themes/DearDiaryDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

namespace CompletionistUI::DearDiaryDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Worn bronze on near-black, silver-grey rules, a deep red for what's chosen
		constexpr ImU32 bronze = IM_COL32(112, 86, 62, 255);
		constexpr ImU32 bronzeLight = IM_COL32(168, 134, 98, 255);
		constexpr ImU32 bronzeDark = IM_COL32(40, 30, 22, 255);
		constexpr ImU32 silver = IM_COL32(176, 172, 166, 255);
		constexpr ImU32 blood = IM_COL32(150, 26, 22, 255);
		constexpr ImU32 bloodLight = IM_COL32(214, 52, 40, 255);
		constexpr ImU32 ink = IM_COL32(6, 5, 5, 255);

		//---------------------------------------------------
		//-- Dear Diary Decor ( Grain ) ---------------------
		//---------------------------------------------------

		// Scuffed dark leather: fine speckle and a few long scratches, heavier towards the edges
		void Grain(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_band, float a_strength) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			Random      random{ static_cast<std::uint32_t>(width) * 131u + static_cast<std::uint32_t>(height) * 7u + 1u };
			const int   specks = std::clamp(static_cast<int>(width * height / (size * size * 0.4f)), 40, 2000);
			for (int i = 0; i < specks; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  weight = 0.25f + 0.75f * EdgeWeight(at, a_min, a_max, a_band);
				const bool   light = random.Next() < 0.5f;
				a_drawList->AddCircleFilled(at, random.Range(0.5f, 1.3f), WithAlpha(light ? IM_COL32(120, 112, 104, 255) : ink, weight * a_strength * (light ? 0.18f : 0.5f)), 4);
			}
			const int scratches = std::clamp(static_cast<int>(width * height / (size * size * 40.0f)), 3, 40);
			for (int i = 0; i < scratches; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  angle = random.Range(-0.5f, 0.5f) + (random.Next() < 0.5f ? 0.0f : pi * 0.5f);
				const float  length = size * random.Range(0.8f, 3.0f);
				const ImVec2 to(std::clamp(at.x + std::cos(angle) * length, a_min.x, a_max.x), std::clamp(at.y + std::sin(angle) * length, a_min.y, a_max.y));
				const float  weight = 0.3f + 0.7f * EdgeWeight(at, a_min, a_max, a_band);
				a_drawList->AddLine(at, to, WithAlpha(IM_COL32(150, 140, 128, 255), weight * a_strength * 0.07f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Dear Diary Decor ( Bevel ) ---------------------
		//---------------------------------------------------

		// A thin bronze moulding: dark outside, lit inner edge, the bronze between
		void Bevel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_width) {

			a_drawList->AddRect(a_min, a_max, bronzeDark, 0.0f, 0, a_width + 2.0f);
			a_drawList->AddRect(a_min, a_max, bronze, 0.0f, 0, a_width);
			const float in = a_width * 0.5f;
			a_drawList->AddLine(ImVec2(a_min.x - in, a_min.y - in), ImVec2(a_max.x + in, a_min.y - in), WithAlpha(bronzeLight, 0.55f), 1.0f);
			a_drawList->AddLine(ImVec2(a_min.x - in, a_min.y - in), ImVec2(a_min.x - in, a_max.y + in), WithAlpha(bronzeLight, 0.35f), 1.0f);
			a_drawList->AddRect(ImVec2(a_min.x + in + 1.0f, a_min.y + in + 1.0f), ImVec2(a_max.x - in - 1.0f, a_max.y - in - 1.0f), WithAlpha(ink, 0.9f), 0.0f, 0, 1.0f);
		}

		//---------------------------------------------------
		//-- Dear Diary Decor ( Bracket ) -------------------
		//---------------------------------------------------

		// A corner bracket: two arms with a cut tip, a small spur off each, a_sx/a_sy point inwards
		void Bracket(ImDrawList* a_drawList, ImVec2 a_corner, float a_arm, float a_thick, float a_sx, float a_sy) {

			const auto at = [&](float a_x, float a_y) { return ImVec2(a_corner.x + a_sx * a_x, a_corner.y + a_sy * a_y); };
			const ImVec2 arm[3] = { at(a_arm, 0.0f), at(0.0f, 0.0f), at(0.0f, a_arm) };
			a_drawList->AddPolyline(arm, 3, bronzeDark, ImDrawFlags_None, a_thick + 2.0f);
			a_drawList->AddPolyline(arm, 3, bronze, ImDrawFlags_None, a_thick);
			// Cut tips, angled back towards the corner
			for (const auto& [tip, back] : { std::pair{ at(a_arm, 0.0f), at(a_arm - a_thick * 1.6f, a_thick * 1.6f) }, std::pair{ at(0.0f, a_arm), at(a_thick * 1.6f, a_arm - a_thick * 1.6f) } }) {
				a_drawList->AddLine(tip, back, bronze, a_thick * 0.8f);
			}
			// The lit edge and a spur
			a_drawList->AddLine(at(a_thick * 0.2f, -a_thick * 0.3f), at(a_arm * 0.95f, -a_thick * 0.3f), WithAlpha(bronzeLight, 0.7f), 1.0f);
			a_drawList->AddLine(at(a_arm * 0.35f, a_thick * 0.5f), at(a_arm * 0.35f + a_thick * 1.4f, a_thick * 1.9f), WithAlpha(bronze, 0.9f), std::max(1.0f, a_thick * 0.5f));
			a_drawList->AddLine(at(a_thick * 0.5f, a_arm * 0.35f), at(a_thick * 1.9f, a_arm * 0.35f + a_thick * 1.4f), WithAlpha(bronze, 0.9f), std::max(1.0f, a_thick * 0.5f));
		}

		//---------------------------------------------------
		//-- Dear Diary Decor ( Knot Head ) -----------------
		//---------------------------------------------------

		// The knotted arrowhead that ends a double rule, pointing along a_facing (-1 left, 1 right)
		void KnotHead(ImDrawList* a_drawList, ImVec2 a_tip, float a_size, float a_facing, ImU32 a_colour) {

			const auto  at = [&](float a_x, float a_y) { return ImVec2(a_tip.x + a_x * a_size * a_facing, a_tip.y + a_y * a_size); };
			const float stroke = std::max(1.0f, a_size * 0.1f);
			const ImU32 cut = WithAlpha(ink, ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f);
			// Two loops crossing: an arrow tip and a bow behind it
			const ImVec2 tip[4] = { at(0.0f, 0.0f), at(-0.55f, -0.42f), at(-0.95f, 0.0f), at(-0.55f, 0.42f) };
			for (const auto& [colour, thick] : { std::pair{ cut, stroke + 2.0f }, std::pair{ a_colour, stroke } }) {
				a_drawList->AddPolyline(tip, 4, colour, ImDrawFlags_Closed, thick);
				a_drawList->AddBezierCubic(at(-0.3f, -0.22f), at(-1.2f, -0.75f), at(-1.2f, 0.75f), at(-0.3f, 0.22f), colour, thick, 16);
			}
			a_drawList->AddLine(at(-0.95f, 0.0f), at(-1.3f, 0.0f), a_colour, stroke);
		}

		//---------------------------------------------------
		//-- Dear Diary Decor ( Double Line ) ---------------
		//---------------------------------------------------

		// Two fine parallel lines; a_fadeFrom / a_fadeTo fade that end out
		void DoubleLine(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_gap, bool a_fadeFrom, bool a_fadeTo) {

			const float midX = (a_from.x + a_to.x) * 0.5f;
			const ImU32 clear = WithAlpha(a_colour, 0.0f);
			for (const float offset : { -a_gap * 0.5f, a_gap * 0.5f }) {
				const float y = a_from.y + offset;
				a_drawList->AddRectFilledMultiColor(ImVec2(a_from.x, y - 0.5f), ImVec2(midX, y + 0.5f), a_fadeFrom ? clear : a_colour, a_colour, a_colour, a_fadeFrom ? clear : a_colour);
				a_drawList->AddRectFilledMultiColor(ImVec2(midX, y - 0.5f), ImVec2(a_to.x, y + 0.5f), a_colour, a_fadeTo ? clear : a_colour, a_fadeTo ? clear : a_colour, a_colour);
			}
		}
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Diamond ) -------------------
	//---------------------------------------------------

	// The journal's objective bullet: an outlined diamond with a dot, solid red when filled
	void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(points, 4, a_colour);
			a_drawList->AddPolyline(points, 4, WithAlpha(ink, 0.6f), ImDrawFlags_Closed, 1.0f);
			return;
		}
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.2f));
		a_drawList->AddCircleFilled(a_centre, std::max(1.0f, a_radius * 0.2f), a_colour, 6);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Marker ) --------------------
	//---------------------------------------------------

	// Needed: the diamond bullet. Found: the green tick the journal puts on a finished objective
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.28f;
		if (a_fill < 1.0f) {
			Diamond(a_drawList, a_centre, radius, WithAlpha(a_colour, 1.0f - a_fill), false);
		}
		if (a_fill <= 0.0f) {
			return;
		}
		// The tick draws in, short stroke then long
		const ImVec2 start(a_centre.x - a_size * 0.3f, a_centre.y + a_size * 0.02f);
		const ImVec2 bend(a_centre.x - a_size * 0.08f, a_centre.y + a_size * 0.24f);
		const ImVec2 end(a_centre.x + a_size * 0.34f, a_centre.y - a_size * 0.26f);
		const float  stroke = std::max(1.5f, a_size * 0.12f);
		const float  first = std::min(1.0f, a_fill * 2.5f);
		const float  second = std::clamp(a_fill * 2.5f - 1.0f, 0.0f, 1.0f);
		const ImU32  colour = WithAlpha(a_colour, a_fill);
		a_drawList->AddLine(start, ImVec2(start.x + (bend.x - start.x) * first, start.y + (bend.y - start.y) * first), colour, stroke);
		if (second > 0.0f) {
			a_drawList->AddLine(bend, ImVec2(bend.x + (end.x - bend.x) * second, bend.y + (end.y - bend.y) * second), colour, stroke);
		}
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Knot ) ----------------------
	//---------------------------------------------------

	// A small interlaced bow, two knot heads back to back
	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		KnotHead(a_drawList, ImVec2(a_centre.x + a_radius * 1.1f, a_centre.y), a_radius * 0.9f, 1.0f, a_colour);
		KnotHead(a_drawList, ImVec2(a_centre.x - a_radius * 1.1f, a_centre.y), a_radius * 0.9f, -1.0f, a_colour);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Rule ) ----------------------
	//---------------------------------------------------

	// A double rule, faded at both ends
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		if (a_to.x - a_from.x < 4.0f) {
			return;
		}
		DoubleLine(a_drawList, a_from, a_to, WithAlpha(silver, 0.75f * (((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f)), std::max(3.0f, ImGui::GetFontSize() * 0.2f), true, true);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Heading Tail ) --------------
	//---------------------------------------------------

	// After a heading: the knotted head pointing back at it, then the double rule running out
	void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize() * 0.5f;
		if (a_to.x - a_from.x < size * 3.0f) {
			return;
		}
		const ImU32 colour = WithAlpha(silver, 0.85f * (((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f));
		KnotHead(a_drawList, ImVec2(a_from.x, a_from.y), size, -1.0f, colour);
		DoubleLine(a_drawList, ImVec2(a_from.x + size * 1.3f, a_from.y), a_to, colour, std::max(3.0f, size * 0.38f), false, true);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Divider ) -------------------
	//---------------------------------------------------

	// Two double rules, their knotted heads pointing in at a red diamond
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float size = ImGui::GetFontSize() * 0.45f;
		const float gap = size * 2.2f;
		const ImU32 colour = WithAlpha(silver, 0.7f);
		DoubleLine(a_drawList, a_from, ImVec2(midX - gap - size * 1.3f, a_to.y), colour, std::max(3.0f, size * 0.38f), true, false);
		DoubleLine(a_drawList, ImVec2(midX + gap + size * 1.3f, a_from.y), a_to, colour, std::max(3.0f, size * 0.38f), false, true);
		KnotHead(a_drawList, ImVec2(midX - gap, a_from.y), size, 1.0f, colour);
		KnotHead(a_drawList, ImVec2(midX + gap, a_from.y), size, -1.0f, colour);
		Diamond(a_drawList, ImVec2(midX, a_from.y), size * 0.55f, blood, true);
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Frame ) ---------------------
	//---------------------------------------------------

	// Scuffed near-black leather inside a thin bronze moulding, brackets on the corners
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.5f, 1.0f);
		const float inset = size * 0.3f;

		a_drawList->PushClipRect(a_min, a_max, true);
		Grain(a_drawList, a_min, a_max, std::min(width, height) * 0.25f, 1.0f);
		EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.12f, IM_COL32(0, 0, 0, 170));
		a_drawList->PopClipRect();

		Bevel(a_drawList, ImVec2(a_min.x + inset, a_min.y + inset), ImVec2(a_max.x - inset, a_max.y - inset), std::max(2.0f, size * 0.12f));

		const float arm = size * 1.6f * compact;
		const float thick = std::max(2.0f, size * 0.16f);
		const float in = inset - thick * 0.2f;
		Bracket(a_drawList, ImVec2(a_min.x + in, a_min.y + in), arm, thick, 1.0f, 1.0f);
		Bracket(a_drawList, ImVec2(a_max.x - in, a_min.y + in), arm, thick, -1.0f, 1.0f);
		Bracket(a_drawList, ImVec2(a_max.x - in, a_max.y - in), arm, thick, -1.0f, -1.0f);
		Bracket(a_drawList, ImVec2(a_min.x + in, a_max.y - in), arm, thick, 1.0f, -1.0f);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Panel ) ---------------------
	//---------------------------------------------------

	// A darker inset: worn at the edges, a faint red glow behind an emblem knot
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(ink, 0.35f));
		Grain(a_drawList, a_min, a_max, std::min(width, height) * 0.3f + size, 0.7f);
		a_drawList->AddRect(a_min, a_max, WithAlpha(bronzeDark, 0.9f), 0.0f, 0, 1.0f);
		a_drawList->AddLine(ImVec2(a_min.x + 1.0f, a_min.y + 1.0f), ImVec2(a_max.x - 1.0f, a_min.y + 1.0f), WithAlpha(bronze, 0.35f), 1.0f);

		if (a_emblem) {
			const float  radius = std::min(height * 0.18f, width * 0.08f);
			const ImVec2 centre(a_max.x - radius * 2.6f, a_max.y - radius * 1.6f);
			SoftSpot(a_drawList, centre, radius * 2.4f, WithAlpha(blood, 0.012f));
			Knot(a_drawList, centre, radius, WithAlpha(silver, 0.1f));
		}
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Panel Corners ) -------------
	//---------------------------------------------------

	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 0.9f;
		const float thick = std::max(2.0f, size * 0.12f);
		const float in = thick * 0.5f + 1.0f;
		Bracket(a_drawList, ImVec2(a_min.x + in, a_min.y + in), arm, thick, 1.0f, 1.0f);
		Bracket(a_drawList, ImVec2(a_max.x - in, a_min.y + in), arm, thick, -1.0f, 1.0f);
		Bracket(a_drawList, ImVec2(a_max.x - in, a_max.y - in), arm, thick, -1.0f, -1.0f);
		Bracket(a_drawList, ImVec2(a_min.x + in, a_max.y - in), arm, thick, 1.0f, -1.0f);
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Glow Bar ) ------------------
	//---------------------------------------------------

	// The chosen row: a fine light outline with a pointed right end, a red arrow at the left
	void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha) {

		const float height = a_max.y - a_min.y;
		const float point = std::min(height * 0.45f, (a_max.x - a_min.x) * 0.1f);
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const ImVec2 outline[5] = { a_min, ImVec2(a_max.x - point, a_min.y), ImVec2(a_max.x, midY), ImVec2(a_max.x - point, a_max.y), ImVec2(a_min.x, a_max.y) };
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x - point, a_max.y), WithAlpha(silver, 0.12f * a_alpha), WithAlpha(silver, 0.04f * a_alpha), WithAlpha(silver, 0.04f * a_alpha), WithAlpha(silver, 0.12f * a_alpha));
		a_drawList->AddPolyline(outline, 5, WithAlpha(IM_COL32(222, 218, 210, 255), 0.75f * a_alpha), ImDrawFlags_Closed, 1.0f);
		// Red chevron in the left margin, as on the menus' chosen line
		const float  arrow = std::min(height * 0.28f, ImGui::GetFontSize() * 0.3f);
		const ImVec2 tip(a_min.x + arrow * 1.1f, midY);
		for (const float back : { 0.0f, arrow * 0.6f }) {
			const ImVec2 chevron[3] = { ImVec2(tip.x - arrow - back, midY - arrow), ImVec2(tip.x - back, midY), ImVec2(tip.x - arrow - back, midY + arrow) };
			a_drawList->AddPolyline(chevron, 3, WithAlpha(back == 0.0f ? bloodLight : blood, a_alpha), ImDrawFlags_None, std::max(1.5f, arrow * 0.35f));
		}
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Perk Node ) -----------------
	//---------------------------------------------------

	// Taken: a red gem in a bronze setting. Next: the empty setting. Locked: a dull stud
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.8f, WithAlpha(bloodLight, 0.05f));
			Diamond(a_drawList, a_centre, a_radius * 1.1f, bronze, true);
			Diamond(a_drawList, a_centre, a_radius * 0.75f, blood, true);
			Diamond(a_drawList, ImVec2(a_centre.x - a_radius * 0.15f, a_centre.y - a_radius * 0.2f), a_radius * 0.25f, WithAlpha(bloodLight, 0.9f), true);
			break;
		case PerkState::kNext:
			Diamond(a_drawList, a_centre, a_radius * 1.0f, a_colour, false);
			break;
		default:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.35f, a_colour, 10);
			break;
		}
	}

	//---------------------------------------------------
	//-- Dear Diary Decor ( Perk Link ) -----------------
	//---------------------------------------------------

	// Straight runs: the double rule when taken, one faint line when not
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		if (!a_lit) {
			a_drawList->AddLine(a_from, a_to, a_colour, 1.0f);
			return;
		}
		const std::vector<ImVec2> points{ a_from, a_to };
		for (const float side : { -1.0f, 1.0f }) {
			const auto offset = OffsetPath(points, side * std::max(1.5f, a_radius * 0.16f));
			a_drawList->AddLine(offset[0], offset[1], a_colour, 1.0f);
		}
	}
}
