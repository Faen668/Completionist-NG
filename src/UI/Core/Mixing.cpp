#include "UI/Core/Mixing.hpp"

#include <algorithm>

namespace CompletionistUI
{
	//---------------------------------------------------
	//-- Mixing ( Brew Of ) -----------------------------
	//---------------------------------------------------

	// The effects at least two of them share, each from the ingredient with the dearest
	// version. The most valuable names the brew and makes it a potion or a poison; with
	// Purity, the effects of the other kind are left out
	Brew Mixing::BrewOf(const MixingData& a_data, const std::vector<const MixIngredient*>& a_ingredients) {

		Brew brew;
		struct Holder
		{
			const MixIngredient* ingredient;
			const MixEffect*     effect;
		};
		std::vector<std::pair<std::uint32_t, std::vector<Holder>>> byEffect;
		for (const auto* ingredient : a_ingredients) {
			if (!ingredient) {
				continue;
			}
			for (const auto& effect : ingredient->effects) {
				auto it = std::ranges::find(byEffect, effect.id, &std::pair<std::uint32_t, std::vector<Holder>>::first);
				if (it == byEffect.end()) {
					it = byEffect.insert(byEffect.end(), { effect.id, {} });
				}
				it->second.push_back({ ingredient, std::addressof(effect) });
			}
		}

		std::vector<const MixIngredient*> used;
		std::vector<std::pair<const MixEffect*, bool>> shared;
		for (const auto& [id, holders] : byEffect) {
			if (holders.size() < 2) {
				continue;
			}
			const auto* strongest = std::ranges::max_element(holders, {}, [](const Holder& a_holder) { return a_holder.effect->priority; })->effect;
			bool known = false;
			for (const auto& holder : holders) {
				known |= holder.effect->known;
				brew.teaches += holder.effect->known ? 0 : 1;
				if (std::ranges::find(used, holder.ingredient) == used.end()) {
					used.push_back(holder.ingredient);
				}
			}
			shared.emplace_back(strongest, known);
		}
		brew.wasted = !shared.empty() && std::ranges::count_if(a_ingredients, [](const MixIngredient* a_ingredient) { return a_ingredient != nullptr; }) > static_cast<std::ptrdiff_t>(used.size());
		if (shared.empty()) {
			return brew;
		}

		const auto main = std::ranges::max_element(shared, {}, [](const auto& a_shared) { return a_shared.first->value[0]; });
		brew.poison = main->first->hostile;
		const int kind = brew.poison ? 2 : 1;
		for (const auto& [effect, known] : shared) {
			if (a_data.purity && effect->hostile != brew.poison) {
				continue;
			}
			brew.effects.push_back({ effect, effect->magnitude[kind], effect->duration[kind], effect->value[kind], known });
			brew.value += effect->value[kind];
		}
		std::ranges::stable_sort(brew.effects, std::greater{}, &BrewEffect::value);
		return brew;
	}

	//---------------------------------------------------
	//-- Mixing ( Groups ) ------------------------------
	//---------------------------------------------------

