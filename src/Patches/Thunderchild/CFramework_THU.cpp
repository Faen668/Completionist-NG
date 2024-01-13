#include "Serialization.hpp"
#include "CFramework_THU.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_THU {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x24B9BB,0x066DE2,0x204AB3,
	};

	constexpr Serialization::FormArray Books = {
	0x0C57F3,0x072C8F,

	};

	constexpr Serialization::FormArray MapMa = {
	0x3111AD,0x311191,0x3111A1,0x31119D,0x31118F,
	0x31119F,0x311193,0x311184,0x311199,0x3111A9,
	0x31118D,0x31119B,0x311195,0x311187,0x31118B,
	0x3111A5,0x311188,0x311180,0x311181,0x3111A3,
	0x3111AB,0x3111A7,0x311197,
	};

	// clang-format on

	constexpr std::string_view modname = "Thunderchild - Epic Shout Package.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();

		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		FrameworkAPI::AddMapMarkerDiscovery(ProcessHookedMarker);

		CEvents::EventHandler::RegisterForEvent_OnBooksReadEvent(CHandler::OnBooksReadEvent);
		CEvents::EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(CHandler::OnMenuOpenCloseEvent);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, log::kCollected, "NotifyItems");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void CHandler::OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (!BookData.HasForm(a_event->book->GetFormID())) { return; }

		auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, FrameworkAPI::GetBookLogType(a_event->book), "NotifyBooks");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void CHandler::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, log::kBook, "NotifyBooks");
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

		ItemData.CompileFormArray(CPatch_THU::Items, modname);
		BookData.CompileFormArray(CPatch_THU::Books, modname);
		MapsData.CompileFormArray(CPatch_THU::MapMa, modname);

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
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageThunderchild", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], "$MCMPageThunderchild", FrameworkAPI::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], "$MCMPageThunderchild", std::to_underlying(EntryCategory::kMapM)));
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