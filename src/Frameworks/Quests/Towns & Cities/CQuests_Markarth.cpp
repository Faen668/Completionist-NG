#include "CQuests_Markarth.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Markarth 
{
	CQuestData QuestData[]
	{
		{"Markarth_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthJ"},
		{"Markarth_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "WIAddItem07"},
		{"Markarth_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthN"},
		{"Markarth_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthD"},
		{"Markarth_Quest04", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor151"},
		{"Markarth_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthE"},
		{"Markarth_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthO"},
		{"Markarth_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "MS02"},
		{"Markarth_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthM"},
		{"Markarth_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMarkarthL"},
		{"Markarth_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "MS01"},
		{"Markarth_Quest11", CFlagEnum::kSide, CCompEnum::kStage, "T01DegaineFavor"},
		{"Markarth_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "T01"},
		{"Markarth_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "DA10"},
		{"Markarth_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "dunNchuandZelQst"},
		{"Markarth_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DA11Intro"},
		{"Markarth_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "DA11"},
		{"Markarth_Quest17", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor013"},
		{"Markarth_Quest18", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor001"},
		{"Markarth_Quest19", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor157"},
		{"Markarth_Quest20", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor017"},
		{"Markarth_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Markarth_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Markarth_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Markarth_Quest24", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
	};

	CRadiantData RadiantData[]{
		{"Markarth_Quest04",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor151Igmund" },
		{"Markarth_Quest17",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor013Omluag" },
		{"Markarth_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_Favor001Banning" },
		{"Markarth_Quest19",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor157Igmund" },
		{"Markarth_Quest20",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_Favor017Cosnach" },
		{"Markarth_Quest21",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarDegaine" },
		{"Markarth_Quest22",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorDrunksDegaine" },
		{"Markarth_Quest23",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorDrunksCosnach" },
	};

	CStageData StageData[]{
		{"Markarth_Quest00", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest01", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest02", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest03", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest05", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest06", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest08", CStageEnum::kPast, 15,	0},
		{"Markarth_Quest09", CStageEnum::kPast, 10,	0},
		{"Markarth_Quest11", CStageEnum::kPast, 20,	0},
	};

	CDrunkData DrunkData[]{
		{"Markarth_Quest22", 0x0072EA2, 0x001F325, "Skyrim.esm", "Completionist_FavorDrunksDegaine" },
		{"Markarth_Quest23", 0x0072EA2, 0x00198FA, "Skyrim.esm", "Completionist_FavorDrunksCosnach" },
	};

	CThaneData ThaneData[]{
		{"Markarth_Quest24", "ReachImpGetOutofJail", "ReachSonsGetOutofJail"},
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
				->initStageData(StageData)
				->initRadiantData(RadiantData)
				->initThaneData(ThaneData)
				->initDrunkData(DrunkData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 9));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};