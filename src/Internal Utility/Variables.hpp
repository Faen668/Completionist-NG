#pragma once
#include "Enums.hpp"
#include "UI/Core/UISettings.hpp"

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
	inline std::string 		V_HUD_ColourString_Displayable;
	inline std::string 		V_HUD_ColourString_Displayed;
	inline std::string 		V_HUD_ColourString_Occupied;

	inline std::int32_t		V_HUD_CustomColour_New_Crosshair;
	inline std::int32_t		V_HUD_CustomColour_Found_Crosshair;
	inline std::string 		V_HUD_CustomColourString_New_Crosshair;
	inline std::string 		V_HUD_CustomColourString_Found_Crosshair;

	inline std::string 		V_HUD_CustomColourString_Displayable;
	inline std::string 		V_HUD_CustomColourString_Displayed;
	inline std::string 		V_HUD_CustomColourString_Occupied;

	inline std::string		V_CrosshairTag_New;
	inline std::string		V_CrosshairTag_Found;
	inline std::string		V_CrosshairTag_Displayable;
	inline std::string		V_CrosshairTag_Displayed;
	inline std::string		V_CrosshairTag_Occupied;

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
	inline bool				V_CellScanner_Closest;
	inline bool				V_CellScanner_Markers;

	inline bool				V_CellScanner_ExcludeBooks;
	inline bool				V_CellScanner_ExcludeArmor;
	inline bool				V_CellScanner_ExcludeWeapons;
	inline bool				V_CellScanner_ExcludeOwned;

	// Item icons
	inline bool				V_ItemIcons{ true };
	inline std::int32_t		V_ItemIcons_Size{ 100 };
	inline std::int32_t		V_ItemIcons_Opacity{ 100 };
	inline bool				V_ItemIcons_Needed{ true };
	inline bool				V_ItemIcons_Found{ true };
	inline bool				V_ItemIcons_Museum{ true };

	// Completionist sort
	inline std::int32_t		V_Sort_Displayable{ 1 };
	inline std::int32_t		V_Sort_Occupied{ 2 };
	inline std::int32_t		V_Sort_Displayed{ 3 };
	inline std::int32_t		V_Sort_Needed{ 4 };
	inline std::int32_t		V_Sort_Found{ 5 };
	inline bool				V_Sort_Bands{ true };

	// Item names in the item lists, format 7 is no edits
	inline std::int32_t		V_MenuFormat_N{ 7 };
	inline std::int32_t		V_MenuFormat_G{ 7 };
	inline std::int32_t		V_MenuFormat_Displayable{ 7 };
	inline std::int32_t		V_MenuFormat_Displayed{ 7 };
	inline std::int32_t		V_MenuFormat_Occupied{ 7 };
	inline std::int32_t		V_MenuPrefix_N{ 0 };
	inline std::int32_t		V_MenuPrefix_G{ 0 };
	inline std::int32_t		V_MenuPrefix_Displayable{ 0 };
	inline std::int32_t		V_MenuPrefix_Displayed{ 0 };
	inline std::int32_t		V_MenuPrefix_Occupied{ 0 };
	inline std::string		V_MenuColour_N;
	inline std::string		V_MenuColour_G;
	inline bool				V_CellScanner_ExcludeAlchemy;
	inline bool				V_CellScanner_ExcludeMiscItems;
	inline std::int32_t		V_CellScanner_ScanRange;

	inline bool				V_TreatBooksAsItems;
	inline bool				V_Debugging;
	inline bool				V_Global_Patch_Logging;
	inline std::vector<CMiscPatchType> V_TrackedEntityTypes;
	inline bool				V_RestrictBooksToTomesAndSkillBooks;

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

		  static RE::BSScript::Variable* GetProperty(const char* a_prop);

		  static bool IsMuseumAPILoggingEnabled();
		  static bool IsGlobalPatchInstallLoggingEnabled();
		  static std::vector<CMiscPatchType> GetTrackedEntityTypes();
		  static bool GetRestrictBooksToTomesAndSkillBooks();
		  static bool IsDebuggingEnabled();
		  static bool GetIniBoolValue(const std::string& key);
		  static std::string GetIniStringValue(const std::string& key);
		  static std::string GetIniValue(const char* a_section, const std::string& a_key, const char* a_default);
		  static CompletionistUI::UISettings GetUISettings();
		  static void SaveUISettings(const CompletionistUI::UISettings& a_settings);

		  VariablesAPI(VariablesAPI const&) = delete;
		  VariablesAPI(VariablesAPI const&&) = delete;
		  VariablesAPI operator=(VariablesAPI&) = delete;
		  VariablesAPI operator=(VariablesAPI&&) = delete;
	};
}