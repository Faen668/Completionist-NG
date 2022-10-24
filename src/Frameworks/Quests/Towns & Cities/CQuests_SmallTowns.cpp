#include "CQuests_SmallTowns.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_SmallTowns {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"SmallTowns_Quest00_Key", "$SmallTowns_Quest00_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest00_Data", "FreeformRiverwood01"},
	/*01*/ {"SmallTowns_Quest01_Key", "$SmallTowns_Quest01_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest01_Data", "FreeformIvarstead04"},
	/*02*/ {"SmallTowns_Quest02_Key", "$SmallTowns_Quest02_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest02_Data", "FreeformDragonBridge01"},
	/*03*/ {"SmallTowns_Quest03_Key", "$SmallTowns_Quest03_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest03_Data", "RoriksteadFreeform"},
	/*04*/ {"SmallTowns_Quest04_Key", "$SmallTowns_Quest04_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest04_Data", "FreeformIvarstead03"},
	/*05*/ {"SmallTowns_Quest05_Key", "$SmallTowns_Quest05_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest05_Data", "FreeformIvarstead01"},
	/*06*/ {"SmallTowns_Quest06_Key", "$SmallTowns_Quest06_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest06_Data", "FreeformShorsStone01"},
	/*07*/ {"SmallTowns_Quest07_Key", "$SmallTowns_Quest07_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest07_Data", "FreeformShorsStone02"},
	/*08*/ {"SmallTowns_Quest08_Key", "$SmallTowns_Quest08_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest08_Data", "FreeformIvarstead02"},
	/*09*/ {"SmallTowns_Quest09_Key", "$SmallTowns_Quest09_Name", SIDE_QUEST_FLAG, "$SmallTowns_Quest09_Data", "MS13"},
	/*10*/ {"SmallTowns_Quest10_Key", "$SmallTowns_Quest10_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest10_Data", "FavorJobsChopWood"},
	/*11*/ {"SmallTowns_Quest11_Key", "$SmallTowns_Quest11_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest11_Data", "FavorJobsChopWood"},
	/*12*/ {"SmallTowns_Quest12_Key", "$SmallTowns_Quest12_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest12_Data", "FavorJobsChopWood"},
	/*13*/ {"SmallTowns_Quest13_Key", "$SmallTowns_Quest13_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest13_Data", "FavorJobsGatherWheat"},
	/*14*/ {"SmallTowns_Quest14_Key", "$SmallTowns_Quest14_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest14_Data", "FavorJobsGatherWheat"},
	/*15*/ {"SmallTowns_Quest15_Key", "$SmallTowns_Quest15_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest15_Data", "FavorJobsGatherWheat"},
	/*16*/ {"SmallTowns_Quest16_Key", "$SmallTowns_Quest16_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest16_Data", "FavorJobsGatherWheat"},
	/*17*/ {"SmallTowns_Quest17_Key", "$SmallTowns_Quest17_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest17_Data", "FavorJobsDrunks"},
	/*18*/ {"SmallTowns_Quest18_Key", "$SmallTowns_Quest18_Name", RADI_QUEST_FLAG, "$SmallTowns_Quest18_Data", "FavorJobsBeggars"},
	};

	constexpr std::size_t StandardCompletion[] = {
	9,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{10, "Completionist_Favor_ChopWoodHod", -4 },
	{11, "Completionist_Favor_ChopWoodHorgeir", -4},
	{12, "Completionist_Favor_ChopWoodTembaWideArm", -4},
	{13, "Completionist_FavorHarvestAzzada", -4},
	{14, "Completionist_FavorHarvestBoti", -4},
	{15, "Completionist_FavorHarvestLemkil", -4},
	{16, "Completionist_FavorHarvestReldith", -4},
	{17, "Completionist_FavorDrunksEmbry", -4},
	{18, "Completionist_FavorBeggarNarfi", -4},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 309 },
	{ 1, 199 },
	{ 2, 19 },
	{ 3, 20 },
	{ 4, 199 },
	{ 5, 199 },
	{ 6, 199 },
	{ 7, 199 },
	{ 8, 199 },
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

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
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
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || !a_event->baseObj || a_event->oldContainer != RE::PlayerCharacter::GetSingleton()->GetFormID()) { return EventResult::kContinue; }

		auto* List = static_cast<RE::BGSListForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x0072EA2, "Skyrim.esm"));
		if (!List || !List->HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto speaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();
		if (!speaker || speaker->GetFormID() != a_event->newContainer) { return EventResult::kContinue; }

		auto* embry = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x003550C, "Skyrim.esm"));
		if (!embry) { return EventResult::kContinue; }

		if (speaker == embry && a_event->newContainer == embry->GetFormID()) {
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksEmbry")) {
				var->value += 1;
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