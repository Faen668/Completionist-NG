#pragma once
#include "Serialization.hpp"

namespace CFramework_PlayerHomes_VH {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_PlayerHomes_CH {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_PlayerHomes_PH {
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

	inline std::vector<std::string>		CH_NameArray;
	inline std::vector<std::string>		CH_TextArray;
	inline std::vector<RE::TESForm*>	CH_FormArray;
	inline std::vector<std::string>		CH_QuestArray;
	inline std::vector<bool>			CH_BoolArray;
	inline std::int32_t					CH_EntriesTotal;
	inline std::int32_t					CH_EntriesFound;

	inline std::vector<std::string>		PH_NameArray;
	inline std::vector<std::string>		PH_TextArray;
	inline std::vector<RE::TESForm*>	PH_FormArray;
	inline std::vector<std::string>		PH_QuestArray;
	inline std::vector<bool>			PH_BoolArray;
	inline std::int32_t					PH_EntriesTotal;
	inline std::int32_t					PH_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESContainerChangedEvent>

	{
	public:
		[[nodiscard]] static CHandler* GetSingleton()
		{
			static CHandler singleton;
			return &singleton;
		}

		static void RegisterEvents() {
			register_event<RE::TESQuestStageEvent>();
			register_event<RE::TESContainerChangedEvent>();
		}

		EventResult			ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
		EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

		static void			InstallFramework();
		static void			InjectAndCompileData();
		static void			ProcessFoundForm(std::string a_editorID);
		static void			UpdateFoundForms();

		static void			Compile_VH();
		static void			Compile_CH();
		static void			Compile_PH();

	private:

		CHandler() = default;
		CHandler(const CHandler&) = delete;
		CHandler(CHandler&&) = delete;

		~CHandler() override = default;

		CHandler& operator=(const CHandler&) = delete;
		CHandler& operator=(CHandler&&) = delete;

		template <class T>
		static void register_event()
		{
			if (const auto scripts = RE::ScriptEventSourceHolder::GetSingleton(); scripts) {
				scripts->AddEventSink<T>(GetSingleton());
			}
		}
	};
}