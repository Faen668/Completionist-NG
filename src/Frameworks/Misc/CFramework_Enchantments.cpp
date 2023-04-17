#include "CFramework_Enchantments.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Enchantments {

	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray Forms_VA = {
	0x10FB70,0x10FB71,0x10FB7E,0x10FB84,0x10FB72,0x10FB73,0x10FB74,0x10FB75,
	0x10FB77,0x10FB78,0x10FB79,0x10FB7A,0x10FB7B,0x10FB7C,0x10FB7F,0x10FB80,
	0x10FB81,0x10FB82,0x10FB83,0x10FB85,0x10FB87,0x10FB88,0x10FB89,0x10FB76,
	0x10FB7D,0x10FB86,0x10FB8A,0x10FB8B,0x10FB8C,0x10FB8D,0x10FB8E,0x10FB8F,
	0x10FB90, 
	};

	constexpr Serialization::FormArray Forms_SA = {
	0x051266,0x0512AD,0x0512B5,0x0512CA,0x0512C3,0x0512BC,0x3473E3,0x051552,
	0x05128B,0x1E9CFF,0x051493,0x328D7F,0x328D81,0x393519,0x051513,0x3892DD,
	0x0512FF,0x051303,0x3841AC,0x3892C8,0x051368,0x0513CE,0x05155C,0x051278,
	0x051291,0x0514ED,0x3841A3,0x3841A8,0x38419E,0x2A5326,0x3892CC,0x328D91,
	0x051347,0x0513C1,0x1EEE21,0x1EEE1F,0x32DEB3,0x3892D6,0x051583,0x0512A5,
	0x3841B1,0x3892B8,0x3892BC,0x05139F,0x051500,0x39D738,0x328D95,0x05135F,
	0x051544,0x051362,0x051470,0x0514E1,0x05151B,0x0513A7,0x3473ED,0x32DEB9,
	0x3422D1,0x051328,0x051319,0x05126A,0x0B9EEF,0x1E9D01,0x193B1B,0x3892CA,
	0x051284,0x051536,0x38E40C,0x38E410,0x0513D6,0x051524,0x356785,
	};

	constexpr Serialization::FormArray Forms_VW = {
	0x10FB91,0x10FB9D,0x10FB92,0x10FB93,0x10FB94,0x10FB95,0x10FB96,0x10FB97,
	0x10FB98,0x10FB99,0x10FB9A,0x10FB9B,0x10FB9C,0x040003,0x105830,0x03B0B2,
	};

	constexpr Serialization::FormArray Forms_DW = {
	0x02C46E,
	};

	constexpr Serialization::FormArray Forms_SW = {
	0x0510DC,0x0510E0,0x0513F8,0x0511F4,0x051432,0x05143E,0x05143F,0x05143A,
	0x05143B,0x051218,0x051104,0x051177,0x051132,0x05150B,0x051188,0x0511CB,
	0x0513FF,0x05118E,0x05119D,0x0510DA,0x051157,0x0511D6,0x051229,0x0511FE,
	0x051241,0x05115E,0x05110B,0x0511C4,0x0511A7,0x0510ED,0x05117E,0x05112A,
	0x051208,0x05142A,0x051221,0x05122E,0x05125A,0x0513EF,0x0510F1,0x05140E,
	0x25E4D2,0x051213,0x0511E9,0x051242,0x05141E,0x051150,0x0510F9,0x25E4D5,
	0x05115F,0x051115,0x0510FC,0x0511AE,
	};

	constexpr Serialization::FormArray Forms_NGA = {
	0x00087E,0x0008CC,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		for (auto i = 0; i < VA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(VA_FormArray[i]); enchantment && enchantment->GetKnown()) {
				VA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < SA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(SA_FormArray[i]); enchantment && enchantment->GetKnown()) {
				SA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < VW_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(VW_FormArray[i]); enchantment && enchantment->GetKnown()) {
				VW_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < SW_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(SW_FormArray[i]); enchantment && enchantment->GetKnown()) {
				SW_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < NGA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(NGA_FormArray[i]); enchantment && enchantment->GetKnown()) {
				NGA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		CHandler::UpdateCounts();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CFramework_Enchantments_VA::Data.CompileFormArray(CFramework_Enchantments::Forms_VA, "Skyrim.esm");
		CFramework_Enchantments_VW::Data.CompileFormArray(CFramework_Enchantments::Forms_VW, "Skyrim.esm");
		CFramework_Enchantments_VW::Data.CompileFormArray(CFramework_Enchantments::Forms_DW, "Dragonborn.esm");

		CFramework_Enchantments_SA::Data.CompileFormArray(CFramework_Enchantments::Forms_SA, "Summermyst - Enchantments of Skyrim.esp");
		CFramework_Enchantments_SW::Data.CompileFormArray(CFramework_Enchantments::Forms_SW, "Summermyst - Enchantments of Skyrim.esp");

		if (Serialization::CompletionistData::IsModInstalled("ccvsvsse003-necroarts.esl"))
		{
			CFramework_Enchantments_NGA::Data.CompileFormArray(CFramework_Enchantments::Forms_NGA, "ccvsvsse003-necroarts.esl");
			CFramework_Enchantments_NGA::Data.Populate(NGA_NameArray, NGA_FormArray, NGA_BoolArray, NGA_TextArray);
		}

		CFramework_Enchantments_VA::Data.Populate(VA_NameArray, VA_FormArray, VA_BoolArray, VA_TextArray);
		CFramework_Enchantments_VW::Data.Populate(VW_NameArray, VW_FormArray, VW_BoolArray, VW_TextArray);
		CFramework_Enchantments_SA::Data.Populate(SA_NameArray, SA_FormArray, SA_BoolArray, SA_TextArray);
		CFramework_Enchantments_SW::Data.Populate(SW_NameArray, SW_FormArray, SW_BoolArray, SW_TextArray);

		CHandler::UpdateCounts();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < VA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(VA_FormArray[i]); enchantment && (enchantment->GetKnown() || FoundItemData_NoShow.HasForm(enchantment->GetFormID()))) {
				VA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < SA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(SA_FormArray[i]); enchantment && (enchantment->GetKnown() || FoundItemData_NoShow.HasForm(enchantment->GetFormID()))) {
				SA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < VW_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(VW_FormArray[i]); enchantment && (enchantment->GetKnown() || FoundItemData_NoShow.HasForm(enchantment->GetFormID()))) {
				VW_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < SW_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(SW_FormArray[i]); enchantment && (enchantment->GetKnown() || FoundItemData_NoShow.HasForm(enchantment->GetFormID()))) {
				SW_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}

		for (auto i = 0; i < NGA_FormArray.size(); i++) {
			if (auto* enchantment = static_cast<RE::EnchantmentItem*>(NGA_FormArray[i]); enchantment && (enchantment->GetKnown() || FoundItemData_NoShow.HasForm(enchantment->GetFormID()))) {
				NGA_BoolArray[i] = true;
				FoundItemData_NoShow.AddForm(enchantment);
			}
		}
		CHandler::UpdateCounts();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Counts ) ----------
	//---------------------------------------------------

	void CHandler::UpdateCounts() {

		VA_EntriesTotal = VA_FormArray.size();
		VA_EntriesFound = std::ranges::count(VA_BoolArray, true);

		SA_EntriesTotal = SA_FormArray.size();
		SA_EntriesFound = std::ranges::count(SA_BoolArray, true);

		VW_EntriesTotal = VW_FormArray.size();
		VW_EntriesFound = std::ranges::count(VW_BoolArray, true);

		SW_EntriesTotal = SW_FormArray.size();
		SW_EntriesFound = std::ranges::count(SW_BoolArray, true);

		NGA_EntriesTotal = NGA_FormArray.size();
		NGA_EntriesFound = std::ranges::count(NGA_BoolArray, true);
	}
}