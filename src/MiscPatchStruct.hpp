#pragma once
#include "Serialization.hpp"
#include "Internal Utility/Variables.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#ifndef LOG_IF_ENABLED
#define LOG_IF_ENABLED(...) if (CVariables::V_Global_Patch_Logging || log_install) { INFO(__VA_ARGS__); }
#endif

#ifndef ProcessPatchFoundFormArgs
#define ProcessPatchFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, CMiscPatchData* data, Serialization::CompletionistLog::logType eventHandle
#endif

using namespace CVariables;

struct CMiscPatchData
{
	int32_t ID{};
	CMiscPatchType type{};
	bool enabled{};

	std::vector<RE::FormID> originalOrder{};
	Serialization::CompletionistData data;
	std::vector<PetsDataStruct> petsData{};
	std::vector<PlayerHomesDataStruct> playerHomeData{};
	std::unordered_map<RE::FormID, bool> FishPickupMap{};

	std::vector<std::string> names{};
	std::vector<std::string> texts{};
	std::vector<RE::TESForm*> forms{};
	std::vector<bool> bools{};
	std::int32_t found{};
	std::int32_t total{};

	std::string pageheaderL{};
	std::string pageheaderR{};
	int32_t displayOnPage{};

	//Shout Specific
	std::vector<RE::SpellItem*> words_1{};
	std::vector<RE::SpellItem*> words_2{};
	std::vector<RE::SpellItem*> words_3{};
	Serialization::CompletionistData data_w1;
	Serialization::CompletionistData data_w2;
	Serialization::CompletionistData data_w3;

	//Overrides the default highlight text on install.
	std::unordered_map<RE::FormID, std::string> CustomDisplayNames{};
	std::unordered_map<RE::FormID, std::string> CustomHighlightText{};

	//Quest Specific
	std::vector<CQuestData*> quest_data_array{};
	int32_t quest_data_id = -1;

	std::string localisationFileName{};

	//Set by the PatchListener if this page should be cleaned and removed.
	bool FailedInstallCondition{};

	bool IsEmpty() const { return FailedInstallCondition || (type == CMiscPatchType::kQuests && quest_data_array.size() <= 0) || (type != CMiscPatchType::kQuests && data.data.size() <= 0); }
};

struct CDropDownMenu
{
	std::string name;
	std::string highlight;
	std::vector<std::string> options;
};

enum CMiscPatchSortMode
{
	kDefault = 0,
	kIgnoreArticles = 1,
	kDeveloperOrder = 2,
};

struct CMiscPatch
{
	using master = CFramework_Master::FrameworkAPI;
	
	std::string mcmpage{};
	std::vector<CMiscPatchData> type_sections{};
	std::map<int32_t, std::string> section_defs{};

	std::string iniFileName{};
	std::string headerName{};

	bool log_install{};
	bool prependPageNumber{};
	int32_t PageCount{};

	bool HasDropDownMenu{};
	CDropDownMenu DropDownMenu{};

	std::unordered_map<std::string, std::string> translations{};

	//Sets the mods priority, this determines the display order in the window.
	std::int32_t priority{};

	//True if this patch handles vanilla / creation club tracking.
	bool isVanillaTracking{};

	// Sorting Mode
	CMiscPatchSortMode sortMode = kDefault;

	//---------------------------------------------------
	//-- function ---------------------------------------
	//---------------------------------------------------

