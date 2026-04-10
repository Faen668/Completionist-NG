#pragma once
#include "Enums.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Internal Utility/Localisation.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/Variables.hpp"

//link - value - base - vari - stage - name - times requried
struct CRadiantData
{
	std::string link{};
	CRadiantEnum value{};
	RE::FormID baseID{};
	RE::FormID variID{};
	int32_t stage{};

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;

	//Is this quest a generic favor quest in the ini framework.
	bool isFavorQuest{};
};

//Faction, Faction File, Actor, Actor File
struct CFavorData 
{
	const char* link{};
	RE::FormID faction{};
	const char* faction_provider{};
	RE::FormID actor{};
	const char* actor_provider{};

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;
};

struct CStageData
{
	std::string link;
	CStageEnum type;
	int32_t stage;
	int32_t optional_stage = 0;

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;
};

struct CThaneData
{
	const char* link;
	const char* Sons;
	const char* Imps;
	int32_t stage = 25;

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;
};

struct CDrunkData
{
	std::string link;
	RE::FormID listID;
	RE::FormID formID;
	std::string baseActor_file_name;
	std::string itemLists_file_name;

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;
};

struct FavorMerchantData
{
	RE::FormID formID;
	std::string fileName;
	std::string quest_key;
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

struct PlayerHomesDataStruct
{
	PlayerHomesDataStructType type;
	int32_t stage;
	int32_t optional_stage;
	RE::FormID formID;
};

struct PetsDataStruct
{
	PetsDataStructType type;
	int32_t stage;
	RE::FormID formID;
};

struct CCivilWarData
{
	const char* link;
	RE::FormID location;

	//Should this quest be included in the quest stage processor
	std::optional<CQuestProcessor> process = std::nullopt;
};

//Format: CC*(Key), CFlagEnum(kType), CCompEnum(kType), CC*(editor_id)
struct CQuestData
{
	// Static
	std::string		unique_identifier{};
	CFlagEnum		quest_type{};
	CCompEnum		completion_type{};
	std::string		editor_id{};

	// Dynamic Types
	CThaneData*		thane_data{};
	CStageData*		stage_data{};
	CRadiantData*	radiant_data{};
	CFavorData*		favor_data{};
	CCivilWarData*	civil_war_data{};

	// Dynamic Strings
	std::string		localisation_key{};
	std::string		search_term{};
	std::string		search_description{};

	// Dynamic Bools
	CQuestProcessor should_process{};
	bool			is_completed{};

	// For patches
	std::string		mcmPage{};
	bool			official{};
	bool			log_install{};

	//Override Enum & Map
	enum override
	{
		kName = 0,
		kData = 1,
		kEdit = 2,
		kLocKey = 3,
		kProcess = 4,
	};

	const std::vector<std::pair<override, std::string>> override_Map{
		{override::kName,	"Quest Name"},
		{override::kData,	"Highlight Text"},
		{override::kEdit,	"EditorID"},
		{override::kLocKey,	"Localisation Key"},
		{override::kProcess,"Quest Processor"},
	};

	// Builder Functions
	auto init(bool a_official = true, std::optional<std::string> a_page = std::nullopt)
	{
		official = a_official;

		if (a_page.has_value()) {
			mcmPage = a_page.value();
		}

		should_process = CQuestProcessor::kIncluded;
		localisation_key = unique_identifier;
		return this;
	};

	auto set_editorID(const std::string& a_string) {
		editor_id = a_string;
		return this;
	}

	auto set_quest_type(CFlagEnum a_type) {
		quest_type = a_type;
		return this;
	}

	auto set_completion_type(CCompEnum a_type) {
		completion_type = a_type;
		return this;
	}

	auto set_name(const std::string& a_string) {
		search_term = a_string;
		return this;
	}

	auto set_highlight(const std::string& a_string) {
		search_description = a_string;
		return this;
	}

	auto finalize() {
		SetLocalisedTranslationParameters();
		return this;
	}

	template <std::size_t N>
	auto initThaneData(CThaneData(&a_data)[N])
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				thane_data = &a_data[idx];
				if (!thane_data) { ERROR("Unable to initialise thane_data for [{}]", unique_identifier); }

