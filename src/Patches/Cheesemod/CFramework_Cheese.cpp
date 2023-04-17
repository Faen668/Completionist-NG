#include "Serialization.hpp"
#include "CFramework_Cheese.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_CHM {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Misc
	0x001e94,0x028aaa,0x000d76,0x00378a,0x00378c,0x0018c9,0x001304,
	0x003773,0x0012f6,0x028aae,0x001909,0x001314,0x0012f8,0x000d6c,
	0x004d3d,0x000d78,0x000d80,0x0018c5,0x0018e5,0x0025cd,0x001300,
	0x001905,0x0018ad,0x0018c3,0x00245c,0x028aac,0x001310,0x029014,
	0x004d88,
	};

	constexpr Serialization::FormArray ItmL2 = { //Sliced
	0x0018df,0x0018f3,0x0018d3,0x001ee6,0x0018cd,0x001ee8,0x001eda,
	0x001ec8,0x001322,0x001e9a,0x001318,0x001e83,0x001eaa,0x004d69,
	0x003782,0x0018f9,0x00130a,0x003788,0x004d7f,0x000d84,0x000d68,
	0x0047d5,0x001ebe,0x0018bd,0x0018b5,0x0018e7,0x001ee0,0x004d91,
	0x001e98,0x001eb0,0x000d74,0x004d43,0x000d8e,0x001e75,0x001ece,
	0x001e7d,0x001e88,0x00131c,0x001ea6,0x0012fa,0x0018ed,0x000d7a,
	0x004d5e,0x000d6e,0x001e9c,0x00377b,0x001e6f,0x001ed6,0x001eb8,
	0x00190b,0x004d78,0x0018db,0x001e8e,0x003775,0x001ec4,0x0018cb,
	};

	constexpr Serialization::FormArray ItmL3 = { // Wedges
	0x0018e3,0x0018f7,0x001eac,0x0018d7,0x004d70,0x0018b1,0x003786,
	0x004d66,0x004d8e,0x0018fd,0x001316,0x00130e,0x004d85,0x0018c7,
	0x004d5b,0x00131a,0x000d86,0x004d75,0x00245a,0x000d66,0x0047d9,
	0x001901,0x0018d1,0x001ec2,0x0018c1,0x004d8b,0x0018b9,0x0018eb,
	0x001ee4,0x0018b3,0x004d9b,0x0018bb,0x001312,0x004d4a,0x001308,
	0x001eb4,0x000d64,0x004d45,0x001eec,0x000d92,0x001e79,0x001ed2,
	0x001306,0x001e81,0x001e8c,0x001320,0x001ea8,0x0012fe,0x0018f1,
	0x0018af,0x000d7e,0x004d61,0x000d62,0x001ea2,0x004d9d,0x001ede,
	0x003780,0x001e73,0x001e71,0x001ef1,0x001ed8,0x001ebc,0x001e92,
	0x001e86,0x001ecc,0x000d88,0x004d7d,0x0018dd,0x001eb6,0x003779,
	};

	constexpr Serialization::FormArray ItmL4 = { // Wheels
	0x0018e1,0x0018f5,0x001eae,0x001903,0x0018d5,0x004d6c,0x003784,
	0x0018fb,0x00130c,0x004d82,0x0018ff,0x001e7b,0x000d82,0x002458,
	0x000d6a,0x0047d7,0x0018cf,0x001ec0,0x0018bf,0x0018b7,0x0018e9,
	0x001ee2,0x004d97,0x001e96,0x001eb2,0x000d72,0x004d47,0x001eea,
	0x000d90,0x001e77,0x001ed0,0x001e7f,0x001e8a,0x00131e,0x001ea4,
	0x0012fc,0x0018ef,0x000d7c,0x004d63,0x000d70,0x001ea0,0x001edc,
	0x00377e,0x001eef,0x001ed4,0x001eba,0x004d56,0x001eca,0x000d8c,
	0x004d7b,0x0018d9,0x001e90,0x003777,
	};

	// clang-format on

	constexpr std::string_view modname = "yumcheese.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
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
		using enum Section;

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CPatch_CHM_ItmL1::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_CHM_ItmL1::Data.GetBase(a_event->baseObj) ? CPatch_CHM_ItmL1::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k1);
			return EventResult::kContinue;
		}

		if (CPatch_CHM_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_CHM_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_CHM_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k2);
			return EventResult::kContinue;
		}

		if (CPatch_CHM_ItmL3::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_CHM_ItmL3::Data.GetBase(a_event->baseObj) ? CPatch_CHM_ItmL3::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k3);
			return EventResult::kContinue;
		}

		if (CPatch_CHM_ItmL4::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_CHM_ItmL4::Data.GetBase(a_event->baseObj) ? CPatch_CHM_ItmL4::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, k4);
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
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_CHM_ItmL1::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CHM_ItmL1::Data.GetAllVariations()) {
				if (CPatch_CHM_ItmL1::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL1_FormArray, CPatch_CHM_ItmL1::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_FormArray.begin(), t_pos);
			ItmL1_BoolArray[b_pos] = true;

			ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
		}
			break;

		case k2: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_CHM_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CHM_ItmL2::Data.GetAllVariations()) {
				if (CPatch_CHM_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_CHM_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
		}
			break;
		case k3: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_CHM_ItmL3::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CHM_ItmL3::Data.GetAllVariations()) {
				if (CPatch_CHM_ItmL3::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL3_FormArray, CPatch_CHM_ItmL3::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_FormArray.begin(), t_pos);
			ItmL3_BoolArray[b_pos] = true;

			ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
		}
			break;
		case k4: {
			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_CHM_ItmL4::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_CHM_ItmL4::Data.GetAllVariations()) {
				if (CPatch_CHM_ItmL4::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}

			auto t_pos = std::ranges::find(ItmL4_FormArray, CPatch_CHM_ItmL4::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL4_FormArray.begin(), t_pos);
			ItmL4_BoolArray[b_pos] = true;

			ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
		}
			break;

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_CHM_ItmL1::Data.CompileFormArray(CPatch_CHM::ItmL1, modname);
		CPatch_CHM_ItmL2::Data.CompileFormArray(CPatch_CHM::ItmL2, modname);
		CPatch_CHM_ItmL3::Data.CompileFormArray(CPatch_CHM::ItmL3, modname);
		CPatch_CHM_ItmL4::Data.CompileFormArray(CPatch_CHM::ItmL4, modname);

		CPatch_CHM_ItmL1::Data.MergeAsCollectable();
		CPatch_CHM_ItmL2::Data.MergeAsCollectable();
		CPatch_CHM_ItmL3::Data.MergeAsCollectable();
		CPatch_CHM_ItmL4::Data.MergeAsCollectable();

		CPatch_CHM_ItmL1::Data.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		CPatch_CHM_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_CHM_ItmL3::Data.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		CPatch_CHM_ItmL4::Data.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);

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
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < ItmL1_FormArray.size(); i++) {
			ItmL1_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_FormArray[i], &CPatch_CHM_ItmL1::Data);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &CPatch_CHM_ItmL2::Data);
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			ItmL3_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL3_FormArray[i], &CPatch_CHM_ItmL3::Data);
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			ItmL4_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL4_FormArray[i], &CPatch_CHM_ItmL4::Data);
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