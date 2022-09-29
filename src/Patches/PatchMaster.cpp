#include "PatchMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

//Patches
#include "Patches/Fishing/CFramework_Fishing.hpp"
#include "AdditionalHearthfireDolls/CFramework_AHD.hpp"
#include "BrotherhoodOfOld/CFramework_BOO.hpp"
#include "Clockwork/CFramework_CLW.hpp"
#include "Falskaar/CFramework_FSK.hpp"
#include "FossilMining/CFramework_FOS.hpp"
#include "GrayCowlOfNocturnal/CFramework_GCN.hpp"
#include "OblivionArtifacts/CFramework_OAP.hpp"
#include "HelgenReborn/CFramework_HRB.hpp"
#include "InterestingNPCs/CFramework_3DC.hpp"
#include "MoonAndStar/CFramework_MAS.hpp"
#include "MoonpathToElsweyr/CFramework_MTE.hpp"
#include "ProjectAHO/CFramework_AHO.hpp"
#include "TeldrynSerious/CFramework_TEL.hpp"
#include "Thunderchild/CFramework_THU.hpp"
#include "Undeath/CFramework_UND.hpp"
#include "WheelsOfLull/CFramework_WOL.hpp"
#include "Wintersun/CFramework_WSN.hpp"
#include "Wyrmstooth/CFramework_WYR.hpp"
#include "Vigilant/CFramework_VIG.hpp"

//Skyrim Unique Treasures
#include "SkyrimUniqueTreasures/CFramework_ST1.hpp"
#include "SkyrimUniqueTreasures/CFramework_ST2.hpp"
#include "SkyrimUniqueTreasures/CFramework_ST3.hpp"

namespace CPatch_Master {

	int PatchesInstalled;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void PatchAPI::InstallPatches() {

		CPatch_AHD::CHandler::InstallFramework();
		CPatch_BOO::CHandler::InstallFramework();
		CPatch_CLW::CHandler::InstallFramework();
		CPatch_FSK::CHandler::InstallFramework();
		CPatch_FOS::CHandler::InstallFramework();
		CPatch_GCN::CHandler::InstallFramework();
		CPatch_OAP::CHandler::InstallFramework();
		CPatch_HRB::CHandler::InstallFramework();
		CPatch_3DC::CHandler::InstallFramework();
		CPatch_MAS::CHandler::InstallFramework();
		CPatch_MTE::CHandler::InstallFramework();
		CPatch_AHO::CHandler::InstallFramework();
		CPatch_ST1::CHandler::InstallFramework();
		CPatch_ST2::CHandler::InstallFramework();
		CPatch_ST3::CHandler::InstallFramework();
		CPatch_TEL::CHandler::InstallFramework();
		CPatch_THU::CHandler::InstallFramework();
		CPatch_UND::CHandler::InstallFramework();
		CPatch_WOL::CHandler::InstallFramework();
		CPatch_WSN::CHandler::InstallFramework();
		CPatch_WYR::CHandler::InstallFramework();
		CPatch_VIG::CHandler::InstallFramework();

		CPatch_Fishing::CHandler::InstallFramework();
	}

	//---------------------------------------------------
	//-- Patches Functions ( Master Registration ) ------
	//---------------------------------------------------

	void PatchAPI::LoadallPatches() {

		CPatch_AHD::CHandler::UpdateFoundForms();
		CPatch_BOO::CHandler::UpdateFoundForms();
		CPatch_CLW::CHandler::UpdateFoundForms();
		CPatch_FSK::CHandler::UpdateFoundForms();
		CPatch_FOS::CHandler::UpdateFoundForms();
		CPatch_GCN::CHandler::UpdateFoundForms();
		CPatch_OAP::CHandler::UpdateFoundForms();
		CPatch_HRB::CHandler::UpdateFoundForms();
		CPatch_3DC::CHandler::UpdateFoundForms();
		CPatch_MAS::CHandler::UpdateFoundForms();
		CPatch_MTE::CHandler::UpdateFoundForms();
		CPatch_AHO::CHandler::UpdateFoundForms();
		CPatch_ST1::CHandler::UpdateFoundForms();
		CPatch_ST2::CHandler::UpdateFoundForms();
		CPatch_ST3::CHandler::UpdateFoundForms();
		CPatch_TEL::CHandler::UpdateFoundForms();
		CPatch_THU::CHandler::UpdateFoundForms();
		CPatch_UND::CHandler::UpdateFoundForms();
		CPatch_WOL::CHandler::UpdateFoundForms();
		CPatch_WSN::CHandler::UpdateFoundForms();
		CPatch_WYR::CHandler::UpdateFoundForms();
		CPatch_VIG::CHandler::UpdateFoundForms();
		
		CPatch_Fishing::CHandler::UpdateFoundForms();
	}

