#include "Serialization.hpp"
#include "CFramework_BOO.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_BOO 
{
	using namespace CFramework_Master;

	CQuestData QuestData[] {
		{"TBOO_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "TBO01"},
		{"TBOO_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "TBO02"},
		{"TBOO_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "TBO03"},
		{"TBOO_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "TBO04"},
		{"TBOO_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "TBO05"},
		{"TBOO_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "TBO05B"},
		{"TBOO_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "TBO05Part2"},
		{"TBOO_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "TBO06"},
		{"TBOO_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "TBO07"},
		{"TBOO_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "TBO08"},
		{"TBOO_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "TBO09"},
		{"TBOO_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "TBO11"},
		{"TBOO_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "TBO10"},
		{"TBOO_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "TBO12"},
		{"TBOO_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "TBO13"},
		{"TBOO_Quest15", CFlagEnum::kMain, CCompEnum::kStand, "TBO14"},
		{"TBOO_Quest16", CFlagEnum::kMain, CCompEnum::kStand, "TBO15"},
		{"TBOO_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ1"},
		{"TBOO_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ2"},
		{"TBOO_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ3"},
		{"TBOO_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ4"},
		{"TBOO_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ5Dead"},
		{"TBOO_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ6"},
		{"TBOO_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ7"},
		{"TBOO_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "TBOSQ8"},
	};

	CArrayData ArrayData{ &Quest_IdenArray, &Quest_NameArray, &Quest_TextArray, &Quest_BoolArray, &Quest_RadiArray };

	// clang-format off

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

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }
		
		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		CHandler::InstallSearchTerms();
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
				->initQuestData(&ArrayData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 43));
		}
		Quest_BoolArray = std::vector<bool>(CArraySize, false);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() 
	{
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
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_BOO_Books::Data.GetForm(a_eventID)->GetName());
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_BOO_Items::Data.GetForm(a_eventID)->GetName());
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

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageBrotherhood", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_NameArray[i], "$MCMPageBrotherhood", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto& name : MapMa_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageBrotherhood", std::to_underlying(EntryCategory::kMapM)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_BOO_Items::Data);
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