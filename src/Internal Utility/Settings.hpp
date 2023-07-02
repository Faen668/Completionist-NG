#include "Simpleini.h"

#pragma once

namespace Settings
{
	class Main
	{
		public: [[nodiscard]] static Main* GetSingleton() { static Main singleton; return &singleton; }

		static void Register();
		static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		static bool IsValid(RE::StaticFunctionTag*);

		static void SaveBool(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, bool value);
		static void SaveInt(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, int32_t value);
		static void SaveFloat(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, float value);
		static void SaveStr(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value);

		static bool LoadBool(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value);
		static int32_t LoadInt(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value);
		static float LoadFloat(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value);
		static RE::BSFixedString LoadStr(RE::StaticFunctionTag*, RE::BSFixedString section, RE::BSFixedString key, RE::BSFixedString value);
	};
}
