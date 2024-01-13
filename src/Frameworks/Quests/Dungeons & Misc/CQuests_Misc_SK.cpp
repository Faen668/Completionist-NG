#include "CQuests_Misc_SK.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Misc_SK
{
	CQuestData QuestData_S[]{
		{"Radi_SK_Quest00", CFlagEnum::kSide, CCompEnum::kFavor, "Favor013"},
		{"Side_SK_Quest00", CFlagEnum::kSide, CCompEnum::kStage, "FreeformCaravansA"},
		{"Radi_SK_Quest10", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001" },
		{"Radi_SK_Quest11", CFlagEnum::kSide, CCompEnum::kFavor, "Favor001" },
		{"Radi_SK_Quest14", CFlagEnum::kSide, CCompEnum::kFavor, "Favor205" },
		{"Side_SK_Quest01", CFlagEnum::kSide, CCompEnum::kStage, "DarkwaterCrossingDerkeethusRescueQuest"},
		{"Radi_SK_Quest38", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017" },
		{"Radi_SK_Quest39", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017" },
		{"Radi_SK_Quest40", CFlagEnum::kSide, CCompEnum::kFavor, "Favor017" },
		{"Side_SK_Quest02", CFlagEnum::kSide, CCompEnum::kStage, "FreeformDushnikhYalA"},
		{"Radi_SK_Quest26", CFlagEnum::kSide, CCompEnum::kFavor, "Favor104" },
		{"Side_SK_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "dunHunterQST"},
		{"Side_SK_Quest04", CFlagEnum::kSide, CCompEnum::kStage, "FavorOrcsIntro"},
		{"Side_SK_Quest05", CFlagEnum::kSide, CCompEnum::kStage, "FreeformHeartwoodMill"},
		{"Side_SK_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "FreeformHighHrothgarA"},
		{"Side_SK_Quest07", CFlagEnum::kSide, CCompEnum::kStage, "FreeformKarthwastenA"},
		{"Side_SK_Quest08", CFlagEnum::kSide, CCompEnum::kStage, "FreeformKolskeggrA"},
		{"Side_SK_Quest09", CFlagEnum::kSide, CCompEnum::kStage, "FreeformKynesgroveA"},
		{"Side_SK_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DA06"},
		{"Side_SK_Quest11", CFlagEnum::kSide, CCompEnum::kStage, "FreeformMerryfairFarm"},
		{"Side_SK_Quest12", CFlagEnum::kSide, CCompEnum::kStage, "dunMossMotherQST"},
		{"Side_SK_Quest13", CFlagEnum::kSide, CCompEnum::kStage, "FreeformOldHroldanB"},
		{"Side_SK_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "DA02"},
		{"Side_SK_Quest15", CFlagEnum::kSide, CCompEnum::kStage, "FreeformSalviusFarmA"},
		{"Side_SK_Quest16", CFlagEnum::kSide, CCompEnum::kStage, "FreeformSarethiFarm"},
		{"Side_SK_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "DA04"},
		{"Side_SK_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "DA01"},
		{"Side_SK_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "DA13"},
		{"Side_SK_Quest20", CFlagEnum::kSide, CCompEnum::kStage, "FreeformSoljundsSinkholeA"},
		{"Side_SK_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "DA09"},
		{"Side_SK_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "FreeformStonehillsA"},
		{"Side_SK_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "DA14"},
		{"Side_SK_Quest24", CFlagEnum::kSide, CCompEnum::kStage, "dunGauldursonQST"},
		{"Side_SK_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "NN01"},
	};

	CStageData StageData_S[] {
		{"Side_SK_Quest00", CStageEnum::kPast, 19},
		{"Side_SK_Quest01", CStageEnum::kPast, 199},
		{"Side_SK_Quest02", CStageEnum::kPast, 19},
		{"Side_SK_Quest04", CStageEnum::kPast, 99},
		{"Side_SK_Quest05", CStageEnum::kPast, 199},
		{"Side_SK_Quest06", CStageEnum::kDone, 20},
		{"Side_SK_Quest07", CStageEnum::kPast, 99},
		{"Side_SK_Quest08", CStageEnum::kPast, 99},
		{"Side_SK_Quest09", CStageEnum::kPast, 19},
		{"Side_SK_Quest11", CStageEnum::kPast, 199},
		{"Side_SK_Quest12", CStageEnum::kPast, 59},
		{"Side_SK_Quest13", CStageEnum::kPast, 99},
		{"Side_SK_Quest15", CStageEnum::kPast, 19},
		{"Side_SK_Quest16", CStageEnum::kPast, 199},
		{"Side_SK_Quest20", CStageEnum::kPast, 19},
		{"Side_SK_Quest24", CStageEnum::kPast, 199},
	};

	CFavorData FavorData_S[]{
		{"Radi_SK_Quest00", 0x0CA20A, "Skyrim.esm", 0x013662, "Skyrim.esm", CQuestProcessor::kExcluded}, //A Few Words with You (Iddra)
		{"Radi_SK_Quest10", 0x0403B1, "Skyrim.esm", 0x01360b, "Skyrim.esm", CQuestProcessor::kExcluded}, //Delivery (Aeri)
		{"Radi_SK_Quest11", 0x0403B1, "Skyrim.esm", 0x01366b, "Skyrim.esm", CQuestProcessor::kExcluded}, //Delivery(Sondas)
		{"Radi_SK_Quest14", 0x04D6DA, "Skyrim.esm", 0x01403f, "Skyrim.esm", CQuestProcessor::kExcluded}, //Dungeon Delving (Roggi)
		{"Radi_SK_Quest26", 0x06FD64, "Skyrim.esm", 0x013666, "Skyrim.esm", CQuestProcessor::kExcluded}, //Kill the Bandit Leader (Annekke)
		{"Radi_SK_Quest38", 0x0CA213, "Skyrim.esm", 0x013b79, "Skyrim.esm", CQuestProcessor::kExcluded}, //Fight! (Chief Burguk)
		{"Radi_SK_Quest39", 0x0CA213, "Skyrim.esm", 0x019951, "Skyrim.esm", CQuestProcessor::kExcluded}, //Fight! (Chief Larak)
		{"Radi_SK_Quest40", 0x0CA213, "Skyrim.esm", 0x01b075, "Skyrim.esm", CQuestProcessor::kExcluded}, //Fight! (Chief Mauhulakh)
	};

	CQuestData QuestData_R[]{
		{"Radi_SK_Quest01", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ01"},
		{"Radi_SK_Quest02", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ04"},
		{"Radi_SK_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ03"},
		{"Radi_SK_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ02"},
		{"Radi_SK_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Radi_SK_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Radi_SK_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Radi_SK_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Radi_SK_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		{"Radi_SK_Quest12", CFlagEnum::kRadi, CCompEnum::kGlobl, "FreeformSkyHavenTempleB"},
		{"Radi_SK_Quest13", CFlagEnum::kRadi, CCompEnum::kGlobl, "FreeformSkyHavenTempleD"},
		{"Radi_SK_Quest16", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest17", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest24", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest25", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		{"Radi_SK_Quest27", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest28", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest29", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest30", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest31", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest32", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest33", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest34", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest35", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest36", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		{"Radi_SK_Quest37", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
	};

	CRadiantData RadiantData_R[]{
		{"Radi_SK_Quest01",  CRadiantEnum::kRadiant_Bty, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest02",  CRadiantEnum::kRadiant_Bty, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest03",  CRadiantEnum::kRadiant_Bty, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest04",  CRadiantEnum::kRadiant_Bty, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest05",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest06",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest07",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest08",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest09",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest12",  CRadiantEnum::kRadiant_BLD, 0x0E3E67,0,100, CQuestProcessor::kIncluded},
		{"Radi_SK_Quest13",  CRadiantEnum::kRadiant_BLD, 0x0E6DF1,0,100, CQuestProcessor::kIncluded},
		{"Radi_SK_Quest16",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest17",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest18",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest19",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest20",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest21",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest22",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest23",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest24",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest25",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest27",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest28",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest29",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest30",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest31",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest32",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest33",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest34",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest35",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest36",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
		{"Radi_SK_Quest37",  CRadiantEnum::kRadiant_Def, 0,0,0, CQuestProcessor::kExcluded},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData_S)>; i++)
		{
			QuestData_S[i].init()->initStageData(StageData_S)->initFavorData(FavorData_S)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_S[i], QuestData_S[i].GetName(), 28, QuestData_S[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(QuestData_R)>; i++)
		{
			QuestData_R[i].init()->initRadiantData(RadiantData_R)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_R[i], QuestData_R[i].GetName(), 29, QuestData_R[i].unique_identifier));
		}
	};
};