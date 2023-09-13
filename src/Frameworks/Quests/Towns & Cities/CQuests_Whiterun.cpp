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
		{"Whiterun_Quest10", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor013"},
		{"Whiterun_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Whiterun_Quest12", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor204"},
		{"Whiterun_Quest13", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor017"},
		{"Whiterun_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Whiterun_Quest15", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Whiterun_Quest16", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		{"Whiterun_Quest17", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
	};

	CRadiantData RadiantData[]{
	{"Whiterun_Quest10", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor013CarlottaValentia" },
	{"Whiterun_Quest11", CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodHulda" },
	{"Whiterun_Quest12", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor204Amren" },
	{"Whiterun_Quest13", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor017Uthgerd" },
	{"Whiterun_Quest14", CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorBeggarBrenuin" },
	{"Whiterun_Quest15", CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorBeggarLucia" },
	{"Whiterun_Quest16", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor110Ysolda" },
	};

	CStageData StageData[]{
		{"Whiterun_Quest07", CStageEnum::kDone, 3, 4},
	};

	CThaneData ThaneData[]{
		{"Whiterun_Quest17", "WhiterunImpGetOutofJail", "WhiterunSonsGetOutofJail"},
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
				->initStageData(StageData)
				->initRadiantData(RadiantData)
				->initThaneData(ThaneData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 13, QuestData[i].unique_identifier));

		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};