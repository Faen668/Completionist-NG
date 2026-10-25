#include "UI/Themes/SkyUIDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"

#include <algorithm>
#include <cmath>

namespace CompletionistUI::SkyUIDecor
{
	using namespace DecorDraw;

	//---------------------------------------------------
	//-- Sky UI Decor ( Knot ) --------------------------
	//---------------------------------------------------

	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const float  stroke = std::max(1.0f, a_radius * 0.16f);
		const float  reach = a_radius * 0.52f;
		const float  loop = a_radius * 0.40f;
		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - reach),
			ImVec2(a_centre.x + reach, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + reach),
			ImVec2(a_centre.x - reach, a_centre.y),
		};
		for (const auto& point : points) {
			const float out = std::atan2(point.y - a_centre.y, point.x - a_centre.x);
			a_drawList->PathArcTo(point, loop, out - 2.4f, out + 2.4f, 12);
			a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
		}
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, stroke);
		Decor::Diamond(a_drawList, a_centre, a_radius * 0.16f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Double Rule ) -------------------
	//---------------------------------------------------

	void DoubleRule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		if (a_to.x <= a_from.x) {
			return;
		}
		const float gap = std::max(2.0f, std::round(ImGui::GetFontSize() * 0.13f));
		const float top = std::round(a_from.y - gap * 0.5f);
		a_drawList->AddRectFilled(ImVec2(a_from.x, top), ImVec2(a_to.x, top + 1.0f), a_colour);
		a_drawList->AddRectFilled(ImVec2(a_from.x, top + gap), ImVec2(a_to.x, top + gap + 1.0f), WithAlpha(a_colour, 0.6f));
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Banner ) ------------------------
	//---------------------------------------------------

	void Banner(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_fill, ImU32 a_edge) {

		const float  midY = (a_min.y + a_max.y) * 0.5f;
		const float  point = (a_max.y - a_min.y) * 0.5f;
		const ImVec2 outline[6] = {
			ImVec2(a_min.x, a_min.y),
			ImVec2(a_max.x, a_min.y),
			ImVec2(a_max.x + point, midY),
			ImVec2(a_max.x, a_max.y),
			ImVec2(a_min.x, a_max.y),
			ImVec2(a_min.x - point, midY),
		};
		a_drawList->AddConvexPolyFilled(outline, 6, a_fill);
		a_drawList->AddPolyline(outline, 6, a_edge, ImDrawFlags_Closed, 1.0f);
		const float  inset = std::max(2.0f, point * 0.3f);
		const ImVec2 inner[6] = {
			ImVec2(a_min.x, a_min.y + inset),
			ImVec2(a_max.x, a_min.y + inset),
			ImVec2(a_max.x + point - inset, midY),
			ImVec2(a_max.x, a_max.y - inset),
			ImVec2(a_min.x, a_max.y - inset),
			ImVec2(a_min.x - point + inset, midY),
		};
		a_drawList->AddPolyline(inner, 6, WithAlpha(a_edge, 0.45f), ImDrawFlags_Closed, 1.0f);
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// SkyUI check: a diamond in a diamond
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.36f;
		Decor::Diamond(a_drawList, a_centre, radius, a_colour, false);
		Decor::Diamond(a_drawList, a_centre, radius * 0.62f, (a_colour & ~IM_COL32_A_MASK) | (((a_colour >> IM_COL32_A_SHIFT) / 3) << IM_COL32_A_SHIFT), false);
		if (a_fill > 0.0f) {
			Decor::Diamond(a_drawList, a_centre, radius * 0.62f * a_fill, a_colour, true);
		}
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.6f;
		DoubleRule(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour);
		DoubleRule(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour);
		Knot(a_drawList, ImVec2(midX, a_from.y), gap * 0.55f, a_colour);
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Heading Tail ) ------------------
	//---------------------------------------------------

	void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_ornament, ImU32 a_rule) {

		const float size = ImGui::GetFontSize() * 0.42f;
		Knot(a_drawList, ImVec2(a_from.x + size, a_from.y), size, a_ornament);
		DoubleRule(a_drawList, ImVec2(a_from.x + size * 2.4f, a_from.y), a_to, a_rule);
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Two rules, knots in the corners, a banner on the bottom edge
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour) {

		const float inset = ImGui::GetFontSize() * 0.3f;
		const float corner = ImGui::GetFontSize() * 0.35f;
		a_drawList->AddRect(a_min, a_max, WithAlpha(a_colour, 0.35f), 0.0f, 0, 1.0f);
		a_drawList->AddRect(ImVec2(a_min.x + inset, a_min.y + inset), ImVec2(a_max.x - inset, a_max.y - inset), a_colour, 0.0f, 0, 1.0f);

		for (const auto& point : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float x = point.x == a_min.x ? a_min.x + inset : a_max.x - inset;
			const float y = point.y == a_min.y ? a_min.y + inset : a_max.y - inset;
			const float knot = corner * 1.8f;
			a_drawList->AddCircleFilled(ImVec2(x, y), knot * 0.8f, IM_COL32(0, 0, 0, 255));
			Knot(a_drawList, ImVec2(x, y), knot, a_colour);
		}

		const float half = ImGui::GetFontSize() * 4.0f;
		const float tall = ImGui::GetFontSize() * 0.32f;
		const float midX = (a_min.x + a_max.x) * 0.5f;
		const float y = a_max.y - inset;
		Banner(a_drawList, ImVec2(midX - half, y - tall), ImVec2(midX + half, y + tall), IM_COL32(0, 0, 0, 255), a_colour);
	}

	//---------------------------------------------------
	//-- Sky UI Decor ( Glow Bar ) ----------------------
	//---------------------------------------------------

	// Grey bar ending in an arrow tip
	void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha) {

		const float tip = (a_max.y - a_min.y) * 0.5f;
		const float bodyRight = a_max.x - tip;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const ImU32 faint = WithAlpha(a_colour, 0.04f * a_alpha);
		const ImU32 strong = WithAlpha(a_colour, 0.24f * a_alpha);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(bodyRight, a_max.y), faint, strong, strong, faint);
		a_drawList->AddTriangleFilled(ImVec2(bodyRight, a_min.y), ImVec2(a_max.x, midY), ImVec2(bodyRight, a_max.y), strong);
		const ImU32 edge = WithAlpha(a_colour, 0.6f * a_alpha);
		const ImU32 edgeClear = WithAlpha(a_colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(bodyRight, a_min.y + 1.0f), edgeClear, edge, edge, edgeClear);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - 1.0f), ImVec2(bodyRight, a_max.y), edgeClear, edge, edge, edgeClear);
		a_drawList->AddLine(ImVec2(bodyRight, a_min.y), ImVec2(a_max.x, midY), edge);
		a_drawList->AddLine(ImVec2(a_max.x, midY), ImVec2(bodyRight, a_max.y), edge);
	}
}

namespace CompletionistUI::SkyUIDecor
{
	//---------------------------------------------------
	//-- SkyUI Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Plain rings, filled when taken
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		const float radius = a_radius * (a_state == PerkState::kLocked ? 0.45f : 0.65f);
		if (a_state == PerkState::kTaken) {
			a_drawList->AddCircleFilled(a_centre, radius, a_colour, 24);
		} else {
			a_drawList->AddCircle(a_centre, radius, a_colour, 24, std::max(1.0f, a_radius * 0.1f));
		}
	}

	//---------------------------------------------------
	//-- SkyUI Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// Straight lines, brighter between taken perks
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		a_drawList->AddLine(a_from, a_to, a_colour, a_lit ? std::max(1.5f, a_radius * 0.12f) : 1.0f);
	}
}
