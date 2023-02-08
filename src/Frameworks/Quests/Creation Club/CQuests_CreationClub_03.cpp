#include "Structs.hpp"
#include "CQuests_CreationClub_03.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC3 {
	using namespace CFramework_Master;

	CQuestData QuestData[]{
	{"CC03_Quest00", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_MiscQuest_AmberMadnessGear"},
	{"CC03_Quest01", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_QuestA"},
	{"CC03_Quest02", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE068_Quest"},
	{"CC03_Quest03", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE069_Quest"},
	{"CC03_Quest04", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE034_UnicornQuest"},
	{"CC03_Quest05", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEDHSSE002_Quest"},
	{"CC03_Quest06", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE021_LordsMailQuest"},
	{"CC03_Quest07", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_MiscQuest_GSDSGearBook"},
	{"CC03_Quest08", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEEJSSE004_Quest"},
	{"CC03_Quest09", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 10  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE034_WildHorsesQuest"},
	{"CC03_Quest10", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE006_Quest"},
	{"CC03_Quest11", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEDHSSE003_Quest"},
	{"CC03_Quest12", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE008_Quest"},
	{"CC03_Quest13", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE041_Quest"},
	{"CC03_Quest14", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_ElytraPetAcquisition_Dementia"},
	{"CC03_Quest15", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 25  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_ElytraPetAcquisition_Mania"},
	{"CC03_Quest16", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 40  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE035_PetAcquireQuest"},
	{"CC03_Quest17", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEEJSSE002_MageTowerQuest"},
	{"CC03_Quest18", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_MiscQuest_Nerveshatter"},
	{"CC03_Quest19", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 100 , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEDHSSE001_CraftsmanEncounterQuest"},
	{"CC03_Quest20", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 20  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccVSVSSE002_MainQuest"},
	{"CC03_Quest21", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE019_StaffOfSheogorathQuest"},
	{"CC03_Quest22", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_QuestB"},
	{"CC03_Quest23", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 20  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccVSVSSE001_MainQuest"},
	{"CC03_Quest24", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccEEJSSE003_HouseQuest"},
	{"CC03_Quest25", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE025_StaadaQuest"},
	{"CC03_Quest26", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 250 , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE067_Quest"},
	{"CC03_Quest27", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE067_Quest2"},
	{"CC03_Quest28", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE004_Quest"},
	{"CC03_Quest29", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE020_Quest"},
	{"CC03_Quest30", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE003_ZombieQuestStartEncounter"},
	{"CC03_Quest31", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccAARSSE001ManufactoryControlQuest"},
	{"CC03_Quest32", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE045_Quest"},
	{"CC03_Quest33", CFlagEnum::kMain, CCompEnum::kStage, { CStageEnum::kDone, 20  , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "BGSSSE018_ShadowrendQuest"},
	{"CC03_Quest34", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccBGSSSE016_UmbraMainQuest"},
	{"CC03_Quest35", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ", CRadiantEnum::kRadiant_Non }, "ccMTYSSE002_Quest"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {
		SinkEvents();

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