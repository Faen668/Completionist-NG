#pragma once

namespace CPatch_Cloaks_Items {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_CloaksDP_Items {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_Cloaks
{
	inline std::vector<std::string> Items_NameArray;
	inline std::vector<std::string> Items_TextArray;
	inline std::vector<RE::TESForm*> Items_FormArray;
	inline std::vector<bool> Items_BoolArray;
	inline std::int32_t Items_EntriesTotal;
	inline std::int32_t Items_EntriesFound;

	inline std::vector<std::string> ItemsDP_NameArray;
	inline std::vector<std::string> ItemsDP_TextArray;
	inline std::vector<RE::TESForm*> ItemsDP_FormArray;
	inline std::vector<bool> ItemsDP_BoolArray;
	inline std::int32_t ItemsDP_EntriesFound;
	inline std::int32_t ItemsDP_EntriesTotal;

	enum Section
	{
		k1 = 0,
		k2 = 1,
	};

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

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, Section k_value);
	};
}