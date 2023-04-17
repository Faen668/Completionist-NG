#include "CQuests_DarkBrotherhood.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_DarkBrotherhood 
{
	CRadiantData RadiantData[]{
		/*33*/ { "DarkBrotherhood_Quest33", CRadiantEnum::kRadiant_DBR, 0x01EA5A, 0x000000, 200, "Completionist_DBrecurring"},
	};

	CQuestData QuestData[]
	{
		/*00*/ {"DarkBrotherhood_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DB01"},
		/*01*/ {"DarkBrotherhood_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DB01Misc"},
		/*02*/ {"DarkBrotherhood_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DB02"},
		/*03*/ {"DarkBrotherhood_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DB02a"},
		/*04*/ {"DarkBrotherhood_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DB03"},
		/*05*/ {"DarkBrotherhood_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DB04"},
		/*06*/ {"DarkBrotherhood_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DB04a"},
		/*07*/ {"DarkBrotherhood_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "DB05"},
		/*08*/ {"DarkBrotherhood_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "DB06"},
		/*09*/ {"DarkBrotherhood_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "DB07"},
		/*10*/ {"DarkBrotherhood_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "DB08"},
		/*11*/ {"DarkBrotherhood_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "DB09"},
		/*12*/ {"DarkBrotherhood_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "DB10"},
		/*13*/ {"DarkBrotherhood_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "DB11"},
		/*14*/ {"DarkBrotherhood_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "DBDestroy"},
		/*15*/ {"DarkBrotherhood_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract09"},
		/*16*/ {"DarkBrotherhood_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract08"},
		/*17*/ {"DarkBrotherhood_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract03"},
		/*18*/ {"DarkBrotherhood_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract06"},
		/*19*/ {"DarkBrotherhood_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract02"},
		/*20*/ {"DarkBrotherhood_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract11"},
		/*21*/ {"DarkBrotherhood_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract04"},
		/*22*/ {"DarkBrotherhood_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract05"},
		/*23*/ {"DarkBrotherhood_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract10"},
		/*24*/ {"DarkBrotherhood_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract07"},
		/*25*/ {"DarkBrotherhood_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract01"},
		/*26*/ {"DarkBrotherhood_Quest26", CFlagEnum::kSide, CCompEnum::kStand, "DBSideContract12"},
		/*27*/ {"DarkBrotherhood_Quest27", CFlagEnum::kSide, CCompEnum::kStand, "DBOlavaReadingTreasureObjective"},
		/*28*/ {"DarkBrotherhood_Quest28", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective1"},
		/*29*/ {"DarkBrotherhood_Quest29", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective2"},
		/*30*/ {"DarkBrotherhood_Quest30", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective3"},
		/*31*/ {"DarkBrotherhood_Quest31", CFlagEnum::kSide, CCompEnum::kStand, "DBTortureTreasureMiscObjective4"},
		/*32*/ {"DarkBrotherhood_Quest32", CFlagEnum::kSide, CCompEnum::kStand, "DarkBrotherhoodSanctuaryRepair"},
		/*33*/ {"DarkBrotherhood_Quest33", CFlagEnum::kRadi, CCompEnum::kGlobl, "DBrecurring"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&QuestArrays)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 23));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};