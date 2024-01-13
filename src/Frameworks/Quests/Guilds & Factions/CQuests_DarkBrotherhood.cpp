#include "CQuests_DarkBrotherhood.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_DarkBrotherhood 
{
	CRadiantData RadiantData[]{
		{ "DarkBrotherhood_Quest33", CRadiantEnum::kRadiant_DBR, 0x01EA5A, 0, 200},
	};

	CQuestData QuestData[]
	{
		{"DarkBrotherhood_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DB01"},
		{"DarkBrotherhood_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DB01Misc"},
		{"DarkBrotherhood_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DB02"},
		{"DarkBrotherhood_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DB02a"},
		{"DarkBrotherhood_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DB03"},
		{"DarkBrotherhood_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DB04"},
		{"DarkBrotherhood_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DB04a"},
		{"DarkBrotherhood_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "DB05"},
		{"DarkBrotherhood_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "DB06"},
		{"DarkBrotherhood_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "DB07"},
		{"DarkBrotherhood_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "DB08"},
		{"DarkBrotherhood_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "DB09"},
		{"DarkBrotherhood_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "DB10"},
		{"DarkBrotherhood_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "DB11"},
		{"DarkBrotherhood_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "DBDestroy"},
		{"DarkBrotherhood_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract09"},
		{"DarkBrotherhood_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract08"},
		{"DarkBrotherhood_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract03"},
		{"DarkBrotherhood_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract06"},
		{"DarkBrotherhood_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract02"},
		{"DarkBrotherhood_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract11"},
		{"DarkBrotherhood_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract04"},
		{"DarkBrotherhood_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract05"},
		{"DarkBrotherhood_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract10"},
		{"DarkBrotherhood_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract07"},
		{"DarkBrotherhood_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract01"},
		{"DarkBrotherhood_Quest26", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract12"},
		{"DarkBrotherhood_Quest27", CFlagEnum::kSide, CCompEnum::kStand, "DBOlavaReadingTreasureObjective"},
		{"DarkBrotherhood_Quest28", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective1"},
		{"DarkBrotherhood_Quest29", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective2"},
		{"DarkBrotherhood_Quest30", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective3"},
		{"DarkBrotherhood_Quest31", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective4"},
		{"DarkBrotherhood_Quest32", CFlagEnum::kSide, CCompEnum::kStand, "DarkBrotherhoodSanctuaryRepair"},
		{"DarkBrotherhood_Quest33", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBrecurring"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initRadiantData(RadiantData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 23, QuestData[i].unique_identifier));
		}
	};
};