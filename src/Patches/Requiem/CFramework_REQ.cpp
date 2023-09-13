#include "Serialization.hpp"
#include "CFramework_REQ.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_REQ {
	using namespace CFramework_Master;

	// clang-format off
	
	// Armor
	constexpr Serialization::FormArray ItmL1_A = {
		0x000D6B,0x003E01,0x003E02,0x003E03,0x0A1167,0x0A1168,0x0A1169,0x0A116A,0x0BED17,0x43FE4C,
		0x45D3AB,0xAD36E1,0xAD3AFF,0xAD3B00,0xAD3B3E,0xAD8CA9,0xAD8CAA,0xAD8CAB,0xAD8CAD,0xADDD76,
		0xADDD77,0xADDD78,0xADDD81,0xADDD82,0xADDD83,0xADDD84,0xADDD85,0xADDD86,0xADDD87,0xADDD88,
		0xADDD89,0xADDD8A,0xADDD8B,0xADDD8C,0xADDD8D,0xADDD8E,0xADDD9A,0xADDD9B,0xADDD9C,0xADDD9D,
		0xADDDA5,0xADDDA6,0xADDDA7,0xADDDA8,0xADDDB1,0xADDDB2,0xADDDB3,0xADDDB4,0xADDDC0,0xADDDC1,
		0xADDDC2,0xADDDC3,0xADDDE0,0xADE426,0xADE427,0xADE428,0xADE434,0xADE435,0xADE436,0xADE437,
	};

	// Misc
	constexpr Serialization::FormArray ItmL1_M = {
		0x04FEBC, 0x04FEC0,0x21025A,
	};

	// Weapons
	constexpr Serialization::FormArray ItmL2 = {
		0x044BFD,0x0731D3,0x0801CC,0x094B3F,0x0AB005,0x169B46,0x1AA25A,0x1AA25B,0x1AA25D,0x1AA260,
		0x1AA261,0x1AA262,0x1AA263,0x352150,0x372455,0x447573,0x5CBCA7,0xADDD6D,0xADDD6E,
	};

	// General Books
	constexpr Serialization::FormArray ItmL3_B = {
		0x05CBC7,0x05CBCA,0x05CBD4,0x05CBD7,0x05CBDA,0x05CBDC,0x05CBDE,0x05CBE0,0x069947,0x06BA1A,
		0x06BA1D,0x06BA21,0x085D6C,0x08850D,0x0A8863,0x0AB007,0x0C6ADC,0x35B997,0x4BBD05,0x4CAABF,
		0xAD36E8,0xAD3748,0xAD374B,0xAD38D2,
	};

	// Skill Books
	constexpr Serialization::FormArray ItmL3_S = {
		0x4BBCC5,0x4BBCCB,
	};

	// Spell Tomes
	constexpr Serialization::FormArray ItmL4 = {
		0x02D142,0x02D143,0x02D145,0x02D147,0x02D148,0x02FC87,0x02FC88,0x02FC89,0x02FC8A,0x02FC8B,
		0x02FC8C,0x02FC8D,0x02FC8E,0x02FC8F,0x02FC90,0x02FC91,0x02FC92,0x02FC93,0x02FC94,0x02FC95,
		0x02FC96,0x02FC97,0x02FC98,0x02FC99,0x02FC9A,0x02FC9B,0x02FC9C,0x02FC9D,0x02FC9E,0x02FC9F,
		0x02FCA0,0x02FCA1,0x02FCA2,0x02FCA3,0x02FCA4,0x02FCA5,0x02FCA6,0x02FCA7,0x02FCA8,0x02FCA9,
		0x02FCAA,0x02FCAB,0x02FCAC,0x02FCAD,0x02FCAE,0x02FCAF,0x02FCB0,0x033DD8,0x033DDA,0x03CA22,
		0x03DA6C,0x04675B,0x04675C,0x04675D,0x04675E,0x04675F,0x046760,0x06F5B6,0x06F5D0,0x06F5D2,
		0x06F5D3,0x07373B,0x11D598,0x11D59A,0x11D59B,0x11D59C,0x129549,0x153D12,0x15B3E4,0x225F3C,
		0x26DCA5,0x26DCA6,0x270460,0x270461,0x270462,0x270463,0x27CA84,0x27CA86,0x331E47,0x458468,
		0x61FF5B,0x61FF5C,0x61FF5D,0x61FF5E,0x61FF5F,0x61FF60,0x61FF61,0x61FF62,0x61FF64,0x61FF67,
		0xA552E3,0xA552E4,0xA552E5,0xA552E6,0xA552E7,0xA552E8,0xAD38BC,0xAD38BD,0xAD38E0,
	};

	// clang-format on

	constexpr std::string_view modname = "Requiem.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (CPatch_REQ_ItmL3_S::Data.HasForm(a_event->book->GetFormID())) {
			auto base = CPatch_REQ_ItmL3_S::Data.GetBase(a_event->book->GetFormID()) ? CPatch_REQ_ItmL3_S::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), section::k_ItmL3S);
			return EventResult::kContinue;
		}

		if (CPatch_REQ_ItmL3_B::Data.HasForm(a_event->book->GetFormID())) {
			auto base = CPatch_REQ_ItmL3_B::Data.GetBase(a_event->book->GetFormID()) ? CPatch_REQ_ItmL3_B::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), section::k_ItmL3B);
			return EventResult::kContinue;
		}

		if (CPatch_REQ_ItmL4::Data.HasForm(a_event->book->GetFormID())) {
			auto base = CPatch_REQ_ItmL4::Data.GetBase(a_event->book->GetFormID()) ? CPatch_REQ_ItmL4::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
			CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), section::k_ItmL4);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CPatch_REQ_ItmL1_A::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_REQ_ItmL1_A::Data.GetBase(a_event->baseObj) ? CPatch_REQ_ItmL1_A::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, section::k_ItmL1A);
			return EventResult::kContinue;
		}

		if (CPatch_REQ_ItmL1_M::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_REQ_ItmL1_M::Data.GetBase(a_event->baseObj) ? CPatch_REQ_ItmL1_M::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, section::k_ItmL1M);
			return EventResult::kContinue;
		}

		if (CPatch_REQ_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_REQ_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_REQ_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, section::k_ItmL2);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, section k_section) {

		switch (k_section)
		{
		case CPatch_REQ::k_ItmL1A: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL1_A::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_REQ_ItmL1_A::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL1_A::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL1_A::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL1_A_FormArray, CPatch_REQ_ItmL1_A::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_A_FormArray.begin(), t_pos);
			ItmL1_A_BoolArray[b_pos] = true;
			ItmL1_A_EntriesFound = std::ranges::count(ItmL1_A_BoolArray, true);
			break;
		}

		case CPatch_REQ::k_ItmL1M: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL1_M::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_REQ_ItmL1_M::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL1_M::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL1_M::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL1_M_FormArray, CPatch_REQ_ItmL1_M::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_M_FormArray.begin(), t_pos);
			ItmL1_M_BoolArray[b_pos] = true;
			ItmL1_M_EntriesFound = std::ranges::count(ItmL1_M_BoolArray, true);
			break;
		}


		case CPatch_REQ::k_ItmL2: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_REQ_ItmL2::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL2::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_REQ_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			break;
		}

		case CPatch_REQ::k_ItmL3S: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL3_S::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				if (auto* book = static_cast<RE::TESObjectBOOK*>(CPatch_REQ_ItmL3_S::Data.GetForm(a_eventID)); book && book->GetSpell()) {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
				}
				else {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, CPatch_REQ_ItmL3_S::Data.GetForm(a_eventID)->GetName());
				}
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL3_S::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL3_S::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL3_S_FormArray, CPatch_REQ_ItmL3_S::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_S_FormArray.begin(), t_pos);
			ItmL3_S_BoolArray[b_pos] = true;

			ItmL3_S_EntriesFound = std::ranges::count(ItmL3_S_BoolArray, true);
			break;
		}

		case CPatch_REQ::k_ItmL3B: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL3_B::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				if (auto* book = static_cast<RE::TESObjectBOOK*>(CPatch_REQ_ItmL3_B::Data.GetForm(a_eventID)); book && book->GetSpell()) {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
				}
				else {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, CPatch_REQ_ItmL3_B::Data.GetForm(a_eventID)->GetName());
				}
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL3_B::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL3_B::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL3_B_FormArray, CPatch_REQ_ItmL3_B::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_B_FormArray.begin(), t_pos);
			ItmL3_B_BoolArray[b_pos] = true;

			ItmL3_B_EntriesFound = std::ranges::count(ItmL3_B_BoolArray, true);
			break;
		}

		case CPatch_REQ::k_ItmL4: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_REQ_ItmL4::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
				if (auto* book = static_cast<RE::TESObjectBOOK*>(CPatch_REQ_ItmL4::Data.GetForm(a_eventID)); book && book->GetSpell()) {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
				}
				else {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, CPatch_REQ_ItmL4::Data.GetForm(a_eventID)->GetName());
				}
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_REQ_ItmL4::Data.GetAllVariations()) {
				if (CPatch_REQ_ItmL4::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL4_FormArray, CPatch_REQ_ItmL4::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL4_FormArray.begin(), t_pos);
			ItmL4_BoolArray[b_pos] = true;

			ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
			break;
		}

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_REQ_ItmL1_A::Data.CompileFormArray(CPatch_REQ::ItmL1_A, modname);
		CPatch_REQ_ItmL1_M::Data.CompileFormArray(CPatch_REQ::ItmL1_M, modname);

		CPatch_REQ_ItmL3_S::Data.CompileFormArray(CPatch_REQ::ItmL3_S, modname);
		CPatch_REQ_ItmL3_B::Data.CompileFormArray(CPatch_REQ::ItmL3_B, modname);

		CPatch_REQ_ItmL2::Data.CompileFormArray(CPatch_REQ::ItmL2, modname);
		CPatch_REQ_ItmL4::Data.CompileFormArray(CPatch_REQ::ItmL4, modname);

		CPatch_REQ_ItmL1_A::Data.MergeAsCollectable();
		CPatch_REQ_ItmL1_M::Data.MergeAsCollectable();

		CPatch_REQ_ItmL3_S::Data.MergeAsCollectable();
		CPatch_REQ_ItmL3_B::Data.MergeAsCollectable();

		CPatch_REQ_ItmL2::Data.MergeAsCollectable();
		CPatch_REQ_ItmL4::Data.MergeAsCollectable();

		CPatch_REQ_ItmL1_A::Data.Populate(ItmL1_A_NameArray, ItmL1_A_FormArray, ItmL1_A_BoolArray, ItmL1_A_TextArray);
		CPatch_REQ_ItmL1_M::Data.Populate(ItmL1_M_NameArray, ItmL1_M_FormArray, ItmL1_M_BoolArray, ItmL1_M_TextArray);

		CPatch_REQ_ItmL3_S::Data.Populate(ItmL3_S_NameArray, ItmL3_S_FormArray, ItmL3_S_BoolArray, ItmL3_S_TextArray, false, 1);
		CPatch_REQ_ItmL3_B::Data.Populate(ItmL3_B_NameArray, ItmL3_B_FormArray, ItmL3_B_BoolArray, ItmL3_B_TextArray, false, 1);

		CPatch_REQ_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_REQ_ItmL4::Data.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray, false, 1);

		ItmL1_A_EntriesTotal = ItmL1_A_FormArray.size();
		ItmL1_A_EntriesFound = std::ranges::count(ItmL1_A_BoolArray, true);

		ItmL1_M_EntriesTotal = ItmL1_M_FormArray.size();
		ItmL1_M_EntriesFound = std::ranges::count(ItmL1_M_BoolArray, true);

		ItmL3_S_EntriesTotal = ItmL3_S_FormArray.size();
		ItmL3_S_EntriesFound = std::ranges::count(ItmL3_S_BoolArray, true);

		ItmL3_B_EntriesTotal = ItmL3_B_FormArray.size();
		ItmL3_B_EntriesFound = std::ranges::count(ItmL3_B_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : ItmL1_A_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageRequiem1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto& name : ItmL1_M_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageRequiem1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_S_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_S_NameArray[i], "$MCMPageRequiem2", FrameworkAPI::GetBookCategoryType(ItmL3_S_FormArray[i])));
		}
		for (auto i = 0; i < ItmL3_B_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_B_NameArray[i], "$MCMPageRequiem2", FrameworkAPI::GetBookCategoryType(ItmL3_B_FormArray[i])));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_NameArray[i], "$MCMPageRequiem3", FrameworkAPI::GetBookCategoryType(ItmL4_FormArray[i])));
		}

		for (auto& name : ItmL2_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageRequiem4", std::to_underlying(EntryCategory::kItem)));
		}

	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < ItmL1_A_FormArray.size(); i++) {
			ItmL1_A_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_A_FormArray[i], &CPatch_REQ_ItmL1_A::Data);
		}

		for (auto i = 0; i < ItmL1_M_FormArray.size(); i++) {
			ItmL1_M_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_M_FormArray[i], &CPatch_REQ_ItmL1_M::Data);
		}

		for (auto i = 0; i < ItmL3_S_FormArray.size(); i++) {
			ItmL3_S_BoolArray[i] = FrameworkAPI::IsBookKnown(ItmL3_S_FormArray[i]);
		}

		for (auto i = 0; i < ItmL3_B_FormArray.size(); i++) {
			ItmL3_B_BoolArray[i] = FrameworkAPI::IsBookKnown(ItmL3_B_FormArray[i]);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &CPatch_REQ_ItmL2::Data);
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			ItmL4_BoolArray[i] = FrameworkAPI::IsBookKnown(ItmL4_FormArray[i]);
		}

		ItmL1_A_EntriesTotal = ItmL1_A_FormArray.size();
		ItmL1_A_EntriesFound = std::ranges::count(ItmL1_A_BoolArray, true);

		ItmL1_M_EntriesTotal = ItmL1_M_FormArray.size();
		ItmL1_M_EntriesFound = std::ranges::count(ItmL1_M_BoolArray, true);

		ItmL3_S_EntriesTotal = ItmL3_S_FormArray.size();
		ItmL3_S_EntriesFound = std::ranges::count(ItmL3_S_BoolArray, true);

		ItmL3_B_EntriesTotal = ItmL3_B_FormArray.size();
		ItmL3_B_EntriesFound = std::ranges::count(ItmL3_B_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
	}
}