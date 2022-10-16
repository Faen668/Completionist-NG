#include "Serialization.hpp"
#include "CFramework_LOD.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_LOD {
	using namespace Serialization;
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*, const char*, const char*> Quest1_QuestData[] = {
		/*00*/ {"LOTD_Main_Quest00_Key", "$LOTD_Main_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest00_Data_VD", "$LOTD_Main_Quest00_Data_ND", "$LOTD_Main_Quest00_Data_ED", "DBM_MuseumIntro"},
		/*01*/ {"LOTD_Main_Quest01_Key", "$LOTD_Main_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest01_Data_VD", "$LOTD_Main_Quest01_Data_ND", "$LOTD_Main_Quest01_Data_ED", "DBM_LALStart"},
		/*02*/ {"LOTD_Main_Quest02_Key", "$LOTD_Main_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest02_Data_VD", "$LOTD_Main_Quest02_Data_ND", "$LOTD_Main_Quest02_Data_ED", "DBM_RHStartQST"},
		/*03*/ {"LOTD_Main_Quest03_Key", "$LOTD_Main_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest03_Data_VD", "$LOTD_Main_Quest03_Data_ND", "$LOTD_Main_Quest03_Data_ED", "DBM_GuildMasterStart"},
		/*04*/ {"LOTD_Main_Quest04_Key", "$LOTD_Main_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest04_Data_VD", "$LOTD_Main_Quest04_Data_ND", "$LOTD_Main_Quest04_Data_ED", "DBM_LALStart"},
		/*05*/ {"LOTD_Main_Quest05_Key", "$LOTD_Main_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest05_Data_VD", "$LOTD_Main_Quest05_Data_ND", "$LOTD_Main_Quest05_Data_ED", "DBM_HauntedMuseumQuest"},
		/*06*/ {"LOTD_Main_Quest06_Key", "$LOTD_Main_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest06_Data_VD", "$LOTD_Main_Quest06_Data_ND", "$LOTD_Main_Quest06_Data_ED", "DBM_MuseumHeist"},
		/*07*/ {"LOTD_Main_Quest07_Key", "$LOTD_Main_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest07_Data_VD", "$LOTD_Main_Quest07_Data_ND", "$LOTD_Main_Quest07_Data_ED", "DBM_ShatteredLegacy"},
		/*08*/ {"LOTD_Main_Quest08_Key", "$LOTD_Main_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest08_Data_VD", "$LOTD_Main_Quest08_Data_ND", "$LOTD_Main_Quest08_Data_ED", "DBM_WaystoneQuest"},
		/*09*/ {"LOTD_Main_Quest09_Key", "$LOTD_Main_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest09_Data_VD", "$LOTD_Main_Quest09_Data_ND", "$LOTD_Main_Quest09_Data_ED", "DBM_MuseumIndarysQuest"},
		/*10*/ {"LOTD_Main_Quest10_Key", "$LOTD_Main_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest10_Data_VD", "$LOTD_Main_Quest10_Data_ND", "$LOTD_Main_Quest10_Data_ED", "DBM_DHQuest"},
		/*11*/ {"LOTD_Main_Quest11_Key", "$LOTD_Main_Quest11_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest11_Data_VD", "$LOTD_Main_Quest11_Data_ND", "$LOTD_Main_Quest11_Data_ED", "DBM_RadiantFindersKeepers"},
		/*12*/ {"LOTD_Main_Quest12_Key", "$LOTD_Main_Quest12_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Main_Quest12_Data_VD", "$LOTD_Main_Quest12_Data_ND", "$LOTD_Main_Quest12_Data_ED", "DBM_RadiantResearch"},
	};

	constexpr std::size_t Quest1_StandardCompletion[] = {
	0,1,2,3,4,5,6,7,8,9,10
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> Quest1_GlobalCompletion[] = {
	{11,  "Completionist_Favor_FindersKeepers",		LEGACY_COUNTER_VALUE},
	{12,  "Completionist_Favor_ResearchProject",	LEGACY_COUNTER_VALUE},
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest2_QuestData[] = {
		/*00*/ {"LOTD_Misc_Quest00_Key", "$LOTD_Misc_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest00_Data", "DBM_HOSDiscoveryQST"},
		/*01*/ {"LOTD_Misc_Quest01_Key", "$LOTD_Misc_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest01_Data", "DBM_MuchAdoAboutSnowElves"},
		/*02*/ {"LOTD_Misc_Quest02_Key", "$LOTD_Misc_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest02_Data", "DBM_HandofGloryQuest"},
		/*03*/ {"LOTD_Misc_Quest03_Key", "$LOTD_Misc_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest03_Data", "DBM_HOGCleanseQuest"},
		/*04*/ {"LOTD_Misc_Quest04_Key", "$LOTD_Misc_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest04_Data", "DBM_OngarKegbreaker"},
		/*05*/ {"LOTD_Misc_Quest05_Key", "$LOTD_Misc_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest05_Data", "DBM_ImbuningMachine"},
		/*06*/ {"LOTD_Misc_Quest06_Key", "$LOTD_Misc_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest06_Data", "DBM_LordsMailQuest"},
		/*07*/ {"LOTD_Misc_Quest07_Key", "$LOTD_Misc_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Misc_Quest07_Data", "DBM_KOTNQuest"},
	};

	constexpr std::size_t Quest2_StandardCompletion[] = {
		0,1,2,3,4,5,6,7
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest3_QuestData[] = {
		/*00*/ {"LOTD_Notes_Quest00_Key", "$LOTD_Notes_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest00_Data", "DBM_AMJournalQST01"},
		/*01*/ {"LOTD_Notes_Quest01_Key", "$LOTD_Notes_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest01_Data", "DBM_AMJournalQST02"},
		/*02*/ {"LOTD_Notes_Quest02_Key", "$LOTD_Notes_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest02_Data", "DBM_AMJournalQST03"},
		/*03*/ {"LOTD_Notes_Quest03_Key", "$LOTD_Notes_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest03_Data", "DBM_AMJournalQST04"},
		/*04*/ {"LOTD_Notes_Quest04_Key", "$LOTD_Notes_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest04_Data", "DBM_AMJournalQST05"},
		/*05*/ {"LOTD_Notes_Quest05_Key", "$LOTD_Notes_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest05_Data", "DBM_AMJournalQST06"},
		/*06*/ {"LOTD_Notes_Quest06_Key", "$LOTD_Notes_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest06_Data", "DBM_AMJournalQST07"},
		/*07*/ {"LOTD_Notes_Quest07_Key", "$LOTD_Notes_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest07_Data", "DBM_AMJournalQST08"},
		/*08*/ {"LOTD_Notes_Quest08_Key", "$LOTD_Notes_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest08_Data", "DBM_AMJournalQST09"},
		/*09*/ {"LOTD_Notes_Quest09_Key", "$LOTD_Notes_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Notes_Quest09_Data", "DBM_AMJournalQST10"},
	};

	constexpr std::size_t Quest3_StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest4_QuestData[] = {
		/*00*/ {"LOTD_Journals_Quest00_Key", "$LOTD_Journals_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest00_Data", "DBM_JournalDungeon01QST"},
		/*01*/ {"LOTD_Journals_Quest01_Key", "$LOTD_Journals_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest01_Data", "DBM_JournalDungeon02QST"},
		/*02*/ {"LOTD_Journals_Quest02_Key", "$LOTD_Journals_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest02_Data", "DBM_JournalDungeon03QST"},
		/*03*/ {"LOTD_Journals_Quest03_Key", "$LOTD_Journals_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest03_Data", "DBM_JournalDungeon04QST"},
		/*04*/ {"LOTD_Journals_Quest04_Key", "$LOTD_Journals_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest04_Data", "DBM_JournalDungeon05QST"},
		/*05*/ {"LOTD_Journals_Quest05_Key", "$LOTD_Journals_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Journals_Quest05_Data", "DBM_GhostChestQST"},
	};

	constexpr std::size_t Quest4_StandardCompletion[] = {
		0,1,2,3,4,5
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*, const char*, const char*> Quest5_QuestData[] = {
		/*00*/ {"LOTD_Exp_Quest00_Key", "$LOTD_Exp_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest00_Data_VD", "$LOTD_Exp_Quest00_Data_ND", "$LOTD_Exp_Quest00_Data_ED", "DBM_ExplorerGuildHouse"},
		/*01*/ {"LOTD_Exp_Quest01_Key", "$LOTD_Exp_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest01_Data_VD", "$LOTD_Exp_Quest01_Data_ND", "$LOTD_Exp_Quest01_Data_ED", "DBM_ExplorerGuildmembers"},
		/*02*/ {"LOTD_Exp_Quest02_Key", "$LOTD_Exp_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest02_Data_VD", "$LOTD_Exp_Quest02_Data_ND", "$LOTD_Exp_Quest02_Data_ED", "DBM_Excavation01"},
		/*03*/ {"LOTD_Exp_Quest03_Key", "$LOTD_Exp_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest03_Data_VD", "$LOTD_Exp_Quest03_Data_ND", "$LOTD_Exp_Quest03_Data_ED", "DBM_Excavation02"},
		/*04*/ {"LOTD_Exp_Quest04_Key", "$LOTD_Exp_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest04_Data_VD", "$LOTD_Exp_Quest04_Data_ND", "$LOTD_Exp_Quest04_Data_ED", "DBM_Excavation03Prelude"},
		/*05*/ {"LOTD_Exp_Quest05_Key", "$LOTD_Exp_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest05_Data_VD", "$LOTD_Exp_Quest05_Data_ND", "$LOTD_Exp_Quest05_Data_ED", "DBM_Excavation03A"},
		/*06*/ {"LOTD_Exp_Quest06_Key", "$LOTD_Exp_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest06_Data_VD", "$LOTD_Exp_Quest06_Data_ND", "$LOTD_Exp_Quest06_Data_ED", "DBM_HrormirStaffQuest"},
		/*07*/ {"LOTD_Exp_Quest07_Key", "$LOTD_Exp_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest07_Data_VD", "$LOTD_Exp_Quest07_Data_ND", "$LOTD_Exp_Quest07_Data_ED", "DBM_PlanetariumQuest"},
		/*08*/ {"LOTD_Exp_Quest08_Key", "$LOTD_Exp_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest08_Data_VD", "$LOTD_Exp_Quest08_Data_ND", "$LOTD_Exp_Quest08_Data_ED", "DBM_MalrusCodexQST"},
		/*09*/ {"LOTD_Exp_Quest09_Key", "$LOTD_Exp_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest09_Data_VD", "$LOTD_Exp_Quest09_Data_ND", "$LOTD_Exp_Quest09_Data_ED", "DBM_ExplorerRelicFindQST"},
		/*10*/ {"LOTD_Exp_Quest10_Key", "$LOTD_Exp_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest10_Data_VD", "$LOTD_Exp_Quest10_Data_ND", "$LOTD_Exp_Quest10_Data_ED", "DBM_ExplorerFieldStation01"},
		/*11*/ {"LOTD_Exp_Quest11_Key", "$LOTD_Exp_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest11_Data_VD", "$LOTD_Exp_Quest11_Data_ND", "$LOTD_Exp_Quest11_Data_ED", "DBM_ExplorerFieldStation02"},
		/*12*/ {"LOTD_Exp_Quest12_Key", "$LOTD_Exp_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest12_Data_VD", "$LOTD_Exp_Quest12_Data_ND", "$LOTD_Exp_Quest12_Data_ED", "DBM_ExplorerFieldStation03"},
		/*13*/ {"LOTD_Exp_Quest13_Key", "$LOTD_Exp_Quest13_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest13_Data_VD", "$LOTD_Exp_Quest13_Data_ND", "$LOTD_Exp_Quest13_Data_ED", "DBM_RadiantRuinedBooks"},
		/*14*/ {"LOTD_Exp_Quest14_Key", "$LOTD_Exp_Quest14_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$LOTD_Exp_Quest14_Data_VD", "$LOTD_Exp_Quest14_Data_ND", "$LOTD_Exp_Quest14_Data_ED", "DBM_SextantHandler"},
	};

	constexpr std::size_t Quest5_StandardCompletion[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> Quest5_GlobalCompletion[] = {
	{13,  "Completionist_Favor_OneMansJunk",	LEGACY_COUNTER_VALUE},
	{14,  "Completionist_Favor_RelicHunt",		LEGACY_COUNTER_VALUE},
	};

	// clang-format on

	constexpr std::string_view modname = "LegacyoftheDragonborn.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		CHandler::SinkEvents();
		CHandler::InstallQuestFramework();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework() {

		auto ND_Installed = CompletionistData::IsModInstalled("KRI_DBMDelayPatch.esp");
		auto ED_Installed = CompletionistData::IsModInstalled("KRI_DBM_EXTRA_DelayPatch.esp");
		auto MP_Installed = CompletionistData::IsModInstalled("DBM_Moonpath_Patch.esp");

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

		Quest5_KeysArray.clear();
		Quest5_NameArray.clear();
		Quest5_TextArray.clear();
		Quest5_IdenArray.clear();
		Quest5_RadiArray.clear();
		Quest5_StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text_VD, text_ND, text_ED, id] : Quest1_QuestData) {
			Quest1_KeysArray.push_back(key);
			Quest1_NameArray.push_back(name);
			Quest1_RadiArray.push_back(flag);
			Quest1_TextArray.push_back(ND_Installed ? text_ND : ED_Installed ? text_ED : text_VD);
			Quest1_IdenArray.push_back(key == "LOTD_Main_Quest09_Key" && MP_Installed ? "DBM_MoonpathIndarys" : id);
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

		for (auto& [key, name, flag, isStageDone, text_VD, text_ND, text_ED, id] : Quest5_QuestData) {
			Quest5_KeysArray.push_back(key);
			Quest5_NameArray.push_back(name);
			Quest5_RadiArray.push_back(flag);
			Quest5_TextArray.push_back(ND_Installed ? text_ND : ED_Installed ? text_ED : text_VD);
			Quest5_IdenArray.push_back(id);
			Quest5_StgeArray.push_back(isStageDone);
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

		assert(Quest5_KeysArray.size() == ArraySize);
		assert(Quest5_IdenArray.size() == ArraySize);
		assert(Quest5_NameArray.size() == ArraySize);
		assert(Quest5_RadiArray.size() == ArraySize);
		assert(Quest5_TextArray.size() == ArraySize);
		assert(Quest5_StgeArray.size() == ArraySize);
		Quest5_BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

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
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::JournalMenu::MENU_NAME) {
			CHandler::UpdateQuestFramework();
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateQuestFramework() {

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

		for (auto i : Quest5_StandardCompletion) {
			Quest5_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest5_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest5_KeysArray[i], Quest5_IdenArray[i]);
		};

		for (auto& [i, global, value] : Quest1_GlobalCompletion) {
			Quest1_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest1_KeysArray[i]) || FrameworkAPI::IsCompleted_G(Quest1_KeysArray[i], Quest1_IdenArray[i], global, value);
		}

		for (auto& [i, global, value] : Quest5_GlobalCompletion) {
			Quest5_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest5_KeysArray[i]) || FrameworkAPI::IsCompleted_G(Quest5_KeysArray[i], Quest5_IdenArray[i], global, value);
		}
	}
}