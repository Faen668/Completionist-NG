#pragma once

namespace CPatch_SpellTomes_Apocalypse {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_SpellTomes_Odin {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_SpellTomes_Mysticism {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_SpellTomes_ForgottenMagic {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_SpellTomes_Triumvirate {
	inline Serialization::CompletionistData Data;
}

namespace CPatch_SpellTomes
{	

	enum class patchID : int32_t
	{
		kpatch_Apocalypse = 0,
		kpatch_ForgottenMagic = 1,
		kpatch_Mysticism = 2,
		kpatch_Odin = 3,
		kpatch_Triumvirate = 4,

		kTotal,
	};

	inline std::vector<std::string> Odin_A_NameArray;
	inline std::vector<std::string> Odin_A_TextArray;
	inline std::vector<RE::TESForm*> Odin_A_FormArray;
	inline std::vector<bool> Odin_A_BoolArray;
	inline std::int32_t Odin_A_EntriesTotal;
	inline std::int32_t Odin_A_EntriesFound;

	inline std::vector<std::string> Odin_C_NameArray;
	inline std::vector<std::string> Odin_C_TextArray;
	inline std::vector<RE::TESForm*> Odin_C_FormArray;
	inline std::vector<bool> Odin_C_BoolArray;
	inline std::int32_t Odin_C_EntriesTotal;
	inline std::int32_t Odin_C_EntriesFound;

	inline std::vector<std::string> Odin_D_NameArray;
	inline std::vector<std::string> Odin_D_TextArray;
	inline std::vector<RE::TESForm*> Odin_D_FormArray;
	inline std::vector<bool> Odin_D_BoolArray;
	inline std::int32_t Odin_D_EntriesTotal;
	inline std::int32_t Odin_D_EntriesFound;

	inline std::vector<std::string> Odin_I_NameArray;
	inline std::vector<std::string> Odin_I_TextArray;
	inline std::vector<RE::TESForm*> Odin_I_FormArray;
	inline std::vector<bool> Odin_I_BoolArray;
	inline std::int32_t Odin_I_EntriesTotal;
	inline std::int32_t Odin_I_EntriesFound;

	inline std::vector<std::string> Odin_R_NameArray;
	inline std::vector<std::string> Odin_R_TextArray;
	inline std::vector<RE::TESForm*> Odin_R_FormArray;
	inline std::vector<bool> Odin_R_BoolArray;
	inline std::int32_t Odin_R_EntriesTotal;
	inline std::int32_t Odin_R_EntriesFound;

	inline std::vector<std::string> ForgottenMagic_A_NameArray;
	inline std::vector<std::string> ForgottenMagic_A_TextArray;
	inline std::vector<RE::TESForm*> ForgottenMagic_A_FormArray;
	inline std::vector<bool> ForgottenMagic_A_BoolArray;
	inline std::int32_t ForgottenMagic_A_EntriesTotal;
	inline std::int32_t ForgottenMagic_A_EntriesFound;

	inline std::vector<std::string> ForgottenMagic_C_NameArray;
	inline std::vector<std::string> ForgottenMagic_C_TextArray;
	inline std::vector<RE::TESForm*> ForgottenMagic_C_FormArray;
	inline std::vector<bool> ForgottenMagic_C_BoolArray;
	inline std::int32_t ForgottenMagic_C_EntriesTotal;
	inline std::int32_t ForgottenMagic_C_EntriesFound;

	inline std::vector<std::string> ForgottenMagic_D_NameArray;
	inline std::vector<std::string> ForgottenMagic_D_TextArray;
	inline std::vector<RE::TESForm*> ForgottenMagic_D_FormArray;
	inline std::vector<bool> ForgottenMagic_D_BoolArray;
	inline std::int32_t ForgottenMagic_D_EntriesTotal;
	inline std::int32_t ForgottenMagic_D_EntriesFound;

	inline std::vector<std::string> ForgottenMagic_I_NameArray;
	inline std::vector<std::string> ForgottenMagic_I_TextArray;
	inline std::vector<RE::TESForm*> ForgottenMagic_I_FormArray;
	inline std::vector<bool> ForgottenMagic_I_BoolArray;
	inline std::int32_t ForgottenMagic_I_EntriesTotal;
	inline std::int32_t ForgottenMagic_I_EntriesFound;

	inline std::vector<std::string> ForgottenMagic_R_NameArray;
	inline std::vector<std::string> ForgottenMagic_R_TextArray;
	inline std::vector<RE::TESForm*> ForgottenMagic_R_FormArray;
	inline std::vector<bool> ForgottenMagic_R_BoolArray;
	inline std::int32_t ForgottenMagic_R_EntriesTotal;
	inline std::int32_t ForgottenMagic_R_EntriesFound;

	inline std::vector<std::string> Apocalypse_A_NameArray;
	inline std::vector<std::string> Apocalypse_A_TextArray;
	inline std::vector<RE::TESForm*> Apocalypse_A_FormArray;
	inline std::vector<bool> Apocalypse_A_BoolArray;
	inline std::int32_t Apocalypse_A_EntriesTotal;
	inline std::int32_t Apocalypse_A_EntriesFound;

	inline std::vector<std::string> Apocalypse_C_NameArray;
	inline std::vector<std::string> Apocalypse_C_TextArray;
	inline std::vector<RE::TESForm*> Apocalypse_C_FormArray;
	inline std::vector<bool> Apocalypse_C_BoolArray;
	inline std::int32_t Apocalypse_C_EntriesTotal;
	inline std::int32_t Apocalypse_C_EntriesFound;

