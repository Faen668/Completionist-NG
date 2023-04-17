#include "Quests_Favors.hpp"

using namespace Serialization;

constexpr const char* SK_NAME = "Skyrim.esm";
constexpr const char* DG_NAME = "Dawnguard.esm";
constexpr const char* DB_NAME = "Dragonborn.esm";
constexpr const char* FS_NAME = "Favor Quests Seperated.esp";

FavorMergedData Favor_001[] = {
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x00082d, FS_NAME}, FavorActorData{0x01413c, SK_NAME}, 20, 1, "Completionist_Favor001Adonato"},
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x000835, FS_NAME}, FavorActorData{0x01360b, SK_NAME}, 20, 1, "Completionist_Favor001Aeri" },
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x000838, FS_NAME}, FavorActorData{0x09a7a8, SK_NAME}, 20, 1, "Completionist_Favor001Banning" },
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x00083d, FS_NAME}, FavorActorData{0x0135ec, SK_NAME}, 20, 1, "Completionist_Favor001Idgrod" },
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x000842, FS_NAME}, FavorActorData{0x01366b, SK_NAME}, 20, 1, "Completionist_Favor001Sondas" },
	{ FavorQuestData{0x0403b0, SK_NAME}, FavorQuestData{0x000847, FS_NAME}, FavorActorData{0x04e5e9, SK_NAME}, 20, 1, "Completionist_Favor001Thadgeir" },
};

FavorMergedData Favor_013[] = {
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x000869, FS_NAME}, FavorActorData{0x013b99, SK_NAME}, 20, 1, "Completionist_Favor013CarlottaValentia"},
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x000887, FS_NAME}, FavorActorData{0x01C184, SK_NAME}, 20, 1, "Completionist_Favor013Haran" },
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x000896, FS_NAME}, FavorActorData{0x013662, SK_NAME}, 20, 1, "Completionist_Favor013Iddra" },
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x0008A5, FS_NAME}, FavorActorData{0x01329d, SK_NAME}, 20, 1, "Completionist_Favor013OctieveSan" },
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x0008B3, FS_NAME}, FavorActorData{0x0133ae, SK_NAME}, 20, 1, "Completionist_Favor013Omluag" },
	{ FavorQuestData{0x0CA209, SK_NAME}, FavorQuestData{0x000878, FS_NAME}, FavorActorData{0x01412e, SK_NAME}, 20, 1, "Completionist_Favor013ScoutsManyMarshes" },
};

FavorMergedData Favor_017[] = {
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x0135e8, SK_NAME}, 20, 1, "Completionist_Favor017Benor"},
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x013b79, SK_NAME}, 20, 1, "Completionist_Favor017Burguk" },
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x019951, SK_NAME}, 20, 1, "Completionist_Favor017Larak" },
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x01b075, SK_NAME}, 20, 1, "Completionist_Favor017Mauhulakh" },
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x013390, SK_NAME}, 20, 1, "Completionist_Favor017Cosnach" },
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x03efe9, SK_NAME}, 20, 1, "Completionist_Favor017Rolff" },
	{ FavorQuestData{0x0CA210, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x0918e2, SK_NAME}, 20, 1, "Completionist_Favor017Uthgerd" },
};

FavorMergedData Favor_018[] = {
	{ FavorQuestData{0x03F46A, SK_NAME}, FavorQuestData{0x000A0C, FS_NAME}, FavorActorData{0x01365a, SK_NAME}, 20, 1, "Completionist_Favor018Dengeir"},
	{ FavorQuestData{0x03F46A, SK_NAME}, FavorQuestData{0x000A1D, FS_NAME}, FavorActorData{0x01C182, SK_NAME}, 20, 1, "Completionist_Favor018Malur" },
	{ FavorQuestData{0x03F46A, SK_NAME}, FavorQuestData{0x000A14, FS_NAME}, FavorActorData{0x014130, SK_NAME}, 20, 1, "Completionist_Favor018StandInShallows" },
};

FavorMergedData Favor_019[] = {
	{ FavorQuestData{0x03F4EC, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x01413a, SK_NAME}, 20, 1, "Completionist_Favor019Revyn"},
};

