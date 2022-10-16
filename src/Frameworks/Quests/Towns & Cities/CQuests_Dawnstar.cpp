#include "CQuests_Dawnstar.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Dawnstar {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/ {"Dawnstar_Quest00_Key", "$Dawnstar_Quest00_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest00_Data", "Favor153"},
		/*01*/ {"Dawnstar_Quest01_Key", "$Dawnstar_Quest01_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest01_Data", "DA07"},
		/*02*/ {"Dawnstar_Quest02_Key", "$Dawnstar_Quest02_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest02_Data", "DA16"},
		/*03*/ {"Dawnstar_Quest03_Key", "$Dawnstar_Quest03_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest03_Data", "FreeformShipsDawnstar"},
		/*04*/ {"Dawnstar_Quest04_Key", "$Dawnstar_Quest04_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest04_Data", "Favor205"},
		/*05*/ {"Dawnstar_Quest05_Key", "$Dawnstar_Quest05_Name", RADI_QUEST_FLAG, "$Dawnstar_Quest05_Data", "FavorJobsDrunks"},
		/*06*/ {"Dawnstar_Quest06_Key", "$Dawnstar_Quest06_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest06_Data", "Favor110"},
		/*07*/ {"Dawnstar_Quest07_Key", "$Dawnstar_Quest07_Name", SIDE_QUEST_FLAG, "$Dawnstar_Quest07_Data", "FavorJarlsMakeFriends"},
		/*08*/ {"Dawnstar_Quest08_Key", "$Dawnstar_Quest08_Name", RADI_QUEST_FLAG, "$Dawnstar_Quest08_Data", "FavorJobsMineOre"},
		/*09*/ {"Dawnstar_Quest09_Key", "$Dawnstar_Quest09_Name", RADI_QUEST_FLAG, "$Dawnstar_Quest09_Data", "FavorJobsMineOre"},
	};

	constexpr std::size_t StandardCompletion[] = { 
		1,2 
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
		{0, "Completionist_Favor153Skald", 1},
		{4, "Completionist_Favor205Frida", 1},
		{5, "Completionist_FavorDrunksKarl", -4},
		{6, "Completionist_Favor110Rustleif", 1},
		{8, "Completionist_FavorMineOreBeitld", -4},
		{9, "Completionist_FavorMineOreLeigelf", -4},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
		{7, "PaleImpGetOutofJail", "PaleSonsGetOutofJail"},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
		{3,  199},
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	}
} 