#include "Serialization.hpp"
#include "CFramework_UND.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_UND {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"Undeath_Quest00_Key", "$Undeath_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest00_Data", "NecroQuest01"},
		/*01*/ {"Undeath_Quest01_Key", "$Undeath_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest01_Data", "NecroQuest02"},
		/*02*/ {"Undeath_Quest02_Key", "$Undeath_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest02_Data", "NecroQuest03"},
		/*03*/ {"Undeath_Quest03_Key", "$Undeath_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest03_Data", "NecroQuest04"},
		/*04*/ {"Undeath_Quest04_Key", "$Undeath_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest04_Data", "NecroQuest05"},
		/*05*/ {"Undeath_Quest05_Key", "$Undeath_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest05_Data", "NecroBlackBookQuest01"},
		/*06*/ {"Undeath_Quest06_Key", "$Undeath_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Undeath_Quest06_Data", "NecroLichRitualQuest"},
	};

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,6
	};

	constexpr Serialization::FormArray Books = {
	0x26720A,0x26721D,0x1B68D5,0x002741,0x1C5C99,0x002729,
	0x00272A,0x00272B,0x002CDD,0x010264,0x0509A2,0x04435D,
	0x0492BB,0x00F10A,0x0509A3,0x04BA6D,0x0F73C4,0x04BA6C,
	0x05839E,
	};

	constexpr Serialization::FormArray MapMa = {
	0x173276,0x0C7DE2,0x1887B0,0x053418,0x285990,0x0EAC41,
	};

	// clang-format on

	constexpr std::string_view modname = "Undeath.esp";

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
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_UND_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_UND_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_UND_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_UND_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_UND_Books::Data.GetBase(target) ? CPatch_UND_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_UND_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_UND_Books::Data.GetAllVariations()) {
				if (CPatch_UND_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_UND_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
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

		CPatch_UND_Books::Data.CompileFormArray(CPatch_UND::Books, modname);
		CPatch_UND_MapMa::Data.CompileFormArray(CPatch_UND::MapMa, modname);

		CPatch_UND_Books::Data.MergeAsCollectable();

		CPatch_UND_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_UND_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

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
		}
	}
}