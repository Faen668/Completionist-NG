#include "PCH.h"
#include "Radiant Quests Manager.hpp"
#include "Quests_Companions.hpp"

using namespace Quest_Manager;

namespace CompanionsQuests
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "CR01" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR01")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR02" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR02")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR03" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR03")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR04" && a_event->stage == 100) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR04")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR05" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR05")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
			else if (questID == "CR06" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR06")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR07" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR07")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR08" && a_event->stage == 100) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR08")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR09" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR09")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR10" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR10")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR11" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR11")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR12" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR12")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "CR13" && a_event->stage == 200) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "CR13");
								return EventResult::kContinue;
							}
						}
					}
				}
			}

			if (questID == "CR14" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Companions_CR14")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
