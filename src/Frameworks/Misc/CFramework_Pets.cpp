#include "CFramework_Pets.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_Pets {
	using namespace CFramework_Master;

	// clang-format off

	constexpr Serialization::FormArray VanillaPets = {
	0x01381A,0x01381B,0x013817,0x013818,0x013819,
	};

	constexpr Serialization::FormArray PetsOfSkyrim = {
	0x00082B,0x000830,0x00082D,0x000831,0x00082F,
	};

	constexpr Serialization::FormArray SaintsSeducers = {
	0x000CB2,0x000CBB,
	};

	constexpr Serialization::FormArray Miscellaneous = {
	0,
	};

	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		PetsOfSkyrim_Installed = Serialization::CompletionistData::IsModInstalled("ccvsvsse002-pets.esl");
		SaintSeducer_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse025-advdsgs.esm");

		Petcrab_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse010-petdwarvenarmoredmudcrab.esl");
		PetWolf_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse036-petbwolf.esl");
		PetHund_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse035-petnhound.esl");

		Miscellaneous_Installed = Petcrab_Installed || PetWolf_Installed || PetHund_Installed;

		InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::Framework_UpdatePetOwnership(RE::StaticFunctionTag*, std::string a_name) {

		if (auto t_pos = std::ranges::find(Pets_VP_NameArray, a_name); t_pos != Pets_VP_NameArray.end()) {
			auto b_pos = std::distance(Pets_VP_NameArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pets_VP_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pets_VP_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(Pets_VP_FormArray[b_pos]);
			Pets_VP_BoolArray[b_pos] = true;
			Pets_VP_EntriesFound = std::ranges::count(Pets_VP_BoolArray, true);
			return;
		}

		if (PetsOfSkyrim_Installed) {
			if (auto t_pos = std::ranges::find(Pets_PS_NameArray, a_name); t_pos != Pets_PS_NameArray.end()) {
				auto b_pos = std::distance(Pets_PS_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pets_PS_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pets_PS_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pets_PS_FormArray[b_pos]);
				Pets_PS_BoolArray[b_pos] = true;
				Pets_PS_EntriesFound = std::ranges::count(Pets_PS_BoolArray, true);
				return;
			}
		}

		if (SaintSeducer_Installed) {
			if (auto t_pos = std::ranges::find(Pets_SS_NameArray, a_name); t_pos != Pets_SS_NameArray.end()) {
				auto b_pos = std::distance(Pets_SS_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pets_SS_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pets_SS_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pets_SS_FormArray[b_pos]);
				Pets_SS_BoolArray[b_pos] = true;
				Pets_SS_EntriesFound = std::ranges::count(Pets_SS_BoolArray, true);
				return;
			}
		}

		if (Miscellaneous_Installed) {
			if (auto t_pos = std::ranges::find(Pets_MP_NameArray, a_name); t_pos != Pets_MP_NameArray.end()) {
				auto b_pos = std::distance(Pets_MP_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pets_MP_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pets_MP_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pets_MP_FormArray[b_pos]);
				Pets_MP_BoolArray[b_pos] = true;
				Pets_MP_EntriesFound = std::ranges::count(Pets_MP_BoolArray, true);
				return;
			}
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

		if (PetsOfSkyrim_Installed) {
			CFramework_Pets_PS::Data.CompileFormArray(CFramework_Pets::PetsOfSkyrim, "ccvsvsse002-pets.esl");
			CFramework_Pets_PS::Data.Populate(Pets_PS_NameArray, Pets_PS_FormArray, Pets_PS_BoolArray, Pets_PS_TextArray, false);

			Pets_PS_TextArray.clear();
			Pets_PS_TextArray.push_back("$PetHighlightPetsOfSkyrim00{" + Pets_PS_NameArray[0] + "}{" + Pets_PS_NameArray[0] + "}"); //Arachnia
			Pets_PS_TextArray.push_back("$PetHighlightPetsOfSkyrim01{" + Pets_PS_NameArray[1] + "}{" + Pets_PS_NameArray[1] + "}"); //Hilda
			Pets_PS_TextArray.push_back("$PetHighlightPetsOfSkyrim02{" + Pets_PS_NameArray[2] + "}{" + Pets_PS_NameArray[2] + "}"); //Skritch
			Pets_PS_TextArray.push_back("$PetHighlightPetsOfSkyrim03{" + Pets_PS_NameArray[3] + "}{" + Pets_PS_NameArray[3] + "}"); //Sweet Roll
			Pets_PS_TextArray.push_back("$PetHighlightPetsOfSkyrim04{" + Pets_PS_NameArray[4] + "}{" + Pets_PS_NameArray[4] + "}"); //Thistle

			Pets_PS_EntriesTotal = Pets_PS_FormArray.size();
			Pets_PS_EntriesFound = std::ranges::count(Pets_PS_BoolArray, true);
		}

		if (SaintSeducer_Installed) {
			CFramework_Pets_SS::Data.CompileFormArray(CFramework_Pets::SaintsSeducers, "ccbgssse025-advdsgs.esm");
			CFramework_Pets_SS::Data.Populate(Pets_SS_NameArray, Pets_SS_FormArray, Pets_SS_BoolArray, Pets_SS_TextArray, false);

			Pets_SS_TextArray.clear();
			Pets_SS_TextArray.push_back("$PetHighlightSaintsSeducers00{" + Pets_SS_NameArray[0] + "}{" + Pets_SS_NameArray[0] + "}"); //Demented Elytra Nymph
			Pets_SS_TextArray.push_back("$PetHighlightSaintsSeducers01{" + Pets_SS_NameArray[1] + "}{" + Pets_SS_NameArray[1] + "}"); //Manic Elytra Nymph

			Pets_SS_EntriesTotal = Pets_SS_FormArray.size();
			Pets_SS_EntriesFound = std::ranges::count(Pets_SS_BoolArray, true);
		}

		if (Miscellaneous_Installed) {

			if (PetWolf_Installed) {
				CFramework_Pets_MP::Data.AddForm(0x000D62, "ccbgssse010-petdwarvenarmoredmudcrab.esl");
			}

			if (Petcrab_Installed) {
				CFramework_Pets_MP::Data.AddForm(0x000D63, "ccbgssse036-petbwolf.esl");
			}

			if (PetHund_Installed) {
				CFramework_Pets_MP::Data.AddForm(0x000D64, "ccbgssse035-petnhound.esl");
			}

			CFramework_Pets_MP::Data.CompileFormArray(CFramework_Pets::Miscellaneous, "");
			CFramework_Pets_MP::Data.Populate(Pets_MP_NameArray, Pets_MP_FormArray, Pets_MP_BoolArray, Pets_MP_TextArray, false);

			Pets_MP_TextArray.clear();

			if (PetWolf_Installed) {
				Pets_MP_TextArray.push_back("$PetHighlightMisc00{" + Pets_MP_NameArray[0] + "}{" + Pets_MP_NameArray[0] + "}"); //Bone Wolf
			}

			if (Petcrab_Installed) {
				Pets_MP_TextArray.push_back("$PetHighlightMisc01{" + Pets_MP_NameArray[1] + "}{" + Pets_MP_NameArray[1] + "}"); //Dwarven Armored Mudcrab
			}

			if (PetHund_Installed) {
				Pets_MP_TextArray.push_back("$PetHighlightMisc02{" + Pets_MP_NameArray[2] + "}{" + Pets_MP_NameArray[2] + "}"); //Nix-Hound
			}

			Pets_MP_EntriesTotal = Pets_MP_FormArray.size();
			Pets_MP_EntriesFound = std::ranges::count(Pets_MP_BoolArray, true);
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

		for (auto i = 0; i < Pets_PS_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pets_PS_FormArray[i]->GetFormID())) {
				Pets_PS_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < Pets_SS_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pets_SS_FormArray[i]->GetFormID())) {
				Pets_SS_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < Pets_MP_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pets_MP_FormArray[i]->GetFormID())) {
				Pets_MP_BoolArray[i] = true;
			}
		}

		Pets_VP_EntriesTotal = Pets_VP_FormArray.size();
		Pets_VP_EntriesFound = std::ranges::count(Pets_VP_BoolArray, true);

		Pets_PS_EntriesTotal = Pets_PS_FormArray.size();
		Pets_PS_EntriesFound = std::ranges::count(Pets_PS_BoolArray, true);

		Pets_SS_EntriesTotal = Pets_SS_FormArray.size();
		Pets_SS_EntriesFound = std::ranges::count(Pets_SS_BoolArray, true);

		Pets_MP_EntriesTotal = Pets_MP_FormArray.size();
		Pets_MP_EntriesFound = std::ranges::count(Pets_MP_BoolArray, true);
	}
}