#pragma once
#include "Structs.hpp"
#include "Serialization.hpp"
#include "Serialized Data Sets/PlayerKills.hpp"
#include "Serialized Data Sets/PatchSettings.hpp"
#include "Serialized Data Sets/CellTargets.hpp"

namespace CFramework_Master
{	
	inline Serialization::CompletionistKey CQuestKeys_Natural;
	inline Serialization::CompletionistKey CQuestKeys_Manual;

	inline Serialization::CompletionistData FoundItemData;
	inline Serialization::CompletionistData FoundItemData_NoShow;

	inline Serialization::CompletionistLog LoggingData;
	inline Serialization::CompletionistRadiantCounter RadiantCountData;

	inline Serialization::CompletionistExcludedReferences ExcludedCellScannerRefs;

	inline Serialization::CompletionistPatchSettings PatchSettings;

	inline Serialization::CompletionistData PlayerHits;
	inline Serialization::CompletionistDeathSentance PlayerKills;

	inline Serialization::CompletionistCellTargets CellTargets;

	inline int InstalledPatchesForMCMDisplay;

	//Form, Name, Page
	inline std::vector<std::tuple<RE::TESForm*, std::string, std::string, int32_t>> CItemsDataVec;

	inline std::vector<std::function<void(const char* nam)>> _OnMapMarkerfunctions;
	inline std::vector<std::function<void()>> _OnUpdateFoundForms;
	inline std::vector<std::function<bool(const char* nam)>> _OnIsCompleted;

	enum EntryCategory
	{
		kNone, //Spell tomes & Skill Books
		kItem,
		kBook,
		kMapM,
		kShou,
		kEnch,
		kHome,
		kPets,
		kClaw,
		kMask,
		kFish,
		kShrine,
		kStones,
		kBarenziah,
		kShard,
	};

	class FrameworkAPI 
	{
		using VM = RE::BSScript::Internal::VirtualMachine;
		using StackID = RE::VMStackID;
		using Severity = RE::BSScript::ErrorLogger::Severity;

	public:

		template<typename Function>
		static void AddMapMarkerDiscovery(Function&& fn)
		{
			_OnMapMarkerfunctions.push_back(std::forward<Function>(fn));
		};

		template<typename Function>
		static void AddUpdateFoundForms(Function&& fn)
		{
			_OnUpdateFoundForms.push_back(std::forward<Function>(fn));
		};

		static void AddMapMarkerDiscovery_Invoke(const char* nam)
		{
			for (auto&& fn : _OnMapMarkerfunctions) {
				fn(nam);
			}
		};

		static void AddUpdateFoundForms_Invoke()
		{
			for (auto&& fn : _OnUpdateFoundForms) {
				fn();
			}
		};

		//General Functions
		static void							Register();
		static void							RegisterCustomPatches();
		static void							FinalizeRegistrations();

		static void							Update();
		// New game or load, called from main.cpp
		static void							OnGameStarted(bool a_newGame);

		static void							SendNotification(std::string a_msg, std::string a_setting);

		static std::string					ReplaceStr(std::string const& in, std::string const& from, std::string const& to);
		static std::string					GetVersion(RE::StaticFunctionTag*);

		static void							NotifyStartup();

		static void							AddNewEventToLog(Serialization::CompletionistLog::logType kType, std::string a_log);

		//Items Functions
		static std::vector<RE::TESForm*>	GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<std::string>		GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static std::vector<bool>			GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_FrameworkID);
		static void							SetOptionCompleted(std::int32_t a_FrameworkID, RE::TESForm* a_form, bool a_completed);


		static std::int32_t					GetPatchCount(RE::StaticFunctionTag*);
		
		static bool							IsBookKnown(RE::TESForm* a_form);
		static bool							IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data);
		static bool							IsEnchantmentKnown(RE::TESForm* a_form);


		static void							ProcessCaughtFish(RE::TESForm* a_form);
		static void							WatchFishing();

		static std::int32_t					GetBookCategoryType(RE::TESForm*);

		static std::vector<std::string>		GetLoggingDates(RE::StaticFunctionTag*);
		static std::vector<std::string>		GetLoggedEventsForDate(RE::StaticFunctionTag*, std::string a_date, bool b_prefix);

		static const char*					OnMapMarkerDiscovered(RE::TESFullName* a_form);
		static void							OnMapMarkerAdded(RE::TESFullName* a_form);
		static bool							compare_dates(const std::string& a, const std::string& b);

		static void							OnHit(RE::TESHitEvent const* a_event);
		static void							OnDeath(RE::TESDeathEvent const* a_event);
	private:
		static inline REL::Relocation<decltype(OnMapMarkerDiscovered)> _OnMapMarkerDiscovered;
		static inline REL::Relocation<decltype(OnMapMarkerAdded)> _OnMapMarkerAdded;
	};
}