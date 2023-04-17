#include "CQuests_CollegeOfWinterhold.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_CollegeOfWinterhold 
{
	CRadiantData RadiantData[] {
		/*22*/ {"CollegeOfWinterhold_Quest22",  CRadiantEnum::kRadiant_COL, 0x0C1E72, 0x000000, 200, "Completionist_CollegeMGR30" },
		/*23*/ {"CollegeOfWinterhold_Quest23",  CRadiantEnum::kRadiant_COL, 0x028A13, 0x000000, 200, "Completionist_CollegeMGR10" },
		/*24*/ {"CollegeOfWinterhold_Quest24",  CRadiantEnum::kRadiant_COL, 0x028A24, 0x0BC096, 200, "Completionist_CollegeMGR20" },
		/*25*/ {"CollegeOfWinterhold_Quest25",  CRadiantEnum::kRadiant_COL, 0x028A1E, 0x000000, 200, "Completionist_CollegeMGR12" },
		/*26*/ {"CollegeOfWinterhold_Quest26",  CRadiantEnum::kRadiant_COL, 0x0266F2, 0x000000, 200, "Completionist_CollegeMGR02" },
		/*27*/ {"CollegeOfWinterhold_Quest27",  CRadiantEnum::kRadiant_COL, 0x05D2EA, 0x000000, 200, "Completionist_CollegeFreeformWinterholdCollegeA" },
		/*28*/ {"CollegeOfWinterhold_Quest28",  CRadiantEnum::kRadiant_COL, 0x028A19, 0x000000, 200, "Completionist_CollegeMGR11" },
		/*29*/ {"CollegeOfWinterhold_Quest29",  CRadiantEnum::kRadiant_COL, 0x05A609, 0x000000, 200, "Completionist_CollegeMGRRogue" },
		/*30*/ {"CollegeOfWinterhold_Quest30",  CRadiantEnum::kRadiant_COL, 0x0E0E04, 0x000000, 200, "Completionist_CollegeMGR21" },
		/*31*/ {"CollegeOfWinterhold_Quest31",  CRadiantEnum::kRadiant_COL, 0x0266F1, 0x000000, 200, "Completionist_CollegeMGR01" },
	};

	CStageData StageData[] {
		{"CollegeOfWinterhold_Quest16", CStageEnum::kPast, 79, 0},
	};

	CQuestData QuestData[] {
		/*00*/ {"CollegeOfWinterhold_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "MG01"},
		/*01*/ {"CollegeOfWinterhold_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "MG02"},
		/*02*/ {"CollegeOfWinterhold_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "MG03"},
		/*03*/ {"CollegeOfWinterhold_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "MG04"},
		/*04*/ {"CollegeOfWinterhold_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "mg06"},
		/*05*/ {"CollegeOfWinterhold_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "MG05"},
		/*06*/ {"CollegeOfWinterhold_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "MG07"},
		/*07*/ {"CollegeOfWinterhold_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "MG08"},
		/*08*/ {"CollegeOfWinterhold_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "MGRitual05"},
		/*09*/ {"CollegeOfWinterhold_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "MGRArniel01"},
		/*10*/ {"CollegeOfWinterhold_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "MGRArniel02"},
		/*11*/ {"CollegeOfWinterhold_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "MGRArniel03"},
		/*12*/ {"CollegeOfWinterhold_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "MGRArniel04"},
		/*13*/ {"CollegeOfWinterhold_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "MGRAppBrelyna01"},
		/*14*/ {"CollegeOfWinterhold_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "MGRitual03"},
		/*15*/ {"CollegeOfWinterhold_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "MGRitual01"},
		/*16*/ {"CollegeOfWinterhold_Quest16", CFlagEnum::kSide, CCompEnum::kStage, "dunMidden01QST"},
		/*17*/ {"CollegeOfWinterhold_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "MGRitual02"},
		/*18*/ {"CollegeOfWinterhold_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "MGRAppJzargo01"},
		/*19*/ {"CollegeOfWinterhold_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "MGRAppOnmund01"},
		/*20*/ {"CollegeOfWinterhold_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "MGRitual04"},
		/*21*/ {"CollegeOfWinterhold_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "FreeformWinterholdCollegeB"}, //CRF
		/*22*/ {"CollegeOfWinterhold_Quest22", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR30"},
		/*23*/ {"CollegeOfWinterhold_Quest23", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR10"},
		/*24*/ {"CollegeOfWinterhold_Quest24", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR20"},
		/*25*/ {"CollegeOfWinterhold_Quest25", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR12"}, //CRF
		/*26*/ {"CollegeOfWinterhold_Quest26", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR02"},
		/*27*/ {"CollegeOfWinterhold_Quest27", CFlagEnum::kSide, CCompEnum::kGlobl, "FreeformWinterholdCollegeA"}, //CRF
		/*28*/ {"CollegeOfWinterhold_Quest28", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR11"},
		/*29*/ {"CollegeOfWinterhold_Quest29", CFlagEnum::kSide, CCompEnum::kGlobl, "MGRRogue"}, //CRF
		/*30*/ {"CollegeOfWinterhold_Quest30", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR21"},
		/*31*/ {"CollegeOfWinterhold_Quest31", CFlagEnum::kSide, CCompEnum::kGlobl, "MGR01"}, //CRF
	};

	const std::vector<std::string> CRFQuests = {
		"FreeformWinterholdCollegeB", "MGR12", "FreeformWinterholdCollegeA", "MGRRogue", "MGR01"
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		
		auto CRF_Installed = Serialization::CompletionistData::IsModInstalled("Cutting Room Floor.esp");

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			// Handle Exclusions
			if (!CRF_Installed && std::ranges::find(CRFQuests, QuestData[i].editor_id) != CRFQuests.end()) {
				INFO("Excluded quest - {}", QuestData[i].editor_id); 
			}
			else {
				QuestData[i].init()
					->initQuestData(&QuestArrays)
					->initStageData(StageData)
					->initRadiantData(RadiantData);
				CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 21));
			}
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};