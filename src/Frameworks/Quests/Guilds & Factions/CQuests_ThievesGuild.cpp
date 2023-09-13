#include "CQuests_ThievesGuild.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_ThievesGuild 
{
	CRadiantData RadiantData[]{
		/*22*/ {"ThievesGuild_Quest22",  CRadiantEnum::kRadiant_THG, 0x060990, 0x000000, 200, "Completionist_TGR_Bedlam" },
		/*23*/ {"ThievesGuild_Quest23",  CRadiantEnum::kRadiant_THG, 0x02893B, 0x000000, 200, "Completionist_TGR_Burglary" },
		/*24*/ {"ThievesGuild_Quest24",  CRadiantEnum::kRadiant_THG, 0x028922, 0x000000, 200, "Completionist_TGR_Fishing" },
		/*25*/ {"ThievesGuild_Quest25",  CRadiantEnum::kRadiant_THG, 0x02893E, 0x000000, 200, "Completionist_TGR_Heist" },
		/*26*/ {"ThievesGuild_Quest26",  CRadiantEnum::kRadiant_THG, 0x06098E, 0x000000, 200, "Completionist_TGR_Numbers" },
		/*27*/ {"ThievesGuild_Quest27",  CRadiantEnum::kRadiant_THG, 0x015D24, 0x000000, 200, "Completionist_TGR_Shill" },
		/*28*/ {"ThievesGuild_Quest28",  CRadiantEnum::kRadiant_THG, 0x028936, 0x000000, 200, "Completionist_TGR_Sweep" },
	};

	CStageData StageData[]{
		{"ThievesGuild_Quest15", CStageEnum::kPast, 199, 0},
		{"ThievesGuild_Quest16", CStageEnum::kPast, 199, 0},
	};

	CQuestData QuestData[]
	{
		/*00*/ {"ThievesGuild_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "TG00"},
		/*01*/ {"ThievesGuild_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "TG01"},
		/*02*/ {"ThievesGuild_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "TG02"},
		/*03*/ {"ThievesGuild_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "TG03"},
		/*04*/ {"ThievesGuild_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "TG04"},
		/*05*/ {"ThievesGuild_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "TG05"},
		/*06*/ {"ThievesGuild_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "TG06"},
		/*07*/ {"ThievesGuild_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "TG07"},
		/*08*/ {"ThievesGuild_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "TG08A"},
		/*09*/ {"ThievesGuild_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "TG08B"},
		/*10*/ {"ThievesGuild_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "TG09"},
		/*11*/ {"ThievesGuild_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "TGLeadership"},
		/*12*/ {"ThievesGuild_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "TG02B"},
		/*13*/ {"ThievesGuild_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "TGCrown"},
		/*14*/ {"ThievesGuild_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "TGCrownMisc"},
		/*15*/ {"ThievesGuild_Quest15", CFlagEnum::kSide, CCompEnum::kStage, "TGLarceny"},
		/*16*/ {"ThievesGuild_Quest16", CFlagEnum::kSide, CCompEnum::kStage, "TGFenceCaravan"},
		/*17*/ {"ThievesGuild_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "FreeformRiften21"},
		/*18*/ {"ThievesGuild_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "TGTQ03"},
		/*19*/ {"ThievesGuild_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "TGTQ01"},
		/*20*/ {"ThievesGuild_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "TGTQ04"},
		/*21*/ {"ThievesGuild_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "TGTQ02"},
		/*22*/ {"ThievesGuild_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRNT"},
		/*23*/ {"ThievesGuild_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRBE"},
		/*24*/ {"ThievesGuild_Quest24", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRGF"},
		/*25*/ {"ThievesGuild_Quest25", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRSL"},
		/*26*/ {"ThievesGuild_Quest26", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRFO"},
		/*27*/ {"ThievesGuild_Quest27", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRDU"},
		/*28*/ {"ThievesGuild_Quest28", CFlagEnum::kRadi, CCompEnum::kGlobl, "TGRHC"},
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
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 25, QuestData[i].unique_identifier));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};