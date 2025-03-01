#include "Structs.hpp"
#include "Serialization.hpp";
#include "CQuests_Drunks.hpp"
#include "Internal Utility/Events.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using namespace Serialization;

std::vector<CDrunkData> Drunk_Data = 
{
	{"Dawnstar_Quest05",	0x072EA2, 0x013619, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
	{"Markarth_Quest22",	0x072EA2, 0x013392, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
	{"Markarth_Quest23",	0x072EA2, 0x013390, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
	{"SmallTowns_Quest17",	0x072EA2, 0x03550B, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
	{"Windhelm_Quest10",	0x072EA2, 0x01413F, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
	{"Winterhold_Quest03",	0x072EA2, 0x01C186, "Skyrim.esm", "Skyrim.esm", CQuestProcessor::kExcluded},
};

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace CQFramework_FavorQuests
{
	void DrunkHandler::InstallFramework()
	{
		CEvents::EventHandler::RegisterForEvent_OnContainerChangedEvent(OnContainerChanged);
	}

	//---------------------------------------------------
	//-- Add Drunk Data ---------------------------------
	//---------------------------------------------------

	void DrunkHandler::AddDrunkData(const std::string& Quest_Key, const RE::FormID a_listID, const RE::FormID a_baseID, const std::string& listFileName, const std::string& baseFileName)
	{	
		CDrunkData data = { Quest_Key, a_listID, a_baseID, listFileName, baseFileName, CQuestProcessor::kExcluded };
		Drunk_Data.push_back(data);
	}

	//---------------------------------------------------
	//-- Process Drunk Data -----------------------------
	//---------------------------------------------------

	void DrunkHandler::OnContainerChanged(RE::TESContainerChangedEvent const* a_event)
	{
		if (a_event && a_event->baseObj && a_event->newContainer && a_event->oldContainer == RE::PlayerCharacter::GetSingleton()->GetFormID())
		{
			auto* speaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();
			if (!speaker)
			{
				return;
			}

			auto* speakerActr = speaker->As<RE::Actor>();
			if (!speakerActr)
			{
				return;
			}

			for (auto& [key, lFormID, aFormID, lName, aName, Processor] : Drunk_Data)
			{
				auto* npc = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESNPC>(aFormID, aName);
				if (!npc)
				{
					continue;
				}

				if (npc->GetFormID() == speakerActr->GetActorBase()->GetFormID() && speakerActr->GetFormID() == a_event->newContainer)
				{
					auto* fList = static_cast<RE::BGSListForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(lFormID, lName));
					if (fList && fList->HasForm(a_event->baseObj)) {
						CFramework_Master::RadiantCountData.IncreaseCount(key, 1);
						CQuestMaster::QuestAPI::CheckForRadiantQuestCompletion(key);
					}
				}
			}
		}
	}
}