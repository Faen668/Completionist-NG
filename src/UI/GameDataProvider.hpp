#pragma once

#include "UI/Core/IDataProvider.hpp"
#include "UI/SettingsPages.hpp"

namespace CompletionistUI
{
	// What the nearest-find key turned up, nearest first
	struct NearestFinds
	{
		std::vector<RE::NiPoint3> places;
		std::vector<std::pair<RE::FormID, RE::FormID>> ids;	// reference and item of each place
		bool                      marked{ false };
		bool                      cleared{ false };	// the press cancelled the last find instead
	};

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
		std::vector<KillRecord>   GetDragonKills() override;
		void                      ResetDragonKills(std::string_view a_name, bool a_remove) override;
		PinList                   GetPins() override;
		void                      SetPin(const Entry& a_entry) override;
		void                      ClearPin(std::uint32_t a_formID) override;
		void                      ClearPins() override;
		void                      SetLocationMarker(const Entry& a_entry) override;
		void                      ClearLocationMarker(std::uint32_t a_refID) override;
		CellScan                  GetCellScan() override;
		void                      RescanCell() override;
		void                      MarkCellItem(const CellItem& a_item) override;
		// Game thread. Each press in turn marks the nearest find, then cancels it, then marks the next nearest
		// With a_moveOn there's no cancel step: the last find's marker moves straight to the next nearest
		[[nodiscard]] NearestFinds MarkNearestCellItem(bool a_moveOn = false);
		// Game thread. Where each of these finds is now, empty once it's been collected or has gone
		[[nodiscard]] std::vector<std::optional<RE::NiPoint3>> WhereFindsAre(const std::vector<std::pair<RE::FormID, RE::FormID>>& a_ids, bool a_rescan);
		void                      ClearCellMarker(const CellItem& a_item) override;
		void                      ClearCellMarkers() override;
		void                      ExcludeCurrentCell() override;
		void                      ExcludeCellItem(const CellItem& a_item) override;
		std::uint32_t             Revision() override { return _revision.load(); }
		std::string               Translate(std::string_view a_key) override;
		void*                     AchievementIcon(std::int32_t a_icon) override;
		void*                     ItemPicture(std::uint32_t a_formID) override;
		bool                      ShowsItemPicture(std::uint32_t a_formID) override;
		void                      UpdatePictures() override;
		void                      PreparePictures(bool a_redraw) override;
		void                      CancelPictures() override;
		std::pair<int, int>       PicturesProgress() override;

		struct GeneralSettings
		{
			bool                                          valid{ false };
			std::vector<std::string>                      radiantOptions;
			std::int32_t                                  radiantChoice{ 0 };
			std::unordered_map<std::string, std::int32_t> counters;
			bool                                          treatBooksAsItems{ false };
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
		std::vector<KillRecord>    _dragonKills;
		PinList                    _pins;

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

		// The nearest-find key, game thread
		std::optional<std::pair<RE::FormID, RE::FormID>> _nearestCurrent;	// reference, item
		std::vector<std::pair<RE::FormID, RE::FormID>>   _nearestCancelled;
		RE::FormID                                       _nearestPlace{ 0 };
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
		std::unordered_map<std::int32_t, void*>      _icons;	// shader resource views, render thread
	};
}
