#include "PCH.h"
#include "Quests_Favors.hpp"
#include "Radiant Quests Manager.hpp"

using namespace Quest_Manager;

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor001
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {
			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor001") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor001");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor001AngasMill" ||
			questID == "Favor001DarkwaterCrossing" ||
			questID == "Favor001Falkreath" ||
			questID == "Favor001Markarth" ||
			questID == "Favor001Morthal" ||
			questID == "Favor001Windhelm";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor013
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor013") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor013");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor013Kynesgrove" ||
			questID == "Favor013Markarth" ||
			questID == "Favor013Solitude" ||
			questID == "Favor013Whiterun" ||
			questID == "Favor013Windhelm" ||
			questID == "Favor013Winterhold";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor017
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor017" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor017");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor018
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor018") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor018");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor018Falkreath" ||
			questID == "Favor018Windhelm" ||
			questID == "Favor018Winterhold";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor019
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor019" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor019");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor104
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor104") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "QuestGiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor104");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor104DarkwaterCrossing" ||
			questID == "Favor104SkaalVillage" ||
			questID == "Favor104Solitude" ||
			questID == "Favor104Windhelm";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor109
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor109" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor109");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor110
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor110") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor110");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor110Dawnstar" ||
			questID == "Favor110Falkreath" ||
			questID == "Favor110Morthal" ||
			questID == "Favor110Solitude" ||
			questID == "Favor110Whiterun" ||
			questID == "Favor110Windhelm";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor151
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor151" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor151");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor153
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor153" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor153");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor154
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor154" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor154");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor157
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor157" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor157");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor158
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (questID == "Favor158" && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor158");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor204
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor204") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor204");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor204Whiterun" ||
			questID == "Favor204Windhelm";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor205
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event) { return EventResult::kContinue; }

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID)) {

			auto questID = std::string(quest->GetFormEditorID());

			if (IsValidQuest(questID, "Favor205") && a_event->stage == 20) {
				for (const auto& alias : quest->aliases) {
					if (alias && alias->aliasName == "Questgiver") {
						if (auto reference = static_cast<RE::BGSRefAlias*>(alias); reference) {
							if (auto _ID = reference->GetActorReference()->GetActorBase()->GetFormID(); _ID) {
								RadiantQuestManager::ProcessFormID(_ID, "Favor205");
							}
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	bool ScriptEventHandler::IsValidQuest(std::string questID, std::string checkID) {

		if (const auto* modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName("Favor Quests Seperated.esp"); !modInfo) {
			return questID == checkID;
		}

		return
			questID == checkID ||
			questID == "Favor205Dawnstar" ||
			questID == "Favor205Falkreath" ||
			questID == "Favor205Kynesgrove" ||
			questID == "Favor205Solitude" ||
			questID == "Favor205Windhelm";
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
