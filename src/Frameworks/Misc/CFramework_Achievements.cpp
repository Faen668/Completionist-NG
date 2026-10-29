#include "CFramework_Achievements.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Frameworks/Misc/CFramework_Shouts.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"

namespace CFramework_Achievements
{
	namespace
	{
		// Steam's flat API, from the game's own steam_api64.dll
		struct Steam
		{
			void* (*userStats)(){ nullptr };
			void* (*utils)(){ nullptr };
			bool (*requestStats)(void*){ nullptr };
			bool (*getAchievement)(void*, const char*, bool*, std::uint32_t*){ nullptr };
			const char* (*getAttribute)(void*, const char*, const char*){ nullptr };
			bool (*getPercent)(void*, const char*, float*){ nullptr };
			std::uint64_t (*requestPercents)(void*){ nullptr };
			std::int32_t (*getIcon)(void*, const char*){ nullptr };
			bool (*getImageSize)(void*, std::int32_t, std::uint32_t*, std::uint32_t*){ nullptr };
			bool (*getImageRGBA)(void*, std::int32_t, std::uint8_t*, std::int32_t){ nullptr };
			std::uint32_t (*getCount)(void*){ nullptr };
			const char* (*getName)(void*, std::uint32_t){ nullptr };
			bool percentsAsked{ false };
			bool requestResult{ false };
		};

		// Galaxy's GalaxyID, with its own copy constructor so it's passed as the SDK passes it
		struct GalaxyID
		{
			GalaxyID() = default;
			GalaxyID(const GalaxyID& a_other) : value(a_other.value) {}
			GalaxyID& operator=(const GalaxyID& a_other) { value = a_other.value; return *this; }

			std::uint64_t value{ 0 };
		};

		// galaxy::api::IStats, as far as GetAchievement
		class IGalaxyStats
		{
		public:
			virtual ~IGalaxyStats() = default;
			virtual void         RequestUserStatsAndAchievements(GalaxyID a_user, void* a_listener) = 0;
			virtual std::int32_t GetStatInt(const char* a_name, GalaxyID a_user) = 0;
			virtual float        GetStatFloat(const char* a_name, GalaxyID a_user) = 0;
			virtual void         SetStatInt(const char* a_name, std::int32_t a_value) = 0;
			virtual void         SetStatFloat(const char* a_name, float a_value) = 0;
			virtual void         UpdateAvgRateStat(const char* a_name, float a_count, double a_length) = 0;
			virtual void         GetAchievement(const char* a_name, bool& a_unlocked, std::uint32_t& a_time, GalaxyID a_user) = 0;
		};

		// galaxy::api::IError
		class IGalaxyError
		{
		public:
			virtual ~IGalaxyError() = default;
			virtual const char* GetName() const = 0;
			virtual const char* GetMsg() const = 0;
		};

		struct Galaxy
		{
			IGalaxyStats* (*stats)(){ nullptr };
			const IGalaxyError* (*error)(){ nullptr };
			bool requested{ false };
		};

		// What was wrong with a store DLL that was there
		enum class Reason : std::uint8_t
		{
			kNone,
			kSteamOff,
			kSteamCalls,
			kGalaxyCalls,
		};

		Store  store{ Store::kNone };
		Reason steamReason{ Reason::kNone };
		Reason galaxyReason{ Reason::kNone };
		Steam  steam;
		Galaxy galaxy;
		bool   canUnlock{ false };
		bool   failLogged{ false };

		// A failure is listed in full the first few times, then in a line
		constexpr std::int32_t maxDumps = 3;
		std::int32_t           dumps{ 0 };

		std::chrono::steady_clock::time_point requestedAt{};

