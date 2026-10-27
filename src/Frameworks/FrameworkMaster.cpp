#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/Misc/CFramework_Enchantments.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "Internal Utility/Events.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "UI/GameUI.hpp"

#undef GetObject	// windows.h macro

namespace CFramework_Master 
{
	using namespace CVariables;
	using namespace ArrayHolder;
	using namespace Serialization;
	using namespace CExternalPatchHandler;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register()
	{
		_OnMapMarkerAdded = CEvents::EventHandler::RegisterMapMarkerAddedHook(&OnMapMarkerAdded);
		_OnMapMarkerDiscovered = CEvents::EventHandler::RegisterMapMarkerDiscoveredHook(&OnMapMarkerDiscovered);
		CEvents::EventHandler::RegisterForEvent_OnDeathEvent(OnDeath);
		CEvents::EventHandler::RegisterForEvent_OnHitEvent(OnHit);

		WatchFishing();

		SetSerializableInfo(FoundItemData);
		SetSerializableInfo(FoundItemData_NoShow);
		SetSerializableInfo(CQuestKeys_Natural);
		SetSerializableInfo(CQuestKeys_Manual);
		SetSerializableInfo(LoggingData);
		SetSerializableInfo(RadiantCountData);
		SetSerializableInfo(ExcludedCellScannerRefs);
		SetSerializableInfo(PatchSettings);
		SetSerializableInfo(PlayerKills);
		SetSerializableInfo(PlayerHits);
		SetSerializableInfo(CellTargets);

		//Frameworks
		CFramework_Others::CHandler::InstallFramework();
		CFramework_MapMa::CHandler::InstallFramework();
		CFramework_Blessings::CHandler::InstallFramework();
		CFramework_Enchantments::CHandler::InstallFramework();
		CFramework_Pets::CHandler::InstallFramework();
		CFramework_PlayerHomes::CHandler::InstallFramework();
		CFramework_Shouts::CHandler::InstallFramework();
		CFramework_Trainers::CHandler::InstallFramework();
	};

