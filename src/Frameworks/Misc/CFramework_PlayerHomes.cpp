#include "Structs.hpp"
#include "CFramework_PlayerHomes.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

PlayerHomesData Vanilla_Homes[]{
	{0x000804, "Completionist.esp", 0,	 0, "$HouseHighlight00", "key_Breezehome",		0x093B08, "Skyrim.esm"}, //Breezehome
	{0x000807, "Completionist.esp", 100, 0, "$HouseHighlight01", "BYOHHousePale",		0,		  ""},			 //Heljarchen Hall
	{0x000808, "Completionist.esp", 0,	 0, "$HouseHighlight02", "key_Hjerim",			0x021679, "Skyrim.esm"}, //Hjerim
	{0x000809, "Completionist.esp", 0,	 0, "$HouseHighlight03", "key_Honeyside",		0x0A7B36, "Skyrim.esm"}, //Honeyside
	{0x00080A, "Completionist.esp", 100, 0, "$HouseHighlight04", "BYOHHouseFalkreath",	0,		  ""},			 //Lakeview Manor
	{0x00080B, "Completionist.esp", 0,	 0, "$HouseHighlight05", "key_ProudspireManor", 0x0A7B37, "Skyrim.esm"}, //Proudspire Manor
	{0x00080C, "Completionist.esp", 200, 0, "$HouseHighlight06", "DLC2RR02",			0,		  ""},			 //Severin Manor
	{0x00080D, "Completionist.esp", 0,	 0, "$HouseHighlight07", "key_VlindrelHall",	0x094391, "Skyrim.esm"}, //Vlindrel Hall
	{0x00080E, "Completionist.esp", 100, 0, "$HouseHighlight08", "BYOHHouseHjaalmarch",	0,		  ""},			 //Windstad Manor
};

PlayerHomesData CreationClub_Homes[]{
	{0x00080F, "Completionist.esp", 200, 300, "$HouseHighlight18", "ccEEJSSE005_Quest",					 0, "", "cceejsse005-cave.esm"},		 //Bloodchill Manor
	{0x000810, "Completionist.esp", 30,	 0,	  "$HouseHighlight19", "ccBGSSSE031_HomeQuest",				 0, "", "ccbgssse031-advcyrus.esm"},	 //Dead Man's Dread
	{0x000811, "Completionist.esp", 200, 0,	  "$HouseHighlight21", "ccRMSSSE001_Quest",					 0, "", "ccrmssse001-necrohouse.esl"},	 //Gallows Hall
	{0x000812, "Completionist.esp", 300, 0,	  "$HouseHighlight20", "ccVSVSSE004_MainQuest",				 0, "", "ccvsvsse004-beafarmer.esl"},	 //Goldenhills Plantation
	{0x000813, "Completionist.esp", 100, 0,	  "$HouseHighlight22", "ccEEJSSE004_Quest",					 0, "", "cceejsse004-hall.esl"},		 //Hendraheim
	{0x000814, "Completionist.esp", 100, 0,	  "$HouseHighlight23", "ccEEJSSE002_MageTowerQuest",		 0, "", "cceejsse002-tower.esl"},		 //Myrwatch
	{0x000815, "Completionist.esp", 100, 0,	  "$HouseHighlight24", "ccAARSSE001ManufactoryControlQuest", 0, "", "ccafdsse001-dwesanctuary.esm"}, //Nchuanthumz
	{0x000816, "Completionist.esp", 100, 0,	  "$HouseHighlight26", "ccEEJSSE003_HouseQuest",			 0, "", "cceejsse003-hollow.esl"},		 //Shadowfoot Sanctum
	{0x000817, "Completionist.esp", 10,	 0,	  "$HouseHighlight25", "ccEEJSSE001_Quest",					 0, "", "cceejsse001-hstead.esm"},		 //Tundra Homestead
};

PlayerHomesData ModAdded_Homes[]{ 
	{0x000A14, "Completionist.esp", 200, 0,  "$HouseHighlight09", "CLWStory04Quest",				0,			"Clockwork.esp"},							//Clockwork Castle
	{0x000A15, "Completionist.esp", 20,	 0,  "$HouseHighlight17", "WTPlayerHome",					0,			"Wyrmstooth.esp"},							//Fort Valus
	{0x000A16, "Completionist.esp", 0,	 0,  "$HouseHighlight16", "TC_HighHrotgarLibraryKey",		0x06B452,	"Thunderchild - Epic Shout Package.esp"},	//High Hrothgar Library
	{0x000A17, "Completionist.esp", 1,	 0,  "$HouseHighlight15", "DwaSpSadrithKegranHomeOwner",	0,			"Dwarfsphere.esp"},							//Hla Fang
	{0x000A18, "Completionist.esp", 10,	 0,  "$HouseHighlight10", "FalskaarHousePurchase",			0,			"Falskaar.esm"},							//Horndew Lodge
	{0x000A19, "Completionist.esp", 0,	 0,  "$HouseHighlight11", "manny_GF_Key_AlikrPlayerHome01",	0x03C3EF,	"Gray Fox Cowl.esm"},						//Moonlight Home
	{0x000A1A, "Completionist.esp", 20,	 0,  "$HouseHighlight12", "BalokHelgen01",					0,			"Helgen Reborn.esp"},						//Private Tower
	{0x000A1B, "Completionist.esp", 5,	 0,  "$HouseHighlight14", "AnvilQuestameir",				0,			"moonpath.esp"},							//The Priderock
};

