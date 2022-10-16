#pragma once
#include "Serialization.hpp"

namespace CFramework_Shouts_VS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Shouts_TS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Shouts_MS {
	inline Serialization::CompletionistData Data;
}

namespace CFramework_Shouts{

	enum class Section : std::int32_t
	{
		kVanilla = 0,
		kThunderchild = 1,
		kMiscellaneous = 2,

		kTotal
	};

	//Vanilla Shouts
	inline std::vector<std::string>				Vanilla_SH_NameOutput;
	inline std::vector<std::string>				Vanilla_SH_NameArray;
	inline std::vector<std::string>				Vanilla_SH_TextArray;
	inline std::vector<RE::TESForm*>			Vanilla_SH_FormArray;
	inline std::vector<RE::SpellItem*>			Vanilla_W1_FormArray;
	inline std::vector<RE::SpellItem*>			Vanilla_W2_FormArray;
	inline std::vector<RE::SpellItem*>			Vanilla_W3_FormArray;
	inline std::vector<bool>					Vanilla_SH_BoolArray;
	inline std::int32_t							Vanilla_SH_EntriesTotal;
	inline std::int32_t							Vanilla_SH_EntriesFound;

	//Thunderchild Shouts
	inline std::vector<std::string>				Thunderchild_SH_NameOutput;
	inline std::vector<std::string>				Thunderchild_SH_NameArray;
	inline std::vector<std::string>				Thunderchild_SH_TextArray;
	inline std::vector<RE::TESForm*>			Thunderchild_SH_FormArray;
	inline std::vector<RE::SpellItem*>			Thunderchild_W1_FormArray;
	inline std::vector<RE::SpellItem*>			Thunderchild_W2_FormArray;
	inline std::vector<RE::SpellItem*>			Thunderchild_W3_FormArray;
	inline std::vector<bool>					Thunderchild_SH_BoolArray;
	inline std::int32_t							Thunderchild_SH_EntriesTotal;
	inline std::int32_t							Thunderchild_SH_EntriesFound;

	//Miscellaneous Shouts
	inline std::vector<std::string>				Miscellaneous_SH_NameOutput;
	inline std::vector<std::string>				Miscellaneous_SH_NameArray;
	inline std::vector<std::string>				Miscellaneous_SH_TextArray;
	inline std::vector<RE::TESForm*>			Miscellaneous_SH_FormArray;
	inline std::vector<RE::SpellItem*>			Miscellaneous_W1_FormArray;
	inline std::vector<RE::SpellItem*>			Miscellaneous_W2_FormArray;
	inline std::vector<RE::SpellItem*>			Miscellaneous_W3_FormArray;
	inline std::vector<bool>					Miscellaneous_SH_BoolArray;
	inline std::int32_t							Miscellaneous_SH_EntriesTotal;
	inline std::int32_t							Miscellaneous_SH_EntriesFound;

	using EventResult = RE::BSEventNotifyControl;

	class CHandler final : public RE::BSTEventSink<RE::TESSpellCastEvent> {

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		  EventResult			ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>*) override;

		  static void			SinkEvents();
		  static void			InstallFramework();

		  static void			UpdateFoundForms();
		  static void			UpdateFoundFormsExt(RE::StaticFunctionTag*);

		  static void			InjectAndCompileData();

		  static void			BuildArrays();
		  static void			ClearArrays();
		  static void			ReSizeArrays();

		  static void			AddToArray(Section a_section, RE::FormID a_formID, std::string a_names, std::string a_texts, std::string a_Filename);

		  static void			ProcessFoundForm(Section a_section, RE::FormID a_formID);
	};
}