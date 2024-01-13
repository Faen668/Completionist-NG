#include "CQuests_Misc_DB.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Misc_DB 
{	
	CStageData StageData[] {
		{"Misc_DB_Quest00", CStageEnum::kDone, 200},
		{"Misc_DB_Quest02", CStageEnum::kDone, 10},
		{"Misc_DB_Quest04", CStageEnum::kDone, 30},
		{"Misc_DB_Quest05", CStageEnum::kDone, 600},
	};

	CQuestData QuestData[] {
		{"Misc_DB_Quest00", CFlagEnum::kSide,  CCompEnum::kStage, "DLC2dunHaknirTreasureQST"},
		{"Misc_DB_Quest01", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2EbonyWarriorQuest"},
		{"Misc_DB_Quest02", CFlagEnum::kSide,  CCompEnum::kStage, "DLC2dunHrodulf"},
		{"Misc_DB_Quest03", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2KagrumezQST"},
		{"Misc_DB_Quest04", CFlagEnum::kSide,  CCompEnum::kStage, "DLC2dunKarstaagQST"},
		{"Misc_DB_Quest05", CFlagEnum::kSide,  CCompEnum::kStage, "DLC2dunKolbjornQST"},
		{"Misc_DB_Quest06", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2BlackBook03Quest"},
		{"Misc_DB_Quest07", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2BlackBook04Quest"},
		{"Misc_DB_Quest08", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2BlackBook05Quest"},
		{"Misc_DB_Quest09", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2BlackBook06Quest"},
		{"Misc_DB_Quest10", CFlagEnum::kSide,  CCompEnum::kStand, "DLC2BlackBook07Quest"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initStageData(StageData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 31, QuestData[i].unique_identifier));
		}
	};
};