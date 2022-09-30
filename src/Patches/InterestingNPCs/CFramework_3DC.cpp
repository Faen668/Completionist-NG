#include "Serialization.hpp"
#include "CFramework_3DC.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_3DC_Items {
	Serialization::CompletionistData Data;
}

namespace CPatch_3DC_Books {
	Serialization::CompletionistData Data;
}

namespace CPatch_3DC_MapMa {
	Serialization::CompletionistData Data;
}

namespace CPatch_3DC {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x186CD2,0x28493D,0x1B2920,0x0DF4B8,0x1067D8,0x0DF4BA,0x0DF4BB,
	0x1FF21B,0x19A951,0x0DF4B7,0x19853F,0x0E97FD,0x22B94F,0x12E113,
	0x1A6EF3,0x27F184,0x1D8052,0x18F49B,0x1A6EF7,0x2A25FB,0x200C93,
	0x1B018C,0x2BE76E,0x500F44,0x363BF7,0x363BF6,0x152283,0x16A25B,
	0x1C99CF,0x14E0DD,0x144638,0x19A970,0x148844,0x2CB194,0x12A149,
	0x18A9F9,0x1C3AC4,0x1C3AC3,0x1C3AC6,0x238527,0x1C5CDB,0x1ACFFA,
	0x0DE9E9,0x14E0DE,0x1915E0,0x205FD9,0x1F3B47,0x0CDC78,0x182E25,
	};

	constexpr Serialization::FormArray Books = {
	0x272F9B,0x02B06F,0x130790,0x1A37C7,0x1A118C,0x13A92F,0x058932,
	0x190A9B,0x191023,0x191024,0x19159D,0x130792,0x01652C,0x130D40,
	0x1B018E,0x12F1B5,0x1CED4E,0x1AE651,0x1B4638,
	};

	constexpr Serialization::FormArray MapMa = {
	0x21908B,0x30846C,0x19DBFB,0x2C5C97,0x29844D,0x2A269C,0x380979,
	0x28BD0E,0x1A7F98,0x1DBB14,0x1DA7F2,0x248CAF,0x1FD608,0x1C2283,
	0x245AA4,0x17CB26,0x1CC676,0x1D40FC,0x24E51F,0x1BD250,
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

	inline std::string_view modname = "3DNPC.esp";

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_3DC_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_3DC_Items::Data.GetBase(a_event->baseObj) ? CPatch_3DC_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_3DC_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_3DC_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_3DC_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_3DC_Books::Data.GetBase(target) ? CPatch_3DC_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_3DC_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Books::Data.GetAllVariations()) {
				if (CPatch_3DC_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_3DC_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_3DC_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_3DC_Items::Data.GetAllVariations()) {
				if (CPatch_3DC_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_3DC_Items::Data.GetForm(a_baseID));
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

		CPatch_3DC_Items::Data.CompileFormArray(CPatch_3DC::Items, modname);
		CPatch_3DC_Books::Data.CompileFormArray(CPatch_3DC::Books, modname);
		CPatch_3DC_MapMa::Data.CompileFormArray(CPatch_3DC::MapMa, modname);

		CPatch_3DC_Items::Data.MergeAsCollectable();
		CPatch_3DC_Books::Data.MergeAsCollectable();

		CPatch_3DC_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_3DC_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_3DC_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

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