#include "UI/GameDataProvider.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Misc/CFramework_Locations.hpp"
#include "Frameworks/Misc/CFramework_Dragons.hpp"
#include "Frameworks/Misc/CFramework_Ingredients.hpp"
#include "Frameworks/Misc/CFramework_Achievements.hpp"
#include "Frameworks/Misc/CFramework_Shouts.hpp"
#include "Frameworks/Misc/CFramework_Trainers.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/Localisation.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "Internal Utility/Settings.hpp"
#include "Internal Utility/Variables.hpp"
#include "Internal Utility/WordsOfPower.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "UI/Core/TranslationFile.hpp"
#include "UI/Tracker.hpp"

#include <d3d11.h>
#include <functional>

#undef GetObject

namespace CompletionistUI
{
	namespace
	{
		struct FrameworkPage
		{
			const char*                                       id;
			const char*                                       category;
			std::vector<std::pair<std::int32_t, const char*>> sections;
		};

		//---------------------------------------------------
		//-- Game Data ( Framework Pages ) ------------------
		//---------------------------------------------------

		// Same IDs and headers as MCMScript2's Select_Page
		const std::vector<FrameworkPage>& FrameworkPages() {

			static const std::vector<FrameworkPage> pages{
				{ "$MCMPageBarenziah", "UI_Category_Collections", { { 0, "$PageHeaderBarenziah1" } } },
				{ "$MCMPageDoomstone", "UI_Category_Collections", { { 6, "$PageHeaderDoomstone1" } } },
				{ "$MCMPageShrines", "UI_Category_Collections", { { 7, "$PageHeaderShrines1" }, { 8, "$PageHeaderShrines2" } } },
				{ "$MCMPageAEnchantments", "UI_Category_Collections", { { 9, "$PageHeaderAEnchantments" }, { 10, "$PageHeaderSEnchantments" } } },
				{ "$MCMPageWEnchantments", "UI_Category_Collections", { { 11, "$PageHeaderWEnchantments" }, { 12, "$PageHeaderSEnchantments" } } },
				{ "$MCMPagePets", "UI_Category_Collections", { { 13, "$PageHeaderPets1" } } },
				{ "$MCMPageHouses", "UI_Category_Collections", { { 14, "$PageHeaderHouses1" } } },
				{ "$MCMPageShouts", "UI_Category_Collections", { { 15, "$PageHeaderShouts1" }, { 16, "$PageHeaderShouts2" }, { 17, "$PageHeaderShouts3" } } },
				{ "$MCMPageLocations1", "UI_Category_Locations", { { 1, "$PageHeaderPlaces1" } } },
				{ "$MCMPageLocations2", "UI_Category_Locations", { { 2, "$PageHeaderPlaces1" } } },
				{ "$MCMPageLocations3", "UI_Category_Locations", { { 3, "$PageHeaderPlaces1" } } },
				{ "$MCMPageLocations4", "UI_Category_Locations", { { 4, "$PageHeaderPlaces1" } } },
				{ "$MCMPageLocations5", "UI_Category_Locations", { { 5, "$PageHeaderPlaces1" } } },
			};
			return pages;
		}

		constexpr std::string_view patchPrefix = "patch:";

		//---------------------------------------------------
		//-- Game Data ( Read Game File ) -------------------
		//---------------------------------------------------

		std::string ReadGameFile(const std::string& a_path) {

			// Loose files and BSAs
			RE::BSResourceNiBinaryStream stream(a_path);
			if (!stream.good()) {
				return {};
			}

			std::string data;
			char        byte{};
			while (stream.get(byte)) {
				data.push_back(byte);
			}
			return data;
		}

		//---------------------------------------------------
		//-- Game Data ( Shout Words ) ----------------------
		//---------------------------------------------------

		// A shout's words, the first a_learnt of them learnt
		void AddShoutWords(Entry& a_entry, const RE::TESForm* a_form, std::int32_t a_learnt) {

			const auto* shout = a_form ? a_form->As<RE::TESShout>() : nullptr;
			if (!shout || a_learnt < 0) {
				return;
			}
			for (std::int32_t i = 0; i < static_cast<std::int32_t>(std::size(shout->variations)); ++i) {
				if (const auto* word = shout->variations[i].word) {
					a_entry.words.push_back({ word->translation.c_str() ? word->translation.c_str() : word->GetName(), i < a_learnt });
				}
			}
		}

		//---------------------------------------------------
		//-- Game Data ( Add Map Marker ) -------------------
		//---------------------------------------------------

		// Map markers and placed items
		void AddMapMarker(Entry& a_entry, RE::TESForm* a_form, IDataProvider& a_provider) {

			auto* ref = a_form ? a_form->As<RE::TESObjectREFR>() : nullptr;
			if (!ref) {
				return;
			}
			if (ref->extraList.HasType<RE::ExtraMapMarker>()) {
				a_entry.canMark = true;
				if (CFramework_MapMa::CHandler::MarkerIsCleared(nullptr, ref)) {
					a_entry.detail = a_provider.Translate("MapMarkerCleared");
				}
				return;
			}
			a_entry.canMark = !a_entry.IsComplete() && ref->GetParentCell() && !ref->IsDisabled() && !ref->IsDeleted();
		}

		//---------------------------------------------------
		//-- Game Data ( Is Discovered ) --------------------
		//---------------------------------------------------

		// Discovered once its map marker can be travelled to
		bool IsDiscovered(RE::FormID a_location) {

			const auto* location = a_location ? RE::TESForm::LookupByID<RE::BGSLocation>(a_location) : nullptr;
			const auto  marker = location ? location->worldLocMarker.get() : nullptr;
			const auto* extra = marker ? marker->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
			return extra && extra->mapData && extra->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo);
		}

		//---------------------------------------------------
		//-- Game Data ( Add Word Places ) ------------------
		//---------------------------------------------------

		// Word walls first, then quests
		void AddWordPlaces(Entry& a_entry) {

			auto* shout = a_entry.formID ? RE::TESForm::LookupByID<RE::TESShout>(a_entry.formID) : nullptr;
			if (!shout || !WordsOfPower::Ready()) {
				return;
			}
			if (a_entry.words.empty()) {
				for (const auto& variation : shout->variations) {
					if (variation.word) {
						a_entry.words.push_back({ variation.word->translation.c_str() ? variation.word->translation.c_str() : variation.word->GetName(), false });
					}
				}
			}

			for (std::size_t i = 0; i < a_entry.words.size() && i < std::size(shout->variations) && i < 8; ++i) {
				const auto* word = shout->variations[i].word;
				if (!word) {
					continue;
				}
				for (const auto& source : WordsOfPower::SourcesFor(word->GetFormID())) {
					auto it = std::ranges::find_if(a_entry.places, [&](const EntryPlace& a_place) {
						return a_place.name == source.place && a_place.region == source.region && a_place.quest == source.quest;
					});
					if (it == a_entry.places.end()) {
						it = a_entry.places.insert(a_entry.places.end(), EntryPlace{ source.place, source.region, source.quest, 0, !source.quest && IsDiscovered(source.location) });
					}
					it->words |= static_cast<std::uint8_t>(1u << i);
				}
			}
			std::ranges::stable_partition(a_entry.places, [](const EntryPlace& a_place) { return !a_place.quest; });
			a_entry.placesKnown = true;
		}

		//---------------------------------------------------
		//-- Game Data ( Add Framework Section ) ------------
		//---------------------------------------------------

		void AddFrameworkSection(Page& a_page, std::int32_t a_id, const std::string& a_title, IDataProvider& a_provider) {

			using master = CFramework_Master::FrameworkAPI;

			const auto names = master::GetNameArrayByID(nullptr, a_id);
			const auto texts = master::GetTextArrayByID(nullptr, a_id);
			const auto bools = master::GetBoolArrayByID(nullptr, a_id);
			const auto forms = master::GetFormArrayByID(nullptr, a_id);
			if (names.empty()) {
				return;
			}

			auto& section = a_page.sections.emplace_back();
			section.title = a_title;
			for (std::size_t i = 0; i < names.size(); ++i) {
				auto& entry = section.entries.emplace_back();
				entry.name = names[i];
				if (i < forms.size()) {
					AddShoutWords(entry, forms[i], CFramework_Shouts::CHandler::LearntWords(forms[i]));
				}
				entry.state = i < bools.size() && bools[i] ? EntryState::kFound : EntryState::kNeeded;
				entry.formID = i < forms.size() && forms[i] ? forms[i]->GetFormID() : 0;
				entry.canPin = i < forms.size() && Completionist::CellScanner::IsItemPinnable(nullptr, forms[i]);
				// Locations (1-5) come from discovery, as in the MCM
				entry.frameworkID = a_id;
				entry.canToggle = entry.formID != 0 && (a_id < 1 || a_id > 5);
				AddMapMarker(entry, i < forms.size() ? forms[i] : nullptr, a_provider);
				if (i < texts.size() && texts[i] != "NO_HIGHLIGHT") {
					entry.description = a_provider.Translate(texts[i]);
				}
				AddWordPlaces(entry);
			}
		}

		//---------------------------------------------------
		//-- Game Data ( Add Patch Sections ) ---------------
		//---------------------------------------------------

		Entry QuestEntry(const CQuestData& a_data, std::int32_t a_id, IDataProvider& a_provider);
		void  AddPatchItems(Page& a_page, const CMiscPatch& a_patch, std::int32_t a_subPage, std::int32_t a_choice, IDataProvider& a_provider);

		//---------------------------------------------------
		//-- Game Data ( Add Journal Sections ) -------------
		//---------------------------------------------------

		// Main, side, radiant and completed, as the quest pages
		void AddJournalSections(Page& a_page, std::vector<Entry> a_entries, std::int32_t a_tab) {

			Section main{ "UI_QuestSection_Main", {}, a_tab };
			Section side{ "UI_QuestSection_Side", {}, a_tab };
			Section radiant{ "UI_QuestSection_Radiant", {}, a_tab };
			Section completed{ "UI_QuestSection_Completed", {}, a_tab };
			for (auto& entry : a_entries) {
				auto& section = entry.IsComplete() ? completed : entry.type == "UI_QuestType_Main" ? main : entry.type == "UI_QuestType_Side" ? side : radiant;
				section.entries.push_back(std::move(entry));
			}
			for (auto* section : { &main, &side, &radiant, &completed }) {
				if (!section->entries.empty()) {
					a_page.sections.push_back(std::move(*section));
				}
			}
		}

		//---------------------------------------------------
		//-- Game Data ( Add Patch Sections ) ---------------
		//---------------------------------------------------

		void AddPatchSections(Page& a_page, const CMiscPatch& a_patch, IDataProvider& a_provider) {

			using CHCMHandler::MCMAPI;

			// MCM headers end in ": "
			const auto clean = [](std::string a_text) {
				while (!a_text.empty() && (a_text.back() == ' ' || a_text.back() == ':')) {
					a_text.pop_back();
				}
				return a_text;
			};

			// The dropdown hides the entries for other choices
			const auto& pageName = a_page.title;
			const auto  choice = a_patch.HasDropDownMenu ? MCMAPI::GetCurrentUserOption(nullptr, pageName) : 0;
			if (a_patch.HasDropDownMenu && !a_patch.DropDownMenu.options.empty()) {
				a_page.choice.name = clean(a_patch.DropDownMenu.name);
				a_page.choice.info = a_patch.DropDownMenu.highlight;
				a_page.choice.options = a_patch.DropDownMenu.options;
				a_page.choice.selected = std::clamp(choice, 0, static_cast<std::int32_t>(a_patch.DropDownMenu.options.size()) - 1);
			}

			// Sections kept together by the patch's sub-page
			std::set<std::int32_t> subPages;
			for (const auto& data : a_patch.type_sections) {
				if (!data.IsEmpty() && data.enabled) {
					subPages.insert(data.displayOnPage);
				}
			}

			// A tab per sub-page, quests in the journal layout
			for (const auto subPage : subPages) {
				const auto named = a_patch.section_defs.find(subPage);
				const auto tabTitle = named != a_patch.section_defs.end() ? clean(named->second) : std::string{};
				const auto tab = static_cast<std::int32_t>(a_page.tabs.size());

				std::vector<Entry> quests;
				for (const auto& data : a_patch.type_sections) {
					if (data.IsEmpty() || !data.enabled || data.displayOnPage != subPage || data.type != CMiscPatchType::kQuests) {
						continue;
					}
					for (const auto* quest : data.quest_data_array) {
						if (quest && quest->GetQuest() && MCMAPI::IsQuestVisible(nullptr, pageName, quest->GetEditorID(), choice)) {
							quests.push_back(QuestEntry(*quest, data.quest_data_id, a_provider));
						}
					}
				}
				if (!quests.empty()) {
					AddJournalSections(a_page, std::move(quests), tab);
					a_page.tabs.push_back({ tabTitle, PageLayout::kJournal });
				}

				const auto itemTab = static_cast<std::int32_t>(a_page.tabs.size());
				const auto firstItems = a_page.sections.size();
				AddPatchItems(a_page, a_patch, subPage, choice, a_provider);
				if (a_page.sections.size() > firstItems) {
					for (auto i = firstItems; i < a_page.sections.size(); ++i) {
						a_page.sections[i].tab = itemTab;
						a_page.sections[i].title = clean(a_page.sections[i].title);
					}
					a_page.tabs.push_back({ tabTitle, PageLayout::kList });
				}
			}

			// One tab is just the page
			if (a_page.tabs.size() == 1) {
				a_page.layout = a_page.tabs.front().layout;
				a_page.tabs.clear();
				for (auto& section : a_page.sections) {
					section.tab = -1;
				}
			}
		}

