#pragma once
#define FuncParam_Book RE::BooksRead::Event const* a_event
#define FuncParam_Menu RE::MenuOpenCloseEvent const* a_event
#define FuncParam_Cont RE::TESContainerChangedEvent const* a_event
#define FuncParam_Equp RE::TESEquipEvent const* a_event
#define FuncParam_Acti RE::TESActivateEvent const* a_event
#define FuncParam_Deth RE::TESDeathEvent const* a_event
#define FuncParam_oHit RE::TESHitEvent const* a_event
#define FuncParam_Stag RE::TESQuestStageEvent const* a_event

namespace CEvents
{
	constexpr inline REL::RelocationID IsStageDoneAddress = RELOCATION_ID(24483, 25011);
	constexpr inline REL::RelocationID GetAliasLocAddress = RELOCATION_ID(24538, 25067);

	using EventResult = RE::BSEventNotifyControl;

	class EventHandler :
		public RE::BSTEventSink<RE::BooksRead::Event>,
		public RE::BSTEventSink<RE::TESEquipEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESActivateEvent>,
		public RE::BSTEventSink<RE::TESDeathEvent>,
		public RE::BSTEventSink<RE::TESHitEvent>,
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent> {

	private:
		EventHandler() = default;
		~EventHandler() = default; 

	public:
		static EventHandler* GetSingleton() { static EventHandler singleton; return &singleton; }

		static void Register();
		static uintptr_t RegisterMapMarkerAddedHook(void (*f)(RE::TESFullName* a_form));
		static uintptr_t RegisterMapMarkerDiscoveredHook(const char* (*f)(RE::TESFullName* a_form));
		static uintptr_t RegisterUpdateCrosshairHook(void (*f)(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data));
		static uintptr_t RegisterUpdateInventoryNameHook(const char* (*f)(RE::InventoryEntryData* a_this));
		static void RegisterUpdateCraftingMenuHook();

		static void RegisterForEvent_OnBooksReadEvent(void (*f)(FuncParam_Book));
		static void RegisterForEvent_OnMenuOpenCloseEvent(void (*f)(FuncParam_Menu));
		static void RegisterForEvent_OnContainerChangedEvent(void (*f)(FuncParam_Cont));
		static void RegisterForEvent_OnEquipEvent(void (*f)(FuncParam_Equp));
		static void RegisterForEvent_OnActivateEvent(void (*f)(FuncParam_Acti));
		static void RegisterForEvent_OnDeathEvent(void (*f)(FuncParam_Deth));
		static void RegisterForEvent_OnHitEvent(void (*f)(FuncParam_oHit));
		static void RegisterForEvent_OnStageTriggeredEvent(void (*f)(FuncParam_Stag));

		EventResult	ProcessEvent(FuncParam_Book, RE::BSTEventSource<RE::BooksRead::Event>*) override;
		EventResult	ProcessEvent(FuncParam_Menu, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Cont, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Equp, RE::BSTEventSource<RE::TESEquipEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Acti, RE::BSTEventSource<RE::TESActivateEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Deth, RE::BSTEventSource<RE::TESDeathEvent>*) override;
		EventResult	ProcessEvent(FuncParam_oHit, RE::BSTEventSource<RE::TESHitEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Stag, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

		static uintptr_t RegisterEnchantmentHook(const char* (*f)(RE::TESForm* a_form), bool a_yesImSureCompat);

		EventHandler(EventHandler const&) = delete;
		EventHandler(EventHandler const&&) = delete;
		EventHandler operator=(EventHandler&) = delete;
		EventHandler operator=(EventHandler&&) = delete;
	};
};