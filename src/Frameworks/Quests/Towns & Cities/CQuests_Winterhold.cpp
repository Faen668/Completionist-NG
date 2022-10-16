#include "CQuests_Winterhold.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Winterhold {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
		/*00*/ {"Winterhold_Quest00_Key", "$Winterhold_Quest00_Name", SIDE_QUEST_FLAG, "$Winterhold_Quest00_Data", "FreeformWinterholdC"},
		/*01*/ {"Winterhold_Quest01_Key", "$Winterhold_Quest01_Name", SIDE_QUEST_FLAG, "$Winterhold_Quest01_Data", "Favor013"},
		/*02*/ {"Winterhold_Quest02_Key", "$Winterhold_Quest02_Name", SIDE_QUEST_FLAG, "$Winterhold_Quest02_Data", "Favor158"},
		/*03*/ {"Winterhold_Quest03_Key", "$Winterhold_Quest03_Name", RADI_QUEST_FLAG, "$Winterhold_Quest03_Data", "FavorJobsDrunks"},
		/*04*/ {"Winterhold_Quest04_Key", "$Winterhold_Quest04_Name", SIDE_QUEST_FLAG, "$Winterhold_Quest04_Data", "Favor018"},
		/*05*/ {"Winterhold_Quest05_Key", "$Winterhold_Quest05_Name", SIDE_QUEST_FLAG, "$Winterhold_Quest05_Data", "FavorJarlsMakeFriends"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{1, "Completionist_Favor019Revyn", 1},
	{2, "Completionist_Favor013ScoutsManyMarshes", 1},
	{3, "Completionist_Favor018StandInShallows", -4},
	{4, "Completionist_Favor110Torbjorn", 1},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{5, "WinterholdImpGetOutofJail", "WinterholdSonsGetOutofJail"},
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
}