#pragma once

namespace CPatch_ST1_ItmL1 {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_ST1_ItmL2 {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_ST1_ItmL3 {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_ST1_ItmL4 {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_ST1 
{	
	inline std::vector<std::string> ItmL1_NameArray;
	inline std::vector<std::string> ItmL1_TextArray;
	inline std::vector<RE::TESForm*> ItmL1_FormArray;
	inline std::vector<bool> ItmL1_BoolArray;
	inline std::int32_t ItmL1_EntriesTotal;
	inline std::int32_t ItmL1_EntriesFound;

	inline std::vector<std::string> ItmL2_NameArray;
	inline std::vector<std::string> ItmL2_TextArray;
	inline std::vector<RE::TESForm*> ItmL2_FormArray;
	inline std::vector<bool> ItmL2_BoolArray;
	inline std::int32_t ItmL2_EntriesTotal;
	inline std::int32_t ItmL2_EntriesFound;

	inline std::vector<std::string> ItmL3_NameArray;
	inline std::vector<std::string> ItmL3_TextArray;
	inline std::vector<RE::TESForm*> ItmL3_FormArray;
	inline std::vector<bool> ItmL3_BoolArray;
	inline std::int32_t ItmL3_EntriesTotal;
	inline std::int32_t ItmL3_EntriesFound;

	inline std::vector<std::string> ItmL4_NameArray;
	inline std::vector<std::string> ItmL4_TextArray;
	inline std::vector<RE::TESForm*> ItmL4_FormArray;
	inline std::vector<bool> ItmL4_BoolArray;
	inline std::int32_t ItmL4_EntriesTotal;
	inline std::int32_t ItmL4_EntriesFound;
	
	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESContainerChangedEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

		static void			SinkEvents();
		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();

		static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}