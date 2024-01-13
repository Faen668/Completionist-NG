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
		{"SkaalVillage_Quest05", CFlagEnum::kRadi, CCompEnum::kFavor, "Favor104"},
	};

	CFavorData FavorData[]{
		{"SkaalVillage_Quest05", 0x06FD64, "Skyrim.esm", 0x018fc5, "Dragonborn.esm", CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"SkaalVillage_Quest00", CStageEnum::kPast, 30, 0},
		{"SkaalVillage_Quest04", CStageEnum::kPast, 20, 0},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initStageData(StageData)->initFavorData(FavorData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 17, QuestData[i].unique_identifier));

		}
	};
};