#pragma once

#include "UI/Core/IDataProvider.hpp"
#include "UI/SettingsPages.hpp"

namespace CompletionistUI
{
	// Rebuild() on the game thread, getters on the render thread
	class GameDataProvider final :
		public IDataProvider,
		public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>,
		public RE::BSTEventSink<RE::BGSActorCellEvent>
	{
	public:
		void Rebuild();
		void OnGameLoaded();

		// Rescans on cell change, also puts the pin marker back
		void WatchCells();
		RE::BSEventNotifyControl ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) override;
		RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) override;

		std::vector<Category>     GetCategories() override;
		Page                      GetPage(std::string_view a_id) override;
		void                      SetCompleted(std::string_view a_pageID, const Entry& a_entry, bool a_completed) override;
		void                      SetPageChoice(std::string_view a_pageID, std::int32_t a_choice) override;
		std::vector<SettingsPage> GetSettingsPages() override;
		void                      ApplySetting(std::string_view a_pageID, const SettingItem& a_item) override;
		AboutInfo                 GetAbout() override;
		std::vector<RecentEvent>  GetRecentEvents() override;
		std::vector<KillRecord>   GetKills() override;
		void                      ResetKills(std::string_view a_name, bool a_remove) override;
		PinList                   GetPins() override;
		void                      SetPin(const Entry& a_entry) override;
		void                      ClearPin(std::uint32_t a_formID) override;
		void                      ClearPins() override;
		std::uint32_t             GetLocationMarker() override { return _locationMarker; }
		void                      SetLocationMarker(const Entry& a_entry) override;
		void                      ClearLocationMarker() override;
		CellScan                  GetCellScan() override;
		void                      RescanCell() override;
		void                      MarkCellItem(const CellItem& a_item) override;
		void                      ClearCellMarker(const CellItem& a_item) override;
		void                      ClearCellMarkers() override;
		void                      ExcludeCurrentCell() override;
		void                      ExcludeCellItem(const CellItem& a_item) override;
		std::uint32_t             Revision() override { return _revision.load(); }
		std::string               Translate(std::string_view a_key) override;

		struct GeneralSettings
		{
			bool                                          valid{ false };
			std::vector<std::string>                      radiantOptions;
			std::int32_t                                  radiantChoice{ 0 };
			std::unordered_map<std::string, std::int32_t> counters;
			bool                                          treatBooksAsItems{ false };
			bool                                          debug{ false };
			std::vector<std::string>                      dawnguardOptions;
			std::int32_t                                  dawnguardChoice{ 0 };
			std::vector<std::string>                      legacyOptions;
			std::int32_t                                  legacyChoice{ 0 };
			bool                                          legacyInstalled{ false };
			std::string                                   dllVersion;
			std::int32_t                                  patchCount{ 0 };
		};

	private:
		void LoadTranslations();
		void RebuildSettings();
		void RebuildKills();
		void RebuildPins();
		[[nodiscard]] std::string PageListing(std::uint32_t a_formID);

		std::vector<KillRecord>    _kills;
		PinList                    _pins;
		std::atomic<std::uint32_t> _locationMarker{ 0 };

		void ScanCellNow();
		struct CellRecord
		{
			CellItem            item;
			RE::ObjectRefHandle ref;
		};
		std::vector<CellRecord>               _cellRecords;
		CellScan                              _cellScan;
		RE::FormID                            _scannedCell{ 0 };
		std::chrono::steady_clock::time_point _lastCellScan{};
		std::atomic<bool>                     _cellScanQueued{ false };
		std::atomic<bool>                     _pruneTracking{ false };
		GeneralSettings          _general;
		std::vector<RecentEvent> _recent;
		SettingsPages            _settingsBuilder;
		std::vector<SettingsPage> _settingsPages;
		std::atomic<std::uint32_t> _revision{ 0 };

		std::mutex                                   _lock;
		std::mutex                                   _translationLock;
		std::vector<Category>                        _categories;
		std::unordered_map<std::string, Page>        _pages;
		std::unordered_map<std::string, std::string> _uiStrings;
		std::unordered_map<std::string, std::string> _translations;
		bool                                         _translationsLoaded{ false };
	};
}
