#pragma once
#include "Serialization.hpp"

namespace CFramework_PlayerHomes_VH {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_PlayerHomes
{
	inline std::vector<std::string>		VH_NameArray;
	inline std::vector<std::string>		VH_TextArray;
	inline std::vector<RE::TESForm*>	VH_FormArray;
	inline std::vector<std::string>		VH_QuestArray;
	inline std::vector<bool>			VH_BoolArray;
	inline std::int32_t					VH_EntriesTotal;
	inline std::int32_t					VH_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent>{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		  EventResult			ProcessEvent(RE::TESContainerChangedEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			InstallSearchTerms();

		  static void			UpdateFoundForms();
		  static void			InjectAndCompileData();
		  static void			ProcessFoundForm(std::string a_editorID);	  
	};
}