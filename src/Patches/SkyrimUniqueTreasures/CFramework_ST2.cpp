#include "Serialization.hpp"
#include "CFramework_ST2.hpp"
#include "Patches/PatchMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_ST2_ItmL1 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST2_ItmL2 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST2_ItmL3 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST2_ItmL4 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST2 {

	using namespace CFramework_Master;
	using namespace CPatch_Master;

	// clang-format off

	constexpr Serialization::FormArray ItmL1 = { // Skulls
	0x0064DF,0x013D45,0x013268,0x013D47,0x013D49,
	0x0064D9,0x013D4B,0x013D4D,0x01326A,
	};

	constexpr Serialization::FormArray ItmL2 = { // Glass
	0x006A5B,0x006A53,0x006A55,0x006A57,0x006A50,
	0x016DD3,0x016DD2,0x01686E,0x0178A4,
	};

	constexpr Serialization::FormArray ItmL3 = { // Gems
	0x012D02,0x0049A7,0x0049A9,0x0049AB,0x0049AE,
	0x0049B4,0x0049B0,0x0049B2,0x000DDA,
	};

	constexpr Serialization::FormArray ItmL4 = { // Wine Glasses
	0x013D33,0x013D3F,0x013D35,0x013266,0x013D37,
	0x013D39,0x013D3B,0x013D3D,0x013D41,0x013D43,
	};

	// clang-format on

	inline std::vector<std::string> ItmL1_NameArray;
	inline std::vector<std::string> ItmL1_TextArray;
	inline std::vector<RE::TESForm*> ItmL1_FormArray;
	inline std::vector<bool> ItmL1_BoolArray;
	inline std::int32_t ItmL1_EntriesTotal;
	inline std::int32_t ItmL1_EntriesFound;

	inline std::vector<std::string> ItmL2_NameArray;
	inline std::vector<std::string> ItmL2_TextArray;
	inline std::vector<RE::TESForm*> ItmL2_FormArray;
	inline std::vector<bool> ItmL2_BoolArray;
	inline std::int32_t ItmL2_EntriesTotal;
	inline std::int32_t ItmL2_EntriesFound;

	inline std::vector<std::string> ItmL3_NameArray;
	inline std::vector<std::string> ItmL3_TextArray;
	inline std::vector<RE::TESForm*> ItmL3_FormArray;
	inline std::vector<bool> ItmL3_BoolArray;
	inline std::int32_t ItmL3_EntriesTotal;
	inline std::int32_t ItmL3_EntriesFound;

	inline std::vector<std::string> ItmL4_NameArray;
	inline std::vector<std::string> ItmL4_TextArray;
	inline std::vector<RE::TESForm*> ItmL4_FormArray;
	inline std::vector<bool> ItmL4_BoolArray;
	inline std::int32_t ItmL4_EntriesTotal;
	inline std::int32_t ItmL4_EntriesFound;

	inline std::string_view modname = "SkyrimsUniqueTreasures.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

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

		if (CPatch_ST2_ItmL1::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST2_ItmL1::Data.GetBase(a_event->baseObj) ? CPatch_ST2_ItmL1::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST2_ItmL1");
			return EventResult::kContinue;
		}

		if (CPatch_ST2_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST2_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_ST2_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST2_ItmL2");
			return EventResult::kContinue;
		}

		if (CPatch_ST2_ItmL3::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST2_ItmL3::Data.GetBase(a_event->baseObj) ? CPatch_ST2_ItmL3::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST2_ItmL3");
			return EventResult::kContinue;
		}

		if (CPatch_ST2_ItmL4::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST2_ItmL4::Data.GetBase(a_event->baseObj) ? CPatch_ST2_ItmL4::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST2_ItmL4");
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "CPatch_ST2_ItmL1") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST2_ItmL1::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST2_ItmL1::Data.GetAllVariations()) {
				if (CPatch_ST2_ItmL1::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL1_FormArray, CPatch_ST2_ItmL1::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_FormArray.begin(), t_pos);
			ItmL1_BoolArray[b_pos] = true;

			ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST2_ItmL2") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST2_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST2_ItmL2::Data.GetAllVariations()) {
				if (CPatch_ST2_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_ST2_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST2_ItmL3") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST2_ItmL3::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST2_ItmL3::Data.GetAllVariations()) {
				if (CPatch_ST2_ItmL3::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL3_FormArray, CPatch_ST2_ItmL3::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_FormArray.begin(), t_pos);
			ItmL3_BoolArray[b_pos] = true;

			ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST2_ItmL4") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST2_ItmL4::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST2_ItmL4::Data.GetAllVariations()) {
				if (CPatch_ST2_ItmL4::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL4_FormArray, CPatch_ST2_ItmL4::Data.GetForm(a_baseID));
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

		CPatch_ST2_ItmL1::Data.CompileFormArray(CPatch_ST2::ItmL1, modname);
		CPatch_ST2_ItmL2::Data.CompileFormArray(CPatch_ST2::ItmL2, modname);
		CPatch_ST2_ItmL3::Data.CompileFormArray(CPatch_ST2::ItmL3, modname);
		CPatch_ST2_ItmL4::Data.CompileFormArray(CPatch_ST2::ItmL4, modname);

		CPatch_ST2_ItmL1::Data.MergeAsCollectable();
		CPatch_ST2_ItmL2::Data.MergeAsCollectable();
		CPatch_ST2_ItmL3::Data.MergeAsCollectable();
		CPatch_ST2_ItmL4::Data.MergeAsCollectable();

		CPatch_ST2_ItmL1::Data.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		CPatch_ST2_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_ST2_ItmL3::Data.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		CPatch_ST2_ItmL4::Data.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);

		//using namespace FrameworkHandler;
		//RegisterAs<FrameworkID::kST2_ItmL1>(&ItmL1_NameArray, &ItmL1_FormArray, ItmL1_BoolArray, &ItmL1_TextArray);
		//RegisterAs<FrameworkID::kST2_ItmL2>(&ItmL2_NameArray, &ItmL2_FormArray, ItmL2_BoolArray, &ItmL2_TextArray);
		//RegisterAs<FrameworkID::kST2_ItmL3>(&ItmL3_NameArray, &ItmL3_FormArray, ItmL3_BoolArray, &ItmL3_TextArray);
		//RegisterAs<FrameworkID::kST2_ItmL4>(&ItmL4_NameArray, &ItmL4_FormArray, ItmL4_BoolArray, &ItmL4_TextArray);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		for (auto i = 0; i < ItmL1_FormArray.size(); i++) {
			if (FoundItemData.HasForm(ItmL1_FormArray[i]->GetFormID())) {
				ItmL1_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < ItmL2_FormArray.size(); i++) {
			if (FoundItemData.HasForm(ItmL2_FormArray[i]->GetFormID())) {
				ItmL2_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < ItmL3_FormArray.size(); i++) {
			if (FoundItemData.HasForm(ItmL3_FormArray[i]->GetFormID())) {
				ItmL3_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < ItmL4_FormArray.size(); i++) {
			if (FoundItemData.HasForm(ItmL4_FormArray[i]->GetFormID())) {
				ItmL4_BoolArray[i] = true;
			}
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