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

static void PerformLoadOrderValidationChecks()
{
	const auto& Handler = RE::TESDataHandler::GetSingleton();
	if (!Handler) {
		ERROR("Completionist was unable to retrieve TESDataHandler.");
	}

	const auto& frm = Handler->LookupForm(0x000823, modName);
	if (!frm)
	{
		INFO("");
		INFO("Loaded plugins:");
		for (size_t i = 0; i < Handler->GetLoadedModCount(); i++) {
			INFO("  {:02X}     {}", i, Handler->GetLoadedMods()[i]->fileName);
		}

		for (size_t i = 0; i < Handler->GetLoadedLightModCount(); i++) {
			INFO("  FE {:03X} {}", i, Handler->GetLoadedLightMods()[i]->fileName);
		}

		INFO("");
		INFO("Working directory: {}", std::filesystem::current_path().string());
		INFO("Virtual data folder contents:");
		for (auto& entry : std::filesystem::directory_iterator("Data")) {
			if (!entry.is_regular_file()) {
				continue;
			}

			if (entry.path().extension() != ".esm" &&
				entry.path().extension() != ".esp" &&
				entry.path().extension() != ".esl") {
				continue;
			}

			INFO("  {}", entry.path().filename().string());
		}

		INFO("");

		const auto comp = Handler->GetLoadedLightModIndex("Completionist.esp");
		if (comp.has_value())
		{
			INFO("Completionist plugin index: FE {:03X}", comp.value());
		}
		else
		{
			INFO("Completionist plugin index: not found");
		}

		ERROR("Completionist.esp not found in load order.\n\nA load order report was generated and logged to Completionist.log.");
	}
}

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message) 
{
	switch (message->type)
	{
	case SKSE::MessagingInterface::kDataLoaded: 
	{
		PerformLoadOrderValidationChecks();

		auto t1 = std::chrono::steady_clock::now();

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
		Completionist::MuseumAPI::Register();
		INFO("Finished installing Completionist in - {} Milliseconds", (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1)).count());
		break;
	}

	case SKSE::MessagingInterface::kNewGame:
		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
		Completionist::MuseumAPI::OnPlayerLoadGame();
		break;

	case SKSE::MessagingInterface::kPostLoadGame:
		CVariables::VariablesAPI::Update();
		CFramework_Master::FrameworkAPI::Update();
		Completionist::MuseumAPI::OnPlayerLoadGame();
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