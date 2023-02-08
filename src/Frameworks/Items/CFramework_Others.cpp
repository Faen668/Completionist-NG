#include "CFramework_Others.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Others {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray VC_Forms_SK = {
	0x0AB375,0x05AF48,0x0AB7BB,0x07C260,0x0B634C,0x08CDFA,
	0x0ED417,0x039647,0x04B56C,0x0663D7,
	};

	constexpr Serialization::FormArray VC_Forms_DB = {
	0x01CAC0,0x01CAC1,
	};

	constexpr Serialization::FormArray VM_Forms_SK = {
	0x0061CC1,0x0061CD6,0x0061CB9,0x0061C8B,0x0061CA5,0x0061CC2,0x0061CC0,
	0x0061CC9,0x0061CAB,0x0061CCA,
	};

	constexpr Serialization::FormArray VM_Forms_DB = {
	0x0240FE,0x0240FF,0x024037,
	};

	constexpr Serialization::Variation VM_FormV_DB[] = {
	{ 0x039FA1, { 0x039FA2, 0x039FA3, 0x039D2B, 0x039D2E, 0x039D2F } },
	};

	constexpr Serialization::FormArray Forms_PC = {
	0
	};

	constexpr Serialization::FormArray Forms_PM = {
	0
	};

	constexpr Serialization::FormArray Forms_SB = {
	0x07FB5D,0x07FB56,0x07F908,0x07F927,0x07F901,0x07FB4F,0x07FB58,
	0x07FB63,0x07F925,0x07F909,0x07FB45,0x07FB64,0x07FB55,0x07F926,
	0x07FB3A,0x07FB65,0x07FB51,0x07FB54,0x07F956,0x07F95D,0x07F923,
	0x07F965,0x07F962,0x07FB4A,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(GetSingleton()));
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Activated ) ---------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || !CFramework_Others_SB::Data.HasForm(a_event->objectActivated.get()->GetFormID())) { return EventResult::kContinue; }

		if (auto t_pos = std::ranges::find(SB_FormArray, a_event->objectActivated.get()); t_pos == SB_FormArray.end()) { return EventResult::kContinue; }

		CHandler::ProcessFoundForm(a_event->objectActivated.get());
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* VC_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!VC_event || VC_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CFramework_Others_VC::Data.HasForm(VC_event->baseObj)) {
			auto base = CFramework_Others_VC::Data.GetBase(VC_event->baseObj) ? CFramework_Others_VC::Data.GetBase(VC_event->baseObj) : VC_event->baseObj;
			CHandler::ProcessFoundForm(base, VC_event->baseObj, "CFramework_Others_VC");
			return EventResult::kContinue;
		}

		if (CFramework_Others_PC::Data.HasForm(VC_event->baseObj)) {
			auto base = CFramework_Others_PC::Data.GetBase(VC_event->baseObj) ? CFramework_Others_PC::Data.GetBase(VC_event->baseObj) : VC_event->baseObj;
			CHandler::ProcessFoundForm(base, VC_event->baseObj, "CFramework_Others_PC");
			return EventResult::kContinue;
		}

		if (CFramework_Others_VM::Data.HasForm(VC_event->baseObj)) {
			auto base = CFramework_Others_VM::Data.GetBase(VC_event->baseObj) ? CFramework_Others_VM::Data.GetBase(VC_event->baseObj) : VC_event->baseObj;
			CHandler::ProcessFoundForm(base, VC_event->baseObj, "CFramework_Others_VM");
			return EventResult::kContinue;
		}

		if (CFramework_Others_PM::Data.HasForm(VC_event->baseObj)) {
			auto base = CFramework_Others_PM::Data.GetBase(VC_event->baseObj) ? CFramework_Others_PM::Data.GetBase(VC_event->baseObj) : VC_event->baseObj;
			CHandler::ProcessFoundForm(base, VC_event->baseObj, "CFramework_Others_PM");
			return EventResult::kContinue;
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::TESObjectREFR* a_object) {

		auto t_pos = std::ranges::find(SB_FormArray, a_object);
		auto b_pos = std::distance(SB_FormArray.begin(), t_pos);

		if (!FoundItemData_NoShow.HasForm(a_object->GetFormID())) {
			auto msg = fmt::format("{:s}!"sv, CVariables::V_NotificationTextBarenziah);
			FrameworkAPI::SendNotification(msg, "NotifySpecial");
		}

		FoundItemData_NoShow.AddForm(a_object->GetFormID());
		SB_BoolArray[b_pos] = true;

		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID VC_baseID, RE::FormID VC_eventID, std::string VC_variable) {

		if (VC_variable == "CFramework_Others_VC") {

			if (!FoundItemData.HasForm(VC_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CFramework_Others_VC::Data.GetForm(VC_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(VC_baseID);
			for (auto var : CFramework_Others_VC::Data.GetAllVariations()) {
				if (CFramework_Others_VC::Data.GetBase(var) == VC_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(VC_FormArray, CFramework_Others_VC::Data.GetForm(VC_baseID));
			auto b_pos = std::distance(VC_FormArray.begin(), t_pos);

			VC_BoolArray[b_pos] = true;
			VC_EntriesFound = std::ranges::count(VC_BoolArray, true);
			return;
		}

		if (VC_variable == "CFramework_Others_PC") {

			if (!FoundItemData.HasForm(VC_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CFramework_Others_PC::Data.GetForm(VC_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(VC_baseID);
			for (auto var : CFramework_Others_PC::Data.GetAllVariations()) {
				if (CFramework_Others_PC::Data.GetBase(var) == VC_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(PC_FormArray, CFramework_Others_PC::Data.GetForm(VC_baseID));
			auto b_pos = std::distance(PC_FormArray.begin(), t_pos);

			PC_BoolArray[b_pos] = true;
			PC_EntriesFound = std::ranges::count(PC_BoolArray, true);
			return;
		}

		if (VC_variable == "CFramework_Others_VM") {

			if (!FoundItemData.HasForm(VC_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CFramework_Others_VM::Data.GetForm(VC_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(VC_baseID);
			for (auto var : CFramework_Others_VM::Data.GetAllVariations()) {
				if (CFramework_Others_VM::Data.GetBase(var) == VC_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(VM_FormArray, CFramework_Others_VM::Data.GetForm(VC_baseID));
			auto b_pos = std::distance(VM_FormArray.begin(), t_pos);

			VM_BoolArray[b_pos] = true;
			VM_EntriesFound = std::ranges::count(VM_BoolArray, true);
			return;
		}

		if (VC_variable == "CFramework_Others_PM") {

			if (!FoundItemData.HasForm(VC_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CFramework_Others_PM::Data.GetForm(VC_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(VC_baseID);
			for (auto var : CFramework_Others_PM::Data.GetAllVariations()) {
				if (CFramework_Others_PM::Data.GetBase(var) == VC_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(PM_FormArray, CFramework_Others_PM::Data.GetForm(VC_baseID));
			auto b_pos = std::distance(PM_FormArray.begin(), t_pos);

			PM_BoolArray[b_pos] = true;
			PM_EntriesFound = std::ranges::count(PM_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled("Helgen Reborn.esp"); Mod) {
			CFramework_Others_PC::Data.AddForm(0x11035C, "Helgen Reborn.esp");
		}

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled("Wyrmstooth.esp"); Mod) {
			CFramework_Others_PC::Data.AddForm(0x27978A, "Wyrmstooth.esp");
			CFramework_Others_PM::Data.AddForm(0x4F2E1F, "Wyrmstooth.esp");
		}

		CFramework_Others_PC::Data.CompileFormArray(CFramework_Others::Forms_PC, "");
		CFramework_Others_VC::Data.CompileFormArray(CFramework_Others::VC_Forms_SK, "Skyrim.esm");
		CFramework_Others_VC::Data.CompileFormArray(CFramework_Others::VC_Forms_DB, "Dragonborn.esm");
		CFramework_Others_VC::Data.MergeAsCollectable();
		CFramework_Others_PC::Data.MergeAsCollectable();

		CFramework_Others_PM::Data.CompileFormArray(CFramework_Others::Forms_PM, "");
		CFramework_Others_VM::Data.CompileFormArray(CFramework_Others::VM_Forms_SK, "Skyrim.esm");
		CFramework_Others_VM::Data.CompileFormArray(CFramework_Others::VM_Forms_DB, "Dragonborn.esm");
		CFramework_Others_VM::Data.CompileVariation(CFramework_Others::VM_FormV_DB, "Dragonborn.esm");
		CFramework_Others_VM::Data.MergeAsCollectable();
		CFramework_Others_PM::Data.MergeAsCollectable();

		CFramework_Others_SB::Data.CompileFormArray(CFramework_Others::Forms_SB, "Skyrim.esm");
		BuildBaranziahArrays();

		CFramework_Others_VC::Data.Populate(VC_NameArray, VC_FormArray, VC_BoolArray, VC_TextArray);
		VC_EntriesTotal = VC_FormArray.size();
		VC_EntriesFound = std::ranges::count(VC_BoolArray, true);

		CFramework_Others_PC::Data.Populate(PC_NameArray, PC_FormArray, PC_BoolArray, PC_TextArray);
		PC_EntriesTotal = PC_FormArray.size();
		PC_EntriesFound = std::ranges::count(PC_BoolArray, true);

		CFramework_Others_VM::Data.Populate(VM_NameArray, VM_FormArray, VM_BoolArray, VM_TextArray);
		VM_EntriesTotal = VM_FormArray.size();
		VM_EntriesFound = std::ranges::count(VM_BoolArray, true);

		CFramework_Others_PM::Data.Populate(PM_NameArray, PM_FormArray, PM_BoolArray, PM_TextArray);
		PM_EntriesTotal = PM_FormArray.size();
		PM_EntriesFound = std::ranges::count(PM_BoolArray, true);

		SB_EntriesTotal = SB_FormArray.size();
		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < VC_FormArray.size(); i++) {
			VC_BoolArray[i] = FrameworkAPI::IsItemKnown(VC_FormArray[i], &CFramework_Others_VC::Data);
		}

		for (auto i = 0; i < PC_FormArray.size(); i++) {
			PC_BoolArray[i] = FrameworkAPI::IsItemKnown(PC_FormArray[i], &CFramework_Others_PC::Data);
		}

		for (auto i = 0; i < VM_FormArray.size(); i++) {
			VM_BoolArray[i] = FrameworkAPI::IsItemKnown(VM_FormArray[i], &CFramework_Others_VM::Data);
		}

		for (auto i = 0; i < PM_FormArray.size(); i++) {
			PM_BoolArray[i] = FrameworkAPI::IsItemKnown(PM_FormArray[i], &CFramework_Others_PM::Data);
		}

		for (auto i = 0; i < SB_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(SB_FormArray[i]->GetFormID())) {
				SB_BoolArray[i] = true;
			}
		}

		VC_EntriesTotal = VC_FormArray.size();
		VC_EntriesFound = std::ranges::count(VC_BoolArray, true);

		PC_EntriesTotal = PC_FormArray.size();
		PC_EntriesFound = std::ranges::count(PC_BoolArray, true);

		VM_EntriesTotal = VM_FormArray.size();
		VM_EntriesFound = std::ranges::count(VM_BoolArray, true);

		PM_EntriesTotal = PM_FormArray.size();
		PM_EntriesFound = std::ranges::count(PM_BoolArray, true);

		SB_EntriesTotal = SB_FormArray.size();
		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	void CHandler::BuildBaranziahArrays() {

		SB_FormArray.clear();
		SB_NameArray.clear();
		SB_TextArray.clear();
		SB_BoolArray.clear();

		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB5D));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB56));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F908));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F927));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F901));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB4F));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB58));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB63));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F925));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F909));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB45));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB64));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB55));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F926));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB3A));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB65));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB51));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB54));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F956));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F95D));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F923));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F965));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07F962));
		SB_FormArray.push_back(RE::TESForm::LookupByID(0x07FB4A));

		SB_NameArray.push_back("$BarenziahList00");
		SB_NameArray.push_back("$BarenziahList01");
		SB_NameArray.push_back("$BarenziahList02");
		SB_NameArray.push_back("$BarenziahList03");
		SB_NameArray.push_back("$BarenziahList04");
		SB_NameArray.push_back("$BarenziahList05");
		SB_NameArray.push_back("$BarenziahList06");
		SB_NameArray.push_back("$BarenziahList07");
		SB_NameArray.push_back("$BarenziahList08");
		SB_NameArray.push_back("$BarenziahList09");
		SB_NameArray.push_back("$BarenziahList10");
		SB_NameArray.push_back("$BarenziahList11");
		SB_NameArray.push_back("$BarenziahList12");
		SB_NameArray.push_back("$BarenziahList13");
		SB_NameArray.push_back("$BarenziahList14");
		SB_NameArray.push_back("$BarenziahList15");
		SB_NameArray.push_back("$BarenziahList16");
		SB_NameArray.push_back("$BarenziahList17");
		SB_NameArray.push_back("$BarenziahList18");
		SB_NameArray.push_back("$BarenziahList19");
		SB_NameArray.push_back("$BarenziahList20");
		SB_NameArray.push_back("$BarenziahList21");
		SB_NameArray.push_back("$BarenziahList22");
		SB_NameArray.push_back("$BarenziahList23");

		SB_TextArray.push_back("$BarenziahHighlight00");
		SB_TextArray.push_back("$BarenziahHighlight01");
		SB_TextArray.push_back("$BarenziahHighlight02");
		SB_TextArray.push_back("$BarenziahHighlight03");
		SB_TextArray.push_back("$BarenziahHighlight04");
		SB_TextArray.push_back("$BarenziahHighlight05");
		SB_TextArray.push_back("$BarenziahHighlight06");
		SB_TextArray.push_back("$BarenziahHighlight07");
		SB_TextArray.push_back("$BarenziahHighlight08");
		SB_TextArray.push_back("$BarenziahHighlight09");
		SB_TextArray.push_back("$BarenziahHighlight10");
		SB_TextArray.push_back("$BarenziahHighlight11");
		SB_TextArray.push_back("$BarenziahHighlight12");
		SB_TextArray.push_back("$BarenziahHighlight13");
		SB_TextArray.push_back("$BarenziahHighlight14");
		SB_TextArray.push_back("$BarenziahHighlight15");
		SB_TextArray.push_back("$BarenziahHighlight16");
		SB_TextArray.push_back("$BarenziahHighlight17");
		SB_TextArray.push_back("$BarenziahHighlight18");
		SB_TextArray.push_back("$BarenziahHighlight19");
		SB_TextArray.push_back("$BarenziahHighlight20");
		SB_TextArray.push_back("$BarenziahHighlight21");
		SB_TextArray.push_back("$BarenziahHighlight22");
		SB_TextArray.push_back("$BarenziahHighlight23");

		SB_NameArray.resize(SB_FormArray.size());
		SB_TextArray.resize(SB_FormArray.size());
		SB_BoolArray.resize(SB_FormArray.size());
	}
}