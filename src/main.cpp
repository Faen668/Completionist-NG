#include "Serialization.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/Variables.hpp"

const SKSE::MessagingInterface* g_messaging = nullptr;
const SKSE::LoadInterface* g_LoadInterface = nullptr;
const SKSE::QueryInterface* g_QueryInterface = nullptr;

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) {
	using namespace Completionist_MainHUD;
	using namespace CFramework_Master;
	using namespace CVariables;

	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded:
		
		TextnTagsAPI::Register();
		VariablesAPI::Register();
		FrameworkAPI::Register();
		break;

	case SKSE::MessagingInterface::kNewGame:

		VariablesAPI::Update();
		FrameworkAPI::Update();
		break;

	case SKSE::MessagingInterface::kPreLoadGame:
		break;

	case SKSE::MessagingInterface::kPostLoadGame:

		VariablesAPI::Update();
		FrameworkAPI::Update();
		break;

	case SKSE::MessagingInterface::kPostLoad:
		TextnTagsAPI::RegisterQuickLootListener();
		break;

	case SKSE::MessagingInterface::kPostPostLoad:
		TextnTagsAPI::RegistermoreHUDListener();
		TextnTagsAPI::RegisterQuickLootEEListener();
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
