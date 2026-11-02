#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Oblivion theme: a Cyrodiil map on old parchment, worn bronze, crimson banners with the seal, pen and ink etchings of its places
namespace CompletionistUI::OblivionDecor
{
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Rosette(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius);
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	// A panel's drawing of the castle and far hills, standing a_baseline of the way down it
	void PanelEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_baseline);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);

	// The window's dressing, drawn under what sits on it
	void HeaderBand(ImDrawList* a_drawList, float a_bottom, ImVec2 a_titleMin, ImVec2 a_titleMax);
	void FooterBand(ImDrawList* a_drawList, float a_top);
	void SidebarEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed);
	void NavPlaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void PadCap(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_rounding);
}
