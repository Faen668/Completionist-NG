#include "UI/SettingsPages.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Forms.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/Settings.hpp"
#include "Internal Utility/Variables.hpp"
#include "UI/Core/SettingsLayout.hpp"
#include "UI/KeyNames.hpp"

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
		//-- Settings Pages ( Targets ) ---------------------
		//---------------------------------------------------

		Serialization::CompletionistCellTargets& Targets() {

			return CFramework_Master::CellTargets;
		}

		using Tracked = Serialization::CompletionistCellTargets::Tracked;

		//---------------------------------------------------
		//-- Settings Pages ( Excluded Cells ) --------------
		//---------------------------------------------------

		RE::BGSListForm* ExcludedCells() {

			return CForms::Get<RE::BGSListForm>(CForms::ExcludedCells);
		}

		//---------------------------------------------------
		//-- The pinned item's glow and sound ---------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Settings Pages ( Stop Glow ) -------------------
		//---------------------------------------------------

		void StopGlow(RE::TESObjectREFR* a_ref) {

			auto* shader = CForms::Get<RE::TESEffectShader>(CForms::PinnedItemShader);
			auto* lists = RE::ProcessLists::GetSingleton();
			if (!a_ref || !shader || !lists) {
				return;
			}
			const auto handle = a_ref->GetHandle();
			lists->ForEachShaderEffect([&](RE::ShaderReferenceEffect* a_effect) {
				if (a_effect && a_effect->effectData == shader && a_effect->target == handle) {
					a_effect->finished = true;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		//---------------------------------------------------
		//-- Settings Pages ( Start Glow ) ------------------
		//---------------------------------------------------

		void StartGlow(RE::TESObjectREFR* a_ref) {

			auto* shader = CForms::Get<RE::TESEffectShader>(CForms::PinnedItemShader);
			if (a_ref && shader && a_ref->Is3DLoaded()) {
				StopGlow(a_ref);
				a_ref->ApplyEffectShader(shader, -1.0f);
			}
		}

		//---------------------------------------------------
		//-- Settings Pages ( Play Pin Sound ) --------------
		//---------------------------------------------------

		// Choice 0 is none
		void PlayPinSound(std::int32_t a_choice) {

			auto* list = CForms::Get<RE::BGSListForm>(CForms::PinnedItemSounds);
			if (!list || a_choice <= 0 || static_cast<std::size_t>(a_choice) >= list->forms.size()) {
				return;
			}
			auto*                   form = list->forms[a_choice];
			RE::BSISoundDescriptor* descriptor = nullptr;
			if (auto* sound = form ? form->As<RE::TESSound>() : nullptr) {
				descriptor = sound->descriptor;
			} else if (auto* soundDescriptor = form ? form->As<RE::BGSSoundDescriptorForm>() : nullptr) {
				descriptor = soundDescriptor;
			}
			auto*             audio = RE::BSAudioManager::GetSingleton();
			auto*             player = RE::PlayerCharacter::GetSingleton();
			RE::BSSoundHandle handle;
			if (descriptor && audio && audio->GetSoundHandle(handle, descriptor)) {
				if (auto* root = player ? player->Get3D() : nullptr) {
					handle.SetObjectToFollow(root);
				}
				handle.Play();
			}
		}

		//---------------------------------------------------
		//-- The tracker slots ------------------------------
		//---------------------------------------------------

		// Each slot is a copy of Completionist_Tracker, made on first use
		std::array<RE::TESQuest*, CForms::TrackerSlots> slotQuests{};

		//---------------------------------------------------
		//-- Settings Pages ( Make Slot Quest ) -------------
		//---------------------------------------------------

		RE::TESQuest* MakeSlotQuest(std::int32_t a_slot) {

			auto* source = CForms::Get<RE::TESQuest>(CForms::TrackerTemplate);
			auto* factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::TESQuest>();
			auto* handler = RE::TESDataHandler::GetSingleton();
			if (!source || !factory || !handler || source->aliases.empty() || source->objectives.empty()) {
				INFO("Tracker: can't make slot {} (template {}, factory {})", a_slot, source != nullptr, factory != nullptr);
				return nullptr;
			}
			const auto* sourceAlias = skyrim_cast<RE::BGSRefAlias*>(source->aliases[0]);
			const auto* sourceObjective = source->objectives.front();
			auto*       quest = factory->Create();
			if (!quest || !sourceAlias || !sourceObjective) {
				return nullptr;
			}
			quest->SetFormID(handler->GetNextID(), false);
			quest->data = source->data;
			quest->fullName = source->fullName;
			quest->SetFormEditorID(fmt::format("Completionist_Tracker_{:02d}", a_slot).c_str());

			auto* alias = RE::malloc<RE::BGSRefAlias>(sizeof(RE::BGSRefAlias));
			std::memcpy(static_cast<void*>(alias), sourceAlias, sizeof(RE::BGSRefAlias));
			std::construct_at(std::addressof(alias->aliasName), sourceAlias->aliasName);
			alias->owningQuest = quest;
			alias->conditions = nullptr;
			quest->aliases.push_back(alias);

			// Teleport path starts empty, never shared with the template
			auto* target = RE::malloc<RE::TESQuestTarget>(sizeof(RE::TESQuestTarget));
			std::memset(static_cast<void*>(target), 0, sizeof(RE::TESQuestTarget));
			target->alias = 0;
			auto** targets = RE::malloc<RE::TESQuestTarget*>(sizeof(RE::TESQuestTarget*));
			targets[0] = target;
			auto* objective = RE::malloc<RE::BGSQuestObjective>(sizeof(RE::BGSQuestObjective));
			std::memset(static_cast<void*>(objective), 0, sizeof(RE::BGSQuestObjective));
			std::construct_at(std::addressof(objective->displayText), sourceObjective->displayText);
			objective->ownerQuest = quest;
			objective->targets = targets;
			objective->numTargets = 1;
			objective->index = 0;
			objective->initialized = sourceObjective->initialized;
			objective->flags = sourceObjective->flags;
			quest->objectives.push_front(objective);

			bool       result = false;
			const bool started = quest->EnsureQuestStarted(result, true);
			INFO("Tracker: made slot {} as quest {:08X} (started {} {}, running {})", a_slot, quest->GetFormID(), started, result, quest->IsRunning());
			return quest;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Slot Quest ) ------------------
		//---------------------------------------------------

		RE::TESQuest* SlotQuest(std::int32_t a_slot) {

			if (a_slot < 0 || a_slot >= CForms::TrackerSlots) {
				return nullptr;
			}
			auto& quest = slotQuests[static_cast<std::size_t>(a_slot)];
			if (!quest) {
				quest = MakeSlotQuest(a_slot);
			} else if (!quest->IsRunning()) {
				bool result = false;
				quest->EnsureQuestStarted(result, true);
			}
			return quest;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Slot Alias ) ------------------
		//---------------------------------------------------

		RE::BGSRefAlias* SlotAlias(std::int32_t a_slot) {

			auto* quest = SlotQuest(a_slot);
			return quest && !quest->aliases.empty() ? skyrim_cast<RE::BGSRefAlias*>(quest->aliases[0]) : nullptr;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Show Slot Objective ) ---------
		//---------------------------------------------------

		void ShowSlotObjective(std::int32_t a_slot, bool a_displayed, bool a_force) {

			if (auto* quest = SlotQuest(a_slot)) {
				CForms::Call(CForms::NativeObject(CForms::HandleFor(quest), "Quest"), "SetObjectiveDisplayed"sv, std::int32_t{ 0 }, a_displayed, a_force);
			}
		}

		RE::BGSQuestObjective* SlotObjective(std::int32_t a_slot);

		//---------------------------------------------------
		//-- Settings Pages ( Restore Slot Objective ) ------
		//---------------------------------------------------

		// Restore without the objective added message
		void RestoreSlotObjective(std::int32_t a_slot, RE::TESObjectREFR* a_ref, bool a_active) {

			auto* quest = SlotQuest(a_slot);
			auto* objective = SlotObjective(a_slot);
			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!quest || !objective || !player) {
				INFO("Tracker: can't restore slot {} (quest {}, objective {})", a_slot, quest != nullptr, objective != nullptr);
				return;
			}
			if (a_ref) {
				quest->ForceRefIntoAlias(0, a_ref);
			}
			if (a_active) {
				quest->data.flags.set(RE::QuestFlag::kActive);
			} else {
				quest->data.flags.reset(RE::QuestFlag::kActive);
			}
			quest->data.flags.set(RE::QuestFlag::kDisplayedInHUD);
			objective->state = RE::QUEST_OBJECTIVE_STATE::kDisplayed;
			auto& objectives = player->GetPlayerRuntimeData().objectives;
			if (std::ranges::none_of(objectives, [&](const RE::BGSInstancedQuestObjective& a_entry) { return a_entry.Objective == objective; })) {
				objectives.push_back(RE::BGSInstancedQuestObjective{ objective, 0, RE::QUEST_OBJECTIVE_STATE::kDisplayed });
			}
			RE::BSSpinLockGuard lock(player->GetQuestTargetsLock());
			auto& targets = player->GetQuestTargets();
			if (!targets.contains(quest)) {
				auto* list = RE::malloc<RE::BSTArray<RE::TESQuestTarget*>>(sizeof(RE::BSTArray<RE::TESQuestTarget*>));
				std::construct_at(list);
				for (std::uint32_t i = 0; i < objective->numTargets; ++i) {
					list->push_back(objective->targets[i]);
				}
				targets.insert({ quest, list });
			}
			const auto aliased = quest->GetAliasedRef(0).get();
			INFO("Tracker: restored slot {} quietly (quest {:08X} flags {:04X}, running {}, alias on {:08X}, {} compass targets)", a_slot, quest->GetFormID(),
				quest->data.flags.underlying(), quest->IsRunning(), aliased ? aliased->GetFormID() : 0, targets.size());
		}

		//---------------------------------------------------
		//-- Settings Pages ( Clear Slot Alias ) ------------
		//---------------------------------------------------

		void ClearSlotAlias(std::int32_t a_slot) {

			if (auto* alias = SlotAlias(a_slot)) {
				CForms::Call(CForms::NativeObject(CForms::HandleFor(alias), "ReferenceAlias"), "Clear"sv);
			}
		}

		//---------------------------------------------------
		//-- Settings Pages ( Slot Reference ) --------------
		//---------------------------------------------------

		RE::TESObjectREFR* SlotReference(std::int32_t a_slot) {

			auto* alias = a_slot >= 0 && slotQuests[static_cast<std::size_t>(a_slot)] ? SlotAlias(a_slot) : nullptr;
			return alias ? alias->GetReference() : nullptr;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Slot Objective ) --------------
		//---------------------------------------------------

		RE::BGSQuestObjective* SlotObjective(std::int32_t a_slot) {

			if (auto* quest = SlotQuest(a_slot)) {
				for (auto* objective : quest->objectives) {
					if (objective && objective->index == 0) {
						return objective;
					}
				}
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Free Slot ) -------------------
		//---------------------------------------------------

		std::int32_t FreeSlot() {

			for (std::int32_t slot = 0; slot < CForms::TrackerSlots; ++slot) {
				if (std::ranges::none_of(Targets().tracked, [&](const Tracked& a_item) { return a_item.slot == slot; })) {
					return slot;
				}
			}
			return -1;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Notify Slots Full ) -----------
		//---------------------------------------------------

		void NotifySlotsFull() {

			if (SettingsPages::translate) {
				RE::SendHUDMessage::ShowHUDMessage(SettingsPages::translate("UI_Track_Full{" + std::to_string(CForms::TrackerSlots) + "}").c_str());
			}
		}

		//---------------------------------------------------
		//-- Settings Pages ( Shows Objective ) -------------
		//---------------------------------------------------

		bool ShowsObjective(const Tracked& a_item) {

			return !a_item.pinned || StoredValues{}.GetBool("bCellScanner_Pinning_Marker");
		}

		//---------------------------------------------------
		//-- Settings Pages ( Fill Slot ) -------------------
		//---------------------------------------------------

		void FillSlot(const Tracked& a_item, RE::TESObjectREFR* a_ref, bool a_announce) {

			if (auto* objective = SlotObjective(a_item.slot); objective && SettingsPages::translate) {
				objective->displayText = SettingsPages::translate(std::string(a_item.pinned ? "UI_Track_Pinned{" : "UI_Track_Marked{") + a_item.type + "}{" + a_item.name + "}");
			}
			CForms::Call(CForms::NativeObject(CForms::HandleFor(SlotAlias(a_item.slot)), "ReferenceAlias"), "ForceRefTo"sv, a_ref);
			if (ShowsObjective(a_item)) {
				if (a_announce) {
					ShowSlotObjective(a_item.slot, true, true);
				} else {
					RestoreSlotObjective(a_item.slot, a_ref, a_item.active);
				}
			}
			if (a_item.pinned && StoredValues{}.GetBool("bCellScanner_Pinning_Effect")) {
				StartGlow(a_ref);
			}
		}

		//---------------------------------------------------
		//-- Settings Pages ( Release Slot ) ----------------
		//---------------------------------------------------

		void ReleaseSlot(Tracked& a_item) {

			if (a_item.slot < 0) {
				return;
			}
			if (a_item.pinned) {
				StopGlow(SlotReference(a_item.slot));
			}
			ShowSlotObjective(a_item.slot, false, false);
			ClearSlotAlias(a_item.slot);
			a_item.slot = -1;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Place Pin ) -------------------
		//---------------------------------------------------

		bool PlacePin(Tracked& a_item, bool a_sound) {

			auto* form = RE::TESForm::LookupByID(a_item.form);
			auto* ref = form ? Completionist::CellScanner::GetPinnedReferenceRefr(nullptr, form) : nullptr;
			if (!ref) {
				return false;
			}
			const auto slot = FreeSlot();
			if (slot < 0) {
				NotifySlotsFull();
				return false;
			}
			a_item.slot = slot;
			a_item.ref = ref->GetFormID();
			a_item.name = Completionist::CellScanner::GetPinnedReferenceName(nullptr, form);
			a_item.type = Completionist::CellScanner::GetPinnedReferenceType(nullptr, form);
			FillSlot(a_item, ref, true);
			if (a_sound) {
				PlayPinSound(StoredValues{}.GetInt("CellScanner_Pinning_Sound_Choice"));
			}
			return true;
		}

		//---------------------------------------------------
		//-- Settings Pages ( Unplace ) ---------------------
		//---------------------------------------------------

		void Unplace(std::vector<Tracked>::iterator a_item, std::string_view a_why) {

			INFO("Tracker: {} {} ({}), slot {}", a_item->pinned ? "pin parked" : "marker ended", a_item->name, a_why, a_item->slot);
			ReleaseSlot(*a_item);
			if (!a_item->pinned) {
				Targets().tracked.erase(a_item);
			}
		}

		//---------------------------------------------------
		//-- The location marker ----------------------------
		//---------------------------------------------------

		std::atomic<bool> locationMarkerOut{ false };

		//---------------------------------------------------
		//-- Settings Pages ( Name Location Marker ) --------
		//---------------------------------------------------

		// Travel To <Name>
		void NameLocationMarker() {

			auto* text = CForms::Get<RE::TESObjectREFR>(CForms::LocationText);
			auto* full = text && text->GetBaseObject() ? text->GetBaseObject()->As<RE::TESFullName>() : nullptr;
			if (full) {
				full->SetFullName(Targets().location.name.c_str());
			}
			const auto force = [](std::uint32_t a_alias, RE::TESObjectREFR* a_target) {
				if (a_target) {
					CForms::Call(CForms::NativeObject(CForms::HandleFor(CForms::Alias(CForms::MarkerQuest, a_alias)), "ReferenceAlias"), "ForceRefTo"sv, a_target);
				}
			};
			force(CForms::LocationNameAlias, text);
			force(CForms::LocationAlias, CForms::Get<RE::TESObjectREFR>(CForms::LocMarker));
		}

		//---------------------------------------------------
		//-- What the targets' alias scripts did ------------
		//---------------------------------------------------

		class TargetWatch final :
			public RE::BSTEventSink<RE::TESContainerChangedEvent>,
			public RE::BSTEventSink<RE::TESCellAttachDetachEvent>
		{
		public:
			static TargetWatch* Get()
			{
				static TargetWatch watch;
				return &watch;
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override
			{
				const auto* player = RE::PlayerCharacter::GetSingleton();
				if (!a_event || !player || a_event->newContainer != player->GetFormID() || !a_event->baseObj) {
					return RE::BSEventNotifyControl::kContinue;
				}
				auto& tracked = Targets().tracked;
				for (std::size_t i = tracked.size(); i-- > 0;) {
					if (tracked[i].form == a_event->baseObj) {
						ReleaseSlot(tracked[i]);
						tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*) override
			{
				auto* ref = a_event && !a_event->attached ? a_event->reference.get() : nullptr;
				if (!ref) {
					return RE::BSEventNotifyControl::kContinue;
				}
				auto& tracked = Targets().tracked;
				for (std::size_t i = tracked.size(); i-- > 0;) {
					if (tracked[i].slot >= 0 && SlotReference(tracked[i].slot) == ref) {
						Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), fmt::format("{:08X} detached from its cell", ref->GetFormID()));
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};

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
			log.entries = CFramework_Master::FrameworkAPI::GetLoggedEventsForDate(nullptr, log.dates[a_date], prefix && prefix->GetBool(), false, "", "", "", "");
			log.entries.resize(std::min(log.entries.size(), std::size_t{ 126 }));

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
				std::string names;
				for (const auto& item : Targets().tracked) {
					if (item.pinned) {
						names += (names.empty() ? "" : ", ") + item.name;
					}
				}
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
	//-- Settings Pages ( Tracked ) ---------------------
	//---------------------------------------------------

	std::vector<SettingsPages::TrackedItem> SettingsPages::Tracked() {

		std::vector<TrackedItem> out;
		for (const auto& item : Targets().tracked) {
			out.push_back({ item.form, item.ref, item.name, item.type, item.pinned, item.slot >= 0 });
		}
		return out;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Pin ) -------------------------
	//---------------------------------------------------

	void SettingsPages::Pin(RE::TESForm* a_form, const std::string& a_name) {

		auto& tracked = Targets().tracked;
		if (!a_form || std::ranges::any_of(tracked, [&](const ::Serialization::CompletionistCellTargets::Tracked& a_item) { return a_item.pinned && a_item.form == a_form->GetFormID(); })) {
			return;
		}
		tracked.push_back({ .form = a_form->GetFormID(), .name = a_name, .pinned = true });
		PlacePin(tracked.back(), true);
	}

	//---------------------------------------------------
	//-- Settings Pages ( Unpin ) -----------------------
	//---------------------------------------------------

	void SettingsPages::Unpin(RE::FormID a_form) {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].pinned && tracked[i].form == a_form) {
				ReleaseSlot(tracked[i]);
				tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Unpin All ) -------------------
	//---------------------------------------------------

	void SettingsPages::UnpinAll() {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].pinned) {
				ReleaseSlot(tracked[i]);
				tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Place Pins Found ) ------------
	//---------------------------------------------------

	void SettingsPages::PlacePinsFound(const std::vector<RE::FormID>& a_items) {

		if (!StoredValues{}.GetBool("bCellScanner_Pinning_Enabled")) {
			return;
		}
		for (auto& item : Targets().tracked) {
			if (item.pinned && item.slot < 0 && std::ranges::find(a_items, item.form) != a_items.end() && PlacePin(item, true) && translate) {
				CFramework_Master::FrameworkAPI::SendNotification(translate("UI_Cell_PinHere{" + item.name + "}"), "bCellScanner_Pinning_Notify");
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Mark Reference ) --------------
	//---------------------------------------------------

	bool SettingsPages::MarkReference(RE::TESObjectREFR* a_ref, RE::TESForm* a_item, const std::string& a_name, const std::string& a_type) {

		auto& tracked = Targets().tracked;
		if (!a_ref) {
			return false;
		}
		const auto item = a_item ? a_item->GetFormID() : 0;
		if (std::ranges::any_of(tracked, [&](const ::Serialization::CompletionistCellTargets::Tracked& a_tracked) { return !a_tracked.pinned && a_tracked.ref == a_ref->GetFormID() && a_tracked.form == item; })) {
			return true;
		}
		const auto slot = FreeSlot();
		if (slot < 0) {
			NotifySlotsFull();
			return false;
		}
		tracked.push_back({ .form = item, .ref = a_ref->GetFormID(), .name = a_name, .type = a_type, .slot = slot });
		FillSlot(tracked.back(), a_ref, true);
		return true;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Clear Marker ) ----------------
	//---------------------------------------------------

	void SettingsPages::ClearMarker(RE::FormID a_ref, RE::FormID a_item) {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (!tracked[i].pinned && tracked[i].ref == a_ref && (a_item == 0 || tracked[i].form == a_item)) {
				Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), "cleared");
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Drop Unlisted ) ---------------
	//---------------------------------------------------

	void SettingsPages::DropUnlisted(const std::vector<std::pair<RE::FormID, RE::FormID>>& a_listed) {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].slot >= 0 && std::ranges::find(a_listed, std::pair{ tracked[i].ref, tracked[i].form }) == a_listed.end()) {
				Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), "left out by the scanner's settings");
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Clear Markers ) ---------------
	//---------------------------------------------------

	void SettingsPages::ClearMarkers() {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (!tracked[i].pinned) {
				Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), "all cleared");
			}
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Location Marker ) -------------
	//---------------------------------------------------

	RE::FormID SettingsPages::LocationMarker() {

		return Targets().location.form;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Place Location Marker ) -------
	//---------------------------------------------------

	void SettingsPages::PlaceLocationMarker(RE::TESObjectREFR* a_mapMarker, const std::string& a_name) {

		auto* marker = CForms::Get<RE::TESObjectREFR>(CForms::LocMarker);
		if (!a_mapMarker || !marker) {
			return;
		}

		CForms::ShowObjective(CForms::MarkerQuest, false, true);
		const bool item = !a_mapMarker->extraList.HasType<RE::ExtraMapMarker>();
		Targets().location = { a_mapMarker->GetFormID(), a_name, item ? "item" : "" };
		NameLocationMarker();
		marker->MoveTo(a_mapMarker);
		CForms::CompleteObjective(CForms::MarkerQuest, false);
		CForms::ShowObjective(CForms::MarkerQuest, true, true);
		locationMarkerOut = true;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Clear Location Marker ) -------
	//---------------------------------------------------

	void SettingsPages::ClearLocationMarker(bool a_arrived) {

		const auto marker = CForms::HandleFor(CForms::Get<RE::TESObjectREFR>(CForms::LocMarker));
		CForms::Call(CForms::NativeObject(marker, "ObjectReference"), "MoveToMyEditorLocation"sv);
		if (a_arrived) {
			CForms::CompleteObjective(CForms::MarkerQuest, true);
		}
		CForms::ShowObjective(CForms::MarkerQuest, false, true);
		Targets().location = {};
		locationMarkerOut = false;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Check Location Marker ) -------
	//---------------------------------------------------

	void SettingsPages::CheckLocationMarker() {

		const auto& target = Targets().location;
		if (!target.form) {
			locationMarkerOut = false;
			return;
		}

		if (target.type == "item") {
			const auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(target.form);
			if (!ref || !ref->GetParentCell() || ref->IsDisabled() || ref->IsDeleted() || CFramework_Master::FoundItemData_NoShow.HasForm(target.form)) {
				INFO("Location marker: picked up {}", target.name);
				ClearLocationMarker(true);
			}
			return;
		}

		// Map markers are all outside
		const auto* player = RE::PlayerCharacter::GetSingleton();
		const auto* marker = CForms::Get<RE::TESObjectREFR>(CForms::LocMarker);
		const auto* world = player ? player->GetWorldspace() : nullptr;
		if (!marker || !world || world != marker->GetWorldspace()) {
			return;
		}
		const auto range = static_cast<float>(StoredValues{}.GetInt("State_MarkerDetectionVal"));
		if (player->GetPosition().GetDistance(marker->GetPosition()) <= range) {
			INFO("Location marker: arrived at {}", Targets().location.name);
			ClearLocationMarker(true);
		}
	}

	//---------------------------------------------------
	//-- Settings Pages ( Restore Targets ) -------------
	//---------------------------------------------------

	void SettingsPages::RestoreTargets() {

		const auto& targets = Targets();

		// Slot quests survive loads in the same session
		for (auto& quest : slotQuests) {
			if (quest && RE::TESForm::LookupByID(quest->GetFormID()) != quest) {
				INFO("Tracker: after the load, {:08X} is gone", quest->GetFormID());
				quest = nullptr;
			}
		}
		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].slot < 0) {
				continue;
			}
			auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(tracked[i].ref);
			if (ref && ref->GetParentCell() && ref->GetParentCell()->IsAttached()) {
				INFO("Tracker: after the load, slot {} back on {:08X} ({})", tracked[i].slot, tracked[i].ref, tracked[i].name);
				FillSlot(tracked[i], ref, false);
			} else {
				INFO("Tracker: after the load, {} not put back ({})", tracked[i].name, !ref ? "reference gone" : !ref->GetParentCell() ? "no cell" : "its cell isn't loaded");
				tracked[i].slot = -1;
				if (!tracked[i].pinned) {
					tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
				}
			}
		}
		if (targets.location.form) {
			NameLocationMarker();
		}
		locationMarkerOut = targets.location.form != 0;
	}

	//---------------------------------------------------
	//-- Settings Pages ( Before Save ) -----------------
	//---------------------------------------------------

	void SettingsPages::BeforeSave() {

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player || std::ranges::none_of(slotQuests, [](const RE::TESQuest* a_quest) { return a_quest != nullptr; })) {
			return;
		}
		const auto ours = [](const RE::TESQuest* a_quest) { return a_quest && std::ranges::find(slotQuests, a_quest) != slotQuests.end(); };

		auto& objectives = player->GetPlayerRuntimeData().objectives;
		std::size_t removed = 0;
		for (auto it = objectives.begin(); it != objectives.end();) {
			if (it->Objective && ours(it->Objective->ownerQuest)) {
				it = objectives.erase(it);
				++removed;
			} else {
				++it;
			}
		}
		{
			RE::BSSpinLockGuard lock(player->GetQuestTargetsLock());
			auto& targets = player->GetQuestTargets();
			for (auto* quest : slotQuests) {
				if (quest) {
					targets.erase(quest);
				}
			}
		}
		// Hidden for the save, shown again after
		for (auto& item : Targets().tracked) {
			if (const auto* quest = item.slot >= 0 ? slotQuests[static_cast<std::size_t>(item.slot)] : nullptr) {
				item.active = quest->data.flags.any(RE::QuestFlag::kActive);
			}
		}
		for (auto* quest : slotQuests) {
			for (auto* objective : quest ? quest->objectives : RE::BSSimpleList<RE::BGSQuestObjective*>{}) {
				if (objective) {
					objective->state = RE::QUEST_OBJECTIVE_STATE::kDormant;
				}
			}
		}
		INFO("Tracker: before saving, took {} objectives off", removed);

		SKSE::GetTaskInterface()->AddTask([]() {
			for (auto& item : Targets().tracked) {
				if (item.slot < 0) {
					continue;
				}
				if (auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(item.ref)) {
					FillSlot(item, ref, false);
				}
			}
			INFO("Tracker: after saving, slots filled again");
		});
	}

	//---------------------------------------------------
	//-- Settings Pages ( Watch Targets ) ---------------
	//---------------------------------------------------

	void SettingsPages::WatchTargets() {

		if (auto* events = RE::ScriptEventSourceHolder::GetSingleton()) {
			events->AddEventSink<RE::TESContainerChangedEvent>(TargetWatch::Get());
			events->AddEventSink<RE::TESCellAttachDetachEvent>(TargetWatch::Get());
		}

		// Four times a second, as the old script polled
		static std::jthread arrival([](std::stop_token a_stop) {
			while (!a_stop.stop_requested()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				if (locationMarkerOut) {
					SKSE::GetTaskInterface()->AddTask([]() { CheckLocationMarker(); });
				}
			}
		});
	}

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
			ClearMarker(formID);
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
			ClearLocationMarker(false);
		} else if (action == "pinningEnabled") {
			if (!a_item.on) {
				UnpinAll();
			}
		} else if (action == "pinningMarker") {
			for (const auto& item : Targets().tracked) {
				if (item.pinned && item.slot >= 0) {
					ShowSlotObjective(item.slot, a_item.on, a_item.on);
				}
			}
		} else if (action == "pinningEffect") {
			for (const auto& item : Targets().tracked) {
				if (item.pinned && item.slot >= 0) {
					a_item.on ? StartGlow(SlotReference(item.slot)) : StopGlow(SlotReference(item.slot));
				}
			}
		} else if (action == "clearPin") {
			UnpinAll();
		}

		SettingsAPI::Save();
		CVariables::VariablesAPI::Update();
	}
}
