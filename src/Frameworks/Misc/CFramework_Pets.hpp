#pragma once
#include "Serialization.hpp"

namespace CFramework_Pets_VP {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Pets
{
	inline std::vector<std::string>		Pets_VP_NameArray;
	inline std::vector<std::string>		Pets_VP_TextArray;
	inline std::vector<RE::TESForm*>	Pets_VP_FormArray;
	inline std::vector<bool>			Pets_VP_BoolArray;
	inline std::int32_t					Pets_VP_EntriesTotal;
	inline std::int32_t					Pets_VP_EntriesFound;

	class CHandler
	{
	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();
		static void			Framework_UpdatePetOwnership(RE::StaticFunctionTag*, RE::TESActorBase* a_actorBase);
	};
}