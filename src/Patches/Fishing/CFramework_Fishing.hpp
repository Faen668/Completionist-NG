#pragma once
#define ProcessFoundFormArgs RE::FormID a_baseID, RE::FormID a_eventID, Serialization::CompletionistData data, std::vector<RE::TESForm*> forms, std::vector<bool>* bools, std::int32_t* found, Serialization::CompletionistLog::logType eventHandle

namespace CPatch_FSH
{
	inline Serialization::CompletionistData FishData;
	inline Serialization::CompletionistData ItemData;
	inline Serialization::CompletionistData BookData;
	inline Serialization::CompletionistData MapsDataA;
	inline Serialization::CompletionistData MapsDataC;
	inline Serialization::CompletionistData MapsDataL;
	inline Serialization::CompletionistData MapsDataS;

	inline std::vector<std::string>		F_NameArray;
	inline std::vector<std::string>		F_TextArray;
	inline std::vector<RE::TESForm*>	F_FormArray;
	inline std::vector<bool>			F_BoolArray;
	inline std::int32_t					F_EntriesTotal;
	inline std::int32_t					F_EntriesFound;

	inline std::vector<std::string>		I_NameArray;
	inline std::vector<std::string>		I_TextArray;
	inline std::vector<RE::TESForm*>	I_FormArray;
	inline std::vector<bool>			I_BoolArray;
	inline std::int32_t					I_EntriesTotal;
	inline std::int32_t					I_EntriesFound;

	inline std::vector<std::string>		B_NameArray;
	inline std::vector<std::string>		B_TextArray;
	inline std::vector<RE::TESForm*>	B_FormArray;
	inline std::vector<bool>			B_BoolArray;
	inline std::int32_t					B_EntriesTotal;
	inline std::int32_t					B_EntriesFound;

	inline std::vector<std::string>		A_NameArray;
	inline std::vector<std::string>		A_TextArray;
	inline std::vector<RE::TESForm*>	A_FormArray;
	inline std::vector<bool>			A_BoolArray;
	inline std::int32_t					A_EntriesTotal;
	inline std::int32_t					A_EntriesFound;

	inline std::vector<std::string>		C_NameArray;
	inline std::vector<std::string>		C_TextArray;
	inline std::vector<RE::TESForm*>	C_FormArray;
	inline std::vector<bool>			C_BoolArray;
	inline std::int32_t					C_EntriesTotal;
	inline std::int32_t					C_EntriesFound;

	inline std::vector<std::string>		L_NameArray;
	inline std::vector<std::string>		L_TextArray;
	inline std::vector<RE::TESForm*>	L_FormArray;
	inline std::vector<bool>			L_BoolArray;
	inline std::int32_t					L_EntriesTotal;
	inline std::int32_t					L_EntriesFound;

	inline std::vector<std::string>		S_NameArray;
	inline std::vector<std::string>		S_TextArray;
	inline std::vector<RE::TESForm*>	S_FormArray;
	inline std::vector<bool>			S_BoolArray;
	inline std::int32_t					S_EntriesTotal;
	inline std::int32_t					S_EntriesFound;

	inline std::vector<std::string> temploc{};
	inline std::vector<std::string> temprod{};
	inline std::vector<RE::TESForm*> tempfsh{};

	enum class FishingMarkerType : int32_t
	{
		kMapMa_A = 0,
		kMapMa_C = 1,
		kMapMa_L = 2,
		kMapMa_S = 3,
	};

	class CHandler
	{
		public: [[nodiscard]] static CHandler* GetSingleton() { static CHandler singleton; return &singleton; }

		static void			InstallFramework();
		static void			UpdateFoundForms();

		static void			ProcessFoundForm(ProcessFoundFormArgs, std::string a_section);
		static bool			ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos, FishingMarkerType a_section, bool from_hook);
		static void			ProcessHookedMarker(const char* nam);
		static void			ProcessCaughtFishFromPapyrus(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static bool			MarkerIsValid(RE::TESObjectREFR* a_marker);

		static void			InjectAndCompileData();;
		static void			InstallSearchTerms();
		static void			BuildFishArrays(RE::TESForm* a_form, std::string a_rod, std::string a_loc);

		static void			AddCACOFishingForms();

		static void			OnBooksReadEvent(RE::BooksRead::Event const* a_event);
		static void			OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);
		static void			OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);
	};

}