		//---------------------------------------------------
		//-- Game Data ( Add Patch Items ) ------------------
		//---------------------------------------------------

		void AddPatchItems(Page& a_page, const CMiscPatch& a_patch, std::int32_t a_subPage, std::int32_t a_choice, IDataProvider& a_provider) {

			using CHCMHandler::MCMAPI;

			const auto& pageName = a_page.title;
			for (const auto& data : a_patch.type_sections) {
				if (data.IsEmpty() || !data.enabled || data.displayOnPage != a_subPage || data.type == CMiscPatchType::kQuests) {
					continue;
				}

				const auto& names = data.names;
				auto&       section = a_page.sections.emplace_back();
				section.title = data.pageheaderL;
				for (std::size_t i = 0; i < names.size(); ++i) {
					if (i < data.forms.size() && !MCMAPI::IsFormVisible(nullptr, pageName, data.forms[i], a_choice)) {
						continue;
					}
					auto& entry = section.entries.emplace_back();
					entry.name = names[i];
					if (data.type == CMiscPatchType::kShouts && i < data.forms.size()) {
						AddShoutWords(entry, data.forms[i], CMiscPatch::LearntWords(data, i));
					}
					entry.state = i < data.bools.size() && data.bools[i] ? EntryState::kFound : EntryState::kNeeded;
					entry.formID = i < data.forms.size() && data.forms[i] ? data.forms[i]->GetFormID() : 0;
					entry.canPin = i < data.forms.size() && Completionist::CellScanner::IsItemPinnable(nullptr, data.forms[i]);
					entry.frameworkID = data.ID;
					entry.canToggle = entry.formID != 0 && data.type != CMiscPatchType::kLocations;
					if (data.type == CMiscPatchType::kLocations) {
						AddMapMarker(entry, i < data.forms.size() ? data.forms[i] : nullptr, a_provider);
					}
					if (i < data.texts.size() && !data.texts[i].empty() && data.texts[i] != "NO_HIGHLIGHT") {
						entry.description = a_provider.Translate(data.texts[i]);
					}
					if (data.type == CMiscPatchType::kShouts) {
						AddWordPlaces(entry);
					}
				}
				if (section.entries.empty()) {
					a_page.sections.pop_back();
				}
			}
		}

		//---------------------------------------------------
		//-- MCM settings ( General Settings page ) ---------
		//---------------------------------------------------

		struct RadiantCounter
		{
			const char*  choice;	// Radiant_Quests entry
			const char*  property;
			const char*  label;
			std::int32_t maximum;
			const char*  plugin;
		};

		// Same order as the MCM's Build_Menus, so saved indexes match
		constexpr RadiantCounter radiantCounters[] = {
			{ "$RadiantChoice01", "State_RadiantCounterVal", "$State_RadiantCounterText", 20, nullptr },
			{ "$RadiantChoice02", "State_BountyCounterVal", "$State_BountyCounterText", 20, nullptr },
			{ "$RadiantChoice08", "State_CollegeCounterVal", "$State_CollegeCounterText", 20, nullptr },
			{ "$RadiantChoice07", "State_CompanionsCounterVal", "$State_CompanionsCounterText", 20, nullptr },
			{ "$RadiantChoice09", "State_DBCounterVal", "$State_DBCounterText", 20, nullptr },
			{ "$RadiantChoice03", "State_TGRCounterVal", "$State_TGRCounterText", 20, nullptr },
			{ "$RadiantChoice04", "State_DLC1CounterVal", "$State_DLC1CounterText", 20, nullptr },
			{ "$RadiantChoice11", "State_BladesCounterVal", "$State_TheBladesCounterText", 20, nullptr },
			{ "$RadiantChoice10", "State_FishingCounterVal", "$State_FishingCounterText", 4, "ccbgssse001-fish.esm" },
			{ "$RadiantChoice05", "State_LOTDCounterVal", "$State_LOTDCounterText", 20, "LegacyoftheDragonborn.esm" },
			{ "$RadiantChoice06", "State_VigilantCounterVal", "$State_VigilantCounterText", 20, "Vigilant.esm" },
		};

		//---------------------------------------------------
		//-- Game Data ( Is Plugin Loaded ) -----------------
		//---------------------------------------------------

		bool IsPluginLoaded(std::string_view a_plugin) {

			auto* handler = RE::TESDataHandler::GetSingleton();
			return handler && (handler->LookupLoadedModByName(a_plugin) || handler->LookupLoadedLightModByName(a_plugin));
		}

		//---------------------------------------------------
		//-- Game Data ( Counter For ) ----------------------
		//---------------------------------------------------

