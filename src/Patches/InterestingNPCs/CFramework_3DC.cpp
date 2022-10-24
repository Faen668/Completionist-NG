#include "Serialization.hpp"
#include "CFramework_3DC.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_3DC {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest1_QuestData[] = {
		/*00*/ {"3DNPC_Main_Quest00_Key", "$3DNPC_Main_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest00_Data", "DJGQuest"},
		/*01*/ {"3DNPC_Main_Quest01_Key", "$3DNPC_Main_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest01_Data", "ErevanQuest"},
		/*02*/ {"3DNPC_Main_Quest02_Key", "$3DNPC_Main_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest02_Data", "DJGQuest2"},
		/*03*/ {"3DNPC_Main_Quest03_Key", "$3DNPC_Main_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest03_Data", "AmaleeQuest"},
		/*04*/ {"3DNPC_Main_Quest04_Key", "$3DNPC_Main_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest04_Data", "hagquest"},
		/*05*/ {"3DNPC_Main_Quest05_Key", "$3DNPC_Main_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest05_Data", "RaynesGriffithQuest"},
		/*06*/ {"3DNPC_Main_Quest06_Key", "$3DNPC_Main_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest06_Data", "CWQQ2"},
		/*07*/ {"3DNPC_Main_Quest07_Key", "$3DNPC_Main_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest07_Data", "AnumLaQuest"},
		/*08*/ {"3DNPC_Main_Quest08_Key", "$3DNPC_Main_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest08_Data", "AmicusDialogue"},
		/*09*/ {"3DNPC_Main_Quest09_Key", "$3DNPC_Main_Quest09_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest09_Data", "GorrQuest"},
		/*10*/ {"3DNPC_Main_Quest10_Key", "$3DNPC_Main_Quest10_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest10_Data", "warrensquest"},
		/*11*/ {"3DNPC_Main_Quest11_Key", "$3DNPC_Main_Quest11_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest11_Data", "DialogueNair"},
		/*12*/ {"3DNPC_Main_Quest12_Key", "$3DNPC_Main_Quest12_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest12_Data", "ZoraQuest"},
		/*13*/ {"3DNPC_Main_Quest13_Key", "$3DNPC_Main_Quest13_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest13_Data", "Herranquest"},
		/*14*/ {"3DNPC_Main_Quest14_Key", "$3DNPC_Main_Quest14_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest14_Data", "ArilwaenQuest"},
		/*15*/ {"3DNPC_Main_Quest15_Key", "$3DNPC_Main_Quest15_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest15_Data", "DialogueJasparGaerston"},
		/*16*/ {"3DNPC_Main_Quest16_Key", "$3DNPC_Main_Quest16_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest16_Data", "RDQuest"},
		/*17*/ {"3DNPC_Main_Quest17_Key", "$3DNPC_Main_Quest17_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest17_Data", "AsteriaQuest"},
		/*18*/ {"3DNPC_Main_Quest18_Key", "$3DNPC_Main_Quest18_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest18_Data", "RumarinQuest2"},
		/*19*/ {"3DNPC_Main_Quest19_Key", "$3DNPC_Main_Quest19_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest19_Data", "AldiQuest"},
		/*20*/ {"3DNPC_Main_Quest20_Key", "$3DNPC_Main_Quest20_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest20_Data", "SvashniQuest"},
		/*21*/ {"3DNPC_Main_Quest21_Key", "$3DNPC_Main_Quest21_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest21_Data", "HorkerQuest"},
		/*22*/ {"3DNPC_Main_Quest22_Key", "$3DNPC_Main_Quest22_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest22_Data", "KjoliQuest"},
		/*23*/ {"3DNPC_Main_Quest23_Key", "$3DNPC_Main_Quest23_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Main_Quest23_Data", "ViranyaQuest"},
	};

	constexpr std::size_t Quest1_StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest2_QuestData[] = {
		/*00*/ {"3DNPC_BOK_Quest00_Key", "$3DNPC_BOK_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest00_Data", "3DNPCMQ1"},
		/*01*/ {"3DNPC_BOK_Quest01_Key", "$3DNPC_BOK_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest01_Data", "3DNPCMQ2"},
		/*02*/ {"3DNPC_BOK_Quest02_Key", "$3DNPC_BOK_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest02_Data", "3DNPCMQ3"},
		/*03*/ {"3DNPC_BOK_Quest03_Key", "$3DNPC_BOK_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest03_Data", "3DNPCMQ4"},
		/*04*/ {"3DNPC_BOK_Quest04_Key", "$3DNPC_BOK_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest04_Data", "3DNPCMQ5"},
		/*05*/ {"3DNPC_BOK_Quest05_Key", "$3DNPC_BOK_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest05_Data", "3DNPCMQ6"},
		/*06*/ {"3DNPC_BOK_Quest06_Key", "$3DNPC_BOK_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_BOK_Quest06_Data", "3DNPCMQ7"},
	};

	constexpr std::size_t Quest2_StandardCompletion[] = {
		0,1,2,3,4,5,6
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest3_QuestData[] = {
		/*00*/ {"3DNPC_DS_Quest00_Key", "$3DNPC_DS_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest00_Data", "3dmcue"},
		/*01*/ {"3DNPC_DS_Quest01_Key", "$3DNPC_DS_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest01_Data", "3dmcue2"},
		/*02*/ {"3DNPC_DS_Quest02_Key", "$3DNPC_DS_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest02_Data", "3dmcue3"},
		/*03*/ {"3DNPC_DS_Quest03_Key", "$3DNPC_DS_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest03_Data", "3dmcue4"},
		/*04*/ {"3DNPC_DS_Quest04_Key", "$3DNPC_DS_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest04_Data", "3dmcue5"},
		/*05*/ {"3DNPC_DS_Quest05_Key", "$3DNPC_DS_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_DS_Quest05_Data", "3dmcue6"},
	};

	constexpr std::size_t Quest3_StandardCompletion[] = {
		0,1,2,3,4,5
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest4_QuestData[] = {
		/*00*/ {"3DNPC_Misc_Quest00_Key", "$3DNPC_Misc_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest00_Data", "Jurykquest"},
		/*01*/ {"3DNPC_Misc_Quest01_Key", "$3DNPC_Misc_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest01_Data", "Rumarinquest"},
		/*02*/ {"3DNPC_Misc_Quest02_Key", "$3DNPC_Misc_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest02_Data", "Paintingquest"},
		/*03*/ {"3DNPC_Misc_Quest03_Key", "$3DNPC_Misc_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest03_Data", "CarmellaSkjarnQuest"},
		/*04*/ {"3DNPC_Misc_Quest04_Key", "$3DNPC_Misc_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest04_Data", "DialogueAzzarian"},
		/*05*/ {"3DNPC_Misc_Quest05_Key", "$3DNPC_Misc_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest05_Data", "Lurgokquest"},
		/*06*/ {"3DNPC_Misc_Quest06_Key", "$3DNPC_Misc_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest06_Data", "BlackRobesQuest"},
		/*07*/ {"3DNPC_Misc_Quest07_Key", "$3DNPC_Misc_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest07_Data", "Arielequest"},
		/*08*/ {"3DNPC_Misc_Quest08_Key", "$3DNPC_Misc_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest08_Data", "Morndasargonianquest"},
		/*09*/ {"3DNPC_Misc_Quest09_Key", "$3DNPC_Misc_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest09_Data", "DialogueBrakh"},
		/*10*/ {"3DNPC_Misc_Quest10_Key", "$3DNPC_Misc_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest10_Data", "Beatricequest"},
		/*11*/ {"3DNPC_Misc_Quest11_Key", "$3DNPC_Misc_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest11_Data", "DialogueTheRelic"},
		/*12*/ {"3DNPC_Misc_Quest12_Key", "$3DNPC_Misc_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest12_Data", "RobbersRefuge3DNPC"},
		/*13*/ {"3DNPC_Misc_Quest13_Key", "$3DNPC_Misc_Quest13_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest13_Data", "CassockQuest"},
		/*14*/ {"3DNPC_Misc_Quest14_Key", "$3DNPC_Misc_Quest14_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest14_Data", "Skjelquest"},
		/*15*/ {"3DNPC_Misc_Quest15_Key", "$3DNPC_Misc_Quest15_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest15_Data", "DialogueYtharil"},
		/*16*/ {"3DNPC_Misc_Quest16_Key", "$3DNPC_Misc_Quest16_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest16_Data", "DialogueDarrakki"},
		/*17*/ {"3DNPC_Misc_Quest17_Key", "$3DNPC_Misc_Quest17_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest17_Data", "Hagcursequest"},
		/*18*/ {"3DNPC_Misc_Quest18_Key", "$3DNPC_Misc_Quest18_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest18_Data", "Vartheimquest"},
		/*19*/ {"3DNPC_Misc_Quest19_Key", "$3DNPC_Misc_Quest19_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest19_Data", "HWSQuest"},
		/*20*/ {"3DNPC_Misc_Quest20_Key", "$3DNPC_Misc_Quest20_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest20_Data", "DialogueRaynes"},
		/*21*/ {"3DNPC_Misc_Quest21_Key", "$3DNPC_Misc_Quest21_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest21_Data", "DialogueJilkmar"},
		/*22*/ {"3DNPC_Misc_Quest22_Key", "$3DNPC_Misc_Quest22_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest22_Data", "Bookquest3dnpc"},
		/*23*/ {"3DNPC_Misc_Quest23_Key", "$3DNPC_Misc_Quest23_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest23_Data", "DialogueThriceBitten"},
		/*24*/ {"3DNPC_Misc_Quest24_Key", "$3DNPC_Misc_Quest24_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$3DNPC_Misc_Quest24_Data", "Terynnequest"},
	};

	constexpr std::size_t Quest4_StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
	};

	constexpr Serialization::FormArray Items = {
	0x186CD2,0x28493D,0x1B2920,0x0DF4B8,0x1067D8,0x0DF4BA,0x0DF4BB,
	0x1FF21B,0x19A951,0x0DF4B7,0x19853F,0x0E97FD,0x22B94F,0x12E113,
	0x1A6EF3,0x27F184,0x1D8052,0x18F49B,0x1A6EF7,0x2A25FB,0x200C93,
	0x1B018C,0x2BE76E,0x500F44,0x363BF7,0x363BF6,0x152283,0x16A25B,
	0x1C99CF,0x14E0DD,0x144638,0x19A970,0x148844,0x2CB194,0x12A149,
	0x18A9F9,0x1C3AC4,0x1C3AC3,0x1C3AC6,0x238527,0x1C5CDB,0x1ACFFA,
	0x0DE9E9,0x14E0DE,0x1915E0,0x205FD9,0x1F3B47,0x0CDC78,0x182E25,
	};

	constexpr Serialization::FormArray Books = {
	0x272F9B,0x02B06F,0x130790,0x1A37C7,0x1A118C,0x13A92F,0x058932,
	0x190A9B,0x191023,0x191024,0x19159D,0x130792,0x01652C,0x130D40,
	0x1B018E,0x12F1B5,0x1CED4E,0x1AE651,0x1B4638,
	};

	constexpr Serialization::FormArray MapMa = {
	0x21908B,0x30846C,0x19DBFB,0x2C5C97,0x29844D,0x2A269C,0x380979,
	0x28BD0E,0x1A7F98,0x1DBB14,0x1DA7F2,0x248CAF,0x1FD608,0x1C2283,
	0x245AA4,0x17CB26,0x1CC676,0x1D40FC,0x24E51F,0x1BD250,
	};
	// clang-format on

	constexpr std::string_view modname = "3DNPC.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework() {

		Quest1_KeysArray.clear();
		Quest1_NameArray.clear();
		Quest1_TextArray.clear();
		Quest1_IdenArray.clear();
		Quest1_RadiArray.clear();
		Quest1_StgeArray.clear();

		Quest2_KeysArray.clear();
		Quest2_NameArray.clear();
		Quest2_TextArray.clear();
		Quest2_IdenArray.clear();
		Quest2_RadiArray.clear();
		Quest2_StgeArray.clear();

		Quest3_KeysArray.clear();
		Quest3_NameArray.clear();
		Quest3_TextArray.clear();
		Quest3_IdenArray.clear();
		Quest3_RadiArray.clear();
		Quest3_StgeArray.clear();

		Quest4_KeysArray.clear();
		Quest4_NameArray.clear();
		Quest4_TextArray.clear();
		Quest4_IdenArray.clear();
		Quest4_RadiArray.clear();
		Quest4_StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text, id] : Quest1_QuestData) {
			Quest1_KeysArray.push_back(key);
			Quest1_NameArray.push_back(name);
			Quest1_RadiArray.push_back(flag);
			Quest1_TextArray.push_back(text);
			Quest1_IdenArray.push_back(id);
			Quest1_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest2_QuestData) {
			Quest2_KeysArray.push_back(key);
			Quest2_NameArray.push_back(name);
			Quest2_RadiArray.push_back(flag);
			Quest2_TextArray.push_back(text);
			Quest2_IdenArray.push_back(id);
			Quest2_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest3_QuestData) {
			Quest3_KeysArray.push_back(key);
			Quest3_NameArray.push_back(name);
			Quest3_RadiArray.push_back(flag);
			Quest3_TextArray.push_back(text);
			Quest3_IdenArray.push_back(id);
			Quest3_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest4_QuestData) {
			Quest4_KeysArray.push_back(key);
			Quest4_NameArray.push_back(name);
			Quest4_RadiArray.push_back(flag);
			Quest4_TextArray.push_back(text);
			Quest4_IdenArray.push_back(id);
			Quest4_StgeArray.push_back(isStageDone);
		}

		assert(Quest1_KeysArray.size() == ArraySize);
		assert(Quest1_IdenArray.size() == ArraySize);
		assert(Quest1_NameArray.size() == ArraySize);
		assert(Quest1_RadiArray.size() == ArraySize);
		assert(Quest1_TextArray.size() == ArraySize);
		assert(Quest1_StgeArray.size() == ArraySize);
		Quest1_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest2_KeysArray.size() == ArraySize);
		assert(Quest2_IdenArray.size() == ArraySize);
		assert(Quest2_NameArray.size() == ArraySize);
		assert(Quest2_RadiArray.size() == ArraySize);
		assert(Quest2_TextArray.size() == ArraySize);
		assert(Quest2_StgeArray.size() == ArraySize);
		Quest2_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest3_KeysArray.size() == ArraySize);
		assert(Quest3_IdenArray.size() == ArraySize);
		assert(Quest3_NameArray.size() == ArraySize);
		assert(Quest3_RadiArray.size() == ArraySize);
		assert(Quest3_TextArray.size() == ArraySize);
		assert(Quest3_StgeArray.size() == ArraySize);
		Quest3_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest4_KeysArray.size() == ArraySize);
		assert(Quest4_IdenArray.size() == ArraySize);
		assert(Quest4_NameArray.size() == ArraySize);
		assert(Quest4_RadiArray.size() == ArraySize);
		assert(Quest4_TextArray.size() == ArraySize);
		assert(Quest4_StgeArray.size() == ArraySize);
		Quest4_BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(Quest1_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest1_IdenArray.end()) { 
			if (Quest1_StgeArray.at(std::distance(Quest1_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest1_KeysArray.at(std::distance(Quest1_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest1_IdenArray.at(std::distance(Quest1_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest2_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest2_IdenArray.end()) {
			if (Quest2_StgeArray.at(std::distance(Quest2_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest2_KeysArray.at(std::distance(Quest2_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest2_IdenArray.at(std::distance(Quest2_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest3_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest3_IdenArray.end()) {
			if (Quest3_StgeArray.at(std::distance(Quest3_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest3_KeysArray.at(std::distance(Quest3_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest3_IdenArray.at(std::distance(Quest3_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest4_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest4_IdenArray.end()) {
			if (Quest4_StgeArray.at(std::distance(Quest4_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest4_KeysArray.at(std::distance(Quest4_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest4_IdenArray.at(std::distance(Quest4_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_3DC_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_3DC_Items::Data.GetBase(a_event->baseObj) ? CPatch_3DC_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_3DC_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_3DC_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_3DC_Books::Data.GetBase(target) ? CPatch_3DC_Books::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "NotifyBooks");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i);
			}
		}

		if (a_event->menuName == RE::JournalMenu::MENU_NAME) {
			CHandler::UpdateQuestFramework();
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_3DC_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Books::Data.GetAllVariations()) {
				if (CPatch_3DC_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_3DC_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_3DC_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Items::Data.GetAllVariations()) {
				if (CPatch_3DC_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_3DC_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_marker);
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_3DC_Items::Data.CompileFormArray(CPatch_3DC::Items, modname);
		CPatch_3DC_Books::Data.CompileFormArray(CPatch_3DC::Books, modname);
		CPatch_3DC_MapMa::Data.CompileFormArray(CPatch_3DC::MapMa, modname);

		CPatch_3DC_Items::Data.MergeAsCollectable();
		CPatch_3DC_Books::Data.MergeAsCollectable();

		CPatch_3DC_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_3DC_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_3DC_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_3DC_Items::Data);
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateQuestFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i : Quest1_StandardCompletion) {
			Quest1_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest1_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest1_KeysArray[i], Quest1_IdenArray[i]);
		};

		for (auto i : Quest2_StandardCompletion) {
			Quest2_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest2_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest2_KeysArray[i], Quest2_IdenArray[i]);
		};

		for (auto i : Quest3_StandardCompletion) {
			Quest3_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest3_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest3_KeysArray[i], Quest3_IdenArray[i]);
		};

		for (auto i : Quest4_StandardCompletion) {
			Quest4_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest4_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest4_KeysArray[i], Quest4_IdenArray[i]);
		};
	}
}