FavorMergedData Favor_104[] = {
	{ FavorQuestData{0x06FD35, SK_NAME}, FavorQuestData{0x000B50, FS_NAME}, FavorActorData{0x018fc5, DB_NAME}, 20, 1, "Completionist_Favor104Fanari"},
	{ FavorQuestData{0x06FD35, SK_NAME}, FavorQuestData{0x000B34, FS_NAME}, FavorActorData{0x01325f, SK_NAME}, 20, 1, "Completionist_Favor104Ahtar" },
	{ FavorQuestData{0x06FD35, SK_NAME}, FavorQuestData{0x000B3E, FS_NAME}, FavorActorData{0x013666, SK_NAME}, 20, 1, "Completionist_Favor104Annekke" },
	{ FavorQuestData{0x06FD35, SK_NAME}, FavorQuestData{0x000B47, FS_NAME}, FavorActorData{0x014149, SK_NAME}, 20, 1, "Completionist_Favor104Brunwulf" },
};

FavorMergedData Favor_109[] = {
	{ FavorQuestData{0x0940E0, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x0132aa, SK_NAME}, 20, 1, "Completionist_Favor109Sybille"},
};

FavorMergedData Favor_110[] = {
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D6E, FS_NAME}, FavorActorData{0x041fb8, SK_NAME}, 20, 1, "Completionist_Favor110Aldis"},
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D76, FS_NAME}, FavorActorData{0x0135ef, SK_NAME}, 20, 1, "Completionist_Favor110Lami" },
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D7E, FS_NAME}, FavorActorData{0x01361e, SK_NAME}, 20, 1, "Completionist_Favor110Rustleif" },
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D86, FS_NAME}, FavorActorData{0x013653, SK_NAME}, 20, 1, "Completionist_Favor110Siddgeir" },
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D6E, FS_NAME}, FavorActorData{0x01413f, SK_NAME}, 20, 1, "Completionist_Favor110Torbjorn" },
	{ FavorQuestData{0x0CA439, SK_NAME}, FavorQuestData{0x000D97, FS_NAME}, FavorActorData{0x013bab, SK_NAME}, 20, 1, "Completionist_Favor110Ysolda" },
};

FavorMergedData Favor_151[] = {
{ FavorQuestData{0x06C824, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x01339f, SK_NAME}, 20, 1, "Completionist_Favor151Igmund"},
};

FavorMergedData Favor_153[] = {
	{ FavorQuestData{0x090E36, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x013620, SK_NAME}, 20, 1, "Completionist_Favor153Skald"},
};

FavorMergedData Favor_154[] = {
	{ FavorQuestData{0x07172A, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x013653, SK_NAME}, 20, 1, "Completionist_Favor154Siddgeir"},
};

FavorMergedData Favor_157[] = {
	{ FavorQuestData{0x0A3504, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x01339f, SK_NAME}, 20, 1, "Completionist_Favor157Igmund"},
};

FavorMergedData Favor_158[] = {
	{ FavorQuestData{0x0940D0, SK_NAME}, FavorQuestData{0, ""}, FavorActorData{0x01c188, SK_NAME}, 20, 1, "Completionist_Favor158Korir"},
};

FavorMergedData Favor_204[] = {
	{ FavorQuestData{0x07105B, SK_NAME}, FavorQuestData{0x000DB9, FS_NAME}, FavorActorData{0x013baa, SK_NAME}, 20, 1, "Completionist_Favor204Amren"},
	{ FavorQuestData{0x07105B, SK_NAME}, FavorQuestData{0x000DC2, FS_NAME}, FavorActorData{0x01411a, SK_NAME}, 20, 1, "Completionist_Favor204Shahvee"},
};

FavorMergedData Favor_205[] = {
	{ FavorQuestData{0x06FE37, SK_NAME}, FavorQuestData{0x000DD5, FS_NAME}, FavorActorData{0x013614, SK_NAME}, 20, 1, "Completionist_Favor205Frida"},
	{ FavorQuestData{0x06FE37, SK_NAME}, FavorQuestData{0x000DDE, FS_NAME}, FavorActorData{0x01329c, SK_NAME}, 20, 1, "Completionist_Favor205Noster"},
	{ FavorQuestData{0x06FE37, SK_NAME}, FavorQuestData{0x000DE7, FS_NAME}, FavorActorData{0x014142, SK_NAME}, 20, 1, "Completionist_Favor205Oengul"},
	{ FavorQuestData{0x06FE37, SK_NAME}, FavorQuestData{0x000DF0, FS_NAME}, FavorActorData{0x01403f, SK_NAME}, 20, 1, "Completionist_Favor205Roggi"},
	{ FavorQuestData{0x06FE37, SK_NAME}, FavorQuestData{0x000DF9, FS_NAME}, FavorActorData{0x01364d, SK_NAME}, 20, 1, "Completionist_Favor205Runil"},
};

MerchantFavors activeMerchant{};

