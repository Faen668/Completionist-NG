#pragma once

#include "Serialization.hpp"

namespace CFramework_Enchantments_VA {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_VW {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_SA {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_SW {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments {

	extern std::vector<std::string>		VA_NameArray;
	extern std::vector<std::string>		VA_TextArray;
	extern std::vector<RE::TESForm*>	VA_FormArray;
	extern std::vector<bool>			VA_BoolArray;
	extern std::int32_t					VA_EntriesTotal;
	extern std::int32_t					VA_EntriesFound;

	extern std::vector<std::string>		SA_NameArray;
	extern std::vector<std::string>		SA_TextArray;
	extern std::vector<RE::TESForm*>	SA_FormArray;
	extern std::vector<bool>			SA_BoolArray;
	extern std::int32_t					SA_EntriesTotal;
	extern std::int32_t					SA_EntriesFound;

	extern std::vector<std::string>		VW_NameArray;
	extern std::vector<std::string>		VW_TextArray;
	extern std::vector<RE::TESForm*>	VW_FormArray;
	extern std::vector<bool>			VW_BoolArray;
	extern std::int32_t					VW_EntriesTotal;
	extern std::int32_t					VW_EntriesFound;

	extern std::vector<std::string>		SW_NameArray;
	extern std::vector<std::string>		SW_TextArray;
	extern std::vector<RE::TESForm*>	SW_FormArray;
	extern std::vector<bool>			SW_BoolArray;
	extern std::int32_t					SW_EntriesTotal;
	extern std::int32_t					SW_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	 EventResult		ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();

	static void			UpdateFoundForms();
	static void			UpdateCounts();

	static void			InjectAndCompileData();
	
	};
}