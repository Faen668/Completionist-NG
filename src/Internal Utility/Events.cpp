#include "Events.hpp"
#include "DKUtil/Hook.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Internal Utility/PatchListener.hpp"

#define REGISTER_EVENT(EventType) RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<EventType>*>(EventHandler::GetSingleton()))

namespace CEvents 
{
	std::vector<std::function<void(FuncParam_oHit)>> _OnHitEvent;
	std::vector<std::function<void(FuncParam_Deth)>> _OnDeathEvent;
	std::vector<std::function<void(FuncParam_Acti)>> _OnActivateEvent;
	std::vector<std::function<void(FuncParam_Cont)>> _OnTESContainerChangedEvent;

	//---------------------------------------------------
	//-- Event Distributor (Register Events ) -----------
	//---------------------------------------------------

	void EventHandler::Register() 
	{
		RE::BooksRead::GetEventSource()->AddEventSink(EventHandler::GetSingleton());
		RE::UI::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(EventHandler::GetSingleton()));
		REGISTER_EVENT(RE::TESContainerChangedEvent);
		REGISTER_EVENT(RE::TESActivateEvent);
		REGISTER_EVENT(RE::TESDeathEvent);
		REGISTER_EVENT(RE::TESHitEvent);
		REGISTER_EVENT(RE::TESQuestStageEvent);
	};

	//---------------------------------------------------
	//-- Event Distributor (Register Hooks ) ------------
	//---------------------------------------------------

	uintptr_t EventHandler::RegisterUpdateCrosshairHook(void (*f)(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data))
	{
		auto& trampoline = SKSE::GetTrampoline();
		return trampoline.write_call<5>(RELOCATION_ID(39535, 40621).address() + REL::Relocate(0x289, 0x280), f);
	}

	//---------------------------------------------------
	//-- Event Distributor (Register Hooks ) ------------
	//---------------------------------------------------

	uintptr_t EventHandler::RegisterMapMarkerAddedHook(void (*f)(RE::TESFullName* a_form))
	{
		auto& trampoline = SKSE::GetTrampoline();
		return trampoline.write_call<5>(RELOCATION_ID(55617, 56146).address() + REL::Relocate(0x9D, 0x9D), f);
	}

	//---------------------------------------------------
	//-- Event Distributor (Register Hooks ) ------------
	//---------------------------------------------------

	uintptr_t EventHandler::RegisterMapMarkerDiscoveredHook(const char* (*f)(RE::TESFullName* a_form))
	{
		auto& trampoline = SKSE::GetTrampoline();
		return trampoline.write_call<5>(RELOCATION_ID(39663, 40750).address() + REL::Relocate(0x1CC, 0x1EC), f);
	}

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnContainerChangedEvent(void (*f)(FuncParam_Cont))
	{
		_OnTESContainerChangedEvent.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnActivateEvent(void (*f)(FuncParam_Acti))
	{
		_OnActivateEvent.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnDeathEvent(void (*f)(FuncParam_Deth))
	{
		_OnDeathEvent.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnHitEvent(void (*f)(FuncParam_oHit))
	{
		_OnHitEvent.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Stage Triggered / Set ) -
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Stag, RE::BSTEventSource<RE::TESQuestStageEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnQuestStageTriggered(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Menu Open / Close ) -----
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_oHit, RE::BSTEventSource<RE::TESHitEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnHitEvent) {
			fn(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Menu Open / Close ) -----
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Menu, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnMenuOpenCloseEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Activate ) --------------
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Acti, RE::BSTEventSource<RE::TESActivateEvent>*)
	{
		if (!a_event || !a_event->objectActivated || !a_event->actionRef || a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) { return EventResult::kContinue; }

		for (auto& fn : _OnActivateEvent) {
			fn(a_event);
		}

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnActivateEvent(a_event->objectActivated.get());
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Contaienr Changed ) -----
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Cont, RE::BSTEventSource<RE::TESContainerChangedEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnTESContainerChangedEvent) {
			fn(a_event);
		};

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnContainerChangedEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( Books Read ) ---------------
	//---------------------------------------------------

	EventResult EventHandler::ProcessEvent(FuncParam_Book, RE::BSTEventSource<RE::BooksRead::Event>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnBooksReadEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Death ) -----------------
	//---------------------------------------------------

	EventResult EventHandler::ProcessEvent(FuncParam_Deth, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnDeathEvent) {
			fn(a_event);
		};
		return EventResult::kContinue;
	};
}