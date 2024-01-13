#include "CQuests_Windhelm.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Windhelm
{
	CQuestData QuestData[]
	{
		{"Windhelm_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "MS11"},
		{"Windhelm_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "FreeformWindhelmA"},
		{"Windhelm_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "MQ201Malborn"},
		{"Windhelm_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "MS12b"},
		{"Windhelm_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "MS10"},
		{"Windhelm_Quest05", CFlagEnum::kSide, CCompEnum::kFavor, "Favor019"},
		{"Windhelm_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "MS12"},
		{"Windhelm_Quest07", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Windhelm_Quest08", CFlagEnum::kSide, CCompEnum::kFavor, "Favor018"},
		{"Windhelm_Quest09", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		{"Windhelm_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Windhelm_Quest11", CFlagEnum::kSide, CCompEnum::kFavor, "Favor104"},
		{"Windhelm_Quest12", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShipsWindhelm"},
		{"Windhelm_Quest13", CFlagEnum::kSide, CCompEnum::kThane, "Favor254"},
		{"Windhelm_Quest14", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017"},
		{"Windhelm_Quest15", CFlagEnum::kSide, CCompEnum::kFavor, "Favor205"},
		{"Windhelm_Quest16", CFlagEnum::kSide, CCompEnum::kFavor, "Favor204"},
		{"Windhelm_Quest17", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001"},
		{"Windhelm_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Windhelm_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
	};

	CFavorData FavorData[]{
		{"Windhelm_Quest05",  0x0403A7, "Skyrim.esm", 0x01413a, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest07",  0x0CA20A, "Skyrim.esm", 0x01412e, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest08",  0x03F48D, "Skyrim.esm", 0x014130, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest09",  0x0CA43B, "Skyrim.esm", 0x01413f, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest11",  0x06FD64, "Skyrim.esm", 0x014149, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest14",  0x0CA213, "Skyrim.esm", 0x03efe9, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest15",  0x04D6DA, "Skyrim.esm", 0x014142, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest16",  0x04D6C9, "Skyrim.esm", 0x01411a, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Windhelm_Quest17",  0x0403B1, "Skyrim.esm", 0x01413c, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		{"Windhelm_Quest10",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"Windhelm_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		{"Windhelm_Quest19",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CDrunkData DrunkData[]{
		{"Windhelm_Quest10", 0x0072EA2, 0x001B12B, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Windhelm_Quest01", CStageEnum::kPast, 199, 0},
		{"Windhelm_Quest02", CStageEnum::kPast, 140, 0},
		{"Windhelm_Quest12", CStageEnum::kPast, 199, 0},
	};

	CThaneData ThaneData[]{
		{"Windhelm_Quest13", "EastmarchImpGetOutofJail", "EastmarchSonsGetOutofJail"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initThaneData(ThaneData)
				->initDrunkData(DrunkData)
				->initStageData(StageData)
				->initRadiantData(RadiantData)
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 14, QuestData[i].unique_identifier));
		}
	};
};