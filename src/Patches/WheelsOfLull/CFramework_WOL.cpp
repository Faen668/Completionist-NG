#include "Serialization.hpp"
#include "CFramework_WOL.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_WOL {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"WheelsOfLull_Quest00_Key", "$WheelsOfLull_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest00_Data", "_Lull_MQ01"},
		/*01*/ {"WheelsOfLull_Quest01_Key", "$WheelsOfLull_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest01_Data", "_Lull_MQ02"},
		/*02*/ {"WheelsOfLull_Quest02_Key", "$WheelsOfLull_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest02_Data", "_Lull_MQ03"},
		/*03*/ {"WheelsOfLull_Quest03_Key", "$WheelsOfLull_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest03_Data", "_Lull_MQ04"},
		/*04*/ {"WheelsOfLull_Quest04_Key", "$WheelsOfLull_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest04_Data", "_Lull_MQ05"},
		/*05*/ {"WheelsOfLull_Quest05_Key", "$WheelsOfLull_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest05_Data", "_Lull_MQ06"},
		/*06*/ {"WheelsOfLull_Quest06_Key", "$WheelsOfLull_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest06_Data", "_Lull_MQ07"},
		/*07*/ {"WheelsOfLull_Quest07_Key", "$WheelsOfLull_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest07_Data", "_Lull_SQ_Watchman_Restore"},
		/*08*/ {"WheelsOfLull_Quest08_Key", "$WheelsOfLull_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest08_Data", "_Lull_SQ_Water"},
		/*09*/ {"WheelsOfLull_Quest09_Key", "$WheelsOfLull_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest09_Data", "_Lull_SQ_HammarUpdate"},
		/*10*/ {"WheelsOfLull_Quest10_Key", "$WheelsOfLull_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest10_Data", "_Lull_SQ_CartRide"},
		/*11*/ {"WheelsOfLull_Quest11_Key", "$WheelsOfLull_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest11_Data", "_Lull_SQ_ArcheronMines"},
		/*12*/ {"WheelsOfLull_Quest12_Key", "$WheelsOfLull_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$WheelsOfLull_Quest12_Data", "_Lull_SQ_Seamonster"},
	};

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12
	};

	constexpr Serialization::FormArray Items = {
	0x4F3AA5,0x000B5B,0x25D297,0x25D294,0x25D292,0x25D295,
	0x27C1DC,0x27C1D9,0x338EC6,0x205406,0x25D28D,0x267AEE,
	0x25D298,0x02983B,0x0241D8,0x48ED3D,0x3671B0,0x48ED3F,
	0x27C1DB,0x27C1DA,0x25D28F,0x25D296,0x267588,0x267B1A,
	0x07BCA8,
	};

	constexpr Serialization::Variation ItmsV[] = {
	{ 0x271EB0, { 0x271EB1,0x271EB2,0x271EB3,0x271EB4,0x271EB5,0x271EB6 } },
	};
	
	constexpr Serialization::FormArray Books = {
	0x371595,0x376714,0x376715,0x371593,0x37158D,0x371594,
	0x43D473,0x37158E,0x371592,0x43D472,0x37158F,0x371590,
	0x43D471,0x371591,
	};

	constexpr Serialization::FormArray MapMa = {
	0x0BE84A,0x352084,0x000B19,
	};

	// clang-format on

	constexpr std::string_view modname = "WheelsOfLull.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_WOL_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_WOL_Items::Data.GetBase(a_event->baseObj) ? CPatch_WOL_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_WOL_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_WOL_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_WOL_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_WOL_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_WOL_Books::Data.GetBase(target) ? CPatch_WOL_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_WOL_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_WOL_Books::Data.GetAllVariations()) {
				if (CPatch_WOL_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_WOL_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_WOL_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_WOL_Items::Data.GetAllVariations()) {
				if (CPatch_WOL_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_WOL_Items::Data.GetForm(a_baseID));
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

		CPatch_WOL_Items::Data.CompileFormArray(CPatch_WOL::Items, modname);
		CPatch_WOL_Items::Data.CompileVariation(CPatch_WOL::ItmsV, modname);
		
		CPatch_WOL_Books::Data.CompileFormArray(CPatch_WOL::Books, modname);
		CPatch_WOL_MapMa::Data.CompileFormArray(CPatch_WOL::MapMa, modname);
		
		CPatch_WOL_Items::Data.MergeAsCollectable();
		CPatch_WOL_Books::Data.MergeAsCollectable();

		CPatch_WOL_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_WOL_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_WOL_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_WOL_Items::Data);
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
		}
	}
}