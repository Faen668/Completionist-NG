#pragma once
#include "MiscPatchStruct.hpp"

namespace CHCMHandler
{
	//FormID, Menu uuid, Option Index
	inline std::unordered_map<RE::FormID, std::pair<std::string, int32_t>> formVisibilityMap{};

	//EditorID, Menu uuid, Option Index
	inline std::unordered_map<std::string, std::pair<std::string, int32_t>> questVisibilityMap{};

	inline std::vector<std::pair<std::string, CMiscPatch*>> CustomPatches{};	
	inline std::vector<std::tuple<std::string, std::string, uint32_t>> MainPatchesMCMPagesDefs{};
	inline std::vector<std::tuple<const char*, const char*, int32_t>> MainMCMPagesDefs =
	{ {
		{"$MCMPageSettings", "", -1},
		{"$MCMPageSettings3", "", -1},
		{"$MCMPageSettings2", "", -1},
		{"$MCMPageSettings4", "", -1},
		{"$MCMPageSettings5", "", -1},
		{" ", "", -1},
		{"$Header0", "", -1},
		{"$MCMPageMain", "", 0},
		{"$MCMPageMainCW", "", 1},
		{"$MCMPageMainDG", "", 2},
		{"$MCMPageMainDB", "", 3},
		{" ", "", -1},
		{"$Header2", "", -1},
		{"$MCMPageMainDawnstar", "", 7},
		{"$MCMPageMainFalkreath", "", 8},
		{"$MCMPageMainMarkarth", "", 9},
		{"$MCMPageMainMorthal", "", 10},
		{"$MCMPageMainRavenRock", "", 16},
		{"$MCMPageMainRiften", "", 11},
		{"$MCMPageMainSkaal", "", 17},
		{"$MCMPageMainSolitude", "", 12},
		{"$MCMPageMainTelMithryn", "", 18},
		{"$MCMPageMainThirsk", "", 19},
		{"$MCMPageMainWhiterun", "", 13},
		{"$MCMPageMainWindhelm", "", 14},
		{"$MCMPageMainWinterhold", "", 15},
		{"$MCMPageMainSmall", "", 20},
		{" ", "", -1},
		{"$Header3", "", -1},
		{"$MCMPageMainCompanions", "", 22},
		{"$MCMPageMainCollege", "", 21},
		{"$MCMPageMainThieves", "", 25},
		{"$MCMPageMainBrotherhood", "", 23},
		{"$MCMPageMainDawnguard", "", 24},
		{"$MCMPageMainVampires", "", 26},
		{" ", "", -1},
		{"$Header4", "", -1},
		{"$MCMPageMainDungeons", "", 27},
		{"$MCMPageMainMiscSide", "", 28},
		{"$MCMPageMainMiscRadi", "", 29},
		{"$MCMPageMainMiscDG", "", 30},
		{"$MCMPageMainMiscDB", "", 31},

		{" ", "LegacyoftheDragonborn.esm", -1},
		{"$Header5", "LegacyoftheDragonborn.esm", -1},
		{"$MCMPageMuseum", "LegacyoftheDragonborn.esm", 52},
		{"$MCMPageExplorer", "LegacyoftheDragonborn.esm", 56},
		{"$MCMPageMisc", "LegacyoftheDragonborn.esm", 53},
		{"$MCMPageNotes", "LegacyoftheDragonborn.esm", 54},
		{"$MCMPageJournals", "LegacyoftheDragonborn.esm", 55},
	} };

	inline std::vector< std::tuple<const char*, const char*, int32_t>> MiscMCMPagesDefs =
	{ {
		{"$Header9", "", -1},
		{"$MCMPagePets", "", -1},
		{"$MCMPageShouts", "", -1},
		{"$MCMPageHouses", "", -1},
		{"$MCMPageShrines", "", -1},
		{"$MCMPageDoomstone", "", -1},
		{"$MCMPageBarenziah", "", -1},
		{"$MCMPageKillCounts", "", -1},

		{" ", "", -1},
		{"$HeaderLocations", "", -1},
		{"$MCMPageLocations1", "", -1},
		{"$MCMPageLocations2", "", -1},
		{"$MCMPageLocations3", "", -1},
		{"$MCMPageLocations4", "", -1},
		{"$MCMPageLocations5", "", -1},

		{" ", "", -1},
		{"$HeaderEnchantments", "", -1},
		{"$MCMPageAEnchantments", "", -1},
		{"$MCMPageWEnchantments", "", -1},
	} };

	inline std::vector<std::string> MainMCMPages{};
	inline std::vector<std::string> MiscMCMPages{};
	inline std::vector<std::string> PatchPages{};

	class MCMAPI final 
	{
	private:
		MCMAPI() = default;
		~MCMAPI() = default;

	public:
		static void						Register();
		static bool						RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		static MCMAPI*					GetSingleton() { static MCMAPI singleton; return &singleton; }
		static void						OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);

