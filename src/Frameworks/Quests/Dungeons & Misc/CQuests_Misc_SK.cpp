#include "CQuests_Misc_SK.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Misc_SK
{
	CStageData StageData_S[] {
		{"Side_SK_Quest00", CStageEnum::kPast, 19		, 0 },
		{"Side_SK_Quest01", CStageEnum::kPast, 199		, 0 },
		{"Side_SK_Quest02", CStageEnum::kPast, 19		, 0 },
		{"Side_SK_Quest04", CStageEnum::kPast, 99		, 0 },
		{"Side_SK_Quest05", CStageEnum::kPast, 199		, 0 },
		{"Side_SK_Quest06", CStageEnum::kDone, 20		, 0 },
		{"Side_SK_Quest07", CStageEnum::kPast, 99		, 0 },
		{"Side_SK_Quest08", CStageEnum::kPast, 99		, 0 },
		{"Side_SK_Quest09", CStageEnum::kPast, 19		, 0 },
		{"Side_SK_Quest11", CStageEnum::kPast, 199   	, 0 },
		{"Side_SK_Quest12", CStageEnum::kPast, 59		, 0 },
		{"Side_SK_Quest13", CStageEnum::kPast, 99		, 0 },
		{"Side_SK_Quest15", CStageEnum::kPast, 19		, 0 },
		{"Side_SK_Quest16", CStageEnum::kPast, 199		, 0 },
		{"Side_SK_Quest20", CStageEnum::kPast, 19		, 0 },
		{"Side_SK_Quest24", CStageEnum::kPast, 199		, 0 },
	};

	CQuestData QuestData_S[]{
		{"Side_SK_Quest00", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformCaravansA"},
		{"Side_SK_Quest01", CFlagEnum::kSide,  CCompEnum::kStage, "DarkwaterCrossingDerkeethusRescueQuest"},
		{"Side_SK_Quest02", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformDushnikhYalA"},
		{"Side_SK_Quest03", CFlagEnum::kSide,  CCompEnum::kStand, "dunHunterQST"},
		{"Side_SK_Quest04", CFlagEnum::kSide,  CCompEnum::kStage, "FavorOrcsIntro"},
		{"Side_SK_Quest05", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformHeartwoodMill"},
		{"Side_SK_Quest06", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformHighHrothgarA"},
		{"Side_SK_Quest07", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformKarthwastenA"},
		{"Side_SK_Quest08", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformKolskeggrA"},
		{"Side_SK_Quest09", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformKynesgroveA"},
		{"Side_SK_Quest10", CFlagEnum::kSide,  CCompEnum::kStand, "DA06"},
		{"Side_SK_Quest11", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformMerryfairFarm"},
		{"Side_SK_Quest12", CFlagEnum::kSide,  CCompEnum::kStage, "dunMossMotherQST"},
		{"Side_SK_Quest13", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformOldHroldanB"},
		{"Side_SK_Quest14", CFlagEnum::kSide,  CCompEnum::kStand, "DA02"},
		{"Side_SK_Quest15", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformSalviusFarmA"},
		{"Side_SK_Quest16", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformSarethiFarm"},
		{"Side_SK_Quest17", CFlagEnum::kSide,  CCompEnum::kStand, "DA04"},
		{"Side_SK_Quest18", CFlagEnum::kSide,  CCompEnum::kStand, "DA01"},
		{"Side_SK_Quest19", CFlagEnum::kSide,  CCompEnum::kStand, "DA13"},
		{"Side_SK_Quest20", CFlagEnum::kSide,  CCompEnum::kStage, "FreeformSoljundsSinkholeA"},
		{"Side_SK_Quest21", CFlagEnum::kSide,  CCompEnum::kStand, "DA09"},
		{"Side_SK_Quest22", CFlagEnum::kSide,  CCompEnum::kStand, "FreeformStonehillsA"},
		{"Side_SK_Quest23", CFlagEnum::kSide,  CCompEnum::kStand, "DA14"},
		{"Side_SK_Quest24", CFlagEnum::kSide,  CCompEnum::kStage, "dunGauldursonQST"},
		{"Side_SK_Quest25", CFlagEnum::kSide,  CCompEnum::kStand, "NN01"},
	};

	CStageData StageData_R[]{
		{"Radi_SK_Quest12", CStageEnum::kPast, 25, 0 },
		{"Radi_SK_Quest13", CStageEnum::kPast, 10, 0 },
	};

	CRadiantData RadiantData[]{
		/*00*/ {"Radi_SK_Quest00",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor013Iddra"},
		/*01*/ {"Radi_SK_Quest01",  CRadiantEnum::kRadiant_Bty, 0,0,0, "Completionist_BountyBQ01" },
		/*02*/ {"Radi_SK_Quest02",  CRadiantEnum::kRadiant_Bty, 0,0,0, "Completionist_BountyBQ04" },
		/*03*/ {"Radi_SK_Quest03",  CRadiantEnum::kRadiant_Bty, 0,0,0, "Completionist_BountyBQ03" },
		/*04*/ {"Radi_SK_Quest04",  CRadiantEnum::kRadiant_Bty, 0,0,0, "Completionist_BountyBQ02" },
		/*05*/ {"Radi_SK_Quest05",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodAeri" },
		/*06*/ {"Radi_SK_Quest06",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodGrosta" },
		/*07*/ {"Radi_SK_Quest07",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodGannaUriel" },
		/*08*/ {"Radi_SK_Quest08",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodGilfre" },
		/*09*/ {"Radi_SK_Quest09",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_Favor_ChopWoodHjorunn" },
		/*10*/ {"Radi_SK_Quest10",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor001Aeri" },
		/*11*/ {"Radi_SK_Quest11",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor001Sondas" },
		/*12*/ {"Radi_SK_Quest14",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor205Roggi" },
		/*13*/ {"Radi_SK_Quest16",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestAddvild" },
		/*14*/ {"Radi_SK_Quest17",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestAlfhild" },
		/*15*/ {"Radi_SK_Quest18",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestAvrusa" },
		/*16*/ {"Radi_SK_Quest19",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestBelyn" },
		/*17*/ {"Radi_SK_Quest20",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestBolfrida" },
		/*18*/ {"Radi_SK_Quest21",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestKatla" },
		/*19*/ {"Radi_SK_Quest22",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestSeverio" },
		/*20*/ {"Radi_SK_Quest23",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestSynda" },
		/*21*/ {"Radi_SK_Quest24",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestTulvur" },
		/*22*/ {"Radi_SK_Quest25",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorHarvestVigdis" },
		/*23*/ {"Radi_SK_Quest26",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor104Annekke" },
		/*24*/ {"Radi_SK_Quest27",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreDushnamub" },
		/*25*/ {"Radi_SK_Quest28",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreGestur" },
		/*26*/ {"Radi_SK_Quest29",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreGharol" },
		/*27*/ {"Radi_SK_Quest30",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreGrogmar" },
		/*28*/ {"Radi_SK_Quest31",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreKjeld" },
		/*29*/ {"Radi_SK_Quest32",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOrePavo" },
		/*30*/ {"Radi_SK_Quest33",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOrePerth" },
		/*31*/ {"Radi_SK_Quest34",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreShuftharz" },
		/*32*/ {"Radi_SK_Quest35",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreSkaggi" },
		/*33*/ {"Radi_SK_Quest36",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreThorgar" },
		/*34*/ {"Radi_SK_Quest37",  CRadiantEnum::kRadiant_Def, 0,0,0, "Completionist_FavorMineOreVerner" },
		/*35*/ {"Radi_SK_Quest38",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor017Burguk" },
		/*36*/ {"Radi_SK_Quest39",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor017Larak" },
		/*37*/ {"Radi_SK_Quest40",  CRadiantEnum::kRadiant_DF1, 0,0,0, "Completionist_Favor017Mauhulakh" },
	};

	CQuestData QuestData_R[]{
		/*00*/ {"Radi_SK_Quest00", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor013"},
		/*01*/ {"Radi_SK_Quest01", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ01"},
		/*02*/ {"Radi_SK_Quest02", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ04"},
		/*03*/ {"Radi_SK_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ03"},
		/*04*/ {"Radi_SK_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, "BQ02"},
		/*05*/ {"Radi_SK_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*06*/ {"Radi_SK_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*07*/ {"Radi_SK_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*08*/ {"Radi_SK_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*09*/ {"Radi_SK_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsChopWood"},
		/*10*/ {"Radi_SK_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor001"},
		/*11*/ {"Radi_SK_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor001"},
		/*NA*/ {"Radi_SK_Quest12", CFlagEnum::kRadi, CCompEnum::kStage, "FreeformSkyHavenTempleB"},
		/*NA*/ {"Radi_SK_Quest13", CFlagEnum::kRadi, CCompEnum::kStage, "FreeformSkyHavenTempleD"},
		/*12*/ {"Radi_SK_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor205"},
		/*13*/ {"Radi_SK_Quest16", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*14*/ {"Radi_SK_Quest17", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*15*/ {"Radi_SK_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*16*/ {"Radi_SK_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*17*/ {"Radi_SK_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*18*/ {"Radi_SK_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*19*/ {"Radi_SK_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*20*/ {"Radi_SK_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*21*/ {"Radi_SK_Quest24", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*22*/ {"Radi_SK_Quest25", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsGatherWheat"},
		/*23*/ {"Radi_SK_Quest26", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor104"},
		/*24*/ {"Radi_SK_Quest27", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*25*/ {"Radi_SK_Quest28", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*26*/ {"Radi_SK_Quest29", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*27*/ {"Radi_SK_Quest30", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*28*/ {"Radi_SK_Quest31", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*29*/ {"Radi_SK_Quest32", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*30*/ {"Radi_SK_Quest33", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*31*/ {"Radi_SK_Quest34", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*32*/ {"Radi_SK_Quest35", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*33*/ {"Radi_SK_Quest36", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*34*/ {"Radi_SK_Quest37", CFlagEnum::kRadi, CCompEnum::kGlobl, "FavorJobsMineOre"},
		/*35*/ {"Radi_SK_Quest38", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor017"},
		/*36*/ {"Radi_SK_Quest39", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor017"},
		/*37*/ {"Radi_SK_Quest40", CFlagEnum::kRadi, CCompEnum::kGlobl, "Favor017"},
	};

	CArrayData QuestArrays[]{
		{&IdenArray_S, &NameArray_S, &TextArray_S, &BoolArray_S, &RadiArray_S, &KeysArray_S },
		{&IdenArray_R, &NameArray_R, &TextArray_R, &BoolArray_R, &RadiArray_R, &KeysArray_R },
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData_S)>; i++)
		{
			QuestData_S[i].init()
				->initQuestData(&QuestArrays[0])
				->initStageData(StageData_S);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_S[i], QuestData_S[i].GetName(), 28, QuestData_S[i].unique_identifier));
		}

		for (auto i = 0; i < std::extent_v<decltype(QuestData_R)>; i++)
		{
			QuestData_R[i].init()
				->initQuestData(&QuestArrays[1])
				->initStageData(StageData_R)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData_R[i], QuestData_R[i].GetName(), 29, QuestData_R[i].unique_identifier));
		}

		BoolArray_S = std::vector<bool>(CArraySize, false);
		BoolArray_R = std::vector<bool>(CArraySize, false);
	};
};