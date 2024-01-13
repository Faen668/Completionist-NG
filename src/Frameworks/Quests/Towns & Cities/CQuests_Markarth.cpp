#include "CQuests_Markarth.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Markarth 
{
	CQuestData QuestData[]
	{
		{"Markarth_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthJ"},
		{"Markarth_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "WIAddItem07"},
		{"Markarth_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthN"},
		{"Markarth_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthD"},
		{"Markarth_Quest04", CFlagEnum::kSide, CCompEnum::kFavor, "Favor151"},
		{"Markarth_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthE"},
		{"Markarth_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthO"},
		{"Markarth_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "MS02"},
		{"Markarth_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthM"},
		{"Markarth_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthL"},
		{"Markarth_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "MS01"},
		{"Markarth_Quest11", CFlagEnum::kSide, CCompEnum::kStage, "T01DegaineFavor"},
		{"Markarth_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "T01"},
		{"Markarth_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "DA10"},
		{"Markarth_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "dunNchuandZelQst"},
		{"Markarth_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DA11Intro"},
		{"Markarth_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "DA11"},
		{"Markarth_Quest17", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Markarth_Quest18", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001"},
		{"Markarth_Quest19", CFlagEnum::kSide, CCompEnum::kFavor, "Favor157"},
		{"Markarth_Quest20", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017"},
		{"Markarth_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Markarth_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Markarth_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Markarth_Quest24", CFlagEnum::kSide, CCompEnum::kThane, "Favor250"},
	};

	CFavorData FavorData[]{
		{"Markarth_Quest04", 0x06C825, "Skyrim.esm", 0x01339f, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Markarth_Quest17", 0x0CA20A, "Skyrim.esm", 0x0133ae, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Markarth_Quest18", 0x0403B1, "Skyrim.esm", 0x09a7a8, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Markarth_Quest19", 0x04B766, "Skyrim.esm", 0x01339f, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Markarth_Quest20", 0x0CA213, "Skyrim.esm", 0x013390, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		{"Markarth_Quest21",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"Markarth_Quest22",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"Markarth_Quest23",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Markarth_Quest00", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest01", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest02", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest03", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest05", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest06", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest08", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest09", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest11", CStageEnum::kPast, 20,	0},
	};

	CDrunkData DrunkData[]{
		{"Markarth_Quest22", 0x0072EA2, 0x001F325, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Markarth_Quest23", 0x0072EA2, 0x00198FA, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CThaneData ThaneData[]{
		{"Markarth_Quest24", "ReachImpGetOutofJail", "ReachSonsGetOutofJail"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initStageData(StageData)
				->initRadiantData(RadiantData)
				->initThaneData(ThaneData)
				->initDrunkData(DrunkData)
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 9, QuestData[i].unique_identifier));
		}
	};
};