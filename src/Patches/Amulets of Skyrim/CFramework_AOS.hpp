#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_AOS
{	
	inline Serialization::CompletionistData ItemData1;	// 1 - Misc Amulets = 45
	inline Serialization::CompletionistData ItemData2;	// 1 - Misc Rings = 4
	inline Serialization::CompletionistData ItemData3;	// 1 - Daedric Amulets = 24
	inline Serialization::CompletionistData ItemData4;	// 2 - Dragon Priest Amulets = 14
	inline Serialization::CompletionistData ItemData5;	// 2 - Dragon Priest Rings = 14
	inline Serialization::CompletionistData ItemData6;	// 2 - Faith Amulets = 7
	inline Serialization::CompletionistData ItemData7;	// 2 - Doomstone Amulets = 13
	inline Serialization::CompletionistData ItemData8;	// 3 - Solder Amulets = 19
	inline Serialization::CompletionistData ItemData9;	// 3 - Guild Amulets = 22
	inline Serialization::CompletionistData ItemData10;	// 3 - Thane Amulets = 9
	inline Serialization::CompletionistData ItemData11;	// 3 - Jarl Amulets = 9
	inline Serialization::CompletionistData ItemData12;	// 3 - Hold Amulets = 42

	inline std::vector<std::string> ItmL1_NameArray;
	inline std::vector<std::string> ItmL1_TextArray;
	inline std::vector<RE::TESForm*> ItmL1_FormArray;
	inline std::vector<bool> ItmL1_BoolArray;
	inline std::int32_t ItmL1_EntriesTotal;
	inline std::int32_t ItmL1_EntriesFound;

	inline std::vector<std::string> ItmL2_NameArray;
	inline std::vector<std::string> ItmL2_TextArray;
	inline std::vector<RE::TESForm*> ItmL2_FormArray;
	inline std::vector<bool> ItmL2_BoolArray;
	inline std::int32_t ItmL2_EntriesTotal;
	inline std::int32_t ItmL2_EntriesFound;

	inline std::vector<std::string> ItmL3_NameArray;
	inline std::vector<std::string> ItmL3_TextArray;
	inline std::vector<RE::TESForm*> ItmL3_FormArray;
	inline std::vector<bool> ItmL3_BoolArray;
	inline std::int32_t ItmL3_EntriesTotal;
	inline std::int32_t ItmL3_EntriesFound;

	inline std::vector<std::string> ItmL4_NameArray;
	inline std::vector<std::string> ItmL4_TextArray;
	inline std::vector<RE::TESForm*> ItmL4_FormArray;
	inline std::vector<bool> ItmL4_BoolArray;
	inline std::int32_t ItmL4_EntriesTotal;
	inline std::int32_t ItmL4_EntriesFound;

	inline std::vector<std::string> ItmL5_NameArray;
	inline std::vector<std::string> ItmL5_TextArray;
	inline std::vector<RE::TESForm*> ItmL5_FormArray;
	inline std::vector<bool> ItmL5_BoolArray;
	inline std::int32_t ItmL5_EntriesTotal;
	inline std::int32_t ItmL5_EntriesFound;

	inline std::vector<std::string> ItmL6_NameArray;
	inline std::vector<std::string> ItmL6_TextArray;
	inline std::vector<RE::TESForm*> ItmL6_FormArray;
	inline std::vector<bool> ItmL6_BoolArray;
	inline std::int32_t ItmL6_EntriesTotal;
	inline std::int32_t ItmL6_EntriesFound;

	inline std::vector<std::string> ItmL7_NameArray;
	inline std::vector<std::string> ItmL7_TextArray;
	inline std::vector<RE::TESForm*> ItmL7_FormArray;
	inline std::vector<bool> ItmL7_BoolArray;
	inline std::int32_t ItmL7_EntriesTotal;
	inline std::int32_t ItmL7_EntriesFound;

	inline std::vector<std::string> ItmL8_NameArray;
	inline std::vector<std::string> ItmL8_TextArray;
	inline std::vector<RE::TESForm*> ItmL8_FormArray;
	inline std::vector<bool> ItmL8_BoolArray;
	inline std::int32_t ItmL8_EntriesTotal;
	inline std::int32_t ItmL8_EntriesFound;

	inline std::vector<std::string> ItmL9_NameArray;
	inline std::vector<std::string> ItmL9_TextArray;
	inline std::vector<RE::TESForm*> ItmL9_FormArray;
	inline std::vector<bool> ItmL9_BoolArray;
	inline std::int32_t ItmL9_EntriesTotal;
	inline std::int32_t ItmL9_EntriesFound;

	inline std::vector<std::string> ItmL10_NameArray;
	inline std::vector<std::string> ItmL10_TextArray;
	inline std::vector<RE::TESForm*> ItmL10_FormArray;
	inline std::vector<bool> ItmL10_BoolArray;
	inline std::int32_t ItmL10_EntriesTotal;
	inline std::int32_t ItmL10_EntriesFound;

	inline std::vector<std::string> ItmL11_NameArray;
	inline std::vector<std::string> ItmL11_TextArray;
	inline std::vector<RE::TESForm*> ItmL11_FormArray;
	inline std::vector<bool> ItmL11_BoolArray;
	inline std::int32_t ItmL11_EntriesTotal;
	inline std::int32_t ItmL11_EntriesFound;

	inline std::vector<std::string> ItmL12_NameArray;
	inline std::vector<std::string> ItmL12_TextArray;
	inline std::vector<RE::TESForm*> ItmL12_FormArray;
	inline std::vector<bool> ItmL12_BoolArray;
	inline std::int32_t ItmL12_EntriesTotal;
	inline std::int32_t ItmL12_EntriesFound;

	class CHandler
	{
		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InstallFramework();
		static void			UpdateFoundForms();
		static void			InjectAndCompileData();
		static void			InstallSearchTerms();

		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}