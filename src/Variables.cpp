#include "Variables.hpp"
#include "Internal Utility/ScriptObject.hpp"

namespace CVariables {

	inline bool 			V_Ammo_Enabled_New;
	inline bool 			V_Ammo_Enabled_Found;
	inline std::int32_t		V_Ammo_Colour_New;
	inline std::int32_t		V_Ammo_Colour_Found;
	inline std::string 		V_Ammo_ColourString_New;
	inline std::string 		V_Ammo_ColourString_Found;

	inline bool 			V_Alchemy_Enabled_New;
	inline bool 			V_Alchemy_Enabled_Found;
	inline std::int32_t		V_Alchemy_Colour_New;
	inline std::int32_t		V_Alchemy_Colour_Found;
	inline std::string 		V_Alchemy_ColourString_New;
	inline std::string 		V_Alchemy_ColourString_Found;

	inline bool 			V_Armor_Enabled_New;
	inline bool 			V_Armor_Enabled_Found;
	inline std::int32_t		V_Armor_Colour_New;
	inline std::int32_t		V_Armor_Colour_Found;
	inline std::string 		V_Armor_ColourString_New;
	inline std::string 		V_Armor_ColourString_Found;

	inline bool 			V_Books_Enabled_New;
	inline bool 			V_Books_Enabled_Found;
	inline std::int32_t		V_Books_Colour_New;
	inline std::int32_t		V_Books_Colour_Found;
	inline std::string 		V_Books_ColourString_New;
	inline std::string 		V_Books_ColourString_Found;

	inline bool 			V_Weapons_Enabled_New;
	inline bool 			V_Weapons_Enabled_Found;
	inline std::int32_t		V_Weapons_Colour_New;
	inline std::int32_t		V_Weapons_Colour_Found;
	inline std::string 		V_Weapons_ColourString_New;
	inline std::string 		V_Weapons_ColourString_Found;

	inline bool 			V_Other_Enabled_New;
	inline bool 			V_Other_Enabled_Found;
	inline std::int32_t		V_Other_Colour_New;
	inline std::int32_t		V_Other_Colour_Found;
	inline std::string 		V_Other_ColourString_New;
	inline std::string 		V_Other_ColourString_Found;

	inline std::string		V_CrosshairTag_New;
	inline std::string		V_CrosshairTag_Found;

	inline std::int32_t		V_TextChoice;
	inline std::int32_t		V_IconChoice;

	inline bool				V_DearDiary;

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

		V_DearDiary = MCM->GetProperty("varTags_DearDiaryWM")->GetBool();

		V_TextChoice = MCM->GetProperty("InventoryMode_PrAp_Choice")->GetSInt();
		V_IconChoice = MCM->GetProperty("InventoryMode_Icon_Choice")->GetUInt();

		V_CrosshairTag_New = MCM->GetProperty("State_OverRide_N_Name_String")->GetString();
		V_CrosshairTag_Found = MCM->GetProperty("State_OverRide_G_Name_String")->GetString();

		V_Ammo_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Ammo_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Ammo_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Ammo_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Ammo_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Ammo_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();

		V_Alchemy_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Alchemy_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Alchemy_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Alchemy_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Alchemy_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Alchemy_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();

		V_Armor_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Armor_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Armor_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Armor_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Armor_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Armor_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();

		V_Books_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Books_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Books_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Books_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Books_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Books_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();

		V_Weapons_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Weapons_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Weapons_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Weapons_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Weapons_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Weapons_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();

		V_Other_Colour_New = MCM->GetProperty("State_ColourVal_N_Ammo")->GetSInt();
		V_Other_Colour_Found = MCM->GetProperty("State_ColourVal_G_Ammo")->GetSInt();
		V_Other_Enabled_New = MCM->GetProperty("varTags_Ammo_N")->GetBool();
		V_Other_Enabled_Found = MCM->GetProperty("varTags_Ammo_G")->GetBool();
		V_Other_ColourString_New = MCM->GetProperty("State_ColourString_N_Ammo")->GetString();
		V_Other_ColourString_Found = MCM->GetProperty("State_ColourString_G_Ammo")->GetString();
	}
}