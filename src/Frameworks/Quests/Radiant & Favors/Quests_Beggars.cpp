#include "Quests_Beggars.hpp"

namespace BeggarQuests
{
	constexpr std::tuple<RE::FormID, const char*, const char*> Begggars[] = {
	{0x01B138, "Completionist_FavorBeggarAngrenor",		"Skyrim.esm"},
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

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
		
		if (!a_event || !a_event->baseObj || a_event->baseObj != 0x00000F) { return EventResult::kContinue; }
		
		auto speaker = RE::MenuTopicManager::GetSingleton()->speaker.get();
		if ( !speaker) { return EventResult::kContinue; }

		for (auto& [FormID, Global, FileName] : Begggars) {

			auto* Beggar = RE::TESDataHandler::GetSingleton()->LookupForm(FormID, FileName);
			if (!Beggar) { return EventResult::kContinue; }

			//INFO("Attemping To Match Speaker FormID [{}] To [{}]", speaker->GetFormID(), Beggar->GetFormID());

			if (speaker->GetFormID() == Beggar->GetFormID() && Beggar->GetFormID() == a_event->newContainer) {
				RE::TESForm::LookupByEditorID<RE::TESGlobal>(Global)->value += 1;
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}
}
