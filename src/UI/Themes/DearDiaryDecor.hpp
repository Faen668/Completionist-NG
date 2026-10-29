#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Dear Diary theme, in the style of Dear Diary Dark Mode: near-black panels in worn bronze frames, double rules with knotted ends, red for the selection
namespace CompletionistUI::DearDiaryDecor
{
	void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);
}
