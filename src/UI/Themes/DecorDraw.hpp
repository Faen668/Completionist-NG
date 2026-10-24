#pragma once

#include <imgui.h>

#include <algorithm>
#include <cstdint>

// Drawing helpers shared by the theme ornaments
namespace CompletionistUI::DecorDraw
{

	//---------------------------------------------------
	//-- Decor Draw ( With Alpha ) ----------------------
	//---------------------------------------------------

	inline ImU32 WithAlpha(ImU32 a_colour, float a_alpha) {

		const auto alpha = static_cast<ImU32>(((a_colour >> IM_COL32_A_SHIFT) & 0xFF) * std::clamp(a_alpha, 0.0f, 1.0f));
		return (a_colour & ~IM_COL32_A_MASK) | (alpha << IM_COL32_A_SHIFT);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Shade ) ---------------------------
	//---------------------------------------------------

	// Darker or lighter, keeps alpha
	inline ImU32 Shade(ImU32 a_colour, float a_factor) {

		const auto channel = [&](int a_shift) {
			const float value = static_cast<float>((a_colour >> a_shift) & 0xFF) * a_factor;
			return static_cast<ImU32>(std::clamp(value, 0.0f, 255.0f)) << a_shift;
		};
		return channel(IM_COL32_R_SHIFT) | channel(IM_COL32_G_SHIFT) | channel(IM_COL32_B_SHIFT) | (a_colour & IM_COL32_A_MASK);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Random ) --------------------------
	//---------------------------------------------------

	// Same sequence every frame, so the decoration stays put
	struct Random
	{
		std::uint32_t state;

		float Next() {

			state ^= state << 13;
			state ^= state >> 17;
			state ^= state << 5;
			return static_cast<float>(state & 0xFFFFFF) / static_cast<float>(0x1000000);
		}

		float Range(float a_low, float a_high) { return a_low + (a_high - a_low) * Next(); }
	};

	//---------------------------------------------------
	//-- Decor Draw ( Gradient Quad ) -------------------
	//---------------------------------------------------

	// Four corners, four colours, for light shafts and haze
	inline void GradientQuad(ImDrawList* a_drawList, const ImVec2 (&a_points)[4], const ImU32 (&a_colours)[4]) {

		const auto uv = ImGui::GetFontTexUvWhitePixel();
		a_drawList->PrimReserve(6, 4);
		const auto base = static_cast<ImDrawIdx>(a_drawList->_VtxCurrentIdx);
		for (int i = 0; i < 4; ++i) {
			a_drawList->PrimWriteVtx(a_points[i], uv, a_colours[i]);
		}
		a_drawList->PrimWriteIdx(base);
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 1));
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 2));
		a_drawList->PrimWriteIdx(base);
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 2));
		a_drawList->PrimWriteIdx(static_cast<ImDrawIdx>(base + 3));
	}

	//---------------------------------------------------
	//-- Decor Draw ( Soft Spot ) -----------------------
	//---------------------------------------------------

	// Round glow, rings fading out
	inline void SoftSpot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		constexpr int rings = 8;
		for (int ring = rings; ring > 0; --ring) {
			a_drawList->AddCircleFilled(a_centre, a_radius * static_cast<float>(ring) / rings, a_colour, 32);
		}
	}

	//---------------------------------------------------
	//-- Decor Draw ( Edge Shadow ) ---------------------
	//---------------------------------------------------

	// Darkens a rect towards its edges
	inline void EdgeShadow(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_fade, ImU32 a_dark) {

		const ImU32 clear = WithAlpha(a_dark, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + a_fade), a_dark, a_dark, clear, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - a_fade), a_max, clear, clear, a_dark, a_dark);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + a_fade, a_max.y), a_dark, clear, clear, a_dark);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_max.x - a_fade, a_min.y), a_max, clear, a_dark, a_dark, clear);
	}

	//---------------------------------------------------
	//-- Decor Draw ( Edge Weight ) ---------------------
	//---------------------------------------------------

	// 1 at a rect's edge, falling to 0 a_band in, so panel dressing clears behind the text
	inline float EdgeWeight(ImVec2 a_point, ImVec2 a_min, ImVec2 a_max, float a_band) {

		const float distance = std::min({ a_point.x - a_min.x, a_max.x - a_point.x, a_point.y - a_min.y, a_max.y - a_point.y });
		const float t = 1.0f - std::clamp(distance / a_band, 0.0f, 1.0f);
		return t * t;
	}
}
