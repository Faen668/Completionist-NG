#include "MCMHandler.hpp"
#include "Serialization.hpp"
#include "DKUtil/Utility.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CHCMHandler 
{
	bool MCMAPI::IsFormVisible(RE::StaticFunctionTag*, std::string pageName, RE::TESForm* a_form, int32_t optSelection)
	{
		bool has_visibility_condition = a_form && formVisibilityMap.contains(a_form->GetFormID());
		if (!has_visibility_condition) { return true; }

		if (optSelection == -1) {
			optSelection = GetCurrentUserOption(nullptr, pageName);
		}

		auto &entry = formVisibilityMap.at(a_form->GetFormID());

		if (entry.first == pageName && entry.second == optSelection) { return true; }

		return false;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	bool MCMAPI::IsQuestVisible(RE::StaticFunctionTag*, std::string pageName, std::string editorID, int32_t optSelection)
	{
		bool has_visibility_condition = questVisibilityMap.contains(editorID);
		if (!has_visibility_condition) { return true; }

		if (optSelection == -1) {
			optSelection = GetCurrentUserOption(nullptr, pageName);
		}

		auto& entry = questVisibilityMap.at(editorID);

		if (entry.first == pageName && entry.second == (optSelection)) { return true; }

		return false;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::AddMiscPatchedPageDefinitions(std::vector<std::pair<std::string, CMiscPatch*>> defs) {

		CustomPatches = defs;

		for (auto& [pageName, patchData] : defs)
		{
			if (patchData->log_install)
			{
				for (auto& section : patchData->type_sections)
				{
					INFO("Misc Page Section Added: {} - {} - {}", pageName, section.pageheaderL, section.ID);
				}
			}
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page)
	{
		return CFramework_Master::PatchSettings.GetCurrentUserOption(a_page);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::SetCurrentUserOption(RE::StaticFunctionTag*, std::string a_page, int32_t a_value)
	{
		CFramework_Master::PatchSettings.SetCurrentUserOption(a_page, a_value);
	}
};
