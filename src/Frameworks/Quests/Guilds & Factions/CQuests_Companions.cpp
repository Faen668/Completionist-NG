#include "CQuests_Companions.hpp"
#include "Serialization.hpp"
#include "Frameworks/FrameworkMaster.hpp"

namespace CQFramework_Companions {
	using namespace CFramework_Master;
	using namespace Serialization;

	constexpr std::tuple<const char*, const char*, std::int32_t, const char*, const char*> QuestData[] = {
	/*00*/ {"Companions_Quest00_Key", "$Companions_Quest00_Name", MAIN_QUEST_FLAG, "$Companions_Quest00_Data", "C00"},
	/*01*/ {"Companions_Quest01_Key", "$Companions_Quest01_Name", MAIN_QUEST_FLAG, "$Companions_Quest01_Data", "C01"},
	/*02*/ {"Companions_Quest02_Key", "$Companions_Quest02_Name", MAIN_QUEST_FLAG, "$Companions_Quest02_Data", "C03"},
	/*03*/ {"Companions_Quest03_Key", "$Companions_Quest03_Name", MAIN_QUEST_FLAG, "$Companions_Quest03_Data", "C04"},
	/*04*/ {"Companions_Quest04_Key", "$Companions_Quest04_Name", MAIN_QUEST_FLAG, "$Companions_Quest04_Data", "C05"},
	/*05*/ {"Companions_Quest05_Key", "$Companions_Quest05_Name", MAIN_QUEST_FLAG, "$Companions_Quest05_Data", "C06"},
	/*06*/ {"Companions_Quest06_Key", "$Companions_Quest06_Name", RADI_QUEST_FLAG, "$Companions_Quest06_Data", "CR01"},
	/*07*/ {"Companions_Quest07_Key", "$Companions_Quest07_Name", RADI_QUEST_FLAG, "$Companions_Quest07_Data", "CR02"},
	/*08*/ {"Companions_Quest08_Key", "$Companions_Quest08_Name", RADI_QUEST_FLAG, "$Companions_Quest08_Data", "CR03"},
	/*09*/ {"Companions_Quest09_Key", "$Companions_Quest09_Name", RADI_QUEST_FLAG, "$Companions_Quest09_Data", "CR14"},
	/*10*/ {"Companions_Quest10_Key", "$Companions_Quest10_Name", RADI_QUEST_FLAG, "$Companions_Quest10_Data", "CR07"},
	/*11*/ {"Companions_Quest11_Key", "$Companions_Quest11_Name", RADI_QUEST_FLAG, "$Companions_Quest11_Data", "CR06"},
	/*12*/ {"Companions_Quest12_Key", "$Companions_Quest12_Name", RADI_QUEST_FLAG, "$Companions_Quest12_Data", "CR04"},
	/*13*/ {"Companions_Quest13_Key", "$Companions_Quest13_Name", RADI_QUEST_FLAG, "$Companions_Quest13_Data", "CR13"},
	/*14*/ {"Companions_Quest14_Key", "$Companions_Quest14_Name", RADI_QUEST_FLAG, "$Companions_Quest14_Data", "CR13"},
	/*15*/ {"Companions_Quest15_Key", "$Companions_Quest15_Name", RADI_QUEST_FLAG, "$Companions_Quest15_Data", "CR08"},
	/*16*/ {"Companions_Quest16_Key", "$Companions_Quest16_Name", RADI_QUEST_FLAG, "$Companions_Quest16_Data", "CR11"},
	/*17*/ {"Companions_Quest17_Key", "$Companions_Quest17_Name", RADI_QUEST_FLAG, "$Companions_Quest17_Data", "CR10"},
	/*18*/ {"Companions_Quest18_Key", "$Companions_Quest18_Name", RADI_QUEST_FLAG, "$Companions_Quest18_Data", "CR09"},
	/*19*/ {"Companions_Quest19_Key", "$Companions_Quest19_Name", RADI_QUEST_FLAG, "$Companions_Quest19_Data", "CR12"},
	/*20*/ {"Companions_Quest20_Key", "$Companions_Quest20_Name", RADI_QUEST_FLAG, "$Companions_Quest20_Data", "CR05"},
	};

