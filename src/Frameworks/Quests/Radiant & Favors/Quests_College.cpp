#include "PCH.h"
#include "Quests_College.hpp"

/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
constexpr std::tuple<RE::FormID, RE::FormID, std::int32_t, std::int32_t, const char*> Radiant_QuestData[] = {
	/*00*/ {0x0266F1, 0x000000, 1, 200, "Completionist_CollegeMGR01"},
	/*01*/ {0x0266F2, 0x000000, 1, 200, "Completionist_CollegeMGR02"},
	/*02*/ {0x028A13, 0x000000, 1, 200, "Completionist_CollegeMGR10"},
	/*03*/ {0x028A19, 0x000000, 1, 200, "Completionist_CollegeMGR11"},
	/*04*/ {0x028A1E, 0x000000, 1, 200, "Completionist_CollegeMGR12"},
	/*05*/ {0x028A24, 0x0BC096, 1, 200, "Completionist_CollegeMGR20"},
	/*06*/ {0x0E0E04, 0x000000, 1, 200, "Completionist_CollegeMGR21"},
	/*07*/ {0x0C1E72, 0x000000, 1, 200, "Completionist_CollegeMGR30"},
	/*08*/ {0x05A609, 0x000000, 1, 200, "Completionist_CollegeMGRRouge"},
	/*09*/ {0x05D2EA, 0x000000, 1, 200, "Completionist_CollegeFreeformWinterholdCollegeA"},
};

namespace CollegeQuests
{
	EventResult ScriptEventHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		for (auto& [aformID, bformID, value, stage, global] : Radiant_QuestData) {
			const auto* aquest = RE::TESForm::LookupByID<RE::TESQuest>(aformID);
			const auto* bquest = RE::TESForm::LookupByID<RE::TESQuest>(bformID);

			if (((aquest && aquest == event) || (bquest && bquest == event)) && a_event->stage == stage) {
				if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)) {
					var->value += value;
					INFO("Increasing Var For {}", aquest ? aquest->GetName() : bquest ? bquest->GetName() : "");
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
	
	
	
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "MGR01" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR02" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR10" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR11" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR12" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if ((questID == "MGR20" || questID == "MGR20B") && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR21" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGR30" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "MGRRogue" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}

			if (questID == "FreeformWinterholdCollegeA" && a_event->stage == 200) {
				if (auto global = RE::TESForm::LookupByEditorID<RE::TESGlobal>("")) {
					global->value += 1;
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
