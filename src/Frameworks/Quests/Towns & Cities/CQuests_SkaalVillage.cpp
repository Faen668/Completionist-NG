#include "CQuests_SkaalVillage.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_SkaalVillage {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"SkaalVillage_Quest00_Key", "$SkaalVillage_Quest00_Name", SIDE_QUEST_FLAG, "$SkaalVillage_Quest00_Data", "DLC2SkaalVillageFreeform2"},
	/*01*/ {"SkaalVillage_Quest01_Key", "$SkaalVillage_Quest01_Name", SIDE_QUEST_FLAG, "$SkaalVillage_Quest01_Data", "DLC2WB01"},
	/*02*/ {"SkaalVillage_Quest02_Key", "$SkaalVillage_Quest02_Name", SIDE_QUEST_FLAG, "$SkaalVillage_Quest02_Data", "DLC2SV01"},
	/*03*/ {"SkaalVillage_Quest03_Key", "$SkaalVillage_Quest03_Name", SIDE_QUEST_FLAG, "$SkaalVillage_Quest03_Data", "DLC2SV02"},
	/*04*/ {"SkaalVillage_Quest04_Key", "$SkaalVillage_Quest04_Name", SIDE_QUEST_FLAG, "$SkaalVillage_Quest04_Data", "DLC2SkaalVillageFreeform1"},
	/*05*/ {"SkaalVillage_Quest05_Key", "$SkaalVillage_Quest05_Name", RADI_QUEST_FLAG, "$SkaalVillage_Quest05_Data", "Favor104"},
	};

	constexpr std::size_t StandardCompletion[] = {
	1,2,3,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{5, "Completionist_Favor104Fanari", 1},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 30 },
	{ 4, 20 },
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

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}