#pragma once

#include "Serialization.hpp"

namespace CFramework_MapMa_AG {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_HR {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_SZ {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_DG {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_DB {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa_CC {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_MapMa 
{	
	extern std::vector<std::string>			MapMa_AG_NameArray;
	extern std::vector<std::string>			MapMa_AG_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_AG_FormArray;
	extern std::vector<bool>				MapMa_AG_BoolArray;
	extern std::int32_t						MapMa_AG_EntriesTotal;
	extern std::int32_t						MapMa_AG_EntriesFound;

	extern std::vector<std::string>			MapMa_HR_NameArray;
	extern std::vector<std::string>			MapMa_HR_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_HR_FormArray;
	extern std::vector<bool>				MapMa_HR_BoolArray;
	extern std::int32_t						MapMa_HR_EntriesTotal;
	extern std::int32_t						MapMa_HR_EntriesFound;

	extern std::vector<std::string>			MapMa_SZ_NameArray;
	extern std::vector<std::string>			MapMa_SZ_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_SZ_FormArray;
	extern std::vector<bool>				MapMa_SZ_BoolArray;
	extern std::int32_t						MapMa_SZ_EntriesTotal;
	extern std::int32_t						MapMa_SZ_EntriesFound;

	extern std::vector<std::string>			MapMa_DG_NameArray;
	extern std::vector<std::string>			MapMa_DG_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_DG_FormArray;
	extern std::vector<bool>				MapMa_DG_BoolArray;
	extern std::int32_t						MapMa_DG_EntriesTotal;
	extern std::int32_t						MapMa_DG_EntriesFound;

	extern std::vector<std::string>			MapMa_DB_NameArray;
	extern std::vector<std::string>			MapMa_DB_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_DB_FormArray;
	extern std::vector<bool>				MapMa_DB_BoolArray;
	extern std::int32_t						MapMa_DB_EntriesTotal;
	extern std::int32_t						MapMa_DB_EntriesFound;

	extern std::vector<std::string>			MapMa_CC_NameArray;
	extern std::vector<std::string>			MapMa_CC_TextArray;
	extern std::vector<RE::TESForm*>		MapMa_CC_FormArray;
	extern std::vector<bool>				MapMa_CC_BoolArray;
	extern std::int32_t						MapMa_CC_EntriesTotal;
	extern std::int32_t						MapMa_CC_EntriesFound;

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

	public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

	static void			SinkEvents();
	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();

	static void			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, MapMa_Sec a_section);
	static bool			MarkerIsValid(RE::TESObjectREFR* a_marker);
	static void			Install_CCM();
	};
}