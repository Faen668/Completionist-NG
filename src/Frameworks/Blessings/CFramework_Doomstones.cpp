#include "Serialization.hpp"
#include "CFramework_Doomstones.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Doomstones {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray DoomStoneForms = {
	0x0D2331,0x0D2334,0x0D2330,0x0D2336,0x0D2332,0x0D232E,0x0D2337,0x0D2339,
	0x0D2335,0x0D2333,0x0D232F,0x0D2338,0x0D232D,
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

		Data.CompileFormArray(CFramework_Doomstones::DoomStoneForms, "Skyrim.esm");
		CFramework_Doomstones::Data.Populate(NameArray, FormArray, BoolArray, TextArray);

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