#pragma once

#include <SKSE\API.h>

namespace MiscQuests
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final : 
		public RE::BSTEventSink<RE::TESQuestStageEvent>, 
		public RE::BSTEventSink<RE::TESQuestStartStopEvent> {
	
		public: [[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
			register_event<RE::TESQuestStartStopEvent>();
		}
		
		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
		EventResult ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*) override;

	private:
		ScriptEventHandler() = default;
		ScriptEventHandler(const ScriptEventHandler&) = delete;
		ScriptEventHandler(ScriptEventHandler&&) = delete;

		~ScriptEventHandler() override = default;

		ScriptEventHandler& operator=(const ScriptEventHandler&) = delete;
		ScriptEventHandler& operator=(ScriptEventHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}
