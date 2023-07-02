#include "PCH.h"
#include "CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/MCMHandler.hpp"

//Quest Frameworks
#include "Frameworks/Quests/Main Story/CQuests_MainStory_SK.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_CW.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_DG.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_DB.hpp"
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_01.hpp"
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_02.hpp"
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_03.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Dawnstar.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Falkreath.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Markarth.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Morthal.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Riften.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Solitude.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Whiterun.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Windhelm.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Winterhold.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_RavenRock.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_SkaalVillage.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_TelMithryn.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_Thirsk.hpp"
#include "Frameworks/Quests/Towns & Cities/CQuests_SmallTowns.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_CollegeOfWinterhold.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_Companions.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_DarkBrotherhood.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_Dawnguard.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_ThievesGuild.hpp"
#include "Frameworks/Quests/Guilds & Factions/CQuests_Vampires.hpp"
#include "Frameworks/Quests/Dungeons & Misc/CQuests_Dungeons.hpp"
#include "Frameworks/Quests/Dungeons & Misc/CQuests_Misc_SK.hpp"
#include "Frameworks/Quests/Dungeons & Misc/CQuests_Misc_DG.hpp"
#include "Frameworks/Quests/Dungeons & Misc/CQuests_Misc_DB.hpp"

#include "Frameworks/Quests/Radiant & Favors/Radiant Quests Manager.hpp"
#include "Frameworks/Quests/Radiant & Favors/Quests_Favors.hpp"


namespace CQuestMaster
{
	void QuestAPI::Register()
	{
		SinkEvents();
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);

		//Quests (Main Story)
		CQFramework_SK::CHandler::InstallFramework();
		CQFramework_CW::CHandler::InstallFramework();
		CQFramework_DG::CHandler::InstallFramework();
		CQFramework_DB::CHandler::InstallFramework();

		//Quests (Creation Club)
		CQFramework_CC1::CHandler::InstallFramework();
		CQFramework_CC2::CHandler::InstallFramework();
		CQFramework_CC3::CHandler::InstallFramework();

		//Quests (Towns & Cities)
		CQFramework_Dawnstar::CHandler::InstallFramework();
		CQFramework_Falkreath::CHandler::InstallFramework();
		CQFramework_Markarth::CHandler::InstallFramework();
		CQFramework_Morthal::CHandler::InstallFramework();
		CQFramework_Riften::CHandler::InstallFramework();
		CQFramework_Solitude::CHandler::InstallFramework();
		CQFramework_Whiterun::CHandler::InstallFramework();
		CQFramework_Windhelm::CHandler::InstallFramework();
		CQFramework_Winterhold::CHandler::InstallFramework();
		CQFramework_RavenRock::CHandler::InstallFramework();
		CQFramework_SkaalVillage::CHandler::InstallFramework();
		CQFramework_TelMithryn::CHandler::InstallFramework();
		CQFramework_Thirsk::CHandler::InstallFramework();
		CQFramework_SmallTowns::CHandler::InstallFramework();

		//Quests (Guilds & Factions)
		CQFramework_CollegeOfWinterhold::CHandler::InstallFramework();
		CQFramework_Companions::CHandler::InstallFramework();
		CQFramework_DarkBrotherhood::CHandler::InstallFramework();
		CQFramework_Dawnguard::CHandler::InstallFramework();
		CQFramework_ThievesGuild::CHandler::InstallFramework();
		CQFramework_Vampires::CHandler::InstallFramework();

		//Quests (Dungeons & Misc)
		CQFramework_Dungeons::CHandler::InstallFramework();
		CQFramework_Misc_SK::CHandler::InstallFramework();
		CQFramework_Misc_DG::CHandler::InstallFramework();
		CQFramework_Misc_DB::CHandler::InstallFramework();

