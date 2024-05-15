#include "Variables.hpp"
#include "ScriptObject.hpp"
#include "DKUtil/Utility.hpp"
#include "SimpleIni.h"

static ScriptObjectPtr MCM;

namespace CVariables {

	void VariablesAPI::Register() {
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(VariablesAPI::GetSingleton()));
	};

	//---------------------------------------------------
	//-- Variables Functions ( Set MCM Pointer ) --------
	//---------------------------------------------------

	void VariablesAPI::SetFrameworkQuest(RE::StaticFunctionTag*, RE::TESQuest* a_quest)
	{
		if (!a_quest)
		{
			INFO("No Quest Passed To Registration Function");
			return;
		};

		MCM = ScriptObject::FromForm(a_quest, "Completionist_MCMScript");
		if (!MCM) 
		{
			INFO("Unable To Locate MCM Script on Form");
			return;
		};

		INFO("MCM Pointer Set Successfully");
		VariablesAPI::Update();
	};

	//---------------------------------------------------
	//-- Variables Functions ( On Menu Open / Close ) ---
	//---------------------------------------------------

	EventResult	VariablesAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) 
	{
		if (!a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) 
		{
			Update();
		};

		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Variables Functions ( Get MCM Property ) -------
	//---------------------------------------------------

	RE::BSScript::Variable* VariablesAPI::GetProperty(const char* a_prop) 
	{
		if (!MCM)
		{
			return nullptr;
		};

		if (!a_prop || !MCM->GetProperty(a_prop)) {
			INFO("Variables API: Unable To Get Property - [{}]", a_prop);
			return nullptr;
		};

		return MCM->GetProperty(a_prop);
	};

	//---------------------------------------------------
	//-- Variables Functions ( Is Debugging Enabled ) ---
	//---------------------------------------------------

	bool VariablesAPI::IsDebuggingEnabled()
	{
		return V_Debugging;
	};

	//---------------------------------------------------
	//-- Variables Functions ( Linux Compatibility ) ----
	//---------------------------------------------------

	bool VariablesAPI::IsUsingLinux() {
		CSimpleIniA ini;
		const char* completionistSection = "Completionist";
		const char* linuxKey = "Is_Using_Linux";
		const char* iniFilePath = "Data/SKSE/Plugins/Completionist.ini";

		// Load the INI file
		SI_Error rc = ini.LoadFile(iniFilePath);

		// If the file doesn't exist, create it and add the required lines
		if (rc < 0 && rc != SI_FILE) {
			// Create the section and set the value
			ini.SetValue(completionistSection, linuxKey, "false");

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
		if (!ini.GetSection(completionistSection) || !ini.GetValue(completionistSection, linuxKey)) {
			ini.SetValue(completionistSection, linuxKey, "false");

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
		return ini.GetBoolValue(completionistSection, linuxKey, false);
	}

	//---------------------------------------------------
	//-- Variables Functions ( Update Properties ) ------
	//---------------------------------------------------

	void VariablesAPI::Update() 
	{
		TCC_New = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558285, "DBM_RelicNotifications.esp");
		TCC_FND = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558286, "DBM_RelicNotifications.esp");
		TCC_DSP = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558287, "DBM_RelicNotifications.esp");

		if (!MCM)
		{
			INFO("[Update] Script Pointer Not Set.");
		};

		V_TreatBooksAsItems = false;
		if (const auto* prop = VariablesAPI::GetProperty("TreatBooksAsItems")) {
			V_TreatBooksAsItems = prop->GetBool();
		}

		V_FishingSpotMarkers = true;
		if (const auto* prop = VariablesAPI::GetProperty("FishingSpotMarkers")) {
			V_FishingSpotMarkers = prop->GetBool();
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

		V_CellScanner_DETA = true;
		if (const auto* prop = VariablesAPI::GetProperty("bCellScanner_DETA")) {
			V_CellScanner_DETA = prop->GetBool();
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

		V_CellScanner_ScanRange = 100;
		if (const auto* prop = VariablesAPI::GetProperty("iCellScanner_Range")) {
			V_CellScanner_ScanRange = prop->GetSInt();
		}

		V_NotificationText = "Completionist: Entry Complete - ";
		if (const auto* prop = VariablesAPI::GetProperty("notificationText")) {
			V_NotificationText = prop->GetString();
		}

		V_notificationTextCellScan0 = " Has ";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan0")) {
			V_notificationTextCellScan0 = prop->GetString();
		}

		V_notificationTextCellScan1 = " Collectable Items!";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan1")) {
			V_notificationTextCellScan1 = prop->GetString();
		}

		V_notificationTextCellScan2 = " Has Collectable Items!";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan2")) {
			V_notificationTextCellScan2 = prop->GetString();
		}

		V_notificationTextCellScan3 = "Collectables";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan3")) {
			V_notificationTextCellScan3 = prop->GetString();
		}

		V_notificationTextCellScan4 = "Loose Items";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan4")) {
			V_notificationTextCellScan4 = prop->GetString();
		}

		V_notificationTextCellScan5 = "Items On NPC's";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan5")) {
			V_notificationTextCellScan5 = prop->GetString();
		}

		V_notificationTextCellScan6 = "Items In Containers";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextCellScan6")) {
			V_notificationTextCellScan6 = prop->GetString();
		}

		V_NotificationTextShout0 = "Completionist: ";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextShouts0")) {
			V_NotificationTextShout0 = prop->GetString();
		}

		V_NotificationTextShout1 = " - Word 1 (";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextShouts1")) {
			V_NotificationTextShout1 = prop->GetString();
		}

		V_NotificationTextShout2 = " - Word 2 (";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextShouts2")) {
			V_NotificationTextShout2 = prop->GetString();
		}

		V_NotificationTextShout3 = ") - Learnt!";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextShouts3")) {
			V_NotificationTextShout3 = prop->GetString();
		}

		V_NotificationTextBarenziah = "Completionist: Entry Complete - Stone of Barenziah!";
		if (const auto* prop = VariablesAPI::GetProperty("notificationTextBarenziah")) {
			V_NotificationTextBarenziah = prop->GetString();
		}

		V_TextChoice_N = 4;
		if (const auto* prop = VariablesAPI::GetProperty("InventoryMode_PrAP_Choice_N")) {
			V_TextChoice_N = prop->GetSInt();
		}

		V_TextChoice_G = 4;
		if (const auto* prop = VariablesAPI::GetProperty("InventoryMode_PrAP_Choice_G")) {
			V_TextChoice_G = prop->GetSInt();
		}

		V_PrefixChoice_N = 3;
		if (const auto* prop = VariablesAPI::GetProperty("InventoryMode_PrFx_Choice_N")) {
			V_PrefixChoice_N = prop->GetSInt();
		}

		V_PrefixChoice_G = 3;
		if (const auto* prop = VariablesAPI::GetProperty("InventoryMode_PrFx_Choice_G")) {
			V_PrefixChoice_G = prop->GetSInt();
		}

		V_CrosshairTag_New = "Need It!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_N_Name_String")) {
			V_CrosshairTag_New = prop->GetString();
		}

		V_CrosshairTag_Found = "Got It!";
		if (const auto* prop = VariablesAPI::GetProperty("State_OverRide_G_Name_String")) {
			V_CrosshairTag_Found = prop->GetString();
		}

		V_HUD_Override_Enabled_New_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_N_HUD_Crosshair")) {
			V_HUD_Override_Enabled_New_Crosshair = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_G_HUD_Crosshair")) {
			V_HUD_Override_Enabled_Found_Crosshair = prop->GetBool();
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

		V_HUD_Override_Enabled_New_Menus = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_N_HUD_Menus")) {
			V_HUD_Override_Enabled_New_Menus = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Menus = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_CustomColour_G_HUD_Menus")) {
			V_HUD_Override_Enabled_Found_Menus = prop->GetBool();
		}

		V_HUD_Colour_New_Menus = 4430046;
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourVal_N_HUD_Menus")) {
			V_HUD_Colour_New_Menus = prop->GetSInt();
		}

		V_HUD_Colour_Found_Menus = 1288220;
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourVal_G_HUD_Menus")) {
			V_HUD_Colour_Found_Menus = prop->GetSInt();
		}

		V_HUD_ColourString_New_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_N_HUD_Menus")) {
			V_HUD_ColourString_New_Menus = prop->GetString();
		}

		V_HUD_ColourString_Found_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_ColourString_G_HUD_Menus")) {
			V_HUD_ColourString_Found_Menus = prop->GetString();
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

		V_HUD_CustomColour_New_Menus = -1;
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourVal_N_HUD_Menus")) {
			V_HUD_CustomColour_New_Menus = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Menus = -1;
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourVal_G_HUD_Menus")) {
			V_HUD_CustomColour_Found_Menus = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_N_HUD_Menus")) {
			V_HUD_CustomColourString_New_Menus = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty("State_CustomColourString_G_HUD_Menus")) {
			V_HUD_CustomColourString_Found_Menus = prop->GetString();
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

		V_moreHudEnabled_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_moreHUDEnabled_Crosshair")) {
			V_moreHudEnabled_Crosshair = prop->GetBool();
		}

		V_moreHudEnabled_Menus = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_moreHUDEnabled_Menus")) {
			V_moreHudEnabled_Menus = prop->GetBool();
		}

		V_quickLoot_Enabled = false;
		if (const auto* prop = VariablesAPI::GetProperty("b_quickLoot_Enabled")) {
			V_quickLoot_Enabled = prop->GetBool();
		}

		V_Debugging = false;
		if (const auto* prop = VariablesAPI::GetProperty("bDebug")) {
			V_Debugging = prop->GetBool();
		}
	}
}