#include "CQuests_Winterhold.hpp"
#include "Internal Utility/Array.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Winterhold 
{
	using namespace CFramework_Master;

	CQuestData QuestData[]{
		/*00*/ {"Winterhold_Quest00", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 40,	0  }, { "",									CRadiantEnum::kRadiant_Non }, "FreeformWinterholdC", {"", ""} },
		/*01*/ {"Winterhold_Quest01", CFlagEnum::kSide, CCompEnum::kGlobl, { CStageEnum::kNone, 0,	0  }, { "Completionist_Favor013Haran",		CRadiantEnum::kRadiant_One }, "Favor013", {"", ""} },
		/*02*/ {"Winterhold_Quest02", CFlagEnum::kSide, CCompEnum::kGlobl, { CStageEnum::kNone, 0,	0  }, { "Completionist_Favor158Korir",		CRadiantEnum::kRadiant_One }, "Favor158", {"", ""} },
		/*03*/ {"Winterhold_Quest03", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 0,	0  }, { "Completionist_FavorDrunksRanmir",	CRadiantEnum::kRadiant_Def }, "FavorJobsDrunks", {"", ""} },
		/*04*/ {"Winterhold_Quest04", CFlagEnum::kSide, CCompEnum::kGlobl, { CStageEnum::kNone, 0,	0  }, { "Completionist_Favor018Malur",		CRadiantEnum::kRadiant_One }, "Favor018", {"", ""} },
		/*05*/ {"Winterhold_Quest05", CFlagEnum::kSide, CCompEnum::kJarls, { CStageEnum::kNone, 0,	0  }, { "",									CRadiantEnum::kRadiant_Non }, "FavorJarlsMakeFriends", {"WinterholdImpGetOutofJail", "WinterholdSonsGetOutofJail"} },
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() 
	{
		SinkEvents();

		for (auto itr = 0; itr < std::extent_v<decltype(QuestData)>; itr++) 
		{
			auto& Data = QuestData[itr];

			KeysArray.push_back(fmt::format("{:s}_Key"sv, Data.UDID));
			NameArray.push_back(fmt::format("${:s}_Name"sv, Data.UDID));
			TextArray.push_back(fmt::format("${:s}_Data"sv, Data.UDID));
			RadiArray.push_back(std::to_underlying(Data.FLAG));
			IdenArray.push_back(Data.EDID);
		}

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Events ) ------------
	//---------------------------------------------------

	void CHandler::SinkEvents() 
	{
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) 
	{
		if (!a_event || !a_event->stage) 
		{ 
			return RE::BSEventNotifyControl::kContinue; 
		}

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) 
		{ 
			return EventResult::kContinue; 
		}

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			auto& Data = QuestData[i];
			if (Data.STAGE.CONDITION == CStageEnum::kDone && DKUtil::string::iequals(Data.EDID, quest->GetFormEditorID()))
			{
				CQuestKeys_Stages.AddStage(fmt::format("{:s}_Key"sv, Data.UDID), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, fmt::format("{:s}_Key [{}]"sv, Data.UDID, quest->GetName()));
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) 
	{
		if (!a_event || !a_event->baseObj || a_event->oldContainer != RE::PlayerCharacter::GetSingleton()->GetFormID()) 
		{ 
			return EventResult::kContinue; 
		}

		auto* List = static_cast<RE::BGSListForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x0072EA2, "Skyrim.esm"));
		if (!List || !List->HasForm(a_event->baseObj)) 
		{ 
			return EventResult::kContinue; 
		}

		auto speaker = RE::MenuTopicManager::GetSingleton()->speaker.get().get();
		if (!speaker || speaker->GetFormID() != a_event->newContainer) 
		{ 
			return EventResult::kContinue; 
		}

		auto* ranmir = static_cast<RE::TESObjectREFR*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x001C18B, "Skyrim.esm"));
		if (!ranmir) { return EventResult::kContinue; }

		if (speaker == ranmir && a_event->newContainer == ranmir->GetFormID()) 
		{
			if (auto* var = RE::TESForm::LookupByEditorID<RE::TESGlobal>("Completionist_FavorDrunksRanmir")) 
			{
				var->value++;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) 
	{
		if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME || !a_event->opening) 
		{ 
			return RE::BSEventNotifyControl::kContinue; 
		}

		CHandler::UpdateCompletion();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Set Completed Quests ) ---
	//---------------------------------------------------

	void CHandler::UpdateCompletion() 
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++) {
			auto& Data = QuestData[i];

			if (ExcludedQuests.contains(Data.UDID)) 
			{
				continue;
			}

			BoolArray[i] = FrameworkAPI::UpdateQuestCompletion(CQuestData_Params);
		}
	};
}