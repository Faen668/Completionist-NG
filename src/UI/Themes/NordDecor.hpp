#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Nord theme: a mead hall's timber and iron, knotwork, hearth light and snow
namespace CompletionistUI::NordDecor
{
	void Boss(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void RuneStone(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
	void Knotwork(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);
}
