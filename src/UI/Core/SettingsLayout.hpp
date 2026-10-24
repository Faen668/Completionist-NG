#pragma once

#include "UI/Core/UIModel.hpp"

#include <string>
#include <vector>

namespace CompletionistUI
{
	// One settings row, rebuilt from the MCM
	struct SettingRow
	{
		SettingKind              kind{ SettingKind::kText };
		std::string              id;
		std::string              label;
		std::string              info{};
		std::string              value{};	// setting name
		std::string              text{};
		std::string              colour{};	// HUD colour suffix
		std::vector<std::string> options{};
		std::int32_t             minimum{ 0 };
		std::int32_t             maximum{ 0 };
		std::int32_t             step{ 1 };
		std::string              enabledBy{};	// bool setting the row needs, "!name" for off
		std::string              action{};	// handled by the game provider
		std::string              fallback{};
		std::string              button{};
		bool                     confirm{ false };
	};

	struct SettingsPageLayout
	{
		std::string             id;
		std::string             title;
		std::string             plugin;	// only shown when this plugin is loaded
		std::vector<SettingRow> columns[2];
		bool                    singleColumn{ false };
		bool                    tab{ true };
	};

	[[nodiscard]] const std::vector<SettingsPageLayout>& MCMSettingsLayouts();

	// Filled in by the provider
	inline constexpr std::string_view dynamicRowPrefix = "dyn:";

	class SettingValues
	{
	public:
		virtual ~SettingValues() = default;

		[[nodiscard]] virtual bool         IsBool(const std::string& a_name) = 0;
		[[nodiscard]] virtual bool         GetBool(const std::string& a_name) = 0;
		[[nodiscard]] virtual std::int32_t GetInt(const std::string& a_name) = 0;
		[[nodiscard]] virtual std::string  GetString(const std::string& a_name) = 0;
		virtual void                       SetBool(const std::string& a_name, bool a_value) = 0;
		virtual void                       SetInt(const std::string& a_name, std::int32_t a_value) = 0;
		virtual void                       SetString(const std::string& a_name, const std::string& a_value) = 0;
		[[nodiscard]] virtual std::string  KeyName(std::int32_t a_scanCode) { return std::to_string(a_scanCode); }
	};

	[[nodiscard]] SettingItem MakeSettingItem(const SettingRow& a_row, SettingValues& a_values);

	void WriteSettingItem(const SettingRow& a_row, const SettingItem& a_item, SettingValues& a_values);

	// #RRGGBB as the MCM writes it
	[[nodiscard]] std::string HexColour(std::uint32_t a_colour);

	[[nodiscard]] const SettingRow* FindSettingRow(std::string_view a_pageID, std::string_view a_rowID);
}
