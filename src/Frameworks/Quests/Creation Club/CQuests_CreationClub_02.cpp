#include "CQuests_CreationClub_02.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC2 {
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
		0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,26,28,29,30,32,34,35,36,37,38,40,41,
		42,43,44,45
	};

	constexpr std::pair<std::size_t, std::uint16_t> StageDoneCompletion[] = {
		{7,  40},
		{27, 30},
		{31, 100},
		{33, 30},
		{39, 1000},
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

		/*00*/ AddQuest("CC02_Quest00_Key", "$CC02_Quest00_Name", MAIN_QUEST_FLAG, "$CC02_Quest00_Data", "ccKRTSSE001_zQuest");
		/*01*/ AddQuest("CC02_Quest01_Key", "$CC02_Quest01_Name", MAIN_QUEST_FLAG, "$CC02_Quest01_Data", "ccBGSSSE005_MainQuest");
		/*02*/ AddQuest("CC02_Quest02_Key", "$CC02_Quest02_Name", MAIN_QUEST_FLAG, "$CC02_Quest02_Data", "ccBGSSSE013_AquisitionQuest");
		/*03*/ AddQuest("CC02_Quest03_Key", "$CC02_Quest03_Name", MAIN_QUEST_FLAG, "$CC02_Quest03_Data", "ccBGSSSE057_Quest");
		/*04*/ AddQuest("CC02_Quest04_Key", "$CC02_Quest04_Name", MAIN_QUEST_FLAG, "$CC02_Quest04_Data", "ccASVSSE001_QuestD");
		/*05*/ AddQuest("CC02_Quest05_Key", "$CC02_Quest05_Name", MAIN_QUEST_FLAG, "$CC02_Quest05_Data", "ccFFBSSE001_Quest");
		/*06*/ AddQuest("CC02_Quest06_Key", "$CC02_Quest06_Name", MAIN_QUEST_FLAG, "$CC02_Quest06_Data", "ccBGSSSE050_Quest");
		/*07*/ AddQuest("CC02_Quest07_Key", "$CC02_Quest07_Name", MAIN_QUEST_FLAG, "$CC02_Quest07_Data", "ccBGSSSE013_BloodthirstQuest");
		/*08*/ AddQuest("CC02_Quest08_Key", "$CC02_Quest08_Name", MAIN_QUEST_FLAG, "$CC02_Quest08_Data", "ccBGSSSE040_Quest");
		/*09*/ AddQuest("CC02_Quest09_Key", "$CC02_Quest09_Name", MAIN_QUEST_FLAG, "$CC02_Quest09_Data", "ccBGSSSE059_Quest");
		/*10*/ AddQuest("CC02_Quest10_Key", "$CC02_Quest10_Name", MAIN_QUEST_FLAG, "$CC02_Quest10_Data", "ccBGSSSE052_Quest");
		/*11*/ AddQuest("CC02_Quest11_Key", "$CC02_Quest11_Name", MAIN_QUEST_FLAG, "$CC02_Quest11_Data", "ccASVSSE001_QuestA");
		/*12*/ AddQuest("CC02_Quest12_Key", "$CC02_Quest12_Name", MAIN_QUEST_FLAG, "$CC02_Quest12_Data", "ccASVSSE001_QuestC");
		/*13*/ AddQuest("CC02_Quest13_Key", "$CC02_Quest13_Name", MAIN_QUEST_FLAG, "$CC02_Quest13_Data", "ccBGSSSE053_Quest");
		/*14*/ AddQuest("CC02_Quest14_Key", "$CC02_Quest14_Name", MAIN_QUEST_FLAG, "$CC02_Quest14_Data", "ccRMSSSE001_Quest");
		/*15*/ AddQuest("CC02_Quest15_Key", "$CC02_Quest15_Name", MAIN_QUEST_FLAG, "$CC02_Quest15_Data", "ccBGSSSE062_Quest");	
		/*16*/ AddQuest("CC02_Quest16_Key", "$CC02_Quest16_Name", MAIN_QUEST_FLAG, "$CC02_Quest16_Data", "DLCDwarvenPuzzleDungeonQuest01");	
		/*17*/ AddQuest("CC02_Quest17_Key", "$CC02_Quest17_Name", MAIN_QUEST_FLAG, "$CC02_Quest17_Data", "ccKRTSSE001_QuestFortune");
		/*18*/ AddQuest("CC02_Quest18_Key", "$CC02_Quest18_Name", MAIN_QUEST_FLAG, "$CC02_Quest18_Data", "ccBGSSSE055_Quest");
		/*19*/ AddQuest("CC02_Quest19_Key", "$CC02_Quest19_Name", MAIN_QUEST_FLAG, "$CC02_Quest19_Data", "ccASVSSE001_Quest");
		/*20*/ AddQuest("CC02_Quest20_Key", "$CC02_Quest20_Name", MAIN_QUEST_FLAG, "$CC02_Quest20_Data", "ccEEJSSE005_Quest");
		/*21*/ AddQuest("CC02_Quest21_Key", "$CC02_Quest21_Name", MAIN_QUEST_FLAG, "$CC02_Quest21_Data", "ccBGSSSE063_Quest");
		/*22*/ AddQuest("CC02_Quest22_Key", "$CC02_Quest22_Name", MAIN_QUEST_FLAG, "$CC02_Quest22_Data", "ccASVSSE001_QuestB");
		/*23*/ AddQuest("CC02_Quest23_Key", "$CC02_Quest23_Name", MAIN_QUEST_FLAG, "$CC02_Quest23_Data", "ccBGSSSE038_Quest");
		/*24*/ AddQuest("CC02_Quest00_Key", "$CC02_Quest00_Name", MAIN_QUEST_FLAG, "$CC02_Quest24_Data", "ccBGSSSE036_PetBWolfAcquisitionQuest");
		/*25*/ AddQuest("CC02_Quest01_Key", "$CC02_Quest01_Name", MAIN_QUEST_FLAG, "$CC02_Quest25_Data", "ccBGSSSE061_Quest");
		/*26*/ AddQuest("CC02_Quest02_Key", "$CC02_Quest02_Name", MAIN_QUEST_FLAG, "$CC02_Quest26_Data", "ccBGSSSE051_Quest");
		/*27*/ AddQuest("CC02_Quest03_Key", "$CC02_Quest03_Name", MAIN_QUEST_FLAG, "$CC02_Quest27_Data", "ccBGSSSE010_PetPurchaseQuest");
		/*28*/ AddQuest("CC02_Quest04_Key", "$CC02_Quest04_Name", MAIN_QUEST_FLAG, "$CC02_Quest28_Data", "ccBGSSSE043_VampireHunterQuest");
		/*29*/ AddQuest("CC02_Quest05_Key", "$CC02_Quest05_Name", MAIN_QUEST_FLAG, "$CC02_Quest29_Data", "ccBGSSSE064_Quest");
		/*30*/ AddQuest("CC02_Quest06_Key", "$CC02_Quest06_Name", MAIN_QUEST_FLAG, "$CC02_Quest30_Data", "ccBGSSSE058_Quest");
		/*31*/ AddQuest("CC02_Quest07_Key", "$CC02_Quest07_Name", MAIN_QUEST_FLAG, "$CC02_Quest31_Data", "ccMTYSSE001_StartupQuest_Alt");
		/*32*/ AddQuest("CC02_Quest08_Key", "$CC02_Quest08_Name", MAIN_QUEST_FLAG, "$CC02_Quest32_Data", "ccBGSSSE054_Quest");
		/*33*/ AddQuest("CC02_Quest09_Key", "$CC02_Quest09_Name", MAIN_QUEST_FLAG, "$CC02_Quest33_Data", "ccBGSSSE002_SoulStealerQuest");
		/*34*/ AddQuest("CC02_Quest10_Key", "$CC02_Quest10_Name", MAIN_QUEST_FLAG, "$CC02_Quest34_Data", "ccBGSSSE002_TelekinesisArrowQuest");
		/*35*/ AddQuest("CC02_Quest11_Key", "$CC02_Quest11_Name", MAIN_QUEST_FLAG, "$CC02_Quest35_Data", "ccPEWSSE002_Quest");
		/*36*/ AddQuest("CC02_Quest12_Key", "$CC02_Quest12_Name", MAIN_QUEST_FLAG, "$CC02_Quest36_Data", "DLCDwarvenPuzzleDungeonCrownQuest");
		/*37*/ AddQuest("CC02_Quest13_Key", "$CC02_Quest13_Name", MAIN_QUEST_FLAG, "$CC02_Quest37_Data", "DLCDwarvenPuzzleDungeonHorseQuest");
		/*38*/ AddQuest("CC02_Quest14_Key", "$CC02_Quest14_Name", MAIN_QUEST_FLAG, "$CC02_Quest38_Data", "ccBGSSSE014_SpellPack_StartupQuest");
		/*39*/ AddQuest("CC02_Quest15_Key", "$CC02_Quest15_Name", MAIN_QUEST_FLAG, "$CC02_Quest39_Data", "ccBGSSSE007_Quest");
		/*40*/ AddQuest("CC02_Quest16_Key", "$CC02_Quest16_Name", MAIN_QUEST_FLAG, "$CC02_Quest40_Data", "ccMTYSSE001_Quest");
		/*41*/ AddQuest("CC02_Quest17_Key", "$CC02_Quest17_Name", MAIN_QUEST_FLAG, "$CC02_Quest41_Data", "ccKRTSSE001_QuestPower");
		/*42*/ AddQuest("CC02_Quest18_Key", "$CC02_Quest18_Name", MAIN_QUEST_FLAG, "$CC02_Quest42_Data", "ccBGSSSE031_QUEST");
		/*43*/ AddQuest("CC02_Quest19_Key", "$CC02_Quest19_Name", MAIN_QUEST_FLAG, "$CC02_Quest43_Data", "ccBGSSSE060_Quest");
		/*44*/ AddQuest("CC02_Quest20_Key", "$CC02_Quest20_Name", MAIN_QUEST_FLAG, "$CC02_Quest44_Data", "ccASVSSE001_QuestE");	
		/*45*/ AddQuest("CC02_Quest21_Key", "$CC02_Quest21_Name", MAIN_QUEST_FLAG, "$CC02_Quest45_Data", "ccBGSSSE056_Quest");
		 
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

		for (auto& [i, stage] : StageDoneCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], stage);
		}
	};
}