	void CleanEmptyPages()
	{
		LOG_IF_ENABLED("[{}] Starting CleanEmptyPages().", mcmpage);
		LOG_IF_ENABLED("[{}] Initial PageCount: {}", mcmpage, PageCount);

		// Step 1: Remove all empty sections
		std::vector<CMiscPatchData> nonEmptySections;
		for (const auto& section : type_sections)
		{
			if (!section.IsEmpty())
			{
				nonEmptySections.push_back(section);
				LOG_IF_ENABLED("[{}] Keeping non-empty section on page {}", mcmpage, section.displayOnPage);
			}
			else
			{
				LOG_IF_ENABLED("[{}] Removing empty section on page {}", mcmpage, section.displayOnPage);
			}
		}

		// Step 2: Identify pages that actually have sections
		std::set<int32_t> validPages;
		for (const auto& section : nonEmptySections)
		{
			validPages.insert(section.displayOnPage);
		}

		LOG_IF_ENABLED("[{}] Valid pages after removing empty sections: {}", mcmpage, validPages.size());

		// Step 3: Create page remapping (old page -> new consecutive page)
		std::map<int32_t, int32_t> pageRemap;
		int newPageNum = 1;
		for (int oldPage = 1; oldPage <= PageCount; ++oldPage)
		{
			if (validPages.count(oldPage))
			{
				pageRemap[oldPage] = newPageNum++;
				LOG_IF_ENABLED("[{}] Remapping old page {} → new page {}", mcmpage, oldPage, pageRemap[oldPage]);
			}
			else
			{
				LOG_IF_ENABLED("[{}] Page {} has no sections and will be removed.", mcmpage, oldPage);
			}
		}

		// Step 4: Update sections' page numbers
		std::vector<CMiscPatchData> updatedSections;
		for (auto& section : nonEmptySections)
		{
			auto it = pageRemap.find(section.displayOnPage);
			if (it != pageRemap.end())
			{
				section.displayOnPage = it->second;
				updatedSections.push_back(std::move(section));
				LOG_IF_ENABLED("[{}] Updated section from old page {} to new page {}", mcmpage, it->first, it->second);
			}
			else
			{
				LOG_IF_ENABLED("[{}] ERROR: Section from page {} not in remap", mcmpage, section.displayOnPage);
			}
		}

		type_sections = std::move(updatedSections);
		LOG_IF_ENABLED("[{}] Sections after cleanup: {}", mcmpage, type_sections.size());

		// Step 5: Update PageCount
		PageCount = newPageNum - 1;
		LOG_IF_ENABLED("[{}] Updated PageCount: {}", mcmpage, PageCount);

		// Step 6: Update section_defs
		std::map<int32_t, std::string> updatedSectionDefs;
		for (const auto& [oldPage, value] : section_defs)
		{
			if (pageRemap.count(oldPage))
			{
				updatedSectionDefs[pageRemap[oldPage]] = value;
				LOG_IF_ENABLED("[{}] Updated section_defs from old page {} to new page {}", mcmpage, oldPage, pageRemap[oldPage]);
			}
			else
			{
				LOG_IF_ENABLED("[{}] Removing section_defs entry for old page {}", mcmpage, oldPage);
			}
		}
		section_defs = std::move(updatedSectionDefs);

		LOG_IF_ENABLED("[{}] CleanEmptyPages() completed successfully.", mcmpage);
	}

	//---------------------------------------------------
	//-- function ---------------------------------------
	//---------------------------------------------------

