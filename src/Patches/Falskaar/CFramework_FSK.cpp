#include "Serialization.hpp"
#include "CFramework_FSK.hpp"
#include "Internal Utility/Events.hpp"
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

		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
		CHandler::InstallSearchTerms();

		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		FrameworkAPI::AddMapMarkerDiscovery(ProcessHookedMarker);

		CEvents::EventHandler::RegisterForEvent_OnBooksReadEvent(CHandler::OnBooksReadEvent);
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(CHandler::OnMenuOpenCloseEvent);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 33, QuestData[i].unique_identifier));
		}
	};

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, cmd::kCollected, "NotifyItems");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (!BookData.HasForm(a_event->book->GetFormID())) { return; }

		auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, cmd::kBook, "NotifyBooks");
				return;
			}
			return;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i, false);
			}
		}
		return;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(ProcessFoundFormArgs, std::string a_section) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, a_section);
			FrameworkAPI::AddNewEventToLog(eventHandle, data.GetForm(a_eventID)->GetName());
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : data.GetAllVariations()) {
			if (data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		bools->at(std::distance(forms.begin(), std::ranges::find(forms, data.GetForm(a_baseID)))) = true;
		*found = std::ranges::count(*bools, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, bool from_hook) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker && !FoundItemData_NoShow.HasForm(a_form)) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_form);
					if (from_hook)
					{
						auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, MapMa_NameArray[a_pos]);
						FrameworkAPI::SendNotification(msg, "NotifySpecial");
						FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, MapMa_NameArray[a_pos]);
					}
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Hooked Markers ) ----
	//---------------------------------------------------

	void CHandler::ProcessHookedMarker(const char* nam)
	{
		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, MapMa_NameArray[i]) && !FoundItemData_NoShow.HasForm(MapMa_FormArray[i])) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i, true);
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemData.CompileFormArray(CPatch_FSK::Items, modname);
		BookData.CompileFormArray(CPatch_FSK::Books, modname);
		MapsData.CompileFormArray(CPatch_FSK::MapMa, modname);

		ItemData.MergeAsCollectable();
		BookData.MergeAsCollectable();

		ItemData.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		BookData.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		MapsData.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageFalskaar", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageFalskaar", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], "$MCMPageFalskaar", std::to_underlying(EntryCategory::kMapM)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &ItemData);
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