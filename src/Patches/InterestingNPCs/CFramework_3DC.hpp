#pragma once

namespace CPatch_3DC_Items {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_3DC_Books {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_3DC_MapMa {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_3DC 
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

	inline std::vector<std::string>		Quest1_NameArray;
	inline std::vector<std::string>		Quest1_IdenArray;
	inline std::vector<std::string>		Quest1_TextArray;
	inline std::vector<std::int32_t>	Quest1_RadiArray;
	inline std::vector<bool>			Quest1_BoolArray;

	inline std::vector<std::string>		Quest2_NameArray;
	inline std::vector<std::string>		Quest2_IdenArray;
	inline std::vector<std::string>		Quest2_TextArray;
	inline std::vector<std::int32_t>	Quest2_RadiArray;
	inline std::vector<bool>			Quest2_BoolArray;

	inline std::vector<std::string>		Quest3_NameArray;
	inline std::vector<std::string>		Quest3_IdenArray;
	inline std::vector<std::string>		Quest3_TextArray;
	inline std::vector<std::int32_t>	Quest3_RadiArray;
	inline std::vector<bool>			Quest3_BoolArray;

	inline std::vector<std::string>		Quest4_NameArray;
	inline std::vector<std::string>		Quest4_IdenArray;
	inline std::vector<std::string>		Quest4_TextArray;
	inline std::vector<std::int32_t>	Quest4_RadiArray;
	inline std::vector<bool>			Quest4_BoolArray;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::TESContainerChangedEvent>,
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public RE::BSTEventSink<RE::BooksRead::Event>
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
	EventResult			ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;
	EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InjectAndCompileData();
	static void			InstallSearchTerms();

	static void			InstallFramework();
	static void			InstallQuestFramework();
	static void			UpdateFoundForms();

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	static void			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos);
	};
}