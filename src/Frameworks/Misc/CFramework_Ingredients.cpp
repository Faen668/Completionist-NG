#include "CFramework_Ingredients.hpp"

namespace CFramework_Ingredients
{
	namespace
	{
		// The game's, its DLC's and the Creations' ingredients by record, for their lore. Mods'
		// ingredients are left to their patches. The Rare Curios roots are in Update.esm for
		// everyone, but only the Creation brings them into the game
		struct Known
		{
			const char* key;
			const char* plugin;
			RE::FormID  id;
		};

		constexpr Known known[] = {
			{ "AbeceanLongfin", "Skyrim.esm", 0x106E1B },
			{ "BearClaws", "Skyrim.esm", 0x06BC02 },
			{ "Bee", "Skyrim.esm", 0x0A9195 },
			{ "BeehiveHusk", "Skyrim.esm", 0x0A9191 },
			{ "BeritsAshes", "Skyrim.esm", 0x0705B7 },
			{ "BleedingCrown", "Skyrim.esm", 0x04DA20 },
			{ "Blisterwort", "Skyrim.esm", 0x04DA25 },
			{ "BlueButterflyWing", "Skyrim.esm", 0x0727DE },
			{ "BlueDartwing", "Skyrim.esm", 0x0E4F0C },
			{ "BlueMountainFlower", "Skyrim.esm", 0x077E1C },
			{ "BoneMeal", "Skyrim.esm", 0x034CDD },
			{ "BriarHeart", "Skyrim.esm", 0x03AD61 },
			{ "ButterflyWing", "Skyrim.esm", 0x0727E0 },
			{ "CanisRoot", "Skyrim.esm", 0x06ABCB },
			{ "CharredSkeeverHide", "Skyrim.esm", 0x052695 },
			{ "ChaurusEggs", "Skyrim.esm", 0x03AD56 },
			{ "ChickensEgg", "Skyrim.esm", 0x023D77 },
			{ "CreepCluster", "Skyrim.esm", 0x0B2183 },
			{ "CrimsonNirnroot", "Skyrim.esm", 0x0B701A },
			{ "CyrodilicSpadetail", "Skyrim.esm", 0x106E19 },
			{ "DaedraHeart", "Skyrim.esm", 0x03AD5B },
			{ "Deathbell", "Skyrim.esm", 0x0516C8 },
			{ "DragonsTongue", "Skyrim.esm", 0x0889A2 },
			{ "DwarvenOil", "Skyrim.esm", 0x0F11C0 },
			{ "Ectoplasm", "Skyrim.esm", 0x03AD63 },
			{ "ElvesEar", "Skyrim.esm", 0x034D31 },
			{ "EyeOfSabreCat", "Skyrim.esm", 0x06BC07 },
			{ "FalmerEar", "Skyrim.esm", 0x03AD5D },
			{ "FireSalts", "Skyrim.esm", 0x03AD5E },
			{ "FlyAmanita", "Skyrim.esm", 0x04DA00 },
			{ "FrostMirriam", "Skyrim.esm", 0x034D32 },
			{ "FrostSalts", "Skyrim.esm", 0x03AD5F },
			{ "Garlic", "Skyrim.esm", 0x034D22 },
			{ "GiantLichen", "Skyrim.esm", 0x07E8C1 },
			{ "GiantsToe", "Skyrim.esm", 0x03AD64 },
			{ "GlowDust", "Skyrim.esm", 0x03AD73 },
			{ "GlowingMushroom", "Skyrim.esm", 0x07EE01 },
			{ "GrassPod", "Skyrim.esm", 0x083E64 },
			{ "HagravenClaw", "Skyrim.esm", 0x06B689 },
			{ "HagravenFeathers", "Skyrim.esm", 0x03AD66 },
			{ "HangingMoss", "Skyrim.esm", 0x057F91 },
			{ "HawkBeak", "Skyrim.esm", 0x0E7EBC },
			{ "HawkFeathers", "Skyrim.esm", 0x0E7ED0 },
			{ "Histcarp", "Skyrim.esm", 0x106E18 },
			{ "Honeycomb", "Skyrim.esm", 0x0B08C5 },
			{ "HumanFlesh", "Skyrim.esm", 0x1016B3 },
			{ "HumanHeart", "Skyrim.esm", 0x0B18CD },
			{ "IceWraithTeeth", "Skyrim.esm", 0x03AD6A },
			{ "ImpStool", "Skyrim.esm", 0x04DA23 },
			{ "JarrinRoot", "Skyrim.esm", 0x01BCBC },
			{ "JazbayGrapes", "Skyrim.esm", 0x06AC4A },
			{ "JuniperBerries", "Skyrim.esm", 0x05076E },
			{ "LargeAntlers", "Skyrim.esm", 0x06BC0A },
			{ "Lavender", "Skyrim.esm", 0x045C28 },
			{ "LunaMothWing", "Skyrim.esm", 0x0727DF },
			{ "MoonSugar", "Skyrim.esm", 0x0D8E3F },
			{ "MoraTapinella", "Skyrim.esm", 0x0EC870 },
			{ "MudcrabChitin", "Skyrim.esm", 0x06BC00 },
			{ "NamirasRot", "Skyrim.esm", 0x04DA24 },
			{ "Nightshade", "Skyrim.esm", 0x02F44C },
			{ "Nirnroot", "Skyrim.esm", 0x059B86 },
			{ "NordicBarnacle", "Skyrim.esm", 0x07EDF5 },
			{ "OrangeDartwing", "Skyrim.esm", 0x0BB956 },
			{ "Pearl", "Skyrim.esm", 0x0854FE },
			{ "PineThrushEgg", "Skyrim.esm", 0x023D6F },
			{ "PowderedMammothTusk", "Skyrim.esm", 0x06BC10 },
			{ "PurpleMountainFlower", "Skyrim.esm", 0x077E1E },
			{ "RedMountainFlower", "Skyrim.esm", 0x077E1D },
			{ "RiverBetty", "Skyrim.esm", 0x106E1A },
			{ "RockWarblerEgg", "Skyrim.esm", 0x07E8C8 },
			{ "SabreCatTooth", "Skyrim.esm", 0x06BC04 },
			{ "SaltPile", "Skyrim.esm", 0x034CDF },
			{ "SaltPile", "Skyrim.esm", 0x074A19 },	// Black-Briar Meadery's, a second record of it
			{ "ScalyPholiota", "Skyrim.esm", 0x06F950 },
			{ "SilversidePerch", "Skyrim.esm", 0x106E1C },
			{ "SkeeverTail", "Skyrim.esm", 0x03AD6F },
			{ "SlaughterfishEgg", "Skyrim.esm", 0x07E8C5 },
			{ "SlaughterfishScales", "Skyrim.esm", 0x03AD70 },
			{ "SmallAntlers", "Skyrim.esm", 0x06BC0B },
			{ "SmallPearl", "Skyrim.esm", 0x085500 },
			{ "Snowberries", "Skyrim.esm", 0x01B3BD },
			{ "SpiderEgg", "Skyrim.esm", 0x09151B },
			{ "SprigganSap", "Skyrim.esm", 0x063B5F },
			{ "SwampFungalPod", "Skyrim.esm", 0x07E8B7 },
			{ "Taproot", "Skyrim.esm", 0x03AD71 },
			{ "ThistleBranch", "Skyrim.esm", 0x0134AA },
			{ "TorchbugThorax", "Skyrim.esm", 0x04DA73 },
			{ "TrollFat", "Skyrim.esm", 0x03AD72 },
			{ "TundraCotton", "Skyrim.esm", 0x03F7F8 },
			{ "VampireDust", "Skyrim.esm", 0x03AD76 },
			{ "VoidSalts", "Skyrim.esm", 0x03AD60 },
			{ "Wheat", "Skyrim.esm", 0x04B0BA },
			{ "WhiteCap", "Skyrim.esm", 0x04DA22 },
			{ "WispWrappings", "Skyrim.esm", 0x06BC0E },
			{ "RotScale", "Update.esm", 0x00306D },
			{ "ScreamingMaw", "Update.esm", 0x00306E },
			{ "ThornHook", "Update.esm", 0x00306F },
			{ "AncestorMothWing", "Dawnguard.esm", 0x0059BA },
			{ "ChaurusHunterAntennae", "Dawnguard.esm", 0x0183B7 },
			{ "Gleamblossom", "Dawnguard.esm", 0x00B097 },
			{ "PoisonBloom", "Dawnguard.esm", 0x0185FB },
			{ "YellowMountainFlower", "Dawnguard.esm", 0x002A78 },
			{ "HawksEgg", "HearthFires.esm", 0x00F1CC },
			{ "SalmonRoe", "HearthFires.esm", 0x003545 },
			{ "AshCreepCluster", "Dragonborn.esm", 0x01CD74 },
			{ "AshHopperJelly", "Dragonborn.esm", 0x01CD71 },
			{ "AshenGrassPod", "Dragonborn.esm", 0x016E26 },
			{ "BoarTusk", "Dragonborn.esm", 0x01CD6F },
			{ "BurntSprigganWood", "Dragonborn.esm", 0x01CD6E },
			{ "EmperorParasolMoss", "Dragonborn.esm", 0x01FF75 },
			{ "FelsaadTernFeathers", "Dragonborn.esm", 0x03CD8E },
			{ "NetchJelly", "Dragonborn.esm", 0x01CD72 },
			{ "Scathecraw", "Dragonborn.esm", 0x017E97 },
			{ "SpawnAsh", "Dragonborn.esm", 0x01CD6D },
			{ "TramaRoot", "Dragonborn.esm", 0x017008 },
			{ "Angelfish", "ccBGSSSE001-Fish.esm", 0x0008EB },
			{ "AnglerLarvae", "ccBGSSSE001-Fish.esm", 0x0008F0 },
			{ "Glassfish", "ccBGSSSE001-Fish.esm", 0x0008EE },
			{ "Goldfish", "ccBGSSSE001-Fish.esm", 0x0008EF },
			{ "JuvenileMudcrab", "ccBGSSSE001-Fish.esm", 0x0008F2 },
			{ "LyretailAnthias", "ccBGSSSE001-Fish.esm", 0x0008F1 },
			{ "Pearlfish", "ccBGSSSE001-Fish.esm", 0x0008EC },
			{ "PygmySunfish", "ccBGSSSE001-Fish.esm", 0x0008ED },
			{ "Spadefish", "ccBGSSSE001-Fish.esm", 0x0008F3 },
			{ "BlissBugThorax", "ccBGSSSE025-AdvDSGS.esm", 0x059155 },
			{ "FlameStalk", "ccBGSSSE025-AdvDSGS.esm", 0x095E23 },
			{ "GreenButterflyWing", "ccBGSSSE025-AdvDSGS.esm", 0x059151 },
			{ "PurpleButterflyWing", "ccBGSSSE025-AdvDSGS.esm", 0x059153 },
			{ "AlocasiaFruit", "ccBGSSSE037-Curios.esl", 0x000D62 },
			{ "Ambrosia", "ccBGSSSE037-Curios.esl", 0x000836 },
			{ "AsterBloomCore", "ccBGSSSE037-Curios.esl", 0x000D66 },
			{ "BittergreenPetals", "ccBGSSSE037-Curios.esl", 0x000D68 },
			{ "BlindWatchersEye", "ccBGSSSE037-Curios.esl", 0x000822 },
			{ "BlisterPodCap", "ccBGSSSE037-Curios.esl", 0x000D6A },
			{ "BogBeacon", "ccBGSSSE037-Curios.esl", 0x000D6C },
			{ "BunglersBane", "ccBGSSSE037-Curios.esl", 0x000802 },
			{ "Chokeberry", "ccBGSSSE037-Curios.esl", 0x000838 },
			{ "Chokeweed", "ccBGSSSE037-Curios.esl", 0x000804 },
			{ "CodaFlower", "ccBGSSSE037-Curios.esl", 0x000805 },
			{ "Comberry", "ccBGSSSE037-Curios.esl", 0x000806 },
			{ "CongealedPutrescence", "ccBGSSSE037-Curios.esl", 0x000807 },
			{ "CorkbulbRoot", "ccBGSSSE037-Curios.esl", 0x000809 },
			{ "DaedraSilk", "ccBGSSSE037-Curios.esl", 0x00080A },
			{ "DaedraVenin", "ccBGSSSE037-Curios.esl", 0x00080B },
			{ "DaedrothTeeth", "ccBGSSSE037-Curios.esl", 0x000826 },
			{ "DreughWax", "ccBGSSSE037-Curios.esl", 0x000827 },
			{ "ElytraIchor", "ccBGSSSE037-Curios.esl", 0x000828 },
			{ "FirePetal", "ccBGSSSE037-Curios.esl", 0x000829 },
			{ "FlameStalk", "ccBGSSSE037-Curios.esl", 0x00082A },
			{ "FungusStalk", "ccBGSSSE037-Curios.esl", 0x00082B },
			{ "GnarlBark", "ccBGSSSE037-Curios.esl", 0x00082C },
			{ "GoldKanet", "ccBGSSSE037-Curios.esl", 0x00082D },
			{ "HackleLoLeaf", "ccBGSSSE037-Curios.esl", 0x00082E },
			{ "HeartOfOrder", "ccBGSSSE037-Curios.esl", 0x000851 },
			{ "HungerTongue", "ccBGSSSE037-Curios.esl", 0x00080C },
			{ "HydnumAzureGiantSpore", "ccBGSSSE037-Curios.esl", 0x00080D },
			{ "HyphaFacia", "ccBGSSSE037-Curios.esl", 0x00080E },
			{ "ImpGall", "ccBGSSSE037-Curios.esl", 0x00080F },
			{ "KagoutiHide", "ccBGSSSE037-Curios.esl", 0x000810 },
			{ "KreshFiber", "ccBGSSSE037-Curios.esl", 0x000811 },
			{ "Lichor", "ccBGSSSE037-Curios.esl", 0x000837 },
			{ "LuminousRussula", "ccBGSSSE037-Curios.esl", 0x000813 },
			{ "Marshmerrow", "ccBGSSSE037-Curios.esl", 0x000814 },
			{ "MinotaurHorn", "ccBGSSSE037-Curios.esl", 0x000815 },
			{ "OgresTeeth", "ccBGSSSE037-Curios.esl", 0x000816 },
			{ "RedKelpGasBladder", "ccBGSSSE037-Curios.esl", 0x000817 },
			{ "RedwortFlower", "ccBGSSSE037-Curios.esl", 0x000818 },
			{ "Roobrush", "ccBGSSSE037-Curios.esl", 0x000819 },
			{ "Saltrice", "ccBGSSSE037-Curios.esl", 0x00081A },
			{ "ScalonFin", "ccBGSSSE037-Curios.esl", 0x00081B },
			{ "ScribJelly", "ccBGSSSE037-Curios.esl", 0x00081C },
			{ "ScribJerky", "ccBGSSSE037-Curios.esl", 0x00081D },
			{ "SloadSoap", "ccBGSSSE037-Curios.esl", 0x00081E },
			{ "StoneflowerPetals", "ccBGSSSE037-Curios.esl", 0x00081F },
			{ "VoidEssence", "ccBGSSSE037-Curios.esl", 0x000820 },
			{ "WatchersEye", "ccBGSSSE037-Curios.esl", 0x000821 },
			{ "WispStalkCaps", "ccBGSSSE037-Curios.esl", 0x000823 },
			{ "WitheringMoon", "ccBGSSSE037-Curios.esl", 0x000824 },
			{ "WormsHeadCap", "ccBGSSSE037-Curios.esl", 0x000825 },
			{ "MortFlesh", "ccbgssse003-zombies.esl", 0x00080C },
			{ "SteelBlueEntoloma", "ccbgssse040-advobgobs.esl", 0x000832 },
			{ "Bloodgrass", "ccbgssse067-daedinv.esm", 0x024C13 },
			{ "Harrada", "ccbgssse067-daedinv.esm", 0x024C11 },
			{ "SpiddalStick", "ccbgssse067-daedinv.esm", 0x024C15 },
			{ "CorruptedHumanHeart", "ccedhsse002-splkntset.esl", 0x00085B },
			{ "SimonRodaynesHeart", "ccedhsse002-splkntset.esl", 0x00085D },
			{ "Nightshade", "cceejsse005-cave.esm", 0x042A61 },
			{ "HealingSalts", "cckrtsse001_altar.esl", 0x000F86 },
			{ "IronwoodFruit", "cckrtsse001_altar.esl", 0x000F0F },
			{ "WildGrassPod", "cctwbsse001-puzzledungeon.esm", 0x019D3A },
		};

