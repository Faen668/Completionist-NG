#include "UI/SettingsPages.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Forms.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/Settings.hpp"
#include "Internal Utility/Variables.hpp"
#include "UI/Core/SettingsLayout.hpp"
#include "UI/KeyNames.hpp"
#include "UI/ModelPictures.hpp"
#include "UI/Tracker.hpp"

#undef GetObject

namespace CompletionistUI
{
	namespace
	{
		using CSettings::SettingsAPI;

		class StoredValues final : public SettingValues
		{
		public:
			bool IsBool(const std::string& a_name) override {

				const auto* value = SettingsAPI::Get(a_name);
				return value && value->IsBool();
			}

			bool GetBool(const std::string& a_name) override {

				const auto* value = SettingsAPI::Get(a_name);
				return value && value->IsBool() && value->GetBool();
			}

			std::int32_t GetInt(const std::string& a_name) override {

				const auto* value = SettingsAPI::Get(a_name);
				return value && value->IsInt() ? value->GetSInt() : 0;
			}

			std::string GetString(const std::string& a_name) override {

				const auto* value = SettingsAPI::Get(a_name);
				return value && value->IsString() ? std::string(value->GetString()) : std::string{};
			}

			void SetBool(const std::string& a_name, bool a_value) override { SettingsAPI::SetBool(a_name, a_value); }
			void SetInt(const std::string& a_name, std::int32_t a_value) override { SettingsAPI::SetInt(a_name, a_value); }
			void SetString(const std::string& a_name, const std::string& a_value) override { SettingsAPI::SetString(a_name, a_value); }
			std::string KeyName(std::int32_t a_scanCode) override { return CompletionistUI::KeyName(static_cast<std::uint32_t>(a_scanCode)); }
		};

		//---------------------------------------------------
		//-- Game objects the cell scanner uses -------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Settings Pages ( Script On ) -------------------
		//---------------------------------------------------

		ScriptObjectPtr ScriptOn(RE::FormID a_quest, const char* a_script) {

			return CForms::AttachedScript(CForms::HandleFor(CForms::Get<RE::TESQuest>(a_quest)), a_script);
		}

		//---------------------------------------------------
		//-- Settings Pages ( Excluded Cells ) --------------
		//---------------------------------------------------

		RE::BGSListForm* ExcludedCells() {

			return CForms::Get<RE::BGSListForm>(CForms::ExcludedCells);
		}

		//---------------------------------------------------
		//-- Settings Pages ( Player Cell ) -----------------
		//---------------------------------------------------

		RE::TESObjectCELL* PlayerCell() {

			const auto* player = RE::PlayerCharacter::GetSingleton();
			return player ? player->GetParentCell() : nullptr;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Form ID Text ) ----------------
		//---------------------------------------------------

		std::string FormIDText(RE::FormID a_id) {

			return fmt::format("{:08X}", a_id);
		}

		//---------------------------------------------------
		//-- Settings Pages ( Parse Form ID ) ---------------
		//---------------------------------------------------

		RE::FormID ParseFormID(std::string_view a_text) {

			RE::FormID id = 0;
			std::from_chars(a_text.data(), a_text.data() + a_text.size(), id, 16);
			return id;
		}

		//---------------------------------------------------
		//-- Rows filled in at run time ---------------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Settings Pages ( Text Row ) --------------------
		//---------------------------------------------------

		SettingItem TextRow(std::string a_id, std::string a_label, std::string a_value = {}, bool a_enabled = true) {

			return { .id = std::move(a_id), .kind = SettingKind::kText, .label = std::move(a_label), .value = std::move(a_value), .enabled = a_enabled };
		}

		//---------------------------------------------------
		//-- Settings Pages ( Button Row ) ------------------
		//---------------------------------------------------

		SettingItem ButtonRow(std::string a_id, std::string a_label, std::string a_button, bool a_enabled = true) {

			return { .id = std::move(a_id), .kind = SettingKind::kButton, .label = std::move(a_label), .value = std::move(a_button), .enabled = a_enabled, .confirm = true };
		}

		struct LogView
		{
			std::vector<std::string>  dates;
			std::vector<std::string>  entries;
			std::vector<std::int32_t> colours;	// 0xRRGGBB, 0 when off
		};

		//---------------------------------------------------
		//-- Settings Pages ( Parse Hex Colour ) ------------
		//---------------------------------------------------

		std::int32_t ParseHexColour(std::string_view a_text) {

			std::int32_t colour = 0;
			if (a_text.starts_with('#')) {
				std::from_chars(a_text.data() + 1, a_text.data() + a_text.size(), colour, 16);
			}
			return colour;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Read Log ) --------------------
		//---------------------------------------------------

