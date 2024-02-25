#include "MCMHandler.hpp"
#include "ScriptObject.hpp"
#include "Serialization.hpp"
#include "DKUtil/Utility.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CHCMHandler 
{
	void MCMAPI::Register() 
	{
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);

		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MCMAPI::GetSingleton()));
		BuildMCMPages(nullptr);
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	auto MCMAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("GetMCMPages", "Completionist_Native", GetMCMPages);
		a_vm->RegisterFunction("BuildMCMPages", "Completionist_Native", BuildMCMPages);
		a_vm->RegisterFunction("GetSkyUIMCMPositionalIndex", "Completionist_Native", GetSkyUIMCMPositionalIndex);
		a_vm->RegisterFunction("GetMCMPageIdentifierFromName", "Completionist_Native", GetMCMPageIdentifierFromName);
		a_vm->RegisterFunction("GetValidMainPatchPageID", "Completionist_Native", GetValidMainPatchPageID);
		a_vm->RegisterFunction("GetPageConfiguration", "Completionist_Native", GetPageConfiguration);
		a_vm->RegisterFunction("IsSettingsPage", "Completionist_Native", IsSettingsPage);

		a_vm->RegisterFunction("IsMultiPage", "Completionist_Native", IsMultiPage);
		a_vm->RegisterFunction("GetMultiPageCount", "Completionist_Native", GetMultiPageCount);
		a_vm->RegisterFunction("GetTotalEntriesForPage", "Completionist_Native", GetTotalEntriesForPage);
		a_vm->RegisterFunction("GetTotalEntriesFoundForPage", "Completionist_Native", GetTotalEntriesFoundForPage);
		a_vm->RegisterFunction("GetPageNumberForForm", "Completionist_Native", GetPageNumberForForm);
		a_vm->RegisterFunction("GetPageNumberForSection", "Completionist_Native", GetPageNumberForSection);

		a_vm->RegisterFunction("GetActivePage", "Completionist_Native", GetActivePage);
		a_vm->RegisterFunction("SetActivePage", "Completionist_Native", SetActivePage);
		a_vm->RegisterFunction("GetDefaultPage", "Completionist_Native", GetDefaultPage);
		a_vm->RegisterFunction("SetDefaultPage", "Completionist_Native", SetDefaultPage);
		a_vm->RegisterFunction("GetUseDefaultPage", "Completionist_Native", GetUseDefaultPage);
		a_vm->RegisterFunction("SetUseDefaultPage", "Completionist_Native", SetUseDefaultPage);
		a_vm->RegisterFunction("AddSearchTerm", "Completionist_Native", AddSearchTerm);
		a_vm->RegisterFunction("GetSearchHistory", "Completionist_Native", GetSearchHistory);
		a_vm->RegisterFunction("ClearSearchHistory", "Completionist_Native", ClearSearchHistory);
		a_vm->RegisterFunction("ResetPageSettings", "Completionist_Native", ResetPageSettings);
		a_vm->RegisterFunction("GetMultiPageSplashScreenConfig", "Completionist_Native", GetMultiPageSplashScreenConfig);
		a_vm->RegisterFunction("GetQuestID", "Completionist_Native", GetQuestID);
		a_vm->RegisterFunction("GetActivePageName", "Completionist_Native", GetActivePageName);
		a_vm->RegisterFunction("SearchMultiPage", "Completionist_Native", SearchMultiPage);
		
		return true;
	}
	
	bool MCMAPI::IsSettingsPage(RE::StaticFunctionTag*, std::string a_page) {
		return a_page == "" || a_page == " " || DKUtil::string::icontains(a_page, "MCMPageSettings") || DKUtil::string::icontains(a_page, "Header");
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::int32_t MCMAPI::GetSkyUIMCMPositionalIndex(RE::StaticFunctionTag*, std::string MCMPage)
	{
		auto ConfigBase = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00802, "SkyUI_SE.esp")), "SKI_ConfigManager");
		if (!ConfigBase) { return -1; }

		auto namearrray = ScriptObject::GetArray(ConfigBase, "_modNames");
		if (!namearrray) { return -1; }

		std::int32_t Idx{};
		for (auto& name : *namearrray)
		{
			if (DKUtil::string::iequals(name.GetString(), MCMPage))
			{
				return Idx;
			}
			Idx++; 
		}

		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> GetQuestNames(std::string a_page)
	{
		std::vector<std::string> list{};

		for (auto& [groupName, group] : CustomPatches)
		{
			for (auto& page : group->GetPatches())
			{
				if (!DKUtil::string::iequals(a_page, page.first))
				{
					continue;
				};

				for (auto& section : page.second->type_sections)
				{
					if (section.type == CMiscPatchType::kQuests)
					{
						for (auto& quest_name : section.names)
						{
							list.push_back(quest_name);
						};
					};
				};
			};
		};
		return list;
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::SearchMultiPage(RE::StaticFunctionTag*, std::string a_page, std::string s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t i_searchType)
	{
		std::vector<std::string> list{};
		auto result = 1;
		auto process = false;

		for (auto& [form, name, mcmPage, Category] : CFramework_Master::CItemsDataVec)
		{
			if (!DKUtil::string::iequals(a_page, mcmPage))
			{
				continue;
			};

			if (list.size() >= i_maxResults)
			{
				break;
			};

			switch (i_searchType)
			{
			case 0: { process = DKUtil::string::icontains(name, s_term); break; }
			case 1: { process = name.starts_with(s_term); break; }
			case 2: { process = DKUtil::string::iequals(name, s_term); break; }
			default: break;
			}

			if (process)
			{
				if (b_ignoreCompleted && (CFramework_Master::FoundItemData.HasForm(form) || CFramework_Master::FoundItemData_NoShow.HasForm(form))) 
				{
					continue;
				}

				list.push_back("$PatchResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + GetSectionNameForForm(mcmPage, name, true) + "}{" + CFramework_Master::FrameworkAPI::GetLocalisedCategory(Category) + "}{" + name + "}");
				list.push_back(mcmPage);
				list.push_back(name);
				list.push_back("Misc");
				result++;
			};
		};

		for (auto& [data, name, ID, key] : CQuestMaster::CQuestDataVec)
		{
			if (!DKUtil::string::iequals(a_page, data->mcmPage))
			{
				continue;
			};

			if (list.size() >= i_maxResults)
				break;

			switch (i_searchType)
			{
			case 0: { process = data->HasSearchData() && DKUtil::string::icontains(data->GetSearchTerm(), s_term); break; }
			case 1: { process = data->HasSearchData() && data->GetSearchTerm().starts_with(s_term); break; }
			case 2: { process = data->HasSearchData() && DKUtil::string::iequals(data->GetSearchTerm(), s_term); break; }
			default: break;
			}

			if (process) {

				if (b_ignoreCompleted && (CFramework_Master::CQuestKeys_Manual.HasKey(key) || CFramework_Master::CQuestKeys_Natural.HasKey(key)))
				{
					continue;
				}

				list.push_back("$PatchResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + GetSectionNameForForm(data->mcmPage, name, true) + "}{" + GET_LOC_STRING_BY_KEY("Category_Quest") + "}{" + name + "}");
				list.push_back(data->mcmPage);
				list.push_back(name);
				list.push_back("Misc");
				result++;
			};
		};

		for (auto i = 0; i < list.size(); i += 4)
		{
			list[i].replace(list[i].find("["), 9, std::to_string(list.size() / 4));
		}

		return list;
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::int32_t MCMAPI::GetMCMPageIdentifierFromName(RE::StaticFunctionTag*, std::string MCMPage)
	{
		for (auto& [page, mod, id] : MainMCMPagesDefs) {

			if (DKUtil::string::iequals(page, MCMPage.c_str())) {
				return id;
			}
		}

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (DKUtil::string::iequals(page.c_str(), MCMPage.c_str())) {
				return id;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	EventResult	MCMAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
		
		if (a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) { 
			BuildMCMPages(nullptr);
		}

		return EventResult::kContinue;
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

	void MCMAPI::AddMiscPatchedPageDefinitions(std::vector<std::pair<std::string, CMiscPatchGroupData*>> defs) {

		CustomPatches = defs;

		for (auto& [groupName, group] : defs)
		{
			for (auto& page : group->GetPatches())
			{
				if (page.second->log_install)
				{
					for (auto& section : page.second->type_sections)
					{
						INFO("Misc Page Section Added: {} - {} - {}", page.first, section.pageheaderL, section.ID);
					};
				};
			};
		};
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetQuestID(RE::StaticFunctionTag*, std::string a_page, int32_t activePage) {
		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				if (DKUtil::string::iequals(a_page, pageName)) {
					for (auto& section : patchData->type_sections)
					{
						if (section.displayOnPage == activePage)
						{
							return section.quest_data_id;
						}
					}
				}
			};
		};
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	bool MCMAPI::IsMultiPage(RE::StaticFunctionTag*, std::string a_page) {
		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				if (DKUtil::string::iequals(a_page, pageName)) {
					return patchData->multipage;
				}
			};
		};
		return false;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetMultiPageCount(RE::StaticFunctionTag*, std::string a_page) {
		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				if (DKUtil::string::iequals(a_page, pageName)) {
					return patchData->multiPageCount + 1;
				}
			};
		};
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetTotalEntriesFoundForPage(RE::StaticFunctionTag*, std::string a_page) {
		int32_t found = 0;

		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				if (DKUtil::string::iequals(a_page, pageName)) {
					for (auto& section : patchData->type_sections)
					{
						if (section.type == CMiscPatchType::kQuests)
						{
							auto totals = CQuestMaster::QuestAPI::qGetQuestCompletionTotals(section.quest_data_array);
							section.found = totals.first;
							section.total = totals.second;
						};

						found += section.found;
					};
				};
			};
		};
		return found;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetTotalEntriesForPage(RE::StaticFunctionTag*, std::string a_page) {
		int32_t total = 0;

		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				if (DKUtil::string::iequals(a_page, pageName)) {
					for (auto& section : patchData->type_sections)
					{
						if (section.type == CMiscPatchType::kQuests)
						{
							auto totals = CQuestMaster::QuestAPI::qGetQuestCompletionTotals(section.quest_data_array);
							section.found = totals.first;
							section.total = totals.second;
						};

						total += section.total;
					};
				};
			};
		};
		return total;
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

	bool MCMAPI::GetUseDefaultPage(RE::StaticFunctionTag*, std::string a_page)
	{
		return CFramework_Master::PatchSettings.GetUseDefault(a_page);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::SetActivePage(RE::StaticFunctionTag*, std::string a_page, int32_t a_value) 
	{
		CFramework_Master::PatchSettings.UpdateSetting<Serialization::PatchDataEnum::kactivePage>(a_page, a_value);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::SetDefaultPage(RE::StaticFunctionTag*, std::string a_page, int32_t a_value)
	{
		CFramework_Master::PatchSettings.UpdateSetting<Serialization::PatchDataEnum::kdefaultPage>(a_page, a_value);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::SetUseDefaultPage(RE::StaticFunctionTag*, std::string a_page, bool a_value)
	{
		CFramework_Master::PatchSettings.UpdateSetting<Serialization::PatchDataEnum::kuse_default_page>(a_page, a_value);
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

	std::vector<std::string> MCMAPI::GetSearchHistory(RE::StaticFunctionTag*, std::string a_page)
	{
		return CFramework_Master::PatchSettings.GetCompiledSearchHistory(a_page);
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

	void MCMAPI::ResetPageSettings(RE::StaticFunctionTag*, std::string a_page)
	{
		CFramework_Master::PatchSettings.ResetSettings(a_page);
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetPageNumberForForm(RE::StaticFunctionTag*, std::string a_page, std::string a_name) {

		for (auto& [groupName, group] : CustomPatches) 
		{
			for (auto& [pageName, patchData] : group->GetPatches())
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
									return section.displayOnPage;
								};
							};
						}
						else
						{
							for (auto& name : section.names)
							{
								if (DKUtil::string::iequals(a_name, name))
								{
									return section.displayOnPage;
								};
							};
						};
					};
					return 0;
				};
			};
		};
		return 0;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::string MCMAPI::GetSectionNameForForm(std::string a_page, std::string a_name, bool incPageNumber) {

		for (auto& [groupName, group] : CustomPatches)
		{
			for (auto& [pageName, patchData] : group->GetPatches())
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
		};
		return "";
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetPageNumberForSection(RE::StaticFunctionTag*, std::string a_page, std::string a_name) {

		for (auto& [groupName, group] : CustomPatches)
		{
			for (auto& [pageName, patchData] : group->GetPatches())
			{
				if (DKUtil::string::iequals(a_page, pageName))
				{
					for (auto& [Idx, page] : patchData->section_defs)
					{
						if (DKUtil::string::iequals(a_name, patchData->prependPageNumber ? fmt::format("{}. {}", std::to_string(Idx), page) : page))
						{
							return Idx;
						};
					};
					return 0;
				};
			};
		};
		return 0;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetValidMainPatchPageID(RE::StaticFunctionTag*, std::string a_page) {

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (DKUtil::string::iequals(page.c_str(), a_page.c_str())) {
				return id;
			}
		}
		return -1;
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

		DisplayOfficialPatches();
		DisplayCreationClubPatches();
		DisplayUnOfficialPatches();
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::DisplayOfficialPatches() {

		bool OfficialHeaderAdded{};

		for (auto& [gName, gData] : CustomPatches)
		{
			auto patches = gData->GetPatches<gData->kOfficial>();

			if (patches.size() > 0)
			{
				if (!OfficialHeaderAdded)
				{
					PatchPages.push_back("$HeaderO");
					OfficialHeaderAdded = true;
				}

				if (!gData->defaultGroup && PatchPages.size() > 1)
				{
					PatchPages.push_back(" ");
				};

				for (auto& page : patches)
				{
					PatchPages.push_back(page.first);
				};
			};
		};
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::DisplayUnOfficialPatches() 
	{
		bool UnOfficialHeaderAdded{};
		for (auto& [gName, gData] : CustomPatches)
		{
			auto patches = gData->GetPatches<gData->kUnofficial>();

			if (patches.size() > 0)
			{
				if (PatchPages.size() > 0)
				{
					PatchPages.push_back(" ");
				};
				PatchPages.push_back("$HeaderU");
				UnOfficialHeaderAdded = true;

				if (!gData->defaultGroup && PatchPages.size() > 1)
				{
					PatchPages.push_back(" ");
				};

				for (auto& page : patches)
				{
					PatchPages.push_back(page.first);
				};
			};
		};
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::DisplayCreationClubPatches()
	{
		bool HeaderAdded{};
		for (auto& [gName, gData] : CustomPatches)
		{
			auto patches = gData->GetPatches<gData->kCreationClub>();

			if (patches.size() > 0)
			{
				if (PatchPages.size() > 0)
				{
					PatchPages.push_back(" ");
				};
				PatchPages.push_back("$HeaderC");
				HeaderAdded = true;

				if (!gData->defaultGroup && PatchPages.size() > 1)
				{
					PatchPages.push_back(" ");
				};

				for (auto& page : patches)
				{
					PatchPages.push_back(page.first);
				};
			};
		};
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::GetMCMPages(RE::StaticFunctionTag*, std::int32_t menu_identifier) {

		switch (menu_identifier)
		{
		case 0: return MainMCMPages; break;
		case 1: return MiscMCMPages; break;
		case 2: return PatchPages; break;
		default: return std::vector<std::string>{}; break;
		}
	};

	std::string GetPercentage(double found, double total)
	{
		double percentage = (double)found / total * 100;
		return std::to_string((int)percentage);
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::GetMultiPageSplashScreenConfig(RE::StaticFunctionTag*, std::string mcmpage)
	{
		std::vector<std::string> config{};
		int32_t total{};
		int32_t found{};

		for (auto& [groupName, group] : CustomPatches) 
		{
			for (auto& [pageName, patch] : group->GetPatches())
			{
				if (DKUtil::string::iequals(pageName.c_str(), mcmpage.c_str()))
				{
					for (auto& [Idx, page] : patch->section_defs) 
					{
						total = 0;
						found = 0;

						for (auto& section : patch->type_sections)
						{
							if (section.displayOnPage == Idx && section.enabled)
							{
								if (section.type == CMiscPatchType::kQuests)
								{
									auto totals = CQuestMaster::QuestAPI::qGetQuestCompletionTotals(section.quest_data_array);
									section.found = totals.first;
									section.total = totals.second;
								};

								total += section.total;
								found += section.found;
							};
						};

						if (total > 0) {
							config.push_back(patch->prependPageNumber ? fmt::format("{}. {}", std::to_string(Idx), page) : page);
							config.push_back(fmt::format("{}/{} - {}% Completed", found, total, GetPercentage(found, total)));
						}
					}
				};
			};
		};
		return config;
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::string MCMAPI::GetActivePageName(RE::StaticFunctionTag*, int32_t activePage, std::string mcmpage)
	{
		if (activePage == 0) {
			return GET_LOC_STRING_BY_KEY("MCMText_PageDirectory");
		};

		std::vector<std::string> sections{};
		std::string page_name{};

		for (auto& [groupName, group] : CustomPatches)
		{
			for (auto& [pageName, patch] : group->GetPatches())
			{
				if (DKUtil::string::iequals(pageName.c_str(), mcmpage.c_str()))
				{
					return patch->section_defs.at(activePage);
				};
			};
		};
		return GET_LOC_STRING_BY_KEY("MCMText_Collectables");
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::GetPageConfiguration(RE::StaticFunctionTag*, std::string mcmpage, int32_t pageNumber)
	{
		std::vector<std::string> config{};

		for (auto& [groupName, group] : CustomPatches) {
			for (auto& [pageName, patch] : group->GetPatches()) {

				if (DKUtil::string::iequals(pageName.c_str(), mcmpage.c_str()))
				{
					config.push_back("0");

					for (auto& section : patch->type_sections) {
						if (section.enabled && pageNumber == section.displayOnPage) {
							config.push_back(std::to_string(section.ID));
							config.push_back(section.pageheaderL);
							config.push_back(section.pageheaderR);
							config.push_back(std::to_string(static_cast<int32_t>(section.type)));
							config[0] = std::to_string(std::stoi(config[0]) + 1);
						};
					};
				};
			};
		};
		return config;
	};
};