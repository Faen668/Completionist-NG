#pragma once

#include "UI/Core/IDataProvider.hpp"
#include "UI/Core/Mixing.hpp"
#include "UI/Core/Toasts.hpp"
#include "UI/Core/UISettings.hpp"

#include <array>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

struct ImVec2;

namespace CompletionistUI
{
	// Trainer panel perk trees, laid out by depth
	struct PerkTreeLayout
	{
		std::vector<std::int32_t> column;
		std::vector<std::int32_t> row;
		std::vector<float>        place;	// row, centred on the column's middle
		std::vector<float>        x;	// across, in column gaps; zigzagged perks sit half a gap on
		std::vector<float>        labelUnits;	// how much of a gap its label may take
		float                     width{ 0.0f };	// first column to last, in column gaps
		std::int32_t              columns{ 0 };
		std::int32_t              rows{ 0 };	// the busiest column
	};
	PerkTreeLayout LayoutPerkTree(const std::vector<TrainerPerk>& a_perks);
	std::int32_t   PerkTreeRows(const std::vector<TrainerPerk>& a_perks);

	class Menu
	{
	public:
		// Returns false once closed
		bool Draw(IDataProvider& a_provider, UISettings& a_settings);

		void Refresh() noexcept { _dirty = true; }
		void Opened()
		{
			_justOpened = true;
			// Closed partway through, the tour doesn't pick up again
			EndTour();
		}
		// Harness
		void ForcePadHints(bool a_force) noexcept { _forcePadHints = a_force; }
		void OpenPage(std::string_view a_id);
		void OpenSettingsTab(std::string_view a_id) { _settingsTab = a_id; }
		// Harness
		void OpenColourPicker(std::string_view a_key)
		{
			_colourKey = a_key;
			_colourOriginal = -1;
		}

		void PinEntry(const Entry& a_entry)
		{
			_pinnedEntry = a_entry;
			_pinnedKey = a_entry.name + '\x1f' + a_entry.detail;
		}

		// Short message in the header
		void ShowStatus(std::string a_text, float a_seconds = 5.0f);

		[[nodiscard]] bool ConsumeScreenshotRequest() noexcept { return std::exchange(_screenshotRequested, false); }

		[[nodiscard]] bool ConsumeSettingsChanged() noexcept { return std::exchange(_settingsChanged, false); }

		static constexpr std::string_view settingsPage = "#settings";
		static constexpr std::string_view killsPage = "#kills";
		static constexpr std::string_view findPage = "#find";

		// Harness
		void OpenFind(std::string_view a_text);

		// Cell Scanner window
		void ShowCellView(bool a_cell) noexcept { _cellView = a_cell; }
		// Notification placement
		void StartToastPlacement() noexcept { _placingToasts = true; }
		[[nodiscard]] bool PlacingToasts() const noexcept { return _placingToasts; }
		void ShowCellOptionsSection(int a_section) noexcept { _cellOptionsSection = a_section; }	// Harness
		[[nodiscard]] bool CellView() const noexcept { return _cellView; }
		// On screen panel, no input. With notifications joined it frames them too, a_toasts the stack's size;
		// a_dim greys it, for placing
		struct CellPanelLayout
		{
			ImVec2 min{};	// the panel on its own, unjoined
			ImVec2 max{};
			ImVec2 dockAt{};	// where the joined notifications go
			float  dockWidth{ 0.0f };
			bool   dockUp{ false };
		};
		CellPanelLayout DrawCellPanel(IDataProvider& a_provider, const UISettings& a_settings, ToastJoin a_join = ToastJoin::kNone, ImVec2 a_toasts = {}, float a_dim = 0.0f);

		[[nodiscard]] const std::string& CurrentPage() const noexcept { return _selectedPage; }

