#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Gothic theme: the Arcane University's library by night, charcoal slate, silver tracery, the shelves' leather spines, the guild's violet banners
namespace CompletionistUI::GothicDecor
{
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);

	// The window's dressing. OpenFrame before Frame leaves the window bare, for HeaderBand to frame only its body
	void OpenFrame();
	void HeaderBand(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_top, float a_bottom);
	void BodyFrame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void FooterBox(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);

	// Sidebar rows are books on a shelf; a_book picks the binding
	void Spine(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, int a_book, bool a_chosen, float a_glow);
	[[nodiscard]] float SpineRight() noexcept;

	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed);
	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow);
	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit);
	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);

	// A page's heading between two ruled lines
	void HeadingBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	[[nodiscard]] float HeadingHeight() noexcept;

	// Line drawings in a panel's empty corner
	void ArcaneCircle(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void OpenBook(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
}
