#include "CQuests_Favors.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

std::vector<FavorMerchantData> Merchant_Data = 
{
	{0x01360b, "Skyrim.esm", "Radi_SK_Quest05"},
	{0x01365d, "Skyrim.esm", "Radi_SK_Quest07"},
	{0x01367a, "Skyrim.esm", "Radi_SK_Quest08"},
	{0x019c00, "Skyrim.esm", "Radi_SK_Quest06"},
	{0x01367c, "Skyrim.esm", "Falkreath_Quest04"},
	{0x013287, "Skyrim.esm", "Radi_SK_Quest09"},
	{0x019a1d, "Skyrim.esm", "SmallTowns_Quest11"},
	{0x0135f0, "Skyrim.esm", "Morthal_Quest03"},
	{0x013ba3, "Skyrim.esm", "Whiterun_Quest11"},
	{0x01347d, "Skyrim.esm", "SmallTowns_Quest10"},
	{0x0138b6, "Skyrim.esm", "Morthal_Quest04"},
	{0x0658d2, "Skyrim.esm", "SmallTowns_Quest12"},

	{0x013612, "Skyrim.esm", "Dawnstar_Quest08"},
	{0x01b079, "Skyrim.esm", "Radi_SK_Quest27"},
	{0x013603, "Skyrim.esm", "Radi_SK_Quest28"},
	{0x013b7c, "Skyrim.esm", "Radi_SK_Quest29"},
	{0x0136c6, "Skyrim.esm", "Radi_SK_Quest30"},
	{0x013663, "Skyrim.esm", "Radi_SK_Quest31"},
	{0x01361b, "Skyrim.esm", "Dawnstar_Quest09"},
	{0x0133b1, "Skyrim.esm", "Radi_SK_Quest32"},
	{0x01996c, "Skyrim.esm", "Radi_SK_Quest33"},
	{0x019957, "Skyrim.esm", "Radi_SK_Quest34"},
	{0x0133b8, "Skyrim.esm", "Radi_SK_Quest35"},
	{0x0ae777, "Skyrim.esm", "Radi_SK_Quest36"},
	{0x013665, "Skyrim.esm", "Radi_SK_Quest37"},

	{0x019dc7, "Skyrim.esm", "Radi_SK_Quest16"},
	{0x013bb0, "Skyrim.esm", "Radi_SK_Quest17"},
	{0x019bfe, "Skyrim.esm", "Radi_SK_Quest18"},
	{0x019a2a, "Skyrim.esm", "SmallTowns_Quest13"},
	{0x014138, "Skyrim.esm", "Radi_SK_Quest19"},
	{0x014126, "Skyrim.esm", "Radi_SK_Quest20"},
	{0x0136be, "Skyrim.esm", "SmallTowns_Quest14"},
	{0x013290, "Skyrim.esm", "Radi_SK_Quest21"},
	{0x0136b8, "Skyrim.esm", "SmallTowns_Quest15"},
	{0x01364e, "Skyrim.esm", "Falkreath_Quest07"},
	{0x0136b4, "Skyrim.esm", "SmallTowns_Quest16"},
	{0x02c925, "Skyrim.esm", "Radi_SK_Quest22"},
	{0x0bb2c0, "Skyrim.esm", "Radi_SK_Quest23"},
	{0x014139, "Skyrim.esm", "Radi_SK_Quest24"},
	{0x0133bf, "Skyrim.esm", "Radi_SK_Quest25"},
};