		// As the game's scripts award them: quest stages from their fragments, AchievementsScript's stats and counters
		constexpr Goal goals[] = {
			{ 1, "#Stage:Skyrim.esm:03372B:900", 1, "Done", true },
			{ 2, "#Stage:Skyrim.esm:0D0800:190", 1, "Done", true },
			{ 3, "#Stage:Skyrim.esm:0242BA:160", 1, "Done", true },
			{ 4, "#Stage:Skyrim.esm:035D5F:250|255", 1, "Done", true },
			{ 5, "#Stage:Skyrim.esm:036192:280", 1, "Done", true },
			{ 6, "#Stage:Skyrim.esm:02D516:100", 1, "Done", true },
			{ 7, "#Stage:Skyrim.esm:044C56:220", 1, "Done", true },
			{ 8, "#Stage:Skyrim.esm:046EF2:200", 1, "Done", true },
			{ 9, "#Faction:Skyrim.esm:048362", 1, "Done", true },
			{ 10, "#Stage:Skyrim.esm:01CEF4:25|30", 1, "Done", true },
			{ 11, "#Stage:Skyrim.esm:01CEF6:200", 1, "Done", true },
			{ 12, "#Stage:Skyrim.esm:01F251:30", 1, "Done", true },
			{ 13, "#Stage:Skyrim.esm:01F256:200", 1, "Done", true },
			{ 14, "#Stage:Skyrim.esm:01F258:200", 1, "Done", true },
			{ 15, "#Stage:Skyrim.esm:01F326:200", 1, "Done", true },
			{ 16, "#Stage:Skyrim.esm:021555:200", 1, "Done", true },
			{ 17, "#Stage:Skyrim.esm:0D7D69:200", 1, "Done", true },
			{ 18, "#Stage:Skyrim.esm:01EA51:200", 1, "Done", true },
			{ 19, "#Stage:Skyrim.esm:01EA54:200", 1, "Done", true },
			{ 20, "#Stage:Skyrim.esm:01EA59:200", 1, "Done", true },
			{ 21, "#Stage:Skyrim.esm:0D517A:200,Skyrim.esm:0E2D29:200", 1, "Done", true },
			{ 23, "#Stage:Skyrim.esm:0D1444:330", 1, "Done", true },
			{ 24, "Side Quests Completed", 10, "SideQuestsCompleted" },
			{ 25, "Misc Objectives Completed", 50, "MiscObjectivesCompleted" },
			{ 26, "#Script:Skyrim.esm:0FC85D:AchievementsScript:HardWorkerChoppedWood", 1, "WoodChopped", true },
			{ 26, "#Script:Skyrim.esm:0FC85D:AchievementsScript:HardWorkerMinedOre", 1, "OreMined", true },
			{ 26, "#Script:Skyrim.esm:0FC85D:AchievementsScript:HardWorkerCookedFood", 1, "FoodCooked", true },
			{ 27, "Locks Picked", 50, "LocksPicked" },
			{ 27, "Pockets Picked", 50, "PocketsPicked" },
			{ 28, "Persuasions", 1, "Persuasions" },
			{ 28, "Bribes", 1, "Bribes" },
			{ 28, "Intimidations", 1, "Intimidations" },
			{ 30, "Standing Stones Found", 13, "StandingStonesFound" },
			{ 31, "Houses Owned", 1, "HousesOwned" },
			{ 32, "#Stage:Skyrim.esm:0FCC2F:10", 1, "Done", true },
			{ 33, "#Stage:Skyrim.esm:07404E:100", 1, "Done", true },
			{ 34, "Weapons Made+Armor Made", 1, "WeaponsMadeArmorMade" },
			{ 34, "Magic Items Made", 1, "MagicItemsMade" },
			{ 34, "Potions Mixed+Poisons Mixed", 1, "PotionsMixed" },
			{ 35, "#Holds", 9, "Holds" },
			{ 36, "Most Gold Carried", 100000, "MostGoldCarried" },
			{ 37, "Dungeons Cleared", 50, "DungeonsCleared" },
			{ 38, "Skill Increases", 100, "SkillIncreases" },
			{ 39, "Locations Discovered", 100, "LocationsDiscovered" },
			{ 40, "Skill Books Read", 50, "SkillBooksRead" },
			{ 41, "#Script:Skyrim.esm:0FC85D:AchievementsScript:DaedricArtifactCount", 1, "Artifacts" },
			{ 42, "#Script:Skyrim.esm:0FC85D:AchievementsScript:DaedricArtifactCount", 15, "Artifacts" },
			{ 43, "Dragon Souls Collected", 1, "DragonSoulsCollected" },
			{ 44, "Dragon Souls Collected", 20, "DragonSoulsCollected" },
			{ 45, "Shouts Mastered", 1, "ShoutsMastered" },
			{ 46, "Shouts Learned", 20, "ShoutsLearned" },
			{ 47, "Level Increases", 5, "LevelIncreases" },
			{ 48, "Level Increases", 10, "LevelIncreases" },
			{ 49, "Level Increases", 25, "LevelIncreases" },
			{ 50, "Level Increases", 50, "LevelIncreases" },
			{ 51, "#Stage:Dawnguard.esm:00352A:200", 1, "Done", true },
			{ 52, "#Stage:Dawnguard.esm:002850:200", 1, "Done", true },
			{ 53, "#Stage:Dawnguard.esm:007C25:200", 1, "Done", true },
			{ 54, "#Stage:Dawnguard.esm:005817:255", 1, "Done", true },
			{ 55, "#Stage:Dawnguard.esm:00C73A:60", 1, "Done", true },
			{ 57, "NumWerewolfPerks", 11, "NumWerewolfPerks" },
			{ 58, "NumVampirePerks", 11, "NumVampirePerks" },
			{ 60, "#Legendary", 1, "LegendaryDragons" },
			{ 61, "#Script:HearthFires.esm:0042B4:BYOHRelationshipAdoptionScript:numChildrenAdopted", 1, "ChildrenAdopted" },
			{ 62, "#Stage:HearthFires.esm:00305E:100,HearthFires.esm:00BE09:100,HearthFires.esm:010BEE:100", 1, "Plots" },
			{ 63, "#Wings", 3, "Wings" },
			{ 64, "#Stage:HearthFires.esm:00305E:100,HearthFires.esm:00BE09:100,HearthFires.esm:010BEE:100", 3, "Plots" },
			{ 65, "#Houses", 3, "Houses" },
			{ 66, "#Stage:Dragonborn.esm:016E02:100", 1, "Done", true },
			{ 67, "#Stage:Dragonborn.esm:017F8F:200", 1, "Done", true },
			{ 68, "#Stage:Dragonborn.esm:016E1F:550", 1, "Done", true },
			{ 69, "#Stage:Dragonborn.esm:0179D7:550", 1, "Done", true },
			{ 70, "#Global:Dragonborn.esm:027434", 3, "DragonAspectWords" },
			{ 71, "#Script:Dragonborn.esm:016E21:DLC2BookDungeonControllerScript:iBlackBookFinishedCount", 5, "BlackBooks" },
			{ 72, "StalhrimItemsCrafted", 1, "StalhrimItemsCrafted" },
			{ 73, "#Script:Dragonborn.esm:0179E2:DLC2TameDragonScript:iDragonsRiddenCount", 5, "DragonsRidden" },
			{ 74, "#Stage:Dragonborn.esm:018B14:200", 1, "Done", true },
			{ 75, "Solstheim Locations Discovered", 30, "SolstheimLocationsDiscovered" },
		};

