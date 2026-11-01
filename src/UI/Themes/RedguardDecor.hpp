#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Redguard theme: an Alik'r desert tent, striped canopy, rope-lashed poles, kilim, brass lamps, turquoise beads and curved swords
namespace CompletionistUI::RedguardDecor
{
	void Tile(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Beads(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size, bool a_fade = true);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);
}
