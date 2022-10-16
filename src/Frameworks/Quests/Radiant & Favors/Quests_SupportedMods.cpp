#include "PCH.h"
#include "Quests_SupportedMods.hpp"

namespace LOTD
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "DBM_RadiantFindersKeepers" && a_event->stage == 30) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor_FindersKeepers")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DBM_RadiantResearch" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor_ResearchProject")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DBM_RadiantRuinedBooks" && a_event->stage == 30) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor_OneMansJunk")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "DBM_SextantHandler" && a_event->stage == 6) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_Favor_RelicHunt")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Vigilant
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "zzzAomBountyBook" && a_event->stage == 20) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantBook")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAomBountyChickTrader" && a_event->stage == 20) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantChick")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAomBountyRelic" && a_event->stage == 20) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantRelic")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMBountyRelicHorn" && a_event->stage == 20) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantHorn")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMBountyPiper" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantPiper")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAomBountySummoner" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantSummoner")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAomBountyVampire" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantVampire")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAomBountyWitch" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_BountyVigilantWitch")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMRadDead" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_RadianceVigilantDead")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMRadDoppler" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_RadianceVigilantDopple")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMRadMolag" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_RadianceVigilantSpawn")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMRadVampire" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_RadianceVigilantVampire")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "zzzAoMRadWitchHunter" && a_event->stage == 10) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_RadianceVigilantWitchHunter")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