		constexpr const char* holds[] = { "Eastmarch Bounty", "Falkreath Bounty", "Haafingar Bounty", "Hjaalmarch Bounty", "The Pale Bounty", "The Reach Bounty", "The Rift Bounty", "Whiterun Bounty", "Winterhold Bounty" };

		// BYOHHouseScript's RoomDoneFlags for each house; rooms 3 to 11 are the wings
		constexpr RE::FormID roomFlags[] = { 0x00B0D2, 0x00B0FC, 0x010C05 };

		// Dawnguard's DLC1LegendaryDragon keyword
		constexpr RE::FormID legendaryKeyword = 0x00CD59;

		std::mutex                                    statLock;
		std::unordered_map<std::string, std::int32_t> statValues;
		std::atomic<std::int32_t>                     statsPending{ 0 };
		std::atomic<bool>                             statsChanged{ false };

		std::string Key(std::int32_t a_number) { return fmt::format("NEW_ACHIEVEMENT_{}_0", a_number); }

		template <class T>
		bool Find(HMODULE a_module, const char* a_name, T& a_out) {

			a_out = reinterpret_cast<T>(GetProcAddress(a_module, a_name));
			return a_out != nullptr;
		}

		const char* Text(const char* a_text) { return a_text ? a_text : ""; }

		std::string Narrow(const wchar_t* a_text) {

			const auto size = WideCharToMultiByte(CP_UTF8, 0, a_text, -1, nullptr, 0, nullptr, nullptr);
			std::string text(size > 0 ? size - 1 : 0, '\0');
			if (size > 1) { WideCharToMultiByte(CP_UTF8, 0, a_text, -1, text.data(), size, nullptr, nullptr); }
			return text;
		}

