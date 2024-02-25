#include "Localisation.hpp"
#include "DKUtil/Utility.hpp"

namespace CLocalisation {

	//---------------------------------------------------
	//-- Localisation ( Register Localisation Class ) ---
	//---------------------------------------------------

	void LocalisationAPI::Register() {
		SKSE::GetPapyrusInterface()->Register(RegisterFunctions);
		BuildLocalisedMaps();
	}

	//---------------------------------------------------
	//-- Localisation ( Register Papyrus Functions ) ----
	//---------------------------------------------------

	bool LocalisationAPI::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) {
		const char* scriptName = "Completionist_Native";

		a_vm->RegisterFunction("GetStringLength", scriptName, GetStringLength);
		a_vm->RegisterFunction("IsMatchingShout", scriptName, IsMatchingShout);
		a_vm->RegisterFunction("GetLocStringByKeyExt", scriptName, GetLocStringByKeyExt);
		return true;
	}

	//---------------------------------------------------
	//-- Localisation ( GetString Length. ) -------------
	//---------------------------------------------------

	int32_t LocalisationAPI::GetStringLength(RE::StaticFunctionTag*, RE::BSFixedString s_key) {
		return s_key.length();
	}

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Name With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocNameByKey(const char* s_key) {
		return localisedMap_N.contains(s_key) ? localisedMap_N.at(s_key).c_str() : s_key;
	}

	std::string LocalisationAPI::GetLocNameByKeyExt(RE::StaticFunctionTag*, std::string s_key) {
		return localisedMap_N.contains(s_key) ? localisedMap_N.at(s_key) : s_key;
	}

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Desc With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocDescriptionByKey(const char* s_key) {
		return localisedMap_D.contains(s_key) ? localisedMap_D.at(s_key).c_str() : s_key;
	}

	std::string LocalisationAPI::GetLocDescriptionByKeyExt(RE::StaticFunctionTag*, std::string s_key) {
		return localisedMap_D.contains(s_key) ? localisedMap_D.at(s_key) : s_key;
	}

	//---------------------------------------------------
	//-- Localisation ( GetLocalised Misc With Ext. ) ---
	//---------------------------------------------------

	const char* LocalisationAPI::GetLocStringByKey(const char* s_key) {
		return localisedMap_M.contains(s_key) ? localisedMap_M.at(s_key).c_str() : s_key;
	}

	std::string LocalisationAPI::GetLocStringByKeyExt(RE::StaticFunctionTag*, std::string s_key) {
		return localisedMap_M.contains(s_key) ? localisedMap_M.at(s_key) : s_key;
	}

	//---------------------------------------------------
	//-- Localisation ( Is Matching Shout ) -------------
	//---------------------------------------------------

	bool LocalisationAPI::IsMatchingShout(RE::StaticFunctionTag*, std::string s_key, std::string s_key2) {
		return s_key.find('(') != std::string::npos && DKUtil::string::iequals(s_key.substr(0, s_key.find('(') - 1), s_key2);
	}

	//---------------------------------------------------
	//-- Localisation Function ( Build Localised Map ) --
	//---------------------------------------------------

	void LocalisationAPI::BuildLocalisedMaps() {
		const std::string basePath = R"(.\Data\SKSE\Plugins\CompletionistData\Translations\)";

		// Define file paths as constants
		const std::string filePath_M = basePath + "Completionist_Translations_M.txt";
		const std::string filePath_N = basePath + "Completionist_Translations_N.txt";
		const std::string filePath_D = basePath + "Completionist_Translations_D.txt";

		// Check if maps are already populated
		if (!localisedMap_D.empty() && !localisedMap_N.empty() && !localisedMap_M.empty()) {
			return;
		}

		// Read and populate maps
		ReadAndPopulateMap(filePath_M, localisedMap_M);
		ReadAndPopulateMap(filePath_N, localisedMap_N);
		ReadAndPopulateMap(filePath_D, localisedMap_D);
		INFO("Generated Localised Maps");
	}

	void LocalisationAPI::ReadAndPopulateMap(const std::string& filePath, std::unordered_map<std::string, std::string>& targetMap) {
		std::ifstream newfile(filePath);
		std::string tempstring;

		if (!newfile.is_open()) {
			ERROR("Failed to open file: {}", filePath);
			return;
		}

		while (getline(newfile, tempstring)) {
			if (!tempstring.empty() && !tempstring.starts_with("-")) {
				size_t tabPos = tempstring.find("	");
				size_t pipePos = tempstring.find("|");

				if (tabPos != std::string::npos && pipePos != std::string::npos) {
					targetMap.emplace(tempstring.substr(0, tabPos), tempstring.substr(pipePos + 1));
				}
			}
		}
		newfile.close();
	}
}