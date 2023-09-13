#pragma once

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
#include "Patches/Legacy of the Dragonborn/CFramework_LOD.hpp"
#include "Patches/SpellTomes/CFramework_SpellTomes.hpp"
#include "Patches/Skyrim Unique Drinks/CFramework_SUD.hpp"
#include "Patches/Cheesemod/CFramework_Cheese.hpp"
#include "Patches/Requiem/CFramework_REQ.hpp"
#include "Patches/InnSoaps/CFramework_Inn.hpp"
#include "Patches/Jaysus Swords/CFramework_Jay.hpp"
#include "Patches/Royal Armory/CFramework_RAR.hpp"
#include "Patches/Cloaks Of Skyrim/CFramework_Cloaks.hpp"

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
		kFramework_NGA = 66, // Armor (Necromantic Grimoire)
		kFramework_SAE = 37, // Armor (Summermyst)
		kFramework_VWE = 38, // Weapons	(Vanilla)
		kFramework_SWE = 39, // Weapons (Summermyst)
		kFramework_VPS = 40, // Pets (Vanilla)
		kFramework_POS = 41, // Pets (Pets of Skyrim)
		kFramework_SAS = 42, // Pets (Saints & Seducers)
		kFramework_MSC = 43, // Pets (Miscellaneous)
		kFramework_VPH = 44, // Player Homes (Vanilla)
		kFramework_CPH = 45, // Player Homes (Creation Club)
		kFramework_PPH = 46, // Player Homes (Supported Mods)
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
		
		kPatch_SpellTomes_ApoA = 300, // Apocalypse - Magic of Skyrim
		kPatch_SpellTomes_ApoC = 301, // Apocalypse - Magic of Skyrim
		kPatch_SpellTomes_ApoD = 302, // Apocalypse - Magic of Skyrim
		kPatch_SpellTomes_ApoI = 303, // Apocalypse - Magic of Skyrim
		kPatch_SpellTomes_ApoR = 304, // Apocalypse - Magic of Skyrim

		kPatch_SpellTomes_OdiA = 305, // Odin - Skyrim Magic Overhaul
		kPatch_SpellTomes_OdiC = 306, // Odin - Skyrim Magic Overhaul
		kPatch_SpellTomes_OdiD = 307, // Odin - Skyrim Magic Overhaul
		kPatch_SpellTomes_OdiI = 308, // Odin - Skyrim Magic Overhaul
		kPatch_SpellTomes_OdiR = 309, // Odin - Skyrim Magic Overhaul

		kPatch_SpellTomes_MysA = 310, // Mysticism
		kPatch_SpellTomes_MysC = 311, // Mysticism
		kPatch_SpellTomes_MysD = 312, // Mysticism
		kPatch_SpellTomes_MysI = 313, // Mysticism
		kPatch_SpellTomes_MysR = 314, // Mysticism

		kPatch_SpellTomes_ForA = 315, // Forgotten Magic Redone
		kPatch_SpellTomes_ForC = 316, // Forgotten Magic Redone
		kPatch_SpellTomes_ForD = 317, // Forgotten Magic Redone
		kPatch_SpellTomes_ForI = 318, // Forgotten Magic Redone
		kPatch_SpellTomes_ForR = 319, // Forgotten Magic Redone

		kPatch_SpellTomes_TriA = 320, // Triumvirate
		kPatch_SpellTomes_TriC = 321, // Triumvirate
		kPatch_SpellTomes_TriD = 322, // Triumvirate
		kPatch_SpellTomes_TriI = 323, // Triumvirate
		kPatch_SpellTomes_TriR = 324, // Triumvirate

		kPatch_SUD_I = 325, // Skyrim Unique Drinks

		kPatch_CHM_1 = 326, // CheeseMod (Misc)
		kPatch_CHM_2 = 327, // CheeseMod (Sliced)
		kPatch_CHM_3 = 328, // CheeseMod (Wedges)
		kPatch_CHM_4 = 329, // CheeseMod (Wheels)

		kPatch_REQ_A = 330, // Requiem (Armor)
		kPatch_REQ_M = 331, // Requiem (Misc)
		kPatch_REQ_S = 332, // Requiem (Skill Books)
		kPatch_REQ_B = 333, // Requiem (Books)
		kPatch_REQ_T = 334, // Requiem (Spell Tomes)
		kPatch_REQ_W = 335, // Requiem (Weapons)

		kPatch_INN = 336, // Inn Soaps
		kPatch_JAY = 337, // Jaysus Swords
		kPatch_RAR = 338, // Royal Armory

		kFramework_WHO = 339,	// Pets (Wild Horses)
		kPatch_Cloaks = 340,	// Cloaks of Skyrim
		kPatch_CloaksDP = 341,	// Cloaks of Skyrim (Dragon Priests)

		kTotal,
	};

	enum class QuestID : std::int32_t
	{
		//DO NOT CHANGE THIS ORDER - USED BY CQUESTDATA

		kQuest_MSQ_SK = 0,  // Main Story Quests (Skyrim)
		kQuest_MSQ_CW = 1,  // Main Story Quests (Civil War)
		kQuest_MSQ_DG = 2,  // Main Story Quests (Dawnguard)
		kQuest_MSQ_DB = 3,  // Main Story Quests (Dragonborn)
						    
		kQuest_CCQ_01 = 4,  // Creation Club (Farming and Fishing)
		kQuest_CCQ_02 = 5,  // Creation Club (A-G)
		kQuest_CCQ_03 = 6,  // Creation Club (H-W)
						    
		kQuest_Dawnst = 7,  // Dawnstar Quests
		kQuest_Falkre = 8,  // Falkreath Quests
		kQuest_Markar = 9,  // Markarth Quests
		kQuest_Mortha = 10, // Morthal Quests
		kQuest_Riften = 11, // Riften Quests
		kQuest_Solitu = 12, // Solitude Quests
		kQuest_Whiter = 13, // Whiterun Quests
		kQuest_Windhe = 14, // Windhelm Quests
		kQuest_Winter = 15, // Winterhold Quests
		kQuest_RavenR = 16, // Raven Rock Quests
		kQuest_SkaalV = 17, // Skaal Village Quests
		kQuest_TelMit = 18, // Tel Mithryn Quests
		kQuest_Thirsk = 19, // Thirsk Quests
		kQuest_SmallT = 20, // Small Towns Quests

		kQuest_Colleg = 21, // College of Winterhold Quests
		kQuest_Compan = 22, // Companions Quests
		kQuest_DarkBr = 23, // Dark Brotherhood Quests
		kQuest_Dawngu = 24, // Dawnguard Quests
		kQuest_Thieve = 25, // Thieves Guild Quests
		kQuest_Vampir = 26, // Vampires Quests

		kQuest_Dungeo = 27, // Dungeon Quests
		kQuest_MiscSK = 28, // Misc Quests (Skyrim)
		kQuest_RadiSK = 29, // Radiant Quests (Skyrim)
		kQuest_MiscDG = 30, // Misc Quests (Dawnguard)
		kQuest_MiscDB = 31, // Misc Quests (Dragonborn)

		kQuest_CLW = 32, // Clockwork
		kQuest_FSK = 33, // Falskaar
		kQuest_HRB = 34, // Helgen Reborn
		kQuest_MTE = 35, // Moonpath To Elsweyr
		kQuest_MAS = 36, // Moon And Star
		kQuest_AHO = 37, // Project AHO
		kQuest_TEL = 38, // Teldryn Serious
		kQuest_WOL = 39, // The Wheels of Lull
		kQuest_GCN = 40, // The Gray Cowl of Nocturnal
		kQuest_WYR = 41, // Wyrmstooth
		kQuest_UND = 42, // Undeath
		kQuest_BOO = 43, // Brotherhood of Old

		kQuest_3D1 = 44, // Interesting NPC's (Main)
		kQuest_3D2 = 45, // Interesting NPC's (BOK)
		kQuest_3D3 = 46, // Interesting NPC's (DS)
		kQuest_3D4 = 47, // Interesting NPC's (Misc)

		kQuest_VG1 = 48, // Vigilant (Main)
		kQuest_VG2 = 49, // Vigilant (Side)
		kQuest_VG3 = 50, // Vigilant (Memory)
		kQuest_VG4 = 51, // Vigilant (Radiant)

		kQuest_LD1 = 52, // Legacy of the Dragonborn (Main)
		kQuest_LD2 = 53, // Legacy of the Dragonborn (Misc)
		kQuest_LD3 = 54, // Legacy of the Dragonborn (Notes)
		kQuest_LD4 = 55, // Legacy of the Dragonborn (Journals)
		kQuest_LD5 = 56, // Legacy of the Dragonborn (Explorer)

		kTotal,
	};

	//Misc Array Containers 
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

	[[nodiscard]] static auto& HandleNameSet(FrameworkID a_frameworkID) noexcept {
		return (NameSet.contains(a_frameworkID) && NameSet.at(a_frameworkID)) ? *NameSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleTextSet(FrameworkID a_frameworkID) noexcept {
		return (TextSet.contains(a_frameworkID) && TextSet.at(a_frameworkID)) ? *TextSet.at(a_frameworkID) : ETextVec;
	}

	[[nodiscard]] static auto& HandleBoolSet(FrameworkID a_frameworkID) noexcept {
		return (BoolSet.contains(a_frameworkID) && BoolSet.at(a_frameworkID)) ? *BoolSet.at(a_frameworkID) : EBoolVec;
	}

	[[nodiscard]] static auto& HandleFormSet(FrameworkID a_frameworkID) noexcept {
		return (FormSet.contains(a_frameworkID) && FormSet.at(a_frameworkID)) ? *FormSet.at(a_frameworkID) : EFormVec;
	}

	[[nodiscard]] static auto& HandleFoundSet(FrameworkID a_frameworkID) noexcept {
		return (CntFSet.contains(a_frameworkID) && CntFSet.at(a_frameworkID)) ? *CntFSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleTotalSet(FrameworkID a_frameworkID) noexcept {
		return (CntTSet.contains(a_frameworkID) && CntTSet.at(a_frameworkID)) ? *CntTSet.at(a_frameworkID) : EsInt;
	}

	[[nodiscard]] static auto& HandleDataSet(FrameworkID a_frameworkID) noexcept {
		return (DataSet.contains(a_frameworkID) && DataSet.at(a_frameworkID)) ? *DataSet.at(a_frameworkID) : EDataSet;
	}

	[[nodiscard]] static auto HandleNoShow(FrameworkID a_frameworkID) noexcept {
		return (NoShow.contains(a_frameworkID) && NoShow.at(a_frameworkID)) ? NoShow.at(a_frameworkID) : false;
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

	static void RegisterArrays() noexcept 
	{
		//Frameworks - Items (Uniques) (Start) --------------
		AttemptToAdd<FrameworkID::kFramework_ARM>(&CFramework_Uniques::A_NameArray, &CFramework_Uniques::A_TextArray, &CFramework_Uniques::A_BoolArray, &CFramework_Uniques::A_FormArray, &CFramework_Uniques::A_EntriesFound, &CFramework_Uniques::A_EntriesTotal, &CFramework_Uniques_A::Data);
		AttemptToAdd<FrameworkID::kFramework_WPN>(&CFramework_Uniques::W_NameArray, &CFramework_Uniques::W_TextArray, &CFramework_Uniques::W_BoolArray, &CFramework_Uniques::W_FormArray, &CFramework_Uniques::W_EntriesFound, &CFramework_Uniques::W_EntriesTotal, &CFramework_Uniques_W::Data);
		AttemptToAdd<FrameworkID::kFramework_ITM>(&CFramework_Uniques::I_NameArray, &CFramework_Uniques::I_TextArray, &CFramework_Uniques::I_BoolArray, &CFramework_Uniques::I_FormArray, &CFramework_Uniques::I_EntriesFound, &CFramework_Uniques::I_EntriesTotal, &CFramework_Uniques_I::Data);
		AttemptToAdd<FrameworkID::kFramework_JWY>(&CFramework_Uniques::J_NameArray, &CFramework_Uniques::J_TextArray, &CFramework_Uniques::J_BoolArray, &CFramework_Uniques::J_FormArray, &CFramework_Uniques::J_EntriesFound, &CFramework_Uniques::J_EntriesTotal, &CFramework_Uniques_J::Data);
		AttemptToAdd<FrameworkID::kFramework_LIQ>(&CFramework_Uniques::L_NameArray, &CFramework_Uniques::L_TextArray, &CFramework_Uniques::L_BoolArray, &CFramework_Uniques::L_FormArray, &CFramework_Uniques::L_EntriesFound, &CFramework_Uniques::L_EntriesTotal, &CFramework_Uniques_L::Data);
		AttemptToAdd<FrameworkID::kFramework_QSR>(&CFramework_Uniques::Q_NameArray, &CFramework_Uniques::Q_TextArray, &CFramework_Uniques::Q_BoolArray, &CFramework_Uniques::Q_FormArray, &CFramework_Uniques::Q_EntriesFound, &CFramework_Uniques::Q_EntriesTotal, &CFramework_Uniques_Q::Data);

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
		AttemptToAdd<FrameworkID::kFramework_BDG>(&CFramework_Books::Books_DG_NameArray, &CFramework_Books::Books_DG_TextArray, &CFramework_Books::Books_DG_BoolArray, &CFramework_Books::Books_DG_FormArray, &CFramework_Books::Books_DG_EntriesFound, &CFramework_Books::Books_DG_EntriesTotal, &CFramework_Books_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_BDB>(&CFramework_Books::Books_DB_NameArray, &CFramework_Books::Books_DB_TextArray, &CFramework_Books::Books_DB_BoolArray, &CFramework_Books::Books_DB_FormArray, &CFramework_Books::Books_DB_EntriesFound, &CFramework_Books::Books_DB_EntriesTotal, &CFramework_Books_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_MSK>(&CFramework_Books::TMaps_SK_NameArray, &CFramework_Books::TMaps_SK_TextArray, &CFramework_Books::TMaps_SK_BoolArray, &CFramework_Books::TMaps_SK_FormArray, &CFramework_Books::TMaps_SK_EntriesFound, &CFramework_Books::TMaps_SK_EntriesTotal, &CFramework_TMaps_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_MNT>(&CFramework_Books::TMaps_NT_NameArray, &CFramework_Books::TMaps_NT_TextArray, &CFramework_Books::TMaps_NT_BoolArray, &CFramework_Books::TMaps_NT_FormArray, &CFramework_Books::TMaps_NT_EntriesFound, &CFramework_Books::TMaps_NT_EntriesTotal, &CFramework_TMaps_NT::Data);
		AttemptToAdd<FrameworkID::kFramework_MTH>(&CFramework_Books::TMaps_TH_NameArray, &CFramework_Books::TMaps_TH_TextArray, &CFramework_Books::TMaps_TH_BoolArray, &CFramework_Books::TMaps_TH_FormArray, &CFramework_Books::TMaps_TH_EntriesFound, &CFramework_Books::TMaps_TH_EntriesTotal, &CFramework_TMaps_TH::Data);
		AttemptToAdd<FrameworkID::kFramework_CCB>(&CFramework_Books::Books_CC_NameArray, &CFramework_Books::Books_CC_TextArray, &CFramework_Books::Books_CC_BoolArray, &CFramework_Books::Books_CC_FormArray, &CFramework_Books::Books_CC_EntriesFound, &CFramework_Books::Books_CC_EntriesTotal, &CFramework_Books_CC::Data);

		AttemptToAdd<FrameworkID::kFramework_T0A>(&CFramework_Books::Tomes_SK_A_NameArray, &CFramework_Books::Tomes_SK_A_TextArray, &CFramework_Books::Tomes_SK_A_BoolArray, &CFramework_Books::Tomes_SK_A_FormArray, &CFramework_Books::Tomes_SK_A_EntriesFound, &CFramework_Books::Tomes_SK_A_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_T0C>(&CFramework_Books::Tomes_SK_C_NameArray, &CFramework_Books::Tomes_SK_C_TextArray, &CFramework_Books::Tomes_SK_C_BoolArray, &CFramework_Books::Tomes_SK_C_FormArray, &CFramework_Books::Tomes_SK_C_EntriesFound, &CFramework_Books::Tomes_SK_C_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_T0D>(&CFramework_Books::Tomes_SK_D_NameArray, &CFramework_Books::Tomes_SK_D_TextArray, &CFramework_Books::Tomes_SK_D_BoolArray, &CFramework_Books::Tomes_SK_D_FormArray, &CFramework_Books::Tomes_SK_D_EntriesFound, &CFramework_Books::Tomes_SK_D_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_T0I>(&CFramework_Books::Tomes_SK_I_NameArray, &CFramework_Books::Tomes_SK_I_TextArray, &CFramework_Books::Tomes_SK_I_BoolArray, &CFramework_Books::Tomes_SK_I_FormArray, &CFramework_Books::Tomes_SK_I_EntriesFound, &CFramework_Books::Tomes_SK_I_EntriesTotal, &CFramework_Tomes_SK::Data);
		AttemptToAdd<FrameworkID::kFramework_T0R>(&CFramework_Books::Tomes_SK_R_NameArray, &CFramework_Books::Tomes_SK_R_TextArray, &CFramework_Books::Tomes_SK_R_BoolArray, &CFramework_Books::Tomes_SK_R_FormArray, &CFramework_Books::Tomes_SK_R_EntriesFound, &CFramework_Books::Tomes_SK_R_EntriesTotal, &CFramework_Tomes_SK::Data);

		AttemptToAdd<FrameworkID::kFramework_T1A>(&CFramework_Books::Tomes_DG_A_NameArray, &CFramework_Books::Tomes_DG_A_TextArray, &CFramework_Books::Tomes_DG_A_BoolArray, &CFramework_Books::Tomes_DG_A_FormArray, &CFramework_Books::Tomes_DG_A_EntriesFound, &CFramework_Books::Tomes_DG_A_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_T1C>(&CFramework_Books::Tomes_DG_C_NameArray, &CFramework_Books::Tomes_DG_C_TextArray, &CFramework_Books::Tomes_DG_C_BoolArray, &CFramework_Books::Tomes_DG_C_FormArray, &CFramework_Books::Tomes_DG_C_EntriesFound, &CFramework_Books::Tomes_DG_C_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_T1D>(&CFramework_Books::Tomes_DG_D_NameArray, &CFramework_Books::Tomes_DG_D_TextArray, &CFramework_Books::Tomes_DG_D_BoolArray, &CFramework_Books::Tomes_DG_D_FormArray, &CFramework_Books::Tomes_DG_D_EntriesFound, &CFramework_Books::Tomes_DG_D_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_T1I>(&CFramework_Books::Tomes_DG_I_NameArray, &CFramework_Books::Tomes_DG_I_TextArray, &CFramework_Books::Tomes_DG_I_BoolArray, &CFramework_Books::Tomes_DG_I_FormArray, &CFramework_Books::Tomes_DG_I_EntriesFound, &CFramework_Books::Tomes_DG_I_EntriesTotal, &CFramework_Tomes_DG::Data);
		AttemptToAdd<FrameworkID::kFramework_T1R>(&CFramework_Books::Tomes_DG_R_NameArray, &CFramework_Books::Tomes_DG_R_TextArray, &CFramework_Books::Tomes_DG_R_BoolArray, &CFramework_Books::Tomes_DG_R_FormArray, &CFramework_Books::Tomes_DG_R_EntriesFound, &CFramework_Books::Tomes_DG_R_EntriesTotal, &CFramework_Tomes_DG::Data);

		AttemptToAdd<FrameworkID::kFramework_T2A>(&CFramework_Books::Tomes_DB_A_NameArray, &CFramework_Books::Tomes_DB_A_TextArray, &CFramework_Books::Tomes_DB_A_BoolArray, &CFramework_Books::Tomes_DB_A_FormArray, &CFramework_Books::Tomes_DB_A_EntriesFound, &CFramework_Books::Tomes_DB_A_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_T2C>(&CFramework_Books::Tomes_DB_C_NameArray, &CFramework_Books::Tomes_DB_C_TextArray, &CFramework_Books::Tomes_DB_C_BoolArray, &CFramework_Books::Tomes_DB_C_FormArray, &CFramework_Books::Tomes_DB_C_EntriesFound, &CFramework_Books::Tomes_DB_C_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_T2D>(&CFramework_Books::Tomes_DB_D_NameArray, &CFramework_Books::Tomes_DB_D_TextArray, &CFramework_Books::Tomes_DB_D_BoolArray, &CFramework_Books::Tomes_DB_D_FormArray, &CFramework_Books::Tomes_DB_D_EntriesFound, &CFramework_Books::Tomes_DB_D_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_T2I>(&CFramework_Books::Tomes_DB_I_NameArray, &CFramework_Books::Tomes_DB_I_TextArray, &CFramework_Books::Tomes_DB_I_BoolArray, &CFramework_Books::Tomes_DB_I_FormArray, &CFramework_Books::Tomes_DB_I_EntriesFound, &CFramework_Books::Tomes_DB_I_EntriesTotal, &CFramework_Tomes_DB::Data);
		AttemptToAdd<FrameworkID::kFramework_T2R>(&CFramework_Books::Tomes_DB_R_NameArray, &CFramework_Books::Tomes_DB_R_TextArray, &CFramework_Books::Tomes_DB_R_BoolArray, &CFramework_Books::Tomes_DB_R_FormArray, &CFramework_Books::Tomes_DB_R_EntriesFound, &CFramework_Books::Tomes_DB_R_EntriesTotal, &CFramework_Tomes_DB::Data);

		AttemptToAdd<FrameworkID::kFramework_T3A>(&CFramework_Books::Tomes_CC_A_NameArray, &CFramework_Books::Tomes_CC_A_TextArray, &CFramework_Books::Tomes_CC_A_BoolArray, &CFramework_Books::Tomes_CC_A_FormArray, &CFramework_Books::Tomes_CC_A_EntriesFound, &CFramework_Books::Tomes_CC_A_EntriesTotal, &CFramework_Tomes_CC::Data);
		AttemptToAdd<FrameworkID::kFramework_T3C>(&CFramework_Books::Tomes_CC_C_NameArray, &CFramework_Books::Tomes_CC_C_TextArray, &CFramework_Books::Tomes_CC_C_BoolArray, &CFramework_Books::Tomes_CC_C_FormArray, &CFramework_Books::Tomes_CC_C_EntriesFound, &CFramework_Books::Tomes_CC_C_EntriesTotal, &CFramework_Tomes_CC::Data);
		AttemptToAdd<FrameworkID::kFramework_T3D>(&CFramework_Books::Tomes_CC_D_NameArray, &CFramework_Books::Tomes_CC_D_TextArray, &CFramework_Books::Tomes_CC_D_BoolArray, &CFramework_Books::Tomes_CC_D_FormArray, &CFramework_Books::Tomes_CC_D_EntriesFound, &CFramework_Books::Tomes_CC_D_EntriesTotal, &CFramework_Tomes_CC::Data);
		AttemptToAdd<FrameworkID::kFramework_T3I>(&CFramework_Books::Tomes_CC_I_NameArray, &CFramework_Books::Tomes_CC_I_TextArray, &CFramework_Books::Tomes_CC_I_BoolArray, &CFramework_Books::Tomes_CC_I_FormArray, &CFramework_Books::Tomes_CC_I_EntriesFound, &CFramework_Books::Tomes_CC_I_EntriesTotal, &CFramework_Tomes_CC::Data);
		AttemptToAdd<FrameworkID::kFramework_T3R>(&CFramework_Books::Tomes_CC_R_NameArray, &CFramework_Books::Tomes_CC_R_TextArray, &CFramework_Books::Tomes_CC_R_BoolArray, &CFramework_Books::Tomes_CC_R_FormArray, &CFramework_Books::Tomes_CC_R_EntriesFound, &CFramework_Books::Tomes_CC_R_EntriesTotal, &CFramework_Tomes_CC::Data);

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

		//Frameworks - Misc (Enchantments) (Start) ----------
		AttemptToAdd<FrameworkID::kFramework_VAE>(&CFramework_Enchantments::VA_NameArray, &CFramework_Enchantments::VA_TextArray, &CFramework_Enchantments::VA_BoolArray, &CFramework_Enchantments::VA_FormArray, &CFramework_Enchantments::VA_EntriesFound, &CFramework_Enchantments::VA_EntriesTotal, &CFramework_Enchantments_VA::Data, true);
		AttemptToAdd<FrameworkID::kFramework_NGA>(&CFramework_Enchantments::NGA_NameArray, &CFramework_Enchantments::NGA_TextArray, &CFramework_Enchantments::NGA_BoolArray, &CFramework_Enchantments::NGA_FormArray, &CFramework_Enchantments::NGA_EntriesFound, &CFramework_Enchantments::NGA_EntriesTotal, &CFramework_Enchantments_NGA::Data, true);
		AttemptToAdd<FrameworkID::kFramework_SAE>(&CFramework_Enchantments::SA_NameArray, &CFramework_Enchantments::SA_TextArray, &CFramework_Enchantments::SA_BoolArray, &CFramework_Enchantments::SA_FormArray, &CFramework_Enchantments::SA_EntriesFound, &CFramework_Enchantments::SA_EntriesTotal, &CFramework_Enchantments_SA::Data, true);
		AttemptToAdd<FrameworkID::kFramework_VWE>(&CFramework_Enchantments::VW_NameArray, &CFramework_Enchantments::VW_TextArray, &CFramework_Enchantments::VW_BoolArray, &CFramework_Enchantments::VW_FormArray, &CFramework_Enchantments::VW_EntriesFound, &CFramework_Enchantments::VW_EntriesTotal, &CFramework_Enchantments_VW::Data, true);
		AttemptToAdd<FrameworkID::kFramework_SWE>(&CFramework_Enchantments::SW_NameArray, &CFramework_Enchantments::SW_TextArray, &CFramework_Enchantments::SW_BoolArray, &CFramework_Enchantments::SW_FormArray, &CFramework_Enchantments::SW_EntriesFound, &CFramework_Enchantments::SW_EntriesTotal, &CFramework_Enchantments_SW::Data, true);

		//Frameworks - Misc (Pets) (Start) ------------------
		AttemptToAdd<FrameworkID::kFramework_VPS>(&CFramework_Pets::Pets_VP_NameArray, &CFramework_Pets::Pets_VP_TextArray, &CFramework_Pets::Pets_VP_BoolArray, &CFramework_Pets::Pets_VP_FormArray, &CFramework_Pets::Pets_VP_EntriesFound, &CFramework_Pets::Pets_VP_EntriesTotal, &CFramework_Pets_VP::Data, true);
		AttemptToAdd<FrameworkID::kFramework_POS>(&CFramework_Pets::Pets_PS_NameArray, &CFramework_Pets::Pets_PS_TextArray, &CFramework_Pets::Pets_PS_BoolArray, &CFramework_Pets::Pets_PS_FormArray, &CFramework_Pets::Pets_PS_EntriesFound, &CFramework_Pets::Pets_PS_EntriesTotal, &CFramework_Pets_PS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_SAS>(&CFramework_Pets::Pets_SS_NameArray, &CFramework_Pets::Pets_SS_TextArray, &CFramework_Pets::Pets_SS_BoolArray, &CFramework_Pets::Pets_SS_FormArray, &CFramework_Pets::Pets_SS_EntriesFound, &CFramework_Pets::Pets_SS_EntriesTotal, &CFramework_Pets_SS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MSC>(&CFramework_Pets::Pets_MP_NameArray, &CFramework_Pets::Pets_MP_TextArray, &CFramework_Pets::Pets_MP_BoolArray, &CFramework_Pets::Pets_MP_FormArray, &CFramework_Pets::Pets_MP_EntriesFound, &CFramework_Pets::Pets_MP_EntriesTotal, &CFramework_Pets_MP::Data, true);
		AttemptToAdd<FrameworkID::kFramework_WHO>(&CFramework_Pets::Pets_WH_NameArray, &CFramework_Pets::Pets_WH_TextArray, &CFramework_Pets::Pets_WH_BoolArray, &CFramework_Pets::Pets_WH_FormArray, &CFramework_Pets::Pets_WH_EntriesFound, &CFramework_Pets::Pets_WH_EntriesTotal, &CFramework_Pets_WH::Data, true);

		//Frameworks - Misc (Player Homes) (Start) ----------
		AttemptToAdd<FrameworkID::kFramework_VPH>(&CFramework_PlayerHomes::VH_NameArray, &CFramework_PlayerHomes::VH_TextArray, &CFramework_PlayerHomes::VH_BoolArray, &CFramework_PlayerHomes::VH_FormArray, &CFramework_PlayerHomes::VH_EntriesFound, &CFramework_PlayerHomes::VH_EntriesTotal, &CFramework_PlayerHomes_VH::Data, true);
		AttemptToAdd<FrameworkID::kFramework_CPH>(&CFramework_PlayerHomes::CH_NameArray, &CFramework_PlayerHomes::CH_TextArray, &CFramework_PlayerHomes::CH_BoolArray, &CFramework_PlayerHomes::CH_FormArray, &CFramework_PlayerHomes::CH_EntriesFound, &CFramework_PlayerHomes::CH_EntriesTotal, &CFramework_PlayerHomes_CH::Data, true);
		AttemptToAdd<FrameworkID::kFramework_PPH>(&CFramework_PlayerHomes::PH_NameArray, &CFramework_PlayerHomes::PH_TextArray, &CFramework_PlayerHomes::PH_BoolArray, &CFramework_PlayerHomes::PH_FormArray, &CFramework_PlayerHomes::PH_EntriesFound, &CFramework_PlayerHomes::PH_EntriesTotal, &CFramework_PlayerHomes_PH::Data, true);

		//Frameworks - Misc (Shouts) (Start) ----------------
		AttemptToAdd<FrameworkID::kFramework_VNS>(&CFramework_Shouts::Vanilla_SH_NameOutput, &CFramework_Shouts::Vanilla_SH_TextArray, &CFramework_Shouts::Vanilla_SH_BoolArray, &CFramework_Shouts::Vanilla_SH_FormArray, &CFramework_Shouts::Vanilla_SH_EntriesFound, &CFramework_Shouts::Vanilla_SH_EntriesTotal, &CFramework_Shouts_VS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_TCS>(&CFramework_Shouts::Thunderchild_SH_NameOutput, &CFramework_Shouts::Thunderchild_SH_TextArray, &CFramework_Shouts::Thunderchild_SH_BoolArray, &CFramework_Shouts::Thunderchild_SH_FormArray, &CFramework_Shouts::Thunderchild_SH_EntriesFound, &CFramework_Shouts::Thunderchild_SH_EntriesTotal, &CFramework_Shouts_TS::Data, true);
		AttemptToAdd<FrameworkID::kFramework_MCS>(&CFramework_Shouts::Miscellaneous_SH_NameOutput, &CFramework_Shouts::Miscellaneous_SH_TextArray, &CFramework_Shouts::Miscellaneous_SH_BoolArray, &CFramework_Shouts::Miscellaneous_SH_FormArray, &CFramework_Shouts::Miscellaneous_SH_EntriesFound, &CFramework_Shouts::Miscellaneous_SH_EntriesTotal, &CFramework_Shouts_TS::Data, true);


		//---------------
		//-- Patches ----
		//---------------


		//Patches - Aditional Hearthfire Dolls (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_AHD_I>(&CPatch_AHD::Items_NameArray, &CPatch_AHD::Items_TextArray, &CPatch_AHD::Items_BoolArray, &CPatch_AHD::Items_FormArray, &CPatch_AHD::Items_EntriesFound, &CPatch_AHD::Items_EntriesTotal, &CPatch_AHD_Items::Data);

		//Patches - Inn Soaps (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_INN>(&CPatch_INN::Items_NameArray, &CPatch_INN::Items_TextArray, &CPatch_INN::Items_BoolArray, &CPatch_INN::Items_FormArray, &CPatch_INN::Items_EntriesFound, &CPatch_INN::Items_EntriesTotal, &CPatch_Inn_Items::Data);
		
		//Patches - Jaysus Swords (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_JAY>(&CPatch_JAY::Items_NameArray, &CPatch_JAY::Items_TextArray, &CPatch_JAY::Items_BoolArray, &CPatch_JAY::Items_FormArray, &CPatch_JAY::Items_EntriesFound, &CPatch_JAY::Items_EntriesTotal, &CPatch_Jay_Items::Data);

		//Patches - Royal Armory (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_RAR>(&CPatch_RAR::Items_NameArray, &CPatch_RAR::Items_TextArray, &CPatch_RAR::Items_BoolArray, &CPatch_RAR::Items_FormArray, &CPatch_RAR::Items_EntriesFound, &CPatch_RAR::Items_EntriesTotal, &CPatch_Rar_Items::Data);

		//Patches - The Brotherhood of Old (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_BOO_I>(&CPatch_BOO::Items_NameArray, &CPatch_BOO::Items_TextArray, &CPatch_BOO::Items_BoolArray, &CPatch_BOO::Items_FormArray, &CPatch_BOO::Items_EntriesFound, &CPatch_BOO::Items_EntriesTotal, &CPatch_BOO_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_BOO_B>(&CPatch_BOO::Books_NameArray, &CPatch_BOO::Books_TextArray, &CPatch_BOO::Books_BoolArray, &CPatch_BOO::Books_FormArray, &CPatch_BOO::Books_EntriesFound, &CPatch_BOO::Books_EntriesTotal, &CPatch_BOO_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_BOO_L>(&CPatch_BOO::MapMa_NameArray, &CPatch_BOO::MapMa_TextArray, &CPatch_BOO::MapMa_BoolArray, &CPatch_BOO::MapMa_FormArray, &CPatch_BOO::MapMa_EntriesFound, &CPatch_BOO::MapMa_EntriesTotal, &CPatch_BOO_MapMa::Data, true);

		//Patches - Clockwork (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_CLW_I>(&CPatch_CLW::Items_NameArray, &CPatch_CLW::Items_TextArray, &CPatch_CLW::Items_BoolArray, &CPatch_CLW::Items_FormArray, &CPatch_CLW::Items_EntriesFound, &CPatch_CLW::Items_EntriesTotal, &CPatch_CLW_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_CLW_B>(&CPatch_CLW::Books_NameArray, &CPatch_CLW::Books_TextArray, &CPatch_CLW::Books_BoolArray, &CPatch_CLW::Books_FormArray, &CPatch_CLW::Books_EntriesFound, &CPatch_CLW::Books_EntriesTotal, &CPatch_CLW_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_CLW_L>(&CPatch_CLW::MapMa_NameArray, &CPatch_CLW::MapMa_TextArray, &CPatch_CLW::MapMa_BoolArray, &CPatch_CLW::MapMa_FormArray, &CPatch_CLW::MapMa_EntriesFound, &CPatch_CLW::MapMa_EntriesTotal, &CPatch_CLW_MapMa::Data, true);

		//Patches - Falskaar (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_FSK_I>(&CPatch_FSK::Items_NameArray, &CPatch_FSK::Items_TextArray, &CPatch_FSK::Items_BoolArray, &CPatch_FSK::Items_FormArray, &CPatch_FSK::Items_EntriesFound, &CPatch_FSK::Items_EntriesTotal, &CPatch_FSK_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_FSK_B>(&CPatch_FSK::Books_NameArray, &CPatch_FSK::Books_TextArray, &CPatch_FSK::Books_BoolArray, &CPatch_FSK::Books_FormArray, &CPatch_FSK::Books_EntriesFound, &CPatch_FSK::Books_EntriesTotal, &CPatch_FSK_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_FSK_L>(&CPatch_FSK::MapMa_NameArray, &CPatch_FSK::MapMa_TextArray, &CPatch_FSK::MapMa_BoolArray, &CPatch_FSK::MapMa_FormArray, &CPatch_FSK::MapMa_EntriesFound, &CPatch_FSK::MapMa_EntriesTotal, &CPatch_FSK_MapMa::Data, true);

		//Patches - Fossil Mining (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_FOS_1>(&CPatch_FOS::ItmL1_NameArray, &CPatch_FOS::ItmL1_TextArray, &CPatch_FOS::ItmL1_BoolArray, &CPatch_FOS::ItmL1_FormArray, &CPatch_FOS::ItmL1_EntriesFound, &CPatch_FOS::ItmL1_EntriesTotal, &CPatch_FOS_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_FOS_2>(&CPatch_FOS::ItmL2_NameArray, &CPatch_FOS::ItmL2_TextArray, &CPatch_FOS::ItmL2_BoolArray, &CPatch_FOS::ItmL2_FormArray, &CPatch_FOS::ItmL2_EntriesFound, &CPatch_FOS::ItmL2_EntriesTotal, &CPatch_FOS_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_FOS_3>(&CPatch_FOS::ItmL3_NameArray, &CPatch_FOS::ItmL3_TextArray, &CPatch_FOS::ItmL3_BoolArray, &CPatch_FOS::ItmL3_FormArray, &CPatch_FOS::ItmL3_EntriesFound, &CPatch_FOS::ItmL3_EntriesTotal, &CPatch_FOS_ItmL3::Data);

		//Patches - Gray Cowl of Nocturnal (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_GCN_I>(&CPatch_GCN::Items_NameArray, &CPatch_GCN::Items_TextArray, &CPatch_GCN::Items_BoolArray, &CPatch_GCN::Items_FormArray, &CPatch_GCN::Items_EntriesFound, &CPatch_GCN::Items_EntriesTotal, &CPatch_GCN_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_GCN_B>(&CPatch_GCN::Books_NameArray, &CPatch_GCN::Books_TextArray, &CPatch_GCN::Books_BoolArray, &CPatch_GCN::Books_FormArray, &CPatch_GCN::Books_EntriesFound, &CPatch_GCN::Books_EntriesTotal, &CPatch_GCN_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_GCN_L>(&CPatch_GCN::MapMa_NameArray, &CPatch_GCN::MapMa_TextArray, &CPatch_GCN::MapMa_BoolArray, &CPatch_GCN::MapMa_FormArray, &CPatch_GCN::MapMa_EntriesFound, &CPatch_GCN::MapMa_EntriesTotal, &CPatch_GCN_MapMa::Data, true);

		//Patches - Oblivion Artifacts (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_OAP_I>(&CPatch_OAP::Items_NameArray, &CPatch_OAP::Items_TextArray, &CPatch_OAP::Items_BoolArray, &CPatch_OAP::Items_FormArray, &CPatch_OAP::Items_EntriesFound, &CPatch_OAP::Items_EntriesTotal, &CPatch_OAP_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_OAP_B>(&CPatch_OAP::Books_NameArray, &CPatch_OAP::Books_TextArray, &CPatch_OAP::Books_BoolArray, &CPatch_OAP::Books_FormArray, &CPatch_OAP::Books_EntriesFound, &CPatch_OAP::Books_EntriesTotal, &CPatch_OAP_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_OAP_L>(&CPatch_OAP::MapMa_NameArray, &CPatch_OAP::MapMa_TextArray, &CPatch_OAP::MapMa_BoolArray, &CPatch_OAP::MapMa_FormArray, &CPatch_OAP::MapMa_EntriesFound, &CPatch_OAP::MapMa_EntriesTotal, &CPatch_OAP_MapMa::Data, true);

		//Patches - Helgen Reborn (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_HRB_I>(&CPatch_HRB::Items_NameArray, &CPatch_HRB::Items_TextArray, &CPatch_HRB::Items_BoolArray, &CPatch_HRB::Items_FormArray, &CPatch_HRB::Items_EntriesFound, &CPatch_HRB::Items_EntriesTotal, &CPatch_HRB_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_HRB_B>(&CPatch_HRB::Books_NameArray, &CPatch_HRB::Books_TextArray, &CPatch_HRB::Books_BoolArray, &CPatch_HRB::Books_FormArray, &CPatch_HRB::Books_EntriesFound, &CPatch_HRB::Books_EntriesTotal, &CPatch_HRB_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_HRB_L>(&CPatch_HRB::MapMa_NameArray, &CPatch_HRB::MapMa_TextArray, &CPatch_HRB::MapMa_BoolArray, &CPatch_HRB::MapMa_FormArray, &CPatch_HRB::MapMa_EntriesFound, &CPatch_HRB::MapMa_EntriesTotal, &CPatch_HRB_MapMa::Data, true);

		//Patches - Interesting NPC's (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_3DC_I>(&CPatch_3DC::Items_NameArray, &CPatch_3DC::Items_TextArray, &CPatch_3DC::Items_BoolArray, &CPatch_3DC::Items_FormArray, &CPatch_3DC::Items_EntriesFound, &CPatch_3DC::Items_EntriesTotal, &CPatch_3DC_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_3DC_B>(&CPatch_3DC::Books_NameArray, &CPatch_3DC::Books_TextArray, &CPatch_3DC::Books_BoolArray, &CPatch_3DC::Books_FormArray, &CPatch_3DC::Books_EntriesFound, &CPatch_3DC::Books_EntriesTotal, &CPatch_3DC_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_3DC_L>(&CPatch_3DC::MapMa_NameArray, &CPatch_3DC::MapMa_TextArray, &CPatch_3DC::MapMa_BoolArray, &CPatch_3DC::MapMa_FormArray, &CPatch_3DC::MapMa_EntriesFound, &CPatch_3DC::MapMa_EntriesTotal, &CPatch_3DC_MapMa::Data, true);

		//Patches - Moon and Star (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_MAS_I>(&CPatch_MAS::Items_NameArray, &CPatch_MAS::Items_TextArray, &CPatch_MAS::Items_BoolArray, &CPatch_MAS::Items_FormArray, &CPatch_MAS::Items_EntriesFound, &CPatch_MAS::Items_EntriesTotal, &CPatch_MAS_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_MAS_B>(&CPatch_MAS::Books_NameArray, &CPatch_MAS::Books_TextArray, &CPatch_MAS::Books_BoolArray, &CPatch_MAS::Books_FormArray, &CPatch_MAS::Books_EntriesFound, &CPatch_MAS::Books_EntriesTotal, &CPatch_MAS_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_MAS_L>(&CPatch_MAS::MapMa_NameArray, &CPatch_MAS::MapMa_TextArray, &CPatch_MAS::MapMa_BoolArray, &CPatch_MAS::MapMa_FormArray, &CPatch_MAS::MapMa_EntriesFound, &CPatch_MAS::MapMa_EntriesTotal, &CPatch_MAS_MapMa::Data, true);

		//Patches - Moonpath to Elsweyr (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_MTE_I>(&CPatch_MTE::Items_NameArray, &CPatch_MTE::Items_TextArray, &CPatch_MTE::Items_BoolArray, &CPatch_MTE::Items_FormArray, &CPatch_MTE::Items_EntriesFound, &CPatch_MTE::Items_EntriesTotal, &CPatch_MTE_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_MTE_B>(&CPatch_MTE::Books_NameArray, &CPatch_MTE::Books_TextArray, &CPatch_MTE::Books_BoolArray, &CPatch_MTE::Books_FormArray, &CPatch_MTE::Books_EntriesFound, &CPatch_MTE::Books_EntriesTotal, &CPatch_MTE_Books::Data);

		//Patches - Project AHO (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_AHO_I>(&CPatch_AHO::Items_NameArray, &CPatch_AHO::Items_TextArray, &CPatch_AHO::Items_BoolArray, &CPatch_AHO::Items_FormArray, &CPatch_AHO::Items_EntriesFound, &CPatch_AHO::Items_EntriesTotal, &CPatch_AHO_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_AHO_B>(&CPatch_AHO::Books_NameArray, &CPatch_AHO::Books_TextArray, &CPatch_AHO::Books_BoolArray, &CPatch_AHO::Books_FormArray, &CPatch_AHO::Books_EntriesFound, &CPatch_AHO::Books_EntriesTotal, &CPatch_AHO_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_AHO_L>(&CPatch_AHO::MapMa_NameArray, &CPatch_AHO::MapMa_TextArray, &CPatch_AHO::MapMa_BoolArray, &CPatch_AHO::MapMa_FormArray, &CPatch_AHO::MapMa_EntriesFound, &CPatch_AHO::MapMa_EntriesTotal, &CPatch_AHO_MapMa::Data, true);

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
		AttemptToAdd<FrameworkID::kPatch_FSH_F>(&CPatch_FSH::F_NameArray, &CPatch_FSH::F_TextArray, &CPatch_FSH::F_BoolArray, &CPatch_FSH::F_FormArray, &CPatch_FSH::F_EntriesFound, &CPatch_FSH::F_EntriesTotal, &CPatch_FSH_F::Data, true);
		AttemptToAdd<FrameworkID::kPatch_FSH_I>(&CPatch_FSH::I_NameArray, &CPatch_FSH::I_TextArray, &CPatch_FSH::I_BoolArray, &CPatch_FSH::I_FormArray, &CPatch_FSH::I_EntriesFound, &CPatch_FSH::I_EntriesTotal, &CPatch_FSH_I::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_B>(&CPatch_FSH::B_NameArray, &CPatch_FSH::B_TextArray, &CPatch_FSH::B_BoolArray, &CPatch_FSH::B_FormArray, &CPatch_FSH::B_EntriesFound, &CPatch_FSH::B_EntriesTotal, &CPatch_FSH_B::Data);
		AttemptToAdd<FrameworkID::kPatch_FSH_L>(&CPatch_FSH::L_NameArray, &CPatch_FSH::L_TextArray, &CPatch_FSH::L_BoolArray, &CPatch_FSH::L_FormArray, &CPatch_FSH::L_EntriesFound, &CPatch_FSH::L_EntriesTotal, &CPatch_FSH_L::Data, true);
		AttemptToAdd<FrameworkID::kPatch_FSH_A>(&CPatch_FSH::A_NameArray, &CPatch_FSH::A_TextArray, &CPatch_FSH::A_BoolArray, &CPatch_FSH::A_FormArray, &CPatch_FSH::A_EntriesFound, &CPatch_FSH::A_EntriesTotal, &CPatch_FSH_A::Data, true);
		AttemptToAdd<FrameworkID::kPatch_FSH_C>(&CPatch_FSH::C_NameArray, &CPatch_FSH::C_TextArray, &CPatch_FSH::C_BoolArray, &CPatch_FSH::C_FormArray, &CPatch_FSH::C_EntriesFound, &CPatch_FSH::C_EntriesTotal, &CPatch_FSH_C::Data, true);
		AttemptToAdd<FrameworkID::kPatch_FSH_S>(&CPatch_FSH::S_NameArray, &CPatch_FSH::S_TextArray, &CPatch_FSH::S_BoolArray, &CPatch_FSH::S_FormArray, &CPatch_FSH::S_EntriesFound, &CPatch_FSH::S_EntriesTotal, &CPatch_FSH_S::Data, true);

		//Patches - Teldryn Serious (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_TEL_I>(&CPatch_TEL::Items_NameArray, &CPatch_TEL::Items_TextArray, &CPatch_TEL::Items_BoolArray, &CPatch_TEL::Items_FormArray, &CPatch_TEL::Items_EntriesFound, &CPatch_TEL::Items_EntriesTotal, &CPatch_TEL_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_TEL_B>(&CPatch_TEL::Books_NameArray, &CPatch_TEL::Books_TextArray, &CPatch_TEL::Books_BoolArray, &CPatch_TEL::Books_FormArray, &CPatch_TEL::Books_EntriesFound, &CPatch_TEL::Books_EntriesTotal, &CPatch_TEL_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_TEL_L>(&CPatch_TEL::MapMa_NameArray, &CPatch_TEL::MapMa_TextArray, &CPatch_TEL::MapMa_BoolArray, &CPatch_TEL::MapMa_FormArray, &CPatch_TEL::MapMa_EntriesFound, &CPatch_TEL::MapMa_EntriesTotal, &CPatch_TEL_MapMa::Data, true);
		
		//Patches - Thunderchild (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_THU_I>(&CPatch_THU::Items_NameArray, &CPatch_THU::Items_TextArray, &CPatch_THU::Items_BoolArray, &CPatch_THU::Items_FormArray, &CPatch_THU::Items_EntriesFound, &CPatch_THU::Items_EntriesTotal, &CPatch_THU_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_THU_B>(&CPatch_THU::Books_NameArray, &CPatch_THU::Books_TextArray, &CPatch_THU::Books_BoolArray, &CPatch_THU::Books_FormArray, &CPatch_THU::Books_EntriesFound, &CPatch_THU::Books_EntriesTotal, &CPatch_THU_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_THU_L>(&CPatch_THU::MapMa_NameArray, &CPatch_THU::MapMa_TextArray, &CPatch_THU::MapMa_BoolArray, &CPatch_THU::MapMa_FormArray, &CPatch_THU::MapMa_EntriesFound, &CPatch_THU::MapMa_EntriesTotal, &CPatch_THU_MapMa::Data, true);

		//Patches - Undeath (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_UND_B>(&CPatch_UND::Books_NameArray, &CPatch_UND::Books_TextArray, &CPatch_UND::Books_BoolArray, &CPatch_UND::Books_FormArray, &CPatch_UND::Books_EntriesFound, &CPatch_UND::Books_EntriesTotal, &CPatch_UND_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_UND_L>(&CPatch_UND::MapMa_NameArray, &CPatch_UND::MapMa_TextArray, &CPatch_UND::MapMa_BoolArray, &CPatch_UND::MapMa_FormArray, &CPatch_UND::MapMa_EntriesFound, &CPatch_UND::MapMa_EntriesTotal, &CPatch_UND_MapMa::Data, true);

		//Patches - Wheels of Lull (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WOL_I>(&CPatch_WOL::Items_NameArray, &CPatch_WOL::Items_TextArray, &CPatch_WOL::Items_BoolArray, &CPatch_WOL::Items_FormArray, &CPatch_WOL::Items_EntriesFound, &CPatch_WOL::Items_EntriesTotal, &CPatch_WOL_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WOL_B>(&CPatch_WOL::Books_NameArray, &CPatch_WOL::Books_TextArray, &CPatch_WOL::Books_BoolArray, &CPatch_WOL::Books_FormArray, &CPatch_WOL::Books_EntriesFound, &CPatch_WOL::Books_EntriesTotal, &CPatch_WOL_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WOL_L>(&CPatch_WOL::MapMa_NameArray, &CPatch_WOL::MapMa_TextArray, &CPatch_WOL::MapMa_BoolArray, &CPatch_WOL::MapMa_FormArray, &CPatch_WOL::MapMa_EntriesFound, &CPatch_WOL::MapMa_EntriesTotal, &CPatch_WOL_MapMa::Data, true);

		//Patches - Wintersun (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WSN_I>(&CPatch_WSN::Items_NameArray, &CPatch_WSN::Items_TextArray, &CPatch_WSN::Items_BoolArray, &CPatch_WSN::Items_FormArray, &CPatch_WSN::Items_EntriesFound, &CPatch_WSN::Items_EntriesTotal, &CPatch_WSN_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WSN_B>(&CPatch_WSN::Books_NameArray, &CPatch_WSN::Books_TextArray, &CPatch_WSN::Books_BoolArray, &CPatch_WSN::Books_FormArray, &CPatch_WSN::Books_EntriesFound, &CPatch_WSN::Books_EntriesTotal, &CPatch_WSN_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WSN_L>(&CPatch_WSN::MapMa_NameArray, &CPatch_WSN::MapMa_TextArray, &CPatch_WSN::MapMa_BoolArray, &CPatch_WSN::MapMa_FormArray, &CPatch_WSN::MapMa_EntriesFound, &CPatch_WSN::MapMa_EntriesTotal, &CPatch_WSN_MapMa::Data, true);

		//Patches - Wyrmstooth (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_WYR_I>(&CPatch_WYR::Items_NameArray, &CPatch_WYR::Items_TextArray, &CPatch_WYR::Items_BoolArray, &CPatch_WYR::Items_FormArray, &CPatch_WYR::Items_EntriesFound, &CPatch_WYR::Items_EntriesTotal, &CPatch_WYR_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_WYR_B>(&CPatch_WYR::Books_NameArray, &CPatch_WYR::Books_TextArray, &CPatch_WYR::Books_BoolArray, &CPatch_WYR::Books_FormArray, &CPatch_WYR::Books_EntriesFound, &CPatch_WYR::Books_EntriesTotal, &CPatch_WYR_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_WYR_L>(&CPatch_WYR::MapMa_NameArray, &CPatch_WYR::MapMa_TextArray, &CPatch_WYR::MapMa_BoolArray, &CPatch_WYR::MapMa_FormArray, &CPatch_WYR::MapMa_EntriesFound, &CPatch_WYR::MapMa_EntriesTotal, &CPatch_WYR_MapMa::Data, true);

		//Patches - Vigilant (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_VIG_I>(&CPatch_VIG::Items_NameArray, &CPatch_VIG::Items_TextArray, &CPatch_VIG::Items_BoolArray, &CPatch_VIG::Items_FormArray, &CPatch_VIG::Items_EntriesFound, &CPatch_VIG::Items_EntriesTotal, &CPatch_VIG_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_VIG_B>(&CPatch_VIG::Books_NameArray, &CPatch_VIG::Books_TextArray, &CPatch_VIG::Books_BoolArray, &CPatch_VIG::Books_FormArray, &CPatch_VIG::Books_EntriesFound, &CPatch_VIG::Books_EntriesTotal, &CPatch_VIG_Books::Data);
		AttemptToAdd<FrameworkID::kPatch_VIG_L>(&CPatch_VIG::MapMa_NameArray, &CPatch_VIG::MapMa_TextArray, &CPatch_VIG::MapMa_BoolArray, &CPatch_VIG::MapMa_FormArray, &CPatch_VIG::MapMa_EntriesFound, &CPatch_VIG::MapMa_EntriesTotal, &CPatch_VIG_MapMa::Data, true);

		//Patches - Spell Tomes (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ApoA>(&CPatch_SpellTomes::Apocalypse_A_NameArray, &CPatch_SpellTomes::Apocalypse_A_TextArray, &CPatch_SpellTomes::Apocalypse_A_BoolArray, &CPatch_SpellTomes::Apocalypse_A_FormArray, &CPatch_SpellTomes::Apocalypse_A_EntriesFound, &CPatch_SpellTomes::Apocalypse_A_EntriesTotal, &CPatch_SpellTomes_Apocalypse::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ApoC>(&CPatch_SpellTomes::Apocalypse_C_NameArray, &CPatch_SpellTomes::Apocalypse_C_TextArray, &CPatch_SpellTomes::Apocalypse_C_BoolArray, &CPatch_SpellTomes::Apocalypse_C_FormArray, &CPatch_SpellTomes::Apocalypse_C_EntriesFound, &CPatch_SpellTomes::Apocalypse_C_EntriesTotal, &CPatch_SpellTomes_Apocalypse::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ApoD>(&CPatch_SpellTomes::Apocalypse_D_NameArray, &CPatch_SpellTomes::Apocalypse_D_TextArray, &CPatch_SpellTomes::Apocalypse_D_BoolArray, &CPatch_SpellTomes::Apocalypse_D_FormArray, &CPatch_SpellTomes::Apocalypse_D_EntriesFound, &CPatch_SpellTomes::Apocalypse_D_EntriesTotal, &CPatch_SpellTomes_Apocalypse::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ApoI>(&CPatch_SpellTomes::Apocalypse_I_NameArray, &CPatch_SpellTomes::Apocalypse_I_TextArray, &CPatch_SpellTomes::Apocalypse_I_BoolArray, &CPatch_SpellTomes::Apocalypse_I_FormArray, &CPatch_SpellTomes::Apocalypse_I_EntriesFound, &CPatch_SpellTomes::Apocalypse_I_EntriesTotal, &CPatch_SpellTomes_Apocalypse::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ApoR>(&CPatch_SpellTomes::Apocalypse_R_NameArray, &CPatch_SpellTomes::Apocalypse_R_TextArray, &CPatch_SpellTomes::Apocalypse_R_BoolArray, &CPatch_SpellTomes::Apocalypse_R_FormArray, &CPatch_SpellTomes::Apocalypse_R_EntriesFound, &CPatch_SpellTomes::Apocalypse_R_EntriesTotal, &CPatch_SpellTomes_Apocalypse::Data);

		AttemptToAdd<FrameworkID::kPatch_SpellTomes_OdiA>(&CPatch_SpellTomes::Odin_A_NameArray, &CPatch_SpellTomes::Odin_A_TextArray, &CPatch_SpellTomes::Odin_A_BoolArray, &CPatch_SpellTomes::Odin_A_FormArray, &CPatch_SpellTomes::Odin_A_EntriesFound, &CPatch_SpellTomes::Odin_A_EntriesTotal, &CPatch_SpellTomes_Odin::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_OdiC>(&CPatch_SpellTomes::Odin_C_NameArray, &CPatch_SpellTomes::Odin_C_TextArray, &CPatch_SpellTomes::Odin_C_BoolArray, &CPatch_SpellTomes::Odin_C_FormArray, &CPatch_SpellTomes::Odin_C_EntriesFound, &CPatch_SpellTomes::Odin_C_EntriesTotal, &CPatch_SpellTomes_Odin::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_OdiD>(&CPatch_SpellTomes::Odin_D_NameArray, &CPatch_SpellTomes::Odin_D_TextArray, &CPatch_SpellTomes::Odin_D_BoolArray, &CPatch_SpellTomes::Odin_D_FormArray, &CPatch_SpellTomes::Odin_D_EntriesFound, &CPatch_SpellTomes::Odin_D_EntriesTotal, &CPatch_SpellTomes_Odin::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_OdiI>(&CPatch_SpellTomes::Odin_I_NameArray, &CPatch_SpellTomes::Odin_I_TextArray, &CPatch_SpellTomes::Odin_I_BoolArray, &CPatch_SpellTomes::Odin_I_FormArray, &CPatch_SpellTomes::Odin_I_EntriesFound, &CPatch_SpellTomes::Odin_I_EntriesTotal, &CPatch_SpellTomes_Odin::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_OdiR>(&CPatch_SpellTomes::Odin_R_NameArray, &CPatch_SpellTomes::Odin_R_TextArray, &CPatch_SpellTomes::Odin_R_BoolArray, &CPatch_SpellTomes::Odin_R_FormArray, &CPatch_SpellTomes::Odin_R_EntriesFound, &CPatch_SpellTomes::Odin_R_EntriesTotal, &CPatch_SpellTomes_Odin::Data);

		AttemptToAdd<FrameworkID::kPatch_SpellTomes_MysA>(&CPatch_SpellTomes::Mysticism_A_NameArray, &CPatch_SpellTomes::Mysticism_A_TextArray, &CPatch_SpellTomes::Mysticism_A_BoolArray, &CPatch_SpellTomes::Mysticism_A_FormArray, &CPatch_SpellTomes::Mysticism_A_EntriesFound, &CPatch_SpellTomes::Mysticism_A_EntriesTotal, &CPatch_SpellTomes_Mysticism::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_MysC>(&CPatch_SpellTomes::Mysticism_C_NameArray, &CPatch_SpellTomes::Mysticism_C_TextArray, &CPatch_SpellTomes::Mysticism_C_BoolArray, &CPatch_SpellTomes::Mysticism_C_FormArray, &CPatch_SpellTomes::Mysticism_C_EntriesFound, &CPatch_SpellTomes::Mysticism_C_EntriesTotal, &CPatch_SpellTomes_Mysticism::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_MysD>(&CPatch_SpellTomes::Mysticism_D_NameArray, &CPatch_SpellTomes::Mysticism_D_TextArray, &CPatch_SpellTomes::Mysticism_D_BoolArray, &CPatch_SpellTomes::Mysticism_D_FormArray, &CPatch_SpellTomes::Mysticism_D_EntriesFound, &CPatch_SpellTomes::Mysticism_D_EntriesTotal, &CPatch_SpellTomes_Mysticism::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_MysI>(&CPatch_SpellTomes::Mysticism_I_NameArray, &CPatch_SpellTomes::Mysticism_I_TextArray, &CPatch_SpellTomes::Mysticism_I_BoolArray, &CPatch_SpellTomes::Mysticism_I_FormArray, &CPatch_SpellTomes::Mysticism_I_EntriesFound, &CPatch_SpellTomes::Mysticism_I_EntriesTotal, &CPatch_SpellTomes_Mysticism::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_MysR>(&CPatch_SpellTomes::Mysticism_R_NameArray, &CPatch_SpellTomes::Mysticism_R_TextArray, &CPatch_SpellTomes::Mysticism_R_BoolArray, &CPatch_SpellTomes::Mysticism_R_FormArray, &CPatch_SpellTomes::Mysticism_R_EntriesFound, &CPatch_SpellTomes::Mysticism_R_EntriesTotal, &CPatch_SpellTomes_Mysticism::Data);

		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ForA>(&CPatch_SpellTomes::ForgottenMagic_A_NameArray, &CPatch_SpellTomes::ForgottenMagic_A_TextArray, &CPatch_SpellTomes::ForgottenMagic_A_BoolArray, &CPatch_SpellTomes::ForgottenMagic_A_FormArray, &CPatch_SpellTomes::ForgottenMagic_A_EntriesFound, &CPatch_SpellTomes::ForgottenMagic_A_EntriesTotal, &CPatch_SpellTomes_ForgottenMagic::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ForC>(&CPatch_SpellTomes::ForgottenMagic_C_NameArray, &CPatch_SpellTomes::ForgottenMagic_C_TextArray, &CPatch_SpellTomes::ForgottenMagic_C_BoolArray, &CPatch_SpellTomes::ForgottenMagic_C_FormArray, &CPatch_SpellTomes::ForgottenMagic_C_EntriesFound, &CPatch_SpellTomes::ForgottenMagic_C_EntriesTotal, &CPatch_SpellTomes_ForgottenMagic::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ForD>(&CPatch_SpellTomes::ForgottenMagic_D_NameArray, &CPatch_SpellTomes::ForgottenMagic_D_TextArray, &CPatch_SpellTomes::ForgottenMagic_D_BoolArray, &CPatch_SpellTomes::ForgottenMagic_D_FormArray, &CPatch_SpellTomes::ForgottenMagic_D_EntriesFound, &CPatch_SpellTomes::ForgottenMagic_D_EntriesTotal, &CPatch_SpellTomes_ForgottenMagic::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ForI>(&CPatch_SpellTomes::ForgottenMagic_I_NameArray, &CPatch_SpellTomes::ForgottenMagic_I_TextArray, &CPatch_SpellTomes::ForgottenMagic_I_BoolArray, &CPatch_SpellTomes::ForgottenMagic_I_FormArray, &CPatch_SpellTomes::ForgottenMagic_I_EntriesFound, &CPatch_SpellTomes::ForgottenMagic_I_EntriesTotal, &CPatch_SpellTomes_ForgottenMagic::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_ForR>(&CPatch_SpellTomes::ForgottenMagic_R_NameArray, &CPatch_SpellTomes::ForgottenMagic_R_TextArray, &CPatch_SpellTomes::ForgottenMagic_R_BoolArray, &CPatch_SpellTomes::ForgottenMagic_R_FormArray, &CPatch_SpellTomes::ForgottenMagic_R_EntriesFound, &CPatch_SpellTomes::ForgottenMagic_R_EntriesTotal, &CPatch_SpellTomes_ForgottenMagic::Data);

		AttemptToAdd<FrameworkID::kPatch_SpellTomes_TriA>(&CPatch_SpellTomes::Triumvirate_A_NameArray, &CPatch_SpellTomes::Triumvirate_A_TextArray, &CPatch_SpellTomes::Triumvirate_A_BoolArray, &CPatch_SpellTomes::Triumvirate_A_FormArray, &CPatch_SpellTomes::Triumvirate_A_EntriesFound, &CPatch_SpellTomes::Triumvirate_A_EntriesTotal, &CPatch_SpellTomes_Triumvirate::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_TriC>(&CPatch_SpellTomes::Triumvirate_C_NameArray, &CPatch_SpellTomes::Triumvirate_C_TextArray, &CPatch_SpellTomes::Triumvirate_C_BoolArray, &CPatch_SpellTomes::Triumvirate_C_FormArray, &CPatch_SpellTomes::Triumvirate_C_EntriesFound, &CPatch_SpellTomes::Triumvirate_C_EntriesTotal, &CPatch_SpellTomes_Triumvirate::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_TriD>(&CPatch_SpellTomes::Triumvirate_D_NameArray, &CPatch_SpellTomes::Triumvirate_D_TextArray, &CPatch_SpellTomes::Triumvirate_D_BoolArray, &CPatch_SpellTomes::Triumvirate_D_FormArray, &CPatch_SpellTomes::Triumvirate_D_EntriesFound, &CPatch_SpellTomes::Triumvirate_D_EntriesTotal, &CPatch_SpellTomes_Triumvirate::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_TriI>(&CPatch_SpellTomes::Triumvirate_I_NameArray, &CPatch_SpellTomes::Triumvirate_I_TextArray, &CPatch_SpellTomes::Triumvirate_I_BoolArray, &CPatch_SpellTomes::Triumvirate_I_FormArray, &CPatch_SpellTomes::Triumvirate_I_EntriesFound, &CPatch_SpellTomes::Triumvirate_I_EntriesTotal, &CPatch_SpellTomes_Triumvirate::Data);
		AttemptToAdd<FrameworkID::kPatch_SpellTomes_TriR>(&CPatch_SpellTomes::Triumvirate_R_NameArray, &CPatch_SpellTomes::Triumvirate_R_TextArray, &CPatch_SpellTomes::Triumvirate_R_BoolArray, &CPatch_SpellTomes::Triumvirate_R_FormArray, &CPatch_SpellTomes::Triumvirate_R_EntriesFound, &CPatch_SpellTomes::Triumvirate_R_EntriesTotal, &CPatch_SpellTomes_Triumvirate::Data);

		//Patches - Skyrim Unique Drinks (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_SUD_I>(&CPatch_SUD::Items_NameArray, &CPatch_SUD::Items_TextArray, &CPatch_SUD::Items_BoolArray, &CPatch_SUD::Items_FormArray, &CPatch_SUD::Items_EntriesFound, &CPatch_SUD::Items_EntriesTotal, &CPatch_SUD_Items::Data);

		//Patches - Cloaks of Skyrim (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_Cloaks>(&CPatch_Cloaks::Items_NameArray, &CPatch_Cloaks::Items_TextArray, &CPatch_Cloaks::Items_BoolArray, &CPatch_Cloaks::Items_FormArray, &CPatch_Cloaks::Items_EntriesFound, &CPatch_Cloaks::Items_EntriesTotal, &CPatch_Cloaks_Items::Data);
		AttemptToAdd<FrameworkID::kPatch_CloaksDP>(&CPatch_Cloaks::ItemsDP_NameArray, &CPatch_Cloaks::ItemsDP_TextArray, &CPatch_Cloaks::ItemsDP_BoolArray, &CPatch_Cloaks::ItemsDP_FormArray, &CPatch_Cloaks::ItemsDP_EntriesFound, &CPatch_Cloaks::ItemsDP_EntriesTotal, &CPatch_CloaksDP_Items::Data);

		//Patches - Cheese Mod (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_CHM_1>(&CPatch_CHM::ItmL1_NameArray, &CPatch_CHM::ItmL1_TextArray, &CPatch_CHM::ItmL1_BoolArray, &CPatch_CHM::ItmL1_FormArray, &CPatch_CHM::ItmL1_EntriesFound, &CPatch_CHM::ItmL1_EntriesTotal, &CPatch_CHM_ItmL1::Data);
		AttemptToAdd<FrameworkID::kPatch_CHM_2>(&CPatch_CHM::ItmL2_NameArray, &CPatch_CHM::ItmL2_TextArray, &CPatch_CHM::ItmL2_BoolArray, &CPatch_CHM::ItmL2_FormArray, &CPatch_CHM::ItmL2_EntriesFound, &CPatch_CHM::ItmL2_EntriesTotal, &CPatch_CHM_ItmL2::Data);
		AttemptToAdd<FrameworkID::kPatch_CHM_3>(&CPatch_CHM::ItmL3_NameArray, &CPatch_CHM::ItmL3_TextArray, &CPatch_CHM::ItmL3_BoolArray, &CPatch_CHM::ItmL3_FormArray, &CPatch_CHM::ItmL3_EntriesFound, &CPatch_CHM::ItmL3_EntriesTotal, &CPatch_CHM_ItmL3::Data);
		AttemptToAdd<FrameworkID::kPatch_CHM_4>(&CPatch_CHM::ItmL4_NameArray, &CPatch_CHM::ItmL4_TextArray, &CPatch_CHM::ItmL4_BoolArray, &CPatch_CHM::ItmL4_FormArray, &CPatch_CHM::ItmL4_EntriesFound, &CPatch_CHM::ItmL4_EntriesTotal, &CPatch_CHM_ItmL4::Data);

		//Patches - Requiem (Start) ---------------
		AttemptToAdd<FrameworkID::kPatch_REQ_A>(&CPatch_REQ::ItmL1_A_NameArray, &CPatch_REQ::ItmL1_A_TextArray, &CPatch_REQ::ItmL1_A_BoolArray, &CPatch_REQ::ItmL1_A_FormArray, &CPatch_REQ::ItmL1_A_EntriesFound, &CPatch_REQ::ItmL1_A_EntriesTotal, &CPatch_REQ_ItmL1_A::Data);
		AttemptToAdd<FrameworkID::kPatch_REQ_M>(&CPatch_REQ::ItmL1_M_NameArray, &CPatch_REQ::ItmL1_M_TextArray, &CPatch_REQ::ItmL1_M_BoolArray, &CPatch_REQ::ItmL1_M_FormArray, &CPatch_REQ::ItmL1_M_EntriesFound, &CPatch_REQ::ItmL1_M_EntriesTotal, &CPatch_REQ_ItmL1_M::Data);
		AttemptToAdd<FrameworkID::kPatch_REQ_S>(&CPatch_REQ::ItmL3_S_NameArray, &CPatch_REQ::ItmL3_S_TextArray, &CPatch_REQ::ItmL3_S_BoolArray, &CPatch_REQ::ItmL3_S_FormArray, &CPatch_REQ::ItmL3_S_EntriesFound, &CPatch_REQ::ItmL3_S_EntriesTotal, &CPatch_REQ_ItmL3_S::Data);
		AttemptToAdd<FrameworkID::kPatch_REQ_B>(&CPatch_REQ::ItmL3_B_NameArray, &CPatch_REQ::ItmL3_B_TextArray, &CPatch_REQ::ItmL3_B_BoolArray, &CPatch_REQ::ItmL3_B_FormArray, &CPatch_REQ::ItmL3_B_EntriesFound, &CPatch_REQ::ItmL3_B_EntriesTotal, &CPatch_REQ_ItmL3_B::Data);
		AttemptToAdd<FrameworkID::kPatch_REQ_T>(&CPatch_REQ::ItmL4_NameArray, &CPatch_REQ::ItmL4_TextArray, &CPatch_REQ::ItmL4_BoolArray, &CPatch_REQ::ItmL4_FormArray, &CPatch_REQ::ItmL4_EntriesFound, &CPatch_REQ::ItmL4_EntriesTotal, &CPatch_REQ_ItmL4::Data);
		AttemptToAdd<FrameworkID::kPatch_REQ_W>(&CPatch_REQ::ItmL2_NameArray, &CPatch_REQ::ItmL2_TextArray, &CPatch_REQ::ItmL2_BoolArray, &CPatch_REQ::ItmL2_FormArray, &CPatch_REQ::ItmL2_EntriesFound, &CPatch_REQ::ItmL2_EntriesTotal, &CPatch_REQ_ItmL2::Data);

		INFO("Registered {} Framework Arrays - {} Hidden as NoShow and {} Merged as Collectable", i_Ssize, i_Nsize, (i_Ssize - i_Nsize));
	}
}