		std::string List(const std::vector<std::int32_t>& a_numbers) {

			std::string text;
			for (std::size_t i = 0; i < a_numbers.size() && i < 40; ++i) {
				text += (i ? ", " : "") + std::to_string(a_numbers[i]);
			}
			return a_numbers.size() > 40 ? text + ", ..." : text;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Log Module ) -------------
		//---------------------------------------------------

		// Which DLL the game loaded: a swapped one answers differently
		void LogModule(const wchar_t* a_name) {

			wchar_t path[MAX_PATH]{};
			auto*   module = GetModuleHandleW(a_name);
			if (!module || !GetModuleFileNameW(module, path, MAX_PATH)) { return; }

			std::error_code error;
			const auto      size = std::filesystem::file_size(path, error);
			INFO("Achievements: {} is {} ({} bytes)", Narrow(a_name), Narrow(path), error ? 0 : size);
		}

		//---------------------------------------------------
		//-- Framework Functions ( Find Steam ) -------------
		//---------------------------------------------------

		bool FindSteam() {

			auto* module = GetModuleHandleW(L"steam_api64.dll");
			if (!module) { return false; }

			bool (*running)() = nullptr;
			if (!Find(module, "SteamAPI_IsSteamRunning", running)) {
				steamReason = Reason::kSteamCalls;
				return false;
			}
			if (!running()) {
				INFO("Achievements: steam_api64.dll is loaded but Steam isn't running");
				steamReason = Reason::kSteamOff;
				return false;
			}

			const bool found = Find(module, "SteamAPI_SteamUserStats_v012", steam.userStats)
				&& Find(module, "SteamAPI_SteamUtils_v010", steam.utils)
				&& Find(module, "SteamAPI_ISteamUserStats_RequestCurrentStats", steam.requestStats)
				&& Find(module, "SteamAPI_ISteamUserStats_GetAchievementAndUnlockTime", steam.getAchievement)
				&& Find(module, "SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", steam.getAttribute)
				&& Find(module, "SteamAPI_ISteamUserStats_GetAchievementAchievedPercent", steam.getPercent)
				&& Find(module, "SteamAPI_ISteamUserStats_RequestGlobalAchievementPercentages", steam.requestPercents)
				&& Find(module, "SteamAPI_ISteamUserStats_GetAchievementIcon", steam.getIcon)
				&& Find(module, "SteamAPI_ISteamUtils_GetImageSize", steam.getImageSize)
				&& Find(module, "SteamAPI_ISteamUtils_GetImageRGBA", steam.getImageRGBA);
			Find(module, "SteamAPI_ISteamUserStats_GetNumAchievements", steam.getCount);
			Find(module, "SteamAPI_ISteamUserStats_GetAchievementName", steam.getName);
			if (!found) {
				INFO("Achievements: this steam_api64.dll is missing calls we need");
				steamReason = Reason::kSteamCalls;
				return false;
			}
			return true;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Find Galaxy ) ------------
		//---------------------------------------------------

		bool FindGalaxy() {

			auto* module = GetModuleHandleW(L"Galaxy64.dll");
			if (!module) { return false; }

			if (!Find(module, "?Stats@api@galaxy@@YAPEAVIStats@12@XZ", galaxy.stats) || !Find(module, "?GetError@api@galaxy@@YAPEBVIError@12@XZ", galaxy.error)) {
				INFO("Achievements: Galaxy64.dll is loaded but has no Stats or GetError");
				galaxyReason = Reason::kGalaxyCalls;
				return false;
			}
			return true;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Find Unlocker ) ----------
		//---------------------------------------------------

		// Achievements Mods Enabler under any loader, or Engine Fixes unless its patch is turned off
		bool FindUnlocker() {

			HMODULE modules[1024];
			DWORD   needed = 0;
			if (!EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &needed)) { return false; }

