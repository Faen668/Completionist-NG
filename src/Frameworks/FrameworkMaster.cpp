#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/PatchListener.hpp"

namespace CFramework_Master 
{
	using namespace CVariables;
	using namespace ArrayHolder;
	using namespace Serialization;
	using namespace CExternalPatchHandler;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register() 
	{
		auto& trampoline = SKSE::GetTrampoline();
		_OnMapMarkerDiscovered = trampoline.write_call<5>(RELOCATION_ID(39663, 40750).address() + REL::Relocate(0x1CC, 0x1EC), OnMapMarkerDiscovered);
		_OnMapMarkerAdded = trampoline.write_call<5>(RELOCATION_ID(55617, 56146).address() + REL::Relocate(0x9D, 0x9D), OnMapMarkerAdded);

		SKSE::GetPapyrusInterface()->Register(FrameworkAPI::RegisterFunctions);

		SetSerializableInfo(FoundItemData);
		SetSerializableInfo(FoundItemData_NoShow);
		SetSerializableInfo(CQuestKeys_Natural);
		SetSerializableInfo(CQuestKeys_Manual);
		SetSerializableInfo(CQuestKeys_Stages);
		SetSerializableInfo(LoggingData);
		SetSerializableInfo(RadiantCountData);
		SetSerializableInfo(ExcludedCellScannerRefs);
		SetSerializableInfo(ExcludedMerchantContainers);

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
		CPatch_REQ::				CHandler::InstallFramework();
		CPatch_INN::				CHandler::InstallFramework();
		CPatch_JAY::				CHandler::InstallFramework();
		CPatch_RAR::				CHandler::InstallFramework();
		CPatch_Cloaks::				CHandler::InstallFramework();
		CPatch_ICOW::				CHandler::InstallFramework();
		CPatch_TTB::				CHandler::InstallFramework();
		CPatch_BGC::				CHandler::InstallFramework();
		CPatch_AOS::				CHandler::InstallFramework();

		//SpellTomes
		CPatch_SpellTomes::			CHandler::InstallFramework();

		//Custom Patches
		for (auto& cls : CExternalPatchHandler::Get())
		{ 
			cls->InstallFramework();
		};

		//Register Arrays
		ArrayHolder::RegisterArrays();
	}
	
	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("SetFrameworkQuest", "Completionist_Native", CVariables::VariablesAPI::SetFrameworkQuest);

		a_vm->RegisterFunction("LogWithPlugin",					"Completionist_Native", LogWithPlugin);
		a_vm->RegisterFunction("GetFormArrayByID",				"Completionist_Native", GetFormArrayByID);
		a_vm->RegisterFunction("GetNameArrayByID",				"Completionist_Native", GetNameArrayByID);
		a_vm->RegisterFunction("GetBoolArrayByID",				"Completionist_Native", GetBoolArrayByID);
		a_vm->RegisterFunction("GetTextArrayByID",				"Completionist_Native", GetTextArrayByID);

		a_vm->RegisterFunction("GetEntries_TotalByID",			"Completionist_Native", GetEntries_TotalByID);
		a_vm->RegisterFunction("GetEntries_FoundByID",			"Completionist_Native", GetEntries_FoundByID);

		a_vm->RegisterFunction("IsOptionCompleted",				"Completionist_Native", IsOptionCompleted);
		a_vm->RegisterFunction("SetOptionCompleted",			"Completionist_Native", SetOptionCompleted);

		a_vm->RegisterFunction("GetVersion",					"Completionist_Native", GetVersion);
		a_vm->RegisterFunction("GetHexValue",					"Completionist_Native", GetHexValue);
		a_vm->RegisterFunction("SendNotification",				"Completionist_Native", SendNotificationExt);

		a_vm->RegisterFunction("UpdateVariables",				"Completionist_Native", UpdateVariables);
		a_vm->RegisterFunction("LoadInjectedForms",				"Completionist_Native", LoadInjectedForms);

		a_vm->RegisterFunction("GetLoggingDates",				"Completionist_Native", GetLoggingDates);
		a_vm->RegisterFunction("GetLoggedEventsForDate",		"Completionist_Native", GetLoggedEventsForDate);

