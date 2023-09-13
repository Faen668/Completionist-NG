#include "CQuests_Falkreath.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Falkreath 
{
	CRadiantData RadiantData[]{
		{"Falkreath_Quest03",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor154Siddgeir" },
		{"Falkreath_Quest04",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodHert" },
		{"Falkreath_Quest05",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor001Thadgeir" },
		{"Falkreath_Quest06",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor205Runil" },
		{"Falkreath_Quest07",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestMathies" },
		{"Falkreath_Quest08",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor110Siddgeir" },
		{"Falkreath_Quest09",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor018Dengeir" },
	};

	CQuestData QuestData[]
	{
		{"Falkreath_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DA03"},
		{"Falkreath_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "FreeformFalkreathQuest03"},
		{"Falkreath_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DA05"},
		{"Falkreath_Quest03", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor154"},
		{"Falkreath_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Falkreath_Quest05", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor001"},
		{"Falkreath_Quest06", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor205"},
		{"Falkreath_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Falkreath_Quest08", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		{"Falkreath_Quest09", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor018"},
		{"Falkreath_Quest10", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
	};

	CThaneData ThaneData[]{
		{"Falkreath_Quest10", "FalkreathImpGetOutofJail", "FalkreathSonsGetOutofJail"},
	};

	CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray, &KeysArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData)
				->initThaneData(ThaneData)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 8, QuestData[i].unique_identifier));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};