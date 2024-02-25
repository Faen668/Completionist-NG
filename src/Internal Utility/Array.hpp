#pragma once
#include "Structs.hpp"

//Frameworks
#include "Frameworks/Items/CFramework_Uniques.hpp"
#include "Frameworks/Items/CFramework_Others.hpp"
#include "Frameworks/Items/CFramework_Books.hpp"
#include "Frameworks/Misc/CFramework_Locations.hpp"
#include "Frameworks/Misc/CFramework_Blessings.hpp"
#include "Frameworks/Misc/CFramework_Enchantments.hpp"
#include "Frameworks/Misc/CFramework_Pets.hpp"
#include "Frameworks/Misc/CFramework_PlayerHomes.hpp"
#include "Frameworks/Misc/CFramework_Shouts.hpp"

//Custom Patches
#include "Internal Utility/PatchListener.hpp"

namespace ArrayHolder {
	using namespace Serialization;

	enum class FrameworkID : std::int32_t
	{
		//Items (Uniques)
		kFramework_ARM = 0,  // Unique Armor (Vanilla)
		kFramework_WPN = 1,  // Unique Weapons (Vanilla)
		kFramework_ITM = 2,  // Unique Items (Vanilla)
		kFramework_JWY = 3,  // Unique Jewelry (Vanilla)
		kFramework_LIQ = 4,  // Unique Liquor (Vanilla)
		kFramework_CCA = 5,  // Unique Armor (Creation Club)
		kFramework_CCI = 6,  // Unique Items (Creation Club)
		kFramework_CCW = 7,  // Unique Weapons (Creation Club)
		kFramework_VDC = 8,  // Dragon Claws (Vanilla)
		kFramework_PDC = 9,  // Dragon Claws (Patches)
		kFramework_VDM = 10, // Dragon Priest Masks (Vanilla)
		kFramework_PDM = 11, // Dragon Priest Masks (Patches)
		kFramework_SBH = 12, // Stones of Barenziah
		kFramework_BAG = 13, // Books (A-G)
		kFramework_BHS = 14, // Books (H-S)
		kFramework_BTY = 15, // Books (T-Y)
		kFramework_SSK = 16, // Skill (Skyrim)
		kFramework_T0A = 17, // Tomes (Skyrim)
		kFramework_BDG = 18, // Books (Dawnguard)
		kFramework_T1A = 19, // Tomes (Dawnguard)
		kFramework_BDB = 20, // Books (Dragonborn)
		kFramework_T2A = 21, // Tomes (Dragonborn)
		kFramework_MSK = 22, // Treasure Maps (Skyrim)
		kFramework_MNT = 23, // Treasure Maps (New Treasure Hunt)
		kFramework_MTH = 24, // Treasure Maps (Treasure Hunter)
		kFramework_CCB = 25, // Books (Creation Club)
		kFramework_T3A = 26, // Tomes (Creation Club)
		kFramework_MAG = 27, // Locations (A-G)
		kFramework_MHR = 28, // Locations (H-R)
		kFramework_MSZ = 29, // Locations (S-Z)
		kFramework_MDG = 30, // Locations (Dawngaurd)
		kFramework_MDB = 31, // Locations (Dragonborn)
		kFramework_MCC = 32, // Locations (Creation Club)
		kFramework_VDS = 33, // Doomstones
		kFramework_VSH = 34, // Shrines	(Vanilla)
		kFramework_WSH = 35, // Shrines (Wintersun)
		kFramework_VAE = 36, // Armor (Vanilla)
		kFramework_SAE = 37, // Armor (Summermyst)
		kFramework_VWE = 38, // Weapons	(Vanilla)
		kFramework_SWE = 39, // Weapons (Summermyst)
		kFramework_VPS = 40, // Pets (Vanilla)
		kFramework_VPH = 44, // Player Homes (Vanilla)
		kFramework_VNS = 47, // Shouts (Vanilla)
		kFramework_TCS = 48, // Shouts (Thunderchild)
		kFramework_MCS = 49, // Shouts (Miscellaneous)
		kFramework_T0C = 50, // Tomes (Skyrim)
		kFramework_T0D = 51, // Tomes (Skyrim)
		kFramework_T0I = 52, // Tomes (Skyrim)
		kFramework_T0R = 53, // Tomes (Skyrim)
		kFramework_T1C = 54, // Tomes (Dawnguard)
		kFramework_T1D = 55, // Tomes (Dawnguard)
		kFramework_T1I = 56, // Tomes (Dawnguard)
		kFramework_T1R = 57, // Tomes (Dawnguard)
		kFramework_T2C = 58, // Tomes (Dragonborn)
		kFramework_T2D = 59, // Tomes (Dragonborn)
		kFramework_T2I = 60, // Tomes (Dragonborn)
		kFramework_T2R = 61, // Tomes (Dragonborn)
		kFramework_T3C = 62, // Tomes (Creation Club)
		kFramework_T3D = 63, // Tomes (Creation Club)
		kFramework_T3I = 64, // Tomes (Creation Club)
		kFramework_T3R = 65, // Tomes (Creation Club)
		kFramework_QSR = 67, // Unique Quest Reward Items (Vanilla)
		kPatch_SUD_I = 325,	 // Skyrim Unique Drinks
		kTotal,
	};

