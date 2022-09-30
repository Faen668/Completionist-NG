#include "Serialization.hpp"
#include "CFramework_Fishing.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm
#undef GetObject

namespace CPatch_FSH_F {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_I {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_B {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_A {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_C {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_L {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH_S {
	Serialization::CompletionistData Data;
}

namespace CPatch_FSH {
	using namespace CFramework_Master;

	constexpr Serialization::FormArray I_Forms = {
	0x000ED3,0x000A6F,0x000C66,0x000B2B,0x000B29,0x000B94,0x0EA5CC,
	0x0008E3,0x000BC0,0x000BA0,0x000C07,0x000F07,0x000B2A,
	};

	constexpr Serialization::Variation I_Varia[] = {
	{ 0x07AED6, { 0x0009D9,0x04D05E } },
	};

	constexpr Serialization::FormArray B_Forms = {
	0x000E7F,0x0009AE,0x0009AF,0x000ABA,0x000ABC,0x070CCC,0x070CCD,
	0x070CCE,0x070CCF,0x070CD0,0x070CD1,0x070CD2,0x070CD3,0x070CD4,
	0x070CD5,0x0008E5,
	};

	constexpr Serialization::FormArray A_Forms = {
	0x000956,0x00093C,0x000964,0x000951,0x00095A,0x000961,0x00095F,0x000962,
	0x000963,0x000959,0x000958,0x000965,0x000950,0x000957,0x000966,0x00095B,
	0x000953,0x000967,0x000955,0x000954,0x000952,
	};

	constexpr Serialization::FormArray C_Forms = {
	0x00095E,0x000960,
	};

	constexpr Serialization::FormArray L_Forms = {
	0x000932,0x000937,0x00092F,0x000936,0x000939,0x000935,0x000930,0x000931,
	0x000938,0x000933,
	};

	constexpr Serialization::FormArray S_Forms = {
	0x000934,0x000945,0x00094C,0x000944,0x000948,0x000943,0x00094A,0x00093A,
	0x00093E,0x00094F,0x000941,0x00093F,0x000946,0x00095D,0x000949,0x000940,
	0x00093D,0x00093B,0x00095C,0x00094E,0x00094D,0x000942,0x00094B,0x000947,
	};

	inline std::vector<std::string>		F_NameArray;
	inline std::vector<std::string>		F_TextArray;
	inline std::vector<RE::TESForm*>	F_FormArray;
	inline std::vector<bool>			F_BoolArray;
	inline std::int32_t					F_EntriesTotal;
	inline std::int32_t					F_EntriesFound;

	inline std::vector<std::string>		I_NameArray;
	inline std::vector<std::string>		I_TextArray;
	inline std::vector<RE::TESForm*>	I_FormArray;
	inline std::vector<bool>			I_BoolArray;
	inline std::int32_t					I_EntriesTotal;
	inline std::int32_t					I_EntriesFound;

	inline std::vector<std::string>		B_NameArray;
	inline std::vector<std::string>		B_TextArray;
	inline std::vector<RE::TESForm*>	B_FormArray;
	inline std::vector<bool>			B_BoolArray;
	inline std::int32_t					B_EntriesTotal;
	inline std::int32_t					B_EntriesFound;

	inline std::vector<std::string>		A_NameArray;
	inline std::vector<std::string>		A_TextArray;
	inline std::vector<RE::TESForm*>	A_FormArray;
	inline std::vector<bool>			A_BoolArray;
	inline std::int32_t					A_EntriesTotal;
	inline std::int32_t					A_EntriesFound;
	inline
	inline std::vector<std::string>		C_NameArray;
	inline std::vector<std::string>		C_TextArray;
	inline std::vector<RE::TESForm*>	C_FormArray;
	inline std::vector<bool>			C_BoolArray;
	inline std::int32_t					C_EntriesTotal;
	inline std::int32_t					C_EntriesFound;

