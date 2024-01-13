#include "CQuests_Falkreath.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Falkreath 
{
	CRadiantData RadiantData[]{
		{"Falkreath_Quest04",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Falkreath_Quest07",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
	};

	CFavorData FavorData[]{
		{"Falkreath_Quest03", 0x04B774, "Skyrim.esm", 0x013653, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Falkreath_Quest05", 0x0403B1, "Skyrim.esm", 0x04e5e9, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Falkreath_Quest06", 0x04D6DA, "Skyrim.esm", 0x01364d, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Falkreath_Quest08", 0x0CA43B, "Skyrim.esm", 0x013653, "Skyrim.esm", CQuestProcessor::kExcluded},
		{"Falkreath_Quest09", 0x03F48D, "Skyrim.esm", 0x01365a, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CQuestData QuestData[]
	{
		{"Falkreath_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DA03"},
		{"Falkreath_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "FreeformFalkreathQuest03"},
		{"Falkreath_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DA05"},
		{"Falkreath_Quest03", CFlagEnum::kSide, CCompEnum::kFavor, "Favor154"},
		{"Falkreath_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Falkreath_Quest05", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001"},
		{"Falkreath_Quest06", CFlagEnum::kSide, CCompEnum::kFavor, "Favor205"},
		{"Falkreath_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Falkreath_Quest08", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		{"Falkreath_Quest09", CFlagEnum::kSide, CCompEnum::kFavor, "Favor018"},
		{"Falkreath_Quest10", CFlagEnum::kSide, CCompEnum::kThane, "Favor258"},
	};

	CThaneData ThaneData[]{
		{"Falkreath_Quest10", "FalkreathImpGetOutofJail", "FalkreathSonsGetOutofJail"},
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
				->initRadiantData(RadiantData)
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 8, QuestData[i].unique_identifier));
		}
	};
};