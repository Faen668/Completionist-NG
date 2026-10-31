#include "UI/Tracker.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Forms.hpp"
#include "Internal Utility/PlacedReferences.hpp"
#include "Internal Utility/Settings.hpp"
#include "UI/SettingsPages.hpp"

namespace CompletionistUI
{
	namespace
	{
		using CSettings::SettingsAPI;
		using Slotted = Serialization::CompletionistCellTargets::Tracked;
		using Kind = Serialization::CompletionistCellTargets::Kind;

		//---------------------------------------------------
		//-- Tracker ( Targets ) ----------------------------
		//---------------------------------------------------

		Serialization::CompletionistCellTargets& Targets() {

			return CFramework_Master::CellTargets;
		}

		//---------------------------------------------------
		//-- Tracker ( Setting On ) -------------------------
		//---------------------------------------------------

		bool SettingOn(const char* a_name) {

			const auto* value = SettingsAPI::Get(a_name);
			return value && value->IsBool() && value->GetBool();
		}

		//---------------------------------------------------
		//-- Tracker ( Setting Number ) ---------------------
		//---------------------------------------------------

		std::int32_t SettingNumber(const char* a_name) {

			const auto* value = SettingsAPI::Get(a_name);
			return value && value->IsInt() ? value->GetSInt() : 0;
		}

		//---------------------------------------------------
		//-- Tracker ( Translate ) --------------------------
		//---------------------------------------------------

		std::string Translate(const std::string& a_key) {

			return SettingsPages::translate ? SettingsPages::translate(a_key) : std::string{};
		}

		//---------------------------------------------------
		//-- The pinned item's glow and sound ---------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Tracker ( Stop Glow ) --------------------------
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
		//-- Tracker ( Start Glow ) -------------------------
		//---------------------------------------------------

