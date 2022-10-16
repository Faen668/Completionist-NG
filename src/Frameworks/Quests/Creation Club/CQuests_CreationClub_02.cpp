#include "CQuests_CreationClub_02.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC2 {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"CC02_Quest00_Key", "$CC02_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest00_Data", "ccKRTSSE001_zQuest"},
		/*01*/ {"CC02_Quest01_Key", "$CC02_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest01_Data", "ccBGSSSE005_MainQuest"},
		/*02*/ {"CC02_Quest02_Key", "$CC02_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest02_Data", "ccBGSSSE013_AquisitionQuest"},
		/*03*/ {"CC02_Quest03_Key", "$CC02_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest03_Data", "ccBGSSSE057_Quest"},
		/*04*/ {"CC02_Quest04_Key", "$CC02_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest04_Data", "ccASVSSE001_QuestD"},
		/*05*/ {"CC02_Quest05_Key", "$CC02_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest05_Data", "ccFFBSSE001_Quest"},
		/*06*/ {"CC02_Quest06_Key", "$CC02_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest06_Data", "ccBGSSSE050_Quest"},
		/*07*/ {"CC02_Quest07_Key", "$CC02_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC02_Quest07_Data", "ccBGSSSE013_BloodthirstQuest"},
		/*08*/ {"CC02_Quest08_Key", "$CC02_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest08_Data", "ccBGSSSE040_Quest"},
		/*09*/ {"CC02_Quest09_Key", "$CC02_Quest09_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest09_Data", "ccBGSSSE059_Quest"},
		/*10*/ {"CC02_Quest10_Key", "$CC02_Quest10_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest10_Data", "ccBGSSSE052_Quest"},
		/*11*/ {"CC02_Quest11_Key", "$CC02_Quest11_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest11_Data", "ccASVSSE001_QuestA"},
		/*12*/ {"CC02_Quest12_Key", "$CC02_Quest12_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest12_Data", "ccASVSSE001_QuestC"},
		/*13*/ {"CC02_Quest13_Key", "$CC02_Quest13_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest13_Data", "ccBGSSSE053_Quest"},
		/*14*/ {"CC02_Quest14_Key", "$CC02_Quest14_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest14_Data", "ccRMSSSE001_Quest"},
		/*15*/ {"CC02_Quest15_Key", "$CC02_Quest15_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest15_Data", "ccBGSSSE062_Quest"},
		/*16*/ {"CC02_Quest16_Key", "$CC02_Quest16_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest16_Data", "DLCDwarvenPuzzleDungeonQuest01"},
		/*17*/ {"CC02_Quest17_Key", "$CC02_Quest17_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest17_Data", "ccKRTSSE001_QuestFortune"},
		/*18*/ {"CC02_Quest18_Key", "$CC02_Quest18_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest18_Data", "ccBGSSSE055_Quest"},
		/*19*/ {"CC02_Quest19_Key", "$CC02_Quest19_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest19_Data", "ccASVSSE001_Quest"},
		/*20*/ {"CC02_Quest20_Key", "$CC02_Quest20_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest20_Data", "ccEEJSSE005_Quest"},
		/*21*/ {"CC02_Quest21_Key", "$CC02_Quest21_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest21_Data", "ccBGSSSE063_Quest"},
		/*22*/ {"CC02_Quest22_Key", "$CC02_Quest22_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest22_Data", "ccASVSSE001_QuestB"},
		/*23*/ {"CC02_Quest23_Key", "$CC02_Quest23_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest23_Data", "ccBGSSSE038_Quest"},
		/*24*/ {"CC02_Quest24_Key", "$CC02_Quest24_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest24_Data", "ccBGSSSE036_PetBWolfAcquisitionQuest"},
		/*25*/ {"CC02_Quest25_Key", "$CC02_Quest25_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest25_Data", "ccBGSSSE061_Quest"},
		/*26*/ {"CC02_Quest26_Key", "$CC02_Quest26_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest26_Data", "ccBGSSSE051_Quest"},
		/*27*/ {"CC02_Quest27_Key", "$CC02_Quest27_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC02_Quest27_Data", "ccBGSSSE010_PetPurchaseQuest"},
		/*28*/ {"CC02_Quest28_Key", "$CC02_Quest28_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest28_Data", "ccBGSSSE043_VampireHunterQuest"},
		/*29*/ {"CC02_Quest29_Key", "$CC02_Quest29_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest29_Data", "ccBGSSSE064_Quest"},
		/*30*/ {"CC02_Quest30_Key", "$CC02_Quest30_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest30_Data", "ccBGSSSE058_Quest"},
		/*31*/ {"CC02_Quest31_Key", "$CC02_Quest31_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC02_Quest31_Data", "ccMTYSSE001_StartupQuest_Alt"},
		/*32*/ {"CC02_Quest32_Key", "$CC02_Quest32_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest32_Data", "ccBGSSSE054_Quest"},
		/*33*/ {"CC02_Quest33_Key", "$CC02_Quest33_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC02_Quest33_Data", "ccBGSSSE002_SoulStealerQuest"},
		/*34*/ {"CC02_Quest34_Key", "$CC02_Quest34_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest34_Data", "ccBGSSSE002_TelekinesisArrowQuest"},
		/*35*/ {"CC02_Quest35_Key", "$CC02_Quest35_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest35_Data", "ccPEWSSE002_Quest"},
		/*36*/ {"CC02_Quest36_Key", "$CC02_Quest36_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest36_Data", "DLCDwarvenPuzzleDungeonCrownQuest"},
		/*37*/ {"CC02_Quest37_Key", "$CC02_Quest37_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest37_Data", "DLCDwarvenPuzzleDungeonHorseQuest"},
		/*38*/ {"CC02_Quest38_Key", "$CC02_Quest38_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest38_Data", "ccBGSSSE014_SpellPack_StartupQuest"},
		/*39*/ {"CC02_Quest39_Key", "$CC02_Quest39_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC02_Quest39_Data", "ccBGSSSE007_Quest"},
		/*40*/ {"CC02_Quest40_Key", "$CC02_Quest40_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest40_Data", "ccMTYSSE001_Quest"},
		/*41*/ {"CC02_Quest41_Key", "$CC02_Quest41_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest41_Data", "ccKRTSSE001_QuestPower"},
		/*42*/ {"CC02_Quest42_Key", "$CC02_Quest42_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest42_Data", "ccBGSSSE031_QUEST"},
		/*43*/ {"CC02_Quest43_Key", "$CC02_Quest43_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest43_Data", "ccBGSSSE060_Quest"},
		/*44*/ {"CC02_Quest44_Key", "$CC02_Quest44_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest44_Data", "ccASVSSE001_QuestE"},
		/*45*/ {"CC02_Quest45_Key", "$CC02_Quest45_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC02_Quest45_Data", "ccBGSSSE056_Quest"},
	};

	constexpr std::size_t StandardCompletion[] = { 
		0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,20,
		21,22,23,24,25,26,28,29,30,32,34,35,36,37,38,40,41,
		42,43,44,45
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
		{ 7,  40  },
		{ 27, 30  },
		{ 31, 100 },
		{ 33, 30  }, 
		{ 39, 1000},
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
		StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text, id] : QuestData) {
			KeysArray.push_back(key);
			NameArray.push_back(name);
			RadiArray.push_back(flag);
			TextArray.push_back(text);
			IdenArray.push_back(id);
			StgeArray.push_back(isStageDone);
		}

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		assert(StgeArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto userinterface = RE::UI::GetSingleton();
		userinterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(IdenArray, quest->GetFormEditorID());
		if (t_pos == IdenArray.end()) { return EventResult::kContinue; }


		if (StgeArray.at(std::distance(IdenArray.begin(), t_pos))) {
			CQuestKeys_Stages.AddStage(KeysArray.at(std::distance(IdenArray.begin(), t_pos)), a_event->stage);
			INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, IdenArray.at(std::distance(IdenArray.begin(), t_pos)));
		}
		return EventResult::kContinue;
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

		for (auto& [i, stage] : StageCompletion) {
			BoolArray[i] = StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}