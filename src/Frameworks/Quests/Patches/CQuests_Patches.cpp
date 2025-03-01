#include "CQuests_Patches.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Patches
{
	using cmd = Serialization::CompletionistData;

	//---------------------------------------------------
	//-- Quest Functions ( Legacy of the Dragonborn ) ---
	//---------------------------------------------------

	CRadiantData RadiaData_LD1[]
	{
		{"LOTD_Main_Quest11", CRadiantEnum::kRadiant_LEG, 0x06B90A, 0, 30},
		{"LOTD_Main_Quest12", CRadiantEnum::kRadiant_LEG, 0x740A17, 0, 10},

		{"LOTD_Exp_Quest13",  CRadiantEnum::kRadiant_LEG, 0x5a94c3, 0, 30},
		{"LOTD_Exp_Quest14",  CRadiantEnum::kRadiant_LEG, 0x18DDE8, 0, 6},
		{"LOTD_Exp_Quest15",  CRadiantEnum::kRadiant_LEG, 0x0F0D35, 0, 100},
	};

	CQuestData QuestData_LD1[]
	{
		{"LOTD_Main_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DBM_MuseumIntro"},
		{"LOTD_Main_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DBM_LALStart"},
		{"LOTD_Main_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DBM_RHStartQST"},
		{"LOTD_Main_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DBM_GuildMasterStart"},
		{"LOTD_Main_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DBM_LALStart"},
		{"LOTD_Main_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DBM_HauntedMuseumQuest"},
		{"LOTD_Main_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DBM_MuseumHeist"},
		{"LOTD_Main_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "DBM_ShatteredLegacy"},
		{"LOTD_Main_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "DBM_WaystoneQuest"},
		{"LOTD_Main_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DBM_MuseumIndarysQuest"},
		{"LOTD_Main_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DBM_DHQuest"},
		{"LOTD_Main_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBM_RadiantFindersKeepers"},
		{"LOTD_Main_Quest12", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBM_RadiantResearch"},
		{"LOTD_Main_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "DBM_VigilantsQuest1"},
		{"LOTD_Main_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "DBM_VigilantsQuest2"},
	};

	CQuestData QuestData_LD2[]
	{
		{"LOTD_Misc_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DBM_HOSDiscoveryQST"},
		{"LOTD_Misc_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DBM_MuchAdoAboutSnowElves"},
		{"LOTD_Misc_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DBM_HandofGloryQuest"},
		{"LOTD_Misc_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DBM_HOGCleanseQuest"},
		{"LOTD_Misc_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DBM_OngarKegbreaker"},
		{"LOTD_Misc_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DBM_ImbuningMachine"},
		{"LOTD_Misc_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "DBM_LordsMailQuest"},
		{"LOTD_Misc_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "DBM_KOTNQuest"},
	};

	CQuestData QuestData_LD3[]
	{
		{"LOTD_Notes_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST01"},
		{"LOTD_Notes_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST02"},
		{"LOTD_Notes_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST03"},
		{"LOTD_Notes_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST04"},
		{"LOTD_Notes_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST05"},
		{"LOTD_Notes_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST06"},
		{"LOTD_Notes_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST07"},
		{"LOTD_Notes_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST08"},
		{"LOTD_Notes_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST09"},
		{"LOTD_Notes_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DBM_AMJournalQST10"},
	};

	CQuestData QuestData_LD4[]
	{
		{"LOTD_Journals_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon01QST"},
		{"LOTD_Journals_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon02QST"},
		{"LOTD_Journals_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon03QST"},
		{"LOTD_Journals_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon04QST"},
		{"LOTD_Journals_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon05QST"},
		{"LOTD_Journals_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DBM_GhostChestQST"},
	};

	CQuestData QuestData_LD5[]
	{
		{"LOTD_Exp_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DBM_ExplorerGuildHouse"},
		{"LOTD_Exp_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DBM_ExplorerGuildmembers"},
		{"LOTD_Exp_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DBM_Excavation01"},
		{"LOTD_Exp_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DBM_Excavation02"},
		{"LOTD_Exp_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DBM_Excavation03Prelude"},
		{"LOTD_Exp_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DBM_Excavation03A"},
		{"LOTD_Exp_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DBM_HrormirStaffQuest"},
		{"LOTD_Exp_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "DBM_PlanetariumQuest"},
		{"LOTD_Exp_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "DBM_MalrusCodexQST"},
		{"LOTD_Exp_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DBM_ExplorerRelicFindQST"},
		{"LOTD_Exp_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DBM_ExplorerFieldStation01"},
		{"LOTD_Exp_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "DBM_ExplorerFieldStation02"},
		{"LOTD_Exp_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "DBM_ExplorerFieldStation03"},
		{"LOTD_Exp_Quest13", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBM_RadiantRuinedBooks"},
		{"LOTD_Exp_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBM_SextantHandler"},
		{"LOTD_Exp_Quest15", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBM_ArtifactOrderQuest"},
	};

	//---------------------------------------------------
	//-- Quest Functions ( Install Frameowork ) ---------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		using cmd = Serialization::CompletionistData;

		if (cmd::IsModInstalled("LegacyoftheDragonborn.esm")) {
			InstallLegacyoftheDragonBornQuests();
			CFramework_Master::InstalledPatchesForMCMDisplay++;
		};
	};

	//---------------------------------------------------
	//-- Quest Functions ( Install Quest Data ) ---------
	//---------------------------------------------------

	void CHandler::InstallLegacyoftheDragonBornQuests()
	{
		auto RH_Installed = cmd::IsModInstalled("DBM_RelicHunter.esp");
		auto MP_Installed = cmd::IsModInstalled("DBM_Moonpath_Patch.esp");

		for (auto i = 0; i < std::extent_v<decltype(QuestData_LD1)>; i++)
		{
			// Handle Exclusions
			if ((i == 2 || i == 3) && !RH_Installed) {
				continue;
			}

			if (i == 9 && MP_Installed) {
				QuestData_LD1[i].editor_id = "DBM_MoonpathIndarys";
			}

			QuestData_LD1[i].init()->initRadiantData(RadiaData_LD1)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD1[i], QuestData_LD1[i].GetName(), 52, QuestData_LD1[i].unique_identifier));
		};

		for (auto i = 0; i < std::extent_v<decltype(QuestData_LD2)>; i++)
		{
			QuestData_LD2[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD2[i], QuestData_LD2[i].GetName(), 53, QuestData_LD2[i].unique_identifier));
		};

		for (auto i = 0; i < std::extent_v<decltype(QuestData_LD3)>; i++)
		{
			QuestData_LD3[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD3[i], QuestData_LD3[i].GetName(), 54, QuestData_LD3[i].unique_identifier));
		};

		for (auto i = 0; i < std::extent_v<decltype(QuestData_LD4)>; i++)
		{
			QuestData_LD4[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD4[i], QuestData_LD4[i].GetName(), 55, QuestData_LD4[i].unique_identifier));
		};

		for (auto i = 0; i < std::extent_v<decltype(QuestData_LD5)>; i++)
		{
			QuestData_LD5[i].init()->initRadiantData(RadiaData_LD1)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD5[i], QuestData_LD5[i].GetName(), 56, QuestData_LD5[i].unique_identifier));
		};
	};
};