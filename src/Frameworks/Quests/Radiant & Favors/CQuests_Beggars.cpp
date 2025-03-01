#include "Serialization.hpp"
#include "CQuests_Beggars.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

std::vector<FavorMerchantData> Beggar_Data =
{
	{0x013BA7, "Skyrim.esm",		"Whiterun_Quest14"},
	{0x013392, "Skyrim.esm",		"Markarth_Quest21"},
	{0x01327C, "Skyrim.esm",		"Solitude_Quest18"},
	{0x013356, "Skyrim.esm",		"Riften_Quest27"},
	{0x003F5F, "Hearthfires.esm",	"Whiterun_Quest15"},
	{0x0136C0, "Skyrim.esm",		"SmallTowns_Quest18"},
	{0x0132A9, "Skyrim.esm",		"Solitude_Quest20"},
	{0x01329C, "Skyrim.esm",		"Solitude_Quest19"},
	{0x014121, "Skyrim.esm",		"Windhelm_Quest19"},
	{0x01B071, "Skyrim.esm",		"Riften_Quest28"},
	{0x014137, "Skyrim.esm",		"Windhelm_Quest18"},
};

struct activatedBeggar 
{
	RE::Actor* Actor;
	std::string quest_key;
};

activatedBeggar curspeaker = {};

namespace CQFramework_Beggars
{
	//---------------------------------------------------
	//-- Framework Functions ( Register Events ) --------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(CHandler::GetSingleton()));
	};

	//---------------------------------------------------
	//-- Add beggar Data --------------------------------
	//---------------------------------------------------

	void CHandler::AddBeggarData(RE::FormID NPCReferenceFormID, const std::string& fileName, const std::string& Quest_Key)
	{
		FavorMerchantData data = { NPCReferenceFormID, fileName, Quest_Key };
		Beggar_Data.push_back(data);
		//INFO("Added New Merchant Data: {} - {} - {}", Serialization::CompletionistData::GetFormIDHexString(NPCBaseFormID), fileName, Quest_Key)
	}

	//---------------------------------------------------
	//-- Framework Function ( Is Valid Reference  ) -----
	//---------------------------------------------------

	void CHandler::EvaluateActivatedReference(RE::Actor* a_actor) 
	{
		if (a_actor)
		{
			for (auto& [formID, fileName, quest_key] : Beggar_Data)
			{
				auto* npc = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESNPC>(formID, fileName);
				if (npc && npc->GetFormID() == a_actor->GetActorBase()->GetFormID())
				{
					curspeaker = activatedBeggar{ a_actor, quest_key };
					break;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Framework Events ( On Activate ) ---------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) 
	{
		if (!a_event || !a_event->objectActivated || a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) 
		{
			return EventResult::kContinue;
		}

		CHandler::EvaluateActivatedReference(a_event->objectActivated.get()->As<RE::Actor>());

		if (curspeaker.Actor && !curspeaker.quest_key.empty())
		{
			INFO("{} is a valid beggar with paired quest key [{}]", curspeaker.Actor->GetName(), curspeaker.quest_key);
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added To Beggar ) ---
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) 
	{
		if (!a_event || !a_event->baseObj || !a_event->newContainer || a_event->baseObj != RE::TESForm::LookupByID(0x00000F)->GetFormID()) 
		{ 
			return EventResult::kContinue; 
		}

		if (curspeaker.Actor && !curspeaker.quest_key.empty())
		{
			if (a_event->newContainer == curspeaker.Actor->GetFormID())
			{
				CFramework_Master::RadiantCountData.IncreaseCount(curspeaker.quest_key, 1);
				CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(curspeaker.quest_key);
			}
			curspeaker = {};
		}
		return EventResult::kContinue;
	}
}
