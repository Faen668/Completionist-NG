#include "CQuests_MainStory_SK.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_SK {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/ {"MSQ_Quest00_Key", "$MSQ_Quest00_Name", MAIN_QUEST_FLAG, "$MSQ_Quest00_Data", "ARTHLALChargenQuest"},
		/*01*/ {"MSQ_Quest01_Key", "$MSQ_Quest01_Name", MAIN_QUEST_FLAG, "$MSQ_Quest01_Data", "ARTHLALRumorsOfWarQuest"},
		/*02*/ {"MSQ_Quest02_Key", "$MSQ_Quest02_Name", MAIN_QUEST_FLAG, "$MSQ_Quest02_Data", "MQ101"},
		/*03*/ {"MSQ_Quest03_Key", "$MSQ_Quest03_Name", MAIN_QUEST_FLAG, "$MSQ_Quest03_Data", "MQ102"},
		/*04*/ {"MSQ_Quest04_Key", "$MSQ_Quest04_Name", MAIN_QUEST_FLAG, "$MSQ_Quest04_Data", "MQ103"},
		/*05*/ {"MSQ_Quest05_Key", "$MSQ_Quest05_Name", MAIN_QUEST_FLAG, "$MSQ_Quest05_Data", "MQ104"},
		/*06*/ {"MSQ_Quest06_Key", "$MSQ_Quest06_Name", MAIN_QUEST_FLAG, "$MSQ_Quest06_Data", "MQ105"},
		/*07*/ {"MSQ_Quest07_Key", "$MSQ_Quest07_Name", MAIN_QUEST_FLAG, "$MSQ_Quest07_Data", "MQ105Ustengrav"},
		/*08*/ {"MSQ_Quest08_Key", "$MSQ_Quest08_Name", MAIN_QUEST_FLAG, "$MSQ_Quest08_Data", "MQ106"},
		/*09*/ {"MSQ_Quest09_Key", "$MSQ_Quest09_Name", MAIN_QUEST_FLAG, "$MSQ_Quest09_Data", "MQ201"},
		/*10*/ {"MSQ_Quest10_Key", "$MSQ_Quest10_Name", MAIN_QUEST_FLAG, "$MSQ_Quest10_Data", "MQ202"},
		/*11*/ {"MSQ_Quest11_Key", "$MSQ_Quest11_Name", MAIN_QUEST_FLAG, "$MSQ_Quest11_Data", "MQ203"},
		/*12*/ {"MSQ_Quest12_Key", "$MSQ_Quest12_Name", MAIN_QUEST_FLAG, "$MSQ_Quest12_Data", "MQ204"},
		/*13*/ {"MSQ_Quest13_Key", "$MSQ_Quest13_Name", MAIN_QUEST_FLAG, "$MSQ_Quest13_Data", "MQ205"},
		/*14*/ {"MSQ_Quest14_Key", "$MSQ_Quest14_Name", MAIN_QUEST_FLAG, "$MSQ_Quest14_Data", "MQ206"},
		/*15*/ {"MSQ_Quest15_Key", "$MSQ_Quest15_Name", MAIN_QUEST_FLAG, "$MSQ_Quest15_Data", "MQ301"},
		/*16*/ {"MSQ_Quest16_Key", "$MSQ_Quest16_Name", MAIN_QUEST_FLAG, "$MSQ_Quest16_Data", "MQ302"},
		/*17*/ {"MSQ_Quest17_Key", "$MSQ_Quest17_Name", MAIN_QUEST_FLAG, "$MSQ_Quest17_Data", "MQPaarthurnax"},
		/*18*/ {"MSQ_Quest18_Key", "$MSQ_Quest18_Name", MAIN_QUEST_FLAG, "$MSQ_Quest18_Data", "MQ303"},
		/*19*/ {"MSQ_Quest19_Key", "$MSQ_Quest19_Name", MAIN_QUEST_FLAG, "$MSQ_Quest19_Data", "MQ304"},
		/*20*/ {"MSQ_Quest20_Key", "$MSQ_Quest20_Name", MAIN_QUEST_FLAG, "$MSQ_Quest20_Data", "MQ305"},
	};

	constexpr std::size_t StandardCompletion[] = { 
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
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
	}
}