			bool engineFixes = false;
			for (DWORD i = 0; i < std::min<DWORD>(needed / sizeof(HMODULE), 1024); ++i) {
				wchar_t name[MAX_PATH]{};
				if (!GetModuleBaseNameW(GetCurrentProcess(), modules[i], name, MAX_PATH)) { continue; }
				auto lower = std::wstring(name);
				std::ranges::transform(lower, lower.begin(), ::towlower);
				if (lower.find(L"achievement") != std::wstring::npos && lower.find(L"enabler") != std::wstring::npos) {
					INFO("Achievements: Achievements Mods Enabler lets them unlock with mods");
					return true;
				}
				engineFixes |= lower == L"enginefixes.dll";
			}
			if (!engineFixes) { return false; }

			std::ifstream file("Data/SKSE/Plugins/EngineFixes.toml");
			std::string   line;
			while (std::getline(file, line)) {
				std::erase_if(line, [](char a_char) { return a_char == ' ' || a_char == '\t'; });
				if (line.starts_with("bEnableAchievementsWithMods=")) {
					INFO("Achievements: Engine Fixes has bEnableAchievementsWithMods {}", line.substr(line.find('=') + 1));
					return line.find("true") != std::string::npos;
				}
			}
			INFO("Achievements: Engine Fixes is loaded, so they unlock with mods");
			return true;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Log Steam Failure ) ------
		//---------------------------------------------------

		void LogSteamFailure(void* a_stats, const std::vector<std::int32_t>& a_failed) {

			if (dumps++ >= maxDumps) {
				INFO("Achievements: Steam still has no answer for {} of {}", a_failed.size(), Count);
				return;
			}

			const auto ago = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - requestedAt).count();
			INFO("Achievements: Steam has no answer for {} of {}: {}", a_failed.size(), Count, List(a_failed));
			INFO("Achievements: stats were asked for {} ms ago, RequestCurrentStats said {}", ago, steam.requestResult ? "yes" : "no");
			LogModule(L"steam_api64.dll");
			if (!steam.getCount || !steam.getName) { return; }

			// Its own list, to tell a name it doesn't have from stats that haven't arrived
			const auto               total = steam.getCount(a_stats);
			std::vector<std::string> names;
			for (std::uint32_t i = 0; i < total && i < 200; ++i) {
				names.emplace_back(Text(steam.getName(a_stats, i)));
			}
			INFO("Achievements: Steam lists {} achievements for this game", total);

			std::string first;
			for (std::size_t i = 0; i < names.size() && i < 5; ++i) {
				first += (i ? ", " : "") + names[i];
			}
			INFO("Achievements: the first of them: {}", first.empty() ? "none" : first);

