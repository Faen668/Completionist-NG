#include "Serialization.hpp"
#include "CFramework_ST1.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_ST1 {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Jade Statues
	0x00FC98,0x00FC99,0x00FC9A,0x00FC9B,0x00FC9C,0x00FC9D, 
	0x00FC9E,0x00FC9F,0x00FCA0,0x00FCA1,0x00FCA2,0x00FCA3,
	};

	constexpr Serialization::FormArray ItmL2 = { // Plates
	0x0049B6,0x004F1E,0x004F20,0x004F22,0x004F24,0x004F26, 
	0x004F28,0x004F2A,0x004F2C,0x004F2E,0x004F30,
	};

	constexpr Serialization::FormArray ItmL3 = { // Figurines
	0x0178AE,0x0178B1,0x0178AA,0x00F731,0x00F733,0x0178A7,
	0x017E15,0x0193B2,0x018379,0x0178AC,0x00F730,0x00F732,
	0x000E00,0x006A4D,0x013D4F,0x00855F,0x001903,0x001904,
	0x001906,0x001905,
	};

	constexpr Serialization::FormArray ItmL4 = { // Instruments
	0x000DFA,0x01733A,0x000DF8,0x000DFE,0x006A59,0x017338,
	0x016308,0x000DDB,0x014D7B,0x0059FD,0x015DA4,0x01630A,
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

		ItemData1.CompileFormArray(CPatch_ST1::ItmL1, modname);
		ItemData2.CompileFormArray(CPatch_ST1::ItmL2, modname);
		ItemData3.CompileFormArray(CPatch_ST1::ItmL3, modname);
		ItemData4.CompileFormArray(CPatch_ST1::ItmL4, modname);

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
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL1_FormArray[i], ItmL1_NameArray[i], "$MCMPageSUT1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL2_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL2_FormArray[i], ItmL2_NameArray[i], "$MCMPageSUT1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_FormArray[i], ItmL3_NameArray[i], "$MCMPageSUT1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_FormArray[i], ItmL4_NameArray[i], "$MCMPageSUT1", std::to_underlying(EntryCategory::kItem)));
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