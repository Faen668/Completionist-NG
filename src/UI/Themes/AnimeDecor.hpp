#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Anime theme: floating pale panels over the game, dark glass controls, signal yellow slashes, and little cat-eared mascots peeking over everything
namespace CompletionistUI::AnimeDecor
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

	// The main window has no ground of its own: OpenFrame before Frame, then its parts each have a plate
	void OpenFrame();
	void BodyPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void SidebarPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void StripPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void FooterBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	// A notification: a small pale plate, a slash at its head and hazard marks at its tail
	// A tooltip's slash and hazard marks, in the room its padding leaves
	void TooltipMarks(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void ToastPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha, float a_opacity);
	// A dark tab rising from the plates to hold the header's buttons, a_inward the side it slants off towards
	void HeaderTab(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_inward, float a_alpha);

	// The title on a soft veil, so it reads over any scenery
	void TitleBack(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_bottom);
	// Text in the current font, thickened, leaning forward by a_lean
	void BoldText(ImDrawList* a_drawList, ImVec2 a_at, ImU32 a_colour, const char* a_text, float a_lean = 0.0f);

	void NavRow(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_chosen, float a_glow);

	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed);
	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow);
	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit);
	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_fill);
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void Meter(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_fraction);
	void Toolbar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);

	// A page's heading: the band behind the title, a slash before the count. a_textEnd is where the title ends
	void PageHeading(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd);
	[[nodiscard]] float PageHeadingHeight() noexcept;
	[[nodiscard]] float PageHeadingIndent() noexcept;

	// A section's heading, dark, and a selected entry's, yellow
	void SectionBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd);
	void TitleBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd);

	// Whether the spots below hold mascots, or the emblem standing on a_edge with no paws
	void SetMascots(bool a_show) noexcept;

	// Mascots peek over a_edge with their chins hidden and their paws on it. a_style picks which, a_side which way their burst goes
	void Peek(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style, float a_side);
	void Paws(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style);
	// A whole mascot resting on a_edge: the head cut off at the edge, whatever's below it left clean, the paws over it
	void PeekOver(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style, float a_side);
	// The two either side of the title, under the plates; HeaderPaws goes after the plates are down
	void HeaderMascots(ImDrawList* a_drawList, float a_midX, float a_titleHalf, float a_edge);
	void HeaderPaws(ImDrawList* a_drawList);
}
