#include "CQuests_CreationClub_01.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC1 {
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

	constexpr std::size_t StandardCompletion[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
		{21, "Completionist_Fishing2", 4},
		{22, "Completionist_Fishing3", 4},
		{23, "Completionist_Fishing1", 4},
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

		/*00*/ AddQuest("CC01_Quest00_Key", "$CC01_Quest00_Name", MAIN_QUEST_FLAG, "$CC01_Quest00_Data", "ccVSVSSE004_MainQuest");
		/*01*/ AddQuest("CC01_Quest01_Key", "$CC01_Quest01_Name", MAIN_QUEST_FLAG, "$CC01_Quest01_Data", "ccVSVSSE004_RestoreFarmQuest");
		/*02*/ AddQuest("CC01_Quest02_Key", "$CC01_Quest02_Name", MAIN_QUEST_FLAG, "$CC01_Quest02_Data", "ccBGSSSE001_Start_MQ2");
		/*03*/ AddQuest("CC01_Quest03_Key", "$CC01_Quest03_Name", SIDE_QUEST_FLAG, "$CC01_Quest03_Data", "ccBGSSSE001_Misc_Dwarven");
		/*04*/ AddQuest("CC01_Quest04_Key", "$CC01_Quest04_Name", SIDE_QUEST_FLAG, "$CC01_Quest04_Data", "ccBGSSSE001_Misc_Falkreath");
		/*05*/ AddQuest("CC01_Quest05_Key", "$CC01_Quest05_Name", MAIN_QUEST_FLAG, "$CC01_Quest05_Data", "ccBGSSSE001_Fish_MQ1");
		/*06*/ AddQuest("CC01_Quest06_Key", "$CC01_Quest06_Name", MAIN_QUEST_FLAG, "$CC01_Quest06_Data", "ccBGSSSE001_Fish_MQ2");
		/*07*/ AddQuest("CC01_Quest07_Key", "$CC01_Quest07_Name", MAIN_QUEST_FLAG, "$CC01_Quest07_Data", "ccBGSSSE001_Crab_MQ2");
		/*08*/ AddQuest("CC01_Quest08_Key", "$CC01_Quest08_Name", MAIN_QUEST_FLAG, "$CC01_Quest08_Data", "ccBGSSSE001_Fish_MQ4");
		/*09*/ AddQuest("CC01_Quest09_Key", "$CC01_Quest09_Name", MAIN_QUEST_FLAG, "$CC01_Quest09_Data", "ccBGSSSE001_Crab_MQ3");
		/*10*/ AddQuest("CC01_Quest10_Key", "$CC01_Quest10_Name", MAIN_QUEST_FLAG, "$CC01_Quest10_Data", "ccBGSSSE001_Fish_MQ5");
		/*11*/ AddQuest("CC01_Quest11_Key", "$CC01_Quest11_Name", MAIN_QUEST_FLAG, "$CC01_Quest11_Data", "ccBGSSSE001_Fish_MQ3");
		/*12*/ AddQuest("CC01_Quest12_Key", "$CC01_Quest12_Name", SIDE_QUEST_FLAG, "$CC01_Quest12_Data", "ccBGSSSE001_Misc_WhiterunPartOne");
		/*13*/ AddQuest("CC01_Quest13_Key", "$CC01_Quest13_Name", MAIN_QUEST_FLAG, "$CC01_Quest13_Data", "ccBGSSSE001_Crab_MQ1");
		/*14*/ AddQuest("CC01_Quest14_Key", "$CC01_Quest14_Name", SIDE_QUEST_FLAG, "$CC01_Quest14_Data", "ccBGSSSE001_Misc_Morthal");	
		/*15*/ AddQuest("CC01_Quest15_Key", "$CC01_Quest15_Name", SIDE_QUEST_FLAG, "$CC01_Quest15_Data", "ccBGSSSE001_Misc_Markarth");
		/*16*/ AddQuest("CC01_Quest16_Key", "$CC01_Quest16_Name", SIDE_QUEST_FLAG, "$CC01_Quest16_Data", "ccBGSSSE001_Misc_Khajiit");	
		/*17*/ AddQuest("CC01_Quest17_Key", "$CC01_Quest17_Name", SIDE_QUEST_FLAG, "$CC01_Quest17_Data", "ccBGSSSE001_Misc_Solitude");
		/*18*/ AddQuest("CC01_Quest18_Key", "$CC01_Quest18_Name", SIDE_QUEST_FLAG, "$CC01_Quest18_Data", "ccBGSSSE001_Misc_Windhelm");	
		/*19*/ AddQuest("CC01_Quest19_Key", "$CC01_Quest19_Name", SIDE_QUEST_FLAG, "$CC01_Quest19_Data", "ccBGSSSE001_Misc_WhiterunPartTwo");
		/*20*/ AddQuest("CC01_Quest20_Key", "$CC01_Quest20_Name", MAIN_QUEST_FLAG, "$CC01_Quest20_Data", "ccBGSSSE001_Crab_MQ4");	
		/*21*/ AddQuest("CC01_Quest21_Key", "$CC01_Quest21_Name", RADI_QUEST_FLAG, "$CC01_Quest21_Data", "ccBGSSSE001_Radiant_2");
		/*22*/ AddQuest("CC01_Quest22_Key", "$CC01_Quest22_Name", RADI_QUEST_FLAG, "$CC01_Quest22_Data", "ccBGSSSE001_Radiant_3");
		/*23*/ AddQuest("CC01_Quest23_Key", "$CC01_Quest23_Name", RADI_QUEST_FLAG, "$CC01_Quest23_Data", "ccBGSSSE001_Radiant_1");

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
	};
} 