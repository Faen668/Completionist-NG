#pragma once
#include "MiscPatchStruct.hpp"

namespace CHCMHandler
{
	//FormID, Menu uuid, Option Index
	inline std::unordered_map<RE::FormID, std::pair<std::string, int32_t>> formVisibilityMap{};

	//EditorID, Menu uuid, Option Index
	inline std::unordered_map<std::string, std::pair<std::string, int32_t>> questVisibilityMap{};

	inline std::vector<std::pair<std::string, CMiscPatch*>> CustomPatches{};	
	inline std::vector<std::tuple<const char*, const char*, int32_t>> MainMCMPagesDefs =
	{ {
		{"$MCMPageSettings", "", -1},
		{"$MCMPageSettings3", "", -1},
		{"$MCMPageSettings6", "LegacyoftheDragonborn.esm", -1},
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

	class MCMAPI final 
	{
	private:
		MCMAPI() = default;
		~MCMAPI() = default;

	public:
		static MCMAPI*					GetSingleton() { static MCMAPI singleton; return &singleton; }

		static void						AddMiscPatchedPageDefinitions(std::vector<std::pair<std::string, CMiscPatch*>> defs);


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
		static void						SetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page, int32_t a_value);
		static int32_t					GetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page);

		MCMAPI(MCMAPI const&) = delete;
		MCMAPI(MCMAPI const&&) = delete;
		MCMAPI operator=(MCMAPI&) = delete;
		MCMAPI operator=(MCMAPI&&) = delete;
	};
}