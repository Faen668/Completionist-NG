#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_INN
{
	inline Serialization::CompletionistData ItemData;

	inline std::vector<std::string> Items_NameArray;
	inline std::vector<std::string> Items_TextArray;
	inline std::vector<RE::TESForm*> Items_FormArray;
	inline std::vector<bool> Items_BoolArray;
	inline std::int32_t Items_EntriesTotal;
	inline std::int32_t Items_EntriesFound;

	class CHandler
	{
	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();
	static void			InstallSearchTerms();

	static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
	static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}