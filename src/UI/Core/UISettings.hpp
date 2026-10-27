#pragma once

#include <cstdint>
#include <string>

namespace CompletionistUI
{
	enum class Theme : std::uint8_t
	{
		kCompletionist,
		kSkyUI,
		kBosmer,
		kArgonian,
		kKhajiit,
		kDunmer,
		kNord,
		kAltmer,
	};

	// Button names for each pad, same buttons through XInput
	enum class PadStyle : std::uint8_t
	{
		kXbox,
		kPlayStation,
		kSwitch,
		kSteamDeck,
	};

	struct UISettings
	{
		static constexpr std::int32_t maxDescriptionLines{ 20 };

		std::uint32_t openKey{ 199 };	// Home
		std::uint32_t openPadButton{ 0x0080 };	// RS click
		std::uint32_t openPadModifier{ 0x0040 };	// LS click, 0 for none
		Theme         theme{ Theme::kCompletionist };
		float         scale{ 1.0f };
		float         widthPercent{ 98.0f };
		float         heightPercent{ 93.0f };
		float         opacity{ 1.0f };
		std::int32_t  columns{ 0 };	// 0 = automatic
		std::int32_t  descriptionLines{ 5 };
		bool          descriptionCollapsed{ false };
		bool          navigationCollapsed{ false };
		std::int32_t  findMatch{ 0 };	// 0 contains, 1 starts with, 2 exact
		bool          findIgnoreCompleted{ false };
		bool          openOnLastPage{ true };
		bool          hideCompleted{ false };
		bool          screenshotButton{ false };
		bool          printScreen{ true };
		bool          pauseGame{ true };	// Off leaves the world running while the window is open
		PadStyle      padStyle{ PadStyle::kXbox };
		std::string   lastPage{};

		// Cell Scanner
		std::uint32_t cellKey{ 207 };	// End
		std::uint32_t cellPadButton{ 0 };
		std::uint32_t cellPadModifier{ 0 };
		bool          cellPanel{ true };
		bool          cellOptionsOpen{ true };
		bool          cellHeight{ true };

		// Notifications
		bool  toasts{ true };
		bool  toastsNewestFirst{ true };
		float toastX{ 0.0f };
		float toastY{ 0.317f };

		std::string openKeyName{};
		bool capturingOpenKey{ false };
		bool capturingOpenPad{ false };
		std::string cellKeyName{};
		bool        capturingCellKey{ false };
		bool        capturingCellPad{ false };

		// Key capture for settings rows
		static constexpr std::int32_t captureNone = -2;
		std::string                   captureTarget{};
		std::int32_t                  captureResult{ captureNone };
	};

	[[nodiscard]] inline std::string GamepadButtonName(std::uint32_t a_button, PadStyle a_style = PadStyle::kXbox)
	{
		switch (a_button) {
		case 0x0001: return "D-pad Up";
		case 0x0002: return "D-pad Down";
		case 0x0004: return "D-pad Left";
		case 0x0008: return "D-pad Right";
		default: break;
		}

		struct Names
		{
			const char* start;
			const char* back;
			const char* leftStick;
			const char* rightStick;
			const char* leftBumper;
			const char* rightBumper;
			const char* down;
			const char* right;
			const char* left;
			const char* up;
		};
		static constexpr Names xbox{ "Menu", "View", "LS", "RS", "LB", "RB", "A", "B", "X", "Y" };
		static constexpr Names playStation{ "Options", "Create", "L3", "R3", "L1", "R1", "Cross", "Circle", "Square", "Triangle" };
		static constexpr Names nintendo{ "+", "-", "LS", "RS", "L", "R", "B", "A", "Y", "X" };
		static constexpr Names steamDeck{ "Menu", "View", "L3", "R3", "L1", "R1", "A", "B", "X", "Y" };
		const auto& names = a_style == PadStyle::kPlayStation ? playStation : a_style == PadStyle::kSwitch ? nintendo : a_style == PadStyle::kSteamDeck ? steamDeck : xbox;
		switch (a_button) {
		case 0x0010: return names.start;
		case 0x0020: return names.back;
		case 0x0040: return names.leftStick;
		case 0x0080: return names.rightStick;
		case 0x0100: return names.leftBumper;
		case 0x0200: return names.rightBumper;
		case 0x1000: return names.down;
		case 0x2000: return names.right;
		case 0x4000: return names.left;
		case 0x8000: return names.up;
		default: return std::to_string(a_button);
		}
	}

	// Combo, or Start / RS alone
	[[nodiscard]] inline bool IsSafePadBinding(std::uint32_t a_modifier, std::uint32_t a_button)
	{
		return a_button != 0 && (a_modifier != 0 || a_button == 0x0010 || a_button == 0x0080);
	}

	[[nodiscard]] inline std::string GamepadComboName(std::uint32_t a_modifier, std::uint32_t a_button, PadStyle a_style = PadStyle::kXbox)
	{
		return a_modifier ? GamepadButtonName(a_modifier, a_style) + " + " + GamepadButtonName(a_button, a_style) : GamepadButtonName(a_button, a_style);
	}
}
