#pragma once

#define GET_LOC_STRING_BY_KEY(a_key)		CLocalisation::LocalisationAPI::GetSingleton()->GetLocStringByKey(a_key)
#define GET_LOC_NAME_BY_KEY(a_key)			CLocalisation::LocalisationAPI::GetSingleton()->GetLocNameByKey(a_key)
#define GET_LOC_DESCRIPTION_BY_KEY(a_key)	CLocalisation::LocalisationAPI::GetSingleton()->GetLocDescriptionByKey(a_key)

namespace CLocalisation
{
	class LocalisationAPI final 
	{
	public:
		 static void Register();
		 static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		 static LocalisationAPI* GetSingleton() { static LocalisationAPI singleton; return &singleton; }

		 static void BuildLocalisedMaps();

		 static const char* GetLocStringByKey(const char* s_key);
		 static std::string GetLocStringByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		 static const char* GetLocNameByKey(const char* s_key);
		 static std::string GetLocNameByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		 static const char* GetLocDescriptionByKey(const char* s_key);
		 static std::string GetLocDescriptionByKeyExt(RE::StaticFunctionTag*, std::string s_key);

		 static int32_t GetStringLength(RE::StaticFunctionTag*, RE::BSFixedString);

		 static bool IsMatchingShout(RE::StaticFunctionTag*, std::string s_key, std::string s_key2);

		 LocalisationAPI(LocalisationAPI const&) = delete;
		 LocalisationAPI(LocalisationAPI const&&) = delete;
		 LocalisationAPI operator=(LocalisationAPI&) = delete;
		 LocalisationAPI operator=(LocalisationAPI&&) = delete;

	private:
		LocalisationAPI() = default;
		~LocalisationAPI() = default;

		static inline std::unordered_map<std::string, std::string> localisedMap_M;
		static inline std::unordered_map<std::string, std::string> localisedMap_N;
		static inline std::unordered_map<std::string, std::string> localisedMap_D;

		// Helper function to read and populate a map
		static void ReadAndPopulateMap(const std::string& filePath, std::unordered_map<std::string, std::string>& targetMap);
	};
}