		// Harness nav test
		[[nodiscard]] std::string DescribeFocus(std::uint32_t a_id) const;
		[[nodiscard]] std::string DescribeControls() const;
		std::string               lastRoute;
		struct SpotView
		{
			std::uint32_t id{ 0 };
			char          kind{ 'E' };	// E entry, C page control, H header control, S sidebar
			int           section{ 0 };
			int           index{ 0 };
			void*         window{ nullptr };
			float         minX{ 0.0f }, minY{ 0.0f }, maxX{ 0.0f }, maxY{ 0.0f };
			bool          visible{ false };
		};
		[[nodiscard]] std::vector<SpotView> Spots() const;
		void OpenPageTab(std::string_view a_page, int a_tab) { _pageTabs[std::string(a_page)] = std::to_string(a_tab); }
		void SetMixSlots(const std::array<std::uint32_t, 3>& a_slots) { _mixSlots = a_slots; }
		void ShowPicturesDialog(int a_kind) { _picturesDialog = a_kind == 2 ? PicturesDialog::kProgress : a_kind == 3 ? PicturesDialog::kRedraw : PicturesDialog::kOffer; }	// 1 offer, 2 progress, 3 redraw
		[[nodiscard]] const char* PicturesDialogShown() const { return _picturesDialog == PicturesDialog::kOffer ? "offer" : _picturesDialog == PicturesDialog::kRedraw ? "redraw" : _picturesDialog == PicturesDialog::kProgress ? "progress" : "none"; }
		void OpenMixPotion(std::string a_effect) { _mixOpenName = std::move(a_effect); }	// by its main effect's name

		void RecordControl();

		// Guided tour, offered the first time the window opens and started again from Settings
		void StartTour();
		void ShowTourStep(int a_step);	// Harness, -1 for the offer
		[[nodiscard]] int  TourStep() const noexcept { return _tourStep; }
		[[nodiscard]] static int TourSteps();

		[[nodiscard]] static float ComputeScale(const UISettings& a_settings, float a_displayHeight) noexcept;

	private:
		enum class SortMode : int
		{
			kDefault,
			kName,
			kNeededFirst,
		};

		// X button action on a row
		enum class RowAction : int
		{
			kNone,
			kMarker,
			kPin,
			kComplete,
		};
		[[nodiscard]] RowAction RowActionFor(const Entry& a_entry) const;
		[[nodiscard]] static const char* RowActionKey(RowAction a_action, bool a_on);
		void DoRowAction(IDataProvider& a_provider, const Entry& a_entry, RowAction a_action);
		bool RowActionButton(IDataProvider& a_provider, RowAction a_action, bool a_on, ImVec2 a_min, float a_side, bool a_clickable);

		// Controller navigation
		struct NavSpot
		{
			std::uint32_t id{ 0 };
			void*         window{ nullptr };
			float         minX{ 0.0f }, minY{ 0.0f }, maxX{ 0.0f }, maxY{ 0.0f };
			int           section{ 0 };
			int           index{ 0 };
			int           columns{ 1 };
			int           count{ 0 };
			bool          visible{ false };
		};
		void RouteNavigation();
		void RecordSpot(std::vector<NavSpot>& a_spots, int a_section, int a_index, int a_columns, int a_count);
		void ScrollFocusWithRightStick();

		void HandleGamepadShortcuts(bool& a_open);
		void TrackInputDevice();
		[[nodiscard]] bool  PadHintsShown() const noexcept { return _padActive || _forcePadHints || _tourPadHints; }
		[[nodiscard]] float PadHintsHeight() const;
		void                DrawPadHints(IDataProvider& a_provider, bool a_cellWindow = false);
		float UpdateNavigationOpen();
		void  DrawNavigationGrip(float a_gap, float a_footer);
		void DrawHeader(IDataProvider& a_provider, bool& a_open);
		void DrawHeaderButtons(IDataProvider& a_provider, bool& a_open, float a_right, float a_y);
		void DrawNavigation(IDataProvider& a_provider);
		void DrawOverview(IDataProvider& a_provider);
		void DrawPage(IDataProvider& a_provider);
		void DrawSettings(IDataProvider& a_provider);
		void DrawJournal(IDataProvider& a_provider, const Page& a_page);
		void DrawKills(IDataProvider& a_provider, const std::vector<KillRecord>& a_records, bool a_dragons = false);	// dragons: the Dragons page's tab
		void DrawPinnedBar(IDataProvider& a_provider);
		bool DrawCellWindow(IDataProvider& a_provider, UISettings& a_settings);	// false when it should close
		void DrawToastPlacement(IDataProvider& a_provider, UISettings& a_settings);
		void DrawCellOptions(IDataProvider& a_provider, UISettings& a_settings);
		void DrawCellList(IDataProvider& a_provider, const CellScan& a_scan);
		void TogglePin(IDataProvider& a_provider, const Entry& a_entry);
		void ToggleLocationMarker(IDataProvider& a_provider, const Entry& a_entry);
		void DrawFind(IDataProvider& a_provider);

