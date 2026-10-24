#pragma once

#include "UI/Core/UISettings.hpp"

#include <cstdint>
#include <imgui.h>
#include <string_view>

namespace CompletionistUI
{
	enum class Icon : std::uint8_t
	{
		kOverview,
		kCollections,
		kLocations,
		kPatches,
		kQuests,
		kSettings,
	};

	// Drawn ornaments for every theme; each theme's own drawing is in UI/Themes
	class Decor
	{
	public:
		static void SetTheme(Theme a_theme) noexcept { _theme = a_theme; }
		[[nodiscard]] static bool SkyUI() noexcept { return _theme == Theme::kSkyUI; }
		[[nodiscard]] static bool Bosmer() noexcept { return _theme == Theme::kBosmer; }
		[[nodiscard]] static bool Argonian() noexcept { return _theme == Theme::kArgonian; }
		[[nodiscard]] static bool Khajiit() noexcept { return _theme == Theme::kKhajiit; }
		[[nodiscard]] static bool Dunmer() noexcept { return _theme == Theme::kDunmer; }
		// The race themes, as against Completionist and SkyUI
		[[nodiscard]] static bool Race() noexcept { return _theme != Theme::kCompletionist && _theme != Theme::kSkyUI; }
		// How far a race's frame reaches in past the plain one, for padding small panels
		[[nodiscard]] static float FrameDepth() noexcept;
		// Inside a framed panel; false when the theme leaves panels plain
		static bool Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem);
		static void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max);

		static void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled);
		static void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill);
		static void FadedLine(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_thickness = 1.0f);
		static void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
		static void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour);
		static void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha);

		static void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
		static void DoubleRule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
		static void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour);
		static void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour);
		static void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_ornament, ImU32 a_rule);
		static void Banner(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_fill, ImU32 a_edge);
		static void DrawIcon(ImDrawList* a_drawList, Icon a_icon, ImVec2 a_centre, float a_size, ImU32 a_colour);

		[[nodiscard]] static Icon IconFor(std::string_view a_categoryKey) noexcept;

		[[nodiscard]] static float Animate(ImGuiID a_id, bool a_active, float a_speed = 10.0f);

	private:
		static inline Theme _theme{ Theme::kCompletionist };
	};
}