			std::vector<std::int32_t> missing;
			for (const auto number : a_failed) {
				if (std::ranges::find(names, Key(number)) == names.end()) {
					missing.push_back(number);
				}
			}
			INFO("Achievements: {} of those are not in its list{}", missing.size(), missing.empty() ? ", so the stats haven't arrived" : ": " + List(missing));
		}

		//---------------------------------------------------
		//-- Framework Functions ( Refresh Steam ) ----------
		//---------------------------------------------------

		bool RefreshSteam(std::vector<Achievement>& a_out) {

			auto* stats = steam.userStats();
			if (!stats) {
				if (dumps++ < maxDumps) { INFO("Achievements: Steam gave no user stats"); }
				return false;
			}

			if (!steam.percentsAsked) {
				steam.requestResult = steam.requestStats(stats);
				steam.requestPercents(stats);
				steam.percentsAsked = true;
				requestedAt = std::chrono::steady_clock::now();
			}

			std::vector<std::int32_t> failed;
			for (std::int32_t i = 1; i <= Count; ++i) {
				const auto  key = Key(i);
				Achievement achievement{ i };
				if (!steam.getAchievement(stats, key.c_str(), &achievement.unlocked, &achievement.time)) {
					failed.push_back(i);
					continue;
				}
				if (const auto* name = steam.getAttribute(stats, key.c_str(), "name")) { achievement.name = name; }
				if (const auto* desc = steam.getAttribute(stats, key.c_str(), "desc")) { achievement.description = desc; }
				float percent = 0.0f;
				if (steam.getPercent(stats, key.c_str(), &percent)) { achievement.percent = percent; }
				achievement.icon = steam.getIcon(stats, key.c_str());
				a_out.push_back(std::move(achievement));
			}

			if (!failed.empty()) {
				LogSteamFailure(stats, failed);
				return false;
			}
			return true;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Refresh Galaxy ) ---------
		//---------------------------------------------------

		bool RefreshGalaxy(std::vector<Achievement>& a_out) {

			auto* stats = galaxy.stats();
			if (!stats) {
				if (dumps++ < maxDumps) { INFO("Achievements: Galaxy gave no stats"); }
				return false;
			}

			std::vector<std::int32_t> failed;
			std::vector<std::string>  reasons;
			for (std::int32_t i = 1; i <= Count; ++i) {
				const auto  key = Key(i);
				Achievement achievement{ i };
				stats->GetAchievement(key.c_str(), achievement.unlocked, achievement.time, GalaxyID{});
				if (const auto* error = galaxy.error()) {
					failed.push_back(i);
					const auto reason = fmt::format("{}: {}", Text(error->GetName()), Text(error->GetMsg()));
					if (reasons.size() < 3 && std::ranges::find(reasons, reason) == reasons.end()) {
						reasons.push_back(reason);
					}
					continue;
				}
				a_out.push_back(std::move(achievement));
			}
			if (failed.empty()) {
				return true;
			}

			// The game asks for them itself; once more in case it hasn't yet
			const bool asked = !galaxy.requested;
			if (asked) {
				galaxy.requested = true;
				stats->RequestUserStatsAndAchievements(GalaxyID{}, nullptr);
			}

			if (dumps++ >= maxDumps) {
				INFO("Achievements: GOG Galaxy still has no answer for {} of {}", failed.size(), Count);
				return false;
			}
			INFO("Achievements: GOG Galaxy has no answer for {} of {}: {}", failed.size(), Count, List(failed));
			for (const auto& reason : reasons) {
				INFO("Achievements: Galaxy said {}", reason);
			}
			INFO("Achievements: stats were asked for just now: {}", asked ? "yes" : "no, already asked");
			LogModule(L"Galaxy64.dll");
			return false;
		}

		//---------------------------------------------------
		//-- Framework Functions ( Stat Callback ) ----------
		//---------------------------------------------------

		class StatCallback final : public RE::BSScript::IStackCallbackFunctor
		{
		public:
			StatCallback(std::string a_name, std::function<void()> a_changed) : name(std::move(a_name)), changed(std::move(a_changed)) {}

			void operator()(RE::BSScript::Variable a_result) override {

				if (a_result.IsInt()) {
					std::scoped_lock lock(statLock);
					auto& value = statValues[name];
					if (value != a_result.GetSInt()) {
						value = a_result.GetSInt();
						statsChanged = true;
					}
				}
				if (--statsPending == 0 && statsChanged.exchange(false) && changed) {
					SKSE::GetTaskInterface()->AddTask(changed);
				}
			}

			void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override {}

		private:
			std::string           name;
			std::function<void()> changed;
		};
	}

	//---------------------------------------------------
	//-- Framework Functions ( Install Framework ) ------
	//---------------------------------------------------

	// Steam or GOG, from the store DLL the game loaded; Game Pass has neither
	void CHandler::InstallFramework() {

		store = FindSteam() ? Store::kSteam : FindGalaxy() ? Store::kGOG : Store::kNone;
		INFO("Achievements: {}", store == Store::kSteam ? "reading them from Steam" : store == Store::kGOG ? "reading them from GOG Galaxy" : "no Steam or GOG");
		if (store != Store::kNone) {
			LogModule(store == Store::kSteam ? L"steam_api64.dll" : L"Galaxy64.dll");
		}

		canUnlock = FindUnlocker();
		if (!canUnlock) {
			INFO("Achievements: no Achievements Mods Enabler or Engine Fixes, so mods keep new ones from unlocking");
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Store In Use ) -----------
	//---------------------------------------------------

	Store CHandler::StoreInUse() noexcept {

		return store;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Refresh ) ----------------
	//---------------------------------------------------

	// False when there's no store, or it couldn't answer for every one
	bool CHandler::Refresh(std::vector<Achievement>& a_out) {

		a_out.clear();
		const bool done = store == Store::kSteam ? RefreshSteam(a_out) : store == Store::kGOG ? RefreshGalaxy(a_out) : false;
		if (!done) {
			a_out.clear();
			failLogged = true;
			return false;
		}
		dumps = 0;
		if (failLogged) {
			INFO("Achievements: read all {} after all", Count);
			failLogged = false;
		}
		return true;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Why Not ) ----------------
	//---------------------------------------------------

	const char* CHandler::WhyNot() noexcept {

		if (store == Store::kSteam) { return "UI_Achievements_WhyNoAnswerSteam"; }
		if (store == Store::kGOG) { return "UI_Achievements_WhyNoAnswerGalaxy"; }
		switch (steamReason != Reason::kNone ? steamReason : galaxyReason) {
		case Reason::kSteamOff: return "UI_Achievements_WhySteamOff";
		case Reason::kSteamCalls: return "UI_Achievements_WhySteamCalls";
		case Reason::kGalaxyCalls: return "UI_Achievements_WhyGalaxyCalls";
		default: return "UI_Achievements_WhyNoStore";
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Icon Pixels ) ------------
	//---------------------------------------------------

	bool CHandler::IconPixels(std::int32_t a_icon, std::vector<std::uint8_t>& a_rgba, std::uint32_t& a_width, std::uint32_t& a_height) {

		if (store != Store::kSteam || a_icon <= 0) { return false; }
		auto* utils = steam.utils();
		if (!utils || !steam.getImageSize(utils, a_icon, &a_width, &a_height) || a_width == 0 || a_height == 0 || a_width > 512 || a_height > 512) { return false; }

		a_rgba.resize(static_cast<std::size_t>(a_width) * a_height * 4);
		return steam.getImageRGBA(utils, a_icon, a_rgba.data(), static_cast<std::int32_t>(a_rgba.size()));
	}

	//---------------------------------------------------
	//-- Framework Functions ( Can Unlock ) -------------
	//---------------------------------------------------

	bool CHandler::CanUnlock() noexcept {

		return canUnlock;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Goals ) ------------------
	//---------------------------------------------------

	std::span<const Goal> CHandler::Goals() {

		return goals;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Query Stats ) ------------
	//---------------------------------------------------

	// Answers come back on the script thread; a_changed runs on the game thread when any moved
	void CHandler::QueryStats(std::function<void()> a_changed) {

		auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm || statsPending > 0) { return; }

		std::vector<std::string> names(std::begin(holds), std::end(holds));
		for (const auto& goal : goals) {
			std::string_view stat = goal.stat;
			while (!stat.empty() && !stat.starts_with('#')) {
				const auto plus = stat.find('+');
				const auto name = std::string(stat.substr(0, plus));
				if (name != "Level" && name != "Skill" && std::ranges::find(names, name) == names.end()) {
					names.push_back(name);
				}
				stat = plus == std::string_view::npos ? std::string_view{} : stat.substr(plus + 1);
			}
		}

		statsPending = static_cast<std::int32_t>(names.size());
		for (const auto& name : names) {
			auto*             args = RE::MakeFunctionArguments(RE::BSFixedString(name));
			ScriptCallbackPtr callback(new StatCallback(name, a_changed));
			if (!vm->DispatchStaticCall("Game", "QueryStat", args, callback)) {
				--statsPending;
			}
			delete args;
		}
	}

	//---------------------------------------------------
	//-- Framework Functions ( Stat ) -------------------
	//---------------------------------------------------

	// -1 until the game has answered
	std::int32_t CHandler::Stat(std::string_view a_name) {

		std::scoped_lock lock(statLock);
		const auto       it = statValues.find(std::string(a_name));
		return it != statValues.end() ? it->second : -1;
	}

	//---------------------------------------------------
	//-- Framework Functions ( Value ) ------------------
	//---------------------------------------------------

	// A goal's value on this save, -1 when it can't be told yet
	std::int32_t CHandler::Value(std::string_view a_stat) {

		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!dataHandler || !player) { return -1; }

		const auto take = [](std::string_view& a_text, char a_separator) {
			const auto end = a_text.find(a_separator);
			const auto part = a_text.substr(0, end);
			a_text = end == std::string_view::npos ? std::string_view{} : a_text.substr(end + 1);
			return part;
		};
		// "Plugin.esm:FormID" off the front
		const auto form = [&](std::string_view& a_text) -> RE::TESForm* {
			const auto plugin = take(a_text, ':');
			const auto id = static_cast<RE::FormID>(std::strtoul(std::string(take(a_text, ':')).c_str(), nullptr, 16));
			return dataHandler->LookupForm(id, plugin);
		};

		// Quests at one of their stages: "Plugin.esm:FormID:stage|stage,..."
		if (a_stat.starts_with("#Stage:")) {
			auto         list = a_stat.substr(7);
			std::int32_t done = 0;
			while (!list.empty()) {
				auto  one = take(list, ',');
				auto* quest = form(one);
				auto* asQuest = quest ? quest->As<RE::TESQuest>() : nullptr;
				bool  reached = false;
				while (asQuest && !one.empty() && !reached) {
					reached = CQuestMaster::QuestAPI::IsStageDone(asQuest, std::atoi(std::string(take(one, '|')).c_str()));
				}
				done += reached ? 1 : 0;
			}
			return done;
		}

		// A script's variable or property on a quest: "Plugin.esm:FormID:Script:Name"
		if (a_stat.starts_with("#Script:")) {
			auto        rest = a_stat.substr(8);
			auto*       quest = form(rest);
			const auto  script = std::string(take(rest, ':'));
			const auto  name = std::string(rest);
			auto*       vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
			const auto  handle = quest && vm ? vm->GetObjectHandlePolicy()->GetHandleForObject(quest->GetFormType(), quest) : 0;
			ScriptObjectPtr object;
			if (!handle || !vm->FindBoundObject(handle, script.c_str(), object) || !object) { return -1; }
			const auto* value = object->GetProperty(name);
			value = value ? value : object->GetVariable(name);
			return !value ? -1 : value->IsBool() ? (value->GetBool() ? 1 : 0) : value->IsInt() ? value->GetSInt() : -1;
		}

		// The player in a faction: "Plugin.esm:FormID"
		if (a_stat.starts_with("#Faction:")) {
			auto  rest = a_stat.substr(9);
			auto* faction = form(rest);
			return faction && faction->Is(RE::FormType::Faction) ? (player->IsInFaction(faction->As<RE::TESFaction>()) ? 1 : 0) : -1;
		}

		// A global's value: "Plugin.esm:FormID"
		if (a_stat.starts_with("#Global:")) {
			auto        rest = a_stat.substr(8);
			const auto* global = form(rest);
			return global && global->Is(RE::FormType::Global) ? static_cast<std::int32_t>(global->As<RE::TESGlobal>()->value) : -1;
		}

		// Holds with a 1,000 bounty: the game keeps each one's bounty as it is now
		if (a_stat == "#Holds") {
			std::int32_t count = 0;
			for (const auto* hold : holds) {
				const auto bounty = Stat(hold);
				if (bounty < 0) { return -1; }
				count += bounty >= 1000 ? 1 : 0;
			}
			return count;
		}

		// Hearthfire wings: the most on one house, or houses with three
		if (a_stat == "#Wings" || a_stat == "#Houses") {
			std::int32_t best = 0;
			std::int32_t complete = 0;
			for (const auto id : roomFlags) {
				auto* list = dataHandler->LookupForm<RE::BGSListForm>(id, "HearthFires.esm");
				if (!list) { return -1; }
				std::int32_t wings = 0;
				for (std::uint32_t room = 3; room < 12 && room < list->forms.size(); ++room) {
					const auto* flag = list->forms[room] ? list->forms[room]->As<RE::TESGlobal>() : nullptr;
					wings += flag && flag->value >= 1.0f ? 1 : 0;
				}
				best = std::max(best, wings);
				complete += wings >= 3 ? 1 : 0;
			}
			return a_stat == "#Wings" ? best : complete;
		}

		// Legendary dragons killed, from the dragon kills by kind
		if (a_stat == "#Legendary") {
			auto* keyword = dataHandler->LookupForm<RE::BGSKeyword>(legendaryKeyword, "Dawnguard.esm");
			if (!keyword) { return -1; }
			std::set<std::string> names;
			for (auto* npc : dataHandler->GetFormArray<RE::TESNPC>()) {
				if (npc && npc->HasKeyword(keyword) && npc->GetName() && *npc->GetName()) {
					names.emplace(npc->GetName());
				}
			}
			std::int32_t kills = 0;
			for (const auto& name : names) {
				if (const auto it = CFramework_Master::PlayerDragonKills.data.find(name); it != CFramework_Master::PlayerDragonKills.data.end()) {
					kills += it->second.killCount;
				}
			}
			return kills;
		}
		return -1;
	}
}
