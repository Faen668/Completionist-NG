#include "Quests_Beggars.hpp"
#include "Serialization.hpp"

using namespace Serialization;

std::unordered_set<RE::Actor*> refs;
RE::Actor* curspeaker;

namespace BeggarQuests
{
	constexpr std::tuple<RE::FormID, const char*, const char*> Begggars[] = {
	{0x01B138, "Completionist_FavorBeggarAngrenor",		"Skyrim.esm" },
	{0x02C90F, "Completionist_FavorBeggarBrenuin",		"Skyrim.esm" },
	{0x01F325, "Completionist_FavorBeggarDegaine",		"Skyrim.esm" },
	{0x0198DC, "Completionist_FavorBeggarDervenin",		"Skyrim.esm" },
	{0x044A8D, "Completionist_FavorBeggarEdda",			"Skyrim.esm" },
	{0x003F5E, "Completionist_FavorBeggarLucia",		"Hearthfires.esm" },
	{0x019E24, "Completionist_FavorBeggarNarfi",		"Skyrim.esm" },
	{0x0198BD, "Completionist_FavorBeggarNoster",		"Skyrim.esm" },
	{0x01B122, "Completionist_FavorBeggarSilda",		"Skyrim.esm" },
	{0x044A8E, "Completionist_FavorBeggarSnilf",		"Skyrim.esm" },
	{0x01A636, "Completionist_FavorBeggarSvari",		"Skyrim.esm" },
	};

	//---------------------------------------------------
	//-- Framework Events ( Install ) -------------------
	//---------------------------------------------------

	void CHandler::Install() {

		for (auto& [formID, global, fileName] : Begggars) {

			auto* Beggar = CompletionistData::GetFullForm<RE::Actor>(formID, fileName);
			if (Beggar && Beggar->GetFormID() != 0) {
				refs.emplace(Beggar);
			}
		}

	}

	//---------------------------------------------------
	//-- Framework Events ( On Activate ) ---------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) {
			return EventResult::kContinue;
		}

		if (refs.contains(a_event->objectActivated.get()->As<RE::Actor>())) {
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
				if (Beggar->GetFormID() == curspeaker->GetFormID()) {
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
