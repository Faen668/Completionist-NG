#include "CQuests_Misc_DG.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Misc_DG {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
	/*00*/ {"Misc_DG_Quest00_Key", "$Misc_DG_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Misc_DG_Quest00_Data", "DLC1VQDragon"},
	/*01*/ {"Misc_DG_Quest01_Key", "$Misc_DG_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Misc_DG_Quest01_Data", "DLC1VQFVBooks"},
	/*02*/ {"Misc_DG_Quest02_Key", "$Misc_DG_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Misc_DG_Quest02_Data", "DLC1VQSaint"},
	/*03*/ {"Misc_DG_Quest03_Key", "$Misc_DG_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Misc_DG_Quest03_Data", "DLC1LD"},
	/*04*/ {"Misc_DG_Quest04_Key", "$Misc_DG_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Misc_DG_Quest04_Data", "DLC01SoulCairnHorseQuest2"},
	};

	constexpr std::size_t StandardCompletion[] = {
	2
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
	{ 0,  60  },
	{ 1,  200 },
	{ 3,  255 },
	{ 4,  255 },
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