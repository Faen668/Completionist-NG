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
		{"Solitude_Quest04", CFlagEnum::kSide, CCompEnum::kFavor, "Favor109"},
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
		{"Solitude_Quest15", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Solitude_Quest16", CFlagEnum::kSide, CCompEnum::kFavor, "Favor205"},
		{"Solitude_Quest17", CFlagEnum::kSide, CCompEnum::kFavor, "Favor104"},
		{"Solitude_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsBeggars"},
		{"Solitude_Quest21", CFlagEnum::kSide, CCompEnum::kFavor, "Favor110"},
		{"Solitude_Quest22", CFlagEnum::kSide, CCompEnum::kThane, "Favor252"},
	};

	CFavorData FavorData[]{
		/*04*/ {"Solitude_Quest04", 0x04214B, "Skyrim.esm", 0x0132aa, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*15*/ {"Solitude_Quest15", 0x0CA20A, "Skyrim.esm", 0x01329d, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*16*/ {"Solitude_Quest16", 0x04D6DA, "Skyrim.esm", 0x01329c, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*17*/ {"Solitude_Quest17", 0x06FD64, "Skyrim.esm", 0x01325f, "Skyrim.esm", CQuestProcessor::kExcluded},
		/*21*/ {"Solitude_Quest21", 0x0CA43B, "Skyrim.esm", 0x041fb8, "Skyrim.esm", CQuestProcessor::kExcluded},
	};

	CRadiantData RadiantData[]{
		/*18*/ {"Solitude_Quest18",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		/*19*/ {"Solitude_Quest19",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
		/*20*/ {"Solitude_Quest20",  CRadiantEnum::kRadiant_Def, 0, 0, 0, CQuestProcessor::kExcluded},
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

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initRadiantData(RadiantData)
				->initStageData(StageData)
				->initThaneData(ThaneData)
				->initFavorData(FavorData)
				->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 12, QuestData[i].unique_identifier));

		}
	};
};