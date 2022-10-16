#pragma once

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
	inline std::int32_t		V_Radiant_CollegeVal;
	inline std::int32_t		V_Radiant_CompanionsVal;
	inline std::int32_t		V_Radiant_DBrotherhoodVal;
	inline std::int32_t		V_Radiant_DawnguardVal;
	inline std::int32_t		V_Radiant_ThievesGuildVal;
	inline std::int32_t		V_Radiant_BountyVal;
	inline std::int32_t		V_Radiant_VigilantVal;
	inline std::int32_t		V_Radiant_LegacyVal;

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