	//Misc Array Containers 
	static std::unordered_map<int32_t, std::vector<std::string>*>	NameSet;
	static std::unordered_map<int32_t, std::vector<std::string>*>	TextSet;
	static std::unordered_map<int32_t, std::vector<RE::TESForm*>*>	FormSet;
	static std::unordered_map<int32_t, std::vector<bool>*>			BoolSet;
	static std::unordered_map<int32_t, CompletionistData*>			DataSet;
	static std::unordered_map<int32_t, std::int32_t*>				CntFSet;
	static std::unordered_map<int32_t, std::int32_t*>				CntTSet;
	static std::unordered_map<int32_t, bool>						NoShow;

	// Empty Return Containers
	static std::vector<RE::TESForm*>	EFormVec{};
	static std::vector<std::int32_t>	ERadiVec{};
	static std::vector<std::string>		ETextVec{};
	static std::vector<bool>			EBoolVec{};
	static CompletionistData			EDataSet{};
	static std::int32_t					EsInt = -1;

	//Counters 
	static std::int32_t i_Nsize = 0;
	static std::int32_t i_Ssize = 0;

	//---------------------------------------------------
	//-- Framework Functions ( Array Getter (MCM Proc) --
	//---------------------------------------------------

	[[nodiscard]] static auto& HandleNameSet(int32_t a_frameworkID) noexcept {
		return (NameSet.contains(a_frameworkID) && NameSet.at(a_frameworkID)) ? *NameSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleTextSet(int32_t a_frameworkID) noexcept {
		return (TextSet.contains(a_frameworkID) && TextSet.at(a_frameworkID)) ? *TextSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleBoolSet(int32_t a_frameworkID) noexcept {
		return (BoolSet.contains(a_frameworkID) && BoolSet.at(a_frameworkID)) ? *BoolSet.at(a_frameworkID) : EBoolVec;
	}

	[[nodiscard]] static auto& HandleFormSet(int32_t a_frameworkID) noexcept {
		return (FormSet.contains(a_frameworkID) && FormSet.at(a_frameworkID)) ? *FormSet.at(a_frameworkID) : EFormVec;
	}

	[[nodiscard]] static auto& HandleFoundSet(int32_t a_frameworkID) noexcept {
		return (CntFSet.contains(a_frameworkID) && CntFSet.at(a_frameworkID)) ? *CntFSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleTotalSet(int32_t a_frameworkID) noexcept {
		return (CntTSet.contains(a_frameworkID) && CntTSet.at(a_frameworkID)) ? *CntTSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleDataSet(int32_t a_frameworkID) noexcept {
		return (DataSet.contains(a_frameworkID) && DataSet.at(a_frameworkID)) ? *DataSet.at(a_frameworkID) : EDataSet;
	}

	[[nodiscard]] static auto HandleNoShow(int32_t a_frameworkID) noexcept {
		return (NoShow.contains(a_frameworkID) && NoShow.at(a_frameworkID)) ? NoShow.at(a_frameworkID) : false;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add To Unordered Map  ) --
	//---------------------------------------------------
	
	static void AttemptToAdd(int32_t a_FrameworkID, std::vector<std::string>* a_names, std::vector<std::string>* a_texts, std::vector<bool>* a_bools, std::vector<RE::TESForm*>* a_forms, std::int32_t* a_found, std::int32_t* a_total, CompletionistData* a_data, bool a_NoShow = false) noexcept
	{
		NameSet.try_emplace(a_FrameworkID, a_names);
		TextSet.try_emplace(a_FrameworkID, a_texts);
		BoolSet.try_emplace(a_FrameworkID, a_bools);
		FormSet.try_emplace(a_FrameworkID, a_forms);
		CntFSet.try_emplace(a_FrameworkID, a_found);
		CntTSet.try_emplace(a_FrameworkID, a_total);
		DataSet.try_emplace(a_FrameworkID, a_data);
		NoShow.try_emplace(a_FrameworkID, a_NoShow);
		
		i_Ssize += 1;
		if (a_NoShow) { i_Nsize += 1; }
	}

	//---------------------------------------------------
	//-- Framework Functions ( Array Registrations ) ----
	//---------------------------------------------------

	static void RegisterArrays() noexcept 
	{
		//Frameworks - Items (Uniques) (Start) --------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_ARM), &CFramework_Uniques::A_NameArray, &CFramework_Uniques::A_TextArray, &CFramework_Uniques::A_BoolArray, &CFramework_Uniques::A_FormArray, &CFramework_Uniques::A_EntriesFound, &CFramework_Uniques::A_EntriesTotal, &CFramework_Uniques::ItemDataA);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_WPN), &CFramework_Uniques::W_NameArray, &CFramework_Uniques::W_TextArray, &CFramework_Uniques::W_BoolArray, &CFramework_Uniques::W_FormArray, &CFramework_Uniques::W_EntriesFound, &CFramework_Uniques::W_EntriesTotal, &CFramework_Uniques::ItemDataW);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_ITM), &CFramework_Uniques::I_NameArray, &CFramework_Uniques::I_TextArray, &CFramework_Uniques::I_BoolArray, &CFramework_Uniques::I_FormArray, &CFramework_Uniques::I_EntriesFound, &CFramework_Uniques::I_EntriesTotal, &CFramework_Uniques::ItemDataI);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_JWY), &CFramework_Uniques::J_NameArray, &CFramework_Uniques::J_TextArray, &CFramework_Uniques::J_BoolArray, &CFramework_Uniques::J_FormArray, &CFramework_Uniques::J_EntriesFound, &CFramework_Uniques::J_EntriesTotal, &CFramework_Uniques::ItemDataJ);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_LIQ), &CFramework_Uniques::L_NameArray, &CFramework_Uniques::L_TextArray, &CFramework_Uniques::L_BoolArray, &CFramework_Uniques::L_FormArray, &CFramework_Uniques::L_EntriesFound, &CFramework_Uniques::L_EntriesTotal, &CFramework_Uniques::ItemDataL);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_QSR), &CFramework_Uniques::Q_NameArray, &CFramework_Uniques::Q_TextArray, &CFramework_Uniques::Q_BoolArray, &CFramework_Uniques::Q_FormArray, &CFramework_Uniques::Q_EntriesFound, &CFramework_Uniques::Q_EntriesTotal, &CFramework_Uniques::ItemDataQ);

		//Frameworks - Items (Others) (Start) ---------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VDC), &CFramework_Others::VC_NameArray, &CFramework_Others::VC_TextArray, &CFramework_Others::VC_BoolArray, &CFramework_Others::VC_FormArray, &CFramework_Others::VC_EntriesFound, &CFramework_Others::VC_EntriesTotal, &CFramework_Others::ItemDataVC);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_PDC), &CFramework_Others::PC_NameArray, &CFramework_Others::PC_TextArray, &CFramework_Others::PC_BoolArray, &CFramework_Others::PC_FormArray, &CFramework_Others::PC_EntriesFound, &CFramework_Others::PC_EntriesTotal, &CFramework_Others::ItemDataPC);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VDM), &CFramework_Others::VM_NameArray, &CFramework_Others::VM_TextArray, &CFramework_Others::VM_BoolArray, &CFramework_Others::VM_FormArray, &CFramework_Others::VM_EntriesFound, &CFramework_Others::VM_EntriesTotal, &CFramework_Others::ItemDataVM);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_PDM), &CFramework_Others::PM_NameArray, &CFramework_Others::PM_TextArray, &CFramework_Others::PM_BoolArray, &CFramework_Others::PM_FormArray, &CFramework_Others::PM_EntriesFound, &CFramework_Others::PM_EntriesTotal, &CFramework_Others::ItemDataPM);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_SBH), &CFramework_Others::SB_NameArray, &CFramework_Others::SB_TextArray, &CFramework_Others::SB_BoolArray, &CFramework_Others::SB_FormArray, &CFramework_Others::SB_EntriesFound, &CFramework_Others::SB_EntriesTotal, &CFramework_Others::ItemDataSB, true);

		//Frameworks - Items (Books) (Start) ----------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_BAG), &CFramework_Books::Books_AG_NameArray, &CFramework_Books::Books_AG_TextArray, &CFramework_Books::Books_AG_BoolArray, &CFramework_Books::Books_AG_FormArray, &CFramework_Books::Books_AG_EntriesFound, &CFramework_Books::Books_AG_EntriesTotal, &CFramework_Books_AG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_BHS), &CFramework_Books::Books_HS_NameArray, &CFramework_Books::Books_HS_TextArray, &CFramework_Books::Books_HS_BoolArray, &CFramework_Books::Books_HS_FormArray, &CFramework_Books::Books_HS_EntriesFound, &CFramework_Books::Books_HS_EntriesTotal, &CFramework_Books_HS::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_BTY), &CFramework_Books::Books_TY_NameArray, &CFramework_Books::Books_TY_TextArray, &CFramework_Books::Books_TY_BoolArray, &CFramework_Books::Books_TY_FormArray, &CFramework_Books::Books_TY_EntriesFound, &CFramework_Books::Books_TY_EntriesTotal, &CFramework_Books_TY::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_SSK), &CFramework_Books::Skill_SK_NameArray, &CFramework_Books::Skill_SK_TextArray, &CFramework_Books::Skill_SK_BoolArray, &CFramework_Books::Skill_SK_FormArray, &CFramework_Books::Skill_SK_EntriesFound, &CFramework_Books::Skill_SK_EntriesTotal, &CFramework_Skill_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_BDG), &CFramework_Books::Books_DG_NameArray, &CFramework_Books::Books_DG_TextArray, &CFramework_Books::Books_DG_BoolArray, &CFramework_Books::Books_DG_FormArray, &CFramework_Books::Books_DG_EntriesFound, &CFramework_Books::Books_DG_EntriesTotal, &CFramework_Books_DG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_BDB), &CFramework_Books::Books_DB_NameArray, &CFramework_Books::Books_DB_TextArray, &CFramework_Books::Books_DB_BoolArray, &CFramework_Books::Books_DB_FormArray, &CFramework_Books::Books_DB_EntriesFound, &CFramework_Books::Books_DB_EntriesTotal, &CFramework_Books_DB::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MSK), &CFramework_Books::TMaps_SK_NameArray, &CFramework_Books::TMaps_SK_TextArray, &CFramework_Books::TMaps_SK_BoolArray, &CFramework_Books::TMaps_SK_FormArray, &CFramework_Books::TMaps_SK_EntriesFound, &CFramework_Books::TMaps_SK_EntriesTotal, &CFramework_TMaps_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MNT), &CFramework_Books::TMaps_NT_NameArray, &CFramework_Books::TMaps_NT_TextArray, &CFramework_Books::TMaps_NT_BoolArray, &CFramework_Books::TMaps_NT_FormArray, &CFramework_Books::TMaps_NT_EntriesFound, &CFramework_Books::TMaps_NT_EntriesTotal, &CFramework_TMaps_NT::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MTH), &CFramework_Books::TMaps_TH_NameArray, &CFramework_Books::TMaps_TH_TextArray, &CFramework_Books::TMaps_TH_BoolArray, &CFramework_Books::TMaps_TH_FormArray, &CFramework_Books::TMaps_TH_EntriesFound, &CFramework_Books::TMaps_TH_EntriesTotal, &CFramework_TMaps_TH::Data);

		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T0A), &CFramework_Books::Tomes_SK_A_NameArray, &CFramework_Books::Tomes_SK_A_TextArray, &CFramework_Books::Tomes_SK_A_BoolArray, &CFramework_Books::Tomes_SK_A_FormArray, &CFramework_Books::Tomes_SK_A_EntriesFound, &CFramework_Books::Tomes_SK_A_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T0C), &CFramework_Books::Tomes_SK_C_NameArray, &CFramework_Books::Tomes_SK_C_TextArray, &CFramework_Books::Tomes_SK_C_BoolArray, &CFramework_Books::Tomes_SK_C_FormArray, &CFramework_Books::Tomes_SK_C_EntriesFound, &CFramework_Books::Tomes_SK_C_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T0D), &CFramework_Books::Tomes_SK_D_NameArray, &CFramework_Books::Tomes_SK_D_TextArray, &CFramework_Books::Tomes_SK_D_BoolArray, &CFramework_Books::Tomes_SK_D_FormArray, &CFramework_Books::Tomes_SK_D_EntriesFound, &CFramework_Books::Tomes_SK_D_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T0I), &CFramework_Books::Tomes_SK_I_NameArray, &CFramework_Books::Tomes_SK_I_TextArray, &CFramework_Books::Tomes_SK_I_BoolArray, &CFramework_Books::Tomes_SK_I_FormArray, &CFramework_Books::Tomes_SK_I_EntriesFound, &CFramework_Books::Tomes_SK_I_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T0R), &CFramework_Books::Tomes_SK_R_NameArray, &CFramework_Books::Tomes_SK_R_TextArray, &CFramework_Books::Tomes_SK_R_BoolArray, &CFramework_Books::Tomes_SK_R_FormArray, &CFramework_Books::Tomes_SK_R_EntriesFound, &CFramework_Books::Tomes_SK_R_EntriesTotal, &CFramework_Tomes_SK::Data);

		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T1A), &CFramework_Books::Tomes_DG_A_NameArray, &CFramework_Books::Tomes_DG_A_TextArray, &CFramework_Books::Tomes_DG_A_BoolArray, &CFramework_Books::Tomes_DG_A_FormArray, &CFramework_Books::Tomes_DG_A_EntriesFound, &CFramework_Books::Tomes_DG_A_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T1C), &CFramework_Books::Tomes_DG_C_NameArray, &CFramework_Books::Tomes_DG_C_TextArray, &CFramework_Books::Tomes_DG_C_BoolArray, &CFramework_Books::Tomes_DG_C_FormArray, &CFramework_Books::Tomes_DG_C_EntriesFound, &CFramework_Books::Tomes_DG_C_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T1D), &CFramework_Books::Tomes_DG_D_NameArray, &CFramework_Books::Tomes_DG_D_TextArray, &CFramework_Books::Tomes_DG_D_BoolArray, &CFramework_Books::Tomes_DG_D_FormArray, &CFramework_Books::Tomes_DG_D_EntriesFound, &CFramework_Books::Tomes_DG_D_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T1I), &CFramework_Books::Tomes_DG_I_NameArray, &CFramework_Books::Tomes_DG_I_TextArray, &CFramework_Books::Tomes_DG_I_BoolArray, &CFramework_Books::Tomes_DG_I_FormArray, &CFramework_Books::Tomes_DG_I_EntriesFound, &CFramework_Books::Tomes_DG_I_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T1R), &CFramework_Books::Tomes_DG_R_NameArray, &CFramework_Books::Tomes_DG_R_TextArray, &CFramework_Books::Tomes_DG_R_BoolArray, &CFramework_Books::Tomes_DG_R_FormArray, &CFramework_Books::Tomes_DG_R_EntriesFound, &CFramework_Books::Tomes_DG_R_EntriesTotal, &CFramework_Tomes_DG::Data);

		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T2A), &CFramework_Books::Tomes_DB_A_NameArray, &CFramework_Books::Tomes_DB_A_TextArray, &CFramework_Books::Tomes_DB_A_BoolArray, &CFramework_Books::Tomes_DB_A_FormArray, &CFramework_Books::Tomes_DB_A_EntriesFound, &CFramework_Books::Tomes_DB_A_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T2C), &CFramework_Books::Tomes_DB_C_NameArray, &CFramework_Books::Tomes_DB_C_TextArray, &CFramework_Books::Tomes_DB_C_BoolArray, &CFramework_Books::Tomes_DB_C_FormArray, &CFramework_Books::Tomes_DB_C_EntriesFound, &CFramework_Books::Tomes_DB_C_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T2D), &CFramework_Books::Tomes_DB_D_NameArray, &CFramework_Books::Tomes_DB_D_TextArray, &CFramework_Books::Tomes_DB_D_BoolArray, &CFramework_Books::Tomes_DB_D_FormArray, &CFramework_Books::Tomes_DB_D_EntriesFound, &CFramework_Books::Tomes_DB_D_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T2I), &CFramework_Books::Tomes_DB_I_NameArray, &CFramework_Books::Tomes_DB_I_TextArray, &CFramework_Books::Tomes_DB_I_BoolArray, &CFramework_Books::Tomes_DB_I_FormArray, &CFramework_Books::Tomes_DB_I_EntriesFound, &CFramework_Books::Tomes_DB_I_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_T2R), &CFramework_Books::Tomes_DB_R_NameArray, &CFramework_Books::Tomes_DB_R_TextArray, &CFramework_Books::Tomes_DB_R_BoolArray, &CFramework_Books::Tomes_DB_R_FormArray, &CFramework_Books::Tomes_DB_R_EntriesFound, &CFramework_Books::Tomes_DB_R_EntriesTotal, &CFramework_Tomes_DB::Data);

		//Frameworks - Misc (Locations) (Start) -------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MAG), &CFramework_MapMa::MapMa_AG_NameArray, &CFramework_MapMa::MapMa_AG_TextArray, &CFramework_MapMa::MapMa_AG_BoolArray, &CFramework_MapMa::MapMa_AG_FormArray, &CFramework_MapMa::MapMa_AG_EntriesFound, &CFramework_MapMa::MapMa_AG_EntriesTotal, &CFramework_MapMa_AG::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MHR), &CFramework_MapMa::MapMa_HR_NameArray, &CFramework_MapMa::MapMa_HR_TextArray, &CFramework_MapMa::MapMa_HR_BoolArray, &CFramework_MapMa::MapMa_HR_FormArray, &CFramework_MapMa::MapMa_HR_EntriesFound, &CFramework_MapMa::MapMa_HR_EntriesTotal, &CFramework_MapMa_HR::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MSZ), &CFramework_MapMa::MapMa_SZ_NameArray, &CFramework_MapMa::MapMa_SZ_TextArray, &CFramework_MapMa::MapMa_SZ_BoolArray, &CFramework_MapMa::MapMa_SZ_FormArray, &CFramework_MapMa::MapMa_SZ_EntriesFound, &CFramework_MapMa::MapMa_SZ_EntriesTotal, &CFramework_MapMa_SZ::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MDG), &CFramework_MapMa::MapMa_DG_NameArray, &CFramework_MapMa::MapMa_DG_TextArray, &CFramework_MapMa::MapMa_DG_BoolArray, &CFramework_MapMa::MapMa_DG_FormArray, &CFramework_MapMa::MapMa_DG_EntriesFound, &CFramework_MapMa::MapMa_DG_EntriesTotal, &CFramework_MapMa_DG::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MDB), &CFramework_MapMa::MapMa_DB_NameArray, &CFramework_MapMa::MapMa_DB_TextArray, &CFramework_MapMa::MapMa_DB_BoolArray, &CFramework_MapMa::MapMa_DB_FormArray, &CFramework_MapMa::MapMa_DB_EntriesFound, &CFramework_MapMa::MapMa_DB_EntriesTotal, &CFramework_Tomes_DB::Data, true);

		//Frameworks - Misc (Blessings) (Start) -------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VDS), &CFramework_Blessings::DS_NameArray, &CFramework_Blessings::DS_TextArray, &CFramework_Blessings::DS_BoolArray, &CFramework_Blessings::DS_FormArray, &CFramework_Blessings::DS_EntriesFound, &CFramework_Blessings::DS_EntriesTotal, &CFramework_Blessings_DS::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VSH), &CFramework_Blessings::VS_NameArray, &CFramework_Blessings::VS_TextArray, &CFramework_Blessings::VS_BoolArray, &CFramework_Blessings::VS_FormArray, &CFramework_Blessings::VS_EntriesFound, &CFramework_Blessings::VS_EntriesTotal, &CFramework_Blessings_VS::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_WSH), &CFramework_Blessings::WS_NameArray, &CFramework_Blessings::WS_TextArray, &CFramework_Blessings::WS_BoolArray, &CFramework_Blessings::WS_FormArray, &CFramework_Blessings::WS_EntriesFound, &CFramework_Blessings::WS_EntriesTotal, &CFramework_Blessings_WS::Data, true);

		//Frameworks - Misc (Enchantments) (Start) ----------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VAE), &CFramework_Enchantments::VA_NameArray, &CFramework_Enchantments::VA_TextArray, &CFramework_Enchantments::VA_BoolArray, &CFramework_Enchantments::VA_FormArray, &CFramework_Enchantments::VA_EntriesFound, &CFramework_Enchantments::VA_EntriesTotal, &CFramework_Enchantments_VA::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_SAE), &CFramework_Enchantments::SA_NameArray, &CFramework_Enchantments::SA_TextArray, &CFramework_Enchantments::SA_BoolArray, &CFramework_Enchantments::SA_FormArray, &CFramework_Enchantments::SA_EntriesFound, &CFramework_Enchantments::SA_EntriesTotal, &CFramework_Enchantments_SA::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VWE), &CFramework_Enchantments::VW_NameArray, &CFramework_Enchantments::VW_TextArray, &CFramework_Enchantments::VW_BoolArray, &CFramework_Enchantments::VW_FormArray, &CFramework_Enchantments::VW_EntriesFound, &CFramework_Enchantments::VW_EntriesTotal, &CFramework_Enchantments_VW::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_SWE), &CFramework_Enchantments::SW_NameArray, &CFramework_Enchantments::SW_TextArray, &CFramework_Enchantments::SW_BoolArray, &CFramework_Enchantments::SW_FormArray, &CFramework_Enchantments::SW_EntriesFound, &CFramework_Enchantments::SW_EntriesTotal, &CFramework_Enchantments_SW::Data, true);

		//Frameworks - Misc (Pets) (Start) ------------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VPS), &CFramework_Pets::Pets_VP_NameArray, &CFramework_Pets::Pets_VP_TextArray, &CFramework_Pets::Pets_VP_BoolArray, &CFramework_Pets::Pets_VP_FormArray, &CFramework_Pets::Pets_VP_EntriesFound, &CFramework_Pets::Pets_VP_EntriesTotal, &CFramework_Pets_VP::Data, true);

		//Frameworks - Misc (Player Homes) (Start) ----------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VPH), &CFramework_PlayerHomes::VH_NameArray, &CFramework_PlayerHomes::VH_TextArray, &CFramework_PlayerHomes::VH_BoolArray, &CFramework_PlayerHomes::VH_FormArray, &CFramework_PlayerHomes::VH_EntriesFound, &CFramework_PlayerHomes::VH_EntriesTotal, &CFramework_PlayerHomes_VH::Data, true);

		//Frameworks - Misc (Shouts) (Start) ----------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_VNS), &CFramework_Shouts::Vanilla_SH_NameOutput, &CFramework_Shouts::Vanilla_SH_TextArray, &CFramework_Shouts::Vanilla_SH_BoolArray, &CFramework_Shouts::Vanilla_SH_FormArray, &CFramework_Shouts::Vanilla_SH_EntriesFound, &CFramework_Shouts::Vanilla_SH_EntriesTotal, &CFramework_Shouts_VS::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_TCS), &CFramework_Shouts::Thunderchild_SH_NameOutput, &CFramework_Shouts::Thunderchild_SH_TextArray, &CFramework_Shouts::Thunderchild_SH_BoolArray, &CFramework_Shouts::Thunderchild_SH_FormArray, &CFramework_Shouts::Thunderchild_SH_EntriesFound, &CFramework_Shouts::Thunderchild_SH_EntriesTotal, &CFramework_Shouts_TS::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MCS), &CFramework_Shouts::Miscellaneous_SH_NameOutput, &CFramework_Shouts::Miscellaneous_SH_TextArray, &CFramework_Shouts::Miscellaneous_SH_BoolArray, &CFramework_Shouts::Miscellaneous_SH_FormArray, &CFramework_Shouts::Miscellaneous_SH_EntriesFound, &CFramework_Shouts::Miscellaneous_SH_EntriesTotal, &CFramework_Shouts_TS::Data, true);

		//---------------
		//-- Patches ----
		//---------------

		AttemptToAdd(std::to_underlying(FrameworkID::kPatch_SUD_I), &CFramework_Uniques::SUDS_NameArray, &CFramework_Uniques::SUDS_TextArray, &CFramework_Uniques::SUDS_BoolArray, &CFramework_Uniques::SUDS_FormArray, &CFramework_Uniques::SUDS_EntriesFound, &CFramework_Uniques::SUDS_EntriesTotal, &CFramework_Uniques::ItemDataSUDS);

		//Custom Patches
		for (auto& [groupName, group] : CExternalPatchHandler::Get()) {
			for (auto& [pageName, patchData] : group->GetPatches()) {
				for (auto& section : patchData->type_sections) {
					AttemptToAdd(section.ID, section.type == CMiscPatchType::kShouts ? &section.shout_names : &section.names, &section.texts, &section.bools, &section.forms, &section.found, &section.total, &section.data, static_cast<int32_t>(section.type) > 1);
				};
			};
		};
		INFO("Registered {} Framework Arrays - {} Hidden as NoShow and {} Merged as Collectable", i_Ssize, i_Nsize, (i_Ssize - i_Nsize));
	}
}