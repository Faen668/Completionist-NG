#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_WYR
{
	inline Serialization::CompletionistData ItemData;
	inline Serialization::CompletionistData BookData;
	inline Serialization::CompletionistData MapsData;

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

	class CHandler
	{
		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InjectAndCompileData();
		static void			InstallSearchTerms();

		static void			InstallFramework();
		static void			InstallQuestFramework();
		static void			UpdateFoundForms();

		static void			ProcessHookedMarker(const char* nam);
		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
		static void			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, bool from_hook);

		static void			OnBooksReadEvent(RE::BooksRead::Event const* a_event);
		static void			OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}