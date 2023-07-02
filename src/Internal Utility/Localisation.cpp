#include "Localisation.hpp"
#include "DKUtil/Utility.hpp"

namespace CLocalisation {

	//---------------------------------------------------
	//-- Localisation ( Register Localisation Class ) ---
	//---------------------------------------------------

	void LocalisationAPI::Register() {
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);
		BuildLocalisedMaps();
	};

	//---------------------------------------------------
	//-- Localisation ( Register Papyrus Functions ) ----
	//---------------------------------------------------

	auto LocalisationAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) -> bool
	{
		a_vm->RegisterFunction("GetStringLength", "Completionist_Native", GetStringLength);
		a_vm->RegisterFunction("IsMatchingShout", "Completionist_Native", IsMatchingShout);
		a_vm->RegisterFunction("GetLocStringByKeyExt", "Completionist_Native", GetLocStringByKeyExt);
		return true;
	}

	int32_t LocalisationAPI::GetStringLength(RE::StaticFunctionTag*, RE::BSFixedString s_key)
	{
		return s_key.length();
	};

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Name With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocNameByKey(const char* s_key)
	{
		return localisedMap_N.contains(s_key) ? localisedMap_N.at(s_key).c_str() : s_key;
	};

	std::string LocalisationAPI::GetLocNameByKeyExt(RE::StaticFunctionTag*, std::string s_key)
	{
		return localisedMap_N.contains(s_key) ? localisedMap_N.at(s_key).c_str() : s_key.c_str();
	};

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Desc With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocDescriptionByKey(const char* s_key)
	{
		return localisedMap_D.contains(s_key) ? localisedMap_D.at(s_key).c_str() : s_key;
	};

	std::string LocalisationAPI::GetLocDescriptionByKeyExt(RE::StaticFunctionTag*, std::string s_key)
	{
		return localisedMap_D.contains(s_key) ? localisedMap_D.at(s_key).c_str() : s_key.c_str();
	};

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Misc With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocStringByKey(const char* s_key)
	{
		return localisedMap_M.contains(s_key) ? localisedMap_M.at(s_key).c_str() : s_key;
	};

	std::string LocalisationAPI::GetLocStringByKeyExt(RE::StaticFunctionTag*, std::string s_key)
	{
		return localisedMap_M.contains(s_key) ? localisedMap_M.at(s_key).c_str() : s_key.c_str();
	};

	//---------------------------------------------------
	//-- Localisation ( Is Matching Shout ) -------------
	//---------------------------------------------------

	bool LocalisationAPI::IsMatchingShout(RE::StaticFunctionTag*, std::string s_key, std::string s_key2)
	{
		return s_key.contains("(") && DKUtil::string::iequals(s_key.substr(0, s_key.find("(") - 1), s_key2);
	};

	//---------------------------------------------------
	//-- Localisation Function ( Build Localised Map ) --
	//---------------------------------------------------

	void LocalisationAPI::BuildLocalisedMaps() {

		if (localisedMap_D.size() && localisedMap_N.size() && localisedMap_M.size()) {
			return;
		}

		std::ifstream newfile_M(fmt::format(R"(.\Data\SKSE\Plugins\Completionist_Translations_M.txt)"sv));
		std::ifstream newfile_N(fmt::format(R"(.\Data\SKSE\Plugins\Completionist_Translations_N.txt)"sv));
		std::ifstream newfile_D(fmt::format(R"(.\Data\SKSE\Plugins\Completionist_Translations_D.txt)"sv));
		std::string tempstring;

		INFO("Generating Localised Maps");

		while (newfile_M.is_open() && getline(newfile_M, tempstring))
		{
			if (tempstring.starts_with("-") || tempstring.empty()) {
				continue;
			}

			localisedMap_M.emplace(tempstring.substr(0, tempstring.find("	")), tempstring.substr(tempstring.find("|") + 1));
		}
		newfile_M.close();

		while (newfile_N.is_open() && getline(newfile_N, tempstring))
		{
			if (tempstring.starts_with("-") || tempstring.empty()) {
				continue;
			}

			localisedMap_N.emplace(tempstring.substr(0, tempstring.find("	")), tempstring.substr(tempstring.find("|") + 1));
		}
		newfile_N.close();

		while (newfile_D.is_open() && getline(newfile_D, tempstring))
		{
			if (tempstring.starts_with("-") || tempstring.empty()) {
				continue;
			}

			localisedMap_D.emplace(tempstring.substr(0, tempstring.find("	")), tempstring.substr(tempstring.find("|") + 1));
		}
		newfile_D.close();
	}
}