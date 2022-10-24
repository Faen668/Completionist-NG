#include "PCH.h"
#include "Quests_ThievesGuild.hpp"

namespace ThievesGuildQuests
{

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<RE::FormID, std::int32_t, std::int32_t, const char*> QuestData[] = {
		/*00*/ {0x060990, 1, 200, "Completionist_TGR_Bedlam"},
		/*01*/ {0x02893B, 1, 200, "Completionist_TGR_Burglary"},
		/*02*/ {0x028922, 1, 200, "Completionist_TGR_Fishing"},
		/*03*/ {0x02893E, 1, 200, "Completionist_TGR_Heist"},
		/*04*/ {0x02893B, 1, 200, "Completionist_TGR_Numbers"},
		/*05*/ {0x06098E, 1, 200, "Completionist_TGR_Shill"},
		/*06*/ {0x028936, 1, 200, "Completionist_TGR_Sweep"},

	};

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		for (auto& [formID, value, stage, global] : QuestData) {
			const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(formID);

			if (event == quest && a_event->stage == stage) {
				if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global)) {
					var->value += value;
					INFO("Increasing Var For {}", quest->GetName());
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}
