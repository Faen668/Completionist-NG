#pragma once
#include "Serialization.hpp"
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CFramework_Others
{
	inline Serialization::CompletionistData ItemDataSB;

	inline std::vector<std::string> SB_NameArray;
	inline std::vector<std::string> SB_TextArray;
	inline std::vector<RE::TESForm*> SB_FormArray;
	inline std::vector<bool> SB_BoolArray;
	inline std::int32_t SB_EntriesTotal;
	inline std::int32_t SB_EntriesFound;

	class CHandler
	{
		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			BuildBaranziahArrays();
		static void			InstallSearchTerms();

		static void			ProcessFoundForm(RE::TESObjectREFR* a_object);
		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);

		static void			OnActivateEvent(RE::TESActivateEvent const* a_event);
	};
}