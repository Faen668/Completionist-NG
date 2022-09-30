#include "Serialization.hpp"
#include "CFramework_Shrines.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Shrines_V {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray Shrines_SK = {
	0x0D9883 /*Akatosh*/,
	0x071854 /*Arkay*/,
	0x0D9881 /*Dibella*/,
	0x0D9885 /*Julianos*/,
	0x0D987F /*Kynareth*/,
	0x0D9887 /*Mara*/,
	0x10E8B0 /*Nocturnal*/,
	0x0D987D /*Stendarr*/,
	0x100780 /*Talos*/,
	0x0D987B /*Zenithar*/,
	0x04E4D5 /*Meridia*/,
	};

	constexpr Serialization::FormArray Shrines_DG = {
	0x00C86B /*Auriel*/,
	};

	constexpr Serialization::FormArray Shrines_DB = {
	0x03A484 /*Azura*/,
	0x039E34 /*Boethiah*/,
	0x03A481 /*Mephala*/,
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || !Data.HasForm(a_event->objectActivated->GetBaseObject()->GetFormID())) { return EventResult::kContinue; }

		auto formID = a_event->objectActivated->GetBaseObject()->GetFormID();
		auto base = Data.GetBase(formID) ? Data.GetBase(formID) : formID;
		CHandler::ProcessFoundForm(base, formID);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData_NoShow.HasForm(a_eventID)) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifySpecial");
		}

		FoundItemData_NoShow.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData_NoShow.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto pname = "Wintersun - Faiths of Skyrim.esp";
		if (const auto* Mod = RE::TESDataHandler::GetSingleton()->LookupLoadedModByName(pname); Mod) {
			Data.AddForm(0x03A484, "Dragonborn.esm", 0x07F163, pname); // Azura
			Data.AddForm(0x03A484, "Dragonborn.esm", 0x2D00B0, pname); // Azura
			Data.AddForm(0x039E34, "Dragonborn.esm", 0x09D7ED, pname); // Boethiah
		}

		Data.CompileFormArray(CFramework_Shrines_V::Shrines_SK, "Skyrim.esm");
		Data.CompileFormArray(CFramework_Shrines_V::Shrines_DG, "Dawnguard.esm");
		Data.CompileFormArray(CFramework_Shrines_V::Shrines_DB, "Dragonborn.esm");
		CFramework_Shrines_V::Data.Populate(NameArray, FormArray, BoolArray, TextArray);

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData_NoShow.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData_NoShow.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Wintersun
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_Shrines_P {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray Shrines_WI = {
	0x0E476B,0x107FC5,0x107FC6,0x0C60C4,0x0C6099,0x107FC7,0x0FDD3E,
	0x107FC8,0x09D7FA,0x107FCB,0x1B9652,0x1B9653,0x1B9650,0x1B9651,
	0x3D283F,0x1B964F,0x107FCC,0x2EE923,0x3F5FA1,0x107FC4,0x1B964E,
	0x1B964B,0x107FCE,0x1B964D,0x26FD37,0x1C89C7,0x1B964C,
	};

	constexpr Serialization::Variation Shrines_WIV[] = {
	{ 0x0986CA, { 0x2B6B9A, } }, //Mehrunes Dagon
	{ 0x0C60C5, { 0x3D283B, } }, //Sanguine
	{ 0x2D0116, { 0x2E4677, } }, //Jephre
	{ 0x2E464B, { 0x065C05, } }, //Magnus
	{ 0x107FCD, { 0x2E464E, } }, //Trinimac
	{ 0x0F8C25, { 0x2E464F, } }, //Xarxes	
	{ 0x2E464C, { 0x0935B3, } }, //Phynaster
	{ 0x2E46A7, { 0x019B85, 0x2E464D } }, //Syrabane
	};

	// clang-format on

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};

	inline ScriptObjectPtr  MCMScript;

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::RegisterEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || !Data.HasForm(a_event->objectActivated->GetBaseObject()->GetFormID())) { return EventResult::kContinue; }

		auto formID = a_event->objectActivated->GetBaseObject()->GetFormID();
		auto base = Data.GetBase(formID) ? Data.GetBase(formID) : formID;
		CHandler::ProcessFoundForm(base, formID);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData_NoShow.HasForm(a_eventID)) {
			auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifySpecial");
		}

		FoundItemData_NoShow.AddForm(a_baseID);
		for (auto var : Data.GetAllVariations()) {
			if (Data.GetBase(var) == a_baseID) {
				FoundItemData_NoShow.AddForm(var);
			}
		}

		auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID));
		auto b_pos = std::distance(FormArray.begin(), t_pos);
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		Data.CompileFormArray(CFramework_Shrines_P::Shrines_WI, "Wintersun - Faiths of Skyrim.esp");
		Data.CompileVariation(CFramework_Shrines_P::Shrines_WIV, "Wintersun - Faiths of Skyrim.esp");
		CFramework_Shrines_P::Data.Populate(NameArray, FormArray, BoolArray, TextArray);

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::find(NameArray.begin(), NameArray.end(), a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData_NoShow.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData_NoShow.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}