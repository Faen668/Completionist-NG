#include "Serialization.hpp"
#include "CFramework_VIG.hpp"
#include "Frameworks/FrameworkMaster.hpp"

#undef AddForm

namespace CPatch_VIG {
	using namespace CFramework_Master;

	// clang-format off

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest1_QuestData[] = {
		/*00*/ {"Vigilant_Main_Quest00_Key", "$Vigilant_Main_Quest00_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest00_Data", "zzzAoMMq00"},
		/*01*/ {"Vigilant_Main_Quest01_Key", "$Vigilant_Main_Quest01_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest01_Data", "zzzAoMMq01"},
		/*02*/ {"Vigilant_Main_Quest02_Key", "$Vigilant_Main_Quest02_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest02_Data", "zzzAoMMq02"},
		/*03*/ {"Vigilant_Main_Quest03_Key", "$Vigilant_Main_Quest03_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest03_Data", "zzzAoMMq03"},
		/*04*/ {"Vigilant_Main_Quest04_Key", "$Vigilant_Main_Quest04_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest04_Data", "zzzAoMMq04"},
		/*05*/ {"Vigilant_Main_Quest05_Key", "$Vigilant_Main_Quest05_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest05_Data", "zzzAoMMq05"},
		/*06*/ {"Vigilant_Main_Quest06_Key", "$Vigilant_Main_Quest06_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest06_Data", "zzzAoMMq06"},
		/*07*/ {"Vigilant_Main_Quest07_Key", "$Vigilant_Main_Quest07_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest07_Data", "zzzAoMMq07"},
		/*08*/ {"Vigilant_Main_Quest08_Key", "$Vigilant_Main_Quest08_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest08_Data", "zzzAoMMq08"},
		/*09*/ {"Vigilant_Main_Quest09_Key", "$Vigilant_Main_Quest09_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest09_Data", "zzzAoMMqGoodEnd"},
		/*10*/ {"Vigilant_Main_Quest10_Key", "$Vigilant_Main_Quest10_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest10_Data", "zzzAoMMq09"},
		/*11*/ {"Vigilant_Main_Quest11_Key", "$Vigilant_Main_Quest11_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest11_Data", "zzzAoMMq10"},
		/*12*/ {"Vigilant_Main_Quest12_Key", "$Vigilant_Main_Quest12_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest12_Data", "zzzAoMSubQ01"},
		/*13*/ {"Vigilant_Main_Quest13_Key", "$Vigilant_Main_Quest13_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest13_Data", "zzzBMMq01"},
		/*14*/ {"Vigilant_Main_Quest14_Key", "$Vigilant_Main_Quest14_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest14_Data", "zzzBMMq02"},
		/*15*/ {"Vigilant_Main_Quest15_Key", "$Vigilant_Main_Quest15_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest15_Data", "zzzBMMq03"},
		/*16*/ {"Vigilant_Main_Quest16_Key", "$Vigilant_Main_Quest16_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest16_Data", "zzzCOMq01"},
		/*17*/ {"Vigilant_Main_Quest17_Key", "$Vigilant_Main_Quest17_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest17_Data", "zzzCHMQ00"},
		/*18*/ {"Vigilant_Main_Quest18_Key", "$Vigilant_Main_Quest18_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest18_Data", "zzzCHMQ01"},
		/*19*/ {"Vigilant_Main_Quest19_Key", "$Vigilant_Main_Quest19_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest19_Data", "zzzCHMQ02"},
		/*20*/ {"Vigilant_Main_Quest20_Key", "$Vigilant_Main_Quest20_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest20_Data", "zzzCHEpilogueQuest01"},
		/*21*/ {"Vigilant_Main_Quest21_Key", "$Vigilant_Main_Quest21_Name", MAIN_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Main_Quest21_Data", "zzzAoMSubQ02"},
	};

	constexpr std::size_t Quest1_StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,21
	};

