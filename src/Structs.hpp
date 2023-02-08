#pragma once

enum class CFlagEnum
{
	kMain = 0,
	kSide = 1,
	kRadi = 2,
};

enum class CCompEnum
{
	kStand = 0,
	kStage = 1,
	kGlobl = 2,
};

enum class CStageEnum
{
	kNone = 0,
	kPast = 1,
	kDone = 2,
};

enum class CRadiantEnum
{
	kRadiant_Non = -1,

	kRadiant_One = 1,
	kRadiant_Fsh = 4,

	kRadiant_Def = 10,
	kRadiant_Bty = 11,

	kRadiant_COL = 12,
	kRadiant_COM = 13,
	kRadiant_DBR = 14,
	kRadiant_DGU = 15,
	kRadiant_THG = 16,
	kRadiant_LEG = 17,
	kRadiant_VIG = 18,
};

struct CRadiantData
{
	const char* NAME;
	CRadiantEnum VALUE;
};

struct CStageData
{
	CStageEnum CONDITION;
	int32_t VALUE;
	int32_t OPVALUE;
};

struct CQuestData
{
	const char* UDID;
	CFlagEnum FLAG;
	CCompEnum COMP;
	CStageData STAGE;
	CRadiantData GLOBAL;

	const char* EDID;
};

struct PlayerHomesData
{
	RE::FormID FORM;
	const char* FORM_PROVIDER;
	int32_t STAGE;
	int32_t STAGE2;

	const char* TEXT;
	const char* IDEN;

	RE::FormID KEY;
	const char* PROVIDER;

	const char* MODNAME;

	
};