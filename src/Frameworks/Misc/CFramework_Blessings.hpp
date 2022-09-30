#pragma once
#include "Serialization.hpp"

namespace CFramework_Blessings_DS {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Blessings_VS {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Blessings_WS {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Blessings{

	enum class Section : int32_t
	{
		kSection_DS = 0,
		kSection_VS = 1,
		kSection_WS = 2,

	};

	extern std::vector<std::string>		DS_NameArray;
	extern std::vector<std::string>		DS_TextArray;
	extern std::vector<RE::TESForm*>	DS_FormArray;
	extern std::vector<bool>			DS_BoolArray;
	extern std::int32_t					DS_EntriesTotal;
	extern std::int32_t					DS_EntriesFound;

	extern std::vector<std::string>		VS_NameArray;
	extern std::vector<std::string>		VS_TextArray;
	extern std::vector<RE::TESForm*>	VS_FormArray;
	extern std::vector<bool>			VS_BoolArray;
	extern std::int32_t					VS_EntriesTotal;
	extern std::int32_t					VS_EntriesFound;

	extern std::vector<std::string>		WS_NameArray;
	extern std::vector<std::string>		WS_TextArray;
	extern std::vector<RE::TESForm*>	WS_FormArray;
	extern std::vector<bool>			WS_BoolArray;
	extern std::int32_t					WS_EntriesTotal;
	extern std::int32_t					WS_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public RE::BSTEventSink<RE::TESActivateEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			InjectAndCompileData();

		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, Section a_section);
	};
}