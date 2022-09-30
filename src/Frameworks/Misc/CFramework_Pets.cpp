#include "Serialization.hpp"
#include "CFramework_Pets.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

namespace CFramework_VanillaPets {
	Serialization::CompletionistData Data;
}

namespace CFramework_PetsOfSkyrim {
	Serialization::CompletionistData Data;
}

namespace CFramework_SaintsSeducers {
	Serialization::CompletionistData Data;
}

namespace CFramework_MiscellaneousPets {
	Serialization::CompletionistData Data;
}

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

	inline std::vector<std::string> Pet_1_NameArray;
	inline std::vector<std::string> Pet_1_TextArray;
	inline std::vector<RE::TESForm*> Pet_1_FormArray;
	inline std::vector<bool> Pet_1_BoolArray;
	inline std::int32_t Pet_1_EntriesTotal = 0;
	inline std::int32_t Pet_1_EntriesFound = 0;

	inline std::vector<std::string> Pet_2_NameArray;
	inline std::vector<std::string> Pet_2_TextArray;
	inline std::vector<RE::TESForm*> Pet_2_FormArray;
	inline std::vector<bool> Pet_2_BoolArray;
	inline std::int32_t Pet_2_EntriesTotal = 0;
	inline std::int32_t Pet_2_EntriesFound = 0;

	inline std::vector<std::string> Pet_3_NameArray;
	inline std::vector<std::string> Pet_3_TextArray;
	inline std::vector<RE::TESForm*> Pet_3_FormArray;
	inline std::vector<bool> Pet_3_BoolArray;
	inline std::int32_t Pet_3_EntriesTotal = 0;
	inline std::int32_t Pet_3_EntriesFound = 0;

	inline std::vector<std::string> Pet_4_NameArray;
	inline std::vector<std::string> Pet_4_TextArray;
	inline std::vector<RE::TESForm*> Pet_4_FormArray;
	inline std::vector<bool> Pet_4_BoolArray;
	inline std::int32_t Pet_4_EntriesTotal = 0;
	inline std::int32_t Pet_4_EntriesFound = 0;

	inline bool PetsOfSkyrim_Installed = false;
	inline bool SaintSeducer_Installed = false;

	inline bool Petcrab_Installed = false;
	inline bool PetWolf_Installed = false;
	inline bool PetHund_Installed = false;

	inline bool Miscellaneous_Installed = false;

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

		if (auto t_pos = std::ranges::find(Pet_1_NameArray, a_name); t_pos != Pet_1_NameArray.end()) {
			auto b_pos = std::distance(Pet_1_NameArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pet_1_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pet_1_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(Pet_1_FormArray[b_pos]);
			Pet_1_BoolArray[b_pos] = true;
			Pet_1_EntriesFound = std::ranges::count(Pet_1_BoolArray, true);
			return;
		}

		if (PetsOfSkyrim_Installed) {
			if (auto t_pos = std::ranges::find(Pet_2_NameArray, a_name); t_pos != Pet_2_NameArray.end()) {
				auto b_pos = std::distance(Pet_2_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pet_2_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pet_2_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pet_2_FormArray[b_pos]);
				Pet_2_BoolArray[b_pos] = true;
				Pet_2_EntriesFound = std::ranges::count(Pet_2_BoolArray, true);
				return;
			}
		}

		if (SaintSeducer_Installed) {
			if (auto t_pos = std::ranges::find(Pet_3_NameArray, a_name); t_pos != Pet_3_NameArray.end()) {
				auto b_pos = std::distance(Pet_3_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pet_3_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pet_3_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pet_3_FormArray[b_pos]);
				Pet_3_BoolArray[b_pos] = true;
				Pet_3_EntriesFound = std::ranges::count(Pet_3_BoolArray, true);
				return;
			}
		}

		if (Miscellaneous_Installed) {
			if (auto t_pos = std::ranges::find(Pet_4_NameArray, a_name); t_pos != Pet_4_NameArray.end()) {
				auto b_pos = std::distance(Pet_4_NameArray.begin(), t_pos);

				if (!FoundItemData_NoShow.HasForm(Pet_4_FormArray[b_pos]->GetFormID())) {
					auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, Pet_4_NameArray[b_pos]);
					FrameworkAPI::SendNotification(msg, "NotifySpecial");
				}

				FoundItemData_NoShow.AddForm(Pet_4_FormArray[b_pos]);
				Pet_4_BoolArray[b_pos] = true;
				Pet_4_EntriesFound = std::ranges::count(Pet_4_BoolArray, true);
				return;
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto handler = RE::TESDataHandler::GetSingleton();

		CFramework_VanillaPets::Data.CompileFormArray(CFramework_Pets::VanillaPets, "HearthFires.esm");
		CFramework_VanillaPets::Data.Populate(Pet_1_NameArray, Pet_1_FormArray, Pet_1_BoolArray, Pet_1_TextArray);

		for (auto i = 0; i < Pet_1_FormArray.size(); i++) {
			Pet_1_TextArray[i] = "$PetHighlight00{" + Pet_1_NameArray[i] + "}{" + Pet_1_NameArray[i] + "}";
		}

		Pet_1_EntriesTotal = Pet_1_FormArray.size();
		Pet_1_EntriesFound = std::ranges::count(Pet_1_BoolArray, true);

		if (PetsOfSkyrim_Installed) {
			CFramework_PetsOfSkyrim::Data.CompileFormArray(CFramework_Pets::PetsOfSkyrim, "ccvsvsse002-pets.esl");
			CFramework_PetsOfSkyrim::Data.Populate(Pet_2_NameArray, Pet_2_FormArray, Pet_2_BoolArray, Pet_2_TextArray, false, false);

			Pet_2_TextArray.clear();
			Pet_2_TextArray.push_back("$PetHighlightPetsOfSkyrim00{" + Pet_2_NameArray[0] + "}{" + Pet_2_NameArray[0] + "}"); //Arachnia
			Pet_2_TextArray.push_back("$PetHighlightPetsOfSkyrim01{" + Pet_2_NameArray[1] + "}{" + Pet_2_NameArray[1] + "}"); //Hilda
			Pet_2_TextArray.push_back("$PetHighlightPetsOfSkyrim02{" + Pet_2_NameArray[2] + "}{" + Pet_2_NameArray[2] + "}"); //Skritch
			Pet_2_TextArray.push_back("$PetHighlightPetsOfSkyrim03{" + Pet_2_NameArray[3] + "}{" + Pet_2_NameArray[3] + "}"); //Sweet Roll
			Pet_2_TextArray.push_back("$PetHighlightPetsOfSkyrim04{" + Pet_2_NameArray[4] + "}{" + Pet_2_NameArray[4] + "}"); //Thistle

			Pet_2_EntriesTotal = Pet_2_FormArray.size();
			Pet_2_EntriesFound = std::ranges::count(Pet_2_BoolArray, true);
		}

		if (SaintSeducer_Installed) {
			CFramework_SaintsSeducers::Data.CompileFormArray(CFramework_Pets::SaintsSeducers, "ccbgssse025-advdsgs.esm");
			CFramework_SaintsSeducers::Data.Populate(Pet_3_NameArray, Pet_3_FormArray, Pet_3_BoolArray, Pet_3_TextArray, false, false);

			Pet_3_TextArray.clear();
			Pet_3_TextArray.push_back("$PetHighlightSaintsSeducers00{" + Pet_3_NameArray[0] + "}{" + Pet_3_NameArray[0] + "}"); //Demented Elytra Nymph
			Pet_3_TextArray.push_back("$PetHighlightSaintsSeducers01{" + Pet_3_NameArray[1] + "}{" + Pet_3_NameArray[1] + "}"); //Manic Elytra Nymph

			Pet_3_EntriesTotal = Pet_3_FormArray.size();
			Pet_3_EntriesFound = std::ranges::count(Pet_3_BoolArray, true);
		}

		if (Miscellaneous_Installed) {

			if (PetWolf_Installed) {
				CFramework_MiscellaneousPets::Data.AddForm(0x000D62, "ccbgssse010-petdwarvenarmoredmudcrab.esl");
			}

			if (Petcrab_Installed) {
				CFramework_MiscellaneousPets::Data.AddForm(0x000D63, "ccbgssse036-petbwolf.esl");
			}

			if (PetHund_Installed) {
				CFramework_MiscellaneousPets::Data.AddForm(0x000D64, "ccbgssse035-petnhound.esl");
			}

			CFramework_MiscellaneousPets::Data.CompileFormArray(CFramework_Pets::Miscellaneous, "");
			CFramework_MiscellaneousPets::Data.Populate(Pet_4_NameArray, Pet_4_FormArray, Pet_4_BoolArray, Pet_4_TextArray, false, false);

			Pet_4_TextArray.clear();

			if (PetWolf_Installed) {
				Pet_4_TextArray.push_back("$PetHighlightMisc00{" + Pet_4_NameArray[0] + "}{" + Pet_4_NameArray[0] + "}"); //Bone Wolf
			}

			if (Petcrab_Installed) {
				Pet_4_TextArray.push_back("$PetHighlightMisc01{" + Pet_4_NameArray[1] + "}{" + Pet_4_NameArray[1] + "}"); //Dwarven Armored Mudcrab
			}

			if (PetHund_Installed) {
				Pet_4_TextArray.push_back("$PetHighlightMisc02{" + Pet_4_NameArray[2] + "}{" + Pet_4_NameArray[2] + "}"); //Nix-Hound
			}

			Pet_4_EntriesTotal = Pet_4_FormArray.size();
			Pet_4_EntriesFound = std::ranges::count(Pet_4_BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < Pet_1_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pet_1_FormArray[i]->GetFormID())) {
				Pet_1_BoolArray[i] = true;
			}
		}
		Pet_1_EntriesTotal = Pet_1_FormArray.size();
		Pet_1_EntriesFound = std::ranges::count(Pet_1_BoolArray, true);

		//-------------------------------

		if (PetsOfSkyrim_Installed) {
			for (auto i = 0; i < Pet_2_FormArray.size(); i++) {
				if (FoundItemData_NoShow.HasForm(Pet_2_FormArray[i]->GetFormID())) {
					Pet_2_BoolArray[i] = true;
				}
			}
			Pet_2_EntriesTotal = Pet_2_FormArray.size();
			Pet_2_EntriesFound = std::ranges::count(Pet_2_BoolArray, true);
		}

		//-------------------------------

		if (SaintSeducer_Installed) {
			for (auto i = 0; i < Pet_3_FormArray.size(); i++) {
				if (FoundItemData_NoShow.HasForm(Pet_3_FormArray[i]->GetFormID())) {
					Pet_3_BoolArray[i] = true;
				}
			}
			Pet_3_EntriesTotal = Pet_3_FormArray.size();
			Pet_3_EntriesFound = std::ranges::count(Pet_3_BoolArray, true);
		}

		//-------------------------------

		if (Miscellaneous_Installed) {
			for (auto i = 0; i < Pet_4_FormArray.size(); i++) {
				if (FoundItemData_NoShow.HasForm(Pet_4_FormArray[i]->GetFormID())) {
					Pet_4_BoolArray[i] = true;
				}
			}
			Pet_4_EntriesTotal = Pet_4_FormArray.size();
			Pet_4_EntriesFound = std::ranges::count(Pet_4_BoolArray, true);
		}
	}
}