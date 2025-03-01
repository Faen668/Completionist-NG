#pragma once
#include "Structs.hpp"

//Frameworks
#include "Frameworks/Items/CFramework_Others.hpp"
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
		kFramework_SBH = 0, // Stones of Barenziah
		kFramework_MAG = 1, // Locations (A-G)
		kFramework_MHR = 2, // Locations (H-R)
		kFramework_MSZ = 3, // Locations (S-Z)
		kFramework_MDG = 4, // Locations (Dawngaurd)
		kFramework_MDB = 5, // Locations (Dragonborn)
		kFramework_VDS = 6, // Doomstones
		kFramework_VSH = 7, // Shrines	(Vanilla)
		kFramework_WSH = 8, // Shrines (Wintersun)
		kFramework_VAE = 9, // Armor (Vanilla)
		kFramework_SAE = 10, // Armor (Summermyst)
		kFramework_VWE = 11, // Weapons	(Vanilla)
		kFramework_SWE = 12, // Weapons (Summermyst)
		kFramework_VPS = 13, // Pets (Vanilla)
		kFramework_VPH = 14, // Player Homes (Vanilla)
		kFramework_VNS = 15, // Shouts (Vanilla)
		kFramework_TCS = 16, // Shouts (Thunderchild)
		kFramework_MCS = 17, // Shouts (Miscellaneous)
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
		//Frameworks - Items (Others) (Start) ---------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_SBH), &CFramework_Others::SB_NameArray, &CFramework_Others::SB_TextArray, &CFramework_Others::SB_BoolArray, &CFramework_Others::SB_FormArray, &CFramework_Others::SB_EntriesFound, &CFramework_Others::SB_EntriesTotal, &CFramework_Others::ItemDataSB, true);

		//Frameworks - Misc (Locations) (Start) -------------
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MAG), &CFramework_MapMa::MapMa_AG_NameArray, &CFramework_MapMa::MapMa_AG_TextArray, &CFramework_MapMa::MapMa_AG_BoolArray, &CFramework_MapMa::MapMa_AG_FormArray, &CFramework_MapMa::MapMa_AG_EntriesFound, &CFramework_MapMa::MapMa_AG_EntriesTotal, &CFramework_MapMa_AG::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MHR), &CFramework_MapMa::MapMa_HR_NameArray, &CFramework_MapMa::MapMa_HR_TextArray, &CFramework_MapMa::MapMa_HR_BoolArray, &CFramework_MapMa::MapMa_HR_FormArray, &CFramework_MapMa::MapMa_HR_EntriesFound, &CFramework_MapMa::MapMa_HR_EntriesTotal, &CFramework_MapMa_HR::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MSZ), &CFramework_MapMa::MapMa_SZ_NameArray, &CFramework_MapMa::MapMa_SZ_TextArray, &CFramework_MapMa::MapMa_SZ_BoolArray, &CFramework_MapMa::MapMa_SZ_FormArray, &CFramework_MapMa::MapMa_SZ_EntriesFound, &CFramework_MapMa::MapMa_SZ_EntriesTotal, &CFramework_MapMa_SZ::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MDG), &CFramework_MapMa::MapMa_DG_NameArray, &CFramework_MapMa::MapMa_DG_TextArray, &CFramework_MapMa::MapMa_DG_BoolArray, &CFramework_MapMa::MapMa_DG_FormArray, &CFramework_MapMa::MapMa_DG_EntriesFound, &CFramework_MapMa::MapMa_DG_EntriesTotal, &CFramework_MapMa_DG::Data, true);
		AttemptToAdd(std::to_underlying(FrameworkID::kFramework_MDB), &CFramework_MapMa::MapMa_DB_NameArray, &CFramework_MapMa::MapMa_DB_TextArray, &CFramework_MapMa::MapMa_DB_BoolArray, &CFramework_MapMa::MapMa_DB_FormArray, &CFramework_MapMa::MapMa_DB_EntriesFound, &CFramework_MapMa::MapMa_DB_EntriesTotal, &CFramework_MapMa_DB::Data, true);

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
		//Custom Patches
		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			for (auto& section : patchData->type_sections) {
				AttemptToAdd(section.ID, section.type == CMiscPatchType::kShouts ? &section.shout_names : &section.names, &section.texts, &section.bools, &section.forms, &section.found, &section.total, &section.data, static_cast<int32_t>(section.type) > 1);
			};
		};
		INFO("Registered {} Framework Arrays - {} Hidden as NoShow and {} Merged as Collectable", i_Ssize, i_Nsize, (i_Ssize - i_Nsize));
	}
}