#pragma once

namespace CPatch_WOL_Items {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_WOL_Books {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_WOL_MapMa {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_WOL 
{	

	inline std::vector<std::string> Items_NameArray;
	inline std::vector<std::string> Items_TextArray;
	inline std::vector<RE::TESForm*> Items_FormArray;
	inline std::vector<bool> Items_BoolArray;
	inline std::int32_t Items_EntriesTotal;
	inline std::int32_t Items_EntriesFound;

	inline std::vector<std::string> Books_NameArray;
	inline std::vector<std::string> Books_TextArray;
	inline std::vector<RE::TESForm*> Books_FormArray;
	inline std::vector<bool> Books_BoolArray;
	inline std::int32_t Books_EntriesTotal;
	inline std::int32_t Books_EntriesFound;

	inline std::vector<std::string> MapMa_NameArray;
	inline std::vector<std::string> MapMa_TextArray;
	inline std::vector<RE::TESForm*> MapMa_FormArray;
	inline std::vector<bool> MapMa_BoolArray;
	inline std::int32_t MapMa_EntriesTotal;
	inline std::int32_t MapMa_EntriesFound;

	inline std::vector<std::string>		Quest_NameArray;
	inline std::vector<std::string>		Quest_IdenArray;
	inline std::vector<std::string>		Quest_TextArray;
	inline std::vector<std::string>		Quest_KeysArray;
	inline std::vector<std::int32_t>	Quest_RadiArray;
	inline std::vector<bool>			Quest_BoolArray;
	inline std::vector<bool>			Quest_StgeArray;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::BooksRead::Event>,
		public RE::BSTEventSink<RE::TESQuestStageEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		EventResult			ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;
		EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
		EventResult			ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;

		static void			SinkEvents();
		static void			InjectAndCompileData();

		static void			InstallFramework();
		static void			InstallQuestFramework();

		static void			UpdateFoundForms();
		static void			UpdateQuestFramework();

		static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
		static void			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos);
	};
}