#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CFramework_Master 
{
	using namespace CVariables;
	using namespace ArrayHolder;
	using namespace Serialization;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register() 
	{
		SKSE::GetPapyrusInterface()->Register(FrameworkAPI::RegisterFunctions);

		SetSerializableInfo(FoundItemData);
		SetSerializableInfo(FoundItemData_NoShow);

		SetSerializableInfo(CQuestKeys_Natural);
		SetSerializableInfo(CQuestKeys_Manual);
		SetSerializableInfo(CQuestKeys_Stages);

		//Frameworks
		CFramework_Uniques::		CHandler::InstallFramework();
		CFramework_Others::			CHandler::InstallFramework();
		CFramework_Books::			CHandler::InstallFramework();
		CFramework_MapMa::			CHandler::InstallFramework();
		CFramework_Blessings::		CHandler::InstallFramework();
		CFramework_Enchantments::	CHandler::InstallFramework();
		CFramework_Pets::			CHandler::InstallFramework();
		CFramework_PlayerHomes::	CHandler::InstallFramework();
		CFramework_Shouts::			CHandler::InstallFramework();

		// Patches
		CPatch_AHD::				CHandler::InstallFramework();
		CPatch_BOO::				CHandler::InstallFramework();
		CPatch_CLW::				CHandler::InstallFramework();
		CPatch_FSK::				CHandler::InstallFramework();
		CPatch_FOS::				CHandler::InstallFramework();
		CPatch_GCN::				CHandler::InstallFramework();
		CPatch_OAP::				CHandler::InstallFramework();
		CPatch_HRB::				CHandler::InstallFramework();
		CPatch_3DC::				CHandler::InstallFramework();
		CPatch_MAS::				CHandler::InstallFramework();
		CPatch_MTE::				CHandler::InstallFramework();
		CPatch_AHO::				CHandler::InstallFramework();
		CPatch_ST1::				CHandler::InstallFramework();
		CPatch_ST2::				CHandler::InstallFramework();
		CPatch_ST3::				CHandler::InstallFramework();
		CPatch_TEL::				CHandler::InstallFramework();
		CPatch_THU::				CHandler::InstallFramework();
		CPatch_UND::				CHandler::InstallFramework();
		CPatch_WOL::				CHandler::InstallFramework();
		CPatch_WSN::				CHandler::InstallFramework();
		CPatch_WYR::				CHandler::InstallFramework();
		CPatch_VIG::				CHandler::InstallFramework();
		CPatch_FSH::				CHandler::InstallFramework();
		CPatch_LOD::				CHandler::InstallFramework();
		CPatch_SUD::				CHandler::InstallFramework();
		CPatch_CHM::				CHandler::InstallFramework();

		//SpellTomes
		CPatch_SpellTomes::			CHandler::InstallFramework();

		//Register Arrays
		ArrayHolder::RegisterArrays();
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("LogWithPlugin",					"Completionist_Native", LogWithPlugin);

		a_vm->RegisterFunction("GetFormArrayByID",				"Completionist_Native", GetFormArrayByID);
		a_vm->RegisterFunction("GetNameArrayByID",				"Completionist_Native", GetNameArrayByID);
		a_vm->RegisterFunction("GetBoolArrayByID",				"Completionist_Native", GetBoolArrayByID);
		a_vm->RegisterFunction("GetTextArrayByID",				"Completionist_Native", GetTextArrayByID);

		a_vm->RegisterFunction("GetEntries_TotalByID",			"Completionist_Native", GetEntries_TotalByID);
		a_vm->RegisterFunction("GetEntries_FoundByID",			"Completionist_Native", GetEntries_FoundByID);

		a_vm->RegisterFunction("CCItemsInstalled",				"Completionist_Native", CCItemsInstalled);
		a_vm->RegisterFunction("CCBooksInstalled",				"Completionist_Native", CCBooksInstalled);
		a_vm->RegisterFunction("CCLocationsInstalled",			"Completionist_Native", CCLocationsInstalled);

		a_vm->RegisterFunction("IsOptionCompleted",				"Completionist_Native", IsOptionCompleted);
		a_vm->RegisterFunction("SetOptionCompleted",			"Completionist_Native", SetOptionCompleted);

		a_vm->RegisterFunction("ShouldDisplayMiscHeader",		"Completionist_Native", ShouldDisplayMiscHeader);
		a_vm->RegisterFunction("ShouldDisplayTomeHeader",		"Completionist_Native", ShouldDisplayTomeHeader);

		a_vm->RegisterFunction("GetVersion",					"Completionist_Native", GetVersion);
		a_vm->RegisterFunction("GetHexValue",					"Completionist_Native", GetHexValue);
		a_vm->RegisterFunction("SendNotification",				"Completionist_Native", SendNotificationExt);

		a_vm->RegisterFunction("UpdateVariables",				"Completionist_Native", UpdateCompletion);
		a_vm->RegisterFunction("LoadInjectedForms",				"Completionist_Native", LoadInjectedForms);
		
		a_vm->RegisterFunction("Framework_UpdatePetOwnership",	"Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("Framework_UpdateShouts",		"Completionist_Native", CFramework_Shouts::CHandler::UpdateFoundFormsExt);
		a_vm->RegisterFunction("ActivateShrineByID",			"Completionist_Native", CFramework_Blessings::CHandler::ActivateShrineFromPapyrus);
		a_vm->RegisterFunction("CheckForReferences",			"Completionist_Native", CellScanner::CHandler::CheckForReferences);
		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::LoadInjectedForms(RE::StaticFunctionTag*) {

		CPatch_FSH::CHandler::AddCACOFishingForms();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::LogWithPlugin(RE::StaticFunctionTag*, std::string a_message) {

		INFO("Papyrus Message: {}", a_message);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::UpdateCompletion(RE::StaticFunctionTag*) 
	{
		VariablesAPI::Update();
		CQuestMaster::QuestAPI::UpdateQuestCompletion();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Send Notification ) ------
	//---------------------------------------------------

	void FrameworkAPI::SendNotification(std::string a_msg, std::string a_setting) {
		using namespace ScriptObject;

		auto MCMScript = FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCMScript->GetProperty(a_setting)->GetBool()) { return; }

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, MCMScript->GetProperty("ColourString")->GetString(), a_msg);
		if (!MCMScript->GetProperty("NotificationColourEnabled")->GetBool()) { RE::DebugNotification(a_msg.c_str()); return; }
		RE::DebugNotification(message.c_str());
	}

	//---------------------------------------------------
	//-- Utility Functions ------------------------------
	//---------------------------------------------------

	void FrameworkAPI::SendNotificationExt(RE::StaticFunctionTag*, std::string a_msg, std::string a_colour, bool a_enabled) {

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, a_colour, a_msg);
		if (!a_enabled) { RE::DebugNotification(a_msg.c_str()); return; }
		RE::DebugNotification(message.c_str());
	}

	//---------------------------------------------------
	//-- String Functions -------------------------------
	//---------------------------------------------------

	std::string FrameworkAPI::GetVersion(RE::StaticFunctionTag*) { return std::string(FrameworkAPI::ReplaceStr(Plugin::Version.string(), "-", ".")); }
	std::string FrameworkAPI::GetHexValue(RE::StaticFunctionTag*, uint32_t IntVal) { if (!IntVal) { return ""; } return fmt::format("#{:X}", IntVal); }
	std::string FrameworkAPI::ReplaceStr(std::string const& in, std::string const& from, std::string const& to) { return std::regex_replace(in, std::regex(from), to); }

	//---------------------------------------------------
	//-- Framework Functions ( CC Variable Setter ) -----
	//---------------------------------------------------

	bool FrameworkAPI::CCLocationsInstalled(RE::StaticFunctionTag*)		{ return bool(CFramework_MapMa_CC::Data.data.size()); }
	bool FrameworkAPI::CCBooksInstalled(RE::StaticFunctionTag*)			{ return bool(CFramework_Books_CC::Data.data.size());; }
	bool FrameworkAPI::CCItemsInstalled(RE::StaticFunctionTag*)			{ return bool(CFramework_Uniques_CCA::Data.data.size()) || bool(CFramework_Uniques_CCI::Data.data.size()) || bool(CFramework_Uniques_CCW::Data.data.size()); }
	bool FrameworkAPI::ShouldDisplayMiscHeader(RE::StaticFunctionTag*)	{ return bool(PatchesInstalled); }
	bool FrameworkAPI::ShouldDisplayTomeHeader(RE::StaticFunctionTag*)	{ return bool(TomesInstalled); }

	//---------------------------------------------------
	//-- Framework Functions ( Load Frameworks ) --------
	//---------------------------------------------------

	void FrameworkAPI::Update() {

		//Frameworks
		CFramework_Uniques::		CHandler::UpdateFoundForms();
		CFramework_Others::			CHandler::UpdateFoundForms();
		CFramework_Books::			CHandler::UpdateFoundForms();
		CFramework_MapMa::			CHandler::UpdateFoundForms();
		CFramework_Blessings::		CHandler::UpdateFoundForms();
		CFramework_Enchantments::	CHandler::UpdateFoundForms();
		CFramework_Pets::			CHandler::UpdateFoundForms();
		CFramework_PlayerHomes::	CHandler::UpdateFoundForms();
		CFramework_Shouts::			CHandler::UpdateFoundForms();

		// Patches
		CPatch_AHD::				CHandler::UpdateFoundForms();
		CPatch_BOO::				CHandler::UpdateFoundForms();
		CPatch_CLW::				CHandler::UpdateFoundForms();
		CPatch_FSK::				CHandler::UpdateFoundForms();
		CPatch_FOS::				CHandler::UpdateFoundForms();
		CPatch_GCN::				CHandler::UpdateFoundForms();
		CPatch_OAP::				CHandler::UpdateFoundForms();
		CPatch_HRB::				CHandler::UpdateFoundForms();
		CPatch_3DC::				CHandler::UpdateFoundForms();
		CPatch_MAS::				CHandler::UpdateFoundForms();
		CPatch_MTE::				CHandler::UpdateFoundForms();
		CPatch_AHO::				CHandler::UpdateFoundForms();
		CPatch_ST1::				CHandler::UpdateFoundForms();
		CPatch_ST2::				CHandler::UpdateFoundForms();
		CPatch_ST3::				CHandler::UpdateFoundForms();
		CPatch_TEL::				CHandler::UpdateFoundForms();
		CPatch_THU::				CHandler::UpdateFoundForms();
		CPatch_UND::				CHandler::UpdateFoundForms();
		CPatch_WOL::				CHandler::UpdateFoundForms();
		CPatch_WSN::				CHandler::UpdateFoundForms();
		CPatch_WYR::				CHandler::UpdateFoundForms();
		CPatch_VIG::				CHandler::UpdateFoundForms();
		CPatch_FSH::				CHandler::UpdateFoundForms();
		CPatch_SUD::				CHandler::UpdateFoundForms();
		CPatch_CHM::				CHandler::UpdateFoundForms();

		//SpellTomes
		CPatch_SpellTomes::			CHandler::UpdateFoundForms();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleTotalSet(FrameworkID(a_ID));
		INFO("Returning total count for framework {} with a value of: {}", std::to_underlying(FrameworkID(a_ID)), value);
		return value;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleFoundSet(FrameworkID(a_ID));
		INFO("Returning found count for framework {} with a value of: {}", std::to_underlying(FrameworkID(a_ID)), value);
		return value;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleFormSet(FrameworkID(a_ID));
		INFO("Returning form array for framework {} with a size of: {}", std::to_underlying(FrameworkID(a_ID)), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleNameSet(FrameworkID(a_ID));
		INFO("Returning name array for framework {} with a size of: {}", std::to_underlying(FrameworkID(a_ID)), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleTextSet(FrameworkID(a_ID));
		INFO("Returning text array for framework {} with a size of: {}", std::to_underlying(FrameworkID(a_ID)), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleBoolSet(FrameworkID(a_ID));
		INFO("Returning bool array for framework {} with a size of: {}", std::to_underlying(FrameworkID(a_ID)), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t FrameworkAPI::IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

		if (auto t_pos = std::ranges::find(HandleFormSet(FrameworkID(a_ID)), a_form); t_pos != HandleFormSet(FrameworkID(a_ID)).end()) {
			return std::int32_t(HandleBoolSet(FrameworkID(a_ID))[std::distance(HandleFormSet(FrameworkID(a_ID)).begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

		if (auto t_pos = std::ranges::find(HandleFormSet(FrameworkID(a_ID)), a_form); t_pos != HandleFormSet(FrameworkID(a_ID)).end()) {
			auto b_pos = std::distance(HandleFormSet(FrameworkID(a_ID)).begin(), t_pos);

			if (HandleBoolSet(FrameworkID(a_ID)).at(b_pos)) {
				HandleBoolSet(FrameworkID(a_ID)).at(b_pos) = false;

				if (HandleNoShow(FrameworkID(a_ID))) { FoundItemData_NoShow.RemoveForm(HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()); }
				else { FoundItemData.RemoveForm(HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(FrameworkID(a_ID)).GetAllVariations()) {
					if (HandleDataSet(FrameworkID(a_ID)).GetBase(var) == HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(FrameworkID(a_ID))) { FoundItemData_NoShow.RemoveForm(var); }
						else { FoundItemData.RemoveForm(var); }

					}
				}
			}
			else {
				HandleBoolSet(FrameworkID(a_ID)).at(b_pos) = true;

				if (HandleNoShow(FrameworkID(a_ID))) { FoundItemData_NoShow.AddForm(HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()); }
				else { FoundItemData.AddForm(HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(FrameworkID(a_ID)).GetAllVariations()) {
					if (HandleDataSet(FrameworkID(a_ID)).GetBase(var) == HandleFormSet(FrameworkID(a_ID)).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(FrameworkID(a_ID))) { FoundItemData_NoShow.AddForm(var); }
						else { FoundItemData.AddForm(var); }

					}
				}
			}

			HandleTotalSet(FrameworkID(a_ID)) = HandleFormSet(FrameworkID(a_ID)).size();
			HandleFoundSet(FrameworkID(a_ID)) = std::ranges::count(HandleBoolSet(FrameworkID(a_ID)), true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Has Book Spell / Read ) --
	//---------------------------------------------------

	bool FrameworkAPI::IsBookKnown(RE::TESForm* a_form) {

		auto* itm = static_cast<RE::TESObjectBOOK*>(a_form);
		auto* pcr = RE::PlayerCharacter::GetSingleton();
		if (!itm || !pcr) { return false; }

		if (itm->IsRead() || (itm->GetSpell() ? pcr->HasSpell(itm->GetSpell()) : false)) {
			FoundItemData.AddForm(itm);
		}

		return FoundItemData.HasForm(itm);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Item Known ) ----------
	//---------------------------------------------------

	bool FrameworkAPI::IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data = nullptr) {

		if (!a_form) { return false; }

		auto base = a_data->GetBase(a_form->GetFormID()) ? a_data->GetBase(a_form->GetFormID()) : a_form->GetFormID();
		auto* pcr = RE::PlayerCharacter::GetSingleton();

		if (pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(base)) > 0) {
			FoundItemData.AddForm(base);
			for (auto var : a_data->GetAllVariations()) {
				if (a_data->GetBase(var) == base) {
					FoundItemData.AddForm(var);
				}
			}
			return FoundItemData.HasForm(a_form);
		}

		for (auto var : a_data->GetAllVariations()) {
			if (a_data->GetBase(var) == base && pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(var)) > 0) {

				FoundItemData.AddForm(base);
				for (auto variation : a_data->GetAllVariations()) {
					if (a_data->GetBase(variation) == base) {
						FoundItemData.AddForm(variation);
					}
				}
				return FoundItemData.HasForm(a_form);
			}
		}

		return FoundItemData.HasForm(a_form);
	}
}