#include "CQuests_Riften.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Riften {
	using namespace CFramework_Master;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Riften_Quest00_Key", "$Riften_Quest00_Name", SIDE_QUEST_FLAG, "$Riften_Quest00_Data", "FreeformRiften19"},
	/*01*/ {"Riften_Quest01_Key", "$Riften_Quest01_Name", SIDE_QUEST_FLAG, "$Riften_Quest01_Data", "FreeformRiften16"},
	/*02*/ {"Riften_Quest02_Key", "$Riften_Quest02_Name", SIDE_QUEST_FLAG, "$Riften_Quest02_Data", "FreeformRiften11"},
	/*03*/ {"Riften_Quest03_Key", "$Riften_Quest03_Name", SIDE_QUEST_FLAG, "$Riften_Quest03_Data", "FreeformRiften08"},
	/*04*/ {"Riften_Quest04_Key", "$Riften_Quest04_Name", SIDE_QUEST_FLAG, "$Riften_Quest04_Data", "FreeformRiften04"},
	/*05*/ {"Riften_Quest05_Key", "$Riften_Quest05_Name", SIDE_QUEST_FLAG, "$Riften_Quest05_Data", "FreeformRiften09"},
	/*06*/ {"Riften_Quest06_Key", "$Riften_Quest06_Name", SIDE_QUEST_FLAG, "$Riften_Quest06_Data", "FreeformRiften12"},
	/*07*/ {"Riften_Quest07_Key", "$Riften_Quest07_Name", SIDE_QUEST_FLAG, "$Riften_Quest07_Data", "FreeformRiften14"},
	/*08*/ {"Riften_Quest08_Key", "$Riften_Quest08_Name", SIDE_QUEST_FLAG, "$Riften_Quest08_Data", "FreeformRiften07"},
	/*09*/ {"Riften_Quest09_Key", "$Riften_Quest09_Name", SIDE_QUEST_FLAG, "$Riften_Quest09_Data", "FreeformRiften13"},
	/*10*/ {"Riften_Quest10_Key", "$Riften_Quest10_Name", SIDE_QUEST_FLAG, "$Riften_Quest10_Data", "MS03"},
	/*11*/ {"Riften_Quest11_Key", "$Riften_Quest11_Name", SIDE_QUEST_FLAG, "$Riften_Quest11_Data", "FreeformRiften18"},
	/*12*/ {"Riften_Quest12_Key", "$Riften_Quest12_Name", SIDE_QUEST_FLAG, "$Riften_Quest12_Data", "FreeformRiften06"},
	/*13*/ {"Riften_Quest13_Key", "$Riften_Quest13_Name", SIDE_QUEST_FLAG, "$Riften_Quest13_Data", "FreeformRiften15"},
	/*14*/ {"Riften_Quest14_Key", "$Riften_Quest14_Name", SIDE_QUEST_FLAG, "$Riften_Quest14_Data", "FreeformRiften05"},
	/*15*/ {"Riften_Quest15_Key", "$Riften_Quest15_Name", SIDE_QUEST_FLAG, "$Riften_Quest15_Data", "FreeformRiften22"},
	/*16*/ {"Riften_Quest16_Key", "$Riften_Quest16_Name", SIDE_QUEST_FLAG, "$Riften_Quest16_Data", "FreeformRiften10"},
	/*17*/ {"Riften_Quest17_Key", "$Riften_Quest17_Name", SIDE_QUEST_FLAG, "$Riften_Quest17_Data", "FreeformRiften01"},
	/*18*/ {"Riften_Quest18_Key", "$Riften_Quest18_Name", SIDE_QUEST_FLAG, "$Riften_Quest18_Data", "FavorJarlsMakeFriends"},
	/*19*/ {"Riften_Quest19_Key", "$Riften_Quest19_Name", SIDE_QUEST_FLAG, "$Riften_Quest19_Data", "RelationshipMarriage"},
	/*20*/ {"Riften_Quest20_Key", "$Riften_Quest20_Name", SIDE_QUEST_FLAG, "$Riften_Quest20_Data", "T02"},
	/*21*/ {"Riften_Quest21_Key", "$Riften_Quest21_Name", SIDE_QUEST_FLAG, "$Riften_Quest21_Data", "FreeformRiften02"},
	/*22*/ {"Riften_Quest22_Key", "$Riften_Quest22_Name", SIDE_QUEST_FLAG, "$Riften_Quest22_Data", "FreeformRiften20"},
	/*23*/ {"Riften_Quest23_Key", "$Riften_Quest23_Name", SIDE_QUEST_FLAG, "$Riften_Quest23_Data", "FreeformRiften17"},
	/*24*/ {"Riften_Quest24_Key", "$Riften_Quest24_Name", SIDE_QUEST_FLAG, "$Riften_Quest24_Data", "FreeformRiften03"},
	/*25*/ {"Riften_Quest25_Key", "$Riften_Quest25_Name", SIDE_QUEST_FLAG, "$Riften_Quest25_Data", "MS04"},
	/*26*/ {"Riften_Quest26_Key", "$Riften_Quest26_Name", SIDE_QUEST_FLAG, "$Riften_Quest26_Data", "FreeformValdDebt"},
	/*27*/ {"Riften_Quest27_Key", "$Riften_Quest27_Name", RADI_QUEST_FLAG, "$Riften_Quest27_Data", "FavorJobsBeggars"},
	/*28*/ {"Riften_Quest28_Key", "$Riften_Quest28_Name", RADI_QUEST_FLAG, "$Riften_Quest28_Data", "FavorJobsBeggars"},
	};

	constexpr std::size_t StandardCompletion[] = {
	10,17,20,25
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{27, "Completionist_FavorBeggarEdda", -4},
	{28, "Completionist_FavorBeggarSnilf", -4},
	};

	constexpr std::tuple<std::size_t, const char*, const char*> JarlCompletion[] = {
	{18, "RiftImpGetOutofJail", "RiftSonsGetOutofJail"},
	};

	constexpr std::pair<std::size_t, std::int32_t> StagePastCompletion[] = {
	{ 0, 199 },
	{ 1, 199 },
	{ 2, 199 },
	{ 3, 199 },
	{ 4, 199 },
	{ 5, 199 },
	{ 6, 199 },
	{ 7, 199 },
	{ 8, 199 },
	{ 9, 199 },
	{ 11, 199 },
	{ 12, 199 },
	{ 13, 199 },
	{ 14, 199 },
	{ 15, 199 },
	{ 16, 199 },
	{ 19, 99 },
	{ 21, 199 },
	{ 22, 199 },
	{ 23, 199 },
	{ 24, 199 },
	{ 26, 199 },
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

		for (auto& [i, imp, son] : JarlCompletion) {
			BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(KeysArray[i]) || FrameworkAPI::IsCompleted_J(KeysArray[i], imp, son);
		}
	};
}