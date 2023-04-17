#include "CQuests_Misc_DG.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Misc_DG 
{
	CStageData StageData[]{
	{"Misc_DG_Quest00", CStageEnum::kDone, 60,  0, },
	{"Misc_DG_Quest01", CStageEnum::kDone, 200,	0, },
	{"Misc_DG_Quest03", CStageEnum::kDone, 255, 0, },
	{"Misc_DG_Quest04", CStageEnum::kDone, 255, 0, },
	};

	CQuestData QuestData[] {
	{"Misc_DG_Quest00", CFlagEnum::kSide,  CCompEnum::kStage, "DLC1VQDragon"},
	{"Misc_DG_Quest01", CFlagEnum::kSide,  CCompEnum::kStage, "DLC1VQFVBooks"},
	{"Misc_DG_Quest02", CFlagEnum::kSide,  CCompEnum::kStand, "DLC1VQSaint"},
	{"Misc_DG_Quest03", CFlagEnum::kSide,  CCompEnum::kStage, "DLC1LD"},
	{"Misc_DG_Quest04", CFlagEnum::kSide,  CCompEnum::kStage, "DLC01SoulCairnHorseQuest2"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&QuestArrays)
				->initStageData(StageData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 30));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};