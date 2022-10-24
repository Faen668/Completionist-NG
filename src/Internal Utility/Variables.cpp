#include "Variables.hpp"
#include "ScriptObject.hpp"

namespace CVariables {

	void VariablesAPI::Register() {
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(VariablesAPI::GetSingleton()));
	}

	EventResult	VariablesAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
		
		if (!a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) { 
			Update(); 
		}
		return EventResult::kContinue;
	}

	void VariablesAPI::Update() {

		INFO("Loading Variables");

		auto MCM = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCM) { return;  }

		V_TextChoice_N = 4;
		if (const auto* prop = MCM->GetProperty("InventoryMode_PrAP_Choice_N")) {
			V_TextChoice_N = prop->GetSInt();
		}

		V_TextChoice_G = 4;
		if (const auto* prop = MCM->GetProperty("InventoryMode_PrAP_Choice_G")) {
			V_TextChoice_G = prop->GetSInt();
		}

		V_CrosshairTag_New = "Need It!";
		if (const auto* prop = MCM->GetProperty("State_OverRide_N_Name_String")) {
			V_CrosshairTag_New = prop->GetString();
		}

		V_CrosshairTag_Found = "Got It!";
		if (const auto* prop = MCM->GetProperty("State_OverRide_G_Name_String")) {
			V_CrosshairTag_Found = prop->GetString();
		}

		V_Ammo_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Ammo_N")) {
			V_Ammo_Enabled_New = prop->GetBool();
		}

		V_Ammo_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Ammo_G")) {
			V_Ammo_Enabled_Found = prop->GetBool();
		}

		V_Armor_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Armor_N")) {
			V_Armor_Enabled_New = prop->GetBool();
		}

		V_Armor_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Armor_G")) {
			V_Armor_Enabled_Found = prop->GetBool();
		}

		V_Alchemy_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Alchemy_N")) {
			V_Alchemy_Enabled_New = prop->GetBool();
		}

		V_Alchemy_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Alchemy_G")) {
			V_Alchemy_Enabled_Found = prop->GetBool();
		}

		V_Books_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Books_N")) {
			V_Books_Enabled_New = prop->GetBool();
		}

		V_Books_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Books_G")) {
			V_Books_Enabled_Found = prop->GetBool();
		}

		V_Weapons_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Weapons_N")) {
			V_Weapons_Enabled_New = prop->GetBool();
		}

		V_Weapons_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Weapons_G")) {
			V_Weapons_Enabled_Found = prop->GetBool();
		}

		V_Other_Enabled_New = true;
		if (const auto* prop = MCM->GetProperty("varTags_Other_N")) {
			V_Other_Enabled_New = prop->GetBool();
		}

		V_Other_Enabled_Found = true;
		if (const auto* prop = MCM->GetProperty("varTags_Other_G")) {
			V_Other_Enabled_Found = prop->GetBool();
		}

		V_HUD_Override_Enabled_New_Crosshair = false;
		if (const auto* prop = MCM->GetProperty("b_CustomColour_N_HUD_Crosshair")) {
			V_HUD_Override_Enabled_New_Crosshair = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Crosshair = false;
		if (const auto* prop = MCM->GetProperty("b_CustomColour_G_HUD_Crosshair")) {
			V_HUD_Override_Enabled_Found_Crosshair = prop->GetBool();
		}

		V_HUD_Colour_New_Crosshair = 4430046;
		if (const auto* prop = MCM->GetProperty("State_ColourVal_N_HUD_Crosshair")) {
			V_HUD_Colour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_Colour_Found_Crosshair = 1288220;
		if (const auto* prop = MCM->GetProperty("State_ColourVal_G_HUD_Crosshair")) {
			V_HUD_Colour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_ColourString_New_Crosshair = "";
		if (const auto* prop = MCM->GetProperty("State_ColourString_N_HUD_Crosshair")) {
			V_HUD_ColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_ColourString_Found_Crosshair = "";
		if (const auto* prop = MCM->GetProperty("State_ColourString_G_HUD_Crosshair")) {
			V_HUD_ColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_Override_Enabled_New_Menus = false;
		if (const auto* prop = MCM->GetProperty("b_CustomColour_N_HUD_Menus")) {
			V_HUD_Override_Enabled_New_Menus = prop->GetBool();
		}

		V_HUD_Override_Enabled_Found_Menus = false;
		if (const auto* prop = MCM->GetProperty("b_CustomColour_G_HUD_Menus")) {
			V_HUD_Override_Enabled_Found_Menus = prop->GetBool();
		}

		V_HUD_Colour_New_Menus = 4430046;
		if (const auto* prop = MCM->GetProperty("State_ColourVal_N_HUD_Menus")) {
			V_HUD_Colour_New_Menus = prop->GetSInt();
		}

		V_HUD_Colour_Found_Menus = 1288220;
		if (const auto* prop = MCM->GetProperty("State_ColourVal_G_HUD_Menus")) {
			V_HUD_Colour_Found_Menus = prop->GetSInt();
		}

		V_HUD_ColourString_New_Menus = "";
		if (const auto* prop = MCM->GetProperty("State_ColourString_N_HUD_Menus")) {
			V_HUD_ColourString_New_Menus = prop->GetString();
		}

		V_HUD_ColourString_Found_Menus = "";
		if (const auto* prop = MCM->GetProperty("State_ColourString_G_HUD_Menus")) {
			V_HUD_ColourString_Found_Menus = prop->GetString();
		}

		V_HUD_CustomColour_New_Crosshair = -1;
		if (const auto* prop = MCM->GetProperty("State_CustomColourVal_N_HUD_Crosshair")) {
			V_HUD_CustomColour_New_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Crosshair = -1;
		if (const auto* prop = MCM->GetProperty("State_CustomColourVal_G_HUD_Crosshair")) {
			V_HUD_CustomColour_Found_Crosshair = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Crosshair = "";
		if (const auto* prop = MCM->GetProperty("State_CustomColourString_N_HUD_Crosshair")) {
			V_HUD_CustomColourString_New_Crosshair = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Crosshair = "";
		if (const auto* prop = MCM->GetProperty("State_CustomColourString_G_HUD_Crosshair")) {
			V_HUD_CustomColourString_Found_Crosshair = prop->GetString();
		}

		V_HUD_CustomColour_New_Menus = -1;
		if (const auto* prop = MCM->GetProperty("State_CustomColourVal_N_HUD_Menus")) {
			V_HUD_CustomColour_New_Menus = prop->GetSInt();
		}

		V_HUD_CustomColour_Found_Menus = -1;
		if (const auto* prop = MCM->GetProperty("State_CustomColourVal_G_HUD_Menus")) {
			V_HUD_CustomColour_Found_Menus = prop->GetSInt();
		}

		V_HUD_CustomColourString_New_Menus = "";
		if (const auto* prop = MCM->GetProperty("State_CustomColourString_N_HUD_Menus")) {
			V_HUD_CustomColourString_New_Menus = prop->GetString();
		}

		V_HUD_CustomColourString_Found_Menus = "";
		if (const auto* prop = MCM->GetProperty("State_CustomColourString_G_HUD_Menus")) {
			V_HUD_CustomColourString_Found_Menus = prop->GetString();
		}

		V_ShoutColour_New = "";
		if (const auto* prop = MCM->GetProperty("StateShout_NotColourString")) {
			V_ShoutColour_New = prop->GetString();
		}

		V_ShoutColour_Found = "";
		if (const auto* prop = MCM->GetProperty("StateShout_GotColourString")) {
			V_ShoutColour_Found = prop->GetString();
		}

		V_RadiantCounterVal = 5;
		if (const auto* prop = MCM->GetProperty("State_RadiantCounterVal")) {
			V_RadiantCounterVal = prop->GetSInt();
		}

		V_Radiant_CollegeVal = 5;
		if (const auto* prop = MCM->GetProperty("State_CollegeCounterVal")) {
			V_Radiant_CollegeVal = prop->GetSInt();
		}

		V_Radiant_CompanionsVal = 5;
		if (const auto* prop = MCM->GetProperty("State_CompanionsCounterVal")) {
			V_Radiant_CompanionsVal = prop->GetSInt();
		}

		V_Radiant_DBrotherhoodVal = 5;
		if (const auto* prop = MCM->GetProperty("State_DBCounterVal")) {
			V_Radiant_DBrotherhoodVal = prop->GetSInt();
		}

		V_Radiant_DawnguardVal = 5;
		if (const auto* prop = MCM->GetProperty("State_DLC1CounterVal")) {
			V_Radiant_DawnguardVal = prop->GetSInt();
		}

		V_Radiant_ThievesGuildVal = 5;
		if (const auto* prop = MCM->GetProperty("State_TGRCounterVal")) {
			V_Radiant_ThievesGuildVal = prop->GetSInt();
		}

		V_Radiant_BountyVal = 5;
		if (const auto* prop = MCM->GetProperty("State_BountyCounterVal")) {
			V_Radiant_BountyVal = prop->GetSInt();
		}

		V_Radiant_VigilantVal = 5;
		if (const auto* prop = MCM->GetProperty("State_VigilantCounterVal")) {
			V_Radiant_VigilantVal = prop->GetSInt();
		}

		V_Radiant_LegacyVal = 5;
		if (const auto* prop = MCM->GetProperty("State_LOTDCounterVal")) {
			V_Radiant_LegacyVal = prop->GetSInt();
		}


		V_moreHudEnabled_Crosshair = true;
		if (const auto* prop = MCM->GetProperty("b_moreHUDEnabled_Crosshair")) {
			V_moreHudEnabled_Crosshair = prop->GetBool();
		}

		V_moreHudEnabled_Menus = true;
		if (const auto* prop = MCM->GetProperty("b_moreHUDEnabled_Menus")) {
			V_moreHudEnabled_Menus = prop->GetBool();
		}
	}
}