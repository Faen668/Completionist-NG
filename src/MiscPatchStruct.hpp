#pragma once
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

struct CMiscPatch
{
	using master = CFramework_Master::FrameworkAPI;

	int32_t ItemsID{};
	int32_t BooksID{};
	int32_t MapMaID{};

	std::string modname{};
	std::string mcmpage{};

	Serialization::CompletionistData ItemData;
	Serialization::CompletionistData BookData;
	Serialization::CompletionistData MapsData;

	bool itemsSecReq{};
	bool booksSecReq{};
	bool mapmaSecReq{};

	std::vector<std::string> Items_NameArray{};
	std::vector<std::string> Items_TextArray{};
	std::vector<RE::TESForm*> Items_FormArray{};
	std::vector<bool> Items_BoolArray{};
	std::int32_t Items_EntriesTotal{};
	std::int32_t Items_EntriesFound{};

	std::vector<std::string> Books_NameArray{};
	std::vector<std::string> Books_TextArray{};
	std::vector<RE::TESForm*> Books_FormArray{};
	std::vector<bool> Books_BoolArray{};
	std::int32_t Books_EntriesTotal{};
	std::int32_t Books_EntriesFound{};

	std::vector<std::string> MapMa_NameArray{};
	std::vector<std::string> MapMa_TextArray{};
	std::vector<RE::TESForm*> MapMa_FormArray{};
	std::vector<bool> MapMa_BoolArray{};
	std::int32_t MapMa_EntriesTotal{};
	std::int32_t MapMa_EntriesFound{};

	//---------------------------------------------------
	//-- function ---------------------------------------
	//---------------------------------------------------

	void InstallFramework() 
	{
		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		InjectAndCompileData();
		InstallSearchTerms();
		CFramework_Master::PatchesInstalled += 1;
	};

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void InjectAndCompileData() 
	{
		if (itemsSecReq) {
			ItemData.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
			ItemData.MergeAsCollectable();
		}

		if (booksSecReq) {
			BookData.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
			BookData.MergeAsCollectable();
		}

		if (mapmaSecReq) {
			MapsData.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void InstallSearchTerms() 
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], mcmpage, std::to_underlying(CFramework_Master::EntryCategory::kItem)));
		}
		for (auto i = 0; i < Books_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Books_FormArray[i], Books_NameArray[i], mcmpage, master::GetBookCategoryType(Books_FormArray[i])));
		}
		for (auto i = 0; i < MapMa_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(MapMa_FormArray[i], MapMa_NameArray[i], mcmpage, std::to_underlying(CFramework_Master::EntryCategory::kMapM)));
		}
	};

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void UpdateFoundForms() {
		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = master::IsItemKnown(Items_FormArray[i], &ItemData);
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = master::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = CFramework_Master::FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	};

	//---------------------------------------------------
	//-- Framework Events ( Process Hooked Markers ) ----
	//---------------------------------------------------

	void ProcessHookedMarker(const char* nam) {
		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, MapMa_NameArray[i]) && !CFramework_Master::FoundItemData_NoShow.HasForm(MapMa_FormArray[i])) {
				ProcessMapMarker(MapMa_FormArray[i], i, true);
				return;
			}
		}
	};

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, bool from_hook) {
		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker && !CFramework_Master::FoundItemData_NoShow.HasForm(a_form)) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					CFramework_Master::FoundItemData_NoShow.AddForm(a_form);
					if (from_hook)
					{
						auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, MapMa_NameArray[a_pos]);
						master::SendNotification(msg, "NotifySpecial");
						master::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, MapMa_NameArray[a_pos]);
					}
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	};

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void ProcessFoundForm(ProcessFoundFormArgs, std::string a_section) {

		if (!CFramework_Master::FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data.GetForm(a_eventID)->GetName());
			master::SendNotification(msg, a_section);

			if (auto* book = static_cast<RE::TESObjectBOOK*>(BookData.GetForm(a_eventID)); book && book->GetSpell()) {
				master::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
			}
			else {
				master::AddNewEventToLog(eventHandle, BookData.GetForm(a_eventID)->GetName());
			}
		}

		CFramework_Master::FoundItemData.AddForm(a_baseID);
		for (auto var : data.GetAllVariations()) {
			if (data.GetBase(var) == a_baseID) {
				CFramework_Master::FoundItemData.AddForm(var);
			}
		}

		bools->at(std::distance(forms.begin(), std::ranges::find(forms, data.GetForm(a_baseID)))) = true;
		*found = std::ranges::count(*bools, true);
	};

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, cmd::kCollected, "NotifyItems");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	void OnBooksReadEvent(RE::BooksRead::Event const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (!BookData.HasForm(a_event->book->GetFormID())) { return; }

		auto base = BookData.GetBase(a_event->book->GetFormID()) ? BookData.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		ProcessFoundForm(base, a_event->book->GetFormID(), BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, cmd::kBook, "NotifyBooks");
		return;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	void OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && BookData.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = BookData.GetBase(target) ? BookData.GetBase(target) : target;
				ProcessFoundForm(base, target, BookData, Books_FormArray, &Books_BoolArray, &Books_EntriesFound, cmd::kBook, "NotifyBooks");
				return;
			}
			return;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				ProcessMapMarker(MapMa_FormArray[i], i, false);
			}
		}
		return;
	}
};