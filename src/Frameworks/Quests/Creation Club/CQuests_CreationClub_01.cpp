#include "Structs.hpp"
#include "CQuests_CreationClub_01.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_CC1 {
	using namespace CFramework_Master;

	CQuestData QuestData[] {
		{"CC01_Quest00", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccVSVSSE004_MainQuest"},
		{"CC01_Quest01", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccVSVSSE004_RestoreFarmQuest"},
		{"CC01_Quest02", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Start_MQ2"},
		{"CC01_Quest03", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Dwarven"},
		{"CC01_Quest04", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Falkreath"},
		{"CC01_Quest05", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Fish_MQ1"},
		{"CC01_Quest06", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Fish_MQ2"},
		{"CC01_Quest07", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Crab_MQ2"},
		{"CC01_Quest08", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Fish_MQ4"},
		{"CC01_Quest09", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Crab_MQ3"},
		{"CC01_Quest10", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Fish_MQ5"},
		{"CC01_Quest11", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Fish_MQ3"},
		{"CC01_Quest12", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_WhiterunPartOne"},
		{"CC01_Quest13", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Crab_MQ1"},
		{"CC01_Quest14", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Morthal"},
		{"CC01_Quest15", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Markarth"},
		{"CC01_Quest16", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Khajiit"},
		{"CC01_Quest17", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Solitude"},
		{"CC01_Quest18", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_Windhelm"},
		{"CC01_Quest19", CFlagEnum::kSide, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Misc_WhiterunPartTwo"},
		{"CC01_Quest20", CFlagEnum::kMain, CCompEnum::kStand, { CStageEnum::kNone, 0   , 0 }, { " ",						CRadiantEnum::kRadiant_Non }, "ccBGSSSE001_Crab_MQ4"},
		{"CC01_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 0   , 0 }, { "Completionist_Fishing2",	CRadiantEnum::kRadiant_Fsh }, "ccBGSSSE001_Radiant_2"},
		{"CC01_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 0   , 0 }, { "Completionist_Fishing3",	CRadiantEnum::kRadiant_Fsh }, "ccBGSSSE001_Radiant_3"},
		{"CC01_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, { CStageEnum::kNone, 0   , 0 }, { "Completionist_Fishing1",	CRadiantEnum::kRadiant_Fsh }, "ccBGSSSE001_Radiant_1"},
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
		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));
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