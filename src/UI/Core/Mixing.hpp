#pragma once

#include "UI/Core/UIModel.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace CompletionistUI
{
	// One effect of a brew, from the ingredient whose version of it is the strongest
	struct BrewEffect
	{
		const MixEffect* effect{ nullptr };
		std::int32_t     magnitude{ 0 };
		std::int32_t     duration{ 0 };
		std::int32_t     value{ 0 };
		bool             known{ false };	// learnt on at least one of the ingredients
	};

	// What two or three ingredients make, as the alchemy table would brew it
	struct Brew
	{
		std::vector<BrewEffect> effects;	// strongest first, the first names it
		bool                    poison{ false };
		std::int32_t            value{ 0 };
		std::int32_t            teaches{ 0 };	// effects it would teach on the ingredients used
		bool                    wasted{ false };	// an ingredient shares nothing with the others

		[[nodiscard]] bool Empty() const noexcept { return effects.empty(); }
	};

	struct Recipe
	{
		std::vector<const MixIngredient*>    ingredients;
		Brew                                 brew;
		std::pair<std::int32_t, std::int32_t> strength{};	// the main effect's value, then the brew's
	};

	// Everything that makes one potion or poison, named for its strongest effect
	struct PotionGroup
	{
		const MixEffect*    main{ nullptr };
		bool                poison{ false };
		std::vector<Recipe> variations;	// the strongest first
		std::int32_t        best{ 0 };
	};

	class Mixing
	{
	public:
		[[nodiscard]] static Brew BrewOf(const MixingData& a_data, const std::vector<const MixIngredient*>& a_ingredients);

		// What can be brewed from what's carried, a group per potion or poison with its
		// strongest ways of making it
		[[nodiscard]] static std::vector<PotionGroup> Groups(const MixingData& a_data, std::size_t a_perGroup);

		// The game's description with the numbers in
		[[nodiscard]] static std::string Describe(const BrewEffect& a_effect);
	};
}
