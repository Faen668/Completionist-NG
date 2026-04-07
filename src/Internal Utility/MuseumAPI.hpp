#pragma once

namespace Completionist
{
	class MuseumAPI
	{
	public:
		MuseumAPI(MuseumAPI const&) = delete;
		MuseumAPI(MuseumAPI const&&) = delete;
		MuseumAPI operator=(MuseumAPI&) = delete;
		MuseumAPI operator=(MuseumAPI&&) = delete;

		static void Register();
		static bool	RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);
		static void OnPlayerLoadGame();
		static void VerifyAltLists();

		static void BuildAllVariations();
		static void BuildMuseumInventory();

		static bool SetDisplayable(RE::FormID a_formID, std::unordered_set<RE::FormID> a_variations);
		static void SetDisplayable(RE::TESForm* a_form, std::unordered_set<RE::TESForm*> a_variations);

		static bool IsMuseumDisplayable(RE::TESForm* a_form);
		static bool IsMuseumDisplayable(RE::FormID a_formID);

		static bool HasVariations(RE::TESForm* a_form);
		static bool HasVariations(RE::FormID a_formID);

		static std::unordered_set<RE::FormID> GetVariations(RE::FormID a_formID);

		static bool IsDisplayed(RE::FormID a_formID);
		static bool IsDisplayed(RE::TESForm* a_form);
		static bool IsDisplayed(RE::FormID a_formID, bool& matchedByVariation);
		static bool IsDisplayed(RE::TESForm* a_form, bool& matchedByVariation);

		static void OnContainerChangedEvent(RE::TESContainerChangedEvent const* a_event);		
		static void InsertPatchReplacedItems();

		static void FindAndTrackMuseumItems();
		static void FindAndTrackMuseumPatchItems();
		static std::unordered_set<RE::TESForm*> GetReplicaItems(const RE::TESForm* element, const RE::BGSListForm* replicaBaseItems, const RE::BGSListForm* replicaItems);

		// Papyrus Functions
		static RE::BSFixedString GetMuseumDisplayStatus(RE::StaticFunctionTag*, RE::TESForm* a_form);

	private:
		MuseumAPI() = default;
		~MuseumAPI() = default;

		inline static std::unordered_map<RE::FormID, int32_t> _museumInventory;
		inline static std::unordered_set<RE::FormID> _museumContainers;
		inline static std::unordered_map<RE::FormID, std::unordered_set<RE::FormID>> _AllVariations;
		inline static std::unordered_map<RE::FormID, std::unordered_set<RE::FormID>> _DisplayableFormIDs;

		inline static RE::TESDataHandler* _handler;
		inline static RE::BSScript::Internal::VirtualMachine* _vm;
		inline static RE::BSScript::IObjectHandlePolicy* _policy;
		inline static bool _logging;
		inline static bool _ready;

		inline static constexpr std::pair<RE::FormID, std::string_view> museumItemListsMap[] = {
			{0x161CE6, "LegacyoftheDragonborn.esm"},	// Ancient Nord
			{0x161CE7, "LegacyoftheDragonborn.esm"},	// Blades
			{0x161CE8, "LegacyoftheDragonborn.esm"},	// Daedric
			{0x161CE9, "LegacyoftheDragonborn.esm"},	// Dawnguard
			{0x161CEA, "LegacyoftheDragonborn.esm"},	// Dragon
			{0x161CEB, "LegacyoftheDragonborn.esm"},	// Dwarven
			{0x161CEC, "LegacyoftheDragonborn.esm"},	// Ebony
			{0x161CED, "LegacyoftheDragonborn.esm"},	// Elven								
			{0x161CEF, "LegacyoftheDragonborn.esm"},	// Falmer
			{0x161CF0, "LegacyoftheDragonborn.esm"},	// Forsworn
			{0x161CF1, "LegacyoftheDragonborn.esm"},	// Glass
			{0x08F99E, "LegacyoftheDragonborn.esm"},	// Guard Armor
			{0x161CF2, "LegacyoftheDragonborn.esm"},	// Iron
			{0x161CEE, "LegacyoftheDragonborn.esm"},	// Extra / Misc
			{0x161CF3, "LegacyoftheDragonborn.esm"},	// Nordic
			{0x161CF4, "LegacyoftheDragonborn.esm"},	// Orcish
			{0x161CF5, "LegacyoftheDragonborn.esm"},	// Snowelf
			{0x161CF6, "LegacyoftheDragonborn.esm"},	// Stalhrim
			{0x161CF7, "LegacyoftheDragonborn.esm"},	// Steel
			{0x161CE1, "LegacyoftheDragonborn.esm"},	// Thane Weapons
			{0x1484A0, "LegacyoftheDragonborn.esm"},	// Daedric Gallery
			{0x152948, "LegacyoftheDragonborn.esm"},	// Hall of Achievements
			{0x06642A, "LegacyoftheDragonborn.esm"},	// Guildhouse
			{0x14D7DB, "LegacyoftheDragonborn.esm"},	// Hall of Lost Empires (Main Floor)
			{0x1387A5, "LegacyoftheDragonborn.esm"},	// Hall of Lost Empires (Upper Ring)
			{0x152944, "LegacyoftheDragonborn.esm"},	// Hall of Oddities
			{0x15CBC4, "LegacyoftheDragonborn.esm"},	// Hall of Secrets
			{0x415118, "LegacyoftheDragonborn.esm"},	// Gallery of Natural Science (Gemstones)
			{0x33D88C, "LegacyoftheDragonborn.esm"},	// Gallery of Natural Science (Shells)
			{0x15CBC2, "LegacyoftheDragonborn.esm"},	// Gallery of Natural Science (Animals)
			{0x0F0AC8, "LegacyoftheDragonborn.esm"},	// Gallery of Natural Science (Fossils)
			{0x0AE79D, "LegacyoftheDragonborn.esm"},	// Storeroom
			{0x1387AD, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Ground Floor - Left)
			{0x14CEF2, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Ground Floor - Right)
			{0x13D915, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Jewelry Cabinet)
			{0x13D911, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Upper Gallery)
			{0x1387AB, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Masks & Claws)
			{0x13D917, "LegacyoftheDragonborn.esm"},	// Hall of Heroes (Reception)
			{0x1387A1, "LegacyoftheDragonborn.esm"},	// Library (Lower Floor - Left)
			{0x13879B, "LegacyoftheDragonborn.esm"},	// Library (Lower Floor - Right)
			{0x1387A4, "LegacyoftheDragonborn.esm"},	// Library (Treasure Maps)
			{0x13879D, "LegacyoftheDragonborn.esm"},	// Library (Upper Floor)
			{0x13879F, "LegacyoftheDragonborn.esm"},	// Library (Rare Books)
		};

