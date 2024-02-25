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
		auto ND_Installed = cmd::IsModInstalled("KRI_DBMDelayPatch.esp");
		auto ED_Installed = cmd::IsModInstalled("KRI_DBM_EXTRA_DelayPatch.esp");
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

			QuestData_LD1[i].init()
				->override(QuestData_LD1[i].kLocKey, fmt::format("{:s}{}"sv, QuestData_LD1[i].GetKey(), ND_Installed ? "_N" : ED_Installed ? "_E" : "_V").c_str())
				->initRadiantData(RadiaData_LD1)->finalize();
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
			QuestData_LD5[i].init()
				->override(QuestData_LD5[i].kLocKey, fmt::format("{:s}{}"sv, QuestData_LD5[i].GetKey(), ND_Installed ? "_N" : ED_Installed ? "_E" : "_V").c_str())
				->initRadiantData(RadiaData_LD1)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_LD5[i], QuestData_LD5[i].GetName(), 56, QuestData_LD5[i].unique_identifier));
		};
	};

	/*template<size_t N>
	void CHandler::CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, const char* a_mod, const std::string& a_name)
	{
		const std::string cFilePath = fmt::format(R"(.\Data\SKSE\Plugins\CompletionistData\Translations\{})"sv, a_name);

		// Open a file for writing
		std::ofstream outputFile(cFilePath);

		// Check if the file is successfully opened
		if (!outputFile.is_open()) {
			INFO("Error opening the file for writing!")
				return; // Exit with an error code
		}

		outputFile << "[Completionist Patch Data]" << std::endl;
		outputFile << "PatchType = Quest" << std::endl;
		outputFile << "Enabled = True" << std::endl;
		outputFile << "axPlt = 2" << std::endl;
		outputFile << " " << std::endl;

		for (auto idx = 0; idx < N; ++idx)
		{
			outputFile << "[" + a_data[idx].unique_identifier + "]" << std::endl;

			outputFile << "Enabled				= True" << std::endl;
			outputFile << "UUID				= " + a_data[idx].unique_identifier << std::endl;
			outputFile << "QuestLink			= " + std::to_string(i_id) << std::endl;
			outputFile << fmt::format("MCMPageName			= {}", a_page) << std::endl;
			outputFile << " " << std::endl;

			outputFile << fmt::format("PluginFileName		= {}", a_mod) << std::endl;
			outputFile << "EditorID			= " + a_data[idx].editor_id << std::endl;
			outputFile << " " << std::endl;

			outputFile << "QuestType			= " + std::to_string(static_cast<int>(a_data[idx].quest_type)) << std::endl;
			outputFile << "CompletionType		= " + std::to_string(static_cast<int>(a_data[idx].completion_type)) << std::endl;
			outputFile << " " << std::endl;
		};

		// Close the file
		outputFile.close();

		INFO("Data has been written to the file.");

		return; // Exit successfully
	};

	template<size_t N, size_t S>
	void CHandler::CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, CStageData(&a_stage)[S], const std::string& a_name)
	{
		const std::string cFilePath = fmt::format(R"(.\Data\SKSE\Plugins\CompletionistData\Translations\{})"sv, a_name);

		// Open a file for writing
		std::ofstream outputFile(cFilePath);

		// Check if the file is successfully opened
		if (!outputFile.is_open()) {
			INFO("Error opening the file for writing!")
				return; // Exit with an error code
		}

		outputFile << "[Completionist Patch Data]" << std::endl;
		outputFile << "PatchType = Quest" << std::endl;
		outputFile << "Enabled = True" << std::endl;
		outputFile << "axPlt = 1" << std::endl;
		outputFile << " " << std::endl;

		for (auto idx = 0; idx < N; ++idx)
		{
			outputFile << "[" + a_data[idx].unique_identifier + "]" << std::endl;

			outputFile << "Enabled				= True" << std::endl;
			outputFile << "UUID				= " + a_data[idx].unique_identifier << std::endl;
			outputFile << "QuestLink			= " + std::to_string(i_id) << std::endl;
			outputFile << fmt::format("MCMPageName			= {}", a_page) << std::endl;
			outputFile << " " << std::endl;

			outputFile << "EditorID			= " + a_data[idx].editor_id << std::endl;
			outputFile << " " << std::endl;

			outputFile << "QuestType			= " + std::to_string(static_cast<int>(a_data[idx].quest_type)) << std::endl;
			outputFile << "CompletionType		= " + std::to_string(static_cast<int>(a_data[idx].completion_type)) << std::endl;
			outputFile << " " << std::endl;

			outputFile << "QuestName			= " + fmt::format("cTranslate_QuestName{}", idx < 10 ? "0" + std::to_string(idx) : std::to_string(idx)) << std::endl;
			outputFile << "HighlightText		= " + fmt::format("cTranslate_QuestText{}", idx < 10 ? "0" + std::to_string(idx) : std::to_string(idx)) << std::endl;
			outputFile << " " << std::endl;

			for (auto edx = 0; edx < S; ++edx)
			{
				if (DKUtil::string::iequals(a_stage[edx].link, a_data[idx].unique_identifier)) {
					outputFile << "StageType			= " + std::to_string(static_cast<int>(a_stage[edx].type)) << std::endl;
					outputFile << "Stage				= " + std::to_string(a_stage[edx].stage) << std::endl;
					outputFile << "OptionalStage		= " + std::to_string(a_stage[edx].optional_stage) << std::endl;
					outputFile << " " << std::endl;
				}
			}
		};

		// Close the file
		outputFile.close();

		INFO("Data has been written to the file.");

		return; // Exit successfully
	};

	template<size_t N, size_t S>
	void CHandler::CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, CRadiantData(&a_radiant)[S], const char* a_mod, const std::string& a_name)
	{
		const std::string cFilePath = fmt::format(R"(.\Data\SKSE\Plugins\CompletionistData\Translations\{})"sv, a_name);

		// Open a file for writing
		std::ofstream outputFile(cFilePath);

		// Check if the file is successfully opened
		if (!outputFile.is_open()) {
			INFO("Error opening the file for writing!")
				return; // Exit with an error code
		}

		outputFile << "[Completionist Patch Data]" << std::endl;
		outputFile << "PatchType = Quest" << std::endl;
		outputFile << "Enabled = True" << std::endl;
		outputFile << "axPlt = 1" << std::endl;
		outputFile << " " << std::endl;

		for (auto idx = 0; idx < N; ++idx)
		{
			outputFile << "[" + a_data[idx].unique_identifier + "]" << std::endl;

			outputFile << "Enabled				= True" << std::endl;
			outputFile << "UUID				= " + a_data[idx].unique_identifier << std::endl;
			outputFile << "QuestLink			= " + std::to_string(i_id) << std::endl;
			outputFile << fmt::format("MCMPageName			= {}", a_page) << std::endl;
			outputFile << " " << std::endl;

			outputFile << fmt::format("PluginFileName		= {}", a_mod) << std::endl;
			outputFile << "EditorID			= " + a_data[idx].editor_id << std::endl;
			outputFile << " " << std::endl;

			outputFile << "QuestType			= " + std::to_string(static_cast<int>(a_data[idx].quest_type)) << std::endl;
			outputFile << "CompletionType		= " + std::to_string(static_cast<int>(a_data[idx].completion_type)) << std::endl;
			outputFile << " " << std::endl;

			for (auto edx = 0; edx < S; ++edx)
			{
				if (DKUtil::string::iequals(a_radiant[edx].link, a_data[idx].unique_identifier)) {
					outputFile << "TimesRequired		= " + std::to_string(static_cast<int>(a_radiant[edx].value)) << std::endl;
					outputFile << "BaseFormID				= " + std::format("{:08X}", a_radiant[edx].baseID) << std::endl;
					outputFile << "RadiantStage			= " + std::to_string(a_radiant[edx].stage) << std::endl;
					outputFile << " " << std::endl;
				}
			}
		};

		// Close the file
		outputFile.close();

		INFO("Data has been written to the file.");

		return; // Exit successfully
	};*/


};