		LogView ReadLog(std::int32_t& a_date) {

			LogView log;
			log.dates = CFramework_Master::FrameworkAPI::GetLoggingDates(nullptr);
			if (log.dates.empty()) {
				return log;
			}
			a_date = std::clamp(a_date, 0, static_cast<std::int32_t>(log.dates.size()) - 1);
			const auto* prefix = SettingsAPI::Get("CompletionLog_PrefixEnabled");
			log.entries = CFramework_Master::FrameworkAPI::GetLoggedEventsForDate(nullptr, log.dates[a_date], prefix && prefix->GetBool());

			const auto* coloured = SettingsAPI::Get("CompletionLog_ColoursEnabled");
			const auto  text = [](const char* a_name) {
				const auto* value = SettingsAPI::Get(a_name);
				return value && value->IsString() ? std::string(value->GetString()) : std::string{};
			};
			const auto quests = text("CompletionLog_QuestsTextColourString");
			const auto items = text("CompletionLog_ItemsTextColourString");
			const auto books = text("CompletionLog_BooksTextColourString");
			const auto special = text("CompletionLog_SpecialTextColourString");
			for (const auto& entry : log.entries) {
				log.colours.push_back(coloured && coloured->GetBool() ? ParseHexColour(CFramework_Master::LoggingData.GetEntryTypeByLogPrefx(entry, quests, items, books, special)) : 0);
			}
			return log;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Add Dynamic Rows ) ------------
		//---------------------------------------------------

