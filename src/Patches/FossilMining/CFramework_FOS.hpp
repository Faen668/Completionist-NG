#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_FOS_ItmL1 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FOS_ItmL2 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FOS_ItmL3 {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FOS 
{	
	extern std::vector<std::string> ItmL1_NameArray;
	extern std::vector<std::string> ItmL1_TextArray;
	extern std::vector<RE::TESForm*> ItmL1_FormArray;
	extern std::vector<bool> ItmL1_BoolArray;
	extern std::int32_t ItmL1_EntriesTotal;
	extern std::int32_t ItmL1_EntriesFound;

	extern std::vector<std::string> ItmL2_NameArray;
	extern std::vector<std::string> ItmL2_TextArray;
	extern std::vector<RE::TESForm*> ItmL2_FormArray;
	extern std::vector<bool> ItmL2_BoolArray;
	extern std::int32_t ItmL2_EntriesTotal;
	extern std::int32_t ItmL2_EntriesFound;

	extern std::vector<std::string> ItmL3_NameArray;
	extern std::vector<std::string> ItmL3_TextArray;
	extern std::vector<RE::TESForm*> ItmL3_FormArray;
	extern std::vector<bool> ItmL3_BoolArray;
	extern std::int32_t ItmL3_EntriesTotal;
	extern std::int32_t ItmL3_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::TESContainerChangedEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();

	static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}