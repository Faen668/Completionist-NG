#include "Serialization.hpp"
#include "CFramework_Cloaks.hpp"
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

	constexpr std::string_view modnameSK = "Cloaks.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modnameSK)) { return; }

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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_Cloaks_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		if (CPatch_Cloaks_Items::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_Cloaks_Items::Data.GetBase(a_event->baseObj) ? CPatch_Cloaks_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k1);
			return EventResult::kContinue;
		}

		if (CPatch_CloaksDP_Items::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_CloaksDP_Items::Data.GetBase(a_event->baseObj) ? CPatch_CloaksDP_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k2);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, Section k_value) {
		using enum Section;

		switch (k_value)
		{
		case k1: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_Cloaks_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_Cloaks_Items::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_Cloaks_Items::Data.GetAllVariations()) {
				if (CPatch_Cloaks_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(Items_FormArray, CPatch_Cloaks_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			break;
		}

		case k2: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_CloaksDP_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kCollected, CPatch_CloaksDP_Items::Data.GetForm(a_eventID)->GetName());
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CloaksDP_Items::Data.GetAllVariations()) {
				if (CPatch_CloaksDP_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItemsDP_FormArray, CPatch_CloaksDP_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItemsDP_FormArray.begin(), t_pos);
			ItemsDP_BoolArray[b_pos] = true;

			ItemsDP_EntriesFound = std::ranges::count(ItemsDP_BoolArray, true);
			break;
		}

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {	

		CPatch_Cloaks_Items::Data.CompileFormArray(CPatch_Cloaks::ItemsSK, modnameSK);
		CPatch_Cloaks_Items::Data.MergeAsCollectable();

		CPatch_CloaksDP_Items::Data.CompileFormArray(CPatch_Cloaks::ItemsDP, modnameSK);
		CPatch_CloaksDP_Items::Data.MergeAsCollectable();

		CPatch_Cloaks_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_CloaksDP_Items::Data.Populate(ItemsDP_NameArray, ItemsDP_FormArray, ItemsDP_BoolArray, ItemsDP_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		ItemsDP_EntriesTotal = ItemsDP_FormArray.size();
		ItemsDP_EntriesFound = std::ranges::count(ItemsDP_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : Items_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageCloaksOfSkyrim", std::to_underlying(EntryCategory::kItem)));
		}

		for (auto& name : ItemsDP_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageCloaksOfSkyrim", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modnameSK)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_Cloaks_Items::Data);
		}

		for (auto i = 0; i < ItemsDP_FormArray.size(); i++) {
			ItemsDP_BoolArray[i] = FrameworkAPI::IsItemKnown(ItemsDP_FormArray[i], &CPatch_CloaksDP_Items::Data);
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		ItemsDP_EntriesTotal = ItemsDP_FormArray.size();
		ItemsDP_EntriesFound = std::ranges::count(ItemsDP_BoolArray, true);
	}
}