	// Every pair and three from what's carried, sorted by the potion or poison they make, each
	// with its strongest ways of making it. A
	// three counts only when each of them adds something and it's worth more than any two
	// of them. Each brew is first weighed without building it, since a full pack makes
	// about a million threes; only the ones a group keeps are brewed in full
	std::vector<PotionGroup> Mixing::Groups(const MixingData& a_data, std::size_t a_perGroup) {

		std::vector<const MixIngredient*> carried;
		for (const auto& ingredient : a_data.ingredients) {
			if (ingredient.carried > 0) {
				carried.push_back(std::addressof(ingredient));
			}
		}

		struct Weighed
		{
			const MixEffect* main{ nullptr };
			bool             poison{ false };
			std::int32_t     value{ 0 };
			std::int32_t     strength{ 0 };	// the main effect's alone
			bool             allUsed{ true };
		};
		const auto weigh = [&](const MixIngredient* const* a_mix, int a_count) {
			Weighed out;
			const MixEffect* strongest[12]{};
			int              shared = 0;
			bool             used[3]{};
			for (int a = 0; a < a_count; ++a) {
				for (const auto& effect : a_mix[a]->effects) {
					// Each shared effect once, from the first ingredient that has it
					bool seen = false;
					for (int s = 0; s < shared && !seen; ++s) {
						seen = strongest[s]->id == effect.id;
					}
					if (seen) {
						continue;
					}
					const MixEffect* best = std::addressof(effect);
					int              holders = 1;
					for (int b = a + 1; b < a_count; ++b) {
						for (const auto& other : a_mix[b]->effects) {
							if (other.id == effect.id) {
								++holders;
								used[b] = true;
								if (other.priority > best->priority) {
									best = std::addressof(other);
								}
							}
						}
					}
					if (holders >= 2) {
						used[a] = true;
						strongest[shared++] = best;
					}
				}
			}
			for (int a = 0; a < a_count; ++a) {
				out.allUsed &= used[a];
			}
			for (int s = 0; s < shared; ++s) {
				if (!out.main || strongest[s]->value[0] > out.main->value[0]) {
					out.main = strongest[s];
				}
			}
			if (!out.main) {
				return out;
			}
			out.poison = out.main->hostile;
			const int kind = out.poison ? 2 : 1;
			out.strength = out.main->value[kind];
			for (int s = 0; s < shared; ++s) {
				if (!a_data.purity || strongest[s]->hostile == out.poison) {
					out.value += strongest[s]->value[kind];
				}
			}
			return out;
		};

		std::vector<PotionGroup> groups;
		const auto               add = [&](const Weighed& a_weighed, std::vector<const MixIngredient*> a_mix) {
            auto it = std::ranges::find_if(groups, [&](const PotionGroup& a_group) { return a_group.main->id == a_weighed.main->id && a_group.poison == a_weighed.poison; });
            if (it == groups.end()) {
                it = groups.insert(groups.end(), PotionGroup{ a_weighed.main, a_weighed.poison });
            }
            it->best = std::max(it->best, a_weighed.value);
            // Strongest first: the potion's own effect, then what the rest add
            auto&      kept = it->variations;
            const auto rank = std::pair(a_weighed.strength, a_weighed.value);
            if (kept.size() >= a_perGroup && kept.back().strength >= rank) {
                return;
            }
            auto       brew = BrewOf(a_data, a_mix);
            const auto at = std::ranges::upper_bound(kept, rank, std::greater{}, &Recipe::strength);
            kept.insert(at, Recipe{ std::move(a_mix), std::move(brew), rank });
            if (kept.size() > a_perGroup) {
                kept.pop_back();
            }
		};

		const auto                count = carried.size();
		std::vector<std::int32_t> pairValue(count * count, 0);
		for (std::size_t i = 0; i < count; ++i) {
			for (std::size_t j = i + 1; j < count; ++j) {
				const MixIngredient* mix[] = { carried[i], carried[j] };
				const auto           weighed = weigh(mix, 2);
				if (weighed.main) {
					pairValue[i * count + j] = std::max(weighed.value, 1);
					add(weighed, { carried[i], carried[j] });
				}
			}
		}
		for (std::size_t i = 0; i < count; ++i) {
			for (std::size_t j = i + 1; j < count; ++j) {
				for (std::size_t k = j + 1; k < count; ++k) {
					const auto best = std::max({ pairValue[i * count + j], pairValue[i * count + k], pairValue[j * count + k] });
					if (best == 0) {
						// No two of them share anything, so the three don't either
						continue;
					}
					const MixIngredient* mix[] = { carried[i], carried[j], carried[k] };
					const auto           weighed = weigh(mix, 3);
					if (weighed.main && weighed.allUsed && weighed.value > best) {
						add(weighed, { carried[i], carried[j], carried[k] });
					}
				}
			}
		}

		// Potions then poisons, each by name
		std::ranges::sort(groups, [](const PotionGroup& a_left, const PotionGroup& a_right) {
			return a_left.poison != a_right.poison ? a_right.poison : a_left.main->name < a_right.main->name;
		});
		return groups;
	}

	//---------------------------------------------------
	//-- Mixing ( Describe ) ----------------------------
	//---------------------------------------------------

	std::string Mixing::Describe(const BrewEffect& a_effect) {

		std::string text = a_effect.effect ? a_effect.effect->text : std::string{};
		const auto  fill = [&](std::string_view a_tag, std::int32_t a_value) {
            for (auto at = text.find(a_tag); at != std::string::npos; at = text.find(a_tag, at)) {
                const auto number = std::to_string(a_value);
                text.replace(at, a_tag.size(), number);
                at += number.size();
            }
		};
		fill("<mag>", a_effect.magnitude);
		fill("<dur>", a_effect.duration);
		return text;
	}
}
