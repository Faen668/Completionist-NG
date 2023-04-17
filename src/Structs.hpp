#pragma once
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"

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
	kThane = 3,
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
	kRadiant_DF1 = 1,
	kRadiant_DF2 = 2,
	kRadiant_DF3 = 3,
	kRadiant_DF4 = 4,
	kRadiant_DF5 = 5,
	kRadiant_DF6 = 6,
	kRadiant_Fsh = 7,
	kRadiant_Def = 8,
	kRadiant_Bty = 9,
	kRadiant_COL = 10,
	kRadiant_COM = 11,
	kRadiant_DBR = 12,
	kRadiant_DGU = 13,
	kRadiant_THG = 14,
	kRadiant_LEG = 15,
	kRadiant_VIG = 16,
};
const std::vector<std::pair<CRadiantEnum, std::string>> CRadiantEnum_Map{
	{CRadiantEnum::kRadiant_Non,	"None"},
	{CRadiantEnum::kRadiant_DF1,	"One"},
	{CRadiantEnum::kRadiant_DF2,	"Two"},
	{CRadiantEnum::kRadiant_DF3,	"Three"},
	{CRadiantEnum::kRadiant_DF4,	"Four"},
	{CRadiantEnum::kRadiant_DF5,	"Five"},
	{CRadiantEnum::kRadiant_DF6,	"Six"},
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
};

struct CRadiantData
{
	const char* link;

	CRadiantEnum value;
	RE::FormID baseID;
	RE::FormID variID;
	int32_t stage;

	const char* name;

	int32_t times_required;
};

struct CStageData
{
	const char* link;

	CStageEnum type;
	int32_t stage;
	int32_t optional_stage;
};

struct CThaneData
{
	const char* link;
	const char* Sons;
	const char* Imps;
};

struct CDrunkData
{
	const char* link;

	RE::FormID listID;
	RE::FormID formID;

	const char* file_name;
	const char* globalvariable;
};

struct FavorQuestData
{
	RE::FormID formID;
	const char* fileName;
};

struct FavorActorData
{
	RE::FormID formID;
	const char* fileName;
};

struct FavorMergedData
{
	FavorQuestData base;
	FavorQuestData vari;
	FavorActorData actr;

	std::int32_t Stage;
	std::int32_t Value;

	const char* Global;
};

struct FavorMerchantData
{
	RE::FormID formID;
	const char* Global;
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

struct CompanionsQuestData
{
	RE::FormID QuestID;
	const char* QuestFN;

	const char* Global1;
	const char* Global2;

	std::int32_t Value;
	std::int32_t Stage;
};

struct CArrayData
{
	std::vector<std::string>* editorids;
	std::vector<std::string>* names;
	std::vector<std::string>* highlights;
	std::vector<bool>* bools;
	std::vector<int32_t>* types;
};

//Format: CC*(Key), CFlagEnum(kType), CCompEnum(kType), CC*(editor_id)
struct CQuestData
{
	// Static
	const char*			unique_identifier{};
	const CFlagEnum		quest_type{};
	const CCompEnum		completion_type{};
	const char*			editor_id{};

	// Dynamic
	int32_t			array_position{};
	CThaneData*		thane_data{};
	CDrunkData*		drunk_data{};
	CArrayData*		array_data{};
	CStageData*		stage_data{};
	CRadiantData*	radiant_data{};

	enum override
	{
		kName = 0,
		kData = 1,
		kEdit = 2,
	};

	// Builder Functions

	auto init()
	{
		return this;
	};

	auto initQuestData(CArrayData* a_data) 
	{
		array_data = a_data;
		if (!array_data) { ERROR("Unable to initialise array_data for [{}]", unique_identifier); }

		array_data->editorids->push_back(editor_id);
		array_data->types->push_back(std::to_underlying(quest_type));
		array_data->names->push_back(fmt::format("${:s}_Name"sv, unique_identifier));
		array_data->highlights->push_back(fmt::format("${:s}_Data"sv, unique_identifier));

		array_position = std::distance(array_data->names->begin(), (std::ranges::find(array_data->names->begin(), array_data->names->end(), fmt::format("${:s}_Name"sv, unique_identifier))));
		return this;
	};

