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
		{"Windhelm_Quest05", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor019"},
		{"Windhelm_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "MS12"},
		{"Windhelm_Quest07", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor013"},
		{"Windhelm_Quest08", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor018"},
		{"Windhelm_Quest09", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		{"Windhelm_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Windhelm_Quest11", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor104"},
		{"Windhelm_Quest12", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShipsWindhelm"},
		{"Windhelm_Quest13", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
		{"Windhelm_Quest14", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor017"},
		{"Windhelm_Quest15", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor205"},
		{"Windhelm_Quest16", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor204"},
		{"Windhelm_Quest17", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor001"},
		{"Windhelm_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Windhelm_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
	};

	CRadiantData RadiantData[]{
		{"Windhelm_Quest05",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor019Revyn"},
		{"Windhelm_Quest07",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor013ScoutsManyMarshes" },
		{"Windhelm_Quest08",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor018StandInShallows" },
		{"Windhelm_Quest09",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor110Torbjorn" },
		{"Windhelm_Quest10",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorDrunksTorbjorn" },
		{"Windhelm_Quest11",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor104Brunwulf" },
		{"Windhelm_Quest14",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor017Rolff" },
		{"Windhelm_Quest15",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor205Oengul" },
		{"Windhelm_Quest16",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor204Shahvee" },
		{"Windhelm_Quest17",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor001Adonato" },
		{"Windhelm_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarAngrenor" },
		{"Windhelm_Quest19",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarSilda" },
	};

	CDrunkData DrunkData[]{
		{"Windhelm_Quest10", 0x0072EA2, 0x001B12B, "Skyrim.esm", "Completionist_FavorDrunksTorbjorn"},
	};

	CStageData StageData[]{
		{"Windhelm_Quest01", CStageEnum::kPast, 199, 0},
		{"Windhelm_Quest02", CStageEnum::kPast, 140, 0},
		{"Windhelm_Quest12", CStageEnum::kPast, 199, 0},
	};

	CThaneData ThaneData[]{
		{"Windhelm_Quest13", "EastmarchImpGetOutofJail", "EastmarchSonsGetOutofJail"},
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
				->initDrunkData(DrunkData)
				->initStageData(StageData)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 14, QuestData[i].unique_identifier));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};