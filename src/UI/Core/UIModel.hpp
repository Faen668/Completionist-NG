#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace CompletionistUI
{
	enum class EntryState : std::uint8_t
	{
		kNeeded,
		kFound,
		kDisplayable,
		kDisplayed,
		kOccupied,
	};

	struct EntryWord
	{
		std::string text;
		bool        done{ false };
	};

	struct EntryPlace
	{
		std::string  name;	// quest name, empty for background quests
		std::string  region;
		bool         quest{ false };
		std::uint8_t words{ 0 };	// bit per word
		bool         discovered{ false };	// word walls: map marker found
	};

	// A perk on the skill's tree, placed as the game's Skills menu places it
	struct TrainerPerk
	{
		std::string               name;
		float                     depth{ 0.0f };	// up the tree in the Skills menu, across on the panel
		float                     lane{ 0.0f };	// across the tree in the Skills menu, down on the panel
		std::int32_t              level{ 0 };	// skill needed, 0 when none
		std::int32_t              rank{ 0 };	// ranks taken
		std::int32_t              ranks{ 1 };
		std::vector<std::int32_t> children;
	};

	struct EntryTrainer
	{
		std::int32_t             skill{ -1 };	// -1 when not a trainer
		std::string              skillName;
		std::int32_t             maxLevel{ 0 };
		std::int32_t             playerLevel{ 0 };
		std::string              location;
		std::string              now;	// where they are, when away from home
		std::vector<TrainerPerk> perks;
	};

	// A named dragon: what its name means, where it's met, its story
	struct EntryDragon
	{
		std::string fate;	// empty when not a dragon
		std::string meaning;
		std::string place;
		std::string lore;
	};

	// Progress on an achievement from this save
	struct AchievementGoal
	{
		std::string  label;
		std::int32_t value{ 0 };
		std::int32_t target{ 1 };
		bool         flag{ false };	// done or not, rather than a count
	};

	// A Steam or GOG achievement: how it's won, and the page that tracks it
	struct EntryAchievement
	{
		std::string                  game;	// empty when not an achievement
		std::string                  unlocked;	// when, empty while locked
		std::string                  locked;	// shown while locked, empty for the usual
		std::string                  rarity;
		std::string                  goal;	// what the store says it takes
		std::string                  tip;
		std::string                  page;	// page id to jump to, empty for none
		std::string                  pageTitle;
		bool                         missable{ false };
		std::int32_t                 icon{ 0 };
		std::vector<AchievementGoal> progress;
	};

	// An effect on an ingredient's panel, or an ingredient on an effect's
	struct IngredientCard
	{
		std::string name;	// "not known yet" when hidden
		std::string hint;	// how to learn it, or what it is
		bool        known{ false };
	};

	// An ingredient and its four effects, or an effect and the ingredients that carry it
	struct EntryIngredient
	{
		std::string                 summary;	// empty when not an ingredient or effect
		std::vector<std::string>    facts;
		std::string                 lore;
		std::vector<IngredientCard> cards;
		bool                        effect{ false };
	};

	// How an effect on an ingredient brews for the player now, as the game works it out from
	// their skill, perks and Fortify Alchemy. Index 0 is before the perks that only count in
	// a potion or a poison, 1 is in a potion, 2 in a poison
	struct MixEffect
	{
		std::uint32_t id{ 0 };	// the effect's form, the same on every ingredient that has it
		std::string   name;
		std::string   text;	// the game's description, <mag> and <dur> where the numbers go
		bool          known{ false };
		bool          hostile{ false };
		float         priority{ 0.0f };	// where two ingredients share it, the dearer one's is brewed
		std::int32_t  magnitude[3]{};
		std::int32_t  duration[3]{};
		std::int32_t  value[3]{};
	};

	struct MixIngredient
	{
		std::uint32_t          formID{ 0 };
		std::string            name;
		std::int32_t           carried{ 0 };
		std::vector<MixEffect> effects;
	};

	// Everything the Mixing Station tab brews from
	struct MixingData
	{
		std::vector<MixIngredient> ingredients;
		bool                       purity{ false };	// potions lose their harmful effects, poisons their good ones
		std::string                power;	// what the strength comes from, for the header
	};

	struct Entry
	{
		std::string   name;
		std::string   detail;
		std::string   description;
		EntryState    state{ EntryState::kNeeded };
		bool          showState{ true };	// false for settings
		bool          canToggle{ false };
		bool          marked{ false };	// marked complete by hand
		std::uint32_t formID{ 0 };
		std::int32_t  frameworkID{ -1 };	// framework or patch section, for marking by hand
		std::int32_t  questPage{ -1 };	// patch quests, for marking by hand
		std::string   key;	// quest key
		std::string   status;
		std::string   type;
		std::vector<EntryWord> words;
		std::vector<EntryPlace> places;
		bool                    canPin{ false };
		bool                    canMark{ false };
		bool                    placesKnown{ false };
		bool                    hasModel{ false };	// an item, which can show a picture of its model
		std::uint32_t           refID{ 0 };	// the location marker goes on this when set
		EntryTrainer            trainer;
		EntryDragon             dragon;
		EntryAchievement        achievement;
		EntryIngredient         ingredient;

		[[nodiscard]] bool          IsTrainer() const noexcept { return trainer.skill >= 0; }
		[[nodiscard]] bool          IsDragon() const noexcept { return !dragon.fate.empty(); }
		[[nodiscard]] bool          IsAchievement() const noexcept { return !achievement.game.empty(); }
		[[nodiscard]] bool          IsIngredient() const noexcept { return !ingredient.summary.empty(); }
		[[nodiscard]] std::uint32_t MarkerID() const noexcept { return refID ? refID : formID; }

		[[nodiscard]] bool IsComplete() const noexcept { return state == EntryState::kFound || state == EntryState::kDisplayed; }
	};

	struct Section
	{
		std::string        title;
		std::vector<Entry> entries;
		std::int32_t       tab{ -1 };	// index into Page::tabs, -1 when the page has none
		bool               counted{ true };	// false when another tab already counts the same things

		[[nodiscard]] std::int32_t Found() const noexcept
		{
			std::int32_t found = 0;
			for (const auto& entry : entries) {
				found += entry.IsComplete() ? 1 : 0;
			}
			return found;
		}

		[[nodiscard]] std::int32_t Total() const noexcept { return static_cast<std::int32_t>(entries.size()); }
	};

	enum class PageLayout : std::uint8_t
	{
		kList,
		kJournal,	// quests
		kKills,	// a kill list, as Kill Counts
		kMixing,	// the ingredients page's Mixing Station
	};

	// A patch's own sub-page, e.g. Quests or Gear
	struct PageTab
	{
		std::string title;
		PageLayout  layout{ PageLayout::kList };
	};

	// A patch's dropdown, e.g. which faction's quests to show
	struct PageChoice
	{
		std::string              name;
		std::string              info;
		std::vector<std::string> options;
		std::int32_t             selected{ 0 };
	};

	struct Page
	{
		std::string          id;
		std::string          title;
		std::vector<Section> sections;
		PageLayout           layout{ PageLayout::kList };
		std::string          notice{};	// translation key shown instead of entries
		PageChoice           choice{};	// no options when the patch has none
		std::vector<PageTab> tabs{};	// sections say which tab they're on
		std::string          hint{};	// a line above the entries
		std::shared_ptr<const MixingData> mixing{};	// the ingredients page's

		[[nodiscard]] PageLayout LayoutOf(const Section& a_section) const noexcept
		{
			return a_section.tab >= 0 && a_section.tab < static_cast<std::int32_t>(tabs.size()) ? tabs[a_section.tab].layout : layout;
		}
	};

	struct PageSummary
	{
		std::string  id;
		std::string  title;
		std::int32_t found{ 0 };
		std::int32_t total{ 0 };
	};

	enum class SettingKind : std::uint8_t
	{
		kHeader,
		kSpacer,
		kToggle,
		kChoice,
		kSlider,
		kText,
		kColour,	// 0xRRGGBB
		kInput,
		kKey,	// scan code, -1 unbound
		kButton,
		kList,
	};

	struct SettingItem
	{
		std::string              id;
		SettingKind              kind{ SettingKind::kText };
		std::string              label;
		std::string              info;
		std::string              value;
		bool                     on{ false };
		std::int32_t             choice{ 0 };
		std::vector<std::string> options;
		std::int32_t             number{ 0 };
		std::int32_t             minimum{ 0 };
		std::int32_t             maximum{ 1 };
		std::int32_t             step{ 1 };
		bool                     enabled{ true };
		bool                     confirm{ false };
		std::vector<std::string> keys;
	};

	struct SettingsPage
	{
		std::string              id;
		std::string              title;
		std::vector<SettingItem> columns[2];
		bool                     singleColumn{ false };
		bool                     tab{ true };
	};

	struct AboutInfo
	{
		std::string              thanks;
		std::vector<std::string> facts;
	};

	struct Category
	{
		std::string              title;
		std::string              group;	// summed on the overview
		std::vector<PageSummary> pages;

		[[nodiscard]] std::int32_t Found() const noexcept
		{
			std::int32_t found = 0;
			for (const auto& page : pages) {
				found += page.found;
			}
			return found;
		}

		[[nodiscard]] std::int32_t Total() const noexcept
		{
			std::int32_t total = 0;
			for (const auto& page : pages) {
				total += page.total;
			}
			return total;
		}

		[[nodiscard]] const std::string& Group() const noexcept { return group.empty() ? title : group; }
	};

	struct PinInfo
	{
		std::uint32_t formID{ 0 };
		std::string   name;
		std::string   pageID;
	};

	struct PinList
	{
		bool                       enabled{ false };
		std::vector<PinInfo>       pins;
		std::vector<std::uint32_t> located;	 // NPCs, items and map markers tracked from the pages

		[[nodiscard]] bool Has(std::uint32_t a_formID) const
		{
			return enabled && a_formID != 0 && std::ranges::any_of(pins, [&](const PinInfo& a_pin) { return a_pin.formID == a_formID; });
		}

		[[nodiscard]] bool Locates(std::uint32_t a_refID) const
		{
			return a_refID != 0 && std::ranges::find(located, a_refID) != located.end();
		}
	};

	enum class CellPlace : std::uint8_t
	{
		kLoose,
		kContainer,
		kCarried,
	};

	struct CellItem
	{
		std::string   name;
		std::string   type;
		std::string   collection;
		std::string   holder;	// container or NPC
		CellPlace     place{ CellPlace::kLoose };
		float         distance{ -1.0f };	// -1 when unknown
		float         bearing{ 0.0f };	// radians, 0 ahead
		std::uint32_t formID{ 0 };
		std::uint32_t refID{ 0 };	// marker goes on this
		bool          marked{ false };
		float         height{ 0.0f };
		bool          pinned{ false };
		bool          stealing{ false };
	};

	struct CellScan
	{
		bool                  scanned{ false };
		bool                  excluded{ false };
		bool                  interior{ false };
		std::string           cellName;
		std::vector<CellItem> items;
	};

	struct KillRecord
	{
		std::string  name;
		std::int32_t kills{ 0 };
		std::int32_t assists{ 0 };
		std::string  lastKill;
	};

	struct RecentEvent
	{
		std::string text;
		std::string date;	// dd/mm/yyyy
		std::string time;
	};
}
