#include "Settings.hpp"
#include "DKUtil/Utility.hpp"
#include "SettingsFile.hpp"

namespace CSettings
{
	namespace
	{
		constexpr const char* section = "Settings";

		enum class Type
		{
			kBool,
			kInt,
			kString,
		};

		struct Default
		{
			const char* name;
			Type        type;
			const char* value;
		};

		// Defaults from Begin_Config_Default
		constexpr Default defaults[] = {
			// General Settings
			{ "TreatBooksAsItems", Type::kBool, "false" },
			{ "DG_Faction_Choice", Type::kInt, "0" },
			{ "Legacy_Faction_Choice", Type::kInt, "0" },
			{ "Radiant_Quests_Choice", Type::kInt, "0" },
			{ "Radiant_Quests_String", Type::kString, "$RadiantChoice01" },
			{ "State_RadiantCounterVal", Type::kInt, "5" },
			{ "State_BountyCounterVal", Type::kInt, "5" },
			{ "State_TGRCounterVal", Type::kInt, "5" },
			{ "State_DLC1CounterVal", Type::kInt, "5" },
			{ "State_LOTDCounterVal", Type::kInt, "5" },
			{ "State_VigilantCounterVal", Type::kInt, "5" },
			{ "State_CompanionsCounterVal", Type::kInt, "5" },
			{ "State_CollegeCounterVal", Type::kInt, "5" },
			{ "State_DBCounterVal", Type::kInt, "5" },
			{ "State_FishingCounterVal", Type::kInt, "4" },
			{ "State_BladesCounterVal", Type::kInt, "5" },

			// Achievements
			{ "AchievementsShow", Type::kBool, "true" },
			{ "AchievementsProgress", Type::kBool, "true" },
			{ "AchievementsTips", Type::kBool, "true" },
			{ "AchievementsRarity", Type::kBool, "true" },

			// Ingredients
			{ "IngredientsNameUnknown", Type::kBool, "true" },

			// UI Settings
			{ "ItemPictures", Type::kBool, "false" },
			{ "NotifyItems", Type::kBool, "true" },
			{ "NotifyBooks", Type::kBool, "true" },
			{ "NotifySpecial", Type::kBool, "true" },
			{ "IndexColour", Type::kInt, "16777215" },
			{ "ColourString", Type::kString, "#FFFFFF" },

			// HUD Settings and Museum Mode
			{ "MuseumModeEnabled", Type::kBool, "true" },
			{ "TreatOccupiedAsDisplayed", Type::kBool, "false" },
			{ "MuseumItemsOnly", Type::kBool, "false" },
			{ "State_OverRide_N_Name_String", Type::kString, "Need It!" },
			{ "State_OverRide_G_Name_String", Type::kString, "Got It!" },
			{ "State_OverRide_Displayable_Name_String", Type::kString, "Displayable!" },
			{ "State_OverRide_Displayed_Name_String", Type::kString, "Displayed!" },
			{ "State_OverRide_Occupied_Name_String", Type::kString, "Occupied!" },
			{ "State_ColourVal_N_HUD_Crosshair", Type::kInt, "4430046" },
			{ "State_ColourVal_G_HUD_Crosshair", Type::kInt, "1288220" },
			{ "State_ColourVal_Displayable", Type::kInt, "11796224" },
			{ "State_ColourVal_Displayed", Type::kInt, "8388736" },
			{ "State_ColourVal_Occupied", Type::kInt, "16753920" },
			{ "State_ColourString_N_HUD_Crosshair", Type::kString, "#4398DE" },
			{ "State_ColourString_G_HUD_Crosshair", Type::kString, "#13A81C" },
			{ "State_ColourString_Displayable", Type::kString, "#B3FF00" },
			{ "State_ColourString_Displayed", Type::kString, "#800080" },
			{ "State_ColourString_Occupied", Type::kString, "#FFA500" },
			{ "b_CustomColour_N_HUD_Crosshair", Type::kBool, "false" },
			{ "b_CustomColour_G_HUD_Crosshair", Type::kBool, "false" },
			{ "b_CustomColour_Displayable", Type::kBool, "false" },
			{ "b_CustomColour_Displayed", Type::kBool, "false" },
			{ "b_CustomColour_Occupied", Type::kBool, "false" },
			{ "State_CustomColourVal_N_HUD_Crosshair", Type::kInt, "-1" },
			{ "State_CustomColourVal_G_HUD_Crosshair", Type::kInt, "-1" },
			{ "State_CustomColourVal_Displayable", Type::kInt, "-1" },
			{ "State_CustomColourVal_Displayed", Type::kInt, "-1" },
			{ "State_CustomColourVal_Occupied", Type::kInt, "-1" },
			{ "State_CustomColourString_N_HUD_Crosshair", Type::kString, "Enter Decimal" },
			{ "State_CustomColourString_G_HUD_Crosshair", Type::kString, "Enter Decimal" },
			{ "State_CustomColourString_Displayable", Type::kString, "Enter Decimal" },
			{ "State_CustomColourString_Displayed", Type::kString, "Enter Decimal" },
			{ "State_CustomColourString_Occupied", Type::kString, "Enter Decimal" },
			{ "State_ColourVal_N_HUD_Menus", Type::kInt, "4430046" },
			{ "State_ColourVal_G_HUD_Menus", Type::kInt, "1288220" },
			{ "State_ColourString_N_HUD_Menus", Type::kString, "#4398DE" },
			{ "State_ColourString_G_HUD_Menus", Type::kString, "#13A81C" },
			{ "b_CustomColour_N_HUD_Menus", Type::kBool, "false" },
			{ "b_CustomColour_G_HUD_Menus", Type::kBool, "false" },
			{ "State_CustomColourVal_N_HUD_Menus", Type::kInt, "-1" },
			{ "State_CustomColourVal_G_HUD_Menus", Type::kInt, "-1" },
			{ "State_CustomColourString_N_HUD_Menus", Type::kString, "Enter Decimal" },
			{ "State_CustomColourString_G_HUD_Menus", Type::kString, "Enter Decimal" },
			{ "iMenuFormat_N", Type::kInt, "7" },
			{ "iMenuFormat_G", Type::kInt, "7" },
			{ "iMenuFormat_Displayable", Type::kInt, "7" },
			{ "iMenuFormat_Displayed", Type::kInt, "7" },
			{ "iMenuFormat_Occupied", Type::kInt, "7" },
			{ "iMenuPrefix_N", Type::kInt, "0" },
			{ "iMenuPrefix_G", Type::kInt, "0" },
			{ "iMenuPrefix_Displayable", Type::kInt, "0" },
			{ "iMenuPrefix_Displayed", Type::kInt, "0" },
			{ "iMenuPrefix_Occupied", Type::kInt, "0" },

			// Cell Scanner
			{ "bCellScanner_ExcludeAlchemy", Type::kBool, "false" },
			{ "bCellScanner_ExcludeIngredients", Type::kBool, "false" },
			{ "bCellScanner_ExcludeArmor", Type::kBool, "false" },
			{ "bCellScanner_ExcludeBooks", Type::kBool, "false" },
			{ "bCellScanner_ExcludeMiscItems", Type::kBool, "false" },
			{ "bCellScanner_ExcludeWeapons", Type::kBool, "false" },
			{ "bCellScanner_ExcludeOwned", Type::kBool, "false" },
			{ "bCellScanner_ExcludeLiving", Type::kBool, "true" },

			// Item icons
			{ "bItemIcons", Type::kBool, "true" },
			{ "iItemIcons_Size", Type::kInt, "100" },
			{ "iItemIcons_Opacity", Type::kInt, "100" },
			{ "bItemIcons_Needed", Type::kBool, "true" },
			{ "bItemIcons_Found", Type::kBool, "true" },
			{ "bItemIcons_Museum", Type::kBool, "true" },
			{ "iItemIconColour_New_Primary", Type::kInt, "16777215" },
			{ "sItemIconColour_New_Primary", Type::kString, "#FFFFFF" },
			{ "iItemIconColour_New_Secondary", Type::kInt, "6997502" },
			{ "sItemIconColour_New_Secondary", Type::kString, "#6AC5FE" },
			{ "iItemIconColour_Found_Primary", Type::kInt, "16777215" },
			{ "sItemIconColour_Found_Primary", Type::kString, "#FFFFFF" },
			{ "iItemIconColour_Found_Secondary", Type::kInt, "569168" },
			{ "sItemIconColour_Found_Secondary", Type::kString, "#08AF50" },
			{ "iItemIconColour_Displayable_Primary", Type::kInt, "16777215" },
			{ "sItemIconColour_Displayable_Primary", Type::kString, "#FFFFFF" },
			{ "iItemIconColour_Displayable_Secondary", Type::kInt, "6997502" },
			{ "sItemIconColour_Displayable_Secondary", Type::kString, "#6AC5FE" },
			{ "iItemIconColour_Displayed_Primary", Type::kInt, "16777215" },
			{ "sItemIconColour_Displayed_Primary", Type::kString, "#FFFFFF" },
			{ "iItemIconColour_Displayed_Secondary", Type::kInt, "569168" },
			{ "sItemIconColour_Displayed_Secondary", Type::kString, "#08AF50" },
			{ "iItemIconColour_Occupied_Primary", Type::kInt, "16777215" },
			{ "sItemIconColour_Occupied_Primary", Type::kString, "#FFFFFF" },
			{ "iItemIconColour_Occupied_Secondary", Type::kInt, "16027660" },
			{ "sItemIconColour_Occupied_Secondary", Type::kString, "#F4900C" },

			// Completionist sort, 1 first
			{ "iSort_Displayable", Type::kInt, "1" },
			{ "iSort_Occupied", Type::kInt, "2" },
			{ "iSort_Displayed", Type::kInt, "3" },
			{ "iSort_Needed", Type::kInt, "4" },
			{ "iSort_Found", Type::kInt, "5" },
			{ "bSort_Bands", Type::kBool, "true" },
			{ "iCellScanner_Range", Type::kInt, "100" },
			{ "bCellScanner_Pinning_Enabled", Type::kBool, "true" },
			{ "CellScanner_Pinning_Sound_Choice", Type::kInt, "1" },
		};

