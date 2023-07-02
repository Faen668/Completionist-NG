#include "Simpleini.h"

#pragma once

namespace CLocalisation
{
	inline std::unordered_map<std::string, std::string> localisedMap_M;
	inline std::unordered_map<std::string, std::string> localisedMap_N;
	inline std::unordered_map<std::string, std::string> localisedMap_D;

	class LocalisationAPI {

	public: [[nodiscard]] static LocalisationAPI* GetSingleton() {
		static LocalisationAPI singleton;
		return &singleton;
	}
		  static void Register();
		  static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		  static void BuildLocalisedMaps();

		  static const char* GetLocStringByKey(const char* s_key);
		  static std::string GetLocStringByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		  static const char* GetLocNameByKey(const char* s_key);
		  static std::string GetLocNameByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		  static const char* GetLocDescriptionByKey(const char* s_key);
		  static std::string GetLocDescriptionByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		  static int32_t GetStringLength(RE::StaticFunctionTag*, RE::BSFixedString);

		  static bool IsMatchingShout(RE::StaticFunctionTag*, std::string, std::string);
	};
}
