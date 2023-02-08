#pragma once

#include "Serialization.hpp";

struct BountyQuestData
{
	RE::FormID VBQ_ID;
	const char* VBQ_FN;

	const std::array<RE::FormID, 9> FQS_ID;
	const char* FQS_FN;

	const char* Global;
};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace BountyProcessor
{
	using EventResult = RE::BSEventNotifyControl;

	class CHandler : public
		RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  static void Sink();

		  static void ProcessQuest(const RE::FormID a_quest, const std::uint16_t a_stage);

	};
}
