#include "CQuests_Whiterun.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Whiterun {
	using namespace Serialization;
	using namespace CFramework_Master;

	// Main Quests
	inline std::vector<RE::TESForm*>	FormArray;
	inline std::vector<std::string>		NameArray;
	inline std::vector<std::string>		IdenArray;
	inline std::vector<std::string>		TextArray;
	inline std::vector<std::int32_t>	RadiArray;
	inline std::vector<bool>			BoolArray;
	inline std::vector<std::string>		KeysArray;

	std::int32_t ArraySize = 128;
	std::int32_t MAIN_QUEST_FLAG = 0;
	std::int32_t SIDE_QUEST_FLAG = 1;
	std::int32_t RADI_QUEST_FLAG = 2;

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

		/*00*/ AddQuest("Whiterun_Quest00_Key", "$Whiterun_Quest00_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest00_Data", "FreeformWhiterunQuest04");
		/*01*/ AddQuest("Whiterun_Quest01_Key", "$Whiterun_Quest01_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest01_Data", "FreeformWhiterunQuest02");
		/*02*/ AddQuest("Whiterun_Quest02_Key", "$Whiterun_Quest02_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest02_Data", "FreeformWhiterunQuest01");
		/*03*/ AddQuest("Whiterun_Quest03_Key", "$Whiterun_Quest03_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest03_Data", "FreeformWhiterunQuest03");
		/*04*/ AddQuest("Whiterun_Quest04_Key", "$Whiterun_Quest04_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest04_Data", "MS08");
		/*05*/ AddQuest("Whiterun_Quest05_Key", "$Whiterun_Quest05_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest05_Data", "MS09");
		/*06*/ AddQuest("Whiterun_Quest06_Key", "$Whiterun_Quest06_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest06_Data", "FreeformWhiterunQuest05");
		/*07*/ AddQuest("Whiterun_Quest07_Key", "$Whiterun_Quest07_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest07_Data", "dunSleepingTreeCampQST");
		/*08*/ AddQuest("Whiterun_Quest08_Key", "$Whiterun_Quest08_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest08_Data", "T03");
		/*09*/ AddQuest("Whiterun_Quest09_Key", "$Whiterun_Quest09_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest09_Data", "DA08");
		/*10*/ AddQuest("Whiterun_Quest10_Key", "$Whiterun_Quest10_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest10_Data", "Favor013");
		/*11*/ AddQuest("Whiterun_Quest11_Key", "$Whiterun_Quest11_Name", RADI_QUEST_FLAG, "$Whiterun_Quest11_Data", "FavorJobsChopWood");
		/*12*/ AddQuest("Whiterun_Quest12_Key", "$Whiterun_Quest12_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest12_Data", "Favor204");
		/*13*/ AddQuest("Whiterun_Quest13_Key", "$Whiterun_Quest13_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest13_Data", "Favor017");
		/*14*/ AddQuest("Whiterun_Quest14_Key", "$Whiterun_Quest14_Name", RADI_QUEST_FLAG, "$Whiterun_Quest14_Data", "FavorJobsBeggars");
		/*15*/ AddQuest("Whiterun_Quest15_Key", "$Whiterun_Quest15_Name", RADI_QUEST_FLAG, "$Whiterun_Quest15_Data", "FavorJobsBeggars");
		/*16*/ AddQuest("Whiterun_Quest16_Key", "$Whiterun_Quest16_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest16_Data", "Favor110");
		/*17*/ AddQuest("Whiterun_Quest17_Key", "$Whiterun_Quest17_Name", SIDE_QUEST_FLAG, "$Whiterun_Quest17_Data", "FavorJarlsMakeFriends");

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add Quest To Array ) -----
	//---------------------------------------------------

	void CHandler::AddQuest(std::string k, std::string n, std::int32_t t, std::string h, std::string g) {

		KeysArray.push_back(k);
		NameArray.push_back(n);
		RadiArray.push_back(t);
		TextArray.push_back(h);
		IdenArray.push_back(g);
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