#pragma once
#include "Serialization.hpp"

//Frameworks
#include "Frameworks/Items/CFramework_Uniques.hpp"
#include "Frameworks/Items/CFramework_Others.hpp"
#include "Frameworks/Items/CFramework_Books.hpp"
#include "Frameworks/Misc/CFramework_Locations.hpp"
#include "Frameworks/Misc/CFramework_Blessings.hpp"
#include "Frameworks/Misc/CFramework_Enchantments.hpp"

//Patches
#include "Patches/AdditionalHearthfireDolls/CFramework_AHD.hpp"
#include "Patches/BrotherhoodOfOld/CFramework_BOO.hpp"
#include "Patches/Clockwork/CFramework_CLW.hpp"
#include "Patches/Falskaar/CFramework_FSK.hpp"
#include "Patches/FossilMining/CFramework_FOS.hpp"
#include "Patches/Fishing/CFramework_Fishing.hpp"
#include "Patches/GrayCowlOfNocturnal/CFramework_GCN.hpp"
#include "Patches/OblivionArtifacts/CFramework_OAP.hpp"
#include "Patches/HelgenReborn/CFramework_HRB.hpp"
#include "Patches/InterestingNPCs/CFramework_3DC.hpp"
#include "Patches/MoonAndStar/CFramework_MAS.hpp"
#include "Patches/MoonpathToElsweyr/CFramework_MTE.hpp"
#include "Patches/ProjectAHO/CFramework_AHO.hpp"
#include "Patches/SkyrimUniqueTreasures/CFramework_ST1.hpp"
#include "Patches/SkyrimUniqueTreasures/CFramework_ST2.hpp"
#include "Patches/SkyrimUniqueTreasures/CFramework_ST3.hpp"
#include "Patches/TeldrynSerious/CFramework_TEL.hpp"
#include "Patches/Thunderchild/CFramework_THU.hpp"
#include "Patches/Undeath/CFramework_UND.hpp"
#include "Patches/WheelsOfLull/CFramework_WOL.hpp"
#include "Patches/Wintersun/CFramework_WSN.hpp"
#include "Patches/Wyrmstooth/CFramework_WYR.hpp"
#include "Patches/Vigilant/CFramework_VIG.hpp"

namespace ArrayHolder {
	using namespace Serialization;

	enum class FrameworkID : std::int32_t
	{
		//Items (Uniques)
		kFramework_ARM = 0, // Unique Armor (Vanilla)
		kFramework_WPN = 1, // Unique Weapons (Vanilla)
		kFramework_ITM = 2, // Unique Items (Vanilla)
		kFramework_JWY = 3, // Unique Jewelry (Vanilla)
		kFramework_LIQ = 4, // Unique Liquor (Vanilla)

		kFramework_CCA = 5, // Unique Armor (Creation Club)
		kFramework_CCI = 6, // Unique Items (Creation Club)
		kFramework_CCW = 7, // Unique Weapons (Creation Club)

		//Items (Others)
		kFramework_VDC = 8, // Dragon Claws (Vanilla)
		kFramework_PDC = 9, // Dragon Claws (Patches)
		kFramework_VDM = 10, // Dragon Priest Masks (Vanilla)
		kFramework_PDM = 11, // Dragon Priest Masks (Patches)
		kFramework_SBH = 12, // Stones of Barenziah

		//Items (Books)
		kFramework_BAG = 13, // Books (A-G)
		kFramework_BHS = 14, // Books (H-S)
		kFramework_BTY = 15, // Books (T-Y)
		kFramework_SSK = 16, // Skill (Skyrim)
		kFramework_TSK = 17, // Tomes (Skyrim)
		kFramework_BDG = 18, // Books (Dawnguard)
		kFramework_TDG = 19, // Tomes (Dawnguard)
		kFramework_BDB = 20, // Books (Dragonborn)
		kFramework_TDB = 21, // Tomes (Dragonborn)
		kFramework_MSK = 22, // Treasure Maps (Skyrim)
		kFramework_MNT = 23, // Treasure Maps (New Treasure Hunt)
		kFramework_MTH = 24, // Treasure Maps (Treasure Hunter)
		kFramework_CCB = 25, // Books (Creation Club)
		kFramework_CCT = 26, // Tomes (Creation Club)

		//Misc (Locations)
		kFramework_MAG = 27, // Locations (A-G)
		kFramework_MHR = 28, // Locations (H-R)
		kFramework_MSZ = 29, // Locations (S-Z)
		kFramework_MDG = 30, // Locations (Dawngaurd)
		kFramework_MDB = 31, // Locations (Dragonborn)
		kFramework_MCC = 32, // Locations (Creation Club)

		//Misc (Blessings)
		kFramework_VDS = 33, // Doomstones
		kFramework_VSH = 34, // Shrines	(Vanilla)
		kFramework_WSH = 35, // Shrines (Wintersun)

		//Misc (Enchantments)
		kFramework_VAE = 36, // Armor (Vanilla)
		kFramework_SAE = 37, // Armor (Summermyst)
		kFramework_VWE = 38, // Weapons	(Vanilla)
		kFramework_SWE = 39, // Weapons (Summermyst)

		//Patches
		kPatch_AHD_I = 200, // Additional Hearthfire Dolls

		kPatch_BOO_I = 201, // Brotherhood of Old {Items)
		kPatch_BOO_B = 202, // Brotherhood of Old {Books)
		kPatch_BOO_L = 203, // Brotherhood of Old {Locations)

		kPatch_CLW_I = 204, // Clockwork {Items)
		kPatch_CLW_B = 205,	// Clockwork {Books)
		kPatch_CLW_L = 206,	// Clockwork {Locations)

		kPatch_FSK_I = 207, // Falskaar {Items)
		kPatch_FSK_B = 208,	// Falskaar {Books)
		kPatch_FSK_L = 209,	// Falskaar {Locations)

		kPatch_FOS_1 = 210, // Fossil Mining {Common)
		kPatch_FOS_2 = 211,	// Fossil Mining {Uncommon)
		kPatch_FOS_3 = 212,	// Fossil Mining {Rare)

		kPatch_GCN_I = 213, // Gray Cowl Of Nocturnal {Items)
		kPatch_GCN_B = 214,	// Gray Cowl Of Nocturnal {Books)
		kPatch_GCN_L = 215,	// Gray Cowl Of Nocturnal {Locations)

		kPatch_OAP_I = 216, // Oblivion Artifacts {Items)
		kPatch_OAP_B = 217,	// Oblivion Artifacts {Books)
		kPatch_OAP_L = 218,	// Oblivion Artifacts {Locations)

		kPatch_HRB_I = 219, // Helgen Reborn {Items)
		kPatch_HRB_B = 220,	// Helgen Reborn {Books)
		kPatch_HRB_L = 221,	// Helgen Reborn {Locations)

		kPatch_3DC_I = 222, // Interesting NPC's {Items)
		kPatch_3DC_B = 223,	// Interesting NPC's {Books)
		kPatch_3DC_L = 224,	// Interesting NPC's {Locations)

		kPatch_MAS_I = 225, // Moon and Star {Items)
		kPatch_MAS_B = 226,	// Moon and Star {Books)
		kPatch_MAS_L = 227,	// Moon and Star {Locations)

