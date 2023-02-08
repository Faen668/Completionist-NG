#include "CQuests_CollegeOfWinterhold.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CollegeOfWinterhold {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/ {"CollegeOfWinterhold_Quest00_Key", "$CollegeOfWinterhold_Quest00_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest00_Data", "MG01"},
		/*01*/ {"CollegeOfWinterhold_Quest01_Key", "$CollegeOfWinterhold_Quest01_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest01_Data", "MG02"},
		/*02*/ {"CollegeOfWinterhold_Quest02_Key", "$CollegeOfWinterhold_Quest02_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest02_Data", "MG03"},
		/*03*/ {"CollegeOfWinterhold_Quest03_Key", "$CollegeOfWinterhold_Quest03_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest03_Data", "MG04"},
		/*04*/ {"CollegeOfWinterhold_Quest04_Key", "$CollegeOfWinterhold_Quest04_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest04_Data", "mg06"},
		/*05*/ {"CollegeOfWinterhold_Quest05_Key", "$CollegeOfWinterhold_Quest05_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest05_Data", "MG05"},
		/*06*/ {"CollegeOfWinterhold_Quest06_Key", "$CollegeOfWinterhold_Quest06_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest06_Data", "MG07"},
		/*07*/ {"CollegeOfWinterhold_Quest07_Key", "$CollegeOfWinterhold_Quest07_Name", MAIN_QUEST_FLAG, "$CollegeOfWinterhold_Quest07_Data", "MG08"},
		/*08*/ {"CollegeOfWinterhold_Quest08_Key", "$CollegeOfWinterhold_Quest08_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest08_Data", "MGRitual05"},
		/*09*/ {"CollegeOfWinterhold_Quest09_Key", "$CollegeOfWinterhold_Quest09_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest09_Data", "MGRArniel01"},
		/*10*/ {"CollegeOfWinterhold_Quest10_Key", "$CollegeOfWinterhold_Quest10_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest10_Data", "MGRArniel02"},
		/*11*/ {"CollegeOfWinterhold_Quest11_Key", "$CollegeOfWinterhold_Quest11_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest11_Data", "MGRArniel03"},
		/*12*/ {"CollegeOfWinterhold_Quest12_Key", "$CollegeOfWinterhold_Quest12_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest12_Data", "MGRArniel04"},
		/*13*/ {"CollegeOfWinterhold_Quest13_Key", "$CollegeOfWinterhold_Quest13_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest13_Data", "MGRAppBrelyna01"},
		/*14*/ {"CollegeOfWinterhold_Quest14_Key", "$CollegeOfWinterhold_Quest14_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest14_Data", "MGRitual03"},
		/*15*/ {"CollegeOfWinterhold_Quest15_Key", "$CollegeOfWinterhold_Quest15_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest15_Data", "MGRitual01"},
		/*16*/ {"CollegeOfWinterhold_Quest16_Key", "$CollegeOfWinterhold_Quest16_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest16_Data", "dunMidden01QST"},
		/*17*/ {"CollegeOfWinterhold_Quest17_Key", "$CollegeOfWinterhold_Quest17_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest17_Data", "MGRitual02"},
		/*18*/ {"CollegeOfWinterhold_Quest18_Key", "$CollegeOfWinterhold_Quest18_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest18_Data", "MGRAppJzargo01"},
		/*19*/ {"CollegeOfWinterhold_Quest19_Key", "$CollegeOfWinterhold_Quest19_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest19_Data", "MGRAppOnmund01"},
		/*20*/ {"CollegeOfWinterhold_Quest20_Key", "$CollegeOfWinterhold_Quest20_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest20_Data", "MGRitual04"},
		/*21*/ {"CollegeOfWinterhold_Quest21_Key", "$CollegeOfWinterhold_Quest21_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest21_Data", "FreeformWinterholdCollegeB"},
		/*22*/ {"CollegeOfWinterhold_Quest22_Key", "$CollegeOfWinterhold_Quest22_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest22_Data", "MGR30"},
		/*23*/ {"CollegeOfWinterhold_Quest23_Key", "$CollegeOfWinterhold_Quest23_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest23_Data", "MGR10"},
		/*24*/ {"CollegeOfWinterhold_Quest24_Key", "$CollegeOfWinterhold_Quest24_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest24_Data", "MGR20"},
		/*25*/ {"CollegeOfWinterhold_Quest25_Key", "$CollegeOfWinterhold_Quest25_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest25_Data", "MGR12"},
		/*26*/ {"CollegeOfWinterhold_Quest26_Key", "$CollegeOfWinterhold_Quest26_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest26_Data", "MGR02"},
		/*27*/ {"CollegeOfWinterhold_Quest27_Key", "$CollegeOfWinterhold_Quest27_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest27_Data", "FreeformWinterholdCollegeA"},
		/*28*/ {"CollegeOfWinterhold_Quest28_Key", "$CollegeOfWinterhold_Quest28_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest28_Data", "MGR11"},
		/*29*/ {"CollegeOfWinterhold_Quest29_Key", "$CollegeOfWinterhold_Quest29_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest29_Data", "MGRRogue"},
		/*30*/ {"CollegeOfWinterhold_Quest30_Key", "$CollegeOfWinterhold_Quest30_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest30_Data", "MGR21"},
		/*31*/ {"CollegeOfWinterhold_Quest31_Key", "$CollegeOfWinterhold_Quest31_Name", SIDE_QUEST_FLAG, "$CollegeOfWinterhold_Quest31_Data", "MGR01"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{22, "Completionist_CollegeMGR30", RADIANT_COLLEGE_VALUE},
	{23, "Completionist_CollegeMGR10", RADIANT_COLLEGE_VALUE},
	{24, "Completionist_CollegeMGR20", RADIANT_COLLEGE_VALUE},
	{25, "Completionist_CollegeMGR12", RADIANT_COLLEGE_VALUE},
	{26, "Completionist_CollegeMGR02", RADIANT_COLLEGE_VALUE},
	{27, "Completionist_CollegeFreeformWinterholdCollegeA", RADIANT_COLLEGE_VALUE},
	{28, "Completionist_CollegeMGR11", RADIANT_COLLEGE_VALUE},
	{29, "Completionist_CollegeMGRRogue", RADIANT_COLLEGE_VALUE},
	{30, "Completionist_CollegeMGR21", RADIANT_COLLEGE_VALUE},
	{31, "Completionist_CollegeMGR01", RADIANT_COLLEGE_VALUE},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 16, 79 },
	};

	constexpr std::tuple<RE::FormID, RE::FormID, std::int32_t, std::int32_t, const char*> Radiant_QuestData[] = {
	{0x0266F1, 0x000000, 1, 200, "Completionist_CollegeMGR01"},
	{0x0266F2, 0x000000, 1, 200, "Completionist_CollegeMGR02"},
	{0x028A13, 0x000000, 1, 200, "Completionist_CollegeMGR10"},
	{0x028A19, 0x000000, 1, 200, "Completionist_CollegeMGR11"},
	{0x028A1E, 0x000000, 1, 200, "Completionist_CollegeMGR12"},
	{0x028A24, 0x0BC096, 1, 200, "Completionist_CollegeMGR20"},
	{0x0E0E04, 0x000000, 1, 200, "Completionist_CollegeMGR21"},
	{0x0C1E72, 0x000000, 1, 200, "Completionist_CollegeMGR30"},
	{0x05A609, 0x000000, 1, 200, "Completionist_CollegeMGRRouge"},
	{0x05D2EA, 0x000000, 1, 200, "Completionist_CollegeFreeformWinterholdCollegeA"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		SinkEvents();

		IdenArray.clear();
		NameArray.clear();
		RadiArray.clear();
		NameArray.clear();
		KeysArray.clear();

		for (auto& [Key, Name, Flag, Text, ID] : QuestData) {
			KeysArray.push_back(Key);
			NameArray.push_back(Name);
			RadiArray.push_back(Flag);
			TextArray.push_back(Text);
			IdenArray.push_back(ID);
		}

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
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
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		for (auto& [aformID, bformID, value, stage, global] : Radiant_QuestData) {
			const auto* aquest = RE::TESForm::LookupByID<RE::TESQuest>(aformID);
			const auto* bquest = RE::TESForm::LookupByID<RE::TESQuest>(bformID);

			if (((aquest && aquest == event) || (bquest && bquest == event)) && a_event->stage == stage) {
				if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)) {
					var->value++;
					INFO("Increasing Var For {}", aquest ? aquest->GetName() : bquest ? bquest->GetName() : "");
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateCompletion() {

		for (auto i : StandardCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_N(KeysArray[i], IdenArray[i]);
		}

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}