#include "Structs.hpp"
#include "PatchListener.hpp"
#include "Import/SimpleIni.h"
#include "Internal Utility/MCMHandler.hpp"

namespace CExternalPatchHandler
{
	//---------------------------------------------------
	//-- Patch API Functions-----------------------------
	//---------------------------------------------------

	int32_t CHandler::GetRandomID()
	{
		std::random_device rd;     // Only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(500, 999999);
		int32_t x = uni(rng);

		while (std::ranges::find(usedIdentifiers, x) != usedIdentifiers.end())
		{
			x++;
		}
		return usedIdentifiers.emplace_back(x);
	}

	//---------------------------------------------------
	//-- Patch API Functions-----------------------------
	//---------------------------------------------------

	void CHandler::Register()
	{
		using namespace CHCMHandler;

		std::vector<std::string> files;
		std::vector<std::string> quest;
		std::vector<std::string> misc;
		using rdi = std::filesystem::recursive_directory_iterator;

		const std::filesystem::path path = L"Data/SKSE/Plugins/CompletionistData/Addons/";

		if (!std::filesystem::exists(path) || std::filesystem::is_empty(path))
		{
			INFO("Patch Folder Is Empty Or Does Not Exist.");
			return;
		}

		for (const auto& dirEntry : rdi(path))
		{
			auto file = dirEntry.path().string();

			if (!DKUtil::string::icontains(file, ".ini")) {
				continue;
			}

			files.push_back(file);
		};

		if (files.size() == 0)
		{
			return;
		};

		for (auto& file : files)
		{
			filename = file.substr(file.find_last_of("/") + 1);

			SI_Error rc{};

			ini.Reset();
			ini.SetUnicode();
			ini.SetMultiLine(true);
			rc = ini.LoadFile(file.c_str());

			if (rc < 0)
			{
				INFO("Unable to load useable data from file: {} : [{}]", filename, "File Corrupt Or Wrong Format");
				continue;
			};

			bool log_install = CVariables::V_Global_Patch_Logging || GetLoggingAllowed();

			if (!GetEnabled()) {
				if (log_install) {
					INFO("Unable to load useable data from file: {} : [{}]", filename, "Patch Disabled");
				}
				continue;
			};

			if (!ShouldInstall("Completionist Patch Data")) {
				if (log_install) {
					INFO("Unable to load useable data from file: {} : [{}]", filename, "Install Condition Not Met");
				}
				continue;
			};

			if (log_install) {
				INFO("Processing file: {}", filename);
			}

			ProcessiniFile(file);
		};

		LinkPatchAndQuestData();

		for (auto& patch : CustomPatches)
		{
			patch.second->CleanEmptyPages();
			patch.second->ValidatePageSizeRules();
		}

		if (CustomPatches.size() > 0)
		{
			std::sort(CustomPatches.begin(), CustomPatches.end(),
				[](const auto& a, const auto& b) {
					// Primary: Sort so that headers come before empty headers
					if (a.second->headerName.empty() && !b.second->headerName.empty()) {
						return false; // a should come after b
					}
					if (!a.second->headerName.empty() && b.second->headerName.empty()) {
						return true; // a should come before b
					}

					// Secondary: Group by headerName
					if (a.second->headerName != b.second->headerName) {
						return a.second->headerName < b.second->headerName;
					}
					// Tertiary: Sort by priority within the same header group
					if (a.second->priority != b.second->priority) {
						return a.second->priority < b.second->priority;
					}
					// Final: Alphabetical order of the string key
					return a.first < b.first;
				});

			MCMAPI::AddMiscPatchedPageDefinitions(CustomPatches);
		};
	};

	void CHandler::ProcessiniFile(std::string file)
	{
		auto IsTrackedEntity = [](CMiscPatchType a_type) -> bool
		{
			return std::find(CVariables::V_TrackedEntityTypes.begin(),
				CVariables::V_TrackedEntityTypes.end(), a_type) != CVariables::V_TrackedEntityTypes.end();
		};

		auto fName = file.substr(file.find_last_of("/") + 1);

		using namespace CHCMHandler;

		if (!Serialization::CompletionistData::IsModInstalled(GetPluginFileName())) {
			return;
		};

		auto* data = GetNewPatchData();
		data->log_install = CVariables::V_Global_Patch_Logging || GetLoggingAllowed();
		data->priority = GetPriority();
		data->isVanillaTracking = GetIsVanillaTracking();
		data->sortMode = GetSortMode();

		std::string lName = fName.substr(0, fName.find_last_of('.')) + ".txt";

		auto overrideName = GetOverrideLocalisationFileName();
		if (!DKUtil::string::iequals(overrideName, "None")) {
			lName = overrideName;
		}

		data->iniFileName = lName;
		data->BuildLocalisedMap();

		if (data->log_install)
		{
			INFO("Adding Patched Misc Support From File: {}", fName);
		}

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);
		sections.sort(typename CSimpleIniA::Entry::LoadOrder());

