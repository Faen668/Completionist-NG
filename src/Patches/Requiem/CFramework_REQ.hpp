#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_REQ
{
	inline Serialization::CompletionistData ItemDataA; //Armor
	inline Serialization::CompletionistData ItemDataM; //Misc
	inline Serialization::CompletionistData ItemDataW; //Weapons
	inline Serialization::CompletionistData ItemDataS; //Skill Books
	inline Serialization::CompletionistData ItemDataB; //Books
	inline Serialization::CompletionistData ItemDataT; //Tomes

	enum section
	{
		k_ItmL1A,
		k_ItmL1M,
		k_Weapon,
		k_ItmL3S,
		k_ItmL3B,
		k_SpellTome,
	};

	// Armor
	inline std::vector<std::string> Armor_NameArray;
	inline std::vector<std::string> Armor_TextArray;
	inline std::vector<RE::TESForm*> Armor_FormArray;
	inline std::vector<bool> Armor_BoolArray;
	inline std::int32_t Armor_EntriesTotal;
	inline std::int32_t Armor_EntriesFound;

	// Misc
	inline std::vector<std::string> Misc_NameArray;
	inline std::vector<std::string> Misc_TextArray;
	inline std::vector<RE::TESForm*> Misc_FormArray;
	inline std::vector<bool> Misc_BoolArray;
	inline std::int32_t Misc_EntriesTotal;
	inline std::int32_t Misc_EntriesFound;

	// Weapons
	inline std::vector<std::string> Weapon_NameArray;
	inline std::vector<std::string> Weapon_TextArray;
	inline std::vector<RE::TESForm*> Weapon_FormArray;
	inline std::vector<bool> Weapon_BoolArray;
	inline std::int32_t Weapon_EntriesTotal;
	inline std::int32_t Weapon_EntriesFound;

	// Skill Books
	inline std::vector<std::string> SkillBook_NameArray;
	inline std::vector<std::string> SkillBook_TextArray;
	inline std::vector<RE::TESForm*> SkillBook_FormArray;
	inline std::vector<bool> SkillBook_BoolArray;
	inline std::int32_t SkillBook_EntriesTotal;
	inline std::int32_t SkillBook_EntriesFound;

	// General Books
	inline std::vector<std::string> Book_NameArray;
	inline std::vector<std::string> Book_TextArray;
	inline std::vector<RE::TESForm*> Book_FormArray;
	inline std::vector<bool> Book_BoolArray;
	inline std::int32_t Book_EntriesTotal;
	inline std::int32_t Book_EntriesFound;

	// Spell Tomes
	inline std::vector<std::string> SpellTome_NameArray;
	inline std::vector<std::string> SpellTome_TextArray;
	inline std::vector<RE::TESForm*> SpellTome_FormArray;
	inline std::vector<bool> SpellTome_BoolArray;
	inline std::int32_t SpellTome_EntriesTotal;
	inline std::int32_t SpellTome_EntriesFound;

	class CHandler
	{

	public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

	static void			InstallFramework();
	static void			UpdateFoundForms();
	static void			InjectAndCompileData();
	static void			InstallSearchTerms();

	static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
	static void			OnBooksReadEvent(RE::BooksRead::Event const* a_event);
	static void			OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);
	static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};
}