#include "Structs.hpp"
#include "CQuests_Misc_SK.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Misc_SK {
	using namespace CFramework_Master;

	CQuestData QuestData_Side[]{
	{"Side_SK_Quest00", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformCaravansA"},
	{"Side_SK_Quest01", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 199,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DarkwaterCrossingDerkeethusRescueQuest"},
	{"Side_SK_Quest02", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformDushnikhYalA"},
	{"Side_SK_Quest03", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunHunterQST"},
	{"Side_SK_Quest04", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 99,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FavorOrcsIntro"},
	{"Side_SK_Quest05", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 199,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformHeartwoodMill"},
	{"Side_SK_Quest06", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformHighHrothgarA"},
	{"Side_SK_Quest07", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 99,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformKarthwastenA"},
	{"Side_SK_Quest08", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 99,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformKolskeggrA"},
	{"Side_SK_Quest09", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformKynesgroveA"},
	{"Side_SK_Quest10", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA06"},
	{"Side_SK_Quest11", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 199,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformMerryfairFarm"},
	{"Side_SK_Quest12", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 59,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunMossMotherQST"},
	{"Side_SK_Quest13", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 99,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformOldHroldanB"},
	{"Side_SK_Quest14", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA02"},
	{"Side_SK_Quest15", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformSalviusFarmA"},
	{"Side_SK_Quest16", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 199,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformSarethiFarm"},
	{"Side_SK_Quest17", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA04"},
	{"Side_SK_Quest18", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA01"},
	{"Side_SK_Quest19", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA13"},
	{"Side_SK_Quest20", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 19,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformSoljundsSinkholeA"},
	{"Side_SK_Quest21", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA09"},
	{"Side_SK_Quest22", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "FreeformStonehillsA"},
	{"Side_SK_Quest23", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DA14"},
	{"Side_SK_Quest24", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kPast, 199,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunGauldursonQST"},
	{"Side_SK_Quest25", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 00,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "NN01"},
	};

	CQuestData QuestData_Radi[]{
	{"Radi_SK_Quest00", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor013Iddra",				CRadiantEnum::kRadiant_One }, "Favor013"},
	{"Radi_SK_Quest01", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_BountyBQ01",					CRadiantEnum::kRadiant_Bty }, "BQ01"},
	{"Radi_SK_Quest02", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_BountyBQ04",					CRadiantEnum::kRadiant_Bty }, "BQ04"},
	{"Radi_SK_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_BountyBQ03",					CRadiantEnum::kRadiant_Bty }, "BQ03"},
	{"Radi_SK_Quest04", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_BountyBQ02",					CRadiantEnum::kRadiant_Bty }, "BQ02"},
	{"Radi_SK_Quest05", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor_ChopWoodAeri",			CRadiantEnum::kRadiant_Def }, "FavorJobsChopWood"},
	{"Radi_SK_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor_ChopWoodGrosta",		CRadiantEnum::kRadiant_Def }, "FavorJobsChopWood"},
	{"Radi_SK_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor_ChopWoodGannaUriel",	CRadiantEnum::kRadiant_Def }, "FavorJobsChopWood"},
	{"Radi_SK_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor_ChopWoodGilfre",		CRadiantEnum::kRadiant_Def }, "FavorJobsChopWood"},
	{"Radi_SK_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor_ChopWoodHjorunn",		CRadiantEnum::kRadiant_Def }, "FavorJobsChopWood"},
	{"Radi_SK_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor001Aeri",				CRadiantEnum::kRadiant_One }, "Favor001"},
	{"Radi_SK_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor001Sondas",				CRadiantEnum::kRadiant_One }, "Favor001"},
	{"Radi_SK_Quest12", CFlagEnum::kRadi, CCompEnum::kStage, { CStageEnum::kPast, 25,	0  }, { " ",										CRadiantEnum::kRadiant_Non }, "FreeformSkyHavenTempleB"},
	{"Radi_SK_Quest13", CFlagEnum::kRadi, CCompEnum::kStage, { CStageEnum::kPast, 10,	0  }, { " ",										CRadiantEnum::kRadiant_Non }, "FreeformSkyHavenTempleD"},
	{"Radi_SK_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor205Roggi",				CRadiantEnum::kRadiant_One }, "Favor205"},
	{"Radi_SK_Quest16", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestAddvild",		CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest17", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestAlfhild",		CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestAvrusa",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestBelyn",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestBolfrida",		CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestKatla",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestSeverio",		CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestSynda",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest24", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestTulvur",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest25", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorHarvestVigdis",			CRadiantEnum::kRadiant_Def }, "FavorJobsGatherWheat"},
	{"Radi_SK_Quest26", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor104Annekke",			CRadiantEnum::kRadiant_One }, "Favor104"},
	{"Radi_SK_Quest27", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreDushnamub",		CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest28", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreGestur",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest29", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreGharol",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest30", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreGrogmar",		CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest31", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreKjeld",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest32", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOrePavo",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest33", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOrePerth",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest34", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreShuftharz",		CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest35", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreSkaggi",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest36", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreThorgar",		CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest37", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_FavorMineOreVerner",			CRadiantEnum::kRadiant_Def }, "FavorJobsMineOre"},
	{"Radi_SK_Quest38", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor017Burguk",				CRadiantEnum::kRadiant_One }, "Favor017"},
	{"Radi_SK_Quest39", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor017Larak",				CRadiantEnum::kRadiant_One }, "Favor017"},
	{"Radi_SK_Quest40", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 00,	0  }, { "Completionist_Favor017Mauhulakh",			CRadiantEnum::kRadiant_One }, "Favor017"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		SinkEvents();

		IdenArray_Side.clear();
		NameArray_Side.clear();
		RadiArray_Side.clear();
		NameArray_Side.clear();
		KeysArray_Side.clear();

		IdenArray_Radi.clear();
		NameArray_Radi.clear();
		RadiArray_Radi.clear();
		NameArray_Radi.clear();
		KeysArray_Radi.clear();

		for (auto i = 0; i < std::extent_v<decltype(QuestData_Side)>; i++) {
			auto& Data = QuestData_Side[i];

			KeysArray_Side.push_back(fmt::format("{:s}_Key"sv, Data.UDID));
			NameArray_Side.push_back(fmt::format("${:s}_Name"sv, Data.UDID));
			TextArray_Side.push_back(fmt::format("${:s}_Data"sv, Data.UDID));
			RadiArray_Side.push_back(std::to_underlying(Data.FLAG));
			IdenArray_Side.push_back(Data.EDID);
		}

		for (auto i = 0; i < std::extent_v<decltype(QuestData_Radi)>; i++) {
			auto& Data = QuestData_Radi[i];

			KeysArray_Radi.push_back(fmt::format("{:s}_Key"sv, Data.UDID));
			NameArray_Radi.push_back(fmt::format("${:s}_Name"sv, Data.UDID));
			TextArray_Radi.push_back(fmt::format("${:s}_Data"sv, Data.UDID));
			RadiArray_Radi.push_back(std::to_underlying(Data.FLAG));
			IdenArray_Radi.push_back(Data.EDID);
		}

		assert(KeysArray_Side.size() == ArraySize);
		assert(IdenArray_Side.size() == ArraySize);
		assert(NameArray_Side.size() == ArraySize);
		assert(RadiArray_Side.size() == ArraySize);
		assert(TextArray_Side.size() == ArraySize);

		assert(KeysArray_Radi.size() == ArraySize);
		assert(IdenArray_Radi.size() == ArraySize);
		assert(NameArray_Radi.size() == ArraySize);
		assert(RadiArray_Radi.size() == ArraySize);
		assert(TextArray_Radi.size() == ArraySize);

		BoolArray_Side = std::vector<bool>(ArraySize, false);
		BoolArray_Radi = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto userinterface = RE::UI::GetSingleton();
		userinterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		CHandler::UpdateCompletion();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateCompletion() {

		for (auto i = 0; i < std::extent_v<decltype(QuestData_Side)>; i++) {
			auto& Data = QuestData_Side[i];

			if (std::ranges::find(ExcludedQuestsArray, Data.EDID) != ExcludedQuestsArray.end()) {
				continue;
			}

			switch (Data.COMP) {

			case CCompEnum::kStand: {
				BoolArray_Side[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Side[i]) || FrameworkAPI::IsCompleted_N(KeysArray_Side[i], IdenArray_Side[i]);
				break;
			}

			case CCompEnum::kStage: {
				BoolArray_Side[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Side[i]) || FrameworkAPI::IsCompleted_P(KeysArray_Side[i], IdenArray_Side[i], Data.STAGE.VALUE) || (Data.STAGE.OPVALUE != 0 && FrameworkAPI::IsCompleted_P(KeysArray_Side[i], IdenArray_Side[i], Data.STAGE.OPVALUE));
				break;
			}

			default: {
				BoolArray_Side[i] = false;
				break;
			}
			}
		}

		for (auto i = 0; i < std::extent_v<decltype(QuestData_Radi)>; i++) {
			auto& Data = QuestData_Radi[i];

			if (std::ranges::find(ExcludedQuestsArray, Data.EDID) != ExcludedQuestsArray.end()) {
				continue;
			}

			switch (Data.COMP) {

			case CCompEnum::kGlobl: {
				BoolArray_Radi[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Radi[i]) || FrameworkAPI::IsCompleted_G(KeysArray_Radi[i], IdenArray_Radi[i], Data.GLOBAL.NAME, Data.GLOBAL.VALUE);
				break;
			}

			case CCompEnum::kStage: {
				BoolArray_Radi[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Radi[i]) || FrameworkAPI::IsCompleted_P(KeysArray_Radi[i], IdenArray_Radi[i], Data.STAGE.VALUE) || (Data.STAGE.OPVALUE != 0 && FrameworkAPI::IsCompleted_P(KeysArray_Radi[i], IdenArray_Radi[i], Data.STAGE.OPVALUE));
				break;
			}

			default: {
				BoolArray_Radi[i] = false;
				break;
			}
			}
		}
	};
}