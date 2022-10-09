#include "CQuests_MainStory_DG.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_DG {
	using namespace Serialization;
	using namespace CFramework_Master;

	// Main Quests
	inline std::vector<RE::TESForm*>	FormArray;
	inline std::vector<std::string>		NameArray;
	inline std::vector<std::string>		IdenArray;
	inline std::vector<std::string>		TextArray;
	inline std::vector<std::int32_t>	RadiArray;
	inline std::vector<bool>			BoolArray;
	inline std::vector<std::string>		KeysArray;

	std::int32_t ArraySize = 128;
	std::int32_t MAIN_QUEST_FLAG = 0;
	std::int32_t SIDE_QUEST_FLAG = 1;
	std::int32_t RADI_QUEST_FLAG = 2;

	constexpr std::size_t StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13
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

		/*00*/ AddQuest("Main_DG_Quest00_Key", "$Main_DG_Quest00_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest00_Data", "DLC1VQ01MiscObjective");
		/*01*/ AddQuest("Main_DG_Quest01_Key", "$Main_DG_Quest01_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest01_Data", "DLC1VQ01");
		/*02*/ AddQuest("Main_DG_Quest02_Key", "$Main_DG_Quest02_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest02_Data", "DLC1VQ02");
		/*03*/ AddQuest("Main_DG_Quest03_Key", "$Main_DG_Quest03_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest03_Data", "DLC1HunterBaseIntro");
		/*04*/ AddQuest("Main_DG_Quest04_Key", "$Main_DG_Quest04_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest04_Data", "DLC1VQ03Hunter");
		/*05*/ AddQuest("Main_DG_Quest05_Key", "$Main_DG_Quest05_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest05_Data", "DLC1VampireBaseIntro");
		/*06*/ AddQuest("Main_DG_Quest06_Key", "$Main_DG_Quest06_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest06_Data", "DLC1VQ03Vampire");
		/*07*/ AddQuest("Main_DG_Quest07_Key", "$Main_DG_Quest07_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest07_Data", "DLC1VQ04");
		/*08*/ AddQuest("Main_DG_Quest08_Key", "$Main_DG_Quest08_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest08_Data", "DLC1VQElderHandler");
		/*09*/ AddQuest("Main_DG_Quest09_Key", "$Main_DG_Quest09_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest09_Data", "DLC1VQ05");
		/*10*/ AddQuest("Main_DG_Quest10_Key", "$Main_DG_Quest10_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest10_Data", "DLC1VQElder");
		/*11*/ AddQuest("Main_DG_Quest11_Key", "$Main_DG_Quest11_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest11_Data", "DLC1VQ06");
		/*12*/ AddQuest("Main_DG_Quest12_Key", "$Main_DG_Quest12_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest12_Data", "DLC1VQ07");
		/*13*/ AddQuest("Main_DG_Quest13_Key", "$Main_DG_Quest13_Name", MAIN_QUEST_FLAG, "$Main_DG_Quest13_Data", "DLC1VQ08");

		assert(KeysArray.size() == ArraySize);
		assert(IdenArray.size() == ArraySize);
		assert(NameArray.size() == ArraySize);
		assert(RadiArray.size() == ArraySize);
		assert(TextArray.size() == ArraySize);
		BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Add Quest To Array ) -----
	//---------------------------------------------------

	void CHandler::AddQuest(std::string k, std::string n, std::int32_t t, std::string h, std::string g) {

		KeysArray.push_back(k);
		NameArray.push_back(n);
		RadiArray.push_back(t);
		TextArray.push_back(h);
		IdenArray.push_back(g);
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