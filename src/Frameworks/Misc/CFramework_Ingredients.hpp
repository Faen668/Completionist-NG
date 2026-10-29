#pragma once
#include "Serialization.hpp"

namespace CFramework_Ingredients
{
	// An ingredient and the key its lore is under, empty for ones the lore doesn't cover
	struct Ingredient
	{
		std::string         key;
		std::string         plugin;
		std::string         source;	// translation key for the game and DLC, a Creation's title
		RE::IngredientItem* form{ nullptr };
	};

	inline std::vector<Ingredient> Ingredients;

	class CHandler
	{
	public:
		static void InstallFramework();

		[[nodiscard]] static bool        Knows(const RE::IngredientItem* a_ingredient, std::uint32_t a_index);
		[[nodiscard]] static std::string EffectKey(const RE::EffectSetting* a_effect);	// empty for mod effects
	};
}
