#pragma once

namespace CQFramework_CW
{	
	using EventResult = RE::BSEventNotifyControl;
	class CHandler : public RE::BSTEventSink<RE::TESQuestStageEvent>
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult	ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;		
		
		static RE::BGSLocation* GetAliasLocation(RE::TESQuest* a_quest, uint32_t alias_id);
		static void InstallFramework();
	};
}
