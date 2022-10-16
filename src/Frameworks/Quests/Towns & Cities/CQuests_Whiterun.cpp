#include "CQuests_Whiterun.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Whiterun {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Whiterun_Quest00_Key", "$Whiterun_Quest00_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest00_Data", "FreeformWhiterunQuest04"},
	/*01*/ {"Whiterun_Quest01_Key", "$Whiterun_Quest01_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest01_Data", "FreeformWhiterunQuest02"},
	/*02*/ {"Whiterun_Quest02_Key", "$Whiterun_Quest02_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest02_Data", "FreeformWhiterunQuest01"},
	/*03*/ {"Whiterun_Quest03_Key", "$Whiterun_Quest03_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest03_Data", "FreeformWhiterunQuest03"},
	/*04*/ {"Whiterun_Quest04_Key", "$Whiterun_Quest04_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest04_Data", "MS08"},
	/*05*/ {"Whiterun_Quest05_Key", "$Whiterun_Quest05_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest05_Data", "MS09"},
	/*06*/ {"Whiterun_Quest06_Key", "$Whiterun_Quest06_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest06_Data", "FreeformWhiterunQuest05"},
	/*07*/ {"Whiterun_Quest07_Key", "$Whiterun_Quest07_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest07_Data", "dunSleepingTreeCampQST"},
	/*08*/ {"Whiterun_Quest08_Key", "$Whiterun_Quest08_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest08_Data", "T03"},
	/*09*/ {"Whiterun_Quest09_Key", "$Whiterun_Quest09_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest09_Data", "DA08"},
	/*10*/ {"Whiterun_Quest10_Key", "$Whiterun_Quest10_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest10_Data", "Favor013"},
	/*11*/ {"Whiterun_Quest11_Key", "$Whiterun_Quest11_Name", RADI_QUEST_FLAG, "$Whiterun_Quest11_Data", "FavorJobsChopWood"},
	/*12*/ {"Whiterun_Quest12_Key", "$Whiterun_Quest12_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest12_Data", "Favor204"},
	/*13*/ {"Whiterun_Quest13_Key", "$Whiterun_Quest13_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest13_Data", "Favor017"},
	/*14*/ {"Whiterun_Quest14_Key", "$Whiterun_Quest14_Name", RADI_QUEST_FLAG, "$Whiterun_Quest14_Data", "FavorJobsBeggars"},
	/*15*/ {"Whiterun_Quest15_Key", "$Whiterun_Quest15_Name", RADI_QUEST_FLAG, "$Whiterun_Quest15_Data", "FavorJobsBeggars"},
	/*16*/ {"Whiterun_Quest16_Key", "$Whiterun_Quest16_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest16_Data", "Favor110"},
	/*17*/ {"Whiterun_Quest17_Key", "$Whiterun_Quest17_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest17_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,6,8,9,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{7, "Completionist_dunSleepingTreeCampQST", 1},
	{10, "Completionist_Favor013CarlottaValentia", 1},
	{11, "Completionist_Favor_ChopWoodHulda", -4},
	{12, "Completionist_Favor204Amren", 1},
	{13, "Completionist_Favor017Uthgerd", 1},
	{14, "Completionist_FavorBeggarBrenuin", -4},
	{15, "Completionist_FavorBeggarLucia", -4},
	{16, "Completionist_Favor110Ysolda", 1},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{17, "WhiterunImpGetOutofJail", "WhiterunSonsGetOutofJail"},
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