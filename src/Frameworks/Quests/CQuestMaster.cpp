 #include "PCH.h"
#include "CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "Internal Utility/Events.hpp"
#include "Internal Utility/PatchListener.hpp"

//Quest Frameworks
#include "Frameworks/Quests/Main Story/CQuests_MainStory_SK.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_CW.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_DG.hpp"
#include "Frameworks/Quests/Main Story/CQuests_MainStory_DB.hpp"
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
#include "Frameworks/Quests/Radiant & Favors/CQuests_Favors.hpp"
#include "Frameworks/Quests/Radiant & Favors/CQuests_Bounties.hpp"
#include "Frameworks/Quests/Radiant & Favors/CQuests_Beggars.hpp"
#include "Frameworks/Quests/Patches/CQuests_Patches.hpp"

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
		CQFramework_Beggars::CHandler::InstallFramework();
		CQFramework_Bounties::CHandler::InstallFramework();
		CQFramework_FavorQuests::CHandler::InstallFramework();

		//Quests (Patches)
		CQFramework_Patches::CHandler::InstallFramework();
	}

	//---------------------------------------------------
	//-- Quest Functions ( Papyrus Registrations ) ------
	//---------------------------------------------------

	auto QuestAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("qDumpQuestData",		"Completionist_Native", qDumpQuestData);
		a_vm->RegisterFunction("qGetNameArrayByName",	"Completionist_Native", qGetNameArrayByID);
		a_vm->RegisterFunction("qGetTextArrayByName",	"Completionist_Native", qGetTextArrayByID);
		a_vm->RegisterFunction("qGetIdenArrayByName",	"Completionist_Native", qGetIdenArrayByID);
		a_vm->RegisterFunction("qGetRadiArrayByName",	"Completionist_Native", qGetRadiArrayByID);
		a_vm->RegisterFunction("qGetKeysArrayByName",	"Completionist_Native", qGetKeysArrayByID);

		a_vm->RegisterFunction("qIsOptionToggled",		"Completionist_Native",	qIsOptionToggled);		// Returns true if completed manually.
		a_vm->RegisterFunction("qIsOptionCompleted",	"Completionist_Native",	qIsOptionCompleted);	// Returns true if completed by any natural means.
		a_vm->RegisterFunction("qSetOptionCompleted",	"Completionist_Native",	qSetOptionCompleted);	// Only used to manually complete (CQuestKeys_Manual)
		
		a_vm->RegisterFunction("RegisterMerchant",		"Completionist_Native", CQFramework_FavorQuests::CHandler::RegisterMerchant);
		a_vm->RegisterFunction("UnRegisterMerchant",	"Completionist_Native", CQFramework_FavorQuests::CHandler::UnRegisterMerchant);

		a_vm->RegisterFunction("SearchAndReportPage",	"Completionist_Native", SearchAndReportPage);
		a_vm->RegisterFunction("qGetTimesCompletedVsTimesRequiredText", "Completionist_Native", qGetTimesCompletedVsTimesRequiredText);

		a_vm->RegisterFunction("qGetMiscQuestIdenArrayByID", "Completionist_Native", qGetMiscQuestIdenArrayByID);
		a_vm->RegisterFunction("qGetMiscQuestKeysArrayByID", "Completionist_Native", qGetMiscQuestKeysArrayByID);
		a_vm->RegisterFunction("qGetMiscQuestNameArrayByID", "Completionist_Native", qGetMiscQuestNameArrayByID);
		a_vm->RegisterFunction("qGetMiscQuestTextArrayByID", "Completionist_Native", qGetMiscQuestTextArrayByID);
		a_vm->RegisterFunction("qGetMiscQuestRadiArrayByID", "Completionist_Native", qGetMiscQuestRadiArrayByID);
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

	//---------------------------------------------------
	//-- Quest Functions ( Add Quest Data ) -------------
	//---------------------------------------------------

	void QuestAPI::AddQuestData(CQuestData* a_data, std::string a_name, int32_t a_ID, std::string a_key) 
	{
		CQuestDataVec.push_back(std::make_tuple(a_data, a_name, a_ID, a_key));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Dump Quest Data ) ------------
	//---------------------------------------------------

	void QuestAPI::qDumpQuestData(RE::StaticFunctionTag*)
	{
		int32_t idx = 0;

		for (auto& [data, name, ID, key] : CQuestMaster::CQuestDataVec) {
			data->DumpToLog(idx, ID);

			auto count = CFramework_Master::RadiantCountData.GetCount(key);
			if (count > 0)
			{
				INFO("Times Completed = {}", count)
			}

			idx++;
		};
	}

	//---------------------------------------------------
	//-- Quest Functions ( Dump Quest Data ) ------------
	//---------------------------------------------------

	void QuestAPI::ValidateLocalisation()
	{
		for (auto& [data, name, ID, key] : CQuestMaster::CQuestDataVec) {
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

		for (auto& [data, name, ID, key] : CQuestMaster::CQuestDataVec)
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
					
				if (b_ignoreCompleted && (CFramework_Master::CQuestKeys_Manual.HasKey(key) || CFramework_Master::CQuestKeys_Natural.HasKey(key))) {
					continue;
				}

				if (!data->isNative)
				{
					list.push_back("$MiscResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + data->mcmPage + "}{" + GET_LOC_STRING_BY_KEY("Category_Quest") + "}{" + name + "}");
					list.push_back(data->mcmPage);
					list.push_back(data->GetSearchTerm());
					list.push_back("Misc");
				}
				else
				{
					list.push_back("$PageResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + GetLocalisedPageName(ID) + "}{" + data->GetName() + "}");
					list.push_back(GetLocalisedPageName(ID));
					list.push_back(data->GetSearchTerm());
					list.push_back("Quest");
				}
				result++;
			}
		};

		for (auto i = 0; i < list.size(); i += 4)
		{
			list[i].replace(list[i].find("["), 9, std::to_string(list.size() / 4));
		}

		return list;
	};

	//---------------------------------------------------
	//-- Quest Functions ( MCM Quest Search ) -----------
	//---------------------------------------------------

	std::string QuestAPI::GetLocalisedPageName(int32_t ID)
	{
		for (auto& [page, mod, id] : CHCMHandler::MainMCMPagesDefs) {
			if (ID == id) {
				return page;
			}
		}

		for (auto& [page, mod, id] : CHCMHandler::MainPatchesMCMPagesDefs) {
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
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				data->is_completed = IsQuestCompleted(data);
			};
		}
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Quest Events ( On Radiant Stage Set ) ----------
	//---------------------------------------------------
	
	EventResult QuestAPI::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource)
	{	
		if (!a_event || !a_event->stage) 
		{
			return EventResult::kContinue;
		}

		const auto* equest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!equest)
		{
			return EventResult::kContinue;
		}

		for (auto& [data, name, ID, key] : CQuestDataVec) 
		{
			const auto* cquest = data->GetQuest();
			if (!cquest || data->ShouldProcess() == CQuestProcessor::kExcluded || (cquest && cquest->GetFormID() != equest->GetFormID()))
			{
				continue;
			}

			if (!data->HasRadiantData() && !CFramework_Master::CQuestKeys_Natural.HasKey(data->GetKey()))
			{
				CheckForCompletionFromStageEvent(data, a_event->stage);
			};

			if (data->HasRadiantData() && a_event->stage == data->radiant_data->stage)
			{
				CFramework_Master::RadiantCountData.IncreaseCount(data->GetKey(), 1);
				//INFO("Incremening times completed on {} for quest [{}]", data->GetKey(), equest->GetName());

				if ((!CFramework_Master::CQuestKeys_Natural.HasKey(data->GetKey())) && (CFramework_Master::RadiantCountData.GetCount(data->GetKey()) >= data->GetRadiantTimesRequired()))
				{
					data->is_completed = true;
					CFramework_Master::CQuestKeys_Natural.AddKey(data->GetKey());
					CFramework_Master::CQuestKeys_Manual.RemoveKey(data->GetKey());
					CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kQuestComplete, data->GetName());
					//INFO("Setting {} to completed status for quest [{}]", data->GetKey(), equest->GetName());
					return EventResult::kContinue;
				}
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Quest Data ) -------------
	//---------------------------------------------------

	void QuestAPI::CheckForCompletionFromStageEvent(CQuestData* data, int32_t a_stage)
	{
		bool is_complete = false;
		switch (data->completion_type)
		{
		case CCompEnum::kStand: { is_complete = IsQuestCompletedNaturally(data); break; }
		case CCompEnum::kThane: { is_complete = IsThaneOfHold(data); break; }
		case CCompEnum::kFavor: { is_complete = IsFavorCompleted(data); break; }
		case CCompEnum::kStage: { is_complete = IsStageDoneOrPast(data); break; }
		default: break;
		}

		//INFO("Checking {} at stage {} - {}", data->GetKey(), a_stage, is_complete);

		if (is_complete) {

			if (DKUtil::string::iequals(data->GetKey(), "MSQ_Quest05")) {
				auto* _data = GetQuestDataByKey("Whiterun_Quest17");
				CheckForCompletionFromStageEvent(_data, a_stage);
			};

			data->is_completed = true;
			CFramework_Master::RadiantCountData.IncreaseCount(data->GetKey(), 1);
			CFramework_Master::CQuestKeys_Natural.AddKey(data->GetKey());
			CFramework_Master::CQuestKeys_Manual.RemoveKey(data->GetKey());
			CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kQuestComplete, data->GetName());
			//INFO("Setting {} to completed status for quest [{}]", data->GetKey(), data->GetName());
		}
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Quest Data ) -------------
	//---------------------------------------------------

	CQuestData* QuestAPI::GetQuestDataByKey(std::string a_key)
	{
		for (auto& [data, name, ID, key] : CQuestDataVec) {
			if (DKUtil::string::iequals(a_key, key))
			{
				return data;
			}
		}
		return nullptr;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added To Drunk ) ----
	//---------------------------------------------------

	EventResult QuestAPI::ProcessEvent(RE::TESContainerChangedEvent const* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (a_event && a_event->baseObj && a_event->oldContainer == RE::PlayerCharacter::GetSingleton()->GetFormID())
		{
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				ProcessDrunkardQuest(data->drunk_data, a_event->baseObj, a_event->newContainer, RE::MenuTopicManager::GetSingleton()->speaker.get().get());
			};
		};
		return EventResult::kContinue;
	};

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
			CFramework_Master::RadiantCountData.IncreaseCount(a_data->link, 1);
			CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(a_data->link);
		}
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Idens ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetIdenArrayByID(RE::StaticFunctionTag*, std::string a_page)
	{
		std::vector<std::string> list{};

		if (auto a_ID = CHCMHandler::MCMAPI::GetMCMPageIdentifierFromName(nullptr, a_page); a_ID != -1)
		{
			for (auto& [data, name, ID, key] : CQuestDataVec) 
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data->editor_id);
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Names ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetNameArrayByID(RE::StaticFunctionTag*, std::string a_page)
	{
		std::vector<std::string> list{};

		if (auto a_ID = CHCMHandler::MCMAPI::GetMCMPageIdentifierFromName(nullptr, a_page); a_ID != -1)
		{
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data->GetName());
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Texts ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetTextArrayByID(RE::StaticFunctionTag*, std::string a_page)
	{
		std::vector<std::string> list{};

		if (auto a_ID = CHCMHandler::MCMAPI::GetMCMPageIdentifierFromName(nullptr, a_page); a_ID != -1)
		{
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data->GetHighlight());
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Radiant Texts ) -----
	//---------------------------------------------------

	std::string QuestAPI::qGetTimesCompletedVsTimesRequiredText(RE::StaticFunctionTag*, std::string a_key)
	{
		for (auto& [data, name, ID, key] : CQuestDataVec) {
			if (DKUtil::string::iequals(a_key, key))
			{
				return data->HasRadiantData() ? fmt::format("Completed {} / {} Times.", std::to_string(CFramework_Master::RadiantCountData.GetCount(key)), std::to_string(data->GetRadiantTimesRequired())) : std::string{};
			}
		}
		return std::string{};
	};

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Texts ) -------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetKeysArrayByID(RE::StaticFunctionTag*, std::string a_page)
	{
		std::vector<std::string> list{};

		if (auto a_ID = CHCMHandler::MCMAPI::GetMCMPageIdentifierFromName(nullptr, a_page); a_ID != -1)
		{
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data->GetKey());
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Radis ) -------------
	//---------------------------------------------------

	std::vector<int32_t> QuestAPI::qGetRadiArrayByID(RE::StaticFunctionTag*, std::string a_page)
	{
		std::vector<int32_t> list{};

		if (auto a_ID = CHCMHandler::MCMAPI::GetMCMPageIdentifierFromName(nullptr, a_page); a_ID != -1)
		{
			for (auto& [data, name, ID, key] : CQuestDataVec)
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data->GetType());
				}
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Radis ) -------------
	//---------------------------------------------------

	std::vector<int32_t> QuestAPI::qGetMiscQuestRadiArrayByID(RE::StaticFunctionTag*, int32_t a_patchID)
	{
		std::vector<int32_t> list{};

		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (ID == a_patchID && data->GetQuest()) {
				list.push_back(data->GetType());
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Keys ) --------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetMiscQuestKeysArrayByID(RE::StaticFunctionTag*, int32_t a_patchID)
	{
		std::vector<std::string> list{};

		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (ID == a_patchID && data->GetQuest()) {
				list.push_back(data->GetKey());
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Keys ) --------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetMiscQuestNameArrayByID(RE::StaticFunctionTag*, int32_t a_patchID)
	{
		std::vector<std::string> list{};

		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (ID == a_patchID && data->GetQuest()) {
				list.push_back(data->GetName());
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Keys ) --------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetMiscQuestTextArrayByID(RE::StaticFunctionTag*, int32_t a_patchID)
	{
		std::vector<std::string> list{};

		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (ID == a_patchID && data->GetQuest()) {
				list.push_back(data->GetHighlight());
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Keys ) --------------
	//---------------------------------------------------

	std::vector<std::string> QuestAPI::qGetMiscQuestIdenArrayByID(RE::StaticFunctionTag*, int32_t a_patchID)
	{
		std::vector<std::string> list{};

		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (ID == a_patchID && data->GetQuest()) {
				list.push_back(data->GetEditorID());
			}
		}
		return list;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Totals ) ------------
	//---------------------------------------------------

	std::pair<int32_t, int32_t>	QuestAPI::qGetQuestCompletionTotals(std::vector<CQuestData*> data) {

		std::pair<int32_t, int32_t> totals{};

		for (auto& q : data)
		{
			totals.first += IsQuestCompleted(q) ? 1 : 0;
			totals.second += q->GetQuest() ? 1 : 0;
		};

		return totals;
	};

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	bool QuestAPI::IsQuestCompleted(CQuestData* a_data)
	{
		auto TimesCompleted = CFramework_Master::RadiantCountData.GetCount(a_data->GetKey());
		auto is_complete = false;

		switch (a_data->completion_type)
		{
		case CCompEnum::kStand: { is_complete = TimesCompleted > 0 || IsQuestCompletedNaturally(a_data); break; }
		case CCompEnum::kThane: { is_complete = TimesCompleted > 0 || IsThaneOfHold(a_data); break; }
		case CCompEnum::kGlobl: { is_complete = TimesCompleted >= a_data->GetRadiantTimesRequired(); break; }
		case CCompEnum::kFavor: { is_complete = TimesCompleted > 0 || IsFavorCompleted(a_data); break; }
		case CCompEnum::kStage: { is_complete = TimesCompleted > 0 || IsStageDoneOrPast(a_data); break; }
		case CCompEnum::kCiWar: { is_complete = TimesCompleted > 0; break; }
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
	//-- Quest Functions ( Completion Checks ) ----------
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

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	bool QuestAPI::IsStageDone(RE::TESQuest* a_quest, int32_t a_stage)
	{
		using func_t = decltype(IsStageDone);
		REL::Relocation<func_t> func{ IsStageDoneAddress };
		return a_quest && func(a_quest, a_stage);
	}

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	bool QuestAPI::IsQuestCompletedNaturally(CQuestData* a_data)
	{
		return a_data->GetQuest() && a_data->GetQuest()->data.flags.any(RE::QuestFlag::kCompleted, RE::QuestFlag::kFailed);
	};

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	bool QuestAPI::IsFavorCompleted(CQuestData* a_data)
	{
		return a_data->HasFavorData() && a_data->GetActor() && !a_data->GetActor()->IsInFaction(a_data->GetActorFaction());
	};

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	bool QuestAPI::IsThaneOfHold(CQuestData* a_data)
	{
		return a_data->HasThaneData() && (IsStageDone(a_data->GetQuest(), a_data->thane_data->stage) || a_data->IsThane());
	};

	//---------------------------------------------------
	//-- Quest Functions ( Completion Checks ) ----------
	//---------------------------------------------------

	void QuestAPI::CheckForRadiantQuestCompletion(std::string a_key)
	{
		if (!CFramework_Master::CQuestKeys_Natural.HasKey(a_key))
		{
			for (auto& [data, name, ID, key] : CQuestDataVec) {
				if (DKUtil::string::iequals(a_key, key) && data->HasRadiantData() && CFramework_Master::RadiantCountData.GetCount(key) >= data->GetRadiantTimesRequired())
				{
					data->is_completed = true;
					CFramework_Master::CQuestKeys_Natural.AddKey(key);
					CFramework_Master::CQuestKeys_Manual.RemoveKey(key);
					CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kQuestComplete, data->GetName());
					return;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	bool QuestAPI::qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key)
	{
		return CFramework_Master::CQuestKeys_Manual.HasKey(a_key);
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	std::int32_t QuestAPI::qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key)
	{
		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (DKUtil::string::iequals(a_key, key))
			{
				if (CFramework_Master::CQuestKeys_Natural.HasKey(key))
				{
					if (data->completion_type != CCompEnum::kGlobl) {
						return 2;
					}
					else
					{
						//Fix for when changing radiant requirements at run-time.
						return data->GetRadiantTimesRequired() <= CFramework_Master::RadiantCountData.GetCount(key) ? 2 : 0;
					}
				}

				if (CFramework_Master::CQuestKeys_Manual.HasKey(key))
				{
					return 1;
				}

				return 0;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Setter - Status ) --------
	//---------------------------------------------------

	void QuestAPI::qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key, bool b_completed)
	{
		for (auto& [data, name, ID, key] : CQuestDataVec)
		{
			if (DKUtil::string::iequals(a_key, key))
			{
				data->Switch();

				if (data->IsCompleted()) {
					CFramework_Master::CQuestKeys_Manual.AddKey(key);
				}
				else {
					CFramework_Master::CQuestKeys_Manual.RemoveKey(key);
				}
				return;
			}
		}
	}
};