		struct FindItem
		{
			std::string pageID;
			std::string pageTitle;
			std::string group;
			Entry       entry;
			bool        journal{ false };
		};
		void         BuildFindIndex(IDataProvider& a_provider);
		void         JumpTo(const FindItem& a_item);
		bool DrawSection(IDataProvider& a_provider, const Section& a_section, bool a_showTitle);
		void DrawEntry(IDataProvider& a_provider, const Entry& a_entry);
		void DrawDescriptionPanel(IDataProvider& a_provider, float a_height, float a_gap, const Page* a_journal = nullptr, const Entry* a_quest = nullptr);
		void DrawQuestDetail(IDataProvider& a_provider, const Page& a_page, const Entry& a_entry);
		void                       DrawShoutWords(IDataProvider& a_provider, const Entry& a_entry, bool a_sideBySide);
		[[nodiscard]] static float ShoutWordsHeight();
		void DrawShoutPanel(IDataProvider& a_provider, const Page& a_page, float a_height, float a_gap);
		void                       DrawTrainerInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName);
		[[nodiscard]] float TrainerInfoHeight() const;
		std::int32_t               _trainerRows{ 1 };	// most perks in one tree column on the page, so the panel keeps its height
		void                       DrawDragonInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName);
		[[nodiscard]] static float DragonInfoHeight(float a_lore, float a_width);
		[[nodiscard]] static float DragonLoreHeight(const std::string& a_lore, float a_width);
		float                      _dragonLore{ 0.0f };	// the page's longest story, so the panel keeps its height
		void                       DrawIngredientInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName);
		void                       DrawMixing(IDataProvider& a_provider, const Page& a_page);
		enum class PicturesDialog : std::uint8_t
		{
			kNone,
			kOffer,	// pictures just switched on: prepare them all now?
			kRedraw,	// Redraw pressed: throw them all away and draw them again?
			kProgress,	// preparing, with Cancel; ready, with Close
		};
		void                       DrawPicturesDialog(IDataProvider& a_provider);
		PicturesDialog             _picturesDialog{ PicturesDialog::kNone };
		bool                       _picturesStarted{ false };	// preparing has been seen under way
		void                       MixToggle(std::uint32_t a_formID);
		[[nodiscard]] static std::string BrewName(IDataProvider& a_provider, const Brew& a_brew);
		std::array<std::uint32_t, 3>      _mixSlots{};	// the Mixing Station's ingredients
		bool                              _mixAll{ false };	// every ingredient, not only the ones carried
		std::shared_ptr<const MixingData> _mixData;
		std::vector<PotionGroup>          _mixGroups;	// what can be brewed from what's carried, for _mixData
		std::unordered_set<std::uint64_t> _mixOpen;	// potions opened to their variations
		std::string                       _mixOpenName;	// opened once the groups are worked out
		static void                DrawIngredientPips(const Entry& a_entry);
		void                       MeasureIngredientPage(const Page& a_page, float a_width);
		[[nodiscard]] float        IngredientInfoHeight(float a_width) const;
		float                      _ingredientLore{ 0.0f };	// the tab's longest lore, most card rows and facts, so the panel keeps its height
		std::int32_t               _ingredientRows{ 1 };
		std::size_t                _ingredientFacts{ 1 };
		void                       DrawAchievementInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName);
		[[nodiscard]] static float AchievementInfoHeight(float a_body, float a_width);
		[[nodiscard]] static float AchievementBodyHeight(IDataProvider& a_provider, const Entry& a_entry, float a_width);
		float                      _achievementBody{ 0.0f };	// the page's tallest, so the panel keeps its height

		[[nodiscard]] float DescriptionPanelHeight() const;
		void                UpdateDescriptionPanel(float a_available, float a_gap);

		[[nodiscard]] bool PassesFilter(const Entry& a_entry) const;

		// What a tour step points at, noted as it's drawn
		enum class TourSpot : std::uint8_t
		{
			kNone,
			kSidebar,
			kBody,
			kHeaderTools,
			kEntries,
			kPageTools,
			kDescription,
			kCellList,
			kCellOptions,
			kCellBar,
			kOnScreen,
			kOnScreenToasts,	// shown with kOnScreen, where the notifications aren't joined to the panel
			kPadHints,
			kTourButton,
			kCount,
		};
		// The page or window a step needs open
		enum class TourView : std::uint8_t
		{
			kMain,
			kOverview,
			kPage,
			kCell,
			kSettings,
			kSettingsWindow,	// Settings on its Window tab
			kOnScreen,	// the window hidden, the panel and notifications shown as in play
		};
		struct TourStepDef
		{
			const char* key;	// key + "Title", key + "Text"
			TourSpot    spot;
			TourView    view;
		};
		static const TourStepDef& TourStepAt(int a_step);
		struct TourRect
		{
			float minX{ 0.0f }, minY{ 0.0f }, maxX{ 0.0f }, maxY{ 0.0f };
			bool  seen{ false };
		};
		void MarkTour(TourSpot a_spot, const ImVec2& a_min, const ImVec2& a_max);
		void MarkTourItem(TourSpot a_spot);	// the last item
		[[nodiscard]] bool TourRunning() const noexcept { return _tourStep >= 0 || _tourOffer; }
		[[nodiscard]] bool TourWants(TourSpot a_spot) const noexcept;
		void GoToTourStep(int a_step);
		void EndTour();
		void DrawTour(IDataProvider& a_provider);
		void DrawTourOnScreen(IDataProvider& a_provider, const UISettings& a_settings);
		void SetTourNavigation();
		std::array<TourRect, static_cast<std::size_t>(TourSpot::kCount)> _tourRects{};
		int                    _tourStep{ -1 };	// -1 off
		bool                   _tourOffer{ false };	// "take the tour?" first
		bool                   _tourPadHints{ false };	// the controller step shows the hints bar
		bool                   _tourScroll{ false };	// scroll the step's spot into view once
		float                  _tourShown[4]{};	// the highlight, eased to the step's spot
		float                  _tourHighlight{ 0.0f };	// 0 none, 1 shown
		float                  _tourCardW{ 0.0f };	// the card's size last frame, to place it
		float                  _tourCardH{ 0.0f };
		std::optional<ImVec4>  _tourDim;	// ImGui's modal wash, cleared while the tour shows
		int                    _tourStarted{ -1 };	// the frame it started, whose key press started it
		std::string            _tourPage;	// the page the list steps show
		std::string            _tourReturnPage;	// put back when it ends
		std::string            _tourReturnTab;
		bool                   _tourReturnCell{ false };
		bool                   _tourReturnFolded{ false };
		int                    _tourFocus{ 2 };	// the card's button the d-pad is on: 0 leave, 1 back, 2 on
		bool                   _tourKeyed{ false };	// moved with keys or the pad, so the focus shows
		ImGuiConfigFlags       _tourNavFlags{ 0 };	// ImGui's own steering, off while the card shows
		std::unique_ptr<Toasts> _tourToasts;	// the on-screen step's sample notifications

		struct TabItem
		{
			std::string id;
			std::string title;
			std::string count;
		};
		enum class TabKeys : std::uint8_t
		{
			kNone,
			kLeft,
			kBoth,
		};
		bool                      TabRow(const std::vector<TabItem>& a_tabs, std::string& a_selected, bool a_leftOnly = false);	// true when it changed
		[[nodiscard]] const Page& TabView(const Page& a_page, std::int32_t a_tab);

		std::vector<Category>                         _categories;
		std::vector<SettingsPage>                     _settingsPages;
		AboutInfo                                     _about;
		std::vector<RecentEvent>                      _recent;
		std::vector<KillRecord>                       _kills;
		std::vector<KillRecord>                       _dragonKills;
		PinList                                       _pins;
		std::uint32_t                                 _shownMarker{ 0 };
		bool                                          _cellView{ false };
		int                                           _cellOptionsSection{ 0 };	// Searching, Filters, Item Pinning
		std::unordered_map<std::string, std::int32_t> _pendingCellNumbers;
		std::uint32_t                                 _scrollToForm{ 0 };
		std::string                                   _killKey;
		int                                           _killSort{ 0 };	// kills, name, assists
		std::array<char, 128>                         _findText{};
		std::string                                   _findCategory;
		std::vector<FindItem>                         _findIndex;
		bool                                          _findIndexDirty{ true };
		bool                                          _findFocus{ false };
		std::string                                   _scrollToKey;
		std::unordered_map<std::string, std::int32_t> _pendingNumbers;
		std::unordered_map<std::string, std::array<char, 256>> _pendingTexts;
		std::unordered_set<std::string>                        _editingText;
		std::string                                            _settingsTab{ "window" };
		std::string                                            _confirmKey;
		float                                                  _spectrumPosition{ 0.0f };	// the colour picker's hue
		std::string                                            _colourKey;
		std::string                                            _colourReturnKey;	// focus goes back to this colour button after its picker closes
		std::int32_t                                           _colourOriginal{ 0 };
		std::optional<Page>   _page;
		std::string           _selectedPage;
		std::array<char, 128> _filter{};
		bool                  _hideCompleted{ false };
		bool                    _placingToasts{ false };
		ImVec2                  _placeFrom{};	// for Cancel
		ToastJoin               _placeJoinFrom{ ToastJoin::kNone };
		bool                    _placeMoving{ false };	// dragged or moved with keys, joins when let go
		float                   _joinedSize{ 0.0f };	// joined notifications' room in the panel frame, eased
		std::unique_ptr<Toasts> _toastSample;
		SortMode              _sort{ SortMode::kDefault };
		bool                  _dirty{ true };
		bool                  _backHandledByImGui{ false };
		bool                  _justOpened{ true };
		bool                  _focusSidebar{ false };
		bool                  _focusPage{ false };	// sidebar folded at opening: the page takes the focus
		std::uint32_t         _revealFocus{ 0 };	// that focus, shown by the first press
		std::uint32_t         _pendingHidden{ 0 };	// this _pendingFocus goes on without showing the focus box
		bool                  _focusCellList{ false };
		std::uint32_t         _cellParkedRow{ 0 };
		bool                  _showSidebarFocus{ false };
		bool                  _sidebarFocused{ false };
		bool                  _padActive{ false };
		bool                  _forcePadHints{ false };
		std::string           _padActionKey;
		std::string           _padTriggerKey;
		TabKeys               _tabsShown{ TabKeys::kNone };
		std::unordered_map<std::string, std::string> _pageTabs;	// page id to tab
		Page                  _tabView;
		std::string           _tabViewKey;
		std::uint32_t         _pageLoads{ 0 };
		std::vector<NavSpot>  _entrySpots;
		std::vector<NavSpot>  _entrySpotsLast;
		std::vector<NavSpot>  _controlSpots;
		std::vector<NavSpot>  _controlSpotsLast;
		std::vector<NavSpot>  _sidebarSpots;
		std::vector<NavSpot>  _sidebarSpotsLast;
		std::uint32_t         _sidebarSelectedID{ 0 };
		std::uint32_t         _sidebarSelectedLast{ 0 };
		std::uint32_t         _lastEntryFocus{ 0 };
		std::uint32_t         _lastMoveFrom{ 0 };	// last routed move, to come straight back
		std::uint32_t         _lastMoveTo{ 0 };
		int                   _lastMoveDir{ -1 };
		std::uint32_t         _pendingFocus{ 0 };
		std::uint32_t         _focusedSpot{ 0 };
		std::uint32_t         _focusedSpotLast{ 0 };
		int                   _openFrames{ 0 };
		int                   _spotSection{ 0 };
		bool                  _recordingHeader{ false };
		std::string           _focusKey;
		std::string           _restoreFocusKey;
		void                  KeepFocus(const std::string& a_key);
		NavSpot               _spotNext;
		std::uint32_t         _revision{ 0 };
		UISettings*           _settings{ nullptr };
		bool                  _settingsApplied{ false };
		bool                  _settingsChanged{ false };
		float                 _pendingScale{ -1.0f };
		float                 _pendingWidth{ 0.0f };
		float                 _pendingHeight{ 0.0f };
		bool                  _showScreenshotButton{ false };
		bool                  _screenshotRequested{ false };
		std::optional<Entry>  _hoveredEntry;
		std::optional<Entry>  _pinnedEntry;
		std::string           _pinnedKey;
		std::string           _journalKey;
		std::string           _status;
		float                 _statusTime{ 0.0f };
		float                 _descriptionOpen{ -1.0f };	// 1 open, 0 collapsed
		float                 _navigationOpen{ -1.0f };
		float                 _descriptionDragLines{ -1.0f };
		bool                  _descriptionHeld{ false };	// Y held, so its release isn't a tap
		bool                  _fitDescription{ false };	// fit the panel to the text once it's measured
		float                 _descriptionFitLines{ -1.0f };	// Y's fit, -1 for the saved height
		std::int32_t          _descriptionFitBase{ 0 };	// saved height when it was fitted
		float                 _descriptionMaxLines{ 12.0f };
		bool                  _descriptionDragged{ false };
	};
}
