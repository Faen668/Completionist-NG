#pragma once

namespace CVariables {

	extern bool 		V_Ammo_Enabled_New;
	extern bool 		V_Ammo_Enabled_Found;
	extern std::int32_t V_Ammo_Colour_New;
	extern std::int32_t V_Ammo_Colour_Found;
	extern std::string 	V_Ammo_ColourString_New;
	extern std::string 	V_Ammo_ColourString_Found;

	extern bool 		V_Alchemy_Enabled_New;
	extern bool 		V_Alchemy_Enabled_Found;
	extern std::int32_t V_Alchemy_Colour_New;
	extern std::int32_t V_Alchemy_Colour_Found;
	extern std::string 	V_Alchemy_ColourString_New;
	extern std::string 	V_Alchemy_ColourString_Found;

	extern bool 		V_Armor_Enabled_New;
	extern bool 		V_Armor_Enabled_Found;
	extern std::int32_t V_Armor_Colour_New;
	extern std::int32_t V_Armor_Colour_Found;
	extern std::string 	V_Armor_ColourString_New;
	extern std::string 	V_Armor_ColourString_Found;

	extern bool 		V_Books_Enabled_New;
	extern bool 		V_Books_Enabled_Found;
	extern std::int32_t V_Books_Colour_New;
	extern std::int32_t V_Books_Colour_Found;
	extern std::string 	V_Books_ColourString_New;
	extern std::string 	V_Books_ColourString_Found;

	extern bool 		V_Weapons_Enabled_New;
	extern bool 		V_Weapons_Enabled_Found;
	extern std::int32_t V_Weapons_Colour_New;
	extern std::int32_t V_Weapons_Colour_Found;
	extern std::string 	V_Weapons_ColourString_New;
	extern std::string 	V_Weapons_ColourString_Found;

	extern bool 		V_Other_Enabled_New;
	extern bool 		V_Other_Enabled_Found;
	extern std::int32_t V_Other_Colour_New;
	extern std::int32_t V_Other_Colour_Found;
	extern std::string 	V_Other_ColourString_New;
	extern std::string 	V_Other_ColourString_Found;

	extern std::string V_CrosshairTag_New;
	extern std::string V_CrosshairTag_Found;

	extern std::int32_t V_TextChoice;
	extern std::int32_t V_IconChoice;

	extern bool			V_DearDiary;

	using EventResult = RE::BSEventNotifyControl;

	class VariablesAPI final :

		public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static VariablesAPI* GetSingleton() {
		static VariablesAPI singleton;
		return &singleton;
	}

		  EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		 
		  static void Register();
		  static void Update();
	};
}