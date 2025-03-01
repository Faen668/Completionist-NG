#include "CQuests_SmallTowns.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_SmallTowns 
{
	CQuestData QuestData[]
	{
		{"SmallTowns_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiverwood01"},
		{"SmallTowns_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "FreeformIvarstead04"},
		{"SmallTowns_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "FreeformDragonBridge01"},
		{"SmallTowns_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "RoriksteadFreeform"},
		{"SmallTowns_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "FreeformIvarstead03"},
		{"SmallTowns_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "FreeformIvarstead01"},
		{"SmallTowns_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShorsStone01"},
		{"SmallTowns_Quest07", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShorsStone02"},
		{"SmallTowns_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "FreeformIvarstead02"},
		{"SmallTowns_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "MS13"},
		{"SmallTowns_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"SmallTowns_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"SmallTowns_Quest12", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"SmallTowns_Quest13", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"SmallTowns_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"SmallTowns_Quest15", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"SmallTowns_Quest16", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"SmallTowns_Quest17", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"SmallTowns_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
	};

	CRadiantData RadiantData[]{
		{"SmallTowns_Quest10",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest11",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest12",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest13",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest14",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest15",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest16",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest17",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"SmallTowns_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"SmallTowns_Quest00", CStageEnum::kPast, 300,	0},
		{"SmallTowns_Quest01", CStageEnum::kPast, 199,	0},
		{"SmallTowns_Quest02", CStageEnum::kPast, 19,	0},
		{"SmallTowns_Quest03", CStageEnum::kPast, 29,	0},
		{"SmallTowns_Quest04", CStageEnum::kPast, 199,	0},
		{"SmallTowns_Quest05", CStageEnum::kPast, 199,	0},
		{"SmallTowns_Quest06", CStageEnum::kPast, 199,	0},
		{"SmallTowns_Quest07", CStageEnum::kPast, 199,	0},
		{"SmallTowns_Quest08", CStageEnum::kPast, 199,	0},
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
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 20, QuestData[i].unique_identifier));
		}
	};
};