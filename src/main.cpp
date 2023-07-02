#include "Serialization.hpp"
#include "Internal Utility/Array.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/Variables.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "Internal Utility/Settings.hpp"
#include "Internal Utility/Localisation.hpp"

const SKSE::MessagingInterface* g_messaging = nullptr;
const SKSE::LoadInterface* g_LoadInterface = nullptr;
const SKSE::QueryInterface* g_QueryInterface = nullptr;

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) 
{
	auto t1 = std::chrono::steady_clock::now();

	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:

		CLocalisation::LocalisationAPI::Register();
		Completionist_MainHUD::TextnTagsAPI::Register();
		CVariables::VariablesAPI::Register();

		CQuestMaster::QuestAPI::Register();
		CFramework_Master::FrameworkAPI::Register();
		CHCMHandler::MCMAPI::Register();
		Settings::Main::Register();

		INFO("Finished installing Completionist in - {} Milliseconds", (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1)).count());
		break;

	case SKSE::MessagingInterface::kNewGame:

		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
		break;

	case SKSE::MessagingInterface::kPreLoadGame:
		break;

	case SKSE::MessagingInterface::kPostLoadGame:

		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
		break;

	case SKSE::MessagingInterface::kPostLoad:
		Completionist_MainHUD::TextnTagsAPI::RegisterQuickLootListener();
		break;

	case SKSE::MessagingInterface::kPostPostLoad:
		Completionist_MainHUD::TextnTagsAPI::RegistermoreHUDListener();
		Completionist_MainHUD::TextnTagsAPI::RegisterQuickLootEEListener();
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
