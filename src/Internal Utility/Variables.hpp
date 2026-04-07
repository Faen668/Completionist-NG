#pragma once

namespace CVariables
{
	inline std::string 		V_ShoutColour_New;
	inline std::string 		V_ShoutColour_Found;

	inline bool 			V_HUD_Override_Enabled_New_Crosshair;
	inline bool 			V_HUD_Override_Enabled_Found_Crosshair;
	inline std::int32_t		V_HUD_Colour_New_Crosshair;
	inline std::int32_t		V_HUD_Colour_Found_Crosshair;
	inline std::string 		V_HUD_ColourString_New_Crosshair;
	inline std::string 		V_HUD_ColourString_Found_Crosshair;

	inline bool 			V_HUD_Override_Enabled_Displayable;
	inline bool 			V_HUD_Override_Enabled_Displayed;
	inline bool 			V_HUD_Override_Enabled_Occupied;
	inline std::int32_t		V_HUD_Colour_Displayable;
	inline std::int32_t		V_HUD_Colour_Displayed;
	inline std::int32_t		V_HUD_Colour_Occupied;
	inline std::string 		V_HUD_ColourString_Displayable;
	inline std::string 		V_HUD_ColourString_Displayed;
	inline std::string 		V_HUD_ColourString_Occupied;

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

	inline std::int32_t		V_HUD_CustomColour_Displayable;
	inline std::int32_t		V_HUD_CustomColour_Displayed;
	inline std::int32_t		V_HUD_CustomColour_Occupied;
	inline std::string 		V_HUD_CustomColourString_Displayable;
	inline std::string 		V_HUD_CustomColourString_Displayed;
	inline std::string 		V_HUD_CustomColourString_Occupied;

	inline std::int32_t		V_HUD_CustomColour_New_Menus;
	inline std::int32_t		V_HUD_CustomColour_Found_Menus;
	inline std::string 		V_HUD_CustomColourString_New_Menus;
	inline std::string 		V_HUD_CustomColourString_Found_Menus;

	inline std::int32_t		V_TextChoice_N;
	inline std::int32_t		V_TextChoice_G;

	inline std::int32_t		V_TextChoice_Displayable;
	inline std::int32_t		V_TextChoice_Displayed;
	inline std::int32_t		V_TextChoice_Occupied;

	inline std::int32_t		V_PrefixChoice_N;
	inline std::int32_t		V_PrefixChoice_G;

	inline std::int32_t		V_PrefixChoice_Displayable;
	inline std::int32_t		V_PrefixChoice_Displayed;
	inline std::int32_t		V_PrefixChoice_Occupied ;

	inline std::string		V_CrosshairTag_New;
	inline std::string		V_CrosshairTag_Found;
	inline std::string		V_CrosshairTag_Displayable;
	inline std::string		V_CrosshairTag_Displayed;
	inline std::string		V_CrosshairTag_Occupied;

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
	inline std::int32_t		V_Radiant_FishingVal;
	inline std::int32_t		V_Radiant_BladesVal;

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
	inline bool				V_CellScanner_Closest;
	inline bool				V_CellScanner_Markers;

	inline bool				V_CellScanner_ExcludeBooks;
	inline bool				V_CellScanner_ExcludeArmor;
	inline bool				V_CellScanner_ExcludeWeapons;
	inline bool				V_CellScanner_ExcludeAlchemy;
	inline bool				V_CellScanner_ExcludeMiscItems;
	inline std::int32_t		V_CellScanner_ScanRange;

	inline bool				V_FishingSpotMarkers;
	inline bool				V_TreatBooksAsItems;
	inline bool				V_Debugging;
	inline bool				V_Global_Patch_Logging;

	inline bool				V_MuseumModeEnabled;
	inline bool				V_TreatOccupiedAsDisplayed;
	
	class VariablesAPI final
	{
	private:
		VariablesAPI() = default;
		~VariablesAPI() = default;
	
	public:
		  static VariablesAPI* GetSingleton() { static VariablesAPI singleton; return &singleton; }

		  static void Register();
		  static void Update();
		  static void OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);
		  static void SetFrameworkQuest(RE::StaticFunctionTag*, RE::TESQuest* a_quest);

		  static RE::BSScript::Variable* GetProperty(const char* a_prop);

		  static bool IsUsingLinux();
		  static bool IsMuseumAPILoggingEnabled();
		  static bool IsGlobalPatchInstallLoggingEnabled();
		  static bool IsDebuggingEnabled();
		  static bool GetIniBoolValue(const std::string& key);

		  VariablesAPI(VariablesAPI const&) = delete;
		  VariablesAPI(VariablesAPI const&&) = delete;
		  VariablesAPI operator=(VariablesAPI&) = delete;
		  VariablesAPI operator=(VariablesAPI&&) = delete;
	};
}