#include "Serialization.hpp"
#include "CQuests_Beggars.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

constexpr std::tuple<RE::FormID, const char*, const char*> Begggars[] =
{
	{0x1B138, "Windhelm_Quest18",	"Skyrim.esm" },
	{0x2C90F, "Whiterun_Quest14",	"Skyrim.esm" },
	{0x1F325, "Markarth_Quest21",	"Skyrim.esm" },
	{0x198DC, "Solitude_Quest18",	"Skyrim.esm" },
	{0x44A8D, "Riften_Quest27",		"Skyrim.esm" },
	{0x03F5E, "Whiterun_Quest15",	"Hearthfires.esm" },
	{0x19E24, "SmallTowns_Quest18",	"Skyrim.esm" },
	{0x198BD, "Solitude_Quest19",	"Skyrim.esm" },
	{0x1B122, "Windhelm_Quest19",	"Skyrim.esm" },
	{0x44A8E, "Riften_Quest28",		"Skyrim.esm" },
	{0x1A636, "Solitude_Quest20",	"Skyrim.esm" },
};

struct activatedBeggar 
{
	RE::Actor* Actor;
	const char* quest_key;
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
	//-- Framework Function ( Is Valid Reference  ) -----
	//---------------------------------------------------

	void CHandler::EvaluateActivatedReference(RE::Actor* a_actor) 
	{
		if (a_actor)
		{
			for (auto& [formID, quest_key, fileName] : Begggars)
			{
				auto* Beggar = CompletionistData::GetFullForm<RE::Actor>(formID, fileName);

				if (Beggar && Beggar->GetFormID() == a_actor->GetFormID())
				{
					curspeaker = activatedBeggar{ Beggar, quest_key };
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

		if (curspeaker.Actor && curspeaker.quest_key)
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

		if (curspeaker.Actor && curspeaker.quest_key)
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
