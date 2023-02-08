#include "Structs.hpp"
#include "CQuests_Misc_DB.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Misc_DB {
	using namespace CFramework_Master;

	CQuestData QuestData[]{
	{"Misc_DB_Quest00", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kDone, 200,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2dunHaknirTreasureQST"},
	{"Misc_DB_Quest01", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2EbonyWarriorQuest"},
	{"Misc_DB_Quest02", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kDone, 10,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2dunHrodulf"},
	{"Misc_DB_Quest03", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2KagrumezQST"},
	{"Misc_DB_Quest04", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kDone, 255,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2dunKarstaagQST"},
	{"Misc_DB_Quest05", CFlagEnum::kSide,  CCompEnum::kStage, { CStageEnum::kDone, 600,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2dunKolbjornQST"},
	{"Misc_DB_Quest06", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2BlackBook03Quest"},
	{"Misc_DB_Quest07", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2BlackBook04Quest"},
	{"Misc_DB_Quest08", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2BlackBook05Quest"},
	{"Misc_DB_Quest09", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2BlackBook06Quest"},
	{"Misc_DB_Quest10", CFlagEnum::kSide,  CCompEnum::kStand, { CStageEnum::kNone, 0,	0  }, { " ", CRadiantEnum::kRadiant_Non }, "DLC2BlackBook07Quest"},
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
				BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], Data.GLOBAL.NAME, std::to_underlying(Data.GLOBAL.VALUE));
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