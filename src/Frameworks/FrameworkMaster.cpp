#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Patches/PatchMaster.hpp"

//Item Frameworks
#include "Items/CFramework_Armor.hpp"
#include "Items/CFramework_Claws.hpp"
#include "Items/CFramework_Items.hpp"
#include "Items/CFramework_Jewelry.hpp"
#include "Items/CFramework_Liquor.hpp"
#include "Items/CFramework_Masks.hpp"
#include "Items/CFramework_Weapons.hpp"
#include "Items/CFramework_Barenziah.hpp"

//Misc Frameworks
#include "Pets/CFramework_Pets.hpp"
#include "Books/CFramework_Maps.hpp"
#include "Books/CFramework_Books.hpp"
#include "Locations/CFramework_Locations.hpp"
#include "Enchantments/CFramework_Enchantments.hpp"
#include "Player Homes/CFramework_PlayerHomes.hpp"

//Blessings Frameworks
#include "Blessings/CFramework_Shrines.hpp"
#include "Blessings/CFramework_Doomstones.hpp"

//Creation Club
#include "Creation Club/CFramework_CreationClub.hpp"

namespace CFramework_Master {

	inline Serialization::CompletionistData FoundItemData;
	inline Serialization::CompletionistData FoundItemData_NoShow;

	std::vector<RE::TESForm*> EmptyFormArray;
	std::vector<std::string> EmptyNameArray;
	std::vector<std::string> EmptyTextArray;
	std::vector<bool> EmptyBoolArray;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register() {

		auto t1 = std::chrono::system_clock::now();

		FoundItemData.SetAsSerializable();
		FoundItemData_NoShow.SetAsSerializable();

		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(FrameworkAPI::RegisterFunctions);

		//Items
		CFramework_Armor::CHandler::InstallFramework();
		CFramework_Items::CHandler::InstallFramework();
		CFramework_Liquor::CHandler::InstallFramework();
		CFramework_Jewelry::CHandler::InstallFramework();
		CFramework_Weapons::CHandler::InstallFramework();
		CFramework_Barenziah::CHandler::InstallFramework();

		//Dragon Claws
		CFramework_DragonClaws_V::CHandler::InstallFramework();
		CFramework_DragonClaws_P::CHandler::InstallFramework();
		CFramework_DragonMasks_V::CHandler::InstallFramework();
		CFramework_DragonMasks_P::CHandler::InstallFramework();

		//Misc
		CFramework_Pets::CHandler::InstallFramework();
		CFramework_MapsManager::CHandler::InstallFramework();
		CFramework_BooksManager::CHandler::InstallFramework();
		CFramework_LocationsManager::CHandler::InstallFramework();
		CFramework_EnchantmentsManager::CHandler::InstallFramework();

		// Blessings 
		CFramework_Shrines_V::CHandler::InstallFramework();
		CFramework_Shrines_P::CHandler::InstallFramework();
		CFramework_Doomstones::CHandler::InstallFramework();

		//Player Homes
		CFramework_PlayerHomes_V::CHandler::InstallFramework();
		CFramework_PlayerHomes_C::CHandler::InstallFramework();
		CFramework_PlayerHomes_P::CHandler::InstallFramework();

		//Creation Club
		CFramework_CCManager::CHandler::InstallFramework();

		// Patches
		CPatch_Master::PatchAPI::InstallPatches();

		auto t2 = std::chrono::system_clock::now();
		auto elapsedSE = (std::chrono::duration_cast<std::chrono::seconds>(t2 - t1)).count();
		auto elapsedMS = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();
		auto elapsedNS = (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1)).count();

		INFO("FInished Installing Completionist in {} seconds - {} Milliseconds - {} Nanoseconds", elapsedSE, elapsedMS, elapsedNS);
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("Framework_GetFormArrayByID", "Completionist_Native", Framework_GetFormArrayByID);
		a_vm->RegisterFunction("Framework_GetNameArrayByID", "Completionist_Native", Framework_GetNameArrayByID);
		a_vm->RegisterFunction("Framework_GetBoolArrayByID", "Completionist_Native", Framework_GetBoolArrayByID);
		a_vm->RegisterFunction("Framework_GetTextArrayByID", "Completionist_Native", Framework_GetTextArrayByID);

		a_vm->RegisterFunction("Framework_GetEntries_TotalByID", "Completionist_Native", Framework_GetEntries_TotalByID);
		a_vm->RegisterFunction("Framework_GetEntries_FoundByID", "Completionist_Native", Framework_GetEntries_FoundByID);

		a_vm->RegisterFunction("Framework_CCItemsInstalled", "Completionist_Native", Framework_CCItemsInstalled);
		a_vm->RegisterFunction("Framework_CCBooksInstalled", "Completionist_Native", Framework_CCBooksInstalled);
		a_vm->RegisterFunction("Framework_CCLocationsInstalled", "Completionist_Native", Framework_CCLocationsInstalled);

