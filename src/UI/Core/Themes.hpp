#pragma once

#include "UI/Core/UISettings.hpp"

#include <imgui.h>
#include <string>
#include <string_view>

namespace CompletionistUI
{
	struct Palette
	{
		ImVec4 background;
		ImVec4 panel;
		ImVec4 text;
		ImVec4 textDim;
		ImVec4 accent;
		ImVec4 heading;	// headings and tabs
		ImVec4 count;	// sidebar x / x
		ImVec4 bar;	// progress fill
		ImVec4 found;
		ImVec4 needed;
		ImVec4 displayable;
		ImVec4 occupied;
		ImVec4 barBackground;
		ImVec4 separator;
		ImVec4 pinned;	// cell scanner pinned item
		ImVec4 stealing;	// steal hand
	};

	struct ThemeInfo
	{
		Theme            theme;
		std::string_view name;	// Completionist.ini
		const char*      key;	// translation
		bool             race{ false };	// listed after the separator
	};

	// Settings list order: ours and the UI mods' looks, then the races A to Z
	inline constexpr ThemeInfo themeList[] = {
		{ Theme::kCompletionist, "Completionist", "UI_Theme_Completionist" },
		{ Theme::kSkyUI, "SkyUI", "UI_Theme_SkyUI" },
		{ Theme::kDearDiary, "DearDiary", "UI_Theme_DearDiary" },
		{ Theme::kNordic, "Nordic", "UI_Theme_Nordic" },
		{ Theme::kAltmer, "Altmer", "UI_Theme_Altmer", true },
		{ Theme::kArgonian, "Argonian", "UI_Theme_Argonian", true },
		{ Theme::kBosmer, "Bosmer", "UI_Theme_Bosmer", true },
		{ Theme::kDunmer, "Dunmer", "UI_Theme_Dunmer", true },
		{ Theme::kKhajiit, "Khajiit", "UI_Theme_Khajiit", true },
		{ Theme::kNord, "Nord", "UI_Theme_Nord", true },
	};

	struct FontPaths
	{
		std::string regular;
		std::string bold;
		std::string heading;
		std::string title;	// window title, heading when empty
		std::string fallback;	// for missing glyphs
	};

	class Themes
	{
	public:
		static void BuildFonts(const FontPaths& a_paths, float a_scale);
		static void Apply(Theme a_theme, float a_scale);

		[[nodiscard]] static Theme            FromName(std::string_view a_name) noexcept;
		[[nodiscard]] static std::string_view Name(Theme a_theme) noexcept;

		[[nodiscard]] static const Palette& GetPalette() noexcept { return _palette; }
		[[nodiscard]] static Theme          GetTheme() noexcept { return _theme; }

		[[nodiscard]] static ImFont* BodyFont() noexcept { return _body; }
		[[nodiscard]] static ImFont* SmallFont() noexcept { return _small; }
		[[nodiscard]] static ImFont* HeaderFont() noexcept { return _header; }
		[[nodiscard]] static ImFont* TitleFont() noexcept { return _title; }

	private:
		static inline Palette _palette{};
		static inline Theme   _theme{ Theme::kCompletionist };
		static inline ImFont* _body{ nullptr };
		static inline ImFont* _small{ nullptr };
		static inline ImFont* _header{ nullptr };
		static inline ImFont* _title{ nullptr };
	};
}
