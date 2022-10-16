#include "Serialization.hpp"
#include "CFramework_BOO.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_BOO {
	using namespace CFramework_Master;

	// clang-format off

		/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"TBOO_Quest00_Key", "$TBOO_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest00_Data", "TBO01"},
		/*01*/ {"TBOO_Quest01_Key", "$TBOO_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest01_Data", "TBO02"},
		/*02*/ {"TBOO_Quest02_Key", "$TBOO_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest02_Data", "TBO03"},
		/*03*/ {"TBOO_Quest03_Key", "$TBOO_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest03_Data", "TBO04"},
		/*04*/ {"TBOO_Quest04_Key", "$TBOO_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest04_Data", "TBO05"},
		/*05*/ {"TBOO_Quest05_Key", "$TBOO_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest05_Data", "TBO05B"},
		/*06*/ {"TBOO_Quest06_Key", "$TBOO_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest06_Data", "TBO05Part2"},
		/*07*/ {"TBOO_Quest07_Key", "$TBOO_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest07_Data", "TBO06"},
		/*08*/ {"TBOO_Quest08_Key", "$TBOO_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest08_Data", "TBO07"},
		/*09*/ {"TBOO_Quest09_Key", "$TBOO_Quest09_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest09_Data", "TBO08"},
		/*10*/ {"TBOO_Quest10_Key", "$TBOO_Quest10_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest10_Data", "TBO09"},
		/*11*/ {"TBOO_Quest11_Key", "$TBOO_Quest11_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest11_Data", "TBO11"},
		/*12*/ {"TBOO_Quest12_Key", "$TBOO_Quest12_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest12_Data", "TBO10"},
		/*13*/ {"TBOO_Quest13_Key", "$TBOO_Quest13_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest13_Data", "TBO12"},
		/*14*/ {"TBOO_Quest14_Key", "$TBOO_Quest14_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest14_Data", "TBO13"},
		/*15*/ {"TBOO_Quest15_Key", "$TBOO_Quest15_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest15_Data", "TBO14"},
		/*16*/ {"TBOO_Quest16_Key", "$TBOO_Quest16_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest16_Data", "TBO15"},
		/*17*/ {"TBOO_Quest17_Key", "$TBOO_Quest17_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest17_Data", "TBOSQ1"},
		/*18*/ {"TBOO_Quest18_Key", "$TBOO_Quest18_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest18_Data", "TBOSQ2"},
		/*19*/ {"TBOO_Quest19_Key", "$TBOO_Quest19_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest19_Data", "TBOSQ3"},
		/*20*/ {"TBOO_Quest20_Key", "$TBOO_Quest20_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest20_Data", "TBOSQ4"},
		/*21*/ {"TBOO_Quest21_Key", "$TBOO_Quest21_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest21_Data", "TBOSQ5Dead"},
		/*22*/ {"TBOO_Quest22_Key", "$TBOO_Quest22_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest22_Data", "TBOSQ6"},
		/*23*/ {"TBOO_Quest23_Key", "$TBOO_Quest23_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest23_Data", "TBOSQ7"},
		/*24*/ {"TBOO_Quest24_Key", "$TBOO_Quest24_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$TBOO_Quest24_Data", "TBOSQ8"},
	};

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
	};

	constexpr Serialization::FormArray Items = {
	0x0D0417,0x01CAC9,0x0BCA45,0x215502,0x2A85F0,
	0x0BCA63,0x0BCA52,0x0937F5,0x08445F,
	};

	constexpr Serialization::FormArray Books = {
	0x02801E,0x003E91,
	};

	constexpr Serialization::FormArray MapMa = {
	0x055728,0x284E95,0x022949,0x00F600,0x03AFE6,0x00E51D,
	0x284E96,0x02790B,0x17D55F,0x015590,0x074E2C,0x013D2C,
	};
	// clang-format on

	constexpr std::string_view modname = "The Brotherhood of Old.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}
		
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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_BOO_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_BOO_Items::Data.GetBase(a_event->baseObj) ? CPatch_BOO_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_BOO_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_BOO_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_BOO_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_BOO_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_BOO_Books::Data.GetBase(target) ? CPatch_BOO_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_BOO_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_BOO_Books::Data.GetAllVariations()) {
				if (CPatch_BOO_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_BOO_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_BOO_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_BOO_Items::Data.GetAllVariations()) {
				if (CPatch_BOO_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_BOO_Items::Data.GetForm(a_baseID));
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

		CPatch_BOO_Items::Data.CompileFormArray(CPatch_BOO::Items, modname);
		CPatch_BOO_Books::Data.CompileFormArray(CPatch_BOO::Books, modname);
		CPatch_BOO_MapMa::Data.CompileFormArray(CPatch_BOO::MapMa, modname);
		
		CPatch_BOO_Items::Data.MergeAsCollectable();
		CPatch_BOO_Books::Data.MergeAsCollectable();

		CPatch_BOO_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_BOO_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_BOO_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			if (FoundItemData.HasForm(Items_FormArray[i]->GetFormID())) {
				Items_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			if (FoundItemData.HasForm(Books_FormArray[i]->GetFormID())) {
				Books_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID())) {
				MapMa_BoolArray[i] = true;
			}
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

		for (auto i : StandardCompletion) {
			Quest_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest_KeysArray[i], Quest_IdenArray[i]);
		};
	}
}