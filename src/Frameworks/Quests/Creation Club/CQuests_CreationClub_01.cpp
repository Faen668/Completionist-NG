#include "CQuests_CreationClub_01.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CQFramework_CC1 
{
	CRadiantData RadiantData[]{
		{"CC01_Quest21", CRadiantEnum::kRadiant_Fsh, 0x000C2F,0,200},
		{"CC01_Quest22", CRadiantEnum::kRadiant_Fsh, 0x000C8C,0,200},
		{"CC01_Quest23", CRadiantEnum::kRadiant_Fsh, 0x000B98,0,200},
	};

	CQuestData QuestData[] {
		{"CC01_Quest00", CFlagEnum::kMain, CCompEnum::kStand, "ccVSVSSE004_MainQuest"},
		{"CC01_Quest01", CFlagEnum::kMain, CCompEnum::kStand, "ccVSVSSE004_RestoreFarmQuest"},
		{"CC01_Quest02", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Start_MQ2"},
		{"CC01_Quest03", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Dwarven"},
		{"CC01_Quest04", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Falkreath"},
		{"CC01_Quest05", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Fish_MQ1"},
		{"CC01_Quest06", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Fish_MQ2"},
		{"CC01_Quest07", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Crab_MQ2"},
		{"CC01_Quest08", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Fish_MQ4"},
		{"CC01_Quest09", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Crab_MQ3"},
		{"CC01_Quest10", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Fish_MQ5"},
		{"CC01_Quest11", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Fish_MQ3"},
		{"CC01_Quest12", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_WhiterunPartOne"},
		{"CC01_Quest13", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Crab_MQ1"},
		{"CC01_Quest14", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Morthal"},
		{"CC01_Quest15", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Markarth"},
		{"CC01_Quest16", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Khajiit"},
		{"CC01_Quest17", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Solitude"},
		{"CC01_Quest18", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_Windhelm"},
		{"CC01_Quest19", CFlagEnum::kSide, CCompEnum::kStand, "ccBGSSSE001_Misc_WhiterunPartTwo"},
		{"CC01_Quest20", CFlagEnum::kMain, CCompEnum::kStand, "ccBGSSSE001_Crab_MQ4"},
		{"CC01_Quest21", CFlagEnum::kRadi, CCompEnum::kGlobl, "ccBGSSSE001_Radiant_2"},
		{"CC01_Quest22", CFlagEnum::kRadi, CCompEnum::kGlobl, "ccBGSSSE001_Radiant_3"},
		{"CC01_Quest23", CFlagEnum::kRadi, CCompEnum::kGlobl, "ccBGSSSE001_Radiant_1"},
	};

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework()
	{
		for (auto i = 0; i < std::extent_v<decltype(QuestData)>; i++)
		{
			auto quest = static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(QuestData[i].editor_id));
			if (quest)
			{
				QuestData[i].init()->initRadiantData(RadiantData)->finalize();
				CQuestMaster::CQuestDataVec.push_back(std::make_tuple(&QuestData[i], QuestData[i].GetName(), 4, QuestData[i].unique_identifier));
				QuestsInstalled++;
			}
		}
	};
};