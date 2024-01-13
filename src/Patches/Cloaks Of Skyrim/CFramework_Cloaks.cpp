#include "Serialization.hpp"
#include "CFramework_Cloaks.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_Cloaks {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItemsSK = {
	0x007412,0x007976,0x007978,0x007EDC,0x008440,0x009480,0x009F51,0x00B4EC,
	0x00BFB2,0x00CFDD,0x00CFE3,0x00CFE5,0x00E012,0x00E57F,0x00F601,0x0126BF,
	0x0126C0,0x012C32,0x006EA7,0x006EA9,0x00F616
	};

	constexpr Serialization::FormArray ItemsDP = {
	0x01213B,0x01213C,0x01213D,0x01213E,0x01213F,0x012140,0x010BA8,0x010BAA
	};

	// clang-format on
	constexpr std::string_view modname = "Cloaks.esp";

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

	void CHandler::OnContainerChangedEvent(const RE::TESContainerChangedEvent* a_event) {
		using cmd = Serialization::CompletionistLog::logType;

		if (a_event->newContainer != 0x00014) {
			return;
		}

		if (ItemData1.HasForm(a_event->baseObj)) {
			auto base = ItemData1.GetBase(a_event->baseObj) ? ItemData1.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData1, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, cmd::kCollected, "NotifyItems");
			return;
		}

		if (ItemData2.HasForm(a_event->baseObj)) {
			auto base = ItemData2.GetBase(a_event->baseObj) ? ItemData2.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData2, ItemsDP_FormArray, &ItemsDP_BoolArray, &ItemsDP_EntriesFound, cmd::kCollected, "NotifyItems");
			return;
		}

		return;
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

		ItemData1.CompileFormArray(CPatch_Cloaks::ItemsSK, modname);
		ItemData1.MergeAsCollectable();

		ItemData2.CompileFormArray(CPatch_Cloaks::ItemsDP, modname);
		ItemData2.MergeAsCollectable();

		ItemData1.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		ItemData2.Populate(ItemsDP_NameArray, ItemsDP_FormArray, ItemsDP_BoolArray, ItemsDP_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		ItemsDP_EntriesTotal = ItemsDP_FormArray.size();
		ItemsDP_EntriesFound = std::ranges::count(ItemsDP_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPageCloaksOfSkyrim", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto i = 0; i < ItemsDP_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(ItemsDP_FormArray[i], ItemsDP_NameArray[i], "$MCMPageCloaksOfSkyrim", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &ItemData1);
		}

		for (auto i = 0; i < ItemsDP_FormArray.size(); i++) {
			ItemsDP_BoolArray[i] = FrameworkAPI::IsItemKnown(ItemsDP_FormArray[i], &ItemData2);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		ItemsDP_EntriesTotal = ItemsDP_FormArray.size();
		ItemsDP_EntriesFound = std::ranges::count(ItemsDP_BoolArray, true);
	}
}