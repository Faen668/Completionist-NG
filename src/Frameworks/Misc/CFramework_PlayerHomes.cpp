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

		VH_NameArray.resize(VH_FormArray.size());
		VH_TextArray.resize(VH_FormArray.size());
		VH_BoolArray.resize(VH_FormArray.size());
		VH_QuestArray.resize(VH_FormArray.size());

		VH_EntriesTotal = VH_FormArray.size();
		VH_EntriesFound = std::ranges::count(VH_BoolArray, true);
	}

	void CHandler::InstallSearchTerms()
	{
		for (auto i = 0; i < VH_NameArray.size(); i++) {
			CFramework_Master::CItemsDataVec.push_back(std::make_tuple(VH_FormArray[i], VH_NameArray[i], "$MCMPageHouses", std::to_underlying(EntryCategory::kHome)));
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

		VH_EntriesTotal = VH_FormArray.size();
		VH_EntriesFound = std::ranges::count(VH_BoolArray, true);
	}
}