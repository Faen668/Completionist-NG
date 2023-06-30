#include "Serialization.hpp"
#include "CFramework_WYR.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_WYR 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]
	{
		{"Wyrmstooth_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "WTDragonHunt"},
		{"Wyrmstooth_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "WTBarrowOfTheWyrm"},
		{"Wyrmstooth_Quest02", CFlagEnum::kSide, CCompEnum::kStand, "WTBountyAnimal"},
		{"Wyrmstooth_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "WTBountyBandit"},
		{"Wyrmstooth_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "WTBountyVampire"},
		{"Wyrmstooth_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "WTBountyWarlock"},
		{"Wyrmstooth_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "WTWolfFollower"},
		{"Wyrmstooth_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "WTPricelessCommodity"},
		{"Wyrmstooth_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "WTPlayerHome"},
		{"Wyrmstooth_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "WTShargamFavor"},
		{"Wyrmstooth_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "WTAthirFavor"},
		{"Wyrmstooth_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "WTDaenlitFavor"},
		{"Wyrmstooth_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "WTUberEncounter"},
		{"Wyrmstooth_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "WTKillThalmor"},
		{"Wyrmstooth_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "WTWrapMeUp"},
		{"Wyrmstooth_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "WTBlindRobbersCache"},
		{"Wyrmstooth_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "WTElmeraFavor"},
	};

	CArrayData ArrayData{ &Quest_IdenArray, &Quest_NameArray, &Quest_TextArray, &Quest_BoolArray, &Quest_RadiArray };

	// clang-format off

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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 41));
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
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_WYR_Books::Data.GetForm(a_eventID)->GetName());
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_WYR_Items::Data.GetForm(a_eventID)->GetName());
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

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageWyrmstooth", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_NameArray[i], "$MCMPageWyrmstooth", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto& name : MapMa_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageWyrmstooth", std::to_underlying(EntryCategory::kMapM)));
		}
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
}