		kPatch_MTE_I = 228, // Moonpath to Elsweyr {Items)
		kPatch_MTE_B = 229,	// Moonpath to Elsweyr {Books)

		kPatch_AHO_I = 230, // Project AHO {Items)
		kPatch_AHO_B = 231,	// Project AHO {Books)
		kPatch_AHO_L = 232,	// Project AHO {Locations)

		kPatch_ST1_1 = 233, // Skyrim Unique Treasures (Jade Ornaments)
		kPatch_ST1_2 = 234, // Skyrim Unique Treasures (Unique Plates)
		kPatch_ST1_3 = 235, // Skyrim Unique Treasures (Figurines & Models)
		kPatch_ST1_4 = 236, // Skyrim Unique Treasures (Musical Instruments)
							   
		kPatch_ST2_1 = 237, // Skyrim Unique Treasures (Skulls)
		kPatch_ST2_2 = 238,	// Skyrim Unique Treasures (Glass Ornaments)
		kPatch_ST2_3 = 239,	// Skyrim Unique Treasures (Gems & Stones)
		kPatch_ST2_4 = 240,	// Skyrim Unique Treasures (Wine Glasses)
							   
		kPatch_ST3_1 = 241, // Skyrim Unique Treasures (Armor & Weapons)
		kPatch_ST3_2 = 242, // Skyrim Unique Treasures (Alcohol)
		kPatch_ST3_3 = 243,	// Skyrim Unique Treasures (Gold Ornaments)
		kPatch_ST3_4 = 244,	// Skyrim Unique Treasures (Copper Ornaments)

		kPatch_FSH_F = 245, // Fishing {Fish)
		kPatch_FSH_I = 246,	// Fishing {Items)
		kPatch_FSH_B = 247,	// Fishing {Books)
		kPatch_FSH_A = 248,	// Fishing {Locations - Arctic)
		kPatch_FSH_C = 249,	// Fishing {Locations - Caves)
		kPatch_FSH_L = 250,	// Fishing {Locations - Lakes)
		kPatch_FSH_S = 251,	// Fishing {Locations - Streams)

		kPatch_TEL_I = 252, // Teldryn Serious {Items)
		kPatch_TEL_B = 253,	// Teldryn Serious {Books)
		kPatch_TEL_L = 254,	// Teldryn Serious {Locations)

		kPatch_THU_I = 255, // Thunderchild {Items)
		kPatch_THU_B = 256,	// Thunderchild {Books)
		kPatch_THU_L = 257,	// Thunderchild {Locations)

		kPatch_UND_B = 258, // Undeath {Books)
		kPatch_UND_L = 259,	// Undeath {Locations)

		kPatch_WOL_I = 260, // Wheels of Lull {Items)
		kPatch_WOL_B = 261,	// Wheels of Lull {Books)
		kPatch_WOL_L = 262,	// Wheels of Lull {Locations)

		kPatch_WSN_I = 263, // Wintersun {Items)
		kPatch_WSN_B = 264,	// Wintersun {Books)
		kPatch_WSN_L = 265,	// Wintersun {Locations)

		kPatch_WYR_I = 266, // Wyrmstooth {Items)
		kPatch_WYR_B = 267,	// Wyrmstooth {Books)
		kPatch_WYR_L = 268,	// Wyrmstooth {Locations)

		kPatch_VIG_I = 269, // Vigilant {Items)
		kPatch_VIG_B = 270,	// Vigilant {Books)
		kPatch_VIG_L = 271,	// Vigilant {Locations)

		kTotal,
	};

	//Main Array Containers 
	static std::unordered_map<FrameworkID, std::vector<std::string>*>	NameSet;
	static std::unordered_map<FrameworkID, std::vector<std::string>*>	TextSet;
	static std::unordered_map<FrameworkID, std::vector<RE::TESForm*>*>	FormSet;
	static std::unordered_map<FrameworkID, std::vector<bool>*>			BoolSet;
	static std::unordered_map<FrameworkID, CompletionistData*>			DataSet;
	static std::unordered_map<FrameworkID, std::int32_t*>				CntFSet;
	static std::unordered_map<FrameworkID, std::int32_t*>				CntTSet;
	static std::unordered_map<FrameworkID, bool>						NoShow;
	
	// Empty Return Containers
	static std::vector<RE::TESForm*>	EFormVec{};
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

