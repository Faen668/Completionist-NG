#include "CFramework_Blessings.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Blessings {
	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray DS_Forms_SK = {
	0x0D2331,0x0D2334,0x0D2330,0x0D2336,0x0D2332,0x0D232E,0x0D2337,0x0D2339,
	0x0D2335,0x0D2333,0x0D232F,0x0D2338,0x0D232D,
	};

	constexpr Serialization::FormArray VS_Forms_SK = {
	0x0D9883, //Akatosh
	0x071854, //Arkay
	0x0D9881, //Dibella
	0x0D9885, //Julianos
	0x0D987F, //Kynareth
	0x0D9887, //Mara
	0x10E8B0, //Nocturnal
	0x0D987D, //Stendarr
	0x100780, //Talos
	0x0D987B, //Zenithar
	};

	constexpr Serialization::FormArray VS_Forms_DG = {
	0x00C86B, //Auriel
	};

	constexpr Serialization::FormArray VS_Forms_DB = {
	0x03A484, //Azura
	0x039E34, //Boethiah
	0x03A481, //Mephala
	};

	constexpr Serialization::FormArray WS_Forms = {
	0x0E476B,0x107FC5,0x107FC6,0x0C60C4,0x0C6099,0x107FC7,0x0FDD3E,
	0x107FC8,0x09D7FA,0x107FCB,0x1B9652,0x1B9653,0x1B9650,0x1B9651,
	0x3D283F,0x1B964F,0x107FCC,0x2EE923,0x3F5FA1,0x107FC4,0x1B964E,
	0x1B964B,0x107FCE,0x1B964D,0x26FD37,0x1C89C7,0x1B964C,0x0986CA
	};

	constexpr Serialization::Variation WS_FormV[] = {
	{ 0x0C60C5, { 0x3D283B, } },			//Sanguine
	{ 0x2D0116, { 0x2E4677, } },			//Jephre
	{ 0x2E464B, { 0x065C05, } },			//Magnus
	{ 0x107FCD, { 0x2E464E, } },			//Trinimac
	{ 0x0F8C25, { 0x2E464F, } },			//Xarxes	
	{ 0x2E464C, { 0x0935B3, } },			//Phynaster
	{ 0x2E46A7, { 0x019B85, 0x2E464D } },	//Syrabane
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(GetSingleton()));
		if (auto* input = RE::BSInputDeviceManager::GetSingleton()) {
			input->AddEventSink(static_cast<RE::BSTEventSink<RE::InputEvent*>*>(GetSingleton()));
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( Survival Mode Shrines ) -----
	//---------------------------------------------------

	// Survival's TempleBlessing perk replaces shrine activation so no activate event fires,
	// count it from the Activate button and the crosshair target instead
	EventResult CHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {

		if (!a_event) { return EventResult::kContinue; }

		for (auto* event = *a_event; event; event = event->next) {
			const auto* button = event->AsButtonEvent();
			if (!button || !button->IsDown() || button->QUserEvent() != RE::UserEvents::GetSingleton()->activate) {
				continue;
			}

			auto* ui = RE::UI::GetSingleton();
			if (!ui || ui->GameIsPaused() || ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {
				return EventResult::kContinue;
			}
			static auto* survival = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Survival_ModeEnabledShared");
			if (!survival || survival->value < 1.0f) {
				return EventResult::kContinue;
			}

			const auto target = RE::CrosshairPickData::GetSingleton()->GetActiveTarget().get();
			auto*      base = target ? target->GetBaseObject() : nullptr;
			if (!base) {
				return EventResult::kContinue;
			}
			const auto formID = base->GetFormID();
			if (CFramework_Blessings_VS::Data.HasForm(formID) || CFramework_Blessings_WS::Data.HasForm(formID)) {
				INFO("Blessings: Survival Mode shrine activated, {} ({:08X})", base->GetName(), formID);
				SKSE::GetTaskInterface()->AddTask([formID]() {
					ActivateShrine(RE::TESForm::LookupByID(formID));
				});
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::ActivateShrine(RE::TESForm* a_shrine) {

		if (!a_shrine) { return; }

		const auto formID = a_shrine->GetFormID();

		if (CFramework_Blessings_VS::Data.HasForm(formID)) {
			auto base = CFramework_Blessings_VS::Data.GetBase(formID) ? CFramework_Blessings_VS::Data.GetBase(formID) : formID;
			CHandler::ProcessFoundForm(base, formID, Section::kSection_VS);
			return;
		}

		if (CFramework_Blessings_WS::Data.HasForm(a_shrine->GetFormID())) {
			auto base = CFramework_Blessings_WS::Data.GetBase(formID) ? CFramework_Blessings_WS::Data.GetBase(formID) : formID;
			CHandler::ProcessFoundForm(base, formID, Section::kSection_WS);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Activated ) ---------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated) { return EventResult::kContinue; }

		if (CFramework_Blessings_DS::Data.HasForm(a_event->objectActivated->GetBaseObject()->GetFormID())) {
			auto formID = a_event->objectActivated->GetBaseObject()->GetFormID();
			auto base = CFramework_Blessings_DS::Data.GetBase(formID) ? CFramework_Blessings_DS::Data.GetBase(formID) : formID;
			CHandler::ProcessFoundForm(base, formID, Section::kSection_DS);
			return EventResult::kContinue;
		}

		if (CFramework_Blessings_VS::Data.HasForm(a_event->objectActivated->GetBaseObject()->GetFormID())) {
			auto formID = a_event->objectActivated->GetBaseObject()->GetFormID();
			auto base = CFramework_Blessings_VS::Data.GetBase(formID) ? CFramework_Blessings_VS::Data.GetBase(formID) : formID;
			CHandler::ProcessFoundForm(base, formID, Section::kSection_VS);
			return EventResult::kContinue;
		}

		if (CFramework_Blessings_WS::Data.HasForm(a_event->objectActivated->GetBaseObject()->GetFormID())) {
			auto formID = a_event->objectActivated->GetBaseObject()->GetFormID();
			auto base = CFramework_Blessings_WS::Data.GetBase(formID) ? CFramework_Blessings_WS::Data.GetBase(formID) : formID;
			CHandler::ProcessFoundForm(base, formID, Section::kSection_WS);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, Section a_section) {

		INFO("Blessings: shrine {:08X} (base {:08X}) counted{}", a_eventID, a_baseID, FoundItemData_NoShow.HasForm(a_baseID) ? ", already found" : "");

		if (!FoundItemData_NoShow.HasForm(a_baseID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, RE::TESForm::LookupByID(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifySpecial");
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, RE::TESForm::LookupByID(a_eventID)->GetName());
		}

		FoundItemData_NoShow.AddForm(a_baseID);

		switch (a_section) {

		case CFramework_Blessings::Section::kSection_DS:
		{
			for (auto var : CFramework_Blessings_DS::Data.GetAllVariations()) {
				if (CFramework_Blessings_DS::Data.GetBase(var) == a_baseID) {
					FoundItemData_NoShow.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(DS_FormArray, CFramework_Blessings_DS::Data.GetForm(a_baseID));
			auto b_pos = std::distance(DS_FormArray.begin(), t_pos);
			DS_BoolArray[b_pos] = true;
			break;
		}

		case CFramework_Blessings::Section::kSection_VS:
		{
			for (auto var : CFramework_Blessings_VS::Data.GetAllVariations()) {
				if (CFramework_Blessings_VS::Data.GetBase(var) == a_baseID) {
					FoundItemData_NoShow.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(VS_FormArray, CFramework_Blessings_VS::Data.GetForm(a_baseID));
			auto b_pos = std::distance(VS_FormArray.begin(), t_pos);
			VS_BoolArray[b_pos] = true;
			break;
		}

		case CFramework_Blessings::Section::kSection_WS:
		{
			for (auto var : CFramework_Blessings_WS::Data.GetAllVariations()) {
				if (CFramework_Blessings_WS::Data.GetBase(var) == a_baseID) {
					FoundItemData_NoShow.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(WS_FormArray, CFramework_Blessings_WS::Data.GetForm(a_baseID));
			auto b_pos = std::distance(WS_FormArray.begin(), t_pos);
			WS_BoolArray[b_pos] = true;
			break;
		}

		default:
			break;
		}
		DS_EntriesFound = std::ranges::count(DS_BoolArray, true);
		VS_EntriesFound = std::ranges::count(VS_BoolArray, true);
		WS_EntriesFound = std::ranges::count(WS_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		if (Serialization::CompletionistData::IsModInstalled("Wintersun - Faiths of Skyrim.esp"))
		{
			CFramework_Blessings_VS::Data.AddForm(0x03A484, "Dragonborn.esm", 0x07F163, "Wintersun - Faiths of Skyrim.esp"); // Azura
			CFramework_Blessings_VS::Data.AddForm(0x03A484, "Dragonborn.esm", 0x2D00B0, "Wintersun - Faiths of Skyrim.esp"); // Azura
			CFramework_Blessings_VS::Data.AddForm(0x039E34, "Dragonborn.esm", 0x09D7ED, "Wintersun - Faiths of Skyrim.esp"); // Boethiah

			CFramework_Blessings_WS::Data.CompileFormArray(CFramework_Blessings::WS_Forms, "Wintersun - Faiths of Skyrim.esp");
			CFramework_Blessings_WS::Data.CompileVariation(CFramework_Blessings::WS_FormV, "Wintersun - Faiths of Skyrim.esp");
			CFramework_Blessings_WS::Data.Populate(WS_NameArray, WS_FormArray, WS_BoolArray, WS_TextArray);
		}

		CFramework_Blessings_DS::Data.CompileFormArray(CFramework_Blessings::DS_Forms_SK, "Skyrim.esm");
		CFramework_Blessings_VS::Data.CompileFormArray(CFramework_Blessings::VS_Forms_SK, "Skyrim.esm");
		CFramework_Blessings_VS::Data.CompileFormArray(CFramework_Blessings::VS_Forms_DG, "Dawnguard.esm");
		CFramework_Blessings_VS::Data.CompileFormArray(CFramework_Blessings::VS_Forms_DB, "Dragonborn.esm");

		CFramework_Blessings_DS::Data.Populate(DS_NameArray, DS_FormArray, DS_BoolArray, DS_TextArray);
		CFramework_Blessings_VS::Data.Populate(VS_NameArray, VS_FormArray, VS_BoolArray, VS_TextArray);
		
		DS_EntriesTotal = DS_FormArray.size();
		DS_EntriesFound = std::ranges::count(DS_BoolArray, true);

		VS_EntriesTotal = VS_FormArray.size();
		VS_EntriesFound = std::ranges::count(VS_BoolArray, true);

		WS_EntriesTotal = WS_FormArray.size();
		WS_EntriesFound = std::ranges::count(WS_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < DS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(DS_FormArray[i], DS_NameArray[i], "$MCMPageDoomstone", std::to_underlying(EntryCategory::kStones)));
		}
		for (auto i = 0; i < VS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(VS_FormArray[i], VS_NameArray[i], "$MCMPageShrines", std::to_underlying(EntryCategory::kShrine)));
		}
		for (auto i = 0; i < WS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(WS_FormArray[i], WS_NameArray[i], "$MCMPageShrines", std::to_underlying(EntryCategory::kShrine)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < DS_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(DS_FormArray[i]->GetFormID())) {
				DS_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < VS_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(VS_FormArray[i]->GetFormID())) {
				VS_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < WS_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(WS_FormArray[i]->GetFormID())) {
				WS_BoolArray[i] = true;
			}
		}

		DS_EntriesTotal = DS_FormArray.size();
		DS_EntriesFound = std::ranges::count(DS_BoolArray, true);

		VS_EntriesTotal = VS_FormArray.size();
		VS_EntriesFound = std::ranges::count(VS_BoolArray, true);

		WS_EntriesTotal = WS_FormArray.size();
		WS_EntriesFound = std::ranges::count(WS_BoolArray, true);
	}
}