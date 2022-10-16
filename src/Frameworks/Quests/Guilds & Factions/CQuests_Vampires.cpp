#include "CQuests_Vampires.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Vampires {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Vampires_Quest00_Key", "$Vampires_Quest00_Name", SIDE_QUEST_FLAG, "$Vampires_Quest00_Data", "DLC1RV09"},
	/*01*/ {"Vampires_Quest01_Key", "$Vampires_Quest01_Name", SIDE_QUEST_FLAG, "$Vampires_Quest01_Data", "DLC1RV10"},
	/*02*/ {"Vampires_Quest02_Key", "$Vampires_Quest02_Name", SIDE_QUEST_FLAG, "$Vampires_Quest02_Data", "DLC1RV06"},
	/*03*/ {"Vampires_Quest03_Key", "$Vampires_Quest03_Name", SIDE_QUEST_FLAG, "$Vampires_Quest03_Data", "DLC1RV08"},
	/*04*/ {"Vampires_Quest04_Key", "$Vampires_Quest04_Name", RADI_QUEST_FLAG, "$Vampires_Quest04_Data", "DLC1RV05"},
	/*05*/ {"Vampires_Quest05_Key", "$Vampires_Quest05_Name", RADI_QUEST_FLAG, "$Vampires_Quest05_Data", "DLC1RV02"},
	/*06*/ {"Vampires_Quest06_Key", "$Vampires_Quest06_Name", RADI_QUEST_FLAG, "$Vampires_Quest06_Data", "DLC1RV03"},
	/*07*/ {"Vampires_Quest07_Key", "$Vampires_Quest07_Name", RADI_QUEST_FLAG, "$Vampires_Quest07_Data", "DLC1RV07"},
	/*08*/ {"Vampires_Quest08_Key", "$Vampires_Quest08_Name", RADI_QUEST_FLAG, "$Vampires_Quest08_Data", "DLC1RV04"},
	/*09*/ {"Vampires_Quest09_Key", "$Vampires_Quest09_Name", RADI_QUEST_FLAG, "$Vampires_Quest09_Data", "DLC1RV01"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{4, "Completionist_DLC1RV05", RADIANT_DAWNGUARD_VALUE},
	{5, "Completionist_DLC1RV02", RADIANT_DAWNGUARD_VALUE},
	{6, "Completionist_DLC1RV03", RADIANT_DAWNGUARD_VALUE},
	{7, "Completionist_DLC1RV07", RADIANT_DAWNGUARD_VALUE},
	{8, "Completionist_DLC1RV04", RADIANT_DAWNGUARD_VALUE},
	{9, "Completionist_DLC1RV01", RADIANT_DAWNGUARD_VALUE},
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