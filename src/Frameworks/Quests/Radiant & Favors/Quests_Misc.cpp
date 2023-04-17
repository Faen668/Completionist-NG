#include "PCH.h"
#include "Radiant Quests Manager.hpp"
#include "Quests_Misc.hpp"

using namespace Quest_Manager;

namespace MiscQuests
{
	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			auto questID = std::string(quest->GetFormEditorID());

			if ((questID == "CWSiegeObj" || questID == "CWFortSiegeFort") && a_event->stage == 9000) {
				RadiantQuestManager::ProcessFormID(questID);
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}
}

