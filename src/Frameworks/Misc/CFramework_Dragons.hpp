#pragma once
#include "Serialization.hpp"

namespace CFramework_Dragons
{
	enum class Fate : std::uint8_t
	{
		kNotMet,
		kMet,
		kTamed,
		kAlly,
		kSlain,
	};

	// A named dragon, every record the game uses for it
	struct Dragon
	{
		std::string                      key;	// lore keys, empty for dragons the lore doesn't cover
		std::string                      name;
		std::string                      plugin;
		RE::FormID                       formID{ 0 };	// for marking by hand
		std::vector<RE::TESNPC*>         npcs;
		std::vector<RE::ObjectRefHandle> refs;
		bool                             known{ false };	// on the list below, kept even when nobody's placed
	};

	inline std::vector<Dragon> Dragons;

	// Marking by hand, clear of the trainers' 100
	inline constexpr std::int32_t FrameworkID = 101;

	class CHandler
	{
	public:
		static void InstallFramework();
		static void FindPlacedDragons();

		[[nodiscard]] static Fate        FateOf(const Dragon& a_dragon);
		[[nodiscard]] static bool        IsDone(const Dragon& a_dragon, Fate a_fate);
		[[nodiscard]] static bool        IsDragon(const RE::Actor* a_actor);
		[[nodiscard]] static const char* FateKey(Fate a_fate);
	};
}
