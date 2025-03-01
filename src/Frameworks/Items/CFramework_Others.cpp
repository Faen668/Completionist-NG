#include "CFramework_Others.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Others {
	using namespace CFramework_Master;

	// clang-format off

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

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		CEvents::EventHandler::RegisterForEvent_OnActivateEvent(CHandler::OnActivateEvent);
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Activated ) ---------
	//---------------------------------------------------

	void CHandler::OnActivateEvent(RE::TESActivateEvent const* a_event) 
	{
		if (a_event->objectActivated && ItemDataSB.HasForm(a_event->objectActivated.get()->GetFormID())) 
		{ 
			CHandler::ProcessFoundForm(a_event->objectActivated.get());
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(ProcessFoundFormArgs, std::string a_section) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, a_section);
			FrameworkAPI::AddNewEventToLog(eventHandle, data.GetForm(a_eventID)->GetName());
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : data.GetAllVariations()) {
			if (data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		bools->at(std::distance(forms.begin(), std::ranges::find(forms, data.GetForm(a_baseID)))) = true;
		*found = std::ranges::count(*bools, true);
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
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kBarenziah, SB_NameArray[b_pos]);
		}

		FoundItemData_NoShow.AddForm(a_object->GetFormID());
		SB_BoolArray[b_pos] = true;

		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemDataSB.CompileFormArray(CFramework_Others::Forms_SB, "Skyrim.esm");
		BuildBaranziahArrays();

		SB_EntriesTotal = SB_FormArray.size();
		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < SB_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(SB_FormArray[i], SB_NameArray[i], "$MCMPageBarenziah", std::to_underlying(EntryCategory::kBarenziah)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < SB_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(SB_FormArray[i]->GetFormID())) {
				SB_BoolArray[i] = true;
			}
		}

		SB_EntriesTotal = SB_FormArray.size();
		SB_EntriesFound = std::ranges::count(SB_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Build Barenziah Array ) --
	//---------------------------------------------------

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

		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name00"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name01"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name02"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name03"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name04"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name05"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name06"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name07"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name08"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name09"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name10"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name11"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name12"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name13"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name14"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name15"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name16"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name17"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name18"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name19"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name20"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name21"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name22"));
		SB_NameArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Name23"));

		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text00"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text01"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text02"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text03"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text04"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text05"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text06"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text07"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text08"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text09"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text10"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text11"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text12"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text13"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text14"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text15"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text16"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text17"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text18"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text19"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text20"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text21"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text22"));
		SB_TextArray.push_back(GET_LOC_STRING_BY_KEY("Barenziah_Text23"));

		SB_NameArray.resize(SB_FormArray.size());
		SB_TextArray.resize(SB_FormArray.size());
		SB_BoolArray.resize(SB_FormArray.size());
	}
}