#include "Serialization.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/Variables.hpp"
#include "Internal Utility/MCMHandler.hpp"
#include "Internal Utility/Localisation.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "Internal Utility/Events.hpp"

const SKSE::MessagingInterface* g_messaging = nullptr;
const SKSE::LoadInterface* g_LoadInterface = nullptr;
const SKSE::QueryInterface* g_QueryInterface = nullptr;
constexpr const char* modName = "Completionist.esp";

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) 
{
	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded: 
	{
		auto t1 = std::chrono::steady_clock::now();

		const auto& Handler = RE::TESDataHandler::GetSingleton();
		if (!Handler) {
			ERROR("Completionist was unable to retrieve TESDataHandler.");
		}

		const auto& frm = Handler->LookupForm(0x000823, modName);
		if (!frm) {
			ERROR("Completionist.esp not found in load order.\n\nTry moving the .esp further up your load order so it has a higher priority.");
		}

		CEvents::EventHandler::Register();
		CVariables::VariablesAPI::Register();
		CLocalisation::LocalisationAPI::Register();

		Completionist_MainHUD::TextnTagsAPI::Register();

		CQuestMaster::QuestAPI::Register();
		CFramework_Master::FrameworkAPI::Register();
		CExternalPatchHandler::CHandler::Register();
		CFramework_Master::FrameworkAPI::RegisterCustomPatches();
		CFramework_Master::FrameworkAPI::FinalizeRegistrations();
		CHCMHandler::MCMAPI::Register();
		INFO("Finished installing Completionist in - {} Milliseconds", (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1)).count());
		break;
	}

	case SKSE::MessagingInterface::kNewGame:
		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
		break;

	case SKSE::MessagingInterface::kPostLoadGame:
		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
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
	SKSE::AllocTrampoline(256);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
	if (!g_messaging)
	{
		INFO("Failed to load messaging interface! This error is fatal, plugin will not load.");
		return false;
	}

	g_messaging->RegisterListener("SKSE", SKSEMessageHandler);
	const auto* serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kHeader);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	serialization->SetRevertCallback(Serialization::RevertCallback);
	return true;
}