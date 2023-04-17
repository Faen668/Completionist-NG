#include "CFramework_LOD.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CPatch_LOD 
{
	CRadiantData RadiantData[] 
	{
		{"LOTD_Main_Quest11", CRadiantEnum::kRadiant_LEG, 0x06B90A, 0x000000, 30, "Completionist_Favor_FindersKeepers"},
		{"LOTD_Main_Quest12", CRadiantEnum::kRadiant_LEG, 0x740A17, 0x000000, 10, "Completionist_Favor_ResearchProject"},

		{"LOTD_Exp_Quest13",  CRadiantEnum::kRadiant_LEG, 0x5a94c3, 0x000000, 30, "Completionist_Favor_OneMansJunk"},
		{"LOTD_Exp_Quest14",  CRadiantEnum::kRadiant_LEG, 0x18DDE8, 0x000000, 6,  "Completionist_Favor_RelicHunt"},
	};

	CQuestData Quest1_Data[] 
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
	};

	CQuestData Quest2_Data[]
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

	CQuestData Quest3_Data[]
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

	CQuestData Quest4_Data[]
	{
		{"LOTD_Journals_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon01QST"},
		{"LOTD_Journals_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon02QST"},
		{"LOTD_Journals_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon03QST"},
		{"LOTD_Journals_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon04QST"},
		{"LOTD_Journals_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DBM_JournalDungeon05QST"},
		{"LOTD_Journals_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DBM_GhostChestQST"},
	};

	CQuestData Quest5_Data[]
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
	};

	CArrayData ArrayData1{ &Quest1_IdenArray, &Quest1_NameArray, &Quest1_TextArray, &Quest1_BoolArray, &Quest1_RadiArray };
	CArrayData ArrayData2{ &Quest2_IdenArray, &Quest2_NameArray, &Quest2_TextArray, &Quest2_BoolArray, &Quest2_RadiArray };
	CArrayData ArrayData3{ &Quest3_IdenArray, &Quest3_NameArray, &Quest3_TextArray, &Quest3_BoolArray, &Quest3_RadiArray };
	CArrayData ArrayData4{ &Quest4_IdenArray, &Quest4_NameArray, &Quest4_TextArray, &Quest4_BoolArray, &Quest4_RadiArray };
	CArrayData ArrayData5{ &Quest5_IdenArray, &Quest5_NameArray, &Quest5_TextArray, &Quest5_BoolArray, &Quest5_RadiArray };

	constexpr std::string_view modname = "LegacyoftheDragonborn.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		auto ND_Installed = Serialization::CompletionistData::IsModInstalled("KRI_DBMDelayPatch.esp");
		auto ED_Installed = Serialization::CompletionistData::IsModInstalled("KRI_DBM_EXTRA_DelayPatch.esp");

		for (auto i = 0; i < std::extent_v<decltype(Quest1_Data)>; i++)
		{
			if (Quest1_Data[i].editor_id == "DBM_MuseumIndarysQuest" && Serialization::CompletionistData::IsModInstalled("DBM_Moonpath_Patch.esp")) {
				Quest1_Data[i].editor_id = "DBM_MoonpathIndarys";
			}

			Quest1_Data[i].init()
				->initQuestData(&ArrayData1)
				->initRadiantData(RadiantData)
				->override(Quest1_Data[i].kData, fmt::format("${:s}_Data{}"sv, Quest1_Data[i].GetKey(), ND_Installed ? "_ND" : ED_Installed ? "_ED" : "_VD"));
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest1_Data[i], Quest1_Data[i].GetName(), 52));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest2_Data)>; i++)
		{
			Quest2_Data[i].init()
				->initQuestData(&ArrayData2)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest2_Data[i], Quest2_Data[i].GetName(), 53));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest3_Data)>; i++)
		{
			Quest3_Data[i].init()
				->initQuestData(&ArrayData3)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest3_Data[i], Quest3_Data[i].GetName(), 54));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest4_Data)>; i++)
		{
			Quest4_Data[i].init()
				->initQuestData(&ArrayData4)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest4_Data[i], Quest4_Data[i].GetName(), 55));
		}

		for (auto i = 0; i < std::extent_v<decltype(Quest5_Data)>; i++)
		{
			Quest5_Data[i].init()
				->initQuestData(&ArrayData5)
				->initRadiantData(RadiantData)
				->override(Quest5_Data[i].kData, fmt::format("${:s}_Data{}"sv, Quest5_Data[i].GetKey(), ND_Installed ? "_ND" : ED_Installed ? "_ED" : "_VD"));
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&Quest5_Data[i], Quest5_Data[i].GetName(), 56));
		}

		Quest1_BoolArray = std::vector<bool>(CArraySize, false);
		Quest2_BoolArray = std::vector<bool>(CArraySize, false);
		Quest3_BoolArray = std::vector<bool>(CArraySize, false);
		Quest4_BoolArray = std::vector<bool>(CArraySize, false);
		Quest5_BoolArray = std::vector<bool>(CArraySize, false);
	};
}