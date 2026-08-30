#include "Internal Utility/Variables.hpp"
#include "DKUtil/Utility.hpp"
#include "Internal Utility/CellScanner.hpp"
#include "Internal Utility/Array.hpp"
#include "FrameworkMaster.hpp"
#include "Internal Utility/ScriptObject.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Internal Utility/PatchListener.hpp"
#include "Internal Utility/Events.hpp"
#include "Internal Utility/MCMHandler.hpp"

namespace CFramework_Master 
{
	using namespace CVariables;
	using namespace ArrayHolder;
	using namespace Serialization;
	using namespace CExternalPatchHandler;

	//---------------------------------------------------
	//-- Framework Functions ( Master Registration ) ----
	//---------------------------------------------------

	void FrameworkAPI::Register()
	{
		_OnMapMarkerAdded = CEvents::EventHandler::RegisterMapMarkerAddedHook(&OnMapMarkerAdded);
		_OnMapMarkerDiscovered = CEvents::EventHandler::RegisterMapMarkerDiscoveredHook(&OnMapMarkerDiscovered);
		CEvents::EventHandler::RegisterForEvent_OnDeathEvent(OnDeath);
		CEvents::EventHandler::RegisterForEvent_OnHitEvent(OnHit);

		SKSE::GetPapyrusInterface()->Register(FrameworkAPI::RegisterFunctions);

		SetSerializableInfo(FoundItemData);
		SetSerializableInfo(FoundItemData_NoShow);
		SetSerializableInfo(CQuestKeys_Natural);
		SetSerializableInfo(CQuestKeys_Manual);
		SetSerializableInfo(CQuestKeys_Stages);
		SetSerializableInfo(LoggingData);
		SetSerializableInfo(RadiantCountData);
		SetSerializableInfo(ExcludedCellScannerRefs);
		SetSerializableInfo(ExcludedMerchantContainers);
		SetSerializableInfo(PatchSettings);
		SetSerializableInfo(PlayerKills);
		SetSerializableInfo(PlayerHits);

		//Frameworks
		CFramework_Others::CHandler::InstallFramework();
		CFramework_MapMa::CHandler::InstallFramework();
		CFramework_Blessings::CHandler::InstallFramework();
		CFramework_Enchantments::CHandler::InstallFramework();
		CFramework_Pets::CHandler::InstallFramework();
		CFramework_PlayerHomes::CHandler::InstallFramework();
		CFramework_Shouts::CHandler::InstallFramework();
	};

