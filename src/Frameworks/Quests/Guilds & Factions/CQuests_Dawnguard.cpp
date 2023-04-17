#include "CQuests_Dawnguard.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Dawnguard 
{
	CRadiantData RadiantData[]{
		/*01*/ {"Dawnguard_Quest01",  CRadiantEnum::kRadiant_DF6,	0x006BAC, 0x000000, 255, "Completionist_DLC1RH05" },
		/*02*/ {"Dawnguard_Quest02",  CRadiantEnum::kRadiant_DGU,	0x005E53, 0x000000, 255, "Completionist_DLC1RH02" },
		/*03*/ {"Dawnguard_Quest03",  CRadiantEnum::kRadiant_DGU,	0x005E4A, 0x000000, 255, "Completionist_DLC1RH01" },
		/*04*/ {"Dawnguard_Quest04",  CRadiantEnum::kRadiant_DGU,	0x006952, 0x000000, 255, "Completionist_DLC1RH04" },
		/*05*/ {"Dawnguard_Quest05",  CRadiantEnum::kRadiant_DGU,	0x007EC5, 0x000000, 255, "Completionist_DLC1RH07" },
		/*06*/ {"Dawnguard_Quest06",  CRadiantEnum::kRadiant_DF3,	0x015017, 0x000000, 255, "Completionist_DLC1RH08" },
		/*07*/ {"Dawnguard_Quest07",  CRadiantEnum::kRadiant_DGU,	0x005E58, 0x000000, 255, "Completionist_DLC1RH03" },
		/*08*/ {"Dawnguard_Quest08",  CRadiantEnum::kRadiant_DGU,	0x007201, 0x000000, 255, "Completionist_DLC1RH06" },
	};

	CQuestData QuestData[]
	{
		/*00*/ {"Dawnguard_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DLC1HunterBaseStage2"},
		/*01*/ {"Dawnguard_Quest01", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH05"},
		/*02*/ {"Dawnguard_Quest02", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH02"},
		/*03*/ {"Dawnguard_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH01"},
		/*04*/ {"Dawnguard_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH04"},
		/*05*/ {"Dawnguard_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH07"},
		/*06*/ {"Dawnguard_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH08"},
		/*07*/ {"Dawnguard_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH03"},
		/*08*/ {"Dawnguard_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RH06"},
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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 24));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};