	//---------------------------------------------------
	//-- Patches Functions ( Master Registration ) ------
	//---------------------------------------------------

	bool PatchAPI::ShouldDisplayMiscHeader(RE::StaticFunctionTag*) {

		return bool(PatchesInstalled);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Counts ) --------
	//---------------------------------------------------

	std::int32_t PatchAPI::GetCountValues(std::int32_t a_patchID, std::string a_section) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B ||
			a_patchID == CPatch_Fishing::ksection_A || a_patchID == CPatch_Fishing::ksection_C || a_patchID == CPatch_Fishing::ksection_L ||
			a_patchID == CPatch_Fishing::ksection_S) {
			return CPatch_Fishing::CHandler::ReturnEntriesInt(a_patchID, a_section);
		}

		if (a_patchID == CPatch_AHD::kItems) { return CPatch_AHD::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks || a_patchID == CPatch_BOO::KMapMa) { return CPatch_BOO::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks || a_patchID == CPatch_CLW::KMapMa) { return CPatch_CLW::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks || a_patchID == CPatch_FSK::KMapMa) { return CPatch_FSK::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks || a_patchID == CPatch_GCN::KMapMa) { return CPatch_GCN::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks || a_patchID == CPatch_OAP::KMapMa) { return CPatch_OAP::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks || a_patchID == CPatch_HRB::KMapMa) { return CPatch_HRB::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks || a_patchID == CPatch_3DC::KMapMa) { return CPatch_3DC::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks || a_patchID == CPatch_MAS::KMapMa) { return CPatch_MAS::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks || a_patchID == CPatch_AHO::KMapMa) { return CPatch_AHO::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks || a_patchID == CPatch_TEL::KMapMa) { return CPatch_TEL::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks || a_patchID == CPatch_THU::KMapMa) { return CPatch_THU::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks || a_patchID == CPatch_WOL::KMapMa) { return CPatch_WOL::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks || a_patchID == CPatch_WSN::KMapMa) { return CPatch_WSN::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks || a_patchID == CPatch_WYR::KMapMa) { return CPatch_WYR::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks || a_patchID == CPatch_VIG::KMapMa) { return CPatch_VIG::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { return CPatch_UND::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { return CPatch_MTE::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { return CPatch_FOS::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { return CPatch_ST1::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { return CPatch_ST2::CHandler::ReturnEntriesInt(a_patchID, a_section); }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { return CPatch_ST3::CHandler::ReturnEntriesInt(a_patchID, a_section); }

		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Strings ) -------
	//---------------------------------------------------

	std::vector<std::string> PatchAPI::GetStringValues(std::int32_t a_patchID, std::string a_section) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B ||
			a_patchID == CPatch_Fishing::ksection_A || a_patchID == CPatch_Fishing::ksection_C || a_patchID == CPatch_Fishing::ksection_L ||
			a_patchID == CPatch_Fishing::ksection_S) {
			return CPatch_Fishing::CHandler::ReturnEntriesString(a_patchID, a_section); 
		}

		if (a_patchID == CPatch_AHD::kItems) { return CPatch_AHD::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks || a_patchID == CPatch_BOO::KMapMa) { return CPatch_BOO::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks || a_patchID == CPatch_CLW::KMapMa) { return CPatch_CLW::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks || a_patchID == CPatch_FSK::KMapMa) { return CPatch_FSK::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks || a_patchID == CPatch_GCN::KMapMa) { return CPatch_GCN::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks || a_patchID == CPatch_OAP::KMapMa) { return CPatch_OAP::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks || a_patchID == CPatch_HRB::KMapMa) { return CPatch_HRB::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks || a_patchID == CPatch_3DC::KMapMa) { return CPatch_3DC::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks || a_patchID == CPatch_MAS::KMapMa) { return CPatch_MAS::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks || a_patchID == CPatch_AHO::KMapMa) { return CPatch_AHO::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks || a_patchID == CPatch_TEL::KMapMa) { return CPatch_TEL::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks || a_patchID == CPatch_THU::KMapMa) { return CPatch_THU::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks || a_patchID == CPatch_WOL::KMapMa) { return CPatch_WOL::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks || a_patchID == CPatch_WSN::KMapMa) { return CPatch_WSN::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks || a_patchID == CPatch_WYR::KMapMa) { return CPatch_WYR::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks || a_patchID == CPatch_VIG::KMapMa) { return CPatch_VIG::CHandler::ReturnEntriesString(a_patchID, a_section); }
		
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { return CPatch_UND::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { return CPatch_MTE::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { return CPatch_FOS::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { return CPatch_ST1::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { return CPatch_ST2::CHandler::ReturnEntriesString(a_patchID, a_section); }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { return CPatch_ST3::CHandler::ReturnEntriesString(a_patchID, a_section); }

		return {};
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> PatchAPI::GetFormArrayByID(std::int32_t a_patchID) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B ||
			a_patchID == CPatch_Fishing::ksection_A || a_patchID == CPatch_Fishing::ksection_C || a_patchID == CPatch_Fishing::ksection_L ||
			a_patchID == CPatch_Fishing::ksection_S) {
			return CPatch_Fishing::CHandler::ReturnEntriesForm(a_patchID);
		}

		if (a_patchID == CPatch_AHD::kItems) { return CPatch_AHD::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks || a_patchID == CPatch_BOO::KMapMa) { return CPatch_BOO::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks || a_patchID == CPatch_CLW::KMapMa) { return CPatch_CLW::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks || a_patchID == CPatch_FSK::KMapMa) { return CPatch_FSK::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks || a_patchID == CPatch_GCN::KMapMa) { return CPatch_GCN::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks || a_patchID == CPatch_OAP::KMapMa) { return CPatch_OAP::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks || a_patchID == CPatch_HRB::KMapMa) { return CPatch_HRB::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks || a_patchID == CPatch_3DC::KMapMa) { return CPatch_3DC::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks || a_patchID == CPatch_MAS::KMapMa) { return CPatch_MAS::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks || a_patchID == CPatch_AHO::KMapMa) { return CPatch_AHO::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks || a_patchID == CPatch_TEL::KMapMa) { return CPatch_TEL::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks || a_patchID == CPatch_THU::KMapMa) { return CPatch_THU::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks || a_patchID == CPatch_WOL::KMapMa) { return CPatch_WOL::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks || a_patchID == CPatch_WSN::KMapMa) { return CPatch_WSN::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks || a_patchID == CPatch_WYR::KMapMa) { return CPatch_WYR::CHandler::ReturnEntriesForm(a_patchID); }	
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks || a_patchID == CPatch_VIG::KMapMa) { return CPatch_VIG::CHandler::ReturnEntriesForm(a_patchID); }
		
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { return CPatch_UND::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { return CPatch_MTE::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { return CPatch_FOS::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { return CPatch_ST1::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { return CPatch_ST2::CHandler::ReturnEntriesForm(a_patchID); }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { return CPatch_ST3::CHandler::ReturnEntriesForm(a_patchID); }

		return {};
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> PatchAPI::GetBoolArrayByID(std::int32_t a_patchID) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B ||
			a_patchID == CPatch_Fishing::ksection_A || a_patchID == CPatch_Fishing::ksection_C || a_patchID == CPatch_Fishing::ksection_L ||
			a_patchID == CPatch_Fishing::ksection_S) {
			return CPatch_Fishing::CHandler::ReturnEntriesBool(a_patchID);
		}

		if (a_patchID == CPatch_AHD::kItems) { return CPatch_AHD::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks || a_patchID == CPatch_BOO::KMapMa) { return CPatch_BOO::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks || a_patchID == CPatch_CLW::KMapMa) { return CPatch_CLW::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks || a_patchID == CPatch_FSK::KMapMa) { return CPatch_FSK::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks || a_patchID == CPatch_GCN::KMapMa) { return CPatch_GCN::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks || a_patchID == CPatch_OAP::KMapMa) { return CPatch_OAP::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks || a_patchID == CPatch_HRB::KMapMa) { return CPatch_HRB::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks || a_patchID == CPatch_3DC::KMapMa) { return CPatch_3DC::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks || a_patchID == CPatch_MAS::KMapMa) { return CPatch_MAS::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks || a_patchID == CPatch_AHO::KMapMa) { return CPatch_AHO::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks || a_patchID == CPatch_TEL::KMapMa) { return CPatch_TEL::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks || a_patchID == CPatch_THU::KMapMa) { return CPatch_THU::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks || a_patchID == CPatch_WOL::KMapMa) { return CPatch_WOL::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks || a_patchID == CPatch_WSN::KMapMa) { return CPatch_WSN::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks || a_patchID == CPatch_WYR::KMapMa) { return CPatch_WYR::CHandler::ReturnEntriesBool(a_patchID); }	
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks || a_patchID == CPatch_VIG::KMapMa) { return CPatch_VIG::CHandler::ReturnEntriesBool(a_patchID); }
		
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { return CPatch_UND::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { return CPatch_MTE::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { return CPatch_FOS::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { return CPatch_ST1::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { return CPatch_ST2::CHandler::ReturnEntriesBool(a_patchID); }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { return CPatch_ST3::CHandler::ReturnEntriesBool(a_patchID); }

		return {};
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t PatchAPI::IsOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B) {
			return CPatch_Fishing::CHandler::IsOptionCompleted(a_patchID, a_name);
		}

		if (a_patchID == CPatch_AHD::kItems) { return CPatch_AHD::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks) { return CPatch_BOO::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks) { return CPatch_CLW::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks) { return CPatch_FSK::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks) { return CPatch_GCN::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks) { return CPatch_OAP::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks) { return CPatch_HRB::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks) { return CPatch_3DC::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks) { return CPatch_MAS::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks) { return CPatch_AHO::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks) { return CPatch_TEL::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks) { return CPatch_THU::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks) { return CPatch_WOL::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks) { return CPatch_WSN::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks) { return CPatch_WYR::CHandler::IsOptionCompleted(a_patchID, a_name); }	
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { return CPatch_UND::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { return CPatch_MTE::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks) { return CPatch_VIG::CHandler::IsOptionCompleted(a_patchID, a_name); }
		
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { return CPatch_FOS::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { return CPatch_ST1::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { return CPatch_ST2::CHandler::IsOptionCompleted(a_patchID, a_name); }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { return CPatch_ST3::CHandler::IsOptionCompleted(a_patchID, a_name); }

		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void PatchAPI::SetOptionCompleted(std::int32_t a_patchID, std::string a_name) {

		if (a_patchID == CPatch_Fishing::ksection_F || a_patchID == CPatch_Fishing::ksection_I || a_patchID == CPatch_Fishing::ksection_B) {
			CPatch_Fishing::CHandler::SetOptionCompleted(a_patchID, a_name);
			return;
		}

		if (a_patchID == CPatch_AHD::kItems) { CPatch_AHD::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_BOO::kItems || a_patchID == CPatch_BOO::KBooks) { CPatch_BOO::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_CLW::kItems || a_patchID == CPatch_CLW::KBooks) { CPatch_CLW::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_FSK::kItems || a_patchID == CPatch_FSK::KBooks) { CPatch_FSK::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_GCN::kItems || a_patchID == CPatch_GCN::KBooks) { CPatch_GCN::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_OAP::kItems || a_patchID == CPatch_OAP::KBooks) { CPatch_OAP::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_HRB::kItems || a_patchID == CPatch_HRB::KBooks) { CPatch_HRB::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_3DC::kItems || a_patchID == CPatch_3DC::KBooks) { CPatch_3DC::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_MAS::kItems || a_patchID == CPatch_MAS::KBooks) { CPatch_MAS::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_AHO::kItems || a_patchID == CPatch_AHO::KBooks) { CPatch_AHO::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_TEL::kItems || a_patchID == CPatch_TEL::KBooks) { CPatch_TEL::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_THU::kItems || a_patchID == CPatch_THU::KBooks) { CPatch_THU::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_WOL::kItems || a_patchID == CPatch_WOL::KBooks) { CPatch_WOL::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_WSN::kItems || a_patchID == CPatch_WSN::KBooks) { CPatch_WSN::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_WYR::kItems || a_patchID == CPatch_WYR::KBooks) { CPatch_WYR::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_UND::KBooks || a_patchID == CPatch_UND::KMapMa) { CPatch_UND::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_MTE::kItems || a_patchID == CPatch_MTE::KBooks) { CPatch_MTE::CHandler::SetOptionCompleted(a_patchID, a_name); return; }	
		if (a_patchID == CPatch_VIG::kItems || a_patchID == CPatch_VIG::KBooks) { CPatch_VIG::CHandler::SetOptionCompleted(a_patchID, a_name); return; }	
		
		if (a_patchID == CPatch_FOS::kItmL1 || a_patchID == CPatch_FOS::kItmL2 || a_patchID == CPatch_FOS::kItmL3) { CPatch_FOS::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_ST1::kItmL1 || a_patchID == CPatch_ST1::kItmL2 || a_patchID == CPatch_ST1::kItmL3 || a_patchID == CPatch_ST1::kItmL4) { CPatch_ST1::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_ST2::kItmL1 || a_patchID == CPatch_ST2::kItmL2 || a_patchID == CPatch_ST2::kItmL3 || a_patchID == CPatch_ST2::kItmL4) { CPatch_ST2::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
		if (a_patchID == CPatch_ST3::kItmL1 || a_patchID == CPatch_ST3::kItmL2 || a_patchID == CPatch_ST3::kItmL3 || a_patchID == CPatch_ST3::kItmL4) { CPatch_ST3::CHandler::SetOptionCompleted(a_patchID, a_name); return; }
	}
}
