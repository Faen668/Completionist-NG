#include "Serialization.hpp"
#include "Quests_Bounty.hpp"

constexpr std::tuple<RE::FormID, int32_t, const char*> Quests[] = {

	{0x095125, 200, "Completionist_BountyBQ01"}, //BQ01 
	{0x0BD77F, 200, "Completionist_BountyBQ02"}, //BQ02
	{0x0BD78C, 200, "Completionist_BountyBQ03"}, //BQ03
	{0x0CBA9A, 200, "Completionist_BountyBQ04"}, //BQ04
};

constexpr RE::FormID BQ01_FQS[] = {

	{0x000E42}, //BQ01_Eastmarch
	{0x000E36}, //BQ01Falkreath
	{0x000E1E}, //BQ01Haafingar
	{0x000E24}, //BQ01Hjaalmarch
	{0x000E30}, //BQ01Pale
	{0x000E4E}, //BQ01Reach
	{0x000E48}, //BQ01Rift
	{0x000E2A}, //BQ01Whiterun
	{0x000E3C}, //BQ01Winterhold
};

constexpr RE::FormID BQ02_FQS[] = {

	{0x000E90}, //BQ02Eastmarch
	{0x000E86}, //BQ02Falkreath
	{0x000E72}, //BQ02Haafingar
	{0x000E77}, //BQ02Hjaalmarch
	{0x000E81}, //BQ02Pale
	{0x000E9A}, //BQ02Reach
	{0x000E95}, //BQ02Rift
	{0x000E7C}, //BQ02Whiterun
	{0x000E8B}, //BQ02Winterhold
};

constexpr RE::FormID BQ03_FQS[] = {

	{0x000ED3}, //BQ03Eastmarch
	{0x000EC9}, //BQ03Falkreath
	{0x000EE2}, //BQ03Haafingar
	{0x000EBA}, //BQ03Hjaalmarch
	{0x000ECA}, //BQ03Pale
	{0x000EDD}, //BQ03Reach
	{0x000ED8}, //BQ03Rift
	{0x000EBF}, //BQ03Whiterun
	{0x000ECE}, //BQ03Winterhold
};

constexpr RE::FormID BQ04_FQS[] = {

	{0x000F20}, //BQ04Eastmarch
	{0x000F16}, //BQ04Falkreath
	{0x000F02}, //BQ04Haafingar
	{0x000F07}, //BQ04Hjaalmarch
	{0x000F11}, //BQ04Pale
	{0x000F2A}, //BQ04Reach
	{0x000F25}, //BQ04Rift
	{0x000F0C}, //BQ04Whiterun
	{0x000F1B}, //BQ04Winterhold
};

constexpr std::string_view FQS_Name = "Favor Quests Seperated.esp";

namespace BountyQuests
{
	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
		
		if (!a_event) { return EventResult::kContinue; }

		for (auto& [formID, stage, global] : Quests) {

			const auto* aquest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
			const auto* bquest = RE::TESForm::LookupByID<RE::TESQuest>(formID);

			auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>(global);
			if (!aquest || !bquest || !var || a_event->stage != stage) { return EventResult::kContinue; }

			if (IsValidQuest(aquest->GetFormID(), bquest->GetFormID())) {
				var->value += 1;
				return EventResult::kContinue;
			}

		}
		return EventResult::kContinue;
	}

	bool CHandler::IsValidQuest(RE::FormID eventID, RE::FormID checkID) {

		if (!Serialization::CompletionistData::IsModInstalled(FQS_Name)) { return eventID == checkID; }

		const auto* BQ01 = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(0x095125, "Skyrim.esm");
		const auto* BQ02 = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(0x0BD77F, "Skyrim.esm");
		const auto* BQ03 = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(0x0BD78C, "Skyrim.esm");
		const auto* BQ04 = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(0x0CBA9A, "Skyrim.esm");
		if (!BQ01 || !BQ02 || !BQ03 || !BQ04) { return false; }
		
		if (checkID == BQ01->GetFormID()) {
			for (auto& formID : BQ01_FQS) {
				const auto* cquest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(formID, FQS_Name);
				if (cquest && cquest->GetFormID() == eventID) {
					return true;
				}
			}
			return false;
		}

		if (checkID == BQ02->GetFormID()) {
			for (auto& formID : BQ02_FQS) {
				const auto* cquest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(formID, FQS_Name);
				if (cquest && cquest->GetFormID() == eventID) {
					return true;
				}
			}
			return false;
		}

		if (checkID == BQ03->GetFormID()) {
			for (auto& formID : BQ03_FQS) {
				const auto* cquest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(formID, FQS_Name);
				if (cquest && cquest->GetFormID() == eventID) {
					return true;
				}
			}
			return false;
		}

		if (checkID == BQ04->GetFormID()) {
			for (auto& formID : BQ04_FQS) {
				const auto* cquest = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(formID, FQS_Name);
				if (cquest && cquest->GetFormID() == eventID) {
					return true;
				}
			}
			return false;
		}
		return false;
	}
}
