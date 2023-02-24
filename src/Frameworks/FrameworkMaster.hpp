#pragma once
#include "Structs.hpp"
#include "Serialization.hpp"

namespace CFramework_Master
{
	inline Serialization::CompletionistKey CQuestKeys_Natural;
	inline Serialization::CompletionistKey CQuestKeys_Manual;
	inline Serialization::CompletionistKey CQuestKeys_Stages;

	inline Serialization::CompletionistData FoundItemData;
	inline Serialization::CompletionistData FoundItemData_NoShow;
	inline Serialization::CompletionistData ExcludedCellData;

	inline int PatchesInstalled;
	inline int TomesInstalled;
	
	inline constexpr std::int32_t RADIANT_BOUNTY_VALUE = 55245;
	inline constexpr std::int32_t RADIANT_COUNTER_VALUE = 72541;
	inline constexpr std::int32_t RADIANT_COLLEGE_VALUE = 62541;
	inline constexpr std::int32_t RADIANT_COMPANIONS_VALUE = 55285;
	inline constexpr std::int32_t RADIANT_BROTHERHOOD_VALUE = 45845;
	inline constexpr std::int32_t RADIANT_DAWNGUARD_VALUE = 45825;
	inline constexpr std::int32_t RADIANT_THIEVESGUILD_VALUE = 11548;
	inline constexpr std::int32_t VIGILANT_COUNTER_VALUE = 25141;
	inline constexpr std::int32_t LEGACY_COUNTER_VALUE = 33524;

	inline constexpr std::int32_t ArraySize = 128;
	inline constexpr std::int32_t MAIN_QUEST_FLAG = 0;
	inline constexpr std::int32_t SIDE_QUEST_FLAG = 1;
	inline constexpr std::int32_t RADI_QUEST_FLAG = 2;

	inline constexpr bool IS_STAGE_DONE_Y = true;
	inline constexpr bool IS_STAGE_DONE_N = false;

	inline std::vector<std::string> ExcludedQuestsArray;
	inline std::unordered_map<std::string, std::string> ExcludedQuests;

	class FrameworkAPI {

		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		static void							Register();
		static bool							RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		static void							Update();
		
		static std::vector<std::string>		qGetNameArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetTextArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetIdenArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<std::string>		qGetKeysArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<bool>			qGetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);
		static std::vector<int32_t>			qGetRadiArrayByID(RE::StaticFunctionTag*, std::int32_t q_questID);

		static bool							qIsOptionToggledInternal(std::string a_key);
		static bool							qIsOptionToggled(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);

		static std::int32_t					qIsOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);
		static void							qSetOptionCompleted(RE::StaticFunctionTag*, std::int32_t q_questID, std::string a_key);

		static std::vector<RE::TESForm*>	GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<bool>			GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::int32_t					IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, RE::TESForm* a_form);
		static void							SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_FrameworkID, RE::TESForm* a_form);

		static bool							CCItemsInstalled(RE::StaticFunctionTag*);
		static bool							CCBooksInstalled(RE::StaticFunctionTag*);
		static bool							CCLocationsInstalled(RE::StaticFunctionTag*);
		static bool							ShouldDisplayMiscHeader(RE::StaticFunctionTag*);
		static bool							ShouldDisplayTomeHeader(RE::StaticFunctionTag*);

		static void							SendNotification(std::string a_msg, std::string a_setting);
		static void							SendNotificationExt(RE::StaticFunctionTag*, std::string Message, std::string Colour, bool ColourEnabled);
		
		static std::string					ReplaceStr(std::string const& in, std::string const& from, std::string const& to);
		static std::string					GetHexValue(RE::StaticFunctionTag*, std::uint32_t IntVal);
		static std::string					GetVersion(RE::StaticFunctionTag*);

		static void							UpdateCompletion(RE::StaticFunctionTag*);

		static RE::TESQuest*				GetQuest(std::string a_questID);
		static bool							IsCompleted_P(std::string a_key, std::string a_questID, std::int32_t a_stage); //GetStage > Check
		static bool							IsCompleted_S(std::string a_key, std::string a_questID, std::int32_t a_stage); //IsStageDone Check
		static bool							IsCompleted_N(std::string a_key, std::string a_questID); // Normal Completion Check
		static bool							IsCompleted_G(std::string a_key, std::string a_questID, std::string a_globalID, int32_t a_value = 1); // GLobal Check
		static bool							IsCompleted_G(std::string a_key, std::string a_questID, std::string a_globalID, CRadiantEnum a_value); // GLobal Check
		static bool							IsCompleted_J(std::string a_key, std::string a_imp, std::string a_son); // Thane Check

		static bool							IsBookKnown(RE::TESForm* a_form);
		static bool							IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data);

		static void							LoadInjectedForms(RE::StaticFunctionTag*);

		static bool							UpdateQuestCompletion(CQuestData a_data, std::string a_name, std::string a_identifier);
		
	};
}
