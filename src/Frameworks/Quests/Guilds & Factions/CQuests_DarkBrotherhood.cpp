#include "CQuests_DarkBrotherhood.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_DarkBrotherhood {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"DarkBrotherhood_Quest00_Key", "$DarkBrotherhood_Quest00_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest00_Data", "DB01"},
	/*01*/ {"DarkBrotherhood_Quest01_Key", "$DarkBrotherhood_Quest01_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest01_Data", "DB01Misc"},
	/*02*/ {"DarkBrotherhood_Quest02_Key", "$DarkBrotherhood_Quest02_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest02_Data", "DB02"},
	/*03*/ {"DarkBrotherhood_Quest03_Key", "$DarkBrotherhood_Quest03_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest03_Data", "DB02a"},
	/*04*/ {"DarkBrotherhood_Quest04_Key", "$DarkBrotherhood_Quest04_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest04_Data", "DB03"},
	/*05*/ {"DarkBrotherhood_Quest05_Key", "$DarkBrotherhood_Quest05_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest05_Data", "DB04"},
	/*06*/ {"DarkBrotherhood_Quest06_Key", "$DarkBrotherhood_Quest06_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest06_Data", "DB04a"},
	/*07*/ {"DarkBrotherhood_Quest07_Key", "$DarkBrotherhood_Quest07_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest07_Data", "DB05"},
	/*08*/ {"DarkBrotherhood_Quest08_Key", "$DarkBrotherhood_Quest08_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest08_Data", "DB06"},
	/*09*/ {"DarkBrotherhood_Quest09_Key", "$DarkBrotherhood_Quest09_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest09_Data", "DB07"},
	/*10*/ {"DarkBrotherhood_Quest10_Key", "$DarkBrotherhood_Quest10_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest10_Data", "DB08"},
	/*11*/ {"DarkBrotherhood_Quest11_Key", "$DarkBrotherhood_Quest11_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest11_Data", "DB09"},
	/*12*/ {"DarkBrotherhood_Quest12_Key", "$DarkBrotherhood_Quest12_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest12_Data", "DB10"},
	/*13*/ {"DarkBrotherhood_Quest13_Key", "$DarkBrotherhood_Quest13_Name", MAIN_QUEST_FLAG, "$DarkBrotherhood_Quest13_Data", "DB11"},
	/*14*/ {"DarkBrotherhood_Quest14_Key", "$DarkBrotherhood_Quest14_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest14_Data", "DBDestroy"},
	/*15*/ {"DarkBrotherhood_Quest15_Key", "$DarkBrotherhood_Quest15_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest15_Data", "DBSideContract09"},
	/*16*/ {"DarkBrotherhood_Quest16_Key", "$DarkBrotherhood_Quest16_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest16_Data", "DBSideContract08"},
	/*17*/ {"DarkBrotherhood_Quest17_Key", "$DarkBrotherhood_Quest17_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest17_Data", "DBSideContract03"},
	/*18*/ {"DarkBrotherhood_Quest18_Key", "$DarkBrotherhood_Quest18_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest18_Data", "DBSideContract06"},
	/*19*/ {"DarkBrotherhood_Quest19_Key", "$DarkBrotherhood_Quest19_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest19_Data", "DBSideContract02"},
	/*20*/ {"DarkBrotherhood_Quest20_Key", "$DarkBrotherhood_Quest20_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest20_Data", "DBSideContract11"},
	/*21*/ {"DarkBrotherhood_Quest21_Key", "$DarkBrotherhood_Quest21_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest21_Data", "DBSideContract04"},
	/*22*/ {"DarkBrotherhood_Quest22_Key", "$DarkBrotherhood_Quest22_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest22_Data", "DBSideContract05"},
	/*23*/ {"DarkBrotherhood_Quest23_Key", "$DarkBrotherhood_Quest23_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest23_Data", "DBSideContract10"},
	/*24*/ {"DarkBrotherhood_Quest24_Key", "$DarkBrotherhood_Quest24_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest24_Data", "DBSideContract07"},
	/*25*/ {"DarkBrotherhood_Quest25_Key", "$DarkBrotherhood_Quest25_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest25_Data", "DBSideContract01"},
	/*26*/ {"DarkBrotherhood_Quest26_Key", "$DarkBrotherhood_Quest26_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest26_Data", "DBSideContract12"},
	/*27*/ {"DarkBrotherhood_Quest27_Key", "$DarkBrotherhood_Quest27_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest27_Data", "DBOlavaReadingTreasureObjective"},
	/*28*/ {"DarkBrotherhood_Quest28_Key", "$DarkBrotherhood_Quest28_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest28_Data", "DBTortureTreasureMiscObjective1"},
	/*29*/ {"DarkBrotherhood_Quest29_Key", "$DarkBrotherhood_Quest29_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest29_Data", "DBTortureTreasureMiscObjective2"},
	/*30*/ {"DarkBrotherhood_Quest30_Key", "$DarkBrotherhood_Quest30_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest30_Data", "DBTortureTreasureMiscObjective3"},
	/*31*/ {"DarkBrotherhood_Quest31_Key", "$DarkBrotherhood_Quest31_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest31_Data", "DBTortureTreasureMiscObjective4"},
	/*32*/ {"DarkBrotherhood_Quest32_Key", "$DarkBrotherhood_Quest32_Name", SIDE_QUEST_FLAG, "$DarkBrotherhood_Quest32_Data", "DarkBrotherhoodSanctuaryRepair"},
	/*33*/ {"DarkBrotherhood_Quest33_Key", "$DarkBrotherhood_Quest33_Name", RADI_QUEST_FLAG, "$DarkBrotherhood_Quest33_Data", "DBrecurring"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{33, "Completionist_DBrecurring", RADIANT_BROTHERHOOD_VALUE},
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

		for (auto& [Key, Name, Flag, Text, ID] : QuestData) {
			KeysArray.push_back(Key);
			NameArray.push_back(Name);
			RadiArray.push_back(Flag);
			TextArray.push_back(Text);
			IdenArray.push_back(ID);
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

		for (auto i : StandardCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_N(KeysArray[i], IdenArray[i]);
		}

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}
	};
}