FavorMerchantData Merchant_Data[] = {
	{0x01360b, "Completionist_Favor_ChopWoodAeri"},
	{0x01365d, "Completionist_Favor_ChopWoodGannaUriel"},
	{0x01367a, "Completionist_Favor_ChopWoodGilfre"},
	{0x019c00, "Completionist_Favor_ChopWoodGrosta"},
	{0x01367c, "Completionist_Favor_ChopWoodHert"},
	{0x013287, "Completionist_Favor_ChopWoodHjorunn"},
	{0x019a1d, "Completionist_Favor_ChopWoodHorgeir"},
	{0x0135f0, "Completionist_Favor_ChopWoodHroggar"},
	{0x013ba3, "Completionist_Favor_ChopWoodHulda"},
	{0x01347d, "Completionist_Favor_ChopWoodHod"},
	{0x0138b6, "Completionist_Favor_ChopWoodJorgen"},
	{0x0658d2, "Completionist_Favor_ChopWoodTembaWideArm"},

	{0x013612, "Completionist_FavorMineOreBeitld"},
	{0x01b079, "Completionist_FavorMineOreDushnamub"},
	{0x013603, "Completionist_FavorMineOreGestur"},
	{0x013b7c, "Completionist_FavorMineOreGharol"},
	{0x0136c6, "Completionist_FavorMineOreGrogmar"},
	{0x013663, "Completionist_FavorMineOreKjeld"},
	{0x01361b, "Completionist_FavorMineOreLeigelf"},
	{0x0133b1, "Completionist_FavorMineOrePavo"},
	{0x01996c, "Completionist_FavorMineOrePerth"},
	{0x019957, "Completionist_FavorMineOreShuftharz"},
	{0x0133b8, "Completionist_FavorMineOreSkaggi"},
	{0x0ae777, "Completionist_FavorMineOreThorgar"},
	{0x013665, "Completionist_FavorMineOreVerner"},

	{0x019dc7, "Completionist_FavorHarvestAddvild"},
	{0x013bb0, "Completionist_FavorHarvestAlfhild"},
	{0x019bfe, "Completionist_FavorHarvestAvrusa"},
	{0x019a2a, "Completionist_FavorHarvestAzzada"},
	{0x014138, "Completionist_FavorHarvestBelyn"},
	{0x014126, "Completionist_FavorHarvestBolfrida"},
	{0x0136be, "Completionist_FavorHarvestBoti"},
	{0x013290, "Completionist_FavorHarvestKatla"},
	{0x0136b8, "Completionist_FavorHarvestLemkil"},
	{0x01364e, "Completionist_FavorHarvestMathies"},
	{0x0136b4, "Completionist_FavorHarvestReldith"},
	{0x02c925, "Completionist_FavorHarvestSeverio"},
	{0x0bb2c0, "Completionist_FavorHarvestSynda"},
	{0x014139, "Completionist_FavorHarvestTulvur"},
	{0x0133bf, "Completionist_FavorHarvestVigdis"},
};

