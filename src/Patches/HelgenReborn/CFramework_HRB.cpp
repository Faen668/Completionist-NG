#include "Serialization.hpp"
#include "CFramework_HRB.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_HRB {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"HelgenReborn_Quest00_Key", "$HelgenReborn_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest00_Data", "BalokCourierTimer"},
		/*01*/ {"HelgenReborn_Quest01_Key", "$HelgenReborn_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest01_Data", "BalokHelgen01"},
		/*02*/ {"HelgenReborn_Quest02_Key", "$HelgenReborn_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest02_Data", "BalokThalmorPrison"},
		/*03*/ {"HelgenReborn_Quest03_Key", "$HelgenReborn_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest03_Data", "BalokConstructionQuest"},
		/*04*/ {"HelgenReborn_Quest04_Key", "$HelgenReborn_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest04_Data", "BalokBitterWounds"},
		/*05*/ {"HelgenReborn_Quest05_Key", "$HelgenReborn_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest05_Data", "BalokCiennaQuest"},
		/*06*/ {"HelgenReborn_Quest06_Key", "$HelgenReborn_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest06_Data", "BalokHamingQuest"},
		/*07*/ {"HelgenReborn_Quest07_Key", "$HelgenReborn_Quest07_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest07_Data", "BalokRadiant01"},
		/*08*/ {"HelgenReborn_Quest08_Key", "$HelgenReborn_Quest08_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest08_Data", "BalokRadiant02"},
		/*09*/ {"HelgenReborn_Quest09_Key", "$HelgenReborn_Quest09_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest09_Data", "BalokRadiant03"},
		/*10*/ {"HelgenReborn_Quest10_Key", "$HelgenReborn_Quest10_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest10_Data", "BalokRadiant04"},
		/*11*/ {"HelgenReborn_Quest11_Key", "$HelgenReborn_Quest11_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest11_Data", "BalokRadiant05"},
		/*12*/ {"HelgenReborn_Quest12_Key", "$HelgenReborn_Quest12_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest12_Data", "BalokRadiant06"},
		/*13*/ {"HelgenReborn_Quest13_Key", "$HelgenReborn_Quest13_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest13_Data", "BalokRadiant01Stormcloak"},
		/*14*/ {"HelgenReborn_Quest14_Key", "$HelgenReborn_Quest14_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest14_Data", "BalokRadiant02Stormcloak"},
		/*15*/ {"HelgenReborn_Quest15_Key", "$HelgenReborn_Quest15_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest15_Data", "BalokRadiant03Stormcloak"},
		/*16*/ {"HelgenReborn_Quest16_Key", "$HelgenReborn_Quest16_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest16_Data", "BalokRadiant04Stormcloak"},
		/*17*/ {"HelgenReborn_Quest17_Key", "$HelgenReborn_Quest17_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest17_Data", "BalokRadiant05Stormcloak"},
		/*18*/ {"HelgenReborn_Quest18_Key", "$HelgenReborn_Quest18_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest18_Data", "BalokRadiant06Stormcloak"},
		/*19*/ {"HelgenReborn_Quest19_Key", "$HelgenReborn_Quest19_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest19_Data", "BalokRadiant01Empire"},
		/*20*/ {"HelgenReborn_Quest20_Key", "$HelgenReborn_Quest20_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest20_Data", "BalokRadiant02Empire"},
		/*21*/ {"HelgenReborn_Quest21_Key", "$HelgenReborn_Quest21_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest21_Data", "BalokRadiant03Empire"},
		/*22*/ {"HelgenReborn_Quest22_Key", "$HelgenReborn_Quest22_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest22_Data", "BalokRadiant04Empire"},
		/*23*/ {"HelgenReborn_Quest23_Key", "$HelgenReborn_Quest23_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest23_Data", "BalokRadiant05Empire"},
		/*24*/ {"HelgenReborn_Quest24_Key", "$HelgenReborn_Quest24_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$HelgenReborn_Quest24_Data", "BalokRadiant06Empire"},
	};

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
	};

	constexpr std::tuple<std::size_t, std::int32_t> StageCompletion[] = {
		{ 6,  30  },
	};

	constexpr Serialization::FormArray Items = {
	0x111E89,0x0022F2,0x130577,0x0B79CD,
	};

	constexpr Serialization::FormArray Books = {
	0x047FB6,0x04184C,0x09F94D,
	};

	constexpr Serialization::FormArray MapMa = {
	0x04FB56,0x128D5B,0x128D57,0x0317BC,0x011C92,
	0x11DA6D,0x119E30,
	};
	// clang-format on

	constexpr std::string_view modname = "Helgen Reborn.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_HRB_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_HRB_Items::Data.GetBase(a_event->baseObj) ? CPatch_HRB_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_HRB_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_HRB_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_HRB_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_HRB_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_HRB_Books::Data.GetBase(target) ? CPatch_HRB_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_HRB_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_HRB_Books::Data.GetAllVariations()) {
				if (CPatch_HRB_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_HRB_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_HRB_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_HRB_Items::Data.GetAllVariations()) {
				if (CPatch_HRB_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_HRB_Items::Data.GetForm(a_baseID));
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

		CPatch_HRB_Items::Data.CompileFormArray(CPatch_HRB::Items, modname);
		CPatch_HRB_Books::Data.CompileFormArray(CPatch_HRB::Books, modname);
		CPatch_HRB_MapMa::Data.CompileFormArray(CPatch_HRB::MapMa, modname);

		CPatch_HRB_Items::Data.MergeAsCollectable();
		CPatch_HRB_Books::Data.MergeAsCollectable();

		CPatch_HRB_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_HRB_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_HRB_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_HRB_Items::Data);
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