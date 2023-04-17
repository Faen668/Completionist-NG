#include "CQuests_TelMithryn.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_TelMithryn 
{
	CQuestData QuestData[] {
		{"TelMithryn_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "DLC2TTR7"},
		{"TelMithryn_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR5"},
		{"TelMithryn_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR4a"},
		{"TelMithryn_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR4b"},
		{"TelMithryn_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTF1"},
		{"TelMithryn_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTF2"},
		{"TelMithryn_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR2"},
		{"TelMithryn_Quest07", CFlagEnum::kSide, CCompEnum::kStage, "DLC2TTR8"},
		{"TelMithryn_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR1"},
		{"TelMithryn_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TT1b"},
		{"TelMithryn_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TT2"},
		{"TelMithryn_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TT1"},
		{"TelMithryn_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR3a"},
		{"TelMithryn_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "DLC2TTR3b"},
	};

	CStageData StageData[]{
		{"TelMithryn_Quest00", CStageEnum::kDone, 300, 0},
		{"TelMithryn_Quest07", CStageEnum::kDone, 100, 0},
	};

	CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 18));

		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};