#include "CQuests_Morthal.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Morthal {
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
	2,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{3, "Completionist_Favor_ChopWoodHroggar", -4},
	{4, "Completionist_Favor_ChopWoodHroggar", -4},
	{5, "Completionist_Favor001Idgrod", 1},
	{6, "Completionist_Favor017Benor", 1},
	{7, "Completionist_Favor110Lami", 1},
	};

	constexpr std::pair<std::size_t, std::uint16_t> StageDoneCompletion[] = {
	{7,  40},
	{27, 30},
	{31, 100},
	{33, 30},
	{39, 1000},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 20 },
	{ 1, 10 },
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{8, "HjaalmarchImpGetOutofJail", "HjaalmarchSonsGetOutofJail"},
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

		/*00*/ AddQuest("Morthal_Quest00_Key", "$Morthal_Quest00_Name", SIDE_QUEST_FLAG, "$Morthal_Quest00_Data", "FreeformMorthalA");
		/*01*/ AddQuest("Morthal_Quest01_Key", "$Morthal_Quest01_Name", SIDE_QUEST_FLAG, "$Morthal_Quest01_Data", "FreeformMorthalB");
		/*02*/ AddQuest("Morthal_Quest02_Key", "$Morthal_Quest02_Name", SIDE_QUEST_FLAG, "$Morthal_Quest02_Data", "MS14");
		/*03*/ AddQuest("Morthal_Quest03_Key", "$Morthal_Quest03_Name", RADI_QUEST_FLAG, "$Morthal_Quest03_Data", "FavorJobsChopWood");
		/*04*/ AddQuest("Morthal_Quest04_Key", "$Morthal_Quest04_Name", RADI_QUEST_FLAG, "$Morthal_Quest04_Data", "FavorJobsChopWood");
		/*05*/ AddQuest("Morthal_Quest05_Key", "$Morthal_Quest05_Name", SIDE_QUEST_FLAG, "$Morthal_Quest05_Data", "Favor001");
		/*06*/ AddQuest("Morthal_Quest06_Key", "$Morthal_Quest06_Name", SIDE_QUEST_FLAG, "$Morthal_Quest06_Data", "Favor017");
		/*07*/ AddQuest("Morthal_Quest07_Key", "$Morthal_Quest07_Name", SIDE_QUEST_FLAG, "$Morthal_Quest07_Data", "Favor110");
		/*08*/ AddQuest("Morthal_Quest08_Key", "$Morthal_Quest08_Name", SIDE_QUEST_FLAG, "$Morthal_Quest08_Data", "FavorJarlsMakeFriends");

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

		for (auto& [i, global, value] : GlobalCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_G(KeysArray[i], IdenArray[i], global, value);
		}

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
} 