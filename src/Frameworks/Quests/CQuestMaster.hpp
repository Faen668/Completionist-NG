#pragma once
#include "Structs.hpp"
#include "Serialization.hpp"

constexpr auto CArraySize = 128;

namespace CQuestMaster
{
	inline std::vector<std::tuple<CQuestData*, std::string, int32_t, std::string>> CQuestDataVec;

	using EventResult = RE::BSEventNotifyControl;

	class QuestAPI final :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESQuestStageEvent>
	{

	public: [[nodiscard]] static QuestAPI* GetSingleton() { static QuestAPI singleton; return &singleton; }

		  EventResult							ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		  EventResult							ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;

		  static void							SinkEvents();
		  static void							Register();


		  static bool							IsQuestCompleted(CQuestData* a_data);
		  static bool							IsStageDoneOrPast(CQuestData* a_data);
		  static bool							IsStageDone(RE::TESQuest* a_quest, int32_t a_stage);
		  static bool							IsFavorCompleted(CQuestData* a_data);
		  static bool							IsThaneOfHold(CQuestData* a_data);
		  static bool							IsQuestCompletedNaturally(CQuestData* a_data);
		  static void							CheckForRadiantQuestCompletion(std::string a_key);
		  static void							CheckForCompletionFromStageEvent(CQuestData* a_data, int32_t a_stage);

		  static bool							qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_editorid);
		  static std::int32_t					qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);
		  static void							qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key, bool b_completed);
		  static void							ValidateLocalisation();

		  static std::vector<std::string>		SearchAndReportPage(RE::StaticFunctionTag*, std::string s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t searchType);
		  static std::string                    GetLocalisedPageName(int32_t ID);
		  static CQuestData*					GetQuestDataByKey(std::string a_key);
		  static void							AddQuestData(CQuestData* a_data, std::string a_name, int32_t a_ID, std::string a_key);


		  static std::pair<int32_t, int32_t>	qGetQuestCompletionTotals(std::vector<CQuestData*> data);
	};
};
