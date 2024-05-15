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
		std::uniform_int_distribution<int> uni(500, 999); // Guaranteed unbiased
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

			bool log_install = GetLoggingAllowed();

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

			if (DKUtil::string::iequals(GetPatchType(), "Quest"))
			{
				AddQuestSupport(file);
				continue;
			}

			if (DKUtil::string::iequals(GetPatchType(), "Misc"))
			{
				AddMiscSupport(file);
				continue;
			}
		};

		LinkPatchAndQuestData();

		INFO("Sorting {} Groups", CustomItemsPatches.size());
		if (CustomItemsPatches.size() > 0) 
		{
			std::sort(CustomItemsPatches.begin(), CustomItemsPatches.end());

			for (auto& [groupName, group] : CustomItemsPatches) 
			{
				std::sort(group->patches.begin(), group->patches.end());
				std::sort(group->official_patches.begin(), group->official_patches.end());
			};
			MCMAPI::AddMiscPatchedPageDefinitions(CustomItemsPatches);
		};
	};

	//---------------------------------------------------
	//-- Framework Functions ( Add Quest Support ) ------
	//---------------------------------------------------

	void CHandler::AddQuestSupport(std::string file)
	{
		auto fName = file.substr(file.find_last_of("/") + 1);

		using namespace CHCMHandler;

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);
		sections.sort(typename CSimpleIniA::Entry::LoadOrder());

		bool log_install = GetLoggingAllowed();

		//If this is not an official patch then create a localised map for translations.
		bool official = GetCustomPatchType() == 1;
		bool creation = GetCustomPatchType() == 2;

		std::unordered_map<std::string, std::string> translations;
		
		if (!official) {
			std::string lName = fName.substr(0, fName.find_last_of('.')) + ".txt";
			BuildLocalisedMap(lName.c_str(), translations);
		};

		for (auto& section : sections)
		{
			auto sec = section.pItem; 

			//Skip Patch Data.
			if (DKUtil::string::iequals(section.pItem, "Completionist Patch Data"))
			{
				continue;
			};

			//Skip If Quest Disabled.
			if (!GetEnabled(section.pItem))
			{
				if (log_install)
				{
					INFO("Unable to load useable data from quest: {} : [{}]", section.pItem, "Quest Disabled");
				}
				continue;
			};

			//Skip if quest not found.
			RE::TESQuest* quest = RE::TESForm::LookupByEditorID<RE::TESQuest>(GetEditorID(sec));
			if (!quest) {
				if (log_install)
				{
					INFO("Unable to load useable data from quest: {} : [{}]", section.pItem, "Quest Not Found");
				}
				continue;
			};

			if (!ShouldInstall(sec)) {
				if (log_install)
				{
					INFO("Unable to load useable data from quest: {} : [{}]", section.pItem, "Install Condition Not Met.");
				}
				continue;
			}

			//Get localised page name.
			const std::string& mcmPageName = GetMCMPageName(sec);
			std::string page = official ? GET_LOC_STRING_BY_KEY(mcmPageName.c_str()) : GetLocStringByKey(mcmPageName.c_str(), translations);

			//mPagelink is used to tie this quest to an existing page. (Does not create an mcm page and is grabbed by tied page) 
			int32_t mPagelink = GetQuestLink(sec);
			if (mPagelink == -1) {
				continue;
			}

			//Set Quest Data
			CQuestData* quest_data = GetNewQuestData();
			quest_data->unique_identifier = GetUUID(sec);
			quest_data->init((mPagelink < 500), official, page);
			quest_data->set_editorID(GetEditorID(sec));
			quest_data->set_quest_type(GetQuestType(sec));
			quest_data->set_completion_type(GetCompletionType(sec));
			if (!official)
			{
				quest_data->set_name(GetLocStringByKey(GetQuestName(sec).c_str(), translations));
				quest_data->set_highlight(GetLocStringByKey(GetHighlightText(sec).c_str(), translations));
			}

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
				quest_data->initPatchRadiantData(radiant_data);
			};
			quest_data->finalize();

			//Finalise and Create Data Struct.
			CQuestMaster::QuestAPI::AddQuestData(quest_data, quest_data->GetName(), mPagelink, quest_data->GetKey());
		};
	};

	//---------------------------------------------------
	//-- Framework Functions ( Add Misc Support ) -------
	//---------------------------------------------------

	[[nodiscard]] static auto GetBookSkill(RE::ActorValue a_val) noexcept
	{
		static DKUtil::enumeration<RE::ActorValue, std::uint32_t> actorValueNameTbl{};

		auto rawName = actorValueNameTbl.to_string(a_val);

		for (auto i = 0; i < rawName.length(); ++i) {
			if (std::isupper(rawName[i])) {
				rawName.insert(i++, " ");
			}
		}

		return rawName.erase(0, 2);
	}

	void CHandler::AddMiscSupport(std::string file)
	{
		auto fName = file.substr(file.find_last_of("/") + 1);

		using namespace CHCMHandler;

		if (!Serialization::CompletionistData::IsModInstalled(GetPluginFileName())) {
			return;
		};

		if (Analyse())
		{
			std::vector<std::pair<std::string, std::string>> W{};
			std::vector<std::pair<std::string, std::string>> A{};
			std::vector<std::pair<std::string, std::string>> E{};
			std::vector<std::pair<std::string, std::string>> I{};
			std::vector<std::pair<std::string, std::string>> S{};
			std::vector<std::pair<std::string, std::string>> L{};
			std::vector<std::pair<std::string, std::string>> NB{};
			std::vector<std::pair<std::string, std::string>> SB{};
			std::vector<std::pair<std::string, std::string>> ST{};
			std::vector<std::vector<std::pair<std::string, std::string>>> pair_data = { W, A, E, I, S, L, NB, SB, ST };

			int Idx = 0;
			int Edx = 0;
			for (auto& [formID, raw, pluginFileName] : GetFormIDArray("Analyse", true))
			{
				auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, pluginFileName);
				if (!form)
				{
					INFO("Unable to load useable data from entry: {} - {} in section Analyse: [{}]", Idx, raw,  "Incorrect FormID or Form not found.");
					Idx++;
					continue;
				};

				if (static_cast<RE::TESObjectREFR*>(form)->GetBaseObject() && static_cast<RE::TESObjectREFR*>(form)->GetBaseObject()->GetFormID() == 0x000010) {
					L.push_back(std::make_pair(raw + "*" + pluginFileName, Serialization::CompletionistData::GetMapMarkerName(form))); Edx++;
					continue;
				};

				switch (form->GetFormType())
				{
				default: I.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;

				case RE::FormType::Ammo:			W.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Weapon:			W.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Armor:			A.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Enchantment:		E.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Misc:			I.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Shout:			S.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++; break;
				case RE::FormType::Book: 
				{
					auto* book = static_cast<RE::TESObjectBOOK*>(form);
					if (book && book->GetSpell())
					{
						ST.push_back(std::make_pair(raw + "*" + pluginFileName, "[" + GetBookSkill(book->GetSpell()->GetAssociatedSkill()) + "]" + " - " + form->GetName())); Edx++;
					}
					else if (book && book->TeachesSkill())
					{
						SB.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++;
					}
					else
					{
						NB.push_back(std::make_pair(raw + "*" + pluginFileName, form->GetName())); Edx++;
					}
					break;
				}
				};
			};

			INFO("Analysis Complete On {} entries from File - {}", Edx, fName);

			auto wInstall = W.size() > 0;
			auto aInstall = A.size() > 0;
			auto eInstall = E.size() > 0;
			auto iInstall = I.size() > 0;
			auto sInstall = S.size() > 0;
			auto lInstall = L.size() > 0;

			auto nbInstall = NB.size() > 0;
			auto sbInstall = SB.size() > 0;
			auto stInstall = ST.size() > 0;
			auto sectionsReq = 0;

			sectionsReq += (int)wInstall;
			sectionsReq += (int)aInstall;
			sectionsReq += (int)eInstall;
			sectionsReq += (int)iInstall;
			sectionsReq += (int)sInstall;
			sectionsReq += (int)lInstall;
			sectionsReq += (int)nbInstall;
			sectionsReq += (int)sbInstall;
			sectionsReq += (int)stInstall;
			auto fsectionsReq = sectionsReq;

			INFO("{} Sections are recommended at a minimum based on item types analysed, you can always add more \n if you'd like to split sections down into specific categories. ", sectionsReq);

			auto maxE = 126;
			if (sectionsReq > 1) {
				sectionsReq -= 1;

				maxE = 126 - ( (sectionsReq) * 4);
			}

			INFO("The maximum tracked entries allowed for a single page setup with {} sections is {}", fsectionsReq, maxE);
			INFO("");

			INFO("{} Page(s) with MultiPage {} Recommended", Edx > maxE ? std::ceil(Edx / maxE) : 1, std::ceil(Edx / maxE) > 1 ? "Enabled" : "Disabled");
			INFO("");
			if (Edx <= maxE)
			{
				INFO("Auto Analysis shows that you are aiming to track {} item(s) which is less than or equal to the above maximum allowed based on {} section(s). \n If you do decide to add more sections you must decrease the maximum allowed by 4 for each added section. \n This will tell you how many entries you can fit on one page. \n If your total tracked entries count exceeds the maximum allowed then you can enable MultiPage for further sections.", Edx, fsectionsReq);
			}
			else
			{
				INFO("Auto Analysis shows that you are aiming to track {} item(s) which is more than the above maximum allowed based on {} section(s). \n In this case 'MultiPage' must be enabled to handle the overflow from page1, see the miscTemplate.ini file for configuartion help.", Edx, fsectionsReq);
			}

			INFO("");
			INFO("Section Summary:");

			if (wInstall)
			{
				INFO("");
				INFO("Weapons section for a total of {} Items.", W.size());
				for (auto& [raw, name] : W) {
					INFO("{} - {}", raw, name);
				};
			};

			if (aInstall)
			{
				INFO("");
				INFO("Armor section for a total of {} Items.", A.size());
				for (auto& [raw, name] : A) {
					INFO("{} - {}", raw, name);
				};
			};

			if (eInstall)
			{
				INFO("");
				INFO("Enchantments section for a total of {} Items.", E.size());
				for (auto& [raw, name] : E) {
					INFO("{} - {}", raw, name);
				};
			};

			if (iInstall)
			{
				INFO("");
				INFO("Misc Items section for a total of {} Items.", I.size());
				for (auto& [raw, name] : I) {
					INFO("{} - {}", raw, name);
				};
			};

			if (sInstall)
			{
				INFO("");
				INFO("Shouts section for a total of {} Items.", S.size());
				for (auto& [raw, name] : S) {
					INFO("{} - {}", raw, name);
				};
			};

			if (lInstall)
			{
				INFO("");
				INFO("Locations section for a total of {} Items.", L.size());
				for (auto& [raw, name] : L) {
					INFO("{} - {}", raw, name);
				};
			};

			if (nbInstall)
			{
				INFO("");
				INFO("Books section for a total of {} Items.", NB.size());
				for (auto& [raw, name] : NB) {
					INFO("{} - {}", raw, name);
				};
			};

			if (sbInstall)
			{
				INFO("");
				INFO("Skill Books section for a total of {} Items.",SB.size());
				for (auto& [raw, name] : SB) {
					INFO("{} - {}", raw, name);
				};
			};

			if (stInstall)
			{
				INFO("");
				INFO("Spell Tomes section for a total of {} Items.", ST.size());
				for (auto& [raw, name] : ST) {
					INFO("{} - {}", raw, name);
				};
			};

			return;
		};

		auto* data = GetNewPatchData();
		data->official = GetCustomPatchType() == 1;
		data->creation = GetCustomPatchType() == 2;
		data->log_install = GetLoggingAllowed();

		if (!data->official) {
			std::string lName = fName.substr(0, fName.find_last_of('.')) + ".txt";

			auto overrideName = GetOverrideLocalisationFileName();
			if (!DKUtil::string::iequals(overrideName, "None")) {
				lName = overrideName;
			}

			data->BuildLocalisedMap(lName.c_str());
		}

		if (data->log_install) 
		{
			INFO("Adding Patched Misc Support From File: {}", fName);
		}

		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);
		sections.sort(typename CSimpleIniA::Entry::LoadOrder());
		
		data->mcmpage = data->GetLocStringByKey(GetMCMPageName().c_str());
		data->multipage = IsMultiPage();
		data->prependPageNumber = data->multipage ? GetMultiPagePrependPageNumber() : false;
		data->multiPageCount = data->multipage ? GetMultiPageCount() : 0;
		if (data->multipage && data->multiPageCount > 1) {
			//data->InitPageDefs();
		}

		int Installed = 0;
		if (data->multipage && data->multiPageCount < 2)
		{
			ERROR("\nIncorrect MultiPage definitions detected in {}\n\n Multipage is enabled however 'MultiPage_PageCount' is less than 2.", fName)
		};

		for (auto& section : sections)
		{
			auto sec = section.pItem;

			//Skip Patch Data.
			if (DKUtil::string::iequals(sec, "Completionist Patch Data") || DKUtil::string::iequals(sec, "Analyse"))
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

			//Skip If mod conditions not met.
			auto conditionalModName = GetRequiredMod(sec);
			if (!DKUtil::string::iequals(conditionalModName, "None"))
			{
				if (GetRequiredModValue(sec) != Serialization::CompletionistData::IsModInstalled(conditionalModName)) 
				{
					if (data->log_install) {
						INFO("Unable to load useable data from section: {} : [{}]", sec, "Failure On Conditional Mod Requirement.");
					}
					continue;
				};
			};

			CMiscPatchData PatchData{};
			PatchData.type = GetItemType(sec);
			PatchData.ID = GetRandomID();
			PatchData.displayOnPage = data->multipage ? GetMultiPageValue(sec) : 0;
			PatchData.pageheaderL = data->GetLocStringByKey(GetSectionHeaderL(sec).c_str());
			PatchData.pageheaderR = data->GetLocStringByKey(GetSectionHeaderR(sec).c_str());

			auto page_def = data->multipage ? data->GetLocStringByKey(GetSliderValue(PatchData.displayOnPage).c_str()) : "";
			data->section_defs.emplace(PatchData.displayOnPage, page_def);

			auto* Handler = RE::TESDataHandler::GetSingleton();

			if (PatchData.type == CMiscPatchType::kQuests)
			{
				PatchData.quest_data_id = GetQuestID(sec);
				Installed++;
			}
			else
			{
				auto Idx = 0;
				for (auto& [formID, raw, pluginFileName] : GetFormIDArray(sec, data->log_install))
				{
					if (!Serialization::CompletionistData::IsModInstalled(pluginFileName)) {
						if (data->log_install) {
							INFO("Unable to load useable data from entry: {} in section {}: [{} not found.]", Idx, sec, pluginFileName);
						}						
						Idx++;
						continue;
					};

					auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, pluginFileName);
					if (!form)
					{
						if (data->log_install) {
							INFO("Unable to load useable data from entry: {} in section {}: [{}]", Idx, sec, "Incorrect FormID or Form not found.");
						}
						Idx++;
						continue;
					};					

					switch (PatchData.type) {
					case CMiscPatchType::kItems:
					{
						if (data->log_install)
						{
							switch (form->GetFormType())
							{
							case RE::FormType::Weapon: INFO("Adding W Support for: {} - {}", raw, form->GetName()); break;
							case RE::FormType::Armor: INFO("Adding A Support for: {} - {}", raw, form->GetName()); break;
							default: INFO("Adding I Support for: {} - {}", raw, form->GetName()); break;
							};
						};

						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

						auto variations = GetVariationsArray(sec, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};

						break;
					}

					case CMiscPatchType::kBooks:
					{
						if (!form->IsBook()) {
							if (data->log_install) {
								INFO("Unable to load useable data for entry: {} in section {}: [{}]", Idx, sec, "Incorrect TypeID");
							}
							Idx++;
							continue;
						}

						if (data->log_install)
						{
							INFO("Adding B Support for: {} - {}", raw, form->GetName());
						}
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

						auto variations = GetVariationsArray(sec, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};
						break;
					}

					case CMiscPatchType::kLocations:
					{
						if (!static_cast<RE::TESObjectREFR*>(form)->GetBaseObject() || static_cast<RE::TESObjectREFR*>(form)->GetBaseObject()->GetFormID() != 0x000010) {
							if (data->log_install) {
								INFO("Unable to load useable data for entry: {} in section {}: [{}]", Idx, sec, "Incorrect TypeID");
							}
							Idx++;
							continue;
						}

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

						if (data->log_install)
						{
							INFO("Adding L Support for: {} - {}", raw, Serialization::CompletionistData::GetMapMarkerName(form));
						}
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kEnchantments:
					{
						if (data->log_install)
						{
							INFO("Adding E Support for: {} - {}", raw, form->GetName());
						}

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

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
							Idx++;
							continue;
						}

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

						if (data->log_install)
						{
							INFO("Adding S Support for: {} - {}", raw, form->GetName());
						}
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kFish:
					{
						if (data->log_install)
						{
							INFO("Adding F Support for: {} - {}", raw, form->GetName());
						};

						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.FishPickupMap.emplace(Handler->LookupFormID(formID, pluginFileName), CanPickUpFish(sec, raw));
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						auto variations = GetVariationsArray(sec, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};

						break;
					}

					case CMiscPatchType::kPlayerHomes:
					{
						if (data->log_install)
						{
							INFO("Adding H Support for: {} - {}", raw, form->GetName());
						};
						PlayerHomesDataStruct houseData{};

						houseData.type = static_cast<PlayerHomesDataStructType>(GetPlayerHomeType(sec, raw));
						houseData.formID = RE::TESDataHandler::GetSingleton()->LookupFormID(formID, pluginFileName);

						if (houseData.type == PlayerHomesDataStructType::kQuest) {
							houseData.stage = GetPlayerHomeStage(sec, raw);
							houseData.optional_stage = GetPlayerHomeOptionalStage(sec, raw);
						}
						PatchData.playerHomeData.push_back(houseData);

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kPets:
					{
						if (data->log_install)
						{
							INFO("Adding P Support for: {} - {}", raw, form->GetName());
						};

						PetsDataStruct petData{};
						petData.type = static_cast<PetsDataStructType>(GetPetCompletionType(sec, raw));
						petData.formID = RE::TESDataHandler::GetSingleton()->LookupFormID(formID, pluginFileName);

						if (petData.type == PetsDataStructType::kQuest) {
							petData.stage = GetPetsCompletionStage(sec, raw);
						}
						PatchData.petsData.push_back(petData);

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayName = GetCustomDisplayName(sec, raw);
						if (!customDisplayName.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayName);
						};

						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;
						break;
					}

					case CMiscPatchType::kInteractableObject:
					{
						if (data->log_install)
						{
							INFO("Adding A Support for: {} - {}", raw, form->GetName());
						}
						PatchData.data.AddForm(formID, pluginFileName);
						PatchData.enabled = true;
						Installed++;

						auto customHighlightText = GetCustomHighlightText(sec, raw);
						if (!customHighlightText.empty()) {
							PatchData.CustomHighlightText.emplace(form->GetFormID(), customHighlightText);
						};

						auto customDisplayname = GetCustomDisplayName(sec, raw);
						if (!customDisplayname.empty()) {
							PatchData.CustomDisplayNames.emplace(form->GetFormID(), customDisplayname);
						};

						auto variations = GetVariationsArray(sec, raw, data->log_install);
						if (!variations.empty()) {
							CompileVariations(sec, &PatchData.data, variations, pluginFileName, formID, data->log_install);
						};
						break;
					}
					default:
						break;
					}
					Idx++;
				};
			};
			data->type_sections.push_back(PatchData);
		};

		if (Installed > 0)
		{
			CFramework_Master::InstalledPatchesForMCMDisplay++;

			auto groupName = fmt::format("{:s}", IsGroupingEnabled() ? GetGroupName() : "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

			if (GroupExists(groupName))
			{
				data->official ? GetGroup(groupName)->official_patches.push_back(std::make_pair(data->mcmpage, data)) : data->creation ? GetGroup(groupName)->creation_club_patches.push_back(std::make_pair(data->mcmpage, data)) : GetGroup(groupName)->patches.push_back(std::make_pair(data->mcmpage, data));
			}
			else
			{
				auto group = GetNewPatchGroupData();;
				group->groupName = data->GetLocStringByKey(groupName.c_str());
				data->official ? group->official_patches.push_back(std::make_pair(data->mcmpage, data)) : data->creation ? group->creation_club_patches.push_back(std::make_pair(data->mcmpage, data)) : group->patches.push_back(std::make_pair(data->mcmpage, data));
				group->defaultGroup = DKUtil::string::iequals(groupName, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") ? true : false;
				CustomItemsPatches.push_back(std::make_pair(group->groupName, group));
			};
		};
	};
};