		const RadiantCounter* CounterFor(const GameDataProvider::GeneralSettings& a_values) {

			if (a_values.radiantChoice < 0 || a_values.radiantChoice >= static_cast<std::int32_t>(a_values.radiantOptions.size())) {
				return nullptr;
			}
			const auto& choice = a_values.radiantOptions[a_values.radiantChoice];
			for (const auto& counter : radiantCounters) {
				if (choice == counter.choice) {
					return std::addressof(counter);
				}
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Game Data ( Info Key ) -------------------------
		//---------------------------------------------------

		// Foo_Text -> Foo_Info
		std::string InfoKey(std::string a_label) {

			if (a_label.ends_with("Text")) {
				a_label.replace(a_label.size() - 4, 4, "Info");
			}
			return a_label;
		}

		//---------------------------------------------------
		//-- Game Data ( Read Int ) -------------------------
		//---------------------------------------------------

		std::int32_t ReadInt(const char* a_name, std::int32_t a_default = 0) {

			const auto* variable = CVariables::VariablesAPI::GetProperty(a_name);
			return variable && variable->IsInt() ? variable->GetSInt() : a_default;
		}

		//---------------------------------------------------
		//-- Game Data ( Read Bool ) ------------------------
		//---------------------------------------------------

		bool ReadBool(const char* a_name) {

			const auto* variable = CVariables::VariablesAPI::GetProperty(a_name);
			return variable && variable->IsBool() && variable->GetBool();
		}

		//---------------------------------------------------
		//-- Game Data ( Read General Settings ) ------------
		//---------------------------------------------------

		GameDataProvider::GeneralSettings ReadGeneralSettings() {

			GameDataProvider::GeneralSettings values;

			for (const auto& counter : radiantCounters) {
				if (!counter.plugin || IsPluginLoaded(counter.plugin)) {
					values.radiantOptions.emplace_back(counter.choice);
				}
				values.counters[counter.property] = ReadInt(counter.property, 5);
			}
			values.radiantChoice = ReadInt("Radiant_Quests_Choice");

			values.dawnguardOptions = { "$MenuChoiceDefault", "$MenuChoice03", "$MenuChoice04" };
			values.dawnguardChoice = ReadInt("DG_Faction_Choice");

			values.legacyInstalled = IsPluginLoaded("LegacyoftheDragonborn.esm");
			values.legacyOptions = { "$MenuChoiceDefault", "$MenuChoice05" };
			if (IsPluginLoaded("Alternate Start - Live Another Life.esp") || IsPluginLoaded("AlternatePerspective.esp")) {
				values.legacyOptions.emplace_back("$MenuChoice06");
				if (IsPluginLoaded("DBM_RelicHunter.esp")) {
					values.legacyOptions.emplace_back("$MenuChoice07");
				}
			}
			values.legacyChoice = ReadInt("Legacy_Faction_Choice");

			values.treatBooksAsItems = ReadBool("TreatBooksAsItems");

			values.dllVersion = CFramework_Master::FrameworkAPI::GetVersion(nullptr);
			values.patchCount = CFramework_Master::FrameworkAPI::GetPatchCount(nullptr);
			values.valid = true;
			return values;
		}

		//---------------------------------------------------
		//-- Game Data ( Build General Page ) ---------------
		//---------------------------------------------------

		SettingsPage BuildGeneralPage(const GameDataProvider::GeneralSettings& a_values) {

			SettingsPage page{ "general", "$MCMPageSettings", {} };
			auto&        left = page.columns[0];
			auto&        right = page.columns[1];

			left.push_back({ .id = "header1", .kind = SettingKind::kHeader, .label = "$MCMPageSettingsHeader" });
			left.push_back({ .id = "radiant", .kind = SettingKind::kChoice, .label = "$State_RadiantQuestsText", .info = "$State_RadiantQuestsInfo", .choice = a_values.radiantChoice, .options = a_values.radiantOptions });
			if (const auto* counter = CounterFor(a_values)) {
				left.push_back({ .id = "counter", .kind = SettingKind::kSlider, .label = counter->label, .info = InfoKey(counter->label), .number = a_values.counters.at(counter->property), .minimum = 1, .maximum = counter->maximum });
			}
			left.push_back({ .id = "spacer1", .kind = SettingKind::kSpacer });
			left.push_back({ .id = "books", .kind = SettingKind::kToggle, .label = "$TreatBooksAsItemsState_Text", .info = "$TreatBooksAsItemsState_Info", .on = a_values.treatBooksAsItems });

			right.push_back({ .id = "header2", .kind = SettingKind::kHeader, .label = "$MCMPageSettingsHeader2" });
			right.push_back({ .id = "dawnguard", .kind = SettingKind::kChoice, .label = "$DGFaction", .info = "$DGFactionInfo", .choice = a_values.dawnguardChoice, .options = a_values.dawnguardOptions });
			if (a_values.legacyInstalled) {
				right.push_back({ .id = "legacy", .kind = SettingKind::kChoice, .label = "$LDFaction", .info = "$LDFactionInfo", .choice = a_values.legacyChoice, .options = a_values.legacyOptions });
			} else {
				right.push_back({ .id = "legacy", .kind = SettingKind::kText, .label = "$LDFaction", .info = "$LDFactionInfo", .value = "$NOFaction" });
			}
			return page;
		}

		//---------------------------------------------------
		//-- Quests -----------------------------------------
		//---------------------------------------------------

		constexpr std::string_view questPrefix = "quest:";

		//---------------------------------------------------
		//-- Game Data ( Clean Quest Text ) -----------------
		//---------------------------------------------------

		std::string CleanQuestText(std::string a_text) {

			for (auto pos = a_text.find("\\n"); pos != std::string::npos; pos = a_text.find("\\n", pos)) {
				auto start = pos;
				while (start > 0 && a_text[start - 1] == ' ') {
					--start;
				}
				a_text.replace(start, pos + 2 - start, "\n");
				pos = start + 1;
			}
			return a_text;
		}

		// Page conditionals from _Build_Quests
		struct QuestPageFilter
		{
			std::string                                    notice;
			std::function<bool(std::size_t, const std::string&)> shows;
		};

		//---------------------------------------------------
		//-- Game Data ( Quest Page Filter For ) ------------
		//---------------------------------------------------

		QuestPageFilter QuestPageFilterFor(std::int32_t a_id, const std::string& a_title) {

			const auto setting = [](const char* a_name) {
				const auto* variable = CSettings::SettingsAPI::Get(a_name);
				return variable ? variable->GetSInt() : 0;
			};

			// CWPlayerAllegiance 1 Imperial, 2 Stormcloak
			if (a_title == "$MCMPageMainCW") {
				const auto* allegiance = RE::TESForm::LookupByEditorID<RE::TESGlobal>("CWPlayerAllegiance");
				const int   side = allegiance ? static_cast<int>(allegiance->value) : 0;
				if (side != 1 && side != 2) {
					return { "$PickFactionErrorMSGCW", {} };
				}
				return { {}, [side](std::size_t a_index, const std::string&) { return side == 1 ? a_index < 16 : a_index >= 16; } };
			}

			if (a_title == "$MCMPageMainDG") {
				const int choice = setting("DG_Faction_Choice");
				if (choice != 1 && choice != 2) {
					return { "$PickFactionErrorMSG", {} };
				}
				return { {}, [choice](std::size_t a_index, const std::string&) {
							if (a_index >= 3 && a_index <= 4) {
								return choice == 1;
							}
							if (a_index >= 5 && a_index <= 6) {
								return choice == 2;
							}
							return true;
						} };
			}

			if (a_title == "$MCMPageMuseum") {
				const int choice = setting("Legacy_Faction_Choice");
				if (choice == 0) {
					return { "$PickFactionErrorMSG", {} };
				}
				const bool noAltStart = !Serialization::CompletionistData::IsModInstalled("Alternate Start - Live Another Life.esp") &&
				                        !Serialization::CompletionistData::IsModInstalled("AlternatePerspective.esp");
				std::size_t haunted = std::numeric_limits<std::size_t>::max();
				std::size_t index = 0;
				for (const auto& [data, name, id, key] : CQuestMaster::CQuestDataVec) {
					if (id == a_id && data && data->GetQuest()) {
						if (data->editor_id == "DBM_HauntedMuseumQuest") {
							haunted = index;
							break;
						}
						++index;
					}
				}
				return { {}, [=](std::size_t a_index, const std::string&) {
							if (a_index >= haunted) {
								return true;
							}
							if (choice == 1 || noAltStart) {
								return a_index == 0;
							}
							if (choice == 2) {
								return a_index == 1;
							}
							return choice == 3 && a_index >= 2 && a_index <= 4;
						} };
			}
			return {};
		}

		//---------------------------------------------------
		//-- Game Data ( Quest Entry ) ----------------------
		//---------------------------------------------------

		Entry QuestEntry(const CQuestData& a_data, std::int32_t a_id, IDataProvider& a_provider) {

			using CQuestMaster::QuestAPI;

			const auto key = a_data.GetKey();
			const auto status = QuestAPI::qIsOptionCompleted(nullptr, a_id, key);
			const bool manual = QuestAPI::qIsOptionToggled(nullptr, a_id, key);

			Entry entry;
			entry.name = a_data.GetName();
			entry.description = CleanQuestText(a_data.GetHighlight());
			entry.state = status > 0 || manual ? EntryState::kFound : EntryState::kNeeded;
			entry.canToggle = status != 2;
			entry.marked = manual;
			entry.questPage = a_id;
			entry.key = key;
			entry.status = entry.IsComplete() ? "UI_QuestState_Done" : "UI_QuestState_Pending";
			entry.type = a_data.GetType() == std::to_underlying(CFlagEnum::kMain) ? "UI_QuestType_Main" : a_data.GetType() == std::to_underlying(CFlagEnum::kSide) ? "UI_QuestType_Side" : "UI_QuestType_Radiant";
			if (a_data.HasRadiantData()) {
				entry.detail = fmt::format("{} / {}", CFramework_Master::RadiantCountData.GetCount(key), a_data.GetRadiantTimesRequired());
			} else if (manual) {
				entry.detail = a_provider.Translate("UI_QuestMarked");
			}
			return entry;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Quest Page ) -----------------
		//---------------------------------------------------

		Page BuildQuestPage(std::int32_t a_id, const std::string& a_title, GameDataProvider& a_provider) {

			Page page{ std::string(questPrefix) + std::to_string(a_id), a_title, {}, PageLayout::kJournal };

			const auto filter = QuestPageFilterFor(a_id, a_title);
			if (!filter.notice.empty()) {
				page.notice = filter.notice;
				return page;
			}

			std::vector<Entry> entries;
			std::size_t        index = 0;
			for (const auto& [data, name, id, key] : CQuestMaster::CQuestDataVec) {
				if (id != a_id || !data || !data->GetQuest()) {
					continue;
				}
				if (const auto at = index++; filter.shows && !filter.shows(at, data->editor_id)) {
					continue;
				}
				entries.push_back(QuestEntry(*data, id, a_provider));
			}

			AddJournalSections(page, std::move(entries), -1);
			return page;
		}

		//---------------------------------------------------
		//-- Completion log ---------------------------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Game Data ( Read Recent Events ) ---------------
		//---------------------------------------------------

		std::vector<RecentEvent> ReadRecentEvents(std::size_t a_count) {

			// dd/mm/yyyy
			const auto sortKey = [](const std::string& a_date) {
				return a_date.size() == 10 ? a_date.substr(6, 4) + a_date.substr(3, 2) + a_date.substr(0, 2) : a_date;
			};

			std::vector<std::pair<std::string, std::string>> days(CFramework_Master::LoggingData.data.begin(), CFramework_Master::LoggingData.data.end());
			std::ranges::sort(days, [&](const auto& a_left, const auto& a_right) { return sortKey(a_left.first) > sortKey(a_right.first); });

			std::vector<RecentEvent> events;
			for (const auto& [date, log] : days) {
				// |hh:mm ~ text||hh:mm ~ text|
				std::vector<std::string_view> lines;
				for (auto pos = log.find('|'); pos != std::string::npos;) {
					const auto end = log.find('|', pos + 1);
					if (end == std::string::npos) {
						break;
					}
					if (end > pos + 1) {
						lines.emplace_back(std::string_view(log).substr(pos + 1, end - pos - 1));
					}
					pos = log.find('|', end + 1);
				}
				for (auto it = lines.rbegin(); it != lines.rend() && events.size() < a_count; ++it) {
					const auto split = it->find(" ~ ");
					if (split == std::string_view::npos) {
						continue;
					}
					events.push_back({ std::string(it->substr(split + 3)), date, std::string(it->substr(0, split)) });
				}
				if (events.size() >= a_count) {
					break;
				}
			}
			return events;
		}

		//---------------------------------------------------
		//-- Game Data ( Is Within ) ------------------------
		//---------------------------------------------------

		bool IsWithin(const RE::BGSLocation* a_location, const RE::BGSLocation* a_parent) {

			for (auto* location = a_location; location; location = location->parentLoc) {
				if (location == a_parent) {
					return true;
				}
			}
			return false;
		}

		//---------------------------------------------------
		//-- Game Data ( Hold Of ) --------------------------
		//---------------------------------------------------

		// The hold, else the furthest named place above it (e.g. Solstheim)
		const RE::BGSLocation* HoldOf(const RE::BGSLocation* a_location) {

			const RE::BGSLocation* top = nullptr;
			for (auto* location = a_location ? a_location->parentLoc : nullptr; location; location = location->parentLoc) {
				if (location->HasKeywordString("LocTypeHold")) {
					return location;
				}
				if (location->parentLoc && location->GetFullName() && *location->GetFullName()) {
					top = location;
				}
			}
			return top;
		}

		//---------------------------------------------------
		//-- Game Data ( Place Name ) -----------------------
		//---------------------------------------------------

		// "Warmaiden's, Whiterun Hold"
		std::string PlaceName(const RE::BGSLocation* a_location, const RE::TESObjectREFR* a_ref) {

			const auto* name = a_location ? a_location->GetFullName() : nullptr;
			if (!name || !*name) {
				const auto* cell = a_ref ? a_ref->GetSaveParentCell() : nullptr;
				return cell && cell->GetFullName() ? cell->GetFullName() : std::string{};
			}
			const auto* hold = HoldOf(a_location);
			const auto* holdName = hold ? hold->GetFullName() : nullptr;
			return holdName && *holdName ? fmt::format("{}, {}", name, holdName) : std::string(name);
		}

		//---------------------------------------------------
		//-- Game Data ( Required Level ) -------------------
		//---------------------------------------------------

		// The perk's own GetBaseActorValue condition on the skill, as the Skills menu shows it
		std::int32_t RequiredLevel(RE::BGSPerk* a_perk, RE::ActorValue a_skill) {

			for (auto* item = a_perk ? a_perk->perkConditions.head : nullptr; item; item = item->next) {
				const auto& data = item->data;
				if (data.functionData.function.get() == RE::FUNCTION_DATA::FunctionID::kGetBaseActorValue && !data.flags.global &&
					static_cast<RE::ActorValue>(reinterpret_cast<std::uintptr_t>(data.functionData.params[0])) == a_skill) {
					return static_cast<std::int32_t>(data.comparisonValue.f);
				}
			}
			return 0;
		}

		//---------------------------------------------------
		//-- Game Data ( Perk Tree ) ------------------------
		//---------------------------------------------------

		// From the skill's own tree, so perk overhauls show as they are in game
		std::vector<TrainerPerk> PerkTree(RE::ActorValue a_skill, RE::PlayerCharacter* a_player) {

			const auto* info = RE::ActorValueList::GetActorValueInfo(a_skill);
			auto*       root = info ? info->perkTree : nullptr;
			if (!root) {
				return {};
			}

			// Every node with a perk, the root holds none
			std::vector<RE::BGSSkillPerkTreeNode*>                  nodes;
			std::unordered_map<RE::BGSSkillPerkTreeNode*, std::int32_t> index;
			std::vector<RE::BGSSkillPerkTreeNode*>                  pending{ root };
			std::unordered_set<RE::BGSSkillPerkTreeNode*>           seen{ root };
			while (!pending.empty() && nodes.size() < 256) {
				auto* node = pending.back();
				pending.pop_back();
				if (node->perk) {
					index.emplace(node, static_cast<std::int32_t>(nodes.size()));
					nodes.push_back(node);
				}
				for (auto* child : node->children) {
					if (child && seen.insert(child).second) {
						pending.push_back(child);
					}
				}
			}

			std::vector<TrainerPerk> perks;
			perks.reserve(nodes.size());
			for (auto* node : nodes) {
				auto& perk = perks.emplace_back();
				perk.name = node->perk->GetName();
				perk.depth = static_cast<float>(node->perkGridY) + node->verticalPosition;
				perk.lane = static_cast<float>(node->perkGridX) + node->horizontalPosition;
				perk.level = RequiredLevel(node->perk, a_skill);
				perk.ranks = 0;
				std::unordered_set<RE::BGSPerk*> ranks;
				for (auto* rank = node->perk; rank && perk.ranks < 10 && ranks.insert(rank).second; rank = rank->nextPerk) {
					++perk.ranks;
					perk.rank += a_player && a_player->HasPerk(rank) ? 1 : 0;
				}
				for (auto* child : node->children) {
					if (const auto it = index.find(child); it != index.end()) {
						perk.children.push_back(it->second);
					}
				}
			}
			return perks;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Trainer Page ) ---------------
		//---------------------------------------------------

		// A tab per specialisation, a section per skill, as the Skills menu
		Page BuildTrainerPage(IDataProvider& a_provider) {

			using CFramework_Trainers::CHandler;

			Page page{ "$MCMPageTrainers", "$MCMPageTrainers", {} };
			page.tabs = { { "UI_Trainers_Combat" }, { "UI_Trainers_Magic" }, { "UI_Trainers_Stealth" } };
			const auto tabOf = [](std::int32_t a_skill) { return a_skill <= 6 ? 0 : a_skill >= 12 ? 1 : 2; };

			auto*      player = RE::PlayerCharacter::GetSingleton();
			const auto tier = [&](std::int32_t a_level) {
				return a_provider.Translate(a_level <= 50 ? "UI_Trainers_Tier0" : a_level <= 75 ? "UI_Trainers_Tier1" : "UI_Trainers_Tier2") + "  \xC2\xB7  " + std::to_string(a_level);
			};

			std::vector<const CFramework_Trainers::Trainer*> placed;
			for (const auto& trainer : CFramework_Trainers::Trainers) {
				if (trainer.ref) {
					placed.push_back(std::addressof(trainer));
				}
			}
			std::ranges::sort(placed, [](const auto* a_left, const auto* a_right) {
				return std::tuple(a_left->maxLevel, std::string_view(a_left->npc->GetName())) < std::tuple(a_right->maxLevel, std::string_view(a_right->npc->GetName()));
			});

			for (const auto tab : { 0, 1, 2 }) {
				for (std::int32_t skill = 0; skill <= 17; ++skill) {
					if (tabOf(skill) != tab) {
						continue;
					}

					Section    section{ CHandler::SkillName(skill), {}, tab };
					const auto perks = PerkTree(CHandler::SkillValue(skill), player);
					for (const auto* trainer : placed) {
						if (trainer->skill != skill) {
							continue;
						}
						const auto  ref = trainer->ref.get();
						const auto* home = ref ? ref->GetEditorLocation() : nullptr;

						auto& entry = section.entries.emplace_back();
						entry.name = trainer->npc->GetName();
						entry.detail = tier(trainer->maxLevel);
						entry.state = CHandler::IsTrained(*trainer) ? EntryState::kFound : EntryState::kNeeded;
						entry.formID = trainer->npc->GetFormID();
						entry.frameworkID = CFramework_Trainers::FrameworkID;
						entry.canToggle = true;
						entry.refID = ref ? ref->GetFormID() : 0;
						entry.canMark = ref && !ref->IsDisabled() && ref->GetSaveParentCell();

						auto& info = entry.trainer;
						info.skill = skill;
						info.skillName = section.title;
						info.maxLevel = trainer->maxLevel;
						info.playerLevel = player ? static_cast<std::int32_t>(player->AsActorValueOwner()->GetBaseActorValue(CHandler::SkillValue(skill))) : 0;
						info.location = PlaceName(home, ref.get());
						const auto* now = ref ? ref->GetCurrentLocation() : nullptr;
						if (now && home && !IsWithin(now, home) && !IsWithin(home, now)) {
							info.now = PlaceName(now, ref.get());
						}
						info.perks = perks;
					}

					if (!section.entries.empty()) {
						page.sections.push_back(std::move(section));
					}
				}
			}
			return page;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Dragon Page ) ----------------
		//---------------------------------------------------

		// The named dragons by game, and the dragon kills on a tab of their own
		Page BuildDragonPage(IDataProvider& a_provider) {

			using namespace CFramework_Dragons;

			Page page{ "$MCMPageDragons", "$MCMPageDragons", {} };
			page.tabs = { { "UI_Dragons_Unique" }, { "UI_Dragons_Kills", PageLayout::kKills } };

			const auto sectionFor = [&](const std::string& a_plugin) -> Section& {
				const auto title = a_plugin == "Skyrim.esm" ? std::string("UI_Dragons_Skyrim") : a_plugin == "Dawnguard.esm" ? std::string("UI_Dragons_Dawnguard") : a_plugin == "Dragonborn.esm" ? std::string("UI_Dragons_Dragonborn") : a_plugin;
				const auto it = std::ranges::find(page.sections, title, &Section::title);
				return it != page.sections.end() ? *it : page.sections.emplace_back(Section{ title, {}, 0 });
			};

			for (const auto& dragon : Dragons) {
				const auto fate = CHandler::FateOf(dragon);

				// The marker goes on one that's alive and out in the world
				RE::NiPointer<RE::TESObjectREFR> placed;
				for (const auto& handle : dragon.refs) {
					auto        ref = handle.get();
					const auto* actor = ref ? ref->As<RE::Actor>() : nullptr;
					if (actor && !actor->IsDead() && !ref->IsDisabled() && ref->GetSaveParentCell()) {
						placed = ref;
						break;
					}
				}

				auto& entry = sectionFor(dragon.plugin).entries.emplace_back();
				entry.name = dragon.name;
				entry.detail = a_provider.Translate(CHandler::FateKey(fate));
				entry.state = CHandler::IsDone(dragon, fate) ? EntryState::kFound : EntryState::kNeeded;
				entry.formID = dragon.formID;
				entry.frameworkID = FrameworkID;
				entry.canToggle = true;
				entry.refID = placed ? placed->GetFormID() : 0;
				entry.canMark = placed != nullptr;

				auto& info = entry.dragon;
				info.fate = entry.detail;
				if (!dragon.key.empty()) {
					// None for a dragon without a name in the dragon tongue
					const auto meaningKey = "UI_Dragons_Meaning_" + dragon.key;
					if (auto meaning = a_provider.Translate(meaningKey); meaning != meaningKey) {
						info.meaning = std::move(meaning);
					}
					info.place = a_provider.Translate("UI_Dragons_Where_" + dragon.key);
					info.lore = a_provider.Translate("UI_Dragons_Lore_" + dragon.key);
				} else {
					auto        ref = dragon.refs.empty() ? RE::NiPointer<RE::TESObjectREFR>{} : dragon.refs.front().get();
					const auto* home = ref ? ref->GetEditorLocation() : nullptr;
					info.place = PlaceName(home, ref.get());
					info.lore = a_provider.Translate("UI_Dragons_Lore_Mod{" + dragon.plugin + "}");
				}
				entry.description = info.lore;
			}
			return page;
		}

		//---------------------------------------------------
		//-- Game Data ( Alchemy Perk Conditions ) ----------
		//---------------------------------------------------

		// As the game reads an alchemy perk's conditions: an OR joins an item to the next, the
		// ORs first, then the ANDs. Whether it's a poison and the effect's keywords are answered
		// here, since there's no brew; a kind of 0 means not known yet, which fails them
		bool AlchemyConditionsPass(const RE::BGSEntryPointPerkEntry& a_entry, RE::PlayerCharacter* a_player, const RE::EffectSetting* a_effect, int a_kind) {

			using Function = RE::FUNCTION_DATA::FunctionID;
			const auto test = [&](const RE::TESConditionItem& a_item) {
				const auto& data = a_item.data;
				const auto  wanted = data.flags.global ? (data.comparisonValue.g ? data.comparisonValue.g->value : 0.0f) : data.comparisonValue.f;
				const auto  compare = [&](float a_value) {
					switch (data.flags.opCode) {
					case RE::CONDITION_ITEM_DATA::OpCode::kEqualTo: return a_value == wanted;
					case RE::CONDITION_ITEM_DATA::OpCode::kNotEqualTo: return a_value != wanted;
					case RE::CONDITION_ITEM_DATA::OpCode::kGreaterThan: return a_value > wanted;
					case RE::CONDITION_ITEM_DATA::OpCode::kGreaterThanOrEqualTo: return a_value >= wanted;
					case RE::CONDITION_ITEM_DATA::OpCode::kLessThan: return a_value < wanted;
					default: return a_value <= wanted;
					}
				};
				switch (data.functionData.function.get()) {
				case Function::kEPAlchemyGetMakingPoison:
					return a_kind != 0 && compare(a_kind == 2 ? 1.0f : 0.0f);
				case Function::kEPAlchemyEffectHasKeyword: {
					const auto* keyword = static_cast<const RE::BGSKeyword*>(data.functionData.params[0]);
					return a_effect && compare(keyword && a_effect->HasKeyword(keyword) ? 1.0f : 0.0f);
				}
				case Function::kEPTemperingItemIsEnchanted:
				case Function::kEPTemperingItemHasKeyword:
				case Function::kEPModSkillUsage_IsAdvanceSkill:
				case Function::kEPModSkillUsage_AdvanceObjectHasKeyword:
				case Function::kEPModSkillUsage_IsAdvanceAction:
				case Function::kEPMagic_SpellHasKeyword:
				case Function::kEPMagic_SpellHasSkill:
					// About something other than a brew
					return false;
				default: {
					RE::ConditionCheckParams params(a_player, a_player);
					return a_item.IsTrue(params);
				}
				}
			};

			for (std::uint32_t tab = 0; tab < a_entry.conditions.size(); ++tab) {
				bool all = true;
				bool any = false;
				bool open = false;
				for (auto* item = a_entry.conditions[tab].head; item; item = item->next) {
					any |= test(*item);
					open = true;
					if (!item->data.flags.isOR) {
						all &= any;
						any = false;
						open = false;
					}
				}
				if (open) {
					all &= any;
				}
				if (!all) {
					return false;
				}
			}
			return true;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Mixing Data ) ----------------
		//---------------------------------------------------

		// Every listed ingredient's effects as the alchemy table would brew them now: the game's
		// own settings, the player's Alchemy skill and Fortify Alchemy, and the perks they have
		std::shared_ptr<const MixingData> BuildMixingData(IDataProvider& a_provider, const std::function<std::int32_t(const RE::IngredientItem*)>& a_carried) {

			using namespace CFramework_Ingredients;
			using EntryPoint = RE::BGSEntryPoint::ENTRY_POINTS::ENTRY_POINT;

			auto  data = std::make_shared<MixingData>();
			auto* player = RE::PlayerCharacter::GetSingleton();
			auto* dataHandler = RE::TESDataHandler::GetSingleton();
			if (!player || !dataHandler) {
				return data;
			}

			const auto gameSetting = [](const char* a_name, float a_default) {
				auto* settings = RE::GameSettingCollection::GetSingleton();
				auto* setting = settings ? settings->GetSetting(a_name) : nullptr;
				return setting ? setting->GetFloat() : a_default;
			};
			const float ingredientMult = gameSetting("fAlchemyIngredientInitMult", 4.0f);
			const float skillFactor = gameSetting("fAlchemySkillFactor", 1.5f);
			const float skill = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kAlchemy);
			const float fortify = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kAlchemyModifier);

			// The perks that change a brew's strength, and Purity's
			std::vector<const RE::BGSEntryPointPerkEntry*> effectiveness;
			for (auto* perk : dataHandler->GetFormArray<RE::BGSPerk>()) {
				if (!perk || !player->HasPerk(perk)) {
					continue;
				}
				for (auto* entry : perk->perkEntries) {
					if (!entry || entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint) {
						continue;
					}
					const auto* point = static_cast<const RE::BGSEntryPointPerkEntry*>(entry);
					if (point->entryData.entryPoint.get() == EntryPoint::kModAlchemyEffectiveness) {
						effectiveness.push_back(point);
					} else if (point->entryData.entryPoint.get() == EntryPoint::kPurifyAlchemyIngredients && AlchemyConditionsPass(*point, player, nullptr, 1)) {
						data->purity = true;
					}
				}
			}
			const auto perkMult = [&](const RE::EffectSetting* a_effect, int a_kind) {
				float value = 1.0f;
				for (const auto* entry : effectiveness) {
					const auto* one = entry->functionData && entry->functionData->GetType() == RE::BGSEntryPointFunctionData::ENTRY_POINT_FUNCTION_DATA::kOneValue ? static_cast<const RE::BGSEntryPointFunctionDataOneValue*>(entry->functionData) : nullptr;
					if (!one || !AlchemyConditionsPass(*entry, player, a_effect, a_kind)) {
						continue;
					}
					switch (entry->entryData.function.get()) {
					case RE::BGSEntryPointPerkEntry::Function::kSetValue: value = one->data; break;
					case RE::BGSEntryPointPerkEntry::Function::kAddValue: value += one->data; break;
					case RE::BGSEntryPointPerkEntry::Function::kMultiplyValue: value *= one->data; break;
					default: break;
					}
				}
				return value;
			};
			const float power = ingredientMult * (1.0f + (skillFactor - 1.0f) * skill / 100.0f) * (1.0f + fortify / 100.0f);
			data->power = a_provider.Translate("UI_Mix_Power{" + std::to_string(static_cast<int>(skill)) + "}{" + std::to_string(static_cast<int>(std::round(fortify))) + "}");

			using Flag = RE::EffectSetting::EffectSettingData::Flag;
			for (const auto& ingredient : Ingredients) {
				auto& mix = data->ingredients.emplace_back();
				mix.formID = ingredient.form->GetFormID();
				mix.name = ingredient.form->GetFullName();
				mix.carried = a_carried(ingredient.form);
				for (std::uint32_t i = 0; i < ingredient.form->effects.size(); ++i) {
					const auto* item = ingredient.form->effects[i];
					const auto* effect = item ? item->baseEffect : nullptr;
					if (!effect) {
						continue;
					}
					auto& out = mix.effects.emplace_back();
					out.id = effect->GetFormID();
					out.name = effect->GetFullName() ? effect->GetFullName() : "";
					out.text = effect->magicItemDescription.c_str();
					out.known = CHandler::Knows(ingredient.form, i);
					out.hostile = effect->IsHostile();
					out.priority = item->cost;
					const auto& flags = effect->data.flags;
					for (int kind = 0; kind < 3; ++kind) {
						const float factor = power * perkMult(effect, kind);
						const float magnitude = flags.any(Flag::kNoMagnitude) ? 0.0f : std::round(item->effectItem.magnitude * (flags.any(Flag::kPowerAffectsMagnitude) ? factor : 1.0f));
						const float duration = flags.any(Flag::kNoDuration) ? 0.0f : std::round(static_cast<float>(item->effectItem.duration) * (flags.any(Flag::kPowerAffectsDuration) ? factor : 1.0f));
						// The gold the game puts on it
						const float value = effect->data.baseCost * std::max(std::pow(magnitude, 1.1f), 1.0f) * (duration > 0.0f ? std::pow(duration / 10.0f, 1.1f) : 1.0f);
						out.magnitude[kind] = static_cast<std::int32_t>(magnitude);
						out.duration[kind] = static_cast<std::int32_t>(std::max(duration, 0.0f));
						out.value[kind] = static_cast<std::int32_t>(std::floor(value));
					}
				}
			}
			return data;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Ingredient Page ) ------------
		//---------------------------------------------------