		struct Entry
		{
			const Default*          definition;
			RE::BSScript::Variable  value;
		};

		std::unordered_map<std::string, Entry> entries;	// lower case keys
		bool                                   loaded{ false };

		//---------------------------------------------------
		//-- Settings ( Key ) -------------------------------
		//---------------------------------------------------

		std::string Key(std::string_view a_name) {

			std::string key(a_name);
			std::ranges::transform(key, key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return key;
		}

		//---------------------------------------------------
		//-- Settings ( Assign ) ----------------------------
		//---------------------------------------------------

		void Assign(Entry& a_entry, std::string_view a_text) {

			switch (a_entry.definition->type) {
			case Type::kBool:
				a_entry.value.SetBool(DKUtil::string::iequals(a_text, "true") || a_text == "1");
				break;
			case Type::kInt:
				a_entry.value.SetSInt(static_cast<std::int32_t>(std::strtol(std::string(a_text).c_str(), nullptr, 10)));
				break;
			case Type::kString:
				a_entry.value.SetString(a_text);
				break;
			}
		}

		//---------------------------------------------------
		//-- Settings ( Text ) ------------------------------
		//---------------------------------------------------

		std::string Text(const Entry& a_entry) {

			switch (a_entry.definition->type) {
			case Type::kBool:
				return a_entry.value.GetBool() ? "true" : "false";
			case Type::kInt:
				return std::to_string(a_entry.value.GetSInt());
			default:
				return std::string(a_entry.value.GetString());
			}
		}
	}

