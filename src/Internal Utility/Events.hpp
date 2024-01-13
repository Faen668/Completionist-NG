#pragma once

namespace CEvents
{
	using EventResult = RE::BSEventNotifyControl;
	#define FuncParam_Book RE::BooksRead::Event const* a_event
	#define FuncParam_Menu RE::MenuOpenCloseEvent const* a_event
	#define FuncParam_Cont RE::TESContainerChangedEvent const* a_event
	#define FuncParam_Equp RE::TESEquipEvent const* a_event
	#define FuncParam_Acti RE::TESActivateEvent const* a_event

	class EventHandler :
		public RE::BSTEventSink<RE::BooksRead::Event>,
		public RE::BSTEventSink<RE::TESEquipEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESActivateEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent>{

		public: [[nodiscard]] static EventHandler* GetSingleton() { static EventHandler singleton; return &singleton; }

	public:
		static void RegisterEvents();
		static void RegisterForEvent_OnBooksReadEvent(void (*f)(FuncParam_Book));
		static void RegisterForEvent_OnMenuOpenCloseEvent(void (*f)(FuncParam_Menu));
		static void RegisterForEvent_OnContainerChangedEvent(void (*f)(FuncParam_Cont));
		static void RegisterForEvent_OnEquipEvent(void (*f)(FuncParam_Equp));
		static void RegisterForEvent_OnActivateEvent(void (*f)(FuncParam_Acti));

		EventResult	ProcessEvent(FuncParam_Book, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>*) override;
		EventResult	ProcessEvent(FuncParam_Menu, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Cont, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Equp, [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>*) override;
		EventResult	ProcessEvent(FuncParam_Acti, [[maybe_unused]] RE::BSTEventSource<RE::TESActivateEvent>*) override;
	};
}