		// The Creations that add ingredients, by their official titles
		struct Creation
		{
			const char* plugin;
			const char* title;
		};

		constexpr Creation creations[] = {
			{ "ccBGSSSE001-Fish.esm", "Fishing" },
			{ "ccbgssse003-zombies.esl", "Plague of the Dead" },
			{ "ccBGSSSE025-AdvDSGS.esm", "Saints & Seducers" },
			{ "ccBGSSSE037-Curios.esl", "Rare Curios" },
			{ "ccbgssse040-advobgobs.esl", "Goblins" },
			{ "ccbgssse067-daedinv.esm", "The Cause" },
			{ "ccedhsse002-splkntset.esl", "Spell Knight Armor" },
			{ "cceejsse005-cave.esm", "Bloodchill Manor" },
			{ "cckrtsse001_altar.esl", "Bittercup" },
			{ "cctwbsse001-puzzledungeon.esm", "Forgotten Seasons" },
		};

		// Where the page lists an ingredient from: the game and DLC by translation key, a
		// Creation by its title; empty for a mod's
		std::string SourceOf(const std::string& a_plugin) {

			const auto is = [&](const char* a_name) { return _stricmp(a_plugin.c_str(), a_name) == 0; };
			if (is("Skyrim.esm")) { return "UI_Ingredients_Skyrim"; }
			if (is("Update.esm")) { return "Rare Curios"; }
			if (is("Dawnguard.esm")) { return "UI_Ingredients_Dawnguard"; }
			if (is("HearthFires.esm")) { return "UI_Ingredients_Hearthfire"; }
			if (is("Dragonborn.esm")) { return "UI_Ingredients_Dragonborn"; }
			for (const auto& [plugin, title] : creations) {
				if (is(plugin)) { return title; }
			}
			return {};
		}

