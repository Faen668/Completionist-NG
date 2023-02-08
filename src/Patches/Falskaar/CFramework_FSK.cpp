#include "Serialization.hpp"
#include "CFramework_FSK.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_FSK {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> QuestData[] = {
		/*00*/ {"Falskaar_Quest00_Key", "$Falskaar_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest00_Data", "FSMQ01"},
		/*01*/ {"Falskaar_Quest01_Key", "$Falskaar_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest01_Data", "FSMQ02"},
		/*02*/ {"Falskaar_Quest02_Key", "$Falskaar_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest02_Data", "FSMQ03"},
		/*03*/ {"Falskaar_Quest03_Key", "$Falskaar_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest03_Data", "FSMQ04"},
		/*04*/ {"Falskaar_Quest04_Key", "$Falskaar_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest04_Data", "FSMQ05"},
		/*05*/ {"Falskaar_Quest05_Key", "$Falskaar_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest05_Data", "FSMQ06"},
		/*06*/ {"Falskaar_Quest06_Key", "$Falskaar_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest06_Data", "FSMQ07"},
		/*07*/ {"Falskaar_Quest07_Key", "$Falskaar_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest07_Data", "FSMQ08"},
		/*08*/ {"Falskaar_Quest08_Key", "$Falskaar_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest08_Data", "FSMQ09"},
		/*09*/ {"Falskaar_Quest09_Key", "$Falskaar_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest09_Data", "FSSQ11"},
		/*10*/ {"Falskaar_Quest10_Key", "$Falskaar_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest10_Data", "FSSQ08"},
		/*11*/ {"Falskaar_Quest11_Key", "$Falskaar_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest11_Data", "FSSQ06"},
		/*12*/ {"Falskaar_Quest12_Key", "$Falskaar_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest12_Data", "FSSQ05"},
		/*13*/ {"Falskaar_Quest13_Key", "$Falskaar_Quest13_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest13_Data", "FSSQ09"},
		/*14*/ {"Falskaar_Quest14_Key", "$Falskaar_Quest14_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest14_Data", "FSSQ10"},
		/*15*/ {"Falskaar_Quest15_Key", "$Falskaar_Quest15_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest15_Data", "FSSQ02"},
		/*16*/ {"Falskaar_Quest16_Key", "$Falskaar_Quest16_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest16_Data", "FSSQ01"},
		/*17*/ {"Falskaar_Quest17_Key", "$Falskaar_Quest17_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest17_Data", "FSSQ07"},
		/*18*/ {"Falskaar_Quest18_Key", "$Falskaar_Quest18_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest18_Data", "FSSQ13"},
		/*19*/ {"Falskaar_Quest19_Key", "$Falskaar_Quest19_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest19_Data", "FSSQ12"},
		/*20*/ {"Falskaar_Quest20_Key", "$Falskaar_Quest20_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest20_Data", "FSSQ04"},
		/*21*/ {"Falskaar_Quest21_Key", "$Falskaar_Quest21_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest21_Data", "FSSQ16"},
		/*22*/ {"Falskaar_Quest22_Key", "$Falskaar_Quest22_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest22_Data", "FSSQ17"},
		/*23*/ {"Falskaar_Quest23_Key", "$Falskaar_Quest23_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest23_Data", "FSSQ15"},
		/*24*/ {"Falskaar_Quest24_Key", "$Falskaar_Quest24_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest24_Data", "FSSQ14"},
		/*25*/ {"Falskaar_Quest25_Key", "$Falskaar_Quest25_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Falskaar_Quest25_Data", "FSSQ03"},
	};

	constexpr Serialization::FormArray Items = {
	0x1699D4,0x11D62E,0x110AEB,
	};

	constexpr Serialization::FormArray Books = {
	0x09F20F,0x19D06F,0x09F210,0x09F211,0x09F212,
	0x09F213,0x0580B7,0x09F214,0x19D071,0x19D070,
	0x09F216,0x09F215,0x199E91,0x199E89,
	};

	constexpr Serialization::FormArray MapMa = {
	0x033FC8,0x153D38,0x0F4CA4,0x0F4741,0x033FD2,0x048903,0x0DB564,0x053AF9,
	0x1696C0,0x16907D,0x0BFEDF,0x01D755,0x0643A2,0x03659E,0x0A3ECC,0x0866B4,
	0x0C6950,0x03C401,0x03E2CC,0x04835B,0x04C96C,0x119096,0x1195F9,0x09A176,
	0x0239A3,0x0CAFF1,0x0BFEE0,0x0E1983,0x1130A8,0x052A96,0x16907E,0x0D69E2,
	0x0A4507,0x0F4740,0x0A3F0A,0x0985BD,0x153CCA,0x153CC9,0x0E83F7,0x11F697,
	0x0580B4,0x12FF94,0x11F694,0x11FBFA,0x15104A,
	};
	// clang-format on

	constexpr std::string_view modname = "Falskaar.esm";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_FSK_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_FSK_Items::Data.GetBase(a_event->baseObj) ? CPatch_FSK_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_FSK_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_FSK_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_FSK_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_FSK_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_FSK_Books::Data.GetBase(target) ? CPatch_FSK_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FSK_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FSK_Books::Data.GetAllVariations()) {
				if (CPatch_FSK_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_FSK_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FSK_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FSK_Items::Data.GetAllVariations()) {
				if (CPatch_FSK_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_FSK_Items::Data.GetForm(a_baseID));
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

		CPatch_FSK_Items::Data.CompileFormArray(CPatch_FSK::Items, modname);
		CPatch_FSK_Books::Data.CompileFormArray(CPatch_FSK::Books, modname);
		CPatch_FSK_MapMa::Data.CompileFormArray(CPatch_FSK::MapMa, modname);

		CPatch_FSK_Items::Data.MergeAsCollectable();
		CPatch_FSK_Books::Data.MergeAsCollectable();

		CPatch_FSK_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_FSK_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_FSK_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_FSK_Items::Data);
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
	}
}