std::vector<FavorMergedData*> FavorArray{};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace CQFramework_FavorQuests
{
	template <std::size_t N>
	void CHandler::AddToArray(FavorMergedData(&a_data)[N]) {

		for (auto idx = 0; idx < N; ++idx) {
			FavorArray.push_back(&a_data[idx]);

			const auto* questBase = CompletionistData::GetFullForm<RE::TESQuest>(a_data[idx].base.formID, a_data[idx].base.fileName);
			const auto* questVari = CompletionistData::GetFullForm<RE::TESQuest>(a_data[idx].vari.formID, a_data[idx].vari.fileName);
			const auto* questActr = CompletionistData::GetFullForm<RE::TESNPC>(a_data[idx].actr.formID, a_data[idx].actr.fileName);
			const auto* questForm = questVari ? questVari : questBase ? questBase : nullptr;

			INFO("Installed Favor Quest Array - [{}] With Actor - [{}]", (questForm && questForm->GetName()) ? questForm->GetName() : "ERROR GETTING Quest", (questActr && questActr->GetName()) ? questActr->GetName() : "ERROR GETTING ACTOR");
		}
		INFO(" ");
	}

	void CHandler::InstallFramework()
	{
		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(GetSingleton()));

		AddToArray(Favor_001);
		AddToArray(Favor_013);
		AddToArray(Favor_017);
		AddToArray(Favor_018);
		AddToArray(Favor_019);
		AddToArray(Favor_104);
		AddToArray(Favor_109);
		AddToArray(Favor_110);
		AddToArray(Favor_151);
		AddToArray(Favor_153);
		AddToArray(Favor_154);
		AddToArray(Favor_157);
		AddToArray(Favor_158);
		AddToArray(Favor_204);
		AddToArray(Favor_205);
	}

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) 
		{ 
			return RE::BSEventNotifyControl::kContinue; 
		}

		for (auto& data : FavorArray)
		{
			const auto* eventBase = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
			const auto* questBase = CompletionistData::GetFullForm<RE::TESQuest>(data->base.formID, data->base.fileName);
			const auto* questVari = CompletionistData::GetFullForm<RE::TESQuest>(data->vari.formID, data->vari.fileName);
			const auto* questActr = CompletionistData::GetFullForm<RE::TESNPC>(data->actr.formID, data->actr.fileName);
			const auto* questForm = questVari ? questVari : questBase;

			if (!questForm || !questActr || questForm->GetFormID() != eventBase->formID || a_event->stage != data->Stage) {
				continue;
			}

			for (const auto& alias : questForm->aliases) {
				if (alias && alias->aliasName == "QuestGiver") {

					auto* reference = static_cast<RE::BGSRefAlias*>(alias);
					if (reference && reference->GetActorReference()->GetActorBase()->GetFormID() == questActr->GetFormID()) {
						if (auto* global = RE::TESForm::LookupByEditorID<RE::TESGlobal>(data->Global)) {
							INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", data->Global, questForm->GetName(), questActr->GetName());
							global->value++;
						};
					}
				}
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Favors for Chopping Wood and Selling Ore -------
	//---------------------------------------------------

	void CHandler::RegisterMerchant(RE::StaticFunctionTag*, RE::Actor* a_actor) {
		
		auto curspeaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();

		if (!a_actor || !curspeaker || a_actor->GetFormID() != curspeaker->GetFormID()) { 
			return; 
		}

		activeMerchant.Merchant = a_actor->GetFormID();
		activeMerchant.BaseActor = RE::TESForm::LookupByID<RE::TESNPC>(a_actor->GetActorBase()->GetFormID());
		INFO("Registered Merchant: REF:{} BASE:{} - [{}]", activeMerchant.Merchant, activeMerchant.BaseActor->GetFormID(), RE::TESForm::LookupByID<RE::Actor>(activeMerchant.Merchant)->GetName());
	}

	void CHandler::UnRegisterMerchant(RE::StaticFunctionTag*, RE::Actor* a_actor) {

		activeMerchant = {};
		INFO("Unregistered Merchant");
	}

	EventResult CHandler::ProcessEvent(RE::TESContainerChangedEvent const* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (a_event && a_event->baseObj)
		{
			if (a_event->newContainer == RE::PlayerCharacter::GetSingleton()->GetFormID() && activeMerchant.SoldForm) {
				auto* GoldForm = RE::TESForm::LookupByID(0x00000f);
				auto* EvntForm = RE::TESForm::LookupByID(a_event->baseObj);

				if (GoldForm->GetFormID() == EvntForm->GetFormID())
				{
					INFO("Receieved {} Gold for: {} {}. Performing NPC Lookup...", a_event->itemCount, activeMerchant.SoldQuantity, RE::TESForm::LookupByID(activeMerchant.SoldForm)->GetName());
				}

				for (auto& [FormID, Global] : Merchant_Data) 
				{
					auto* LookupNPC = RE::TESForm::LookupByID<RE::TESNPC>(FormID);
					auto* GlobalVar = RE::TESForm::LookupByEditorID<RE::TESGlobal>(Global);

					auto oldvalue = GlobalVar->value;

					if (GlobalVar && LookupNPC->GetFormID() == activeMerchant.BaseActor->GetFormID()) {
						GlobalVar->value++;
						INFO("Incremented Global Variable for Matching Actor: {} from {} > {}", activeMerchant.BaseActor->GetName(), oldvalue, GlobalVar->value);
						return EventResult::kContinue;
					}
				}

				return EventResult::kContinue;
			}

			if (a_event->oldContainer == RE::PlayerCharacter::GetSingleton()->GetFormID()) {
				auto curspeaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();

				if (activeMerchant.Merchant && curspeaker && activeMerchant.Merchant == curspeaker->GetFormID()) {

					auto* SoldForm = RE::TESForm::LookupByID(a_event->baseObj);

					if (SoldForm) {
						activeMerchant.SoldForm = SoldForm->GetFormID();
						activeMerchant.SoldQuantity = a_event->itemCount;
					}
				}
			}
		}
		return EventResult::kContinue;
	};
}