	//---------------------------------------------------
	//-- Settings ( Load / Save ) -----------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Settings ( Load ) ------------------------------
	//---------------------------------------------------

	void SettingsAPI::Load() {

		CSimpleIniA ini;
		CSettingsFile::Load(ini);

		entries.clear();
		for (const auto& definition : defaults) {
			auto& entry = entries[Key(definition.name)];
			entry.definition = std::addressof(definition);
			const char* stored = ini.GetValue(section, definition.name, nullptr);
			Assign(entry, stored ? stored : definition.value);
		}
		loaded = true;
		INFO("Settings: loaded {} values", entries.size());
	}

	//---------------------------------------------------
	//-- Settings ( Save ) ------------------------------
	//---------------------------------------------------

	void SettingsAPI::Save() {

		CSimpleIniA ini;
		const SI_Error rc = CSettingsFile::LoadUser(ini);
		if (rc < 0 && rc != SI_FILE) {
			INFO("Settings: unable to load completionist.ini, not saved");
			return;
		}

		for (const auto& definition : defaults) {
			const auto& entry = entries.at(Key(definition.name));
			ini.SetValue(section, definition.name, Text(entry).c_str());
		}
		// Old MCM keys nothing reads now
		for (const auto* retired : { "State_MarkerDetectionVal", "CompletionLog_ColoursEnabled", "CompletionLog_PrefixEnabled", "CompletionLog_QuestsTextColourVal", "CompletionLog_QuestsTextColourString", "CompletionLog_ItemsTextColourVal", "CompletionLog_ItemsTextColourString", "CompletionLog_BooksTextColourVal", "CompletionLog_BooksTextColourString", "CompletionLog_SpecialTextColourVal", "CompletionLog_SpecialTextColourString", "bCellScanner_Pinning_Notify", "IngredientsShow", "ItemPicturesOffered", "AchievementsMissable",
			"NotifyStartup", "NotificationColourEnabled", "bCellScanner_Pinning_Marker", "bCellScanner_Pinning_Effect",
			"ImportedFromMCM", "b_SearchHighlightQuest", "i_SearchMaxResults", "ReferenceKey", "ExcludeKey", "ExcludeReferenceKey",
			"InventoryMode_PrAp_Choice_N", "InventoryMode_PrAp_Choice_G", "InventoryMode_PrAp_Choice_Displayable", "InventoryMode_PrAp_Choice_Displayed", "InventoryMode_PrAp_Choice_Occupied", "InventoryMode_PrFx_Choice_N", "InventoryMode_PrFx_Choice_G", "InventoryMode_PrFx_Choice_Displayable", "InventoryMode_PrFx_Choice_Displayed", "InventoryMode_PrFx_Choice_Occupied" }) {
			ini.Delete(section, retired);
		}

		if (CSettingsFile::SaveUser(ini) < 0) {
			INFO("Settings: failed to save completionist.ini");
		}
	}

