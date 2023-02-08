#include "Quests_Favors.hpp"

using namespace Serialization;

constexpr const char* SK_NAME = "Skyrim.esm";
constexpr const char* DG_NAME = "Dawnguard.esm";
constexpr const char* DB_NAME = "Dragonborn.esm";
constexpr const char* FS_NAME = "Favor Quests Seperated.esp";

constexpr FavorQuestData Favor_001[] = {
	{ 0x0403B0, SK_NAME, 0x00082D, FS_NAME, 0x01B119, SK_NAME, 20, 1, "Completionist_Favor001Adonato"},
	{ 0x0403B0, SK_NAME, 0x000835, FS_NAME, 0x01A6D2, SK_NAME, 20, 1, "Completionist_Favor001Aeri" },
	{ 0x0403B0, SK_NAME, 0x000838, FS_NAME, 0x09A7A9, SK_NAME, 20, 1, "Completionist_Favor001Banning" },
	{ 0x0403B0, SK_NAME, 0x00083D, FS_NAME, 0x01AA67, SK_NAME, 20, 1, "Completionist_Favor001Idgrod" },
	{ 0x0403B0, SK_NAME, 0x000842, FS_NAME, 0x01B08E, SK_NAME, 20, 1, "Completionist_Favor001Sondas" },
	{ 0x0403B0, SK_NAME, 0x000847, FS_NAME, 0x04E5EA, SK_NAME, 20, 1, "Completionist_Favor001Thadgeir" },
};

constexpr FavorQuestData Favor_013[] = {
	{ 0x0CA209, SK_NAME, 0x000869, FS_NAME, 0x01A675, SK_NAME, 20, 1, "Completionist_Favor013CarlottaValentia"},
	{ 0x0CA209, SK_NAME, 0x000887, FS_NAME, 0x01C18E, SK_NAME, 20, 1, "Completionist_Favor013Haran" },
	{ 0x0CA209, SK_NAME, 0x000896, FS_NAME, 0x01B085, SK_NAME, 20, 1, "Completionist_Favor013Iddra" },
	{ 0x0CA209, SK_NAME, 0x0008A5, FS_NAME, 0x0198B4, SK_NAME, 20, 1, "Completionist_Favor013OctieveSan" },
	{ 0x0CA209, SK_NAME, 0x0008B3, FS_NAME, 0x0198FC, SK_NAME, 20, 1, "Completionist_Favor013Omluag" },
	{ 0x0CA209, SK_NAME, 0x000878, FS_NAME, 0x01B140, SK_NAME, 20, 1, "Completionist_Favor013ScoutsManyMarshes" },
};

constexpr FavorQuestData Favor_017[] = {
	{ 0x0CA210, SK_NAME, 0, "", 0x01AA65, SK_NAME, 20, 1, "Completionist_Favor017Benor"},
	{ 0x0CA210, SK_NAME, 0, "", 0x019928, SK_NAME, 20, 1, "Completionist_Favor017Burguk" },
	{ 0x0CA210, SK_NAME, 0, "", 0x05B683, SK_NAME, 20, 1, "Completionist_Favor017Larak" },
	{ 0x0CA210, SK_NAME, 0, "", 0x01B098, SK_NAME, 20, 1, "Completionist_Favor017Mauhulakh" },
	{ 0x0CA210, SK_NAME, 0, "", 0x0198FA, SK_NAME, 20, 1, "Completionist_Favor017Cosnach" },
	{ 0x0CA210, SK_NAME, 0, "", 0x03EFED, SK_NAME, 20, 1, "Completionist_Favor017Rolff" },
	{ 0x0CA210, SK_NAME, 0, "", 0x091918, SK_NAME, 20, 1, "Completionist_Favor017Uthgerd" },
};

constexpr FavorQuestData Favor_018[] = {
	{ 0x03F46A, SK_NAME, 0x000A0C, FS_NAME, 0x01981B, SK_NAME, 20, 1, "Completionist_Favor018Dengeir"},
	{ 0x03F46A, SK_NAME, 0x000A1D, FS_NAME, 0x01C192, SK_NAME, 20, 1, "Completionist_Favor018Malur" },
	{ 0x03F46A, SK_NAME, 0x000A14, FS_NAME, 0x01B142, SK_NAME, 20, 1, "Completionist_Favor018StandInShallows" },
};

constexpr FavorQuestData Favor_019[] = {
	{ 0x03F4EC, SK_NAME, 0, "", 0x01B123, SK_NAME, 20, 1, "Completionist_Favor019Revyn"},
};

constexpr FavorQuestData Favor_104[] = {
	{ 0x06FD35, SK_NAME, 0x000B50, FS_NAME, 0x018FCF, DB_NAME, 20, 1, "Completionist_Favor104Fanari"},
	{ 0x06FD35, SK_NAME, 0x000B34, FS_NAME, 0x0198B0, SK_NAME, 20, 1, "Completionist_Favor104Ahtar" },
	{ 0x06FD35, SK_NAME, 0x000B3E, FS_NAME, 0x01B092, SK_NAME, 20, 1, "Completionist_Favor104Annekke" },
	{ 0x06FD35, SK_NAME, 0x000B47, FS_NAME, 0x01B117, SK_NAME, 20, 1, "Completionist_Favor104Brunwulf" },
};

constexpr FavorQuestData Favor_109[] = {
	{ 0x0940E0, SK_NAME, 0, "", 0x0198C5, SK_NAME, 20, 1, "Completionist_Favor109Sybille"},
};

