#pragma once
#include "Structs.hpp"
#include "Serialization.hpp"

constexpr auto CArraySize = 128;

namespace CQuestMaster
{
	inline std::vector<std::tuple<CQuestData*, std::string, int32_t>> CQuestDataVec;

	using EventResult = RE::BSEventNotifyControl;

	class QuestAPI final : 
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent>
	{

	public: [[nodiscard]] static QuestAPI* GetSingleton() { static QuestAPI singleton; return &singleton; }

		  EventResult							ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		  EventResult							ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  EventResult							ProcessEvent(RE::TESContainerChangedEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

		  static bool							RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);
		  static void							SinkEvents();
		  static void							Register();

		  static void							UpdateQuestCompletion();
		  static bool							IsQuestCompleted(CQuestData* a_data);

		  static void							ProcessDrunkardQuest(CDrunkData* a_data, RE::FormID a_base, RE::FormID a_container, RE::TESObjectREFR* cur_speaker);

		  static std::vector<std::string>		qGetNameArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		  static std::vector<std::string>		qGetTextArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		  static std::vector<std::string>		qGetIdenArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		  static std::vector<bool>				qGetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		  static std::vector<int32_t>			qGetRadiArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);

		  static bool							IsStageDoneOrPast(CQuestData* a_data);
		  static bool							IsStageDone(RE::TESQuest* a_quest, int32_t a_stage);

		  static bool							qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_editorid);
		  static std::int32_t					qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);
		  static void							qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);

		  static void							Dump();
		  static void							qDumpQuestData(RE::StaticFunctionTag*);
	};
}