		auto header = GetHeaderName();
		data->headerName = header.empty() ? "" : data->GetLocStringByKey(header.c_str(), "headerName");
		data->mcmpage = data->GetLocStringByKey(GetMCMPageName().c_str(), "mcmpage");
		data->prependPageNumber = GetMultiPagePrependPageNumber();

		for (auto i = 1; i < 128; i++) {
			if (!IsPageLocalisationNameValid(i)) {
				data->PageCount = (i - 1);
				break;
			}
		}

		std::vector<int32_t> pageIdentifiers{};
		for (auto i = 0; i <= data->PageCount; i++)
		{
			pageIdentifiers.push_back(GetRandomID());
		}

		data->HasDropDownMenu = HasCustomDropDownMenu();
		if (data->HasDropDownMenu)
		{
			data->DropDownMenu.name = GetCustomDropDownMenuName();
			data->DropDownMenu.highlight = GetCustomDropDownMenuHighlight();

			int32_t menuIndex = 1;
			while (true)
			{
				std::string menuOption = GetCustomDropDownOptionName(menuIndex);
				if (menuOption == "ERROR") {
					break;
				}

				data->DropDownMenu.options.push_back(menuOption);
				menuIndex++;
			}
		}

		int Installed = 0;
		for (auto& section : sections)
		{
			auto sec = section.pItem;

			//Skip Patch Data.
			if (DKUtil::string::iequals(sec, "Completionist Patch Data"))
			{
				continue;
			};

			//Skip If entry Disabled.
			if (!GetEnabled(sec))
			{
				if (data->log_install) {
					INFO("Unable to load useable data from section: {} : [{}]", sec, "Entry Disabled.");
				}
				continue;
			};

			if (!ShouldInstall(sec)) {
				if (data->log_install) {
					INFO("Unable to load useable data from section: {} : [{}]", sec, "Failure On Conditional Mod Requirement.");
				}
				continue;
			}

			//Process Quest.
			auto IsQuestSection = GetIsQuestSection(sec);
			if (IsQuestSection) 
			{
				if (!IsTrackedEntity(CMiscPatchType::kQuests)) {
					if (data->log_install) {
						INFO("Skipping quest section {}: quests not tracked", sec);
					}
					continue;
				}

				//Skip if quest not found.
				RE::TESQuest* quest = RE::TESForm::LookupByEditorID<RE::TESQuest>(GetEditorID(sec));
				if (!quest) {
					if (data->log_install) {
						INFO("Unable to load useable data from quest: {} : [{}]", sec, "Quest Not Found");
					}
					continue;
				};

				//mPagelink is used to tie this quest to an existing page. (Does not create an mcm page and is grabbed by tied page) 
				int32_t mPagelink = pageIdentifiers[GetMultiPageValue(sec) - 1];
				if (mPagelink == -1) {
					continue;
				}

				//Set Quest Data
				CQuestData* quest_data = GetNewQuestData();
				quest_data->unique_identifier = GetUUID(sec);
				quest_data->init(false, data->mcmpage);
				quest_data->set_editorID(GetEditorID(sec));
				quest_data->set_quest_type(GetQuestType(sec));
				quest_data->set_completion_type(GetCompletionType(sec));
				quest_data->set_name(GetLocStringByKey(GetQuestName(sec).c_str(), data->translations));
				quest_data->set_highlight(GetLocStringByKey(GetHighlightText(sec).c_str(), data->translations));

				//Set Stage Data.
				if (quest_data->completion_type == CCompEnum::kStage) {
					CStageData* stage_data = GetNewStageData();
					stage_data->link = quest_data->GetKey();
					stage_data->type = GetStageType(sec);
					stage_data->stage = GetStage(sec);
					stage_data->optional_stage = GetOptionalStage(sec);
					quest_data->initPatchStageData(stage_data);
				};

				//Set Radiant Data.
				if (quest_data->completion_type == CCompEnum::kGlobl) {
					CRadiantData* radiant_data = GetNewRadiantData();
					radiant_data->link = quest_data->GetKey();

					radiant_data->baseID = GetBaseFormID(sec);
					radiant_data->variID = 0;
					radiant_data->stage = GetRadiantStage(sec);
					radiant_data->value = GetTimesRequired(sec);

					if (IsFavorQuest(sec)) {
						radiant_data->isFavorQuest = true;
						radiant_data->stage = -1;
						radiant_data->process = CQuestProcessor::kExcluded;

						switch (GetFavorQuestType(sec))
						{
						case 0:CQFramework_FavorQuests::CHandler::AddMerchantData(radiant_data->baseID, GetPluginFileName(sec), radiant_data->link); break;
						case 1:CQFramework_Beggars::CHandler::AddBeggarData(radiant_data->baseID, GetPluginFileName(sec), radiant_data->link); break;
						case 2:CQFramework_FavorQuests::DrunkHandler::AddDrunkData(quest_data->GetKey(), GetDrunkFileList(sec), radiant_data->baseID, GetDrunkFileListPluginName(sec), GetPluginFileName(sec)); break;
						default:
							ERROR("Invalid 'GenericFavorQuestType' provided for quest {}", radiant_data->link);
						}
					}
					quest_data->initPatchRadiantData(radiant_data);
				};
				quest_data->finalize();

				int32_t visibilityStatus = GetQuestVisibilityFlag(sec);
				if (visibilityStatus > 0) {
					MCMAPI::AddVisibilityOption(quest_data->GetEditorID(), data->mcmpage, visibilityStatus);
				};

				//Finalise and Create Data Struct.
				CQuestMaster::QuestAPI::AddQuestData(quest_data, quest_data->GetName(), mPagelink, quest_data->GetKey());
				Installed++;
				continue;
			}

			CMiscPatchData PatchData{};
			PatchData.type = GetItemType(sec);

			if (!IsTrackedEntity(PatchData.type)) {
				if (data->log_install) {
					INFO("Skipping misc section {}: type {} not tracked", sec, static_cast<int>(PatchData.type));
				}
				continue;
			}

			PatchData.ID = GetRandomID();
			PatchData.displayOnPage = GetMultiPageValue(sec);
			PatchData.FailedInstallCondition = !ShouldInstallPage(std::to_string(PatchData.displayOnPage));
			PatchData.pageheaderL = data->GetLocStringByKey(GetSectionHeaderL(sec).c_str(), "pageheaderL");
			PatchData.pageheaderR = data->GetLocStringByKey(GetSectionHeaderR(sec).c_str(), "pageheaderR");

			auto page_def = data->GetLocStringByKey(GetSliderValue(PatchData.displayOnPage).c_str(), "page_def");
			data->section_defs.emplace(PatchData.displayOnPage, page_def);

			auto* Handler = RE::TESDataHandler::GetSingleton();
			if (PatchData.type == CMiscPatchType::kQuests)
			{
				PatchData.quest_data_id = pageIdentifiers[PatchData.displayOnPage - 1];
				Installed++;
			}
			else
			{
				auto Idx = 0;
				for (auto& [formID, raw, pluginFileName, fullVariableString] : GetFormIDArray(sec, data->log_install))
				{
					if (!Serialization::CompletionistData::IsModInstalled(pluginFileName)) {
						if (data->log_install) {
							INFO("Unable to load useable data from entry: {} in section {}: [{} not found.]", Idx, sec, pluginFileName);
							INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
						}
						Idx++;
						continue;
					};

					if (!ShouldInstallForm(sec, fullVariableString, raw)) {
						if (data->log_install) {
							INFO("Unable to load useable data from entry: {} in section {}: [{} did not meet install condition.]", Idx, sec, raw);
							INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
						}
						Idx++;
						continue;
					};

					auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, pluginFileName);
					if (!form)
					{
						if (data->log_install) {
							INFO("Unable to load useable data from entry: {} in section {}: [{}]", Idx, sec, "Incorrect FormID or Form not found.");
							INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
						}
						Idx++;
						continue;
					};

					int32_t visibilityStatus = GetCustomVisibilityFlag(sec, fullVariableString, raw);
					if (visibilityStatus > -1) {
						MCMAPI::AddVisibilityOption(form->GetFormID(), data->mcmpage, visibilityStatus);
					};

					switch (PatchData.type) {
					case CMiscPatchType::kItems:
					{
						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						auto variations = GetVariationsArray(sec, fullVariableString, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};

						if (data->log_install)
						{
							switch (form->GetFormType())
							{
							case RE::FormType::Weapon: INFO("Added W Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames")); break;
							case RE::FormType::Armor: INFO("Added A Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames")); break;
							default: INFO("Added I Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames")); break;
							};
						};

						break;
					}

