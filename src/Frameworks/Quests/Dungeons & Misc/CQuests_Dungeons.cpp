#include "Structs.hpp"
#include "CQuests_Dungeons.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Dungeons {
	using namespace Serialization;
	using namespace CFramework_Master;

	CQuestData QuestData[]{
	{"Dungeons_Quest00", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 254,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunAngarvundeQST"},
	{"Dungeons_Quest01", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunAnsilvundQST"},
	{"Dungeons_Quest02", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunBlindCliffQST"},
	{"Dungeons_Quest03", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunDarklightQST"},
	{"Dungeons_Quest04", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunForelhostQST"},
	{"Dungeons_Quest05", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunFrostflowAbyssQST"},
	{"Dungeons_Quest06", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 100,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunFrostmereCryptQST"},
	{"Dungeons_Quest07", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunHighGateRuinsQST"},
	{"Dungeons_Quest08", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunHillgrundsTombQST"},
	{"Dungeons_Quest09", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 81,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunIronbindQST"},
	{"Dungeons_Quest10", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 100,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunLiarsRetreatQST"},
	{"Dungeons_Quest11", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunMistwatchQST"},
	{"Dungeons_Quest12", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunNilheimQST"},
	{"Dungeons_Quest13", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 30,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunRagnvaldQST"},
	{"Dungeons_Quest14", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 30,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunRebelsCairnQST"},
	{"Dungeons_Quest15", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 20,	40 }, { " ", CRadiantEnum::kRadiant_Non }, "dunRobbersGorgeQST"},
	{"Dungeons_Quest16", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunSouthfringeQST"},
	{"Dungeons_Quest17", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunTrevasWatchQST"},
	{"Dungeons_Quest18", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunValthumeQST"},
	{"Dungeons_Quest19", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 20,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunVolskyggeQST"},
	{"Dungeons_Quest20", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunVolunruudQST"},
	{"Dungeons_Quest21", CFlagEnum::kSide, CCompEnum::kStage, { CStageEnum::kDone, 100,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "dunYngolBarrowQST"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		SinkEvents();

		IdenArray.clear();
		NameArray.clear();
		RadiArray.clear();
		NameArray.clear();
		KeysArray.clear();

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++) {
			auto& Data = QuestData[i];

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
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		auto userinterface = RE::UI::GetSingleton();
		userinterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		for (auto& Data : QuestData) {
			if (Data.STAGE.CONDITION == CStageEnum::kDone && DKUtil::string::iequals(Data.EDID, quest->GetFormEditorID())) {
				CQuestKeys_Stages.AddStage(fmt::format("{:s}_Key"sv, Data.UDID), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, fmt::format("{:s}_Key [{}]"sv, Data.UDID, quest->GetName()));
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME || !a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		CHandler::UpdateCompletion();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateCompletion() {

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++) {
			auto& Data = QuestData[i];

			if (std::ranges::find(ExcludedQuestsArray, Data.EDID) != ExcludedQuestsArray.end()) {
				continue;
			}

			switch (Data.COMP) {

			case CCompEnum::kStand: {
				BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_N(KeysArray[i], IdenArray[i]);
				break;
			}

			case CCompEnum::kGlobl: {
				BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], Data.GLOBAL.NAME, Data.GLOBAL.VALUE);
				break;
			}

			case CCompEnum::kStage: {

				switch (Data.STAGE.CONDITION) {

				case CStageEnum::kDone:
					BoolArray[i] =
						FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], Data.STAGE.VALUE) || Data.STAGE.OPVALUE != 0 &&
						FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_S(KeysArray[i], IdenArray[i], Data.STAGE.OPVALUE);
					break;

				case CStageEnum::kPast:
					BoolArray[i] =
						FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], Data.STAGE.VALUE) || Data.STAGE.OPVALUE != 0 &&
						FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], Data.STAGE.OPVALUE);
					break;

				default:
					BoolArray[i] = false;
					break;
				}
				break;
			}

			default: {
				BoolArray[i] = false;
				break;
			}
			}
		}
	};
}