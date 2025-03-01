#include "CQuests_Dawnstar.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Dawnstar 
{
	CQuestData QuestData[]
	{
		{"Dawnstar_Quest00", CFlagEnum::kSide, CCompEnum::kFavor, "Favor153"},
		{"Dawnstar_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DA07"},
		{"Dawnstar_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DA16"},
		{"Dawnstar_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShipsDawnstar"},
		{"Dawnstar_Quest04", CFlagEnum::kSide, CCompEnum::kFavor, "Favor205"},
		{"Dawnstar_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Dawnstar_Quest06", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		{"Dawnstar_Quest07", CFlagEnum::kSide, CCompEnum::kThane, "Favor256"},
		{"Dawnstar_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Dawnstar_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
	};

	CFavorData FavorData[]{
		{"Dawnstar_Quest00",  0x04734B, "Skyrim.esm", 0x013620, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Dawnstar_Quest04",  0x04D6DA, "Skyrim.esm", 0x013614, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Dawnstar_Quest06",  0x0CA43B, "Skyrim.esm", 0x01361e, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		{"Dawnstar_Quest05",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Dawnstar_Quest08",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Dawnstar_Quest09",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]{
		{"Dawnstar_Quest03", CStageEnum::kPast, 199, 0},
	};

	CThaneData ThaneData[]{
		{"Dawnstar_Quest07", "PaleImpGetOutofJail", "PaleSonsGetOutofJail"},
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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 7, QuestData[i].unique_identifier));
		};
	};
};