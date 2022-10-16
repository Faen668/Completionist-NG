#include "PCH.h"
#include "Quests_ThievesGuild.hpp"

namespace ThievesGuildQuests
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "TGRNT" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Bedlam")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRBE" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Burglary")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRGF" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Fishing")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRSL" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Heist")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRFO" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Numbers")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRDU" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Shill")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "TGRHC" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_TGR_Sweep")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
