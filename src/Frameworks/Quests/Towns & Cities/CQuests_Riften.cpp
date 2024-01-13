#include "CQuests_Riften.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Riften
{
	CQuestData QuestData[] {
		{"Riften_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften19"},
		{"Riften_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften16"},
		{"Riften_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften11"},
		{"Riften_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften08"},
		{"Riften_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften04"},
		{"Riften_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften09"},
		{"Riften_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften12"},
		{"Riften_Quest07", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften14"},
		{"Riften_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften07"},
		{"Riften_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften13"},
		{"Riften_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "MS03"},
		{"Riften_Quest11", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften18"},
		{"Riften_Quest12", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften06"},
		{"Riften_Quest13", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften15"},
		{"Riften_Quest14", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften05"},
		{"Riften_Quest15", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften22"},
		{"Riften_Quest16", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften10"},
		{"Riften_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "FreeformRiften01"},
		{"Riften_Quest18", CFlagEnum::kSide, CCompEnum::kThane, "FreeformRiftenThane"},
		{"Riften_Quest19", CFlagEnum::kSide, CCompEnum::kStage, "RelationshipMarriage"},
		{"Riften_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "T02"},
		{"Riften_Quest21", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften02"},
		{"Riften_Quest22", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften20"},
		{"Riften_Quest23", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften17"},
		{"Riften_Quest24", CFlagEnum::kSide, CCompEnum::kStage, "FreeformRiften03"},
		{"Riften_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "MS04"},
		{"Riften_Quest26", CFlagEnum::kSide, CCompEnum::kStage, "FreeformValdDebt"},
		{"Riften_Quest27", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Riften_Quest28", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
	};

	CRadiantData RadiantData[]{
		{"Riften_Quest27", CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"Riften_Quest28", CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Riften_Quest00", CStageEnum::kPast, 199,	0},
		{"Riften_Quest01", CStageEnum::kPast, 199,	0},
		{"Riften_Quest02", CStageEnum::kPast, 199,	0},
		{"Riften_Quest03", CStageEnum::kPast, 199,	0},
		{"Riften_Quest04", CStageEnum::kPast, 199,	0},
		{"Riften_Quest05", CStageEnum::kPast, 199,	0},
		{"Riften_Quest06", CStageEnum::kPast, 199,	0},
		{"Riften_Quest07", CStageEnum::kPast, 199,	0},
		{"Riften_Quest08", CStageEnum::kPast, 199,	0},
		{"Riften_Quest09", CStageEnum::kPast, 199,	0},
		{"Riften_Quest11", CStageEnum::kPast, 199,	0},
		{"Riften_Quest12", CStageEnum::kPast, 199,	0},
		{"Riften_Quest13", CStageEnum::kPast, 199,	0},
		{"Riften_Quest14", CStageEnum::kPast, 199,	0},
		{"Riften_Quest15", CStageEnum::kPast, 199,	0},
		{"Riften_Quest16", CStageEnum::kPast, 199,	0},
		{"Riften_Quest19", CStageEnum::kPast, 99,	0},
		{"Riften_Quest21", CStageEnum::kPast, 199,	0},
		{"Riften_Quest22", CStageEnum::kPast, 199,	0},
		{"Riften_Quest23", CStageEnum::kPast, 199,	0},
		{"Riften_Quest24", CStageEnum::kPast, 199,	0},
		{"Riften_Quest26", CStageEnum::kPast, 199,	0},
	};

	CThaneData ThaneData[]{
		{"Riften_Quest18", "RiftImpGetOutofJail", "RiftSonsGetOutofJail", 200},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initStageData(StageData)->initRadiantData(RadiantData)->initThaneData(ThaneData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 11, QuestData[i].unique_identifier));
		}
	};
};