		// The alchemy effects those ingredients carry
		struct KnownEffect
		{
			const char* key;
			RE::FormID  id;
			const char* plugin{ "Skyrim.esm" };
		};

		constexpr KnownEffect knownEffects[] = {
			{ "CureDisease", 0x0AE722 },
			{ "DamageHealth", 0x03EB42 },
			{ "DamageMagicka", 0x03A2B6 },
			{ "DamageMagickaRegen", 0x073F2B },
			{ "DamageStamina", 0x03A2C6 },
			{ "DamageStaminaRegen", 0x073F2C },
			{ "Fear", 0x073F20 },
			{ "FortifyAlteration", 0x03EB24 },
			{ "FortifyBarter", 0x03EB23 },
			{ "FortifyBlock", 0x03EB1C },
			{ "FortifyCarryWeight", 0x03EB01 },
			{ "FortifyConjuration", 0x03EB25 },
			{ "FortifyDestruction", 0x03EB26 },
			{ "FortifyEnchanting", 0x03EB29 },
			{ "FortifyHealth", 0x03EAF3 },
			{ "FortifyHeavyArmor", 0x03EB1E },
			{ "FortifyIllusion", 0x03EB27 },
			{ "FortifyLightArmor", 0x03EB1F },
			{ "FortifyLockpicking", 0x03EB21 },
			{ "FortifyMagicka", 0x03EAF8 },
			{ "FortifyMarksman", 0x03EB1B },
			{ "FortifyOneHanded", 0x03EB19 },
			{ "FortifyPickpocket", 0x03EB20 },
			{ "FortifyRestoration", 0x03EB28 },
			{ "FortifySmithing", 0x03EB1D },
			{ "FortifySneak", 0x03EB22 },
			{ "FortifyStamina", 0x03EAF9 },
			{ "FortifyTwoHanded", 0x03EB1A },
			{ "Frenzy", 0x073F29 },
			{ "Invisibility", 0x03EB3D },
			{ "LingeringDamageHealth", 0x10AA4A },
			{ "LingeringDamageMagicka", 0x10DE5F },
			{ "LingeringDamageStamina", 0x10DE5E },
			{ "Paralysis", 0x073F30 },
			{ "RavageHealth", 0x073F26 },
			{ "RavageMagicka", 0x073F27 },
			{ "RavageStamina", 0x073F23 },
			{ "RegenerateHealth", 0x03EB06 },
			{ "RegenerateMagicka", 0x03EB07 },
			{ "RegenerateStamina", 0x03EB08 },
			{ "ResistFire", 0x03EAEA },
			{ "ResistFrost", 0x03EAEB },
			{ "ResistMagic", 0x039E51 },
			{ "ResistPoison", 0x090041 },
			{ "ResistShock", 0x03EAEC },
			{ "RestoreHealth", 0x03EB15 },
			{ "RestoreMagicka", 0x03EB17 },
			{ "RestoreStamina", 0x03EB16 },
			{ "Slow", 0x073F25 },
			{ "Waterbreathing", 0x03AC2D },
			{ "WeaknessToFire", 0x073F2D },
			{ "WeaknessToFrost", 0x073F2E },
			{ "WeaknessToMagic", 0x073F51 },
			{ "WeaknessToPoison", 0x090042 },
			{ "WeaknessToShock", 0x073F2F },
			{ "CurePoison", 0x109ADD },
			{ "FortifyPersuasion", 0x0D6947 },
			{ "Light", 0x000846, "ccBGSSSE037-Curios.esl" },
			{ "NightEye", 0x000803, "ccBGSSSE037-Curios.esl" },
			{ "SpellAbsorption", 0x000812, "ccBGSSSE037-Curios.esl" },
		};