	constexpr std::size_t StandardCompletion[] = {
	0,1,2,3,4,5
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> GlobalCompletion[] = {
	{6, "Completionist_Companions_CR01", RADIANT_COMPANIONS_VALUE},
	{7, "Completionist_Companions_CR02", RADIANT_COMPANIONS_VALUE},
	{8, "Completionist_Companions_CR03", RADIANT_COMPANIONS_VALUE},
	{9, "Completionist_Companions_CR14", RADIANT_COMPANIONS_VALUE},
	{10, "Completionist_Companions_CR07", RADIANT_COMPANIONS_VALUE},
	{11, "Completionist_Companions_CR06", RADIANT_COMPANIONS_VALUE},
	{12, "Completionist_Companions_CR04", RADIANT_COMPANIONS_VALUE},
	{13, "Completionist_Companions_CR13Farkas", 1},
	{14, "Completionist_Companions_CR13Vilkas", 1},
	{15, "Completionist_Companions_CR08", RADIANT_COMPANIONS_VALUE},
	{16, "Completionist_Companions_CR11", 1},
	{17, "Completionist_Companions_CR10", 1},
	{18, "Completionist_Companions_CR09", 1},
	{19, "Completionist_Companions_CR12", 3},
	{20, "Completionist_Companions_CR05", RADIANT_COMPANIONS_VALUE},
	};

	constexpr CompanionsQuestData CQD[] = {
	{ 0x01CEEE, "Skyrim.esm", "Completionist_Companions_CR01", "", 1, 200 },
	{ 0x01CEEE, "Skyrim.esm", "Completionist_Companions_CR01", "", 1, 200 },
	{ 0x025185, "Skyrim.esm", "Completionist_Companions_CR02", "", 1, 200 },
	{ 0x025230, "Skyrim.esm", "Completionist_Companions_CR03", "", 1, 200 },
	{ 0x025231, "Skyrim.esm", "Completionist_Companions_CR04", "", 1, 100 },
	{ 0x02522F, "Skyrim.esm", "Completionist_Companions_CR05", "", 1, 200 },
	{ 0x0C18E1, "Skyrim.esm", "Completionist_Companions_CR06", "", 1, 200 },
	{ 0x025250, "Skyrim.esm", "Completionist_Companions_CR07", "", 1, 200 },
	{ 0x025251, "Skyrim.esm", "Completionist_Companions_CR08", "", 1, 100 },
	{ 0x025252, "Skyrim.esm", "Completionist_Companions_CR09", "", 1, 200 },
	{ 0x09D6FC, "Skyrim.esm", "Completionist_Companions_CR10", "", 1, 200 },
	{ 0x09D700, "Skyrim.esm", "Completionist_Companions_CR11", "", 1, 200 },
	{ 0x0E3145, "Skyrim.esm", "Completionist_Companions_CR12", "", 1, 200 },
	{ 0x0E3163, "Skyrim.esm", "Completionist_Companions_CR13Farkas", "Completionist_Companions_CR13Vilkas", 1, 200 },
	{ 0x0E3156, "Skyrim.esm", "Completionist_Companions_CR14", "", 1, 200 },
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

		auto EventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		EventHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(CHandler::GetSingleton()));
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
	//-- Framework Events ( On Stage Change ) -----------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* event = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!event) { return EventResult::kContinue; }

		for (auto& dataSet : CQD) {

			const auto* questBase = CompletionistData::GetFullForm<RE::TESQuest>(dataSet.QuestID, dataSet.QuestFN);
			auto* questGlob1 = RE::TESForm::LookupByEditorID<RE::TESGlobal>(dataSet.Global1);
			auto* questGlob2 = RE::TESForm::LookupByEditorID<RE::TESGlobal>(dataSet.Global2);

			if (!questBase || questBase != event || a_event->stage != dataSet.Stage) {
				continue;
			}

			INFO("Got Quest - [{}]", questBase->GetName());

			if (dataSet.Global1 == "Completionist_Companions_CR13Farkas") {
				
				auto* FarkasRef = CompletionistData::GetFullForm<RE::Actor>(0x01A693, dataSet.QuestFN);
				auto* VilkasRef = CompletionistData::GetFullForm<RE::Actor>(0x01A695, dataSet.QuestFN);

				if (!FarkasRef || !VilkasRef || !questGlob1 || !questGlob2) {
					continue;
				}

				for (const auto& alias : questBase->aliases) {
					if (alias && alias->aliasName == "Questgiver") {

						auto* reference = static_cast<RE::BGSRefAlias*>(alias);
						if (reference && reference->GetActorReference() == FarkasRef) {
							questGlob1->value += dataSet.Value;
							INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", dataSet.Global1, questBase->GetName(), FarkasRef->GetName());
							return EventResult::kContinue;
						}

						if (reference && reference->GetActorReference() == VilkasRef) {
							questGlob2->value += dataSet.Value;
							INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", dataSet.Global2, questBase->GetName(), VilkasRef->GetName());
							return EventResult::kContinue;
						}
					}
				}
			}
			else {
				if (questGlob1) {
					questGlob1->value += dataSet.Value;
					INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}]", dataSet.Global1, questBase->GetName());
					return EventResult::kContinue;
				}

			}
		}
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