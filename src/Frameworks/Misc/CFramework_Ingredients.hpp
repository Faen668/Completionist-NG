#pragma once
#include "Serialization.hpp"

namespace CFramework_Ingredients
{
	// The page's sections, three tabs of them, as the SkyrimIngredients add-on laid them out
	enum class Group : std::uint8_t
	{
		kAnimalParts,
		kEggs,
		kFish,
		kFlowers,
		kInsects,
		kMushrooms,
		kPlants,
		kOther,
		kTotal
	};

	// An ingredient and the key its lore is under, empty for ones the lore doesn't cover
	struct Ingredient
	{
		std::string         key;
		std::string         plugin;
		std::string         source;	// translation key for the game and DLC, a Creation's title
		RE::IngredientItem* form{ nullptr };
		Group               group{ Group::kOther };
	};

	inline std::vector<Ingredient> Ingredients;
	inline std::unordered_set<RE::FormID> Covered;	// the listed ones and the same-named records folded into them

	class CHandler
	{
	public:
		static void InstallFramework();

		[[nodiscard]] static bool        Knows(const RE::IngredientItem* a_ingredient, std::uint32_t a_index);
		[[nodiscard]] static std::string EffectKey(const RE::EffectSetting* a_effect);	// empty for mod effects
	};
}
