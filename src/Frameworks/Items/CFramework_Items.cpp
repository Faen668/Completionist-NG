#include "Serialization.hpp"
#include "CFramework_Items.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Items {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray SK_Forms = {
	0x044E8B,0x044E63,0x044E6A,0x044E67,0x044E6C,0x044E6E,0x044E65,0x019954,
	0x05598C,0x060CC2,0x019952,0x01994F,0x06F266,0x019958,0x0F6767,0x01F6D4,
	0x03532C,0x1058AA,0x0B08C7,0x03A3D2,0x028AD7,0x0FBC3C,0x033764,0x044E12,
	0x0FBC3B,0x01A31C,0x03EC95,0x09F7A6,0x05C09E,0x02996F,0x075868,0x01BCBC,
	0x0DAB04,0x02C25E,0x0BBCD5,0x037CE9,0x04E4E6,0x0F257D,0x0FBC3D,0x0C4F2E,
	0x059654,0x04286C,0x02BAAB,0x09E01F,0x0C04BB,0x05E8AD,0x05E52A,0x1092B8,
	0x03A3DD,0x0AADB6,0x0AADB7,0x0D8E43,0x0457AB,0x0D363B,0x0AA03E,0x0F1491,
	0x02AC62,0x03A070,0x0CADFB,0x0E49F7,0x105109,0x02C25A,0x026C31,0x0A34F8,
	0x0FBC3A,0x0E77BB,0x0E49F8,0x043E27,0x043E26,0x043E28,0x105A4E,
	};

	constexpr Serialization::Variation SK_Variations[] = {
		{ 0x063B27,{ 0x063B29 } },
		{ 0x021EA3,{ 0x10CC6A } },
		{ 0x03E6BB,{ 0x03E6BC, 0x08D770 } },
		{ 0x09DFF5,{ 0x0DA74D } },
	};

	constexpr Serialization::FormArray DG_Forms = {
	0x019ABB,0x019ABC,0x019ABD,0x012F97,0x012FC3,0x00575B,0x00575C,0x01433E,
	0x01433C,0x01433D,0x00F1C8,0x006BAD,0x00F1C4,0x00F1C5,0x00F1C7,0x00F1C9,
	0x0066F3,0x0066F5,0x0066F6,
	};

	constexpr Serialization::Variation DG_Variations[] = {
	{ 0x0191CB,{ 0x002B29 } },
	};

	constexpr Serialization::FormArray DB_Forms = {
	0x0173BB,
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

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = Data.GetBase(a_event->baseObj) ? Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (auto t_pos = std::find(FormArray.begin(), FormArray.end(), Data.GetForm(a_baseID)); t_pos != FormArray.end()) {
			auto b_pos = std::distance(FormArray.begin(), t_pos);
			BoolArray.at(b_pos) = true;

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);

			for (auto var : Data.GetAllVariations()) {
				if (Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		Data.CompileFormArray(CFramework_Items::SK_Forms, "Skyrim.esm");
		Data.CompileFormArray(CFramework_Items::DG_Forms, "Dawnguard.esm");
		Data.CompileFormArray(CFramework_Items::DB_Forms, "Dragonborn.esm");

		Data.CompileVariation(CFramework_Items::SK_Variations, "Skyrim.esm");
		Data.CompileVariation(CFramework_Items::DG_Variations, "Dawnguard.esm");
		Data.MergeAsCollectable();

		CFramework_Items::Data.Populate(NameArray, FormArray, BoolArray, TextArray);
		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData.HasForm(FormArray[i]->GetFormID())) {
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

				FoundItemData.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}