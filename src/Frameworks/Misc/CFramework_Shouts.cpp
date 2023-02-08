#include "Internal Utility/Variables.hpp"
#include "CFramework_Shouts.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Shouts {
	using namespace CFramework_Master;
	using namespace CVariables;

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
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESSpellCastEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Activated ) ---------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>*) {

		if (!a_event || !a_event->spell || a_event->object != RE::PlayerCharacter::GetSingleton()) { return EventResult::kContinue; }

		if (CFramework_Shouts_VS::Data.HasForm(a_event->spell)) {
			ProcessFoundForm(Section::kVanilla, a_event->spell);
			return EventResult::kContinue;
		}

		if (CFramework_Shouts_TS::Data.HasForm(a_event->spell)) {
			ProcessFoundForm(Section::kThunderchild, a_event->spell);
			return EventResult::kContinue;
		}

		if (CFramework_Shouts_MS::Data.HasForm(a_event->spell)) {
			ProcessFoundForm(Section::kMiscellaneous, a_event->spell);
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(Section a_section, RE::FormID a_formID) {

		auto* Spell = static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(a_formID));
		if (!Spell) { return; };

		switch (a_section)
		{
		case CFramework_Shouts::Section::kVanilla: {

			if (auto a_pos = std::ranges::find(Vanilla_W3_FormArray, Spell); a_pos != Vanilla_W3_FormArray.end()) { // Player Knows All Words
				auto b_pos = std::distance(Vanilla_W3_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[2].word;
				Vanilla_SH_NameOutput[b_pos] = Vanilla_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}";
				
				if (!FoundItemData_NoShow.HasForm(Vanilla_W3_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Vanilla_SH_FormArray[b_pos]->GetName());
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}
				
				FoundItemData_NoShow.AddForm(Vanilla_W3_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Vanilla_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[b_pos]);
				Vanilla_SH_BoolArray[b_pos] = true;
				Vanilla_SH_EntriesFound = std::ranges::count(Vanilla_SH_BoolArray, true);
				return;
			}

			if (auto a_pos = std::ranges::find(Vanilla_W2_FormArray, Spell); a_pos != Vanilla_W2_FormArray.end()) { // Player Knows 2nd Word
				auto b_pos = std::distance(Vanilla_W2_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[2].word;
				Vanilla_SH_NameOutput[b_pos] = Vanilla_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Vanilla_W2_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Vanilla_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout2, Word2->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Vanilla_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[b_pos]);
				return;
			}
			
			if (auto a_pos = std::ranges::find(Vanilla_W1_FormArray, Spell); a_pos != Vanilla_W1_FormArray.end()) { // Player Knows 1st Word
				auto b_pos = std::distance(Vanilla_W1_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[b_pos])->variations[2].word;
				Vanilla_SH_NameOutput[b_pos] = Vanilla_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Vanilla_W1_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Vanilla_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout1, Word1->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[b_pos]);
				return;
			}
			break;
		}
			
		case CFramework_Shouts::Section::kThunderchild: {

			if (auto a_pos = std::ranges::find(Thunderchild_W3_FormArray, Spell); a_pos != Thunderchild_W3_FormArray.end()) { // Player Knows All Words
				auto b_pos = std::distance(Thunderchild_W3_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[2].word;
				Thunderchild_SH_NameOutput[b_pos] = Thunderchild_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Thunderchild_W3_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Thunderchild_SH_FormArray[b_pos]->GetName());
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Thunderchild_W3_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Thunderchild_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[b_pos]);
				Thunderchild_SH_BoolArray[b_pos] = true;
				Thunderchild_SH_EntriesFound = std::ranges::count(Thunderchild_SH_BoolArray, true);
				return;
			}

			if (auto a_pos = std::ranges::find(Thunderchild_W2_FormArray, Spell); a_pos != Thunderchild_W2_FormArray.end()) { // Player Knows 2nd Word
				auto b_pos = std::distance(Thunderchild_W2_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[2].word;
				Thunderchild_SH_NameOutput[b_pos] = Thunderchild_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Thunderchild_W2_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Thunderchild_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout2, Word2->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Thunderchild_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[b_pos]);
				return;
			}

			if (auto a_pos = std::ranges::find(Thunderchild_W1_FormArray, Spell); a_pos != Thunderchild_W1_FormArray.end()) { // Player Knows 1st Word
				auto b_pos = std::distance(Thunderchild_W1_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[b_pos])->variations[2].word;
				Thunderchild_SH_NameOutput[b_pos] = Thunderchild_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Thunderchild_W1_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Thunderchild_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout1, Word1->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[b_pos]);
				return;
			}
			break;
		}

		case CFramework_Shouts::Section::kMiscellaneous: {

			if (auto a_pos = std::ranges::find(Miscellaneous_W3_FormArray, Spell); a_pos != Miscellaneous_W3_FormArray.end()) { // Player Knows All Words
				auto b_pos = std::distance(Miscellaneous_W3_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[2].word;
				Miscellaneous_SH_NameOutput[b_pos] = Miscellaneous_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Miscellaneous_W3_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Miscellaneous_SH_FormArray[b_pos]->GetName());
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Miscellaneous_W3_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[b_pos]);
				Miscellaneous_SH_BoolArray[b_pos] = true;
				Miscellaneous_SH_EntriesFound = std::ranges::count(Miscellaneous_SH_BoolArray, true);
				return;
			}

			if (auto a_pos = std::ranges::find(Miscellaneous_W2_FormArray, Spell); a_pos != Miscellaneous_W2_FormArray.end()) { // Player Knows 2nd Word
				auto b_pos = std::distance(Miscellaneous_W2_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[2].word;
				Miscellaneous_SH_NameOutput[b_pos] = Miscellaneous_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Miscellaneous_W2_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Miscellaneous_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout2, Word2->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Miscellaneous_W2_FormArray[b_pos]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[b_pos]);
				return;
			}

			if (auto a_pos = std::ranges::find(Miscellaneous_W1_FormArray, Spell); a_pos != Miscellaneous_W1_FormArray.end()) { // Player Knows 1st Word
				auto b_pos = std::distance(Miscellaneous_W1_FormArray.begin(), a_pos);

				auto* Word1 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[0].word;
				auto* Word2 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[1].word;
				auto* Word3 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[b_pos])->variations[2].word;
				Miscellaneous_SH_NameOutput[b_pos] = Miscellaneous_SH_NameArray[b_pos] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}";

				if (!FoundItemData_NoShow.HasForm(Miscellaneous_W1_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("{:s}{:s}{:s}{:s}{:s}"sv, CVariables::V_NotificationTextShout0, Miscellaneous_SH_FormArray[b_pos]->GetName(), CVariables::V_NotificationTextShout1, Word1->GetName(), CVariables::V_NotificationTextShout3);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[b_pos]);
				return;
			}
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

		ClearArrays();
		BuildArrays();
		ReSizeArrays();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundFormsExt(RE::StaticFunctionTag*) {
		
		VariablesAPI::Update();
		CHandler::UpdateFoundForms();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		Vanilla_SH_NameOutput.clear();
		Thunderchild_SH_NameOutput.clear();
		Miscellaneous_SH_NameOutput.clear();

		for (auto i = 0; i < Vanilla_SH_FormArray.size(); i++) {
			
			auto* Word1 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[i])->variations[0].word;
			auto* Word2 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[i])->variations[1].word;
			auto* Word3 = static_cast<RE::TESShout*>(Vanilla_SH_FormArray[i])->variations[2].word;

			if (FoundItemData_NoShow.HasForm(Vanilla_W3_FormArray[i])) {

				Vanilla_SH_NameOutput.push_back(Vanilla_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Vanilla_W3_FormArray[i]);
				FoundItemData_NoShow.AddForm(Vanilla_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[i]);
				Vanilla_SH_BoolArray[i] = true;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Vanilla_W2_FormArray[i])) {

				Vanilla_SH_NameOutput.push_back(Vanilla_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");
				
				FoundItemData_NoShow.AddForm(Vanilla_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[i]);
				Vanilla_SH_BoolArray[i] = false;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Vanilla_W1_FormArray[i])) {
				
				Vanilla_SH_NameOutput.push_back(Vanilla_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");
				
				FoundItemData_NoShow.AddForm(Vanilla_W1_FormArray[i]);
				continue;
			}

			Vanilla_SH_NameOutput.push_back(Vanilla_SH_NameArray[i] + "{" + V_ShoutColour_New + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");
			Vanilla_SH_BoolArray[i] = false;
		}

		//-------------------------
		//-------------------------

		for (auto i = 0; i < Thunderchild_SH_FormArray.size(); i++) {

			auto* Word1 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[i])->variations[0].word;
			auto* Word2 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[i])->variations[1].word;
			auto* Word3 = static_cast<RE::TESShout*>(Thunderchild_SH_FormArray[i])->variations[2].word;

			if (FoundItemData_NoShow.HasForm(Thunderchild_W3_FormArray[i])) {

				Thunderchild_SH_NameOutput.push_back(Thunderchild_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Thunderchild_W3_FormArray[i]);
				FoundItemData_NoShow.AddForm(Thunderchild_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[i]);
				Thunderchild_SH_BoolArray[i] = true;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Thunderchild_W2_FormArray[i])) {

				Thunderchild_SH_NameOutput.push_back(Thunderchild_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Thunderchild_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[i]);
				Thunderchild_SH_BoolArray[i] = false;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Thunderchild_W1_FormArray[i])) {

				Thunderchild_SH_NameOutput.push_back(Thunderchild_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Thunderchild_W1_FormArray[i]);
				continue;
			}

			Thunderchild_SH_NameOutput.push_back(Thunderchild_SH_NameArray[i] + "{" + V_ShoutColour_New + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");
			Thunderchild_SH_BoolArray[i] = false;
		}

		//-------------------------
		//-------------------------

		for (auto i = 0; i < Miscellaneous_SH_FormArray.size(); i++) {

			auto* Word1 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[i])->variations[0].word;
			auto* Word2 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[i])->variations[1].word;
			auto* Word3 = static_cast<RE::TESShout*>(Miscellaneous_SH_FormArray[i])->variations[2].word;

			if (FoundItemData_NoShow.HasForm(Miscellaneous_W3_FormArray[i])) {

				Miscellaneous_SH_NameOutput.push_back(Miscellaneous_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Miscellaneous_W3_FormArray[i]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[i]);
				Miscellaneous_SH_BoolArray[i] = true;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Miscellaneous_W2_FormArray[i])) {

				Miscellaneous_SH_NameOutput.push_back(Miscellaneous_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_Found + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Miscellaneous_W2_FormArray[i]);
				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[i]);
				Miscellaneous_SH_BoolArray[i] = false;
				continue;
			}

			if (FoundItemData_NoShow.HasForm(Miscellaneous_W1_FormArray[i])) {

				Miscellaneous_SH_NameOutput.push_back(Miscellaneous_SH_NameArray[i] + "{" + V_ShoutColour_Found + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");

				FoundItemData_NoShow.AddForm(Miscellaneous_W1_FormArray[i]);
				continue;
			}

			Miscellaneous_SH_NameOutput.push_back(Miscellaneous_SH_NameArray[i] + "{" + V_ShoutColour_New + "}" + "{" + Word1->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word2->GetName() + "}" + "{" + V_ShoutColour_New + "}" + "{" + Word3->GetName() + "}");
			Miscellaneous_SH_BoolArray[i] = false;
		}

		Vanilla_SH_NameOutput.resize(Vanilla_SH_FormArray.size());
		Vanilla_SH_EntriesTotal = Vanilla_SH_FormArray.size();
		Vanilla_SH_EntriesFound = std::ranges::count(Vanilla_SH_BoolArray, true);

		Thunderchild_SH_NameOutput.resize(Thunderchild_SH_FormArray.size());
		Thunderchild_SH_EntriesTotal = Thunderchild_SH_FormArray.size();
		Thunderchild_SH_EntriesFound = std::ranges::count(Thunderchild_SH_BoolArray, true);

		Miscellaneous_SH_NameOutput.resize(Miscellaneous_SH_FormArray.size());
		Miscellaneous_SH_EntriesTotal = Miscellaneous_SH_FormArray.size();
		Miscellaneous_SH_EntriesFound = std::ranges::count(Miscellaneous_SH_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Build Shouts Array ) -----
	//---------------------------------------------------

	void CHandler::BuildArrays() {

		std::string SKName = "Skyrim.esm";
		std::string DGName = "Dawnguard.esm";
		std::string DBName = "Dragonborn.esm";
		
		std::string FSName = "Falskaar.esm";
		std::string WYName = "Wyrmstooth.esp";
		std::string THName = "Thunderchild - Epic Shout Package.esp";
		std::string FTName = "ForcefulTongue.esp";

		CHandler::AddToArray(Section::kVanilla, 0x05FC77, "$MiscDataShout00", "$MiscDataShoutHighlight00", SKName); //Animal Allegiance
		CHandler::AddToArray(Section::kVanilla, 0x07097B, "$MiscDataShout01", "$MiscDataShoutHighlight01", SKName); //Aura Whisper
		CHandler::AddToArray(Section::kVanilla, 0x02AD09, "$MiscDataShout02", "$MiscDataShoutHighlight02", DBName); //Battle Fury
		CHandler::AddToArray(Section::kVanilla, 0x032920, "$MiscDataShout03", "$MiscDataShoutHighlight03", SKName); //Become Ethereal
		CHandler::AddToArray(Section::kVanilla, 0x0179D8, "$MiscDataShout04", "$MiscDataShoutHighlight04", DBName); //Bend Will
		CHandler::AddToArray(Section::kVanilla, 0x046B8C, "$MiscDataShout05", "$MiscDataShoutHighlight05", SKName); //Call Dragon
		CHandler::AddToArray(Section::kVanilla, 0x05196A, "$MiscDataShout06", "$MiscDataShoutHighlight06", SKName); //Call of Valor
		CHandler::AddToArray(Section::kVanilla, 0x03CD34, "$MiscDataShout07", "$MiscDataShoutHighlight07", SKName); //Clear Skies
		CHandler::AddToArray(Section::kVanilla, 0x0200C0, "$MiscDataShout08", "$MiscDataShoutHighlight08", DBName); //Cyclone
		CHandler::AddToArray(Section::kVanilla, 0x070981, "$MiscDataShout09", "$MiscDataShoutHighlight09", SKName); //Disarm
		CHandler::AddToArray(Section::kVanilla, 0x02395A, "$MiscDataShout10", "$MiscDataShoutHighlight10", SKName); //Dismay
		CHandler::AddToArray(Section::kVanilla, 0x01DF92, "$MiscDataShout11", "$MiscDataShoutHighlight11", DBName); //Dragon Aspect
		CHandler::AddToArray(Section::kVanilla, 0x044250, "$MiscDataShout12", "$MiscDataShoutHighlight12", SKName); //Dragonrend
		CHandler::AddToArray(Section::kVanilla, 0x008A62, "$MiscDataShout13", "$MiscDataShoutHighlight13", DGName); //Drain Vitality
		CHandler::AddToArray(Section::kVanilla, 0x032921, "$MiscDataShout14", "$MiscDataShoutHighlight14", SKName); //Elemental Fury
		CHandler::AddToArray(Section::kVanilla, 0x03F9EA, "$MiscDataShout15", "$MiscDataShoutHighlight15", SKName); //Fire Breath
		CHandler::AddToArray(Section::kVanilla, 0x05D16B, "$MiscDataShout16", "$MiscDataShoutHighlight16", SKName); //Frost Breath
		CHandler::AddToArray(Section::kVanilla, 0x070980, "$MiscDataShout17", "$MiscDataShoutHighlight17", SKName); //Ice Form
		CHandler::AddToArray(Section::kVanilla, 0x07097E, "$MiscDataShout18", "$MiscDataShoutHighlight18", SKName); //Kyne's Peace
		CHandler::AddToArray(Section::kVanilla, 0x07097C, "$MiscDataShout19", "$MiscDataShoutHighlight19", SKName); //Marked for Death
		CHandler::AddToArray(Section::kVanilla, 0x048AC9, "$MiscDataShout20", "$MiscDataShoutHighlight20", SKName); //Slow Time
		CHandler::AddToArray(Section::kVanilla, 0x007CB6, "$MiscDataShout21", "$MiscDataShoutHighlight21", DGName); //Soul Tear
		CHandler::AddToArray(Section::kVanilla, 0x07097D, "$MiscDataShout22", "$MiscDataShoutHighlight22", SKName); //Storm Call
		CHandler::AddToArray(Section::kVanilla, 0x0030D2, "$MiscDataShout23", "$MiscDataShoutHighlight23", DGName); //Summon Durnehviir
		CHandler::AddToArray(Section::kVanilla, 0x07097F, "$MiscDataShout24", "$MiscDataShoutHighlight24", SKName); //Throw Voice
		CHandler::AddToArray(Section::kVanilla, 0x013E07, "$MiscDataShout25", "$MiscDataShoutHighlight25", SKName); //Unrelenting Force
		CHandler::AddToArray(Section::kVanilla, 0x02F7BA, "$MiscDataShout26", "$MiscDataShoutHighlight26", SKName); //Whirlwind Sprint

		CHandler::AddToArray(Section::kThunderchild, 0x05EBB4, "$MiscDataShoutThunderChild00", "$MiscDataShoutHighlightThunder00", THName); //Alessia's Love
		CHandler::AddToArray(Section::kThunderchild, 0x0CA921, "$MiscDataShoutThunderChild01", "$MiscDataShoutHighlightThunder01", THName); //Annihilate
		CHandler::AddToArray(Section::kThunderchild, 0x020571, "$MiscDataShoutThunderChild02", "$MiscDataShoutHighlightThunder02", THName); //Arcane Helix
		CHandler::AddToArray(Section::kThunderchild, 0x05294B, "$MiscDataShoutThunderChild03", "$MiscDataShoutHighlightThunder03", THName); //Armageddon
		CHandler::AddToArray(Section::kThunderchild, 0x0012D7, "$MiscDataShoutThunderChild04", "$MiscDataShoutHighlightThunder04", THName); //Curse
		CHandler::AddToArray(Section::kThunderchild, 0x0CA951, "$MiscDataShoutThunderChild05", "$MiscDataShoutHighlightThunder05", THName); //Dance of the Dead
		CHandler::AddToArray(Section::kThunderchild, 0x013CB0, "$MiscDataShoutThunderChild06", "$MiscDataShoutHighlightThunder06", THName); //Earthquake
		CHandler::AddToArray(Section::kThunderchild, 0x00541C, "$MiscDataShoutThunderChild07", "$MiscDataShoutHighlightThunder07", THName); //Essence Rip
		CHandler::AddToArray(Section::kThunderchild, 0x0CA94F, "$MiscDataShoutThunderChild08", "$MiscDataShoutHighlightThunder08", THName); //Evocation
		CHandler::AddToArray(Section::kThunderchild, 0x00AAC5, "$MiscDataShoutThunderChild09", "$MiscDataShoutHighlightThunder09", THName); //Geomagnetism
		CHandler::AddToArray(Section::kThunderchild, 0x05EBB5, "$MiscDataShoutThunderChild10", "$MiscDataShoutHighlightThunder10", THName); //Iceborn
		CHandler::AddToArray(Section::kThunderchild, 0x027BFB, "$MiscDataShoutThunderChild11", "$MiscDataShoutHighlightThunder11", THName); //Jone's Shadow
		CHandler::AddToArray(Section::kThunderchild, 0x0CA935, "$MiscDataShoutThunderChild12", "$MiscDataShoutHighlightThunder12", THName); //Kingsbane
		CHandler::AddToArray(Section::kThunderchild, 0x0CA928, "$MiscDataShoutThunderChild13", "$MiscDataShoutHighlightThunder13", THName); //Lifestream
		CHandler::AddToArray(Section::kThunderchild, 0x01AE61, "$MiscDataShoutThunderChild14", "$MiscDataShoutHighlightThunder14", THName); //Lightning Shield
		CHandler::AddToArray(Section::kThunderchild, 0x0CA93C, "$MiscDataShoutThunderChild15", "$MiscDataShoutHighlightThunder15", THName); //Oblivion
		CHandler::AddToArray(Section::kThunderchild, 0x02817C, "$MiscDataShoutThunderChild16", "$MiscDataShoutHighlightThunder16", THName); //Phantom Decoy
		CHandler::AddToArray(Section::kThunderchild, 0x006F61, "$MiscDataShoutThunderChild17", "$MiscDataShoutHighlightThunder17", THName); //Riftwalk
		CHandler::AddToArray(Section::kThunderchild, 0x05EBB6, "$MiscDataShoutThunderChild18", "$MiscDataShoutHighlightThunder18", THName); //Shattersphere
		CHandler::AddToArray(Section::kThunderchild, 0x05294A, "$MiscDataShoutThunderChild19", "$MiscDataShoutHighlightThunder19", THName); //Shor's Wrath
		CHandler::AddToArray(Section::kThunderchild, 0x001859, "$MiscDataShoutThunderChild20", "$MiscDataShoutHighlightThunder20", THName); //Shroud of Snowfall
		CHandler::AddToArray(Section::kThunderchild, 0x00D5FC, "$MiscDataShoutThunderChild21", "$MiscDataShoutHighlightThunder21", THName); //Speak Unto The Stars
		CHandler::AddToArray(Section::kThunderchild, 0x016D92, "$MiscDataShoutThunderChild22", "$MiscDataShoutHighlightThunder22", THName); //Splinter Twins
		CHandler::AddToArray(Section::kThunderchild, 0x01D462, "$MiscDataShoutThunderChild23", "$MiscDataShoutHighlightThunder23", THName); //Stormblast
		CHandler::AddToArray(Section::kThunderchild, 0x0CA950, "$MiscDataShoutThunderChild24", "$MiscDataShoutHighlightThunder24", THName); //The Conqueror
		CHandler::AddToArray(Section::kThunderchild, 0x01B3DD, "$MiscDataShoutThunderChild25", "$MiscDataShoutHighlightThunder25", THName); //Trueshot
		CHandler::AddToArray(Section::kThunderchild, 0x0043A8, "$MiscDataShoutThunderChild26", "$MiscDataShoutHighlightThunder26", THName); //Wail of the Banshee
		CHandler::AddToArray(Section::kThunderchild, 0x01FFDE, "$MiscDataShoutThunderChild27", "$MiscDataShoutHighlightThunder27", THName); //Wanderlust
		CHandler::AddToArray(Section::kThunderchild, 0x004925, "$MiscDataShoutThunderChild28", "$MiscDataShoutHighlightThunder28", THName); //Warcry
	
		CHandler::AddToArray(Section::kMiscellaneous, 0x0D228F, "$MiscDataShout_FSK_Name", "$MiscDataShout_FSK_Data", FSName);	//Champion's Spirit (Falskaar)

		if (Serialization::CompletionistData::IsModInstalled("ForcefulTongue - Wyrmstooth Patch.esp")) {
			CHandler::AddToArray(Section::kMiscellaneous, 0x000C26, "$MiscDataShout_FFT_Name", "$MiscDataShout_FFTWYR_Data", FTName);	//Phantom Form (Forceful Tongue)
		}
		else {
			CHandler::AddToArray(Section::kMiscellaneous, 0x30C92F, "$MiscDataShout_WYR_Name", "$MiscDataShout_WYR_Data", WYName);	//Phantom Form (Wyrmstooth)
			CHandler::AddToArray(Section::kMiscellaneous, 0x000C26, "$MiscDataShout_FFT_Name", "$MiscDataShout_FFT_Data", FTName);	//Phantom Form (Forceful Tongue)
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add To Array ) -----------
	//---------------------------------------------------

	void CHandler::AddToArray(Section a_section, RE::FormID a_formID, std::string a_names, std::string a_texts, std::string a_Filename) {

		auto* Handler = RE::TESDataHandler::GetSingleton();

		auto* Shout = static_cast<RE::TESShout*>(Handler->LookupForm(a_formID, a_Filename));
		if (!Shout) { return; }

		auto* Word1 = static_cast<RE::SpellItem*>(Shout->variations[0].spell);
		auto* Word2 = static_cast<RE::SpellItem*>(Shout->variations[1].spell);
		auto* Word3 = static_cast<RE::SpellItem*>(Shout->variations[2].spell);
		if (!Word1 || !Word2 || !Word3) { return; }

		switch (a_section)
		{
		case CFramework_Shouts::Section::kVanilla: {

			Vanilla_SH_FormArray.push_back(Shout);
			Vanilla_W1_FormArray.push_back(Word1);
			Vanilla_W2_FormArray.push_back(Word2);
			Vanilla_W3_FormArray.push_back(Word3);

			Vanilla_SH_NameArray.push_back(a_names);
			Vanilla_SH_TextArray.push_back(a_texts);

			CFramework_Shouts_VS::Data.AddForm(Word1);
			CFramework_Shouts_VS::Data.AddForm(Word2);
			CFramework_Shouts_VS::Data.AddForm(Word3);
			break;
		}

		case CFramework_Shouts::Section::kThunderchild: {

			Thunderchild_SH_FormArray.push_back(Shout);
			Thunderchild_W1_FormArray.push_back(Word1);
			Thunderchild_W2_FormArray.push_back(Word2);
			Thunderchild_W3_FormArray.push_back(Word3);

			Thunderchild_SH_NameArray.push_back(a_names);
			Thunderchild_SH_TextArray.push_back(a_texts);

			CFramework_Shouts_TS::Data.AddForm(Word1);
			CFramework_Shouts_TS::Data.AddForm(Word2);
			CFramework_Shouts_TS::Data.AddForm(Word3);
			break;
		}

		case CFramework_Shouts::Section::kMiscellaneous: {

			Miscellaneous_SH_FormArray.push_back(Shout);
			Miscellaneous_W1_FormArray.push_back(Word1);
			Miscellaneous_W2_FormArray.push_back(Word2);
			Miscellaneous_W3_FormArray.push_back(Word3);

			Miscellaneous_SH_NameArray.push_back(a_names);
			Miscellaneous_SH_TextArray.push_back(a_texts);

			CFramework_Shouts_MS::Data.AddForm(Word1);
			CFramework_Shouts_MS::Data.AddForm(Word2);
			CFramework_Shouts_MS::Data.AddForm(Word3);
			break;
		}

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Clear Arrays ) -----------
	//---------------------------------------------------

	void CHandler::ClearArrays() {

		Vanilla_SH_NameArray.clear();
		Vanilla_SH_TextArray.clear();
		Vanilla_SH_FormArray.clear();
		Vanilla_W1_FormArray.clear();
		Vanilla_W2_FormArray.clear();
		Vanilla_W3_FormArray.clear();
		Vanilla_SH_BoolArray.clear();

		Thunderchild_SH_NameArray.clear();
		Thunderchild_SH_TextArray.clear();
		Thunderchild_SH_FormArray.clear();
		Thunderchild_W1_FormArray.clear();
		Thunderchild_W2_FormArray.clear();
		Thunderchild_W3_FormArray.clear();
		Thunderchild_SH_BoolArray.clear();

		Miscellaneous_SH_NameArray.clear();
		Miscellaneous_SH_TextArray.clear();
		Miscellaneous_SH_FormArray.clear();
		Miscellaneous_W1_FormArray.clear();
		Miscellaneous_W2_FormArray.clear();
		Miscellaneous_W3_FormArray.clear();
		Miscellaneous_SH_BoolArray.clear();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Clear Arrays ) -----------
	//---------------------------------------------------

	void CHandler::ReSizeArrays() {

		Vanilla_SH_NameArray.resize(Vanilla_SH_FormArray.size());
		Vanilla_SH_TextArray.resize(Vanilla_SH_FormArray.size());
		Vanilla_SH_BoolArray.resize(Vanilla_SH_FormArray.size());

		Thunderchild_SH_NameArray.resize(Thunderchild_SH_FormArray.size());
		Thunderchild_SH_TextArray.resize(Thunderchild_SH_FormArray.size());
		Thunderchild_SH_BoolArray.resize(Thunderchild_SH_FormArray.size());

		Miscellaneous_SH_NameArray.resize(Miscellaneous_SH_FormArray.size());
		Miscellaneous_SH_TextArray.resize(Miscellaneous_SH_FormArray.size());
		Miscellaneous_SH_BoolArray.resize(Miscellaneous_SH_FormArray.size());
	}
}