	//---------------------------------------------------
	//-- Settings ( Access ) ----------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Settings ( Get ) -------------------------------
	//---------------------------------------------------

	RE::BSScript::Variable* SettingsAPI::Get(std::string_view a_name) {

		if (!loaded) {
			Load();
		}
		const auto it = entries.find(Key(a_name));
		return it != entries.end() ? std::addressof(it->second.value) : nullptr;
	}

	//---------------------------------------------------
	//-- Settings ( Has ) -------------------------------
	//---------------------------------------------------

	bool SettingsAPI::Has(std::string_view a_name) {

		return Get(a_name) != nullptr;
	}

	//---------------------------------------------------
	//-- Settings ( Default Int ) -----------------------
	//---------------------------------------------------

	std::int32_t SettingsAPI::DefaultInt(std::string_view a_name) {

		for (const auto& definition : defaults) {
			if (definition.type == Type::kInt && Key(definition.name) == Key(a_name)) {
				return static_cast<std::int32_t>(std::strtol(definition.value, nullptr, 10));
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Settings ( Set Bool ) --------------------------
	//---------------------------------------------------

	void SettingsAPI::SetBool(std::string_view a_name, bool a_value) {

		if (auto* value = Get(a_name); value && value->IsBool()) {
			value->SetBool(a_value);
		}
	}

	//---------------------------------------------------
	//-- Settings ( Set Int ) ---------------------------
	//---------------------------------------------------

	void SettingsAPI::SetInt(std::string_view a_name, std::int32_t a_value) {

		if (auto* value = Get(a_name); value && value->IsInt()) {
			value->SetSInt(a_value);
		}
	}

	//---------------------------------------------------
	//-- Settings ( Set String ) ------------------------
	//---------------------------------------------------

	void SettingsAPI::SetString(std::string_view a_name, std::string_view a_value) {

		if (auto* value = Get(a_name); value && value->IsString()) {
			value->SetString(a_value);
		}
	}

}
