#include "Serialization.hpp"
#include "CFramework_FSK.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_FSK_Items {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSK_Books {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSK_MapMa {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSK {
	using namespace CFramework_Master;

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

	inline std::vector<std::string> Items_NameArray;
	inline std::vector<std::string> Items_TextArray;
	inline std::vector<RE::TESForm*> Items_FormArray;
	inline std::vector<bool> Items_BoolArray;
	inline std::int32_t Items_EntriesTotal;
	inline std::int32_t Items_EntriesFound;

	inline std::vector<std::string> Books_NameArray;
	inline std::vector<std::string> Books_TextArray;
	inline std::vector<RE::TESForm*> Books_FormArray;
	inline std::vector<bool> Books_BoolArray;
	inline std::int32_t Books_EntriesTotal;
	inline std::int32_t Books_EntriesFound;

	inline std::vector<std::string> MapMa_NameArray;
	inline std::vector<std::string> MapMa_TextArray;
	inline std::vector<RE::TESForm*> MapMa_FormArray;
	inline std::vector<bool> MapMa_BoolArray;
	inline std::int32_t MapMa_EntriesTotal;
	inline std::int32_t MapMa_EntriesFound;

	inline std::string_view modname = "Falskaar.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		PatchesInstalled += 1;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_FSK_Books::Data.GetForm(a_eventID)->GetName());
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_FSK_Items::Data.GetForm(a_eventID)->GetName());
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

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			if (FoundItemData.HasForm(Items_FormArray[i]->GetFormID())) {
				Items_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			if (FoundItemData.HasForm(Books_FormArray[i]->GetFormID())) {
				Books_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID())) {
				MapMa_BoolArray[i] = true;
			}
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}
}