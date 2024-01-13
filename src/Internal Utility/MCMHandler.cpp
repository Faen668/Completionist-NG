#include "MCMHandler.hpp"
#include "ScriptObject.hpp"
#include "DKUtil/Utility.hpp"
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_01.hpp";
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_02.hpp";
#include "Frameworks/Quests/Creation Club/CQuests_CreationClub_03.hpp";

namespace CHCMHandler 
{
	void MCMAPI::Register() 
	{
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);

		auto ui = RE::UI::GetSingleton();
		ui->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(MCMAPI::GetSingleton()));
		BuildMCMPages(nullptr);
	};

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	auto MCMAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("GetMCMPages", "Completionist_Native", GetMCMPages);
		a_vm->RegisterFunction("BuildMCMPages", "Completionist_Native", BuildMCMPages);
		a_vm->RegisterFunction("GetSkyUIMCMPositionalIndex", "Completionist_Native", GetSkyUIMCMPositionalIndex);
		a_vm->RegisterFunction("GetMCMPageIdentifierFromName", "Completionist_Native", GetMCMPageIdentifierFromName);
		a_vm->RegisterFunction("GetValidMainPatchPageID", "Completionist_Native", GetValidMainPatchPageID);
		a_vm->RegisterFunction("GetValidMiscPatchPageIDForItems", "Completionist_Native", GetValidMiscPatchPageIDForItems);
		a_vm->RegisterFunction("GetValidMiscPatchPageIDForBooks", "Completionist_Native", GetValidMiscPatchPageIDForBooks);
		a_vm->RegisterFunction("GetValidMiscPatchPageIDForMapMa", "Completionist_Native", GetValidMiscPatchPageIDForMapMa);
		a_vm->RegisterFunction("IsSettingsPage", "Completionist_Native", IsSettingsPage);
		a_vm->RegisterFunction("GetHeaderRequired", "Completionist_Native", GetHeaderRequired);
		return true;
	}

	bool MCMAPI::IsSettingsPage(RE::StaticFunctionTag*, std::string a_page) {
		return a_page == "" || a_page == " " || DKUtil::string::icontains(a_page, "MCMPageSettings") || DKUtil::string::icontains(a_page, "Header");
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::int32_t MCMAPI::GetSkyUIMCMPositionalIndex(RE::StaticFunctionTag*, std::string MCMPage)
	{
		auto ConfigBase = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00802, "SkyUI_SE.esp")), "SKI_ConfigManager");
		if (!ConfigBase) { return -1; }

		auto namearrray = ScriptObject::GetArray(ConfigBase, "_modNames");
		if (!namearrray) { return -1; }

		std::int32_t Idx{};
		for (auto& name : *namearrray)
		{
			if (DKUtil::string::iequals(name.GetString(), MCMPage))
			{
				return Idx;
			}
			Idx++; 
		}

		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::int32_t MCMAPI::GetMCMPageIdentifierFromName(RE::StaticFunctionTag*, std::string MCMPage)
	{
		for (auto& [page, mod, id] : MainMCMPagesDefs) {

			if (strcmp(page, MCMPage.c_str()) == 0) {
				return id;
			}
		}

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), MCMPage.c_str()) == 0) {
				return id;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	EventResult	MCMAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {
		
		if (a_event->opening && a_event->menuName == RE::JournalMenu::MENU_NAME) { 
			BuildMCMPages(nullptr);
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::AddMainPatchedPageDefinitions(std::vector<std::tuple<std::string, std::string, uint32_t>> defs) {

		MainPatchesMCMPagesDefs = defs;

		for (auto& [page, mod, id] : defs) {
			INFO("Quest Page Definition Added: {} - {} - {}", page, mod, id);
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::AddMiscPatchedPageDefinitions(std::vector<std::tuple<std::string, std::string, uint32_t, uint32_t, uint32_t, bool, bool, bool>> defs) {

		MiscPatchesMCMPagesDefs = defs;

		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : defs) {
			INFO("Misc Page Definition Added: {} - {} - {} - {} - {}", page, mod, ItemsID, BooksID, MapMaID);
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetValidMainPatchPageID(RE::StaticFunctionTag*, std::string a_page) {

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), a_page.c_str()) == 0) {
				return id;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetValidMiscPatchPageIDForItems(RE::StaticFunctionTag*, std::string a_page) {

		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : MiscPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), a_page.c_str()) == 0) {
				return ItemsID;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetValidMiscPatchPageIDForBooks(RE::StaticFunctionTag*, std::string a_page) {

		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : MiscPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), a_page.c_str()) == 0) {
				return BooksID;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	int32_t MCMAPI::GetValidMiscPatchPageIDForMapMa(RE::StaticFunctionTag*, std::string a_page) {

		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : MiscPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), a_page.c_str()) == 0) {
				return MapMaID;
			}
		}
		return -1;
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	void MCMAPI::BuildMCMPages(RE::StaticFunctionTag*) {

		using namespace CFramework_Master;

		MainMCMPages.clear();
		MiscMCMPages.clear();

		auto cc1 = CQFramework_CC1::QuestsInstalled > 0;
		auto cc2 = CQFramework_CC2::QuestsInstalled > 0;
		auto cc3 = CQFramework_CC3::QuestsInstalled > 0;
		
		auto ccB = CFramework_Master::FrameworkAPI::CCBooksInstalled();
		auto ccI = CFramework_Master::FrameworkAPI::CCItemsInstalled();
		auto ccL = CFramework_Master::FrameworkAPI::CCLocationsInstalled();

		auto ms0 = CFramework_Master::PatchesInstalled > 0;
		auto ms1 = CFramework_Master::FrameworkAPI::ShouldDisplayTomeHeader();
		auto ms2 = CVariables::V_FishingSpotMarkers;

		for (auto& [page, mod, id] : MainMCMPagesDefs) {

			if ( (id == MS_Header && !ms0) || (id == CC_Header && (!cc1 && !cc2 && !cc3)) || (id == 4 && !cc1) || (id == 5 && !cc2) || (id == 6 && !cc3) ) {
				continue;
			}

			if (!DKUtil::string::is_empty(mod) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				continue;
			}

			MainMCMPages.push_back(page);
		}

		if (MainPatchesMCMPagesDefs.size() > 0)
		{
			MainMCMPages.push_back(" ");
			MainMCMPages.push_back("$HeaderU");
		}

		for (auto& [page, mod, id] : MainPatchesMCMPagesDefs) {

			if (!DKUtil::string::is_empty(mod.c_str()) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				INFO("ERROR ON PAGE DEFINITIONS");
				continue;
			}

			MainMCMPages.push_back(page);
		}

		for (auto& [page, mod, id] : MiscMCMPagesDefs) {

			if ((id == CB_Header && !ccB) || (id == CL_Header && !ccL) || (id == ST_Header && !ms1) || (id == CI_Header && !ccI) || (id == FL_Header && !ms2) || (id == MS_Header && !ms0) ) {
				continue;
			}

			if (!DKUtil::string::is_empty(mod) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				continue;
			}

			MiscMCMPages.push_back(page);
		}

		if (MiscPatchesMCMPagesDefs.size() > 0)
		{
			MiscMCMPages.push_back(" ");
			MiscMCMPages.push_back("$HeaderU");
		}

		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : MiscPatchesMCMPagesDefs) {

			if (!DKUtil::string::is_empty(mod.c_str()) && !Serialization::CompletionistData::IsModInstalled(mod)) {
				INFO("ERROR ON PAGE DEFINITIONS");
				continue;
			}

			MiscMCMPages.push_back(page);
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	std::vector<std::string> MCMAPI::GetMCMPages(RE::StaticFunctionTag*, std::int32_t menu_identifier) {

		switch (menu_identifier)
		{
		case 0: return MainMCMPages; break;
		case 1: return MiscMCMPages; break;
		default: return std::vector<std::string>{}; break;
		}
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------

	bool MCMAPI::GetHeaderRequired(RE::StaticFunctionTag*, std::string mcmpage, int32_t header)
	{
		for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : MiscPatchesMCMPagesDefs) {

			if (strcmp(page.c_str(), mcmpage.c_str()) == 0) 
			{
				switch (header)
				{
				case 0: return itemsReq; break;
				case 1: return booksReq; break;
				case 2: return mapmaReq; break;

				default:
					return false; break;
				}
			}
		}
		return false;
	}
}