#include "Quests_Beggars.hpp"
#include "Serialization.hpp"

using namespace Serialization;

#define var_name(var_name)  #var_name

struct activatedBeggar 
{
	RE::Actor* Actor;
	RE::TESGlobal* Global;
	const char* GlobalName;
};

activatedBeggar curspeaker = {};

namespace BeggarQuests
{
	constexpr std::tuple<RE::FormID, const char*, const char*> Begggars[] = 
	{
		{0x1B138, "Completionist_FavorBeggarAngrenor",	"Skyrim.esm" },
		{0x2C90F, "Completionist_FavorBeggarBrenuin",	"Skyrim.esm" },
		{0x1F325, "Completionist_FavorBeggarDegaine",	"Skyrim.esm" },
		{0x198DC, "Completionist_FavorBeggarDervenin",	"Skyrim.esm" },
		{0x44A8D, "Completionist_FavorBeggarEdda",		"Skyrim.esm" },
		{0x03F5E, "Completionist_FavorBeggarLucia",		"Hearthfires.esm" },
		{0x19E24, "Completionist_FavorBeggarNarfi",		"Skyrim.esm" },
		{0x198BD, "Completionist_FavorBeggarNoster",	"Skyrim.esm" },
		{0x1B122, "Completionist_FavorBeggarSilda",		"Skyrim.esm" },
		{0x44A8E, "Completionist_FavorBeggarSnilf",		"Skyrim.esm" },
		{0x1A636, "Completionist_FavorBeggarSvari",		"Skyrim.esm" },
	};

	//---------------------------------------------------
	//-- Framework Function ( Is Valid Reference  ) -----
	//---------------------------------------------------

	void CHandler::EvaluateActivatedReference(RE::Actor* a_actor) 
	{
		if (a_actor)
		{
			for (auto& [formID, global, fileName] : Begggars)
			{
				auto* Beggar = CompletionistData::GetFullForm<RE::Actor>(formID, fileName);

				if (Beggar && Beggar->GetFormID() == a_actor->GetFormID())
				{
					curspeaker = activatedBeggar{ Beggar, RE::TESForm::LookupByEditorID<RE::TESGlobal>(global), global };
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

		if (curspeaker.Actor && curspeaker.Global && curspeaker.GlobalName)
		{
			INFO("{} is a valid beggar with paired variable [{}]", curspeaker.Actor->GetName(), curspeaker.GlobalName);
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

		if (curspeaker.Actor && curspeaker.Global && curspeaker.GlobalName)
		{
			if (a_event->newContainer == curspeaker.Actor->GetFormID())
			{
				curspeaker.Global->value++;
				INFO("Updated [{}] to a value of {} for {} ", curspeaker.GlobalName, curspeaker.Global->value, curspeaker.Actor->GetName());
			}
			curspeaker = {};
		}

		return EventResult::kContinue;
	}
}
