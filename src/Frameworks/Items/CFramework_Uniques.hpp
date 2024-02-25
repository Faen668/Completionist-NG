#include "Serialization.hpp"

#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CFramework_Uniques
{
	inline Serialization::CompletionistData ItemDataA;
	inline Serialization::CompletionistData ItemDataW;
	inline Serialization::CompletionistData ItemDataI;
	inline Serialization::CompletionistData ItemDataJ;
	inline Serialization::CompletionistData ItemDataL;
	inline Serialization::CompletionistData ItemDataSUDS;
	inline Serialization::CompletionistData ItemDataQ;

	inline std::vector<std::string> A_NameArray;
	inline std::vector<std::string> A_TextArray;
	inline std::vector<RE::TESForm*> A_FormArray;
	inline std::vector<bool> A_BoolArray;
	inline std::int32_t A_EntriesTotal;
	inline std::int32_t A_EntriesFound;

	inline std::vector<std::string> W_NameArray;
	inline std::vector<std::string> W_TextArray;
	inline std::vector<RE::TESForm*> W_FormArray;
	inline std::vector<bool> W_BoolArray;
	inline std::int32_t W_EntriesTotal;
	inline std::int32_t W_EntriesFound;

	inline std::vector<std::string> I_NameArray;
	inline std::vector<std::string> I_TextArray;
	inline std::vector<RE::TESForm*> I_FormArray;
	inline std::vector<bool> I_BoolArray;
	inline std::int32_t I_EntriesTotal;
	inline std::int32_t I_EntriesFound;

	inline std::vector<std::string> J_NameArray;
	inline std::vector<std::string> J_TextArray;
	inline std::vector<RE::TESForm*> J_FormArray;
	inline std::vector<bool> J_BoolArray;
	inline std::int32_t J_EntriesTotal;
	inline std::int32_t J_EntriesFound;

	inline std::vector<std::string> L_NameArray;
	inline std::vector<std::string> L_TextArray;
	inline std::vector<RE::TESForm*> L_FormArray;
	inline std::vector<bool> L_BoolArray;
	inline std::int32_t L_EntriesTotal;
	inline std::int32_t L_EntriesFound;

	inline std::vector<std::string> SUDS_NameArray;
	inline std::vector<std::string> SUDS_TextArray;
	inline std::vector<RE::TESForm*> SUDS_FormArray;
	inline std::vector<bool> SUDS_BoolArray;
	inline std::int32_t SUDS_EntriesTotal;
	inline std::int32_t SUDS_EntriesFound;

	inline std::vector<std::string> Q_NameArray;
	inline std::vector<std::string> Q_TextArray;
	inline std::vector<RE::TESForm*> Q_FormArray;
	inline std::vector<bool> Q_BoolArray;
	inline std::int32_t Q_EntriesTotal;
	inline std::int32_t Q_EntriesFound;

	class CHandler
	{
		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();

		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
		static void			Install_QuestRewards();
		static void			Install_QuestRewards_Descriptions();
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}