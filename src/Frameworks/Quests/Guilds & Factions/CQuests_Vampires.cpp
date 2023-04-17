#include "CQuests_Vampires.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Vampires 
{
	CRadiantData RadiantData[]{
		/*04*/ {"Vampires_Quest04",  CRadiantEnum::kRadiant_DGU, 0x005702, 0x000000, 255, "Completionist_DLC1RV05" },
		/*05*/ {"Vampires_Quest05",  CRadiantEnum::kRadiant_DGU, 0x0050B9, 0x000000, 255, "Completionist_DLC1RV02" },
		/*06*/ {"Vampires_Quest06",  CRadiantEnum::kRadiant_DGU, 0x0050BD, 0x000000, 255, "Completionist_DLC1RV03" },
		/*07*/ {"Vampires_Quest07",  CRadiantEnum::kRadiant_DGU, 0x00A29C, 0x000000, 255, "Completionist_DLC1RV07" },
		/*08*/ {"Vampires_Quest08",  CRadiantEnum::kRadiant_DGU, 0x0056FD, 0x000000, 255, "Completionist_DLC1RV04" },
		/*09*/ {"Vampires_Quest09",  CRadiantEnum::kRadiant_DGU, 0x004C1B, 0x000000, 255, "Completionist_DLC1RV01" },
	};

	CQuestData QuestData[]
	{
		/*00*/ {"Vampires_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "DLC1RV09"},
		/*01*/ {"Vampires_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "DLC1RV10"},
		/*02*/ {"Vampires_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "DLC1RV06"},
		/*03*/ {"Vampires_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "DLC1RV08"},
		/*04*/ {"Vampires_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV05"},
		/*05*/ {"Vampires_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV02"},
		/*06*/ {"Vampires_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV03"},
		/*07*/ {"Vampires_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV07"},
		/*08*/ {"Vampires_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV04"},
		/*09*/ {"Vampires_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "DLC1RV01"},
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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 26));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};