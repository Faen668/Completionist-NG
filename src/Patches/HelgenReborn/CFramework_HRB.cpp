#include "Serialization.hpp"
#include "CFramework_HRB.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_HRB 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]
	{
		{"HelgenReborn_Quest00", CFlagEnum::kSide, CCompEnum::kStand, "BalokCourierTimer"},
		{"HelgenReborn_Quest01", CFlagEnum::kSide, CCompEnum::kStand, "BalokHelgen01"},
		{"HelgenReborn_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "BalokThalmorPrison"},
		{"HelgenReborn_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "BalokConstructionQuest"},
		{"HelgenReborn_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "BalokBitterWounds"},
		{"HelgenReborn_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "BalokCiennaQuest"},
		{"HelgenReborn_Quest06", CFlagEnum::kSide, CCompEnum::kStage, "BalokHamingQuest"},
		{"HelgenReborn_Quest07", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant01"},
		{"HelgenReborn_Quest08", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant02"},
		{"HelgenReborn_Quest09", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant03"},
		{"HelgenReborn_Quest10", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant04"},
		{"HelgenReborn_Quest11", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant05"},
		{"HelgenReborn_Quest12", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant06"},
		{"HelgenReborn_Quest13", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant01Stormcloak"},
		{"HelgenReborn_Quest14", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant02Stormcloak"},
		{"HelgenReborn_Quest15", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant03Stormcloak"},
		{"HelgenReborn_Quest16", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant04Stormcloak"},
		{"HelgenReborn_Quest17", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant05Stormcloak"},
		{"HelgenReborn_Quest18", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant06Stormcloak"},
		{"HelgenReborn_Quest19", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant01Empire"},
		{"HelgenReborn_Quest20", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant02Empire"},
		{"HelgenReborn_Quest21", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant03Empire"},
		{"HelgenReborn_Quest22", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant04Empire"},
		{"HelgenReborn_Quest23", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant05Empire"},
		{"HelgenReborn_Quest24", CFlagEnum::kRadi, CCompEnum::kStand, "BalokRadiant06Empire"},
	};

	CStageData StageData[]{
		{"HelgenReborn_Quest06", CStageEnum::kPast, 30, 0},
	};

	CArrayData ArrayData{ &Quest_IdenArray, &Quest_NameArray, &Quest_TextArray, &Quest_BoolArray, &Quest_RadiArray };

	// clang-format off

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

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()
				->initQuestData(&ArrayData)
				->initStageData(StageData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 34));
		}
		Quest_BoolArray = std::vector<bool>(CArraySize, false);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
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
}