#pragma once

#include <SKSE\API.h>

namespace BountyQuests
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;

		static bool IsValidQuest(RE::FormID questID, RE::FormID checkID);

		static void Register() {
			auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
		};
	};
}
