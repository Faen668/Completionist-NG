#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_ICOW
{
	inline Serialization::CompletionistData ItemData;
	inline Serialization::CompletionistData BookData;
	inline Serialization::CompletionistData SkilData;
	inline Serialization::CompletionistData TomeData;

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

	inline std::vector<std::string> Tomes_NameArray;
	inline std::vector<std::string> Tomes_TextArray;
	inline std::vector<RE::TESForm*> Tomes_FormArray;
	inline std::vector<bool> Tomes_BoolArray;
	inline std::int32_t Tomes_EntriesTotal;
	inline std::int32_t Tomes_EntriesFound;

	inline std::vector<std::string> Skill_NameArray;
	inline std::vector<std::string> Skill_TextArray;
	inline std::vector<RE::TESForm*> Skill_FormArray;
	inline std::vector<bool> Skill_BoolArray;
	inline std::int32_t Skill_EntriesTotal;
	inline std::int32_t Skill_EntriesFound;

	class CHandler
	{
	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InjectAndCompileData();
		static void			InstallSearchTerms();
		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);

		static void			OnBooksReadEvent(RE::BooksRead::Event const* a_event);
		static void			OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}