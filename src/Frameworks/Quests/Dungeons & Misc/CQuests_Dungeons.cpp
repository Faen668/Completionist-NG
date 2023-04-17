#include "CQuests_Dungeons.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Dungeons 
{
	CStageData StageData[]{
		{"Dungeons_Quest00", CStageEnum::kDone, 254		, 0 },
		{"Dungeons_Quest06", CStageEnum::kDone, 100		, 0 },
		{"Dungeons_Quest09", CStageEnum::kDone, 81		, 0 },
		{"Dungeons_Quest10", CStageEnum::kDone, 100		, 0 },
		{"Dungeons_Quest13", CStageEnum::kDone, 30		, 0 },
		{"Dungeons_Quest14", CStageEnum::kDone, 30		, 0 },
		{"Dungeons_Quest15", CStageEnum::kDone, 20		, 0 },
		{"Dungeons_Quest19", CStageEnum::kDone, 20		, 0 },
		{"Dungeons_Quest21", CStageEnum::kDone, 100		, 0 },
	};

	CQuestData QuestData[] {
		{"Dungeons_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "dunAngarvundeQST"},
		{"Dungeons_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "dunAnsilvundQST"},
		{"Dungeons_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "dunBlindCliffQST"},
		{"Dungeons_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "dunDarklightQST"},
		{"Dungeons_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "dunForelhostQST"},
		{"Dungeons_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "dunFrostflowAbyssQST"},
		{"Dungeons_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "dunFrostmereCryptQST"},
		{"Dungeons_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "dunHighGateRuinsQST"},
		{"Dungeons_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "dunHillgrundsTombQST"},
		{"Dungeons_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "dunIronbindQST"},
		{"Dungeons_Quest10", CFlagEnum::kSide, CCompEnum::kStage, "dunLiarsRetreatQST"},
		{"Dungeons_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "dunMistwatchQST"},
		{"Dungeons_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "dunNilheimQST"},
		{"Dungeons_Quest13", CFlagEnum::kSide, CCompEnum::kStage, "dunRagnvaldQST"},
		{"Dungeons_Quest14", CFlagEnum::kSide, CCompEnum::kStage, "dunRebelsCairnQST"},
		{"Dungeons_Quest15", CFlagEnum::kSide, CCompEnum::kStage, "dunRobbersGorgeQST"},
		{"Dungeons_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "dunSouthfringeQST"},
		{"Dungeons_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "dunTrevasWatchQST"},
		{"Dungeons_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "dunValthumeQST"},
		{"Dungeons_Quest19", CFlagEnum::kSide, CCompEnum::kStage, "dunVolskyggeQST"},
		{"Dungeons_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "dunVolunruudQST"},
		{"Dungeons_Quest21", CFlagEnum::kSide, CCompEnum::kStage, "dunYngolBarrowQST"},
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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 27));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};