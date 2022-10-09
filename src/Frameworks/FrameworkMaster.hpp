#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"

namespace CFramework_Master
{
	extern Serialization::CompletionistKey CQuestKeys_Natural;
	extern Serialization::CompletionistKey CQuestKeys_Manual;

	extern Serialization::CompletionistData FoundItemData;
	extern Serialization::CompletionistData FoundItemData_NoShow;
	extern int PatchesInstalled;

	class FrameworkAPI {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void							Register();
		static bool							RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		static void							Framework_Load();
		
		static std::vector<std::string>		qGetNameArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetTextArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetIdenArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetKeysArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<bool>			qGetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<int32_t>			qGetRadiArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);

		static bool							qIsOptionToggledInternal(std::string a_key);
		static bool							qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);

		static std::int32_t					qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);
		static void							qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);

		static std::vector<RE::TESForm*>	GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<bool>			GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, RE::TESForm* a_form);
		static void							SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, RE::TESForm* a_form);

		static bool							CCItemsInstalled(RE::StaticFunctionTag*);
		static bool							CCBooksInstalled(RE::StaticFunctionTag*);
		static bool							CCLocationsInstalled(RE::StaticFunctionTag*);
		static bool							ShouldDisplayMiscHeader(RE::StaticFunctionTag*);

		static void							SendNotification(std::string a_msg, std::string a_setting);
		static void							SendNotificationExt(RE::StaticFunctionTag*, std::string Message, std::string Colour, bool ColourEnabled);
		
		static std::string					ReplaceStr(std::string const& in, std::string const& from, std::string const& to);
		static std::string					GetHexValue(RE::StaticFunctionTag*, std::uint32_t IntVal);
		static std::string					GetVersion(RE::StaticFunctionTag*);

		static RE::TESQuest*				GetQuest(std::string a_questID);
		static bool							IsCompleted_P(std::string a_key, std::string a_questID, std::int32_t a_stage); //GetStage > Check
		static bool							IsCompleted_S(std::string a_key, std::string a_questID, std::uint16_t a_stage); //IsStageDone Check
		static bool							IsCompleted_N(std::string a_key, std::string a_questID); // Normal Completion Check
		static bool							IsCompleted_G(std::string a_key, std::string a_questID, std::string a_globalID, int32_t a_value = 1); // GLobal Check
		static bool							IsCompleted_J(std::string a_key, std::string a_imp, std::string a_son); // Thane Check
		
	};
}
