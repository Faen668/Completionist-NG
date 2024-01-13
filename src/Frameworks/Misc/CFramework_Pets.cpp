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

	constexpr Serialization::FormArray PetsOfSkyrim = {
	0x00082B,0x000830,0x00082D,0x000831,0x00082F,
	};

	constexpr Serialization::FormArray SaintsSeducers = {
	0x000CB2,0x000CBB,
	};

	constexpr Serialization::FormArray Miscellaneous = {
	0,
	};

	constexpr Serialization::FormArray WildHorses = {
	0x00082c,0x00082d,0x00082e,0x00082f,0x000830,0x000831,0x000832
	};

	std::vector<std::pair<RE::TESForm*, RE::FormID>> WildHorsesMap{};
	// clang-format on

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));

		InjectAndCompileData();
		InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Events ( On Radiant Stage Set ) ------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource)
	{
		if (!a_event || !a_event->stage || a_event-> stage != 10) {
			return EventResult::kContinue;
		}

		const auto* equest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!equest) {return EventResult::kContinue;}

		for (auto& [form, questID] : WildHorsesMap) {

			const auto* quest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(questID, "ccbgssse034-mntuni.esl");

			if (!quest || !form) {return EventResult::kContinue;}

			if (quest && quest->GetFormID() == equest->GetFormID()) {

				if (auto t_pos = std::ranges::find(Pets_WH_FormArray, form); t_pos != Pets_WH_FormArray.end()) {
					auto b_pos = std::distance(Pets_WH_FormArray.begin(), t_pos);

					if (!FoundItemData_NoShow.HasForm(Pets_WH_FormArray[b_pos]->GetFormID())) {
						auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Pets_WH_NameArray[b_pos]);
						FrameworkAPI::SendNotification(msg, "NotifySpecial");
						FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kTamed, Pets_WH_NameArray[b_pos]);
					}

					FoundItemData_NoShow.AddForm(Pets_WH_FormArray[b_pos]);
					Pets_WH_BoolArray[b_pos] = true;
					Pets_WH_EntriesFound = std::ranges::count(Pets_WH_BoolArray, true);
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::Framework_UpdatePetOwnership(RE::StaticFunctionTag*, std::string a_name) {

		if (auto t_pos = std::ranges::find(Pets_VP_NameArray, a_name); t_pos != Pets_VP_NameArray.end()) {
			auto b_pos = std::distance(Pets_VP_NameArray.begin(), t_pos);

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

		if (auto t_pos = std::ranges::find(Pets_PS_NameArray, a_name); t_pos != Pets_PS_NameArray.end()) {
			auto b_pos = std::distance(Pets_PS_NameArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pets_PS_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Pets_PS_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, Pets_PS_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(Pets_PS_FormArray[b_pos]);
			Pets_PS_BoolArray[b_pos] = true;
			Pets_PS_EntriesFound = std::ranges::count(Pets_PS_BoolArray, true);
			return;
		}

		if (auto t_pos = std::ranges::find(Pets_SS_NameArray, a_name); t_pos != Pets_SS_NameArray.end()) {
			auto b_pos = std::distance(Pets_SS_NameArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pets_SS_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Pets_SS_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, Pets_SS_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(Pets_SS_FormArray[b_pos]);
			Pets_SS_BoolArray[b_pos] = true;
			Pets_SS_EntriesFound = std::ranges::count(Pets_SS_BoolArray, true);
			return;
		}

		if (auto t_pos = std::ranges::find(Pets_MP_NameArray, a_name); t_pos != Pets_MP_NameArray.end()) {
			auto b_pos = std::distance(Pets_MP_NameArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(Pets_MP_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, Pets_MP_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kDiscovered, Pets_MP_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(Pets_MP_FormArray[b_pos]);
			Pets_MP_BoolArray[b_pos] = true;
			Pets_MP_EntriesFound = std::ranges::count(Pets_MP_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		auto PetsOfSkyrim_Installed = Serialization::CompletionistData::IsModInstalled("ccvsvsse002-pets.esl");
		auto SaintSeducer_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse025-advdsgs.esm");
		auto WildHorses_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse034-mntuni.esl");

		auto Petcrab_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse010-petdwarvenarmoredmudcrab.esl");
		auto PetWolf_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse036-petbwolf.esl");
		auto PetHund_Installed = Serialization::CompletionistData::IsModInstalled("ccbgssse035-petnhound.esl");

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

		if (PetWolf_Installed) {
			CFramework_Pets_MP::Data.AddForm(0x000D63, "ccbgssse036-petbwolf.esl");
		}

		if (Petcrab_Installed) {
			CFramework_Pets_MP::Data.AddForm(0x000D62, "ccbgssse010-petdwarvenarmoredmudcrab.esl");
		}

		if (PetHund_Installed) {
			CFramework_Pets_MP::Data.AddForm(0x000D64, "ccbgssse035-petnhound.esl");
		}
		CFramework_Pets_MP::Data.Populate(Pets_MP_NameArray, Pets_MP_FormArray, Pets_MP_BoolArray, Pets_MP_TextArray, false);

		Pets_MP_TextArray.clear();

		if (PetWolf_Installed) {
			std::string name = RE::TESDataHandler::GetSingleton()->LookupForm(0x000D63, "ccbgssse036-petbwolf.esl")->GetName();
			Pets_MP_TextArray.push_back("$PetHighlightMisc00{" + name + "}{" + name + "}"); //Bone Wolf
		}

		if (Petcrab_Installed) {
			std::string name = RE::TESDataHandler::GetSingleton()->LookupForm(0x000D62, "ccbgssse010-petdwarvenarmoredmudcrab.esl")->GetName();
			Pets_MP_TextArray.push_back("$PetHighlightMisc01{" + name + "}{" + name + "}"); //Dwarven Armored Mudcrab
		}

		if (PetHund_Installed) {
			std::string name = RE::TESDataHandler::GetSingleton()->LookupForm(0x000D64, "ccbgssse035-petnhound.esl")->GetName();
			Pets_MP_TextArray.push_back("$PetHighlightMisc02{" + name + "}{" + name + "}"); //Nix-Hound
		}

		Pets_MP_EntriesTotal = Pets_MP_FormArray.size();
		Pets_MP_EntriesFound = std::ranges::count(Pets_MP_BoolArray, true);

		if (WildHorses_Installed) {
			CFramework_Pets_WH::Data.CompileFormArray(CFramework_Pets::WildHorses, "Completionist.esp");
			CFramework_Pets_WH::Data.Populate(Pets_WH_NameArray, Pets_WH_FormArray, Pets_WH_BoolArray, Pets_WH_TextArray, true);
			Pets_WH_TextArray.clear();
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses00{" + Pets_WH_NameArray[0] + "}{" + Pets_WH_NameArray[0] + "}"); //Black Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses01{" + Pets_WH_NameArray[1] + "}{" + Pets_WH_NameArray[1] + "}"); //Chestnut Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses02{" + Pets_WH_NameArray[2] + "}{" + Pets_WH_NameArray[2] + "}"); //Dapple Brown Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses03{" + Pets_WH_NameArray[3] + "}{" + Pets_WH_NameArray[3] + "}"); //Grey Spotted Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses04{" + Pets_WH_NameArray[4] + "}{" + Pets_WH_NameArray[4] + "}"); //Pale Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses05{" + Pets_WH_NameArray[5] + "}{" + Pets_WH_NameArray[5] + "}"); //Red Horse
			Pets_WH_TextArray.push_back("$PetHighlightWildHorses06{" + Pets_WH_NameArray[6] + "}{" + Pets_WH_NameArray[6] + "}"); //White Spotted Horse

			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[0], 0x000870));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[1], 0x00086D));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[2], 0x000875));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[3], 0x000871));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[4], 0x000872));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[5], 0x000873));
			WildHorsesMap.push_back(std::make_pair(Pets_WH_FormArray[6], 0x000874));

			Pets_WH_EntriesTotal = Pets_WH_FormArray.size();
			Pets_WH_EntriesFound = std::ranges::count(Pets_WH_BoolArray, true);
		}
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < Pets_VP_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_VP_FormArray[i], Pets_VP_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
		}
		for (auto i = 0; i < Pets_PS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_PS_FormArray[i], Pets_PS_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
		}
		for (auto i = 0; i < Pets_SS_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_SS_FormArray[i], Pets_SS_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
		}
		for (auto i = 0; i < Pets_MP_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_MP_FormArray[i], Pets_MP_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
		}
		for (auto i = 0; i < Pets_WH_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(Pets_WH_FormArray[i], Pets_WH_NameArray[i], "$MCMPagePets", std::to_underlying(EntryCategory::kPets)));
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

		for (auto i = 0; i < Pets_WH_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(Pets_WH_FormArray[i]->GetFormID())) {
				Pets_WH_BoolArray[i] = true;
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

		Pets_WH_EntriesTotal = Pets_WH_FormArray.size();
		Pets_WH_EntriesFound = std::ranges::count(Pets_WH_BoolArray, true);
	}
}