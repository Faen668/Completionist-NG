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

	inline int PatchesInstalled;
	inline int TomesInstalled;

	inline constexpr std::int32_t ArraySize = 128;

	class FrameworkAPI 
	{
		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		//General Functions
		static bool							RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);
		static void							Register();
		static void							Update();
		static void							LogWithPlugin(RE::StaticFunctionTag*, std::string a_message);

		static void							SendNotification(std::string a_msg, std::string a_setting);
		static void							SendNotificationExt(RE::StaticFunctionTag*, std::string Message, std::string Colour, bool ColourEnabled);

		static std::string					ReplaceStr(std::string const& in, std::string const& from, std::string const& to);
		static std::string					GetHexValue(RE::StaticFunctionTag*, std::uint32_t IntVal);
		static std::string					GetVersion(RE::StaticFunctionTag*);

		static void							LoadInjectedForms(RE::StaticFunctionTag*);
		static void							UpdateCompletion(RE::StaticFunctionTag*);
		

		//Items Functions
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

		static bool							IsBookKnown(RE::TESForm* a_form);
		static bool							IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data);
	};
}