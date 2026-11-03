#pragma once

#include "UI/Core/Decor.hpp"

#include <imgui.h>

// Apocrypha theme: Hermaeus Mora's realm, marbled ink, wet black tentacles round the frame, unreadable script, and eyes that watch the cursor
namespace CompletionistUI::ApocryphaDecor
{
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	// An eye: shut when a_fill is 0, wide open at 1
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	// Ground goes first on a window that has room for it; Frame lays the ink and the tentacles either way
	void Ground(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state);
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius);

	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed);
	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit);
	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_fill);
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha);
	void TooltipMarks(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void ToastPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha, float a_opacity, std::uint32_t a_seed);
	void SwatchRim(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit);

	// A big watching eye either side of the title
	void TitleEyes(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_room);
	// The Cell Scanner's heading: an eye and tentacles between the title and the buttons
	void WindowHeader(ImDrawList* a_drawList, float a_top, float a_bottom, float a_left, float a_right);
	// Something large moving behind a long list
	void Creep(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	// The in-game panel's strip, and the small watcher beside its title
	void PanelStrip(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void TitleWatcher(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius);
}
