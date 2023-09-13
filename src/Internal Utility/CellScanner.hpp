#pragma once

namespace CellScanner
{
	static inline std::vector<RE::TESObjectREFR*> MerchantChests{};

	class CHandler
	{
	public:
		static void CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool a_logging);
		
		static void AddMerchantChest(RE::TESObjectREFR* a_chest);
		static void AddExcludedChests();

		static bool IsMerchantChest(RE::TESObjectREFR* a_chest);
		static void BuildMerchantChestData(RE::TESObjectCELL* cell, const RE::TESObjectCELL::RUNTIME_DATA rtd);

	private:
		static bool ItemIsCollectable(RE::TESForm* a_form);
		static bool ItemIsCollected(RE::TESForm* a_form);
	};
}
