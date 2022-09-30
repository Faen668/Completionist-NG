#pragma once

namespace CInventoryEvents
{
	using EventResult = RE::BSEventNotifyControl;

	class InventoryAPI final : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static InventoryAPI* GetSingleton() { static InventoryAPI singleton; return &singleton; }

		  static void	Register();
		  EventResult	ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
	};
}

namespace Completionist_IconSetter{

	void ProcessEntry(RE::ItemList::Item* a_item);
	void ApplyTextModifications(std::string& a_name);
}