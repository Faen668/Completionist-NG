#pragma once

namespace Completionist_MainHUD
{
	inline std::vector<std::string> garbageDump{};

	using EventResult = RE::BSEventNotifyControl;
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	class TextnTagsAPI final :

		public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
		public: [[nodiscard]] static TextnTagsAPI* GetSingleton() { static TextnTagsAPI singleton; return &singleton; }

	public:

		EventResult	ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		static void Register();

		static void RegisterMessageListener();

		static void MessageHandler(SKSE::MessagingInterface::Message* a_msg);

		static void	ProcessCrosshairReference(RE::HUDData* data);

		static bool ItemIsCollectable(RE::FormID a_formID);
		static bool ItemIsCollectable(RE::TESForm* a_form);

		static bool ItemIsCollected(RE::FormID a_formID);
		static bool ItemIsCollected(RE::TESForm* a_form);
		
		static void OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data);
		static const char* OnUpdateInventoryText(RE::InventoryEntryData* a_this);
		static const char* OnUpdateInventoryName(const char* a_this, bool a_displayTag);

	private:
		static inline REL::Relocation<decltype(OnUpdateCrosshairText)> _OnUpdateCrosshairText;
		static inline REL::Relocation<decltype(OnUpdateInventoryText)> _OnUpdateInventoryText;
	};
}
