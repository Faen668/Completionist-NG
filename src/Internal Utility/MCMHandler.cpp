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
		return true;
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

	std::int32_t MCMAPI::GetMCMPageIdentifierFromName(RE::StaticFunctionTag*, std::string MCMPage)
	{
		for (auto& [page, mod, id] : MainMCMPagesDefs) {

			if (strcmp(page, MCMPage.c_str()) == 0) {
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

	void MCMAPI::BuildMCMPages(RE::StaticFunctionTag*) {

		using namespace CFramework_Master;

		MainMCMPages.clear();
		MiscMCMPages.clear();

		auto cc1 = CQFramework_CC1::NameArray.size() > 0;
		auto cc2 = CQFramework_CC2::NameArray.size() > 0;
		auto cc3 = CQFramework_CC3::NameArray.size() > 0;
		
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

		for (auto& [page, mod, id] : MiscMCMPagesDefs) {

			if ((id == CB_Header && !ccB) || (id == CL_Header && !ccL) || (id == ST_Header && !ms1) || (id == CI_Header && !ccI) || (id == FL_Header && !ms2) || (id == MS_Header && !ms0) ) {
				continue;
			}

			if (!DKUtil::string::is_empty(mod) && !Serialization::CompletionistData::IsModInstalled(mod)) {
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
}