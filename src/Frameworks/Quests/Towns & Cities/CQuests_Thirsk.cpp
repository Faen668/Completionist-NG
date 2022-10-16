#include "CQuests_Thirsk.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Thirsk {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Thirsk_Quest00_Key", "$Thirsk_Quest00_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest00_Data", "DLC2MH02"},
	/*01*/ {"Thirsk_Quest01_Key", "$Thirsk_Quest01_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest01_Data", "DLC2ThirskFFElmusBack"},
	/*02*/ {"Thirsk_Quest02_Key", "$Thirsk_Quest02_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest02_Data", "DLC2ThirskFFElmus"},
	/*03*/ {"Thirsk_Quest03_Key", "$Thirsk_Quest03_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest03_Data", "DLC2ThirskFFHalbarn"},
	/*04*/ {"Thirsk_Quest04_Key", "$Thirsk_Quest04_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest04_Data", "DLC2ThirskFFHilund"},
	/*05*/ {"Thirsk_Quest05_Key", "$Thirsk_Quest05_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest05_Data", "DLC2MH01"},
	/*06*/ {"Thirsk_Quest06_Key", "$Thirsk_Quest06_Name", SIDE_QUEST_FLAG, "$Thirsk_Quest06_Data", "DLC2MHMisc"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5,
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 6, 99 },
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

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}
	};
}