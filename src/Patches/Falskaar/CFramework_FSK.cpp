#include "Serialization.hpp"
#include "CFramework_FSK.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_FSK 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]
	{
		/*00*/ {"Falskaar_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ01"},
		/*01*/ {"Falskaar_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ02"},
		/*02*/ {"Falskaar_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ03"},
		/*03*/ {"Falskaar_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ04"},
		/*04*/ {"Falskaar_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ05"},
		/*05*/ {"Falskaar_Quest05", CFlagEnum::kSide, CCompEnum::kStand, "FSMQ06"},
		/*06*/ {"Falskaar_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "FSMQ07"},
		/*07*/ {"Falskaar_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ08"},
		/*08*/ {"Falskaar_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "FSMQ09"},
		/*09*/ {"Falskaar_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ11"},
		/*10*/ {"Falskaar_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ08"},
		/*11*/ {"Falskaar_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ06"},
		/*12*/ {"Falskaar_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ05"},
		/*13*/ {"Falskaar_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ09"},
		/*14*/ {"Falskaar_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ10"},
		/*15*/ {"Falskaar_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ02"},
		/*16*/ {"Falskaar_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ01"},
		/*17*/ {"Falskaar_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ07"},
		/*18*/ {"Falskaar_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ13"},
		/*19*/ {"Falskaar_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ12"},
		/*20*/ {"Falskaar_Quest20", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ04"},
		/*21*/ {"Falskaar_Quest21", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ16"},
		/*22*/ {"Falskaar_Quest22", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ17"},
		/*23*/ {"Falskaar_Quest23", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ15"},
		/*24*/ {"Falskaar_Quest24", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ14"},
		/*25*/ {"Falskaar_Quest25", CFlagEnum::kSide, CCompEnum::kStand, "FSSQ03"},
	};

	/*NA*/ CArrayData ArrayData{ &Quest_IdenArray, &Quest_NameArray, &Quest_TextArray, &Quest_BoolArray, &Quest_RadiArray };

	// clang-format off

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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 33));
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

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFalskaar", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_NameArray[i], "$MCMPageFalskaar", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto& name : MapMa_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFalskaar", std::to_underlying(EntryCategory::kMapM)));
		}
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
}