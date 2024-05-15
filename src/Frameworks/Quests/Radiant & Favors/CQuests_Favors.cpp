#include "CQuests_Favors.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

FavorMerchantData Merchant_Data[] = 
{
	{0x01360b, "Radi_SK_Quest05"},
	{0x01365d, "Radi_SK_Quest07"},
	{0x01367a, "Radi_SK_Quest08"},
	{0x019c00, "Radi_SK_Quest06"},
	{0x01367c, "Falkreath_Quest04"},
	{0x013287, "Radi_SK_Quest09"},
	{0x019a1d, "SmallTowns_Quest11"},
	{0x0135f0, "Morthal_Quest03"},
	{0x013ba3, "Whiterun_Quest11"},
	{0x01347d, "SmallTowns_Quest10"},
	{0x0138b6, "Morthal_Quest04"},
	{0x0658d2, "SmallTowns_Quest12"},

	{0x013612, "Dawnstar_Quest08"},
	{0x01b079, "Radi_SK_Quest27"},
	{0x013603, "Radi_SK_Quest28"},
	{0x013b7c, "Radi_SK_Quest29"},
	{0x0136c6, "Radi_SK_Quest30"},
	{0x013663, "Radi_SK_Quest31"},
	{0x01361b, "Dawnstar_Quest09"},
	{0x0133b1, "Radi_SK_Quest32"},
	{0x01996c, "Radi_SK_Quest33"},
	{0x019957, "Radi_SK_Quest34"},
	{0x0133b8, "Radi_SK_Quest35"},
	{0x0ae777, "Radi_SK_Quest36"},
	{0x013665, "Radi_SK_Quest37"},

	{0x019dc7, "Radi_SK_Quest16"},
	{0x013bb0, "Radi_SK_Quest17"},
	{0x019bfe, "Radi_SK_Quest18"},
	{0x019a2a, "SmallTowns_Quest13"},
	{0x014138, "Radi_SK_Quest19"},
	{0x014126, "Radi_SK_Quest20"},
	{0x0136be, "SmallTowns_Quest14"},
	{0x013290, "Radi_SK_Quest21"},
	{0x0136b8, "SmallTowns_Quest15"},
	{0x01364e, "Falkreath_Quest07"},
	{0x0136b4, "SmallTowns_Quest16"},
	{0x02c925, "Radi_SK_Quest22"},
	{0x0bb2c0, "Radi_SK_Quest23"},
	{0x014139, "Radi_SK_Quest24"},
	{0x0133bf, "Radi_SK_Quest25"},
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

		INFO("Unregistered Merchant: REF:{} BASE:{} - [{}]", activeMerchant.Merchant, activeMerchant.BaseActor->GetFormID(), RE::TESForm::LookupByID<RE::Actor>(activeMerchant.Merchant)->GetName());
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
					INFO("Receieved {} Gold for: {}x {}.", a_event->itemCount, activeMerchant.SoldQuantity, soldForm ? soldForm ->GetName() : "ERROR");
				}

				for (auto& [FormID, Quest_Key] : Merchant_Data) 
				{
					auto* LookupNPC = RE::TESForm::LookupByID<RE::TESNPC>(FormID);

					if (LookupNPC->GetFormID() == activeMerchant.BaseActor->GetFormID()) {
						CFramework_Master::RadiantCountData.IncreaseCount(Quest_Key, 1);
						CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(Quest_Key);
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
					}
				}
			}
		}
		return EventResult::kContinue;
	};
}