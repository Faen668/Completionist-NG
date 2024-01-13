#include "CQuests_Winterhold.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Winterhold
{
	CQuestData QuestData[] {
		{"Winterhold_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformWinterholdC"},
		{"Winterhold_Quest01", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Winterhold_Quest02", CFlagEnum::kSide, CCompEnum::kFavor, "Favor158"},
		{"Winterhold_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Winterhold_Quest04", CFlagEnum::kSide, CCompEnum::kFavor, "Favor018"},
		{"Winterhold_Quest05", CFlagEnum::kSide, CCompEnum::kThane, "Favor257"},
	};

	CFavorData FavorData[]{
		{"Winterhold_Quest01", 0x0CA20A, "Skyrim.esm", 0x01C184, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Winterhold_Quest02", 0x0C0604, "Skyrim.esm", 0x01c188, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Winterhold_Quest04", 0x03F48D, "Skyrim.esm", 0x01C182, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		{"Winterhold_Quest03",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
	};

	CDrunkData DrunkData[]{
		{"Winterhold_Quest03", 0x0072EA2, 0x001C18B, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Winterhold_Quest00", CStageEnum::kDone, 40, 0},
	};

	CThaneData ThaneData[]{
		{"Winterhold_Quest05", "WinterholdImpGetOutofJail", "WinterholdSonsGetOutofJail"},
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
				->initStageData(StageData)
				->initDrunkData(DrunkData)
				->initRadiantData(RadiantData)
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 15, QuestData[i].unique_identifier));
		}
	};
};