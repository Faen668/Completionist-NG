#pragma once

#include <SKSE\API.h>

namespace BeggarQuests
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESActivateEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		EventResult	ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

		static void EvaluateActivatedReference(RE::Actor* a_actor);

		static void	Register() {

			auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));
			ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESActivateEvent>*>(CHandler::GetSingleton()));
		};
	};
}
