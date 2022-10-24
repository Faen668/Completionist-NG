#include "Serialization.hpp"
#include "CFramework_ST1.hpp"
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

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		if (CPatch_ST1_ItmL1::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST1_ItmL1::Data.GetBase(a_event->baseObj) ? CPatch_ST1_ItmL1::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST1_ItmL1");
			return EventResult::kContinue;
		}

		if (CPatch_ST1_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST1_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_ST1_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST1_ItmL2");
			return EventResult::kContinue;
		}

		if (CPatch_ST1_ItmL3::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST1_ItmL3::Data.GetBase(a_event->baseObj) ? CPatch_ST1_ItmL3::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST1_ItmL3");
			return EventResult::kContinue;
		}

		if (CPatch_ST1_ItmL4::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST1_ItmL4::Data.GetBase(a_event->baseObj) ? CPatch_ST1_ItmL4::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST1_ItmL4");
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "CPatch_ST1_ItmL1") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST1_ItmL1::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST1_ItmL1::Data.GetAllVariations()) {
				if (CPatch_ST1_ItmL1::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL1_FormArray, CPatch_ST1_ItmL1::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_FormArray.begin(), t_pos);
			ItmL1_BoolArray[b_pos] = true;

			ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST1_ItmL2") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST1_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST1_ItmL2::Data.GetAllVariations()) {
				if (CPatch_ST1_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_ST1_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST1_ItmL3") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST1_ItmL3::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST1_ItmL3::Data.GetAllVariations()) {
				if (CPatch_ST1_ItmL3::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL3_FormArray, CPatch_ST1_ItmL3::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_FormArray.begin(), t_pos);
			ItmL3_BoolArray[b_pos] = true;

			ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST1_ItmL4") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST1_ItmL4::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST1_ItmL4::Data.GetAllVariations()) {
				if (CPatch_ST1_ItmL4::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL4_FormArray, CPatch_ST1_ItmL4::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL4_FormArray.begin(), t_pos);
			ItmL4_BoolArray[b_pos] = true;

			ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_ST1_ItmL1::Data.CompileFormArray(CPatch_ST1::ItmL1, modname);
		CPatch_ST1_ItmL2::Data.CompileFormArray(CPatch_ST1::ItmL2, modname);
		CPatch_ST1_ItmL3::Data.CompileFormArray(CPatch_ST1::ItmL3, modname);
		CPatch_ST1_ItmL4::Data.CompileFormArray(CPatch_ST1::ItmL4, modname);

		CPatch_ST1_ItmL1::Data.MergeAsCollectable();
		CPatch_ST1_ItmL2::Data.MergeAsCollectable();
		CPatch_ST1_ItmL3::Data.MergeAsCollectable();
		CPatch_ST1_ItmL4::Data.MergeAsCollectable();

		CPatch_ST1_ItmL1::Data.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		CPatch_ST1_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_ST1_ItmL3::Data.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		CPatch_ST1_ItmL4::Data.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);

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
			ItmL1_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL1_FormArray[i], &CPatch_ST1_ItmL1::Data);
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			ItmL2_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL2_FormArray[i], &CPatch_ST1_ItmL2::Data);
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			ItmL3_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL3_FormArray[i], &CPatch_ST1_ItmL3::Data);
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			ItmL4_BoolArray[i] = FrameworkAPI::IsItemKnown(ItmL4_FormArray[i], &CPatch_ST1_ItmL4::Data);
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