	void FrameworkAPI::RegisterCustomPatches()
	{
		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->InstallFramework();
		};
	}

	void FrameworkAPI::FinalizeRegistrations()
	{
		//Register Arrays
		ArrayHolder::RegisterArrays();

		//Exclude Vendor Chests.
		Completionist::CellScanner::init();
	}

	static void HideTrackedItem(RE::TESForm* a_item) {
		
	};

	static std::string GetDeathPrefix(const std::string&  a_location)
	{
		constexpr const char* list[] =
		{
			"abandoned their post in the dead of night, shadows concealing their departure in ",
			"absorbed a blow where the armor was thin in ",
			"adorned the tapestry of fate with their final brushstroke in ",
			"ascended to the celestial stage, leaving footprints in the cosmic dust of ",
			"became a chapter in the book of life, closed in ",
			"became a constellation in the night sky of ",
			"became a constellation in the night sky, a legacy in ",
			"became a ghost story told around campfires in the heart of ",
			"became a legend in the silence, whispers echoing in ",
			"became a memory etched in the stone walls of ",
			"became a memory, a ghost haunting the alleys of ",
			"became a mystery, an unsolved enigma in ",
			"became a relic of tales told by the moonlight in ",
			"became a riddle whispered by the shadows of ",
			"became a secret shared only by the echoes of ",
			"became a specter haunting the corridors of ",
			"became a whisper in the river of time, flowing through the valleys of ",
			"became an echo in the hallowed halls of ",
			"became an echo, bouncing off the canyon walls of ",
			"became an enigma, a puzzle left unsolved in ",
			"became an unsung melody, lingering in the air of ",
			"became stardust in the cosmic ballet, a celestial end in ",
			"became stardust, a celestial tale written in the skies of ",
			"became stardust, painting constellations in the sky of ",
			"became stardust, scattered in the cosmic winds of ",
			"became stardust, weaving dreams in the cosmic tapestry of ",
			"bid adieu to their sweetroll amidst the whispers of ",
			"bowed out gracefully, leaving a vacancy in the dance of life in ",
			"brutally met their end in ",
			"carved their legacy into the ancient stones of ",
			"chose the path of shadows, disappearing into the obsidian night of ",
			"claimed their final victory in the arena of fate, the cheers fading in ",
			"collapsed like a milk drinker in ",
			"conceded to destiny's decree, a scroll sealed in the archives of ",
			"concluded their saga with a final punctuation mark in ",
			"crumbled like ancient parchment, a scroll of destiny unraveled in ",
			"danced with the reaper under the moonlight of ",
			"defended bravely but met an untimely demise in ",
			"defied fate until the final curtain fell in ",
			"disappeared into the mists of time, leaving behind echoes in ",
			"disappeared into the twilight, a fading silhouette in ",
			"disappeared like the morning dew under the sunrise of ",
			"disappeared like the morning mist under the sun of ",
			"disappeared like the shadows, a silhouette blending into ",
			"discovered the alchemy of becoming a memory in ",
			"discovered the art of becoming a memory in ",
			"discovered the art of becoming a whisper in the winds of ",
			"discovered the price of fate's toll, a cost paid in ",
			"disintegrated into echoes, a resonance lingering in ",
			"disintegrated into the void, a silent departure in ",
			"dispersed into the cosmic sea, becoming stardust in ",
			"dissolved into the canvas of eternity, brushstrokes lost in ",
			"dissolved into the echoes of time, a resonance lingering in ",
			"dissolved into the hues of twilight, a canvas painted by ",
			"drew their last breath in ",
			"earned a seat in Sovngarde, their journey ending in ",
			"echoed Heimskr's sermons, a final proclamation in ",
			"echoed their defiance in the valleys of ",
			"echoed their farewell in the catacombs of ",
			"echoed their farewell in the caverns of ",
			"echoed their farewell like a melody in the air of ",
			"echoed their farewell through the corridors of ",
			"echoed their farewell, a symphony of goodbyes in ",
			"embarked on a journey to the far shores of eternity in ",
			"embraced oblivion amidst the haunting shadows of ",
			"embraced the shadows, becoming one with the night in ",
			"entered the realm of dreams, a sleep eternal in ",
			"evanesced into the twilight, a fading mirage in ",
			"evaporated like morning dew under the sun of ",
			"exhaled their last breath, a breeze carrying their essence in ",
			"faced adversity, a brave stand that crumbled in ",
			"faced the final act in ",
			"faced the final act, curtains drawn in ",
			"faced the final curtain, a theatrical exit in ",
			"faced the music of blades, a crescendo of fate in ",
			"faced the music of eternity, notes fading away in ",
			"faded away into nothing with a cutting edge of fate in ",
			"faded into the background, a vanishing act in ",
			"faded like a forgotten dream, leaving traces in ",
			"faded like an autumn leaf, leaving a tale in ",
			"fell like a shooting star, a celestial spectacle in ",
			"felt the sting where armor failed in ",
			"folded into the pages of history, a tome closed in ",
			"fought valiantly but succumbed to destiny in ",
			"found solace in the embrace of darkness, a journey ending in ",
			"found solace in the embrace of the unknown, a journey's end in ",
			"gave their final performance, curtains closing in ",
			"gracefully exited the stage, leaving behind an empty spotlight in ",
			"had their sweetroll taken to the eternal cloud district in ",
			"halted their journey abruptly, footsteps echoing in ",
			"immersed themselves in the river of time, flowing through the epochs of ",
			"insisted 'tis but a scratch' before being turned to dust in ",
			"joined the ranks of Sovngarde, the end of the mortal road in ",
			"journeyed beyond the veil, a destination known only to the stars in ",
			"journeyed to Sovngarde, their saga ending in ",
			"laughed in the face of doom, a defiant smile in ",
			"left their mark on the parchment of history, ink drying in ",
			"listened to the symphony of eternity, a silent conductor in ",
			"lost their sweetroll to the relentless hands of fate in ",
			"melded with eternity, leaving tales echoing in ",
			"melded with the shadows, a silhouette fading away in ",
			"melted into the shades of twilight, a fading memory in ",
			"melted into the shadows, a silent departure in ",
			"melted into the shadows, a silhouette fading away in ",
			"melted into the whispers of the wind, a nameless echo in ",
			"melted like candle wax under the moonlight of ",
			"melted like snowflakes under the gaze of the moon in ",
			"merged with the twilight, a fusion of shadows in ",
			"met destiny's gaze and found their final resting place in ",
			"met their fate with open arms, a final embrace in ",
			"mimicked the autumn leaves, a final descent in ",
			"mocked fate until the very end, fading away in ",
			"murmured a last goodbye, the echoes lingering in ",
			"navigated the labyrinth of fate, reaching a dead-end in ",
			"nervously surrended before being felled in ",
			"nervously surrendered before being felled in ",
			"observed their reflection in the waters of destiny, a ripple fading in ",
			"passed into legend, whispered around the firesides of ",
			"perished like a flame in the gusts of ",
			"played a dangerous game and lost in ",
			"plunged into the abyss, a descent into the unknown in ",
			"proved to be just another pawn in the grand game of life.",
			"put up a good fight but ultimately perished in ",
			"quietly slipped away, footsteps fading into the echoes of ",
			"realized the fragility of existence in the ancient ruins of ",
			"recited their final verse in the poetry of time, ink staining ",
			"relinquished mortal ties to become whispers in the breeze of ",
			"relinquished their mortal coil, a departure unnoticed in ",
			"resonated with the symphony of silence, a final note in ",
			"retreated into the caverns of memory, a silent sanctuary in ",
			"revealed the final card in the deck of fate, a hand played in ",
			"rolled the dice of life and crumbled in ",
			"sailed into the beyond, a vessel disappearing in ",
			"sang their swan song, melodies lingering in the air of ",
			"sank into the depths of shadows, a silhouette vanishing in ",
			"sank into the embrace of darkness, a silhouette lost in ",
			"sank into the embrace of shadows, a silhouette lost in ",
			"sank into the river of time, carried away by the currents of ",
			"scribbled their last lines on the parchment of destiny in ",
			"severed ties with the mortal realm, a departure unmarked in ",
			"slipped through the cracks of reality, a vanishing act in ",
			"spiraled into the abyss, a dance with darkness in ",
			"spiraled into the cosmic dance, a partner in the ballet of ",
			"spiraled into the void, a dance with oblivion in ",
			"succumbed to the dance of blades in the shadows of ",
			"surrendered their essence to the abyss, a departure shrouded in ",
			"surrendered to the inevitable, yielding in ",
			"swept away by the river of time, a current carrying them through ",
			"threw themselves into the flames of fate, a blaze extinguished in ",
			"took a calculated risk in ",
			"took a hit some place we won't mention in ",
			"transcended the boundaries of mortality, a departure beyond the horizon of ",
			"unraveled like threads in the cosmic tapestry, lost in ",
			"unraveled the threads of existence, a tapestry left in ",
			"vanished into the labyrinth of whispers, a maze of forgotten echoes in ",
			"vanished like a mirage, leaving behind echoes in ",
			"vanished like a shadow, leaving behind a tale of ",
			"vanished like a wisp of smoke, dissipating in ",
			"vanished like footprints in the sands of ",
			"vanished like footprints on the shores of ",
			"vanished like whispers in the night, leaving only echoes in ",
			"veiled themselves in the shroud of eternity, a departure shrouded in ",
			"went to Sovngarde after being cut down in ",
			"whirled into the dance of destiny, a fleeting pirouette in ",
			"whispered a silent prayer before the last heartbeat in ",
			"whispered farewell as shadows claimed them in ",
			"whispered their final soliloquy, a monologue lost in ",
			"whispered their final verse, carried away by the night winds of ",
			"whispered their final words amid the echoes of ",
			"whispered their final words, carried away by the zephyrs of ",
			"whispered their legacy to the moon, a secret kept by ",
			"whispered their parting words, carried away by the breezes of ",
			"whispered their parting words, carried away by the winds of ",
			"wilted away like petals carried away by the whispers of ",
			"wilted away like petals in the garden of ",
			"wilted away, a mere memory lingering in ",
			"witnessed the final sunset, a horizon embraced in ",
			"wove their final tapestry, threads unraveling in the loom of ",
			"wove their final verse in the song of fate, fading away in ",
			"yielded to the cosmic forces, a surrender in the cosmic winds of ",
			"yielded to the inevitable, a silent surrender in ",
		};

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, std::extent<decltype(list)>::value - 1);
		return list[uni(rng)] + a_location;
	};

	static std::string isVowel(char ch)
	{
		std::string str = "aeiouAEIOU";
		return (str.find(ch) != std::string::npos) ? "An" : "A";
	}

	//---------------------------------------------------
	//-- Framework Functions ( Record Player Kills ) ----
	//---------------------------------------------------

	void FrameworkAPI::OnDeath(const RE::TESDeathEvent* a_event)
	{
		if (!a_event->actorDying || !a_event->dead || !a_event->actorKiller) {
			return;
		}

		auto* NPC = a_event->actorDying->As<RE::Actor>();
		if (!NPC || !NPC->GetRace() || !PlayerHits.HasTargetRegistered(NPC)) {
			return;
		}

		std::string name = PlayerKills.GetRaceName(NPC);
		std::string unique_name = NPC->GetActorBase() && NPC->GetActorBase()->IsUnique() ? NPC->GetName() : "";
		std::string deathString;

		auto* location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation();
		auto* weapon = PlayerHits.GetTargetWeapon(NPC);

		if (!unique_name.empty()) {
			if (location) {
				deathString = fmt::format("{} {}.\n{} {}", unique_name, GetDeathPrefix(location->GetFullName()), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
			else {
				deathString = fmt::format("{} {}.\n{} {}", unique_name, GetDeathPrefix("the wilds of Tamriel"), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
		}
		else {
			if (location) {
				deathString = fmt::format("{} {} {}\n{} {}", isVowel(NPC->GetName()[0]), NPC->GetName(), GetDeathPrefix(location->GetFullName()), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
			else {
				deathString = fmt::format("{} {} {}\n{} {}", isVowel(NPC->GetName()[0]), NPC->GetName(), GetDeathPrefix("the wilds of Tamriel"), GET_LOC_STRING_BY_KEY("MCMText_DeathSentanceBy"), weapon ? weapon->GetName() : "");
			}
		}

		auto* killer = a_event->actorKiller.get();
		PlayerKills.AddKillOrAssist(name, deathString, (killer == RE::PlayerCharacter::GetSingleton()));
		PlayerHits.RemoveTarget(NPC->GetFormID());
	}

	//---------------------------------------------------
	//-- Framework Functions ( Record Player Kills ) ----
	//---------------------------------------------------

	void FrameworkAPI::OnHit(RE::TESHitEvent const* a_event)
	{
		auto* target = a_event->target.get();
		auto* cause = a_event->cause.get();
		auto* source = RE::TESForm::LookupByID(a_event->source);
		auto* projectile = RE::TESForm::LookupByID(a_event->projectile);
		auto* weapon = source ? source : (projectile ? projectile : nullptr);

		if (!weapon || !target || !target->As<RE::Actor>() || !cause || cause != RE::PlayerCharacter::GetSingleton()) {
			return;
		}

		if (PlayerHits.HasTargetRegistered(target)) {
			PlayerHits.UpdateTargetWeapon(target, weapon);
			return;
		}

		PlayerHits.AddTarget(target, weapon);
	};

	std::vector<std::string> FrameworkAPI::GetPlayerKillNames(RE::StaticFunctionTag*) {
		return PlayerKills.GetKills();
	};

	int32_t FrameworkAPI::GetPlayerKillCount(RE::StaticFunctionTag*, std::string a_name) {
		return PlayerKills.GetKillCountFor(a_name);
	};

	void FrameworkAPI::ResetPlayerKill(RE::StaticFunctionTag*, std::string a_name) {
		PlayerKills.ResetCount(a_name);
	}

	void FrameworkAPI::RemovePlayerKill(RE::StaticFunctionTag*, std::string a_name) {
		PlayerKills.RemoveEnemy(a_name);
	}

	std::string FrameworkAPI::GetDeathString(RE::StaticFunctionTag*, std::string a_name) {
		return PlayerKills.GetDeathStringFor(a_name);
	}

	std::string FrameworkAPI::GetCombinedKillString(RE::StaticFunctionTag*, std::string a_name) {
		return PlayerKills.GetCombinedKillString(a_name);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Papyrus Registrations ) --
	//---------------------------------------------------

	auto FrameworkAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("SetFrameworkQuest", "Completionist_Native", SetFrameworkQuest);

		a_vm->RegisterFunction("LogWithPlugin", "Completionist_Native", LogWithPlugin);
		a_vm->RegisterFunction("GetFormArrayByID", "Completionist_Native", GetFormArrayByID);
		a_vm->RegisterFunction("GetNameArrayByID", "Completionist_Native", GetNameArrayByID);
		a_vm->RegisterFunction("GetBoolArrayByID", "Completionist_Native", GetBoolArrayByID);
		a_vm->RegisterFunction("GetTextArrayByID", "Completionist_Native", GetTextArrayByID);

		a_vm->RegisterFunction("GetEntries_TotalByID", "Completionist_Native", GetEntries_TotalByID);
		a_vm->RegisterFunction("GetEntries_FoundByID", "Completionist_Native", GetEntries_FoundByID);

		a_vm->RegisterFunction("IsOptionCompleted", "Completionist_Native", IsOptionCompleted);
		a_vm->RegisterFunction("SetOptionCompleted", "Completionist_Native", SetOptionCompleted);

		a_vm->RegisterFunction("GetVersion", "Completionist_Native", GetVersion);
		a_vm->RegisterFunction("GetHexValue", "Completionist_Native", GetHexValue);
		a_vm->RegisterFunction("SendNotification", "Completionist_Native", SendNotificationExt);

		a_vm->RegisterFunction("UpdateVariables", "Completionist_Native", UpdateVariables);

		a_vm->RegisterFunction("GetLoggingDates", "Completionist_Native", GetLoggingDates);
		a_vm->RegisterFunction("GetLoggedEventsForDate", "Completionist_Native", GetLoggedEventsForDate);

		a_vm->RegisterFunction("Framework_UpdatePetOwnership", "Completionist_Native", CFramework_Pets::CHandler::Framework_UpdatePetOwnership);
		a_vm->RegisterFunction("Framework_UpdateShouts", "Completionist_Native", CFramework_Shouts::CHandler::UpdateFoundFormsExt);
		a_vm->RegisterFunction("ActivateShrineByID", "Completionist_Native", CFramework_Blessings::CHandler::ActivateShrineFromPapyrus);

		a_vm->RegisterFunction("CheckForReferences", "Completionist_Native", Completionist::CellScanner::CheckForReferences);
		a_vm->RegisterFunction("GetTargetReferenceRefr", "Completionist_Native", Completionist::CellScanner::GetTargetReferenceRefr);
		a_vm->RegisterFunction("GetTargetReferenceName", "Completionist_Native", Completionist::CellScanner::GetTargetReferenceName);
		a_vm->RegisterFunction("GetTargetReferenceType", "Completionist_Native", Completionist::CellScanner::GetTargetReferenceType);
		a_vm->RegisterFunction("GetTargetReferenceForm", "Completionist_Native", Completionist::CellScanner::GetTargetReferenceForm);

		a_vm->RegisterFunction("GetQuestMarkerReferenceFormID", "Completionist_Native", Completionist::CellScanner::GetQuestMarkerReferenceFormID);
		a_vm->RegisterFunction("GetQuestMarkerReferenceOwner", "Completionist_Native", Completionist::CellScanner::GetQuestMarkerReferenceOwner);
		a_vm->RegisterFunction("GetQuestMarkerReferenceIndex", "Completionist_Native", Completionist::CellScanner::GetQuestMarkerReferenceIndex);
		a_vm->RegisterFunction("GetReferenceFormIDs", "Completionist_Native", Completionist::CellScanner::GetReferenceFormIDs);
		a_vm->RegisterFunction("GetReferenceNames", "Completionist_Native", Completionist::CellScanner::GetReferenceNames);
		a_vm->RegisterFunction("GetObjectReferences", "Completionist_Native", Completionist::CellScanner::GetObjectReferences);
		a_vm->RegisterFunction("isCellExcluded", "Completionist_Native", Completionist::CellScanner::isCellExcluded);
		a_vm->RegisterFunction("HasPinnedFormInCell", "Completionist_Native", Completionist::CellScanner::HasPinnedFormInCell);
		a_vm->RegisterFunction("IsItemPinnable", "Completionist_Native", Completionist::CellScanner::IsItemPinnable);

		a_vm->RegisterFunction("GetPinnedReferenceName", "Completionist_Native", Completionist::CellScanner::GetPinnedReferenceName);
		a_vm->RegisterFunction("GetPinnedReferenceType", "Completionist_Native", Completionist::CellScanner::GetPinnedReferenceType);
		a_vm->RegisterFunction("GetPinnedReferenceRefr", "Completionist_Native", Completionist::CellScanner::GetPinnedReferenceRefr);

		a_vm->RegisterFunction("ExcludeReference", "Completionist_Native", Completionist::CellScanner::ExcludeReference);
		a_vm->RegisterFunction("RemoveExcludedReference", "Completionist_Native", Completionist::CellScanner::RemoveExcludedReference);

		a_vm->RegisterFunction("MapMarkerIsCleared", "Completionist_Native", CFramework_MapMa::CHandler::MarkerIsCleared);

		a_vm->RegisterFunction("SetFishCaught", "Completionist_Native", ProcessCaughtFishFromPapyrus);
		a_vm->RegisterFunction("IsItemKnownExternal", "Completionist_Native", IsItemKnownExternal);
		a_vm->RegisterFunction("IsInActualMenuMode", "Completionist_Native", IsInActualMenuMode);

		a_vm->RegisterFunction("GetPlayerKillNames", "Completionist_Native", GetPlayerKillNames);
		a_vm->RegisterFunction("GetPlayerKillCount", "Completionist_Native", GetPlayerKillCount);
		a_vm->RegisterFunction("ResetPlayerKill", "Completionist_Native", ResetPlayerKill);
		a_vm->RegisterFunction("RemovePlayerKill", "Completionist_Native", RemovePlayerKill);
		a_vm->RegisterFunction("GetDeathString", "Completionist_Native", GetDeathString);
		a_vm->RegisterFunction("GetCombinedKillString", "Completionist_Native", GetCombinedKillString);		
		a_vm->RegisterFunction("GetPatchCount", "Completionist_Native", GetPatchCount);
		return true;
	};

	//---------------------------------------------------
	//-- Variables Functions ( Set MCM Pointer ) --------
	//---------------------------------------------------

	void FrameworkAPI::SetFrameworkQuest(RE::StaticFunctionTag*, RE::TESQuest* a_quest)
	{
		CVariables::VariablesAPI::SetFrameworkQuest(nullptr, a_quest);
	};

	//---------------------------------------------------
	//-- Variables Functions (Get Patch Count ) ---------
	//---------------------------------------------------

	int32_t FrameworkAPI::GetPatchCount(RE::StaticFunctionTag*) {
		return InstalledPatchesForMCMDisplay;
	};

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggingDates(RE::StaticFunctionTag*)
	{
		std::vector<std::string> list = LoggingData.GetAllLoggedDates();
		std::sort(list.begin(), list.end(), compare_dates);
		std::reverse(list.begin(), list.end());
		return list;
	}

	//---------------------------------------------------
	//-- Framework Functions (IsInActualMenuMode ) ------
	//---------------------------------------------------

	bool FrameworkAPI::IsInActualMenuMode(RE::StaticFunctionTag*)
	{
		auto map = RE::ControlMap::GetSingleton();
		return !map->IsMovementControlsEnabled();
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	bool FrameworkAPI::compare_dates(const std::string& a, const std::string& b)
	{
		auto extract_date_part = [](const std::string& date, std::size_t start, std::size_t length) {
			return std::stoi(date.substr(start, length));
			};

		auto yr1 = extract_date_part(a, 6, 4);
		auto yr2 = extract_date_part(b, 6, 4);

		if (yr1 != yr2) {
			return yr1 < yr2;
		}

		auto mo1 = extract_date_part(a, 3, 2);
		auto mo2 = extract_date_part(b, 3, 2);

		if (mo1 != mo2) {
			return mo1 < mo2;
		}

		auto da1 = extract_date_part(a, 0, 2);
		auto da2 = extract_date_part(b, 0, 2);

		return da1 < da2;
	}

	//---------------------------------------------------
	//-- Framework Functions (Logging Functions ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetLoggedEventsForDate(RE::StaticFunctionTag*, std::string a_date, bool b_prefix, bool b_colour, std::string_view qc, std::string_view ic, std::string_view bc, std::string_view sc)
	{
		auto list = LoggingData.GetAllLoggedEvents(a_date, b_prefix, b_colour, qc, ic, bc, sc);
		while (list.size() > 126) { list.erase(list.begin()); }
		std::reverse(list.begin(), list.end());
		return list;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Quest Search ) -------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::SearchAndReportPage(const std::string& s_term, bool b_ignoreCompleted, std::int32_t i_maxResults, std::int32_t i_searchType)
	{
		std::vector<std::string> list{};
		auto result = 1;

		for (const auto& [form, name, mcmPage, Category] : CFramework_Master::CItemsDataVec)
		{
			if (!CHCMHandler::MCMAPI::IsFormVisible(nullptr, mcmPage, form, -1))
			{
				INFO("Form: {} is hidden and cannot be processed", form->GetName());
				continue;
			}

			if (list.size() >= i_maxResults)
				break;

			bool process = false;

			switch (i_searchType)
			{
			case 0: process = DKUtil::string::icontains(name, s_term); break;
			case 1: process = name.starts_with(s_term); break;
			case 2: process = DKUtil::string::iequals(name, s_term); break;
			default: break;
			}

			if (process)
			{
				if (b_ignoreCompleted && (FoundItemData.HasForm(form) || FoundItemData_NoShow.HasForm(form)))
				{
					continue;
				}

				list.push_back("$MiscResult{" + std::to_string(result) + "}{" + "[REPLACE]" + "}{" + mcmPage + "}{" + GetLocalisedCategory(Category) + "}{" + name + "}");
				list.push_back(mcmPage);
				list.push_back(name);
				list.push_back("Misc");
				result++;
			}
		}

		return list;
	}

	std::string FrameworkAPI::GetLocalisedCategory(int32_t ID)
	{
		switch (static_cast<EntryCategory>(ID))
		{
		case CFramework_Master::kNone:      return "";
		case CFramework_Master::kItem:      return GetLocalizedCategoryString("Category_Item");
		case CFramework_Master::kBook:      return GetLocalizedCategoryString("Category_Book");
		case CFramework_Master::kMapM:      return GetLocalizedCategoryString("Category_MapP");
		case CFramework_Master::kShou:      return GetLocalizedCategoryString("Category_Shou");
		case CFramework_Master::kEnch:      return GetLocalizedCategoryString("Category_Ench");
		case CFramework_Master::kHome:      return GetLocalizedCategoryString("Category_Home");
		case CFramework_Master::kPets:      return GetLocalizedCategoryString("Category_Pets");
		case CFramework_Master::kClaw:      return GetLocalizedCategoryString("Category_Claw");
		case CFramework_Master::kMask:      return GetLocalizedCategoryString("Category_Mask");
		case CFramework_Master::kFish:      return GetLocalizedCategoryString("Category_Fish");
		case CFramework_Master::kShrine:    return GetLocalizedCategoryString("Category_Shrine");
		case CFramework_Master::kStones:    return GetLocalizedCategoryString("Category_Stones");
		case CFramework_Master::kBarenziah: return GetLocalizedCategoryString("Category_Barenziah");
		case CFramework_Master::kShard:		return GetLocalizedCategoryString("Category_Skyshard");
		default:                            return "";
		}
	}

	std::string FrameworkAPI::GetLocalizedCategoryString(const std::string& key)
	{
		return CLocalisation::LocalisationAPI::GetSingleton()->GetLocStringByKey(key.c_str());
	}

	std::int32_t FrameworkAPI::GetBookCategoryType(RE::TESForm* a_form)
	{
		if (!a_form) {
			return 0;
		}

		auto* book = static_cast<RE::TESObjectBOOK*>(a_form);

		if (book && (book->GetSpell() || book->TeachesSkill())) {
			return 0;  // Book with spells or skill teaching
		}

		return (book != nullptr) ? 2 : 0;  // Regular book or not a book
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::LogWithPlugin(RE::StaticFunctionTag*, std::string a_message) {

		INFO("Papyrus Message: {}", a_message);
	}

	void FrameworkAPI::AddNewEventToLog(Serialization::CompletionistLog::logType kType, std::string a_log)
	{
		LoggingData.AddLoggedEvent(kType, a_log);
	}

	Serialization::CompletionistLog::logType FrameworkAPI::GetBookLogType(RE::TESForm* a_form)
	{
		if (auto* book = static_cast<RE::TESObjectBOOK*>(a_form); book && book->GetSpell()) {
			return Serialization::CompletionistLog::logType::kTome;
		}

		return Serialization::CompletionistLog::logType::kBook;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Update From MCM ) --------
	//---------------------------------------------------

	void FrameworkAPI::UpdateVariables(RE::StaticFunctionTag*) 
	{
		VariablesAPI::Update();
	}

	//---------------------------------------------------
	//-- Framework Functions ( Send Notification ) ------
	//---------------------------------------------------

	void FrameworkAPI::SendNotification(std::string a_msg, std::string a_setting) {
		using namespace ScriptObject;

		auto MCMScript = FromForm(static_cast<RE::TESForm*>(RE::TESDataHandler::GetSingleton()->LookupForm(0x00800, "Completionist.esp")), "Completionist_MCMScript");
		if (!MCMScript->GetProperty(a_setting)->GetBool()) { return; }

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, MCMScript->GetProperty("ColourString")->GetString(), a_msg);
		if (!MCMScript->GetProperty("NotificationColourEnabled")->GetBool()) { RE::SendHUDMessage::ShowHUDMessage(a_msg.c_str()); return; }
		RE::SendHUDMessage::ShowHUDMessage(message.c_str());
	}

	//---------------------------------------------------
	//-- Utility Functions ------------------------------
	//---------------------------------------------------

	void FrameworkAPI::SendNotificationExt(RE::StaticFunctionTag*, std::string a_msg, std::string a_colour, bool a_enabled) {

		auto message = fmt::format("<font color='{:s}'>{:s}</font>"sv, a_colour, a_msg);
		if (!a_enabled) { RE::SendHUDMessage::ShowHUDMessage(a_msg.c_str()); return; }
		RE::SendHUDMessage::ShowHUDMessage(message.c_str());
	}

	//---------------------------------------------------
	//-- String Functions -------------------------------
	//---------------------------------------------------

	std::string FrameworkAPI::GetVersion(RE::StaticFunctionTag*) { return std::string(FrameworkAPI::ReplaceStr(Plugin::Version.string(), "-", ".")); }
	std::string FrameworkAPI::GetHexValue(RE::StaticFunctionTag*, uint32_t IntVal) { if (!IntVal) { return ""; } return fmt::format("#{:X}", IntVal); }
	std::string FrameworkAPI::ReplaceStr(std::string const& in, std::string const& from, std::string const& to) { return std::regex_replace(in, std::regex(from), to); }

	//---------------------------------------------------
	//-- Framework Events ( Load & Update Frameworks ) --
	//---------------------------------------------------

	void FrameworkAPI::Update() 
	{
		AddUpdateFoundForms_Invoke();

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->UpdateFoundForms();
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Added ) -----------
	//---------------------------------------------------

	void FrameworkAPI::OnMapMarkerAdded(RE::TESFullName* a_form)
	{
		_OnMapMarkerAdded(a_form);
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->ProcessHookedMarker(a_form->GetFullName());
		};
	}

	//---------------------------------------------------
	//-- Framework Events ( On Marker Discovered ) ------
	//---------------------------------------------------

	const char* FrameworkAPI::OnMapMarkerDiscovered(RE::TESFullName* a_form)
	{
		AddMapMarkerDiscovery_Invoke(a_form->GetFullName());

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->ProcessHookedMarker(a_form->GetFullName());
		};

		return _OnMapMarkerDiscovered(a_form);
	}

	//---------------------------------------------------
	//-- Framework Events ( Process Caught Fish ) -------
	//---------------------------------------------------

	void FrameworkAPI::ProcessCaughtFishFromPapyrus(RE::StaticFunctionTag*, RE::TESForm* a_form) {
		using ret = Serialization::CompletionistLog;

		for (auto& [pageName, patchData] : CExternalPatchHandler::Get()) {
			patchData->OnFishCaught(a_form);
		};
	};

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Total ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_TotalByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleTotalSet(a_ID);

		int modifiedValue = value;
		modifiedValue = CHCMHandler::MCMAPI::GetDisplayValueForTotalAndFoundItemsByIdentifier(a_ID, modifiedValue);

		//INFO("Returning total count for framework {} with a value of: {}", std::to_underlying(a_ID), modifiedValue);
		return modifiedValue < 0 ? 0 : modifiedValue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Found ) ---------
	//---------------------------------------------------

	std::int32_t FrameworkAPI::GetEntries_FoundByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& value = HandleFoundSet(a_ID);

		int modifiedValue = value;
		modifiedValue = CHCMHandler::MCMAPI::GetDisplayValueForTotalAndFoundItemsByIdentifier(a_ID, modifiedValue);

		//INFO("Returning found count for framework {} with a value of: {}", std::to_underlying(a_ID), modifiedValue);
		return modifiedValue < 0 ? 0 : modifiedValue;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Forms ) ---------
	//---------------------------------------------------

	std::vector<RE::TESForm*> FrameworkAPI::GetFormArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleFormSet(a_ID);
		//INFO("Returning form array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Names ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetNameArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleNameSet(a_ID);
		//INFO("Returning name array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Texts ) ---------
	//---------------------------------------------------

	std::vector<std::string> FrameworkAPI::GetTextArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleTextSet(a_ID);
		//INFO("Returning text array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Getter - Bools ) ---------
	//---------------------------------------------------

	std::vector<bool> FrameworkAPI::GetBoolArrayByID(RE::StaticFunctionTag*, std::int32_t a_ID) {

		auto& array = HandleBoolSet(a_ID);
		//INFO("Returning bool array for framework {} with a size of: {}", std::to_underlying(a_ID), array.size());
		return array;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Getter - Status ) ----
	//---------------------------------------------------

	std::int32_t FrameworkAPI::IsOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {
		if (auto t_pos = std::ranges::find(HandleFormSet(a_ID), a_form); t_pos != HandleFormSet(a_ID).end()) {
			return std::int32_t(HandleBoolSet(a_ID)[std::distance(HandleFormSet(a_ID).begin(), t_pos)]);
		}
		return -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( MCM Setter - Status ) ----
	//---------------------------------------------------

	void FrameworkAPI::SetOptionCompleted(RE::StaticFunctionTag*, std::int32_t a_ID, RE::TESForm* a_form) {

		if (auto t_pos = std::ranges::find(HandleFormSet(a_ID), a_form); t_pos != HandleFormSet(a_ID).end()) {
			auto b_pos = std::distance(HandleFormSet(a_ID).begin(), t_pos);

			if (HandleBoolSet(a_ID).at(b_pos)) {
				HandleBoolSet(a_ID).at(b_pos) = false;

				if (HandleNoShow(a_ID)) { FoundItemData_NoShow.RemoveForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }
				else { FoundItemData.RemoveForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(a_ID).GetAllVariations()) {
					if (HandleDataSet(a_ID).GetBase(var) == HandleFormSet(a_ID).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(a_ID)) { FoundItemData_NoShow.RemoveForm(var); }
						else { FoundItemData.RemoveForm(var); }

					}
				}
			}
			else {
				HandleBoolSet(a_ID).at(b_pos) = true;

				if (HandleNoShow(a_ID)) { FoundItemData_NoShow.AddForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }
				else { FoundItemData.AddForm(HandleFormSet(a_ID).at(b_pos)->GetFormID()); }

				for (auto var : HandleDataSet(a_ID).GetAllVariations()) {
					if (HandleDataSet(a_ID).GetBase(var) == HandleFormSet(a_ID).at(b_pos)->GetFormID()) {
						
						if (HandleNoShow(a_ID)) { FoundItemData_NoShow.AddForm(var); }
						else { FoundItemData.AddForm(var); }

					}
				}
			}

			HandleTotalSet(a_ID) = HandleFormSet(a_ID).size();
			HandleFoundSet(a_ID) = std::ranges::count(HandleBoolSet(a_ID), true);
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Has Book Spell / Read ) --
	//---------------------------------------------------

	bool FrameworkAPI::IsBookKnown(RE::TESForm* a_form) {

		auto* itm = static_cast<RE::TESObjectBOOK*>(a_form);
		auto* pcr = RE::PlayerCharacter::GetSingleton();
		if (!itm || !pcr) { return false; }

		
		if (CVariables::V_TreatBooksAsItems && pcr->GetItemCount(itm) > 0) {
			FoundItemData.AddForm(itm);
			return FoundItemData.HasForm(a_form);
		}

		if (itm->IsRead() || (itm->GetSpell() ? pcr->HasSpell(itm->GetSpell()) : false)) {
			FoundItemData.AddForm(itm);
		}

		return FoundItemData.HasForm(itm);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Enchantment Known ) ---
	//---------------------------------------------------

	bool FrameworkAPI::IsEnchantmentKnown(RE::TESForm* a_form) {
	
		auto* enchantment = static_cast<RE::EnchantmentItem*>(a_form); 
		if (enchantment && enchantment->GetKnown()) {
			FoundItemData_NoShow.AddForm(enchantment);
		}
		return FoundItemData_NoShow.HasForm(enchantment);
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Item Known ) ----------
	//---------------------------------------------------

	bool FrameworkAPI::IsItemKnownExternal(RE::StaticFunctionTag*, RE::TESForm* a_form) 
	{
		return a_form && (FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID()));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Is Item Known ) ----------
	//---------------------------------------------------

	bool FrameworkAPI::IsItemKnown(RE::TESForm* a_form, Serialization::CompletionistData* a_data = nullptr) {

		if (!a_form) { return false; }

		auto base = a_data->GetBase(a_form->GetFormID()) ? a_data->GetBase(a_form->GetFormID()) : a_form->GetFormID();
		auto* pcr = RE::PlayerCharacter::GetSingleton();

		if (pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(base)) > 0) {
			FoundItemData.AddForm(base);
			for (auto var : a_data->GetAllVariations()) {
				if (a_data->GetBase(var) == base) {
					FoundItemData.AddForm(var);
				}
			}
			return FoundItemData.HasForm(a_form);
		}

		for (auto var : a_data->GetAllVariations()) {
			if (a_data->GetBase(var) == base && pcr->GetItemCount(a_data->GetForm<RE::TESBoundObject>(var)) > 0) {

				FoundItemData.AddForm(base);
				for (auto variation : a_data->GetAllVariations()) {
					if (a_data->GetBase(variation) == base) {
						FoundItemData.AddForm(variation);
					}
				}
				return FoundItemData.HasForm(a_form);
			}
		}

		return FoundItemData.HasForm(a_form);
	};
};