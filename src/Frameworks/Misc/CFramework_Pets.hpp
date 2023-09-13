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

namespace CFramework_Pets_WH {
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

	inline std::vector<std::string>		Pets_WH_NameArray;
	inline std::vector<std::string>		Pets_WH_TextArray;
	inline std::vector<RE::TESForm*>	Pets_WH_FormArray;
	inline std::vector<bool>			Pets_WH_BoolArray;
	inline std::int32_t					Pets_WH_EntriesTotal;
	inline std::int32_t					Pets_WH_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESQuestStageEvent> 
	{
	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();
		static void			Framework_UpdatePetOwnership(RE::StaticFunctionTag*, std::string a_name);
	};
}