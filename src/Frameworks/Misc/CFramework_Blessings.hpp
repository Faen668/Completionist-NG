#pragma once
#include "Serialization.hpp"

namespace CFramework_Blessings_DS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Blessings_VS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Blessings_WS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Blessings{

	enum class Section : int32_t
	{
		kSection_DS = 0,
		kSection_VS = 1,
		kSection_WS = 2,

	};

	inline std::vector<std::string>		DS_NameArray;
	inline std::vector<std::string>		DS_TextArray;
	inline std::vector<RE::TESForm*>	DS_FormArray;
	inline std::vector<bool>			DS_BoolArray;
	inline std::int32_t					DS_EntriesTotal;
	inline std::int32_t					DS_EntriesFound;

	inline std::vector<std::string>		VS_NameArray;
	inline std::vector<std::string>		VS_TextArray;
	inline std::vector<RE::TESForm*>	VS_FormArray;
	inline std::vector<bool>			VS_BoolArray;
	inline std::int32_t					VS_EntriesTotal;
	inline std::int32_t					VS_EntriesFound;

	inline std::vector<std::string>		WS_NameArray;
	inline std::vector<std::string>		WS_TextArray;
	inline std::vector<RE::TESForm*>	WS_FormArray;
	inline std::vector<bool>			WS_BoolArray;
	inline std::int32_t					WS_EntriesTotal;
	inline std::int32_t					WS_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public RE::BSTEventSink<RE::TESActivateEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			InjectAndCompileData();
		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, Section a_section);

		  static void			AddFormsToList();
		  static void			ActivateShrineFromPapyrus(RE::StaticFunctionTag*, RE::TESForm* a_shrine);
		  static void			InstallSearchTerms();
	};
}