constexpr FavorQuestData Favor_110[] = {
	{ 0x0CA439, SK_NAME, 0x000D6E, FS_NAME, 0x041FB9, SK_NAME, 20, 1, "Completionist_Favor110Aldis"},
	{ 0x0CA439, SK_NAME, 0x000D76, FS_NAME, 0x01AA61, SK_NAME, 20, 1, "Completionist_Favor110Lami" },
	{ 0x0CA439, SK_NAME, 0x000D7E, FS_NAME, 0x01A6BE, SK_NAME, 20, 1, "Completionist_Favor110Rustleif" },
	{ 0x0CA439, SK_NAME, 0x000D86, FS_NAME, 0x019822, SK_NAME, 20, 1, "Completionist_Favor110Siddgeir" },
	{ 0x0CA439, SK_NAME, 0x000D6E, FS_NAME, 0x01B12B, SK_NAME, 20, 1, "Completionist_Favor110Torbjorn" },
	{ 0x0CA439, SK_NAME, 0x000D97, FS_NAME, 0x01A69A, SK_NAME, 20, 1, "Completionist_Favor110Ysolda" },
};

constexpr FavorQuestData Favor_151[] = {
	{ 0x06C824, SK_NAME, 0, "", 0x019903, SK_NAME, 20, 1, "Completionist_Favor151Igmund"},
};

constexpr FavorQuestData Favor_153[] = {
	{ 0x090E36, SK_NAME, 0, "", 0x01A6C0, SK_NAME, 20, 1, "Completionist_Favor153Skald"},
};

constexpr FavorQuestData Favor_154[] = {
	{ 0x07172A, SK_NAME, 0, "", 0x019822, SK_NAME, 20, 1, "Completionist_Favor154Siddgeir"},
};

constexpr FavorQuestData Favor_157[] = {
	{ 0x0A3504, SK_NAME, 0, "", 0x019903, SK_NAME, 20, 1, "Completionist_Favor157Igmund"},
};

constexpr FavorQuestData Favor_158[] = {
	{ 0x0940D0, SK_NAME, 0, "", 0x01C188, SK_NAME, 20, 1, "Completionist_Favor158Korir"},
};

constexpr FavorQuestData Favor_204[] = {
	{ 0x07105B, SK_NAME, 0x000DB9, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor204Amren"},
	{ 0x07105B, SK_NAME, 0x000DC2, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor204Shahvee"},
};

constexpr FavorQuestData Favor_205[] = {
	{ 0x06FE37, SK_NAME, 0x000DD5, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor205Frida"},
	{ 0x06FE37, SK_NAME, 0x000DDE, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor205Noster"},
	{ 0x06FE37, SK_NAME, 0x000DE7, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor205Oengul"},
	{ 0x06FE37, SK_NAME, 0x000DF0, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor205Roggi"},
	{ 0x06FE37, SK_NAME, 0x000DF9, FS_NAME, 0x0, SK_NAME, 20, 1, "Completionist_Favor205Runil"},
};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor001
{
	void Favor001::CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_001, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor013
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_013, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor017
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_017, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor018
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_018, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor019
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_019, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor104
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_104, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor109
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_109, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor110
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_110, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor151
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_151, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor153
{
	void CHandler::Sink() {
		const auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_153, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor154
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_154, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor157
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_157, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor158
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_158, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor204
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_204, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace Favor205
{
	void CHandler::Sink() {
		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		FavorProcessor::ProcessQuest(event->GetFormID(), Favor_205, a_event->stage);
		return EventResult::kContinue;
	}
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace FavorProcessor
{
	template <std::size_t N>
	void ProcessQuest(const RE::FormID a_formID, const FavorQuestData(&a_array)[N], uint16_t a_stage) {

		for (auto& dataSet : a_array) {

			const auto* questBase = CompletionistData::GetFullForm<RE::TESQuest>(dataSet.base.ID, dataSet.base.FN);
			const auto* questVari = CompletionistData::GetFullForm<RE::TESQuest>(dataSet.vari.ID, dataSet.vari.FN);
			const auto* questActr = CompletionistData::GetFullForm<RE::Actor>(dataSet.actr.ID, dataSet.actr.FN);
			const auto* questForm = questVari ? questVari : questBase;

			if (!questActr || !questForm || questForm->GetFormID() != a_formID || a_stage != dataSet.Stage) {
				continue;
			}

			INFO("Got Quest - [{}] With Actor - [{}]", questForm->GetName(), questActr->GetName());

			for (const auto& alias : questForm->aliases) {
				if (alias && alias->aliasName == "QuestGiver") {

					auto* reference = static_cast<RE::BGSRefAlias*>(alias);
					if (reference && reference->GetActorReference() == questActr) {
						if (auto* global = RE::TESForm::LookupByEditorID<RE::TESGlobal>(dataSet.Global)) {
							INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", dataSet.Global, questForm->GetName(), questActr->GetName());
							global->value++;
						};
					}
				}
			}
		}
	}

	auto Register() -> void {
		Favor001::CHandler::Sink();
		Favor013::CHandler::Sink();
		Favor017::CHandler::Sink();
		Favor018::CHandler::Sink();
		Favor019::CHandler::Sink();
		Favor104::CHandler::Sink();
		Favor109::CHandler::Sink();
		Favor110::CHandler::Sink();
		Favor151::CHandler::Sink();
		Favor153::CHandler::Sink();
		Favor154::CHandler::Sink();
		Favor157::CHandler::Sink();
		Favor158::CHandler::Sink();
		Favor204::CHandler::Sink();
		Favor205::CHandler::Sink();

		INFO("Finished Registering Favor Event Listeners");
	}
}