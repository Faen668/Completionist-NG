#pragma once
#include "Structs.hpp"
#include "Serialization.hpp";	

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

struct MerchantFavors
{
	RE::FormID Merchant;
	RE::FormID SoldForm;
	int32_t SoldQuantity;
	RE::TESNPC* BaseActor;
};

namespace CQFramework_FavorQuests
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;		 
		  EventResult ProcessEvent(RE::TESContainerChangedEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

		  static void InstallFramework();

		  template <std::size_t N>
		  static void AddToArray(FavorMergedData(&a_array)[N]);

		  static void RegisterMerchant(RE::StaticFunctionTag*, RE::Actor*);
		  static void UnRegisterMerchant(RE::StaticFunctionTag*, RE::Actor*);
	};
}