		static int32_t					GetSkyUIMCMPositionalIndex(RE::StaticFunctionTag*, std::string MCMPage);
		static int32_t					GetMCMPageIdentifierFromName(RE::StaticFunctionTag*, std::string MCMPage);
		static void						BuildMCMPages(RE::StaticFunctionTag*);
		static void						AddMainPatchedPageDefinitions(std::vector<std::tuple<std::string, std::string, uint32_t>>);
		static void						AddMiscPatchedPageDefinitions(std::vector<std::pair<std::string, CMiscPatch*>> defs);
		
		static int32_t					GetValidMainPatchPageID(RE::StaticFunctionTag*, std::string MCMPage);
		static std::vector<std::string> GetPageConfiguration(RE::StaticFunctionTag*, std::string mcmpage, int32_t pageNumber);

		static bool						IsSettingsPage(RE::StaticFunctionTag*, std::string MCMPage);
		static std::vector<std::string> GetMCMPages(RE::StaticFunctionTag*, std::int32_t menu_identifier);

		static int32_t					GetQuestID(RE::StaticFunctionTag*, std::string a_page, int32_t activePage);
		static int32_t					GetMultiPageCount(RE::StaticFunctionTag*, std::string MCMPage);
		static std::vector<std::string> GetMultiPageSplashScreenConfig(RE::StaticFunctionTag*, std::string mcmpage);

		static int32_t					GetTotalEntriesForPage(RE::StaticFunctionTag*, std::string a_page);
		static int32_t					GetTotalEntriesFoundForPage(RE::StaticFunctionTag*, std::string a_page);
		static int32_t					GetPageNumberForForm(RE::StaticFunctionTag*, std::string a_page, std::string a_name);
		static int32_t					GetPageNumberForSection(RE::StaticFunctionTag*, std::string a_page, std::string a_name);

		static int32_t					GetActivePage(RE::StaticFunctionTag*, std::string a_page);
		static void						SetActivePage(RE::StaticFunctionTag*, std::string a_page, int32_t a_value);

		static int32_t					GetDefaultPage(RE::StaticFunctionTag*, std::string a_page);
		static void						SetDefaultPage(RE::StaticFunctionTag*, std::string a_page, int32_t a_value);

		static std::vector<std::string> GetSearchHistory(RE::StaticFunctionTag*, std::string a_page);
		static void						ClearSearchHistory(RE::StaticFunctionTag*, std::string a_page);
		static void						AddSearchTerm(RE::StaticFunctionTag*, std::string a_page, std::string a_value);

		static bool						GetUseDefaultPage(RE::StaticFunctionTag*, std::string a_page);
		static void						SetUseDefaultPage(RE::StaticFunctionTag*, std::string a_page, bool a_value);
		static void						ResetPageSettings(RE::StaticFunctionTag*, std::string a_page);
		static std::string				GetActivePageName(RE::StaticFunctionTag*, int32_t activePage, std::string mcmpage);

		static int						GetDisplayValueForTotalAndFoundItemsByIdentifier(int32_t a_id, int32_t defaultValue);

		static int						ModifyTotals(std::string a_pageName, int value, bool isTotal);
		static void						DisplayVanillaTrackingPatches();
		static void						DisplayModAddedTrackingPatches();
		static std::vector<std::string> SearchMultiPage(RE::StaticFunctionTag*, std::string a_page, std::string s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t i_searchType);
		static std::string				GetSectionNameForForm(std::string a_page, std::string a_name, bool incPageNumber);

		template <typename T = RE::FormID>
		static void						AddVisibilityOption(const T& identifier, const std::string& pageName, int32_t optSelection)
		{
			if constexpr (std::is_same_v<T, RE::FormID>) {
				formVisibilityMap.emplace(identifier, std::make_pair(pageName, optSelection - 1));
			}

			if constexpr (std::is_same_v<T, std::string>) {
				questVisibilityMap.emplace(identifier, std::make_pair(pageName, optSelection - 1));
			}
		}

		static bool						IsFormVisible(RE::StaticFunctionTag*, std::string pageName, RE::TESForm* a_form, int32_t optSelection);
		static bool						IsQuestVisible(RE::StaticFunctionTag*, std::string pageName, std::string editorID, int32_t optSelection);
		static bool						HasDropDownMenu(RE::StaticFunctionTag*, std::string a_page);
		static std::string				GetDropDownMenuName(RE::StaticFunctionTag*, std::string a_page);
		static std::string				GetDropDownMenuHighlight(RE::StaticFunctionTag*, std::string a_page);
		static std::vector<std::string> GetDropDownMenuOptions(RE::StaticFunctionTag*, std::string a_page);
		static void						SetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page, int32_t a_value);
		static int32_t					GetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page);

		MCMAPI(MCMAPI const&) = delete;
		MCMAPI(MCMAPI const&&) = delete;
		MCMAPI operator=(MCMAPI&) = delete;
		MCMAPI operator=(MCMAPI&&) = delete;
	};
}