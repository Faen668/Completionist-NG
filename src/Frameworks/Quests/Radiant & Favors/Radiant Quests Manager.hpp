#pragma once

#include <SKSE\API.h>

namespace Quest_Manager
{
	class RadiantQuestManager {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void ProcessFormID(std::uint32_t _ID, std::string questID);
		static bool GetQuestHandle(RE::BSScript::IVirtualMachine* a_vm, std::string ScriptName, RE::VMHandle& handleOut);
	};

	void Install();
}
