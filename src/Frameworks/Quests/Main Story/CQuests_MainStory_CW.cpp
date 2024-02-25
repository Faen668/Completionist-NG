#include "CQuests_MainStory_CW.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/Events.hpp"

namespace CQFramework_CW 
{
	CCivilWarData CivilWarData[]{
		{"CWQuest03", 0x018A56, CQuestProcessor::kExcluded},
		{"CWQuest05", 0x018A57, CQuestProcessor::kExcluded},
		{"CWQuest10", 0x01676A, CQuestProcessor::kExcluded},
		{"CWQuest11", 0x01676D, CQuestProcessor::kExcluded},
		{"CWQuest12", 0x01676C, CQuestProcessor::kExcluded},
		{"CWQuest13", 0x01676F, CQuestProcessor::kExcluded},
		{"CWQuest14", 0x01676E, CQuestProcessor::kExcluded},
		{"CWQuest15", 0x016769, CQuestProcessor::kExcluded},
		{"CWQuest19", 0x018A56, CQuestProcessor::kExcluded},
		{"CWQuest21", 0x018A5A, CQuestProcessor::kExcluded},
		{"CWQuest25", 0x01676D, CQuestProcessor::kExcluded},
		{"CWQuest26", 0x01676C, CQuestProcessor::kExcluded},
		{"CWQuest27", 0x01676B, CQuestProcessor::kExcluded},
		{"CWQuest28", 0x016770, CQuestProcessor::kExcluded},
		{"CWQuest29", 0x01676E, CQuestProcessor::kExcluded},
		{"CWQuest30", 0x016769, CQuestProcessor::kExcluded},
	};

	CStageData StageData[]
	{
		{"CWQuest08", CStageEnum::kDone, 255}
	};

	CQuestData QuestData[]{
		/*00*/ {"CWQuest00", CFlagEnum::kMain, CCompEnum::kStand, "CW01A"},
		/*01*/ {"CWQuest01", CFlagEnum::kMain, CCompEnum::kStand, "CW02A"},
		/*02*/ {"CWQuest02", CFlagEnum::kMain, CCompEnum::kStand, "CW03"},
		/*03*/ {"CWQuest03", CFlagEnum::kMain, CCompEnum::kCiWar, "CWSiegeObj"},
		/*04*/ {"CWQuest04", CFlagEnum::kMain, CCompEnum::kStand, "CWObj"},
		/*05*/ {"CWQuest05", CFlagEnum::kMain, CCompEnum::kCiWar, "CWSiegeObj"},
		/*06*/ {"CWQuest06", CFlagEnum::kSide, CCompEnum::kStand, "CWMission03"},
		/*07*/ {"CWQuest07", CFlagEnum::kSide, CCompEnum::kStand, "CWMission07"},
		/*08*/ {"CWQuest08", CFlagEnum::kSide, CCompEnum::kStage, "CW01AOutfitImperial"},
		/*09*/ {"CWQuest09", CFlagEnum::kSide, CCompEnum::kStand, "CWMission04"},
		/*10*/ {"CWQuest10", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*11*/ {"CWQuest11", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*12*/ {"CWQuest12", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*13*/ {"CWQuest13", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*14*/ {"CWQuest14", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*15*/ {"CWQuest15", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},

		/*16*/ {"CWQuest16", CFlagEnum::kMain, CCompEnum::kStand, "CW01B"},
		/*17*/ {"CWQuest17", CFlagEnum::kMain, CCompEnum::kStand, "CW02B"},
		/*18*/ {"CWQuest18", CFlagEnum::kMain, CCompEnum::kStand, "CW03"},
		/*19*/ {"CWQuest19", CFlagEnum::kMain, CCompEnum::kCiWar, "CWSiegeObj"},
		/*20*/ {"CWQuest20", CFlagEnum::kMain, CCompEnum::kStand, "CWObj"},
		/*21*/ {"CWQuest21", CFlagEnum::kMain, CCompEnum::kCiWar, "CWSiegeObj"},
		/*22*/ {"CWQuest22", CFlagEnum::kSide, CCompEnum::kStand, "CWMission03"},
		/*23*/ {"CWQuest23", CFlagEnum::kSide, CCompEnum::kStand, "CWMission07"},
		/*24*/ {"CWQuest24", CFlagEnum::kSide, CCompEnum::kStand, "CWMission04"},
		/*25*/ {"CWQuest25", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*26*/ {"CWQuest26", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*27*/ {"CWQuest27", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*28*/ {"CWQuest28", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*29*/ {"CWQuest29", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
		/*30*/ {"CWQuest30", CFlagEnum::kSide, CCompEnum::kCiWar, "CWFortSiegeFort"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initStageData(StageData)->initCivilWarData(CivilWarData)->finalize();
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 1, QuestData[i].unique_identifier));
		}
	};

	//---------------------------------------------------
	//-- Framework Functions (Process Events ) ----------
	//---------------------------------------------------

	RE::BGSLocation* CHandler::GetAliasLocation(RE::TESQuest* a_quest, uint32_t alias_id)
	{
		using func_t = decltype(&GetAliasLocation);
		REL::Relocation<func_t> func{ GetAliasLocAddress };
		return a_quest ? func(a_quest, alias_id) : nullptr;
	}

	//---------------------------------------------------
	//-- Framework Functions (Process Events ) ----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {

		if (!a_event || a_event->stage != 9000) 
		{ 
			return EventResult::kContinue; 
		}

		auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest)
		{
			return EventResult::kContinue;
		}

		const std::string questID = quest->GetFormEditorID();
		if (!DKUtil::string::iequals(questID, "CWSiegeObj") && !DKUtil::string::iequals(questID, "CWFortSiegeFort"))
		{
			return EventResult::kContinue;
		}

		for (const auto& alias : quest->aliases) 
		{
			if (alias && (DKUtil::string::iequals(alias->aliasName, "City") || DKUtil::string::iequals(alias->aliasName, "Hold")))
			{
				auto* reference = static_cast<RE::BGSLocAlias*>(alias);
				if (!reference)
				{
					return EventResult::kContinue;
				}

				auto* location = GetAliasLocation(quest, alias->aliasID);
				if (!location)
				{
					return EventResult::kContinue;
				}

				for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
				{
					auto &data = QuestData[i];

					if (data.HasCivilWarData() && !CFramework_Master::CQuestKeys_Natural.HasKey(data.GetKey()))
					{
						auto* loc = data.GetCivilWarLocation();
						if (loc && loc->GetFormID() == location->GetFormID())
						{
							data.is_completed = true;
							CFramework_Master::RadiantCountData.IncreaseCount(data.GetKey(), 1);
							CFramework_Master::CQuestKeys_Natural.AddKey(data.GetKey());
							CFramework_Master::CQuestKeys_Manual.RemoveKey(data.GetKey());
							CFramework_Master::FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::kQuestComplete, data.GetName());
							return EventResult::kContinue;
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
};