		// Each ingredient's four effects, and each effect's ingredients on a tab of their own.
		// Effects not learnt yet stay unnamed unless the player asks for them
		Page BuildIngredientPage(IDataProvider& a_provider) {

			using namespace CFramework_Ingredients;

			const auto setting = [](const char* a_name, bool a_default) {
				const auto* variable = CSettings::SettingsAPI::Get(a_name);
				return variable && variable->IsBool() ? variable->GetBool() : a_default;
			};

			Page page{ "ingredients", "UI_Ingredients", {} };
			if (!setting("IngredientsShow", true) || Ingredients.empty()) {
				return page;
			}
			const bool named = setting("IngredientsNameUnknown", false);
			page.tabs = { { "UI_Ingredients_TabIngredients" }, { "UI_Ingredients_TabEffects" }, { "UI_Ingredients_TabMixing", PageLayout::kMixing } };

			std::unordered_map<const RE::TESBoundObject*, std::int32_t> carried;
			if (auto* player = RE::PlayerCharacter::GetSingleton()) {
				for (const auto& [item, count] : player->GetInventoryCounts()) {
					if (count > 0 && item && item->GetFormType() == RE::FormType::Ingredient) {
						carried.emplace(item, count);
					}
				}
			}
			const auto carriedOf = [&](const Ingredient& a_ingredient) {
				const auto it = carried.find(a_ingredient.form);
				return it != carried.end() ? it->second : 0;
			};

			// Every effect, and the ingredients that carry it with where it is on each
			struct Holder
			{
				const Ingredient* ingredient;
				std::uint32_t     index;
			};
			std::vector<const RE::EffectSetting*>                              effects;
			std::unordered_map<const RE::EffectSetting*, std::vector<Holder>> holders;
			for (const auto& ingredient : Ingredients) {
				for (std::uint32_t i = 0; i < ingredient.form->effects.size(); ++i) {
					const auto* effect = ingredient.form->effects[i] ? ingredient.form->effects[i]->baseEffect : nullptr;
					if (!effect) {
						continue;
					}
					auto& list = holders[effect];
					if (list.empty()) {
						effects.push_back(effect);
					}
					list.push_back({ std::addressof(ingredient), i });
				}
			}
			const auto effectName = [](const RE::EffectSetting* a_effect) {
				const auto* name = a_effect->GetFullName();
				return std::string(name ? name : "");
			};
			// Ours where we wrote some, else where it comes from
			const auto loreOf = [&](const std::string& a_key, const std::string& a_source) {
				if (!a_key.empty()) {
					if (auto lore = a_provider.Translate(a_key); lore != a_key) {
						return lore;
					}
				}
				return a_provider.Translate("UI_Ingredients_LoreMod{" + a_provider.Translate(a_source) + "}");
			};
			const auto weight = [](float a_weight) { return fmt::format("{:g}", std::round(a_weight * 100.0f) / 100.0f); };

			//---------------------------------------------------
			// Ingredients, by the game, DLC or Creation that adds them
			//---------------------------------------------------

			const auto sectionFor = [&](const std::string& a_source) -> Section& {
				const auto it = std::ranges::find(page.sections, a_source, &Section::title);
				return it != page.sections.end() ? *it : page.sections.emplace_back(Section{ a_source, {}, 0 });
			};

			for (const auto& ingredient : Ingredients) {
				auto*      form = ingredient.form;
				const auto count = static_cast<std::uint32_t>(form->effects.size());
				const auto inPack = carriedOf(ingredient);

				std::uint32_t knownCount = 0;
				for (std::uint32_t i = 0; i < count; ++i) {
					knownCount += CHandler::Knows(form, i) ? 1 : 0;
				}

				auto& entry = sectionFor(ingredient.source).entries.emplace_back();
				entry.name = form->GetFullName();
				entry.state = knownCount == count ? EntryState::kFound : EntryState::kNeeded;
				entry.formID = form->GetFormID();

				auto& info = entry.ingredient;
				info.summary = a_provider.Translate("UI_Ingredients_Known{" + std::to_string(knownCount) + "}{" + std::to_string(count) + "}");
				info.facts.push_back(a_provider.Translate("UI_Ingredients_Value{" + std::to_string(form->value) + "}{" + weight(form->weight) + "}"));
				info.facts.push_back(inPack > 0 ? a_provider.Translate("UI_Ingredients_Carried{" + std::to_string(inPack) + "}") : a_provider.Translate("UI_Ingredients_NoneCarried"));
				info.lore = loreOf(ingredient.key.empty() ? std::string{} : "UI_Ingredients_Lore_" + ingredient.key, ingredient.source);
				entry.description = info.lore;

				for (std::uint32_t i = 0; i < count; ++i) {
					const auto* effect = form->effects[i] ? form->effects[i]->baseEffect : nullptr;
					auto&       card = info.cards.emplace_back();
					card.known = CHandler::Knows(form, i);
					card.name = effect && (card.known || named) ? effectName(effect) : a_provider.Translate("UI_Ingredients_NotKnown");
					if (!effect) {
						continue;
					}
					if (card.known) {
						card.hint = a_provider.Translate(effect->IsHostile() ? "UI_Ingredients_Poison" : "UI_Ingredients_Potion");
						continue;
					}
					if (i == 0) {
						// Eating one teaches the first
						card.hint = a_provider.Translate("UI_Ingredients_HintEat");
						continue;
					}

					// Brewing it with another that has it teaches it on both: the one carried most
					const Holder* best = nullptr;
					std::string   others;
					std::int32_t  othersCount = 0;
					for (const auto& holder : holders[effect]) {
						if (holder.ingredient == std::addressof(ingredient)) {
							continue;
						}
						if (!best || carriedOf(*holder.ingredient) > carriedOf(*best->ingredient)) {
							best = std::addressof(holder);
						}
						if (othersCount++ < 2) {
							others += (others.empty() ? "" : ", ") + std::string(holder.ingredient->form->GetFullName());
						}
					}
					if (!best) {
						card.hint = a_provider.Translate("UI_Ingredients_HintOnly");
					} else if (const auto with = carriedOf(*best->ingredient); with > 0) {
						card.hint = a_provider.Translate("UI_Ingredients_HintBrew{" + std::string(best->ingredient->form->GetFullName()) + "}{" + std::to_string(with) + "}");
					} else if (named) {
						card.hint = a_provider.Translate("UI_Ingredients_HintShared{" + others + "}");
					} else {
						card.hint = a_provider.Translate("UI_Ingredients_HintNone");
					}
				}
			}

			// The game and its DLC in release order, then the Creations by title
			const auto rank = [](const Section& a_section) {
				constexpr std::string_view order[] = { "UI_Ingredients_Skyrim", "UI_Ingredients_Dawnguard", "UI_Ingredients_Hearthfire", "UI_Ingredients_Dragonborn" };
				const auto it = std::ranges::find(order, a_section.title);
				return std::pair(static_cast<int>(it - std::begin(order)), it == std::end(order) ? a_section.title : std::string{});
			};
			std::ranges::stable_sort(page.sections, {}, rank);

			//---------------------------------------------------
			// Effects, potions then poisons; the first tab counts
			//---------------------------------------------------

			std::ranges::sort(effects, [&](const RE::EffectSetting* a_left, const RE::EffectSetting* a_right) { return effectName(a_left) < effectName(a_right); });
			Section potions{ "UI_Ingredients_Potions", {}, 1 };
			Section poisons{ "UI_Ingredients_Poisons", {}, 1 };
			potions.counted = poisons.counted = false;
			for (const auto* effect : effects) {
				const auto&  list = holders[effect];
				std::int32_t knownOn = 0;
				for (const auto& holder : list) {
					knownOn += CHandler::Knows(holder.ingredient->form, holder.index) ? 1 : 0;
				}
				if (knownOn == 0 && !named) {
					continue;
				}

				const bool hostile = effect->IsHostile();
				auto&      entry = (hostile ? poisons : potions).entries.emplace_back();
				entry.name = effectName(effect);
				entry.detail = fmt::format("{} / {}", knownOn, list.size());
				entry.state = knownOn == static_cast<std::int32_t>(list.size()) ? EntryState::kFound : EntryState::kNeeded;
				entry.formID = effect->GetFormID();

				auto& info = entry.ingredient;
				info.effect = true;
				info.summary = a_provider.Translate("UI_Ingredients_KnownOn{" + std::to_string(knownOn) + "}{" + std::to_string(list.size()) + "}");
				info.facts.push_back(a_provider.Translate(hostile ? "UI_Ingredients_Poison" : "UI_Ingredients_Potion"));
				const auto  key = CHandler::EffectKey(effect);
				const auto* file = effect->GetFile(0);
				info.lore = loreOf(key.empty() ? std::string{} : "UI_Ingredients_Effect_" + key, file ? std::string(file->GetFilename()) : std::string{});
				entry.description = info.lore;

				// Learnt first, then the ones carried
				std::vector<const Holder*> sorted;
				for (const auto& holder : list) {
					sorted.push_back(std::addressof(holder));
				}
				const auto order = [&](const Holder* a_holder) { return std::tuple(!CHandler::Knows(a_holder->ingredient->form, a_holder->index), -carriedOf(*a_holder->ingredient)); };
				std::ranges::stable_sort(sorted, {}, order);
				for (const auto* holder : sorted) {
					auto&      card = info.cards.emplace_back();
					const auto inPack = carriedOf(*holder->ingredient);
					card.known = CHandler::Knows(holder->ingredient->form, holder->index);
					card.name = card.known || named ? std::string(holder->ingredient->form->GetFullName()) : a_provider.Translate("UI_Ingredients_NotLearnt");
					card.hint = !card.known && !named ? std::string{}
					          : inPack > 0           ? a_provider.Translate("UI_Ingredients_Carried{" + std::to_string(inPack) + "}")
					                                 : a_provider.Translate("UI_Ingredients_NoneCarried");
				}
			}
			for (auto* section : { &potions, &poisons }) {
				if (!section->entries.empty()) {
					page.sections.push_back(std::move(*section));
				}
			}
			page.mixing = BuildMixingData(a_provider, [&](const RE::IngredientItem* a_form) {
				const auto it = carried.find(a_form);
				return it != carried.end() ? it->second : 0;
			});
			return page;
		}

