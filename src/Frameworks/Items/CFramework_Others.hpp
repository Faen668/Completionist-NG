#pragma once
#include "Serialization.hpp"

namespace CFramework_Others_VC {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Others_PC {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Others_VM {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Others_PM {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Others_SB {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Others
{

	inline std::vector<std::string> VC_NameArray;
	inline std::vector<std::string> VC_TextArray;
	inline std::vector<RE::TESForm*> VC_FormArray;
	inline std::vector<bool> VC_BoolArray;
	inline std::int32_t VC_EntriesTotal;
	inline std::int32_t VC_EntriesFound;

	inline std::vector<std::string> PC_NameArray;
	inline std::vector<std::string> PC_TextArray;
	inline std::vector<RE::TESForm*> PC_FormArray;
	inline std::vector<bool> PC_BoolArray;
	inline std::int32_t PC_EntriesTotal;
	inline std::int32_t PC_EntriesFound;

	inline std::vector<std::string> VM_NameArray;
	inline std::vector<std::string> VM_TextArray;
	inline std::vector<RE::TESForm*> VM_FormArray;
	inline std::vector<bool> VM_BoolArray;
	inline std::int32_t VM_EntriesTotal;
	inline std::int32_t VM_EntriesFound;

	inline std::vector<std::string> PM_NameArray;
	inline std::vector<std::string> PM_TextArray;
	inline std::vector<RE::TESForm*> PM_FormArray;
	inline std::vector<bool> PM_BoolArray;
	inline std::int32_t PM_EntriesTotal;
	inline std::int32_t PM_EntriesFound;

	inline std::vector<std::string> SB_NameArray;
	inline std::vector<std::string> SB_TextArray;
	inline std::vector<RE::TESForm*> SB_FormArray;
	inline std::vector<bool> SB_BoolArray;
	inline std::int32_t SB_EntriesTotal;
	inline std::int32_t SB_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::TESActivateEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		  EventResult			ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			InjectAndCompileData();
		  static void			BuildBaranziahArrays();

		  static void			ProcessFoundForm(RE::TESObjectREFR* a_object);
		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}