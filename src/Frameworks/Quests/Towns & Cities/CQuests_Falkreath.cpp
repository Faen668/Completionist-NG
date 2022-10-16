#include "CQuests_Falkreath.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Falkreath {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Falkreath_Quest00_Key", "$Falkreath_Quest00_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest00_Data", "DA03"},
	/*01*/ {"Falkreath_Quest01_Key", "$Falkreath_Quest01_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest01_Data", "FreeformFalkreathQuest03"},
	/*02*/ {"Falkreath_Quest02_Key", "$Falkreath_Quest02_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest02_Data", "DA05"},
	/*03*/ {"Falkreath_Quest03_Key", "$Falkreath_Quest03_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest03_Data", "Favor154"},
	/*04*/ {"Falkreath_Quest04_Key", "$Falkreath_Quest04_Name", RADI_QUEST_FLAG, "$Falkreath_Quest04_Data", "FavorJobsChopWood"},
	/*05*/ {"Falkreath_Quest05_Key", "$Falkreath_Quest05_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest05_Data", "Favor001"},
	/*06*/ {"Falkreath_Quest06_Key", "$Falkreath_Quest06_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest06_Data", "Favor205"},
	/*07*/ {"Falkreath_Quest07_Key", "$Falkreath_Quest07_Name", RADI_QUEST_FLAG, "$Falkreath_Quest07_Data", "FavorJobsGatherWheat"},
	/*08*/ {"Falkreath_Quest08_Key", "$Falkreath_Quest08_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest08_Data", "Favor110"},
	/*09*/ {"Falkreath_Quest09_Key", "$Falkreath_Quest09_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest09_Data", "Favor018"},
	/*10*/ {"Falkreath_Quest10_Key", "$Falkreath_Quest10_Name", SIDE_QUEST_FLAG, "$Falkreath_Quest10_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::size_t StandardCompletion[] = { 
	0,1,2 
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{3, "Completionist_Favor154Siddgeir", 1},
	{4, "Completionist_Favor_ChopWoodHert", -4},
	{5, "Completionist_Favor001Thadgeir", 1},
	{6, "Completionist_Favor205Runil", 1},
	{7, "Completionist_FavorHarvestMathies", -4},
	{8, "Completionist_Favor110Siddgeir", 1},
	{9, "Completionist_Favor018Dengeir", 1},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{10, "FalkreathImpGetOutofJail", "FalkreathSonsGetOutofJail"},
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
} 