#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"

namespace CFramework_Master
{
	extern Serialization::CompletionistData FoundItemData;
	extern Serialization::CompletionistData FoundItemData_NoShow;

	class FrameworkAPI {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void							Register();

		static bool							RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		static void							Framework_Load();

		static std::vector<RE::TESForm*>	Framework_GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		Framework_GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		Framework_GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<bool>			Framework_GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);

		static std::int32_t					Framework_GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					Framework_GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);

		static std::int32_t					Framework_IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, std::string a_entryname);
		static void							Framework_SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, std::string a_entryname);

		static std::string					GetBookSkill(RE::ActorValue a_val);

		static bool							Framework_CCItemsInstalled(RE::StaticFunctionTag*);
		static bool							Framework_CCBooksInstalled(RE::StaticFunctionTag*);
		static bool							Framework_CCLocationsInstalled(RE::StaticFunctionTag*);
		static void							SendNotification(std::string a_msg, std::string a_setting);
	};
}
