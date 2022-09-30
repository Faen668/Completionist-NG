#include "Serialization.hpp"
#include "CFramework_AHO.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_AHO_Items {
	Serialization::CompletionistData Data;
}

namespace CPatch_AHO_Books {
	Serialization::CompletionistData Data;
}

namespace CPatch_AHO_MapMa {
	Serialization::CompletionistData Data;
}

namespace CPatch_AHO {
	using namespace CFramework_Master;

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

	inline std::string_view modname = "Dwarfsphere.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_AHO_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_AHO_Items::Data.GetBase(a_event->baseObj) ? CPatch_AHO_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_AHO_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_AHO_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_AHO_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_AHO_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_AHO_Books::Data.GetBase(target) ? CPatch_AHO_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_AHO_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_AHO_Books::Data.GetAllVariations()) {
				if (CPatch_AHO_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_AHO_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_AHO_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_AHO_Items::Data.GetAllVariations()) {
				if (CPatch_AHO_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_AHO_Items::Data.GetForm(a_baseID));
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

		CPatch_AHO_Items::Data.CompileFormArray(CPatch_AHO::Items, modname);
		CPatch_AHO_Books::Data.CompileFormArray(CPatch_AHO::Books, modname);
		CPatch_AHO_MapMa::Data.CompileFormArray(CPatch_AHO::MapMa, modname);

		CPatch_AHO_Items::Data.MergeAsCollectable();
		CPatch_AHO_Books::Data.MergeAsCollectable();

		CPatch_AHO_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_AHO_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_AHO_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);

		//using namespace FrameworkHandler;
		//RegisterAs<FrameworkID::kAHO_Items>(&Items_NameArray, &Items_FormArray, &Items_BoolArray, &Items_TextArray);
		//RegisterAs<FrameworkID::kAHO_Books>(&Books_NameArray, &Books_FormArray, &Books_BoolArray, &Books_TextArray);
		//RegisterAs<FrameworkID::kAHO_MapMa>(&MapMa_NameArray, &MapMa_FormArray, &MapMa_BoolArray, &MapMa_TextArray);
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