		void AddDynamicRows(std::string_view a_name, bool a_enabled, std::vector<SettingItem>& a_out, const LogView& a_log, std::int32_t a_logDate) {

			if (a_name == "pinnedItem") {
				const auto names = Tracker::PinnedNames();
				a_out.push_back(TextRow("pinnedItem", "$CellScanner_CurrentPin", !names.empty() && a_enabled ? names : "UI_CellScanner_NoPin", a_enabled));
			} else if (a_name == "excludedCells") {
				SettingItem list{ .id = "excludedCells", .kind = SettingKind::kList, .label = "$CellScanner_Setting_EXCL_None", .info = "$CellScanner_Setting_EXCL_Highlight", .value = "UI_Remove", .confirm = true };
				if (auto* cells = ExcludedCells()) {
					cells->ForEachForm([&](RE::TESForm* a_form) {
						if (a_form) {
							list.options.emplace_back(a_form->GetName());
							list.keys.push_back("removeCell:" + FormIDText(a_form->GetFormID()));
						}
						return RE::BSContainer::ForEachResult::kContinue;
					});
				}
				a_out.push_back(std::move(list));
			} else if (a_name == "excludedReferences") {
				SettingItem list{ .id = "excludedReferences", .kind = SettingKind::kList, .label = "$CellScanner_Setting_EXRF_None", .info = "$CellScanner_Setting_EXRF_Highlight", .value = "UI_Remove", .confirm = true };
				if (auto* cell = PlayerCell()) {
					const auto ids = Completionist::CellScanner::GetReferenceFormIDs(nullptr, cell);
					const auto names = Completionist::CellScanner::GetReferenceNames(nullptr, cell);
					const auto refs = Completionist::CellScanner::GetObjectReferences(nullptr, cell);
					const auto count = std::min({ ids.size(), names.size(), refs.size() });
					for (std::size_t i = 0; i < count; ++i) {
						if (refs[i]) {
							list.options.push_back(ids[i] + " - " + names[i]);
							list.keys.push_back("removeRef:" + FormIDText(refs[i]->GetFormID()));
						}
					}
				}
				a_out.push_back(std::move(list));
			} else if (a_name == "excludeCell") {
				auto* cell = PlayerCell();
				if (cell && cell->IsInteriorCell()) {
					const auto* list = ExcludedCells();
					a_out.push_back(ButtonRow("excludeCell", "$CellScanner_Setting_EXCL_Text{" + std::string(cell->GetName()) + "}", "UI_Exclude", !list || !list->HasForm(cell)));
				} else {
					a_out.push_back(TextRow("excludeCellNone", "$CellScanner_Setting_EXCL_TextERROR", {}, false));
				}
			} else if (a_name == "logDate") {
				if (!a_log.dates.empty()) {
					a_out.push_back({ .id = "logDate", .kind = SettingKind::kChoice, .label = "$Log_DateSelection_Text", .info = "$Log_DateSelection_Info", .choice = a_logDate, .options = a_log.dates });
				}
			} else if (a_name == "logEntries") {
				for (std::size_t i = 0; i < a_log.entries.size(); ++i) {
					const std::string_view entry = a_log.entries[i];
					const auto             split = entry.find(" ~ ");
					auto                   row = split == std::string_view::npos ? TextRow("logEntry" + std::to_string(i), {}, std::string(entry)) : TextRow("logEntry" + std::to_string(i), std::string(entry.substr(0, split)), std::string(entry.substr(split + 3)));
					row.number = i < a_log.colours.size() ? a_log.colours[i] : 0;
					a_out.push_back(std::move(row));
				}
				if (a_log.entries.empty()) {
					a_out.push_back(TextRow("logEmpty", "$EmptyLogView", {}, false));
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Settings pages ( Build, game thread ) ----------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Settings Pages ( Is Cell Excluded ) ------------
	//---------------------------------------------------

	bool SettingsPages::IsCellExcluded(RE::TESObjectCELL* a_cell) {

		const auto* list = ExcludedCells();
		return a_cell && ((list && list->HasForm(a_cell)) || Completionist::CellScanner::isCellExcluded(nullptr, a_cell));
	}

	//---------------------------------------------------
	//-- Settings Pages ( Build ) -----------------------
	//---------------------------------------------------

	std::vector<SettingsPage> SettingsPages::Build() {

		StoredValues values;
		auto*        handler = RE::TESDataHandler::GetSingleton();
		const auto   log = ReadLog(_logDate);

		std::vector<SettingsPage> pages;
		for (const auto& layout : MCMSettingsLayouts()) {
			if (!layout.plugin.empty() && (!handler || !handler->LookupLoadedModByName(layout.plugin))) {
				continue;
			}

			auto& page = pages.emplace_back(SettingsPage{ layout.id, layout.title, {}, layout.singleColumn, layout.tab });
			for (int column = 0; column < 2; ++column) {
				for (const auto& row : layout.columns[column]) {
					if (row.id.starts_with(dynamicRowPrefix)) {
						const bool enabled = row.enabledBy.empty() || values.GetBool(row.enabledBy);
						AddDynamicRows(std::string_view(row.id).substr(dynamicRowPrefix.size()), enabled, page.columns[column], log, _logDate);
					} else {
						page.columns[column].push_back(MakeSettingItem(row, values));
					}
				}
			}
		}
		return pages;
	}

	//---------------------------------------------------
	//-- Settings pages ( Apply, game thread ) ----------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Settings Pages ( Apply ) -----------------------
	//---------------------------------------------------

	void SettingsPages::Apply(std::string_view a_pageID, const SettingItem& a_item) {

		const std::string_view id = a_item.id;

		if (id == "logDate") {
			_logDate = a_item.choice;
			return;
		}
		if (id.starts_with("removeCell:")) {
			if (auto* cell = RE::TESForm::LookupByID(ParseFormID(id.substr(11)))) {
				CForms::Call(CForms::NativeObject(CForms::HandleFor(ExcludedCells()), "FormList"), "RemoveAddedForm"sv, cell);
			}
			return;
		}
		if (id.starts_with("removeRef:")) {
			if (auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(ParseFormID(id.substr(10)))) {
				Completionist::CellScanner::RemoveExcludedReference(nullptr, ref);
			}
			return;
		}
		if (id.starts_with("excludeReference:")) {
			const auto formID = ParseFormID(id.substr(17));
			if (auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(formID)) {
				Completionist::CellScanner::ExcludeReference(nullptr, ref, ref->GetParentCell());
			}
			Tracker::ClearMarker(formID);
			return;
		}
		if (id == "excludeCell") {
			auto* cell = PlayerCell();
			auto* list = ExcludedCells();
			if (cell && list && !list->HasForm(cell)) {
				CForms::Call(CForms::NativeObject(CForms::HandleFor(list), "FormList"), "AddForm"sv, static_cast<RE::TESForm*>(cell));
			}
			return;
		}

		const auto* row = FindSettingRow(a_pageID, id);
		if (!row) {
			return;
		}

		StoredValues values;
		WriteSettingItem(*row, a_item, values);

		// What the MCM's option events did
		const auto& action = row->action;
		if (action == "markerCancel") {
			Tracker::UnlocateAll();
		} else if (action == "pinningEnabled") {
			if (!a_item.on) {
				Tracker::UnpinAll();
			}
		} else if (action == "pinningMarker") {
			Tracker::ShowPinObjectives(a_item.on);
		} else if (action == "pinningEffect") {
			Tracker::ShowPinGlow(a_item.on);
		} else if (action == "clearPin") {
			Tracker::UnpinAll();
		} else if (action == "picturesPrepare" || action == "picturesRedraw") {
			ModelPictures::PrepareAll(action == "picturesRedraw");
		}

		SettingsAPI::Save();
		CVariables::VariablesAPI::Update();
	}
}
