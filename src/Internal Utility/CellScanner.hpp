#pragma once

//Array used to hold data for references that should not be included when scanning.
constexpr std::array<std::tuple<RE::FormID, const char*, const char*>, 28> ExcludedModAddedReferences =
{{
	{0x0009C0, "cceejsse003-hollow.esl", "Shadowfoot Sanctum Test Urn"},							// Shadowfoot Sanctum
	{0x1223DF, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Belethor's General Goods"},			// Fishing Creation
	{0x173B66, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Gray Pine Goods"},					// Fishing Creation
	{0x173B67, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Bits and Pieces"},					// Fishing Creation
	{0x173B6B, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Riverwood Trader"},					// Fishing Creation
	{0x173B6A, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Birna's Oddments"},					// Fishing Creation
	{0x173B6D, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Arnleif and Sons Trading Company"},	// Fishing Creation
	{0x173B6C, "ccbgssse001-fish.esm", "Fishing Vendor Chest - Pawned Prawn"},						// Fishing Creation
	{0x345c0e, "LKVM Main House.esp", "Test Chest"},												// Lakeview Manor As It Should Be
	{0x27a88f, "LKVM Cellar and Exterior.esp", "Test Chest"},										// Lakeview Manor As It Should Be - Cellar Addon
	{0x2d4912, "Vigilant.esm", "Hidden Chest"},														// Vigilant
	{0x04C743, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C744, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C745, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C746, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C748, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C749, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x04C74A, "Traveling Merchant Caravans.esp", "Merchant Chest"},								// Traveling Merchant Caravans
	{0x2F38F5, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - AmrasMerchantChest
	{0x2F38F8, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - AmrasMerchantChest
	{0x2F38FB, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - AmrasMerchantChest
	{0x0DBDB1, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x23C0DA, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x23C0DB, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x23C0DC, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x23C0DD, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x23C0DE, "3DNPC.esp", "Merchant Chest"},														// 3DNPC - MerchantFalkreathApprentice
	{0x01DE83, "SL01AmuletsSkyrim.esp", "Merchant Chest"},											// AOS - SL00MerchantGenericAmuletChest
}};

//Array used to hold data for references that should not be included when scanning.
constexpr std::array<std::tuple<RE::FormID, const char*, const char*>, 26> ModAddedCells =
{ {
	{0x1252E1, "LegacyoftheDragonborn.esm", "LOTD - QA Room 1"},							// Legacy of the Dragonborn
	{0x18F66E, "LegacyoftheDragonborn.esm", "LOTD - QA Room 2"},							// Legacy of the Dragonborn
	{0x6A9A13, "LegacyoftheDragonborn.esm", "LOTD - QA Room 3"},							// Legacy of the Dragonborn
	{0x168884, "LegacyoftheDragonborn.esm", "LOTD - Armoury"},								// Legacy of the Dragonborn
	{0x167B25, "LegacyoftheDragonborn.esm", "LOTD - Armoury (East)"},						// Legacy of the Dragonborn
	{0x2B487A, "LegacyoftheDragonborn.esm", "LOTD - Armoury (West)"},						// Legacy of the Dragonborn
	{0x0AEACE, "LegacyoftheDragonborn.esm", "LOTD - Safehouse"},							// Legacy of the Dragonborn
	{0x18F863, "LegacyoftheDragonborn.esm", "LOTD - Planetarium"},							// Legacy of the Dragonborn
	{0x125EE6, "LegacyoftheDragonborn.esm", "LOTD - Hall of Heroes"},						// Legacy of the Dragonborn
	{0x2AE7D5, "LegacyoftheDragonborn.esm", "LOTD - Hall of Heroes - Heist"},				// Legacy of the Dragonborn
	{0x1ABB45, "LegacyoftheDragonborn.esm", "LOTD - Hall of Heroes - Haunted"},				// Legacy of the Dragonborn
	{0x062832, "LegacyoftheDragonborn.esm", "LOTD - Explorers Society Guildhouse"},			// Legacy of the Dragonborn
	{0x2F4A2E, "LegacyoftheDragonborn.esm", "LOTD - Fieldstation 1"},						// Legacy of the Dragonborn
	{0x1D5A02, "LegacyoftheDragonborn.esm", "LOTD - Fieldstation 2"},						// Legacy of the Dragonborn
	{0x28EE1E, "LegacyoftheDragonborn.esm", "LOTD - Fieldstation 3"},						// Legacy of the Dragonborn
	{0x2F8D08, "LegacyoftheDragonborn.esm", "LOTD - Library"},								// Legacy of the Dragonborn
	{0x005955, "LegacyoftheDragonborn.esm", "LOTD - Deepholme"},							// Legacy of the Dragonborn
	{0x37BF27, "LegacyoftheDragonborn.esm", "LOTD - Dev Aveza"},							// Legacy of the Dragonborn
	{0x741ACF, "LegacyoftheDragonborn.esm", "LOTD - Dragonborn Hall"},						// Legacy of the Dragonborn
	{0x125C71, "LegacyoftheDragonborn.esm", "LOTD - East Exhibit Halls"},					// Legacy of the Dragonborn
	{0x0A9278, "LegacyoftheDragonborn.esm", "LOTD - Hall of Secrets 1"},					// Legacy of the Dragonborn
	{0x18F4FB, "LegacyoftheDragonborn.esm", "LOTD - Hall of Secrets 2"},					// Legacy of the Dragonborn
	{0x12517D, "LegacyoftheDragonborn.esm", "LOTD - Storeroom"},							// Legacy of the Dragonborn
	{0x1D5B20, "LegacyoftheDragonborn.esm", "LOTD - Storeroom Sealed Chamber"},				// Legacy of the Dragonborn
	{0x1256C8, "LegacyoftheDragonborn.esm", "LOTD - Gallery of Natural Science"},			// Legacy of the Dragonborn
	{0x17CBFE, "LegacyoftheDragonborn.esm", "LOTD - Gallery of Natural Science - Haunted"},	// Legacy of the Dragonborn
} };

namespace CellScanner
{
	class CHandler
	{
	public:
		static void CheckForReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, bool b_logging, bool b_notify);
		static std::string GetFormType(RE::TESForm* a_form);

		static void ExcludeAllVendorChests();
		static void AddExcludedReferencesFromMods();
		static bool IsExcludedFormType(RE::TESForm* a_form);
		static bool IsReferenceExcluded(RE::TESObjectREFR* a_chest);

		static std::string GetQuestMarkerReferenceFormID(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static std::string GetQuestMarkerReferenceOwner(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static std::string GetQuestMarkerReferenceIndex(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
		static void ExcludeReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectCELL* cell);
		static void RemoveExcludedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

		//For Excluded References
		static std::vector<RE::TESObjectREFR*> GetObjectReferences(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceFormIDs(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static std::vector<std::string> GetReferenceNames(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);

		//For Retrieving The Targetted Reference.
		static bool HasPinnedFormInCell(RE::StaticFunctionTag*, RE::TESObjectCELL* cell, RE::TESForm* a_form);
		static bool IsItemPinnable(RE::StaticFunctionTag*, RE::TESForm* a_form);

		static std::string	GetPinnedReferenceType(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static std::string	GetPinnedReferenceName(RE::StaticFunctionTag*, RE::TESForm* a_form);
		static RE::TESObjectREFR* GetPinnedReferenceRefr(RE::StaticFunctionTag*, RE::TESForm* a_form);

		static RE::TESForm* GetTargetReferenceForm(RE::StaticFunctionTag*);
		static std::string	GetTargetReferenceType(RE::StaticFunctionTag*);
		static std::string	GetTargetReferenceName(RE::StaticFunctionTag*);
		static RE::TESObjectREFR* GetTargetReferenceRefr(RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, RE::TESForm* a_lastForm);

		

		static bool isCellExcluded(RE::StaticFunctionTag*, RE::TESObjectCELL* cell);
		static void ScanCell(RE::TESObjectCELL* cell, bool b_logging, bool b_notify);

		static int GetRandomIndex(const int32_t size)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, (size - 1));
			return dist(gen);
		}
	private:
		static bool ItemIsCollectable(RE::TESForm* a_form);
		static bool ItemIsCollected(RE::TESForm* a_form);
	};
}
