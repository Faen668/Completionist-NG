#include "CQuests_MainStory_SK.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_SK
{
	CQuestData QuestData[] {
		/*00*/ {"MSQ_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "ARTHLALChargenQuest"},
		/*01*/ {"MSQ_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "ARTHLALRumorsOfWarQuest"},
		/*02*/ {"MSQ_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "MQ101"},
		/*03*/ {"MSQ_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "MQ102"},
		/*04*/ {"MSQ_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "MQ103"},
		/*05*/ {"MSQ_Quest05", CFlagEnum::kMain, CCompEnum::kStage, "MQ104"},
		/*06*/ {"MSQ_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "MQ105"},
		/*07*/ {"MSQ_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "MQ105Ustengrav"},
		/*08*/ {"MSQ_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "MQ106"},
		/*09*/ {"MSQ_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "MQ201"},
		/*10*/ {"MSQ_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "MQ202"},
		/*11*/ {"MSQ_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "MQ203"},
		/*12*/ {"MSQ_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "MQ204"},
		/*13*/ {"MSQ_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "MQ205"},
		/*14*/ {"MSQ_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "MQ206"},
		/*15*/ {"MSQ_Quest15", CFlagEnum::kMain, CCompEnum::kStand, "MQ301"},
		/*16*/ {"MSQ_Quest16", CFlagEnum::kMain, CCompEnum::kStand, "MQ302"},
		/*17*/ {"MSQ_Quest17", CFlagEnum::kMain, CCompEnum::kStand, "MQPaarthurnax"},
		/*18*/ {"MSQ_Quest18", CFlagEnum::kMain, CCompEnum::kStand, "MQ303"},
		/*19*/ {"MSQ_Quest19", CFlagEnum::kMain, CCompEnum::kStand, "MQ304"},
		/*20*/ {"MSQ_Quest20", CFlagEnum::kMain, CCompEnum::kStand, "MQ305"},
	};

	//Completed Based on stage to also complete the whiterun thane quest.
	CStageData StageData[]{
		{"MSQ_Quest05", CStageEnum::kDone, 200},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initStageData(StageData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(),0, QuestData[i].unique_identifier));
		}
	};
};