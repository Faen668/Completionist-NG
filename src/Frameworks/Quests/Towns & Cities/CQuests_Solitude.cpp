#include "CQuests_Solitude.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Solitude 
{
	CQuestData QuestData[]
	{
		{"Solitude_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform04"},
		{"Solitude_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform07"},
		{"Solitude_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "BardsCollegeLute"},
		{"Solitude_Quest03", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform02"},
		{"Solitude_Quest04", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor109"},
		{"Solitude_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "MS07"},
		{"Solitude_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform01"},
		{"Solitude_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "BardsCollegeFlute"},
		{"Solitude_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform03"},
		{"Solitude_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "BardsCollegeDrum"},
		{"Solitude_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "MS05"},
		{"Solitude_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "MS06Start"},
		{"Solitude_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "DA15"},
		{"Solitude_Quest13", CFlagEnum::kSide, CCompEnum::kStage, "SolitudeFreeform06"},
		{"Solitude_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "MS06"},
		{"Solitude_Quest15", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor013"},
		{"Solitude_Quest16", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor205"},
		{"Solitude_Quest17", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor104"},
		{"Solitude_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest21", CFlagEnum::kSide, CCompEnum::kGlobl, "Favor110"},
		{"Solitude_Quest22", CFlagEnum::kSide, CCompEnum::kThane, "FavorJarlsMakeFriends"},
	};

	CRadiantData RadiantData[]{
		/*04*/ {"Solitude_Quest04",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor109Sybille" },
		/*15*/ {"Solitude_Quest15",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor013OctieveSan" },
		/*16*/ {"Solitude_Quest16",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor205Noster" },
		/*17*/ {"Solitude_Quest17",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor104Ahtar" },
		/*18*/ {"Solitude_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarDervenin" },
		/*19*/ {"Solitude_Quest19",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarNoster" },
		/*20*/ {"Solitude_Quest20",  CRadiantEnum::kRadiant_Def, 0, 0, 0, "Completionist_FavorBeggarSvari" },
		/*21*/ {"Solitude_Quest21",  CRadiantEnum::kRadiant_DF1, 0, 0, 0, "Completionist_Favor110Aldis" },
	};

	CStageData StageData[]{
		{"Solitude_Quest00", CStageEnum::kPast, 199, 0},
		{"Solitude_Quest01", CStageEnum::kPast, 199, 0},
		{"Solitude_Quest03", CStageEnum::kPast, 199, 0},
		{"Solitude_Quest06", CStageEnum::kPast, 199, 0},
		{"Solitude_Quest08", CStageEnum::kPast, 199, 0},
		{"Solitude_Quest13", CStageEnum::kPast, 199, 0},
	};

	CThaneData ThaneData[]{
		{"Solitude_Quest22", "HaafingarImpGetOutofJail", "HaafingarSonsGetOutofJail"},
	};

	CArrayData ArrayData{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData)
				->initRadiantData(RadiantData)
				->initStageData(StageData)
				->initThaneData(ThaneData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 12));

		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};