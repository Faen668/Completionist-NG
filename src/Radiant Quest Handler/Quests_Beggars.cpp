#include "Quests_Beggars.hpp"

namespace BeggarQuests
{
	const RE::FormID Angrenor =	0x01B138;
	const RE::FormID Brenuin =	0x02C90F; 
	const RE::FormID Degaine =	0x01F325; 
	const RE::FormID Dervenin =	0x0198DC; 
	const RE::FormID Edda =		0x044A8D; 
	const RE::FormID Lucia =	0x003F5F;
	const RE::FormID Narfi =	0x019E24; 
	const RE::FormID Noster =	0x0198BD; 
	const RE::FormID Silda =	0x01B122; 
	const RE::FormID Snilf =	0x044A8E; 
	const RE::FormID Svari =	0x01A636; 

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
		
		if (!a_event || !a_event->baseObj || a_event->baseObj != 0x00000F) { return EventResult::kContinue; }
		
		auto speaker = RE::MenuTopicManager::GetSingleton()->speaker.get();
		if ( !speaker) { return EventResult::kContinue; }

		switch (speaker->GetFormID()) {

		case Angrenor: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarAngrenor"); GV && a_event->newContainer == Angrenor) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Brenuin: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarBrenuin"); GV && a_event->newContainer == Brenuin) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Degaine: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarDegaine"); GV && a_event->newContainer == Degaine) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Dervenin: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarDervenin"); GV && a_event->newContainer == Dervenin) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Edda: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarEdda"); GV && a_event->newContainer == Edda) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Lucia: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarLucia"); GV && a_event->newContainer == Lucia) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Narfi: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarNarfi"); GV && a_event->newContainer == Narfi) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Noster: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarNoster"); GV && a_event->newContainer == Noster) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Silda: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarSilda"); GV && a_event->newContainer == Silda) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Snilf: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarSnilf"); GV && a_event->newContainer == Snilf) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		case Svari: {
			if (auto GV = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorBeggarSvari"); GV && a_event->newContainer == Svari) {
				GV->value += 1;
			}
			return EventResult::kContinue;
		}

		default:
			return EventResult::kContinue;
		}
	}
}
