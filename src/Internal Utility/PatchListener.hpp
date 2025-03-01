#pragma once

#include "SimpleIni.h"
#include "Serialization.hpp"
#include "MiscPatchStruct.hpp"
#include "Frameworks/Quests/CQuestMaster.hpp"
#include "Frameworks/Quests/Patches/CQuests_Patches.hpp"
#include "Frameworks/Quests/Radiant & Favors/CQuests_Drunks.hpp"
#include "Frameworks/Quests/Radiant & Favors/CQuests_Favors.hpp"
#include "Frameworks/Quests/Radiant & Favors/CQuests_Beggars.hpp"

namespace CExternalPatchHandler
{
	static std::vector<int32_t> usedIdentifiers{};
	static CSimpleIniA ini;
	static std::string filename;


	inline std::vector<std::pair<std::string, CMiscPatch*>> CustomPatches{};
	inline std::vector<std::pair<std::string, CMiscPatch*>> Get() { return CustomPatches; }
	class CHandler {

	public:
		static void Register();
		static int32_t GetRandomID();
		static void ProcessiniFile(std::string file);

		static auto GetNewQuestData() {
			return new CQuestData;
		}

		static auto GetNewStageData() {
			return new CStageData;
		}

		static auto GetNewRadiantData() {
			return new CRadiantData;
		}

		static auto GetNewDrunkData() {
			return new CDrunkData;
		}

		static auto GetNewPatchData() {
			return new CMiscPatch;
		}

		[[nodiscard]] static std::string CreateGUID() noexcept
		{
			GUID guid;
			if (CoCreateGuid(&guid) != S_OK) {
				return "";
			}

			char guidString[39]; // 38 characters + null terminator
			snprintf(guidString, sizeof(guidString),
				"{%08x-%04x-%04x-%04x-%012llx}",
				guid.Data1, guid.Data2, guid.Data3,
				(guid.Data4[0] << 8) | guid.Data4[1],
				*(reinterpret_cast<uint64_t*>(&guid.Data4[2])));

			return std::string(guidString);
		}

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static int GetIntValue(const char* section, const char* key) noexcept
		{
			auto val = ini.GetLongValue(section, key, -3);
			if (val == -3)
			{
				ERROR("Unable to retrieve int value for key: {} in section {} in file {}", key, section, filename);
			}
			return val;

		};

		[[nodiscard]] static int GetIntValueWithDefault(const char* section, const char* key) noexcept
		{
			return ini.GetLongValue(section, key, -1);
		};

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static bool GetBoolValue(const char* section, const char* key) noexcept
		{
			return ini.GetBoolValue(section, key, false);
		};

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static std::string GetStringValue(const char* section, const char* key) noexcept
		{
			auto val = ini.GetValue(section, key, "ERROR");
			if (DKUtil::string::iequals(val, "ERROR"))
			{
				ERROR("Unable to retrieve string value for key: {} in section {} in file {}", key, section, filename);
			}
			return fmt::format("{:s}", val);
		};

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static std::string GetStringValueWithDefault(const char* section, const char* key, const char* _default) noexcept
		{
			auto val = ini.GetValue(section, key, "ERROR");
			if (DKUtil::string::iequals(val, "ERROR"))
			{
				return fmt::format("{:s}", _default);
			}
			return fmt::format("{:s}", val);
		};

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static RE::FormID GetFormIDValue(const char* section, const char* key) noexcept
		{
			try
			{
				return static_cast<RE::FormID>(std::stoul(GetStringValue(section, key), nullptr, 16));
			}
			catch (const std::exception& e)
			{
				// Handle the conversion failure appropriately
				INFO("Failed to convert FormID: {}", e.what());
				return RE::FormID{};
			}
		}

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static RE::BGSListForm* GetFormlist(RE::FormID a_formID, const char* a_modname) noexcept
		{
			try
			{
				return RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSListForm>(a_formID, a_modname);
			}
			catch (const std::exception& e)
			{
				// Handle the conversion failure appropriately
				INFO("Failed to convert FormID: {}", e.what());
				return nullptr;
			}
		}

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		template <typename T = int32_t>
		[[nodiscard]] static T GetEnumValue(const char* section, const char* key) noexcept
		{
			return static_cast<T>(GetIntValue(section, key));
		}

