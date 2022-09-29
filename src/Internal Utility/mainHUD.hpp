#pragma once

namespace Completionist_MainHUD
{
	using EventResult = RE::BSEventNotifyControl;
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	class FunctionHolder final :

		public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
		public: [[nodiscard]] static FunctionHolder* GetSingleton() { static FunctionHolder singleton; return &singleton; }

	public:

		EventResult	ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		static void InstallHook();

		static void	ProcessCrosshairReference(RE::HUDData* data);

		static bool ItemIsCollectable(RE::FormID a_formID);
		static bool ItemIsCollected(RE::FormID a_formID);
		static bool ItemIsCollected(RE::TESForm* a_form);
		
		static void OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data);
		static const char* OnUpdateInventoryText(RE::InventoryEntryData* a_this);

	private:
		static inline REL::Relocation<decltype(OnUpdateCrosshairText)> _OnUpdateCrosshairText;
		static inline REL::Relocation<decltype(OnUpdateInventoryText)> _OnUpdateInventoryText;
	};
}