namespace CFramework_PlayerHomes {
	using namespace CFramework_Master;

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallSearchTerms();
		FrameworkAPI::AddUpdateFoundForms(CHandler::UpdateFoundForms);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added  ) ------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (a_event && a_event->newContainer == RE::PlayerCharacter::GetSingleton()->GetFormID())
		{
			auto* key = RE::TESForm::LookupByID(a_event->baseObj);
			if (key && CFramework_PlayerHomes_VH::Data.HasForm(key))
			{
				for (auto& Data : Vanilla_Homes)
				{
					if (Data.KEY != 0) {
						auto* Data_Key = Serialization::CompletionistData::GetFullForm(Data.KEY, Data.PROVIDER);

						if (Data_Key && Data_Key->GetFormID() == key->GetFormID())
						{
							ProcessFoundForm(Data.IDEN);
							return EventResult::kContinue;
						}
					}
				}
				return EventResult::kContinue;
			}

			if (key && CFramework_PlayerHomes_PH::Data.HasForm(key))
			{
				for (auto& Data : ModAdded_Homes)
				{
					if (Data.KEY != 0)
					{
						auto* Data_Key = Serialization::CompletionistData::GetFullForm(Data.KEY, Data.PROVIDER);

						if (Data_Key && Data_Key->GetFormID() == key->GetFormID())
						{
							ProcessFoundForm(Data.IDEN);
							return EventResult::kContinue;
						}
					}
				}
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Quest Stage  ) -----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event) { return EventResult::kContinue; }

		auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto stage = a_event->stage;

		for (auto& Data : Vanilla_Homes) {
			if (DKUtil::string::iequals(quest->GetFormEditorID(), Data.IDEN) && ((Data.STAGE > 0 && Data.STAGE == stage) || (Data.STAGE2 > 0 && Data.STAGE2 == stage))) {
				ProcessFoundForm(Data.IDEN);
				return EventResult::kContinue;
			}
		}

		for (auto& Data : CreationClub_Homes) {
			if (DKUtil::string::iequals(quest->GetFormEditorID(), Data.IDEN) && ((Data.STAGE > 0 && Data.STAGE == stage) || (Data.STAGE2 > 0 && Data.STAGE2 == stage))) {
				ProcessFoundForm(Data.IDEN);
				return EventResult::kContinue;
			}
		}

		for (auto& Data : ModAdded_Homes) {
			if (DKUtil::string::iequals(quest->GetFormEditorID(), Data.IDEN) && ((Data.STAGE > 0 && Data.STAGE == stage) || (Data.STAGE2 > 0 && Data.STAGE2 == stage))) {
				ProcessFoundForm(Data.IDEN);
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(std::string a_editorID) {

		if (auto t_pos = std::ranges::find(VH_QuestArray, a_editorID); t_pos != VH_QuestArray.end()) 
		{
			auto b_pos = std::distance(VH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(VH_FormArray[b_pos]->GetFormID())) 
			{
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, VH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kObtained, VH_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(VH_FormArray[b_pos]);

			VH_BoolArray[b_pos] = true;
			VH_EntriesFound = std::ranges::count(VH_BoolArray, true);
		}

		if (auto t_pos = std::ranges::find(CH_QuestArray, a_editorID); t_pos != CH_QuestArray.end()) 
		{
			auto b_pos = std::distance(CH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(CH_FormArray[b_pos]->GetFormID())) 
			{
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kObtained, CH_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(CH_FormArray[b_pos]);

			CH_BoolArray[b_pos] = true;
			CH_EntriesFound = std::ranges::count(CH_BoolArray, true);
		}

		if (auto t_pos = std::ranges::find(PH_QuestArray, a_editorID); t_pos != PH_QuestArray.end()) 
		{
			auto b_pos = std::distance(PH_QuestArray.begin(), t_pos);

			if (!FoundItemData_NoShow.HasForm(PH_FormArray[b_pos]->GetFormID())) 
			{
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, PH_NameArray[b_pos]);
				FrameworkAPI::SendNotification(msg, "NotifySpecial");
				FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kObtained, PH_NameArray[b_pos]);
			}

			FoundItemData_NoShow.AddForm(PH_FormArray[b_pos]);

			PH_BoolArray[b_pos] = true;
			PH_EntriesFound = std::ranges::count(PH_BoolArray, true);
		}
	}


	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() 
	{
		for (auto& Data : Vanilla_Homes) 
		{
			if (auto* form = Serialization::CompletionistData::GetFullForm<RE::TESForm>(Data.FORM, Data.FORM_PROVIDER)) 
			{
				VH_FormArray.push_back(form);
				VH_NameArray.push_back(form->GetName());
				VH_TextArray.push_back(Data.TEXT);
				VH_QuestArray.push_back(Data.IDEN);
				if (Data.KEY != 0) 
				{
					CFramework_PlayerHomes_VH::Data.AddForm(Data.KEY, Data.PROVIDER);
				}
			}
		}

		for (auto& Data : CreationClub_Homes)
		{
			if (!Serialization::CompletionistData::IsModInstalled(Data.MODNAME)) {
				continue;
			}

			if (auto* form = Serialization::CompletionistData::GetFullForm<RE::TESForm>(Data.FORM, Data.FORM_PROVIDER)) 
			{
				CH_FormArray.push_back(form);
				CH_NameArray.push_back(form->GetName());
				CH_TextArray.push_back(Data.TEXT);
				CH_QuestArray.push_back(Data.IDEN);
				if (Data.KEY != 0) 
				{
					CFramework_PlayerHomes_CH::Data.AddForm(Data.KEY, Data.PROVIDER);
				}
			}
		}

		for (auto& Data : ModAdded_Homes) 
		{
			if (!Serialization::CompletionistData::IsModInstalled(Data.PROVIDER)) {
				continue;
			}

			if (auto* form = Serialization::CompletionistData::GetFullForm<RE::TESForm>(Data.FORM, Data.FORM_PROVIDER)) 
			{
				PH_FormArray.push_back(form);
				PH_NameArray.push_back(form->GetName());
				PH_TextArray.push_back(Data.TEXT);
				PH_QuestArray.push_back(Data.IDEN);
				if (Data.KEY != 0) 
				{
					CFramework_PlayerHomes_PH::Data.AddForm(Data.KEY, Data.PROVIDER);
				}
			}
		}

		VH_NameArray.resize(VH_FormArray.size());
		VH_TextArray.resize(VH_FormArray.size());
		VH_BoolArray.resize(VH_FormArray.size());
		VH_QuestArray.resize(VH_FormArray.size());

		CH_NameArray.resize(CH_FormArray.size());
		CH_TextArray.resize(CH_FormArray.size());
		CH_BoolArray.resize(CH_FormArray.size());
		CH_QuestArray.resize(CH_FormArray.size());

		PH_NameArray.resize(PH_FormArray.size());
		PH_TextArray.resize(PH_FormArray.size());
		PH_BoolArray.resize(PH_FormArray.size());
		PH_QuestArray.resize(PH_FormArray.size());

		VH_EntriesTotal = VH_FormArray.size();
		VH_EntriesFound = std::ranges::count(VH_BoolArray, true);

		CH_EntriesTotal = CH_FormArray.size();
		CH_EntriesFound = std::ranges::count(CH_BoolArray, true);

		PH_EntriesTotal = PH_FormArray.size();
		PH_EntriesFound = std::ranges::count(PH_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto& name : VH_NameArray)
		{
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageHouses", std::to_underlying(EntryCategory::kHome)));
		}

		for (auto& name : CH_NameArray)
		{
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageHouses", std::to_underlying(EntryCategory::kHome)));
		}

		for (auto& name : PH_NameArray)
		{
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(name, "$MCMPageHouses", std::to_underlying(EntryCategory::kHome)));
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() 
	{
		for (auto i = 0; i < VH_FormArray.size(); i++) 
		{
			if (VH_FormArray[i] && FoundItemData_NoShow.HasForm(VH_FormArray[i]->GetFormID())) 
			{
				VH_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < CH_FormArray.size(); i++) 
		{
			if (CH_FormArray[i] && FoundItemData_NoShow.HasForm(CH_FormArray[i]->GetFormID())) 
			{
				CH_BoolArray[i] = true;
			}
		}

		for (auto i = 0; i < PH_FormArray.size(); i++) 
		{
			if (PH_FormArray[i] && FoundItemData_NoShow.HasForm(PH_FormArray[i]->GetFormID())) 
			{
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
}