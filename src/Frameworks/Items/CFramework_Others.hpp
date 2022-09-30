#pragma once
#include "Serialization.hpp"

namespace CFramework_Others_VC {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Others_PC {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Others_VM {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Others_PM {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Others_SB {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Others
{

	extern std::vector<std::string> VC_NameArray;
	extern std::vector<std::string> VC_TextArray;
	extern std::vector<RE::TESForm*> VC_FormArray;
	extern std::vector<bool> VC_BoolArray;
	extern std::int32_t VC_EntriesTotal;
	extern std::int32_t VC_EntriesFound;

	extern std::vector<std::string> PC_NameArray;
	extern std::vector<std::string> PC_TextArray;
	extern std::vector<RE::TESForm*> PC_FormArray;
	extern std::vector<bool> PC_BoolArray;
	extern std::int32_t PC_EntriesTotal;
	extern std::int32_t PC_EntriesFound;

	extern std::vector<std::string> VM_NameArray;
	extern std::vector<std::string> VM_TextArray;
	extern std::vector<RE::TESForm*> VM_FormArray;
	extern std::vector<bool> VM_BoolArray;
	extern std::int32_t VM_EntriesTotal;
	extern std::int32_t VM_EntriesFound;

	extern std::vector<std::string> PM_NameArray;
	extern std::vector<std::string> PM_TextArray;
	extern std::vector<RE::TESForm*> PM_FormArray;
	extern std::vector<bool> PM_BoolArray;
	extern std::int32_t PM_EntriesTotal;
	extern std::int32_t PM_EntriesFound;

	extern std::vector<std::string> SB_NameArray;
	extern std::vector<std::string> SB_TextArray;
	extern std::vector<RE::TESForm*> SB_FormArray;
	extern std::vector<bool> SB_BoolArray;
	extern std::int32_t SB_EntriesTotal;
	extern std::int32_t SB_EntriesFound;

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