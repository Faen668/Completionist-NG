#include "Serialization.hpp"
#include "CFramework_VIG.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_VIG_Items {
	Serialization::CompletionistData Data;
}

namespace CPatch_VIG_Books {
	Serialization::CompletionistData Data;
}

namespace CPatch_VIG_MapMa {
	Serialization::CompletionistData Data;
}

namespace CPatch_VIG {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x1353DF,0x3B3500,0x148177,0x03D213,0x119E3C,0x11B7D0,0x0EF0B6,
	0x12942F,0x0D7360,0x0B6D80,0x0DA99B,0x0B0EAA,0x0C7E35,0x0B0EAB,
	0x0B0EAC,0x0B0EAD,0x142BF4,0x29F2EF,0x0CE832,0x00A962,0x03D783,
	0x1D19BD,0x071CE2,0x0D957B,0x1418A9,0x0C847B,0x1418A8,0x1280CB,
	0x0CE835,0x13F43D,0x12A7A3,0x0CB0AE,0x15CE2D,0x11880C,0x323B2F,
	0x0DFD65,0x0DFD66,0x0DFD67,0x0DFD68,0x1804E9,0x12B2F9,0x130E6B,
	0x187829,0x18782A,0x18782B,0x18782C,0x130E6A,0x119E3B,0x130E69,
	0x22B38C,0x12A7A2,0x142BF5,0x0EB828,0x0EB827,0x1418AB,0x1280CA,
	0x0CB0AB,0x1450A0,0x1418AA,0x1280CC,0x100E9F,0x0EA4C2,0x2699FB,
	0x2699FC,0x2699FD,0x2699FE,0x296938,0x13F43C,0x0D428C,0x11B7CF,
	0x0D428B,0x0CFCB6,0x12942E,0x1804E7,0x0CAA70,
	};

	constexpr Serialization::Variation ItmsV[] = {
	{ 0x23007E, { 0x230080,0x23007C,0x23007D,0x23007F,0x230081 } },
	{ 0x144CDD, { 0x144CDE } },
	{ 0x0BBF35, { 0x0BD352 } },
	{ 0x0D428A, { 0x0D568D } },
	{ 0x323B2F, { 0x43CBAD } },
	{ 0x0C3DA4, { 0x0B828B } },
	{ 0x1AABC2, { 0x1AABC3 } },
	{ 0x0C68B9, { 0x0C68B8 } },
	};
	
	constexpr Serialization::FormArray Books = {
	0x4A8AFA,0x054ED0,0x054ED1,0x4A8AFD,0x4A8B03,0x4A8AFE,0x4A8B05,
	0x4A8B04,0x4A8AFC,0x0D69FC,0x0D69FA,0x0D69FD,0x10832C,0x108323,
	0x108325,0x108322,0x0DE93B,0x0CDE74,0x1258B1,0x0CDE76,0x1404FF,
	0x108324,0x4A8AFB,0x1CB871,0x1CB872,0x1CB873,0x0DB22D,0x4E1317,
	0x13B4F9,0x13B4FA,0x13B4FB,0x24724A,0x13B4FC,0x13B4FD,0x13B501,
	0x13B506,0x13B4F7,0x054ED3,0x4A8B07,0x4A8B02,0x3F7CFC,0x13BAB0,
	0x3FB795,0x12905B,0x18B1BA,0x4A8AF6,0x4A8B00,0x4A8AFF,0x0CB0DF,
	0x12905E,0x114FF7,0x12905D,0x0D429E,0x12905F,0x4A8AF9,0x0DB22E,
	0x054ED2,0x12905C,0x4A8B06,0x18B1B9,0x4A8AF8,0x4A8B01,0x4A8AF7,
	0x054ED5,0x054ED6,0x054ED4,0x038AAE,
	};

	constexpr Serialization::FormArray MapMa = {
	0x088308,0x0801A8,0x093F3A,0x08ABE1,0x1388FA,0x089143,0x0A2517,
	0x0896EC,0x06A6D8,0x0A0C88,0x0A578D,0x091910,0x204817,0x128D5A,
	0x144AD5,0x128D54,0x09205C,0x09205D,0x092058,0x088312,0x21C52E,
	0x128D57,0x25B236,0x08773A,0x093F39,0x07FB64,0x09F7FB,0x145667,
	0x09BFF0,0x0A3E75,0x089DAA,0x0801A9,0x08E83E,0x08773F,0x4C3D8B,
	0x0A95A4,0x0A578F,0x128D51,0x091911,0x09D2B6,0x025045,0x0A87F4,
	0x128D55,0x0AFEBA,0x07FB65,0x09C65C,0x171D71,
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

	inline std::string_view modname = "Vigilant.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_VIG_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_VIG_Items::Data.GetBase(a_event->baseObj) ? CPatch_VIG_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_VIG_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_VIG_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_VIG_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_VIG_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_VIG_Books::Data.GetBase(target) ? CPatch_VIG_Books::Data.GetBase(target) : target;
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_VIG_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_VIG_Books::Data.GetAllVariations()) {
				if (CPatch_VIG_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_VIG_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_VIG_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_VIG_Items::Data.GetAllVariations()) {
				if (CPatch_VIG_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_VIG_Items::Data.GetForm(a_baseID));
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

		CPatch_VIG_Items::Data.CompileFormArray(CPatch_VIG::Items, modname);
		CPatch_VIG_Items::Data.CompileVariation(CPatch_VIG::ItmsV, modname);
		
		CPatch_VIG_Books::Data.CompileFormArray(CPatch_VIG::Books, modname);
		CPatch_VIG_MapMa::Data.CompileFormArray(CPatch_VIG::MapMa, modname);
		
		CPatch_VIG_Items::Data.MergeAsCollectable();
		CPatch_VIG_Books::Data.MergeAsCollectable();

		CPatch_VIG_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_VIG_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_VIG_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);

		//using namespace FrameworkHandler;
		//RegisterAs<FrameworkID::kVIG_Items>(&Items_NameArray, &Items_FormArray, &Items_BoolArray, &Items_TextArray);
		//RegisterAs<FrameworkID::kVIG_Books>(&Books_NameArray, &Books_FormArray, &Books_BoolArray, &Books_TextArray);
		//RegisterAs<FrameworkID::kVIG_MapMa>(&MapMa_NameArray, &MapMa_FormArray, &MapMa_BoolArray, &MapMa_TextArray);
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