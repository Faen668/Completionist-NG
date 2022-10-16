#include "PCH.h"
#include "Quests_Dawnguard.hpp"

namespace DawnguardQuests
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "DLC1RH01" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH01")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH02" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH02")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH03" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH03")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH04" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH04")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH06" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH06")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH07" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH07")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RH08" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RH08")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV05" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV05")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV02" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV02")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV03" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV03")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV07" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV07")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV04" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV04")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DLC1RV01" && a_event->stage == 255) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_DLC1RV01")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

		}
		return EventResult::kContinue;
	}
}