	template <std::size_t N>
	auto initThaneData(CThaneData(&a_data)[N])
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				thane_data = &a_data[idx];
				if (!thane_data) { ERROR("Unable to initialise thane_data for [{}]", unique_identifier); }
			}
		}
		return this;
	}

	template <std::size_t N>
	auto initDrunkData(CDrunkData(&a_data)[N])
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				drunk_data = &a_data[idx];
				if (!drunk_data) { ERROR("Unable to initialise drunk_data for [{}]", unique_identifier); }
			}
		}
		return this;
	}

	template <std::size_t N>
	auto initStageData(CStageData(&a_data)[N]) 
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				stage_data = &a_data[idx];
				if (!stage_data) { ERROR("Unable to initialise stage_data for [{}]", unique_identifier); }
			}
		}
		return this;
	}

	template <std::size_t N>
	auto initRadiantData(CRadiantData(&a_data)[N]) 
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				radiant_data = &a_data[idx];
				if (!radiant_data) { ERROR("Unable to initialise radiant_data for [{}]", unique_identifier); }
			}
		}
		return this;
	}

	auto override(override section, std::string text)
	{
		switch (section)
		{
		case CQuestData::kName:
			array_data->names->at(array_position) = text;
			break;
		case CQuestData::kData:
			array_data->highlights->at(array_position) = text;
			break;
		case CQuestData::kEdit:
			array_data->editorids->at(array_position) = text;
			break;

		default:
			break;
		}
		return this;
	}

	auto DumpToLog(int32_t idx, int32_t ID) {
		
		if ((completion_type == CCompEnum::kGlobl && !radiant_data) || (completion_type != CCompEnum::kGlobl && radiant_data)) {
			ERROR("Radiant Error on quest {}", GetKey());
		}

		if ((completion_type == CCompEnum::kStage && !stage_data) || (completion_type != CCompEnum::kStage && stage_data)) {
			ERROR("Stage Error on quest {}", GetKey());
		}

		if ((completion_type == CCompEnum::kThane && !thane_data) || (completion_type != CCompEnum::kThane && thane_data)) {
			ERROR("Thane Error on quest {}", GetKey());
		}

		if (completion_type == CCompEnum::kStand && (thane_data || stage_data || radiant_data)) {
			ERROR("Enum Type Error on quest {}", GetKey());
		}

		INFO("Quest {}: [{} Initialised at position {} with ID:", idx, GetKey(), array_position);
		INFO("~~~UUID: {}", ID);
		INFO("~~~Name: {}", GetName());
		INFO("~~~Data: {}", GetHighlight());
		INFO("~~~EditorID: {}", GetEditorID());

		if (array_data)
		{
			INFO("          ~Array Data: editorID = {}", GetEditorID());
			INFO("          ~Array Data: type = {}[{}]", GetTypeString(), GetType());
			INFO("          ~Array Data: name = {}", GetName());
			INFO("          ~Array Data: data = {}", GetHighlight());
		}

		if (stage_data)
		{
			INFO("          ~Stage Data: link = {}", GetStageLink());
			INFO("          ~Stage Data: type = {}[{}]", GetStageTypeString(), GetStageType());
			INFO("          ~Stage Data: stage = {}", GetStage());
			INFO("          ~Stage Data: optional stage = {}", GetOptionalStage());
		}

		if (radiant_data)
		{
			INFO("          ~Radiant Data: link = {}", GetRadiantLink());
			INFO("          ~Radiant Data: type = {}[{}]", GetRadiantTypeString(), GetRadiantType());
			INFO("          ~Radiant Data: base formID = {}", GetRadiantBaseFormID());
			INFO("          ~Radiant Data: vari formID = {}", GetRadiantVariFormID());
			INFO("          ~Radiant Data: global name = {}", GetRadiantGlobalname());
		}

		if (drunk_data)
		{
			INFO("          ~Drunk Data: link = {}", drunk_data->link);
			INFO("          ~Drunk Data: npc formID = {}", drunk_data->formID);
			INFO("          ~Drunk Data: npc Owner = {}", drunk_data->file_name);
			INFO("          ~Drunk Data: list formID = {}", drunk_data->listID);
			INFO("          ~Drunk Data: global name = {}", drunk_data->globalvariable);
		}

		if (thane_data)
		{
			auto script = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x087E24, "Skyrim.esm")), "FavorJarlsMakeFriendsScript");

			INFO("          ~Thane Data: link = {}", thane_data->link);
			INFO("          ~Thane Data: Imp = [{} - {}]", thane_data->Imps, script->GetProperty(thane_data->Imps)->GetSInt());
			INFO("          ~Thane Data: Son = [{} - {}]", thane_data->Sons, script->GetProperty(thane_data->Sons)->GetSInt());
			INFO("          ~Thane Data: script state = {}", script->IsConstructed() ? "Constructed" : "Un-Constructed");
		}

		INFO(" ");
		return this;
	};

	// Getters & Setters
	std::string		GetKey()			{ return fmt::format("{:s}"sv, unique_identifier); }
	RE::TESQuest*	GetQuest()			{ return static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(editor_id)); }
	RE::TESGlobal*	GetGlobal()			{ UpdateRadiantValues(); return RE::TESForm::LookupByEditorID<RE::TESGlobal>(radiant_data->name); }
	
	int32_t GetType()					{ return array_data->types->at(array_position);}
	std::string GetName()				{ return array_data->names->at(array_position); }
	std::string GetEditorID()			{ return array_data->editorids->at(array_position); }
	std::string GetHighlight()			{ return array_data->highlights->at(array_position); }
	std::string GetTypeString()			{ return GetType() == 0 ? "Main" : GetType() == 1 ? "Side" : GetType() == 2 ? "Radiant" : ""; }

	bool IsThane() {
		auto script = ScriptObject::FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x087E24, "Skyrim.esm")), "FavorJarlsMakeFriendsScript");
		if (!script) { return false; }

		return script->GetProperty(thane_data->Imps)->GetSInt() > 0 || script->GetProperty(thane_data->Sons)->GetSInt() > 0;
	}

	bool IsCompleted()					{ return array_data->bools->at(array_position); }	
	void Switch()						{ array_data->bools->at(array_position) = !array_data->bools->at(array_position); }

	// Stage Data
	int32_t GetStage()					{ return stage_data->stage; }
	int32_t GetStageType()				{ return std::to_underlying(stage_data->type); }
	CStageEnum GetStageTypeEnum()		{ return stage_data->type; }
	int32_t GetOptionalStage()			{ return stage_data->optional_stage; }
	bool HasOptionalStage()				{ return stage_data->optional_stage != 0 ? true : false; }
	std::string GetStageLink()			{ return stage_data->link; }
	std::string GetStageTypeString()	{ return GetStageType() == 0 ? "None" : GetStageType() == 1 ? "Past" : GetStageType() == 2 ? "Done" : ""; }

	//Radiant Data
	bool HasRadiantData()				{ return radiant_data != nullptr; }
	int32_t GetRadiantType()			{ return std::to_underlying(radiant_data->value); }
	std::string GetRadiantLink()		{ return radiant_data->link; }
	std::string GetRadiantTypeString()	{ for (auto& [value, string] : CRadiantEnum_Map) { if (value == radiant_data->value) { return string; } } return ""; }
	std::string GetRadiantGlobalname()	{ return radiant_data->name; }
	RE::FormID GetRadiantBaseFormID()	{ return radiant_data->baseID;  }
	RE::FormID GetRadiantVariFormID()	{ return radiant_data->variID; }
	CRadiantEnum GetRadiantTypeEnum()	{ return radiant_data->value; }
	
	RE::TESQuest* GetradiantQuest() {
		if (radiant_data) {
			auto* q1 = RE::TESForm::LookupByID<RE::TESQuest>(radiant_data->baseID);
			auto* q2 = RE::TESForm::LookupByID<RE::TESQuest>(radiant_data->variID);
			return q1 ? q1 : q2 ? q2 : nullptr;
		}
		return nullptr;
	}

	void UpdateRadiantValues()
	{
		if (!radiant_data) { return; }

		switch (radiant_data->value)
		{
		using enum CRadiantEnum;
		case kRadiant_DF1: radiant_data->times_required = 1; break;
		case kRadiant_DF2: radiant_data->times_required = 2; break;
		case kRadiant_DF3: radiant_data->times_required = 3; break;
		case kRadiant_DF4: radiant_data->times_required = 4; break;
		case kRadiant_DF5: radiant_data->times_required = 5; break;
		case kRadiant_DF6: radiant_data->times_required = 6; break;
		case kRadiant_Def: radiant_data->times_required = CVariables::V_Radiant_FavorVal; break;
		case kRadiant_Bty: radiant_data->times_required = CVariables::V_Radiant_BountyVal; break;
		case kRadiant_COL: radiant_data->times_required = CVariables::V_Radiant_CollegeVal; break;
		case kRadiant_COM: radiant_data->times_required = CVariables::V_Radiant_CompanionsVal; break;
		case kRadiant_DBR: radiant_data->times_required = CVariables::V_Radiant_DBrotherhoodVal; break;
		case kRadiant_DGU: radiant_data->times_required = CVariables::V_Radiant_DawnguardVal; break;
		case kRadiant_THG: radiant_data->times_required = CVariables::V_Radiant_ThievesGuildVal; break;
		case kRadiant_VIG: radiant_data->times_required = CVariables::V_Radiant_VigilantVal; break;
		case kRadiant_LEG: radiant_data->times_required = CVariables::V_Radiant_LegacyVal; break;
		default: radiant_data->times_required = 1;
		}
	}
};