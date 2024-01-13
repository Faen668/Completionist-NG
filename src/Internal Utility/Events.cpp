#include "Events.hpp"
#include "Internal Utility/PatchListener.hpp"

namespace CEvents 
{
	std::vector<std::function<void(FuncParam_Book)>> _OnBooksRead;
	std::vector<std::function<void(FuncParam_Equp)>> _OnEquipEvent;
	std::vector<std::function<void(FuncParam_Acti)>> _OnActivateEvent;
	std::vector<std::function<void(FuncParam_Menu)>> _OnMenuOpenCloseEvent;
	std::vector<std::function<void(FuncParam_Cont)>> _OnTESContainerChangedEvent;

	//---------------------------------------------------
	//-- Event Distributor (Register Events ) -----------
	//---------------------------------------------------

	void EventHandler::RegisterEvents() 
	{
		RE::BooksRead::GetEventSource()->AddEventSink(EventHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(EventHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(EventHandler::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESEquipEvent>*>(EventHandler::GetSingleton()));
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(GetSingleton()));
	};

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
	//-- Event Distributor ( On Menu Open / Close ) -----
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Menu, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnMenuOpenCloseEvent) {
			fn(a_event);
		};

		for (auto& cls : CExternalPatchHandler::CustomItemsPatches) {
			cls->OnMenuOpenCloseEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Activate ) --------------
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Acti, [[maybe_unused]] RE::BSTEventSource<RE::TESActivateEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnActivateEvent) {
			fn(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Contaienr Changed ) -----
	//---------------------------------------------------

	EventResult	EventHandler::ProcessEvent(FuncParam_Cont, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnTESContainerChangedEvent) {
			fn(a_event);
		};

		for (auto& cls : CExternalPatchHandler::CustomItemsPatches) {
			cls->OnContainerChangedEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( Books Read ) ---------------
	//---------------------------------------------------

	EventResult EventHandler::ProcessEvent(FuncParam_Book, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnBooksRead) {
			fn(a_event);
		};

		for (auto& cls : CExternalPatchHandler::CustomItemsPatches) {
			cls->OnBooksReadEvent(a_event);
		};
		return EventResult::kContinue;
	};

	//---------------------------------------------------
	//-- Event Distributor ( On Equip ) -----------------
	//---------------------------------------------------

	EventResult EventHandler::ProcessEvent(FuncParam_Equp, [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent>*)
	{
		if (!a_event) { return EventResult::kContinue; }

		for (auto& fn : _OnEquipEvent) {
			fn(a_event);
		};
		return EventResult::kContinue;
	};
}