#pragma once

namespace CHCMHandler
{
	constexpr int32_t CC_Header = -3;
	constexpr int32_t MS_Header = -4;
	constexpr int32_t ST_Header = -5;
	constexpr int32_t CB_Header = -6;
	constexpr int32_t CL_Header = -7;
	constexpr int32_t CI_Header = -8;
	constexpr int32_t FL_Header = -9;

	inline constexpr std::array<std::tuple<const char*, const char*, int32_t>, 79> MainMCMPagesDefs =
	{ {
		{"$MCMPageSettings", "", -1},
		{"$MCMPageSettings3", "", -1},
		{"$MCMPageSettings2", "", -1},
		{"$MCMPageSettings4", "", -1},
		{" ", "", -1},
		{"$Header0", "", -1},
		{"$MCMPageMain", "", 0},
		{"$MCMPageMainCW", "", 1},
		{"$MCMPageMainDG", "", 2},
		{"$MCMPageMainDB", "", 3},
		{" ", "", -1},
		{"$Header1", "", CC_Header},
		{"$MCMPageMainFarming", "", 4},
		{"$MCMPageMainCreations1", "", 5},
		{"$MCMPageMainCreations2", "", 6},
		{" ", "", -1},
		{"$Header2", "", -1},
		{"$MCMPageMainDawnstar", "", 7},
		{"$MCMPageMainFalkreath", "", 8},
		{"$MCMPageMainMarkarth", "", 9},
		{"$MCMPageMainMorthal", "", 10},
		{"$MCMPageMainRavenRock", "", 16},
		{"$MCMPageMainRiften", "", 11},
		{"$MCMPageMainSkaal", "", 17},
		{"$MCMPageMainSolitude", "", 12},
		{"$MCMPageMainTelMithryn", "", 18},
		{"$MCMPageMainThirsk", "", 19},
		{"$MCMPageMainWhiterun", "", 13},
		{"$MCMPageMainWindhelm", "", 14},
		{"$MCMPageMainWinterhold", "", 15},
		{"$MCMPageMainSmall", "", 20},
		{" ", "", -1},
		{"$Header3", "", -1},
		{"$MCMPageMainCompanions", "", 22},
		{"$MCMPageMainCollege", "", 21},
		{"$MCMPageMainThieves", "", 25},
		{"$MCMPageMainBrotherhood", "", 23},
		{"$MCMPageMainDawnguard", "", 24},
		{"$MCMPageMainVampires", "", 26},
		{" ", "", -1},
		{"$Header4", "", -1},
		{"$MCMPageMainDungeons", "", 27},
		{"$MCMPageMainMiscSide", "", 28},
		{"$MCMPageMainMiscRadi", "", 29},
		{"$MCMPageMainMiscDG", "", 30},
		{"$MCMPageMainMiscDB", "", 31},

		{" ", "LegacyoftheDragonborn.esm", -1},
		{"$Header5", "LegacyoftheDragonborn.esm", -1},
		{"$MCMPageMuseum", "LegacyoftheDragonborn.esm", 52},
		{"$MCMPageExplorer", "LegacyoftheDragonborn.esm", 56},
		{"$MCMPageMisc", "LegacyoftheDragonborn.esm", 53},
		{"$MCMPageNotes", "LegacyoftheDragonborn.esm", 54},
		{"$MCMPageJournals", "LegacyoftheDragonborn.esm", 55},

		{" ", "3DNPC.esp", -1},
		{"$Header6", "3DNPC.esp", -1},
		{"$MCMPage3DNPCMain", "3DNPC.esp", 44},
		{"$MCMPage3DNPCBlood", "3DNPC.esp", 45},
		{"$MCMPage3DNPCSteel", "3DNPC.esp", 46},
		{"$MCMPage3DNPCMisc", "3DNPC.esp", 47},

		{" ", "Vigilant.esm", -1},
		{"$Header7", "Vigilant.esm", -1},
		{"$MCMPageVigilantMain", "Vigilant.esm", 48},
		{"$MCMPageVigilantSide", "Vigilant.esm", 49},
		{"$MCMPageVigilantMemory", "Vigilant.esm", 50},
		{"$MCMPageVigilantRadiant", "Vigilant.esm", 51},

		{" ", "", MS_Header},
		{"$Header8", "", MS_Header},
		{"$MCMPageClockwork", "Clockwork.esp", 32},
		{"$MCMPageFalskaar", "Falskaar.esm", 33},
		{"$MCMPageHelgen", "Helgen Reborn.esp", 34},
		{"$MCMPageMoonpath", "moonpath.esp", 35},
		{"$MCMPageMoonStar", "MoonAndStar_MAS.esp", 36},
		{"$MCMPageProjectAHO", "Dwarfsphere.esp", 37},
		{"$MCMPageTeldrynSerious", "TSR_TeldrynSerious.esp", 38},
		{"$MCMPageBrotherhood", "The Brotherhood of Old.esp", 43},
		{"$MCMPageLull", "WheelsOfLull.esp", 39},
		{"$MCMPageGrayCowl", "Gray Fox Cowl.esm", 40},
		{"$MCMPageUndeath", "Undeath.esp", 42},
		{"$MCMPageWyrmstooth", "Wyrmstooth.esp", 41},
	} };

