#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// SkyUI theme: black and white, knots and double rules
namespace CompletionistUI::SkyUIDecor
{
	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
	void DoubleRule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Banner(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_fill, ImU32 a_edge);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_ornament, ImU32 a_rule);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour);
	void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);
}
