#include "PCH.h"
#include "Quests_College.hpp"

namespace CollegeQuests
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "MGR01" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR01")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR02" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR02")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR10" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR10")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR11" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR11")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR12" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR12")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if ((questID == "MGR20" || questID == "MGR20B") && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR20")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR21" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR21")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR30" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGR30")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGRRogue" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeMGRRouge")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "FreeformWinterholdCollegeA" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_CollegeFreeformWinterholdCollegeA")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
