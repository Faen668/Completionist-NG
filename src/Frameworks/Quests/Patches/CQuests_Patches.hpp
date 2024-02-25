#pragma once
#include "Structs.hpp"

namespace CQFramework_Patches
{
	class CHandler
	{
	public:
		static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }
		static void InstallFramework();
		static void InstallLegacyoftheDragonBornQuests();

		/*template<size_t N>
		static void CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, const char* a_mod, const std::string& a_name);

		template<size_t N, size_t S>
		static void CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, CStageData(&a_stage)[S], const std::string& a_name);

		template<size_t N, size_t S>
		static void CreateINIFile(CQuestData(&a_data)[N], const char* a_page, int32_t i_id, CRadiantData(&a_stage)[S], const char* a_mod, const std::string& a_name);*/	
	};
}
