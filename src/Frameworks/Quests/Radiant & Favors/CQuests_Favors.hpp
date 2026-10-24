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
		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		  EventResult ProcessEvent(RE::TESContainerChangedEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;
		  EventResult ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		  static void InstallFramework();
		  static void RegisterMerchant(RE::Actor* a_actor);
		  static void UnRegisterMerchant();
		  static void AddMerchantData(RE::FormID NPCBaseFormID, const std::string& fileName, const std::string& Quest_Key);
	};
}