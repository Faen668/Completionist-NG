#pragma once

namespace CQFramework_Beggars
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESActivateEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		EventResult	ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

		static void	InstallFramework();
		static void EvaluateActivatedReference(RE::Actor* a_actor);
		static void AddBeggarData(RE::FormID NPCReferenceFormID, const std::string& fileName, const std::string& Quest_Key);
	};
}
