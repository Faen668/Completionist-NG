#include "CQuests_RavenRock.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_RavenRock {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
	/*00*/ {"RavenRock_Quest00_Key", "$RavenRock_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest00_Data", "DLC2RR03Intro"},
	/*01*/ {"RavenRock_Quest01_Key", "$RavenRock_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest01_Data", "DLC2RRFavor03"},
	/*02*/ {"RavenRock_Quest02_Key", "$RavenRock_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest02_Data", "DLC2RRFavor06"},
	/*03*/ {"RavenRock_Quest03_Key", "$RavenRock_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest03_Data", "DLC2RRFavor02"},
	/*04*/ {"RavenRock_Quest04_Key", "$RavenRock_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest04_Data", "DLC2RRFavor07"},
	/*05*/ {"RavenRock_Quest05_Key", "$RavenRock_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest05_Data", "DLC2RR03"},
	/*06*/ {"RavenRock_Quest06_Key", "$RavenRock_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest06_Data", "DLC2RRFavor04"},
	/*07*/ {"RavenRock_Quest07_Key", "$RavenRock_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest07_Data", "DLC2RR01"},
	/*08*/ {"RavenRock_Quest08_Key", "$RavenRock_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest08_Data", "DLC2TGQuest"},
	/*09*/ {"RavenRock_Quest09_Key", "$RavenRock_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$RavenRock_Quest09_Data", "DLC2RRFavor05"},
	/*10*/ {"RavenRock_Quest10_Key", "$RavenRock_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest10_Data", "DLC2RRFavor01"},
	/*11*/ {"RavenRock_Quest11_Key", "$RavenRock_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$RavenRock_Quest11_Data", "DLC2RR02"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,5,6,7,8,10,11
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
	{ 1, 199 },
	{ 2, 199 },
	{ 3, 199 },
	{ 4, 199 },
	{ 9, 30 },
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
		auto userinterface = RE::UI::GetSingleton();
		userinterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

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

		for (auto i : StandardCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_N(KeysArray[i], IdenArray[i]);
		}

		for (auto& [i, stage] : StageCompletion) {
			BoolArray[i] = StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}