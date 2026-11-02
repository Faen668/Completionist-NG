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
	// A window's paper, inside its frame. Called just before Frame, it leaves the window a clear margin round the rail
	void Ground(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	// The castle and far hills drawn in a box, standing on its bottom edge and kept inside it. a_ruin adds the ruin at its left
	void BoxEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_ruin, float a_ink = 1.0f);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);

	// The window's dressing, drawn under what sits on it
	void HeaderBand(ImDrawList* a_drawList, float a_bottom, ImVec2 a_titleMin, ImVec2 a_titleMax);
	void FooterBand(ImDrawList* a_drawList, float a_top);
	// A smaller window's head: banners and the Imperial City between its title and its buttons, oak behind the buttons
	void WindowHeader(ImDrawList* a_drawList, float a_bottom, float a_titleEnd, float a_buttonsStart);
	// The in-game panel's: laurel running on from its title, the city along its foot
	void TitleSprig(ImDrawList* a_drawList, ImVec2 a_root, float a_length);
	void PanelSkyline(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void SidebarEtching(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed);
	void NavPlaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void PadCap(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_rounding);
}
