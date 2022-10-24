#include "Serialization.hpp"
#include "CFramework_CLW.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_CLW {
	using namespace Serialization;
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"Clockwork_Quest00_Key", "$Clockwork_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest00_Data", "CLWKicker01Quest"},
		/*01*/ {"Clockwork_Quest01_Key", "$Clockwork_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest01_Data", "CLWStory01Quest"},
		/*02*/ {"Clockwork_Quest02_Key", "$Clockwork_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest02_Data", "CLWStory02Quest"},
		/*03*/ {"Clockwork_Quest03_Key", "$Clockwork_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest03_Data", "CLWStory03Quest"},
		/*04*/ {"Clockwork_Quest04_Key", "$Clockwork_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest04_Data", "CLWStory04Quest"},
		/*05*/ {"Clockwork_Quest05_Key", "$Clockwork_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest05_Data", "CLWSide01Quest"},
		/*06*/ {"Clockwork_Quest06_Key", "$Clockwork_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Clockwork_Quest06_Data", "CLWSide02Quest"},
	};

	constexpr Serialization::FormArray Items = {
	0x50BCDD,0x79768C,0x50BCDB,0x792579,
	};

	constexpr Serialization::FormArray Books = {
	0x065F4A,0x332EA2,0x066A37,0x332EA7,0x070265,
	0x332EAA,0x070266,0x332EAF,
	};

	constexpr Serialization::FormArray MapMa = {
	0x05FC3A,
	};
	// clang-format on

	constexpr std::string_view modname = "Clockwork.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_CLW_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_CLW_Items::Data.GetBase(a_event->baseObj) ? CPatch_CLW_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_CLW_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_CLW_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_CLW_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && CPatch_CLW_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_CLW_Books::Data.GetBase(target) ? CPatch_CLW_Books::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "NotifyBooks");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME) {

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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_CLW_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CLW_Books::Data.GetAllVariations()) {
				if (CPatch_CLW_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_CLW_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_CLW_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CLW_Items::Data.GetAllVariations()) {
				if (CPatch_CLW_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_CLW_Items::Data.GetForm(a_baseID));
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

		CPatch_CLW_Items::Data.CompileFormArray(CPatch_CLW::Items, modname);
		CPatch_CLW_Books::Data.CompileFormArray(CPatch_CLW::Books, modname);
		CPatch_CLW_MapMa::Data.CompileFormArray(CPatch_CLW::MapMa, modname);

		CPatch_CLW_Items::Data.MergeAsCollectable();
		CPatch_CLW_Books::Data.MergeAsCollectable();

		CPatch_CLW_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_CLW_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_CLW_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_CLW_Items::Data);
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

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++) {
			Quest_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest_KeysArray[i], Quest_IdenArray[i]);
		}
	};
}