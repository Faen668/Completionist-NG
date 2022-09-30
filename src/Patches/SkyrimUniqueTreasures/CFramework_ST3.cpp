#include "Serialization.hpp"
#include "CFramework_ST3.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_ST3_ItmL1 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL2 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL3 {
	Serialization::CompletionistData Data;
}

namespace CPatch_ST3_ItmL4 {
	Serialization::CompletionistData Data;
}

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

		if (CPatch_ST3_ItmL1::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST3_ItmL1::Data.GetBase(a_event->baseObj) ? CPatch_ST3_ItmL1::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST3_ItmL1");
			return EventResult::kContinue;
		}

		if (CPatch_ST3_ItmL2::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST3_ItmL2::Data.GetBase(a_event->baseObj) ? CPatch_ST3_ItmL2::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST3_ItmL2");
			return EventResult::kContinue;
		}

		if (CPatch_ST3_ItmL3::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST3_ItmL3::Data.GetBase(a_event->baseObj) ? CPatch_ST3_ItmL3::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST3_ItmL3");
			return EventResult::kContinue;
		}

		if (CPatch_ST3_ItmL4::Data.HasForm(a_event->baseObj)) {
			auto base = CPatch_ST3_ItmL4::Data.GetBase(a_event->baseObj) ? CPatch_ST3_ItmL4::Data.GetBase(a_event->baseObj) : a_event->baseObj;
			CHandler::ProcessFoundForm(base, a_event->baseObj, "CPatch_ST3_ItmL4");
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "CPatch_ST3_ItmL1") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST3_ItmL1::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST3_ItmL1::Data.GetAllVariations()) {
				if (CPatch_ST3_ItmL1::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL1_FormArray, CPatch_ST3_ItmL1::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL1_FormArray.begin(), t_pos);
			ItmL1_BoolArray[b_pos] = true;

			ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST3_ItmL2") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST3_ItmL2::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST3_ItmL2::Data.GetAllVariations()) {
				if (CPatch_ST3_ItmL2::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL2_FormArray, CPatch_ST3_ItmL2::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL2_FormArray.begin(), t_pos);
			ItmL2_BoolArray[b_pos] = true;

			ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST3_ItmL3") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST3_ItmL3::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST3_ItmL3::Data.GetAllVariations()) {
				if (CPatch_ST3_ItmL3::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL3_FormArray, CPatch_ST3_ItmL3::Data.GetForm(a_baseID));
			auto b_pos = std::distance(ItmL3_FormArray.begin(), t_pos);
			ItmL3_BoolArray[b_pos] = true;

			ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);
			return;
		}

		//---------------------------------------------------
		//---------------------------------------------------

		if (a_variable == "CPatch_ST3_ItmL4") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_ST3_ItmL4::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, "NotifyItems");
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_ST3_ItmL4::Data.GetAllVariations()) {
				if (CPatch_ST3_ItmL4::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(ItmL4_FormArray, CPatch_ST3_ItmL4::Data.GetForm(a_baseID));
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

		CPatch_ST3_ItmL1::Data.CompileFormArray(CPatch_ST3::ItmL1, modname);
		CPatch_ST3_ItmL2::Data.CompileFormArray(CPatch_ST3::ItmL2, modname);
		CPatch_ST3_ItmL3::Data.CompileFormArray(CPatch_ST3::ItmL3, modname);
		CPatch_ST3_ItmL4::Data.CompileFormArray(CPatch_ST3::ItmL4, modname);

		CPatch_ST3_ItmL1::Data.MergeAsCollectable();
		CPatch_ST3_ItmL2::Data.MergeAsCollectable();
		CPatch_ST3_ItmL3::Data.MergeAsCollectable();
		CPatch_ST3_ItmL4::Data.MergeAsCollectable();

		CPatch_ST3_ItmL1::Data.Populate(ItmL1_NameArray, ItmL1_FormArray, ItmL1_BoolArray, ItmL1_TextArray);
		CPatch_ST3_ItmL2::Data.Populate(ItmL2_NameArray, ItmL2_FormArray, ItmL2_BoolArray, ItmL2_TextArray);
		CPatch_ST3_ItmL3::Data.Populate(ItmL3_NameArray, ItmL3_FormArray, ItmL3_BoolArray, ItmL3_TextArray);
		CPatch_ST3_ItmL4::Data.Populate(ItmL4_NameArray, ItmL4_FormArray, ItmL4_BoolArray, ItmL4_TextArray);

		ItmL1_EntriesTotal = ItmL1_FormArray.size();
		ItmL1_EntriesFound = std::ranges::count(ItmL1_BoolArray, true);

		ItmL2_EntriesTotal = ItmL2_FormArray.size();
		ItmL2_EntriesFound = std::ranges::count(ItmL2_BoolArray, true);

		ItmL3_EntriesTotal = ItmL3_FormArray.size();
		ItmL3_EntriesFound = std::ranges::count(ItmL3_BoolArray, true);

		ItmL4_EntriesTotal = ItmL4_FormArray.size();
		ItmL4_EntriesFound = std::ranges::count(ItmL4_BoolArray, true);

		//using namespace FrameworkHandler;
		//RegisterAs<FrameworkID::kST3_ItmL1>(&ItmL1_NameArray, &ItmL1_FormArray, ItmL1_BoolArray, &ItmL1_TextArray);
		//RegisterAs<FrameworkID::kST3_ItmL2>(&ItmL2_NameArray, &ItmL2_FormArray, ItmL2_BoolArray, &ItmL2_TextArray);
		//RegisterAs<FrameworkID::kST3_ItmL3>(&ItmL3_NameArray, &ItmL3_FormArray, ItmL3_BoolArray, &ItmL3_TextArray);
		//RegisterAs<FrameworkID::kST3_ItmL4>(&ItmL4_NameArray, &ItmL4_FormArray, ItmL4_BoolArray, &ItmL4_TextArray);
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