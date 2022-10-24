#include "Serialization.hpp"
#include "CFramework_WYR.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_WYR {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"Wyrmstooth_Quest00_Key", "$Wyrmstooth_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest00_Data", "WTDragonHunt"},
		/*01*/ {"Wyrmstooth_Quest01_Key", "$Wyrmstooth_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest01_Data", "WTBarrowOfTheWyrm"},
		/*02*/ {"Wyrmstooth_Quest02_Key", "$Wyrmstooth_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest02_Data", "WTBountyAnimal"},
		/*03*/ {"Wyrmstooth_Quest03_Key", "$Wyrmstooth_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest03_Data", "WTBountyBandit"},
		/*04*/ {"Wyrmstooth_Quest04_Key", "$Wyrmstooth_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest04_Data", "WTBountyVampire"},
		/*05*/ {"Wyrmstooth_Quest05_Key", "$Wyrmstooth_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest05_Data", "WTBountyWarlock"},
		/*06*/ {"Wyrmstooth_Quest06_Key", "$Wyrmstooth_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest06_Data", "WTWolfFollower"},
		/*07*/ {"Wyrmstooth_Quest07_Key", "$Wyrmstooth_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest07_Data", "WTPricelessCommodity"},
		/*08*/ {"Wyrmstooth_Quest08_Key", "$Wyrmstooth_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest08_Data", "WTPlayerHome"},
		/*09*/ {"Wyrmstooth_Quest09_Key", "$Wyrmstooth_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest09_Data", "WTShargamFavor"},
		/*10*/ {"Wyrmstooth_Quest10_Key", "$Wyrmstooth_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest10_Data", "WTAthirFavor"},
		/*11*/ {"Wyrmstooth_Quest11_Key", "$Wyrmstooth_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest11_Data", "WTDaenlitFavor"},
		/*12*/ {"Wyrmstooth_Quest12_Key", "$Wyrmstooth_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest12_Data", "WTUberEncounter"},
		/*13*/ {"Wyrmstooth_Quest13_Key", "$Wyrmstooth_Quest13_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest13_Data", "WTKillThalmor"},
		/*14*/ {"Wyrmstooth_Quest14_Key", "$Wyrmstooth_Quest14_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest14_Data", "WTWrapMeUp"},
		/*15*/ {"Wyrmstooth_Quest15_Key", "$Wyrmstooth_Quest15_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest15_Data", "WTBlindRobbersCache"},
		/*16*/ {"Wyrmstooth_Quest16_Key", "$Wyrmstooth_Quest16_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Wyrmstooth_Quest16_Data", "WTElmeraFavor"},
	};

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
	};

	constexpr Serialization::FormArray Items = {
	0x2BF880,0x3B9371,0x6F6806,0x6F67FD,0x8F793C,0x723033,0x784A71,
	0x7295A5,0x511CD0,0x1CF784,0x3B4DEB,0x511CCE,0x6F4631,0x0EA4C9,
	0x4A12C8,0x78F3A2,0x3060D5,
	};

	constexpr Serialization::FormArray Books = {
	0x8EB3B4,0x30EC1D,0x518545,0x332223,0x581C35,0x57F906,0x0B09BB,
	0x0B09BA,0x0B09B8,0x445DA3,0x5862EA,0x445DA7,0x445DB3,0x584019,
	0x4A9EA4,0x4A9F3F,0x57F8EC,0x4A9F38,
	};

	constexpr Serialization::FormArray MapMa = {
	0x450E58,0x8DA8CE,0x872242,0x46DA34,0x47258A,0x163A53,
	0x450DA7,0x22297A,0x163A58,0x185791,0x527D83,0x8B351F,
	0x6AB77D,0x3CA9AA,0x222979,0x22297B,0x81D497,0x82A01A,
	0x986C18,0x82A019,0x486508,0x22297D,0x272C41,0x4D8ADF,
	0x8BD9FE,0x163A56,0x4B9E04,0x163A59,0x709A1D,0x477838,
	0x753B7B,0x4F97F2,0x8D8795,0x47006D,0x002837,0x86DDA7,
	0x2FAF4C,0x163A54,0x450D0E,0x272C21,0x163A55,0x872212,
	0x4A9E92,0x163A5A,0x22297C,
	};

	// clang-format on

	constexpr std::string_view modname = "Wyrmstooth.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_WYR_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_WYR_Items::Data.GetBase(a_event->baseObj) ? CPatch_WYR_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_WYR_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_WYR_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_WYR_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_WYR_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_WYR_Books::Data.GetBase(target) ? CPatch_WYR_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_WYR_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_WYR_Books::Data.GetAllVariations()) {
				if (CPatch_WYR_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_WYR_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_WYR_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_WYR_Items::Data.GetAllVariations()) {
				if (CPatch_WYR_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_WYR_Items::Data.GetForm(a_baseID));
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

		CPatch_WYR_Items::Data.CompileFormArray(CPatch_WYR::Items, modname);
		CPatch_WYR_Books::Data.CompileFormArray(CPatch_WYR::Books, modname);
		CPatch_WYR_MapMa::Data.CompileFormArray(CPatch_WYR::MapMa, modname);

		CPatch_WYR_Items::Data.MergeAsCollectable();
		CPatch_WYR_Books::Data.MergeAsCollectable();

		CPatch_WYR_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_WYR_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_WYR_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_WYR_Items::Data);
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