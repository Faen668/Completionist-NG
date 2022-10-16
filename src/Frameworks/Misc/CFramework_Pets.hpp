#pragma once
#include "Serialization.hpp"

namespace CFramework_Pets_VP {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Pets_PS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Pets_SS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Pets_MP {
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

	inline std::vector<std::string>		Pets_PS_NameArray;
	inline std::vector<std::string>		Pets_PS_TextArray;
	inline std::vector<RE::TESForm*>	Pets_PS_FormArray;
	inline std::vector<bool>			Pets_PS_BoolArray;
	inline std::int32_t					Pets_PS_EntriesTotal;
	inline std::int32_t					Pets_PS_EntriesFound;

	inline std::vector<std::string>		Pets_SS_NameArray;
	inline std::vector<std::string>		Pets_SS_TextArray;
	inline std::vector<RE::TESForm*>	Pets_SS_FormArray;
	inline std::vector<bool>			Pets_SS_BoolArray;
	inline std::int32_t					Pets_SS_EntriesTotal;
	inline std::int32_t					Pets_SS_EntriesFound;

	inline std::vector<std::string>		Pets_MP_NameArray;
	inline std::vector<std::string>		Pets_MP_TextArray;
	inline std::vector<RE::TESForm*>	Pets_MP_FormArray;
	inline std::vector<bool>			Pets_MP_BoolArray;
	inline std::int32_t					Pets_MP_EntriesTotal;
	inline std::int32_t					Pets_MP_EntriesFound;

	inline bool PetsOfSkyrim_Installed = false;
	inline bool SaintSeducer_Installed = false;

	inline bool Petcrab_Installed = false;
	inline bool PetWolf_Installed = false;
	inline bool PetHund_Installed = false;

	inline bool Miscellaneous_Installed = false;

	class CHandler {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();

		static void			Framework_UpdatePetOwnership(RE::StaticFunctionTag*, std::string a_name);
	};
}