#pragma once
#include "Serialization.hpp"

namespace CFramework_Achievements
{
	enum class Store : std::uint8_t
	{
		kNone,
		kSteam,
		kGOG,
	};

	// One of the game's achievements as the store has it, NEW_ACHIEVEMENT_<number>_0
	struct Achievement
	{
		std::int32_t  number{ 0 };
		std::string   name;	// the store's, in the player's language; empty on GOG
		std::string   description;
		bool          unlocked{ false };
		std::uint32_t time{ 0 };	// unix time, 0 when the store doesn't say
		float         percent{ -1.0f };	// of all players, -1 until Steam has it
		std::int32_t  icon{ 0 };	// Steam image handle, 0 when none
	};

	// What counts towards one on this save, as the game's own scripts check it: a misc stat ("A+B" adds
	// them), or "#..." for a quest stage, a script's counter, a faction, a global and so on
	struct Goal
	{
		std::int32_t number;
		const char*  stat;
		std::int32_t target;
		const char*  label;	// UI_Achievements_Stat_<label>
		bool         flag{ false };	// done or not, rather than a count
	};

	inline constexpr std::int32_t Count = 75;

	class CHandler
	{
	public:
		static void InstallFramework();

		[[nodiscard]] static Store StoreInUse() noexcept;
		[[nodiscard]] static bool  Refresh(std::vector<Achievement>& a_out);

		// Translation key saying why the account's can't be read, for when Refresh says no
		[[nodiscard]] static const char* WhyNot() noexcept;

		[[nodiscard]] static bool IconPixels(std::int32_t a_icon, std::vector<std::uint8_t>& a_rgba, std::uint32_t& a_width, std::uint32_t& a_height);

		// Achievements Mods Enabler or SSE Engine Fixes, without which mods keep them from unlocking
		[[nodiscard]] static bool CanUnlock() noexcept;

		[[nodiscard]] static std::span<const Goal> Goals();
		static void                                QueryStats(std::function<void()> a_changed);
		[[nodiscard]] static std::int32_t          Stat(std::string_view a_name);

		[[nodiscard]] static std::int32_t Value(std::string_view a_stat);	// -1 when unknown
	};
}
