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
		/*05*/ {"Morthal_Quest05", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor001"},
		/*06*/ {"Morthal_Quest06", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor017"},
		/*07*/ {"Morthal_Quest07", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		/*08*/ {"Morthal_Quest08", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"}
	};

	CRadiantData RadiantData[]{
		/*03*/ {"Morthal_Quest03", CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_Favor_ChopWoodHroggar" },
		/*04*/ {"Morthal_Quest04", CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_Favor_ChopWoodJorgen"  },
		/*05*/ {"Morthal_Quest05", CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor001Idgrod"		  },
		/*06*/ {"Morthal_Quest06", CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor017Benor"		  },
		/*07*/ {"Morthal_Quest07", CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor110Lami"		  },
	};

	CStageData StageData[]{
		{"Morthal_Quest00", CStageEnum::kDone, 5, 10},
		{"Morthal_Quest01", CStageEnum::kPast, 10, 0},
	};

	CThaneData ThaneData[]{
		{"Morthal_Quest08", "HjaalmarchImpGetOutofJail", "HjaalmarchSonsGetOutofJail"},
	};

	/*NA*/ CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData)
				->initStageData(StageData)
				->initRadiantData(RadiantData)
				->initThaneData(ThaneData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 10));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};