	inline constexpr std::array < std::tuple < const char*, const char*, int32_t> , 106> MiscMCMPagesDefs =
	{ {
		{"$Header9", "", -1},
		{"$MCMPagePets", "", -1},
		{"$MCMPageClaws", "", -1},
		{"$MCMPageMasks", "", -1},
		{"$MCMPageShouts", "", -1},
		{"$MCMPageHouses", "", -1},
		{"$MCMPageShrines", "", -1},
		{"$MCMPageDoomstone", "", -1},
		{"$MCMPageBarenziah", "", -1},
		
		{" ", "", -1},
		{"$HeaderLocations", "", -1},
		{"$MCMPageLocations1", "", -1},
		{"$MCMPageLocations2", "", -1},
		{"$MCMPageLocations3", "", -1},
		{"$MCMPageLocations4", "", -1},
		{"$MCMPageLocations5", "", -1},
		{"$MCMPageLocations6", "", CL_Header},
		
		{" ", "", -1},
		{"$HeaderBooks", "", -1},
		{"$MCMPageBooks1", "", -1},
		{"$MCMPageBooks2", "", -1},
		{"$MCMPageBooks3", "", -1},
		{"$MCMPageBooks4", "", -1},
		{"$MCMPageBooks5", "", -1},
		{"$MCMPageBooks6", "", -1},
		{"$MCMPageBooks7", "", -1},
		{"$MCMPageBooks8", "", -1},
		{"$MCMPageBooksCC", "", CB_Header},
		
		{" ", "", ST_Header},
		{"$HeaderPatchedTomes", "", ST_Header},
		{"$MCMPageTomes_Apocalypse1", "Apocalypse - Magic of Skyrim.esp", -1},
		{"$MCMPageTomes_Apocalypse2", "Apocalypse - Magic of Skyrim.esp", -1},
		{"$MCMPageTomes_ForgottenMagic1", "ForgottenMagic_Redone.esp", -1},
		{"$MCMPageTomes_ForgottenMagic2", "ForgottenMagic_Redone.esp", -1},
		{"$MCMPageTomes_Mysticism1", "MysticismMagic.esp", -1},
		{"$MCMPageTomes_Mysticism2", "MysticismMagic.esp", -1},
		{"$MCMPageTomes_Odin1", "Odin - Skyrim Magic Overhaul.esp", -1},
		{"$MCMPageTomes_Odin2", "Odin - Skyrim Magic Overhaul.esp", -1},
		{"$MCMPageTomes_Triumvirate1", "Triumvirate - Mage Archetypes.esp", -1},
		{"$MCMPageTomes_Triumvirate2", "Triumvirate - Mage Archetypes.esp", -1},
		
		{" ", "", -1},
		{"$HeaderItems", "", -1},
		{"$MCMPageArmor", "", -1},
		{"$MCMPageItems", "", -1},
		{"$MCMPageJewelry", "", -1},
		{"$MCMPageLiquor", "", -1},
		{"$MCMPageWeapons", "", -1},
		{"$MCMPageConditionals", "", -1},
		{"$MCMPageCCItems", "", CI_Header},

		{" ", "", -1},
		{"$HeaderEnchantments", "", -1},
		{"$MCMPageAEnchantments", "", -1},
		{"$MCMPageWEnchantments", "", -1},

		{" ", "yumcheese.esp", -1},
		{"$HeaderCheeseMod", "yumcheese.esp", -1},
		{"$MCMPageCheese1", "yumcheese.esp", -1},
		{"$MCMPageCheese2", "yumcheese.esp", -1},
		{"$MCMPageCheese3", "yumcheese.esp", -1},
		{"$MCMPageCheese4", "yumcheese.esp", -1},


		{" ", "ccbgssse001-fish.esm", -1},
		{"$HeaderFishing", "ccbgssse001-fish.esm", -1},
		{"$MCMPageFishF", "ccbgssse001-fish.esm", -1},
		{"$MCMPageFishI", "ccbgssse001-fish.esm", -1},
		{"$MCMPageFishB", "ccbgssse001-fish.esm", -1},
		{"$MCMPageFishL", "ccbgssse001-fish.esm", FL_Header},

		{" ", "Fossilsyum.esp", -1},
		{"$HeaderFossils", "Fossilsyum.esp", -1},
		{"$MCMPageFossilMining1", "Fossilsyum.esp", -1},
		{"$MCMPageFossilMining2", "Fossilsyum.esp", -1},
		{"$MCMPageFossilMining3", "Fossilsyum.esp", -1},
		
		{" ", "Requiem.esp", -1},
		{"$HeaderRequiem", "Requiem.esp", -1},
		{"$MCMPageRequiem1", "Requiem.esp", -1},
		{"$MCMPageRequiem2", "Requiem.esp", -1},
		{"$MCMPageRequiem3", "Requiem.esp", -1},
		{"$MCMPageRequiem4", "Requiem.esp", -1},

		{" ", "SkyrimsUniqueTreasures.esp", -1},
		{"$HeaderSUT", "SkyrimsUniqueTreasures.esp", -1},
		{"$MCMPageSUT1", "SkyrimsUniqueTreasures.esp", -1},
		{"$MCMPageSUT2", "SkyrimsUniqueTreasures.esp", -1},
		{"$MCMPageSUT3", "SkyrimsUniqueTreasures.esp", -1},

		{" ", "Vigilant.esm", -1},
		{"$HeaderVIG", "Vigilant.esm", -1},
		{"$MCMPageVIG1", "Vigilant.esm", -1},
		{"$MCMPageVIG2", "Vigilant.esm", -1},
		{"$MCMPageVIG3", "Vigilant.esm", -1},

		{" ", "", MS_Header},
		{"$Header8", "", MS_Header},
		{"$MCMPageAdditionalHearthfireDolls", "AdditionalHearthfireDolls.esp", -1},
		{"$MCMPageClockwork", "Clockwork.esp", -1},
		{"$MCMPageFalskaar", "Falskaar.esm", -1},
		{"$MCMPageHelgen", "Helgen Reborn.esp", -1},
		{"$MCMPageInnSoaps", "Inn Soaps.esp", -1},
		{"$MCMPageInterestingNPCs", "3DNPC.esp", -1},
		{"$MCMPageMoonStar", "MoonAndStar_MAS.esp", -1},
		{"$MCMPageMoonpath", "moonpath.esp", -1},
		{"$MCMPageOblivionArtifacts", "WZOblivionArtifacts.esp", -1},
		{"$MCMPageProjectAHO", "Dwarfsphere.esp", -1},
		{"$MCMPageTeldrynSerious", "TSR_TeldrynSerious.esp", -1},
		{"$MCMPageBrotherhood", "The Brotherhood of Old.esp", -1},
		{"$MCMPageGrayCowl", "Gray Fox Cowl.esm", -1},
		{"$MCMPageLull", "WheelsOfLull.esp", -1},
		{"$MCMPageThunderchild", "Thunderchild - Epic Shout Package.esp", -1},
		{"$MCMPageUndeath", "Undeath.esp", -1},
		{"$MCMPageWintersun", "Wintersun - Faiths of Skyrim.esp", -1},
		{"$MCMPageWyrmstooth", "Wyrmstooth.esp", -1},
	} };

	inline std::vector<std::string> MainMCMPages{};
	inline std::vector<std::string> MiscMCMPages{};

	using EventResult = RE::BSEventNotifyControl;

	class MCMAPI final : public RE::BSTEventSink<RE::MenuOpenCloseEvent> 
	{

		public: [[nodiscard]] static MCMAPI* GetSingleton() { static MCMAPI singleton; return &singleton; }

		static void Register();
		static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm);

		EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

		static int32_t GetSkyUIMCMPositionalIndex(RE::StaticFunctionTag*, std::string MCMPage);
		static int32_t GetMCMPageIdentifierFromName(RE::StaticFunctionTag*, std::string MCMPage);

		static void BuildMCMPages();
		static std::vector<std::string> GetMCMPages(RE::StaticFunctionTag*, std::int32_t menu_identifier);
	};
}