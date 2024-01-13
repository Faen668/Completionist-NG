#include "Serialization.hpp"
#include "CFramework_AHO.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

#undef AddForm

namespace CPatch_AHO 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]
	{
		/*00*/ {"ProjectAHO_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ01"},
		/*01*/ {"ProjectAHO_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ03"},
		/*02*/ {"ProjectAHO_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ04"},
		/*03*/ {"ProjectAHO_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ05"},
		/*04*/ {"ProjectAHO_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ06"},
		/*05*/ {"ProjectAHO_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "DwarfSphereQ07"},
		/*06*/ {"ProjectAHO_Quest06", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ01"},
		/*07*/ {"ProjectAHO_Quest07", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ02"},
		/*08*/ {"ProjectAHO_Quest08", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ03"},
		/*09*/ {"ProjectAHO_Quest09", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ04"},
		/*10*/ {"ProjectAHO_Quest10", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ05"},
		/*11*/ {"ProjectAHO_Quest11", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ06"},
		/*12*/ {"ProjectAHO_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ07"},
		/*13*/ {"ProjectAHO_Quest13", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ08"},
		/*14*/ {"ProjectAHO_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ09"},
		/*15*/ {"ProjectAHO_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "DwarfSphereSQ11"},
	};

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0xAF7B13,0xA0D52D,0x7B54F3,
	};

	constexpr Serialization::FormArray Books = {
	0x4E7E5F,0x4E7E9B,0x4E7E91,0x4E7E67,0x4E7E93,0x4E7E8C,0x4E7E92,
	0x4E7E5E,0x4E7E65,0x4E7E5C,0x4E7E8E,0x4E2D5A,0x4E7E6E,0x4E7E8A,
	0x4E7E6F,0x4E7E70,0x4E7E71,0x4E7E72,0x4E7E73,0x4E7E74,0x4E7E75,
	0x4E7E76,0x4E7E77,0x4E7E78,0x4E7E79,0x4E7E7A,0x4E7E90,0x4E7E88,
	0xAF7B11,0xB496D9,0xB496DA,0xB496DB,0xB496DD,0xB496DC,0x947347,
	0x947348,0x4E7E66,0x4E7E5D,0x4E7E94,0x4E7E8B,0x4E7E8D,0x4E7E87,
	0x4E7E8F,0x4E7E89,0x4E7E68,0x5A8541,0xA68B9E,0xB62BC8,0xA68B94,
	0xA68B98,0x8E6EB7,
	};

	constexpr Serialization::FormArray MapMa = {
	0x29157D,
	};
	
	// clang-format on

	constexpr std::string_view modname = "Dwarfsphere.esp";

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
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 43, QuestData[i].unique_identifier));
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

		ItemData.CompileFormArray(CPatch_AHO::Items, modname);
		BookData.CompileFormArray(CPatch_AHO::Books, modname);
		MapsData.CompileFormArray(CPatch_AHO::MapMa, modname);

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

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageProjectAHO", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageProjectAHO", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], "$MCMPageProjectAHO", std::to_underlying(EntryCategory::kMapM)));
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