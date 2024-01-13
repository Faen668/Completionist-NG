#include "Serialization.hpp"
#include "CFramework_Rar.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_RAR {
	using namespace CFramework_Master;

	// clang-format off

	//Normal | Dirt And Blood | Bathing In Skyrim
	constexpr Serialization::FormArray Items = {
	0x00aa07
	};

	constexpr Serialization::Variation ItemsV[] = {
	{ 0x005902, { 0x08e4a5 } },
	{ 0x117047, { 0x11704c } },
	{ 0x05ba6a, { 0x08e4b1 } },
	{ 0x0e9701, { 0x0f390e } },
	{ 0x00aa06, { 0x08938c, 0x08938e, 0x08938f } },
	{ 0x038351, { 0x0a28d4 } },
	{ 0x11704d, { 0x117050 } },
	{ 0x04c75e, { 0x08e4a3 } },
	{ 0x00fb0a, { 0x0935c7 } },
	{ 0x0c0ee4, { 0x0c0ee6 } },
	{ 0x02e14b, { 0x08e4ac } },
	{ 0x056964, { 0x09d7ce } },
	{ 0x056965, { 0x0935b6 } },
	{ 0x153c7a, { 0x153c7e } },
	{ 0x0e9700, { 0x0f3925 } },
	{ 0x149a69, { 0x149a6e } },
	{ 0x019d3d, { 0x0935c0 } },
	{ 0x14eb78, { 0x153c81 } },
	{ 0x111f3d, { 0x111f40 } },
	{ 0x0e96ff, { 0x0f3916 } },
	{ 0x149a6a, { 0x149a70 } },
	{ 0x0e9702, { 0x0ee809 } },
	{ 0x042556, { 0x0935bb } },
	{ 0x04765a, { 0x08e4a9 } },
	};
	// clang-format on

	constexpr std::string_view modname = "PrvtIRoyalArmory.esp";

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

		if (a_event->newContainer != 0x00014 || !ItemData.HasForm(a_event->baseObj)) { return; }

		auto base = ItemData.GetBase(a_event->baseObj) ? ItemData.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, ItemData, Items_FormArray, &Items_BoolArray, &Items_EntriesFound, log::kCollected, "NotifyItems");
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

		ItemData.CompileFormArray(CPatch_RAR::Items, modname);
		ItemData.CompileVariation(CPatch_RAR::ItemsV, modname);
		ItemData.MergeAsCollectable();

		ItemData.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Search Terms ) ---
	//---------------------------------------------------

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Items_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Items_FormArray[i], Items_NameArray[i], "$MCMPagRoyalArmory", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &ItemData);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}
}