#include "Serialization.hpp"
#include "CFramework_Rar.hpp"
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

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);

		PatchesInstalled += 1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_Rar_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_Rar_Items::Data.GetBase(a_event->baseObj) ? CPatch_Rar_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_Rar_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_Rar_Items::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_Rar_Items::Data.GetAllVariations()) {
				if (CPatch_Rar_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(Items_FormArray, CPatch_Rar_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {	

		CPatch_Rar_Items::Data.CompileFormArray(CPatch_RAR::Items, modname);
		CPatch_Rar_Items::Data.CompileVariation(CPatch_RAR::ItemsV, modname);
		CPatch_Rar_Items::Data.MergeAsCollectable();

		CPatch_Rar_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPagRoyalArmory", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_Rar_Items::Data);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}
}