	[[nodiscard]] static auto& HandleNameSet(FrameworkID a_frameworkID) noexcept {

		return (NameSet.find(a_frameworkID) != NameSet.end() && NameSet.at(a_frameworkID) != nullptr) ? *NameSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleTextSet(FrameworkID a_frameworkID) noexcept {

		return (TextSet.find(a_frameworkID) != TextSet.end() && TextSet.at(a_frameworkID) != nullptr) ? *TextSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleBoolSet(FrameworkID a_frameworkID) noexcept {

		return (BoolSet.find(a_frameworkID) != BoolSet.end() && BoolSet.at(a_frameworkID) != nullptr) ? *BoolSet.at(a_frameworkID) : EBoolVec;
	}

	[[nodiscard]] static auto& HandleFormSet(FrameworkID a_frameworkID) noexcept {

		return (FormSet.find(a_frameworkID) != FormSet.end() && FormSet.at(a_frameworkID) != nullptr) ? *FormSet.at(a_frameworkID) : EFormVec;
	}

	[[nodiscard]] static auto& HandleFoundSet(FrameworkID a_frameworkID) noexcept {

		return (CntFSet.find(a_frameworkID) != CntFSet.end() && CntFSet.at(a_frameworkID) != nullptr) ? *CntFSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleTotalSet(FrameworkID a_frameworkID) noexcept {

		return (CntTSet.find(a_frameworkID) != CntTSet.end() && CntTSet.at(a_frameworkID) != nullptr) ? *CntTSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleDataSet(FrameworkID a_frameworkID) noexcept {

		return (DataSet.find(a_frameworkID) != DataSet.end() && DataSet.at(a_frameworkID) != nullptr) ? *DataSet.at(a_frameworkID) : EDataSet;
	}

	[[nodiscard]] static auto HandleNoShow(FrameworkID a_frameworkID) noexcept {

		return NoShow.find(a_frameworkID) != NoShow.end() ? NoShow.at(a_frameworkID) : false;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add To Unordered Map  ) --
	//---------------------------------------------------
	
	template <FrameworkID a_FrameworkID>
	static void AttemptToAdd(std::vector<std::string>* a_names, std::vector<std::string>* a_texts, std::vector<bool>* a_bools, std::vector<RE::TESForm*>* a_forms, std::int32_t* a_found, std::int32_t* a_total, CompletionistData* a_data, bool a_NoShow = false) noexcept
		requires(a_FrameworkID != FrameworkID::kTotal)
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

	static void RegisterArrays() noexcept {

		//Frameworks - Items (Uniques) (Start) --------------
		AttemptToAdd<FrameworkID::kFramework_ARM>(&CFramework_Uniques::A_NameArray, &CFramework_Uniques::A_TextArray, &CFramework_Uniques::A_BoolArray, &CFramework_Uniques::A_FormArray, &CFramework_Uniques::A_EntriesFound, &CFramework_Uniques::A_EntriesTotal, &CFramework_Uniques_A::Data);
		AttemptToAdd<FrameworkID::kFramework_WPN>(&CFramework_Uniques::W_NameArray, &CFramework_Uniques::W_TextArray, &CFramework_Uniques::W_BoolArray, &CFramework_Uniques::W_FormArray, &CFramework_Uniques::W_EntriesFound, &CFramework_Uniques::W_EntriesTotal, &CFramework_Uniques_W::Data);
		AttemptToAdd<FrameworkID::kFramework_ITM>(&CFramework_Uniques::I_NameArray, &CFramework_Uniques::I_TextArray, &CFramework_Uniques::I_BoolArray, &CFramework_Uniques::I_FormArray, &CFramework_Uniques::I_EntriesFound, &CFramework_Uniques::I_EntriesTotal, &CFramework_Uniques_I::Data);
		AttemptToAdd<FrameworkID::kFramework_JWY>(&CFramework_Uniques::J_NameArray, &CFramework_Uniques::J_TextArray, &CFramework_Uniques::J_BoolArray, &CFramework_Uniques::J_FormArray, &CFramework_Uniques::J_EntriesFound, &CFramework_Uniques::J_EntriesTotal, &CFramework_Uniques_J::Data);
		AttemptToAdd<FrameworkID::kFramework_LIQ>(&CFramework_Uniques::L_NameArray, &CFramework_Uniques::L_TextArray, &CFramework_Uniques::L_BoolArray, &CFramework_Uniques::L_FormArray, &CFramework_Uniques::L_EntriesFound, &CFramework_Uniques::L_EntriesTotal, &CFramework_Uniques_L::Data);

		AttemptToAdd<FrameworkID::kFramework_CCA>(&CFramework_Uniques::CCA_NameArray, &CFramework_Uniques::CCA_TextArray, &CFramework_Uniques::CCA_BoolArray, &CFramework_Uniques::CCA_FormArray, &CFramework_Uniques::CCA_EntriesFound, &CFramework_Uniques::CCA_EntriesTotal, &CFramework_Uniques_CCA::Data);
		AttemptToAdd<FrameworkID::kFramework_CCI>(&CFramework_Uniques::CCI_NameArray, &CFramework_Uniques::CCI_TextArray, &CFramework_Uniques::CCI_BoolArray, &CFramework_Uniques::CCI_FormArray, &CFramework_Uniques::CCI_EntriesFound, &CFramework_Uniques::CCI_EntriesTotal, &CFramework_Uniques_CCI::Data);
		AttemptToAdd<FrameworkID::kFramework_CCW>(&CFramework_Uniques::CCW_NameArray, &CFramework_Uniques::CCW_TextArray, &CFramework_Uniques::CCW_BoolArray, &CFramework_Uniques::CCW_FormArray, &CFramework_Uniques::CCW_EntriesFound, &CFramework_Uniques::CCW_EntriesTotal, &CFramework_Uniques_CCW::Data);

		//Frameworks - Items (Others) (Start) ---------------
		AttemptToAdd<FrameworkID::kFramework_VDC>(&CFramework_Others::VC_NameArray, &CFramework_Others::VC_TextArray, &CFramework_Others::VC_BoolArray, &CFramework_Others::VC_FormArray, &CFramework_Others::VC_EntriesFound, &CFramework_Others::VC_EntriesTotal, &CFramework_Others_VC::Data);
		AttemptToAdd<FrameworkID::kFramework_PDC>(&CFramework_Others::PC_NameArray, &CFramework_Others::PC_TextArray, &CFramework_Others::PC_BoolArray, &CFramework_Others::PC_FormArray, &CFramework_Others::PC_EntriesFound, &CFramework_Others::PC_EntriesTotal, &CFramework_Others_PC::Data);
		AttemptToAdd<FrameworkID::kFramework_VDM>(&CFramework_Others::VM_NameArray, &CFramework_Others::VM_TextArray, &CFramework_Others::VM_BoolArray, &CFramework_Others::VM_FormArray, &CFramework_Others::VM_EntriesFound, &CFramework_Others::VM_EntriesTotal, &CFramework_Others_VM::Data);
		AttemptToAdd<FrameworkID::kFramework_PDM>(&CFramework_Others::PM_NameArray, &CFramework_Others::PM_TextArray, &CFramework_Others::PM_BoolArray, &CFramework_Others::PM_FormArray, &CFramework_Others::PM_EntriesFound, &CFramework_Others::PM_EntriesTotal, &CFramework_Others_PM::Data);
		AttemptToAdd<FrameworkID::kFramework_SBH>(&CFramework_Others::SB_NameArray, &CFramework_Others::SB_TextArray, &CFramework_Others::SB_BoolArray, &CFramework_Others::SB_FormArray, &CFramework_Others::SB_EntriesFound, &CFramework_Others::SB_EntriesTotal, &CFramework_Others_SB::Data, true);

		//Frameworks - Items (Books) (Start) ----------------
		AttemptToAdd<FrameworkID::kFramework_BAG>(&CFramework_Books::Books_AG_NameArray, &CFramework_Books::Books_AG_TextArray, &CFramework_Books::Books_AG_BoolArray, &CFramework_Books::Books_AG_FormArray, &CFramework_Books::Books_AG_EntriesFound, &CFramework_Books::Books_AG_EntriesTotal, &CFramework_Books_AG::Data);
		AttemptToAdd<FrameworkID::kFramework_BHS>(&CFramework_Books::Books_HS_NameArray, &CFramework_Books::Books_HS_TextArray, &CFramework_Books::Books_HS_BoolArray, &CFramework_Books::Books_HS_FormArray, &CFramework_Books::Books_HS_EntriesFound, &CFramework_Books::Books_HS_EntriesTotal, &CFramework_Books_HS::Data);
		AttemptToAdd<FrameworkID::kFramework_BTY>(&CFramework_Books::Books_TY_NameArray, &CFramework_Books::Books_TY_TextArray, &CFramework_Books::Books_TY_BoolArray, &CFramework_Books::Books_TY_FormArray, &CFramework_Books::Books_TY_EntriesFound, &CFramework_Books::Books_TY_EntriesTotal, &CFramework_Books_TY::Data);
		AttemptToAdd<FrameworkID::kFramework_SSK>(&CFramework_Books::Skill_SK_NameArray, &CFramework_Books::Skill_SK_TextArray, &CFramework_Books::Skill_SK_BoolArray, &CFramework_Books::Skill_SK_FormArray, &CFramework_Books::Skill_SK_EntriesFound, &CFramework_Books::Skill_SK_EntriesTotal, &CFramework_Skill_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_TSK>(&CFramework_Books::Tomes_SK_NameArray, &CFramework_Books::Tomes_SK_TextArray, &CFramework_Books::Tomes_SK_BoolArray, &CFramework_Books::Tomes_SK_FormArray, &CFramework_Books::Tomes_SK_EntriesFound, &CFramework_Books::Tomes_SK_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_BDG>(&CFramework_Books::Books_DG_NameArray, &CFramework_Books::Books_DG_TextArray, &CFramework_Books::Books_DG_BoolArray, &CFramework_Books::Books_DG_FormArray, &CFramework_Books::Books_DG_EntriesFound, &CFramework_Books::Books_DG_EntriesTotal, &CFramework_Books_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_TDG>(&CFramework_Books::Tomes_DG_NameArray, &CFramework_Books::Tomes_DG_TextArray, &CFramework_Books::Tomes_DG_BoolArray, &CFramework_Books::Tomes_DG_FormArray, &CFramework_Books::Tomes_DG_EntriesFound, &CFramework_Books::Tomes_DG_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_BDB>(&CFramework_Books::Books_DB_NameArray, &CFramework_Books::Books_DB_TextArray, &CFramework_Books::Books_DB_BoolArray, &CFramework_Books::Books_DB_FormArray, &CFramework_Books::Books_DB_EntriesFound, &CFramework_Books::Books_DB_EntriesTotal, &CFramework_Books_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_TDB>(&CFramework_Books::Tomes_DB_NameArray, &CFramework_Books::Tomes_DB_TextArray, &CFramework_Books::Tomes_DB_BoolArray, &CFramework_Books::Tomes_DB_FormArray, &CFramework_Books::Tomes_DB_EntriesFound, &CFramework_Books::Tomes_DB_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_MSK>(&CFramework_Books::TMaps_SK_NameArray, &CFramework_Books::TMaps_SK_TextArray, &CFramework_Books::TMaps_SK_BoolArray, &CFramework_Books::TMaps_SK_FormArray, &CFramework_Books::TMaps_SK_EntriesFound, &CFramework_Books::TMaps_SK_EntriesTotal, &CFramework_TMaps_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_MNT>(&CFramework_Books::TMaps_NT_NameArray, &CFramework_Books::TMaps_NT_TextArray, &CFramework_Books::TMaps_NT_BoolArray, &CFramework_Books::TMaps_NT_FormArray, &CFramework_Books::TMaps_NT_EntriesFound, &CFramework_Books::TMaps_NT_EntriesTotal, &CFramework_TMaps_NT::Data);
		AttemptToAdd<FrameworkID::kFramework_MTH>(&CFramework_Books::TMaps_TH_NameArray, &CFramework_Books::TMaps_TH_TextArray, &CFramework_Books::TMaps_TH_BoolArray, &CFramework_Books::TMaps_TH_FormArray, &CFramework_Books::TMaps_TH_EntriesFound, &CFramework_Books::TMaps_TH_EntriesTotal, &CFramework_TMaps_TH::Data);
		
		AttemptToAdd<FrameworkID::kFramework_CCB>(&CFramework_Books::Books_CC_NameArray, &CFramework_Books::Books_CC_TextArray, &CFramework_Books::Books_CC_BoolArray, &CFramework_Books::Books_CC_FormArray, &CFramework_Books::Books_CC_EntriesFound, &CFramework_Books::Books_CC_EntriesTotal, &CFramework_Books_CC::Data);
		AttemptToAdd<FrameworkID::kFramework_CCT>(&CFramework_Books::Tomes_CC_NameArray, &CFramework_Books::Tomes_CC_TextArray, &CFramework_Books::Tomes_CC_BoolArray, &CFramework_Books::Tomes_CC_FormArray, &CFramework_Books::Tomes_CC_EntriesFound, &CFramework_Books::Tomes_CC_EntriesTotal, &CFramework_Tomes_CC::Data);

		//Frameworks - Misc (Locations) (Start) -------------
		AttemptToAdd<FrameworkID::kFramework_MAG>(&CFramework_MapMa::MapMa_AG_NameArray, &CFramework_MapMa::MapMa_AG_TextArray, &CFramework_MapMa::MapMa_AG_BoolArray, &CFramework_MapMa::MapMa_AG_FormArray, &CFramework_MapMa::MapMa_AG_EntriesFound, &CFramework_MapMa::MapMa_AG_EntriesTotal, &CFramework_MapMa_AG::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MHR>(&CFramework_MapMa::MapMa_HR_NameArray, &CFramework_MapMa::MapMa_HR_TextArray, &CFramework_MapMa::MapMa_HR_BoolArray, &CFramework_MapMa::MapMa_HR_FormArray, &CFramework_MapMa::MapMa_HR_EntriesFound, &CFramework_MapMa::MapMa_HR_EntriesTotal, &CFramework_MapMa_HR::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MSZ>(&CFramework_MapMa::MapMa_SZ_NameArray, &CFramework_MapMa::MapMa_SZ_TextArray, &CFramework_MapMa::MapMa_SZ_BoolArray, &CFramework_MapMa::MapMa_SZ_FormArray, &CFramework_MapMa::MapMa_SZ_EntriesFound, &CFramework_MapMa::MapMa_SZ_EntriesTotal, &CFramework_MapMa_SZ::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MDG>(&CFramework_MapMa::MapMa_DG_NameArray, &CFramework_MapMa::MapMa_DG_TextArray, &CFramework_MapMa::MapMa_DG_BoolArray, &CFramework_MapMa::MapMa_DG_FormArray, &CFramework_MapMa::MapMa_DG_EntriesFound, &CFramework_MapMa::MapMa_DG_EntriesTotal, &CFramework_MapMa_DG::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MDB>(&CFramework_MapMa::MapMa_DB_NameArray, &CFramework_MapMa::MapMa_DB_TextArray, &CFramework_MapMa::MapMa_DB_BoolArray, &CFramework_MapMa::MapMa_DB_FormArray, &CFramework_MapMa::MapMa_DB_EntriesFound, &CFramework_MapMa::MapMa_DB_EntriesTotal, &CFramework_Tomes_DB::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MCC>(&CFramework_MapMa::MapMa_CC_NameArray, &CFramework_MapMa::MapMa_CC_TextArray, &CFramework_MapMa::MapMa_CC_BoolArray, &CFramework_MapMa::MapMa_CC_FormArray, &CFramework_MapMa::MapMa_CC_EntriesFound, &CFramework_MapMa::MapMa_CC_EntriesTotal, &CFramework_MapMa_CC::Data, true);

		//Frameworks - Misc (Blessings) (Start) -------------
		AttemptToAdd<FrameworkID::kFramework_VDS>(&CFramework_Blessings::DS_NameArray, &CFramework_Blessings::DS_TextArray, &CFramework_Blessings::DS_BoolArray, &CFramework_Blessings::DS_FormArray, &CFramework_Blessings::DS_EntriesFound, &CFramework_Blessings::DS_EntriesTotal, &CFramework_Blessings_DS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_VSH>(&CFramework_Blessings::VS_NameArray, &CFramework_Blessings::VS_TextArray, &CFramework_Blessings::VS_BoolArray, &CFramework_Blessings::VS_FormArray, &CFramework_Blessings::VS_EntriesFound, &CFramework_Blessings::VS_EntriesTotal, &CFramework_Blessings_VS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_WSH>(&CFramework_Blessings::WS_NameArray, &CFramework_Blessings::WS_TextArray, &CFramework_Blessings::WS_BoolArray, &CFramework_Blessings::WS_FormArray, &CFramework_Blessings::WS_EntriesFound, &CFramework_Blessings::WS_EntriesTotal, &CFramework_Blessings_WS::Data, true);

		//Frameworks - Misc (Enchantments) (Start) -------------
		AttemptToAdd<FrameworkID::kFramework_VAE>(&CFramework_Enchantments::VA_NameArray, &CFramework_Enchantments::VA_TextArray, &CFramework_Enchantments::VA_BoolArray, &CFramework_Enchantments::VA_FormArray, &CFramework_Enchantments::VA_EntriesFound, &CFramework_Enchantments::VA_EntriesTotal, &CFramework_Enchantments_VA::Data, true);
		AttemptToAdd<FrameworkID::kFramework_SAE>(&CFramework_Enchantments::SA_NameArray, &CFramework_Enchantments::SA_TextArray, &CFramework_Enchantments::SA_BoolArray, &CFramework_Enchantments::SA_FormArray, &CFramework_Enchantments::SA_EntriesFound, &CFramework_Enchantments::SA_EntriesTotal, &CFramework_Enchantments_SA::Data, true);
		AttemptToAdd<FrameworkID::kFramework_VWE>(&CFramework_Enchantments::VW_NameArray, &CFramework_Enchantments::VW_TextArray, &CFramework_Enchantments::VW_BoolArray, &CFramework_Enchantments::VW_FormArray, &CFramework_Enchantments::VW_EntriesFound, &CFramework_Enchantments::VW_EntriesTotal, &CFramework_Enchantments_VW::Data, true);
		AttemptToAdd<FrameworkID::kFramework_SWE>(&CFramework_Enchantments::SW_NameArray, &CFramework_Enchantments::SW_TextArray, &CFramework_Enchantments::SW_BoolArray, &CFramework_Enchantments::SW_FormArray, &CFramework_Enchantments::SW_EntriesFound, &CFramework_Enchantments::SW_EntriesTotal, &CFramework_Enchantments_SW::Data, true);

		//---------------
		//-- Patches ----
		//---------------


		//Patches - Aditional Hearthfire Dolls (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_AHD_I>(&CPatch_AHD::Items_NameArray, &CPatch_AHD::Items_TextArray, &CPatch_AHD::Items_BoolArray, &CPatch_AHD::Items_FormArray, &CPatch_AHD::Items_EntriesFound, &CPatch_AHD::Items_EntriesTotal, &CPatch_AHD_Items::Data);

		//Patches - The Brotherhood of Old (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_BOO_I>(&CPatch_BOO::Items_NameArray, &CPatch_BOO::Items_TextArray, &CPatch_BOO::Items_BoolArray, &CPatch_BOO::Items_FormArray, &CPatch_BOO::Items_EntriesFound, &CPatch_BOO::Items_EntriesTotal, &CPatch_BOO_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_BOO_B>(&CPatch_BOO::Books_NameArray, &CPatch_BOO::Books_TextArray, &CPatch_BOO::Books_BoolArray, &CPatch_BOO::Books_FormArray, &CPatch_BOO::Books_EntriesFound, &CPatch_BOO::Books_EntriesTotal, &CPatch_BOO_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_BOO_L>(&CPatch_BOO::MapMa_NameArray, &CPatch_BOO::MapMa_TextArray, &CPatch_BOO::MapMa_BoolArray, &CPatch_BOO::MapMa_FormArray, &CPatch_BOO::MapMa_EntriesFound, &CPatch_BOO::MapMa_EntriesTotal, &CPatch_BOO_MapMa::Data);

		//Patches - Clockwork (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_CLW_I>(&CPatch_CLW::Items_NameArray, &CPatch_CLW::Items_TextArray, &CPatch_CLW::Items_BoolArray, &CPatch_CLW::Items_FormArray, &CPatch_CLW::Items_EntriesFound, &CPatch_CLW::Items_EntriesTotal, &CPatch_CLW_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_CLW_B>(&CPatch_CLW::Books_NameArray, &CPatch_CLW::Books_TextArray, &CPatch_CLW::Books_BoolArray, &CPatch_CLW::Books_FormArray, &CPatch_CLW::Books_EntriesFound, &CPatch_CLW::Books_EntriesTotal, &CPatch_CLW_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_CLW_L>(&CPatch_CLW::MapMa_NameArray, &CPatch_CLW::MapMa_TextArray, &CPatch_CLW::MapMa_BoolArray, &CPatch_CLW::MapMa_FormArray, &CPatch_CLW::MapMa_EntriesFound, &CPatch_CLW::MapMa_EntriesTotal, &CPatch_CLW_MapMa::Data);

		//Patches - Falskaar (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_FSK_I>(&CPatch_FSK::Items_NameArray, &CPatch_FSK::Items_TextArray, &CPatch_FSK::Items_BoolArray, &CPatch_FSK::Items_FormArray, &CPatch_FSK::Items_EntriesFound, &CPatch_FSK::Items_EntriesTotal, &CPatch_FSK_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_FSK_B>(&CPatch_FSK::Books_NameArray, &CPatch_FSK::Books_TextArray, &CPatch_FSK::Books_BoolArray, &CPatch_FSK::Books_FormArray, &CPatch_FSK::Books_EntriesFound, &CPatch_FSK::Books_EntriesTotal, &CPatch_FSK_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_FSK_L>(&CPatch_FSK::MapMa_NameArray, &CPatch_FSK::MapMa_TextArray, &CPatch_FSK::MapMa_BoolArray, &CPatch_FSK::MapMa_FormArray, &CPatch_FSK::MapMa_EntriesFound, &CPatch_FSK::MapMa_EntriesTotal, &CPatch_FSK_MapMa::Data);

		//Patches - Fossil Mining (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_FOS_1>(&CPatch_FOS::ItmL1_NameArray, &CPatch_FOS::ItmL1_TextArray, &CPatch_FOS::ItmL1_BoolArray, &CPatch_FOS::ItmL1_FormArray, &CPatch_FOS::ItmL1_EntriesFound, &CPatch_FOS::ItmL1_EntriesTotal, &CPatch_FOS_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_FOS_2>(&CPatch_FOS::ItmL2_NameArray, &CPatch_FOS::ItmL2_TextArray, &CPatch_FOS::ItmL2_BoolArray, &CPatch_FOS::ItmL2_FormArray, &CPatch_FOS::ItmL2_EntriesFound, &CPatch_FOS::ItmL2_EntriesTotal, &CPatch_FOS_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_FOS_3>(&CPatch_FOS::ItmL3_NameArray, &CPatch_FOS::ItmL3_TextArray, &CPatch_FOS::ItmL3_BoolArray, &CPatch_FOS::ItmL3_FormArray, &CPatch_FOS::ItmL3_EntriesFound, &CPatch_FOS::ItmL3_EntriesTotal, &CPatch_FOS_ItmL3::Data);

		//Patches - Gray Cowl of Nocturnal (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_GCN_I>(&CPatch_GCN::Items_NameArray, &CPatch_GCN::Items_TextArray, &CPatch_GCN::Items_BoolArray, &CPatch_GCN::Items_FormArray, &CPatch_GCN::Items_EntriesFound, &CPatch_GCN::Items_EntriesTotal, &CPatch_GCN_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_GCN_B>(&CPatch_GCN::Books_NameArray, &CPatch_GCN::Books_TextArray, &CPatch_GCN::Books_BoolArray, &CPatch_GCN::Books_FormArray, &CPatch_GCN::Books_EntriesFound, &CPatch_GCN::Books_EntriesTotal, &CPatch_GCN_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_GCN_L>(&CPatch_GCN::MapMa_NameArray, &CPatch_GCN::MapMa_TextArray, &CPatch_GCN::MapMa_BoolArray, &CPatch_GCN::MapMa_FormArray, &CPatch_GCN::MapMa_EntriesFound, &CPatch_GCN::MapMa_EntriesTotal, &CPatch_GCN_MapMa::Data);

		//Patches - Oblivion Artifacts (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_OAP_I>(&CPatch_OAP::Items_NameArray, &CPatch_OAP::Items_TextArray, &CPatch_OAP::Items_BoolArray, &CPatch_OAP::Items_FormArray, &CPatch_OAP::Items_EntriesFound, &CPatch_OAP::Items_EntriesTotal, &CPatch_OAP_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_OAP_B>(&CPatch_OAP::Books_NameArray, &CPatch_OAP::Books_TextArray, &CPatch_OAP::Books_BoolArray, &CPatch_OAP::Books_FormArray, &CPatch_OAP::Books_EntriesFound, &CPatch_OAP::Books_EntriesTotal, &CPatch_OAP_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_OAP_L>(&CPatch_OAP::MapMa_NameArray, &CPatch_OAP::MapMa_TextArray, &CPatch_OAP::MapMa_BoolArray, &CPatch_OAP::MapMa_FormArray, &CPatch_OAP::MapMa_EntriesFound, &CPatch_OAP::MapMa_EntriesTotal, &CPatch_OAP_MapMa::Data);

		//Patches - Helgen Reborn (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_HRB_I>(&CPatch_HRB::Items_NameArray, &CPatch_HRB::Items_TextArray, &CPatch_HRB::Items_BoolArray, &CPatch_HRB::Items_FormArray, &CPatch_HRB::Items_EntriesFound, &CPatch_HRB::Items_EntriesTotal, &CPatch_HRB_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_HRB_B>(&CPatch_HRB::Books_NameArray, &CPatch_HRB::Books_TextArray, &CPatch_HRB::Books_BoolArray, &CPatch_HRB::Books_FormArray, &CPatch_HRB::Books_EntriesFound, &CPatch_HRB::Books_EntriesTotal, &CPatch_HRB_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_HRB_L>(&CPatch_HRB::MapMa_NameArray, &CPatch_HRB::MapMa_TextArray, &CPatch_HRB::MapMa_BoolArray, &CPatch_HRB::MapMa_FormArray, &CPatch_HRB::MapMa_EntriesFound, &CPatch_HRB::MapMa_EntriesTotal, &CPatch_HRB_MapMa::Data);

		//Patches - Interesting NPC's (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_3DC_I>(&CPatch_3DC::Items_NameArray, &CPatch_3DC::Items_TextArray, &CPatch_3DC::Items_BoolArray, &CPatch_3DC::Items_FormArray, &CPatch_3DC::Items_EntriesFound, &CPatch_3DC::Items_EntriesTotal, &CPatch_3DC_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_3DC_B>(&CPatch_3DC::Books_NameArray, &CPatch_3DC::Books_TextArray, &CPatch_3DC::Books_BoolArray, &CPatch_3DC::Books_FormArray, &CPatch_3DC::Books_EntriesFound, &CPatch_3DC::Books_EntriesTotal, &CPatch_3DC_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_3DC_L>(&CPatch_3DC::MapMa_NameArray, &CPatch_3DC::MapMa_TextArray, &CPatch_3DC::MapMa_BoolArray, &CPatch_3DC::MapMa_FormArray, &CPatch_3DC::MapMa_EntriesFound, &CPatch_3DC::MapMa_EntriesTotal, &CPatch_3DC_MapMa::Data);

		//Patches - Moon and Star (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_MAS_I>(&CPatch_MAS::Items_NameArray, &CPatch_MAS::Items_TextArray, &CPatch_MAS::Items_BoolArray, &CPatch_MAS::Items_FormArray, &CPatch_MAS::Items_EntriesFound, &CPatch_MAS::Items_EntriesTotal, &CPatch_MAS_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_MAS_B>(&CPatch_MAS::Books_NameArray, &CPatch_MAS::Books_TextArray, &CPatch_MAS::Books_BoolArray, &CPatch_MAS::Books_FormArray, &CPatch_MAS::Books_EntriesFound, &CPatch_MAS::Books_EntriesTotal, &CPatch_MAS_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_MAS_L>(&CPatch_MAS::MapMa_NameArray, &CPatch_MAS::MapMa_TextArray, &CPatch_MAS::MapMa_BoolArray, &CPatch_MAS::MapMa_FormArray, &CPatch_MAS::MapMa_EntriesFound, &CPatch_MAS::MapMa_EntriesTotal, &CPatch_MAS_MapMa::Data);

		//Patches - Moonpath to Elsweyr (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_MTE_I>(&CPatch_MTE::Items_NameArray, &CPatch_MTE::Items_TextArray, &CPatch_MTE::Items_BoolArray, &CPatch_MTE::Items_FormArray, &CPatch_MTE::Items_EntriesFound, &CPatch_MTE::Items_EntriesTotal, &CPatch_MTE_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_MTE_B>(&CPatch_MTE::Books_NameArray, &CPatch_MTE::Books_TextArray, &CPatch_MTE::Books_BoolArray, &CPatch_MTE::Books_FormArray, &CPatch_MTE::Books_EntriesFound, &CPatch_MTE::Books_EntriesTotal, &CPatch_MTE_Books::Data);

		//Patches - Project AHO (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_AHO_I>(&CPatch_AHO::Items_NameArray, &CPatch_AHO::Items_TextArray, &CPatch_AHO::Items_BoolArray, &CPatch_AHO::Items_FormArray, &CPatch_AHO::Items_EntriesFound, &CPatch_AHO::Items_EntriesTotal, &CPatch_AHO_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_AHO_B>(&CPatch_AHO::Books_NameArray, &CPatch_AHO::Books_TextArray, &CPatch_AHO::Books_BoolArray, &CPatch_AHO::Books_FormArray, &CPatch_AHO::Books_EntriesFound, &CPatch_AHO::Books_EntriesTotal, &CPatch_AHO_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_AHO_L>(&CPatch_AHO::MapMa_NameArray, &CPatch_AHO::MapMa_TextArray, &CPatch_AHO::MapMa_BoolArray, &CPatch_AHO::MapMa_FormArray, &CPatch_AHO::MapMa_EntriesFound, &CPatch_AHO::MapMa_EntriesTotal, &CPatch_AHO_MapMa::Data);

		//Patches - Skyrim Unique Treasures (1) (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_ST1_1>(&CPatch_ST1::ItmL1_NameArray, &CPatch_ST1::ItmL1_TextArray, &CPatch_ST1::ItmL1_BoolArray, &CPatch_ST1::ItmL1_FormArray, &CPatch_ST1::ItmL1_EntriesFound, &CPatch_ST1::ItmL1_EntriesTotal, &CPatch_ST1_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_ST1_2>(&CPatch_ST1::ItmL2_NameArray, &CPatch_ST1::ItmL2_TextArray, &CPatch_ST1::ItmL2_BoolArray, &CPatch_ST1::ItmL2_FormArray, &CPatch_ST1::ItmL2_EntriesFound, &CPatch_ST1::ItmL2_EntriesTotal, &CPatch_ST1_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_ST1_3>(&CPatch_ST1::ItmL3_NameArray, &CPatch_ST1::ItmL3_TextArray, &CPatch_ST1::ItmL3_BoolArray, &CPatch_ST1::ItmL3_FormArray, &CPatch_ST1::ItmL3_EntriesFound, &CPatch_ST1::ItmL3_EntriesTotal, &CPatch_ST1_ItmL3::Data);
		AttemptToAdd<FrameworkID::kPatch_ST1_4>(&CPatch_ST1::ItmL4_NameArray, &CPatch_ST1::ItmL4_TextArray, &CPatch_ST1::ItmL4_BoolArray, &CPatch_ST1::ItmL4_FormArray, &CPatch_ST1::ItmL4_EntriesFound, &CPatch_ST1::ItmL4_EntriesTotal, &CPatch_ST1_ItmL4::Data);

		//Patches - Skyrim Unique Treasures (2) (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_ST2_1>(&CPatch_ST2::ItmL1_NameArray, &CPatch_ST2::ItmL1_TextArray, &CPatch_ST2::ItmL1_BoolArray, &CPatch_ST2::ItmL1_FormArray, &CPatch_ST2::ItmL1_EntriesFound, &CPatch_ST2::ItmL1_EntriesTotal, &CPatch_ST2_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_ST2_2>(&CPatch_ST2::ItmL2_NameArray, &CPatch_ST2::ItmL2_TextArray, &CPatch_ST2::ItmL2_BoolArray, &CPatch_ST2::ItmL2_FormArray, &CPatch_ST2::ItmL2_EntriesFound, &CPatch_ST2::ItmL2_EntriesTotal, &CPatch_ST2_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_ST2_3>(&CPatch_ST2::ItmL3_NameArray, &CPatch_ST2::ItmL3_TextArray, &CPatch_ST2::ItmL3_BoolArray, &CPatch_ST2::ItmL3_FormArray, &CPatch_ST2::ItmL3_EntriesFound, &CPatch_ST2::ItmL3_EntriesTotal, &CPatch_ST2_ItmL3::Data);
		AttemptToAdd<FrameworkID::kPatch_ST2_4>(&CPatch_ST2::ItmL4_NameArray, &CPatch_ST2::ItmL4_TextArray, &CPatch_ST2::ItmL4_BoolArray, &CPatch_ST2::ItmL4_FormArray, &CPatch_ST2::ItmL4_EntriesFound, &CPatch_ST2::ItmL4_EntriesTotal, &CPatch_ST2_ItmL4::Data);

		//Patches - Skyrim Unique Treasures (3) (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_ST3_1>(&CPatch_ST3::ItmL1_NameArray, &CPatch_ST3::ItmL1_TextArray, &CPatch_ST3::ItmL1_BoolArray, &CPatch_ST3::ItmL1_FormArray, &CPatch_ST3::ItmL1_EntriesFound, &CPatch_ST3::ItmL1_EntriesTotal, &CPatch_ST3_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_ST3_2>(&CPatch_ST3::ItmL2_NameArray, &CPatch_ST3::ItmL2_TextArray, &CPatch_ST3::ItmL2_BoolArray, &CPatch_ST3::ItmL2_FormArray, &CPatch_ST3::ItmL2_EntriesFound, &CPatch_ST3::ItmL2_EntriesTotal, &CPatch_ST3_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_ST3_3>(&CPatch_ST3::ItmL3_NameArray, &CPatch_ST3::ItmL3_TextArray, &CPatch_ST3::ItmL3_BoolArray, &CPatch_ST3::ItmL3_FormArray, &CPatch_ST3::ItmL3_EntriesFound, &CPatch_ST3::ItmL3_EntriesTotal, &CPatch_ST3_ItmL3::Data);
		AttemptToAdd<FrameworkID::kPatch_ST3_4>(&CPatch_ST3::ItmL4_NameArray, &CPatch_ST3::ItmL4_TextArray, &CPatch_ST3::ItmL4_BoolArray, &CPatch_ST3::ItmL4_FormArray, &CPatch_ST3::ItmL4_EntriesFound, &CPatch_ST3::ItmL4_EntriesTotal, &CPatch_ST3_ItmL4::Data);

		//Patches - Fishing (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_FSH_F>(&CPatch_FSH::F_NameArray, &CPatch_FSH::F_TextArray, &CPatch_FSH::F_BoolArray, &CPatch_FSH::F_FormArray, &CPatch_FSH::F_EntriesFound, &CPatch_FSH::F_EntriesTotal, &CPatch_FSH_F::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_I>(&CPatch_FSH::I_NameArray, &CPatch_FSH::I_TextArray, &CPatch_FSH::I_BoolArray, &CPatch_FSH::I_FormArray, &CPatch_FSH::I_EntriesFound, &CPatch_FSH::I_EntriesTotal, &CPatch_FSH_I::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_B>(&CPatch_FSH::B_NameArray, &CPatch_FSH::B_TextArray, &CPatch_FSH::B_BoolArray, &CPatch_FSH::B_FormArray, &CPatch_FSH::B_EntriesFound, &CPatch_FSH::B_EntriesTotal, &CPatch_FSH_B::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_L>(&CPatch_FSH::L_NameArray, &CPatch_FSH::L_TextArray, &CPatch_FSH::L_BoolArray, &CPatch_FSH::L_FormArray, &CPatch_FSH::L_EntriesFound, &CPatch_FSH::L_EntriesTotal, &CPatch_FSH_L::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_A>(&CPatch_FSH::A_NameArray, &CPatch_FSH::A_TextArray, &CPatch_FSH::A_BoolArray, &CPatch_FSH::A_FormArray, &CPatch_FSH::A_EntriesFound, &CPatch_FSH::A_EntriesTotal, &CPatch_FSH_A::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_C>(&CPatch_FSH::C_NameArray, &CPatch_FSH::C_TextArray, &CPatch_FSH::C_BoolArray, &CPatch_FSH::C_FormArray, &CPatch_FSH::C_EntriesFound, &CPatch_FSH::C_EntriesTotal, &CPatch_FSH_C::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_L>(&CPatch_FSH::L_NameArray, &CPatch_FSH::L_TextArray, &CPatch_FSH::L_BoolArray, &CPatch_FSH::L_FormArray, &CPatch_FSH::L_EntriesFound, &CPatch_FSH::L_EntriesTotal, &CPatch_FSH_L::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_S>(&CPatch_FSH::S_NameArray, &CPatch_FSH::S_TextArray, &CPatch_FSH::S_BoolArray, &CPatch_FSH::S_FormArray, &CPatch_FSH::S_EntriesFound, &CPatch_FSH::S_EntriesTotal, &CPatch_FSH_S::Data);

		//Patches - Teldryn Serious (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_TEL_I>(&CPatch_TEL::Items_NameArray, &CPatch_TEL::Items_TextArray, &CPatch_TEL::Items_BoolArray, &CPatch_TEL::Items_FormArray, &CPatch_TEL::Items_EntriesFound, &CPatch_TEL::Items_EntriesTotal, &CPatch_TEL_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_TEL_B>(&CPatch_TEL::Books_NameArray, &CPatch_TEL::Books_TextArray, &CPatch_TEL::Books_BoolArray, &CPatch_TEL::Books_FormArray, &CPatch_TEL::Books_EntriesFound, &CPatch_TEL::Books_EntriesTotal, &CPatch_TEL_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_TEL_L>(&CPatch_TEL::MapMa_NameArray, &CPatch_TEL::MapMa_TextArray, &CPatch_TEL::MapMa_BoolArray, &CPatch_TEL::MapMa_FormArray, &CPatch_TEL::MapMa_EntriesFound, &CPatch_TEL::MapMa_EntriesTotal, &CPatch_TEL_MapMa::Data);
		
		//Patches - Thunderchild (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_THU_I>(&CPatch_THU::Items_NameArray, &CPatch_THU::Items_TextArray, &CPatch_THU::Items_BoolArray, &CPatch_THU::Items_FormArray, &CPatch_THU::Items_EntriesFound, &CPatch_THU::Items_EntriesTotal, &CPatch_THU_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_THU_B>(&CPatch_THU::Books_NameArray, &CPatch_THU::Books_TextArray, &CPatch_THU::Books_BoolArray, &CPatch_THU::Books_FormArray, &CPatch_THU::Books_EntriesFound, &CPatch_THU::Books_EntriesTotal, &CPatch_THU_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_THU_L>(&CPatch_THU::MapMa_NameArray, &CPatch_THU::MapMa_TextArray, &CPatch_THU::MapMa_BoolArray, &CPatch_THU::MapMa_FormArray, &CPatch_THU::MapMa_EntriesFound, &CPatch_THU::MapMa_EntriesTotal, &CPatch_THU_MapMa::Data);

		//Patches - Undeath (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_UND_B>(&CPatch_UND::Books_NameArray, &CPatch_UND::Books_TextArray, &CPatch_UND::Books_BoolArray, &CPatch_UND::Books_FormArray, &CPatch_UND::Books_EntriesFound, &CPatch_UND::Books_EntriesTotal, &CPatch_UND_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_UND_L>(&CPatch_UND::MapMa_NameArray, &CPatch_UND::MapMa_TextArray, &CPatch_UND::MapMa_BoolArray, &CPatch_UND::MapMa_FormArray, &CPatch_UND::MapMa_EntriesFound, &CPatch_UND::MapMa_EntriesTotal, &CPatch_UND_MapMa::Data);

		//Patches - Wheels of Lull (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WOL_I>(&CPatch_WOL::Items_NameArray, &CPatch_WOL::Items_TextArray, &CPatch_WOL::Items_BoolArray, &CPatch_WOL::Items_FormArray, &CPatch_WOL::Items_EntriesFound, &CPatch_WOL::Items_EntriesTotal, &CPatch_WOL_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WOL_B>(&CPatch_WOL::Books_NameArray, &CPatch_WOL::Books_TextArray, &CPatch_WOL::Books_BoolArray, &CPatch_WOL::Books_FormArray, &CPatch_WOL::Books_EntriesFound, &CPatch_WOL::Books_EntriesTotal, &CPatch_WOL_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WOL_L>(&CPatch_WOL::MapMa_NameArray, &CPatch_WOL::MapMa_TextArray, &CPatch_WOL::MapMa_BoolArray, &CPatch_WOL::MapMa_FormArray, &CPatch_WOL::MapMa_EntriesFound, &CPatch_WOL::MapMa_EntriesTotal, &CPatch_WOL_MapMa::Data);

		//Patches - Wintersun (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WSN_I>(&CPatch_WSN::Items_NameArray, &CPatch_WSN::Items_TextArray, &CPatch_WSN::Items_BoolArray, &CPatch_WSN::Items_FormArray, &CPatch_WSN::Items_EntriesFound, &CPatch_WSN::Items_EntriesTotal, &CPatch_WSN_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WSN_B>(&CPatch_WSN::Books_NameArray, &CPatch_WSN::Books_TextArray, &CPatch_WSN::Books_BoolArray, &CPatch_WSN::Books_FormArray, &CPatch_WSN::Books_EntriesFound, &CPatch_WSN::Books_EntriesTotal, &CPatch_WSN_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WSN_L>(&CPatch_WSN::MapMa_NameArray, &CPatch_WSN::MapMa_TextArray, &CPatch_WSN::MapMa_BoolArray, &CPatch_WSN::MapMa_FormArray, &CPatch_WSN::MapMa_EntriesFound, &CPatch_WSN::MapMa_EntriesTotal, &CPatch_WSN_MapMa::Data);

		//Patches - Wyrmstooth (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WYR_I>(&CPatch_WYR::Items_NameArray, &CPatch_WYR::Items_TextArray, &CPatch_WYR::Items_BoolArray, &CPatch_WYR::Items_FormArray, &CPatch_WYR::Items_EntriesFound, &CPatch_WYR::Items_EntriesTotal, &CPatch_WYR_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WYR_B>(&CPatch_WYR::Books_NameArray, &CPatch_WYR::Books_TextArray, &CPatch_WYR::Books_BoolArray, &CPatch_WYR::Books_FormArray, &CPatch_WYR::Books_EntriesFound, &CPatch_WYR::Books_EntriesTotal, &CPatch_WYR_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WYR_L>(&CPatch_WYR::MapMa_NameArray, &CPatch_WYR::MapMa_TextArray, &CPatch_WYR::MapMa_BoolArray, &CPatch_WYR::MapMa_FormArray, &CPatch_WYR::MapMa_EntriesFound, &CPatch_WYR::MapMa_EntriesTotal, &CPatch_WYR_MapMa::Data);

		//Patches - Vigilant (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_VIG_I>(&CPatch_VIG::Items_NameArray, &CPatch_VIG::Items_TextArray, &CPatch_VIG::Items_BoolArray, &CPatch_VIG::Items_FormArray, &CPatch_VIG::Items_EntriesFound, &CPatch_VIG::Items_EntriesTotal, &CPatch_VIG_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_VIG_B>(&CPatch_VIG::Books_NameArray, &CPatch_VIG::Books_TextArray, &CPatch_VIG::Books_BoolArray, &CPatch_VIG::Books_FormArray, &CPatch_VIG::Books_EntriesFound, &CPatch_VIG::Books_EntriesTotal, &CPatch_VIG_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_VIG_L>(&CPatch_VIG::MapMa_NameArray, &CPatch_VIG::MapMa_TextArray, &CPatch_VIG::MapMa_BoolArray, &CPatch_VIG::MapMa_FormArray, &CPatch_VIG::MapMa_EntriesFound, &CPatch_VIG::MapMa_EntriesTotal, &CPatch_VIG_MapMa::Data);

		INFO("Registered {} Framework Arrays - {} Hidden as NoShow and {} Merged as Collectable", i_Ssize, i_Nsize, (i_Ssize - i_Nsize));
	}
}