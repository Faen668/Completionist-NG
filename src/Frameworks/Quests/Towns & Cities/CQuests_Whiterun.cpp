#include "CQuests_Whiterun.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Whiterun 
{
	CQuestData QuestData[] {
		{"Whiterun_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWhiterunQuest04"},
		{"Whiterun_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWhiterunQuest02"},
		{"Whiterun_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWhiterunQuest01"},
		{"Whiterun_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWhiterunQuest03"},
		{"Whiterun_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "MS08"},
		{"Whiterun_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "MS09"},
		{"Whiterun_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWhiterunQuest05"},
		{"Whiterun_Quest07", CFlagEnum::kSide, CCompEnum::kStage, "dunSleepingTreeCampQST"},
		{"Whiterun_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "T03"},
		{"Whiterun_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DA08"},
		{"Whiterun_Quest10", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Whiterun_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Whiterun_Quest12", CFlagEnum::kSide, CCompEnum::kFavor, "Favor204"},
		{"Whiterun_Quest13", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017"},
		{"Whiterun_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Whiterun_Quest15", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Whiterun_Quest16", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		{"Whiterun_Quest17", CFlagEnum::kSide, CCompEnum::kThane, "Favor253"},
	};

	CFavorData FavorData[]{
		{"Whiterun_Quest10", 0x0CA20A, "Skyrim.esm", 0x013b99, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Whiterun_Quest12", 0x04D6C9, "Skyrim.esm", 0x013baa, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Whiterun_Quest13", 0x0CA213, "Skyrim.esm", 0x0918e2, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Whiterun_Quest16", 0x0CA43B, "Skyrim.esm", 0x013bab, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		{"Whiterun_Quest11", CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Whiterun_Quest14", CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Whiterun_Quest15", CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Whiterun_Quest07", CStageEnum::kDone, 3, 4},
	};

	CThaneData ThaneData[]{
		{"Whiterun_Quest17", "WhiterunImpGetOutofJail", "WhiterunSonsGetOutofJail"},
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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 13, QuestData[i].unique_identifier));

		}
	};
};