				if (thane_data->process) { 
					should_process = thane_data->process.value(); 
				}
			}
		}
		return this;
	}

	template <std::size_t N>
	auto initCivilWarData(CCivilWarData(&a_data)[N])
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				civil_war_data = &a_data[idx];
				if (!civil_war_data) { ERROR("Unable to initialise civil_war_data for [{}]", unique_identifier); }

				if (civil_war_data->process) {
					should_process = civil_war_data->process.value();
				}
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

				if (stage_data->process) {
					should_process = stage_data->process.value();
				}
			}
		}
		return this;
	}

	auto initPatchStageData(CStageData* a_data) {
		stage_data = a_data;
		if (!stage_data) { ERROR("Unable to initialise stage_data for [{}]", unique_identifier); }
		return this;
	}

	template <std::size_t N>
	auto initRadiantData(CRadiantData(&a_data)[N]) 
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				radiant_data = &a_data[idx];
				if (!radiant_data) { ERROR("Unable to initialise radiant_data for [{}]", unique_identifier); }

				if (radiant_data->process) {
					should_process = radiant_data->process.value();
				}
			}
		}
		return this;
	}

	auto initPatchRadiantData(CRadiantData* a_data) {
		radiant_data = a_data;
		if (!radiant_data) { ERROR("Unable to initialise radiant_data for [{}]", unique_identifier); }
		return this;
	}

	template <std::size_t N>
	auto initFavorData(CFavorData(&a_data)[N])
	{
		for (auto idx = 0; idx < N; ++idx) {
			if (DKUtil::string::iequals(GetKey(), a_data[idx].link)) {
				favor_data = &a_data[idx];
				if (!favor_data) { ERROR("Unable to initialise favor_data for [{}]", unique_identifier); }
			}
		}
		return this;
	}

	// Localisation
	void SetLocalisedTranslationParameters()
	{
		if (official) {
			search_term = GET_LOC_NAME_BY_KEY(localisation_key.c_str());
			search_description = GET_LOC_DESCRIPTION_BY_KEY(localisation_key.c_str());
		}
	}

	void ValidateLocalisation() const
	{
		auto* quest = GetQuest();
		if (!quest || DKUtil::string::is_empty(quest->GetName()))
		{
			// No quest or quest name is empty, nothing to validate
			return;
		}

		if (!DKUtil::string::iequals(quest->GetName(), search_term))
		{
			// Log validation failure
			INFO("Localisation Validation Failed On Key {}: {} ||| {}", GetKey(), quest->GetName(), GetName());
		}
	}

	// Override base members
	auto override(override section, const char* s_key)
	{
		switch (section)
		{
		case CQuestData::kName:
			search_term = s_key;
			break;
		case CQuestData::kData:
			search_description = s_key;
			break;
		case CQuestData::kEdit:
			editor_id = s_key;
			break;
		case CQuestData::kLocKey:
			localisation_key = s_key;
			SetLocalisedTranslationParameters();
			break;
		default:
			break;
		}
		return this;
	}

	// Quest Aquirement
	template <typename T = RE::TESForm>
	[[nodiscard]] static T* GetFullForm(RE::FormID a_form, const char* a_filename) noexcept
	{
		auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(a_form, a_filename);
		return form ? form->As<T>() : nullptr;
	}

	// Getters & Setters
	auto GetKey()					const -> std::string		{ return fmt::format("{:s}"sv, unique_identifier); }
	auto GetQuest()					const -> RE::TESQuest*		{ if (HasRadiantData()) { return GetradiantQuest(); } return static_cast<RE::TESQuest*>(RE::TESForm::LookupByEditorID(editor_id)); }
	auto GetName()					const -> std::string		{ return search_term; }
	auto GetEditorID()				const -> std::string		{ return editor_id; }
	auto GetHighlight()				const -> std::string		{ return search_description; }
	auto ShouldProcess()			const -> CQuestProcessor	{ return should_process; }
	auto GetQuestProcessorType()	const -> int32_t			{ return std::to_underlying(should_process); }
	auto GetCompletionType()		const -> int32_t			{ return std::to_underlying(completion_type); }
	auto GetCompletionTypeString()	const -> std::string		{ for (const auto& [value, string] : CCompEnum_Map) { if (value == completion_type) { return string; } } return ""; }
	auto GetType()					const -> int32_t			{ return std::to_underlying(quest_type); }
	auto GetTypeString()			const -> std::string		{ for (auto& [value, string] : CFlagEnum_Map) { if (value == quest_type) { return string; } } return ""; }
	bool IsCompleted()				const						{ return is_completed; }
	void Switch()												{ is_completed = !is_completed; }

	auto GetQuestProcessorTypeString() const -> std::string {
		for (const auto& [value, string] : CQuestProcessor_Map) {
			if (value == should_process) {
				return string;
			}
		}
		return "";
	}

	// Stage Data
	
	auto GetStage()					const -> int32_t			{ return stage_data ? stage_data->stage : 0; }
	auto GetStageTypeEnum()			const -> CStageEnum			{ return stage_data ? stage_data->type : CStageEnum::kNone; }
	auto GetOptionalStage()			const -> int32_t			{ return stage_data ? stage_data->optional_stage : 0; }
	auto GetStageLink()				const -> std::string		{ return stage_data ? stage_data->link : "ERROR"; }
	auto GetStageType()				const -> int32_t			{ return stage_data ? std::to_underlying(stage_data->type) : 0; }
	bool HasStageData()				const						{ return stage_data != nullptr; }
	bool HasOptionalStage()			const						{ return GetOptionalStage() != 0; }
	auto GetStageTypeString() const -> std::string {
		for (const auto& [value, string] : CStageEnum_Map) {
			if (value == GetStageTypeEnum()) {
				return string;
			}
		}
		return "";
	}

	//Search Data
	bool HasSearchData()			const						{ return !search_term.empty(); }
	auto GetSearchTerm()			const -> std::string		{ return !search_term.empty() ? search_term : "ERROR"; }
	auto GetSearchDescription()		const -> std::string		{ return !search_description.empty() ? search_description : "ERROR"; }

	//Civil War Data
	bool HasCivilWarData()			const						{ return civil_war_data != nullptr; }
	auto GetCivilWarLocation()		const -> RE::BGSLocation*	{ return GetFullForm<RE::BGSLocation>(civil_war_data->location, "Skyrim.esm"); }

	//Favor Data
	bool HasFavorData()				const						{ return favor_data != nullptr; }
	auto GetFavorLink()				const -> std::string		{ return favor_data ? favor_data->link : "ERROR"; }
	auto GetActor()					const -> RE::TESNPC*		{ return HasFavorData() ? GetFullForm<RE::TESNPC>(favor_data->actor, favor_data->actor_provider) : nullptr; }
	auto GetActorFaction()			const -> RE::TESFaction*	{ return HasFavorData() ? GetFullForm<RE::TESFaction>(favor_data->faction, favor_data->faction_provider) : nullptr; }

	//Thane Data
	bool HasThaneData()				const { return thane_data != nullptr; }
	bool IsThane() const {
		if (!thane_data) {
			return false;
		}

		auto script = ScriptObject::FromForm(GetFullForm(0x087E24, "Skyrim.esm"), "FavorJarlsMakeFriendsScript");

		if (!script) {
			return false;
		}

		auto* impsProperty = script->GetProperty(thane_data->Imps);
		auto* sonsProperty = script->GetProperty(thane_data->Sons);

		if (!impsProperty || !sonsProperty) {
			return false;
		}

		return impsProperty->GetSInt() > 0 || sonsProperty->GetSInt() > 0;
	}

	//Radiant Data
	bool HasRadiantData()					const { return radiant_data != nullptr; }
	auto GetRadiantType()					const -> int32_t { return std::to_underlying(radiant_data->value); }
	auto GetRadiantLink()					const -> std::string { return radiant_data->link; }
	auto GetRadiantBaseFormID()				const -> RE::FormID { return radiant_data->baseID; }
	auto GetRadiantVariFormID()				const -> RE::FormID { return radiant_data->variID; }
	auto GetRadiantTypeEnum()				const -> CRadiantEnum { return radiant_data->value; }

	auto GetRadiantTypeString()	const -> std::string {
		for (const auto& [value, string] : CRadiantEnum_Map) {
			if (value == radiant_data->value) {
				return string;
			}
		}
		return "";
	}

	auto GetradiantQuest() const -> RE::TESQuest* {
		if (radiant_data && radiant_data->isFavorQuest == false) {
			if (auto* q1 = RE::TESQuest::LookupByID<RE::TESQuest>(radiant_data->baseID); q1) {
				return q1;
			}
			if (auto* q2 = RE::TESQuest::LookupByID<RE::TESQuest>(radiant_data->variID); q2) {
				return q2;
			}
		}
		return RE::TESQuest::LookupByEditorID<RE::TESQuest>(editor_id);
	}

	auto GetRadiantTimesRequired() const -> int32_t
	{
		using enum CRadiantEnum;

		if (!radiant_data)
		{
			return 1;
		}

		switch (radiant_data->value)
		{
		case kRadiant_DF1: return 1;
		case kRadiant_DF2: return 2;
		case kRadiant_DF3: return 3;
		case kRadiant_DF4: return 4;
		case kRadiant_DF5: return 5;
		case kRadiant_DF6: return 6;
		case kRadiant_DF7: return 7;
		case kRadiant_DF8: return 8;
		case kRadiant_DF9: return 9;
		case kRadiant_Fsh: return CVariables::V_Radiant_FishingVal;
		case kRadiant_Def: return CVariables::V_Radiant_FavorVal;
		case kRadiant_Bty: return CVariables::V_Radiant_BountyVal;
		case kRadiant_COL: return CVariables::V_Radiant_CollegeVal;
		case kRadiant_COM: return CVariables::V_Radiant_CompanionsVal;
		case kRadiant_DBR: return CVariables::V_Radiant_DBrotherhoodVal;
		case kRadiant_DGU: return CVariables::V_Radiant_DawnguardVal;
		case kRadiant_THG: return CVariables::V_Radiant_ThievesGuildVal;
		case kRadiant_LEG: return CVariables::V_Radiant_LegacyVal;
		case kRadiant_VIG: return CVariables::V_Radiant_VigilantVal;
		case kRadiant_BLD: return CVariables::V_Radiant_BladesVal;
		default: return 1;
		}
	}

	auto DumpToLog(int32_t idx, int32_t ID) {

		switch (completion_type)
		{
		case CCompEnum::kStand:
			if (thane_data || stage_data || radiant_data || favor_data || civil_war_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		case CCompEnum::kStage:
			if (!stage_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		case CCompEnum::kGlobl:
			if (!radiant_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		case CCompEnum::kThane:
			if (!thane_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		case CCompEnum::kFavor:
			if (!favor_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		case CCompEnum::kCiWar:
			if (!civil_war_data) {
				ERROR("completion_type Error on quest {}", unique_identifier);
			};
			break;
		default:
			break;
		}

		INFO("Quest {}: [{} Initialised with ID: {}", idx, GetKey(), ID);
		INFO("          ~Quest Data: editorID = {}", GetEditorID());
		INFO("          ~Quest Data: type = {}[{}]", GetTypeString(), GetType());
		INFO("          ~Quest Data: name = {}", GetName());
		INFO("          ~Quest Data: data = {}", GetHighlight());
		INFO("          ~Quest Data: process = {}[{}]", GetQuestProcessorTypeString(), GetQuestProcessorType());

		if (stage_data)
		{
			INFO("          ~Stage Data: type = {}[{}]", GetStageTypeString(), GetStageType());
			INFO("          ~Stage Data: stage = {}", GetStage());
			INFO("          ~Stage Data: optional stage = {}", GetOptionalStage());
		}

		if (radiant_data)
		{
			INFO("          ~Radiant Data: type = {}[{}]", GetRadiantTypeString(), GetRadiantType());
			INFO("          ~Radiant Data: base formID = {}", GetRadiantBaseFormID());
			INFO("          ~Radiant Data: vari formID = {}", GetRadiantVariFormID());
			INFO("          ~Radiant Data: quest stage = {}", radiant_data->stage);
			INFO("          ~Radiant Data: quest valid = {}", GetradiantQuest() ? "True" : "False");
		}

		if (favor_data)
		{
			INFO("          ~Favor Data: link = {}", GetFavorLink());
			INFO("          ~Favor Data: faction = {}", GetActorFaction()->GetFormID());
			INFO("          ~Favor Data: faction owner = {}", favor_data->faction_provider);
			INFO("          ~Favor Data: actor = {}", GetActor()->GetName());
			INFO("          ~Favor Data: actor owner = {}", favor_data->actor_provider);
		}

		if (thane_data)
		{
			auto script = ScriptObject::FromForm(GetFullForm(0x087E24, "Skyrim.esm"), "FavorJarlsMakeFriendsScript");

			INFO("          ~Thane Data: link = {}", thane_data->link);
			INFO("          ~Thane Data: stage = {}", thane_data->stage);
			INFO("          ~Thane Data: Imp = [{} - {}]", thane_data->Imps, script->GetProperty(thane_data->Imps)->GetSInt());
			INFO("          ~Thane Data: Son = [{} - {}]", thane_data->Sons, script->GetProperty(thane_data->Sons)->GetSInt());
			INFO("          ~Thane Data: script state = {}", script->IsConstructed() ? "Constructed" : "Un-Constructed");
		}

		if (!search_term.empty())
		{
			INFO("          ~Search Data: Term       = {}", GetSearchTerm());
			INFO("          ~Search Data: MCM Page   = {}", mcmPage);
			INFO("          ~Search Data: official   = {}", official);
		}

		INFO(" ");
		return this;
	};
};