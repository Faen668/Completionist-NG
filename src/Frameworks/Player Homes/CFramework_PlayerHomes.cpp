#include "Serialization.hpp"
#include "CFramework_PlayerHomes.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

#undef AddForm

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Vanilla Homes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_PlayerHomes_V {

	using namespace CFramework_Master;

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<std::string> QuestArray{};
	std::vector<bool> BoolArray{};

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	RE::Actor* Player;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		RegisterEvents();
		InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			auto questID = std::string(quest->GetFormEditorID());

			if ((questID == "HousePurchase" && a_event->stage == 10) || (questID == "DLC2RR02" && a_event->stage == 200)) {
				ProcessFoundForm(questID);
			}

			if ((questID == "BYOHHouseFalkreath" || questID == "BYOHHouseHjaalmarch" || questID == "BYOHHousePale") && a_event->stage == 100) {
				ProcessFoundForm(questID);
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(std::string a_editorID) {

		if (a_editorID == "HousePurchase") {
			
			auto location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation();

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("WhiterunHoldLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_Breezehome";
			}

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("PaleHoldLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_HeljarchenHall";
			}

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("WindhelmLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_Hjerim";
			}

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("RiftenLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_Honeyside";
			}

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("SolitudeLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_ProudspireManor";
			}

			if (auto loc = RE::TESForm::LookupByEditorID<RE::BGSLocation>("MarkarthLocation"); location == loc || loc->IsChild(location)) {
				a_editorID = "HousePurchase_VlindrelHall";
			}
		}

		if (auto t_pos = std::ranges::find(QuestArray, a_editorID); t_pos != QuestArray.end()) {
			auto b_pos = std::distance(QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(FormArray[b_pos]);
			BoolArray[b_pos] = true;

			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}


	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		FormArray.clear();
		NameArray.clear();
		TextArray.clear();
		BoolArray.clear();
		QuestArray.clear();

		auto handler = RE::TESDataHandler::GetSingleton();

		FormArray.push_back(handler->LookupForm(0x000804, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight00"); //Breezehome
		QuestArray.push_back("HousePurchase_Breezehome");

		FormArray.push_back(handler->LookupForm(0x000807, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight01"); //Heljarchen Hall
		QuestArray.push_back("HousePurchase_HeljarchenHall");

		FormArray.push_back(handler->LookupForm(0x000808, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight02"); //Hjerim
		QuestArray.push_back("HousePurchase_Hjerim");

		FormArray.push_back(handler->LookupForm(0x000809, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight03"); //Honeyside
		QuestArray.push_back("HousePurchase_Honeyside");

		FormArray.push_back(handler->LookupForm(0x00080A, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight04"); //Lakeview Manor
		QuestArray.push_back("BYOHHouseFalkreath");

		FormArray.push_back(handler->LookupForm(0x00080B, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight05"); //Proudspire Manor
		QuestArray.push_back("HousePurchase_ProudspireManor");

		FormArray.push_back(handler->LookupForm(0x00080C, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight06"); //Severin Manor
		QuestArray.push_back("DLC2RR02");

		FormArray.push_back(handler->LookupForm(0x00080D, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight07"); //Vlindrel Hall
		QuestArray.push_back("HousePurchase_VlindrelHall");

		FormArray.push_back(handler->LookupForm(0x00080E, "Completionist.esp"));
		TextArray.push_back("$HouseHighlight08"); //Windstad Manor
		QuestArray.push_back("BYOHHouseHjaalmarch");

		for (auto form : FormArray) {
			NameArray.push_back(form->GetName());
		}

		NameArray.resize(FormArray.size());
		TextArray.resize(FormArray.size());
		BoolArray.resize(FormArray.size());
		QuestArray.resize(FormArray.size());

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData_NoShow.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData_NoShow.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Creation CLub Homes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_PlayerHomes_C {

	using namespace CFramework_Master;

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};
	std::vector<std::string> QuestArray{};

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		RegisterEvents();
		InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			auto questID = std::string(quest->GetFormEditorID());

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
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(std::string a_editorID) {

		if (auto t_pos = std::ranges::find(QuestArray, a_editorID); t_pos != QuestArray.end()) {
			auto b_pos = std::distance(QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(FormArray[b_pos]);
			BoolArray[b_pos] = true;

			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		FormArray.clear();
		NameArray.clear();
		TextArray.clear();
		BoolArray.clear();
		QuestArray.clear();

		auto handler = RE::TESDataHandler::GetSingleton();
		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse005-cave.esm"); Mod) {
			FormArray.push_back(handler->LookupForm(0x00080F, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight18"); //Bloodchill Manor
			QuestArray.push_back("ccEEJSSE005_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccbgssse031-advcyrus.esm"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000810, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight19"); //Dead Man's Dread
			QuestArray.push_back("ccBGSSSE031_HomeQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccrmssse001-necrohouse.esl"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000811, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight21"); //Gallows Hall
			QuestArray.push_back("ccRMSSSE001_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("ccvsvsse004-beafarmer.esl"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000812, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight20"); //Goldenhills Plantation
			QuestArray.push_back("ccVSVSSE004_MainQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse004-hall.esl"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000813, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight22"); //Hendraheim
			QuestArray.push_back("ccEEJSSE004_Quest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse002-tower.esl"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000814, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight23"); //Myrwatch
			QuestArray.push_back("ccEEJSSE002_MageTowerQuest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("ccafdsse001-dwesanctuary.esm"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000815, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight24"); //Nchuanthumz
			QuestArray.push_back("ccAARSSE001ManufactoryControlQuest");
		}

		if (const auto* Mod = handler->LookupLoadedLightModByName("cceejsse003-hollow.esl"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000816, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight26"); //Shadowfoot Sanctum
			QuestArray.push_back("ccEEJSSE003_HouseQuest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("cceejsse001-hstead.esm"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000817, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight25"); //Tundra Homestead
			QuestArray.push_back("ccEEJSSE001_Quest");
		}

		for (auto form : FormArray) {
			NameArray.push_back(form->GetName());
		}

		NameArray.resize(FormArray.size());
		TextArray.resize(FormArray.size());
		BoolArray.resize(FormArray.size());
		QuestArray.resize(FormArray.size());		

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData_NoShow.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData_NoShow.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------ Patches Homes
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

namespace CFramework_PlayerHomes_P {

	using namespace CFramework_Master;

	Serialization::CompletionistData Data;

	std::vector<std::string> NameArray{};
	std::vector<std::string> TextArray{};
	std::vector<RE::TESForm*> FormArray{};
	std::vector<bool> BoolArray{};
	std::vector<std::string> Identifier{};

	inline uint32_t EntriesTotal;
	inline uint32_t EntriesFound;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		RegisterEvents();
		InjectAndCompileData();
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		if (auto key = RE::TESForm::LookupByEditorID<RE::TESKey>("TC_HighHrotgarLibraryKey"); key && key->GetFormID() == a_event->baseObj) {
			ProcessFoundForm("TC_HighHrotgarLibraryKey");
			return EventResult::kContinue;
		}

		if (auto key = RE::TESForm::LookupByEditorID<RE::TESKey>("manny_GF_Key_AlikrPlayerHome01"); key && key->GetFormID() == a_event->baseObj) {
			ProcessFoundForm("manny_GF_Key_AlikrPlayerHome01");
			return EventResult::kContinue;
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Quest Stage ) ------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			
			auto questID = std::string(quest->GetFormEditorID());
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

		if (auto t_pos = std::ranges::find(Identifier, a_editorID); t_pos != Identifier.end()) {
			auto b_pos = std::distance(Identifier.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(FormArray[b_pos]->GetFormID())) {
				auto msg = fmt::format("Completionist: Entry Complete - {:s}!"sv, NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
			}

			FoundItemData_NoShow.AddForm(FormArray[b_pos]);
			BoolArray[b_pos] = true;

			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		FormArray.clear();
		NameArray.clear();
		TextArray.clear();
		BoolArray.clear();
		Identifier.clear();

		auto handler = RE::TESDataHandler::GetSingleton();
		if (const auto* Mod = handler->LookupLoadedModByName("Clockwork.esp"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A14, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight09"); //Clockwork Castle
			Identifier.push_back("CLWStory04Quest");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Wyrmstooth.esp"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A15, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight17"); //Fort Valus
			Identifier.push_back("WTPlayerHome");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Thunderchild - Epic Shout Package.esp"); Mod) {
			Data.AddForm(0x06B452, "Thunderchild - Epic Shout Package.esp");
			FormArray.push_back(handler->LookupForm(0x000A16, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight16"); //High Hrothgar Library
			Identifier.push_back("TC_HighHrotgarLibraryKey");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Dwarfsphere.esp"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A17, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight15"); //Hla Fang
			Identifier.push_back("DwaSpSadrithKegranHomeOwner");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Falskaar.esm"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A18, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight10"); //Horndew Lodge
			Identifier.push_back("FalskaarHousePurchase");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Gray Fox Cowl.esm"); Mod) {
			Data.AddForm(0x03C3EF, "Gray Fox Cowl.esm");
			FormArray.push_back(handler->LookupForm(0x000A19, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight11"); //Moonlight Home
			Identifier.push_back("manny_GF_Key_AlikrPlayerHome01");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("Helgen Reborn.esp"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A1A, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight12"); //Private Tower
			Identifier.push_back("BalokHelgen01");
		}

		if (const auto* Mod = handler->LookupLoadedModByName("moonpath.esp"); Mod) {
			FormArray.push_back(handler->LookupForm(0x000A1B, "Completionist.esp"));
			TextArray.push_back("$HouseHighlight14"); //The Priderock
			Identifier.push_back("AnvilQuestameir");
		}

		for (auto &form : FormArray) {
			if (form) {
				NameArray.push_back(form->GetName());
			}
		}

		NameArray.resize(FormArray.size());
		TextArray.resize(FormArray.size());
		BoolArray.resize(FormArray.size());
		Identifier.resize(FormArray.size());

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		for (auto i = 0; i < FormArray.size(); i++) {
			if (FoundItemData_NoShow.HasForm(FormArray[i]->GetFormID())) {
				BoolArray[i] = true;
			}
		}

		EntriesTotal = FormArray.size();
		EntriesFound = std::ranges::count(BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM is Entry Complete ) --
	//---------------------------------------------------

	uint32_t CHandler::IsOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			return uint32_t(BoolArray[std::distance(NameArray.begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Set Entry Complete ) -
	//---------------------------------------------------

	void CHandler::SetOptionCompleted(std::string a_name) {

		if (auto t_pos = std::ranges::find(NameArray, a_name); t_pos != NameArray.end()) {
			auto b_pos = std::distance(NameArray.begin(), t_pos);

			if (BoolArray.at(b_pos)) {
				BoolArray.at(b_pos) = false;

				FoundItemData_NoShow.RemoveForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.RemoveForm(var);
					}
				}
			}
			else {
				BoolArray.at(b_pos) = true;
				FoundItemData_NoShow.AddForm(FormArray.at(b_pos)->GetFormID());
				for (auto var : Data.GetAllVariations()) {
					if (Data.GetBase(var) == FormArray.at(b_pos)->GetFormID()) {
						FoundItemData_NoShow.AddForm(var);
					}
				}
			}

			EntriesTotal = FormArray.size();
			EntriesFound = std::ranges::count(BoolArray, true);
		}
	}
}