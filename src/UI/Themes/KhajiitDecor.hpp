#pragma once

#include <imgui.h>

// Khajiit theme: Elsweyr at night, Masser and Secunda, woven rugs and brass
namespace CompletionistUI::KhajiitDecor
{
	void Moon(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, float a_phase);
	void Crescent(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
	void Phase(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
	void TwinMoons(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius);
	void Thread(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size);
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);
}
