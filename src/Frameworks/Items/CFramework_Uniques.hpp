#pragma once
#include "Serialization.hpp"

namespace CFramework_Uniques_A {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_W {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_I {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_J {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_L {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCA {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCI {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques_CCW {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Uniques
{

	inline std::vector<std::string> A_NameArray;
	inline std::vector<std::string> A_TextArray;
	inline std::vector<RE::TESForm*> A_FormArray;
	inline std::vector<bool> A_BoolArray;
	inline std::int32_t A_EntriesTotal;
	inline std::int32_t A_EntriesFound;

	inline std::vector<std::string> W_NameArray;
	inline std::vector<std::string> W_TextArray;
	inline std::vector<RE::TESForm*> W_FormArray;
	inline std::vector<bool> W_BoolArray;
	inline std::int32_t W_EntriesTotal;
	inline std::int32_t W_EntriesFound;

	inline std::vector<std::string> I_NameArray;
	inline std::vector<std::string> I_TextArray;
	inline std::vector<RE::TESForm*> I_FormArray;
	inline std::vector<bool> I_BoolArray;
	inline std::int32_t I_EntriesTotal;
	inline std::int32_t I_EntriesFound;

	inline std::vector<std::string> J_NameArray;
	inline std::vector<std::string> J_TextArray;
	inline std::vector<RE::TESForm*> J_FormArray;
	inline std::vector<bool> J_BoolArray;
	inline std::int32_t J_EntriesTotal;
	inline std::int32_t J_EntriesFound;

	inline std::vector<std::string> L_NameArray;
	inline std::vector<std::string> L_TextArray;
	inline std::vector<RE::TESForm*> L_FormArray;
	inline std::vector<bool> L_BoolArray;
	inline std::int32_t L_EntriesTotal;
	inline std::int32_t L_EntriesFound;

	inline std::vector<std::string> CCA_NameArray;
	inline std::vector<std::string> CCA_TextArray;
	inline std::vector<RE::TESForm*> CCA_FormArray;
	inline std::vector<bool> CCA_BoolArray;
	inline std::int32_t CCA_EntriesTotal;
	inline std::int32_t CCA_EntriesFound;

	inline std::vector<std::string> CCI_NameArray;
	inline std::vector<std::string> CCI_TextArray;
	inline std::vector<RE::TESForm*> CCI_FormArray;
	inline std::vector<bool> CCI_BoolArray;
	inline std::int32_t CCI_EntriesTotal;
	inline std::int32_t CCI_EntriesFound;

	inline std::vector<std::string> CCW_NameArray;
	inline std::vector<std::string> CCW_TextArray;
	inline std::vector<RE::TESForm*> CCW_FormArray;
	inline std::vector<bool> CCW_BoolArray;
	inline std::int32_t CCW_EntriesTotal;
	inline std::int32_t CCW_EntriesFound;

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