		void StartGlow(RE::TESObjectREFR* a_ref) {

			auto* shader = CForms::Get<RE::TESEffectShader>(CForms::PinnedItemShader);
			if (a_ref && shader && a_ref->Is3DLoaded()) {
				StopGlow(a_ref);
				a_ref->ApplyEffectShader(shader, -1.0f);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Play Pin Sound ) ---------------------
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
		//-- Tracker ( Make Slot Quest ) --------------------
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
		//-- Tracker ( Slot Quest ) -------------------------
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
		//-- Tracker ( Slot Alias ) -------------------------
		//---------------------------------------------------

		RE::BGSRefAlias* SlotAlias(std::int32_t a_slot) {

			auto* quest = SlotQuest(a_slot);
			return quest && !quest->aliases.empty() ? skyrim_cast<RE::BGSRefAlias*>(quest->aliases[0]) : nullptr;
		}

		//---------------------------------------------------
		//-- Tracker ( Slot Script ) ------------------------
		//---------------------------------------------------

		ScriptObjectPtr SlotScript(std::int32_t a_slot) {

			return CForms::NativeObject(CForms::HandleFor(SlotQuest(a_slot)), "Quest");
		}

		//---------------------------------------------------
		//-- Tracker ( Show Slot Objective ) ----------------
		//---------------------------------------------------

		void ShowSlotObjective(std::int32_t a_slot, bool a_displayed, bool a_force) {

			if (SlotQuest(a_slot)) {
				CForms::Call(SlotScript(a_slot), "SetObjectiveDisplayed"sv, std::int32_t{ 0 }, a_displayed, a_force);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Complete Slot Objective ) ------------
		//---------------------------------------------------

		void CompleteSlotObjective(std::int32_t a_slot) {

			if (SlotQuest(a_slot)) {
				CForms::Call(SlotScript(a_slot), "SetObjectiveCompleted"sv, std::int32_t{ 0 }, true);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Slot Objective ) ---------------------
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
		//-- Tracker ( Restore Slot Objective ) -------------
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
		//-- Tracker ( Clear Slot Alias ) -------------------
		//---------------------------------------------------

		void ClearSlotAlias(std::int32_t a_slot) {

			if (auto* alias = SlotAlias(a_slot)) {
				CForms::Call(CForms::NativeObject(CForms::HandleFor(alias), "ReferenceAlias"), "Clear"sv);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Slot Reference ) ---------------------
		//---------------------------------------------------

		RE::TESObjectREFR* SlotReference(std::int32_t a_slot) {

			auto* alias = a_slot >= 0 && slotQuests[static_cast<std::size_t>(a_slot)] ? SlotAlias(a_slot) : nullptr;
			return alias ? alias->GetReference() : nullptr;
		}

		//---------------------------------------------------
		//-- Tracker ( Free Slot ) --------------------------
		//---------------------------------------------------

		std::int32_t FreeSlot() {

			for (std::int32_t slot = 0; slot < CForms::TrackerSlots; ++slot) {
				if (std::ranges::none_of(Targets().tracked, [&](const Slotted& a_item) { return a_item.slot == slot; })) {
					return slot;
				}
			}
			return -1;
		}

		//---------------------------------------------------
		//-- Tracker ( Notify Slots Full ) ------------------
		//---------------------------------------------------

		void NotifySlotsFull() {

			if (const auto text = Translate("UI_Track_Full{" + std::to_string(CForms::TrackerSlots) + "}"); !text.empty()) {
				RE::SendHUDMessage::ShowHUDMessage(text.c_str());
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Shows Objective ) --------------------
		//---------------------------------------------------

		bool ShowsObjective(const Slotted& a_item) {

			return !a_item.Pinned() || SettingOn("bCellScanner_Pinning_Marker");
		}

		//---------------------------------------------------
		//-- Tracker ( Place Of ) ---------------------------
		//---------------------------------------------------

		// Where the reference is now, else where it was placed
		std::string PlaceOf(RE::TESObjectREFR* a_ref) {

			for (const auto* location : { a_ref->GetCurrentLocation(), a_ref->GetEditorLocation() }) {
				if (location && location->GetName() && *location->GetName()) {
					return location->GetName();
				}
			}
			const auto* cell = a_ref->GetParentCell();
			return cell && cell->GetFullName() && *cell->GetFullName() ? cell->GetFullName() : std::string{};
		}

		//---------------------------------------------------
		//-- Tracker ( Objective Text ) ---------------------
		//---------------------------------------------------

		std::string ObjectiveText(const Slotted& a_item, RE::TESObjectREFR* a_ref) {

			switch (a_item.kind) {
			case Kind::kTravel:
				return Translate("UI_Track_Travel{" + a_item.name + "}");
			case Kind::kFind:
				if (!a_item.type.empty()) {
					return Translate(a_item.type + "{" + a_item.name + "}");
				}
				// A temporary reference's own place, not that of the marker near it; none when its name says it
				if (const auto place = PlacedReferences::Find(a_item.ref) ? PlacedReferences::PlaceName(a_item.ref) : PlaceOf(a_ref); !place.empty() && a_item.name.find(place) == std::string::npos) {
					return Translate("UI_Track_FindIn{" + a_item.name + "}{" + place + "}");
				}
				return Translate("UI_Track_Find{" + a_item.name + "}");
			default:
				return Translate(std::string(a_item.Pinned() ? "UI_Track_Pinned{" : "UI_Track_Marked{") + a_item.type + "}{" + a_item.name + "}");
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Fill Slot ) --------------------------
		//---------------------------------------------------

		void FillSlot(const Slotted& a_item, RE::TESObjectREFR* a_ref, bool a_announce) {

			if (auto* objective = SlotObjective(a_item.slot); objective && SettingsPages::translate) {
				objective->displayText = ObjectiveText(a_item, a_ref);
			}
			CForms::Call(CForms::NativeObject(CForms::HandleFor(SlotAlias(a_item.slot)), "ReferenceAlias"), "ForceRefTo"sv, a_ref);
			if (ShowsObjective(a_item)) {
				if (a_announce) {
					ShowSlotObjective(a_item.slot, true, true);
				} else {
					RestoreSlotObjective(a_item.slot, a_ref, a_item.active);
				}
			}
			if (a_item.Pinned() && SettingOn("bCellScanner_Pinning_Effect")) {
				StartGlow(a_ref);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Release Slot ) -----------------------
		//---------------------------------------------------

		void ReleaseSlot(Slotted& a_item) {

			if (a_item.slot < 0) {
				return;
			}
			if (a_item.Pinned()) {
				StopGlow(SlotReference(a_item.slot));
			}
			ShowSlotObjective(a_item.slot, false, false);
			ClearSlotAlias(a_item.slot);
			a_item.slot = -1;
		}

		//---------------------------------------------------
		//-- Tracker ( Place Pin ) --------------------------
		//---------------------------------------------------

		bool PlacePin(Slotted& a_item, bool a_sound) {

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
				PlayPinSound(SettingNumber("CellScanner_Pinning_Sound_Choice"));
			}
			return true;
		}

		//---------------------------------------------------
		//-- Tracker ( Unplace ) ----------------------------
		//---------------------------------------------------

		// Pins are parked for the next cell they're in, the rest are dropped
		void Unplace(std::vector<Slotted>::iterator a_item, std::string_view a_why, bool a_done = false) {

			INFO("Tracker: {} {} ({}), slot {}", a_item->Pinned() ? "pin parked" : "marker ended", a_item->name, a_why, a_item->slot);
			if (a_done && a_item->slot >= 0) {
				CompleteSlotObjective(a_item->slot);
			}
			ReleaseSlot(*a_item);
			if (!a_item->Pinned()) {
				Targets().tracked.erase(a_item);
			}
		}

		//---------------------------------------------------
		//-- Tracker ( Unplace Where ) ----------------------
		//---------------------------------------------------

		template <class Pred>
		void UnplaceWhere(Pred a_pred, std::string_view a_why) {

			auto& tracked = Targets().tracked;
			for (std::size_t i = tracked.size(); i-- > 0;) {
				if (a_pred(tracked[i])) {
					Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), a_why);
				}
			}
		}

		//---------------------------------------------------
		//-- Pages' NPCs and places -------------------------
		//---------------------------------------------------

		// Polled while any are out
		std::atomic<bool> locating{ false };

		//---------------------------------------------------
		//-- Tracker ( Update Locating ) --------------------
		//---------------------------------------------------

		void UpdateLocating() {

			locating = std::ranges::any_of(Targets().tracked, [](const Slotted& a_item) { return a_item.Located() && a_item.slot >= 0; });
		}

		//---------------------------------------------------
		//-- Tracker ( Gone ) -------------------------------
		//---------------------------------------------------

		bool Gone(const RE::TESObjectREFR* a_ref) {

			return !a_ref || a_ref->IsDisabled() || a_ref->IsDeleted();
		}

		//---------------------------------------------------
		//-- Tracker ( Reached ) ----------------------------
		//---------------------------------------------------

		// Map markers are all outside, NPCs can be anywhere
		bool Reached(const RE::PlayerCharacter* a_player, RE::TESObjectREFR* a_ref) {

			if (a_ref->Is(RE::FormType::ActorCharacter) && !a_ref->Is3DLoaded()) {
				return false;
			}
			const auto* cell = a_ref->GetParentCell();
			const bool  together = cell && cell->IsInteriorCell() ? a_player->GetParentCell() == cell : a_player->GetWorldspace() && a_player->GetWorldspace() == a_ref->GetWorldspace();
			return together && a_player->GetPosition().GetDistance(a_ref->GetPosition()) <= static_cast<float>(SettingNumber("State_MarkerDetectionVal"));
		}

		//---------------------------------------------------
		//-- Tracker ( Check Located ) ----------------------
		//---------------------------------------------------

		void CheckLocated() {

			const auto* player = RE::PlayerCharacter::GetSingleton();
			auto&       tracked = Targets().tracked;
			for (std::size_t i = tracked.size(); player && i-- > 0;) {
				const auto& item = tracked[i];
				if (!item.Located() || item.slot < 0) {
					continue;
				}
				auto*       ref = PlacedReferences::Target(item.ref);
				const auto* actor = ref ? ref->As<RE::Actor>() : nullptr;
				const auto  at = tracked.begin() + static_cast<std::ptrdiff_t>(i);
				// Some map markers stay disabled until a quest shows them, items are done once picked up
				if (!ref || (item.kind == Kind::kFind && (Gone(ref) || (actor && actor->IsDead())))) {
					Unplace(at, actor ? "dead or gone" : "gone", !actor && ref);
				} else if (!actor && item.kind == Kind::kFind && CFramework_Master::FoundItemData_NoShow.HasForm(item.ref)) {
					Unplace(at, "found", true);
				} else if ((actor || item.kind == Kind::kTravel) && Reached(player, ref)) {
					Unplace(at, "reached", true);
				} else if (PlacedReferences::Find(item.ref) && SlotReference(item.slot) != ref) {
					INFO("Tracker: {} now points at {:08X}", item.name, ref->GetFormID());
					FillSlot(item, ref, false);	// its cell loaded, from the marker near it to the reference
				}
			}
			UpdateLocating();
		}

		//---------------------------------------------------
		//-- What happened to the targets -------------------
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
						if (tracked[i].Located() && tracked[i].slot >= 0) {
							CompleteSlotObjective(tracked[i].slot);
						}
						ReleaseSlot(tracked[i]);
						tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
					}
				}
				UpdateLocating();
				return RE::BSEventNotifyControl::kContinue;
			}

			// Pins and marks are for the loaded cells
			RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*) override
			{
				auto* ref = a_event && !a_event->attached ? a_event->reference.get() : nullptr;
				if (!ref) {
					return RE::BSEventNotifyControl::kContinue;
				}
				auto& tracked = Targets().tracked;
				for (std::size_t i = tracked.size(); i-- > 0;) {
					if (!tracked[i].Located() && tracked[i].slot >= 0 && SlotReference(tracked[i].slot) == ref) {
						Unplace(tracked.begin() + static_cast<std::ptrdiff_t>(i), fmt::format("{:08X} detached from its cell", ref->GetFormID()));
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};
	}

	//---------------------------------------------------
	//-- Tracker ( Tracked ) ----------------------------
	//---------------------------------------------------

	std::vector<Tracker::TrackedItem> Tracker::Tracked() {

		std::vector<TrackedItem> out;
		for (const auto& item : Targets().tracked) {
			if (!item.Located()) {
				out.push_back({ item.form, item.ref, item.name, item.type, item.Pinned(), item.slot >= 0 });
			}
		}
		return out;
	}

	//---------------------------------------------------
	//-- Tracker ( Pinned Names ) -----------------------
	//---------------------------------------------------

	std::string Tracker::PinnedNames() {

		std::string names;
		for (const auto& item : Targets().tracked) {
			if (item.Pinned()) {
				names += (names.empty() ? "" : ", ") + item.name;
			}
		}
		return names;
	}

	//---------------------------------------------------
	//-- Tracker ( Pin ) --------------------------------
	//---------------------------------------------------

	void Tracker::Pin(RE::TESForm* a_form, const std::string& a_name) {

		auto& tracked = Targets().tracked;
		if (!a_form || std::ranges::any_of(tracked, [&](const Slotted& a_item) { return a_item.Pinned() && a_item.form == a_form->GetFormID(); })) {
			return;
		}
		tracked.push_back({ .form = a_form->GetFormID(), .name = a_name, .kind = Kind::kPin });
		PlacePin(tracked.back(), true);
	}

	//---------------------------------------------------
	//-- Tracker ( Unpin ) ------------------------------
	//---------------------------------------------------

	void Tracker::Unpin(RE::FormID a_form) {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].Pinned() && tracked[i].form == a_form) {
				ReleaseSlot(tracked[i]);
				tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
			}
		}
	}

	//---------------------------------------------------
	//-- Tracker ( Unpin All ) --------------------------
	//---------------------------------------------------

	void Tracker::UnpinAll() {

		auto& tracked = Targets().tracked;
		for (std::size_t i = tracked.size(); i-- > 0;) {
			if (tracked[i].Pinned()) {
				ReleaseSlot(tracked[i]);
				tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
			}
		}
	}

	//---------------------------------------------------
	//-- Tracker ( Place Pins Found ) -------------------
	//---------------------------------------------------

	void Tracker::PlacePinsFound(const std::vector<RE::FormID>& a_items) {

		if (!SettingOn("bCellScanner_Pinning_Enabled")) {
			return;
		}
		for (auto& item : Targets().tracked) {
			if (item.Pinned() && item.slot < 0 && std::ranges::find(a_items, item.form) != a_items.end() && PlacePin(item, true)) {
				CFramework_Master::FrameworkAPI::SendNotification(Translate("UI_Cell_PinHere{" + item.name + "}"), "bCellScanner_Pinning_Notify");
			}
		}
	}

	//---------------------------------------------------
	//-- Tracker ( Show Pin Objectives ) ----------------
	//---------------------------------------------------

	void Tracker::ShowPinObjectives(bool a_shown) {

		for (const auto& item : Targets().tracked) {
			if (item.Pinned() && item.slot >= 0) {
				ShowSlotObjective(item.slot, a_shown, a_shown);
			}
		}
	}

	//---------------------------------------------------
	//-- Tracker ( Show Pin Glow ) ----------------------
	//---------------------------------------------------

	void Tracker::ShowPinGlow(bool a_shown) {

		for (const auto& item : Targets().tracked) {
			if (item.Pinned() && item.slot >= 0) {
				a_shown ? StartGlow(SlotReference(item.slot)) : StopGlow(SlotReference(item.slot));
			}
		}
	}

	//---------------------------------------------------
	//-- Tracker ( Mark Reference ) ---------------------
	//---------------------------------------------------

	bool Tracker::MarkReference(RE::TESObjectREFR* a_ref, RE::TESForm* a_item, const std::string& a_name, const std::string& a_type) {

		auto& tracked = Targets().tracked;
		if (!a_ref) {
			return false;
		}
		const auto item = a_item ? a_item->GetFormID() : 0;
		if (std::ranges::any_of(tracked, [&](const Slotted& a_tracked) { return a_tracked.kind == Kind::kMark && a_tracked.ref == a_ref->GetFormID() && a_tracked.form == item; })) {
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
	//-- Tracker ( Clear Marker ) -----------------------
	//---------------------------------------------------

	void Tracker::ClearMarker(RE::FormID a_ref, RE::FormID a_item) {

		UnplaceWhere([&](const Slotted& a_tracked) { return a_tracked.kind == Kind::kMark && a_tracked.ref == a_ref && (a_item == 0 || a_tracked.form == a_item); }, "cleared");
	}

	//---------------------------------------------------
	//-- Tracker ( Drop Unlisted ) ----------------------
	//---------------------------------------------------

	void Tracker::DropUnlisted(const std::vector<std::pair<RE::FormID, RE::FormID>>& a_listed) {

		UnplaceWhere([&](const Slotted& a_tracked) {
			return !a_tracked.Located() && a_tracked.slot >= 0 && std::ranges::find(a_listed, std::pair{ a_tracked.ref, a_tracked.form }) == a_listed.end();
		}, "left out by the scanner's settings");
	}

	//---------------------------------------------------
	//-- Tracker ( Clear Markers ) ----------------------
	//---------------------------------------------------

	void Tracker::ClearMarkers() {

		UnplaceWhere([](const Slotted& a_tracked) { return a_tracked.kind == Kind::kMark; }, "all cleared");
	}

	//---------------------------------------------------
	//-- Tracker ( Located ) ----------------------------
	//---------------------------------------------------

	std::vector<RE::FormID> Tracker::Located() {

		std::vector<RE::FormID> out;
		for (const auto& item : Targets().tracked) {
			if (item.Located()) {
				out.push_back(item.ref);
			}
		}
		return out;
	}

	//---------------------------------------------------
	//-- Tracker ( Locate ) -----------------------------
	//---------------------------------------------------

	// The alias holds the NPC or map marker itself, so the marker follows them about
	bool Tracker::Locate(RE::TESObjectREFR* a_ref, const std::string& a_name, bool a_announce, const std::string& a_text) {

		auto& tracked = Targets().tracked;
		if (!a_ref || !a_ref->GetBaseObject()) {
			return false;
		}
		if (std::ranges::any_of(tracked, [&](const Slotted& a_item) { return a_item.Located() && a_item.ref == a_ref->GetFormID(); })) {
			return true;
		}
		const auto slot = FreeSlot();
		if (slot < 0) {
			NotifySlotsFull();
			return false;
		}
		// A temporary reference far away is found by way of a marker near it, until its cell loads
		const bool standIn = PlacedReferences::Find(a_ref->GetFormID()) == a_ref;
		auto*      target = standIn ? PlacedReferences::Target(a_ref->GetFormID()) : a_ref;
		if (!target) {
			return false;
		}
		const bool travel = !standIn && a_ref->extraList.HasType<RE::ExtraMapMarker>();
		tracked.push_back({ .form = travel ? a_ref->GetFormID() : a_ref->GetBaseObject()->GetFormID(),
			.ref = a_ref->GetFormID(),
			.name = a_name,
			.type = a_text,
			.kind = travel ? Kind::kTravel : Kind::kFind,
			.slot = slot });
		FillSlot(tracked.back(), target, a_announce);
		INFO("Tracker: {} {} ({:08X}) in slot {}", travel ? "travel to" : "find", a_name, a_ref->GetFormID(), slot);
		locating = true;
		return true;
	}

	//---------------------------------------------------
	//-- Tracker ( Unlocate ) ---------------------------
	//---------------------------------------------------

	void Tracker::Unlocate(RE::FormID a_ref) {

		UnplaceWhere([&](const Slotted& a_item) { return a_item.Located() && a_item.ref == a_ref; }, "cleared");
		UpdateLocating();
	}

	//---------------------------------------------------
	//-- Tracker ( Unlocate All ) -----------------------
	//---------------------------------------------------

	void Tracker::UnlocateAll() {

		UnplaceWhere([](const Slotted& a_item) { return a_item.Located(); }, "all cleared");
		UpdateLocating();
	}

	//---------------------------------------------------
	//-- Tracker ( Restore ) ----------------------------
	//---------------------------------------------------

	void Tracker::Restore() {

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
			auto*       ref = tracked[i].Located() ? PlacedReferences::Target(tracked[i].ref) : RE::TESForm::LookupByID<RE::TESObjectREFR>(tracked[i].ref);
			const auto* cell = ref ? ref->GetParentCell() : nullptr;
			const bool  back = tracked[i].kind == Kind::kTravel ? ref != nullptr : tracked[i].Located() ? !Gone(ref) : cell && cell->IsAttached();
			if (back) {
				INFO("Tracker: after the load, slot {} back on {:08X} ({})", tracked[i].slot, tracked[i].ref, tracked[i].name);
				FillSlot(tracked[i], ref, false);
			} else {
				INFO("Tracker: after the load, {} not put back ({})", tracked[i].name, !ref ? "reference gone" : tracked[i].Located() ? "disabled" : !cell ? "no cell" : "its cell isn't loaded");
				tracked[i].slot = -1;
				if (!tracked[i].Pinned()) {
					tracked.erase(tracked.begin() + static_cast<std::ptrdiff_t>(i));
				}
			}
		}

		// Older saves had one location marker on Completionist_Marker
		if (auto& old = Targets().location; old.form) {
			CForms::ShowObjective(CForms::OldMarkerQuest, false, true);
			Locate(RE::TESForm::LookupByID<RE::TESObjectREFR>(old.form), old.name, false);
			INFO("Tracker: moved the old location marker for {} to a slot", old.name);
			old = {};
		}
		UpdateLocating();
	}

	//---------------------------------------------------
	//-- Tracker ( Before Save ) ------------------------
	//---------------------------------------------------

	void Tracker::BeforeSave() {

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player || std::ranges::none_of(slotQuests, [](const RE::TESQuest* a_quest) { return a_quest != nullptr; })) {
			return;
		}
		const auto ours = [](const RE::TESQuest* a_quest) { return a_quest && std::ranges::find(slotQuests, a_quest) != slotQuests.end(); };

		auto&       objectives = player->GetPlayerRuntimeData().objectives;
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
			auto&               targets = player->GetQuestTargets();
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
	//-- Tracker ( Watch ) ------------------------------
	//---------------------------------------------------

	void Tracker::Watch() {

		if (auto* events = RE::ScriptEventSourceHolder::GetSingleton()) {
			events->AddEventSink<RE::TESContainerChangedEvent>(TargetWatch::Get());
			events->AddEventSink<RE::TESCellAttachDetachEvent>(TargetWatch::Get());
		}

		// Four times a second, as the old script polled
		static std::jthread arrival([](std::stop_token a_stop) {
			while (!a_stop.stop_requested()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				if (locating) {
					SKSE::GetTaskInterface()->AddTask([]() { CheckLocated(); });
				}
			}
		});
	}
}
