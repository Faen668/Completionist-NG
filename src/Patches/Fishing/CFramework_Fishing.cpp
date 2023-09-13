#include "Serialization.hpp"
#include "CFramework_Fishing.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm
#undef GetObject

namespace CPatch_FSH {
	using namespace Serialization;
	using namespace CFramework_Master;

	constexpr FormArray I_Forms = {
	0x000ED3,0x000A6F,0x000C66,0x000B2B,0x000B29,0x000B94,0x0EA5CC,
	0x0008E3,0x000BC0,0x000BA0,0x000C07,0x000F07,0x000B2A,
	};

	constexpr Variation I_Varia[] = {
	{ 0x07AED6, { 0x0009D9,0x04D05E } },
	};

	constexpr FormArray B_Forms = {
	0x000E7F,0x0009AE,0x0009AF,0x000ABA,0x000ABC,0x070CCC,0x070CCD,
	0x070CCE,0x070CCF,0x070CD0,0x070CD1,0x070CD2,0x070CD3,0x070CD4,
	0x070CD5,0x0008E5,
	};

	constexpr FormArray A_Forms = {
	0x000F7E,0x000F64,0x000F8C,0x000F79,0x000F82,0x000F89,0x000F87,0x000F8A,
	0x000F8B,0x000F81,0x000F8D,0x000F80,0x000F78,0x000F7F,0x000F8E,0x000F83,
	0x000F7B,0x000F8F,0x000F7D,0x000F7C,0x000F7A,

	};

	constexpr FormArray C_Forms = {
	0x000F86,0x000F88,
	};

	constexpr FormArray L_Forms = {
	0x000F5A,0x000F5F,0x000F57,0x000F5E,0x000F61,0x000F5D,0x000F58,0x000F59,
	0x000F60,0x000F5B,
	};

	constexpr FormArray S_Forms = {
	0x000F5C,0x000F6D,0x000F74,0x000F6C,0x000F70,0x000F6B,0x000F72,0x000F62,
	0x000F66,0x000F77,0x000F69,0x000F67,0x000F6E,0x000F85,0x000F71,0x000F68,
	0x000F65,0x000F63,0x000F84,0x000F76,0x000F75,0x000F6A,0x000F73,0x000F6F,
	};

