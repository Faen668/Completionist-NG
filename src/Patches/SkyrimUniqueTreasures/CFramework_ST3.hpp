#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_ST3
{
	inline Serialization::CompletionistData ItemData1;
	inline Serialization::CompletionistData ItemData2;
	inline Serialization::CompletionistData ItemData3;
	inline Serialization::CompletionistData ItemData4;

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