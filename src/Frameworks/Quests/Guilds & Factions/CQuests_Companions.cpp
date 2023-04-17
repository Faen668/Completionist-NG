#include "CQuests_Companions.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_Companions 
{
	CRadiantData RadiantData[] {
		/*06*/ {"Companions_Quest06",  CRadiantEnum::kRadiant_COM,		0x01CEEE, 0, 200, "Completionist_Companions_CR01" },
		/*07*/ {"Companions_Quest07",  CRadiantEnum::kRadiant_COM,		0x01CEEE, 0, 200, "Completionist_Companions_CR02" },
		/*08*/ {"Companions_Quest08",  CRadiantEnum::kRadiant_COM,		0x025185, 0, 200, "Completionist_Companions_CR03" },
		/*09*/ {"Companions_Quest09",  CRadiantEnum::kRadiant_COM,		0x025230, 0, 200, "Completionist_Companions_CR14" },
		/*10*/ {"Companions_Quest10",  CRadiantEnum::kRadiant_COM,		0x025231, 0, 200, "Completionist_Companions_CR07" },
		/*11*/ {"Companions_Quest11",  CRadiantEnum::kRadiant_COM,		0x02522F, 0, 200, "Completionist_Companions_CR06" },
		/*12*/ {"Companions_Quest12",  CRadiantEnum::kRadiant_COM,		0x0C18E1, 0, 200, "Completionist_Companions_CR04" },
		/*13*/ {"Companions_Quest13",  CRadiantEnum::kRadiant_DF1,		0,		  0, -1,  "Completionist_Companions_CR13Farkas" },
		/*14*/ {"Companions_Quest14",  CRadiantEnum::kRadiant_DF1,		0,		  0, -1,  "Completionist_Companions_CR13Vilkas" },
		/*15*/ {"Companions_Quest15",  CRadiantEnum::kRadiant_COM,		0x025252, 0, 200, "Completionist_Companions_CR08" },
		/*16*/ {"Companions_Quest16",  CRadiantEnum::kRadiant_DF1,		0x09D6FC, 0, 200, "Completionist_Companions_CR11" },
		/*17*/ {"Companions_Quest17",  CRadiantEnum::kRadiant_DF1,		0x09D700, 0, 200, "Completionist_Companions_CR10" },
		/*18*/ {"Companions_Quest18",  CRadiantEnum::kRadiant_DF1,		0x0E3145, 0, 200, "Completionist_Companions_CR09" },
		/*19*/ {"Companions_Quest19",  CRadiantEnum::kRadiant_DF3,	0x0E3163, 0, 200, "Completionist_Companions_CR12" },
		/*20*/ {"Companions_Quest20",  CRadiantEnum::kRadiant_COM,		0x0E3156, 0, 200, "Completionist_Companions_CR05" },
	};

	CQuestData QuestData[] {
		/*00*/ {"Companions_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "C00"},
		/*01*/ {"Companions_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "C01"},
		/*02*/ {"Companions_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "C03"},
		/*03*/ {"Companions_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "C04"},
		/*04*/ {"Companions_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "C05"},
		/*05*/ {"Companions_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "C06"},
		/*06*/ {"Companions_Quest06", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR01"},
		/*07*/ {"Companions_Quest07", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR02"},
		/*08*/ {"Companions_Quest08", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR03"},
		/*09*/ {"Companions_Quest09", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR14"},
		/*10*/ {"Companions_Quest10", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR07"},
		/*11*/ {"Companions_Quest11", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR06"},
		/*12*/ {"Companions_Quest12", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR04"},
		/*13*/ {"Companions_Quest13", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR13"},
		/*14*/ {"Companions_Quest14", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR13"},
		/*15*/ {"Companions_Quest15", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR08"},
		/*16*/ {"Companions_Quest16", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR11"},
		/*17*/ {"Companions_Quest17", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR10"},
		/*18*/ {"Companions_Quest18", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR09"},
		/*19*/ {"Companions_Quest19", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR12"},
		/*20*/ {"Companions_Quest20", CFlagEnum::kRadi, CCompEnum::kGlobl, "CR05"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	CompanionsQuestData GlobalSetter{ 0x0E3163, "Skyrim.esm", "Completionist_Companions_CR13Farkas", "Completionist_Companions_CR13Vilkas" };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			// Handle Exclusions for CRF
			if (!Serialization::CompletionistData::IsModInstalled("Cutting Room Floor.esp")) {
				if (i == 8) { INFO("Excluded quest - {} due to CRF being missing", QuestData[i].editor_id); continue; }
			}

			// Handle Exclusions for 'Dont be a milk drinker'
			if (i == 12 && (Serialization::CompletionistData::IsModInstalled("Companions - Don't be a Milk Drinker.esp") || Serialization::CompletionistData::IsModInstalled("Companions - Don't be a Milk Drinker (Farkas only option).esp"))) {
				INFO("Excluded quest - {} due to 'Don't be a milk drinker' being installed", QuestData[i].editor_id); continue;
			}

			QuestData[i].init()
				->initQuestData(&QuestArrays)
				->initRadiantData(RadiantData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 22));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};

	//---------------------------------------------------
	//-- Framework Events ( On Radiant Stage Set ) ------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage || a_event->stage != 200) { 
			return RE::BSEventNotifyControl::kContinue; 
		}

		const auto* quest1 = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		const auto* quest2 = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESQuest>(GlobalSetter.QuestID, GlobalSetter.QuestFN);
		
		if (!quest1 || !quest2 || quest1->GetFormID() != quest2->GetFormID()) { 
			return EventResult::kContinue; 
		}

		auto* farkasGlobal = RE::TESForm::LookupByEditorID<RE::TESGlobal>(GlobalSetter.Global1);
		auto* vilkasGlobal = RE::TESForm::LookupByEditorID<RE::TESGlobal>(GlobalSetter.Global2);

		INFO("Companions global setter got quest - [{}]", quest1->GetName());

		auto* FarkasRef = RE::TESDataHandler::GetSingleton()->LookupForm<RE::Actor>(0x01A693, GlobalSetter.QuestFN);
		auto* VilkasRef = RE::TESDataHandler::GetSingleton()->LookupForm<RE::Actor>(0x01A695, GlobalSetter.QuestFN);

		if (!FarkasRef || !VilkasRef || !farkasGlobal || !vilkasGlobal) {
			return EventResult::kContinue;
		}

		for (const auto& alias : quest1->aliases) {
			if (alias && alias->aliasName == "Questgiver") {

				auto* reference = static_cast<RE::BGSRefAlias*>(alias);
				if (reference && reference->GetActorReference()->formID == FarkasRef->formID) {
					farkasGlobal->value++;
					INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", GlobalSetter.Global1, quest1->GetName(), FarkasRef->GetName());
					return EventResult::kContinue;
				}

				if (reference && reference->GetActorReference()->formID == VilkasRef->formID) {
					vilkasGlobal->value++;
					INFO("Incrememnting Completion Count On Global - [{}] For Quest - [{}] With Actor - [{}]", GlobalSetter.Global2, quest1->GetName(), VilkasRef->GetName());
					return EventResult::kContinue;
				}
			}
		}
		return EventResult::kContinue;
	}
}