		a_vm->RegisterFunction("Framework_IsOptionCompleted", "Completionist_Native", Framework_IsOptionCompleted);
		a_vm->RegisterFunction("Framework_SetOptionCompleted", "Completionist_Native", Framework_SetOptionCompleted);

		a_vm->RegisterFunction("Framework_UpdatePetOwnership", "Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("ShouldDisplayMiscHeader", "Completionist_Native", CPatch_Master::PatchAPI::ShouldDisplayMiscHeader);
		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Send Notification ) ------
	//---------------------------------------------------

	void FrameworkAPI::SendNotification(std::string a_msg, std::string a_setting) {

		auto MCMScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCMScript->GetProperty(a_setting)->GetBool()) { return; }

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, MCMScript->GetProperty("ColourString")->GetString(), a_msg);
		if (!MCMScript->GetProperty("NotificationColourEnabled")->GetBool()) { RE::DebugNotification(a_msg.c_str()); return; }
		RE::DebugNotification(message.c_str());
	}

	//---------------------------------------------------
	//-- Framework Functions ( CC Variable Setter ) -----
	//---------------------------------------------------

	bool FrameworkAPI::Framework_CCLocationsInstalled(RE::StaticFunctionTag*) {

		return 
			bool(CFramework_CreationClub_L::Data.data.size());
	}

	bool FrameworkAPI::Framework_CCBooksInstalled(RE::StaticFunctionTag*) {

		return 
			bool(CFramework_CreationClub_B::Data.data.size());;
	}

	bool FrameworkAPI::Framework_CCItemsInstalled(RE::StaticFunctionTag*) {

		return
			bool(CFramework_CreationClub_A::Data.data.size()) || 
			bool(CFramework_CreationClub_W::Data.data.size()) ||
			bool(CFramework_CreationClub_I::Data.data.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Load Frameworks ) --------
	//---------------------------------------------------

	void FrameworkAPI::Framework_Load() {

		//Items
		CFramework_Armor::CHandler::UpdateFoundForms();
		CFramework_Items::CHandler::UpdateFoundForms();
		CFramework_Liquor::CHandler::UpdateFoundForms();
		CFramework_Jewelry::CHandler::UpdateFoundForms();
		CFramework_Weapons::CHandler::UpdateFoundForms();
		CFramework_Barenziah::CHandler::UpdateFoundForms();

		//Dragon Claws
		CFramework_DragonClaws_V::CHandler::UpdateFoundForms();
		CFramework_DragonClaws_P::CHandler::UpdateFoundForms();
		CFramework_DragonMasks_V::CHandler::UpdateFoundForms();
		CFramework_DragonMasks_P::CHandler::UpdateFoundForms();

		//Misc
		CFramework_Pets::CHandler::UpdateFoundForms();
		CFramework_MapsManager::CHandler::UpdateFoundForms();
		CFramework_BooksManager::CHandler::UpdateFoundForms();
		CFramework_LocationsManager::CHandler::UpdateFoundForms();
		CFramework_EnchantmentsManager::CHandler::UpdateFoundForms();

		// Blessings 
		CFramework_Shrines_V::CHandler::UpdateFoundForms();
		CFramework_Shrines_P::CHandler::UpdateFoundForms();
		CFramework_Doomstones::CHandler::UpdateFoundForms();

		//Player Homes
		CFramework_PlayerHomes_V::CHandler::UpdateFoundForms();
		CFramework_PlayerHomes_C::CHandler::UpdateFoundForms();
		CFramework_PlayerHomes_P::CHandler::UpdateFoundForms();

		//Creation Club
		CFramework_CCManager::CHandler::UpdateFoundForms();

		// Patches
		CPatch_Master::PatchAPI::LoadallPatches();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::GetCountValues(a_ID, "Total");
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::ReturnEntriesInt(a_ID, "Total");
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 0:
			return CFramework_Armor::EntriesTotal;

		case 1:
			return CFramework_Jewelry::EntriesTotal;

		case 2:
			return CFramework_DragonClaws_V::EntriesTotal;

		case 3:
			return CFramework_DragonClaws_P::EntriesTotal;

		case 4:
			return CFramework_Liquor::EntriesTotal;

		case 5:
			return CFramework_DragonMasks_V::EntriesTotal;

		case 6:
			return CFramework_DragonMasks_P::EntriesTotal;

		case 7:
			return CFramework_Weapons::EntriesTotal;

		case 8:
			return CFramework_Items::EntriesTotal;

		case 9:
			return CFramework_Pets::Pet_1_EntriesTotal;

		case 29:
			return CFramework_Barenziah::EntriesTotal;

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::EntriesTotal;

		case 11:
			return CFramework_Books_HS::EntriesTotal;

		case 12:
			return CFramework_Books_TY::EntriesTotal;

		case 13:
			return CFramework_Books_SB::EntriesTotal;

		case 14:
			return CFramework_Books_ST::EntriesTotal;

		case 15:
			return CFramework_Books_DG::EntriesTotal;

		case 16:
			return CFramework_Books_DGS::EntriesTotal;

		case 17:
			return CFramework_Books_DB::EntriesTotal;

		case 18:
			return CFramework_Books_DBS::EntriesTotal;

		case 19:
			return CFramework_Pets::Pet_2_EntriesTotal;

		case 37:
			return CFramework_Maps_V::EntriesTotal;

		case 38:
			return CFramework_Maps_NTH::EntriesTotal;

		case 39:
			return CFramework_Maps_TH::EntriesTotal;

			//-------------------------------------------- Locations

		case 20:
			return CFramework_Locations_DG::EntriesTotal;

		case 21:
			return CFramework_Locations_DB::EntriesTotal;

		case 23:
			return CFramework_Locations_AG::EntriesTotal;

		case 24:
			return CFramework_Locations_HR::EntriesTotal;

		case 25:
			return CFramework_Locations_SZ::EntriesTotal;

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::EntriesTotal;

		case 27:
			return CFramework_Shrines_V::EntriesTotal;

		case 28:
			return CFramework_Shrines_P::EntriesTotal;

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::EntriesTotal;

		case 31:
			return CFramework_AEnchantments_P::EntriesTotal;

		case 32:
			return CFramework_WEnchantments_V::EntriesTotal;

		case 33:
			return CFramework_WEnchantments_P::EntriesTotal;

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::EntriesTotal;

		case 35:
			return CFramework_PlayerHomes_C::EntriesTotal;

		case 36:
			return CFramework_PlayerHomes_P::EntriesTotal;

			//-------------------------------------------- Creation CLub

		case 40:
			return CFramework_CreationClub_L::EntriesTotal;

		case 41:
			return CFramework_CreationClub_B::EntriesTotal;

		case 42:
			return CFramework_CreationClub_S::EntriesTotal;

		case 43:
			return CFramework_CreationClub_A::EntriesTotal;

		case 44:
			return CFramework_CreationClub_W::EntriesTotal;

		case 45:
			return CFramework_CreationClub_I::EntriesTotal;

		default:
			return -1;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::GetCountValues(a_ID, "Found");
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::ReturnEntriesInt(a_ID, "Found");
		}

		switch (a_ID) {

		//-------------------------------------------- Items

	case 0:
		return CFramework_Armor::EntriesFound;

	case 1:
		return CFramework_Jewelry::EntriesFound;

	case 2:
		return CFramework_DragonClaws_V::EntriesFound;

	case 3:
		return CFramework_DragonClaws_P::EntriesFound;

	case 4:
		return CFramework_Liquor::EntriesFound;

	case 5:
		return CFramework_DragonMasks_V::EntriesFound;

	case 6:
		return CFramework_DragonMasks_P::EntriesFound;

	case 7:
		return CFramework_Weapons::EntriesFound;

	case 8:
		return CFramework_Items::EntriesFound;

	case 9:
		return CFramework_Pets::Pet_1_EntriesFound;

	case 29:
		return CFramework_Barenziah::EntriesFound;

		//-------------------------------------------- Books

	case 10:
		return CFramework_Books_AG::EntriesFound;

	case 11:
		return CFramework_Books_HS::EntriesFound;

	case 12:
		return CFramework_Books_TY::EntriesFound;

	case 13:
		return CFramework_Books_SB::EntriesFound;

	case 14:
		return CFramework_Books_ST::EntriesFound;

	case 15:
		return CFramework_Books_DG::EntriesFound;

	case 16:
		return CFramework_Books_DGS::EntriesFound;

	case 17:
		return CFramework_Books_DB::EntriesFound;

	case 18:
		return CFramework_Books_DBS::EntriesFound;

	case 19:
		return CFramework_Pets::Pet_2_EntriesFound;

	case 37:
		return CFramework_Maps_V::EntriesFound;

	case 38:
		return CFramework_Maps_NTH::EntriesFound;

	case 39:
		return CFramework_Maps_TH::EntriesFound;

		//-------------------------------------------- Locations

	case 20:
		return CFramework_Locations_DG::EntriesFound;

	case 21:
		return CFramework_Locations_DB::EntriesFound;

	case 23:
		return CFramework_Locations_AG::EntriesFound;

	case 24:
		return CFramework_Locations_HR::EntriesFound;

	case 25:
		return CFramework_Locations_SZ::EntriesFound;

		//-------------------------------------------- Blessings

	case 26:
		return CFramework_Doomstones::EntriesFound;

	case 27:
		return CFramework_Shrines_V::EntriesFound;

	case 28:
		return CFramework_Shrines_P::EntriesFound;

		//-------------------------------------------- Enchantments

	case 30:
		return CFramework_AEnchantments_V::EntriesFound;

	case 31:
		return CFramework_AEnchantments_P::EntriesFound;

	case 32:
		return CFramework_WEnchantments_V::EntriesFound;

	case 33:
		return CFramework_WEnchantments_P::EntriesFound;

		//-------------------------------------------- Player Homes

	case 34:
		return CFramework_PlayerHomes_V::EntriesFound;

	case 35:
		return CFramework_PlayerHomes_C::EntriesFound;

	case 36:
		return CFramework_PlayerHomes_P::EntriesFound;

		//-------------------------------------------- Creation CLub

	case 40:
		return CFramework_CreationClub_L::EntriesFound;

	case 41:
		return CFramework_CreationClub_B::EntriesFound;

	case 42:
		return CFramework_CreationClub_S::EntriesFound;

	case 43:
		return CFramework_CreationClub_A::EntriesFound;

	case 44:
		return CFramework_CreationClub_W::EntriesFound;

	case 45:
		return CFramework_CreationClub_I::EntriesFound;

		default:
			return -1;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::Framework_GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			INFO("Diverting To Patch Master Due To ID Being {} ", a_ID);
			return CPatch_Master::PatchAPI::GetFormArrayByID(a_ID);
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			INFO("Diverting To PetFramework Due To ID Being {} ", a_ID);
			return CFramework_Pets::CHandler::ReturnEntriesForm(a_ID);
		}

		INFO("Entering Switch Case for Framework - {} ", a_ID);

		switch (a_ID) {

			//-------------------------------------------- Items

		case 00:
			return CFramework_Armor::FormArray;

		case 01:
			return CFramework_Jewelry::FormArray;

		case 02:
			return CFramework_DragonClaws_V::FormArray;

		case 03:
			return CFramework_DragonClaws_P::FormArray;

		case 04:
			return CFramework_Liquor::FormArray;

		case 05:
			return CFramework_DragonMasks_V::FormArray;

		case 06:
			return CFramework_DragonMasks_P::FormArray;

		case 07:
			return CFramework_Weapons::FormArray;

		case 8:
			return CFramework_Items::FormArray;

		case 9:
			return CFramework_Pets::Pet_1_FormArray;

		case 29:
			return CFramework_Barenziah::FormArray;

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::FormArray;

		case 11:
			return CFramework_Books_HS::FormArray;

		case 12:
			return CFramework_Books_TY::FormArray;

		case 13:
			return CFramework_Books_SB::FormArray;

		case 14:
			return CFramework_Books_ST::FormArray;

		case 15:
			return CFramework_Books_DG::FormArray;

		case 16:
			return CFramework_Books_DGS::FormArray;

		case 17:
			return CFramework_Books_DB::FormArray;

		case 18:
			return CFramework_Books_DBS::FormArray;

		case 19:
			return CFramework_Pets::Pet_2_FormArray;

		case 37:
			return CFramework_Maps_V::FormArray;

		case 38:
			return CFramework_Maps_NTH::FormArray;

		case 39:
			return CFramework_Maps_TH::FormArray;

			//-------------------------------------------- Locations

		case 20:
			return CFramework_Locations_DG::FormArray;

		case 21:
			return CFramework_Locations_DB::FormArray;

		case 23:
			return CFramework_Locations_AG::FormArray;

		case 24:
			return CFramework_Locations_HR::FormArray;

		case 25:
			return CFramework_Locations_SZ::FormArray;

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::FormArray;

		case 27:
			return CFramework_Shrines_V::FormArray;

		case 28:
			return CFramework_Shrines_P::FormArray;

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::FormArray;

		case 31:
			return CFramework_AEnchantments_P::FormArray;

		case 32:
			return CFramework_WEnchantments_V::FormArray;

		case 33:
			return CFramework_WEnchantments_P::FormArray;

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::FormArray;

		case 35:
			return CFramework_PlayerHomes_C::FormArray;

		case 36:
			return CFramework_PlayerHomes_P::FormArray;

			//-------------------------------------------- Creation CLub

		case 40:
			return CFramework_CreationClub_L::FormArray;

		case 41:
			return CFramework_CreationClub_B::FormArray;

		case 42:
			return CFramework_CreationClub_S::FormArray;

		case 43:
			return CFramework_CreationClub_A::FormArray;

		case 44:
			return CFramework_CreationClub_W::FormArray;

		case 45:
			return CFramework_CreationClub_I::FormArray;

		default:
			return EmptyFormArray;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::Framework_GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::GetStringValues(a_ID, "Name");
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::ReturnEntriesString(a_ID, "Name");
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 0:
			//return FrameworkHandler::HandleNameSet(FrameworkHandler::FrameworkID(a_ID));
			return CFramework_Armor::NameArray;

		case 1:
			return CFramework_Jewelry::NameArray;

		case 2:
			return CFramework_DragonClaws_V::NameArray;

		case 3:
			return CFramework_DragonClaws_P::NameArray;

		case 4:
			return CFramework_Liquor::NameArray;

		case 5:
			return CFramework_DragonMasks_V::NameArray;

		case 6:
			return CFramework_DragonMasks_P::NameArray;

		case 7:
			return CFramework_Weapons::NameArray;

		case 8:
			return CFramework_Items::NameArray;

		case 9:
			return CFramework_Pets::Pet_1_NameArray;

		case 29:
			return CFramework_Barenziah::NameArray;

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::NameArray;

		case 11:
			return CFramework_Books_HS::NameArray;

		case 12:
			return CFramework_Books_TY::NameArray;

		case 13:
			return CFramework_Books_SB::NameArray;

		case 14:
			return CFramework_Books_ST::NameArray;

		case 15:
			return CFramework_Books_DG::NameArray;

		case 16:
			return CFramework_Books_DGS::NameArray;

		case 17:
			return CFramework_Books_DB::NameArray;

		case 18:
			return CFramework_Books_DBS::NameArray;

		case 19:
			return CFramework_Pets::Pet_2_NameArray;

		case 37:
			return CFramework_Maps_V::NameArray;

		case 38:
			return CFramework_Maps_NTH::NameArray;

		case 39:
			return CFramework_Maps_TH::NameArray;

			//-------------------------------------------- Locations

		case 20:
			return CFramework_Locations_DG::NameArray;

		case 21:
			return CFramework_Locations_DB::NameArray;

		case 23:
			return CFramework_Locations_AG::NameArray;

		case 24:
			return CFramework_Locations_HR::NameArray;

		case 25:
			return CFramework_Locations_SZ::NameArray;

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::NameArray;

		case 27:
			return CFramework_Shrines_V::NameArray;

		case 28:
			return CFramework_Shrines_P::NameArray;

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::NameArray;

		case 31:
			return CFramework_AEnchantments_P::NameArray;

		case 32:
			return CFramework_WEnchantments_V::NameArray;

		case 33:
			return CFramework_WEnchantments_P::NameArray;

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::NameArray;

		case 35:
			return CFramework_PlayerHomes_C::NameArray;

		case 36:
			return CFramework_PlayerHomes_P::NameArray;

			//-------------------------------------------- Creation CLub

		case 40:
			return CFramework_CreationClub_L::NameArray;

		case 41:
			return CFramework_CreationClub_B::NameArray;

		case 42:
			return CFramework_CreationClub_S::NameArray;

		case 43:
			return CFramework_CreationClub_A::NameArray;

		case 44:
			return CFramework_CreationClub_W::NameArray;

		case 45:
			return CFramework_CreationClub_I::NameArray;

		default:
			return EmptyNameArray;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::Framework_GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::GetStringValues(a_ID, "Text");
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::ReturnEntriesString(a_ID, "Text");
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 00:
			return CFramework_Armor::TextArray;

		case 01:
			return CFramework_Jewelry::TextArray;

		case 02:
			return CFramework_DragonClaws_V::TextArray;

		case 03:
			return CFramework_DragonClaws_P::TextArray;

		case 04:
			return CFramework_Liquor::TextArray;

		case 05:
			return CFramework_DragonMasks_V::TextArray;

		case 06:
			return CFramework_DragonMasks_P::TextArray;

		case 07:
			return CFramework_Weapons::TextArray;

		case 8:
			return CFramework_Items::TextArray;

		case 9:
			return CFramework_Pets::Pet_1_TextArray;

		case 29:
			return CFramework_Barenziah::TextArray;

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::TextArray;

		case 11:
			return CFramework_Books_HS::TextArray;

		case 12:
			return CFramework_Books_TY::TextArray;

		case 13:
			return CFramework_Books_SB::TextArray;

		case 14:
			return CFramework_Books_ST::TextArray;

		case 15:
			return CFramework_Books_DG::TextArray;

		case 16:
			return CFramework_Books_DGS::TextArray;

		case 17:
			return CFramework_Books_DB::TextArray;

		case 18:
			return CFramework_Books_DBS::TextArray;

		case 19:
			return CFramework_Pets::Pet_2_TextArray;

		case 37:
			return CFramework_Maps_V::TextArray;

		case 38:
			return CFramework_Maps_NTH::TextArray;

		case 39:
			return CFramework_Maps_TH::TextArray;

			//-------------------------------------------- Locations

		case 20:
			return CFramework_Locations_DG::TextArray;

		case 21:
			return CFramework_Locations_DB::TextArray;

		case 23:
			return CFramework_Locations_AG::TextArray;

		case 24:
			return CFramework_Locations_HR::TextArray;

		case 25:
			return CFramework_Locations_SZ::TextArray;

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::TextArray;

		case 27:
			return CFramework_Shrines_V::TextArray;

		case 28:
			return CFramework_Shrines_P::TextArray;

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::TextArray;

		case 31:
			return CFramework_AEnchantments_P::TextArray;

		case 32:
			return CFramework_WEnchantments_V::TextArray;

		case 33:
			return CFramework_WEnchantments_P::TextArray;

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::TextArray;

		case 35:
			return CFramework_PlayerHomes_C::TextArray;

		case 36:
			return CFramework_PlayerHomes_P::TextArray;

			//-------------------------------------------- Creation CLub

		case 40:
			return CFramework_CreationClub_L::TextArray;

		case 41:
			return CFramework_CreationClub_B::TextArray;

		case 42:
			return CFramework_CreationClub_S::TextArray;

		case 43:
			return CFramework_CreationClub_A::TextArray;

		case 44:
			return CFramework_CreationClub_W::TextArray;

		case 45:
			return CFramework_CreationClub_I::TextArray;

		default:
			return EmptyTextArray;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::Framework_GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::GetBoolArrayByID(a_ID);
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::ReturnEntriesBool(a_ID);
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 00:
			return CFramework_Armor::BoolArray;

		case 01:
			return CFramework_Jewelry::BoolArray;

		case 02:
			return CFramework_DragonClaws_V::BoolArray;

		case 03:
			return CFramework_DragonClaws_P::BoolArray;

		case 04:
			return CFramework_Liquor::BoolArray;

		case 05:
			return CFramework_DragonMasks_V::BoolArray;

		case 06:
			return CFramework_DragonMasks_P::BoolArray;

		case 07:
			return CFramework_Weapons::BoolArray;

		case 8:
			return CFramework_Items::BoolArray;

		case 9:
			return CFramework_Pets::Pet_1_BoolArray;

		case 29:
			return CFramework_Barenziah::BoolArray;

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::BoolArray;

		case 11:
			return CFramework_Books_HS::BoolArray;

		case 12:
			return CFramework_Books_TY::BoolArray;

		case 13:
			return CFramework_Books_SB::BoolArray;

		case 14:
			return CFramework_Books_ST::BoolArray;

		case 15:
			return CFramework_Books_DG::BoolArray;

		case 16:
			return CFramework_Books_DGS::BoolArray;

		case 17:
			return CFramework_Books_DB::BoolArray;

		case 18:
			return CFramework_Books_DBS::BoolArray;

		case 19:
			return CFramework_Pets::Pet_2_BoolArray;

		case 37:
			return CFramework_Maps_V::BoolArray;

		case 38:
			return CFramework_Maps_NTH::BoolArray;

		case 39:
			return CFramework_Maps_TH::BoolArray;

			//-------------------------------------------- Locations

		case 20:
			return CFramework_Locations_DG::BoolArray;

		case 21:
			return CFramework_Locations_DB::BoolArray;

		case 23:
			return CFramework_Locations_AG::BoolArray;

		case 24:
			return CFramework_Locations_HR::BoolArray;

		case 25:
			return CFramework_Locations_SZ::BoolArray;

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::BoolArray;

		case 27:
			return CFramework_Shrines_V::BoolArray;

		case 28:
			return CFramework_Shrines_P::BoolArray;

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::BoolArray;

		case 31:
			return CFramework_AEnchantments_P::BoolArray;

		case 32:
			return CFramework_WEnchantments_V::BoolArray;

		case 33:
			return CFramework_WEnchantments_P::BoolArray;

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::BoolArray;

		case 35:
			return CFramework_PlayerHomes_C::BoolArray;

		case 36:
			return CFramework_PlayerHomes_P::BoolArray;

			//-------------------------------------------- Creation CLub

		case 40:
			return CFramework_CreationClub_L::BoolArray;

		case 41:
			return CFramework_CreationClub_B::BoolArray;

		case 42:
			return CFramework_CreationClub_S::BoolArray;

		case 43:
			return CFramework_CreationClub_A::BoolArray;

		case 44:
			return CFramework_CreationClub_W::BoolArray;

		case 45:
			return CFramework_CreationClub_I::BoolArray;

		default:
			return EmptyBoolArray;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_name) {

		if (a_ID >= 200) {
			return CPatch_Master::PatchAPI::IsOptionCompleted(a_ID, a_name);
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			return CFramework_Pets::CHandler::IsOptionCompleted(a_ID, a_name);
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 00:
			return CFramework_Armor::CHandler::IsOptionCompleted(a_name);

		case 01:
			return CFramework_Jewelry::CHandler::IsOptionCompleted(a_name);

		case 02:
			return CFramework_DragonClaws_V::CHandler::IsOptionCompleted(a_name);

		case 03:
			return CFramework_DragonClaws_P::CHandler::IsOptionCompleted(a_name);

		case 04:
			return CFramework_Liquor::CHandler::IsOptionCompleted(a_name);

		case 05:
			return CFramework_DragonMasks_V::CHandler::IsOptionCompleted(a_name);

		case 06:
			return CFramework_DragonMasks_P::CHandler::IsOptionCompleted(a_name);

		case 07:
			return CFramework_Weapons::CHandler::IsOptionCompleted(a_name);

		case 8:
			return CFramework_Items::CHandler::IsOptionCompleted(a_name);

		case 9:
			return CFramework_Pets::CHandler::IsOptionCompleted(a_ID, a_name);

		case 29:
			return CFramework_Barenziah::CHandler::IsOptionCompleted(a_name);

			//-------------------------------------------- Books

		case 10:
			return CFramework_Books_AG::BookHandler::IsOptionCompleted(a_name);

		case 11:
			return CFramework_Books_HS::BookHandler::IsOptionCompleted(a_name);

		case 12:
			return CFramework_Books_TY::BookHandler::IsOptionCompleted(a_name);

		case 13:
			return CFramework_Books_SB::BookHandler::IsOptionCompleted(a_name);

		case 14:
			return CFramework_Books_ST::BookHandler::IsOptionCompleted(a_name);

		case 15:
			return CFramework_Books_DG::BookHandler::IsOptionCompleted(a_name);

		case 16:
			return CFramework_Books_DGS::BookHandler::IsOptionCompleted(a_name);

		case 17:
			return CFramework_Books_DB::BookHandler::IsOptionCompleted(a_name);

		case 18:
			return CFramework_Books_DBS::BookHandler::IsOptionCompleted(a_name);

		case 19:
			return CFramework_Pets::CHandler::IsOptionCompleted(a_ID, a_name);

		case 37:
			return CFramework_Maps_V::BookHandler::IsOptionCompleted(a_name);

		case 38:
			return CFramework_Maps_NTH::BookHandler::IsOptionCompleted(a_name);

		case 39:
			return CFramework_Maps_TH::BookHandler::IsOptionCompleted(a_name);

			//-------------------------------------------- Blessings

		case 26:
			return CFramework_Doomstones::CHandler::IsOptionCompleted(a_name);

		case 27:
			return CFramework_Shrines_V::CHandler::IsOptionCompleted(a_name);

		case 28:
			return CFramework_Shrines_P::CHandler::IsOptionCompleted(a_name);

			//-------------------------------------------- Enchantments

		case 30:
			return CFramework_AEnchantments_V::CHandler::IsOptionCompleted(a_name);

		case 31:
			return CFramework_AEnchantments_P::CHandler::IsOptionCompleted(a_name);

		case 32:
			return CFramework_WEnchantments_V::CHandler::IsOptionCompleted(a_name);

		case 33:
			return CFramework_WEnchantments_P::CHandler::IsOptionCompleted(a_name);

			//-------------------------------------------- Player Homes

		case 34:
			return CFramework_PlayerHomes_V::CHandler::IsOptionCompleted(a_name);

		case 35:
			return CFramework_PlayerHomes_C::CHandler::IsOptionCompleted(a_name);

		case 36:
			return CFramework_PlayerHomes_P::CHandler::IsOptionCompleted(a_name);

		//-------------------------------------------- Creation CLub

		case 41:
			return CFramework_CreationClub_B::BookHandler::IsOptionCompleted(a_name);

		case 42:
			return CFramework_CreationClub_S::BookHandler::IsOptionCompleted(a_name);

		case 43:
			return CFramework_CreationClub_A::CHandler::IsOptionCompleted(a_name);

		case 44:
			return CFramework_CreationClub_W::CHandler::IsOptionCompleted(a_name);

		case 45:
			return CFramework_CreationClub_I::CHandler::IsOptionCompleted(a_name);

		default:
			return -1;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::Framework_SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_name) {

		if (a_ID >= 200) {
			CPatch_Master::PatchAPI::SetOptionCompleted(a_ID, a_name);
			return;
		}

		if (a_ID == CFramework_Pets::kPet_1 || a_ID == CFramework_Pets::kPet_2 ||
			a_ID == CFramework_Pets::kPet_3 || a_ID == CFramework_Pets::kPet_4) {
			CFramework_Pets::CHandler::SetOptionCompleted(a_ID, a_name);
		}

		switch (a_ID) {

			//-------------------------------------------- Items

		case 00:
			CFramework_Armor::CHandler::SetOptionCompleted(a_name);
			break;

		case 01:
			CFramework_Jewelry::CHandler::SetOptionCompleted(a_name);
			break;

		case 02:
			CFramework_DragonClaws_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 03:
			CFramework_DragonClaws_P::CHandler::SetOptionCompleted(a_name);
			break;

		case 04:
			CFramework_Liquor::CHandler::SetOptionCompleted(a_name);
			break;

		case 05:
			CFramework_DragonMasks_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 06:
			CFramework_DragonMasks_P::CHandler::SetOptionCompleted(a_name);
			break;

		case 07:
			CFramework_Weapons::CHandler::SetOptionCompleted(a_name);
			break;

		case 8:
			CFramework_Items::CHandler::SetOptionCompleted(a_name);
			break;

		case 9:
			CFramework_Pets::CHandler::SetOptionCompleted(a_ID, a_name);
			break;

		case 29:
			CFramework_Barenziah::CHandler::SetOptionCompleted(a_name);
			break;

			//-------------------------------------------- Books

		case 10:
			CFramework_Books_AG::BookHandler::SetOptionCompleted(a_name);
			break;

		case 11:
			CFramework_Books_HS::BookHandler::SetOptionCompleted(a_name);
			break;

		case 12:
			CFramework_Books_TY::BookHandler::SetOptionCompleted(a_name);
			break;

		case 13:
			CFramework_Books_SB::BookHandler::SetOptionCompleted(a_name);
			break;

		case 14:
			CFramework_Books_ST::BookHandler::SetOptionCompleted(a_name);
			break;

		case 15:
			CFramework_Books_DG::BookHandler::SetOptionCompleted(a_name);
			break;

		case 16:
			CFramework_Books_DGS::BookHandler::SetOptionCompleted(a_name);
			break;

		case 17:
			CFramework_Books_DB::BookHandler::SetOptionCompleted(a_name);
			break;

		case 18:
			CFramework_Books_DBS::BookHandler::SetOptionCompleted(a_name);
			break;

		case 19:
			CFramework_Pets::CHandler::SetOptionCompleted(a_ID, a_name);
			break;

		case 37:
			CFramework_Maps_V::BookHandler::SetOptionCompleted(a_name);
			break;

		case 38:
			CFramework_Maps_NTH::BookHandler::SetOptionCompleted(a_name);
			break;

		case 39:
			CFramework_Maps_TH::BookHandler::SetOptionCompleted(a_name);
			break;

			//-------------------------------------------- Blessings

		case 26:
			CFramework_Doomstones::CHandler::SetOptionCompleted(a_name);
			break;

		case 27:
			CFramework_Shrines_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 28:
			CFramework_Shrines_P::CHandler::SetOptionCompleted(a_name);
			break;

			//-------------------------------------------- Enchantments

		case 30:
			CFramework_AEnchantments_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 31:
			CFramework_AEnchantments_P::CHandler::SetOptionCompleted(a_name);
			break;

		case 32:
			CFramework_WEnchantments_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 33:
			CFramework_WEnchantments_P::CHandler::SetOptionCompleted(a_name);
			break;

			//-------------------------------------------- Player Homes

		case 34:
			CFramework_PlayerHomes_V::CHandler::SetOptionCompleted(a_name);
			break;

		case 35:
			CFramework_PlayerHomes_C::CHandler::SetOptionCompleted(a_name);
			break;

		case 36:
			CFramework_PlayerHomes_P::CHandler::SetOptionCompleted(a_name);
			break;

			//-------------------------------------------- Creation CLub

		case 41:
			CFramework_CreationClub_B::BookHandler::SetOptionCompleted(a_name);
			break;

		case 42:
			CFramework_CreationClub_S::BookHandler::SetOptionCompleted(a_name);
			break;

		case 43:
			CFramework_CreationClub_A::CHandler::SetOptionCompleted(a_name);
			break;

		case 44:
			CFramework_CreationClub_W::CHandler::SetOptionCompleted(a_name);
			break;

		case 45:
			CFramework_CreationClub_I::CHandler::SetOptionCompleted(a_name);
			break;

		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Get Skill Book AVal ) ----
	//---------------------------------------------------

	std::string FrameworkAPI::GetBookSkill(RE::ActorValue a_val){

		switch (a_val) {

		case RE::ActorValue::kOneHanded:
			return "One-Handed";

		case RE::ActorValue::kTwoHanded:
			return "Two-Handed";

		case RE::ActorValue::kArchery:
			return "Marksman";

		case RE::ActorValue::kBlock:
			return "Block";

		case RE::ActorValue::kSmithing:
			return "Smithing";

		case RE::ActorValue::kHeavyArmor:
			return "Heavy Armor";

		case RE::ActorValue::kLightArmor:
			return "Light Armor";

		case RE::ActorValue::kPickpocket:
			return "Pickpocket";

		case RE::ActorValue::kLockpicking:
			return "LockPicking";

		case RE::ActorValue::kSneak:
			return "Sneak";

		case RE::ActorValue::kAlchemy:
			return "Alchemy";

		case RE::ActorValue::kSpeech:
			return "SpeechCraft";

		case RE::ActorValue::kAlteration:
			return "Alteration";

		case RE::ActorValue::kConjuration:
			return "Conjuration";

		case RE::ActorValue::kDestruction:
			return "Destruction";

		case RE::ActorValue::kIllusion:
			return "Illusion";

		case RE::ActorValue::kRestoration:
			return "Restoration";

		case RE::ActorValue::kEnchanting:
			return "Enchanting";

		default:
			return "";
		}
	}
}
