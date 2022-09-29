#include "Serialization.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Papyrus.hpp"
#include "Radiant Quest Handler/Radiant Quests Manager.hpp"
#include "Internal Utility/InventoryMode.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Variables.hpp"

using namespace Completionist;

const SKSE::MessagingInterface* g_messaging = nullptr;
const SKSE::LoadInterface* g_LoadInterface = nullptr;
const SKSE::QueryInterface* g_QueryInterface = nullptr;

/*DLLEXPORT bool SKSEAPI moreHUDItemIsValid(const RE::FormID a_formID) {

	if (a_formID) {
		INFO("Got FormID {}", a_formID);
		return true;
	}
	return false;
};*/

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:

		Completionist_MainHUD::FunctionHolder::InstallHook();
		Completionist_Inventory::FunctionHolder::Register();
		CFramework_Master::FrameworkAPI::Register();
		Quest_Manager::Install();
		Papyrus::Register();
		CVariables::CHandler::Register();
		break;

	case SKSE::MessagingInterface::kNewGame:
		break;

	case SKSE::MessagingInterface::kPreLoadGame:
		break;

	case SKSE::MessagingInterface::kPostLoadGame:
		CVariables::CHandler::Update();
		CFramework_Master::FrameworkAPI::Framework_Load();
		break;

	case SKSE::MessagingInterface::kPostLoad:
		break;

	case SKSE::MessagingInterface::kPostPostLoad:
		break;
	}
}

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) { Sleep(100); }
#endif

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());
	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
	if (!g_messaging)
	{
		INFO("Failed to load messaging interface! This error is fatal, plugin will not load.");
		return false;
	}

	auto papyrus = reinterpret_cast<SKSE::PapyrusInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kPapyrus));
	if (!papyrus)
	{
		INFO("Failed to load scripting interface! This error is fatal, plugin will not load.");
		return false;
	}

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(256);

	g_messaging->RegisterListener("SKSE", SKSEMessageHandler);

	const auto* serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kHeader);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	serialization->SetRevertCallback(Serialization::RevertCallback);
	return true;
}
