#pragma once

namespace CPatch_REQ_ItmL1_A { // Armor
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ_ItmL1_M { // Misc
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ_ItmL2 { // Weapons
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ_ItmL3_S { // Skill Books
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ_ItmL3_B { // General Books
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ_ItmL4 { // Spell Tomes
	inline Serialization::CompletionistData Data;
}

namespace CPatch_REQ
{
	enum section
	{
		k_ItmL1A,
		k_ItmL1M,
		k_ItmL2,
		k_ItmL3S,
		k_ItmL3B,
		k_ItmL4,
	};

	// Armor
	inline std::vector<std::string> ItmL1_A_NameArray;
	inline std::vector<std::string> ItmL1_A_TextArray;
	inline std::vector<RE::TESForm*> ItmL1_A_FormArray;
	inline std::vector<bool> ItmL1_A_BoolArray;
	inline std::int32_t ItmL1_A_EntriesTotal;
	inline std::int32_t ItmL1_A_EntriesFound;

	// Misc
	inline std::vector<std::string> ItmL1_M_NameArray;
	inline std::vector<std::string> ItmL1_M_TextArray;
	inline std::vector<RE::TESForm*> ItmL1_M_FormArray;
	inline std::vector<bool> ItmL1_M_BoolArray;
	inline std::int32_t ItmL1_M_EntriesTotal;
	inline std::int32_t ItmL1_M_EntriesFound;

	// Weapons
	inline std::vector<std::string> ItmL2_NameArray;
	inline std::vector<std::string> ItmL2_TextArray;
	inline std::vector<RE::TESForm*> ItmL2_FormArray;
	inline std::vector<bool> ItmL2_BoolArray;
	inline std::int32_t ItmL2_EntriesTotal;
	inline std::int32_t ItmL2_EntriesFound;

	// Skill Books
	inline std::vector<std::string> ItmL3_S_NameArray;
	inline std::vector<std::string> ItmL3_S_TextArray;
	inline std::vector<RE::TESForm*> ItmL3_S_FormArray;
	inline std::vector<bool> ItmL3_S_BoolArray;
	inline std::int32_t ItmL3_S_EntriesTotal;
	inline std::int32_t ItmL3_S_EntriesFound;

	// General Books
	inline std::vector<std::string> ItmL3_B_NameArray;
	inline std::vector<std::string> ItmL3_B_TextArray;
	inline std::vector<RE::TESForm*> ItmL3_B_FormArray;
	inline std::vector<bool> ItmL3_B_BoolArray;
	inline std::int32_t ItmL3_B_EntriesTotal;
	inline std::int32_t ItmL3_B_EntriesFound;

	// Spell Tomes
	inline std::vector<std::string> ItmL4_NameArray;
	inline std::vector<std::string> ItmL4_TextArray;
	inline std::vector<RE::TESForm*> ItmL4_FormArray;
	inline std::vector<bool> ItmL4_BoolArray;
	inline std::int32_t ItmL4_EntriesTotal;
	inline std::int32_t ItmL4_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::BooksRead::Event>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
	EventResult			ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();
	static void			InstallSearchTerms();

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, section kSection);
	};
}