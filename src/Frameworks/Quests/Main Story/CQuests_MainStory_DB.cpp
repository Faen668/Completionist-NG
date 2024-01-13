#include "CQuests_MainStory_DB.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_DB 
{
	CQuestData QuestData[] {
		/*00*/ {"Main_DB_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ01"},
		/*01*/ {"Main_DB_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ02"},
		/*02*/ {"Main_DB_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ03"},
		/*03*/ {"Main_DB_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ03B"},
		/*04*/ {"Main_DB_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ04"},
		/*05*/ {"Main_DB_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ05"},
		/*06*/ {"Main_DB_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DLC2MQ06"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 3, QuestData[i].unique_identifier));
		}
	};
};