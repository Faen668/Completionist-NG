#include "Serialization.hpp"
#include "CFramework_AHD.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CPatch_AHD_Items {
	Serialization::CompletionistData Data;
}

namespace CPatch_AHD {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Items = {
	0x000814,0x000809,0x000D62,0x000D6A,0x000D63,0x00081D,0x00080A,0x000D64,
	0x000815,0x000816,0x000817,0x000818,0x00080B,0x00081E,0x000819,0x00081A,
	0x00083E,0x00081B,0x00081C,0x00081F,0x000826,0x00080C,0x000D65,0x000D66,
	0x000820,0x00080D,0x000827,0x00080E,0x000D67,0x000829,0x000D68,0x000821,
	0x000822,0x000810,0x000828,0x00080F,0x000811,0x000D69,0x000823,0x000824,
	0x000825,
	};

	// clang-format on

	inline std::vector<std::string> Items_NameArray;
	inline std::vector<std::string> Items_TextArray;
	inline std::vector<RE::TESForm*> Items_FormArray;
	inline std::vector<bool> Items_BoolArray;
	inline std::int32_t Items_EntriesTotal;
	inline std::int32_t Items_EntriesFound;

	inline std::string_view modname = "AdditionalHearthfireDolls.esp";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
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

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_AHD_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_AHD_Items::Data.GetBase(a_event->baseObj) ? CPatch_AHD_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CPatch_AHD_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_AHD_Items::Data.GetAllVariations()) {
				if (CPatch_AHD_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_AHD_Items::Data.GetForm(a_baseID));
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

		CPatch_AHD_Items::Data.CompileFormArray(CPatch_AHD::Items, modname);
		CPatch_AHD_Items::Data.MergeAsCollectable();

		CPatch_AHD_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (const auto Mod = Serialization::CompletionistData::IsModInstalled(modname); !Mod) {
			return;
		}

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			if (FoundItemData.HasForm(Items_FormArray[i]->GetFormID())) {
				Items_BoolArray[i] = true;
			}
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
	}
}