#pragma once
#include "Serialization.hpp"

namespace CFramework_Pets_VP {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Pets_PS {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Pets_SS {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Pets_MP {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Pets
{
	extern std::vector<std::string>		Pets_VP_NameArray;
	extern std::vector<std::string>		Pets_VP_TextArray;
	extern std::vector<RE::TESForm*>	Pets_VP_FormArray;
	extern std::vector<bool>			Pets_VP_BoolArray;
	extern std::int32_t					Pets_VP_EntriesTotal;
	extern std::int32_t					Pets_VP_EntriesFound;

	extern std::vector<std::string>		Pets_PS_NameArray;
	extern std::vector<std::string>		Pets_PS_TextArray;
	extern std::vector<RE::TESForm*>	Pets_PS_FormArray;
	extern std::vector<bool>			Pets_PS_BoolArray;
	extern std::int32_t					Pets_PS_EntriesTotal;
	extern std::int32_t					Pets_PS_EntriesFound;

	extern std::vector<std::string>		Pets_SS_NameArray;
	extern std::vector<std::string>		Pets_SS_TextArray;
	extern std::vector<RE::TESForm*>	Pets_SS_FormArray;
	extern std::vector<bool>			Pets_SS_BoolArray;
	extern std::int32_t					Pets_SS_EntriesTotal;
	extern std::int32_t					Pets_SS_EntriesFound;

	extern std::vector<std::string>		Pets_MP_NameArray;
	extern std::vector<std::string>		Pets_MP_TextArray;
	extern std::vector<RE::TESForm*>	Pets_MP_FormArray;
	extern std::vector<bool>			Pets_MP_BoolArray;
	extern std::int32_t					Pets_MP_EntriesTotal;
	extern std::int32_t					Pets_MP_EntriesFound;

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