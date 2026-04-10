#pragma once

enum class CMiscPatchType
{
	kItems = 0,
	kBooks = 1,
	kLocations = 2,
	kEnchantments = 3,
	kShouts = 4,
	kQuests = 5,
	kFish = 6,
	kPlayerHomes = 7,
	kPets = 8,
	kInteractableObject = 9,
};

enum class CFlagEnum
{
	kMain = 0,
	kSide = 1,
	kRadi = 2,
};
const std::vector<std::pair<CFlagEnum, std::string>> CFlagEnum_Map{
	{CFlagEnum::kMain,	"Main"},
	{CFlagEnum::kSide,	"Side"},
	{CFlagEnum::kRadi,	"Radiant"},
};

enum class CCompEnum
{
	kStand = 0,
	kStage = 1,
	kGlobl = 2,
	kThane = 3,
	kFavor = 4,
	kCiWar = 5,
};

const std::vector<std::pair<CCompEnum, std::string>> CCompEnum_Map{
	{CCompEnum::kStand,	"Standard"},
	{CCompEnum::kStage,	"By Stage"},
	{CCompEnum::kGlobl,	"As Radiant"},
	{CCompEnum::kThane,	"Thane Quest"},
	{CCompEnum::kFavor,	"Favor Quest"},
	{CCompEnum::kCiWar,	"Civil War"},
};

enum class CStageEnum
{
	kNone = 0,
	kPast = 1,
	kDone = 2,
};
const std::vector<std::pair<CStageEnum, std::string>> CStageEnum_Map{
	{CStageEnum::kNone,	"No Stage"},
	{CStageEnum::kPast,	"Stage Past"},
	{CStageEnum::kDone,	"Stage Done"},
};

enum class CQuestProcessor
{
	kIncluded = 0,
	kExcluded = 1,
};
const std::vector<std::pair<CQuestProcessor, std::string>> CQuestProcessor_Map{
	{CQuestProcessor::kIncluded, "Included"},
	{CQuestProcessor::kExcluded, "Excluded"},
};

enum class CRadiantEnum
{
	kRadiant_Non = -1,
	kRadiant_DF1 = 1,
	kRadiant_DF2 = 2,
	kRadiant_DF3 = 3,
	kRadiant_DF4 = 4,
	kRadiant_DF5 = 5,
	kRadiant_DF6 = 6,
	kRadiant_DF7 = 7,
	kRadiant_DF8 = 8,
	kRadiant_DF9 = 9,
	kRadiant_Fsh = 10,
	kRadiant_Def = 11,
	kRadiant_Bty = 12,
	kRadiant_COL = 13,
	kRadiant_COM = 14,
	kRadiant_DBR = 15,
	kRadiant_DGU = 16,
	kRadiant_THG = 17,
	kRadiant_LEG = 18,
	kRadiant_VIG = 19,
	kRadiant_BLD = 20,
};
const std::vector<std::pair<CRadiantEnum, std::string>> CRadiantEnum_Map{
	{CRadiantEnum::kRadiant_Non,	"None"},
	{CRadiantEnum::kRadiant_DF1,	"One"},
	{CRadiantEnum::kRadiant_DF2,	"Two"},
	{CRadiantEnum::kRadiant_DF3,	"Three"},
	{CRadiantEnum::kRadiant_DF4,	"Four"},
	{CRadiantEnum::kRadiant_DF5,	"Five"},
	{CRadiantEnum::kRadiant_DF6,	"Six"},
	{CRadiantEnum::kRadiant_DF7,	"Seven"},
	{CRadiantEnum::kRadiant_DF8,	"Eight"},
	{CRadiantEnum::kRadiant_DF9,	"Nine"},
	{CRadiantEnum::kRadiant_Fsh,	"Fishing"},
	{CRadiantEnum::kRadiant_Def,	"Default"},
	{CRadiantEnum::kRadiant_Bty,	"Bounty"},
	{CRadiantEnum::kRadiant_COL,	"College"},
	{CRadiantEnum::kRadiant_COM,	"Companions"},
	{CRadiantEnum::kRadiant_DBR,	"Dark Brotherhood"},
	{CRadiantEnum::kRadiant_DGU,	"Dawnguard"},
	{CRadiantEnum::kRadiant_THG,	"Thieves Guild"},
	{CRadiantEnum::kRadiant_LEG,	"Legacy of the Dragonborn"},
	{CRadiantEnum::kRadiant_VIG,	"Vigilant"},
	{CRadiantEnum::kRadiant_BLD,	"The Blades"},
};

enum class PlayerHomesDataStructType
{
	kQuest = 0,
	kKey = 1,
};

enum class PetsDataStructType
{
	kActivate = 0,
	kQuest = 1,
};