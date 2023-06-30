#pragma once

#include "Serialization.hpp"

namespace CFramework_Enchantments_VA {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_VW {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_SA {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_SW {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments_NGA {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Enchantments {

	inline std::vector<std::string>		VA_NameArray;
	inline std::vector<std::string>		VA_TextArray;
	inline std::vector<RE::TESForm*>	VA_FormArray;
	inline std::vector<bool>			VA_BoolArray;
	inline std::int32_t					VA_EntriesTotal;
	inline std::int32_t					VA_EntriesFound;

	inline std::vector<std::string>		SA_NameArray;
	inline std::vector<std::string>		SA_TextArray;
	inline std::vector<RE::TESForm*>	SA_FormArray;
	inline std::vector<bool>			SA_BoolArray;
	inline std::int32_t					SA_EntriesTotal;
	inline std::int32_t					SA_EntriesFound;

	inline std::vector<std::string>		VW_NameArray;
	inline std::vector<std::string>		VW_TextArray;
	inline std::vector<RE::TESForm*>	VW_FormArray;
	inline std::vector<bool>			VW_BoolArray;
	inline std::int32_t					VW_EntriesTotal;
	inline std::int32_t					VW_EntriesFound;

	inline std::vector<std::string>		SW_NameArray;
	inline std::vector<std::string>		SW_TextArray;
	inline std::vector<RE::TESForm*>	SW_FormArray;
	inline std::vector<bool>			SW_BoolArray;
	inline std::int32_t					SW_EntriesTotal;
	inline std::int32_t					SW_EntriesFound;

	inline std::vector<std::string>		NGA_NameArray;
	inline std::vector<std::string>		NGA_TextArray;
	inline std::vector<RE::TESForm*>	NGA_FormArray;
	inline std::vector<bool>			NGA_BoolArray;
	inline std::int32_t					NGA_EntriesTotal;
	inline std::int32_t					NGA_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	 EventResult		ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			InstallSearchTerms();

	static void			UpdateFoundForms();
	static void			UpdateCounts();

	static void			InjectAndCompileData();
	
	};
}