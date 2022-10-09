#include "CQuests_Solitude.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Solitude {
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
	2,5,7,9,10,11,12,14
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{4, "Completionist_Favor109Sybille", 1},
	{15, "Completionist_Favor013OctieveSan", 1},
	{16, "Completionist_Favor205Noster", 1},
	{17, "Completionist_Favor104Ahtar", 1},
	{18, "Completionist_FavorBeggarDervenin", -4},
	{19, "Completionist_FavorBeggarNoster", -4},
	{20, "Completionist_FavorBeggarSvari", -4},
	{21, "Completionist_Favor110Aldis", 1},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{22, "HaafingarImpGetOutofJail", "HaafingarSonsGetOutofJail"},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 199 },
	{ 1, 199 },
	{ 3, 199 },
	{ 6, 199 },
	{ 8, 199 },
	{ 13, 199 },
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

		/*00*/ AddQuest("Solitude_Quest00_Key", "$Solitude_Quest00_Name", SIDE_QUEST_FLAG, "$Solitude_Quest00_Data", "SolitudeFreeform04");
		/*01*/ AddQuest("Solitude_Quest01_Key", "$Solitude_Quest01_Name", SIDE_QUEST_FLAG, "$Solitude_Quest01_Data", "SolitudeFreeform07");
		/*02*/ AddQuest("Solitude_Quest02_Key", "$Solitude_Quest02_Name", SIDE_QUEST_FLAG, "$Solitude_Quest02_Data", "BardsCollegeLute");
		/*03*/ AddQuest("Solitude_Quest03_Key", "$Solitude_Quest03_Name", SIDE_QUEST_FLAG, "$Solitude_Quest03_Data", "SolitudeFreeform02");
		/*04*/ AddQuest("Solitude_Quest04_Key", "$Solitude_Quest04_Name", SIDE_QUEST_FLAG, "$Solitude_Quest04_Data", "Favor109");
		/*05*/ AddQuest("Solitude_Quest05_Key", "$Solitude_Quest05_Name", SIDE_QUEST_FLAG, "$Solitude_Quest05_Data", "MS07");
		/*06*/ AddQuest("Solitude_Quest06_Key", "$Solitude_Quest06_Name", SIDE_QUEST_FLAG, "$Solitude_Quest06_Data", "SolitudeFreeform01");
		/*07*/ AddQuest("Solitude_Quest07_Key", "$Solitude_Quest07_Name", SIDE_QUEST_FLAG, "$Solitude_Quest07_Data", "BardsCollegeFlute");
		/*08*/ AddQuest("Solitude_Quest08_Key", "$Solitude_Quest08_Name", SIDE_QUEST_FLAG, "$Solitude_Quest08_Data", "SolitudeFreeform03");
		/*09*/ AddQuest("Solitude_Quest09_Key", "$Solitude_Quest09_Name", SIDE_QUEST_FLAG, "$Solitude_Quest09_Data", "BardsCollegeDrum");
		/*10*/ AddQuest("Solitude_Quest10_Key", "$Solitude_Quest10_Name", SIDE_QUEST_FLAG, "$Solitude_Quest10_Data", "MS05");
		/*11*/ AddQuest("Solitude_Quest11_Key", "$Solitude_Quest11_Name", SIDE_QUEST_FLAG, "$Solitude_Quest11_Data", "MS06Start");
		/*12*/ AddQuest("Solitude_Quest12_Key", "$Solitude_Quest12_Name", SIDE_QUEST_FLAG, "$Solitude_Quest12_Data", "DA15");
		/*13*/ AddQuest("Solitude_Quest13_Key", "$Solitude_Quest13_Name", SIDE_QUEST_FLAG, "$Solitude_Quest13_Data", "SolitudeFreeform06");
		/*14*/ AddQuest("Solitude_Quest14_Key", "$Solitude_Quest14_Name", SIDE_QUEST_FLAG, "$Solitude_Quest14_Data", "MS06");
		/*15*/ AddQuest("Solitude_Quest15_Key", "$Solitude_Quest15_Name", SIDE_QUEST_FLAG, "$Solitude_Quest15_Data", "Favor013");
		/*16*/ AddQuest("Solitude_Quest16_Key", "$Solitude_Quest16_Name", SIDE_QUEST_FLAG, "$Solitude_Quest16_Data", "Favor205");
		/*17*/ AddQuest("Solitude_Quest17_Key", "$Solitude_Quest17_Name", SIDE_QUEST_FLAG, "$Solitude_Quest17_Data", "Favor104");
		/*18*/ AddQuest("Solitude_Quest18_Key", "$Solitude_Quest18_Name", RADI_QUEST_FLAG, "$Solitude_Quest18_Data", "FavorJobsBeggars");
		/*19*/ AddQuest("Solitude_Quest19_Key", "$Solitude_Quest19_Name", RADI_QUEST_FLAG, "$Solitude_Quest19_Data", "FavorJobsBeggars");
		/*20*/ AddQuest("Solitude_Quest20_Key", "$Solitude_Quest20_Name", RADI_QUEST_FLAG, "$Solitude_Quest20_Data", "FavorJobsBeggars");
		/*21*/ AddQuest("Solitude_Quest21_Key", "$Solitude_Quest21_Name", SIDE_QUEST_FLAG, "$Solitude_Quest21_Data", "Favor110");
		/*22*/ AddQuest("Solitude_Quest22_Key", "$Solitude_Quest22_Name", SIDE_QUEST_FLAG, "$Solitude_Quest22_Data", "FavorJarlsMakeFriends");

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

		for (auto& [i, stage] : StagePastCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_P(KeysArray[i], IdenArray[i], stage);
		}

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
}