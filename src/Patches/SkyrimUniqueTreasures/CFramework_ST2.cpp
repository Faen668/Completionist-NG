#include "Serialization.hpp"
#include "CFramework_ST2.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_ST2 {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Skulls
	0x0064DF,0x013D45,0x013268,0x013D47,0x013D49,
	0x0064D9,0x013D4B,0x013D4D,0x01326A,
	};

	constexpr Serialization::FormArray ItmL2 = { // Glass
	0x006A5B,0x006A53,0x006A55,0x006A57,0x006A50,
	0x016DD3,0x016DD2,0x01686E,0x0178A4,
	};

	constexpr Serialization::FormArray ItmL3 = { // Gems
	0x012D02,0x0049A7,0x0049A9,0x0049AB,0x0049AE,
	0x0049B4,0x0049B0,0x0049B2,0x000DDA,
	};

	constexpr Serialization::FormArray ItmL4 = { // Wine Glasses
	0x013D33,0x013D3F,0x013D35,0x013266,0x013D37,
	0x013D39,0x013D3B,0x013D3D,0x013D41,0x013D43,
	};

	// clang-format on

	constexpr std::string_view modname = "SkyrimsUniqueTreasures.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(CHandler::OnContainerChangedEvent);
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

		ItemData1.CompileFormArray(CPatch_ST2::ItmL1, modname);
		ItemData2.CompileFormArray(CPatch_ST2::ItmL2, modname);
		ItemData3.CompileFormArray(CPatch_ST2::ItmL3, modname);
		ItemData4.CompileFormArray(CPatch_ST2::ItmL4, modname);

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
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL1_FormArray[i], ItmL1_NameArray[i], "$MCMPageSUT2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL2_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL2_FormArray[i], ItmL2_NameArray[i], "$MCMPageSUT2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_FormArray[i], ItmL3_NameArray[i], "$MCMPageSUT2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_FormArray[i], ItmL4_NameArray[i], "$MCMPageSUT2", std::to_underlying(EntryCategory::kItem)));
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