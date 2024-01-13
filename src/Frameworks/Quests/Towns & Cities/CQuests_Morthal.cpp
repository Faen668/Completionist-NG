#include "CQuests_Morthal.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Morthal 
{
	CQuestData QuestData[] {
		/*00*/ {"Morthal_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMorthalA"},
		/*01*/ {"Morthal_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMorthalB"},
		/*02*/ {"Morthal_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "MS14"},
		/*03*/ {"Morthal_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*04*/ {"Morthal_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*05*/ {"Morthal_Quest05", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001"},
		/*06*/ {"Morthal_Quest06", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017"},
		/*07*/ {"Morthal_Quest07", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		/*08*/ {"Morthal_Quest08", CFlagEnum::kSide, CCompEnum::kThane, "Favor255"}
	};

	CFavorData FavorData[]{
		/*05*/ {"Morthal_Quest05", 0x0403B1, "Skyrim.esm", 0x0135ec, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*06*/ {"Morthal_Quest06", 0x0CA213, "Skyrim.esm", 0x0135e8, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*07*/ {"Morthal_Quest07", 0x0CA43B, "Skyrim.esm", 0x0135ef, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		/*03*/ {"Morthal_Quest03", CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		/*04*/ {"Morthal_Quest04", CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Morthal_Quest00", CStageEnum::kDone, 5, 10},
		{"Morthal_Quest01", CStageEnum::kPast, 10, 0},
	};

	CThaneData ThaneData[]{
		{"Morthal_Quest08", "HjaalmarchImpGetOutofJail", "HjaalmarchSonsGetOutofJail"},
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
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 10, QuestData[i].unique_identifier));
		}
	};
};