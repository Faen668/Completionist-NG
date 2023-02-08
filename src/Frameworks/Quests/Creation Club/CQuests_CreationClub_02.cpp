#include "Structs.hpp"
#include "CQuests_CreationClub_02.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC2 {
	using namespace CFramework_Master;

	CQuestData QuestData[] {
	{"CC02_Quest00", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccKRTSSE001_zQuest"},
	{"CC02_Quest01", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE005_MainQuest"},
	{"CC02_Quest02", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE013_AquisitionQuest"},
	{"CC02_Quest03", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE057_Quest"},
	{"CC02_Quest04", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_QuestD"},
	{"CC02_Quest05", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccFFBSSE001_Quest"},
	{"CC02_Quest06", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE050_Quest"},
	{"CC02_Quest07", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 40  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE013_BloodthirstQuest"},
	{"CC02_Quest08", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE040_Quest"},
	{"CC02_Quest09", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE059_Quest"},
	{"CC02_Quest10", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE052_Quest"},
	{"CC02_Quest11", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_QuestA"},
	{"CC02_Quest12", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_QuestC"},
	{"CC02_Quest13", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE053_Quest"},
	{"CC02_Quest14", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccRMSSSE001_Quest"},
	{"CC02_Quest15", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE062_Quest"},
	{"CC02_Quest16", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "DLCDwarvenPuzzleDungeonQuest01"},
	{"CC02_Quest17", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccKRTSSE001_QuestFortune"},
	{"CC02_Quest18", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE055_Quest"},
	{"CC02_Quest19", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_Quest"},
	{"CC02_Quest20", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEEJSSE005_Quest"},
	{"CC02_Quest21", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE063_Quest"},
	{"CC02_Quest22", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_QuestB"},
	{"CC02_Quest23", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE038_Quest"},
	{"CC02_Quest24", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE036_PetBWolfAcquisitionQuest"},
	{"CC02_Quest25", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE061_Quest"},
	{"CC02_Quest26", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE051_Quest"},
	{"CC02_Quest27", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 30  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE010_PetPurchaseQuest"},
	{"CC02_Quest28", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE043_VampireHunterQuest"},
	{"CC02_Quest29", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE064_Quest"},
	{"CC02_Quest30", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE058_Quest"},
	{"CC02_Quest31", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 100 , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccMTYSSE001_StartupQuest_Alt"},
	{"CC02_Quest32", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE054_Quest"},
	{"CC02_Quest33", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 30  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE002_SoulStealerQuest"},
	{"CC02_Quest34", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE002_TelekinesisArrowQuest"},
	{"CC02_Quest35", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccPEWSSE002_Quest"},
	{"CC02_Quest36", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "DLCDwarvenPuzzleDungeonCrownQuest"},
	{"CC02_Quest37", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "DLCDwarvenPuzzleDungeonHorseQuest"},
	{"CC02_Quest38", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE014_SpellPack_StartupQuest"},
	{"CC02_Quest39", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 1000, 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE007_Quest"},
	{"CC02_Quest40", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccMTYSSE001_Quest"},
	{"CC02_Quest41", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccKRTSSE001_QuestPower"},
	{"CC02_Quest42", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE031_QUEST"},
	{"CC02_Quest43", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE060_Quest"},
	{"CC02_Quest44", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccASVSSE001_QuestE"},
	{"CC02_Quest45", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE056_Quest"},
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