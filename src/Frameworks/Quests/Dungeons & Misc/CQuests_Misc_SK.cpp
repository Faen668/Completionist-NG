#include "CQuests_Misc_SK.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Misc_SK {
	using namespace CFramework_Master;

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData_Side[] = {
	/*00*/ {"Side_SK_Quest00_Key", "$Side_SK_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest00_Data", "FreeformCaravansA"},
	/*01*/ {"Side_SK_Quest01_Key", "$Side_SK_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest01_Data", "DarkwaterCrossingDerkeethusRescueQuest"},
	/*02*/ {"Side_SK_Quest02_Key", "$Side_SK_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest02_Data", "FreeformDushnikhYalA"},
	/*03*/ {"Side_SK_Quest03_Key", "$Side_SK_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest03_Data", "dunHunterQST"},
	/*04*/ {"Side_SK_Quest04_Key", "$Side_SK_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest04_Data", "FavorOrcsIntro"},
	/*05*/ {"Side_SK_Quest05_Key", "$Side_SK_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest05_Data", "FreeformHeartwoodMill"},
	/*06*/ {"Side_SK_Quest06_Key", "$Side_SK_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest06_Data", "FreeformHighHrothgarA"},
	/*07*/ {"Side_SK_Quest07_Key", "$Side_SK_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest07_Data", "FreeformKarthwastenA"},
	/*08*/ {"Side_SK_Quest08_Key", "$Side_SK_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest08_Data", "FreeformKolskeggrA"},
	/*09*/ {"Side_SK_Quest09_Key", "$Side_SK_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest09_Data", "FreeformKynesgroveA"},
	/*10*/ {"Side_SK_Quest10_Key", "$Side_SK_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest10_Data", "DA06"},
	/*11*/ {"Side_SK_Quest11_Key", "$Side_SK_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest11_Data", "FreeformMerryfairFarm"},
	/*12*/ {"Side_SK_Quest12_Key", "$Side_SK_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest12_Data", "dunMossMotherQST"},
	/*13*/ {"Side_SK_Quest13_Key", "$Side_SK_Quest13_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest13_Data", "FreeformOldHroldanB"},
	/*14*/ {"Side_SK_Quest14_Key", "$Side_SK_Quest14_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest14_Data", "DA02"},
	/*15*/ {"Side_SK_Quest15_Key", "$Side_SK_Quest15_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest15_Data", "FreeformSalviusFarmA"},
	/*16*/ {"Side_SK_Quest16_Key", "$Side_SK_Quest16_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest16_Data", "FreeformSarethiFarm"},
	/*17*/ {"Side_SK_Quest17_Key", "$Side_SK_Quest17_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest17_Data", "DA04"},
	/*18*/ {"Side_SK_Quest18_Key", "$Side_SK_Quest18_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest18_Data", "DA01"},
	/*19*/ {"Side_SK_Quest19_Key", "$Side_SK_Quest19_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest19_Data", "DA13"},
	/*20*/ {"Side_SK_Quest20_Key", "$Side_SK_Quest20_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest20_Data", "FreeformSoljundsSinkholeA"},
	/*21*/ {"Side_SK_Quest21_Key", "$Side_SK_Quest21_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest21_Data", "DA09"},
	/*22*/ {"Side_SK_Quest22_Key", "$Side_SK_Quest22_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest22_Data", "FreeformStonehillsA"},
	/*23*/ {"Side_SK_Quest23_Key", "$Side_SK_Quest23_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest23_Data", "DA14"},
	/*24*/ {"Side_SK_Quest24_Key", "$Side_SK_Quest24_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest24_Data", "dunGauldursonQST"},
	/*25*/ {"Side_SK_Quest25_Key", "$Side_SK_Quest25_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Side_SK_Quest25_Data", "NN01"},
	};

	constexpr std::size_t StandardCompletion_Side[] = {
	3,10,14,17,18,19,21,22,23,25
	};

	constexpr std::pair<std::size_t, std::int32_t> StageCompletion_Side[] = {
	{  0, 19   },
	{  1, 199  },
	{  2, 19   },
	{  4, 99   },
	{  5, 199  },
	{  6, 19   },
	{  7, 99   },
	{  8, 99   },
	{  9, 19   },
	{  11, 199 },
	{  12, 59  },
	{  13, 99  },
	{  15, 19  },
	{  16, 199 },
	{  20, 19  },
	{  24, 199 },
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData_Radi[] = {
	/*00*/ {"Radi_SK_Quest00_Key", "$Radi_SK_Quest00_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest00_Data", "Favor013"},
	/*01*/ {"Radi_SK_Quest01_Key", "$Radi_SK_Quest01_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest01_Data", "BQ01"},
	/*02*/ {"Radi_SK_Quest02_Key", "$Radi_SK_Quest02_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest02_Data", "BQ04"},
	/*03*/ {"Radi_SK_Quest03_Key", "$Radi_SK_Quest03_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest03_Data", "BQ02"},
	/*04*/ {"Radi_SK_Quest04_Key", "$Radi_SK_Quest04_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest04_Data", "BQ03"},
	/*05*/ {"Radi_SK_Quest05_Key", "$Radi_SK_Quest05_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest05_Data", "FavorJobsChopWood"},
	/*06*/ {"Radi_SK_Quest06_Key", "$Radi_SK_Quest06_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest06_Data", "FavorJobsChopWood"},
	/*07*/ {"Radi_SK_Quest07_Key", "$Radi_SK_Quest07_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest07_Data", "FavorJobsChopWood"},
	/*08*/ {"Radi_SK_Quest08_Key", "$Radi_SK_Quest08_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest08_Data", "FavorJobsChopWood"},
	/*09*/ {"Radi_SK_Quest09_Key", "$Radi_SK_Quest09_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest09_Data", "FavorJobsChopWood"},
	/*10*/ {"Radi_SK_Quest10_Key", "$Radi_SK_Quest10_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest10_Data", "Favor001"},
	/*11*/ {"Radi_SK_Quest11_Key", "$Radi_SK_Quest11_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest11_Data", "Favor001"},
	/*12*/ {"Radi_SK_Quest12_Key", "$Radi_SK_Quest12_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest12_Data", "FreeformSkyHavenTempleB"},
	/*13*/ {"Radi_SK_Quest13_Key", "$Radi_SK_Quest13_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest13_Data", "FreeformSkyHavenTempleD"},
	/*14*/ {"Radi_SK_Quest14_Key", "$Radi_SK_Quest14_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest14_Data", "Favor205"},
	/*15*/ {"Radi_SK_Quest15_Key", "$Radi_SK_Quest15_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest15_Data", "FavorJobsDrunks"},
	/*16*/ {"Radi_SK_Quest16_Key", "$Radi_SK_Quest16_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest16_Data", "FavorJobsGatherWheat"},
	/*17*/ {"Radi_SK_Quest17_Key", "$Radi_SK_Quest17_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest17_Data", "FavorJobsGatherWheat"},
	/*18*/ {"Radi_SK_Quest18_Key", "$Radi_SK_Quest18_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest18_Data", "FavorJobsGatherWheat"},
	/*19*/ {"Radi_SK_Quest19_Key", "$Radi_SK_Quest19_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest19_Data", "FavorJobsGatherWheat"},
	/*20*/ {"Radi_SK_Quest20_Key", "$Radi_SK_Quest20_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest20_Data", "FavorJobsGatherWheat"},
	/*21*/ {"Radi_SK_Quest21_Key", "$Radi_SK_Quest21_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest21_Data", "FavorJobsGatherWheat"},
	/*22*/ {"Radi_SK_Quest22_Key", "$Radi_SK_Quest22_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest22_Data", "FavorJobsGatherWheat"},
	/*23*/ {"Radi_SK_Quest23_Key", "$Radi_SK_Quest23_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest23_Data", "FavorJobsGatherWheat"},
	/*24*/ {"Radi_SK_Quest24_Key", "$Radi_SK_Quest24_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest24_Data", "FavorJobsGatherWheat"},
	/*25*/ {"Radi_SK_Quest25_Key", "$Radi_SK_Quest25_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest25_Data", "FavorJobsGatherWheat"},
	/*26*/ {"Radi_SK_Quest26_Key", "$Radi_SK_Quest26_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest26_Data", "Favor104"},
	/*27*/ {"Radi_SK_Quest27_Key", "$Radi_SK_Quest27_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest27_Data", "FavorJobsMineOre"},
	/*28*/ {"Radi_SK_Quest28_Key", "$Radi_SK_Quest28_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest28_Data", "FavorJobsMineOre"},
	/*29*/ {"Radi_SK_Quest29_Key", "$Radi_SK_Quest29_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest29_Data", "FavorJobsMineOre"},
	/*30*/ {"Radi_SK_Quest30_Key", "$Radi_SK_Quest30_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest30_Data", "FavorJobsMineOre"},
	/*31*/ {"Radi_SK_Quest31_Key", "$Radi_SK_Quest31_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest31_Data", "FavorJobsMineOre"},
	/*32*/ {"Radi_SK_Quest32_Key", "$Radi_SK_Quest32_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest32_Data", "FavorJobsMineOre"},
	/*33*/ {"Radi_SK_Quest33_Key", "$Radi_SK_Quest33_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest33_Data", "FavorJobsMineOre"},
	/*34*/ {"Radi_SK_Quest34_Key", "$Radi_SK_Quest34_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest34_Data", "FavorJobsMineOre"},
	/*35*/ {"Radi_SK_Quest35_Key", "$Radi_SK_Quest35_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest35_Data", "FavorJobsMineOre"},
	/*36*/ {"Radi_SK_Quest36_Key", "$Radi_SK_Quest36_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest36_Data", "FavorJobsMineOre"},
	/*37*/ {"Radi_SK_Quest37_Key", "$Radi_SK_Quest37_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest37_Data", "FavorJobsMineOre"},
	/*38*/ {"Radi_SK_Quest38_Key", "$Radi_SK_Quest38_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest38_Data", "Favor017"},
	/*39*/ {"Radi_SK_Quest39_Key", "$Radi_SK_Quest39_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest39_Data", "Favor017"},
	/*40*/ {"Radi_SK_Quest40_Key", "$Radi_SK_Quest40_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Radi_SK_Quest40_Data", "Favor017"},
	};

	constexpr std::pair<std::size_t, std::int32_t> StageCompletion_Radi[] = {
	{ 12, 25 },
	{ 13, 10 },
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion_Radi[] = {
		{0, "Completionist_Favor013Iddra", 1},
		{1, "Completionist_BountyBQ01", RADIANT_BOUNTY_VALUE},
		{2, "Completionist_BountyBQ04", RADIANT_BOUNTY_VALUE},
		{3, "Completionist_BountyBQ02", RADIANT_BOUNTY_VALUE},
		{4, "Completionist_BountyBQ03", RADIANT_BOUNTY_VALUE},
		{5, "Completionist_Favor_ChopWoodAeri", RADIANT_COUNTER_VALUE},
		{6, "Completionist_Favor_ChopWoodGrosta", RADIANT_COUNTER_VALUE},
		{7, "Completionist_Favor_ChopWoodGannaUriel", RADIANT_COUNTER_VALUE},
		{8, "Completionist_Favor_ChopWoodGilfre", RADIANT_COUNTER_VALUE},
		{9, "Completionist_Favor_ChopWoodHjorunn", RADIANT_COUNTER_VALUE},
		{10, "Completionist_Favor001Aeri", 1},
		{11, "Completionist_Favor001Sondas", 1},
		{14, "Completionist_Favor205Roggi", 1},
		{15, "Completionist_FavorDrunksRazelan", RADIANT_COUNTER_VALUE},
		{16, "Completionist_FavorHarvestAddvild", RADIANT_COUNTER_VALUE},
		{17, "Completionist_FavorHarvestAlfhild", RADIANT_COUNTER_VALUE},
		{18, "Completionist_FavorHarvestAvrusa", RADIANT_COUNTER_VALUE},
		{19, "Completionist_FavorHarvestBelyn", RADIANT_COUNTER_VALUE},
		{20, "Completionist_FavorHarvestBolfrida", RADIANT_COUNTER_VALUE},
		{21, "Completionist_FavorHarvestKatla", RADIANT_COUNTER_VALUE},
		{22, "Completionist_FavorHarvestSeverio", RADIANT_COUNTER_VALUE},
		{23, "Completionist_FavorHarvestSynda", RADIANT_COUNTER_VALUE},
		{24, "Completionist_FavorHarvestTulvur", RADIANT_COUNTER_VALUE},
		{25, "Completionist_FavorHarvestVigdis", RADIANT_COUNTER_VALUE},
		{26, "Completionist_Favor104Annekke", 1},
		{27, "Completionist_FavorMineOreDushnamub", RADIANT_COUNTER_VALUE},
		{28, "Completionist_FavorMineOreGestur", RADIANT_COUNTER_VALUE},
		{29, "Completionist_FavorMineOreGharol", RADIANT_COUNTER_VALUE},
		{30, "Completionist_FavorMineOreGrogmar", RADIANT_COUNTER_VALUE},
		{31, "Completionist_FavorMineOreKjeld", RADIANT_COUNTER_VALUE},
		{32, "Completionist_FavorMineOrePavo", RADIANT_COUNTER_VALUE},
		{33, "Completionist_FavorMineOrePerth", RADIANT_COUNTER_VALUE},
		{34, "Completionist_FavorMineOreShuftharz", RADIANT_COUNTER_VALUE},
		{35, "Completionist_FavorMineOreSkaggi", RADIANT_COUNTER_VALUE},
		{36, "Completionist_FavorMineOreThorgar", RADIANT_COUNTER_VALUE},
		{37, "Completionist_FavorMineOreVerner", RADIANT_COUNTER_VALUE},
		{38, "Completionist_Favor017Burguk", 1},
		{39, "Completionist_Favor017Larak", 1},
		{40, "Completionist_Favor017Mauhulakh", 1},
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
		StgeArray_Side.clear();

		IdenArray_Radi.clear();
		NameArray_Radi.clear();
		RadiArray_Radi.clear();
		NameArray_Radi.clear();
		KeysArray_Radi.clear();
		StgeArray_Radi.clear();

		for (auto& [key, name, flag, stage, text, id] : QuestData_Side) {
			KeysArray_Side.push_back(key);
			NameArray_Side.push_back(name);
			RadiArray_Side.push_back(flag);
			TextArray_Side.push_back(text);
			IdenArray_Side.push_back(id);
			StgeArray_Side.push_back(stage);
		}

		for (auto& [key, name, flag, stage, text, id] : QuestData_Radi) {
			KeysArray_Radi.push_back(key);
			NameArray_Radi.push_back(name);
			RadiArray_Radi.push_back(flag);
			TextArray_Radi.push_back(text);
			IdenArray_Radi.push_back(id);
			StgeArray_Radi.push_back(stage);
		}

		assert(KeysArray_Side.size() == ArraySize);
		assert(IdenArray_Side.size() == ArraySize);
		assert(NameArray_Side.size() == ArraySize);
		assert(RadiArray_Side.size() == ArraySize);
		assert(TextArray_Side.size() == ArraySize);
		assert(StgeArray_Side.size() == ArraySize);

		assert(KeysArray_Radi.size() == ArraySize);
		assert(IdenArray_Radi.size() == ArraySize);
		assert(NameArray_Radi.size() == ArraySize);
		assert(RadiArray_Radi.size() == ArraySize);
		assert(TextArray_Radi.size() == ArraySize);
		assert(StgeArray_Radi.size() == ArraySize);

		BoolArray_Side = std::vector<bool>(ArraySize, false);
		BoolArray_Radi = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto userinterface = RE::UI::GetSingleton();
		userinterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(IdenArray_Side, quest->GetFormEditorID());
		if (t_pos != IdenArray_Side.end()) { 
			if (StgeArray_Side.at(std::distance(IdenArray_Side.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(KeysArray_Side.at(std::distance(IdenArray_Side.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, IdenArray_Side.at(std::distance(IdenArray_Side.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(IdenArray_Radi, quest->GetFormEditorID());
		if (t_pos != IdenArray_Radi.end()) {
			if (StgeArray_Radi.at(std::distance(IdenArray_Radi.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(KeysArray_Radi.at(std::distance(IdenArray_Radi.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, IdenArray_Radi.at(std::distance(IdenArray_Radi.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
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

		for (auto i : StandardCompletion_Side) {
			BoolArray_Side[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Side[i]) || FrameworkAPI::IsCompleted_N(KeysArray_Side[i], IdenArray_Side[i]);
		}

		for (auto& [i, stage] : StageCompletion_Side) {
			BoolArray_Side[i] = StgeArray_Side[i] ?
				FrameworkAPI::qIsOptionToggledInternal(KeysArray_Side[i]) || FrameworkAPI::IsCompleted_S(KeysArray_Side[i], IdenArray_Side[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(KeysArray_Side[i]) || FrameworkAPI::IsCompleted_P(KeysArray_Side[i], IdenArray_Side[i], stage);
		}

		for (auto& [i, global, value] : GlobalCompletion_Radi) {
			BoolArray_Radi[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray_Radi[i]) || FrameworkAPI::IsCompleted_G(KeysArray_Radi[i], IdenArray_Radi[i], global, value);
		}

		for (auto& [i, stage] : StageCompletion_Radi) {
			BoolArray_Radi[i] = StgeArray_Radi[i] ?
				FrameworkAPI::qIsOptionToggledInternal(KeysArray_Radi[i]) || FrameworkAPI::IsCompleted_S(KeysArray_Radi[i], IdenArray_Radi[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(KeysArray_Radi[i]) || FrameworkAPI::IsCompleted_P(KeysArray_Radi[i], IdenArray_Radi[i], stage);
		}
	};
}