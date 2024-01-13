#pragma once

#include "Serialization.hpp"

namespace CFramework_MapMa_AG {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_HR {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_SZ {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_DG {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_DB {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_CC {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_MapMa 
{	
	inline std::vector<std::string>			MapMa_AG_NameArray;
	inline std::vector<std::string>			MapMa_AG_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_AG_FormArray;
	inline std::vector<bool>				MapMa_AG_BoolArray;
	inline std::int32_t						MapMa_AG_EntriesTotal;
	inline std::int32_t						MapMa_AG_EntriesFound;

	inline std::vector<std::string>			MapMa_HR_NameArray;
	inline std::vector<std::string>			MapMa_HR_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_HR_FormArray;
	inline std::vector<bool>				MapMa_HR_BoolArray;
	inline std::int32_t						MapMa_HR_EntriesTotal;
	inline std::int32_t						MapMa_HR_EntriesFound;

	inline std::vector<std::string>			MapMa_SZ_NameArray;
	inline std::vector<std::string>			MapMa_SZ_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_SZ_FormArray;
	inline std::vector<bool>				MapMa_SZ_BoolArray;
	inline std::int32_t						MapMa_SZ_EntriesTotal;
	inline std::int32_t						MapMa_SZ_EntriesFound;

	inline std::vector<std::string>			MapMa_DG_NameArray;
	inline std::vector<std::string>			MapMa_DG_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_DG_FormArray;
	inline std::vector<bool>				MapMa_DG_BoolArray;
	inline std::int32_t						MapMa_DG_EntriesTotal;
	inline std::int32_t						MapMa_DG_EntriesFound;

	inline std::vector<std::string>			MapMa_DB_NameArray;
	inline std::vector<std::string>			MapMa_DB_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_DB_FormArray;
	inline std::vector<bool>				MapMa_DB_BoolArray;
	inline std::int32_t						MapMa_DB_EntriesTotal;
	inline std::int32_t						MapMa_DB_EntriesFound;

	inline std::vector<std::string>			MapMa_CC_NameArray;
	inline std::vector<std::string>			MapMa_CC_TextArray;
	inline std::vector<RE::TESForm*>		MapMa_CC_FormArray;
	inline std::vector<bool>				MapMa_CC_BoolArray;
	inline std::int32_t						MapMa_CC_EntriesTotal;
	inline std::int32_t						MapMa_CC_EntriesFound;

	enum class MapMa_Sec : int32_t
	{
		kMapMa_AG = 0,
		kMapMa_HR = 1,
		kMapMa_SZ = 2,
		kMapMa_DG = 3,
		kMapMa_DB = 4,
		kMapMa_CC = 5,
	};

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

	public RE::BSTEventSink<RE::MenuOpenCloseEvent>{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();
	static void			InstallSearchTerms();

	static bool			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, MapMa_Sec a_section);
	static void			ProcessHookedMarker(const char* nam);
	static bool			MarkerIsValid(RE::TESObjectREFR* a_marker);
	static bool			MarkerIsCleared(RE::StaticFunctionTag*, RE::TESForm* a_form);
	};
}