		//Quests (Radiant Handler)
		Quest_Manager::Install();
		CQFramework_FavorQuests::CHandler::InstallFramework();
	}

	//---------------------------------------------------
	//-- Quest Functions ( Papyrus Registrations ) ------
	//---------------------------------------------------

	auto QuestAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("qDumpQuestData",		"Completionist_Native", qDumpQuestData);
		a_vm->RegisterFunction("qGetNameArrayByID",		"Completionist_Native", qGetNameArrayByID);
		a_vm->RegisterFunction("qGetTextArrayByID",		"Completionist_Native", qGetTextArrayByID);
		a_vm->RegisterFunction("qGetIdenArrayByID",		"Completionist_Native", qGetIdenArrayByID);
		a_vm->RegisterFunction("qGetBoolArrayByID",		"Completionist_Native", qGetBoolArrayByID);
		a_vm->RegisterFunction("qGetRadiArrayByID",		"Completionist_Native", qGetRadiArrayByID);

		a_vm->RegisterFunction("qIsOptionToggled",		"Completionist_Native",	qIsOptionToggled);		// Returns true if completed manually.
		a_vm->RegisterFunction("qIsOptionCompleted",	"Completionist_Native",	qIsOptionCompleted);	// Returns true if completed by any natural means.
		a_vm->RegisterFunction("qSetOptionCompleted",	"Completionist_Native",	qSetOptionCompleted);	// Only used to manually complete (CQuestKeys_Manual)
		
		a_vm->RegisterFunction("RegisterMerchant",		"Completionist_Native", CQFramework_FavorQuests::CHandler::RegisterMerchant);
		a_vm->RegisterFunction("UnRegisterMerchant",	"Completionist_Native", CQFramework_FavorQuests::CHandler::UnRegisterMerchant);

		a_vm->RegisterFunction("SearchAndReportPage",	"Completionist_Native", SearchAndReportPage);

		return true;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Sink Events ) ----------------
	//---------------------------------------------------

	void QuestAPI::SinkEvents()
	{
		auto UserInterface = RE::UI::GetSingleton();
		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(QuestAPI::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(QuestAPI::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(QuestAPI::GetSingleton()));
	};

	void QuestAPI::qDumpQuestData(RE::StaticFunctionTag*)
	{
		Dump();
	}

	//---------------------------------------------------
	//-- Quest Functions ( Dump Quest Data ) ------------
	//---------------------------------------------------

	void QuestAPI::Dump()
	{
		int32_t idx = 0;

		for (auto& [data, name, ID] : CQuestMaster::CQuestDataVec) {
			data->DumpToLog(idx, ID);
			idx++;
		};
	};

	void QuestAPI::ValidateLocalisation()
	{
		for (auto& [data, name, ID] : CQuestMaster::CQuestDataVec) {
			data->ValidateLocalisation();
		};
	};

	//---------------------------------------------------
	//-- Quest Functions ( MCM Quest Search ) -----------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::SearchAndReportPage(RE::StaticFunctionTag*, std::string s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t i_searchType)
	{
		auto maxResults = i_maxResults * 4;
		auto process = false;

		std::vector<std::string> list = CFramework_Master::FrameworkAPI::SearchAndReportPage(s_term, b_ignoreCompleted, maxResults, i_searchType);

		auto result = (list.size() / 4) + 1;

		INFO("Running Search For {} with a type of {}", s_term, i_searchType);

		for (auto& [data, name, ID] : CQuestMaster::CQuestDataVec) 
		{
			if (list.size() >= maxResults)
				break;

			switch (i_searchType)
			{
			case 0: { process = data->HasSearchData() && DKUtil::string::icontains(data->GetSearchTerm(), s_term); break; }
			case 1: { process = data->HasSearchData() && data->GetSearchTerm().starts_with(s_term); break; }
			case 2: { process = data->HasSearchData() && DKUtil::string::iequals(data->GetSearchTerm(), s_term); break; }

			default:
				break;
			}

			if (process) {
				list.push_back("$PageResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + GetLocalisedPageName(ID) + "}{" + data->GetName() + "}");
				list.push_back(GetLocalisedPageName(ID));
				list.push_back(data->GetSearchTerm());
				list.push_back("Quest");
				result++;
			}
		};

		for (auto i = 0; i < list.size(); i += 4)
		{
			list[i].replace(list[i].find("["), 9, std::to_string(list.size() / 4));
		}

		return list;
	};

	std::string QuestAPI::GetLocalisedPageName(int32_t ID)
	{
		for (auto& [page, mod, id] : CHCMHandler::MainMCMPagesDefs) {
			if (ID == id) {
				return page;
			}
		}

		return "";
	}

	//---------------------------------------------------
	//-- Quest Events ( On Menu Open ) ------------------
	//---------------------------------------------------

	EventResult QuestAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (a_event && a_event->menuName == RE::JournalMenu::MENU_NAME && a_event->opening)
		{
			QuestAPI::UpdateQuestCompletion();
		}
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Quest Events ( On Radiant Stage Set ) ----------
	//---------------------------------------------------

	EventResult QuestAPI::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource)
	{
		if (!a_event || !a_event->stage) {
			return EventResult::kContinue;
		}

		const auto* equest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!equest) {
			return EventResult::kContinue;
		}

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (data->HasRadiantData()) {
				const auto* bquest = RE::TESForm::LookupByID<RE::TESQuest>(data->GetRadiantBaseFormID());
				const auto* vquest = RE::TESForm::LookupByID<RE::TESQuest>(data->GetRadiantVariFormID());

				if (a_event->stage == data->radiant_data->stage) {
					if (bquest && bquest->GetFormID() == equest->GetFormID() || vquest && vquest->GetFormID() == equest->GetFormID()) {
						data->GetGlobal()->value++;
						INFO("Incremening global value on {} to {} for quest {}", data->GetKey(), data->GetGlobal()->value, equest->GetName());
						return EventResult::kContinue;
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added To Drunk ) ----
	//---------------------------------------------------

	EventResult QuestAPI::ProcessEvent(RE::TESContainerChangedEvent const* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (a_event && a_event->baseObj && a_event->oldContainer == RE::PlayerCharacter::GetSingleton()->GetFormID())
		{
			for (auto& [data, name, ID] : CQuestDataVec) {
				ProcessDrunkardQuest(data->drunk_data, a_event->baseObj, a_event->newContainer, RE::MenuTopicManager::GetSingleton()->speaker.get().get());
			}
		}
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Quest Events ( On Menu Open ) ------------------
	//---------------------------------------------------

	void QuestAPI::UpdateQuestCompletion()
	{
		for (auto& [data, name, ID] : CQuestDataVec) {
			data->array_data->bools->at(data->array_position) = IsQuestCompleted(data);
		}
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Idens ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetIdenArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (ID == a_ID) {
				return *data->array_data->editorids;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Names ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (ID == a_ID) {
				return *data->array_data->names;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Texts ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (ID == a_ID) {
				return *data->array_data->highlights;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Bools ) -------------
	//---------------------------------------------------

	std::vector<bool> QuestAPI::qGetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (ID == a_ID) {
				return *data->array_data->bools;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Radis ) -------------
	//---------------------------------------------------

	std::vector<int32_t> QuestAPI::qGetRadiArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		for (auto& [data, name, ID] : CQuestDataVec) {
			if (ID == a_ID) {
				return *data->array_data->types;
			}
		}
		return {};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Update Drunk Completion ) ----
	//---------------------------------------------------

	void QuestAPI::ProcessDrunkardQuest(CDrunkData* a_data, RE::FormID a_base, RE::FormID a_container, RE::TESObjectREFR* a_speaker)
	{	
		if (!a_data || !a_base || !a_container || !a_speaker) { return; }

		auto* drunk_list = static_cast<RE::BGSListForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(a_data->listID, a_data->file_name));
		auto* drunk_actr = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(a_data->formID, a_data->file_name));

		if (!drunk_actr || !drunk_list || !drunk_list->HasForm(a_base) || a_speaker->GetFormID() != a_container)
		{
			return;
		}

		if (a_speaker == drunk_actr && a_container == drunk_actr->GetFormID())
		{
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(a_data->globalvariable))
			{
				var->value++;
				INFO("Incrementing Global Variable Value for {}", drunk_actr->GetName());
			}
		}
	}

	//---------------------------------------------------
	//-- Quest Functions ( Update Quest Completion ) ----
	//---------------------------------------------------

	bool QuestAPI::IsQuestCompleted(CQuestData* a_data)
	{
		auto is_complete = false;

		switch (a_data->completion_type)
		{
		case CCompEnum::kStand: is_complete = a_data->GetQuest() && a_data->GetQuest()->data.flags.any(RE::QuestFlag::kCompleted, RE::QuestFlag::kFailed); break;
		case CCompEnum::kThane: is_complete = a_data->IsThane(); break;
		case CCompEnum::kGlobl: is_complete = a_data->GetGlobal()->value >= a_data->radiant_data->times_required; break;
		case CCompEnum::kStage: is_complete = IsStageDoneOrPast(a_data); break;
		default: break;
		}

		if (is_complete) {
			CFramework_Master::CQuestKeys_Natural.AddKey(a_data->GetKey());
			CFramework_Master::CQuestKeys_Manual.RemoveKey(a_data->GetKey());
			return true;
		}

		return CFramework_Master::CQuestKeys_Manual.HasKey(a_data->GetKey());;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Is Stage Done ) --------------
	//---------------------------------------------------

	bool QuestAPI::IsStageDoneOrPast(CQuestData* a_data)
	{
		switch (a_data->GetStageTypeEnum())
		{
		case CStageEnum::kDone: return IsStageDone(a_data->GetQuest(), a_data->GetStage()) || (a_data->HasOptionalStage() && IsStageDone(a_data->GetQuest(), a_data->GetOptionalStage())); break;
		case CStageEnum::kPast: return a_data->GetQuest()->currentStage > a_data->GetStage() || (a_data->HasOptionalStage() && a_data->GetQuest()->currentStage > a_data->GetOptionalStage()); break;
		default: return false;
		}
	}

	bool QuestAPI::IsStageDone(RE::TESQuest* a_quest, int32_t a_stage)
	{
		using func_t = decltype(IsStageDone);
		REL::Relocation<func_t> func{ RELOCATION_ID(24483, 25011) };
		return a_quest && func(a_quest, a_stage);
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	bool QuestAPI::qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_questname)
	{
		for (auto& [data, name, ID] : CQuestDataVec) {
			if (DKUtil::string::iequals(a_questname, name)) {
				return CFramework_Master::CQuestKeys_Manual.HasKey(data->GetKey());
			}
		}

		return false;
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	std::int32_t QuestAPI::qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_questname)
	{
		for (auto& [data, name, ID] : CQuestDataVec)
		{
			if (DKUtil::string::iequals(a_questname, name))
			{
				if (CFramework_Master::CQuestKeys_Natural.HasKey(data->GetKey()))
				{
					return -2;
				}
				return std::int32_t(data->IsCompleted());
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Setter - Status ) --------
	//---------------------------------------------------

	void QuestAPI::qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_questname)
	{
		for (auto& [data, name, ID] : CQuestDataVec)
		{
			if (DKUtil::string::iequals(a_questname, name))
			{
				data->Switch();

				if (data->IsCompleted()) {
					CFramework_Master::CQuestKeys_Manual.AddKey(data->GetKey());
				}
				else {
					CFramework_Master::CQuestKeys_Manual.RemoveKey(data->GetKey());
				}
				return;
			}
		}
	}
};