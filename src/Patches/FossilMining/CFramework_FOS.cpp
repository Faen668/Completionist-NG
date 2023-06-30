#include "Serialization.hpp"
#include "CFramework_FOS.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_FOS {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = {
	0x000D73,0x000D8A,0x000D8C,0x000D96,0x000DAB,0x005907,0x005909,
	0x007B9B,0x03D814,0x03D81B,0x03D81C,0x03D81F,0x03D820,0x03D821,
	0x03D823,0x03D825,0x03D82F,0x03D850,0x03D875,0x03E915,
	};
	
	constexpr Serialization::FormArray ItmL2 = {
	0x000D62,0x000D6F,0x000D78,0x000D7C,0x000D82,0x000D8F,0x000D94,
	0x000D9C,0x03D81A,0x03D81D,0x03D81E,0x03D824,0x03D826,0x03D828,
	0x03D829,0x03D82A,0x03D82C,0x03D82D,0x03D82E,0x03D830,0x03D831,
	0x03D832,0x03D833,0x03D834,0x03D837,0x03D838,0x03D83B,0x03D83C,
	0x03D83E,0x03D83F,0x03D840,0x03D841,0x03D842,0x03D844,0x03D848,
	0x03D849,0x03D84A,0x03D84B,0x03D84E,0x03D852,0x03D863,0x03D86E,
	0x03D87B,0x040456,0x04045A,
	};

	constexpr Serialization::FormArray ItmL3 = {
	0x000D71,0x000D74,0x000D7A,0x000D7E,0x000D84,0x000D86,0x000D88,
	0x000D8E,0x000D92,0x000D98,0x000D9A,0x000D9E,0x005901,0x005902,
	0x005903,0x005904,0x005905,0x005906,0x005908,0x00590A,0x00590B,
	0x00590C,0x00590D,0x00590E,0x00760E,0x007611,0x007615,0x007617,
	0x007619,0x007B86,0x007B99,0x008100,0x008105,0x008107,0x008109,
	0x00810B,0x00810E,0x008110,0x008112,0x03D815,0x03D816,0x03D817,
	0x03D818,0x03D819,0x03D822,0x03D827,0x03D82B,0x03D835,0x03D836,
	0x03D839,0x03D83A,0x03D83D,0x03D843,0x03D845,0x03D846,0x03D847,
	0x03D84C,0x03D84D,0x03D84F,0x03D854,0x03D855,0x03D856,0x03D857,
	0x03D858,0x03D859,0x03D85A,0x03D85B,0x03D85C,0x03D85D,0x03D85E,
	0x03D85F,0x03D860,0x03D861,0x03D862,0x03D864,0x03D865,0x03D866,
	0x03D867,0x03D868,0x03D869,0x03D86A,0x03D86B,0x03D86C,0x03D86D,
	0x03D86F,0x03D870,0x03D871,0x03D873,0x03D874,0x03D876,0x03D877,
	0x03D878,0x03D879,0x03D87A,0x03D87C,0x03D87D,0x03D87E,0x040457,
	0x040458,
	};
	// clang-format on

	constexpr std::string_view modname = "Fossilsyum.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
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

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CPatch_FOS_ItmL1::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_FOS_ItmL1::Data.GetBase(a_event->baseObj) ? CPatch_FOS_ItmL1::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_FOS_ItmL1");
			return EventResult::kContinue;
		}

		if (CPatch_FOS_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_FOS_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_FOS_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_FOS_ItmL2");
			return EventResult::kContinue;
		}

		if (CPatch_FOS_ItmL3::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_FOS_ItmL3::Data.GetBase(a_event->baseObj) ? CPatch_FOS_ItmL3::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_FOS_ItmL3");
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "CPatch_FOS_ItmL1") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FOS_ItmL1::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FOS_ItmL1::Data.GetAllVariations()) {
				if (CPatch_FOS_ItmL1::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL1_FormArray, CPatch_FOS_ItmL1::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_FormArray.begin(), t_pos);
			ItmL1_BoolArray[b_pos] = true;

			ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_FOS_ItmL2") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FOS_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FOS_ItmL2::Data.GetAllVariations()) {
				if (CPatch_FOS_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_FOS_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_FOS_ItmL3") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_FOS_ItmL3::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_FOS_ItmL3::Data.GetAllVariations()) {
				if (CPatch_FOS_ItmL3::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL3_FormArray, CPatch_FOS_ItmL3::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_FormArray.begin(), t_pos);
			ItmL3_BoolArray[b_pos] = true;

			ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_FOS_ItmL1::Data.CompileFormArray(CPatch_FOS::ItmL1, modname);
		CPatch_FOS_ItmL2::Data.CompileFormArray(CPatch_FOS::ItmL2, modname);
		CPatch_FOS_ItmL3::Data.CompileFormArray(CPatch_FOS::ItmL3, modname);

		CPatch_FOS_ItmL1::Data.MergeAsCollectable();
		CPatch_FOS_ItmL2::Data.MergeAsCollectable();
		CPatch_FOS_ItmL3::Data.MergeAsCollectable();

		CPatch_FOS_ItmL1::Data.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		CPatch_FOS_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_FOS_ItmL3::Data.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : ItmL1_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFossilMining1", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto& name : ItmL2_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFossilMining2", std::to_underlying(EntryCategory::kItem)));
		}
		for (auto& name : ItmL3_NameArray) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageFossilMining3", std::to_underlying(EntryCategory::kItem)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < ItmL1_FormArray.size(); i++) {
			ItmL1_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_FormArray[i], &CPatch_FOS_ItmL1::Data);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &CPatch_FOS_ItmL2::Data);
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			ItmL3_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL3_FormArray[i], &CPatch_FOS_ItmL3::Data);
		}

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
	}
}