	inline std::vector<std::string>		L_NameArray;
	inline std::vector<std::string>		L_TextArray;
	inline std::vector<RE::TESForm*>	L_FormArray;
	inline std::vector<bool>			L_BoolArray;
	inline std::int32_t					L_EntriesTotal;
	inline std::int32_t					L_EntriesFound;

	inline std::vector<std::string>		S_NameArray;
	inline std::vector<std::string>		S_TextArray;
	inline std::vector<RE::TESForm*>	S_FormArray;
	inline std::vector<bool>			S_BoolArray;
	inline std::int32_t					S_EntriesTotal;
	inline std::int32_t					S_EntriesFound;

	inline ScriptObjectPtr MCMScript;

	std::vector<std::string> temploc{};
	std::vector<std::string> temprod{};
	std::vector<RE::TESForm*> tempfsh{};

	inline static RE::BGSListForm* RodList;
	inline static RE::TESGlobal* GlobalV;
	inline static RE::ControlMap* ContMap;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (auto installed = Serialization::CompletionistData::IsModInstalled("ccbgssse001-fish.esm"); !installed) {
			return;
		}

		RodList = RE::TESForm::LookupByEditorID<RE::BGSListForm>("ccBGSSSE001_FishingRods");
		GlobalV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FishingRods");
		ContMap = RE::ControlMap::GetSingleton();

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

		CPatch_FSH_A::Data.CompileFormArray(CPatch_FSH::A_Forms, "Completionist_ITP.esp");
		CPatch_FSH_A::Data.Populate(A_NameArray, A_FormArray, A_BoolArray, A_TextArray, false, 2);

		A_EntriesTotal = A_FormArray.size();
		A_EntriesFound = std::ranges::count(A_BoolArray, true);

		// Caves Markers ---

		CPatch_FSH_C::Data.CompileFormArray(CPatch_FSH::C_Forms, "Completionist_ITP.esp");
		CPatch_FSH_C::Data.Populate(C_NameArray, C_FormArray, C_BoolArray, C_TextArray, false, 2);

		C_EntriesTotal = C_FormArray.size();
		C_EntriesFound = std::ranges::count(C_BoolArray, true);

		// Lakes Markers ---

		CPatch_FSH_L::Data.CompileFormArray(CPatch_FSH::L_Forms, "Completionist_ITP.esp");
		CPatch_FSH_L::Data.Populate(L_NameArray, L_FormArray, L_BoolArray, L_TextArray, false, 2);

		L_EntriesTotal = L_FormArray.size();
		L_EntriesFound = std::ranges::count(L_BoolArray, true);

		// Streams Markers ---

		CPatch_FSH_S::Data.CompileFormArray(CPatch_FSH::S_Forms, "Completionist_ITP.esp");
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

	//---------------------------------------------------
	//-- Framework Events (Build FIsh Arrays ) ----------
	//---------------------------------------------------

	void CHandler::BuildFishArrays(RE::TESForm* a_form, std::string a_rod, std::string a_loc) {
		
		tempfsh.push_back(a_form);
		temprod.push_back(a_rod);
		temploc.push_back(a_loc);
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
				auto* a_marker = static_cast<RE::TESObjectREFR*>(A_FormArray[i]);

				if (a_marker && CPatch_FSH_A::Data.HasForm(a_marker->GetFormID())) {
					if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
						if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
							A_BoolArray[i] = true;
							FoundItemData_NoShow.AddForm(static_cast<RE::TESObjectREFR*>(A_FormArray[i]));
						}
					}
				}
			}

			for (auto i = 0; i < C_FormArray.size(); i++) {
				auto* a_marker = static_cast<RE::TESObjectREFR*>(C_FormArray[i]);

				if (a_marker && CPatch_FSH_C::Data.HasForm(a_marker->GetFormID())) {
					if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
						if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
							C_BoolArray[i] = true;
							FoundItemData_NoShow.AddForm(static_cast<RE::TESObjectREFR*>(C_FormArray[i]));
						}
					}
				}
			}

