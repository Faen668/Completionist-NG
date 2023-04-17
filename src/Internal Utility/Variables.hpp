#pragma once

namespace CVariables 
{
	inline std::unordered_map<std::string_view, std::string_view> filelist;

	inline RE::BGSListForm* TCC_New;
	inline RE::BGSListForm* TCC_FND;
	inline RE::BGSListForm* TCC_DSP;

	inline std::string 		V_ShoutColour_New;
	inline std::string 		V_ShoutColour_Found;

	inline bool 			V_HUD_Override_Enabled_New_Crosshair;
	inline bool 			V_HUD_Override_Enabled_Found_Crosshair;
	inline std::int32_t		V_HUD_Colour_New_Crosshair;
	inline std::int32_t		V_HUD_Colour_Found_Crosshair;
	inline std::string 		V_HUD_ColourString_New_Crosshair;
	inline std::string 		V_HUD_ColourString_Found_Crosshair;

	inline bool 			V_HUD_Override_Enabled_New_Menus;
	inline bool 			V_HUD_Override_Enabled_Found_Menus;
	inline std::int32_t		V_HUD_Colour_New_Menus;
	inline std::int32_t		V_HUD_Colour_Found_Menus;
	inline std::string 		V_HUD_ColourString_New_Menus;
	inline std::string 		V_HUD_ColourString_Found_Menus;

	inline std::int32_t		V_HUD_CustomColour_New_Crosshair;
	inline std::int32_t		V_HUD_CustomColour_Found_Crosshair;
	inline std::string 		V_HUD_CustomColourString_New_Crosshair;
	inline std::string 		V_HUD_CustomColourString_Found_Crosshair;

	inline std::int32_t		V_HUD_CustomColour_New_Menus;
	inline std::int32_t		V_HUD_CustomColour_Found_Menus;
	inline std::string 		V_HUD_CustomColourString_New_Menus;
	inline std::string 		V_HUD_CustomColourString_Found_Menus;

	inline std::int32_t		V_TextChoice_N;
	inline std::int32_t		V_TextChoice_G;

	inline std::int32_t		V_PrefixChoice_N;
	inline std::int32_t		V_PrefixChoice_G;

	inline std::string		V_CrosshairTag_New;
	inline std::string		V_CrosshairTag_Found;

	inline bool				V_quickLoot_Enabled;
	inline bool				V_moreHudEnabled_Crosshair;
	inline bool				V_moreHudEnabled_Menus;

	inline std::int32_t		V_Radiant_FavorVal;
	inline std::int32_t		V_Radiant_CollegeVal;
	inline std::int32_t		V_Radiant_CompanionsVal;
	inline std::int32_t		V_Radiant_DBrotherhoodVal;
	inline std::int32_t		V_Radiant_DawnguardVal;
	inline std::int32_t		V_Radiant_ThievesGuildVal;
	inline std::int32_t		V_Radiant_BountyVal;
	inline std::int32_t		V_Radiant_VigilantVal;
	inline std::int32_t		V_Radiant_LegacyVal;

	inline std::string 		V_NotificationText;
	inline std::string		V_NotificationTextShout0;
	inline std::string		V_NotificationTextShout1;
	inline std::string		V_NotificationTextShout2;
	inline std::string		V_NotificationTextShout3;
	inline std::string		V_NotificationTextBarenziah;

	inline std::string		V_notificationTextCellScan0;
	inline std::string		V_notificationTextCellScan1;
	inline std::string		V_notificationTextCellScan2;
	inline std::string		V_notificationTextCellScan3;
	inline std::string		V_notificationTextCellScan4;
	inline std::string		V_notificationTextCellScan5;
	inline std::string		V_notificationTextCellScan6;

	inline bool				V_CellScanner_REFS;
	inline bool				V_CellScanner_CONT;
	inline bool				V_CellScanner_NPCS;
	inline bool				V_CellScanner_NUMB;
	inline bool				V_CellScanner_DETA;

	inline bool				V_Debugging;

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

		  static bool IsDebuggingEnabled();

		  static RE::BSScript::Variable* GetProperty(ScriptObjectPtr mcm, const char* a_prop);
	};
}