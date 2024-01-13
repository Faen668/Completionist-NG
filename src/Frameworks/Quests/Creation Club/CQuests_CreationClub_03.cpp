#include "CQuests_CreationClub_03.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_CC3 
{
	CStageData StageData[] {
		{"CC03_Quest09", CStageEnum::kDone, 10},
		{"CC03_Quest15", CStageEnum::kDone, 25},
		{"CC03_Quest16", CStageEnum::kDone, 40},
		{"CC03_Quest19", CStageEnum::kDone, 100},
		{"CC03_Quest20", CStageEnum::kDone, 20},
		{"CC03_Quest20", CStageEnum::kDone, 20},
		{"CC03_Quest23", CStageEnum::kDone, 20},
		{"CC03_Quest26", CStageEnum::kDone, 171},
		{"CC03_Quest28", CStageEnum::kDone, 500, 1000},
		{"CC03_Quest33", CStageEnum::kDone, 20},
	};

	CQuestData QuestData[] {
		{"CC03_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_MiscQuest_AmberMadnessGear"},
		{"CC03_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_QuestA"},
		{"CC03_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE068_Quest"},
		{"CC03_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE069_Quest"},
		{"CC03_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE034_UnicornQuest"},
		{"CC03_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "ccEDHSSE002_Quest"},
		{"CC03_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE021_LordsMailQuest"},
		{"CC03_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_MiscQuest_GSDSGearBook"},
		{"CC03_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "ccEEJSSE004_Quest"},
		{"CC03_Quest09", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE034_WildHorsesQuest"},
		{"CC03_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE006_Quest"},
		{"CC03_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "ccEDHSSE003_Quest"},
		{"CC03_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE008_Quest"},
		{"CC03_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE041_Quest"},
		{"CC03_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_ElytraPetAcquisition_Dementia"},
		{"CC03_Quest15", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE025_ElytraPetAcquisition_Mania"},
		{"CC03_Quest16", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE035_PetAcquireQuest"},
		{"CC03_Quest17", CFlagEnum::kMain, CCompEnum::kStand, "ccEEJSSE002_MageTowerQuest"},
		{"CC03_Quest18", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_MiscQuest_Nerveshatter"},
		{"CC03_Quest19", CFlagEnum::kMain, CCompEnum::kStage, "ccEDHSSE001_CraftsmanEncounterQuest"},
		{"CC03_Quest20", CFlagEnum::kMain, CCompEnum::kStage, "ccVSVSSE002_MainQuest"},
		{"CC03_Quest21", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE019_StaffOfSheogorathQuest"},
		{"CC03_Quest22", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_QuestB"},
		{"CC03_Quest23", CFlagEnum::kMain, CCompEnum::kStage, "ccVSVSSE001_MainQuest"},
		{"CC03_Quest24", CFlagEnum::kMain, CCompEnum::kStand, "ccEEJSSE003_HouseQuest"},
		{"CC03_Quest25", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE025_StaadaQuest"},
		{"CC03_Quest26", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE067_Quest"},
		{"CC03_Quest27", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE067_Quest2"},
		{"CC03_Quest28", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE004_Quest"},
		{"CC03_Quest29", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE020_Quest"},
		{"CC03_Quest30", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE003_ZombieQuestStartEncounter"},
		{"CC03_Quest31", CFlagEnum::kMain, CCompEnum::kStand, "ccAARSSE001ManufactoryControlQuest"},
		{"CC03_Quest32", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE045_Quest"},
		{"CC03_Quest33", CFlagEnum::kMain, CCompEnum::kStage, "BGSSSE018_ShadowrendQuest"},
		{"CC03_Quest34", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE016_UmbraMainQuest"},
		{"CC03_Quest35", CFlagEnum::kMain, CCompEnum::kStand, "ccMTYSSE002_Quest"},
	};
	std::array<int, 4> RequiemExclusions = { 5,11,13,35 };
	std::array<int, 9> SkyExtCExclusions = { 0,1,7,14,15,18,22,25 };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		auto RequiemInstalled = Serialization::CompletionistData::IsModInstalled("Requiem - Creation Club.esp");
		auto SkyExtCInstalled = Serialization::CompletionistData::IsModInstalled("Skyrim Extended Cut - Saints and Seducers.esp");

		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			// Handle Exclusions for 'Requiem'
			if (RequiemInstalled && std::find(RequiemExclusions.begin(), RequiemExclusions.end(), i) != RequiemExclusions.end())
			{
				INFO("Excluded quest {} Due To 'Requiem'", QuestData[i].unique_identifier)
					continue;
			}

			// Handle Exclusions for 'Skyrim Extended Cut - Saints and Seducers'
			if (SkyExtCInstalled && std::find(SkyExtCExclusions.begin(), SkyExtCExclusions.end(), i) != SkyExtCExclusions.end())
			{
				INFO("Excluded quest {} Due To 'Skyrim Extended Cut'", QuestData[i].unique_identifier);
				continue;
			}

			auto quest = static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(QuestData[i].editor_id));
			if (quest)
			{
				QuestData[i].init()->initStageData(StageData)->finalize();
				CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 6, QuestData[i].unique_identifier));
				QuestsInstalled++;
			}
		}
	};
};