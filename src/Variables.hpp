#pragma once

namespace CVariables {

	extern std::string 		V_ShoutColour_New;
	extern std::string 		V_ShoutColour_Found;

	extern bool 			V_Ammo_Enabled_New;
	extern bool 			V_Ammo_Enabled_Found;

	extern bool 			V_Alchemy_Enabled_New;
	extern bool 			V_Alchemy_Enabled_Found;

	extern bool 			V_Armor_Enabled_New;
	extern bool 			V_Armor_Enabled_Found;

	extern bool 			V_Books_Enabled_New;
	extern bool 			V_Books_Enabled_Found;

	extern bool 			V_Weapons_Enabled_New;
	extern bool 			V_Weapons_Enabled_Found;

	extern bool 			V_Other_Enabled_New;
	extern bool 			V_Other_Enabled_Found;

	extern std::int32_t		V_HUD_Colour_New;
	extern std::int32_t		V_HUD_Colour_Found;
	extern std::string 		V_HUD_ColourString_New;
	extern std::string 		V_HUD_ColourString_Found;

	extern std::int32_t		V_TextChoice;
	extern std::string		V_CrosshairTag_New;
	extern std::string		V_CrosshairTag_Found;

	extern bool				V_mainHudEnabled;
	extern bool				V_moreHudEnabled;

	extern std::int32_t		V_RadiantCounterVal;

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