	void FrameworkAPI::RegisterCustomPatches()
	{
		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->InstallFramework();
		};
	}

	void FrameworkAPI::FinalizeRegistrations()
	{
		//Register Arrays
		ArrayHolder::RegisterArrays();

		//Exclude Vendor Chests.
		Completionist::CellScanner::init();
	}

	static std::string GetDeathPrefix(const std::string&  a_location)
	{
		static const std::vector<std::string> list = [] {
			std::vector<std::string> phrases;
			for (auto i = 1;; ++i) {
				const auto key = fmt::format("DeathPrefix_{}", i);
				const std::string phrase = GET_LOC_STRING_BY_KEY(key.c_str());
				if (phrase == key) {
					break;
				}
				phrases.push_back(phrase);
			}

			if (phrases.empty()) {
				phrases.push_back("met their end in");
			}
			return phrases;
		}();

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<std::size_t> uni(0, list.size() - 1);
		return fmt::format("{} {}", list[uni(rng)], a_location);
	};

	static std::string isVowel(char ch)
	{
		std::string str = "aeiouAEIOU";
		return GET_LOC_STRING_BY_KEY(str.find(ch) != std::string::npos ? "DeathArticle_Vowel" : "DeathArticle_Consonant");
	}

	//---------------------------------------------------
	//-- Framework Functions ( Record Player Kills ) ----
	//---------------------------------------------------

	void FrameworkAPI::OnDeath(const RE::TESDeathEvent* a_event)
	{
		if (!a_event->actorDying || !a_event->dead || !a_event->actorKiller) {
			return;
		}

		auto* NPC = a_event->actorDying->As<RE::Actor>();
		if (!NPC || !NPC->GetRace() || !PlayerHits.HasTargetRegistered(NPC)) {
			return;
		}

		std::string name = PlayerKills.GetRaceName(NPC);
		std::string unique_name = NPC->GetActorBase() && NPC->GetActorBase()->IsUnique() ? NPC->GetName() : "";
		std::string deathString;

		auto* location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation();
		auto* weapon = PlayerHits.GetTargetWeapon(NPC);

		if (!unique_name.empty()) {
			if (location) {
				deathString = fmt::format("{} {}.\n{} {}", unique_name, GetDeathPrefix(location->GetFullName()), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
			else {
				deathString = fmt::format("{} {}.\n{} {}", unique_name, GetDeathPrefix(GET_LOC_STRING_BY_KEY("DeathLocation_Unknown")), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
		}
		else {
			if (location) {
				deathString = fmt::format("{} {} {}\n{} {}", isVowel(NPC->GetName()[0]), NPC->GetName(), GetDeathPrefix(location->GetFullName()), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
			else {
				deathString = fmt::format("{} {} {}\n{} {}", isVowel(NPC->GetName()[0]), NPC->GetName(), GetDeathPrefix(GET_LOC_STRING_BY_KEY("DeathLocation_Unknown")), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
		}

		auto* killer = a_event->actorKiller.get();
		PlayerKills.AddKillOrAssist(name, deathString, (killer == RE::PlayerCharacter::GetSingleton()));
		PlayerHits.RemoveTarget(NPC->GetFormID());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Record Player Kills ) ----
	//---------------------------------------------------

	void FrameworkAPI::OnHit(RE::TESHitEvent const* a_event)
	{
		auto* target = a_event->target.get();
		auto* cause = a_event->cause.get();
		auto* source = RE::TESForm::LookupByID(a_event->source);
		auto* projectile = RE::TESForm::LookupByID(a_event->projectile);
		auto* weapon = source ? source : (projectile ? projectile : nullptr);

		if (!weapon || !target || !target->As<RE::Actor>() || !cause || cause != RE::PlayerCharacter::GetSingleton()) {
			return;
		}

		if (PlayerHits.HasTargetRegistered(target)) {
			PlayerHits.UpdateTargetWeapon(target, weapon);
			return;
		}

		PlayerHits.AddTarget(target, weapon);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Game Started ) -----------
	//---------------------------------------------------

	namespace
	{
		// Once the loading screen has gone, and the pet after each conversation
		class GameMenus final : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			static GameMenus* Get() {

				static GameMenus singleton;
				static const bool sunk = [] {
					if (auto* ui = RE::UI::GetSingleton()) {
						ui->AddEventSink<RE::MenuOpenCloseEvent>(&singleton);
						return true;
					}
					return false;
				}();
				(void)sunk;
				return &singleton;
			}

			void NotifyWhenLoaded() {

				pendingNotice = true;
				auto* ui = RE::UI::GetSingleton();
				if (!ui || !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME)) {
					SKSE::GetTaskInterface()->AddTask([this]() { ShowNotice(); });
				}
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {

				if (!a_event || a_event->opening) { return RE::BSEventNotifyControl::kContinue; }
				if (a_event->menuName == RE::LoadingMenu::MENU_NAME) {
					SKSE::GetTaskInterface()->AddTask([this]() { ShowNotice(); });
				} else if (a_event->menuName == RE::DialogueMenu::MENU_NAME) {
					SKSE::GetTaskInterface()->AddTask([]() { CFramework_Pets::CHandler::CheckAdoptedPet(); });
				}
				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			void ShowNotice() {
				if (pendingNotice.exchange(false)) { FrameworkAPI::NotifyStartup(); }
			}

			std::atomic<bool> pendingNotice{ false };
		};
	}

	//---------------------------------------------------
	//-- Framework Functions ( On Game Started ) --------
	//---------------------------------------------------

	// What the API and Load scripts did on a new game and load
	void FrameworkAPI::OnGameStarted(bool a_newGame) {

		INFO("Start-up: {}", a_newGame ? "new game" : "game loaded");
		CFramework_Pets::CHandler::CheckAdoptedPet();
		CFramework_Trainers::CHandler::FindPlacedTrainers();
		GameMenus::Get()->NotifyWhenLoaded();
	}

	//---------------------------------------------------
	//-- Variables Functions (Get Patch Count ) ---------
	//---------------------------------------------------

	int32_t FrameworkAPI::GetPatchCount(RE::StaticFunctionTag*) {
		return InstalledPatchesForMCMDisplay;
	};

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggingDates(RE::StaticFunctionTag*)
	{
		std::vector<std::string> list = LoggingData.GetAllLoggedDates();
		std::sort(list.begin(), list.end(), compare_dates);
		std::reverse(list.begin(), list.end());
		return list;
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	bool FrameworkAPI::compare_dates(const std::string& a, const std::string& b)
	{
		auto extract_date_part = [](const std::string& date, std::size_t start, std::size_t length) {
			int value = 0;
			if (start < date.size()) {
				const auto* begin = date.data() + start;
				std::from_chars(begin, begin + std::min(length, date.size() - start), value);
			}
			return value;
			};

		auto yr1 = extract_date_part(a, 6, 4);
		auto yr2 = extract_date_part(b, 6, 4);

		if (yr1 != yr2) {
			return yr1 < yr2;
		}

		auto mo1 = extract_date_part(a, 3, 2);
		auto mo2 = extract_date_part(b, 3, 2);

		if (mo1 != mo2) {
			return mo1 < mo2;
		}

		auto da1 = extract_date_part(a, 0, 2);
		auto da2 = extract_date_part(b, 0, 2);

		return da1 < da2;
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggedEventsForDate(RE::StaticFunctionTag*, std::string a_date, bool b_prefix)
	{
		auto list = LoggingData.GetAllLoggedEvents(a_date, b_prefix);
		std::reverse(list.begin(), list.end());
		return list;
	}

	std::int32_t FrameworkAPI::GetBookCategoryType(RE::TESForm* a_form)
	{
		if (!a_form) {
			return 0;
		}

		auto* book = a_form->As<RE::TESObjectBOOK>();

		if (book && (book->GetSpell() || book->TeachesSkill())) {
			return 0;  // Book with spells or skill teaching
		}

		return (book != nullptr) ? 2 : 0;  // Regular book or not a book
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::logType kType, std::string a_log)
	{
		LoggingData.AddLoggedEvent(kType, a_log);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Send Notification ) ------
	//---------------------------------------------------

	namespace
	{
		// Own notification stack, "Title - Item" on two lines
		bool ShowOwnNotification(std::string_view a_message, std::string_view a_setting, std::string_view a_colour) {

			if (!CompletionistUI::GameUI::NotificationsEnabled()) { return false; }
			std::string           text(a_message);
			constexpr std::string_view prefix = "Completionist: ";
			if (text.starts_with(prefix)) {
				text.erase(0, prefix.size());
			}
			std::string label;
			if (const auto dash = text.find(" - "); dash != std::string::npos) {
				label = text.substr(0, dash);
				text.erase(0, dash + 3);
			}
			while (!text.empty() && text.back() == '!') {
				text.pop_back();
			}

			using CompletionistUI::ToastKind;
			const auto kind = a_setting == "NotifyMissable" ? ToastKind::kCell : a_setting == "NotifySpecial" ? ToastKind::kDiscovery : a_setting == "NotifyStartup" || a_setting.empty() ? ToastKind::kInfo : ToastKind::kEntry;
			std::optional<ImVec4> colour;
			if (a_colour.size() > 1 && a_colour.front() == '#') {
				const auto value = std::strtoul(std::string(a_colour.substr(1)).c_str(), nullptr, 16);
				colour = ImVec4(static_cast<float>((value >> 16) & 0xFF) / 255.0f, static_cast<float>((value >> 8) & 0xFF) / 255.0f, static_cast<float>(value & 0xFF) / 255.0f, 1.0f);
			}
			CompletionistUI::GameUI::Notify(std::move(label), std::move(text), kind, colour);
			return true;
		}
	}

	void FrameworkAPI::SendNotification(std::string a_msg, std::string a_setting) {
		auto* setting = VariablesAPI::GetProperty(a_setting.c_str());
		if (!setting || !setting->GetBool()) { return; }

		auto* colour = VariablesAPI::GetProperty("ColourString");
		auto* colourEnabled = VariablesAPI::GetProperty("NotificationColourEnabled");
		const bool coloured = colour && colourEnabled && colourEnabled->GetBool();
		if (ShowOwnNotification(a_msg, a_setting, coloured ? std::string_view(colour->GetString()) : std::string_view{})) { return; }
		if (!colour || !colourEnabled || !colourEnabled->GetBool()) { RE::SendHUDMessage::ShowHUDMessage(a_msg.c_str()); return; }

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, colour->GetString(), a_msg);
		RE::SendHUDMessage::ShowHUDMessage(message.c_str());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Notify Startup ) ---------
	//---------------------------------------------------

	// Startup Notifications
	void FrameworkAPI::NotifyStartup() {
		SendNotification("Completionist: Start Up Tasks Complete", "NotifyStartup");
	}

	//---------------------------------------------------
	//-- String Functions -------------------------------
	//---------------------------------------------------

	std::string FrameworkAPI::GetVersion(RE::StaticFunctionTag*) { return std::string(FrameworkAPI::ReplaceStr(Plugin::Version.string(), "-", ".")); }
	std::string FrameworkAPI::ReplaceStr(std::string const& in, std::string const& from, std::string const& to) { return std::regex_replace(in, std::regex(from), to); }

	//---------------------------------------------------
	//-- Framework Events ( Load & Update Frameworks ) --
	//---------------------------------------------------

	void FrameworkAPI::Update()
	{
		std::erase_if(PlayerHits.data, [](const auto& a_hit) {
			const auto* actor = RE::TESForm::LookupByID<RE::Actor>(a_hit.first);
			return !actor || actor->IsDead();
		});

		AddUpdateFoundForms_Invoke();

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->UpdateFoundForms();
		};
	}

	//---------------------------------------------------
	//-- Framework Functions ( Refresh Completion ) -----
	//---------------------------------------------------

	// What the game changed without an event the mod saw, checked as the window opens
	void FrameworkAPI::RefreshCompletion()
	{
		CQuestMaster::QuestAPI::RefreshCompletion();
		CFramework_Enchantments::CHandler::RefreshKnown();
		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->RefreshKnownEnchantments();
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Added ) -----------
	//---------------------------------------------------

	void FrameworkAPI::OnMapMarkerAdded(RE::TESFullName* a_form)
	{
		_OnMapMarkerAdded(a_form);
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->ProcessHookedMarker(a_form->GetFullName());
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Discovered ) ------
	//---------------------------------------------------

	const char* FrameworkAPI::OnMapMarkerDiscovered(RE::TESFullName* a_form)
	{
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->ProcessHookedMarker(a_form->GetFullName());
		};

		return _OnMapMarkerDiscovered(a_form);
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Caught Fish ) -------
	//---------------------------------------------------

	void FrameworkAPI::ProcessCaughtFish(RE::TESForm* a_form) {
		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnFishCaught(a_form);
		};
	};

	namespace
	{
		// CC fishing: a catch is an AddItem with no source container while the fishing script runs.
		// Works with mods that edit ccBGSSSE001_FishingSystemScript.
		class FishingWatch final : public RE::BSTEventSink<RE::TESContainerChangedEvent>
		{
		public:
			static constexpr RE::FormID     fishingQuest = 0x033A57;  // ccBGSSSE001_FishingSystemQuest
			static constexpr std::int32_t   catchResolve = 5;         // SYSTEMSTATE_CATCH_RESOLVE
			static constexpr std::string_view plugin = "ccBGSSSE001-Fish.esm";

			static FishingWatch* Get()
			{
				static FishingWatch watch;
				return &watch;
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override
			{
				const auto* player = RE::PlayerCharacter::GetSingleton();
				if (!a_event || !player || a_event->oldContainer != 0 || !a_event->baseObj) {
					return RE::BSEventNotifyControl::kContinue;
				}
				const auto state = Resolving();
				if (!state.resolving || (a_event->newContainer != player->GetFormID() && a_event->baseObj != state.fish)) {
					return RE::BSEventNotifyControl::kContinue;
				}
				// Read now, patches told next frame
				SKSE::GetTaskInterface()->AddTask([form = a_event->baseObj]() {
					if (auto* caught = RE::TESForm::LookupByID(form)) {
						INFO("Fishing: caught {}", caught->GetName());
						FrameworkAPI::ProcessCaughtFish(caught);
					}
				});
				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			struct State
			{
				bool       resolving{ false };  // a catch is being given out
				RE::FormID fish{ 0 };	// 0 for junk and treasure
			};

			State Resolving()
			{
				auto* quest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(fishingQuest, plugin);
				if (!quest || !quest->IsRunning()) {
					return {};
				}
				const auto script = ScriptObject::FromForm(quest, "ccBGSSSE001_FishingSystemScript", false);
				const auto* state = script ? ScriptObject::GetVariable(script, "currentSystemState") : nullptr;
				if (!state || !state->IsInt()) {
					if (!_warned) {
						_warned = true;
						INFO("Fishing: the fishing system script has no currentSystemState; catches can't be tracked");
					}
					return {};
				}
				if (state->GetSInt() != catchResolve) {
					return {};
				}

				// ccBGSSSE001_FishCatchData fills one of these
				State result{ true };
				const auto* data = ScriptObject::GetVariable(script, "nextCatchData");
				const auto  catchData = data && data->IsObject() ? data->GetObject() : nullptr;
				for (const auto* name : { "fishFood", "fishIngredient", "fishMiscObject" }) {
					if (const auto* property = catchData ? catchData->GetProperty(name) : nullptr; property && property->IsObject()) {
						if (const auto* form = property->Unpack<RE::TESForm*>()) {
							result.fish = form->GetFormID();
							break;
						}
					}
				}
				return result;
			}

			bool _warned{ false };
		};
	}

	void FrameworkAPI::WatchFishing()
	{
		if (!RE::TESDataHandler::GetSingleton()->LookupModByName(FishingWatch::plugin)) {
			return;
		}
		if (auto* events = RE::ScriptEventSourceHolder::GetSingleton()) {
			events->AddEventSink<RE::TESContainerChangedEvent>(FishingWatch::Get());
			INFO("Fishing: watching for catches");
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleFormSet(a_ID);
		//INFO("Returning form array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Set Option Completed ) ---
	//---------------------------------------------------

	// Marking an entry found or not by hand, its variations follow it
	void FrameworkAPI::SetOptionCompleted(std::int32_t a_ID, RE::TESForm* a_form, bool a_completed) {

		auto& forms = HandleFormSet(a_ID);
		auto& bools = HandleBoolSet(a_ID);
		const auto t_pos = std::ranges::find(forms, a_form);
		if (!a_form || t_pos == forms.end()) { return; }

		const auto b_pos = static_cast<std::size_t>(std::distance(forms.begin(), t_pos));
		if (b_pos >= bools.size() || bools[b_pos] == a_completed) { return; }

		bools[b_pos] = a_completed;
		auto& found = HandleNoShow(a_ID) ? FoundItemData_NoShow : FoundItemData;
		const auto formID = a_form->GetFormID();
		a_completed ? found.AddForm(formID) : found.RemoveForm(formID);
		for (auto var : HandleDataSet(a_ID).GetAllVariations()) {
			if (HandleDataSet(a_ID).GetBase(var) == formID) {
				a_completed ? found.AddForm(var) : found.RemoveForm(var);
			}
		}

		HandleTotalSet(a_ID) = static_cast<std::int32_t>(forms.size());
		HandleFoundSet(a_ID) = static_cast<std::int32_t>(std::ranges::count(bools, true));
		INFO("Marked {:08X} {} by hand", formID, a_completed ? "found" : "not found");
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleNameSet(a_ID);
		//INFO("Returning name array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleTextSet(a_ID);
		//INFO("Returning text array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleBoolSet(a_ID);
		//INFO("Returning bool array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Has Book Spell / Read ) --
	//---------------------------------------------------

	bool FrameworkAPI::IsBookKnown(RE::TESForm* a_form) {

		auto* itm = a_form ? a_form->As<RE::TESObjectBOOK>() : nullptr;
		auto* pcr = RE::PlayerCharacter::GetSingleton();
		if (!itm || !pcr) { return false; }

		
		if (CVariables::V_TreatBooksAsItems && pcr->GetItemCount(itm) > 0) {
			FoundItemData.AddForm(itm);
			return FoundItemData.HasForm(a_form);
		}

		if (itm->IsRead() || (itm->GetSpell() ? pcr->HasSpell(itm->GetSpell()) : false)) {
			FoundItemData.AddForm(itm);
		}

		return FoundItemData.HasForm(itm);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Enchantment Known ) ---
	//---------------------------------------------------

	bool FrameworkAPI::IsEnchantmentKnown(RE::TESForm* a_form) {
	
		auto* enchantment = a_form ? a_form->As<RE::EnchantmentItem>() : nullptr;
		if (enchantment && enchantment->GetKnown()) {
			FoundItemData_NoShow.AddForm(enchantment);
		}
		return FoundItemData_NoShow.HasForm(enchantment);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Item Known ) ----------
	//---------------------------------------------------

	bool FrameworkAPI::IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data = nullptr) {

		if (!a_form || !a_data) { return false; }

		auto base = a_data->GetBase(a_form->GetFormID()) ? a_data->GetBase(a_form->GetFormID()) : a_form->GetFormID();
		auto* pcr = RE::PlayerCharacter::GetSingleton();

		if (pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(base)) > 0) {
			FoundItemData.AddForm(base);
			for (auto var : a_data->GetAllVariations()) {
				if (a_data->GetBase(var) == base) {
					FoundItemData.AddForm(var);
				}
			}
			return FoundItemData.HasForm(a_form);
		}

		for (auto var : a_data->GetAllVariations()) {
			if (a_data->GetBase(var) == base && pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(var)) > 0) {

				FoundItemData.AddForm(base);
				for (auto variation : a_data->GetAllVariations()) {
					if (a_data->GetBase(variation) == base) {
						FoundItemData.AddForm(variation);
					}
				}
				return FoundItemData.HasForm(a_form);
			}
		}

		return FoundItemData.HasForm(a_form);
	};
};