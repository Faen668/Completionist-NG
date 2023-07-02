#include "Variables.hpp"
#include "ScriptObject.hpp"

namespace CVariables {

	void VariablesAPI::Register() {
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(VariablesAPI::GetSingleton()));
	};

	//---------------------------------------------------
	//-- Variables Functions ( On Menu Open / Close ) ---
	//---------------------------------------------------

	EventResult	VariablesAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
		
		if (a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) { 
			Update(); 
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Variables Functions ( Get MCM Property ) -------
	//---------------------------------------------------

	RE::BSScript::Variable* VariablesAPI::GetProperty(ScriptObjectPtr a_mcm, const char* a_prop) {

		if (!a_mcm || !a_prop || !a_mcm->GetProperty(a_prop)) {
			INFO("Variables API: Unable To Get Property - [{}]", a_prop);
			return nullptr;
		}
		return a_mcm->GetProperty(a_prop);
	}

	//---------------------------------------------------
	//-- Variables Functions ( Is Debugging Enabled ) ---
	//---------------------------------------------------

	bool VariablesAPI::IsDebuggingEnabled() {
		return V_Debugging;
	}

	//---------------------------------------------------
	//-- Variables Functions ( Update Properties ) ------
	//---------------------------------------------------

	void VariablesAPI::Update() 
	{ 
		auto MCM = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCM) { ERROR("Unable To Locate Completionist.esp. Exiting..."); return; }

		TCC_New = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558285, "DBM_RelicNotifications.esp");
		TCC_FND = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558286, "DBM_RelicNotifications.esp");
		TCC_DSP = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x558287, "DBM_RelicNotifications.esp");

		V_FishingSpotMarkers = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "FishingSpotMarkers")) {
			V_FishingSpotMarkers = prop->GetBool();
		}

		V_CellScanner_CONT = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bCellScanner_CONT")) {
			V_CellScanner_CONT = prop->GetBool();
		}

		V_CellScanner_REFS = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bCellScanner_REFS")) {
			V_CellScanner_REFS = prop->GetBool();
		}

		V_CellScanner_NPCS = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bCellScanner_NPCS")) {
			V_CellScanner_NPCS = prop->GetBool();
		}

		V_CellScanner_NUMB = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bCellScanner_NUMB")) {
			V_CellScanner_NUMB = prop->GetBool();
		}

		V_CellScanner_DETA = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bCellScanner_DETA")) {
			V_CellScanner_DETA = prop->GetBool();
		}

		V_NotificationText = "Completionist: Entry Complete - ";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationText")) {
			V_NotificationText = prop->GetString();
		}

		V_notificationTextCellScan0 = " Has ";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan0")) {
			V_notificationTextCellScan0 = prop->GetString();
		}

		V_notificationTextCellScan1 = " Collectable Items!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan1")) {
			V_notificationTextCellScan1 = prop->GetString();
		}

		V_notificationTextCellScan2 = " Has Collectable Items!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan2")) {
			V_notificationTextCellScan2 = prop->GetString();
		}

		V_notificationTextCellScan3 = "Collectables";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan3")) {
			V_notificationTextCellScan3 = prop->GetString();
		}

		V_notificationTextCellScan4 = "Loose Items";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan4")) {
			V_notificationTextCellScan4 = prop->GetString();
		}

		V_notificationTextCellScan5 = "Items On NPC's";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan5")) {
			V_notificationTextCellScan5 = prop->GetString();
		}

		V_notificationTextCellScan6 = "Items In Containers";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextCellScan6")) {
			V_notificationTextCellScan6 = prop->GetString();
		}

		V_NotificationTextShout0 = "Completionist: ";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextShouts0")) {
			V_NotificationTextShout0 = prop->GetString();
		}

		V_NotificationTextShout1 = " - Word 1 (";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextShouts1")) {
			V_NotificationTextShout1 = prop->GetString();
		}

		V_NotificationTextShout2 = " - Word 2 (";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextShouts2")) {
			V_NotificationTextShout2 = prop->GetString();
		}

		V_NotificationTextShout3 = ") - Learnt!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextShouts3")) {
			V_NotificationTextShout3 = prop->GetString();
		}

		V_NotificationTextBarenziah = "Completionist: Entry Complete - Stone of Barenziah!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "notificationTextBarenziah")) {
			V_NotificationTextBarenziah = prop->GetString();
		}

		V_TextChoice_N = 4;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "InventoryMode_PrAP_Choice_N")) {
			V_TextChoice_N = prop->GetSInt();
		}

		V_TextChoice_G = 4;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "InventoryMode_PrAP_Choice_G")) {
			V_TextChoice_G = prop->GetSInt();
		}

		V_PrefixChoice_N = 3;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "InventoryMode_PrFx_Choice_N")) {
			V_PrefixChoice_N = prop->GetSInt();
		}

		V_PrefixChoice_G = 3;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "InventoryMode_PrFx_Choice_G")) {
			V_PrefixChoice_G = prop->GetSInt();
		}

		V_CrosshairTag_New = "Need It!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_OverRide_N_Name_String")) {
			V_CrosshairTag_New = prop->GetString();
		}

		V_CrosshairTag_Found = "Got It!";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_OverRide_G_Name_String")) {
			V_CrosshairTag_Found = prop->GetString();
		}

		V_HUD_Override_Enabled_New_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_CustomColour_N_HUD_Crosshair")) {
			V_HUD_Override_Enabled_New_Crosshair = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Crosshair = false;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_CustomColour_G_HUD_Crosshair")) {
			V_HUD_Override_Enabled_Found_Crosshair = prop->GetBool();
		}

		V_HUD_Colour_New_Crosshair = 4430046;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourVal_N_HUD_Crosshair")) {
			V_HUD_Colour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_Colour_Found_Crosshair = 1288220;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourVal_G_HUD_Crosshair")) {
			V_HUD_Colour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_ColourString_New_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourString_N_HUD_Crosshair")) {
			V_HUD_ColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_ColourString_Found_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourString_G_HUD_Crosshair")) {
			V_HUD_ColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_Override_Enabled_New_Menus = false;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_CustomColour_N_HUD_Menus")) {
			V_HUD_Override_Enabled_New_Menus = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Menus = false;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_CustomColour_G_HUD_Menus")) {
			V_HUD_Override_Enabled_Found_Menus = prop->GetBool();
		}

		V_HUD_Colour_New_Menus = 4430046;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourVal_N_HUD_Menus")) {
			V_HUD_Colour_New_Menus = prop->GetSInt();
		}

		V_HUD_Colour_Found_Menus = 1288220;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourVal_G_HUD_Menus")) {
			V_HUD_Colour_Found_Menus = prop->GetSInt();
		}

		V_HUD_ColourString_New_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourString_N_HUD_Menus")) {
			V_HUD_ColourString_New_Menus = prop->GetString();
		}

		V_HUD_ColourString_Found_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_ColourString_G_HUD_Menus")) {
			V_HUD_ColourString_Found_Menus = prop->GetString();
		}

		V_HUD_CustomColour_New_Crosshair = -1;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourVal_N_HUD_Crosshair")) {
			V_HUD_CustomColour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Crosshair = -1;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourVal_G_HUD_Crosshair")) {
			V_HUD_CustomColour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourString_N_HUD_Crosshair")) {
			V_HUD_CustomColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Crosshair = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourString_G_HUD_Crosshair")) {
			V_HUD_CustomColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_CustomColour_New_Menus = -1;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourVal_N_HUD_Menus")) {
			V_HUD_CustomColour_New_Menus = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Menus = -1;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourVal_G_HUD_Menus")) {
			V_HUD_CustomColour_Found_Menus = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourString_N_HUD_Menus")) {
			V_HUD_CustomColourString_New_Menus = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Menus = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CustomColourString_G_HUD_Menus")) {
			V_HUD_CustomColourString_Found_Menus = prop->GetString();
		}

		V_ShoutColour_New = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "StateShout_NotColourString")) {
			V_ShoutColour_New = prop->GetString();
		}

		V_ShoutColour_Found = "";
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "StateShout_GotColourString")) {
			V_ShoutColour_Found = prop->GetString();
		}

		V_Radiant_FavorVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_RadiantCounterVal")) {
			V_Radiant_FavorVal = prop->GetSInt();
		}

		V_Radiant_CollegeVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CollegeCounterVal")) {
			V_Radiant_CollegeVal = prop->GetSInt();
		}

		V_Radiant_CompanionsVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_CompanionsCounterVal")) {
			V_Radiant_CompanionsVal = prop->GetSInt();
		}

		V_Radiant_DBrotherhoodVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_DBCounterVal")) {
			V_Radiant_DBrotherhoodVal = prop->GetSInt();
		}

		V_Radiant_DawnguardVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_DLC1CounterVal")) {
			V_Radiant_DawnguardVal = prop->GetSInt();
		}

		V_Radiant_ThievesGuildVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_TGRCounterVal")) {
			V_Radiant_ThievesGuildVal = prop->GetSInt();
		}

		V_Radiant_BountyVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_BountyCounterVal")) {
			V_Radiant_BountyVal = prop->GetSInt();
		}

		V_Radiant_VigilantVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_VigilantCounterVal")) {
			V_Radiant_VigilantVal = prop->GetSInt();
		}

		V_Radiant_LegacyVal = 5;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "State_LOTDCounterVal")) {
			V_Radiant_LegacyVal = prop->GetSInt();
		}


		V_moreHudEnabled_Crosshair = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_moreHUDEnabled_Crosshair")) {
			V_moreHudEnabled_Crosshair = prop->GetBool();
		}

		V_moreHudEnabled_Menus = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_moreHUDEnabled_Menus")) {
			V_moreHudEnabled_Menus = prop->GetBool();
		}

		V_quickLoot_Enabled = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "b_quickLoot_Enabled")) {
			V_quickLoot_Enabled = prop->GetBool();
		}

		V_Debugging = true;
		if (const auto* prop = VariablesAPI::GetProperty(MCM, "bDebug")) {
			V_Debugging = prop->GetBool();
		}
	}
}