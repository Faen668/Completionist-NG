#include "CQuests_Thirsk.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Thirsk 
{
	CQuestData QuestData[] {
		/*00*/ {"Thirsk_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DLC2MH02"},
		/*01*/ {"Thirsk_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DLC2ThirskFFElmusBack"},
		/*02*/ {"Thirsk_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DLC2ThirskFFElmus"},
		/*03*/ {"Thirsk_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DLC2ThirskFFHalbarn"},
		/*04*/ {"Thirsk_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "DLC2ThirskFFHilund"},
		/*05*/ {"Thirsk_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "DLC2MH01"},
		/*06*/ {"Thirsk_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "DLC2MHMisc"},
	};

	CStageData StageData[]{
		{"Thirsk_Quest06", CStageEnum::kDone, 100, 0},
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
				->initStageData(StageData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 19, QuestData[i].unique_identifier));

		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};