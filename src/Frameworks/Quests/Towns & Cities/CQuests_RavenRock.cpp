#include "CQuests_RavenRock.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_RavenRock 
{
	CQuestData QuestData[]{
		{"RavenRock_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RR03Intro"},
		{"RavenRock_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "DLC2RRFavor03"},
		{"RavenRock_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "DLC2RRFavor06"},
		{"RavenRock_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "DLC2RRFavor02"},
		{"RavenRock_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "DLC2RRFavor07"},
		{"RavenRock_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RR03"},
		{"RavenRock_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RRFavor04"},
		{"RavenRock_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RR01"},
		{"RavenRock_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TGQuest"},
		{"RavenRock_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "DLC2RRFavor05"},
		{"RavenRock_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RRFavor01"},
		{"RavenRock_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "DLC2RR02"},
	};

	CStageData StageData[]{
		{"RavenRock_Quest01", CStageEnum::kPast, 199, 0},
		{"RavenRock_Quest02", CStageEnum::kPast, 199, 0},
		{"RavenRock_Quest03", CStageEnum::kPast, 199, 0},
		{"RavenRock_Quest04", CStageEnum::kPast, 199, 0},
		{"RavenRock_Quest09", CStageEnum::kDone, 30,  0},
	};

	CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray, &KeysArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData)
				->initStageData(StageData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 16, QuestData[i].unique_identifier));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};