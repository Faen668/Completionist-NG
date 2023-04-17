#include "CQuests_Winterhold.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Winterhold
{
	CQuestData QuestData[] {
		{"Winterhold_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformWinterholdC"},
		{"Winterhold_Quest01", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor013"},
		{"Winterhold_Quest02", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor158"},
		{"Winterhold_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Winterhold_Quest04", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor018"},
		{"Winterhold_Quest05", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
	};

	CRadiantData RadiantData[]{
	{"Winterhold_Quest01",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor013Haran" },
	{"Winterhold_Quest02",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor158Korir" },
	{"Winterhold_Quest03",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorDrunksRanmir" },
	{"Winterhold_Quest04",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor018Malur" },
	};

	CDrunkData DrunkData[]{
		{"Winterhold_Quest03", 0x0072EA2, 0x001C18B, "Skyrim.esm", "Completionist_FavorDrunksRanmir"},
	};

	CStageData StageData[]{
		{"Winterhold_Quest00", CStageEnum::kDone, 40, 0},
	};

	CThaneData ThaneData[]{
		{"Winterhold_Quest05", "WinterholdImpGetOutofJail", "WinterholdSonsGetOutofJail"},
	};

	CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

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
				->initStageData(StageData)
				->initDrunkData(DrunkData)
				->initRadiantData(RadiantData);

			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 15));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};