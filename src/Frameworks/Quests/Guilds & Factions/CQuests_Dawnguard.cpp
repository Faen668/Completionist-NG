#include "CQuests_Dawnguard.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Dawnguard {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Dawnguard_Quest00_Key", "$Dawnguard_Quest00_Name", SIDE_QUEST_FLAG, "$Dawnguard_Quest00_Data", "DLC1HunterBaseStage2"},
	/*01*/ {"Dawnguard_Quest01_Key", "$Dawnguard_Quest01_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest01_Data", "DLC1RH05"},
	/*02*/ {"Dawnguard_Quest02_Key", "$Dawnguard_Quest02_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest02_Data", "DLC1RH02"},
	/*03*/ {"Dawnguard_Quest03_Key", "$Dawnguard_Quest03_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest03_Data", "DLC1RH01"},
	/*04*/ {"Dawnguard_Quest04_Key", "$Dawnguard_Quest04_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest04_Data", "DLC1RH04"},
	/*05*/ {"Dawnguard_Quest05_Key", "$Dawnguard_Quest05_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest05_Data", "DLC1RH07"},
	/*06*/ {"Dawnguard_Quest06_Key", "$Dawnguard_Quest06_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest06_Data", "DLC1RH08"},
	/*07*/ {"Dawnguard_Quest07_Key", "$Dawnguard_Quest07_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest07_Data", "DLC1RH03"},
	/*08*/ {"Dawnguard_Quest08_Key", "$Dawnguard_Quest08_Name", RADI_QUEST_FLAG, "$Dawnguard_Quest08_Data", "DLC1RH06"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{1, "DLC1RH05FinishedAllQuests", 1},
	{2, "Completionist_DLC1RH02", RADIANT_DAWNGUARD_VALUE},
	{3, "Completionist_DLC1RH01", RADIANT_DAWNGUARD_VALUE},
	{4, "Completionist_DLC1RH04", RADIANT_DAWNGUARD_VALUE},
	{5, "Completionist_DLC1RH07", RADIANT_DAWNGUARD_VALUE},
	{6, "DLC1RH08FinishedAllQuests", 3},
	{7, "Completionist_DLC1RH03", RADIANT_DAWNGUARD_VALUE},
	{8, "Completionist_DLC1RH06", RADIANT_DAWNGUARD_VALUE},
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
	};
}