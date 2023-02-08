#include "Quests_Bounty.hpp"

using namespace Serialization;

constexpr const char* SK_NAME = "Skyrim.esm";
constexpr const char* FS_NAME = "Favor Quests Seperated.esp";

constexpr BountyQuestData BountyQuests[] = {
	{ 0x095125, SK_NAME, { 0x000E42, 0x000E36, 0x000E1E, 0x000E24, 0x000E30, 0x000E4E, 0x000E48, 0x000E2A, 0x000E3C }, FS_NAME, "Completionist_BountyBQ01"},
	{ 0x0BD77F, SK_NAME, { 0x000E90, 0x000E86, 0x000E72, 0x000E77, 0x000E81, 0x000E9A, 0x000E95, 0x000E7C, 0x000E8B }, FS_NAME, "Completionist_BountyBQ02"},
	{ 0x0BD78C, SK_NAME, { 0x000ED3, 0x000EC9, 0x000EE2, 0x000EBA, 0x000ECA, 0x000EDD, 0x000ED8, 0x000EBF, 0x000ECE }, FS_NAME, "Completionist_BountyBQ03"},
	{ 0x0CBA9A, SK_NAME, { 0x000F20, 0x000F16, 0x000F02, 0x000F07, 0x000F11, 0x000F2A, 0x000F25, 0x000F0C, 0x000F1B }, FS_NAME, "Completionist_BountyBQ04"},
};

namespace BountyProcessor
{
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
		INFO("Registered Bounty Quest Handler");
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage || a_event->stage != 200) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		ProcessQuest(event->GetFormID(), a_event->stage);
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void CHandler::ProcessQuest(const RE::FormID a_formID, const uint16_t a_stage) {

		for (auto& dataSet : BountyQuests) {

			auto* quest = CompletionistData::GetFullForm<RE::TESQuest>(dataSet.VBQ_ID, dataSet.VBQ_FN);

			if (quest && quest->GetFormID() != a_formID) {
				for (auto& FQS : dataSet.FQS_ID) {
					if (quest = CompletionistData::GetFullForm<RE::TESQuest>(FQS, dataSet.FQS_FN); quest && quest->GetFormID() == a_formID) {
						break;
					}
				}
			}

			if (quest && quest->GetFormID() == a_formID) {
				if (auto* global = RE::TESForm::LookupByEditorID<RE::TESGlobal>(dataSet.Global)) {
					INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}]", dataSet.Global, quest->GetName());
					global->value++;
				};
			}
		}
	}
}