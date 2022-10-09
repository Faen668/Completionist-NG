#include "Variables.hpp"
#include "Internal Utility/ScriptObject.hpp"

namespace CVariables {

	inline std::string 		V_ShoutColour_New;
	inline std::string 		V_ShoutColour_Found;

	inline bool 			V_Ammo_Enabled_New;
	inline bool 			V_Ammo_Enabled_Found;

	inline bool 			V_Alchemy_Enabled_New;
	inline bool 			V_Alchemy_Enabled_Found;

	inline bool 			V_Armor_Enabled_New;
	inline bool 			V_Armor_Enabled_Found;

	inline bool 			V_Books_Enabled_New;
	inline bool 			V_Books_Enabled_Found;

	inline bool 			V_Weapons_Enabled_New;
	inline bool 			V_Weapons_Enabled_Found;

	inline bool 			V_Other_Enabled_New;
	inline bool 			V_Other_Enabled_Found;

	inline std::int32_t		V_HUD_Colour_New;
	inline std::int32_t		V_HUD_Colour_Found;
	inline std::string 		V_HUD_ColourString_New;
	inline std::string 		V_HUD_ColourString_Found;

	inline std::int32_t		V_TextChoice;
	inline std::string		V_CrosshairTag_New;
	inline std::string		V_CrosshairTag_Found;

	inline bool				V_mainHudEnabled;
	inline bool				V_moreHudEnabled;

	inline std::int32_t		V_RadiantCounterVal;


	void VariablesAPI::Register() {
		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(VariablesAPI::GetSingleton()));
		Update();
	}

	EventResult	VariablesAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
		if (!a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) { Update(); return EventResult::kContinue; }
		return EventResult::kContinue;
	}

	void VariablesAPI::Update() {

		auto MCM = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCM) { return;  }

		V_mainHudEnabled = MCM->GetProperty("varTags_mainHUDEnabled")->GetBool();
		V_moreHudEnabled = MCM->GetProperty("varTags_moreHUDEnabled")->GetBool();

		V_TextChoice = MCM->GetProperty("InventoryMode_PrAp_Choice")->GetSInt();
		V_CrosshairTag_New = MCM->GetProperty("State_OverRide_N_Name_String")->GetString();
		V_CrosshairTag_Found = MCM->GetProperty("State_OverRide_G_Name_String")->GetString();

		V_Ammo_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Ammo_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();

		V_Alchemy_Enabled_New = MCM->GetProperty("varTags_Alchemy_N")->GetBool();
		V_Alchemy_Enabled_Found = MCM->GetProperty("varTags_Alchemy_G")->GetBool();

		V_Armor_Enabled_New = MCM->GetProperty("varTags_Armor_N")->GetBool();
		V_Armor_Enabled_Found = MCM->GetProperty("varTags_Armor_G")->GetBool();

		V_Books_Enabled_New = MCM->GetProperty("varTags_Books_N")->GetBool();
		V_Books_Enabled_Found = MCM->GetProperty("varTags_Books_G")->GetBool();

		V_Weapons_Enabled_New = MCM->GetProperty("varTags_Weapons_N")->GetBool();
		V_Weapons_Enabled_Found = MCM->GetProperty("varTags_Weapons_G")->GetBool();

		V_Other_Enabled_New = MCM->GetProperty("varTags_Other_N")->GetBool();
		V_Other_Enabled_Found = MCM->GetProperty("varTags_Other_G")->GetBool();

		V_HUD_Colour_New = MCM->GetProperty("State_ColourVal_N_HUD")->GetSInt();
		V_HUD_Colour_Found = MCM->GetProperty("State_ColourVal_G_HUD")->GetSInt();
		V_HUD_ColourString_New = MCM->GetProperty("State_ColourString_N_HUD")->GetString();
		V_HUD_ColourString_Found = MCM->GetProperty("State_ColourString_G_HUD")->GetString();

		V_ShoutColour_New = MCM->GetProperty("StateShout_NotColourString")->GetString();
		V_ShoutColour_Found = MCM->GetProperty("StateShout_GotColourString")->GetString();

		V_RadiantCounterVal = MCM->GetProperty("State_RadiantCounterVal")->GetSInt();
	}
}