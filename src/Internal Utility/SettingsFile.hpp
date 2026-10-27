#pragma once
#include "SimpleIni.h"

namespace CSettingsFile
{
	// Settings are saved to My Games\<Skyrim>\SKSE\Completionist.ini, which mod updates never touch.
	// Data/SKSE/Plugins/Completionist.ini is only read, as defaults over the code's own (a modlist can ship one).

	// Both files, the user's on top: for reading settings.
	SI_Error Load(CSimpleIniA& a_ini);

	// The user's file alone: load into this before setting values and saving.
	SI_Error LoadUser(CSimpleIniA& a_ini);
	SI_Error SaveUser(CSimpleIniA& a_ini);
}
