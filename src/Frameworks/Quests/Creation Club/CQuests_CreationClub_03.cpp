#include "CQuests_CreationClub_03.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC3 {
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
		0,1,2,4,6,7,8,10,11,12,13,14,17,18,
		21,22,24,25,27,28,29,30,31,32,34,35 
	};

	constexpr std::pair<std::size_t, std::int32_t> StageDoneCompletion[] = {
		{9,  10},
		{15, 25},
		{16, 40},
		{19, 100},
		{20, 20},
		{23, 20},
		{26, 250},
		{33, 20},
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
		 
		/*00*/ AddQuest("CC03_Quest00_Key", "$CC03_Quest00_Name", MAIN_QUEST_FLAG, "$CC03_Quest00_Data", "ccBGSSSE025_MiscQuest_AmberMadnessGear");
		/*01*/ AddQuest("CC03_Quest01_Key", "$CC03_Quest01_Name", MAIN_QUEST_FLAG, "$CC03_Quest01_Data", "ccBGSSSE025_QuestA");
		/*02*/ AddQuest("CC03_Quest02_Key", "$CC03_Quest02_Name", MAIN_QUEST_FLAG, "$CC03_Quest02_Data", "ccBGSSSE068_Quest");
		/*03*/ AddQuest("CC03_Quest03_Key", "$CC03_Quest03_Name", MAIN_QUEST_FLAG, "$CC03_Quest03_Data", "ccBGSSSE069_Quest");
		/*04*/ AddQuest("CC03_Quest04_Key", "$CC03_Quest04_Name", MAIN_QUEST_FLAG, "$CC03_Quest04_Data", "ccBGSSSE034_UnicornQuest");
		/*05*/ AddQuest("CC03_Quest05_Key", "$CC03_Quest05_Name", MAIN_QUEST_FLAG, "$CC03_Quest05_Data", "ccEDHSSE002_Quest");
		/*06*/ AddQuest("CC03_Quest06_Key", "$CC03_Quest06_Name", MAIN_QUEST_FLAG, "$CC03_Quest06_Data", "ccBGSSSE021_LordsMailQuest");
		/*07*/ AddQuest("CC03_Quest07_Key", "$CC03_Quest07_Name", MAIN_QUEST_FLAG, "$CC03_Quest07_Data", "ccBGSSSE025_MiscQuest_GSDSGearBook");
		/*08*/ AddQuest("CC03_Quest08_Key", "$CC03_Quest08_Name", MAIN_QUEST_FLAG, "$CC03_Quest08_Data", "ccEEJSSE004_Quest");
		/*09*/ AddQuest("CC03_Quest09_Key", "$CC03_Quest09_Name", MAIN_QUEST_FLAG, "$CC03_Quest09_Data", "ccBGSSSE034_WildHorsesQuest");
		/*10*/ AddQuest("CC03_Quest10_Key", "$CC03_Quest10_Name", MAIN_QUEST_FLAG, "$CC03_Quest10_Data", "ccBGSSSE006_Quest");
		/*11*/ AddQuest("CC03_Quest11_Key", "$CC03_Quest11_Name", MAIN_QUEST_FLAG, "$CC03_Quest11_Data", "ccEDHSSE003_Quest");
		/*12*/ AddQuest("CC03_Quest12_Key", "$CC03_Quest12_Name", MAIN_QUEST_FLAG, "$CC03_Quest12_Data", "ccBGSSSE008_Quest");
		/*13*/ AddQuest("CC03_Quest13_Key", "$CC03_Quest13_Name", MAIN_QUEST_FLAG, "$CC03_Quest13_Data", "ccBGSSSE041_Quest");
		/*14*/ AddQuest("CC03_Quest14_Key", "$CC03_Quest14_Name", MAIN_QUEST_FLAG, "$CC03_Quest14_Data", "ccBGSSSE025_ElytraPetAcquisition_Dementia");
		/*15*/ AddQuest("CC03_Quest15_Key", "$CC03_Quest15_Name", MAIN_QUEST_FLAG, "$CC03_Quest15_Data", "ccBGSSSE025_ElytraPetAcquisition_Mania");
		/*16*/ AddQuest("CC03_Quest16_Key", "$CC03_Quest16_Name", MAIN_QUEST_FLAG, "$CC03_Quest16_Data", "ccBGSSSE035_PetAcquireQuest");
		/*17*/ AddQuest("CC03_Quest17_Key", "$CC03_Quest17_Name", MAIN_QUEST_FLAG, "$CC03_Quest17_Data", "ccEEJSSE002_MageTowerQuest");
		/*18*/ AddQuest("CC03_Quest18_Key", "$CC03_Quest18_Name", MAIN_QUEST_FLAG, "$CC03_Quest18_Data", "ccBGSSSE025_MiscQuest_Nerveshatter");
		/*19*/ AddQuest("CC03_Quest19_Key", "$CC03_Quest19_Name", MAIN_QUEST_FLAG, "$CC03_Quest19_Data", "ccEDHSSE001_CraftsmanEncounterQuest");
		/*20*/ AddQuest("CC03_Quest20_Key", "$CC03_Quest20_Name", MAIN_QUEST_FLAG, "$CC03_Quest20_Data", "ccVSVSSE002_MainQuest");
		/*21*/ AddQuest("CC03_Quest21_Key", "$CC03_Quest21_Name", MAIN_QUEST_FLAG, "$CC03_Quest21_Data", "ccBGSSSE019_StaffOfSheogorathQuest");
		/*22*/ AddQuest("CC03_Quest22_Key", "$CC03_Quest22_Name", MAIN_QUEST_FLAG, "$CC03_Quest22_Data", "ccBGSSSE025_QuestB");
		/*23*/ AddQuest("CC03_Quest23_Key", "$CC03_Quest23_Name", MAIN_QUEST_FLAG, "$CC03_Quest23_Data", "ccVSVSSE001_MainQuest");	
		/*24*/ AddQuest("CC03_Quest00_Key", "$CC03_Quest00_Name", MAIN_QUEST_FLAG, "$CC03_Quest24_Data", "ccEEJSSE003_HouseQuest");
		/*25*/ AddQuest("CC03_Quest01_Key", "$CC03_Quest01_Name", MAIN_QUEST_FLAG, "$CC03_Quest25_Data", "ccBGSSSE025_StaadaQuest");
		/*26*/ AddQuest("CC03_Quest02_Key", "$CC03_Quest02_Name", MAIN_QUEST_FLAG, "$CC03_Quest26_Data", "ccBGSSSE067_Quest");
		/*27*/ AddQuest("CC03_Quest03_Key", "$CC03_Quest03_Name", MAIN_QUEST_FLAG, "$CC03_Quest27_Data", "ccBGSSSE067_Quest2");
		/*28*/ AddQuest("CC03_Quest04_Key", "$CC03_Quest04_Name", MAIN_QUEST_FLAG, "$CC03_Quest28_Data", "ccBGSSSE004_Quest");
		/*29*/ AddQuest("CC03_Quest05_Key", "$CC03_Quest05_Name", MAIN_QUEST_FLAG, "$CC03_Quest29_Data", "ccBGSSSE020_Quest");
		/*30*/ AddQuest("CC03_Quest06_Key", "$CC03_Quest06_Name", MAIN_QUEST_FLAG, "$CC03_Quest30_Data", "ccBGSSSE003_ZombieQuestStartEncounter");
		/*31*/ AddQuest("CC03_Quest07_Key", "$CC03_Quest07_Name", MAIN_QUEST_FLAG, "$CC03_Quest31_Data", "ccAARSSE001ManufactoryControlQuest");	
		/*32*/ AddQuest("CC03_Quest08_Key", "$CC03_Quest08_Name", MAIN_QUEST_FLAG, "$CC03_Quest32_Data", "ccBGSSSE045_Quest");
		/*33*/ AddQuest("CC03_Quest09_Key", "$CC03_Quest09_Name", MAIN_QUEST_FLAG, "$CC03_Quest33_Data", "BGSSSE018_ShadowrendQuest");
		/*34*/ AddQuest("CC03_Quest10_Key", "$CC03_Quest10_Name", MAIN_QUEST_FLAG, "$CC03_Quest34_Data", "ccBGSSSE016_UmbraMainQuest");
		/*35*/ AddQuest("CC03_Quest11_Key", "$CC03_Quest11_Name", MAIN_QUEST_FLAG, "$CC03_Quest35_Data", "ccMTYSSE002_Quest");	

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