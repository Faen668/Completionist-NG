#include "MCMHandler.hpp"
#include "Events.hpp"
#include "ScriptObject.hpp"
#include "Serialization.hpp"
#include "DKUtil/Utility.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CHCMHandler 
{
	void MCMAPI::Register() 
	{
		//Register menu open event.
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(&OnMenuOpenCloseEvent);

		//Build MCM Page Definitions.
		BuildMCMPages(nullptr);
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

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

	bool MCMAPI::HasDropDownMenu(RE::StaticFunctionTag*, std::string a_page)
	{
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (!DKUtil::string::iequals(a_page, pageName))
			{
				continue;
			};

			return patchData->HasDropDownMenu;
		};
		return false;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::GetDropDownMenuOptions(RE::StaticFunctionTag*, std::string a_page) 
	{
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (!DKUtil::string::iequals(a_page, pageName))
			{
				continue;
			};

			return patchData->DropDownMenu.options;
		};
		return {};
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::string MCMAPI::GetDropDownMenuName(RE::StaticFunctionTag*, std::string a_page)
	{
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (!DKUtil::string::iequals(a_page, pageName))
			{
				continue;
			};

			return patchData->DropDownMenu.name;
		};
		return {};
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::string MCMAPI::GetDropDownMenuHighlight(RE::StaticFunctionTag*, std::string a_page)
	{
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (!DKUtil::string::iequals(a_page, pageName))
			{
				continue;
			};

			return patchData->DropDownMenu.highlight;
		};
		return {};
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	bool MCMAPI::CanLoadPage(RE::StaticFunctionTag*, std::string a_page) 
	{
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (!DKUtil::string::iequals(a_page, pageName))
			{
				continue;
			};

			return patchData->CanLoadPage;
		};
		return {};
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) 
	{
		if (a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) {
			BuildMCMPages(nullptr);
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::AddMainPatchedPageDefinitions(std::vector<std::tuple<std::string, std::string, uint32_t>> defs) {

		MainPatchesMCMPagesDefs = defs;

		for (auto& [page, mod, id] : defs) {
			INFO("Quest Page Definition Added: {} - {} - {}", page, mod, id);
		}
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

	int32_t MCMAPI::GetQuestID(RE::StaticFunctionTag*, std::string a_page, int32_t activePage) {

		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (DKUtil::string::iequals(a_page, pageName)) {
				for (auto& section : patchData->type_sections)
				{
					if (section.displayOnPage == activePage)
					{
						return section.quest_data_id;
					}
				}
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetMultiPageCount(RE::StaticFunctionTag*, std::string a_page) {
		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (DKUtil::string::iequals(a_page, pageName)) {
				return patchData->PageCount + 1;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int MCMAPI::GetDisplayValueForTotalAndFoundItemsByIdentifier(int32_t a_id, int32_t defaultValue)
	{
		int v = defaultValue;

		for (auto& [pageName, patchData] : CustomPatches)
		{
			for (auto& sectionData : patchData->type_sections)
			{
				if (sectionData.ID == a_id)
				{
					int currentUserOption = GetCurrentUserOption(nullptr, pageName);

					for (auto& form : sectionData.forms)
					{
						if (!IsFormVisible(nullptr, pageName, form, currentUserOption))
						{
							v--;
						}
					}
				}
			}
		}
		return v;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int MCMAPI::ModifyTotals(std::string a_pageName, int value, bool isTotal)
	{
		int v = value;

		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (DKUtil::string::iequals(a_pageName, pageName))
			{
				int currentUserOption = GetCurrentUserOption(nullptr, pageName);

				for (auto& section : patchData->type_sections)
				{
					if (section.type == CMiscPatchType::kQuests)
					{
						for (auto& questData : section.quest_data_array)
						{
							if (!IsQuestVisible(nullptr, pageName, questData->GetEditorID(), currentUserOption))
							{
								if (isTotal) {
									v--;
								}
								else
								{
									if (
										CFramework_Master::CQuestKeys_Manual.HasKey(questData->GetKey()) ||
										CFramework_Master::CQuestKeys_Natural.HasKey(questData->GetKey()))
									{
										v--;
									}
								}
							}
						}
					}
					else
					{
						for (auto& form : section.forms)
						{
							if (!IsFormVisible(nullptr, pageName, form, currentUserOption))
							{
								if (isTotal) {
									v--;
								}
								else
								{
									if (CFramework_Master::FoundItemData.HasForm(form) || CFramework_Master::FoundItemData_NoShow.HasForm(form)) {
										v--;
									}
								}
							}
						}
					}
				}
			}
		}
		return v;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetActivePage(RE::StaticFunctionTag*, std::string a_page) 
	{
		return CFramework_Master::PatchSettings.GetActivePage(a_page);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetDefaultPage(RE::StaticFunctionTag*, std::string a_page)
	{
		return CFramework_Master::PatchSettings.GetDefaultPage(a_page);
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
		CFramework_Master::PatchSettings.UpdateSetting<Serialization::PatchDataEnum::kcurrentUserOption>(a_page, a_value);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::AddSearchTerm(RE::StaticFunctionTag*, std::string a_page, std::string a_value)
	{
		CFramework_Master::PatchSettings.AddSearchTerm(a_page, a_value);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::ClearSearchHistory(RE::StaticFunctionTag*, std::string a_page)
	{
		return CFramework_Master::PatchSettings.ClearSearchHistory(a_page);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::string MCMAPI::GetSectionNameForForm(std::string a_page, std::string a_name, bool incPageNumber) {

		for (auto& [pageName, patchData] : CustomPatches)
		{
			if (DKUtil::string::iequals(a_page, pageName))
			{
				for (auto& section : patchData->type_sections)
				{
					if (section.type == CMiscPatchType::kQuests)
					{
						for (auto& qstData : section.quest_data_array)
						{
							if (DKUtil::string::iequals(a_name, qstData->GetName()))
							{
								std::string substr = section.pageheaderL;

								if (substr.ends_with(":"))
								{
									substr = substr.substr(0, substr.length() - 1);
								};

								return incPageNumber ? fmt::format("Page {} - {}", section.displayOnPage, substr) : fmt::format("{}", substr);
							};
						};
					}
					else
					{
						for (auto& name : section.names)
						{
							if (DKUtil::string::iequals(a_name, name))
							{
								auto& substr = section.pageheaderL;

								if (substr.ends_with(":"))
								{
									substr = substr.substr(0, substr.length() - 1);
								};

								return incPageNumber ? fmt::format("Page {} - {}", section.displayOnPage, substr) : fmt::format("{}", substr);
							};
						};
					};
				};
				return "";
			};
		};
		return "";
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::BuildMCMPages(RE::StaticFunctionTag*) {

		using namespace CFramework_Master;

		MainMCMPages.clear();
		MiscMCMPages.clear();
		PatchPages.clear();

		for (auto& [page, mod, id] : MainMCMPagesDefs) {

			if (!DKUtil::string::is_empty(mod) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				continue;
			}

			MainMCMPages.push_back(page);
		}

		if (MainPatchesMCMPagesDefs.size() > 0)
		{
			MainMCMPages.push_back(" ");
			MainMCMPages.push_back("$HeaderU");
		}

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (!DKUtil::string::is_empty(mod.c_str()) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				INFO("ERROR ON PAGE DEFINITIONS");
				continue;
			}

			MainMCMPages.push_back(page);
		}

		for (auto& [page, mod, id] : MiscMCMPagesDefs) {

			if (!DKUtil::string::is_empty(mod) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				continue;
			}

			MiscMCMPages.push_back(page);
		}

		DisplayVanillaTrackingPatches();
		DisplayModAddedTrackingPatches();
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::DisplayVanillaTrackingPatches() {
		std::string lastHeader;
		bool isDefaultHeaderAdded = false;

		for (auto& [pageName, patchData] : CustomPatches) {
			if (!patchData->isVanillaTracking) {
				continue;
			}

			// Handle default header for patches without a specific Header
			if (patchData->headerName.empty()) {
				if (!isDefaultHeaderAdded) {
					PatchPages.push_back("$HeaderO"); // Default header
					isDefaultHeaderAdded = true;
				}
			}
			else {
				// Handle new specific Header group
				if (lastHeader != patchData->headerName) {
					if (!lastHeader.empty() || isDefaultHeaderAdded) {
						PatchPages.push_back(" "); // Empty page before a new group
					}
					PatchPages.push_back("~~ " + patchData->headerName + " ~~"); // Add the new header
					lastHeader = patchData->headerName;
				}
			}

			PatchPages.push_back(pageName); // Add the patch page
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::DisplayModAddedTrackingPatches() {
		std::string lastHeader;
		bool isDefaultHeaderAdded = false;
		bool isFirstGroupAdded = false;

		for (auto& [pageName, patchData] : CustomPatches) {
			if (patchData->isVanillaTracking) {
				continue;
			}

			// Handle default header for patches without a specific Header
			if (patchData->headerName.empty()) {
				if (!isDefaultHeaderAdded) {
					if (!PatchPages.empty() || !isFirstGroupAdded) {
						PatchPages.push_back(" "); // Empty page before default header
					}
					PatchPages.push_back("$HeaderU"); // Default header
					isDefaultHeaderAdded = true;
					isFirstGroupAdded = true; // Mark that the first group has been added
				}
			}
			else {
				// Handle new specific Header group
				if (lastHeader != patchData->headerName) {
					if (!lastHeader.empty() || isDefaultHeaderAdded || !isFirstGroupAdded) {
						PatchPages.push_back(" "); // Empty page before a new group
					}
					PatchPages.push_back("~~ " + patchData->headerName + " ~~"); // Add the new header
					lastHeader = patchData->headerName;
					isFirstGroupAdded = true; // Mark that the first group has been added
				}
			}

			PatchPages.push_back(pageName); // Add the patch page
		}
	}

	std::string GetPercentage(double found, double total)
	{
		double percentage = (double)found / total * 100;
		return std::to_string((int)percentage);
	};

};