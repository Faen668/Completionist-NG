#include "Quests_Beggars.hpp"

RE::TESObjectREFR* curspeaker;

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
	//-- Framework Events ( On Activate ) ---------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {

		if (!a_event || !a_event->objectActivated || a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) { return EventResult::kContinue; }

		for (auto& [formID, global, fileName] : Begggars) {

			auto* Beggar = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(formID, fileName));
			if (!Beggar) { return EventResult::kContinue; }

			if (a_event->objectActivated.get() == Beggar) {
				curspeaker = a_event->objectActivated.get();
				INFO("Set CurSpeaker to {}", curspeaker->GetName());
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
		
		if (!a_event || !a_event->baseObj || a_event->baseObj != 0x00000F || !curspeaker) { return EventResult::kContinue; }
			
		for (auto& [formID, global, fileName] : Begggars) {

			auto* Beggar = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(formID, fileName));
			if (!Beggar) { return EventResult::kContinue; }

			if (Beggar == curspeaker && curspeaker->GetFormID() == a_event->newContainer) {
				INFO("Updating Global Variable For {}", curspeaker->GetName());
				RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)->value += 1;
				curspeaker = nullptr;
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}
}