	inline std::vector<std::string> Apocalypse_D_NameArray;
	inline std::vector<std::string> Apocalypse_D_TextArray;
	inline std::vector<RE::TESForm*> Apocalypse_D_FormArray;
	inline std::vector<bool> Apocalypse_D_BoolArray;
	inline std::int32_t Apocalypse_D_EntriesTotal;
	inline std::int32_t Apocalypse_D_EntriesFound;

	inline std::vector<std::string> Apocalypse_I_NameArray;
	inline std::vector<std::string> Apocalypse_I_TextArray;
	inline std::vector<RE::TESForm*> Apocalypse_I_FormArray;
	inline std::vector<bool> Apocalypse_I_BoolArray;
	inline std::int32_t Apocalypse_I_EntriesTotal;
	inline std::int32_t Apocalypse_I_EntriesFound;

	inline std::vector<std::string> Apocalypse_R_NameArray;
	inline std::vector<std::string> Apocalypse_R_TextArray;
	inline std::vector<RE::TESForm*> Apocalypse_R_FormArray;
	inline std::vector<bool> Apocalypse_R_BoolArray;
	inline std::int32_t Apocalypse_R_EntriesTotal;
	inline std::int32_t Apocalypse_R_EntriesFound;

	inline std::vector<std::string> Mysticism_A_NameArray;
	inline std::vector<std::string> Mysticism_A_TextArray;
	inline std::vector<RE::TESForm*> Mysticism_A_FormArray;
	inline std::vector<bool> Mysticism_A_BoolArray;
	inline std::int32_t Mysticism_A_EntriesTotal;
	inline std::int32_t Mysticism_A_EntriesFound;

	inline std::vector<std::string> Mysticism_C_NameArray;
	inline std::vector<std::string> Mysticism_C_TextArray;
	inline std::vector<RE::TESForm*> Mysticism_C_FormArray;
	inline std::vector<bool> Mysticism_C_BoolArray;
	inline std::int32_t Mysticism_C_EntriesTotal;
	inline std::int32_t Mysticism_C_EntriesFound;

	inline std::vector<std::string> Mysticism_D_NameArray;
	inline std::vector<std::string> Mysticism_D_TextArray;
	inline std::vector<RE::TESForm*> Mysticism_D_FormArray;
	inline std::vector<bool> Mysticism_D_BoolArray;
	inline std::int32_t Mysticism_D_EntriesTotal;
	inline std::int32_t Mysticism_D_EntriesFound;

	inline std::vector<std::string> Mysticism_I_NameArray;
	inline std::vector<std::string> Mysticism_I_TextArray;
	inline std::vector<RE::TESForm*> Mysticism_I_FormArray;
	inline std::vector<bool> Mysticism_I_BoolArray;
	inline std::int32_t Mysticism_I_EntriesTotal;
	inline std::int32_t Mysticism_I_EntriesFound;

	inline std::vector<std::string> Mysticism_R_NameArray;
	inline std::vector<std::string> Mysticism_R_TextArray;
	inline std::vector<RE::TESForm*> Mysticism_R_FormArray;
	inline std::vector<bool> Mysticism_R_BoolArray;
	inline std::int32_t Mysticism_R_EntriesTotal;
	inline std::int32_t Mysticism_R_EntriesFound;

	inline std::vector<std::string> Triumvirate_A_NameArray;
	inline std::vector<std::string> Triumvirate_A_TextArray;
	inline std::vector<RE::TESForm*> Triumvirate_A_FormArray;
	inline std::vector<bool> Triumvirate_A_BoolArray;
	inline std::int32_t Triumvirate_A_EntriesTotal;
	inline std::int32_t Triumvirate_A_EntriesFound;

	inline std::vector<std::string> Triumvirate_C_NameArray;
	inline std::vector<std::string> Triumvirate_C_TextArray;
	inline std::vector<RE::TESForm*> Triumvirate_C_FormArray;
	inline std::vector<bool> Triumvirate_C_BoolArray;
	inline std::int32_t Triumvirate_C_EntriesTotal;
	inline std::int32_t Triumvirate_C_EntriesFound;

	inline std::vector<std::string> Triumvirate_D_NameArray;
	inline std::vector<std::string> Triumvirate_D_TextArray;
	inline std::vector<RE::TESForm*> Triumvirate_D_FormArray;
	inline std::vector<bool> Triumvirate_D_BoolArray;
	inline std::int32_t Triumvirate_D_EntriesTotal;
	inline std::int32_t Triumvirate_D_EntriesFound;

	inline std::vector<std::string> Triumvirate_I_NameArray;
	inline std::vector<std::string> Triumvirate_I_TextArray;
	inline std::vector<RE::TESForm*> Triumvirate_I_FormArray;
	inline std::vector<bool> Triumvirate_I_BoolArray;
	inline std::int32_t Triumvirate_I_EntriesTotal;
	inline std::int32_t Triumvirate_I_EntriesFound;

	inline std::vector<std::string> Triumvirate_R_NameArray;
	inline std::vector<std::string> Triumvirate_R_TextArray;
	inline std::vector<RE::TESForm*> Triumvirate_R_FormArray;
	inline std::vector<bool> Triumvirate_R_BoolArray;
	inline std::int32_t Triumvirate_R_EntriesTotal;
	inline std::int32_t Triumvirate_R_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final :

		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::BooksRead::Event> {

		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		EventResult			ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;
		EventResult			ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		static void			SinkEvents();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();

		static void			InstallFramework();
		static void			UpdateFoundForms();

		static void			ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_curID, patchID a_patch);
	};
}