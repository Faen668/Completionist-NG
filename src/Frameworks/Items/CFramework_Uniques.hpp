#pragma once
#include "Serialization.hpp"

namespace CFramework_Uniques_A {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_W {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_I {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_J {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_L {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCA {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCI {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCW {
	extern Serialization::CompletionistData Data;
}

namespace CFramework_Uniques
{

	extern std::vector<std::string> A_NameArray;
	extern std::vector<std::string> A_TextArray;
	extern std::vector<RE::TESForm*> A_FormArray;
	extern std::vector<bool> A_BoolArray;
	extern std::int32_t A_EntriesTotal;
	extern std::int32_t A_EntriesFound;

	extern std::vector<std::string> W_NameArray;
	extern std::vector<std::string> W_TextArray;
	extern std::vector<RE::TESForm*> W_FormArray;
	extern std::vector<bool> W_BoolArray;
	extern std::int32_t W_EntriesTotal;
	extern std::int32_t W_EntriesFound;

	extern std::vector<std::string> I_NameArray;
	extern std::vector<std::string> I_TextArray;
	extern std::vector<RE::TESForm*> I_FormArray;
	extern std::vector<bool> I_BoolArray;
	extern std::int32_t I_EntriesTotal;
	extern std::int32_t I_EntriesFound;

	extern std::vector<std::string> J_NameArray;
	extern std::vector<std::string> J_TextArray;
	extern std::vector<RE::TESForm*> J_FormArray;
	extern std::vector<bool> J_BoolArray;
	extern std::int32_t J_EntriesTotal;
	extern std::int32_t J_EntriesFound;

	extern std::vector<std::string> L_NameArray;
	extern std::vector<std::string> L_TextArray;
	extern std::vector<RE::TESForm*> L_FormArray;
	extern std::vector<bool> L_BoolArray;
	extern std::int32_t L_EntriesTotal;
	extern std::int32_t L_EntriesFound;

	extern std::vector<std::string> CCA_NameArray;
	extern std::vector<std::string> CCA_TextArray;
	extern std::vector<RE::TESForm*> CCA_FormArray;
	extern std::vector<bool> CCA_BoolArray;
	extern std::int32_t CCA_EntriesTotal;
	extern std::int32_t CCA_EntriesFound;

	extern std::vector<std::string> CCI_NameArray;
	extern std::vector<std::string> CCI_TextArray;
	extern std::vector<RE::TESForm*> CCI_FormArray;
	extern std::vector<bool> CCI_BoolArray;
	extern std::int32_t CCI_EntriesTotal;
	extern std::int32_t CCI_EntriesFound;

	extern std::vector<std::string> CCW_NameArray;
	extern std::vector<std::string> CCW_TextArray;
	extern std::vector<RE::TESForm*> CCW_FormArray;
	extern std::vector<bool> CCW_BoolArray;
	extern std::int32_t CCW_EntriesTotal;
	extern std::int32_t CCW_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::TESContainerChangedEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

		  static void			SinkEvents();
		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			InjectAndCompileData();

		  static void			Install_CCA();
		  static void			Install_CCI();
		  static void			Install_CCW();

		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, std::string a_variable);
	};
}