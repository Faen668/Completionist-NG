#include "Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Events.hpp"
#include "Settings.hpp"
#include "SimpleIni.h"
#include "UI/Core/Themes.hpp"

namespace CVariables
{

	void VariablesAPI::Register()
	{
		//Register menu open to periodically clear the formattedStringHolder.
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(&OnMenuOpenCloseEvent);
		V_Global_Patch_Logging = VariablesAPI::IsGlobalPatchInstallLoggingEnabled();
		V_TrackedEntityTypes = VariablesAPI::GetTrackedEntityTypes();
		V_RestrictBooksToTomesAndSkillBooks = VariablesAPI::GetRestrictBooksToTomesAndSkillBooks();
		CSettings::SettingsAPI::Load();
	};

	//---------------------------------------------------
	//-- Variables Functions ( On Menu Open / Close ) ---
	//---------------------------------------------------

	void VariablesAPI::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event)
	{
		if (!a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) {
			Update();
		};
	}

	//---------------------------------------------------
	//-- Variables Functions ( Get Setting ) ------------
	//---------------------------------------------------

	RE::BSScript::Variable* VariablesAPI::GetProperty(const char* a_prop)
	{
		auto* setting = a_prop ? CSettings::SettingsAPI::Get(a_prop) : nullptr;
		if (!setting) {
			INFO("Variables API: no setting named [{}]", a_prop ? a_prop : "");
		}
		return setting;
	};

	//---------------------------------------------------
	//-- Variables Functions ( Is Debugging Enabled ) ---
	//---------------------------------------------------

	bool VariablesAPI::IsDebuggingEnabled()
	{
		return V_Debugging;
	};

	bool VariablesAPI::GetIniBoolValue(const std::string& key)
	{
		CSimpleIniA ini;
		const char* completionistSection = "Completionist";
		const char* iniFilePath = "Data/SKSE/Plugins/Completionist.ini";

		// Load the INI file
		SI_Error rc = ini.LoadFile(iniFilePath);

		// If the file doesn't exist, create it and add the required lines
		if (rc == SI_FILE) {
			// Create the section and set the value
			ini.SetValue(completionistSection, key.c_str(), "false");

			// Save the INI file
			if (ini.SaveFile(iniFilePath) < 0) {
				INFO("Failed to create and save completionist.ini");
				return false;
			}

			// Log creation
			INFO("Created completionist.ini");
			return false;
		}

		// If there was another error loading the file, log it and return false
		if (rc < 0) {
			INFO("Unable to load completionist.ini");
			return false;
		}

		// If the section or key doesn't exist, add them
		if (!ini.GetSection(completionistSection) || !ini.GetValue(completionistSection, key.c_str())) {
			ini.SetValue(completionistSection, key.c_str(), "false");

			// Save the INI file
			if (ini.SaveFile(iniFilePath) < 0) {
				INFO("Failed to save completionist.ini");
				return false;
			}

			// Log addition
			INFO("Added missing section or key to completionist.ini");
			return false;
		}

		// Return the value
		return ini.GetBoolValue(completionistSection, key.c_str(), false);
	}

	std::string VariablesAPI::GetIniStringValue(const std::string& key)
	{
		CSimpleIniA ini;
		const char* completionistSection = "Completionist";
		const char* iniFilePath = "Data/SKSE/Plugins/Completionist.ini";
		const char* defaultValue = "0,1,2,3,4,5,6,7,8,9";
		// Load the INI file
		SI_Error rc = ini.LoadFile(iniFilePath);
		// If the file doesn't exist, create it and add the required lines
		if (rc == SI_FILE) {
			// Create the section and set the value
			ini.SetValue(completionistSection, key.c_str(), defaultValue);
			// Save the INI file
			if (ini.SaveFile(iniFilePath) < 0) {
				INFO("Failed to create and save completionist.ini");
				return defaultValue;
			}
			// Log creation
			INFO("Created completionist.ini");
			return defaultValue;
		}
		// If there was another error loading the file, log it and return the default
		if (rc < 0) {
			INFO("Unable to load completionist.ini");
			return defaultValue;
		}
		// If the section or key doesn't exist, add them
		if (!ini.GetSection(completionistSection) || !ini.GetValue(completionistSection, key.c_str()))
		{
			ini.SetValue(completionistSection, key.c_str(), defaultValue);

			// Save the INI file
			if (ini.SaveFile(iniFilePath) < 0) {
				INFO("Failed to save completionist.ini");
				return defaultValue;
			}

			// Log addition
			INFO("Added missing section or key to completionist.ini");
			return defaultValue;
		}
		return ini.GetValue(completionistSection, key.c_str(), "");
	}

	//---------------------------------------------------
	//-- Variables Functions ( Generic INI Value ) ------
	//---------------------------------------------------

	std::string VariablesAPI::GetIniValue(const char* a_section, const std::string& a_key, const char* a_default)
	{
		CSimpleIniA ini;
		const char* iniFilePath = "Data/SKSE/Plugins/Completionist.ini";

		const SI_Error rc = ini.LoadFile(iniFilePath);
		if (rc < 0 && rc != SI_FILE) {
			INFO("Unable to load completionist.ini");
			return a_default;
		}

		if (const auto* value = ini.GetValue(a_section, a_key.c_str())) {
			return value;
		}

		// Write the default back so the key shows up for users to edit.
		ini.SetValue(a_section, a_key.c_str(), a_default);
		if (ini.SaveFile(iniFilePath) < 0) {
			INFO("Failed to save completionist.ini");
		}
		return a_default;
	}

	//---------------------------------------------------
	//-- Variables Functions ( In-Game UI Settings ) ----
	//---------------------------------------------------

	CompletionistUI::UISettings VariablesAPI::GetUISettings()
	{
		const auto flag = [](const std::string& a_key, const char* a_default) { return DKUtil::string::iequals(GetIniValue("UI", a_key, a_default), "true"); };
		const auto number = [](const std::string& a_key, const char* a_default, float a_min, float a_max) { return std::clamp(std::strtof(GetIniValue("UI", a_key, a_default).c_str(), nullptr), a_min, a_max); };

		CompletionistUI::UISettings settings;
		settings.openKey = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "OpenKey", "199").c_str(), nullptr, 10));
		settings.openPadButton = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "OpenGamepadButton", "512").c_str(), nullptr, 10));
		settings.openPadModifier = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "OpenGamepadModifier", "256").c_str(), nullptr, 10));
		settings.scale = number("Scale", "1.0", 0.5f, 3.0f);
		settings.widthPercent = number("WidthPercent", "92", 40.0f, 100.0f);
		settings.heightPercent = number("HeightPercent", "88", 40.0f, 100.0f);
		settings.opacity = number("Opacity", "0.95", 0.3f, 1.0f);
		settings.columns = static_cast<std::int32_t>(number("Columns", "0", 0.0f, 4.0f));
		settings.descriptionLines = static_cast<std::int32_t>(number("DescriptionLines", "5", 0.0f, static_cast<float>(CompletionistUI::UISettings::maxDescriptionLines)));
		settings.descriptionCollapsed = flag("DescriptionCollapsed", "false");
		if (settings.descriptionLines == 0) {
			// 0 used to hide the panel
			settings.descriptionLines = 5;
			settings.descriptionCollapsed = true;
		}
		settings.navigationCollapsed = flag("NavigationCollapsed", "false");
		settings.findMatch = static_cast<std::int32_t>(number("FindMatch", "0", 0.0f, 2.0f));
		settings.findIgnoreCompleted = flag("FindIgnoreCompleted", "false");
		settings.openOnLastPage = flag("OpenOnLastPage", "true");
		settings.hideCompleted = flag("HideCompleted", "false");
		settings.screenshotButton = flag("ShowScreenshotButton", "false");
		settings.printScreen = flag("PrintScreenCapture", "true");
		const auto padStyle = GetIniValue("UI", "ControllerButtons", "Xbox");
		settings.padStyle = DKUtil::string::iequals(padStyle, "PlayStation") ? CompletionistUI::PadStyle::kPlayStation
		                  : DKUtil::string::iequals(padStyle, "Switch")      ? CompletionistUI::PadStyle::kSwitch
		                  : DKUtil::string::iequals(padStyle, "SteamDeck")   ? CompletionistUI::PadStyle::kSteamDeck
		                                                                     : CompletionistUI::PadStyle::kXbox;
		settings.cellKey = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "CellKey", "207").c_str(), nullptr, 10));
		settings.cellPadButton = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "CellGamepadButton", "0").c_str(), nullptr, 10));
		settings.cellPadModifier = static_cast<std::uint32_t>(std::strtoul(GetIniValue("UI", "CellGamepadModifier", "0").c_str(), nullptr, 10));
		settings.cellPanel = flag("CellPanel", "false");
		settings.cellOptionsOpen = flag("CellOptionsOpen", "false");
		settings.cellHeight = flag("CellShowHeight", "true");
		settings.toasts = flag("Notifications", "false");
		settings.toastsNewestFirst = flag("NotificationsNewestFirst", "true");
		settings.toastX = number("NotificationsX", "0", 0.0f, 1.0f);
		settings.toastY = number("NotificationsY", "0", 0.0f, 1.0f);

		const auto theme = GetIniValue("UI", "Theme", "Completionist");
		settings.theme = CompletionistUI::Themes::FromName(theme);
		return settings;
	}

	void VariablesAPI::SaveUISettings(const CompletionistUI::UISettings& a_settings)
	{
		CSimpleIniA ini;
		const char* iniFilePath = "Data/SKSE/Plugins/Completionist.ini";
		const char* section = "UI";

		const SI_Error rc = ini.LoadFile(iniFilePath);
		if (rc < 0 && rc != SI_FILE) {
			INFO("Unable to load completionist.ini, UI settings not saved");
			return;
		}

		const auto theme = std::string(CompletionistUI::Themes::Name(a_settings.theme));
		ini.SetValue(section, "OpenKey", std::to_string(a_settings.openKey).c_str());
		ini.SetValue(section, "OpenGamepadButton", std::to_string(a_settings.openPadButton).c_str());
		ini.SetValue(section, "OpenGamepadModifier", std::to_string(a_settings.openPadModifier).c_str());
		ini.SetValue(section, "Theme", theme.c_str());
		ini.SetValue(section, "Scale", fmt::format("{:.2f}", a_settings.scale).c_str());
		ini.SetValue(section, "WidthPercent", fmt::format("{:.0f}", a_settings.widthPercent).c_str());
		ini.SetValue(section, "HeightPercent", fmt::format("{:.0f}", a_settings.heightPercent).c_str());
		ini.SetValue(section, "Opacity", fmt::format("{:.2f}", a_settings.opacity).c_str());
		ini.SetValue(section, "Columns", std::to_string(a_settings.columns).c_str());
		ini.SetValue(section, "DescriptionLines", std::to_string(a_settings.descriptionLines).c_str());
		ini.SetValue(section, "DescriptionCollapsed", a_settings.descriptionCollapsed ? "true" : "false");
		ini.SetValue(section, "NavigationCollapsed", a_settings.navigationCollapsed ? "true" : "false");
		ini.SetValue(section, "FindMatch", std::to_string(a_settings.findMatch).c_str());
		ini.SetValue(section, "FindIgnoreCompleted", a_settings.findIgnoreCompleted ? "true" : "false");
		ini.SetValue(section, "OpenOnLastPage", a_settings.openOnLastPage ? "true" : "false");
		ini.SetValue(section, "HideCompleted", a_settings.hideCompleted ? "true" : "false");
		ini.SetValue(section, "ShowScreenshotButton", a_settings.screenshotButton ? "true" : "false");
		ini.SetValue(section, "PrintScreenCapture", a_settings.printScreen ? "true" : "false");
		constexpr const char* padStyles[] = { "Xbox", "PlayStation", "Switch", "SteamDeck" };
		ini.SetValue(section, "ControllerButtons", padStyles[static_cast<int>(a_settings.padStyle)]);
		ini.SetValue(section, "CellKey", std::to_string(a_settings.cellKey).c_str());
		ini.SetValue(section, "CellGamepadButton", std::to_string(a_settings.cellPadButton).c_str());
		ini.SetValue(section, "CellGamepadModifier", std::to_string(a_settings.cellPadModifier).c_str());
		ini.SetValue(section, "CellPanel", a_settings.cellPanel ? "true" : "false");
		ini.SetValue(section, "CellOptionsOpen", a_settings.cellOptionsOpen ? "true" : "false");
		ini.SetValue(section, "CellShowHeight", a_settings.cellHeight ? "true" : "false");
		ini.SetValue(section, "Notifications", a_settings.toasts ? "true" : "false");
		ini.SetValue(section, "NotificationsNewestFirst", a_settings.toastsNewestFirst ? "true" : "false");
		ini.SetValue(section, "NotificationsX", fmt::format("{:.3f}", a_settings.toastX).c_str());
		ini.SetValue(section, "NotificationsY", fmt::format("{:.3f}", a_settings.toastY).c_str());

		if (ini.SaveFile(iniFilePath) < 0) {
			INFO("Failed to save UI settings to completionist.ini");
			return;
		}
		INFO("Saved UI settings to completionist.ini");
	}

	//---------------------------------------------------
	//-- Variables Functions ( Is MuseumAPI Enabled ) ---
	//---------------------------------------------------

	bool VariablesAPI::IsMuseumAPILoggingEnabled()
	{
		return GetIniBoolValue("MuseumAPILogging");
	}

	//---------------------------------------------------
	//-- Variables Functions ( Book Restrictions ) ------
	//---------------------------------------------------

	bool VariablesAPI::GetRestrictBooksToTomesAndSkillBooks()
	{
		return GetIniBoolValue("RestrictBooksToTomesAndSkillBooks");
	}
	
	//---------------------------------------------------
	//-- Variables Functions ( Global Patch Logging ) ---
	//---------------------------------------------------

	bool VariablesAPI::IsGlobalPatchInstallLoggingEnabled()
	{
		return GetIniBoolValue("Log_All_Patch_Installs");
	}

	/// <summary>
	/// Gets the Tracked Entity Types from the INI, splits them by comma, and returns them as a vector of strings.
	/// </summary>
	/// <returns></returns>
	std::vector<CMiscPatchType> VariablesAPI::GetTrackedEntityTypes()
	{
		std::vector<CMiscPatchType> entityTypes;
		std::string trackedTypesStr = GetIniStringValue("TrackedEntityTypes");
		std::istringstream ss(trackedTypesStr);
		std::string type;

		while (std::getline(ss, type, ',')) {
			type.erase(std::remove_if(type.begin(), type.end(), ::isspace), type.end());

			if (!type.empty()) {
				try {
					entityTypes.push_back(static_cast<CMiscPatchType>(std::stoi(type)));
				}
				catch (const std::exception&) {
					// Optional: log invalid entry
					// e.g. "Invalid TrackedEntityType: " + type
					INFO("Invalid TrackedEntityType: {}", type);
				}
			}
		}

		return entityTypes;
	}

	//---------------------------------------------------
	//-- Variables Functions ( Update Properties ) ------
	//---------------------------------------------------

	void VariablesAPI::Update()
	{
		V_TreatBooksAsItems = false;
		if (const auto* prop = VariablesAPI::GetProperty("TreatBooksAsItems")) {
			V_TreatBooksAsItems = prop->GetBool();
		}

		V_CellScanner_CONT = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_CONT")) {
			V_CellScanner_CONT = prop->GetBool();
		}

		V_CellScanner_REFS = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_REFS")) {
			V_CellScanner_REFS = prop->GetBool();
		}

		V_CellScanner_NPCS = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_NPCS")) {
			V_CellScanner_NPCS = prop->GetBool();
		}

		V_CellScanner_NUMB = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_NUMB")) {
			V_CellScanner_NUMB = prop->GetBool();
		}


		V_CellScanner_Markers = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_MARK")) {
			V_CellScanner_Markers = prop->GetBool();
		}

		V_CellScanner_Closest = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_UseClosestReference")) {
			V_CellScanner_Closest = prop->GetBool();
		}

		V_CellScanner_ExcludeAlchemy = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeAlchemy")) {
			V_CellScanner_ExcludeAlchemy = prop->GetBool();
		}

		V_CellScanner_ExcludeArmor = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeArmor")) {
			V_CellScanner_ExcludeArmor = prop->GetBool();
		}

		V_CellScanner_ExcludeBooks = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeBooks")) {
			V_CellScanner_ExcludeBooks = prop->GetBool();
		}

		V_CellScanner_ExcludeMiscItems = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeMiscItems")) {
			V_CellScanner_ExcludeMiscItems = prop->GetBool();
		}

		V_CellScanner_ExcludeWeapons = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeWeapons")) {
			V_CellScanner_ExcludeWeapons = prop->GetBool();
		}

		V_CellScanner_ExcludeOwned = false;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_ExcludeOwned")) {
			V_CellScanner_ExcludeOwned = prop->GetBool();
		}

		const auto flag = [](const char* a_name, bool a_default) {
			const auto* prop = VariablesAPI::GetProperty(a_name);
			return prop ? prop->GetBool() : a_default;
		};
		const auto percent = [](const char* a_name, std::int32_t a_min, std::int32_t a_max) {
			const auto* prop = VariablesAPI::GetProperty(a_name);
			return std::clamp(prop ? prop->GetSInt() : 100, a_min, a_max);
		};
		V_ItemIcons = flag("bItemIcons", true);
		V_ItemIcons_Size = percent("iItemIcons_Size", 50, 200);
		V_ItemIcons_Opacity = percent("iItemIcons_Opacity", 20, 100);
		V_ItemIcons_Needed = flag("bItemIcons_Needed", true);
		V_ItemIcons_Found = flag("bItemIcons_Found", true);
		V_ItemIcons_Museum = flag("bItemIcons_Museum", true);

		const auto priority = [](const char* a_name, std::int32_t a_default) {
			const auto* prop = VariablesAPI::GetProperty(a_name);
			return std::clamp(prop ? prop->GetSInt() : a_default, 1, 5);
		};
		V_Sort_Displayable = priority("iSort_Displayable", 1);
		V_Sort_Occupied = priority("iSort_Occupied", 2);
		V_Sort_Displayed = priority("iSort_Displayed", 3);
		V_Sort_Needed = priority("iSort_Needed", 4);
		V_Sort_Found = priority("iSort_Found", 5);
		V_Sort_Bands = flag("bSort_Bands", true);

		const auto choice = [](const char* a_name, std::int32_t a_default, std::int32_t a_max) {
			const auto* prop = VariablesAPI::GetProperty(a_name);
			return std::clamp(prop ? prop->GetSInt() : a_default, 0, a_max);
		};
		V_MenuFormat_N = choice("iMenuFormat_N", 7, 7);
		V_MenuFormat_G = choice("iMenuFormat_G", 7, 7);
		V_MenuFormat_Displayable = choice("iMenuFormat_Displayable", 7, 7);
		V_MenuFormat_Displayed = choice("iMenuFormat_Displayed", 7, 7);
		V_MenuFormat_Occupied = choice("iMenuFormat_Occupied", 7, 7);
		V_MenuPrefix_N = choice("iMenuPrefix_N", 0, 2);
		V_MenuPrefix_G = choice("iMenuPrefix_G", 0, 2);
		V_MenuPrefix_Displayable = choice("iMenuPrefix_Displayable", 0, 2);
		V_MenuPrefix_Displayed = choice("iMenuPrefix_Displayed", 0, 2);
		V_MenuPrefix_Occupied = choice("iMenuPrefix_Occupied", 0, 2);

		// Custom colour wins, as on the crosshair
		const auto colour = [](const char* a_custom, const char* a_customString, const char* a_string, const char* a_default) {
			const auto* custom = VariablesAPI::GetProperty(a_custom);
			const auto* prop = VariablesAPI::GetProperty(custom && custom->GetBool() ? a_customString : a_string);
			return std::string(prop ? prop->GetString() : a_default);
		};
		V_MenuColour_N = colour("b_CustomColour_N_HUD_Menus", "State_CustomColourString_N_HUD_Menus", "State_ColourString_N_HUD_Menus", "#4398DE");
		V_MenuColour_G = colour("b_CustomColour_G_HUD_Menus", "State_CustomColourString_G_HUD_Menus", "State_ColourString_G_HUD_Menus", "#13A81C");

		V_CellScanner_ScanRange = 100;
		if (const auto* prop = VariablesAPI::GetProperty("iCellScanner_Range")) {
			V_CellScanner_ScanRange = prop->GetSInt();
		}

		V_NotificationText = "Completionist: Entry Complete - ";

		V_notificationTextCellScan0 = " Has ";

		V_notificationTextCellScan1 = " Collectable Items!";

		V_notificationTextCellScan2 = " Has Collectable Items!";

		V_notificationTextCellScan3 = "Collectables";

		V_notificationTextCellScan4 = "Loose Items";

		V_notificationTextCellScan5 = "Items On NPC's";

		V_notificationTextCellScan6 = "Items In Containers";

		V_NotificationTextShout0 = "Completionist: ";

		V_NotificationTextShout1 = " - Word 1 (";

		V_NotificationTextShout2 = " - Word 2 (";

		V_NotificationTextShout3 = ") - Learnt!";

		V_NotificationTextBarenziah = "Completionist: Entry Complete - Stone of Barenziah!";

		V_CrosshairTag_New = "Need It!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_N_Name_String")) {
			V_CrosshairTag_New = prop->GetString();
		}

		V_CrosshairTag_Found = "Got It!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_G_Name_String")) {
			V_CrosshairTag_Found = prop->GetString();
		}

		V_CrosshairTag_Displayable = "Displayable!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_Displayable_Name_String")) {
			V_CrosshairTag_Displayable = prop->GetString();
		}

		V_CrosshairTag_Displayed = "Displayed!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_Displayed_Name_String")) {
			V_CrosshairTag_Displayed = prop->GetString();
		}

		V_CrosshairTag_Occupied = "Occupied!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_Occupied_Name_String")) {
			V_CrosshairTag_Occupied = prop->GetString();
		}

		V_HUD_Override_Enabled_New_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_N_HUD_Crosshair")) {
			V_HUD_Override_Enabled_New_Crosshair = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_G_HUD_Crosshair")) {
			V_HUD_Override_Enabled_Found_Crosshair = prop->GetBool();
		}

		V_HUD_Override_Enabled_Displayable = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_Displayable")) {
			V_HUD_Override_Enabled_Displayable = prop->GetBool();
		}

		V_HUD_Override_Enabled_Displayed = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_Displayed")) {
			V_HUD_Override_Enabled_Displayed = prop->GetBool();
		}

		V_HUD_Override_Enabled_Occupied = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_Occupied")) {
			V_HUD_Override_Enabled_Occupied = prop->GetBool();
		}

		V_HUD_Colour_New_Crosshair = 4430046;
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourVal_N_HUD_Crosshair")) {
			V_HUD_Colour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_Colour_Found_Crosshair = 1288220;
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourVal_G_HUD_Crosshair")) {
			V_HUD_Colour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_ColourString_New_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_N_HUD_Crosshair")) {
			V_HUD_ColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_ColourString_Found_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_G_HUD_Crosshair")) {
			V_HUD_ColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_ColourString_Displayable = "#b40000";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_Displayable")) {
			V_HUD_ColourString_Displayable = prop->GetString();
		}

		V_HUD_ColourString_Displayed = "#8000ff";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_Displayed")) {
			V_HUD_ColourString_Displayed = prop->GetString();
		}

		V_HUD_ColourString_Occupied = "#ffaa00";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_Occupied")) {
			V_HUD_ColourString_Occupied = prop->GetString();
		}

		V_HUD_CustomColour_New_Crosshair = -1;
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourVal_N_HUD_Crosshair")) {
			V_HUD_CustomColour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Crosshair = -1;
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourVal_G_HUD_Crosshair")) {
			V_HUD_CustomColour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_N_HUD_Crosshair")) {
			V_HUD_CustomColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_G_HUD_Crosshair")) {
			V_HUD_CustomColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_CustomColourString_Displayable = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_Displayable")) {
			V_HUD_CustomColourString_Displayable = prop->GetString();
		}

		V_HUD_CustomColourString_Displayed = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_Displayed")) {
			V_HUD_CustomColourString_Displayed = prop->GetString();
		}

		V_HUD_CustomColourString_Occupied = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_Occupied")) {
			V_HUD_CustomColourString_Occupied = prop->GetString();
		}

		V_ShoutColour_New = "";
		if (const auto* prop = VariablesAPI::GetProperty("StateShout_NotColourString")) {
			V_ShoutColour_New = prop->GetString();
		}

		V_ShoutColour_Found = "";
		if (const auto* prop = VariablesAPI::GetProperty("StateShout_GotColourString")) {
			V_ShoutColour_Found = prop->GetString();
		}

		V_Radiant_FavorVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_RadiantCounterVal")) {
			V_Radiant_FavorVal = prop->GetSInt();
		}

		V_Radiant_CollegeVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_CollegeCounterVal")) {
			V_Radiant_CollegeVal = prop->GetSInt();
		}

		V_Radiant_CompanionsVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_CompanionsCounterVal")) {
			V_Radiant_CompanionsVal = prop->GetSInt();
		}

		V_Radiant_DBrotherhoodVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_DBCounterVal")) {
			V_Radiant_DBrotherhoodVal = prop->GetSInt();
		}

		V_Radiant_DawnguardVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_DLC1CounterVal")) {
			V_Radiant_DawnguardVal = prop->GetSInt();
		}

		V_Radiant_ThievesGuildVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_TGRCounterVal")) {
			V_Radiant_ThievesGuildVal = prop->GetSInt();
		}

		V_Radiant_BountyVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_BountyCounterVal")) {
			V_Radiant_BountyVal = prop->GetSInt();
		}

		V_Radiant_VigilantVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_VigilantCounterVal")) {
			V_Radiant_VigilantVal = prop->GetSInt();
		}

		V_Radiant_LegacyVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_LOTDCounterVal")) {
			V_Radiant_LegacyVal = prop->GetSInt();
		}

		V_Radiant_FishingVal = 4;
		if (const auto* prop = VariablesAPI::GetProperty("State_FishingCounterVal")) {
			V_Radiant_FishingVal = prop->GetSInt();
		}

		V_Radiant_BladesVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty("State_BladesCounterVal")) {
			V_Radiant_BladesVal = prop->GetSInt();
		}

		V_Debugging = false;
		if (const auto* prop = VariablesAPI::GetProperty("bDebug")) {
			V_Debugging = prop->GetBool();
		}

		V_MuseumModeEnabled = false;
		if (const auto* prop = VariablesAPI::GetProperty("MuseumModeEnabled")) {
			V_MuseumModeEnabled = prop->GetBool();
		}

		V_TreatOccupiedAsDisplayed = false;
		if (const auto* prop = VariablesAPI::GetProperty("TreatOccupiedAsDisplayed")) {
			V_TreatOccupiedAsDisplayed = prop->GetBool();
		}
	}
}