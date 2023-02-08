#include "CQuests_TelMithryn.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_TelMithryn {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"TelMithryn_Quest00_Key", "$TelMithryn_Quest00_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest00_Data", "DLC2TTR7"},
	/*01*/ {"TelMithryn_Quest01_Key", "$TelMithryn_Quest01_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest01_Data", "DLC2TTR5"},
	/*02*/ {"TelMithryn_Quest02_Key", "$TelMithryn_Quest02_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest02_Data", "DLC2TTR4a"},
	/*03*/ {"TelMithryn_Quest03_Key", "$TelMithryn_Quest03_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest03_Data", "DLC2TTR4b"},
	/*04*/ {"TelMithryn_Quest04_Key", "$TelMithryn_Quest04_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest04_Data", "DLC2TTF1"},
	/*05*/ {"TelMithryn_Quest05_Key", "$TelMithryn_Quest05_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest05_Data", "DLC2TTF2"},
	/*06*/ {"TelMithryn_Quest06_Key", "$TelMithryn_Quest06_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest06_Data", "DLC2TTR2"},
	/*07*/ {"TelMithryn_Quest07_Key", "$TelMithryn_Quest07_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest07_Data", "DLC2TTR8"},
	/*08*/ {"TelMithryn_Quest08_Key", "$TelMithryn_Quest08_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest08_Data", "DLC2TTR1"},
	/*09*/ {"TelMithryn_Quest09_Key", "$TelMithryn_Quest09_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest09_Data", "DLC2TT1b"},
	/*10*/ {"TelMithryn_Quest10_Key", "$TelMithryn_Quest10_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest10_Data", "DLC2TT2"},
	/*11*/ {"TelMithryn_Quest11_Key", "$TelMithryn_Quest11_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest11_Data", "DLC2TT1"},
	/*12*/ {"TelMithryn_Quest12_Key", "$TelMithryn_Quest12_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest12_Data", "DLC2TTR3a"},
	/*13*/ {"TelMithryn_Quest13_Key", "$TelMithryn_Quest13_Name", SIDE_QUEST_FLAG, "$TelMithryn_Quest13_Data", "DLC2TTR3b"},
	};

	constexpr std::size_t StandardCompletion[] = {
	1,2,3,4,5,6,8,9,10,11,12,13,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{0, "Completionist_TelMithryn_DLC2TTR7", 1},
	{7, "Completionist_TelMithryn_DLC2TTR8", 1},
	};

	constexpr std::tuple<RE::FormID, const char*, std::int32_t, std::int32_t, const char*> GlobalSetter[] = {
	/*00*/ {0x01BD18, "Dragonborn.esm", 1, 300, "Completionist_TelMithryn_DLC2TTR7"},
	/*07*/ {0x01F13B, "Dragonborn.esm", 1, 100, "Completionist_TelMithryn_DLC2TTR8"},
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

		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
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
	//-- Framework Events ( On Stage Change ) -----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		for (auto& [formID, fileName, value, stage, global] : GlobalSetter) {
			const auto* quest = static_cast<RE::TESQuest*>(RE::TESDataHandler::GetSingleton()->LookupForm(formID, fileName));

			if (event == quest && a_event->stage == stage) {
				if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)) {
					var->value += value;
					INFO("Incrememnting Var For {}", quest->GetName());
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
	};
}