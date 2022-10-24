#include "CQuests_Windhelm.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Windhelm {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/ {"Windhelm_Quest00_Key", "$Windhelm_Quest00_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest00_Data", "MS11"},
		/*01*/ {"Windhelm_Quest01_Key", "$Windhelm_Quest01_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest01_Data", "FreeformWindhelmA"},
		/*02*/ {"Windhelm_Quest02_Key", "$Windhelm_Quest02_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest02_Data", "MQ201Malborn"},
		/*03*/ {"Windhelm_Quest03_Key", "$Windhelm_Quest03_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest03_Data", "MS12b"},
		/*04*/ {"Windhelm_Quest04_Key", "$Windhelm_Quest04_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest04_Data", "MS10"},
		/*05*/ {"Windhelm_Quest05_Key", "$Windhelm_Quest05_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest05_Data", "Favor019"},
		/*06*/ {"Windhelm_Quest06_Key", "$Windhelm_Quest06_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest06_Data", "MS12"},
		/*07*/ {"Windhelm_Quest07_Key", "$Windhelm_Quest07_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest07_Data", "Favor013"},
		/*08*/ {"Windhelm_Quest08_Key", "$Windhelm_Quest08_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest08_Data", "Favor018"},
		/*09*/ {"Windhelm_Quest09_Key", "$Windhelm_Quest09_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest09_Data", "Favor110"},
		/*10*/ {"Windhelm_Quest10_Key", "$Windhelm_Quest10_Name", RADI_QUEST_FLAG, "$Windhelm_Quest10_Data", "FavorJobsDrunks"},
		/*11*/ {"Windhelm_Quest11_Key", "$Windhelm_Quest11_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest11_Data", "Favor104"},
		/*12*/ {"Windhelm_Quest12_Key", "$Windhelm_Quest12_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest12_Data", "FreeformShipsWindhelm"},
		/*13*/ {"Windhelm_Quest13_Key", "$Windhelm_Quest13_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest13_Data", "FavorJarlsMakeFriends"},
		/*14*/ {"Windhelm_Quest14_Key", "$Windhelm_Quest14_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest14_Data", "Favor017"},
		/*15*/ {"Windhelm_Quest15_Key", "$Windhelm_Quest15_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest15_Data", "Favor205"},
		/*16*/ {"Windhelm_Quest16_Key", "$Windhelm_Quest16_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest16_Data", "Favor204"},
		/*17*/ {"Windhelm_Quest17_Key", "$Windhelm_Quest17_Name", SIDE_QUEST_FLAG, "$Windhelm_Quest17_Data", "Favor001"},
		/*18*/ {"Windhelm_Quest18_Key", "$Windhelm_Quest18_Name", RADI_QUEST_FLAG, "$Windhelm_Quest18_Data", "FavorJobsBeggars"},
		/*19*/ {"Windhelm_Quest19_Key", "$Windhelm_Quest19_Name", RADI_QUEST_FLAG, "$Windhelm_Quest19_Data", "FavorJobsBeggars"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,3,4,6,
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{1,  199},
	{2,  140},
	{12,  199},
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{5, "Completionist_Favor019Revyn", 1},
	{7, "Completionist_Favor013ScoutsManyMarshes", 1},
	{8, "Completionist_Favor018StandInShallows", 1},
	{9, "Completionist_Favor110Torbjorn", 1},
	{10, "Completionist_FavorDrunksTorbjorn", -4},
	{11, "Completionist_Favor104Brunwulf", 1},
	{14, "Completionist_Favor017Rolff", 1},
	{15, "Completionist_Favor205Oengul", 1},
	{16, "Completionist_Favor204Shahvee", 1},
	{17, "Completionist_Favor001Adonato", 1},
	{18, "Completionist_FavorBeggarAngrenor", -4},
	{19, "Completionist_FavorBeggarSilda", -4},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{13, "EastmarchImpGetOutofJail", "EastmarchSonsGetOutofJail"},
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

		auto* torbjorn = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x001B12B, "Skyrim.esm"));
		if (!torbjorn) { return EventResult::kContinue; }

		if (speaker == torbjorn && a_event->newContainer == torbjorn->GetFormID()) {
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksTorbjorn")) {
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}