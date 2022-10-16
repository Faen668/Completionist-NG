#include "CQuests_Dungeons.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Dungeons {
	using namespace Serialization;
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
	/*00*/ {"Dungeons_Quest00_Key", "$Dungeons_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest00_Data", "dunAngarvundeQST"},
	/*01*/ {"Dungeons_Quest01_Key", "$Dungeons_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest01_Data", "dunAnsilvundQST"},
	/*02*/ {"Dungeons_Quest02_Key", "$Dungeons_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest02_Data", "dunBlindCliffQST"},
	/*03*/ {"Dungeons_Quest03_Key", "$Dungeons_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest03_Data", "dunDarklightQST"},
	/*04*/ {"Dungeons_Quest04_Key", "$Dungeons_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest04_Data", "dunForelhostQST"},
	/*05*/ {"Dungeons_Quest05_Key", "$Dungeons_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest05_Data", "dunFrostflowAbyssQST"},
	/*06*/ {"Dungeons_Quest06_Key", "$Dungeons_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest06_Data", "dunFrostmereCryptQST"},
	/*07*/ {"Dungeons_Quest07_Key", "$Dungeons_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest07_Data", "dunHighGateRuinsQST"},
	/*08*/ {"Dungeons_Quest08_Key", "$Dungeons_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest08_Data", "dunHillgrundsTombQST"},
	/*09*/ {"Dungeons_Quest09_Key", "$Dungeons_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest09_Data", "dunIronbindQST"},
	/*10*/ {"Dungeons_Quest10_Key", "$Dungeons_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest10_Data", "dunLiarsRetreatQST"},
	/*11*/ {"Dungeons_Quest11_Key", "$Dungeons_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest11_Data", "dunMistwatchQST"},
	/*12*/ {"Dungeons_Quest12_Key", "$Dungeons_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest12_Data", "dunNilheimQST"},
	/*13*/ {"Dungeons_Quest13_Key", "$Dungeons_Quest13_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest13_Data", "dunRagnvaldQST"},
	/*14*/ {"Dungeons_Quest14_Key", "$Dungeons_Quest14_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest14_Data", "dunRebelsCairnQST"},
	/*15*/ {"Dungeons_Quest15_Key", "$Dungeons_Quest15_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest15_Data", "dunRobbersGorgeQST"},
	/*16*/ {"Dungeons_Quest16_Key", "$Dungeons_Quest16_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest16_Data", "dunSouthfringeQST"},
	/*17*/ {"Dungeons_Quest17_Key", "$Dungeons_Quest17_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest17_Data", "dunTrevasWatchQST"},
	/*18*/ {"Dungeons_Quest18_Key", "$Dungeons_Quest18_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest18_Data", "dunValthumeQST"},
	/*19*/ {"Dungeons_Quest19_Key", "$Dungeons_Quest19_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest19_Data", "dunVolskyggeQST"},
	/*20*/ {"Dungeons_Quest20_Key", "$Dungeons_Quest20_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Dungeons_Quest20_Data", "dunVolunruudQST"},
	/*21*/ {"Dungeons_Quest21_Key", "$Dungeons_Quest21_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$Dungeons_Quest21_Data", "dunYngolBarrowQST"},
	};

	constexpr std::size_t StandardCompletion[] = {
	1,2,3,4,5,7,8,11,12,16,17,18,20
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
	{ 0,  254 },
	{ 6,  100 },
	{ 9,  81, },
	{ 10, 100 },
	{ 13, 30, },
	{ 14, 30  },
	{ 15, 20  },
	{ 19, 20  },
	{ 21, 100 },
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
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage) ;
		}
	};
}