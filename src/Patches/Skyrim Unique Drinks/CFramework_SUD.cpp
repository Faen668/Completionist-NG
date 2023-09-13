#include "Serialization.hpp"
#include "CFramework_SUD.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_SUD {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x000800,0x000801,0x000802,0x000803,0x000804,0x000805,0x000806,
	0x000807,0x000808,0x000809,0x00080A,0x00080B,0x00080C,0x00080D,
	0x00080E,0x00080F,0x000810,0x000811,0x000812,0x000813,
	};

	// clang-format on

	constexpr std::string_view modname = "SUDs.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
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

		if (!a_event || a_event->newContainer != RE::PlayerCharacter::GetSingleton()->GetFormID()) { return EventResult::kContinue; }

		if (CPatch_SUD_Items::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_SUD_Items::Data.GetBase(a_event->baseObj) ? CPatch_SUD_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID) {

		if (!FoundItemData.HasForm(a_eventID)) {
			auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_SUD_Items::Data.GetForm(a_eventID)->GetName());
			FrameworkAPI::SendNotification(msg, "NotifyItems");
			FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_SUD_Items::Data.GetForm(a_eventID)->GetName());
		}

		FoundItemData.AddForm(a_baseID);
		for (auto var : CPatch_SUD_Items::Data.GetAllVariations()) {
			if (CPatch_SUD_Items::Data.GetBase(var) == a_baseID) {
				FoundItemData.AddForm(var);
			}
		}

		auto t_pos = std::ranges::find(Items_FormArray, CPatch_SUD_Items::Data.GetForm(a_baseID));
		auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
		Items_BoolArray[b_pos] = true;

		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
		return;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_SUD_Items::Data.CompileFormArray(CPatch_SUD::Items, modname);
		CPatch_SUD_Items::Data.MergeAsCollectable();
		CPatch_SUD_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageLiquor", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_SUD_Items::Data);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}
}