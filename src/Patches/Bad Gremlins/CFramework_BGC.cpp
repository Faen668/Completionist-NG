#include "Serialization.hpp"
#include "CFramework_BGC.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_BGC {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Fairies
	0x000837,0x000838,0x000839,0x00083A,0x00083B,0x00083C,0x00083D,
	0x00083E,0x00083F,0x000842,0x000844,0x000848,0x00084A,0x00084B,
	0x00084C,0x000843,0x000847,0x000849,0x000840,0x000846,0x000845,
	0x000841
	};

	constexpr Serialization::FormArray ItmL2 = { // Fish
	0x00082A,0x00082F,0x00082D,0x00082E,0x000828,0x000836,0x000830,
	0x000831,0x000832,0x000833,0x000834,0x000835,0x000827,0x00082C,
	0x00082B,0x000829,
	};

	constexpr Serialization::FormArray ItmL3 = { // Heads
	0x000823,0x00081D,0x000820,0x00081E,0x000824,0x00081B,0x000822,
	0x000821,0x00081C,0x000826,0x000825,0x00081F,
	};

	constexpr Serialization::FormArray ItmL4 = { // Jars
	0x000812,0x000807,0x000814,0x000811,0x000805,0x000816,0x000813,
	0x00080E,0x00080F,0x000819,0x000815,0x000801,0x000806,0x000800,
	0x000808,0x00080A,0x000804,0x000803,0x00080D,0x00080B,0x00080C,
	0x000817,0x000818,0x000810,0x000809,0x00081A,0x000802

	};
	
	// clang-format on

	constexpr std::string_view modname = "BGCollectables.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	void CHandler::OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event) {
		using log = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014) { return; }

		if (ItemData1.HasForm(a_event->baseObj)) {
			auto base = ItemData1.GetBase(a_event->baseObj) ? ItemData1.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData1, ItmL1_FormArray, &ItmL1_BoolArray, &ItmL1_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData2.HasForm(a_event->baseObj)) {
			auto base = ItemData2.GetBase(a_event->baseObj) ? ItemData2.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData2, ItmL2_FormArray, &ItmL2_BoolArray, &ItmL2_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData3.HasForm(a_event->baseObj)) {
			auto base = ItemData3.GetBase(a_event->baseObj) ? ItemData3.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData3, ItmL3_FormArray, &ItmL3_BoolArray, &ItmL3_EntriesFound, log::kCollected, "NotifyItems");
			return;
		}

		if (ItemData4.HasForm(a_event->baseObj)) {
			auto base = ItemData4.GetBase(a_event->baseObj) ? ItemData4.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData4, ItmL4_FormArray, &ItmL4_BoolArray, &ItmL4_EntriesFound, log::kCollected, "NotifyItems");
			return;
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
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		ItemData1.CompileFormArray(CPatch_BGC::ItmL1, modname);
		ItemData2.CompileFormArray(CPatch_BGC::ItmL2, modname);
		ItemData3.CompileFormArray(CPatch_BGC::ItmL3, modname);
		ItemData4.CompileFormArray(CPatch_BGC::ItmL4, modname);

		ItemData1.MergeAsCollectable();
		ItemData2.MergeAsCollectable();
		ItemData3.MergeAsCollectable();
		ItemData4.MergeAsCollectable();

		ItemData1.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		ItemData2.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		ItemData3.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		ItemData4.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < ItmL1_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL1_FormArray[i], ItmL1_NameArray[i], "$MCMPageBadGremlins", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL2_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL2_FormArray[i], ItmL2_NameArray[i], "$MCMPageBadGremlins", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_FormArray[i], ItmL3_NameArray[i], "$MCMPageBadGremlins", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_FormArray[i], ItmL4_NameArray[i], "$MCMPageBadGremlins", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < ItmL1_FormArray.size(); i++) {
			ItmL1_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_FormArray[i], &ItemData1);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &ItemData2);
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			ItmL3_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL3_FormArray[i], &ItemData3);
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			ItmL4_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL4_FormArray[i], &ItemData4);
		}

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
	}
}