					case CMiscPatchType::kBooks:
					{
						if (!form->IsBook()) {
							if (data->log_install) {
								INFO("Unable to load useable data for entry: {} in section {}: [{}]", Idx, sec, "Incorrect TypeID");
								INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
							}
							Idx++;
							continue;
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						auto variations = GetVariationsArray(sec, fullVariableString, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};

						if (data->log_install)
						{
							INFO("Added B Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						break;
					}

					case CMiscPatchType::kLocations:
					{
						if (!static_cast<RE::TESObjectREFR*>(form)->GetBaseObject() || static_cast<RE::TESObjectREFR*>(form)->GetBaseObject()->GetFormID() != 0x000010) {
							if (data->log_install) {
								INFO("Unable to load useable data for entry: {} in section {}: [{}]", Idx, sec, "Incorrect TypeID");
								INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
							}
							Idx++;
							continue;
						}

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						if (data->log_install)
						{
							INFO("Added L Support for: {} - {}", raw, customDisplayName.empty() ? Serialization::CompletionistData::GetMapMarkerName(form) : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kEnchantments:
					{
						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						if (data->log_install)
						{
							INFO("Added E Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kShouts:
					{
						if (form->formType != RE::FormType::Shout)
						{
							INFO("Unable to load useable data for entry: {} in section {}: [{}]", Idx, sec, "Incorrect TypeID");
							INFO("  - FormID={:08X} - Raw={} - FVS={}", formID, raw, fullVariableString);
							Idx++;
							continue;
						}

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						if (data->log_install)
						{
							INFO("Added S Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kFish:
					{
						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.FishPickupMap.emplace(Handler->LookupFormID(formID, pluginFileName), CanPickUpFish(sec, raw));
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						}

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						}

						auto variations = GetVariationsArray(sec, fullVariableString, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						}

						if (data->log_install)
						{
							INFO("Added F Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						break;
					}

					case CMiscPatchType::kPlayerHomes:
					{
						PlayerHomesDataStruct houseData{};

						houseData.type = static_cast<PlayerHomesDataStructType>(GetPlayerHomeType(sec, raw));
						houseData.formID = RE::TESDataHandler::GetSingleton()->LookupFormID(formID, pluginFileName);

						if (houseData.type == PlayerHomesDataStructType::kQuest) {
							houseData.stage = GetPlayerHomeStage(sec, raw);
							houseData.optional_stage = GetPlayerHomeOptionalStage(sec, raw);
						}
						PatchData.playerHomeData.push_back(houseData);

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						if (data->log_install)
						{
							INFO("Added H Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kPets:
					{
						PetsDataStruct petData{};
						petData.type = static_cast<PetsDataStructType>(GetPetCompletionType(sec, raw));
						petData.formID = RE::TESDataHandler::GetSingleton()->LookupFormID(formID, pluginFileName);

						if (petData.type == PetsDataStructType::kQuest) {
							petData.stage = GetPetsCompletionStage(sec, raw);
						}
						PatchData.petsData.push_back(petData);

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						if (data->log_install)
						{
							INFO("Added P Support for: {} - {}", raw, customDisplayName.empty() ? form->GetName() : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kInteractableObject:
					{
						PatchData.originalOrder.push_back(formID);
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, fullVariableString, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, fullVariableString, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						auto variations = GetVariationsArray(sec, fullVariableString, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};

						if (data->log_install)
						{
							auto* refr = form->As<RE::TESObjectREFR>();
							auto* base = refr ? refr->GetBaseObject() : nullptr;
							INFO("Added A Support for: {} - {}", raw, customDisplayName.empty() ? base ? base->GetName() : "Unknown" : data->GetLocStringByKey(customDisplayName.c_str(), "CustomDisplayNames"));
						}

						break;
					}
					default:
						break;
					}
					Idx++;
				}
			}
			data->type_sections.push_back(PatchData);
		}

		if (Installed > 0)
		{
			CFramework_Master::InstalledPatchesForMCMDisplay++;
			CustomPatches.push_back(std::make_pair(data->mcmpage, data));
		}
	}
};