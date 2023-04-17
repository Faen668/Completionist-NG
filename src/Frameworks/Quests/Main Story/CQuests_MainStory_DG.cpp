#include "CQuests_MainStory_DG.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_DG 
{
	CQuestData QuestData[] {
		/*00*/ {"Main_DG_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ01MiscObjective"},
		/*01*/ {"Main_DG_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ01"},
		/*02*/ {"Main_DG_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ02"},
		/*03*/ {"Main_DG_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DLC1HunterBaseIntro"},
		/*04*/ {"Main_DG_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ03Hunter"},
		/*05*/ {"Main_DG_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VampireBaseIntro"},
		/*06*/ {"Main_DG_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ03Vampire"},
		/*07*/ {"Main_DG_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ04"},
		/*08*/ {"Main_DG_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQElderHandler"},
		/*09*/ {"Main_DG_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ05"},
		/*10*/ {"Main_DG_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQElder"},
		/*11*/ {"Main_DG_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ06"},
		/*12*/ {"Main_DG_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ07"},
		/*13*/ {"Main_DG_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "DLC1VQ08"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initQuestData(&QuestArrays);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 2));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};