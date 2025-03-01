#include "Events.hpp"
#include "DKUtil/Hook.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Internal Utility/PatchListener.hpp"

#define REGISTER_EVENT(EventType) RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<EventType>*>(EventHandler::GetSingleton()))

namespace CEvents 
{
	std::vector<std::function<void(FuncParam_oHit)>> _OnHitEvent;
	std::vector<std::function<void(FuncParam_Deth)>> _OnDeathEvent;
	std::vector<std::function<void(FuncParam_Book)>> _OnBooksRead;
	std::vector<std::function<void(FuncParam_Equp)>> _OnEquipEvent;
	std::vector<std::function<void(FuncParam_Acti)>> _OnActivateEvent;
	std::vector<std::function<void(FuncParam_Menu)>> _OnMenuOpenCloseEvent;
	std::vector<std::function<void(FuncParam_Cont)>> _OnTESContainerChangedEvent;
	std::vector<std::function<void(FuncParam_Stag)>> _OnStageTriggered;

	//---------------------------------------------------
	//-- Event Distributor (Register Events ) -----------
	//---------------------------------------------------

	void EventHandler::Register() 
	{
		RE::BooksRead::GetEventSource()->AddEventSink(EventHandler::GetSingleton());
		RE::UI::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(EventHandler::GetSingleton()));
		REGISTER_EVENT(RE::TESContainerChangedEvent);
		REGISTER_EVENT(RE::TESEquipEvent);
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

	uintptr_t EventHandler::RegisterUpdateInventoryNameHook(const char* (*f)(RE::InventoryEntryData* a_this))
	{
		auto& trampoline = SKSE::GetTrampoline();
		return trampoline.write_branch<5>(RELOCATION_ID(50926, 51803).address() + REL::Relocate(0x4, 0x4), f);
	}

	//---------------------------------------------------
	//-- Event Distributor (Register Hooks ) ------------
	//---------------------------------------------------

	void EventHandler::RegisterUpdateCraftingMenuHook()
	{
		const auto base = REL::RelocationID(50511, 51227).address();
		const auto offset = REL::Relocate(0x28, 0xC4);
		const auto addr = base + offset;

		auto& trampoline = SKSE::GetTrampoline();

		if (REL::Module::get().IsAE()) {
			// reroute skse's AE scaleform hook to ret trampoline instead of jmp back
			const auto skse_trampoline = dku::Hook::GetDisp(addr);
			const auto skse_scaleform_hook = *RE::stl::adjust_pointer<std::uintptr_t>(AsPointer(skse_trampoline), 0x6);
			const auto skse_assembly_size = 0x16;
			const auto skse_detour_return = skse_scaleform_hook + skse_assembly_size;
			const auto skse_detour_size = sizeof(dku::Hook::JmpRip) + sizeof(std::uintptr_t);

			const auto buffer = trampoline.allocate(skse_detour_size);
			// prolog
			dku::Hook::WritePatch(buffer, { AsPointer(skse_detour_return), skse_detour_size });
			dku::Hook::WriteImm(skse_detour_return + sizeof(dku::Hook::JmpRip), AsAddress(buffer));

			auto ae_hook = dku::Hook::AddCaveHook(
				AsAddress(buffer),
				{ 0, skse_detour_size },
				FUNC_INFO(Completionist_MainHUD::TextnTagsAPI::OnUpdateCraftingText),
				{ AsPointer(base + 0xBD), 0x7 },
				{},
				dku::Hook::HookFlag::kRestoreAfterEpilog);
			ae_hook->Enable();
		}
		else {
			trampoline.write_call<5>(addr, Completionist_MainHUD::TextnTagsAPI::OnUpdateCraftingText);
		}
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
	//-- Event Distributor (Register Hooks ) ------------
	//---------------------------------------------------

	uintptr_t EventHandler::RegisterEnchantmentHook(const char* (*f)(RE::TESForm* a_form), bool a_yesImSureCompat = false)
	{
		auto& trampoline = SKSE::GetTrampoline();
		return trampoline.write_call<5>(RELOCATION_ID(50459, 51363).address() + REL::Relocate(0x1B1, 0x1B1), f);
	}

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnBooksReadEvent(void (*f)(FuncParam_Book))
	{
		_OnBooksRead.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnMenuOpenCloseEvent(void (*f)(FuncParam_Menu))
	{
		_OnMenuOpenCloseEvent.push_back(f);
	};

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

	void EventHandler::RegisterForEvent_OnEquipEvent(void (*f)(FuncParam_Equp))
	{
		_OnEquipEvent.push_back(f);
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
	//-- Event Distributor (Register Function ) ---------
	//---------------------------------------------------

	void EventHandler::RegisterForEvent_OnStageTriggeredEvent(void (*f)(FuncParam_Stag))
	{
		_OnStageTriggered.push_back(f);
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Stage Triggered / Set ) -
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Stag, RE::BSTEventSource<RE::TESQuestStageEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnStageTriggered) {
			fn(a_event);
		};

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

		for (auto& fn : _OnMenuOpenCloseEvent) {
			fn(a_event);
		};

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
		if (!a_event || !a_event->objectActivated || a_event->actionRef.get()->GetFormID() != RE::PlayerCharacter::GetSingleton()->GetFormID()) { return EventResult::kContinue; }

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

		for (auto& fn : _OnBooksRead) {
			fn(a_event);
		};

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnBooksReadEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Equip ) -----------------
	//---------------------------------------------------

	EventResult EventHandler::ProcessEvent(FuncParam_Equp, RE::BSTEventSource<RE::TESEquipEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnEquipEvent) {
			fn(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Equip ) -----------------
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