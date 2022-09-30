#pragma once

#include <SKSE\API.h>
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CPatch_FSH_F {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_I {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_B {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_A {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_C {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_L {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH_S {
	extern Serialization::CompletionistData Data;
}

namespace CPatch_FSH
{
	extern std::vector<std::string>		F_NameArray;
	extern std::vector<std::string>		F_TextArray;
	extern std::vector<RE::TESForm*>	F_FormArray;
	extern std::vector<bool>			F_BoolArray;
	extern std::int32_t					F_EntriesTotal;
	extern std::int32_t					F_EntriesFound;

	extern std::vector<std::string>		I_NameArray;
	extern std::vector<std::string>		I_TextArray;
	extern std::vector<RE::TESForm*>	I_FormArray;
	extern std::vector<bool>			I_BoolArray;
	extern std::int32_t					I_EntriesTotal;
	extern std::int32_t					I_EntriesFound;

	extern std::vector<std::string>		B_NameArray;
	extern std::vector<std::string>		B_TextArray;
	extern std::vector<RE::TESForm*>	B_FormArray;
	extern std::vector<bool>			B_BoolArray;
	extern std::int32_t					B_EntriesTotal;
	extern std::int32_t					B_EntriesFound;

	extern std::vector<std::string>		A_NameArray;
	extern std::vector<std::string>		A_TextArray;
	extern std::vector<RE::TESForm*>	A_FormArray;
	extern std::vector<bool>			A_BoolArray;
	extern std::int32_t					A_EntriesTotal;
	extern std::int32_t					A_EntriesFound;

	extern std::vector<std::string>		C_NameArray;
	extern std::vector<std::string>		C_TextArray;
	extern std::vector<RE::TESForm*>	C_FormArray;
	extern std::vector<bool>			C_BoolArray;
	extern std::int32_t					C_EntriesTotal;
	extern std::int32_t					C_EntriesFound;

	extern std::vector<std::string>		L_NameArray;
	extern std::vector<std::string>		L_TextArray;
	extern std::vector<RE::TESForm*>	L_FormArray;
	extern std::vector<bool>			L_BoolArray;
	extern std::int32_t					L_EntriesTotal;
	extern std::int32_t					L_EntriesFound;

	extern std::vector<std::string>		S_NameArray;
	extern std::vector<std::string>		S_TextArray;
	extern std::vector<RE::TESForm*>	S_FormArray;
	extern std::vector<bool>			S_BoolArray;
	extern std::int32_t					S_EntriesTotal;
	extern std::int32_t					S_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :
		public RE::BSTEventSink<RE::TESContainerChangedEvent>,
		public RE::BSTEventSink<RE::BooksRead::Event>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::TESEquipEvent> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;
		  EventResult			ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*) override;
		  EventResult			ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) override;
		  EventResult			ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

		  static void			InstallFramework();
		  static void			UpdateFoundForms();
		  static void			SinkEvents();

		  static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_section);

		  static void			InjectAndCompileData();;
		  static void			BuildFishArrays(RE::TESForm* a_form, std::string a_rod, std::string a_loc);
	};

}