		//---------------------------------------------------
		//-- Game Data ( Build Achievement Page ) -----------
		//---------------------------------------------------

		// The account's achievements from Steam or GOG, so the same on every save, with this
		// save's progress where the game counts it. Nothing when the store can't be read
		Page BuildAchievementPage(IDataProvider& a_provider, const std::unordered_map<std::string, Page>& a_pages) {

			using namespace CFramework_Achievements;

			const auto setting = [](const char* a_name) {
				const auto* variable = CSettings::SettingsAPI::Get(a_name);
				return !variable ? 1 : variable->IsBool() ? (variable->GetBool() ? 1 : 0) : variable->GetSInt();
			};

			Page page{ "achievements", "UI_Achievements", {} };
			if (!setting("AchievementsShow")) {
				return page;
			}

			// With no answer from a store the page is this save's only, and says why
			std::vector<Achievement> achievements;
			const bool               fromStore = CHandler::Refresh(achievements);
			if (!fromStore) {
				page.hint = CHandler::WhyNot();
				for (std::int32_t i = 1; i <= Count; ++i) {
					achievements.push_back({ i });
				}
			} else if (!CHandler::CanUnlock()) {
				page.hint = "UI_Achievements_Disabled";
			}

			const auto gameOf = [](std::int32_t a_number) {
				return a_number <= 50 ? "UI_Achievements_Skyrim" : a_number <= 60 ? "UI_Achievements_Dawnguard" : a_number <= 65 ? "UI_Achievements_Hearthfire" : "UI_Achievements_Dragonborn";
			};
			const auto pageOf = [](std::int32_t a_number) -> std::string {
				if (a_number <= 8) { return "quest:0"; }
				if (a_number <= 11) { return "quest:22"; }
				if (a_number <= 14) { return "quest:21"; }
				if (a_number <= 17) { return "quest:25"; }
				if (a_number <= 20) { return "quest:23"; }
				if (a_number <= 23) { return "quest:1"; }
				if (a_number >= 51 && a_number <= 54) { return "quest:2"; }
				if (a_number >= 66 && a_number <= 69) { return "quest:3"; }
				switch (a_number) {
				case 24: return "quest:28";
				case 29:
				case 30: return "$MCMPageDoomstone";
				case 31:
				case 62:
				case 63:
				case 64:
				case 65:
				case 74: return "$MCMPageHouses";
				case 38: return "$MCMPageTrainers";
				case 43:
				case 44:
				case 60:
				case 73: return "$MCMPageDragons";
				case 45:
				case 46:
				case 55:
				case 70: return "$MCMPageShouts";
				default: return {};
				}
			};
			constexpr std::int32_t missable[] = { 17, 18, 19, 20, 22, 42, 56, 57, 72 };

			const auto date = [](std::int64_t a_time) {
				const auto time = static_cast<std::time_t>(a_time);
				std::tm    local{};
				localtime_s(&local, &time);
				char text[32];
				std::strftime(text, sizeof(text), "%d/%m/%Y", &local);
				return std::string(text);
			};

			const auto statValue = [&](std::string_view a_stat) {
				if (a_stat.starts_with('#')) { return CHandler::Value(a_stat); }
				std::int32_t total = 0;
				while (!a_stat.empty()) {
					const auto plus = a_stat.find('+');
					const auto value = CHandler::Stat(a_stat.substr(0, plus));
					if (value < 0) { return -1; }
					total += value;
					a_stat = plus == std::string_view::npos ? std::string_view{} : a_stat.substr(plus + 1);
				}
				return total;
			};

			const auto store = a_provider.Translate(!fromStore ? "UI_Achievements_SaveOnly" : CHandler::StoreInUse() == Store::kSteam ? "UI_Achievements_Steam" : "UI_Achievements_GOG");
			for (const auto& achievement : achievements) {
				const auto  number = std::to_string(achievement.number);
				const auto* title = gameOf(achievement.number);
				auto        section = std::ranges::find(page.sections, std::string(title), &Section::title);
				if (section == page.sections.end()) {
					section = page.sections.insert(page.sections.end(), Section{ title, {} });
				}

				auto& entry = section->entries.emplace_back();
				entry.name = achievement.name.empty() ? a_provider.Translate("UI_Achievements_Name_" + number) : achievement.name;
				entry.key = "achievement:" + number;
				entry.state = achievement.unlocked ? EntryState::kFound : EntryState::kNeeded;

				auto& info = entry.achievement;
				info.game = a_provider.Translate(title) + "  \xC2\xB7  " + store;
				info.goal = achievement.description.empty() ? a_provider.Translate("UI_Achievements_Goal_" + number) : achievement.description;
				info.icon = achievement.icon;
				if (achievement.unlocked) {
					info.unlocked = achievement.time ? a_provider.Translate("UI_Achievements_Unlocked{" + date(achievement.time) + "}") : a_provider.Translate("UI_Achievements_UnlockedUndated");
				}
				if (setting("AchievementsTips")) {
					info.tip = a_provider.Translate("UI_Achievements_Tip_" + number);
				}
				if (setting("AchievementsMissable")) {
					info.missable = std::ranges::find(missable, achievement.number) != std::end(missable);
				}
				if (setting("AchievementsRarity") && achievement.percent >= 0.0f) {
					info.rarity = a_provider.Translate("UI_Achievements_Rarity{" + fmt::format("{:.1f}", achievement.percent) + "}");
				}
				if (const auto link = pageOf(achievement.number); !link.empty()) {
					if (const auto it = a_pages.find(link); it != a_pages.end()) {
						info.page = link;
						info.pageTitle = a_provider.Translate(it->second.title);
					}
				}

				// Without a store this is all there is to go on, so it shows whatever the setting says
				std::size_t expected = 0;
				bool        allMet = true;
				if (setting("AchievementsProgress") || !fromStore) {
					for (const auto& goal : CHandler::Goals()) {
						expected += goal.number == achievement.number ? 1 : 0;
						const auto value = goal.number == achievement.number ? statValue(goal.stat) : -1;
						if (value >= 0) {
							info.progress.push_back({ a_provider.Translate(std::string("UI_Achievements_Stat_") + goal.label), value, goal.target, goal.flag });
							allMet = allMet && value >= goal.target;
						}
					}
				}

				// Done on this save: every one of its goals is met, and every one could be told
				if (!fromStore) {
					const bool done = expected > 0 && info.progress.size() == expected && allMet;
					entry.state = done ? EntryState::kFound : EntryState::kNeeded;
					info.unlocked = done ? a_provider.Translate("UI_Achievements_DoneOnSave") : std::string{};
					info.locked = a_provider.Translate("UI_Achievements_NotDoneOnSave");
				}

				entry.detail = achievement.unlocked && achievement.time ? date(achievement.time) : std::string{};
				if (entry.state != EntryState::kFound && info.progress.size() == 1 && !info.progress.front().flag) {
					entry.detail = fmt::format("{} / {}", std::min(info.progress.front().value, info.progress.front().target), info.progress.front().target);
				}
				entry.description = info.tip;
			}
			return page;
		}

