#include "SettingsFile.hpp"

namespace CSettingsFile
{
	namespace
	{
		constexpr const char* defaultsFilePath = "Data/SKSE/Plugins/Completionist.ini";

		// Written when there's no settings file at all, so the hand-edited options are explained.
		constexpr std::string_view newFileText = R"([Completionist]

;==================================================
; General Settings
;==================================================

; Developer Logging Options
;--------------------------------------------------
; These settings are intended for development and
; debugging purposes only. They may produce large
; amounts of log output and impact performance.
;
; Enable only if you are diagnosing issues or
; actively developing/debugging the mod.
;
MuseumAPILogging = false
Log_All_Patch_Installs = false

;==================================================
; Tracked Entity Types
;--------------------------------------------------
; Defines which categories are included in progress
; tracking. Multiple values can be specified as a
; comma-separated list.
;
; Available Types:
;
; 0 = Items
;     Weapons, armor, misc items, potions, food, etc.
;
; 1 = Books
;     Books, notes, spell tomes, skill books, etc.
;
; 2 = Locations
;     Map markers only.
;     Must have base object FormID: 00000010.
;
; 3 = Enchantments
;     "Object Effect" in SSEEdit.
;     Must be the playable base enchantment.
;
; 4 = Shouts
;
; 5 = Quests
;
; 6 = Fish
;
; 7 = Player Homes
;
; 8 = Pets
;
; 9 = Interactable Objects
;     Object references that complete on activation.
;
; Example:
; TrackedEntityTypes = 0,1,4,5
; (Tracks Items, Books, Shouts, and Quests only)
TrackedEntityTypes = 0,1,2,3,4,5,6,7,8,9

; Restrict Book Tracking to Specific Types
;--------------------------------------------------
; When enabled, limits tracked books to:
; - Spell Tomes
; - Skill Books
;
; Only applies if "Books" (type 1) is included in
; TrackedEntityTypes.
;
; false = Track all books (default behavior)
; true  = Track only spell tomes and skill books
;
RestrictBooksToTomesAndSkillBooks = false
)";

		bool Failed(SI_Error a_rc) {

			return a_rc < 0 && a_rc != SI_FILE;
		}

		//---------------------------------------------------
		//-- Settings File ( User Path ) --------------------
		//---------------------------------------------------

		const std::filesystem::path& UserPath() {

			static const std::filesystem::path path = [] {
				auto directory = SKSE::log::log_directory();
				if (!directory) {
					INFO("Settings file: no My Games folder, saving to {}", defaultsFilePath);
					return std::filesystem::path(defaultsFilePath);
				}

				std::error_code error;
				std::filesystem::create_directories(*directory, error);
				auto user = *directory / "Completionist.ini";

				// Older versions saved into the Data file: carry it over once so nobody loses their settings.
				if (!std::filesystem::exists(user, error) && std::filesystem::exists(defaultsFilePath, error)) {
					if (std::filesystem::copy_file(defaultsFilePath, user, error)) {
						INFO("Settings file: moved settings from {} to {}", defaultsFilePath, user.string());
					} else {
						INFO("Settings file: couldn't copy {} to {}: {}", defaultsFilePath, user.string(), error.message());
					}
				}

				if (!std::filesystem::exists(user, error)) {
					if (std::ofstream file(user, std::ios::binary); file && file.write(newFileText.data(), newFileText.size())) {
						INFO("Settings file: created {}", user.string());
					} else {
						INFO("Settings file: couldn't create {}", user.string());
					}
				}
				return user;
			}();
			return path;
		}
	}

	//---------------------------------------------------
	//-- Settings File ( Load ) -------------------------
	//---------------------------------------------------

	SI_Error Load(CSimpleIniA& a_ini) {

		// A broken defaults file only loses its defaults, the user's file still decides.
		const auto& user = UserPath();
		if (user != defaultsFilePath && Failed(a_ini.LoadFile(defaultsFilePath))) {
			INFO("Settings file: unable to load {}", defaultsFilePath);
		}

		const SI_Error rc = a_ini.LoadFile(user.c_str());
		if (Failed(rc)) {
			INFO("Settings file: unable to load {}", user.string());
		}
		return rc;
	}

	//---------------------------------------------------
	//-- Settings File ( Load User ) --------------------
	//---------------------------------------------------

	SI_Error LoadUser(CSimpleIniA& a_ini) {

		return a_ini.LoadFile(UserPath().c_str());
	}

	//---------------------------------------------------
	//-- Settings File ( Save User ) --------------------
	//---------------------------------------------------

	SI_Error SaveUser(CSimpleIniA& a_ini) {

		return a_ini.SaveFile(UserPath().c_str());
	}
}