	constexpr std::tuple<std::size_t, std::int32_t> Quest1_StageCompletion[] = {
		{ 19,  50 },
		{ 20,  20 },
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest2_QuestData[] = {
		/*00*/ {"Vigilant_Side_Quest00_Key", "$Vigilant_Side_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest00_Data", "zzzCHSubQuest02"},
		/*01*/ {"Vigilant_Side_Quest01_Key", "$Vigilant_Side_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest01_Data", "zzzCHSubQuest03"},
		/*02*/ {"Vigilant_Side_Quest02_Key", "$Vigilant_Side_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest02_Data", "zzzCHSubQuest04"},
		/*03*/ {"Vigilant_Side_Quest03_Key", "$Vigilant_Side_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest03_Data", "zzzCHSubQuest01"},
		/*04*/ {"Vigilant_Side_Quest04_Key", "$Vigilant_Side_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest04_Data", "zzzCHSubQuest09"},
		/*05*/ {"Vigilant_Side_Quest05_Key", "$Vigilant_Side_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest05_Data", "zzzCHSubQuest10"},
		/*06*/ {"Vigilant_Side_Quest06_Key", "$Vigilant_Side_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest06_Data", "zzzCHSubQuest08"},
		/*07*/ {"Vigilant_Side_Quest07_Key", "$Vigilant_Side_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest07_Data", "zzzCHSubQuest07"},
		/*08*/ {"Vigilant_Side_Quest08_Key", "$Vigilant_Side_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest08_Data", "zzzCHSubQuest12"},
		/*09*/ {"Vigilant_Side_Quest09_Key", "$Vigilant_Side_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest09_Data", "zzzCHSubQuest05"},
		/*10*/ {"Vigilant_Side_Quest10_Key", "$Vigilant_Side_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest10_Data", "zzzCHSubQuest06"},
		/*11*/ {"Vigilant_Side_Quest11_Key", "$Vigilant_Side_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest11_Data", "zzzCHSubQuest11"},
		/*12*/ {"Vigilant_Side_Quest12_Key", "$Vigilant_Side_Quest12_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Side_Quest12_Data", "zzzCHGreymarchQuest"},
	};

	constexpr std::size_t Quest2_StandardCompletion[] = {
		6,7,11
	};

	constexpr std::tuple<std::size_t, std::int32_t> Quest2_StageCompletion[] = {
		{ 0,   255 },
		{ 1,   255 },
		{ 2,   255 },
		{ 3,   255 },
		{ 4,   20 },
		{ 5,   20 },
		{ 8,   50 },
		{ 9,   60 },
		{ 10,  30 },
		{ 12,  20 },
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest3_QuestData[] = {
		/*00*/ {"Vigilant_Memory_Quest00_Key", "$Vigilant_Memory_Quest00_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest00_Data", "zzzCHMemoryQuest03"},
		/*01*/ {"Vigilant_Memory_Quest01_Key", "$Vigilant_Memory_Quest01_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest01_Data", "zzzCHMemoryQuest04"},
		/*02*/ {"Vigilant_Memory_Quest02_Key", "$Vigilant_Memory_Quest02_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest02_Data", "zzzCHMemoryQuest10"},
		/*03*/ {"Vigilant_Memory_Quest03_Key", "$Vigilant_Memory_Quest03_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest03_Data", "zzzCHMemoryQuest01"},
		/*04*/ {"Vigilant_Memory_Quest04_Key", "$Vigilant_Memory_Quest04_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest04_Data", "zzzCHMemoryQuest02"},
		/*05*/ {"Vigilant_Memory_Quest05_Key", "$Vigilant_Memory_Quest05_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest05_Data", "zzzCHMemoryQuest05"},
		/*06*/ {"Vigilant_Memory_Quest06_Key", "$Vigilant_Memory_Quest06_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest06_Data", "zzzCHMemoryQuest11"},
		/*07*/ {"Vigilant_Memory_Quest07_Key", "$Vigilant_Memory_Quest07_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest07_Data", "zzzCHMemoryQuest12"},
		/*08*/ {"Vigilant_Memory_Quest08_Key", "$Vigilant_Memory_Quest08_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest08_Data", "zzzCHMemoryQuest06"},
		/*09*/ {"Vigilant_Memory_Quest09_Key", "$Vigilant_Memory_Quest09_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest09_Data", "zzzCHMemoryQuest07"},
		/*10*/ {"Vigilant_Memory_Quest10_Key", "$Vigilant_Memory_Quest10_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest10_Data", "zzzCHMemoryQuest08"},
		/*11*/ {"Vigilant_Memory_Quest11_Key", "$Vigilant_Memory_Quest11_Name", SIDE_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Memory_Quest11_Data", "zzzCHMemoryQuest09"},
	};

	constexpr std::size_t Quest3_StandardCompletion[] = {
		0,1,2,3,4,5,6,7,8,9,10,11
	};

	/*<Unique Key>, <Quest Name>, <Quest Type>, <Check Stage Done>, <Quest Highlight Text>, <Quest Editor ID>*/
	constexpr std::tuple<const char*, const char*, std::int32_t, bool, const char*, const char*> Quest4_QuestData[] = {
		/*00*/ {"Vigilant_Radiant_Quest00_Key", "$Vigilant_Radiant_Quest00_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest00_Data", "zzzAomBountyBook"},
		/*01*/ {"Vigilant_Radiant_Quest01_Key", "$Vigilant_Radiant_Quest01_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest01_Data", "zzzAomBountyChickTrader"},
		/*02*/ {"Vigilant_Radiant_Quest02_Key", "$Vigilant_Radiant_Quest02_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest02_Data", "zzzAomBountyRelic"},
		/*03*/ {"Vigilant_Radiant_Quest03_Key", "$Vigilant_Radiant_Quest03_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest03_Data", "zzzAoMBountyRelicHorn"},
		/*04*/ {"Vigilant_Radiant_Quest04_Key", "$Vigilant_Radiant_Quest04_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest04_Data", "zzzAoMBountyPiper"},
		/*05*/ {"Vigilant_Radiant_Quest05_Key", "$Vigilant_Radiant_Quest05_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest05_Data", "zzzAomBountySummoner"},
		/*06*/ {"Vigilant_Radiant_Quest06_Key", "$Vigilant_Radiant_Quest06_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest06_Data", "zzzAomBountyVampire"},
		/*07*/ {"Vigilant_Radiant_Quest07_Key", "$Vigilant_Radiant_Quest07_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest07_Data", "zzzAomBountyWitch"},
		/*08*/ {"Vigilant_Radiant_Quest08_Key", "$Vigilant_Radiant_Quest08_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest08_Data", "zzzAoMRadDead"},
		/*09*/ {"Vigilant_Radiant_Quest09_Key", "$Vigilant_Radiant_Quest09_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest09_Data", "zzzAoMRadDoppler"},
		/*10*/ {"Vigilant_Radiant_Quest10_Key", "$Vigilant_Radiant_Quest10_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest10_Data", "zzzAoMRadMolag"},
		/*11*/ {"Vigilant_Radiant_Quest11_Key", "$Vigilant_Radiant_Quest11_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest11_Data", "zzzAoMRadVampire"},
		/*12*/ {"Vigilant_Radiant_Quest12_Key", "$Vigilant_Radiant_Quest12_Name", RADI_QUEST_FLAG, IS_STAGE_DONE_N, "$Vigilant_Radiant_Quest12_Data", "zzzAoMRadWitchHunter"},
	};

	constexpr std::tuple<std::size_t, const char*, std::int32_t> Quest4_GlobalCompletion[] = {
		
	{0,  "Completionist_BountyVigilantBook",			VIGILANT_COUNTER_VALUE},
	{1,  "Completionist_BountyVigilantChick",			VIGILANT_COUNTER_VALUE},
	{2,  "Completionist_BountyVigilantRelic",			VIGILANT_COUNTER_VALUE},
	{3,  "Completionist_BountyVigilantHorn",			VIGILANT_COUNTER_VALUE},
	{4,  "Completionist_BountyVigilantPiper",			VIGILANT_COUNTER_VALUE},
	{5,  "Completionist_BountyVigilantSummoner",		VIGILANT_COUNTER_VALUE},
	{6,  "Completionist_BountyVigilantVampire",			VIGILANT_COUNTER_VALUE},
	{7,  "Completionist_BountyVigilantWitch",			VIGILANT_COUNTER_VALUE},
	{8,  "Completionist_RadianceVigilantDead",			VIGILANT_COUNTER_VALUE},
	{9,  "Completionist_RadianceVigilantDopple",		VIGILANT_COUNTER_VALUE},
	{10, "Completionist_RadianceVigilantSpawn",			VIGILANT_COUNTER_VALUE},
	{11, "Completionist_RadianceVigilantVampire",		VIGILANT_COUNTER_VALUE},
	{12, "Completionist_RadianceVigilantWitchHunter",	VIGILANT_COUNTER_VALUE},
	};

	constexpr Serialization::FormArray Items = {
	0x1353DF,0x3B3500,0x148177,0x03D213,0x119E3C,0x11B7D0,0x0EF0B6,
	0x12942F,0x0D7360,0x0B6D80,0x0DA99B,0x0B0EAA,0x0C7E35,0x0B0EAB,
	0x0B0EAC,0x0B0EAD,0x142BF4,0x29F2EF,0x0CE832,0x00A962,0x03D783,
	0x1D19BD,0x071CE2,0x0D957B,0x1418A9,0x0C847B,0x1418A8,0x1280CB,
	0x0CE835,0x13F43D,0x12A7A3,0x0CB0AE,0x15CE2D,0x11880C,0x323B2F,
	0x0DFD65,0x0DFD66,0x0DFD67,0x0DFD68,0x1804E9,0x12B2F9,0x130E6B,
	0x187829,0x18782A,0x18782B,0x18782C,0x130E6A,0x119E3B,0x130E69,
	0x22B38C,0x12A7A2,0x142BF5,0x0EB828,0x0EB827,0x1418AB,0x1280CA,
	0x0CB0AB,0x1450A0,0x1418AA,0x1280CC,0x100E9F,0x0EA4C2,0x2699FB,
	0x2699FC,0x2699FD,0x2699FE,0x296938,0x13F43C,0x0D428C,0x11B7CF,
	0x0D428B,0x0CFCB6,0x12942E,0x1804E7,0x0CAA70,
	};

	constexpr Serialization::Variation ItmsV[] = {
	{ 0x23007E, { 0x230080,0x23007C,0x23007D,0x23007F,0x230081 } },
	{ 0x144CDD, { 0x144CDE } },
	{ 0x0BBF35, { 0x0BD352 } },
	{ 0x0D428A, { 0x0D568D } },
	{ 0x323B2F, { 0x43CBAD } },
	{ 0x0C3DA4, { 0x0B828B } },
	{ 0x1AABC2, { 0x1AABC3 } },
	{ 0x0C68B9, { 0x0C68B8 } },
	};
	
	constexpr Serialization::FormArray Books = {
	0x4A8AFA,0x054ED0,0x054ED1,0x4A8AFD,0x4A8B03,0x4A8AFE,0x4A8B05,
	0x4A8B04,0x4A8AFC,0x0D69FC,0x0D69FA,0x0D69FD,0x10832C,0x108323,
	0x108325,0x108322,0x0DE93B,0x0CDE74,0x1258B1,0x0CDE76,0x1404FF,
	0x108324,0x4A8AFB,0x1CB871,0x1CB872,0x1CB873,0x0DB22D,0x4E1317,
	0x13B4F9,0x13B4FA,0x13B4FB,0x24724A,0x13B4FC,0x13B4FD,0x13B501,
	0x13B506,0x13B4F7,0x054ED3,0x4A8B07,0x4A8B02,0x3F7CFC,0x13BAB0,
	0x3FB795,0x12905B,0x18B1BA,0x4A8AF6,0x4A8B00,0x4A8AFF,0x0CB0DF,
	0x12905E,0x114FF7,0x12905D,0x0D429E,0x12905F,0x4A8AF9,0x0DB22E,
	0x054ED2,0x12905C,0x4A8B06,0x18B1B9,0x4A8AF8,0x4A8B01,0x4A8AF7,
	0x054ED5,0x054ED6,0x054ED4,0x038AAE,
	};

	constexpr Serialization::FormArray MapMa = {
	0x088308,0x0801A8,0x093F3A,0x08ABE1,0x1388FA,0x089143,0x0A2517,
	0x0896EC,0x06A6D8,0x0A0C88,0x0A578D,0x091910,0x204817,0x128D5A,
	0x144AD5,0x128D54,0x09205C,0x09205D,0x092058,0x088312,0x21C52E,
	0x128D57,0x25B236,0x08773A,0x093F39,0x07FB64,0x09F7FB,0x145667,
	0x09BFF0,0x0A3E75,0x089DAA,0x0801A9,0x08E83E,0x08773F,0x4C3D8B,
	0x0A95A4,0x0A578F,0x128D51,0x091911,0x09D2B6,0x025045,0x0A87F4,
	0x128D55,0x0AFEBA,0x07FB65,0x09C65C,0x171D71,
	};

	// clang-format on

	constexpr std::string_view modname = "Vigilant.esm";

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		CHandler::SinkEvents();
		CHandler::InjectAndCompileData();
		CHandler::InstallQuestFramework();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	void CHandler::InstallQuestFramework() {

		Quest1_KeysArray.clear();
		Quest1_NameArray.clear();
		Quest1_TextArray.clear();
		Quest1_IdenArray.clear();
		Quest1_RadiArray.clear();
		Quest1_StgeArray.clear();

		Quest2_KeysArray.clear();
		Quest2_NameArray.clear();
		Quest2_TextArray.clear();
		Quest2_IdenArray.clear();
		Quest2_RadiArray.clear();
		Quest2_StgeArray.clear();

		Quest3_KeysArray.clear();
		Quest3_NameArray.clear();
		Quest3_TextArray.clear();
		Quest3_IdenArray.clear();
		Quest3_RadiArray.clear();
		Quest3_StgeArray.clear();

		Quest4_KeysArray.clear();
		Quest4_NameArray.clear();
		Quest4_TextArray.clear();
		Quest4_IdenArray.clear();
		Quest4_RadiArray.clear();
		Quest4_StgeArray.clear();

		for (auto& [key, name, flag, isStageDone, text, id] : Quest1_QuestData) {
			Quest1_KeysArray.push_back(key);
			Quest1_NameArray.push_back(name);
			Quest1_RadiArray.push_back(flag);
			Quest1_TextArray.push_back(text);
			Quest1_IdenArray.push_back(id);
			Quest1_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest2_QuestData) {
			Quest2_KeysArray.push_back(key);
			Quest2_NameArray.push_back(name);
			Quest2_RadiArray.push_back(flag);
			Quest2_TextArray.push_back(text);
			Quest2_IdenArray.push_back(id);
			Quest2_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest3_QuestData) {
			Quest3_KeysArray.push_back(key);
			Quest3_NameArray.push_back(name);
			Quest3_RadiArray.push_back(flag);
			Quest3_TextArray.push_back(text);
			Quest3_IdenArray.push_back(id);
			Quest3_StgeArray.push_back(isStageDone);
		}

		for (auto& [key, name, flag, isStageDone, text, id] : Quest4_QuestData) {
			Quest4_KeysArray.push_back(key);
			Quest4_NameArray.push_back(name);
			Quest4_RadiArray.push_back(flag);
			Quest4_TextArray.push_back(text);
			Quest4_IdenArray.push_back(id);
			Quest4_StgeArray.push_back(isStageDone);
		}

		assert(Quest1_KeysArray.size() == ArraySize);
		assert(Quest1_IdenArray.size() == ArraySize);
		assert(Quest1_NameArray.size() == ArraySize);
		assert(Quest1_RadiArray.size() == ArraySize);
		assert(Quest1_TextArray.size() == ArraySize);
		assert(Quest1_StgeArray.size() == ArraySize);
		Quest1_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest2_KeysArray.size() == ArraySize);
		assert(Quest2_IdenArray.size() == ArraySize);
		assert(Quest2_NameArray.size() == ArraySize);
		assert(Quest2_RadiArray.size() == ArraySize);
		assert(Quest2_TextArray.size() == ArraySize);
		assert(Quest2_StgeArray.size() == ArraySize);
		Quest2_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest3_KeysArray.size() == ArraySize);
		assert(Quest3_IdenArray.size() == ArraySize);
		assert(Quest3_NameArray.size() == ArraySize);
		assert(Quest3_RadiArray.size() == ArraySize);
		assert(Quest3_TextArray.size() == ArraySize);
		assert(Quest3_StgeArray.size() == ArraySize);
		Quest3_BoolArray = std::vector<bool>(ArraySize, false);

		assert(Quest4_KeysArray.size() == ArraySize);
		assert(Quest4_IdenArray.size() == ArraySize);
		assert(Quest4_NameArray.size() == ArraySize);
		assert(Quest4_RadiArray.size() == ArraySize);
		assert(Quest4_TextArray.size() == ArraySize);
		assert(Quest4_StgeArray.size() == ArraySize);
		Quest4_BoolArray = std::vector<bool>(ArraySize, false);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Sink Event ) -------------
	//---------------------------------------------------

	void CHandler::SinkEvents() {
		RE::BooksRead::GetEventSource()->AddEventSink(CHandler::GetSingleton());

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(CHandler::GetSingleton()));

		auto ESourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
		ESourceHolder->AddEventSink(static_cast<RE::BSTEventSink<RE::TESContainerChangedEvent>*>(CHandler::GetSingleton()));

		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(static_cast<RE::BSTEventSink<RE::TESQuestStageEvent>*>(GetSingleton()));
	}

