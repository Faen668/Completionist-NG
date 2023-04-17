#include "CQuests_MainStory_CW.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_CW 
{
	CRadiantData RadiantData[]{
		/*03*/ {"CWQuest03", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_SiegeWhiterun" },
		/*05*/ {"CWQuest05", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_SiegeWindhelm" },
		/*10*/ {"CWQuest10", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortEastmarch" },
		/*11*/ {"CWQuest11", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortPale" },
		/*12*/ {"CWQuest12", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortRift" },
		/*13*/ {"CWQuest13", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortFalkreath" },
		/*14*/ {"CWQuest14", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortHjaalmarch" },
		/*15*/ {"CWQuest15", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortReach" },
		/*19*/ {"CWQuest19", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_SiegeWhiterun" },
		/*21*/ {"CWQuest21", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_SiegeSolitude" },
		/*25*/ {"CWQuest25", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortPale" },
		/*26*/ {"CWQuest26", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortRift" },
		/*27*/ {"CWQuest27", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortWinterhold" },
		/*28*/ {"CWQuest28", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortHaafingar" },
		/*29*/ {"CWQuest29", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortHjaalmarch" },
		/*30*/ {"CWQuest30", CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_FortReach" },
	};

	CStageData StageData[]{
		{"CWQuest08", CStageEnum::kPast, 12, 0}
	};

	CQuestData QuestData[]{
		/*00*/ {"CWQuest00", CFlagEnum::kMain, CCompEnum::kStand, "CW01A"},
		/*01*/ {"CWQuest01", CFlagEnum::kMain, CCompEnum::kStand, "CW02A"},
		/*02*/ {"CWQuest02", CFlagEnum::kMain, CCompEnum::kStand, "CW03"},
		/*03*/ {"CWQuest03", CFlagEnum::kMain, CCompEnum::kGlobl, "CWSiegeObj"},
		/*04*/ {"CWQuest04", CFlagEnum::kMain, CCompEnum::kStand, "CWObj"},
		/*05*/ {"CWQuest05", CFlagEnum::kMain, CCompEnum::kGlobl, "CWSiegeObj"},
		/*06*/ {"CWQuest06", CFlagEnum::kSide, CCompEnum::kStand, "CWMission03"},
		/*07*/ {"CWQuest07", CFlagEnum::kSide, CCompEnum::kStand, "CWMission07"},
		/*08*/ {"CWQuest08", CFlagEnum::kSide, CCompEnum::kStage, "CW01AOutfitImperial"},
		/*09*/ {"CWQuest09", CFlagEnum::kSide, CCompEnum::kStand, "CWMission04"},
		/*10*/ {"CWQuest10", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*11*/ {"CWQuest11", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*12*/ {"CWQuest12", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*13*/ {"CWQuest13", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*14*/ {"CWQuest14", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*15*/ {"CWQuest15", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*16*/ {"CWQuest16", CFlagEnum::kMain, CCompEnum::kStand, "CW01B"},
		/*17*/ {"CWQuest17", CFlagEnum::kMain, CCompEnum::kStand, "CW02B"},
		/*18*/ {"CWQuest18", CFlagEnum::kMain, CCompEnum::kStand, "CW03"},
		/*19*/ {"CWQuest19", CFlagEnum::kMain, CCompEnum::kGlobl, "CWSiegeObj"},
		/*20*/ {"CWQuest20", CFlagEnum::kMain, CCompEnum::kStand, "CWObj"},
		/*21*/ {"CWQuest21", CFlagEnum::kMain, CCompEnum::kGlobl, "CWSiegeObj"},
		/*22*/ {"CWQuest22", CFlagEnum::kSide, CCompEnum::kStand, "CWMission03"},
		/*23*/ {"CWQuest23", CFlagEnum::kSide, CCompEnum::kStand, "CWMission07"},
		/*24*/ {"CWQuest24", CFlagEnum::kSide, CCompEnum::kStand, "CWMission04"},
		/*25*/ {"CWQuest25", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*26*/ {"CWQuest26", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*27*/ {"CWQuest27", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*28*/ {"CWQuest28", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*29*/ {"CWQuest29", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
		/*30*/ {"CWQuest30", CFlagEnum::kSide, CCompEnum::kGlobl, "CWFortSiegeFort"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initQuestData(&QuestArrays)->initStageData(StageData)->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 1));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};