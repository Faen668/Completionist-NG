#include "Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"

namespace CFramework_Master {
	using namespace ArrayHolder;
	using namespace Serialization;

	int PatchesInstalled;
	inline CompletionistData FoundItemData;
	inline CompletionistData FoundItemData_NoShow;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register() {

		auto t1 = std::chrono::system_clock::now();

		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(FrameworkAPI::RegisterFunctions);

		FoundItemData.SetAsSerializable();
		FoundItemData_NoShow.SetAsSerializable();

		//Frameworks
		CFramework_Uniques::		CHandler::InstallFramework();
		CFramework_Others::			CHandler::InstallFramework();
		CFramework_Books::			CHandler::InstallFramework();
		CFramework_MapMa::			CHandler::InstallFramework();
		CFramework_Blessings::		CHandler::InstallFramework();
		CFramework_Enchantments::	CHandler::InstallFramework();
		CFramework_Pets::			CHandler::InstallFramework();
		CFramework_PlayerHomes::	CHandler::InstallFramework();

		// Patches
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
		CPatch_FSH::CHandler::InstallFramework();

		//Register Arrays
		ArrayHolder::RegisterArrays();

		auto t2 = std::chrono::system_clock::now();
		auto elapsedMS = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();
		INFO("FInished Installing Completionist in - {} Milliseconds", elapsedMS);
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("Framework_GetFormArrayByID",		"Completionist_Native", Framework_GetFormArrayByID);
		a_vm->RegisterFunction("Framework_GetNameArrayByID",		"Completionist_Native", Framework_GetNameArrayByID);
		a_vm->RegisterFunction("Framework_GetBoolArrayByID",		"Completionist_Native", Framework_GetBoolArrayByID);
		a_vm->RegisterFunction("Framework_GetTextArrayByID",		"Completionist_Native", Framework_GetTextArrayByID);

		a_vm->RegisterFunction("Framework_GetEntries_TotalByID",	"Completionist_Native", Framework_GetEntries_TotalByID);
		a_vm->RegisterFunction("Framework_GetEntries_FoundByID",	"Completionist_Native", Framework_GetEntries_FoundByID);

		a_vm->RegisterFunction("Framework_CCItemsInstalled",		"Completionist_Native", Framework_CCItemsInstalled);
		a_vm->RegisterFunction("Framework_CCBooksInstalled",		"Completionist_Native", Framework_CCBooksInstalled);
		a_vm->RegisterFunction("Framework_CCLocationsInstalled",	"Completionist_Native", Framework_CCLocationsInstalled);

		a_vm->RegisterFunction("Framework_IsOptionCompleted",		"Completionist_Native", Framework_IsOptionCompleted);
		a_vm->RegisterFunction("Framework_SetOptionCompleted",		"Completionist_Native", Framework_SetOptionCompleted);

		a_vm->RegisterFunction("Framework_UpdatePetOwnership",		"Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("ShouldDisplayMiscHeader",			"Completionist_Native", ShouldDisplayMiscHeader);

		a_vm->RegisterFunction("GetVersion",						"Completionist_Native", GetVersion);
		a_vm->RegisterFunction("GetHexValue",						"Completionist_Native", GetHexValue);
		a_vm->RegisterFunction("SendNotification",					"Completionist_Native", SendNotificationExt);
		return true;
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

	bool FrameworkAPI::Framework_CCLocationsInstalled(RE::StaticFunctionTag*)	{ return bool(CFramework_MapMa_CC::Data.data.size()); }
	bool FrameworkAPI::Framework_CCBooksInstalled(RE::StaticFunctionTag*)		{ return bool(CFramework_Books_CC::Data.data.size());; }
	bool FrameworkAPI::Framework_CCItemsInstalled(RE::StaticFunctionTag*)		{ return bool(CFramework_Uniques_CCA::Data.data.size()) || bool(CFramework_Uniques_CCI::Data.data.size()) || bool(CFramework_Uniques_CCW::Data.data.size()); }
	bool FrameworkAPI::ShouldDisplayMiscHeader(RE::StaticFunctionTag*)			{ return bool(PatchesInstalled); }

	//---------------------------------------------------
	//-- Framework Functions ( Load Frameworks ) --------
	//---------------------------------------------------

	void FrameworkAPI::Framework_Load() {

		//Frameworks
		CFramework_Uniques::		CHandler::UpdateFoundForms();
		CFramework_Others::			CHandler::UpdateFoundForms();
		CFramework_Books::			CHandler::UpdateFoundForms();
		CFramework_MapMa::			CHandler::UpdateFoundForms();
		CFramework_Blessings::		CHandler::UpdateFoundForms();
		CFramework_Enchantments::	CHandler::UpdateFoundForms();
		CFramework_Pets::			CHandler::UpdateFoundForms();
		CFramework_PlayerHomes::	CHandler::UpdateFoundForms();

		// Patches
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
		CPatch_FSH::CHandler::UpdateFoundForms();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleTotalSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleFoundSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::Framework_GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleFormSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::Framework_GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleNameSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::Framework_GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleTextSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::Framework_GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleBoolSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t FrameworkAPI::Framework_IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

		if (auto t_pos = std::ranges::find(HandleFormSet(FrameworkID(a_ID)), a_form); t_pos != HandleFormSet(FrameworkID(a_ID)).end()) {
			return std::int32_t(HandleBoolSet(FrameworkID(a_ID))[std::distance(HandleFormSet(FrameworkID(a_ID)).begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::Framework_SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

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
}
