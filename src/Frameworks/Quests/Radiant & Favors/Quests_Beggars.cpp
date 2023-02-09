#include "Quests_Beggars.hpp"
#include "Serialization.hpp"

using namespace Serialization;

RE::Actor* curspeaker;

namespace BeggarQuests
{
	constexpr std::tuple<RE::FormID, const char*, const char*> Begggars[] = {
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
	//-- Framework Events ( Install ) -------------------
	//---------------------------------------------------

	bool CHandler::IsReferenceAValidBeggar(RE::Actor* a_actor) {

		for (auto& [formID, global, fileName] : Begggars) {

			auto* Beggar = CompletionistData::GetFullForm<RE::Actor>(formID, fileName);
			if (Beggar && Beggar->GetFormID() == a_actor->GetFormID()) {
				return true;
			}
		}
		return false;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Activate ) ---------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) {
			return EventResult::kContinue;
		}

		if (CHandler::IsReferenceAValidBeggar(a_event->objectActivated.get()->As<RE::Actor>())) {
			curspeaker = a_event->objectActivated.get()->As<RE::Actor>();
			INFO("{} is a valid beggar", curspeaker->GetName());
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
		
		if (!a_event || !a_event->baseObj || a_event->baseObj != RE::TESForm::LookupByID(0x00000F)->GetFormID() || !curspeaker) { 
			return EventResult::kContinue; 
		}

		if (a_event->newContainer == curspeaker->GetFormID()) {
			INFO("{} is the new container", curspeaker->GetName());

			for (auto& [formID, global, fileName] : Begggars) {

				auto* Beggar = CompletionistData::GetFullForm<RE::Actor>(formID, fileName);
				if (Beggar && Beggar->GetFormID() == curspeaker->GetFormID()) {
					INFO("Updating Global Variable For {}", curspeaker->GetName());
					RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)->value++;
					curspeaker = nullptr;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
