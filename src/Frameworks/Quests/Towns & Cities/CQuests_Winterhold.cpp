#include "CQuests_Winterhold.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Winterhold {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"Winterhold_Quest00_Key", "$Winterhold_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Winterhold_Quest00_Data", "FreeformWinterholdC"},
		/*01*/ {"Winterhold_Quest01_Key", "$Winterhold_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Winterhold_Quest01_Data", "Favor013"},
		/*02*/ {"Winterhold_Quest02_Key", "$Winterhold_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Winterhold_Quest02_Data", "Favor158"},
		/*03*/ {"Winterhold_Quest03_Key", "$Winterhold_Quest03_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Winterhold_Quest03_Data", "FavorJobsDrunks"},
		/*04*/ {"Winterhold_Quest04_Key", "$Winterhold_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Winterhold_Quest04_Data", "Favor018"},
		/*05*/ {"Winterhold_Quest05_Key", "$Winterhold_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Winterhold_Quest05_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
	{ 0, 40 },
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{1, "Completionist_Favor013Haran", 1},
	{2, "Completionist_Favor158Korir", 1},
	{3, "Completionist_FavorDrunksRanmir", RADIANT_COUNTER_VALUE},
	{4, "Completionist_Favor018Malur", 1},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{5, "WinterholdImpGetOutofJail", "WinterholdSonsGetOutofJail"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		SinkEvents();

		IdenArray.clear();
		NameArray.clear();
		RadiArray.clear();
		NameArray.clear();
		KeysArray.clear();
		StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text, id] : QuestData) {
			KeysArray.push_back(key);
			NameArray.push_back(name);
			RadiArray.push_back(flag);
			TextArray.push_back(text);
			IdenArray.push_back(id);
			StgeArray.push_back(isStageDone);
		}

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		assert(StgeArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(IdenArray, quest->GetFormEditorID());
		if (t_pos == IdenArray.end()) { return EventResult::kContinue; }


		if (StgeArray.at(std::distance(IdenArray.begin(), t_pos))) {
			CQuestKeys_Stages.AddStage(KeysArray.at(std::distance(IdenArray.begin(), t_pos)), a_event->stage);
			INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, IdenArray.at(std::distance(IdenArray.begin(), t_pos)));
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || !a_event->baseObj || a_event->oldContainer != RE::PlayerCharacter::GetSingleton()->GetFormID()) { return EventResult::kContinue; }

		auto* List = static_cast<RE::BGSListForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x0072EA2, "Skyrim.esm"));
		if (!List || !List->HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto speaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();
		if (!speaker || speaker->GetFormID() != a_event->newContainer) { return EventResult::kContinue; }

		auto* ranmir = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x001C18B, "Skyrim.esm"));
		if (!ranmir) { return EventResult::kContinue; }

		if (speaker == ranmir && a_event->newContainer == ranmir->GetFormID()) {
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksRanmir")) {
				var->value += 1;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		CHandler::UpdateCompletion();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateCompletion() {

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}

		for (auto& [i, stage] : StageCompletion) {
			BoolArray[i] = StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}