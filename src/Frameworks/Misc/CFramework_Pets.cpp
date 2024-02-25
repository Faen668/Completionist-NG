#include "CFramework_Pets.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Pets {
	using namespace CFramework_Master;
	using namespace Serialization;

	// clang-format off

	constexpr Serialization::FormArray VanillaPets = {
	0x01381A,0x01381B,0x013817,0x013818,0x013819,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		InjectAndCompileData();
		InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::Framework_UpdatePetOwnership(RE::StaticFunctionTag*, RE::TESActorBase* a_actorBase) {

		if (auto t_pos = std::ranges::find(Pets_VP_FormArray, a_actorBase); t_pos != Pets_VP_FormArray.end()) {
			auto b_pos = std::distance(Pets_VP_FormArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pets_VP_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Pets_VP_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, Pets_VP_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(Pets_VP_FormArray[b_pos]);
			Pets_VP_BoolArray[b_pos] = true;
			Pets_VP_EntriesFound = std::ranges::count(Pets_VP_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CFramework_Pets_VP::Data.CompileFormArray(CFramework_Pets::VanillaPets, "HearthFires.esm");
		CFramework_Pets_VP::Data.Populate(Pets_VP_NameArray, Pets_VP_FormArray, Pets_VP_BoolArray, Pets_VP_TextArray);

		for (auto i = 0; i < Pets_VP_FormArray.size(); i++) {
			Pets_VP_TextArray[i] = "$PetHighlight00{" + Pets_VP_NameArray[i] + "}{" + Pets_VP_NameArray[i] + "}";
		}

		Pets_VP_EntriesTotal = Pets_VP_FormArray.size();
		Pets_VP_EntriesFound = std::ranges::count(Pets_VP_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Pets_VP_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_VP_FormArray[i], Pets_VP_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < Pets_VP_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pets_VP_FormArray[i]->GetFormID())) {
				Pets_VP_BoolArray[i] = true;
			}
		}

		Pets_VP_EntriesTotal = Pets_VP_FormArray.size();
		Pets_VP_EntriesFound = std::ranges::count(Pets_VP_BoolArray, true);
	}
}