		inline static constexpr std::tuple<RE::FormID, std::string_view, std::string_view> museumItemAltListsMap[] = {
			{0x161D07, "LegacyoftheDragonborn.esm", "Thane Weapons Alt"},							// Thane Weapons Alt
			{0x1484AF, "LegacyoftheDragonborn.esm", "Daedric Gallery Alt"},							// Daedric Gallery Alt
			{0x161D0D, "LegacyoftheDragonborn.esm", "Hall of Achievements Alt"},					// Hall of Achievements Alt
			{0x066463, "LegacyoftheDragonborn.esm", "Guildhouse Alt"},								// Guildhouse Alt
			{0x152942, "LegacyoftheDragonborn.esm", "Hall of Lost Empires (Main Floor) Alt"},		// Hall of Lost Empires (Main Floor) Alt
			{0x0B3A1E, "LegacyoftheDragonborn.esm", "Hall of Lost Empires (Upper Ring) Alt"},		// Hall of Lost Empires (Upper Ring) Alt
			{0x161D0C, "LegacyoftheDragonborn.esm", "Hall of Oddities Alt"},						// Hall of Oddities Alt
			{0x15CBC6, "LegacyoftheDragonborn.esm", "Hall of Secrets Alt"},							// Hall of Secrets Alt
			{0x13D90E, "LegacyoftheDragonborn.esm", "Hall of Heroes (Ground Floor - Left) Alt"},	// Hall of Heroes (Ground Floor - Left) Alt
			{0x13D90F, "LegacyoftheDragonborn.esm", "Hall of Heroes (Ground Floor - Right) Alt"},	// Hall of Heroes (Ground Floor - Right) Alt
			{0x14849E, "LegacyoftheDragonborn.esm", "Hall of Heroes (Jewelry Cabinet) Alt"},		// Hall of Heroes (Jewelry Cabinet) Alt
			{0x14338B, "LegacyoftheDragonborn.esm", "Hall of Heroes (Upper Gallery) Alt"},			// Hall of Heroes (Upper Gallery) Alt
			{0x161D0A, "LegacyoftheDragonborn.esm", "Hall of Heroes (Masks & Claws) Alt"},			// Hall of Heroes (Masks & Claws) Alt
			{0x0B3A1C, "LegacyoftheDragonborn.esm", "Hall of Heroes (Reception) Alt"},				// Hall of Heroes (Reception) Alt
			{0x0B3A1D, "LegacyoftheDragonborn.esm", "Library (Treasure Maps) Alt"},					// Library (Treasure Maps) Alt
			{0x03B351, "LegacyoftheDragonborn.esm", "Library (Upper Floor) Alt"},					// Library (Upper Floor) Alt
		};

		// Hardcoded variations for patch replaced items that have not been modified in ReplicaBaseItems.
		inline static std::tuple<RE::FormID, std::string_view, std::vector<std::pair<RE::FormID, std::string_view>>> patchedItemMappings[] = {
			{0x06A9F0, "Hothtrooper44_ArmorCompilation.esp", { {0x73C752, "LegacyoftheDragonborn.esm" } } },
		};
	};
}