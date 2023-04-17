#include "CQuests_SkaalVillage.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_SkaalVillage 
{
	CQuestData QuestData[] {
		{"SkaalVillage_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "DLC2SkaalVillageFreeform2"},
		{"SkaalVillage_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DLC2WB01"},
		{"SkaalVillage_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DLC2SV01"},
		{"SkaalVillage_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DLC2SV02"},
		{"SkaalVillage_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "DLC2SkaalVillageFreeform1"},
		{"SkaalVillage_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor104"},
	};

	CRadiantData RadiantData[]{
		{"SkaalVillage_Quest05", CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor104Fanari" },
	};

	CStageData StageData[]{
		{"SkaalVillage_Quest00", CStageEnum::kPast, 30, 0},
		{"SkaalVillage_Quest04", CStageEnum::kPast, 20, 0},
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
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 17));

		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};