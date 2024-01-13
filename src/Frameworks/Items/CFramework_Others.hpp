#pragma once
#include "Serialization.hpp"
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CFramework_Others
{
	inline Serialization::CompletionistData ItemDataVC;
	inline Serialization::CompletionistData ItemDataPC;
	inline Serialization::CompletionistData ItemDataVM;
	inline Serialization::CompletionistData ItemDataPM;
	inline Serialization::CompletionistData ItemDataSB;

	inline std::vector<std::string> VC_NameArray;
	inline std::vector<std::string> VC_TextArray;
	inline std::vector<RE::TESForm*> VC_FormArray;
	inline std::vector<bool> VC_BoolArray;
	inline std::int32_t VC_EntriesTotal;
	inline std::int32_t VC_EntriesFound;

	inline std::vector<std::string> PC_NameArray;
	inline std::vector<std::string> PC_TextArray;
	inline std::vector<RE::TESForm*> PC_FormArray;
	inline std::vector<bool> PC_BoolArray;
	inline std::int32_t PC_EntriesTotal;
	inline std::int32_t PC_EntriesFound;

	inline std::vector<std::string> VM_NameArray;
	inline std::vector<std::string> VM_TextArray;
	inline std::vector<RE::TESForm*> VM_FormArray;
	inline std::vector<bool> VM_BoolArray;
	inline std::int32_t VM_EntriesTotal;
	inline std::int32_t VM_EntriesFound;

	inline std::vector<std::string> PM_NameArray;
	inline std::vector<std::string> PM_TextArray;
	inline std::vector<RE::TESForm*> PM_FormArray;
	inline std::vector<bool> PM_BoolArray;
	inline std::int32_t PM_EntriesTotal;
	inline std::int32_t PM_EntriesFound;

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
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}