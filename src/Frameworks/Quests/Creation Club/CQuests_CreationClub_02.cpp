#include "CQuests_CreationClub_02.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_CC2 
{
	CStageData StageData[] {
		{"CC02_Quest07", CStageEnum::kDone, 40		, 0 },
		{"CC02_Quest27", CStageEnum::kDone, 30		, 0 },
		{"CC02_Quest31", CStageEnum::kDone, 100		, 0 },
		{"CC02_Quest33", CStageEnum::kDone, 30		, 0 },
		{"CC02_Quest39", CStageEnum::kDone, 1000	, 0 },
	};

	CQuestData QuestData[] {
		{"CC02_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "ccKRTSSE001_zQuest"},
		{"CC02_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE005_MainQuest"},
		{"CC02_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE013_AquisitionQuest"},
		{"CC02_Quest03", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE057_Quest"},
		{"CC02_Quest04", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_QuestD"},
		{"CC02_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "ccFFBSSE001_Quest"},
		{"CC02_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE050_Quest"},
		{"CC02_Quest07", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE013_BloodthirstQuest"},
		{"CC02_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE040_Quest"},
		{"CC02_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE059_Quest"},
		{"CC02_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE052_Quest"},
		{"CC02_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_QuestA"},
		{"CC02_Quest12", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_QuestC"},
		{"CC02_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE053_Quest"},
		{"CC02_Quest14", CFlagEnum::kMain, CCompEnum::kStand, "ccRMSSSE001_Quest"},
		{"CC02_Quest15", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE062_Quest"},
		{"CC02_Quest16", CFlagEnum::kMain, CCompEnum::kStand, "DLCDwarvenPuzzleDungeonQuest01"},
		{"CC02_Quest17", CFlagEnum::kMain, CCompEnum::kStand, "ccKRTSSE001_QuestFortune"},
		{"CC02_Quest18", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE055_Quest"},
		{"CC02_Quest19", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_Quest"},
		{"CC02_Quest20", CFlagEnum::kMain, CCompEnum::kStand, "ccEEJSSE005_Quest"},
		{"CC02_Quest21", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE063_Quest"},
		{"CC02_Quest22", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_QuestB"},
		{"CC02_Quest23", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE038_Quest"},
		{"CC02_Quest24", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE036_PetBWolfAcquisitionQuest"},
		{"CC02_Quest25", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE061_Quest"},
		{"CC02_Quest26", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE051_Quest"},
		{"CC02_Quest27", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE010_PetPurchaseQuest"},
		{"CC02_Quest28", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE043_VampireHunterQuest"},
		{"CC02_Quest29", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE064_Quest"},
		{"CC02_Quest30", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE058_Quest"},
		{"CC02_Quest31", CFlagEnum::kMain, CCompEnum::kStage, "ccMTYSSE001_StartupQuest_Alt"},
		{"CC02_Quest32", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE054_Quest"},
		{"CC02_Quest33", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE002_SoulStealerQuest"},
		{"CC02_Quest34", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE002_TelekinesisArrowQuest"},
		{"CC02_Quest35", CFlagEnum::kMain, CCompEnum::kStand, "ccPEWSSE002_Quest"},
		{"CC02_Quest36", CFlagEnum::kMain, CCompEnum::kStand, "DLCDwarvenPuzzleDungeonCrownQuest"},
		{"CC02_Quest37", CFlagEnum::kMain, CCompEnum::kStand, "DLCDwarvenPuzzleDungeonHorseQuest"},
		{"CC02_Quest38", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE014_SpellPack_StartupQuest"},
		{"CC02_Quest39", CFlagEnum::kMain, CCompEnum::kStage, "ccBGSSSE007_Quest"},
		{"CC02_Quest40", CFlagEnum::kMain, CCompEnum::kStand, "ccMTYSSE001_Quest"},
		{"CC02_Quest41", CFlagEnum::kMain, CCompEnum::kStand, "ccKRTSSE001_QuestPower"},
		{"CC02_Quest42", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE031_QUEST"},
		{"CC02_Quest43", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE060_Quest"},
		{"CC02_Quest44", CFlagEnum::kMain, CCompEnum::kStand, "ccASVSSE001_QuestE"},
		{"CC02_Quest45", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE056_Quest"},
	};

	CArrayData QuestArrays{ &IdenArray, &NameArray, &TextArray, &BoolArray, &RadiArray };

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			QuestData[i].init()->initQuestData(&QuestArrays)->initStageData(StageData);
			CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 5));
		}
		BoolArray = std::vector<bool>(CArraySize, false);
	};
};