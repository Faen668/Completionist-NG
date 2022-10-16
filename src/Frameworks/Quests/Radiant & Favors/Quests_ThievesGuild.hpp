#pragma once

#include <SKSE\API.h>

namespace ThievesGuildQuests
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final : 
		public RE::BSTEventSink<RE::TESQuestStageEvent> {
	
		public: [[nodiscard]] static ScriptEventHandler* GetSingleton() {
			static ScriptEventHandler singleton;
			return &singleton;
		}

		static void Register() {
			register_event<RE::TESQuestStageEvent>();
		}
		
		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

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