			for (auto i = 0; i < L_FormArray.size(); i++) {
				auto* a_marker = static_cast<RE::TESObjectREFR*>(L_FormArray[i]);

				if (a_marker && CPatch_FSH_L::Data.HasForm(a_marker->GetFormID())) {
					if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
						if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
							L_BoolArray[i] = true;
							FoundItemData_NoShow.AddForm(static_cast<RE::TESObjectREFR*>(L_FormArray[i]));
						}
					}
				}
			}

			for (auto i = 0; i < S_FormArray.size(); i++) {
				auto* a_marker = static_cast<RE::TESObjectREFR*>(S_FormArray[i]);

				if (a_marker && CPatch_FSH_S::Data.HasForm(a_marker->GetFormID())) {
					if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
						if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
							S_BoolArray[i] = true;
							FoundItemData_NoShow.AddForm(static_cast<RE::TESObjectREFR*>(S_FormArray[i]));
						}
					}
				}
			}
		}
		A_EntriesFound = std::ranges::count(A_BoolArray, true);
		C_EntriesFound = std::ranges::count(C_BoolArray, true);
		L_EntriesFound = std::ranges::count(L_BoolArray, true);
		S_EntriesFound = std::ranges::count(S_BoolArray, true);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_section) {

		if (a_section == "Fish") {
			if (!FoundItemData_NoShow.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_FSH_F::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(a_baseID);
			auto t_pos = std::ranges::find(F_FormArray, CPatch_FSH_F::Data.GetForm(a_baseID));
			auto b_pos = std::distance(F_FormArray.begin(), t_pos);
			F_BoolArray[b_pos] = true;

			F_EntriesFound = std::ranges::count(F_BoolArray, true);
			return;
		}

		if (a_section == "Items") {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_FSH_I::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
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
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_FSH_B::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyBooks");
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

		if (auto installed = Serialization::CompletionistData::IsModInstalled("ccbgssse001-fish.esm"); !installed) {
			return;
		}

		for (auto i = 0; i < F_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(F_FormArray[i]->GetFormID())) {
				F_BoolArray[i] = true;
			}
		}
		F_EntriesTotal = F_FormArray.size();
		F_EntriesFound = std::ranges::count(F_BoolArray, true);

		// ---------------------------------

		for (auto i = 0; i < I_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(I_FormArray[i]->GetFormID())) {
				I_BoolArray[i] = true;
			}
		}
		I_EntriesTotal = I_FormArray.size();
		I_EntriesFound = std::ranges::count(I_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < B_FormArray.size(); i++) {
			if (auto* book = static_cast<RE::TESObjectBOOK*>(B_FormArray[i]); book) {
				if (book->IsRead() || (book->GetSpell() && RE::PlayerCharacter::GetSingleton()->HasSpell(book->GetSpell())) || FoundItemData.HasForm(book->GetFormID())) {
					B_BoolArray[i] = true;
				}
			}
		}
		B_EntriesTotal = B_FormArray.size();
		B_EntriesFound = std::ranges::count(B_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < A_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(A_FormArray[i]->GetFormID())) {
				A_BoolArray[i] = true;
			}
		}
		A_EntriesTotal = A_FormArray.size();
		A_EntriesFound = std::ranges::count(A_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < C_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(C_FormArray[i]->GetFormID())) {
				C_BoolArray[i] = true;
			}
		}
		C_EntriesTotal = C_FormArray.size();
		C_EntriesFound = std::ranges::count(C_BoolArray, true);


		// ---------------------------------

		for (auto i = 0; i < L_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(L_FormArray[i]->GetFormID())) {
				L_BoolArray[i] = true;
			}
		}
		L_EntriesTotal = L_FormArray.size();
		L_EntriesFound = std::ranges::count(L_BoolArray, true);

		// ---------------------------------

		for (auto i = 0; i < S_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(S_FormArray[i]->GetFormID())) {
				S_BoolArray[i] = true;
			}
		}
		S_EntriesTotal = S_FormArray.size();
		S_EntriesFound = std::ranges::count(S_BoolArray, true);
	}
}