	constexpr std::string_view modname = "ccbgssse001-fish.esm";
	constexpr std::string_view modnameCACO = "Complete Alchemy & Cooking Overhaul.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!CompletionistData::IsModInstalled(modname)) { 
			RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FishingEnabled")->value = 0;
			return; 
		}

		RodList = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(0x000843, "ccbgssse001-fish.esm");
		GlobalV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FishingRods");
		ContMap = RE::ControlMap::GetSingleton();

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();

		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		FrameworkAPI::AddMapMarkerDiscovery(ProcessHookedMarker);
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
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESEquipEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();

		// Items ---

		CPatch_FSH_I::Data.CompileFormArray(CPatch_FSH::I_Forms, "ccbgssse001-fish.esm");
		CPatch_FSH_I::Data.CompileVariation(CPatch_FSH::I_Varia, "ccbgssse001-fish.esm");
		CPatch_FSH_I::Data.MergeAsCollectable();

		CPatch_FSH_I::Data.Populate(I_NameArray, I_FormArray, I_BoolArray, I_TextArray);
		I_EntriesTotal = I_FormArray.size();
		I_EntriesFound = std::ranges::count(I_BoolArray, true);

		// Books ---

		CPatch_FSH_B::Data.CompileFormArray(CPatch_FSH::B_Forms, "ccbgssse001-fish.esm");
		CPatch_FSH_B::Data.MergeAsCollectable();

		CPatch_FSH_B::Data.Populate(B_NameArray, B_FormArray, B_BoolArray, B_TextArray, false, 1);
		B_EntriesTotal = B_FormArray.size();
		B_EntriesFound = std::ranges::count(B_BoolArray, true);

		// Arctic Markers ---

		CPatch_FSH_A::Data.CompileFormArray(CPatch_FSH::A_Forms, "Completionist.esp");
		CPatch_FSH_A::Data.Populate(A_NameArray, A_FormArray, A_BoolArray, A_TextArray, false, 2);

		A_EntriesTotal = A_FormArray.size();
		A_EntriesFound = std::ranges::count(A_BoolArray, true);

		// Caves Markers ---

		CPatch_FSH_C::Data.CompileFormArray(CPatch_FSH::C_Forms, "Completionist.esp");
		CPatch_FSH_C::Data.Populate(C_NameArray, C_FormArray, C_BoolArray, C_TextArray, false, 2);

		C_EntriesTotal = C_FormArray.size();
		C_EntriesFound = std::ranges::count(C_BoolArray, true);

		// Lakes Markers ---

		CPatch_FSH_L::Data.CompileFormArray(CPatch_FSH::L_Forms, "Completionist.esp");
		CPatch_FSH_L::Data.Populate(L_NameArray, L_FormArray, L_BoolArray, L_TextArray, false, 2);

		L_EntriesTotal = L_FormArray.size();
		L_EntriesFound = std::ranges::count(L_BoolArray, true);

		// Streams Markers ---

		CPatch_FSH_S::Data.CompileFormArray(CPatch_FSH::S_Forms, "Completionist.esp");
		CPatch_FSH_S::Data.Populate(S_NameArray, S_FormArray, S_BoolArray, S_TextArray, false, 2);

		S_EntriesTotal = S_FormArray.size();
		S_EntriesFound = std::ranges::count(S_BoolArray, true);

		// Fish ---
		std::string pName = "ccbgssse001-fish.esm";
		std::string sName = "Skyrim.esm";

		std::string Rod0 = "";
		std::string Rod1 = "Argonian Fishing Rod";
		std::string Rod2 = "Alik'ri Fishing Rod";
	
		tempfsh.clear();
		temprod.clear();
		temploc.clear();

		BuildFishArrays(handler->LookupForm(0x106E1B, sName), Rod0, "lakes and arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x000890, pName), Rod1, "arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x0008F0, pName), Rod0, "arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x0008A4, pName), Rod0, "arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x0008A3, pName), Rod0, "arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x0008EE, pName), Rod0, "lakes during any weather");
		BuildFishArrays(handler->LookupForm(0x0008A2, pName), Rod0, "arctic waters during any weather");
		BuildFishArrays(handler->LookupForm(0x0008EC, pName), Rod0, "lakes and streams during rainy weather");
		BuildFishArrays(handler->LookupForm(0x00089C, pName), Rod0, "streams during any weather and lakes during clear weather");
		BuildFishArrays(handler->LookupForm(0x0008A1, pName), Rod1, "caves during any weather");
		BuildFishArrays(handler->LookupForm(0x000898, pName), Rod0, "lakes and streams during clear weather");
		BuildFishArrays(handler->LookupForm(0x000897, pName), Rod0, "lakes during rainy weather");
		BuildFishArrays(handler->LookupForm(0x106E19, sName), Rod0, "lakes during any weather");
		BuildFishArrays(handler->LookupForm(0x000896, pName), Rod0, "caves during any weather");
		BuildFishArrays(handler->LookupForm(0x0008EB, pName), Rod2, "lakes during any weather");
		BuildFishArrays(handler->LookupForm(0x0008A0, pName), Rod0, "caves during any weather");
		BuildFishArrays(handler->LookupForm(0x0008EF, pName), Rod0, "lakes during clear weather");
		BuildFishArrays(handler->LookupForm(0x106E18, sName), Rod0, "caves and streams during any weather");
		BuildFishArrays(handler->LookupForm(0x0008F1, pName), Rod2, "lakes during any weather");
		BuildFishArrays(handler->LookupForm(0x0008F3, pName), Rod0, "streams during any weather");
		BuildFishArrays(handler->LookupForm(0x0008ED, pName), Rod0, "lakes during rainy weather");
		BuildFishArrays(handler->LookupForm(0x106E1A, sName), Rod0, "streams during any weather");
		BuildFishArrays(handler->LookupForm(0x00089B, pName), Rod0, "lakes and streams during any weather");
		BuildFishArrays(handler->LookupForm(0x106E1C, sName), Rod0, "lakes and caves during clear weather");
		BuildFishArrays(handler->LookupForm(0x000F25, pName), Rod0, "lakes during any weather and streams during rainy weather");
		BuildFishArrays(handler->LookupForm(0x00089E, pName), Rod0, "caves during any weather");
		BuildFishArrays(handler->LookupForm(0x000891, pName), Rod0, "streams during any weather");
		BuildFishArrays(handler->LookupForm(0x00088B, pName), Rod0, "caves during any weather");

		F_FormArray.clear();
		F_NameArray.clear();
		F_BoolArray.clear();
		F_TextArray.clear();

		for (auto i = 0; i < tempfsh.size(); i++) {

			CPatch_FSH_F::Data.AddForm(tempfsh[i]->GetFormID());

			F_FormArray.push_back(tempfsh[i]);
			F_NameArray.push_back(tempfsh[i]->GetName());
			
			if (temprod[i] != Rod0) {
				F_TextArray.push_back("$AddFishingDataRod{" + F_NameArray[i] + "}{" + F_NameArray[i] + "}{" + temploc[i] + "}{" + temprod[i] + "}");
			}
			else {
				F_TextArray.push_back("$AddFishingData{" + F_NameArray[i] + "}{" + F_NameArray[i] + "}{" + temploc[i] + "}");
			}
		}

		F_NameArray.resize(F_FormArray.size());
		F_TextArray.resize(F_FormArray.size());
		F_BoolArray.resize(F_FormArray.size());

		tempfsh.clear();
		temploc.clear();
		temprod.clear();

		F_EntriesTotal = F_FormArray.size();
		F_EntriesFound = std::ranges::count(F_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : F_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishF", std::to_underlying(EntryCategory::kFish)));
		}
		for (auto& name : I_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishI", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < B_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(B_NameArray[i], "$MCMPageFishB", FrameworkAPI::GetBookCategoryType(B_FormArray[i])));
		}
		for (auto& name : A_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishL", std::to_underlying(EntryCategory::kMapM)));
		}
		for (auto& name : C_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishL", std::to_underlying(EntryCategory::kMapM)));
		}
		for (auto& name : L_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishL", std::to_underlying(EntryCategory::kMapM)));
		}
		for (auto& name : S_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFishL", std::to_underlying(EntryCategory::kMapM)));
		}
	}

	//---------------------------------------------------
	//-- Framework Events (Build FIsh Arrays ) ----------
	//---------------------------------------------------

	void CHandler::BuildFishArrays(RE::TESForm* a_form, std::string a_rod, std::string a_loc) {
		
		tempfsh.push_back(a_form);
		temprod.push_back(a_rod);
		temploc.push_back(a_loc);
	}

	//---------------------------------------------------
	//-- Framework Events (Add CACO Salmon  ) -----------
	//---------------------------------------------------

	void CHandler::AddCACOFishingForms() {

		if (!CompletionistData::IsModInstalled(modnameCACO) || !CompletionistData::IsModInstalled(modname)) { return; }

		CPatch_FSH_F::Data.AddForm(0x00089B, "ccbgssse001-fish.esm", 0xCCA147, "Update.esm");
		CPatch_FSH_F::Data.AddForm(0x00089B, "ccbgssse001-fish.esm", 0xCCA148, "Update.esm");
	}

	//---------------------------------------------------
	//-- Framework Events ( On Equip Event) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*) {

		if (!a_event || a_event->actor.get() != RE::PlayerCharacter::GetSingleton()) { return EventResult::kContinue; }

		if (!GlobalV || !RodList || !RodList->HasForm(a_event->baseObject)) { return EventResult::kContinue; }

		if (a_event->equipped) { GlobalV->value += 1; }

		if (!a_event->equipped) { GlobalV->value -= 1; }

		if (GlobalV->value < 0) { GlobalV->value = 0; }

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CPatch_FSH_F::Data.HasForm(a_event->baseObj)) {
			if (GlobalV->value > 0 && !ContMap->IsLookingControlsEnabled()) {
				auto base = CPatch_FSH_F::Data.GetBase(a_event->baseObj) ? CPatch_FSH_F::Data.GetBase(a_event->baseObj) : a_event->baseObj;
				CHandler::ProcessFoundForm(base, a_event->baseObj, "Fish");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (CPatch_FSH_I::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_FSH_I::Data.GetBase(a_event->baseObj) ? CPatch_FSH_I::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "Items");
			return EventResult::kContinue;
		}

		return EventResult::kContinue; 
	}

	//---------------------------------------------------
	//-- Framework Events ( On Book Read ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) {

		if (!a_event || !CPatch_FSH_B::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_FSH_B::Data.GetBase(a_event->book->GetFormID()) ? CPatch_FSH_B::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "Books");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {

		if (!a_event || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && CPatch_FSH_B::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_FSH_B::Data.GetBase(target) ? CPatch_FSH_B::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "Books");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME) {

			for (auto i = 0; i < A_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(A_FormArray[i], i, FishingMarkerType::kMapMa_A);
			}

			for (auto i = 0; i < C_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(C_FormArray[i], i, FishingMarkerType::kMapMa_C);
			}

			for (auto i = 0; i < L_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(L_FormArray[i], i, FishingMarkerType::kMapMa_L);
			}

			for (auto i = 0; i < S_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(S_FormArray[i], i, FishingMarkerType::kMapMa_S);
			}
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
//-- Framework Events ( Process Hooked Markers ) ----
//---------------------------------------------------

	void CHandler::ProcessHookedMarker(const char* nam)
	{
		for (auto i = 0; i < A_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, A_NameArray[i]) && !FoundItemData_NoShow.HasForm(A_FormArray[i])) {
				if (CHandler::ProcessMapMarker(A_FormArray[i], i, FishingMarkerType::kMapMa_A)) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, nam);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, nam);
					return;
				}
			}
		}

		for (auto i = 0; i < C_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, C_NameArray[i]) && !FoundItemData_NoShow.HasForm(C_FormArray[i])) {
				if (CHandler::ProcessMapMarker(C_FormArray[i], i, FishingMarkerType::kMapMa_C)) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, nam);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, nam);
					return;
				}
			}
		}

		for (auto i = 0; i < L_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, L_NameArray[i]) && !FoundItemData_NoShow.HasForm(L_FormArray[i])) {
				if (CHandler::ProcessMapMarker(L_FormArray[i], i, FishingMarkerType::kMapMa_L)) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, nam);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, nam);
					return;
				}
			}
		}

		for (auto i = 0; i < S_FormArray.size(); i++) {
			if (DKUtil::string::iequals(nam, S_NameArray[i]) && !FoundItemData_NoShow.HasForm(S_FormArray[i])) {
				if (CHandler::ProcessMapMarker(S_FormArray[i], i, FishingMarkerType::kMapMa_S)) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, nam);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, nam);
					return;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	bool CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, FishingMarkerType a_section) {

		if (a_form && FoundItemData_NoShow.HasForm(a_form)) {
			return false;
		}

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);
		auto valid = (a_marker && MarkerIsValid(a_marker));

		if (!valid) { return false; }
		FoundItemData_NoShow.AddForm(a_form);

		switch (a_section) {

		case FishingMarkerType::kMapMa_A:
			A_BoolArray[a_pos] = true;
			A_EntriesFound = std::ranges::count(A_BoolArray, true);
			break;

		case FishingMarkerType::kMapMa_C:
			C_BoolArray[a_pos] = true;
			C_EntriesFound = std::ranges::count(C_BoolArray, true);
			break;

		case FishingMarkerType::kMapMa_L:
			L_BoolArray[a_pos] = true;
			L_EntriesFound = std::ranges::count(L_BoolArray, true);
			break;

		case FishingMarkerType::kMapMa_S:
			S_BoolArray[a_pos] = true;
			S_EntriesFound = std::ranges::count(S_BoolArray, true);
			break;

		default:
			break;
		}

		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Marker Validity Check ) --
	//---------------------------------------------------

	bool CHandler::MarkerIsValid(RE::TESObjectREFR* a_marker) {

		auto* mm = CompletionistData::GetMapMarkerInternal(a_marker);
		if (!mm || !mm->mapData) { return false; }

		return mm->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_section) {

		if (a_section == "Fish") {
			if (!FoundItemData_NoShow.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FSH_F::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kFish, CPatch_FSH_F::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData_NoShow.AddForm(a_baseID);
			for (auto var : CPatch_FSH_F::Data.GetAllVariations()) {
				if (CPatch_FSH_F::Data.GetBase(var) == a_baseID) {
					FoundItemData_NoShow.AddForm(var);
				}
			}
			auto t_pos = std::ranges::find(F_FormArray, CPatch_FSH_F::Data.GetForm(a_baseID));
			auto b_pos = std::distance(F_FormArray.begin(), t_pos);
			F_BoolArray[b_pos] = true;

			F_EntriesFound = std::ranges::count(F_BoolArray, true);
			return;
		}

		if (a_section == "Items") {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FSH_I::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_FSH_I::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FSH_I::Data.GetAllVariations()) {
				if (CPatch_FSH_I::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(I_FormArray, CPatch_FSH_I::Data.GetForm(a_baseID));
			auto b_pos = std::distance(I_FormArray.begin(), t_pos);
			I_BoolArray[b_pos] = true;

			I_EntriesFound = std::ranges::count(I_BoolArray, true);
		}

		if (a_section == "Books") {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FSH_B::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");

				if (auto* book = static_cast<RE::TESObjectBOOK*>(CPatch_FSH_B::Data.GetForm(a_eventID)); book && book->GetSpell()) {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTome, book->GetName());
				}
				else {
					FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBook, CPatch_FSH_B::Data.GetForm(a_eventID)->GetName());
				}				
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FSH_B::Data.GetAllVariations()) {
				if (CPatch_FSH_B::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(B_FormArray, CPatch_FSH_B::Data.GetForm(a_baseID));
			auto b_pos = std::distance(B_FormArray.begin(), t_pos);
			B_BoolArray[b_pos] = true;

			B_EntriesFound = std::ranges::count(B_BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < F_FormArray.size(); i++) {
			F_BoolArray[i] = FoundItemData_NoShow.HasForm(F_FormArray[i]->GetFormID());
		}
		F_EntriesTotal = F_FormArray.size();
		F_EntriesFound = std::ranges::count(F_BoolArray, true);

		// ---------------------------------

		for (auto i = 0; i < I_FormArray.size(); i++) {
			I_BoolArray[i] = FrameworkAPI::IsItemKnown(I_FormArray[i], &CPatch_FSH_I::Data);
		}
		I_EntriesTotal = I_FormArray.size();
		I_EntriesFound = std::ranges::count(I_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < B_FormArray.size(); i++) {
			B_BoolArray[i] = FrameworkAPI::IsBookKnown(B_FormArray[i]);
		}
		B_EntriesTotal = B_FormArray.size();
		B_EntriesFound = std::ranges::count(B_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < A_FormArray.size(); i++) {
			A_BoolArray[i] = FoundItemData_NoShow.HasForm(A_FormArray[i]->GetFormID());
		}
		A_EntriesTotal = A_FormArray.size();
		A_EntriesFound = std::ranges::count(A_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < C_FormArray.size(); i++) {
			C_BoolArray[i] = FoundItemData_NoShow.HasForm(C_FormArray[i]->GetFormID());
		}
		C_EntriesTotal = C_FormArray.size();
		C_EntriesFound = std::ranges::count(C_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < L_FormArray.size(); i++) {
			L_BoolArray[i] = FoundItemData_NoShow.HasForm(L_FormArray[i]->GetFormID());
		}
		L_EntriesTotal = L_FormArray.size();
		L_EntriesFound = std::ranges::count(L_BoolArray, true);

		// ---------------------------------

		for (auto i = 0; i < S_FormArray.size(); i++) {
			S_BoolArray[i] = FoundItemData_NoShow.HasForm(S_FormArray[i]->GetFormID());
		}
		S_EntriesTotal = S_FormArray.size();
		S_EntriesFound = std::ranges::count(S_BoolArray, true);
	}
}