		//---------------------------------------------------
		//-- Game Data ( Summarise ) ------------------------
		//---------------------------------------------------

		PageSummary Summarise(const Page& a_page) {

			PageSummary summary{ a_page.id, a_page.title };
			for (const auto& section : a_page.sections) {
				if (!section.counted) {
					continue;
				}
				summary.found += section.Found();
				summary.total += section.Total();
			}
			return summary;
		}
	}

	//---------------------------------------------------
	//-- Game Data ( Snapshot, game thread ) ------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game Data ( Rebuild ) --------------------------
	//---------------------------------------------------

	void GameDataProvider::Rebuild() {

		{
			std::scoped_lock lock(_translationLock);
			if (!_translationsLoaded) {
				LoadTranslations();
			}
		}

		std::vector<Category> categories{
			{ "UI_Category_Collections", {} },
			{ "UI_Category_Locations", {} },
			{ "UI_Category_Patches", {} },
		};
		std::unordered_map<std::string, Page> pages;

		for (const auto& framework : FrameworkPages()) {
			Page page{ framework.id, framework.id, {} };
			for (const auto& [id, header] : framework.sections) {
				AddFrameworkSection(page, id, header, *this);
			}
			if (page.sections.empty()) {
				continue;
			}

			auto& category = framework.category == std::string_view("UI_Category_Locations") ? categories[1] : categories[0];
			category.pages.push_back(Summarise(page));
			pages.emplace(page.id, std::move(page));
		}

		// Skill trainers, after the MCM's own collections
		if (auto page = BuildTrainerPage(*this); !page.sections.empty()) {
			categories[0].pages.push_back(Summarise(page));
			pages.emplace(page.id, std::move(page));
		}
		if (auto page = BuildDragonPage(*this); !page.sections.empty()) {
			categories[0].pages.push_back(Summarise(page));
			pages.emplace(page.id, std::move(page));
		}
		if (auto page = BuildIngredientPage(*this); !page.sections.empty()) {
			categories[0].pages.push_back(Summarise(page));
			pages.emplace(page.id, std::move(page));
		}

		// Vanilla patches first, then mod ones
		for (const bool vanilla : { true, false }) {
			std::string header;
			for (const auto& [pageName, patch] : CHCMHandler::CustomPatches) {
				if (!patch || patch->isVanillaTracking != vanilla) {
					continue;
				}

				Page page{ std::string(patchPrefix) + pageName, pageName, {} };
				AddPatchSections(page, *patch, *this);
				if (page.sections.empty()) {
					continue;
				}

				const auto wanted = patch->headerName.empty() ? std::string(vanilla ? "$HeaderO" : "$HeaderU") : patch->headerName;
				if (wanted != header) {
					categories.push_back({ wanted, vanilla ? "UI_Category_Collections" : "UI_Category_Patches", {} });
					header = wanted;
				}
				categories.back().pages.push_back(Summarise(page));
				pages.emplace(page.id, std::move(page));
			}
		}

		const auto addQuestPage = [&](std::int32_t a_id, const std::string& a_title) {
			if (a_id < 0 || categories.back().group != "UI_Category_Quests") {
				return;
			}
			auto page = BuildQuestPage(a_id, a_title, *this);
			if (!page.sections.empty() || !page.notice.empty()) {
				categories.back().pages.push_back(Summarise(page));
				pages.emplace(page.id, std::move(page));
			}
		};
		const auto addQuestHeader = [&](std::string a_header) {
			categories.push_back({ std::move(a_header), "UI_Category_Quests", {} });
		};

		for (const auto& [pageKey, plugin, id] : CHCMHandler::MainMCMPagesDefs) {
			if (!DKUtil::string::is_empty(plugin) && !Serialization::CompletionistData::IsModInstalled(plugin)) {
				continue;
			}
			if (std::string_view(pageKey).starts_with("$Header")) {
				addQuestHeader(pageKey);
			} else {
				addQuestPage(id, pageKey);
			}
		}

		// Last, so it can point at the quest pages
		if (auto page = BuildAchievementPage(*this, pages); !page.sections.empty()) {
			categories.push_back({ "UI_Category_Achievements", {}, { Summarise(page) } });
			pages.emplace(page.id, std::move(page));
		}
		CFramework_Achievements::CHandler::QueryStats([this]() { Rebuild(); });

		std::erase_if(categories, [](const Category& a_category) { return a_category.pages.empty(); });

		auto general = ReadGeneralSettings();
		auto recent = ReadRecentEvents(12);
		auto settingsPages = _settingsBuilder.Build();
		RebuildKills();

		{
			std::scoped_lock lock(_lock);
			_categories = std::move(categories);
			_pages = std::move(pages);
			_general = std::move(general);
			_recent = std::move(recent);
			_settingsPages = std::move(settingsPages);
		}
		RebuildPins();
		++_revision;
	}

	//---------------------------------------------------
	//-- Game Data ( On Game Loaded ) -------------------
	//---------------------------------------------------

	// The pins come from the save, the overlay shows them without the window opening
	void GameDataProvider::OnGameLoaded() {

		RebuildPins();
		RescanCell();
		++_revision;
	}

	//---------------------------------------------------
	//-- Game Data ( Rebuild Settings ) -----------------
	//---------------------------------------------------

	void GameDataProvider::RebuildSettings() {

		RebuildPins();
		auto general = ReadGeneralSettings();
		auto settingsPages = _settingsBuilder.Build();

		std::scoped_lock lock(_lock);
		_general = std::move(general);
		_settingsPages = std::move(settingsPages);
		++_revision;
	}

	//---------------------------------------------------
	//-- Game Data ( Getters, render thread ) -----------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game Data ( Get Categories ) -------------------
	//---------------------------------------------------

	std::vector<Category> GameDataProvider::GetCategories() {

		std::scoped_lock lock(_lock);
		return _categories;
	}

	//---------------------------------------------------
	//-- Game Data ( Get Page ) -------------------------
	//---------------------------------------------------

	Page GameDataProvider::GetPage(std::string_view a_id) {

		std::scoped_lock lock(_lock);
		const auto       it = _pages.find(std::string(a_id));
		return it != _pages.end() ? it->second : Page{ std::string(a_id), std::string(a_id), {} };
	}

	//---------------------------------------------------
	//-- Game Data ( Set Completed ) --------------------
	//---------------------------------------------------

	void GameDataProvider::SetCompleted(std::string_view a_pageID, const Entry& a_entry, bool a_completed) {

		if (!a_entry.canToggle) {
			return;
		}

		// Trainers and dragons keep their own found list
		if (a_entry.frameworkID == CFramework_Trainers::FrameworkID || a_entry.frameworkID == CFramework_Dragons::FrameworkID) {
			{
				std::scoped_lock lock(_lock);
				if (auto it = _pages.find(std::string(a_pageID)); it != _pages.end()) {
					for (auto& section : it->second.sections) {
						for (auto& entry : section.entries) {
							if (entry.formID == a_entry.formID) {
								entry.state = a_completed ? EntryState::kFound : EntryState::kNeeded;
							}
						}
					}
				}
			}
			SKSE::GetTaskInterface()->AddTask([this, formID = a_entry.formID, a_completed, trainer = a_entry.frameworkID == CFramework_Trainers::FrameworkID]() {
				auto& found = CFramework_Master::FoundItemData_NoShow;
				a_completed ? found.AddForm(formID) : found.RemoveForm(formID);
				INFO("{}: marked {:08X} {} by hand", trainer ? "Trainers" : "Dragons", formID, a_completed ? "done" : "not done");
				Rebuild();
			});
			return;
		}

		// Collections and patches, as the MCM's SetOptionCompleted
		if (a_entry.frameworkID >= 0) {
			{
				std::scoped_lock lock(_lock);
				if (auto it = _pages.find(std::string(a_pageID)); it != _pages.end()) {
					for (auto& section : it->second.sections) {
						for (auto& entry : section.entries) {
							if (entry.formID == a_entry.formID && entry.frameworkID == a_entry.frameworkID) {
								entry.state = a_completed ? EntryState::kFound : EntryState::kNeeded;
							}
						}
					}
				}
			}
			SKSE::GetTaskInterface()->AddTask([this, id = a_entry.frameworkID, formID = a_entry.formID, a_completed]() {
				CFramework_Master::FrameworkAPI::SetOptionCompleted(id, RE::TESForm::LookupByID(formID), a_completed);
				Rebuild();
			});
			return;
		}

		// Quest pages and patch quests
		if (a_entry.questPage < 0 || a_entry.key.empty()) {
			return;
		}
		const auto id = a_entry.questPage;

		{
			std::scoped_lock lock(_lock);
			if (auto it = _pages.find(std::string(a_pageID)); it != _pages.end()) {
				for (auto& section : it->second.sections) {
					for (auto& entry : section.entries) {
						if (entry.key == a_entry.key) {
							entry.state = a_completed ? EntryState::kFound : EntryState::kNeeded;
							entry.status = a_completed ? "UI_QuestState_Done" : "UI_QuestState_Pending";
							entry.marked = a_completed;
							// Radiant quests keep their count there
							if (entry.detail.empty() || entry.detail == Translate("UI_QuestMarked")) {
								entry.detail = a_completed ? Translate("UI_QuestMarked") : std::string{};
							}
						}
					}
				}
			}
		}

		SKSE::GetTaskInterface()->AddTask([this, id, key = a_entry.key, a_completed]() {
			if (CQuestMaster::QuestAPI::qIsOptionToggled(nullptr, id, key) != a_completed) {
				CQuestMaster::QuestAPI::qSetOptionCompleted(nullptr, id, key, a_completed);
			}
			Rebuild();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Set Page Choice ) ------------------
	//---------------------------------------------------

	void GameDataProvider::SetPageChoice(std::string_view a_pageID, std::int32_t a_choice) {

		if (!a_pageID.starts_with(patchPrefix)) {
			return;
		}

		{
			std::scoped_lock lock(_lock);
			if (auto it = _pages.find(std::string(a_pageID)); it != _pages.end()) {
				it->second.choice.selected = a_choice;
			}
		}
		SKSE::GetTaskInterface()->AddTask([this, pageName = std::string(a_pageID.substr(patchPrefix.size())), a_choice]() {
			CHCMHandler::MCMAPI::SetCurrentUserOption(nullptr, pageName, a_choice);
			Rebuild();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Get Recent Events ) ----------------
	//---------------------------------------------------

	std::vector<RecentEvent> GameDataProvider::GetRecentEvents() {

		std::scoped_lock lock(_lock);
		return _recent;
	}

	//---------------------------------------------------
	//-- Game Data ( Rebuild Kills ) --------------------
	//---------------------------------------------------

	void GameDataProvider::RebuildKills() {

		std::vector<KillRecord> kills;
		kills.reserve(CFramework_Master::PlayerKills.data.size());
		for (const auto& [name, data] : CFramework_Master::PlayerKills.data) {
			kills.push_back({ name, data.killCount, data.assistCount, data.highlight });
		}

		std::vector<KillRecord> dragons;
		for (const auto& [name, data] : CFramework_Master::PlayerDragonKills.data) {
			dragons.push_back({ name, data.killCount, data.assistCount, data.highlight });
		}

		std::scoped_lock lock(_lock);
		_kills = std::move(kills);
		_dragonKills = std::move(dragons);
	}

	//---------------------------------------------------
	//-- Game Data ( Get Kills ) ------------------------
	//---------------------------------------------------

	std::vector<KillRecord> GameDataProvider::GetKills() {

		std::scoped_lock lock(_lock);
		return _kills;
	}

	//---------------------------------------------------
	//-- Game Data ( Achievement Icon ) -----------------
	//---------------------------------------------------

	// Steam's icon as a texture, made once; Steam may still be fetching it, so nothing until then
	void* GameDataProvider::AchievementIcon(std::int32_t a_icon) {

		if (a_icon <= 0) { return nullptr; }
		if (const auto it = _icons.find(a_icon); it != _icons.end()) { return it->second; }

		std::vector<std::uint8_t> pixels;
		std::uint32_t             width = 0;
		std::uint32_t             height = 0;
		const auto*               data = RE::BSGraphics::Renderer::GetRendererDataSingleton();
		auto*                     device = data ? reinterpret_cast<ID3D11Device*>(data->forwarder) : nullptr;
		if (!device || !CFramework_Achievements::CHandler::IconPixels(a_icon, pixels, width, height)) { return nullptr; }

		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		const D3D11_SUBRESOURCE_DATA initial{ pixels.data(), width * 4, 0 };

		ID3D11Texture2D*          texture = nullptr;
		ID3D11ShaderResourceView* view = nullptr;
		if (SUCCEEDED(device->CreateTexture2D(&desc, &initial, &texture))) {
			device->CreateShaderResourceView(texture, nullptr, &view);
			texture->Release();
		}
		_icons.emplace(a_icon, view);
		return view;
	}

	//---------------------------------------------------
	//-- Game Data ( Get Dragon Kills ) -----------------
	//---------------------------------------------------

	std::vector<KillRecord> GameDataProvider::GetDragonKills() {

		std::scoped_lock lock(_lock);
		return _dragonKills;
	}

	//---------------------------------------------------
	//-- Game Data ( Reset Dragon Kills ) ---------------
	//---------------------------------------------------

	// The named dragons go by these too, so the page is rebuilt with them
	void GameDataProvider::ResetDragonKills(std::string_view a_name, bool a_remove) {

		SKSE::GetTaskInterface()->AddTask([this, name = std::string(a_name), a_remove]() {
			if (a_remove) {
				CFramework_Master::PlayerDragonKills.RemoveEnemy(name);
			} else {
				CFramework_Master::PlayerDragonKills.ResetCount(name);
			}
			Rebuild();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Reset Kills ) ----------------------
	//---------------------------------------------------

	void GameDataProvider::ResetKills(std::string_view a_name, bool a_remove) {

		SKSE::GetTaskInterface()->AddTask([this, name = std::string(a_name), a_remove]() {
			if (a_remove) {
				CFramework_Master::PlayerKills.RemoveEnemy(name);
			} else {
				CFramework_Master::PlayerKills.ResetCount(name);
			}
			RebuildKills();
			++_revision;
		});
	}

	//---------------------------------------------------
	//-- Item pinning -----------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game Data ( Page Listing ) ---------------------
	//---------------------------------------------------

	std::string GameDataProvider::PageListing(std::uint32_t a_formID) {

		for (const auto& [id, page] : _pages) {
			for (const auto& section : page.sections) {
				for (const auto& entry : section.entries) {
					if (entry.formID == a_formID) {
						return id;
					}
				}
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Game Data ( Rebuild Pins ) ---------------------
	//---------------------------------------------------

	void GameDataProvider::RebuildPins() {

		PinList list;
		list.enabled = ReadBool("bCellScanner_Pinning_Enabled");
		for (const auto& item : Tracker::Tracked()) {
			if (item.pinned) {
				const auto* form = RE::TESForm::LookupByID(item.form);
				list.pins.insert(list.pins.begin(), { item.form, form ? std::string(form->GetName()) : item.name, {} });
			}
		}

		for (const auto ref : Tracker::Located()) {
			list.located.push_back(ref);
		}

		std::scoped_lock lock(_lock);
		for (auto& pin : list.pins) {
			pin.pageID = PageListing(pin.formID);
		}
		_pins = std::move(list);
	}

	//---------------------------------------------------
	//-- Game Data ( Get Pins ) -------------------------
	//---------------------------------------------------

	PinList GameDataProvider::GetPins() {

		std::scoped_lock lock(_lock);
		return _pins;
	}

	//---------------------------------------------------
	//-- Game Data ( Set Pin ) --------------------------
	//---------------------------------------------------

	void GameDataProvider::SetPin(const Entry& a_entry) {

		if (!a_entry.formID) {
			return;
		}
		{
			std::scoped_lock lock(_lock);
			if (!_pins.Has(a_entry.formID)) {
				_pins.pins.insert(_pins.pins.begin(), { a_entry.formID, a_entry.name, PageListing(a_entry.formID) });
			}
		}
		SKSE::GetTaskInterface()->AddTask([this, formID = a_entry.formID, name = a_entry.name]() {
			Tracker::Pin(RE::TESForm::LookupByID(formID), name);
			RebuildSettings();
		});
	}

	//---------------------------------------------------
	//-- In Current Cell --------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game Data ( Scan Cell Now ) --------------------
	//---------------------------------------------------

	void GameDataProvider::ScanCellNow() {

		_cellScanQueued = false;
		auto* player = RE::PlayerCharacter::GetSingleton();
		auto* cell = player ? player->GetParentCell() : nullptr;
		if (!cell) {
			return;
		}

		CellScan scan;
		scan.scanned = true;
		if (cell->IsInteriorCell()) {
			scan.cellName = cell->GetFullName();
		} else if (const auto* location = player->GetCurrentLocation(); location && location->GetName() && *location->GetName()) {
			scan.cellName = location->GetName();
		} else if (const auto* world = player->GetWorldspace(); world && world->GetName()) {
			scan.cellName = world->GetName();
		}

		std::vector<CellRecord> records;
		scan.excluded = SettingsPages::IsCellExcluded(cell);
		scan.interior = cell->IsInteriorCell();
		if (scan.excluded && _pruneTracking.exchange(false)) {
			Tracker::DropUnlisted({});
		}
		if (!scan.excluded) {
			std::vector<std::pair<RE::FormID, RE::FormID>> tracking;
			for (const auto& item : Tracker::Tracked()) {
				if (item.placed) {
					tracking.emplace_back(item.ref, item.form);
				}
			}
			const auto results = Completionist::CellScanner::Scan(cell, tracking);

			if (_pruneTracking.exchange(false)) {
				std::vector<std::pair<RE::FormID, RE::FormID>> listed;
				for (const auto& result : results) {
					if (const auto ref = result.ref.get(); ref && result.item) {
						listed.emplace_back(ref->GetFormID(), result.item->GetFormID());
					}
				}
				Tracker::DropUnlisted(listed);
			}

			std::vector<RE::FormID> found;
			for (const auto& result : results) {
				if (result.item) {
					found.push_back(result.item->GetFormID());
				}
			}
			Tracker::PlacePinsFound(found);

			std::vector<std::pair<RE::FormID, RE::FormID>> marked;
			for (const auto& item : Tracker::Tracked()) {
				if (!item.pinned) {
					marked.emplace_back(item.ref, item.form);
				}
			}

			for (const auto& result : results) {
				const auto ref = result.ref.get();
				if (!ref || !result.item) {
					continue;
				}
				CellRecord record;
				record.ref = result.ref;
				record.item.name = result.name;
				record.item.type = result.type;
				record.item.place = static_cast<CellPlace>(result.place);
				record.item.holder = result.place == Completionist::CellScanner::Place::kLoose ? std::string{} : std::string(ref->GetDisplayFullName());
				record.item.formID = result.item->GetFormID();
				record.item.refID = ref->GetFormID();
				record.item.stealing = result.stealing;
				record.item.marked = std::ranges::find(marked, std::pair{ ref->GetFormID(), result.item->GetFormID() }) != marked.end();
				records.push_back(std::move(record));
			}

			// Log a marker with no row, once per change
			static std::string lastMiss;
			std::string        miss;
			for (const auto& [refID, formID] : marked) {
				if (std::ranges::none_of(records, [&](const CellRecord& a_record) { return a_record.item.refID == refID && a_record.item.formID == formID; })) {
					std::string found;
					for (const auto& record : records) {
						if (record.item.refID == refID) {
							found += fmt::format(" {:08X}", record.item.formID);
						}
					}
					miss += fmt::format(" [{:08X}/{:08X}: scan has{}]", refID, formID, found.empty() ? " nothing on it" : found);
				}
			}
			if (miss != lastMiss) {
				lastMiss = miss;
				if (!miss.empty()) {
					INFO("Tracker: scan of {:08X} didn't find {} of {} markers:{}", cell->GetFormID(), std::ranges::count(miss, '['), marked.size(), miss);
				}
			}
		}

		std::vector<std::string> titles(records.size());
		{
			std::scoped_lock lock(_lock);
			for (std::size_t i = 0; i < records.size(); ++i) {
				if (const auto id = PageListing(records[i].item.formID); !id.empty()) {
					titles[i] = _pages[id].title;
				}
			}
		}
		for (std::size_t i = 0; i < records.size(); ++i) {
			records[i].item.collection = titles[i].empty() ? std::string{} : Translate(titles[i]);
		}

		std::scoped_lock lock(_lock);
		_cellRecords = std::move(records);
		_cellScan = std::move(scan);
		_scannedCell = cell->GetFormID();
		_lastCellScan = std::chrono::steady_clock::now();
	}

	//---------------------------------------------------
	//-- Game Data ( Rescan Cell ) ----------------------
	//---------------------------------------------------

	void GameDataProvider::RescanCell() {

		if (!_cellScanQueued.exchange(true)) {
			SKSE::GetTaskInterface()->AddTask([this]() { ScanCellNow(); });
		}
	}

	//---------------------------------------------------
	//-- Game Data ( Get Cell Scan ) --------------------
	//---------------------------------------------------

	CellScan GameDataProvider::GetCellScan() {

		// Rescan on cell change or every few seconds
		auto*      player = RE::PlayerCharacter::GetSingleton();
		const auto cell = player && player->GetParentCell() ? player->GetParentCell()->GetFormID() : 0;
		{
			std::scoped_lock lock(_lock);
			const bool stale = std::chrono::steady_clock::now() - _lastCellScan > std::chrono::seconds(3);
			if (cell != 0 && (cell != _scannedCell || stale)) {
				RescanCell();
			}
		}

		CellScan scan;
		{
			std::scoped_lock lock(_lock);
			scan = _cellScan;
			scan.items.reserve(_cellRecords.size());
			for (const auto& record : _cellRecords) {
				auto item = record.item;
				item.pinned = _pins.Has(item.formID);
				// 1 unit is about 1.43 cm
				if (const auto ref = record.ref.get(); ref && player) {
					const auto  from = player->GetPosition();
					const auto  to = ref->GetPosition();
					const float dx = to.x - from.x;
					const float dy = to.y - from.y;
					item.distance = std::sqrt(dx * dx + dy * dy + (to.z - from.z) * (to.z - from.z)) * 0.01428f;
					item.height = (to.z - from.z) * 0.01428f;
					float bearing = std::atan2(dx, dy) - player->GetAngleZ();
					while (bearing > std::numbers::pi_v<float>) {
						bearing -= 2.0f * std::numbers::pi_v<float>;
					}
					while (bearing < -std::numbers::pi_v<float>) {
						bearing += 2.0f * std::numbers::pi_v<float>;
					}
					item.bearing = bearing;
				}
				scan.items.push_back(std::move(item));
			}
		}
		return scan;
	}

	//---------------------------------------------------
	//-- Game Data ( Mark Cell Item ) -------------------
	//---------------------------------------------------

	void GameDataProvider::MarkCellItem(const CellItem& a_item) {

		{
			std::scoped_lock lock(_lock);
			for (auto& record : _cellRecords) {
				record.item.marked = record.item.marked || (record.item.refID == a_item.refID && record.item.formID == a_item.formID);
			}
		}
		SKSE::GetTaskInterface()->AddTask([this, item = a_item]() {
			if (!Tracker::MarkReference(RE::TESForm::LookupByID<RE::TESObjectREFR>(item.refID), RE::TESForm::LookupByID(item.formID), item.name, item.type)) {
				std::scoped_lock lock(_lock);
				for (auto& record : _cellRecords) {
					if (record.item.refID == item.refID && record.item.formID == item.formID) {
						record.item.marked = false;
					}
				}
			}
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Exclude Current Cell ) -------------
	//---------------------------------------------------

	void GameDataProvider::ExcludeCurrentCell() {

		{
			std::scoped_lock lock(_lock);
			_cellScan.excluded = true;
			_cellRecords.clear();
		}
		SKSE::GetTaskInterface()->AddTask([this]() {
			_settingsBuilder.Apply("cellwindow", SettingItem{ .id = "excludeCell" });
			RebuildSettings();
			_pruneTracking = true;
			ScanCellNow();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Exclude Cell Item ) ----------------
	//---------------------------------------------------

	void GameDataProvider::ExcludeCellItem(const CellItem& a_item) {

		{
			std::scoped_lock lock(_lock);
			std::erase_if(_cellRecords, [&](const CellRecord& a_record) { return a_record.item.refID == a_item.refID; });
		}
		SKSE::GetTaskInterface()->AddTask([this, ref = a_item.refID]() {
			_settingsBuilder.Apply("cellwindow", SettingItem{ .id = fmt::format("excludeReference:{:08X}", ref) });
			RebuildSettings();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Watch Cells ) ----------------------
	//---------------------------------------------------

	void GameDataProvider::WatchCells() {

		if (auto* events = RE::ScriptEventSourceHolder::GetSingleton()) {
			events->AddEventSink<RE::TESCellFullyLoadedEvent>(this);
		}
		if (auto* player = RE::PlayerCharacter::GetSingleton()) {
			player->AsBGSActorCellEventSource()->AddEventSink(this);
		}
		Tracker::Watch();
		SettingsPages::translate = [this](std::string_view a_key) { return Translate(a_key); };
	}

	//---------------------------------------------------
	//-- Game Data ( Process Event ) --------------------
	//---------------------------------------------------

	RE::BSEventNotifyControl GameDataProvider::ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) {

		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (a_event && player && a_event->cell && a_event->cell == player->GetParentCell()) {
			RescanCell();
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	//---------------------------------------------------
	//-- Game Data ( Process Event ) --------------------
	//---------------------------------------------------

	RE::BSEventNotifyControl GameDataProvider::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) {

		if (a_event && a_event->flags.get() == RE::BGSActorCellEvent::CellFlag::kEnter) {
			RescanCell();
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Cell Marker ) ----------------
	//---------------------------------------------------

	void GameDataProvider::ClearCellMarker(const CellItem& a_item) {

		{
			std::scoped_lock lock(_lock);
			for (auto& record : _cellRecords) {
				if (record.item.refID == a_item.refID && record.item.formID == a_item.formID) {
					record.item.marked = false;
				}
			}
		}
		SKSE::GetTaskInterface()->AddTask([ref = a_item.refID, form = a_item.formID]() { Tracker::ClearMarker(ref, form); });
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Cell Markers ) ---------------
	//---------------------------------------------------

	void GameDataProvider::ClearCellMarkers() {

		{
			std::scoped_lock lock(_lock);
			for (auto& record : _cellRecords) {
				record.item.marked = false;
			}
		}
		SKSE::GetTaskInterface()->AddTask([]() { Tracker::ClearMarkers(); });
	}

	//---------------------------------------------------
	//-- Game Data ( Set Location Marker ) --------------
	//---------------------------------------------------

	void GameDataProvider::SetLocationMarker(const Entry& a_entry) {

		if (!a_entry.canMark || !a_entry.MarkerID()) {
			return;
		}
		{
			std::scoped_lock lock(_lock);
			if (!_pins.Locates(a_entry.MarkerID())) {
				_pins.located.push_back(a_entry.MarkerID());
			}
		}
		SKSE::GetTaskInterface()->AddTask([this, formID = a_entry.MarkerID(), name = a_entry.name]() {
			Tracker::Locate(RE::TESForm::LookupByID<RE::TESObjectREFR>(formID), name);
			RebuildPins();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Location Marker ) ------------
	//---------------------------------------------------

	void GameDataProvider::ClearLocationMarker(std::uint32_t a_refID) {

		{
			std::scoped_lock lock(_lock);
			std::erase(_pins.located, a_refID);
		}
		SKSE::GetTaskInterface()->AddTask([this, a_refID]() {
			Tracker::Unlocate(a_refID);
			RebuildPins();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Pin ) ------------------------
	//---------------------------------------------------

	void GameDataProvider::ClearPin(std::uint32_t a_formID) {

		{
			std::scoped_lock lock(_lock);
			std::erase_if(_pins.pins, [&](const PinInfo& a_pin) { return a_pin.formID == a_formID; });
		}
		SKSE::GetTaskInterface()->AddTask([this, a_formID]() {
			Tracker::Unpin(a_formID);
			RebuildSettings();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Pins ) -----------------------
	//---------------------------------------------------

	void GameDataProvider::ClearPins() {

		{
			std::scoped_lock lock(_lock);
			_pins.pins.clear();
		}
		SKSE::GetTaskInterface()->AddTask([this]() {
			Tracker::UnpinAll();
			RebuildSettings();
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Get Settings Pages ) ---------------
	//---------------------------------------------------

	std::vector<SettingsPage> GameDataProvider::GetSettingsPages() {

		std::scoped_lock lock(_lock);
		std::vector<SettingsPage> pages;
		if (_general.valid) {
			pages.push_back(BuildGeneralPage(_general));
		}
		pages.insert(pages.end(), _settingsPages.begin(), _settingsPages.end());
		return pages;
	}

	//---------------------------------------------------
	//-- Game Data ( Get About ) ------------------------
	//---------------------------------------------------

	AboutInfo GameDataProvider::GetAbout() {

		std::scoped_lock lock(_lock);
		if (!_general.valid) {
			return {};
		}
		return { "$AuthorThanks", {
			"$DLLVersion{" + _general.dllVersion + "}",
			"$PatchesInstalled{" + std::to_string(_general.patchCount) + "}",
		} };
	}

	//---------------------------------------------------
	//-- Game Data ( Apply Setting ) --------------------
	//---------------------------------------------------

	void GameDataProvider::ApplySetting(std::string_view a_pageID, const SettingItem& a_item) {

		if (a_pageID != "general") {
			SKSE::GetTaskInterface()->AddTask([this, page = std::string(a_pageID), item = a_item]() {
				_settingsBuilder.Apply(page, item);
				// The achievements and ingredients pages are built from their settings
				if (page == "achievements" || page == "ingredients") {
					Rebuild();
				} else {
					RebuildSettings();
				}
				_pruneTracking = true;
				ScanCellNow();
			});
			return;
		}

		std::string  property;
		std::int32_t number = 0;
		bool         on = false;
		std::string  text;
		{
			std::scoped_lock lock(_lock);
			if (!_general.valid) {
				return;
			}

			auto& values = _general;
			if (a_item.id == "radiant") {
				values.radiantChoice = a_item.choice;
				property = "Radiant_Quests_Choice";
				number = a_item.choice;
				if (a_item.choice >= 0 && a_item.choice < static_cast<std::int32_t>(values.radiantOptions.size())) {
					text = values.radiantOptions[a_item.choice];
				}
			} else if (a_item.id == "counter") {
				if (const auto* counter = CounterFor(values)) {
					property = counter->property;
					number = std::clamp(a_item.number, 1, counter->maximum);
					values.counters[property] = number;
				}
			} else if (a_item.id == "books") {
				values.treatBooksAsItems = a_item.on;
				property = "TreatBooksAsItems";
				on = a_item.on;
			} else if (a_item.id == "dawnguard") {
				values.dawnguardChoice = a_item.choice;
				property = "DG_Faction_Choice";
				number = a_item.choice;
			} else if (a_item.id == "legacy") {
				values.legacyChoice = a_item.choice;
				property = "Legacy_Faction_Choice";
				number = a_item.choice;
			}
		}

		if (property.empty()) {
			return;
		}

		SKSE::GetTaskInterface()->AddTask([this, property, number, on, text]() {
			using CSettings::SettingsAPI;
			if (const auto* variable = SettingsAPI::Get(property); variable && variable->IsBool()) {
				SettingsAPI::SetBool(property, on);
			} else {
				SettingsAPI::SetInt(property, number);
			}
			if (property == "Radiant_Quests_Choice") {
				SettingsAPI::SetString("Radiant_Quests_String", text);
			}
			SettingsAPI::Save();
			CVariables::VariablesAPI::Update();
			if (property == "DG_Faction_Choice" || property == "Legacy_Faction_Choice") {
				Rebuild();
			}
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Translation ) ----------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game Data ( Load Translations ) ----------------
	//---------------------------------------------------

	void GameDataProvider::LoadTranslations() {

		_uiStrings = ParseModTranslations(ReadGameFile("SKSE/Plugins/CompletionistData/Translations/Completionist_Translations_UI.txt"));
		_translationsLoaded = true;
		INFO("In-game UI loaded {} window translations", _uiStrings.size());
	}

	//---------------------------------------------------
	//-- Game Data ( Translate ) ------------------------
	//---------------------------------------------------

	std::string GameDataProvider::Translate(std::string_view a_key) {

		std::scoped_lock lock(_translationLock);

		const std::string key(a_key);
		if (const auto it = _translations.find(key); it != _translations.end()) {
			return it->second;
		}

		const auto lookup = [&](const std::string& a_lookupKey) -> std::string {
			if (!a_lookupKey.starts_with('$')) {
				return CLocalisation::LocalisationAPI::GetLocStringByKey(a_lookupKey.c_str());
			}
			const auto it = _uiStrings.find(a_lookupKey);
			return it != _uiStrings.end() ? it->second : a_lookupKey;
		};

		const auto result = FormatInterfaceString(key, lookup);

		_translations.emplace(key, result);
		return result;
	}
}
