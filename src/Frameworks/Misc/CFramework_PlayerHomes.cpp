#include "Serialization.hpp"
#include "CFramework_PlayerHomes.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CFramework_PlayerHomes {
	using namespace CFramework_Master;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		RegisterEvents();
		InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added  ) ------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014) { return EventResult::kContinue; }

		auto* key = RE::TESForm::LookupByID(a_event->baseObj);
		if (!key) { return EventResult::kContinue; }

		if (CFramework_PlayerHomes_VH::Data.HasForm(a_event->baseObj)) {

			if (auto* k1 = RE::TESDataHandler::GetSingleton()->LookupForm(0x093B08, "Skyrim.esm"); k1 && k1 == key) {
				ProcessFoundForm("key_Breezehome");
				return EventResult::kContinue;
			}

			if (auto* k2 = RE::TESDataHandler::GetSingleton()->LookupForm(0x021679, "Skyrim.esm"); k2 && k2 == key) {
				ProcessFoundForm("key_Hjerim");
				return EventResult::kContinue;
			}

			if (auto* k3 = RE::TESDataHandler::GetSingleton()->LookupForm(0x0A7B36, "Skyrim.esm"); k3 && k3 == key) {
				ProcessFoundForm("key_Honeyside");
				return EventResult::kContinue;
			}

			if (auto* k4 = RE::TESDataHandler::GetSingleton()->LookupForm(0x0A7B37, "Skyrim.esm"); k4 && k4 == key) {
				ProcessFoundForm("key_ProudspireManor");
				return EventResult::kContinue;
			}

			if (auto* k5 = RE::TESDataHandler::GetSingleton()->LookupForm(0x094391, "Skyrim.esm"); k5 && k5 == key) {
				ProcessFoundForm("key_VlindrelHall");
				return EventResult::kContinue;
			}
		}

		if (CFramework_PlayerHomes_PH::Data.HasForm(a_event->baseObj)) {

			if (auto key = RE::TESForm::LookupByEditorID<RE::TESKey>("TC_HighHrotgarLibraryKey"); key && key->GetFormID() == a_event->baseObj) {
				ProcessFoundForm("TC_HighHrotgarLibraryKey");
				return EventResult::kContinue;
			}

			if (auto key = RE::TESForm::LookupByEditorID<RE::TESKey>("manny_GF_Key_AlikrPlayerHome01"); key && key->GetFormID() == a_event->baseObj) {
				ProcessFoundForm("manny_GF_Key_AlikrPlayerHome01");
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Quest Stage  ) -----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			auto questID = std::string(quest->GetFormEditorID());

			//Vanilla
			if (questID == "DLC2RR02" && a_event->stage == 200) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}

			if ((questID == "BYOHHouseFalkreath" || questID == "BYOHHouseHjaalmarch" || questID == "BYOHHousePale") && a_event->stage == 100) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}

			//Creations
			if (questID == "ccEEJSSE001_Quest" && a_event->stage == 10) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccEEJSSE002_MageTowerQuest" && a_event->stage == 100) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccEEJSSE003_HouseQuest" && a_event->stage == 100) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccAARSSE001ManufactoryControlQuest" && a_event->stage == 100) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccEEJSSE004_Quest" && a_event->stage == 100) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccRMSSSE001_Quest" && a_event->stage == 200) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccEEJSSE005_Quest" && (a_event->stage == 200 || a_event->stage == 300)) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccBGSSSE031_HomeQuest" && a_event->stage == 30) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "ccVSVSSE004_MainQuest" && a_event->stage == 300) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}

			//Patches
			if (questID == "CLWStory04Quest" && a_event->stage == 200) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "WTPlayerHome" && a_event->stage == 20) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "DwaSpSadrithKegranHomeOwner" && a_event->stage == 1) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "FalskaarHousePurchase" && a_event->stage == 10) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "BalokHelgen01" && a_event->stage == 20) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
			if (questID == "AnvilQuestameir" && a_event->stage == 5) {
				ProcessFoundForm(questID);
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(std::string a_editorID) {

		if (auto t_pos = std::ranges::find(VH_QuestArray, a_editorID); t_pos != VH_QuestArray.end()) {
			auto b_pos = std::distance(VH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(VH_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, VH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(VH_FormArray[b_pos]);
			
			VH_BoolArray[b_pos] = true;
			VH_EntriesFound = std::ranges::count(VH_BoolArray, true);
		}

		if (auto t_pos = std::ranges::find(CH_QuestArray, a_editorID); t_pos != CH_QuestArray.end()) {
			auto b_pos = std::distance(CH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(CH_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, CH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(CH_FormArray[b_pos]);
			
			CH_BoolArray[b_pos] = true;
			CH_EntriesFound = std::ranges::count(CH_BoolArray, true);
		}

		if (auto t_pos = std::ranges::find(PH_QuestArray, a_editorID); t_pos != PH_QuestArray.end()) {
			auto b_pos = std::distance(PH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(PH_FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, PH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(PH_FormArray[b_pos]);
			
			PH_BoolArray[b_pos] = true;
			PH_EntriesFound = std::ranges::count(PH_BoolArray, true);
		}
	}


	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CHandler::Compile_VH();
		CHandler::Compile_CH();
		CHandler::Compile_PH();

		VH_EntriesTotal = VH_FormArray.size();
		VH_EntriesFound = std::ranges::count(VH_BoolArray, true);

		CH_EntriesTotal = CH_FormArray.size();
		CH_EntriesFound = std::ranges::count(CH_BoolArray, true);

		PH_EntriesTotal = PH_FormArray.size();
		PH_EntriesFound = std::ranges::count(PH_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < VH_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(VH_FormArray[i]->GetFormID())) {
				VH_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < CH_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(CH_FormArray[i]->GetFormID())) {
				CH_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < PH_FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(PH_FormArray[i]->GetFormID())) {
				PH_BoolArray[i] = true;
			}
		}

		VH_EntriesTotal = VH_FormArray.size();
		VH_EntriesFound = std::ranges::count(VH_BoolArray, true);

		CH_EntriesTotal = CH_FormArray.size();
		CH_EntriesFound = std::ranges::count(CH_BoolArray, true);

		PH_EntriesTotal = PH_FormArray.size();
		PH_EntriesFound = std::ranges::count(PH_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Compile Vanilla Homes ) --
	//---------------------------------------------------

	void CHandler::Compile_VH() {

		VH_FormArray.clear();
		VH_NameArray.clear();
		VH_TextArray.clear();
		VH_BoolArray.clear();
		VH_QuestArray.clear();

		auto handler = RE::TESDataHandler::GetSingleton();

		VH_FormArray.push_back(handler->LookupForm(0x000804, "Completionist.esp"));
		CFramework_PlayerHomes_VH::Data.AddForm(0x093B08, "Skyrim.esm");
		VH_TextArray.push_back("$HouseHighlight00"); //Breezehome
		VH_QuestArray.push_back("key_Breezehome");

		VH_FormArray.push_back(handler->LookupForm(0x000807, "Completionist.esp"));
		VH_TextArray.push_back("$HouseHighlight01"); //Heljarchen Hall
		VH_QuestArray.push_back("BYOHHousePale");

		VH_FormArray.push_back(handler->LookupForm(0x000808, "Completionist.esp"));
		CFramework_PlayerHomes_VH::Data.AddForm(0x021679, "Skyrim.esm");
		VH_TextArray.push_back("$HouseHighlight02"); //Hjerim
		VH_QuestArray.push_back("key_Hjerim");

		VH_FormArray.push_back(handler->LookupForm(0x000809, "Completionist.esp"));
		CFramework_PlayerHomes_VH::Data.AddForm(0x0A7B36, "Skyrim.esm");
		VH_TextArray.push_back("$HouseHighlight03"); //Honeyside
		VH_QuestArray.push_back("key_Honeyside");

		VH_FormArray.push_back(handler->LookupForm(0x00080A, "Completionist.esp"));
		VH_TextArray.push_back("$HouseHighlight04"); //Lakeview Manor
		VH_QuestArray.push_back("BYOHHouseFalkreath");

		VH_FormArray.push_back(handler->LookupForm(0x00080B, "Completionist.esp"));
		CFramework_PlayerHomes_VH::Data.AddForm(0x0A7B37, "Skyrim.esm");
		VH_TextArray.push_back("$HouseHighlight05"); //Proudspire Manor
		VH_QuestArray.push_back("key_ProudspireManor");

		VH_FormArray.push_back(handler->LookupForm(0x00080C, "Completionist.esp"));
		VH_TextArray.push_back("$HouseHighlight06"); //Severin Manor
		VH_QuestArray.push_back("DLC2RR02");

		VH_FormArray.push_back(handler->LookupForm(0x00080D, "Completionist.esp"));
		CFramework_PlayerHomes_VH::Data.AddForm(0x094391, "Skyrim.esm");
		VH_TextArray.push_back("$HouseHighlight07"); //Vlindrel Hall
		VH_QuestArray.push_back("key_VlindrelHall");

		VH_FormArray.push_back(handler->LookupForm(0x00080E, "Completionist.esp"));
		VH_TextArray.push_back("$HouseHighlight08"); //Windstad Manor
		VH_QuestArray.push_back("BYOHHouseHjaalmarch");

		for (auto form : VH_FormArray) {
			VH_NameArray.push_back(form->GetName());
		}

		VH_NameArray.resize(VH_FormArray.size());
		VH_TextArray.resize(VH_FormArray.size());
		VH_BoolArray.resize(VH_FormArray.size());
		VH_QuestArray.resize(VH_FormArray.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Compile Creation Homes ) -
	//---------------------------------------------------

	void CHandler::Compile_CH() {

		CH_FormArray.clear();
		CH_NameArray.clear();
		CH_TextArray.clear();
		CH_BoolArray.clear();
		CH_QuestArray.clear();

		auto handler = RE::TESDataHandler::GetSingleton();
		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse005-cave.esm"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x00080F, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight18"); //Bloodchill Manor
			CH_QuestArray.push_back("ccEEJSSE005_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000810, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight19"); //Dead Man's Dread
			CH_QuestArray.push_back("ccBGSSSE031_HomeQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccrmssse001-necrohouse.esl"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000811, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight21"); //Gallows Hall
			CH_QuestArray.push_back("ccRMSSSE001_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccvsvsse004-beafarmer.esl"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000812, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight20"); //Goldenhills Plantation
			CH_QuestArray.push_back("ccVSVSSE004_MainQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse004-hall.esl"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000813, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight22"); //Hendraheim
			CH_QuestArray.push_back("ccEEJSSE004_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse002-tower.esl"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000814, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight23"); //Myrwatch
			CH_QuestArray.push_back("ccEEJSSE002_MageTowerQuest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccafdsse001-dwesanctuary.esm"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000815, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight24"); //Nchuanthumz
			CH_QuestArray.push_back("ccAARSSE001ManufactoryControlQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse003-hollow.esl"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000816, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight26"); //Shadowfoot Sanctum
			CH_QuestArray.push_back("ccEEJSSE003_HouseQuest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse001-hstead.esm"); Mod) {
			CH_FormArray.push_back(handler->LookupForm(0x000817, "Completionist.esp"));
			CH_TextArray.push_back("$HouseHighlight25"); //Tundra Homestead
			CH_QuestArray.push_back("ccEEJSSE001_Quest");
		}

		for (auto form : CH_FormArray) {
			CH_NameArray.push_back(form->GetName());
		}

		CH_NameArray.resize(CH_FormArray.size());
		CH_TextArray.resize(CH_FormArray.size());
		CH_BoolArray.resize(CH_FormArray.size());
		CH_QuestArray.resize(CH_FormArray.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Compile Creation Homes ) -
	//---------------------------------------------------

	void CHandler::Compile_PH() {

		PH_FormArray.clear();
		PH_NameArray.clear();
		PH_TextArray.clear();
		PH_BoolArray.clear();
		PH_QuestArray.clear();

		auto handler = RE::TESDataHandler::GetSingleton();
		if (const auto* Mod = handler->LookupLoadedModByName("Clockwork.esp"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A14, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight09"); //Clockwork Castle
			PH_QuestArray.push_back("CLWStory04Quest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Wyrmstooth.esp"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A15, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight17"); //Fort Valus
			PH_QuestArray.push_back("WTPlayerHome");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Thunderchild - Epic Shout Package.esp"); Mod) {
			CFramework_PlayerHomes_PH::Data.AddForm(0x06B452, "Thunderchild - Epic Shout Package.esp");
			PH_FormArray.push_back(handler->LookupForm(0x000A16, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight16"); //High Hrothgar Library
			PH_QuestArray.push_back("TC_HighHrotgarLibraryKey");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Dwarfsphere.esp"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A17, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight15"); //Hla Fang
			PH_QuestArray.push_back("DwaSpSadrithKegranHomeOwner");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Falskaar.esm"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A18, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight10"); //Horndew Lodge
			PH_QuestArray.push_back("FalskaarHousePurchase");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Gray Fox Cowl.esm"); Mod) {
			CFramework_PlayerHomes_PH::Data.AddForm(0x03C3EF, "Gray Fox Cowl.esm");
			PH_FormArray.push_back(handler->LookupForm(0x000A19, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight11"); //Moonlight Home
			PH_QuestArray.push_back("manny_GF_Key_AlikrPlayerHome01");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Helgen Reborn.esp"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A1A, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight12"); //Private Tower
			PH_QuestArray.push_back("BalokHelgen01");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("moonpath.esp"); Mod) {
			PH_FormArray.push_back(handler->LookupForm(0x000A1B, "Completionist.esp"));
			PH_TextArray.push_back("$HouseHighlight14"); //The Priderock
			PH_QuestArray.push_back("AnvilQuestameir");
		}

		for (auto& form : PH_FormArray) {
			if (form) {
				PH_NameArray.push_back(form->GetName());
			}
		}

		PH_NameArray.resize(PH_FormArray.size());
		PH_TextArray.resize(PH_FormArray.size());
		PH_BoolArray.resize(PH_FormArray.size());
		PH_QuestArray.resize(PH_FormArray.size());
	}
}