MerchantFavors activeMerchant{};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace CQFramework_FavorQuests
{
	void CHandler::InstallFramework()
	{
		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Add Merchant Data ------------------------------
	//---------------------------------------------------

	void CHandler::AddMerchantData(RE::FormID NPCBaseFormID, const std::string& fileName, const std::string& Quest_Key)
	{	
		FavorMerchantData data = { NPCBaseFormID, fileName, Quest_Key };
		Merchant_Data.push_back(data);
		//INFO("Added New Merchant Data: {} - {} - {}", Serialization::CompletionistData::GetFormIDHexString(NPCBaseFormID), fileName, Quest_Key)
	}

	//---------------------------------------------------
	//-- Favors for Chopping Wood and Selling Ore -------
	//---------------------------------------------------

	void CHandler::RegisterMerchant(RE::StaticFunctionTag*, RE::Actor* a_actor) {

		auto curspeaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();
		if (!a_actor || !curspeaker || a_actor->GetFormID() != curspeaker->GetFormID()) { 
			return; 
		}

		auto* baseActor = RE::TESForm::LookupByID<RE::TESNPC>(a_actor->GetActorBase()->GetFormID());
		if (!baseActor) {
			return;
		}

		activeMerchant.Merchant = a_actor->GetFormID();
		activeMerchant.BaseActor = baseActor;

		INFO("Registered NPC: REF:{} BASE:{} - [{}]", Serialization::CompletionistData::GetFormIDHexString(activeMerchant.Merchant), Serialization::CompletionistData::GetFormIDHexString(baseActor->GetFormID()), RE::TESForm::LookupByID<RE::Actor>(activeMerchant.Merchant)->GetName());
	}

	void CHandler::UnRegisterMerchant(RE::StaticFunctionTag*, RE::Actor* a_actor) {

		if (activeMerchant.Merchant && activeMerchant.BaseActor) {
			INFO("Unregistered NPC: REF:{} BASE:{} - [{}]", Serialization::CompletionistData::GetFormIDHexString(activeMerchant.Merchant), Serialization::CompletionistData::GetFormIDHexString(activeMerchant.BaseActor->GetFormID()), RE::TESForm::LookupByID<RE::Actor>(activeMerchant.Merchant)->GetName());
		}
		activeMerchant = {};
	}

	EventResult CHandler::ProcessEvent(RE::TESContainerChangedEvent const* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource)
	{
		if (a_event && a_event->baseObj)
		{
			if (a_event->newContainer == RE::PlayerCharacter::GetSingleton()->GetFormID() && activeMerchant.SoldForm) {
				auto* GoldForm = RE::TESForm::LookupByID(0x00000f);
				auto* EvntForm = RE::TESForm::LookupByID(a_event->baseObj);
				auto* soldForm = RE::TESForm::LookupByID(activeMerchant.SoldForm);

				if (GoldForm->GetFormID() == EvntForm->GetFormID())
				{
					INFO("Receieved {} Gold for: x{} {}.", a_event->itemCount, activeMerchant.SoldQuantity, soldForm ? soldForm->GetName() : "ERROR");
				}

				for (const auto& [formID, fileName, uuid] : Merchant_Data)
				{
					auto* npc = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESNPC>(formID, fileName);
					INFO("Processing NPC: {} - [{}] from {}", formID, npc ? npc->GetName() : "None", fileName);

					if (npc && npc->GetFormID() == activeMerchant.BaseActor->GetFormID()) {
						CFramework_Master::RadiantCountData.IncreaseCount(uuid, 1);
						CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(uuid);
						return EventResult::kContinue;
					}
				}
				return EventResult::kContinue;
			}

			if (a_event->oldContainer == RE::PlayerCharacter::GetSingleton()->GetFormID()) {
				auto curspeaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();

				if (activeMerchant.Merchant && curspeaker && activeMerchant.Merchant == curspeaker->GetFormID()) {

					auto* soldForm = RE::TESForm::LookupByID(a_event->baseObj);

					if (soldForm) {
						activeMerchant.SoldForm = soldForm->GetFormID();
						activeMerchant.SoldQuantity = a_event->itemCount;

						INFO("Sold {} {} To NPC: REF:{} BASE:{} - [{}]", a_event->itemCount, soldForm->GetName(), Serialization::CompletionistData::GetFormIDHexString(activeMerchant.Merchant), Serialization::CompletionistData::GetFormIDHexString(activeMerchant.BaseActor->GetFormID()), RE::TESForm::LookupByID<RE::Actor>(activeMerchant.Merchant)->GetName());
					}
				}
			}
		}
		return EventResult::kContinue;
	};
}