		std::unordered_map<RE::FormID, std::string> effectKeys;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	// Every ingredient with a name and effects from the game, its DLC or a Creation; a second
	// record of one under the same name in the same plugin (Black-Briar's salt) is left out
	void CHandler::InstallFramework() {

		Ingredients.clear();
		Covered.clear();
		effectKeys.clear();
		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) { return; }

		for (const auto& [key, id, plugin] : knownEffects) {
			if (const auto* effect = dataHandler->LookupForm<RE::EffectSetting>(id, plugin)) {
				effectKeys.emplace(effect->GetFormID(), key);
			}
		}

		std::unordered_map<RE::FormID, const char*> keys;
		for (const auto& [key, plugin, id] : known) {
			if (const auto* form = dataHandler->LookupForm(id, plugin)) {
				keys.emplace(form->GetFormID(), key);
			}
		}
		const bool curios = dataHandler->LookupModByName("ccBGSSSE037-Curios.esl") != nullptr;

		std::unordered_set<std::string> seen;
		for (auto* ingredient : dataHandler->GetFormArray<RE::IngredientItem>()) {
			if (!ingredient || ingredient->IsDeleted() || ingredient->effects.empty()) { continue; }
			const auto* name = ingredient->GetFullName();
			const auto* file = ingredient->GetFile(0);
			if (!name || !*name || !file) { continue; }

			const auto it = keys.find(ingredient->GetFormID());
			const auto key = it != keys.end() ? std::string(it->second) : std::string{};
			if (!curios && (key == "RotScale" || key == "ScreamingMaw" || key == "ThornHook")) { continue; }

			std::string plugin(file->GetFilename());
			auto        source = SourceOf(plugin);
			if (source.empty()) { continue; }
			Covered.insert(ingredient->GetFormID());
			if (!seen.insert(plugin + '\x1f' + name).second) { continue; }
			Ingredients.push_back({ key, std::move(plugin), std::move(source), ingredient });
		}
		std::ranges::stable_sort(Ingredients, {}, [](const Ingredient& a_ingredient) { return std::string_view(a_ingredient.form->GetFullName()); });
		INFO("Ingredients: {}", Ingredients.size());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Knows ) ------------------
	//---------------------------------------------------

	// Bit per effect, kept in the save as the game learns them
	bool CHandler::Knows(const RE::IngredientItem* a_ingredient, std::uint32_t a_index) {

		return a_ingredient && a_index < 16 && ((a_ingredient->gamedata.knownEffectFlags >> a_index) & 1u);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Effect Key ) -------------
	//---------------------------------------------------

	std::string CHandler::EffectKey(const RE::EffectSetting* a_effect) {

		const auto it = a_effect ? effectKeys.find(a_effect->GetFormID()) : effectKeys.end();
		return it != effectKeys.end() ? it->second : std::string{};
	}
}
