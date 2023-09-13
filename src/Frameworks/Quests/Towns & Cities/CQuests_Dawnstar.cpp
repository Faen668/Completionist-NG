#include "CQuests_Dawnstar.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Dawnstar 
{
	CQuestData QuestData[]
	{
		{"Dawnstar_Quest00", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor153"},
		{"Dawnstar_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DA07"},
		{"Dawnstar_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DA16"},
		{"Dawnstar_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "FreeformShipsDawnstar"},
		{"Dawnstar_Quest04", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor205"},
		{"Dawnstar_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsDrunks"},
		{"Dawnstar_Quest06", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		{"Dawnstar_Quest07", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
		{"Dawnstar_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Dawnstar_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
	};

	CRadiantData RadiantData[]{
		{"Dawnstar_Quest00",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor153Skald" },
		{"Dawnstar_Quest04",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor205Frida" },
		{"Dawnstar_Quest05",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorDrunksKarl" },
		{"Dawnstar_Quest06",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor110Rustleif" },
		{"Dawnstar_Quest08",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreBeitld" },
		{"Dawnstar_Quest09",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreLeigelf" },
	};

	CDrunkData DrunkData[]{
		{"Dawnstar_Quest05", 0x0072EA2, 0x001A6BC, "Skyrim.esm", "Completionist_FavorDrunksKarl"},
	};

	CStageData StageData[]{
		{"Dawnstar_Quest03", CStageEnum::kPast, 199, 0},
	};

	CThaneData ThaneData[]{
		{"Dawnstar_Quest07", "PaleImpGetOutofJail", "PaleSonsGetOutofJail"},
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
				->initThaneData(ThaneData)
				->initDrunkData(DrunkData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 7, QuestData[i].unique_identifier));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};