		[[nodiscard]] static int GetPatchID() noexcept
		{
			return GetIntValue("Completionist Patch Data", "PatchID");
		};

		[[nodiscard]] static int GetCustomPatchType() noexcept
		{
			return GetIntValue("Completionist Patch Data", "axPlt");
		};

		[[nodiscard]] static std::string GetPatchType() noexcept
		{
			return GetStringValue("Completionist Patch Data", "PatchType");
		};

		[[nodiscard]] static bool IsGroupingEnabled() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "UseMCMPageGrouping");
		};

		[[nodiscard]] static int GetMultiPageCount() noexcept
		{
			return GetIntValue("Completionist Patch Data", "PageCount");
		};

		[[nodiscard]] static int GetQuestID(const char* section) noexcept
		{
			return GetIntValue(section, "QuestID");
		};

		[[nodiscard]] static std::string GetHeaderName() noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", "Header", "");
		};

		[[nodiscard]] static std::string GetRequiredMod(const char* section) noexcept
		{
			return GetStringValueWithDefault(section, "RequiresMod", "None");
		};

		[[nodiscard]] static std::string GetOverrideLocalisationFileName() noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", "OverrideLocalisationFileName", "None");
		};

		[[nodiscard]] static std::string GetSliderValue(int32_t a_page) noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", fmt::format("PageName{}", std::to_string(a_page)).c_str(), GET_LOC_STRING_BY_KEY("MCMText_Collectables"));
		};

		[[nodiscard]] static bool GetMultiPagePrependPageNumber() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "PrependPageNumber");
		};

		[[nodiscard]] static bool GetRequiredModValue(const char* section) noexcept
		{
			return GetBoolValue(section, "RequiresMod_Value");
		};

		[[nodiscard]] static bool HasCustomDropDownMenu() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "DropDownMenu");
		};

		[[nodiscard]] static std::string GetCustomDropDownMenuName() noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", "DropDownMenu_Name", "ERROR");
		};

		[[nodiscard]] static std::string GetCustomDropDownMenuHighlight() noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", "DropDownMenu_Highlight", "ERROR");
		};

		[[nodiscard]] static std::string GetCustomDropDownOptionName(int32_t option) noexcept
		{
			return GetStringValueWithDefault("Completionist Patch Data", fmt::format("DropDownMenu_Option_{}", option).c_str(), "ERROR");
		};

		[[nodiscard]] static std::string GetInstallFromMod(const char* section) noexcept
		{
			return GetStringValueWithDefault(section, "InstallFrom", "None");
		};

		// Should Completionist load this form?
		[[nodiscard]] static bool ShouldInstall(const char* section) noexcept
		{
			const std::string condition_string = GetInstallCondition(section);

			if (condition_string.empty() || DKUtil::string::iequals(condition_string, "None")) {
				return true;
			}

			const auto extractSubstring = [](const std::string& str) {
				auto parsed_string = trim(str.substr(str.find('<') + 2));
				return parsed_string;
				};

			const auto splitString = [](const std::string& str, char delimiter) {
				std::vector<std::string> tokens;
				std::string token;
				std::istringstream tokenStream(str);
				while (std::getline(tokenStream, token, delimiter)) {
					if (!token.empty()) {
						tokens.push_back(trim(token));
					}
				}
				return tokens;
				};

			const auto splitConditions = [](const std::string& str) {
				std::vector<std::string> conditions;
				std::string temp;

				for (size_t i = 0; i < str.size(); ++i) {
					if (str[i] == '<') {
						if (!temp.empty()) {
							conditions.push_back(temp);
							temp.clear();
						}
					}
					temp += str[i];
				}

				if (!temp.empty()) {
					conditions.push_back(temp);
				}

				return conditions;
				};

			// Split the condition string by commas to handle OR conditions
			const auto orConditions = splitString(condition_string, ',');

			for (const auto& orCondition : orConditions) {

				// Split each OR condition into individual AND conditions
				const auto andConditions = splitConditions(orCondition);

				bool andResult = true;

				for (const auto& condition : andConditions) {

					if (condition.find("<!") == 0) {
						if (Serialization::CompletionistData::IsModInstalled(extractSubstring(condition))) {
							andResult = false;
							break;  // If any AND condition fails, skip to the next OR condition
						}
					}
					else if (condition.find("<&") == 0) {
						if (!Serialization::CompletionistData::IsModInstalled(extractSubstring(condition))) {
							andResult = false;
							break;  // If any AND condition fails, skip to the next OR condition
						}
					}
					else {
						// Invalid format; return false immediately
						return false;
					}
				}

				// If all AND conditions within an OR condition group are true, return true
				if (andResult) {
					return true;
				}
			}

			// If none of the OR condition groups are true, return false
			return false;
		}

		// Should Completionist load this form?
		[[nodiscard]] static bool ShouldInstallForm(const char* section, const std::string& formID) noexcept
		{
			const std::string condition_string = GetCustomInstallCondition(section, formID);

			if (condition_string.empty() || DKUtil::string::iequals(condition_string, "None")) {
				return true;
			}

			const auto extractSubstring = [](const std::string& str) {
				auto parsed_string = trim(str.substr(str.find('<') + 2));
				return parsed_string;
				};

			const auto splitString = [](const std::string& str, char delimiter) {
				std::vector<std::string> tokens;
				std::string token;
				std::istringstream tokenStream(str);
				while (std::getline(tokenStream, token, delimiter)) {
					if (!token.empty()) {
						tokens.push_back(trim(token));
					}
				}
				return tokens;
				};

			const auto splitConditions = [](const std::string& str) {
				std::vector<std::string> conditions;
				std::string temp;

				for (size_t i = 0; i < str.size(); ++i) {
					if (str[i] == '<') {
						if (!temp.empty()) {
							conditions.push_back(temp);
							temp.clear();
						}
					}
					temp += str[i];
				}

				if (!temp.empty()) {
					conditions.push_back(temp);
				}

				return conditions;
				};

			// Split the condition string by commas to handle OR conditions
			const auto orConditions = splitString(condition_string, ',');

			for (const auto& orCondition : orConditions) {
				// Split each OR condition into individual AND conditions
				const auto andConditions = splitConditions(orCondition);

				bool andResult = true;

				for (const auto& condition : andConditions) {
					if (condition.find("<!") == 0) {
						if (Serialization::CompletionistData::IsModInstalled(extractSubstring(condition))) {
							andResult = false;
							break;  // If any AND condition fails, skip to the next OR condition
						}
					}
					else if (condition.find("<&") == 0) {
						if (!Serialization::CompletionistData::IsModInstalled(extractSubstring(condition))) {
							andResult = false;
							break;  // If any AND condition fails, skip to the next OR condition
						}
					}
					else {
						// Invalid format; return false immediately
						return false;
					}
				}

				// If all AND conditions within an OR condition group are true, return true
				if (andResult) {
					return true;
				}
			}

			// If none of the OR condition groups are true, return false
			return false;
		}

		[[nodiscard]] static bool GetIsQuestSection(const char* section) noexcept
		{
			std::string val = GetStringValueWithDefault(section, "EditorID", "None");
			return val != "None";
		};

		[[nodiscard]] static std::string GetInstallCondition(const char* section) noexcept
		{
			return GetStringValueWithDefault(section, "InstallCondition", "None");
		};

		[[nodiscard]] static int GetMultiPageValue(const char* section) noexcept
		{
			return GetIntValue(section, "DisplayOnPage");
		};

		[[nodiscard]] static int GetPriority(const char* section) noexcept
		{
			return GetIntValueWithDefault(section, "Priority");
		};

		[[nodiscard]] static int GetPriority() noexcept
		{
			return GetIntValueWithDefault("Completionist Patch Data", "Priority");
		};

		[[nodiscard]] static bool GetIsVanillaTracking() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "IsVanillaTracking");
		};
		
		[[nodiscard]] static bool GetEnabled(const char* section) noexcept
		{
			return GetBoolValue(section, "Enabled");
		};

		[[nodiscard]] static bool GetEnabled() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "Enabled");
		};

		[[nodiscard]] static std::string GetMCMPageName(const char* section) noexcept
		{
			return GetStringValue(section, "MCMPageName");
		};

		[[nodiscard]] static std::vector<std::tuple<RE::FormID, std::string, std::string>> GetFormIDArray(const char* section, bool log_install) noexcept
		{
			std::stringstream ss(GetStringValue(section, "FormIDs"));
			std::vector<std::tuple<RE::FormID, std::string, std::string>> formIDs{};
			std::string str;
			RE::FormID formID{};

			auto fromMod = GetInstallFromMod(section);
			auto pluginFileName = (DKUtil::string::iequals(fromMod, "None") || DKUtil::string::iequals(fromMod, "")) ? GetPluginFileName() : fromMod;

			while (getline(ss, str, ','))
			{
				if (!str.contains("0x"))
				{
					continue;
				};

				str = str.substr(str.find("0x"));

				if (str.ends_with(",")) {
					str.erase(str.end());
				};

				if (!str.contains("*")) {

					if (str.contains("<"))
					{
						std::string fName = trim(str.substr(str.find("<") + 2));

						auto shouldContinue = str.contains("!") == Serialization::CompletionistData::IsModInstalled(fName);
						if (shouldContinue)
						{
							if (log_install) {
								INFO("Unable to load useable data from conditional entry: {} in section {}: [{} failed match condition.]", str, section, fName);
							}
							continue;
						};

						str.erase(str.find("<"));
					};
					formIDs.push_back(std::make_tuple(static_cast<RE::FormID>(std::stoul(trim(str), nullptr, 16)), trim(str), pluginFileName));
				}
				else
				{
					if (str.contains("<"))
					{
						std::string fName = trim(str.substr(str.find("<") + 2));

						auto shouldContinue = str.contains("!") == Serialization::CompletionistData::IsModInstalled(fName);
						if (shouldContinue)
						{
							if (log_install) {
								INFO("Unable to load useable data from conditional entry: {} in section {}: [{} failed match condition.]", str, section, fName);
							}
							continue;
						};

						str.erase(str.find("<"));
					};

					std::string delimiter = "*";
					std::string rawformID = trim(str.substr(0, str.find(delimiter)));
					RE::FormID formID = static_cast<RE::FormID>(std::stoul(rawformID, nullptr, 16));
					
					str.erase(0, str.find(delimiter) + delimiter.length());
					formIDs.push_back(std::make_tuple(formID, rawformID, str));
				}
			};
			return formIDs;
		};

		[[nodiscard]] static int GetQuestVisibilityFlag(const char* section) noexcept
		{
			const auto& text = GetStringValueWithDefault(section, "Visibility", "Error");
			if (DKUtil::string::iequals(text, "Error")) {
				return -1;
			};
			return std::stoi(text);
		};

		[[nodiscard]] static std::string GetCustomDisplayName(const char* section, std::string formIDKey) noexcept
		{
			const auto& text = GetStringValueWithDefault(section, fmt::format("{}_DisplayName", formIDKey.c_str()).c_str(), "Error");
			if (DKUtil::string::iequals(text, "Error")) {
				return std::string{};
			};
			return text;
		};

		[[nodiscard]] static int GetCustomVisibilityFlag(const char* section, std::string formIDKey) noexcept
		{
			const auto& text = GetStringValueWithDefault(section, fmt::format("{}_Visibility", formIDKey.c_str()).c_str(), "Error");
			if (DKUtil::string::iequals(text, "Error")) {
				return -1;
			};
			return std::stoi(text);
		};

		[[nodiscard]] static std::string GetCustomHighlightText(const char* section, const std::string &formIDKey) noexcept
		{
			const auto& text = GetStringValueWithDefault(section, fmt::format("{}_Highlight", formIDKey.c_str()).c_str(), "Error");
			if (DKUtil::string::iequals(text, "Error")) {
				return std::string{};
			};

			return text;
		};

		[[nodiscard]] static std::string GetCustomInstallCondition(const char* section, const std::string& formIDKey) noexcept
		{
			const auto &text = GetStringValueWithDefault(section, fmt::format("{}_InstallCondition", formIDKey.c_str()).c_str(), "Error");
			if (DKUtil::string::iequals(text, "Error")) {
				return std::string{};
			};

			return text;
		};

		[[nodiscard]] static std::vector<std::tuple<RE::FormID, std::string, std::string>> GetVariationsArray(const char* section, const std::string &formIDKey, bool log_install) noexcept
		{
			std::vector<std::tuple<RE::FormID, std::string, std::string>> variations{};

			auto variationString = GetStringValueWithDefault(section, formIDKey.c_str(), "Error");
			if (DKUtil::string::iequals(variationString, "Error")) {
				return variations;
			};

			std::stringstream ss(variationString);
			std::string str;

			auto fromMod = GetInstallFromMod(section);
			auto pluginFileName = (DKUtil::string::iequals(fromMod, "None") || DKUtil::string::iequals(fromMod, "")) ? GetPluginFileName() : fromMod;

			while (getline(ss, str, ','))
			{
				if (!str.contains("0x"))
				{
					continue;
				};

				str = str.substr(str.find("0x"));

				if (str.ends_with(",")) {
					str.erase(str.end());
				};

				if (!ShouldInstallForm(section, formIDKey)) {
					continue;
				}

				if (!str.contains("*")) {

					if (str.contains("<"))
					{
						std::string fName = trim(str.substr(str.find("<") + 2));

						auto shouldContinue = str.contains("!") == Serialization::CompletionistData::IsModInstalled(fName);
						if (shouldContinue)
						{
							if (log_install) {
								INFO("Unable to load useable data from conditional entry: {} in section {}: [{} failed match condition.]", str, section, fName);
							}
							continue;
						};

						str.erase(str.find("<"));
					};

					variations.push_back(std::make_tuple(static_cast<RE::FormID>(std::stoul(trim(str), nullptr, 16)), trim(str), pluginFileName));
				}
				else
				{
					if (str.contains("<"))
					{
						std::string fName = trim(str.substr(str.find("<") + 2));

						auto shouldContinue = str.contains("!") == Serialization::CompletionistData::IsModInstalled(fName);
						if (shouldContinue)
						{
							if (log_install) {
								INFO("Unable to load useable data from conditional entry: {} in section {}: [{} failed match condition.]", str, section, fName);
							}
							continue;
						};

						str.erase(str.find("<"));
					};

					std::string delimiter = "*";
					std::string rawformID = trim(str.substr(0, str.find(delimiter)));
					RE::FormID formID = static_cast<RE::FormID>(std::stoul(rawformID, nullptr, 16));

					str.erase(0, str.find(delimiter) + delimiter.length());
					variations.push_back(std::make_tuple(formID, rawformID, str));
				}
			};
			return variations;
		};

		static void CompileVariations(std::string sec, Serialization::CompletionistData* data, std::vector<std::tuple<RE::FormID, std::string, std::string>> variations, const std::string &base_name, RE::FormID a_baseID, bool log_install)
		{
			auto Idx = 0;
			for (auto& [formID, raw, pluginFileName] : variations) 
			{
				if (!data->IsModInstalled(pluginFileName)) {
					if (log_install) {
						INFO("Unable to load useable data from entry variation: {} in section {}: [{} not found.]", Idx, sec, pluginFileName);
					}
					Idx++;
					continue;
				};

				auto* form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, pluginFileName);
				if (!form)
				{
					if (log_install) {
						INFO("Unable to load useable data from entry variation: {} in section {}: [{}]", Idx, sec, "Incorrect FormID or Form not found.");
					}
					Idx++;
					continue;
				};

				if (log_install)
				{
					INFO("Adding V Support for: {} - {}", raw, form->GetName());
				};
				data->AddForm(a_baseID, base_name, formID, pluginFileName);
				Idx++;
			};
		};

		[[nodiscard]] static std::string GetMCMPageName() noexcept
		{
			return GetStringValue("Completionist Patch Data", "MCMPageName");
		};

		[[nodiscard]] static bool IsPageLocalisationNameValid(int32_t index) noexcept
		{
			auto pageName = fmt::format("PageName{}", index);

			return GetStringValueWithDefault("Completionist Patch Data", pageName.c_str(), "Error") != "Error";
		};

		[[nodiscard]] static std::string GetUUID(const char* section) noexcept
		{
			return GetStringValue(section, "UUID");
		};

		[[nodiscard]] static std::string GetPluginFileName(const char* section) noexcept
		{
			return GetStringValue(section, "PluginFileName");
		};

		[[nodiscard]] static std::string GetPluginFileName() noexcept
		{
			return GetStringValue("Completionist Patch Data", "PluginFileName");
		};

		[[nodiscard]] static std::string GetModName(const char* section) noexcept
		{
			return GetStringValue(section, "ModName");
		};

		[[nodiscard]] static std::string GetEditorID(const char* section) noexcept
		{
			return GetStringValue(section, "EditorID");
		};

		[[nodiscard]] static std::string GetQuestName(const char* section) noexcept
		{
			return GetStringValue(section, "QuestName");
		};

		[[nodiscard]] static bool GetLoggingAllowed() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "Log");
		};

		[[nodiscard]] static std::string GetSectionHeaderL(const char* section) noexcept
		{
			return GetStringValue(section, "SectionHeader_L");
		};

		[[nodiscard]] static std::string GetSectionHeaderR(const char* section) noexcept
		{
			return GetStringValue(section, "SectionHeader_R");
		};

		[[nodiscard]] static std::string GetHighlightText(const char* section) noexcept
		{
			return GetStringValue(section, "HighlightText");
		};

		[[nodiscard]] static CFlagEnum GetQuestType(const char* section) noexcept
		{
			return GetEnumValue<CFlagEnum>(section, "QuestType");
		};

		[[nodiscard]] static CCompEnum GetCompletionType(const char* section) noexcept
		{
			return GetEnumValue<CCompEnum>(section, "CompletionType");
		};

		[[nodiscard]] static CStageEnum GetStageType(const char* section) noexcept
		{
			return GetEnumValue<CStageEnum>(section, "StageType");
		};

		[[nodiscard]] static int GetStage(const char* section) noexcept
		{
			return GetIntValue(section, "Stage");
		};

		[[nodiscard]] static int GetOptionalStage(const char* section) noexcept
		{
			return GetIntValue(section, "OptionalStage");
		};

		[[nodiscard]] static CRadiantEnum GetTimesRequired(const char* section) noexcept
		{
			auto tr = GetIntValue(section, "TimesRequired");
			return static_cast<CRadiantEnum>(tr);
		};

		[[nodiscard]] static RE::FormID GetBaseFormID(const char* section) noexcept
		{
			return GetFormIDValue(section, "BaseFormID");
		};

		[[nodiscard]] static RE::FormID GetDrunkFileList(const char* section) noexcept
		{
			return GetFormIDValue(section, "validItemsFormID");
		};

		[[nodiscard]] static std::string GetDrunkFileListPluginName(const char* section) noexcept
		{
			return GetStringValue(section, "validItemsFileName");
		};

		[[nodiscard]] static int GetRadiantStage(const char* section) noexcept
		{
			return GetIntValueWithDefault(section, "RadiantStage");
		};

		[[nodiscard]] static bool IsFavorQuest(const char* section) noexcept
		{
			return GetBoolValue(section, "isGenericFavorQuest");
		};

		[[nodiscard]] static int GetFavorQuestType(const char* section) noexcept
		{
			return GetIntValueWithDefault(section, "GenericFavorQuestType");
		};

		[[nodiscard]] static int GetQuestLink(const char* section) noexcept
		{
			return GetIntValueWithDefault(section, "QuestLink");
		};
		
		[[nodiscard]] static CMiscPatchType GetItemType(const char* section) noexcept
		{
			return GetEnumValue<CMiscPatchType>(section, "Type");
		};

		//Pets Support
		[[nodiscard]] static int GetPetCompletionType(const char* section, const std::string& rawFormID) noexcept
		{
			return GetIntValue(section, fmt::format("{}_Type", rawFormID).c_str());
		};

		[[nodiscard]] static int GetPetsCompletionStage(const char* section, const std::string& rawFormID) noexcept
		{
			return GetIntValue(section, fmt::format("{}_Stage", rawFormID).c_str());
		};

		//Player Homes Support
		[[nodiscard]] static int GetPlayerHomeType(const char* section, const std::string& rawFormID) noexcept
		{
			return GetIntValue(section, fmt::format("{}_Type", rawFormID).c_str());
		};

		[[nodiscard]] static std::string GetPlayerHomeEditorID(const char* section, const std::string& rawFormID) noexcept
		{
			return GetStringValueWithDefault(section, fmt::format("{}_EditorID", rawFormID).c_str(), "ERROR");
		};

		[[nodiscard]] static int GetPlayerHomeStage(const char* section, const std::string& rawFormID) noexcept
		{
			return GetIntValue(section, fmt::format("{}_Stage", rawFormID).c_str());
		};

		[[nodiscard]] static int GetPlayerHomeOptionalStage(const char* section, const std::string& rawFormID) noexcept
		{
			return GetIntValue(section, fmt::format("{}_OptionalStage", rawFormID).c_str());
		};

		//Fishing Support
		[[nodiscard]] static bool CanPickUpFish(const char* section, const std::string& rawFormID) noexcept
		{
			return GetBoolValue(section, fmt::format("{}_PickupEnabled", rawFormID).c_str());
		};

		[[nodiscard]] static std::string trim(const std::string& s) noexcept {
			std::string str = s;
			rtrim(str);
			ltrim(str);
			return str;
		}

		[[nodiscard]] static void ltrim(std::string& s) noexcept {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
				}));
		}

		[[nodiscard]] static void rtrim(std::string& s) noexcept {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
				}).base(), s.end());
		}

		static void BuildLocalisedMap(const char* a_name, std::unordered_map<std::string, std::string>& translations)
		{
			translations.clear(); 

			const std::string &cBasePath = R"(.\Data\SKSE\Plugins\CompletionistData\Translations\)";
			if (!std::filesystem::exists(cBasePath) || std::filesystem::is_empty(cBasePath))
			{
				INFO("Translations Folder Is Empty Or Does Not Exist.");
				return;
			}

			const std::string &cFilePath = fmt::format(R"(.\Data\SKSE\Plugins\CompletionistData\Translations\{})"sv, a_name);
			if (!std::filesystem::exists(cFilePath) || std::filesystem::is_empty(cFilePath))
			{
				INFO("Translations Folder deos not contain {}", a_name);
				return;
			}

			std::ifstream localised_file(cFilePath);
			std::string tempstring;

			if (!localised_file.is_open()) {
				ERROR("Failed to open file: {}", a_name);
				return;
			}

			while (getline(localised_file, tempstring)) {
				if (!tempstring.empty() && !tempstring.starts_with("-")) {
					size_t tabPos = tempstring.find("	");
					size_t pipePos = tempstring.find("|");

					if (tabPos != std::string::npos && pipePos != std::string::npos) {
						translations.emplace(tempstring.substr(0, tabPos), tempstring.substr(pipePos + 1));
					}
				}
			}
			localised_file.close();
		};

		[[nodiscard]] static std::string GetLocStringByKey(const char* s_key, const std::unordered_map<std::string, std::string>& translations) noexcept
		{
			return translations.contains(s_key) ? fmt::format("{:s}", translations.at(s_key)) : s_key;
		};

		static void LinkPatchAndQuestData() 
		{
			for (auto& [page, patch] : CustomPatches)
			{
				for (auto& section : patch->type_sections)
				{
					if (section.type == CMiscPatchType::kQuests)
					{
						section.quest_data_array = GetQuestData(section.quest_data_id);
						section.enabled = true;
					};
				};
			};
		};

		static std::vector<CQuestData*> GetQuestData(int a_ID)
		{
			std::vector<CQuestData*> list{};

			for (auto& [data, name, ID, key] : CQuestMaster::CQuestDataVec)
			{
				if (ID == a_ID && data->GetQuest()) {
					list.push_back(data);
				};
			};
			return list;
		};
	};
};