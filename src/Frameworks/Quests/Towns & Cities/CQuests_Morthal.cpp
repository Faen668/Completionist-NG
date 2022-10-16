#include "CQuests_Morthal.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Morthal {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Morthal_Quest00_Key", "$Morthal_Quest00_Name", SIDE_QUEST_FLAG, "$Morthal_Quest00_Data", "FreeformMorthalA"},
	/*01*/ {"Morthal_Quest01_Key", "$Morthal_Quest01_Name", SIDE_QUEST_FLAG, "$Morthal_Quest01_Data", "FreeformMorthalB"},
	/*02*/ {"Morthal_Quest02_Key", "$Morthal_Quest02_Name", SIDE_QUEST_FLAG, "$Morthal_Quest02_Data", "MS14"},
	/*03*/ {"Morthal_Quest03_Key", "$Morthal_Quest03_Name", RADI_QUEST_FLAG, "$Morthal_Quest03_Data", "FavorJobsChopWood"},
	/*04*/ {"Morthal_Quest04_Key", "$Morthal_Quest04_Name", RADI_QUEST_FLAG, "$Morthal_Quest04_Data", "FavorJobsChopWood"},
	/*05*/ {"Morthal_Quest05_Key", "$Morthal_Quest05_Name", SIDE_QUEST_FLAG, "$Morthal_Quest05_Data", "Favor001"},
	/*06*/ {"Morthal_Quest06_Key", "$Morthal_Quest06_Name", SIDE_QUEST_FLAG, "$Morthal_Quest06_Data", "Favor017"},
	/*07*/ {"Morthal_Quest07_Key", "$Morthal_Quest07_Name", SIDE_QUEST_FLAG, "$Morthal_Quest07_Data", "Favor110"},
	/*08*/ {"Morthal_Quest08_Key", "$Morthal_Quest08_Name", SIDE_QUEST_FLAG, "$Morthal_Quest08_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::size_t StandardCompletion[] = { 
	2,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{3, "Completionist_Favor_ChopWoodHroggar", -4},
	{4, "Completionist_Favor_ChopWoodHroggar", -4},
	{5, "Completionist_Favor001Idgrod", 1},
	{6, "Completionist_Favor017Benor", 1},
	{7, "Completionist_Favor110Lami", 1},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 20 },
	{ 1, 10 },
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{8, "HjaalmarchImpGetOutofJail", "HjaalmarchSonsGetOutofJail"},
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
			KeysArray.push_back(Key); NameArray.push_back(Name); RadiArray.push_back(Flag); TextArray.push_back(Text); IdenArray.push_back(ID);
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
} 