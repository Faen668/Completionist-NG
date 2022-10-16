#include "CQuests_MainStory_CW.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CW {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/{"CWQuest00_Key", "$CWQuest00_Name", MAIN_QUEST_FLAG ,"$CWQuest00_Data", "CW01A"},
		/*01*/{"CWQuest01_Key", "$CWQuest01_Name", MAIN_QUEST_FLAG, "$CWQuest01_Data", "CW02A"},
		/*02*/{"CWQuest02_Key", "$CWQuest02_Name", MAIN_QUEST_FLAG, "$CWQuest02_Data", "CW03"},
		/*03*/{"CWQuest03_Key", "$CWQuest03_Name", MAIN_QUEST_FLAG, "$CWQuest03_Data", "CWSiegeObj"},
		/*04*/{"CWQuest04_Key", "$CWQuest04_Name", MAIN_QUEST_FLAG, "$CWQuest04_Data", "CWObj"},
		/*05*/{"CWQuest05_Key", "$CWQuest05_Name", MAIN_QUEST_FLAG, "$CWQuest05_Data", "CWSiegeObj"},
		/*06*/{"CWQuest06_Key", "$CWQuest06_Name", SIDE_QUEST_FLAG, "$CWQuest06_Data", "CWMission03"},
		/*07*/{"CWQuest07_Key", "$CWQuest07_Name", SIDE_QUEST_FLAG, "$CWQuest07_Data", "CWMission07"},
		/*08*/{"CWQuest08_Key", "$CWQuest08_Name", SIDE_QUEST_FLAG, "$CWQuest08_Data", "CW01AOutfitImperial"},
		/*09*/{"CWQuest09_Key", "$CWQuest09_Name", SIDE_QUEST_FLAG, "$CWQuest09_Data", "CWMission04"},
		/*10*/{"CWQuest10_Key", "$CWQuest10_Name", SIDE_QUEST_FLAG, "$CWQuest10_Data", "CWFortSiegeFort"},
		/*11*/{"CWQuest11_Key", "$CWQuest11_Name", SIDE_QUEST_FLAG, "$CWQuest11_Data", "CWFortSiegeFort"},
		/*12*/{"CWQuest12_Key", "$CWQuest12_Name", SIDE_QUEST_FLAG, "$CWQuest12_Data", "CWFortSiegeFort"},
		/*13*/{"CWQuest13_Key", "$CWQuest13_Name", SIDE_QUEST_FLAG, "$CWQuest13_Data", "CWFortSiegeFort"},
		/*14*/{"CWQuest14_Key", "$CWQuest14_Name", SIDE_QUEST_FLAG, "$CWQuest14_Data", "CWFortSiegeFort"},
		/*15*/{"CWQuest15_Key", "$CWQuest15_Name", SIDE_QUEST_FLAG, "$CWQuest15_Data", "CWFortSiegeFort"},
		/*16*/{"CWQuest16_Key", "$CWQuest16_Name", MAIN_QUEST_FLAG, "$CWQuest16_Data", "CW01B"},
		/*17*/{"CWQuest17_Key", "$CWQuest17_Name", MAIN_QUEST_FLAG, "$CWQuest17_Data", "CW02B"},
		/*18*/{"CWQuest18_Key", "$CWQuest18_Name", MAIN_QUEST_FLAG, "$CWQuest18_Data", "CW03"},
		/*19*/{"CWQuest19_Key", "$CWQuest19_Name", MAIN_QUEST_FLAG, "$CWQuest19_Data", "CWSiegeObj"},
		/*20*/{"CWQuest20_Key", "$CWQuest20_Name", MAIN_QUEST_FLAG, "$CWQuest20_Data", "CWObj"},
		/*21*/{"CWQuest21_Key", "$CWQuest21_Name", MAIN_QUEST_FLAG, "$CWQuest21_Data", "CWSiegeObj"},
		/*22*/{"CWQuest22_Key", "$CWQuest22_Name", SIDE_QUEST_FLAG, "$CWQuest22_Data", "CWMission03"},
		/*23*/{"CWQuest23_Key", "$CWQuest23_Name", SIDE_QUEST_FLAG, "$CWQuest23_Data", "CWMission07"},
		/*24*/{"CWQuest24_Key", "$CWQuest24_Name", SIDE_QUEST_FLAG, "$CWQuest24_Data", "CWMission04"},
		/*25*/{"CWQuest25_Key", "$CWQuest25_Name", SIDE_QUEST_FLAG, "$CWQuest25_Data", "CWFortSiegeFort"},
		/*26*/{"CWQuest26_Key", "$CWQuest26_Name", SIDE_QUEST_FLAG, "$CWQuest26_Data", "CWFortSiegeFort"},
		/*27*/{"CWQuest27_Key", "$CWQuest27_Name", SIDE_QUEST_FLAG, "$CWQuest27_Data", "CWFortSiegeFort"},
		/*28*/{"CWQuest28_Key", "$CWQuest28_Name", SIDE_QUEST_FLAG, "$CWQuest28_Data", "CWFortSiegeFort"},
		/*29*/{"CWQuest29_Key", "$CWQuest29_Name", SIDE_QUEST_FLAG, "$CWQuest29_Data", "CWFortSiegeFort"},
		/*30*/{"CWQuest30_Key", "$CWQuest30_Name", SIDE_QUEST_FLAG, "$CWQuest30_Data", "CWFortSiegeFort"},
	};

	constexpr std::size_t StandardCompletion[] = { 0,1,2,4,6,7,9,16,17,18,20,22,23,24 };

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
		{3, "Completionist_SiegeWhiterun", 1},
		{5, "Completionist_SiegeWindhelm", 1},
		{10, "Completionist_FortEastmarch", 1},
		{11, "Completionist_FortPale", 1},
		{12, "Completionist_FortRift", 1},
		{13, "Completionist_FortFalkreath", 1},
		{14, "Completionist_FortHjaalmarch", 1},
		{15, "Completionist_FortReach", 1},
		{19, "Completionist_SiegeWhiterun", 1},
		{21, "Completionist_SiegeSolitude", 1},
		{25, "Completionist_FortPale", 1},
		{26, "Completionist_FortRift", 1},
		{27, "Completionist_FortWinterhold", 1},
		{28, "Completionist_FortHaafingar", 1},
		{29, "Completionist_FortHjaalmarch", 1},
		{30, "Completionist_FortReach", 1}
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
		{ 8, 12 }
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

		for (auto& [Key, Name, Flag, Text, ID] : QuestData) {
			KeysArray.push_back(Key);
			NameArray.push_back(Name);
			RadiArray.push_back(Flag);
			TextArray.push_back(Text);
			IdenArray.push_back(ID);
		}

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
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

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
} 