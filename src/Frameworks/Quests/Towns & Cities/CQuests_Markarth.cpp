#include "CQuests_Markarth.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Markarth {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/{"Markarth_Quest00_Key", "$Markarth_Quest00_Name", SIDE_QUEST_FLAG, "$Markarth_Quest00_Data", "FreeformMarkarthJ"},
	/*01*/{"Markarth_Quest01_Key", "$Markarth_Quest01_Name", SIDE_QUEST_FLAG, "$Markarth_Quest01_Data", "WIAddItem07"},
	/*02*/{"Markarth_Quest02_Key", "$Markarth_Quest02_Name", SIDE_QUEST_FLAG, "$Markarth_Quest02_Data", "FreeformMarkarthN"},
	/*03*/{"Markarth_Quest03_Key", "$Markarth_Quest03_Name", SIDE_QUEST_FLAG, "$Markarth_Quest03_Data", "FreeformMarkarthD"},
	/*04*/{"Markarth_Quest04_Key", "$Markarth_Quest04_Name", SIDE_QUEST_FLAG, "$Markarth_Quest04_Data", "Favor151"},
	/*05*/{"Markarth_Quest05_Key", "$Markarth_Quest05_Name", SIDE_QUEST_FLAG, "$Markarth_Quest05_Data", "FreeformMarkarthE"},
	/*06*/{"Markarth_Quest06_Key", "$Markarth_Quest06_Name", SIDE_QUEST_FLAG, "$Markarth_Quest06_Data", "FreeformMarkarthO"},
	/*07*/{"Markarth_Quest07_Key", "$Markarth_Quest07_Name", SIDE_QUEST_FLAG, "$Markarth_Quest07_Data", "MS02"},
	/*08*/{"Markarth_Quest08_Key", "$Markarth_Quest08_Name", SIDE_QUEST_FLAG, "$Markarth_Quest08_Data", "FreeformMarkarthM"},
	/*09*/{"Markarth_Quest09_Key", "$Markarth_Quest09_Name", SIDE_QUEST_FLAG, "$Markarth_Quest09_Data", "FreeformMarkarthL"},
	/*10*/{"Markarth_Quest10_Key", "$Markarth_Quest10_Name", SIDE_QUEST_FLAG, "$Markarth_Quest10_Data", "MS01"},
	/*11*/{"Markarth_Quest11_Key", "$Markarth_Quest11_Name", SIDE_QUEST_FLAG, "$Markarth_Quest11_Data", "T01DegaineFavor"},
	/*12*/{"Markarth_Quest12_Key", "$Markarth_Quest12_Name", SIDE_QUEST_FLAG, "$Markarth_Quest12_Data", "T01"},
	/*13*/{"Markarth_Quest13_Key", "$Markarth_Quest13_Name", SIDE_QUEST_FLAG, "$Markarth_Quest13_Data", "DA10"},
	/*14*/{"Markarth_Quest14_Key", "$Markarth_Quest14_Name", SIDE_QUEST_FLAG, "$Markarth_Quest14_Data", "dunNchuandZelQst"},
	/*15*/{"Markarth_Quest15_Key", "$Markarth_Quest15_Name", SIDE_QUEST_FLAG, "$Markarth_Quest15_Data", "DA11Intro"},
	/*16*/{"Markarth_Quest16_Key", "$Markarth_Quest16_Name", SIDE_QUEST_FLAG, "$Markarth_Quest16_Data", "DA11"},
	/*17*/{"Markarth_Quest17_Key", "$Markarth_Quest17_Name", SIDE_QUEST_FLAG, "$Markarth_Quest17_Data", "Favor013"},
	/*18*/{"Markarth_Quest18_Key", "$Markarth_Quest18_Name", SIDE_QUEST_FLAG, "$Markarth_Quest18_Data", "Favor001"},
	/*19*/{"Markarth_Quest19_Key", "$Markarth_Quest19_Name", SIDE_QUEST_FLAG, "$Markarth_Quest19_Data", "Favor157"},
	/*20*/{"Markarth_Quest20_Key", "$Markarth_Quest20_Name", SIDE_QUEST_FLAG, "$Markarth_Quest20_Data", "Favor017"},
	/*21*/{"Markarth_Quest21_Key", "$Markarth_Quest21_Name", RADI_QUEST_FLAG, "$Markarth_Quest21_Data", "FavorJobsBeggars"},
	/*22*/{"Markarth_Quest22_Key", "$Markarth_Quest22_Name", RADI_QUEST_FLAG, "$Markarth_Quest22_Data", "FavorJobsDrunks"},
	/*23*/{"Markarth_Quest23_Key", "$Markarth_Quest23_Name", RADI_QUEST_FLAG, "$Markarth_Quest23_Data", "FavorJobsDrunks"},
	/*24*/{"Markarth_Quest24_Key", "$Markarth_Quest24_Name", SIDE_QUEST_FLAG, "$Markarth_Quest24_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::size_t StandardCompletion[] = { 
	7,10,12,13,14,15,16 
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{0,  10},
	{1,  10},
	{2,  10},
	{3,  10},
	{5,  15},
	{6,  15},
	{8,  15},
	{9,  10},
	{11,  20},
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{4, "Completionist_Favor151Igmund", 1},
	{17, "Completionist_Favor013Omluag", 1},
	{18, "Completionist_Favor001Banning", 1},
	{19, "Completionist_Favor157Igmund", 1},
	{20, "Completionist_Favor017Cosnach", 1},
	{21, "Completionist_FavorBeggarDegaine", -4},
	{22, "Completionist_FavorDrunksDegaine", -4},
	{23, "Completionist_FavorDrunksCosnach", -4},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{24, "ReachImpGetOutofJail", "ReachSonsGetOutofJail"},
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

		auto* degaine = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x001F325, "Skyrim.esm"));
		auto* cosnach = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00198FA, "Skyrim.esm"));
		if (!degaine || !cosnach) { return EventResult::kContinue; }

		if (speaker == degaine && a_event->newContainer == degaine->GetFormID()) {
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksDegaine")) {
				var->value += 1;
				return EventResult::kContinue;
			}
		}

		if (speaker == cosnach && a_event->newContainer == cosnach->GetFormID()) {
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksCosnach")) {
				var->value += 1;
				return EventResult::kContinue;
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

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
} 