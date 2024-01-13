#include "Serialization.hpp"
#include "CFramework_ST3.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_ST3 {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Armor & Weapons
	0x001396,0x001900,0x00138A,0x01A94A,0x018E48,
	0x001387,0x01A948,0x00138C,0x0018FE,0x001390,
	0x0178A2,0x0018FA,0x0018FB,0x01A949,0x002EB4,
	0x001902,0x00F735,0x00A058,0x001901,0x0018FC,
	0x0018FD,0x01733D,0x01279A,
	};

	constexpr Serialization::FormArray ItmL2 = { // Alcohol
	0x000E0F,0x000E0C,0x000E12,0x000E15,0x000E18,
	0x000E1B,0x012CFF,0x000E1E,0x000E21,
	};

	constexpr Serialization::FormArray ItmL3 = { // Gold
	0x0064D5,0x006A45,0x006A49,0x006A4B,0x0064C9,
	0x006A43,0x0064DD,0x0064CB,0x005497,0x005F61,
	0x0064CD,0x0064C7,0x005F62,0x0064D1,0x005499,
	0x006A47,0x000E06,0x0064DB,0x000DE9,0x0064CF,
	0x000E02,0x000DFC,0x0064D3,0x0064D7,
	};

	constexpr Serialization::FormArray ItmL4 = { // Copper
	0x000DDD,0x000E04,0x000DDF,0x000DE1,0x000DE3,
	0x000DE7,0x000DE5,0x01022C,0x000DEF,0x01022F,
	0x01022D,0x01022E,0x000DED,0x000DF5,
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

		ItemData1.CompileFormArray(CPatch_ST3::ItmL1, modname);
		ItemData2.CompileFormArray(CPatch_ST3::ItmL2, modname);
		ItemData3.CompileFormArray(CPatch_ST3::ItmL3, modname);
		ItemData4.CompileFormArray(CPatch_ST3::ItmL4, modname);

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
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL1_FormArray[i], ItmL1_NameArray[i], "$MCMPageSUT3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL2_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL2_FormArray[i], ItmL2_NameArray[i], "$MCMPageSUT3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL3_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL3_FormArray[i], ItmL3_NameArray[i], "$MCMPageSUT3", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItmL4_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItmL4_FormArray[i], ItmL4_NameArray[i], "$MCMPageSUT3", std::to_underlying(EntryCategory::kItem)));
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