	void InstallFramework() 
	{
		InjectAndCompileData();
		InstallSearchTerms();
		AddCustomHighlighting();
		AddCustomDisplayName();
		AddDropDownOptions();
		SortLists();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void SortLists()
	{
		auto ignore_articles = [](const std::string& str) -> std::string {
			static const std::vector<std::string> articles = { "the ", "a ", "an " };
			std::string lower = str;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

			for (const auto& article : articles) {
				if (lower.starts_with(article)) {
					return str.substr(article.size());  // Skip the article
				}
			}
			return str;
			};

		for (auto& section : type_sections)
		{
			std::vector<std::tuple<std::string, std::string, RE::TESForm*, bool>> combined;

			for (size_t i = 0; i < section.names.size(); ++i) {
				combined.emplace_back(section.names[i], section.texts[i], section.forms[i], section.bools[i]);
			}

			std::sort(combined.begin(), combined.end(),
				[this, &ignore_articles, &section](const auto& a, const auto& b) {
					const auto& aName = std::get<0>(a);
					const auto& bName = std::get<0>(b);

					const auto* aForm = std::get<2>(a);
					const auto* bForm = std::get<2>(b);

					switch (sortMode) {
					case kDefault:
						return aName < bName;

					case kIgnoreArticles:
						return ignore_articles(aName) < ignore_articles(bName);

					case kDeveloperOrder:
						if (aForm && bForm) {
							auto aID = aForm->GetFormID();
							auto bID = bForm->GetFormID();

							auto aIt = std::find(section.originalOrder.begin(), section.originalOrder.end(), aID);
							auto bIt = std::find(section.originalOrder.begin(), section.originalOrder.end(), bID);

							size_t aIdx = (aIt != section.originalOrder.end()) ? std::distance(section.originalOrder.begin(), aIt) : SIZE_MAX;
							size_t bIdx = (bIt != section.originalOrder.end()) ? std::distance(section.originalOrder.begin(), bIt) : SIZE_MAX;

							return aIdx < bIdx;
						}
						return false; // fallback if forms are null

					default:
						return aName < bName;
					}
				});

			// Unpack sorted data back into vectors
			for (size_t i = 0; i < combined.size(); ++i) {
				section.names[i] = std::get<0>(combined[i]);
				section.texts[i] = std::get<1>(combined[i]);
				section.forms[i] = std::get<2>(combined[i]);
				section.bools[i] = std::get<3>(combined[i]);
			}

			// Clear original order once it's been used
			if (sortMode == kDeveloperOrder) {
				section.originalOrder.clear();
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void InjectAndCompileData() 
	{
		for (auto& section : type_sections)
		{
			switch (section.type)
			{
			case CMiscPatchType::kItems: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts);
				section.data.MergeAsCollectable(); 
				break;
			}

			case CMiscPatchType::kBooks: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts, false, 1);
				section.data.MergeAsCollectable();
				break;
			}

			case CMiscPatchType::kLocations: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts, false, 2);
				break;
			}

			case CMiscPatchType::kEnchantments: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts);
				break;
			}

			case CMiscPatchType::kFish: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts);
				break;
			}

			case CMiscPatchType::kPlayerHomes: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts);
				break;
			}

			case CMiscPatchType::kPets: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts, true);
				break;
			}

			case CMiscPatchType::kShouts: {
				BuildShoutCompatibility(&section);
				break;
			}

			case CMiscPatchType::kInteractableObject: {
				section.data.Populate(section.names, section.forms, section.bools, section.texts, true);
				break;
			}

			default: break;
			}
			section.total = section.forms.size();
			section.found = std::ranges::count(section.bools, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add Dropdown Options ) --- 
	//---------------------------------------------------

	void AddDropDownOptions()
	{
		if (HasDropDownMenu) {
			DropDownMenu.name = GetLocStringByKey(DropDownMenu.name.c_str(), "DropDownMenuName");
			DropDownMenu.highlight = GetLocStringByKey(DropDownMenu.highlight.c_str(), "DropDownMenuHighlight");
			for (auto i = 0; i < DropDownMenu.options.size(); i++)
			{
				DropDownMenu.options[i] = GetLocStringByKey(DropDownMenu.options[i].c_str(), "DropDownMenuOption");
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add Custom Highlighting ) 
	//---------------------------------------------------

	void AddCustomHighlighting()
	{
		for (auto& section : type_sections)
		{
			for (auto Idx = 0; Idx < section.forms.size(); Idx++) {

				auto it = section.CustomHighlightText.find(section.forms[Idx]->GetFormID());
				if (it != section.CustomHighlightText.end()) {
					section.texts[Idx] = GetLocStringByKey(it->second.c_str(), "CustomHighlightText");
				}
			}
			section.CustomHighlightText.clear();
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add Custom Highlighting ) 
	//---------------------------------------------------

	void AddCustomDisplayName()
	{
		for (auto& section : type_sections)
		{
			for (auto Idx = 0; Idx < section.forms.size(); Idx++) {

				auto it = section.CustomDisplayNames.find(section.forms[Idx]->GetFormID());
				if (it != section.CustomDisplayNames.end()) {
					section.names[Idx] = GetLocStringByKey(it->second.c_str(), "CustomDisplayNames");
				}
			}
			section.CustomDisplayNames.clear();
		}
	}

	bool CanPickupFish(const CMiscPatchData* a_data, const RE::FormID a_formID)
	{
		if (a_data->FishPickupMap.size() == 0) {
			return false;
		};

		return a_data->FishPickupMap.at(a_formID) ? a_data->FishPickupMap.at(a_formID) : false;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Logging Level ) ------
	//---------------------------------------------------

	int32_t GetLogLevel(CMiscPatchType a_type, RE::TESForm* a_book = nullptr)
	{
		switch (a_type)
		{
		case CMiscPatchType::kItems:				return std::to_underlying(CFramework_Master::EntryCategory::kItem); break;
		case CMiscPatchType::kBooks:				return master::GetBookCategoryType(a_book); break;
		case CMiscPatchType::kLocations:			return std::to_underlying(CFramework_Master::EntryCategory::kMapM); break;
		case CMiscPatchType::kEnchantments:			return std::to_underlying(CFramework_Master::EntryCategory::kEnch); break;
		case CMiscPatchType::kShouts:				return std::to_underlying(CFramework_Master::EntryCategory::kShou); break;
		case CMiscPatchType::kFish:					return std::to_underlying(CFramework_Master::EntryCategory::kFish); break;
		case CMiscPatchType::kPlayerHomes:			return std::to_underlying(CFramework_Master::EntryCategory::kHome); break;
		case CMiscPatchType::kPets:					return std::to_underlying(CFramework_Master::EntryCategory::kPets); break;
		case CMiscPatchType::kInteractableObject:	return std::to_underlying(CFramework_Master::EntryCategory::kShard); break;
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void InstallSearchTerms() 
	{
		for (auto& section : type_sections)
		{
			for (auto i = 0; i < section.names.size(); i++) {
				CFramework_Master::CItemsDataVec.push_back(std::make_tuple(section.forms[i], section.names[i], mcmpage, GetLogLevel(section.type, section.forms[i])));
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Build Localised Map ) ----
	//---------------------------------------------------

	void BuildLocalisedMap()
	{
		if (translations.size()) {
			return;
		}
		const std::string cBasePath = R"(.\Data\SKSE\Plugins\CompletionistData\Translations\)";
		if (!std::filesystem::exists(cBasePath) || std::filesystem::is_empty(cBasePath))
		{
			INFO("Translations Folder Is Empty Or Does Not Exist.");
			return;
		}

		const std::string cFilePath = fmt::format(R"(.\Data\SKSE\Plugins\CompletionistData\Translations\{})"sv, iniFileName);
		if (!std::filesystem::exists(cFilePath) || std::filesystem::is_empty(cFilePath))
		{
			ERROR("Unable To Find localisation File {}", iniFileName)
		}

		std::ifstream localised_file(cFilePath);
		std::string tempstring;

		if (!localised_file.is_open()) {
			ERROR("Failed to open file: {}", iniFileName);
			return;
		}

		while (getline(localised_file, tempstring)) {
			if (!tempstring.empty() && !tempstring.starts_with("-")) {
				size_t tabPos = tempstring.find("	");
				size_t pipePos = tempstring.find("|");

				if (tabPos != std::string::npos && pipePos != std::string::npos) {
					translations.emplace(tempstring.substr(0, tabPos), tempstring.substr(pipePos + 1));
				}
			}
		}
		localised_file.close();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Localised String ) ---
	//---------------------------------------------------

	const char* GetLocStringByKey(const char* s_key, const char* s_param)
	{
		if (!translations.contains(s_key)) {
			LOG_IF_ENABLED("Unable to load translation for key {} in param {} from file: {}", s_key, s_param, iniFileName);
			return s_key;
		}
		return translations.at(s_key).c_str();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void UpdateFoundForms()
	{
		for (auto& section : type_sections)
		{
			for (size_t i = 0; i < section.forms.size(); i++)
			{
				switch (section.type)
				{
				case CMiscPatchType::kItems:
					section.bools[i] = master::IsItemKnown(section.forms[i], &section.data);
					break;

				case CMiscPatchType::kBooks:
					section.bools[i] = master::IsBookKnown(section.forms[i]);
					break;

				case CMiscPatchType::kLocations:
				case CMiscPatchType::kFish:
				case CMiscPatchType::kPlayerHomes:
				case CMiscPatchType::kPets:
				case CMiscPatchType::kInteractableObject:
					section.bools[i] = CFramework_Master::FoundItemData_NoShow.HasForm(section.forms[i]->GetFormID());
					break;

				case CMiscPatchType::kEnchantments:
					section.bools[i] = master::IsEnchantmentKnown(section.forms[i]);
					break;

				case CMiscPatchType::kShouts:
				{
					// Each word learnt also counts the ones before it
					auto& found = CFramework_Master::FoundItemData_NoShow;
					const auto learnt = LearntWords(section, i);
					if (learnt >= 1) {
						found.AddForm(section.words_1[i]);
					}
					if (learnt >= 2) {
						found.AddForm(section.words_2[i]);
					}
					if (learnt >= 3) {
						found.AddForm(section.words_3[i]);
					}
					section.bools[i] = learnt == 3;
					break;
				}
				} // end switch
			}

			section.total = section.forms.size();
			section.found = std::ranges::count(section.bools, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Clear Shout Arrays ) -----
	//---------------------------------------------------

	void BuildShoutCompatibility(CMiscPatchData* section) {

		section->names.clear();
		section->words_1.clear();
		section->words_2.clear();
		section->words_3.clear();

		for (auto& [formID, unused] : section->data.data)
		{
			auto* Shout = RE::TESForm::LookupByID<RE::TESShout>(formID);
			if (!Shout) { continue; }

			auto* Word1 = static_cast<RE::SpellItem*>(Shout->variations[0].spell);
			auto* Word2 = static_cast<RE::SpellItem*>(Shout->variations[1].spell);
			auto* Word3 = static_cast<RE::SpellItem*>(Shout->variations[2].spell);

			if (!Word1) { continue; }

			// One- and two-word shouts: the last word fills the missing ones, so learning it completes the shout
			if (!Word2) { Word2 = Word3 = Word1; }
			else if (!Word3) { Word3 = Word2; }

			section->forms.push_back(Shout);
			section->words_1.push_back(Word1);
			section->words_2.push_back(Word2);
			section->words_3.push_back(Word3);

			section->names.push_back(Shout->GetName());
			section->texts.push_back("NO_HIGHLIGHT");

			section->data_w1.AddForm(Word1);
			section->data_w2.AddForm(Word2);
			section->data_w3.AddForm(Word3);
		}

		section->names.resize(section->forms.size());
		section->texts.resize(section->forms.size());
		section->bools.resize(section->forms.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Learnt Words ) ----------
	//---------------------------------------------------

	// Words learnt from a shout's word spells found: 0 to 3
	[[nodiscard]] static int32_t LearntWords(const CMiscPatchData& a_section, std::size_t a_index)
	{
		const auto has = [&](const auto& a_words) { return a_index < a_words.size() && CFramework_Master::FoundItemData_NoShow.HasForm(a_words[a_index]); };
		return has(a_section.words_3) ? 3 : has(a_section.words_2) ? 2 : has(a_section.words_1) ? 1 : 0;
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Hooked Markers ) ----
	//---------------------------------------------------

	void ProcessHookedMarker(const char* nam) {
		
		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kLocations) {
				for (auto i = 0; i < section.forms.size(); i++) {
					if (DKUtil::string::iequals(nam, section.names[i]) && !CFramework_Master::FoundItemData_NoShow.HasForm(section.forms[i])) {
						ProcessMapMarker(section.forms[i], i, true, &section);
						return;
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, bool from_hook, CMiscPatchData* section) {
		auto* a_marker = a_form ? a_form->As<RE::TESObjectREFR>() : nullptr;

		if (a_marker && !CFramework_Master::FoundItemData_NoShow.HasForm(a_form)) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					section->bools[a_pos] = true;
					CFramework_Master::FoundItemData_NoShow.AddForm(a_form);
					if (from_hook)
					{
						auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, section->names[a_pos]);
						master::SendNotification(msg, "NotifySpecial");
						master::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, section->names[a_pos]);
					}
				}
			}
		}
		section->found = std::ranges::count(section->bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void ProcessFoundFormNoShow(ProcessPatchFoundFormArgs, std::string a_section) {

		auto pos_t = std::distance(data->forms.begin(), std::ranges::find(data->forms, data->data.GetForm(a_baseID)));

		if (!CFramework_Master::FoundItemData_NoShow.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data->names[pos_t]);
			master::SendNotification(msg, a_section);
			master::AddNewEventToLog(eventHandle, data->names[pos_t]);
		}

		CFramework_Master::FoundItemData_NoShow.AddForm(a_baseID);
		for (auto var : data->data.GetAllVariations()) {
			if (data->data.GetBase(var) == a_baseID) {
				CFramework_Master::FoundItemData_NoShow.AddForm(var);
			}
		}

		data->bools[pos_t] = true;
		data->found = std::ranges::count(data->bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void ProcessFoundForm(ProcessPatchFoundFormArgs, std::string a_section) {
		
		auto it = std::ranges::find(data->forms, data->data.GetForm(a_baseID));
		std::size_t pos_t = std::distance(data->forms.begin(), it);

		if (!CFramework_Master::FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{}{}!", CVariables::V_NotificationText, data->names[pos_t]);
			master::SendNotification(msg, a_section);

			if (auto* book = data->data.GetForm<RE::TESObjectBOOK>(a_eventID); book && book->GetSpell()) {
				master::AddNewEventToLog(Serialization::CompletionistLog::kTome, data->names[pos_t]);
			}
			else {
				master::AddNewEventToLog(eventHandle, data->names[pos_t]);
			}
		}

		CFramework_Master::FoundItemData.AddForm(a_baseID);
		for (auto var : data->data.GetAllVariations()) {
			if (data->data.GetBase(var) == a_baseID) {
				CFramework_Master::FoundItemData.AddForm(var);
			}
		}

		data->bools[pos_t] = true;
		data->found = std::ranges::count(data->bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void ProcessLearntWord(RE::FormID a_formID) {

		for (auto& section : type_sections)
		{
			if (section.type != CMiscPatchType::kShouts) {
				continue;
			}

			auto* Spell = static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(a_formID));
			if (!Spell) { continue; };

			if (auto a_pos = std::ranges::find(section.words_3, Spell); a_pos != section.words_3.end()) { // Player Knows All Words
				auto b_pos = std::distance(section.words_3.begin(), a_pos);

				if (!CFramework_Master::FoundItemData_NoShow.HasForm(section.words_3[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, section.forms[b_pos]->GetName());
					CFramework_Master::FrameworkAPI::SendNotification(msg, "NotifySpecial");
					CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kShout, section.forms[b_pos]->GetName());
				}

				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_3[b_pos]);
				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_2[b_pos]);
				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_1[b_pos]);
				section.bools[b_pos] = true;
				section.found = std::ranges::count(section.bools, true);
				continue;
			}

			if (auto a_pos = std::ranges::find(section.words_2, Spell); a_pos != section.words_2.end()) { // Player Knows 2nd Word
				auto b_pos = std::distance(section.words_2.begin(), a_pos);

				auto* Word2 = static_cast<RE::TESShout*>(section.forms[b_pos])->variations[1].word;

				if (!CFramework_Master::FoundItemData_NoShow.HasForm(section.words_2[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, section.forms[b_pos]->GetName(), CVariables::V_NotificationTextShout2, Word2->translation.c_str(), CVariables::V_NotificationTextShout3);
					CFramework_Master::FrameworkAPI::SendNotification(msg, "NotifySpecial");
					CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kWord, Word2->translation.c_str());
				}

				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_2[b_pos]);
				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_1[b_pos]);
				continue;
			}

			if (auto a_pos = std::ranges::find(section.words_1, Spell); a_pos != section.words_1.end()) { // Player Knows 1st Word
				auto b_pos = std::distance(section.words_1.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(section.forms[b_pos])->variations[0].word;

				if (!CFramework_Master::FoundItemData_NoShow.HasForm(section.words_1[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, section.forms[b_pos]->GetName(), CVariables::V_NotificationTextShout1, Word1->translation.c_str(), CVariables::V_NotificationTextShout3);
					CFramework_Master::FrameworkAPI::SendNotification(msg, "NotifySpecial");
					CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kWord, Word1->translation.c_str());
				}

				CFramework_Master::FoundItemData_NoShow.AddForm(section.words_1[b_pos]);
				continue;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Enchantment Learnt ) -----
	//---------------------------------------------------

	void OnEnchantmentLearnt(RE::TESForm* a_form) {
		using cmd = Serialization::CompletionistLog::logType;

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kEnchantments && section.data.HasForm(a_form)) {
				auto base = section.data.GetBase(a_form->GetFormID()) ? section.data.GetBase(a_form->GetFormID()) : a_form->GetFormID();
				ProcessFoundFormNoShow(base, a_form->GetFormID(), &section, cmd::kLearnt, "NotifySpecial");
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014) { return; }

		for (auto& section : type_sections)
		{
			auto base = section.data.GetBase(a_event->baseObj) ? section.data.GetBase(a_event->baseObj) : a_event->baseObj;

			if ( (section.type == CMiscPatchType::kItems || (section.type == CMiscPatchType::kBooks && CVariables::V_TreatBooksAsItems)) && section.data.HasForm(base)) {
				auto base = section.data.GetBase(a_event->baseObj) ? section.data.GetBase(a_event->baseObj) : a_event->baseObj;
				ProcessFoundForm(base, a_event->baseObj, &section, cmd::kCollected, "NotifyItems");
				return;
			}

			if (section.type == CMiscPatchType::kFish && section.data.HasForm(a_event->baseObj)) {	
				if (CanPickupFish(&section, base)) {
					ProcessFoundFormNoShow(base, a_event->baseObj, &section, cmd::kFish, "NotifySpecial");
					return;
				}
			}

			if (section.type == CMiscPatchType::kPlayerHomes && section.data.HasForm(a_event->baseObj))
			{
				for (auto& houseData : section.playerHomeData) {

					const auto* key = RE::TESForm::LookupByID<RE::TESKey>(houseData.formID);
					if (key && key->GetFormID() == a_event->baseObj && houseData.type == PlayerHomesDataStructType::kKey) {
						auto base = section.data.GetBase(a_event->baseObj) ? section.data.GetBase(a_event->baseObj) : a_event->baseObj;
						ProcessFoundFormNoShow(base, a_event->baseObj, &section, cmd::kObtained, "NotifySpecial");
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kBooks && section.data.HasForm(a_event->book->GetFormID())) {
				auto base = section.data.GetBase(a_event->book->GetFormID()) ? section.data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
				ProcessFoundForm(base, a_event->book->GetFormID(), &section, cmd::kBook, "NotifyBooks");
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Caught Fish ) -------
	//---------------------------------------------------

	void OnFishCaught(RE::TESForm* a_form) {
		using cmd = Serialization::CompletionistLog::logType;

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kFish && section.data.HasForm(a_form)) {
				auto base = section.data.GetBase(a_form->GetFormID()) ? section.data.GetBase(a_form->GetFormID()) : a_form->GetFormID();
				ProcessFoundFormNoShow(base, a_form->GetFormID(), &section, cmd::kFish, "NotifySpecial");
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Pet ) ------
	//---------------------------------------------------

	void OnActivateEvent(RE::TESObjectREFR* a_reference) {

		using cmd = Serialization::CompletionistLog::logType;

		if (!a_reference) { return; }

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kPets)
			{
				const auto* pet = a_reference->GetBaseObject()->As<RE::TESActorBase>();
				if (pet && section.data.HasForm(pet->GetFormID()))
				{
					auto base = section.data.GetBase(pet->GetFormID()) ? section.data.GetBase(pet->GetFormID()) : pet->GetFormID();
					ProcessFoundFormNoShow(base, pet->GetFormID(), &section, cmd::kTamed, "NotifySpecial");
					return;
				}
			}

			if (section.type == CMiscPatchType::kInteractableObject)
			{
				if (a_reference && section.data.HasForm(a_reference->GetFormID()))
				{
					auto base = section.data.GetBase(a_reference->GetFormID()) ? section.data.GetBase(a_reference->GetFormID()) : a_reference->GetFormID();
					ProcessFoundFormNoShow(base, a_reference->GetFormID(), &section, cmd::kDiscovered, "NotifySpecial");
					return;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Triggered / Set ) --
	//---------------------------------------------------

	void OnQuestStageTriggered(RE::TESQuestStageEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kPlayerHomes && section.data.HasForm(a_event->formID))
			{
				for (auto& houseData : section.playerHomeData) {

					const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(houseData.formID);
					if (quest && quest->GetFormID() == a_event->formID && houseData.type == PlayerHomesDataStructType::kQuest && houseData.stage == a_event->stage) {
						auto base = section.data.GetBase(a_event->formID) ? section.data.GetBase(a_event->formID) : a_event->formID;
						ProcessFoundFormNoShow(base, a_event->formID, &section, cmd::kObtained, "NotifySpecial");
					}
				}
			}

			if (section.type == CMiscPatchType::kPets && section.data.HasForm(a_event->formID))
			{
				for (auto& petData : section.petsData) {
					
					const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(petData.formID);
					if (quest && quest->GetFormID() == a_event->formID && petData.type == PetsDataStructType::kQuest && petData.stage == a_event->stage) {
						auto base = section.data.GetBase(a_event->formID) ? section.data.GetBase(a_event->formID) : a_event->formID;
						ProcessFoundFormNoShow(base, a_event->formID, &section, cmd::kTamed, "NotifySpecial");
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	// Enchantments learnt without an event, e.g. before an old save had the mod
	void RefreshKnownEnchantments() {

		for (auto& section : type_sections)
		{
			if (section.type == CMiscPatchType::kEnchantments) {
				for (auto i = 0; i < section.forms.size(); i++) {
					if (auto* enchantment = section.forms[i] ? section.forms[i]->As<RE::EnchantmentItem>() : nullptr; enchantment && enchantment->GetKnown()) {
						section.bools[i] = true;
						CFramework_Master::FoundItemData_NoShow.AddForm(enchantment);
					}
				}
			}
		}
	}

	void OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening) {
			for (auto& section : type_sections)
			{
				if (section.type == CMiscPatchType::kBooks && section.data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
					if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
						auto base = section.data.GetBase(target) ? section.data.GetBase(target) : target;
						ProcessFoundForm(base, target, &section, cmd::kBook, "NotifyBooks");
						return;
					}
				}
			}
			return;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening)
		{
			for (auto& section : type_sections)
			{
				if (section.type == CMiscPatchType::kLocations) {
					for (auto i = 0; i < section.forms.size(); i++) {
						ProcessMapMarker(section.forms[i], i, false, &section);
					}
				}
			}
		}
		return;
	}
};