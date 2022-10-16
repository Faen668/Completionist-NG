#include "CQuests_CreationClub_03.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC3 {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"CC03_Quest00_Key", "$CC03_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest00_Data", "ccBGSSSE025_MiscQuest_AmberMadnessGear"},
		/*01*/ {"CC03_Quest01_Key", "$CC03_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest01_Data", "ccBGSSSE025_QuestA"},
		/*02*/ {"CC03_Quest02_Key", "$CC03_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest02_Data", "ccBGSSSE068_Quest"},
		/*03*/ {"CC03_Quest03_Key", "$CC03_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest03_Data", "ccBGSSSE069_Quest"},
		/*04*/ {"CC03_Quest04_Key", "$CC03_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest04_Data", "ccBGSSSE034_UnicornQuest"},
		/*05*/ {"CC03_Quest05_Key", "$CC03_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest05_Data", "ccEDHSSE002_Quest"},
		/*06*/ {"CC03_Quest06_Key", "$CC03_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest06_Data", "ccBGSSSE021_LordsMailQuest"},
		/*07*/ {"CC03_Quest07_Key", "$CC03_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest07_Data", "ccBGSSSE025_MiscQuest_GSDSGearBook"},
		/*08*/ {"CC03_Quest08_Key", "$CC03_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest08_Data", "ccEEJSSE004_Quest"},
		/*09*/ {"CC03_Quest09_Key", "$CC03_Quest09_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest09_Data", "ccBGSSSE034_WildHorsesQuest"},
		/*10*/ {"CC03_Quest10_Key", "$CC03_Quest10_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest10_Data", "ccBGSSSE006_Quest"},
		/*11*/ {"CC03_Quest11_Key", "$CC03_Quest11_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest11_Data", "ccEDHSSE003_Quest"},
		/*12*/ {"CC03_Quest12_Key", "$CC03_Quest12_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest12_Data", "ccBGSSSE008_Quest"},
		/*13*/ {"CC03_Quest13_Key", "$CC03_Quest13_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest13_Data", "ccBGSSSE041_Quest"},
		/*14*/ {"CC03_Quest14_Key", "$CC03_Quest14_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest14_Data", "ccBGSSSE025_ElytraPetAcquisition_Dementia"},
		/*15*/ {"CC03_Quest15_Key", "$CC03_Quest15_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest15_Data", "ccBGSSSE025_ElytraPetAcquisition_Mania"},
		/*16*/ {"CC03_Quest16_Key", "$CC03_Quest16_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest16_Data", "ccBGSSSE035_PetAcquireQuest"},
		/*17*/ {"CC03_Quest17_Key", "$CC03_Quest17_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest17_Data", "ccEEJSSE002_MageTowerQuest"},
		/*18*/ {"CC03_Quest18_Key", "$CC03_Quest18_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest18_Data", "ccBGSSSE025_MiscQuest_Nerveshatter"},
		/*19*/ {"CC03_Quest19_Key", "$CC03_Quest19_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest19_Data", "ccEDHSSE001_CraftsmanEncounterQuest"},
		/*20*/ {"CC03_Quest20_Key", "$CC03_Quest20_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest20_Data", "ccVSVSSE002_MainQuest"},
		/*21*/ {"CC03_Quest21_Key", "$CC03_Quest21_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest21_Data", "ccBGSSSE019_StaffOfSheogorathQuest"},
		/*22*/ {"CC03_Quest22_Key", "$CC03_Quest22_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest22_Data", "ccBGSSSE025_QuestB"},
		/*23*/ {"CC03_Quest23_Key", "$CC03_Quest23_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest23_Data", "ccVSVSSE001_MainQuest"},
		/*24*/ {"CC03_Quest24_Key", "$CC03_Quest24_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest24_Data", "ccEEJSSE003_HouseQuest"},
		/*25*/ {"CC03_Quest25_Key", "$CC03_Quest25_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest25_Data", "ccBGSSSE025_StaadaQuest"},
		/*26*/ {"CC03_Quest26_Key", "$CC03_Quest26_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest26_Data", "ccBGSSSE067_Quest"},
		/*27*/ {"CC03_Quest27_Key", "$CC03_Quest27_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest27_Data", "ccBGSSSE067_Quest2"},
		/*28*/ {"CC03_Quest28_Key", "$CC03_Quest28_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest28_Data", "ccBGSSSE004_Quest"},
		/*29*/ {"CC03_Quest29_Key", "$CC03_Quest29_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest29_Data", "ccBGSSSE020_Quest"},
		/*30*/ {"CC03_Quest30_Key", "$CC03_Quest30_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest30_Data", "ccBGSSSE003_ZombieQuestStartEncounter"},
		/*31*/ {"CC03_Quest31_Key", "$CC03_Quest31_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest31_Data", "ccAARSSE001ManufactoryControlQuest"},
		/*32*/ {"CC03_Quest32_Key", "$CC03_Quest32_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest32_Data", "ccBGSSSE045_Quest"},
		/*33*/ {"CC03_Quest33_Key", "$CC03_Quest33_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_Y, "$CC03_Quest33_Data", "BGSSSE018_ShadowrendQuest"},
		/*34*/ {"CC03_Quest34_Key", "$CC03_Quest34_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest34_Data", "ccBGSSSE016_UmbraMainQuest"},
		/*35*/ {"CC03_Quest35_Key", "$CC03_Quest35_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$CC03_Quest35_Data", "ccMTYSSE002_Quest"},
	};

	constexpr std::size_t StandardCompletion[] = { 
		0,1,2,4,6,7,8,10,11,12,13,14,17,18,
		21,22,24,25,27,28,29,30,31,32,34,35 
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
		{ 9,  10  },
		{ 15, 25  },
		{ 16, 40  },
		{ 19, 100 },
		{ 20, 20  },
		{ 23, 20  },
		{ 26, 250 },
		{ 33, 20  },
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