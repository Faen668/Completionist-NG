#include "Serialization.hpp"
#include "CFramework_Barenziah.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_Barenziah {

	using namespace CFramework_Master;

	// clang-format off
	constexpr Serialization::FormArray BarenziahForms = {
	0x07FB5D,0x07FB56,0x07F908,0x07F927,0x07F901,0x07FB4F,0x07FB58,
	0x07FB63,0x07F925,0x07F909,0x07FB45,0x07FB64,0x07FB55,0x07F926,
	0x07FB3A,0x07FB65,0x07FB51,0x07FB54,0x07F956,0x07F95D,0x07F923,
	0x07F965,0x07F962,0x07FB4A,
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

		if (!a_event || !a_event->objectActivated || !Data.HasForm(a_event->objectActivated.get()->GetFormID())) { return EventResult::kContinue; }

		if (auto t_pos = std::ranges::find(FormArray, a_event->objectActivated.get()); t_pos == FormArray.end()) { return EventResult::kContinue; }
		

		CHandler::ProcessFoundForm(a_event->objectActivated.get());
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::TESObjectREFR* a_object) {

		auto t_pos = std::ranges::find(FormArray, a_object);
		auto b_pos = std::distance(FormArray.begin(), t_pos);

		if (!FoundItemData_NoShow.HasForm(a_object->GetFormID())) {
			auto msg = fmt::format("Completionist: Entry Complete - Stone of Barenziah!"sv);
			FrameworkAPI::SendNotification(msg, "NotifySpecial");
		}

		FoundItemData_NoShow.AddForm(a_object->GetFormID());
		BoolArray[b_pos] = true;

		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Form ) ---------------
	//---------------------------------------------------

	RE::TESForm* CHandler::Get(RE::FormID a_formID) {
		if (auto* form = RE::TESForm::LookupByID(a_formID); form) {
			return form;
		}
		return nullptr;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();
		MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(handler->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");

		Data.CompileFormArray(CFramework_Barenziah::BarenziahForms, "Skyrim.esm");

		FormArray.clear();
		NameArray.clear();
		TextArray.clear();
		BoolArray.clear();

		FormArray.push_back(Get(0x07FB5D)); 
		FormArray.push_back(Get(0x07FB56)); 
		FormArray.push_back(Get(0x07F908)); 
		FormArray.push_back(Get(0x07F927)); 
		FormArray.push_back(Get(0x07F901));
		FormArray.push_back(Get(0x07FB4F)); 
		FormArray.push_back(Get(0x07FB58)); 
		FormArray.push_back(Get(0x07FB63)); 
		FormArray.push_back(Get(0x07F925)); 
		FormArray.push_back(Get(0x07F909));
		FormArray.push_back(Get(0x07FB45)); 
		FormArray.push_back(Get(0x07FB64)); 
		FormArray.push_back(Get(0x07FB55)); 
		FormArray.push_back(Get(0x07F926)); 
		FormArray.push_back(Get(0x07FB3A));
		FormArray.push_back(Get(0x07FB65)); 
		FormArray.push_back(Get(0x07FB51)); 
		FormArray.push_back(Get(0x07FB54)); 
		FormArray.push_back(Get(0x07F956)); 
		FormArray.push_back(Get(0x07F95D));
		FormArray.push_back(Get(0x07F923)); 
		FormArray.push_back(Get(0x07F965)); 
		FormArray.push_back(Get(0x07F962)); 
		FormArray.push_back(Get(0x07FB4A));

		NameArray.push_back("$BarenziahList00"); 
		NameArray.push_back("$BarenziahList01"); 
		NameArray.push_back("$BarenziahList02"); 
		NameArray.push_back("$BarenziahList03"); 
		NameArray.push_back("$BarenziahList04");
		NameArray.push_back("$BarenziahList05"); 
		NameArray.push_back("$BarenziahList06"); 
		NameArray.push_back("$BarenziahList07"); 
		NameArray.push_back("$BarenziahList08"); 
		NameArray.push_back("$BarenziahList09");
		NameArray.push_back("$BarenziahList10"); 
		NameArray.push_back("$BarenziahList11"); 
		NameArray.push_back("$BarenziahList12"); 
		NameArray.push_back("$BarenziahList13"); 
		NameArray.push_back("$BarenziahList14");
		NameArray.push_back("$BarenziahList15"); 
		NameArray.push_back("$BarenziahList16"); 
		NameArray.push_back("$BarenziahList17"); 
		NameArray.push_back("$BarenziahList18"); 
		NameArray.push_back("$BarenziahList19");
		NameArray.push_back("$BarenziahList20"); 
		NameArray.push_back("$BarenziahList21"); 
		NameArray.push_back("$BarenziahList22"); 
		NameArray.push_back("$BarenziahList23");
		
		TextArray.push_back("$BarenziahHighlight00"); 
		TextArray.push_back("$BarenziahHighlight01"); 
		TextArray.push_back("$BarenziahHighlight02"); 
		TextArray.push_back("$BarenziahHighlight03"); 
		TextArray.push_back("$BarenziahHighlight04");
		TextArray.push_back("$BarenziahHighlight05"); 
		TextArray.push_back("$BarenziahHighlight06"); 
		TextArray.push_back("$BarenziahHighlight07"); 
		TextArray.push_back("$BarenziahHighlight08"); 
		TextArray.push_back("$BarenziahHighlight09");
		TextArray.push_back("$BarenziahHighlight10"); 
		TextArray.push_back("$BarenziahHighlight11"); 
		TextArray.push_back("$BarenziahHighlight12"); 
		TextArray.push_back("$BarenziahHighlight13"); 
		TextArray.push_back("$BarenziahHighlight14");
		TextArray.push_back("$BarenziahHighlight15"); 
		TextArray.push_back("$BarenziahHighlight16"); 
		TextArray.push_back("$BarenziahHighlight17"); 
		TextArray.push_back("$BarenziahHighlight18"); 
		TextArray.push_back("$BarenziahHighlight19");
		TextArray.push_back("$BarenziahHighlight20"); 
		TextArray.push_back("$BarenziahHighlight21"); 
		TextArray.push_back("$BarenziahHighlight22"); 
		TextArray.push_back("$BarenziahHighlight23");

		NameArray.resize(FormArray.size());
		TextArray.resize(FormArray.size());
		BoolArray.resize(FormArray.size());

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

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
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