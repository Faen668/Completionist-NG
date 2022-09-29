#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"

namespace CPatch_Master
{
	extern int PatchesInstalled;

	class PatchAPI {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void							InstallPatches();
		static void							LoadallPatches();

		static bool							ShouldDisplayMiscHeader(RE::StaticFunctionTag*);

		static std::vector<RE::TESForm*>	GetFormArrayByID(std::int32_t a_patchID);
		static std::vector<bool>			GetBoolArrayByID(std::int32_t a_patchID);

		static std::vector<std::string>		GetStringValues(std::int32_t a_patchID, std::string a_section);
		static std::int32_t					GetCountValues(std::int32_t a_patchID, std::string a_section);

		static std::int32_t					IsOptionCompleted(std::int32_t a_patchID, std::string a_name);
		static void							SetOptionCompleted(std::int32_t a_patchID, std::string a_name);
	};
}
