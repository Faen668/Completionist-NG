#include "UI/GameDataProvider.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Misc/CFramework_Locations.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/Localisation.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/Settings.hpp"
#include "Internal Utility/Variables.hpp"
#include "Internal Utility/WordsOfPower.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "UI/Core/TranslationFile.hpp"

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
		//-- Game Data ( Split Font Tags ) ------------------
		//---------------------------------------------------

		// Strip the MCM's font tags, the window draws the words itself
		void SplitFontTags(Entry& a_entry) {

			const auto& name = a_entry.name;
			const auto  first = name.find("<font");
			if (first == std::string::npos) {
				return;
			}

			std::uint32_t learnt = 0;
			const auto&   learntText = CVariables::V_ShoutColour_Found;
			if (const auto hash = learntText.find('#'); hash != std::string::npos) {
				std::from_chars(learntText.data() + hash + 1, learntText.data() + learntText.size(), learnt, 16);
			}

			std::vector<EntryWord> words;
			for (auto pos = first; (pos = name.find("<font", pos)) != std::string::npos;) {
				const auto tagEnd = name.find('>', pos);
				const auto close = tagEnd == std::string::npos ? std::string::npos : name.find("</font>", tagEnd);
				if (close == std::string::npos) {
					break;
				}
				EntryWord word;
				word.text = name.substr(tagEnd + 1, close - tagEnd - 1);
				if (const auto hash = name.find('#', pos); hash != std::string::npos && hash < tagEnd) {
					std::uint32_t rgb = 0;
					if (std::from_chars(name.data() + hash + 1, name.data() + std::min(hash + 7, tagEnd), rgb, 16).ec == std::errc{}) {
						word.done = rgb == learnt;
					}
				}
				words.push_back(std::move(word));
				pos = close + 7;
			}

			auto plain = name.substr(0, first);
			if (const auto bracket = plain.rfind('('); bracket != std::string::npos) {
				plain.erase(bracket);
			}
			while (!plain.empty() && plain.back() == ' ') {
				plain.pop_back();
			}
			a_entry.name = std::move(plain);
			a_entry.words = std::move(words);
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
				SplitFontTags(entry);
				entry.state = i < bools.size() && bools[i] ? EntryState::kFound : EntryState::kNeeded;
				entry.formID = i < forms.size() && forms[i] ? forms[i]->GetFormID() : 0;
				entry.canPin = i < forms.size() && Completionist::CellScanner::IsItemPinnable(nullptr, forms[i]);
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

		void AddPatchSections(Page& a_page, const CMiscPatch& a_patch, IDataProvider& a_provider) {

			// MCM headers end in ": "
			const auto clean = [](std::string a_text) {
				while (!a_text.empty() && (a_text.back() == ' ' || a_text.back() == ':')) {
					a_text.pop_back();
				}
				return a_text;
			};

			// Sections kept together by the patch's sub-page
			std::set<std::int32_t> subPages;
			for (const auto& data : a_patch.type_sections) {
				if (!data.IsEmpty() && data.enabled && data.type != CMiscPatchType::kQuests) {
					subPages.insert(data.displayOnPage);
				}
			}

			for (const auto subPage : subPages)
			for (const auto& data : a_patch.type_sections) {
				if (data.IsEmpty() || !data.enabled || data.type == CMiscPatchType::kQuests || data.displayOnPage != subPage) {
					continue;
				}

				const auto& names = data.type == CMiscPatchType::kShouts ? data.shout_names : data.names;
				auto&       section = a_page.sections.emplace_back();
				section.title = clean(data.pageheaderL);
				for (std::size_t i = 0; i < names.size(); ++i) {
					auto& entry = section.entries.emplace_back();
					entry.name = names[i];
					SplitFontTags(entry);
					entry.state = i < data.bools.size() && data.bools[i] ? EntryState::kFound : EntryState::kNeeded;
					entry.formID = i < data.forms.size() && data.forms[i] ? data.forms[i]->GetFormID() : 0;
					entry.canPin = i < data.forms.size() && Completionist::CellScanner::IsItemPinnable(nullptr, data.forms[i]);
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
		//-- Game Data ( Find Value ) -----------------------
		//---------------------------------------------------

		RE::BSScript::Variable* FindValue(const ScriptObjectPtr& a_script, const char* a_name) {

			if (!a_script) {
				return nullptr;
			}
			if (auto* property = a_script->GetProperty(a_name)) {
				return property;
			}
			return ScriptObject::GetVariable(a_script, a_name);
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
			values.debug = ReadBool("bDebug");

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
			left.push_back({ .id = "debug", .kind = SettingKind::kToggle, .label = "$State_DebuggingText", .info = "$State_DebuggingInfo", .on = a_values.debug });

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
		//-- Game Data ( Build Quest Page ) -----------------
		//---------------------------------------------------

		Page BuildQuestPage(std::int32_t a_id, const std::string& a_title, GameDataProvider& a_provider) {

			using CQuestMaster::QuestAPI;

			Page    page{ std::string(questPrefix) + std::to_string(a_id), a_title, {}, PageLayout::kJournal };
			Section main{ "UI_QuestSection_Main", {} };
			Section side{ "UI_QuestSection_Side", {} };
			Section radiant{ "UI_QuestSection_Radiant", {} };
			Section completed{ "UI_QuestSection_Completed", {} };

			const auto filter = QuestPageFilterFor(a_id, a_title);
			if (!filter.notice.empty()) {
				page.notice = filter.notice;
				return page;
			}

			std::size_t index = 0;
			for (const auto& [data, name, id, key] : CQuestMaster::CQuestDataVec) {
				if (id != a_id || !data || !data->GetQuest()) {
					continue;
				}
				if (const auto at = index++; filter.shows && !filter.shows(at, data->editor_id)) {
					continue;
				}

				const auto status = QuestAPI::qIsOptionCompleted(nullptr, id, key);
				const bool manual = QuestAPI::qIsOptionToggled(nullptr, id, key);

				Entry entry;
				entry.name = data->GetName();
				entry.description = CleanQuestText(data->GetHighlight());
				entry.state = status > 0 || manual ? EntryState::kFound : EntryState::kNeeded;
				entry.canToggle = status != 2;
				entry.key = key;
				entry.status = entry.IsComplete() ? "UI_QuestState_Done" : "UI_QuestState_Pending";
				entry.type = data->GetType() == std::to_underlying(CFlagEnum::kMain) ? "UI_QuestType_Main" : data->GetType() == std::to_underlying(CFlagEnum::kSide) ? "UI_QuestType_Side" : "UI_QuestType_Radiant";
				if (data->HasRadiantData()) {
					entry.detail = fmt::format("{} / {}", CFramework_Master::RadiantCountData.GetCount(key), data->GetRadiantTimesRequired());
				} else if (manual) {
					entry.detail = a_provider.Translate("UI_QuestMarked");
				}

				if (entry.IsComplete()) {
					completed.entries.push_back(std::move(entry));
				} else if (data->GetType() == std::to_underlying(CFlagEnum::kMain)) {
					main.entries.push_back(std::move(entry));
				} else if (data->GetType() == std::to_underlying(CFlagEnum::kSide)) {
					side.entries.push_back(std::move(entry));
				} else {
					radiant.entries.push_back(std::move(entry));
				}
			}

			for (auto* section : { &main, &side, &radiant, &completed }) {
				if (!section->entries.empty()) {
					page.sections.push_back(std::move(*section));
				}
			}
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
		//-- Game Data ( Summarise ) ------------------------
		//---------------------------------------------------

		PageSummary Summarise(const Page& a_page) {

			PageSummary summary{ a_page.id, a_page.title };
			for (const auto& section : a_page.sections) {
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
		if (!CHCMHandler::MainPatchesMCMPagesDefs.empty()) {
			addQuestHeader("$HeaderU");
			for (const auto& [pageKey, plugin, id] : CHCMHandler::MainPatchesMCMPagesDefs) {
				addQuestPage(static_cast<std::int32_t>(id), pageKey);
			}
		}

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

		// Quests only for now
		if (!a_pageID.starts_with(questPrefix) || a_entry.key.empty() || !a_entry.canToggle) {
			return;
		}

		std::int32_t id = 0;
		const auto   idText = a_pageID.substr(questPrefix.size());
		std::from_chars(idText.data(), idText.data() + idText.size(), id);

		{
			std::scoped_lock lock(_lock);
			if (auto it = _pages.find(std::string(a_pageID)); it != _pages.end()) {
				for (auto& section : it->second.sections) {
					for (auto& entry : section.entries) {
						if (entry.key == a_entry.key) {
							entry.state = a_completed ? EntryState::kFound : EntryState::kNeeded;
							entry.status = a_completed ? "UI_QuestState_Done" : "UI_QuestState_Pending";
							entry.detail = a_completed ? Translate("UI_QuestMarked") : std::string{};
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

		std::scoped_lock lock(_lock);
		_kills = std::move(kills);
	}

	//---------------------------------------------------
	//-- Game Data ( Get Kills ) ------------------------
	//---------------------------------------------------

	std::vector<KillRecord> GameDataProvider::GetKills() {

		std::scoped_lock lock(_lock);
		return _kills;
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
		for (const auto& item : SettingsPages::Tracked()) {
			if (item.pinned) {
				const auto* form = RE::TESForm::LookupByID(item.form);
				list.pins.insert(list.pins.begin(), { item.form, form ? std::string(form->GetName()) : item.name, {} });
			}
		}

		_locationMarker = SettingsPages::LocationMarker();

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
			SettingsPages::Pin(RE::TESForm::LookupByID(formID), name);
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
			SettingsPages::DropUnlisted({});
		}
		if (!scan.excluded) {
			std::vector<std::pair<RE::FormID, RE::FormID>> tracking;
			for (const auto& item : SettingsPages::Tracked()) {
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
				SettingsPages::DropUnlisted(listed);
			}

			std::vector<RE::FormID> found;
			for (const auto& result : results) {
				if (result.item) {
					found.push_back(result.item->GetFormID());
				}
			}
			SettingsPages::PlacePinsFound(found);

			std::vector<std::pair<RE::FormID, RE::FormID>> marked;
			for (const auto& item : SettingsPages::Tracked()) {
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
			if (!SettingsPages::MarkReference(RE::TESForm::LookupByID<RE::TESObjectREFR>(item.refID), RE::TESForm::LookupByID(item.formID), item.name, item.type)) {
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
		SettingsPages::WatchTargets();
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
		SKSE::GetTaskInterface()->AddTask([ref = a_item.refID, form = a_item.formID]() { SettingsPages::ClearMarker(ref, form); });
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
		SKSE::GetTaskInterface()->AddTask([]() { SettingsPages::ClearMarkers(); });
	}

	//---------------------------------------------------
	//-- Game Data ( Set Location Marker ) --------------
	//---------------------------------------------------

	void GameDataProvider::SetLocationMarker(const Entry& a_entry) {

		if (!a_entry.canMark || !a_entry.formID) {
			return;
		}
		_locationMarker = a_entry.formID;
		SKSE::GetTaskInterface()->AddTask([formID = a_entry.formID, name = a_entry.name]() {
			SettingsPages::PlaceLocationMarker(RE::TESForm::LookupByID<RE::TESObjectREFR>(formID), name);
		});
	}

	//---------------------------------------------------
	//-- Game Data ( Clear Location Marker ) ------------
	//---------------------------------------------------

	void GameDataProvider::ClearLocationMarker() {

		_locationMarker = 0;
		SKSE::GetTaskInterface()->AddTask([]() { SettingsPages::ClearLocationMarker(false); });
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
			SettingsPages::Unpin(a_formID);
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
			SettingsPages::UnpinAll();
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
				RebuildSettings();
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
			} else if (a_item.id == "debug") {
				values.debug = a_item.on;
				property = "bDebug";
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
