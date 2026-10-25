#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Bosmer theme: the Green Pact's dark forest, bark, thorns and leaves
namespace CompletionistUI::BosmerDecor
{
	void Leaf(ImDrawList* a_drawList, ImVec2 a_base, float a_length, float a_angle, ImU32 a_colour, bool a_filled, bool a_vein = true);
	void Sprig(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, float a_angle, ImU32 a_colour);
	void Vine(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_stem, ImU32 a_leaves, float a_size, unsigned a_seed);
	void Foliage(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);
}