		a_vm->RegisterFunction("Framework_UpdatePetOwnership",	"Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("Framework_UpdateShouts",		"Completionist_Native", CFramework_Shouts::CHandler::UpdateFoundFormsExt);
		a_vm->RegisterFunction("ActivateShrineByID",			"Completionist_Native", CFramework_Blessings::CHandler::ActivateShrineFromPapyrus);

		a_vm->RegisterFunction("CheckForReferences",			"Completionist_Native", CellScanner::CHandler::CheckForReferences);
		a_vm->RegisterFunction("GetValidItemReferences",		"Completionist_Native", CellScanner::CHandler::GetValidItemReferences);
		a_vm->RegisterFunction("GetValidItemReferenceNames",	"Completionist_Native", CellScanner::CHandler::GetValidItemReferenceNames);
		a_vm->RegisterFunction("GetValidItemReferenceTypes",	"Completionist_Native", CellScanner::CHandler::GetValidItemReferenceTypes);
		a_vm->RegisterFunction("GetQuestMarkerReferenceFormID", "Completionist_Native", CellScanner::CHandler::GetQuestMarkerReferenceFormID);
		a_vm->RegisterFunction("GetQuestMarkerReferenceOwner",	"Completionist_Native", CellScanner::CHandler::GetQuestMarkerReferenceOwner);
		a_vm->RegisterFunction("GetQuestMarkerReferenceIndex",	"Completionist_Native", CellScanner::CHandler::GetQuestMarkerReferenceIndex);
		a_vm->RegisterFunction("GetReferenceFormIDs",			"Completionist_Native", CellScanner::CHandler::GetReferenceFormIDs);
		a_vm->RegisterFunction("GetReferenceNames",				"Completionist_Native", CellScanner::CHandler::GetReferenceNames);
		a_vm->RegisterFunction("GetObjectReferences",			"Completionist_Native", CellScanner::CHandler::GetObjectReferences);
		a_vm->RegisterFunction("isCellExcluded",				"Completionist_Native", CellScanner::CHandler::isCellExcluded);
		
		a_vm->RegisterFunction("ExcludeReference",				"Completionist_Native", CellScanner::CHandler::ExcludeReference);
		a_vm->RegisterFunction("RemoveExcludedReference",		"Completionist_Native", CellScanner::CHandler::RemoveExcludedReference);

		a_vm->RegisterFunction("MapMarkerIsCleared",			"Completionist_Native", CFramework_MapMa::CHandler::MarkerIsCleared);

		a_vm->RegisterFunction("SetFishCaught",					"Completionist_Native", CPatch_FSH::CHandler::ProcessCaughtFishFromPapyrus);
		a_vm->RegisterFunction("IsItemKnownExternal",			"Completionist_Native", IsItemKnownExternal);
		a_vm->RegisterFunction("IsInActualMenuMode",			"Completionist_Native", IsInActualMenuMode);
		
		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggingDates(RE::StaticFunctionTag*)
	{
		std::vector<std::string> list = LoggingData.GetAllLoggedDates();
		std::sort(list.begin(), list.end(), compare_dates);
		std::reverse(list.begin(), list.end());
		return list;
	}

	//---------------------------------------------------
	//-- Framework Functions (IsInActualMenuMode ) ------
	//---------------------------------------------------

	bool FrameworkAPI::IsInActualMenuMode(RE::StaticFunctionTag*)
	{
		auto map = RE::ControlMap::GetSingleton();
		return !map->IsMovementControlsEnabled();
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	bool FrameworkAPI::compare_dates(std::string a, std::string b)
	{
		// Comparing the years
		std::string yr1 = a.substr(6, 4);
		std::string yr2 = b.substr(6, 4);
		if (yr1.compare(yr2) != 0)
		{
			if (yr1.compare(yr2) < 0)
			{
				return true;
			}
			return false;
		}

		// Comparing the months
		std::string mo1 = a.substr(3, 2);
		std::string mo2 = b.substr(3, 2);
		if (mo1.compare(mo2) != 0)
		{
			if (mo1.compare(mo2) < 0)
			{
				return true;
			}
			return false;
		}

		// Comparing the days
		std::string da1 = a.substr(0, 2);
		std::string da2 = b.substr(0, 2);
		return da1.compare(da2) < 0;
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggedEventsForDate(RE::StaticFunctionTag*, std::string a_date, bool b_prefix, bool b_colour, std::string_view qc, std::string_view ic, std::string_view bc, std::string_view sc)
	{
		auto list = LoggingData.GetAllLoggedEvents(a_date, b_prefix, b_colour, qc, ic, bc, sc);
		while (list.size() > 126) { list.erase(list.begin()); }
		std::reverse(list.begin(), list.end());
		return list;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Quest Search ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::SearchAndReportPage(std::string s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t i_searchType)
	{
		std::vector<std::string> list{};
		auto result = 1;
		auto process = false;

		//INFO("Running Misc Search For {} with a type of {}", s_term, i_searchType);

		for (auto& [form, name, mcmPage, Category] : CFramework_Master::CItemsDataVec)
		{
			if (list.size() >= i_maxResults)
				break;

			switch (i_searchType)
			{
			case 0: { process = DKUtil::string::icontains(name, s_term); break; }
			case 1: { process = name.starts_with(s_term); break; }
			case 2: { process = DKUtil::string::iequals(name, s_term); break; }

			default:
				break;
			}

			if (process) 
			{
				if (b_ignoreCompleted && (FoundItemData.HasForm(form) || FoundItemData_NoShow.HasForm(form))) {
					continue;
				}

				list.push_back("$MiscResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + mcmPage + "}{" + GetLocalisedCategory(Category) + "}{" + name + "}");
				list.push_back(mcmPage);
				list.push_back(name);
				list.push_back("Misc");
				result++;
			}
		}

		return list;
	};

	std::string FrameworkAPI::GetLocalisedCategory(int32_t ID)
	{
		switch (static_cast<EntryCategory>(ID))
		{
		case CFramework_Master::kNone: return ""; break;
		case CFramework_Master::kItem: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Item"); break;
		case CFramework_Master::kBook: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Book"); break;
		case CFramework_Master::kMapM: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_MapP"); break;
		case CFramework_Master::kShou: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Shou"); break;
		case CFramework_Master::kEnch: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Ench"); break;
		case CFramework_Master::kHome: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Home"); break;
		case CFramework_Master::kPets: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Pets"); break;
		case CFramework_Master::kClaw: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Claw"); break;
		case CFramework_Master::kMask: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Mask"); break;
		case CFramework_Master::kFish: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Fish"); break;
		case CFramework_Master::kShrine: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Shrine"); break;
		case CFramework_Master::kStones: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Stones"); break;
		case CFramework_Master::kBarenziah: return CLocalisation::LocalisationAPI::GetLocStringByKey("Category_Barenziah"); break;
		default:
			break;
		}

		return "";
	}

	std::int32_t FrameworkAPI::GetBookCategoryType(RE::TESForm* a_form)
	{
		if (!a_form) { return 0; }

		auto* book = static_cast<RE::TESObjectBOOK*>(a_form);
		if (book) {
			if (book && (book->GetSpell() || book->TeachesSkill())) {
				return 0;
			}

			return 2;
		}

		return 0;
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

	void FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::logType kType, std::string a_log)
	{
		LoggingData.AddLoggedEvent(kType, a_log);
	}

	Serialization::CompletionistLog::logType FrameworkAPI::GetBookLogType(RE::TESForm* a_form)
	{
		if (auto* book = static_cast<RE::TESObjectBOOK*>(a_form); book && book->GetSpell()) {
			return Serialization::CompletionistLog::logType::kTome;
		}

		return Serialization::CompletionistLog::logType::kBook;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::UpdateVariables(RE::StaticFunctionTag*) 
	{
		VariablesAPI::Update();
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

	bool FrameworkAPI::CCLocationsInstalled()		{ return bool(CFramework_MapMa_CC::Data.data.size()); }
	bool FrameworkAPI::CCBooksInstalled()			{ return bool(CFramework_Books_CC::Data.data.size());; }
	bool FrameworkAPI::CCItemsInstalled()			{ return bool(CFramework_Uniques::ItemDataCCA.data.size()) || bool(CFramework_Uniques::ItemDataCCI.data.size()) || bool(CFramework_Uniques::ItemDataCCW.data.size()); }
	bool FrameworkAPI::ShouldDisplayMiscHeader()	{ return bool(PatchesInstalled); }
	bool FrameworkAPI::ShouldDisplayTomeHeader()	{ return bool(TomesInstalled); }

	//---------------------------------------------------
	//-- Framework Events ( Load & Update Frameworks ) --
	//---------------------------------------------------

	void FrameworkAPI::Update() 
	{
		AddUpdateFoundForms_Invoke();

		for (auto& cls : CExternalPatchHandler::CustomItemsPatches)
		{
			cls->UpdateFoundForms();
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Added ) -----------
	//---------------------------------------------------

	void FrameworkAPI::OnMapMarkerAdded(RE::TESFullName* a_form)
	{
		_OnMapMarkerAdded(a_form);
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());

		for (auto& cls : CExternalPatchHandler::CustomItemsPatches)
		{
			cls->ProcessHookedMarker(a_form->GetFullName());
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Discovered ) ------
	//---------------------------------------------------

	const char* FrameworkAPI::OnMapMarkerDiscovered(RE::TESFullName* a_form)
	{
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());
		for (auto& cls : CExternalPatchHandler::CustomItemsPatches)
		{
			cls->ProcessHookedMarker(a_form->GetFullName());
		};

		return _OnMapMarkerDiscovered(a_form);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleTotalSet(a_ID);
		//INFO("Returning total count for framework {} with a value of: {}", std::to_underlying(a_ID), value);
		return value;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleFoundSet(a_ID);
		//INFO("Returning found count for framework {} with a value of: {}", std::to_underlying(a_ID), value);
		return value;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleFormSet(a_ID);
		//INFO("Returning form array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleNameSet(a_ID);
		//INFO("Returning name array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleTextSet(a_ID);
		//INFO("Returning text array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleBoolSet(a_ID);
		//INFO("Returning bool array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t FrameworkAPI::IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {
		if (auto t_pos = std::ranges::find(HandleFormSet(a_ID), a_form); t_pos != HandleFormSet(a_ID).end()) {
			return std::int32_t(HandleBoolSet(a_ID)[std::distance(HandleFormSet(a_ID).begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

		if (auto t_pos = std::ranges::find(HandleFormSet(a_ID), a_form); t_pos != HandleFormSet(a_ID).end()) {
			auto b_pos = std::distance(HandleFormSet(a_ID).begin(), t_pos);

			if (HandleBoolSet(a_ID).at(b_pos)) {
				HandleBoolSet(a_ID).at(b_pos) = false;

				if (HandleNoShow(a_ID)) { FoundItemData_NoShow.RemoveForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }
				else { FoundItemData.RemoveForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(a_ID).GetAllVariations()) {
					if (HandleDataSet(a_ID).GetBase(var) == HandleFormSet(a_ID).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(a_ID)) { FoundItemData_NoShow.RemoveForm(var); }
						else { FoundItemData.RemoveForm(var); }

					}
				}
			}
			else {
				HandleBoolSet(a_ID).at(b_pos) = true;

				if (HandleNoShow(a_ID)) { FoundItemData_NoShow.AddForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }
				else { FoundItemData.AddForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(a_ID).GetAllVariations()) {
					if (HandleDataSet(a_ID).GetBase(var) == HandleFormSet(a_ID).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(a_ID)) { FoundItemData_NoShow.AddForm(var); }
						else { FoundItemData.AddForm(var); }

					}
				}
			}

			HandleTotalSet(a_ID) = HandleFormSet(a_ID).size();
			HandleFoundSet(a_ID) = std::ranges::count(HandleBoolSet(a_ID), true);
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

	bool FrameworkAPI::IsItemKnownExternal(RE::StaticFunctionTag*, RE::TESForm* a_form) 
	{
		return a_form && (FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID()));
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