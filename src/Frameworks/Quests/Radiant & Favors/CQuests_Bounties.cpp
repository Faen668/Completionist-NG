#include "Serialization.hpp"
#include "CQuests_Bounties.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

constexpr const char* SK_NAME = "Skyrim.esm";
constexpr const char* FS_NAME = "Favor Quests Seperated.esp";

constexpr BountyQuestData BountyQuests[] = {
	{ 0x095125, SK_NAME, { 0x0D7E42, 0x0D7E36, 0x0D7E1E, 0x0D7E24, 0x0D7E30, 0x0D7E4E, 0x0D7E48, 0x0D7E2A, 0x0D7E3C }, FS_NAME, "Radi_SK_Quest01"},
	{ 0x0BD77F, SK_NAME, { 0x0D7E90, 0x0D7E86, 0x0D7E72, 0x0D7E77, 0x0D7E81, 0x0D7E9A, 0x0D7E95, 0x0D7E7C, 0x0D7E8B }, FS_NAME, "Radi_SK_Quest04"},
	{ 0x0BD78C, SK_NAME, { 0x0D7ED3, 0x0D7EC9, 0x0D7EE2, 0x0D7EBA, 0x0D7ECA, 0x0D7EDD, 0x0D7ED8, 0x0D7EBF, 0x0D7ECE }, FS_NAME, "Radi_SK_Quest03"},
	{ 0x0CBA9A, SK_NAME, { 0x0D7F20, 0x0D7F16, 0x0D7F02, 0x0D7F07, 0x0D7F11, 0x0D7F2A, 0x0D7F25, 0x0D7F0C, 0x0D7F1B }, FS_NAME, "Radi_SK_Quest02"},
};

namespace CQFramework_Bounties
{
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
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

			if (quest && quest->GetFormID() == a_formID)
			{
				CFramework_Master::RadiantCountData.IncreaseCount(dataSet.Quest_Key, 1);
				CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(dataSet.Quest_Key);
			};
		}
	}
}