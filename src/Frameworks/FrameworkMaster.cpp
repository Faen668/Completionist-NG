#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Frameworks/Quests/Radiant & Favors/Radiant Quests Manager.hpp"

namespace CFramework_Master {
	using namespace ArrayHolder;
	using namespace Serialization;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register() {

		auto t1 = std::chrono::steady_clock::now();

		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(FrameworkAPI::RegisterFunctions);

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

		//Quests (Main Story)
		CQFramework_SK::CHandler::InstallFramework();
		CQFramework_CW::CHandler::InstallFramework();
		CQFramework_DG::CHandler::InstallFramework();
		CQFramework_DB::CHandler::InstallFramework();

		//Quests (Creation Club)
		CQFramework_CC1::CHandler::InstallFramework();
		CQFramework_CC2::CHandler::InstallFramework();
		CQFramework_CC3::CHandler::InstallFramework();

		//Quests (Towns & Cities)
		CQFramework_Dawnstar::		CHandler::InstallFramework();
		CQFramework_Falkreath::		CHandler::InstallFramework();
		CQFramework_Markarth::		CHandler::InstallFramework();
		CQFramework_Morthal::		CHandler::InstallFramework();
		CQFramework_Riften::		CHandler::InstallFramework();
		CQFramework_Solitude::		CHandler::InstallFramework();
		CQFramework_Whiterun::		CHandler::InstallFramework();
		CQFramework_Windhelm::		CHandler::InstallFramework();
		CQFramework_Winterhold::	CHandler::InstallFramework();
		CQFramework_RavenRock::		CHandler::InstallFramework();
		CQFramework_SkaalVillage::	CHandler::InstallFramework();
		CQFramework_TelMithryn::	CHandler::InstallFramework();
		CQFramework_Thirsk::		CHandler::InstallFramework();
		CQFramework_SmallTowns::	CHandler::InstallFramework();

		//Quests (Guilds & Factions)
		CQFramework_CollegeOfWinterhold::	CHandler::InstallFramework();
		CQFramework_Companions::			CHandler::InstallFramework();
		CQFramework_DarkBrotherhood::		CHandler::InstallFramework();
		CQFramework_Dawnguard::				CHandler::InstallFramework();
		CQFramework_ThievesGuild::			CHandler::InstallFramework();
		CQFramework_Vampires::				CHandler::InstallFramework();

		//Quests (Guilds & Factions)
		CQFramework_Dungeons::	CHandler::InstallFramework();
		CQFramework_Misc_SK::	CHandler::InstallFramework();
		CQFramework_Misc_DG::	CHandler::InstallFramework();
		CQFramework_Misc_DB::	CHandler::InstallFramework();

		//Quests (Radiant Handler)
		Quest_Manager::Install();

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
		CPatch_LOD::CHandler::InstallFramework();

		//SpellTomes
		CPatch_SpellTomes::CHandler::InstallFramework();

		//Register Arrays
		ArrayHolder::RegisterArrays();

		auto t2 = std::chrono::steady_clock::now();
		auto elapsedMS = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();
		INFO("Finished installing Completionist in - {} Milliseconds", elapsedMS);
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("GetFormArrayByID",				"Completionist_Native", GetFormArrayByID);
		a_vm->RegisterFunction("GetNameArrayByID",				"Completionist_Native", GetNameArrayByID);
		a_vm->RegisterFunction("GetBoolArrayByID",				"Completionist_Native", GetBoolArrayByID);
		a_vm->RegisterFunction("GetTextArrayByID",				"Completionist_Native", GetTextArrayByID);

		a_vm->RegisterFunction("qGetNameArrayByID",				"Completionist_Native", qGetNameArrayByID);
		a_vm->RegisterFunction("qGetTextArrayByID",				"Completionist_Native", qGetTextArrayByID);
		a_vm->RegisterFunction("qGetKeysArrayByID",				"Completionist_Native", qGetKeysArrayByID);
		a_vm->RegisterFunction("qGetIdenArrayByID",				"Completionist_Native", qGetIdenArrayByID);
		a_vm->RegisterFunction("qGetBoolArrayByID",				"Completionist_Native", qGetBoolArrayByID);
		a_vm->RegisterFunction("qGetRadiArrayByID",				"Completionist_Native", qGetRadiArrayByID);

		a_vm->RegisterFunction("GetEntries_TotalByID",			"Completionist_Native", GetEntries_TotalByID);
		a_vm->RegisterFunction("GetEntries_FoundByID",			"Completionist_Native", GetEntries_FoundByID);

		a_vm->RegisterFunction("CCItemsInstalled",				"Completionist_Native", CCItemsInstalled);
		a_vm->RegisterFunction("CCBooksInstalled",				"Completionist_Native", CCBooksInstalled);
		a_vm->RegisterFunction("CCLocationsInstalled",			"Completionist_Native", CCLocationsInstalled);

		a_vm->RegisterFunction("IsOptionCompleted",				"Completionist_Native", IsOptionCompleted);
		a_vm->RegisterFunction("SetOptionCompleted",			"Completionist_Native", SetOptionCompleted);

		a_vm->RegisterFunction("qIsOptionToggled",				"Completionist_Native", qIsOptionToggled); // Returns true if completed manually.
		a_vm->RegisterFunction("qIsOptionCompleted",			"Completionist_Native", qIsOptionCompleted); // Returns true if completed by any natural means.
		a_vm->RegisterFunction("qSetOptionCompleted",			"Completionist_Native", qSetOptionCompleted); // Only used to manually complete (CQuestKeys_Manual)

		a_vm->RegisterFunction("Framework_UpdatePetOwnership",	"Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("Framework_UpdateShouts",		"Completionist_Native", CFramework_Shouts::CHandler::UpdateFoundFormsExt);
		a_vm->RegisterFunction("ShouldDisplayMiscHeader",		"Completionist_Native", ShouldDisplayMiscHeader);
		a_vm->RegisterFunction("ShouldDisplayTomeHeader",		"Completionist_Native", ShouldDisplayTomeHeader);

		a_vm->RegisterFunction("GetVersion",					"Completionist_Native", GetVersion);
		a_vm->RegisterFunction("GetHexValue",					"Completionist_Native", GetHexValue);
		a_vm->RegisterFunction("SendNotification",				"Completionist_Native", SendNotificationExt);

		a_vm->RegisterFunction("UpdateVariables", "Completionist_Native", UpdateCompletion);

		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::UpdateCompletion(RE::StaticFunctionTag*) {
		using namespace CVariables;

		VariablesAPI::Update();

		CQFramework_SK::CHandler::UpdateCompletion();
		CQFramework_DG::CHandler::UpdateCompletion();
		CQFramework_DB::CHandler::UpdateCompletion();
		CQFramework_CW::CHandler::UpdateCompletion();

		CQFramework_Dawnstar::CHandler::UpdateCompletion();
		CQFramework_Falkreath::CHandler::UpdateCompletion();
		CQFramework_Markarth::CHandler::UpdateCompletion();
		CQFramework_Morthal::CHandler::UpdateCompletion();
		CQFramework_RavenRock::CHandler::UpdateCompletion();
		CQFramework_Riften::CHandler::UpdateCompletion();
		CQFramework_SkaalVillage::CHandler::UpdateCompletion();
		CQFramework_SmallTowns::CHandler::UpdateCompletion();
		CQFramework_Solitude::CHandler::UpdateCompletion();
		CQFramework_TelMithryn::CHandler::UpdateCompletion();
		CQFramework_Thirsk::CHandler::UpdateCompletion();
		CQFramework_Whiterun::CHandler::UpdateCompletion();
		CQFramework_Windhelm::CHandler::UpdateCompletion();
		CQFramework_Winterhold::CHandler::UpdateCompletion();

		CQFramework_CollegeOfWinterhold::CHandler::UpdateCompletion();
		CQFramework_Companions::CHandler::UpdateCompletion();
		CQFramework_DarkBrotherhood::CHandler::UpdateCompletion();
		CQFramework_Dawnguard::CHandler::UpdateCompletion();
		CQFramework_ThievesGuild::CHandler::UpdateCompletion();
		CQFramework_Vampires::CHandler::UpdateCompletion();

		CQFramework_Dungeons::CHandler::UpdateCompletion();
		CQFramework_Misc_SK::CHandler::UpdateCompletion();
		CQFramework_Misc_DG::CHandler::UpdateCompletion();
		CQFramework_Misc_DB::CHandler::UpdateCompletion();

		CQFramework_CC1::CHandler::UpdateCompletion();
		CQFramework_CC2::CHandler::UpdateCompletion();
		CQFramework_CC3::CHandler::UpdateCompletion();

		CPatch_BOO::CHandler::UpdateQuestFramework();
		CPatch_CLW::CHandler::UpdateQuestFramework();
		CPatch_FSK::CHandler::UpdateQuestFramework();
		CPatch_GCN::CHandler::UpdateQuestFramework();
		CPatch_HRB::CHandler::UpdateQuestFramework();
		CPatch_3DC::CHandler::UpdateQuestFramework();
		CPatch_LOD::CHandler::UpdateQuestFramework();
		CPatch_MAS::CHandler::UpdateQuestFramework();
		CPatch_MTE::CHandler::UpdateQuestFramework();
		CPatch_AHO::CHandler::UpdateQuestFramework();
		CPatch_TEL::CHandler::UpdateQuestFramework();
		CPatch_UND::CHandler::UpdateQuestFramework();
		CPatch_VIG::CHandler::UpdateQuestFramework();
		CPatch_WOL::CHandler::UpdateQuestFramework();
		CPatch_WYR::CHandler::UpdateQuestFramework();
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

	bool FrameworkAPI::CCLocationsInstalled(RE::StaticFunctionTag*)	{ return bool(CFramework_MapMa_CC::Data.data.size()); }
	bool FrameworkAPI::CCBooksInstalled(RE::StaticFunctionTag*)		{ return bool(CFramework_Books_CC::Data.data.size());; }
	bool FrameworkAPI::CCItemsInstalled(RE::StaticFunctionTag*)		{ return bool(CFramework_Uniques_CCA::Data.data.size()) || bool(CFramework_Uniques_CCI::Data.data.size()) || bool(CFramework_Uniques_CCW::Data.data.size()); }
	bool FrameworkAPI::ShouldDisplayMiscHeader(RE::StaticFunctionTag*)			{ return bool(PatchesInstalled); }
	bool FrameworkAPI::ShouldDisplayTomeHeader(RE::StaticFunctionTag*)			{ return bool(TomesInstalled); }

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

		//SpellTomes
		CPatch_SpellTomes::CHandler::UpdateFoundForms();
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Idens ) -------------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::qGetIdenArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleIdenSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Names ) -------------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::qGetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleNameSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Texts ) -------------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::qGetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleTextSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Unique Keys ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::qGetKeysArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleKeysSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Bools ) -------------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::qGetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleBoolSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Getter - Radis ) -------------
	//---------------------------------------------------

	std::vector<int32_t> FrameworkAPI::qGetRadiArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return qHandleRadiSet(QuestID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleTotalSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleFoundSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleFormSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleNameSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleTextSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		return HandleBoolSet(FrameworkID(a_ID));
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Quest ) ------------------
	//---------------------------------------------------

	RE::TESQuest* FrameworkAPI::GetQuest(std::string a_questID) {

		return static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(a_questID)) ? static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(a_questID)) : nullptr;;
	}

	//---------------------------------------------------
	//-- Quest Functions ( IS Completed ) ---------------
	//---------------------------------------------------

	bool FrameworkAPI::IsCompleted_N(std::string a_key, std::string a_questID) {

		auto* quest = GetQuest(a_questID);
		if (!quest || !quest->data.flags.any(RE::QuestFlag::kCompleted, RE::QuestFlag::kFailed)) { return false; }
	
		CQuestKeys_Natural.AddKey(a_key);
		CQuestKeys_Manual.RemoveKey(a_key);
		return true;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Is Stage Done ) --------------
	//---------------------------------------------------

	bool FrameworkAPI::IsCompleted_S(std::string a_key, std::string a_questID, std::int32_t a_stage) {
		
		if (CQuestKeys_Stages.HasStage(a_key, a_stage)) {
			CQuestKeys_Natural.AddKey(a_key);
			CQuestKeys_Manual.RemoveKey(a_key);
			return true;
		}
		return false;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Complete By Jarl Status )-
	//---------------------------------------------------

	bool FrameworkAPI::IsCompleted_J(std::string a_key, std::string a_imp, std::string a_son) {

		auto JarlScript = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x087E24, "Skyrim.esm")), "FavorJarlsMakeFriendsScript");
		if (!JarlScript) { return false; }

		auto Imp = JarlScript->GetProperty(a_imp);
		auto Son = JarlScript->GetProperty(a_son);
		if (!Imp || !Son) { return false; }

		if (Imp->GetSInt() > 0 || Son->GetSInt() > 0) {
			CQuestKeys_Natural.AddKey(a_key);
			CQuestKeys_Manual.RemoveKey(a_key);
			return true;
			
		}
		return false;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Complete By Global )------
	//---------------------------------------------------

	bool FrameworkAPI::IsCompleted_G(std::string a_key, std::string a_questID, std::string a_globalID, int32_t a_value) {

		const auto* quest = GetQuest(a_questID);
		auto* global = RE::TESForm::LookupByEditorID<RE::TESGlobal>(a_globalID);

		if (!quest || !global) { return false; }

		switch (a_value)
		{
		case RADIANT_COLLEGE_VALUE:
			a_value = CVariables::V_Radiant_CollegeVal;
			break;

		case RADIANT_COMPANIONS_VALUE:
			a_value = CVariables::V_Radiant_CompanionsVal;
			break;

		case RADIANT_BROTHERHOOD_VALUE:
			a_value = CVariables::V_Radiant_DBrotherhoodVal;
			break;

		case RADIANT_DAWNGUARD_VALUE:
			a_value = CVariables::V_Radiant_DawnguardVal;
			break;

		case RADIANT_THIEVESGUILD_VALUE:
			a_value = CVariables::V_Radiant_ThievesGuildVal;
			break;

		case -4:
			a_value = CVariables::V_RadiantCounterVal;
			break;

		case RADIANT_COUNTER_VALUE:
			a_value = CVariables::V_RadiantCounterVal;
			break;

		case RADIANT_BOUNTY_VALUE:
			a_value = CVariables::V_Radiant_BountyVal;
			break;

		case VIGILANT_COUNTER_VALUE:
			a_value = CVariables::V_Radiant_VigilantVal;
			break;

		case LEGACY_COUNTER_VALUE:
			a_value = CVariables::V_Radiant_LegacyVal;
			break;

		default:
			break;
		}

		if (global->value >= a_value) { 
			CQuestKeys_Natural.AddKey(a_key);
			CQuestKeys_Manual.RemoveKey(a_key);
			return true;
		}
		return false;
	}

	//---------------------------------------------------
	//-- Quest Functions ( Get Stage ) ------------------
	//---------------------------------------------------

	bool FrameworkAPI::IsCompleted_P(std::string a_key, std::string a_questID, std::int32_t a_stage) {

		auto* quest = GetQuest(a_questID);
		if (!quest || !quest->currentStage || quest->currentStage <= a_stage) { return false; }

		CQuestKeys_Natural.AddKey(a_key);
		CQuestKeys_Manual.RemoveKey(a_key);
		return true;
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	bool FrameworkAPI::qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key) {
		return CQuestKeys_Manual.HasKey(a_key);
	}

	bool FrameworkAPI::qIsOptionToggledInternal(std::string a_key) {
		return CQuestKeys_Manual.HasKey(a_key);
	}

	//---------------------------------------------------
	//-- Quest Functions ( MCM Getter - Status ) --------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key) {

		if (CQuestKeys_Natural.HasKey(a_key)) { return -2; }

		if (auto t_pos = std::ranges::find(qHandleKeysSet(QuestID(a_ID)), a_key); t_pos != qHandleKeysSet(QuestID(a_ID)).end()) {
			return std::int32_t(qHandleBoolSet(QuestID(a_ID))[std::distance(qHandleKeysSet(QuestID(a_ID)).begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, std::string a_key) {

		if (auto t_pos = std::ranges::find(qHandleKeysSet(QuestID(a_ID)), a_key); t_pos != qHandleKeysSet(QuestID(a_ID)).end()) {
			auto b_pos = std::distance(qHandleKeysSet(QuestID(a_ID)).begin(), t_pos);

			if (qHandleBoolSet(QuestID(a_ID)).at(b_pos)) {
				qHandleBoolSet(QuestID(a_ID)).at(b_pos) = false;
				CQuestKeys_Manual.RemoveKey(a_key);
			}
			else {
				qHandleBoolSet(QuestID(a_ID)).at(b_pos) = true;
				CQuestKeys_Manual.AddKey(a_key);
			}
		}
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
