#include "CQuests_ThievesGuild.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_ThievesGuild {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"ThievesGuild_Quest00_Key", "$ThievesGuild_Quest00_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest00_Data", "TG00"},
	/*01*/ {"ThievesGuild_Quest01_Key", "$ThievesGuild_Quest01_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest01_Data", "TG01"},
	/*02*/ {"ThievesGuild_Quest02_Key", "$ThievesGuild_Quest02_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest02_Data", "TG02"},
	/*03*/ {"ThievesGuild_Quest03_Key", "$ThievesGuild_Quest03_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest03_Data", "TG03"},
	/*04*/ {"ThievesGuild_Quest04_Key", "$ThievesGuild_Quest04_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest04_Data", "TG04"},
	/*05*/ {"ThievesGuild_Quest05_Key", "$ThievesGuild_Quest05_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest05_Data", "TG05"},
	/*06*/ {"ThievesGuild_Quest06_Key", "$ThievesGuild_Quest06_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest06_Data", "TG06"},
	/*07*/ {"ThievesGuild_Quest07_Key", "$ThievesGuild_Quest07_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest07_Data", "TG07"},
	/*08*/ {"ThievesGuild_Quest08_Key", "$ThievesGuild_Quest08_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest08_Data", "TG08A"},
	/*09*/ {"ThievesGuild_Quest09_Key", "$ThievesGuild_Quest09_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest09_Data", "TG08B"},
	/*10*/ {"ThievesGuild_Quest10_Key", "$ThievesGuild_Quest10_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest10_Data", "TG09"},
	/*11*/ {"ThievesGuild_Quest11_Key", "$ThievesGuild_Quest11_Name", MAIN_QUEST_FLAG, "$ThievesGuild_Quest11_Data", "TGLeadership"},
	/*12*/ {"ThievesGuild_Quest12_Key", "$ThievesGuild_Quest12_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest12_Data", "TG02B"},
	/*13*/ {"ThievesGuild_Quest13_Key", "$ThievesGuild_Quest13_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest13_Data", "TGCrown"},
	/*14*/ {"ThievesGuild_Quest14_Key", "$ThievesGuild_Quest14_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest14_Data", "TGCrownMisc"},
	/*15*/ {"ThievesGuild_Quest15_Key", "$ThievesGuild_Quest15_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest15_Data", "TGLarceny"},
	/*16*/ {"ThievesGuild_Quest16_Key", "$ThievesGuild_Quest16_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest16_Data", "TGFenceCaravan"},
	/*17*/ {"ThievesGuild_Quest17_Key", "$ThievesGuild_Quest17_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest17_Data", "FreeformRiften21"},
	/*18*/ {"ThievesGuild_Quest18_Key", "$ThievesGuild_Quest18_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest18_Data", "TGTQ03"},
	/*19*/ {"ThievesGuild_Quest19_Key", "$ThievesGuild_Quest19_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest19_Data", "TGTQ01"},
	/*20*/ {"ThievesGuild_Quest20_Key", "$ThievesGuild_Quest20_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest20_Data", "TGTQ04"},
	/*21*/ {"ThievesGuild_Quest21_Key", "$ThievesGuild_Quest21_Name", SIDE_QUEST_FLAG, "$ThievesGuild_Quest21_Data", "TGTQ02"},
	/*22*/ {"ThievesGuild_Quest22_Key", "$ThievesGuild_Quest22_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest22_Data", "TGRNT"},
	/*23*/ {"ThievesGuild_Quest23_Key", "$ThievesGuild_Quest23_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest23_Data", "TGRBE"},
	/*24*/ {"ThievesGuild_Quest24_Key", "$ThievesGuild_Quest24_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest24_Data", "TGRGF"},
	/*25*/ {"ThievesGuild_Quest25_Key", "$ThievesGuild_Quest25_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest25_Data", "TGRSL"},
	/*26*/ {"ThievesGuild_Quest26_Key", "$ThievesGuild_Quest26_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest26_Data", "TGRFO"},
	/*27*/ {"ThievesGuild_Quest27_Key", "$ThievesGuild_Quest27_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest27_Data", "TGRDU"},
	/*28*/ {"ThievesGuild_Quest28_Key", "$ThievesGuild_Quest28_Name", RADI_QUEST_FLAG, "$ThievesGuild_Quest28_Data", "TGRHC"},
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<RE::FormID, std::int32_t, std::int32_t, const char*> Radiant_QuestData[] = {
		/*00*/ {0x060990, 1, 200, "Completionist_TGR_Bedlam"},
		/*01*/ {0x02893B, 1, 200, "Completionist_TGR_Burglary"},
		/*02*/ {0x028922, 1, 200, "Completionist_TGR_Fishing"},
		/*03*/ {0x02893E, 1, 200, "Completionist_TGR_Heist"},
		/*04*/ {0x06098E, 1, 200, "Completionist_TGR_Numbers"},
		/*05*/ {0x015D24, 1, 200, "Completionist_TGR_Shill"},
		/*06*/ {0x028936, 1, 200, "Completionist_TGR_Sweep"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,17,18,19,20,21
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{22, "Completionist_TGR_Bedlam", RADIANT_THIEVESGUILD_VALUE},
	{23, "Completionist_TGR_Burglary", RADIANT_THIEVESGUILD_VALUE},
	{24, "Completionist_TGR_Fishing", RADIANT_THIEVESGUILD_VALUE},
	{25, "Completionist_TGR_Heist", RADIANT_THIEVESGUILD_VALUE},
	{26, "Completionist_TGR_Numbers", RADIANT_THIEVESGUILD_VALUE},
	{27, "Completionist_TGR_Shill", RADIANT_THIEVESGUILD_VALUE},
	{28, "Completionist_TGR_Sweep", RADIANT_THIEVESGUILD_VALUE},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 15, 199 },
	{ 16, 199 },
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

		for (auto& [formID, value, stage, global] : Radiant_QuestData) {
			const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(formID);

			if (event == quest && a_event->stage == stage) {
				if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)) {
					var->value += value;
					INFO("Increasing Var For {}", quest->GetName());
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