	//---------------------------------------------------
	//-- Framework Events ( On Stage Set ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::TESQuestStageEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) {

		if (!a_event || !a_event->stage) { return RE::BSEventNotifyControl::kContinue; }

		const auto* quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (!quest) { return EventResult::kContinue; }

		auto t_pos = std::ranges::find(Quest1_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest1_IdenArray.end()) {
			if (Quest1_StgeArray.at(std::distance(Quest1_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest1_KeysArray.at(std::distance(Quest1_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest1_IdenArray.at(std::distance(Quest1_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest2_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest2_IdenArray.end()) {
			if (Quest2_StgeArray.at(std::distance(Quest2_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest2_KeysArray.at(std::distance(Quest2_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest2_IdenArray.at(std::distance(Quest2_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest3_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest3_IdenArray.end()) {
			if (Quest3_StgeArray.at(std::distance(Quest3_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest3_KeysArray.at(std::distance(Quest3_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest3_IdenArray.at(std::distance(Quest3_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}

		t_pos = std::ranges::find(Quest4_IdenArray, quest->GetFormEditorID());
		if (t_pos != Quest4_IdenArray.end()) {
			if (Quest4_StgeArray.at(std::distance(Quest4_IdenArray.begin(), t_pos))) {
				CQuestKeys_Stages.AddStage(Quest4_KeysArray.at(std::distance(Quest4_IdenArray.begin(), t_pos)), a_event->stage);
				INFO("Added Stage {} to '{}' Serialized Map.", a_event->stage, Quest4_IdenArray.at(std::distance(Quest4_IdenArray.begin(), t_pos)));
				return EventResult::kContinue;
			}
		}
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Item Added ) -------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {

		if (!a_event || a_event->newContainer != 0x00014 || !CPatch_VIG_Items::Data.HasForm(a_event->baseObj)) { return EventResult::kContinue; }

		auto base = CPatch_VIG_Items::Data.GetBase(a_event->baseObj) ? CPatch_VIG_Items::Data.GetBase(a_event->baseObj) : a_event->baseObj;
		CHandler::ProcessFoundForm(base, a_event->baseObj, "NotifyItems");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( Books Read ) ----------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::BooksRead::Event const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) {

		if (!a_event || !CPatch_VIG_Books::Data.HasForm(a_event->book->GetFormID())) { return RE::BSEventNotifyControl::kContinue; }

		auto base = CPatch_VIG_Books::Data.GetBase(a_event->book->GetFormID()) ? CPatch_VIG_Books::Data.GetBase(a_event->book->GetFormID()) : a_event->book->GetFormID();
		CHandler::ProcessFoundForm(base, a_event->book->GetFormID(), "NotifyBooks");
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Events ( On Menu Open ) --------------
	//---------------------------------------------------

	EventResult CHandler::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (a_event->menuName == RE::BookMenu::MENU_NAME && a_event->opening && CPatch_VIG_Books::Data.HasForm(RE::BookMenu::GetTargetForm()->GetFormID())) {
			if (auto target = RE::BookMenu::GetTargetForm()->GetFormID(); target) {
				auto base = CPatch_VIG_Books::Data.GetBase(target) ? CPatch_VIG_Books::Data.GetBase(target) : target;
				CHandler::ProcessFoundForm(base, target, "NotifyBooks");
				return EventResult::kContinue;
			}
			return EventResult::kContinue;
		}

		if (a_event->menuName == RE::MapMenu::MENU_NAME && a_event->opening) {

			for (auto i = 0; i < MapMa_FormArray.size(); i++) {
				CHandler::ProcessMapMarker(MapMa_FormArray[i], i);
			}
		}

		if (a_event->menuName == RE::JournalMenu::MENU_NAME) {
			CHandler::UpdateQuestFramework();
		}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Found Form ) -----
	//---------------------------------------------------

	void CHandler::ProcessFoundForm(RE::FormID a_baseID, RE::FormID a_eventID, std::string a_variable) {

		if (a_variable == "NotifyBooks") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_VIG_Books::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_VIG_Books::Data.GetAllVariations()) {
				if (CPatch_VIG_Books::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Books_FormArray, CPatch_VIG_Books::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Books_FormArray.begin(), t_pos);
			Books_BoolArray[b_pos] = true;

			Books_EntriesFound = std::ranges::count(Books_BoolArray, true);
			return;
		}

		if (a_variable == "NotifyItems") {

			if (!FoundItemData.HasForm(a_eventID)) {
				auto msg = fmt::format("{:s}{:s}!"sv, CVariables::V_NotificationText, CPatch_VIG_Items::Data.GetForm(a_eventID)->GetName());
				FrameworkAPI::SendNotification(msg, a_variable);
			}

			FoundItemData.AddForm(a_baseID);
			for (auto var : CPatch_VIG_Items::Data.GetAllVariations()) {
				if (CPatch_VIG_Items::Data.GetBase(var) == a_baseID) {
					FoundItemData.AddForm(var);
				}
			}


			auto t_pos = std::ranges::find(Items_FormArray, CPatch_VIG_Items::Data.GetForm(a_baseID));
			auto b_pos = std::distance(Items_FormArray.begin(), t_pos);
			Items_BoolArray[b_pos] = true;

			Items_EntriesFound = std::ranges::count(Items_BoolArray, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Process Map Marker ) -----
	//---------------------------------------------------

	void CHandler::ProcessMapMarker(RE::TESForm* a_form, std::int32_t a_pos) {

		auto* a_marker = static_cast<RE::TESObjectREFR*>(a_form);

		if (a_marker) {
			if (auto extraMapMarker = Serialization::CompletionistData::GetMapMarkerInternal(a_marker); extraMapMarker && extraMapMarker->mapData) {
				if (extraMapMarker->mapData->flags.all(RE::MapMarkerData::Flag::kVisible, RE::MapMarkerData::Flag::kCanTravelTo) && !a_marker->IsDisabled()) {
					MapMa_BoolArray[a_pos] = true;
					FoundItemData_NoShow.AddForm(a_marker);
				}
			}
		}
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Form Injection ) ---------
	//---------------------------------------------------

	void CHandler::InjectAndCompileData() {

		CPatch_VIG_Items::Data.CompileFormArray(CPatch_VIG::Items, modname);
		CPatch_VIG_Items::Data.CompileVariation(CPatch_VIG::ItmsV, modname);
		
		CPatch_VIG_Books::Data.CompileFormArray(CPatch_VIG::Books, modname);
		CPatch_VIG_MapMa::Data.CompileFormArray(CPatch_VIG::MapMa, modname);
		
		CPatch_VIG_Items::Data.MergeAsCollectable();
		CPatch_VIG_Books::Data.MergeAsCollectable();

		CPatch_VIG_Items::Data.Populate(Items_NameArray, Items_FormArray, Items_BoolArray, Items_TextArray);
		CPatch_VIG_Books::Data.Populate(Books_NameArray, Books_FormArray, Books_BoolArray, Books_TextArray, false, 1);
		CPatch_VIG_MapMa::Data.Populate(MapMa_NameArray, MapMa_FormArray, MapMa_BoolArray, MapMa_TextArray, false, 2);

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateFoundForms() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i = 0; i < Items_FormArray.size(); i++) {
			Items_BoolArray[i] = FrameworkAPI::IsItemKnown(Items_FormArray[i], &CPatch_VIG_Items::Data);
		}

		for (auto i = 0; i < Books_FormArray.size(); i++) {
			Books_BoolArray[i] = FrameworkAPI::IsBookKnown(Books_FormArray[i]);
		}

		for (auto i = 0; i < MapMa_FormArray.size(); i++) {
			MapMa_BoolArray[i] = FoundItemData_NoShow.HasForm(MapMa_FormArray[i]->GetFormID());
		}

		Items_EntriesTotal = Items_FormArray.size();
		Items_EntriesFound = std::ranges::count(Items_BoolArray, true);

		Books_EntriesTotal = Books_FormArray.size();
		Books_EntriesFound = std::ranges::count(Books_BoolArray, true);

		MapMa_EntriesTotal = MapMa_FormArray.size();
		MapMa_EntriesFound = std::ranges::count(MapMa_BoolArray, true);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update Found Forms ) -----
	//---------------------------------------------------

	void CHandler::UpdateQuestFramework() {

		if (!Serialization::CompletionistData::IsModInstalled(modname)) { return; }

		for (auto i : Quest1_StandardCompletion) {
			Quest1_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest1_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest1_KeysArray[i], Quest1_IdenArray[i]);
		};

		for (auto i : Quest2_StandardCompletion) {
			Quest2_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest2_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest2_KeysArray[i], Quest2_IdenArray[i]);
		};

		for (auto i : Quest3_StandardCompletion) {
			Quest3_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest3_KeysArray[i]) || FrameworkAPI::IsCompleted_N(Quest3_KeysArray[i], Quest3_IdenArray[i]);
		};

		for (auto& [i, stage] : Quest1_StageCompletion) {
			Quest1_BoolArray[i] = Quest1_StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(Quest1_KeysArray[i]) || FrameworkAPI::IsCompleted_S(Quest1_KeysArray[i], Quest1_IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(Quest1_KeysArray[i]) || FrameworkAPI::IsCompleted_P(Quest1_KeysArray[i], Quest1_IdenArray[i], stage);
		}

		for (auto& [i, stage] : Quest2_StageCompletion) {
			Quest2_BoolArray[i] = Quest2_StgeArray[i] ?
				FrameworkAPI::qIsOptionToggledInternal(Quest2_KeysArray[i]) || FrameworkAPI::IsCompleted_S(Quest2_KeysArray[i], Quest2_IdenArray[i], stage) :
				FrameworkAPI::qIsOptionToggledInternal(Quest2_KeysArray[i]) || FrameworkAPI::IsCompleted_P(Quest2_KeysArray[i], Quest2_IdenArray[i], stage);
		}

		for (auto& [i, global, value] : Quest4_GlobalCompletion) {
			Quest4_BoolArray[i] = FrameworkAPI::qIsOptionToggledInternal(Quest4_KeysArray[i]) || FrameworkAPI::IsCompleted_G(Quest4_KeysArray[i], Quest4_IdenArray[i], global, value);
		}
	}
}