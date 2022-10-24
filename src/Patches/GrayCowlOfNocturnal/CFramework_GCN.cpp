#include "Serialization.hpp"
#include "CFramework_GCN.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_GCN {
	using namespace CFramework_Master;

	// clang-format off

		/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"GrayCowl_Quest00_Key", "$GrayCowl_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest00_Data", "manny_GF_MQ"},
		/*01*/ {"GrayCowl_Quest01_Key", "$GrayCowl_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest01_Data", "manny_GF_ColdharbourQuest"},
		/*02*/ {"GrayCowl_Quest02_Key", "$GrayCowl_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest02_Data", "manny_GF_Farewell"},
		/*03*/ {"GrayCowl_Quest03_Key", "$GrayCowl_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest03_Data", "manny_GF_Books"},
		/*04*/ {"GrayCowl_Quest04_Key", "$GrayCowl_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest04_Data", "manny_GF_Sadraaka"},
		/*05*/ {"GrayCowl_Quest05_Key", "$GrayCowl_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest05_Data", "manny_GF_BuryValenDreth"},
		/*06*/ {"GrayCowl_Quest06_Key", "$GrayCowl_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_Y, "$GrayCowl_Quest06_Data", "manny_GF_MiscQuestDesertWolfPelts"},
		/*07*/ {"GrayCowl_Quest07_Key", "$GrayCowl_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest07_Data", "manny_GF_MiscQuestDuneripperBlood"},
		/*08*/ {"GrayCowl_Quest08_Key", "$GrayCowl_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest08_Data", "manny_GF_MiscQuestMetiliusAmulet"},
		/*09*/ {"GrayCowl_Quest09_Key", "$GrayCowl_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$GrayCowl_Quest09_Data", "manny_GF_MiscQuestPraetorianCores"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,7,8,9
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
	{ 6,  20  },
	};

	constexpr Serialization::FormArray Items = {
	0x034B20,0x004D9F,0x00AE03,0x00AE02,0x00AE01,0x018B4B,
	0x0190FA,0x03A217,0x02B7DC,0x017AE5,0x02298B,0x01FAEC,
	0x00334E,0x019BB2,0x014A6C,0x041911,
	};

	constexpr Serialization::FormArray Books = {
	0x0026F3,0x00315F,0x0026F8,0x03A7EA,0x0026FB,0x022E9C,
	0x0026FC,0x02298A,0x000EB4,0x03BA96,0x015986,0x010202,
	};

	constexpr Serialization::FormArray MapMa = {
	0x015987,0x02861F,0x009750,0x042FED,0x00FA56,0x0101ED,
	0x01D939,0x001F27,0x024C4C,0x028620,0x02B1BC,0x0299BA,
	0x00C993,0x00FA59,0x028CA1,0x028678,0x029283,0x00FA58,
	0x001A9B,0x0280A7,
	};
	// clang-format on

	constexpr std::string_view modname = "Gray Fox Cowl.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework() {

		Quest_IdenArray.clear();
		Quest_NameArray.clear();
		Quest_RadiArray.clear();
		Quest_NameArray.clear();
		Quest_KeysArray.clear();
		Quest_StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text, id] : QuestData) {
			Quest_KeysArray.push_back(key);
			Quest_NameArray.push_back(name);
			Quest_RadiArray.push_back(flag);
			Quest_TextArray.push_back(text);
			Quest_IdenArray.push_back(id);
			Quest_StgeArray.push_back(isStageDone);
		}

		assert(Quest_KeysArray.size() == ArraySize);
		assert(Quest_IdenArray.size() == ArraySize);
		assert(Quest_NameArray.size() == ArraySize);
		assert(Quest_RadiArray.size() == ArraySize);
		assert(Quest_TextArray.size() == ArraySize);
		assert(Quest_StgeArray.size() == ArraySize);
		Quest_BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(Quest_IdenArray, quest->GetFormEditorID());
		if (t_pos == Quest_IdenArray.end()) { return EventResult::kContinue; }


		if (Quest_StgeArray.at(std::distance(Quest_IdenArray.begin(), t_pos))) {
			CQuestKeys_Stages.AddStage(Quest_KeysArray.at(std::distance(Quest_IdenArray.begin(), t_pos)), a_event->stage);
			INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest_IdenArray.at(std::distance(Quest_IdenArray.begin(), t_pos)));
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_GCN_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_GCN_Items::Data.GetBase(a_event->baseObj) ? CPatch_GCN_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_GCN_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_GCN_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_GCN_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_GCN_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_GCN_Books::Data.GetBase(target) ? CPatch_GCN_Books::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "NotifyBooks");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i);
			}
		}

		if (a_event->menuName == RE::JournalMenu::MENU_NAME) {
			CHandler::UpdateQuestFramework();
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_GCN_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_GCN_Books::Data.GetAllVariations()) {
				if (CPatch_GCN_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_GCN_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_GCN_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_GCN_Items::Data.GetAllVariations()) {
				if (CPatch_GCN_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_GCN_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_marker);
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_GCN_Items::Data.CompileFormArray(CPatch_GCN::Items, modname);
		CPatch_GCN_Books::Data.CompileFormArray(CPatch_GCN::Books, modname);
		CPatch_GCN_MapMa::Data.CompileFormArray(CPatch_GCN::MapMa, modname);

		CPatch_GCN_Items::Data.MergeAsCollectable();
		CPatch_GCN_Books::Data.MergeAsCollectable();

		CPatch_GCN_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_GCN_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_GCN_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_GCN_Items::Data);
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateQuestFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i : StandardCompletion) {
			Quest_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest_KeysArray[i], Quest_IdenArray[i]);
		};

		for (auto& [i, stage] : StageCompletion) {
			Quest_BoolArray[i] = Quest_StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(Quest_KeysArray[i]) || FrameworkAPI::IsCompleted_S(Quest_KeysArray[i], Quest_IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(Quest_KeysArray[i]) || FrameworkAPI::IsCompleted_P(Quest_KeysArray[i], Quest_IdenArray[i], stage);
		}
	}
}