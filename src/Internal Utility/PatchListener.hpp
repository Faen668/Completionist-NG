#pragma once
#include "Serialization.hpp"
#include "MiscPatchStruct.hpp"

namespace CExternalPatchHandler
{
	static std::vector<std::string> ini_variable_cache{};
	static std::vector<std::tuple<std::string, std::string, uint32_t>> mainPagedefs{};
	static std::vector<std::tuple<std::string, std::string, uint32_t, uint32_t, uint32_t, bool, bool, bool>> miscPagedefs{};
	static std::vector<int32_t> usedIdentifiers{};
	static CSimpleIniA ini;

	inline std::vector<CMiscPatch*> CustomItemsPatches{};
	inline std::vector<CMiscPatch*> Get() { return CustomItemsPatches; }

	class CHandler {

	public:
		static void Register();
		static int32_t GetRandomID();

		static void AddQuestSupport(std::string file);
		static void AddMiscSupport(std::string file);

		static auto GetNewQuestData() {
			return new CQuestData;
		}

		static auto GetNewStageData() {
			return new CStageData;
		}

		static auto GetNewRadiantData() {
			return new CRadiantData;
		}

		static auto GetNewitemsData() {
			return new CMiscPatch;
		}

		[[nodiscard]] static bool IsQuestPageRegistered(std::string a_page) noexcept {
			for (auto& [page, mod, id] : mainPagedefs) {

				if (strcmp(page.c_str(), a_page.c_str()) == 0) {
					return true;
				}
			}
			return false;
		}

		[[nodiscard]] static bool IsMiscPageRegistered(std::string a_page) noexcept {
			for (auto& [page, mod, ItemsID, BooksID, MapMaID, itemsReq, booksReq, mapmaReq] : miscPagedefs) {

				if (strcmp(page.c_str(), a_page.c_str()) == 0) {
					return true;
				}
			}
			return false;
		}

		//---------------------------------------------------
		//-- INI Parser Wrappers-----------------------------
		//---------------------------------------------------

		[[nodiscard]] static int GetIntValue(const char* section, const char* key) noexcept
		{
			auto val = ini.GetLongValue(section, key, -3);
			if (val == -3)
			{
				ERROR("Unable to retrieve int value for key: {} in section {}", key, section);
			}
			return val;

		};

		[[nodiscard]] static bool GetBoolValue(const char* section, const char* key) noexcept
		{
			return ini.GetBoolValue(section, key, false);
		};

		[[nodiscard]] static std::string GetStringValue(const char* section, const char* key) noexcept
		{
			auto val = ini.GetValue(section, key, "ERROR");
			if (DKUtil::string::iequals(val, "ERROR"))
			{
				ERROR("Unable to retrieve string value for key: {} in section {}", key, section);
			}
			return ini_variable_cache.emplace_back(fmt::format("{:s}"sv, val)).c_str();
		};

		[[nodiscard]] static RE::FormID GetFormIDValue(const char* section, const char* key) noexcept
		{
			return static_cast<RE::FormID>(std::stoul(GetStringValue(section, key), nullptr, 16));
		};

		template <typename T = int32_t>
		[[nodiscard]] static T GetEnumValue(const char* section, const char* key) noexcept
		{
			return static_cast<T>(GetIntValue(section, key));
		}

		//---------------------------------------------------
		//-- INI Value Setters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static void SetMainPatchID(int UUID, std::string file) noexcept
		{
			auto rc = ini.SetValue("Completionist Patch Data", "PatchID", std::to_string(UUID).c_str());
			if (rc < 0)
			{
				ERROR("Unable to set patchID value for file - {}", file);
			}

			rc = ini.SaveFile(file.c_str());
			if (rc < 0)
			{
				ERROR("Unable to save file - {}", file);
			}
		};

		[[nodiscard]] static void SetMiscPatchIDs(int UUID1, int UUID2, int UUID3, std::string file) noexcept
		{
			auto rc = ini.SetValue("Completionist Patch Data", "ItemsPatchID", std::to_string(UUID1).c_str());
			if (rc < 0)
			{
				ERROR("Unable to set ItemsPatchID value for file - {}", file);
			}

			rc = ini.SetValue("Completionist Patch Data", "BooksPatchID", std::to_string(UUID2).c_str());
			if (rc < 0)
			{
				ERROR("Unable to set BooksPatchID value for file - {}", file);
			}

			rc = ini.SetValue("Completionist Patch Data", "MapMaPatchID", std::to_string(UUID3).c_str());
			if (rc < 0)
			{
				ERROR("Unable to set MapMaPatchID value for file - {}", file);
			}

			rc = ini.SaveFile(file.c_str());
			if (rc < 0)
			{
				ERROR("Unable to save file - {}", file);
			}
		};

		//---------------------------------------------------
		//-- INI Value Getters ------------------------------
		//---------------------------------------------------

		[[nodiscard]] static int GetPatchID() noexcept
		{
			return GetIntValue("Completionist Patch Data", "PatchID");
		};

		[[nodiscard]] static std::string GetPatchType() noexcept
		{
			return GetStringValue("Completionist Patch Data", "PatchType");
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

		[[nodiscard]] static std::string GetMCMPageName() noexcept
		{
			return GetStringValue("Completionist Patch Data", "MCMPageName");
		};

		[[nodiscard]] static int GetPatchIDOverride(const char* section) noexcept
		{
			return GetIntValue(section, "PatchIDOverride");
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

		[[nodiscard]] static bool GetAppendModName(const char* section) noexcept
		{
			return GetBoolValue(section, "AppendModName");
		};

		[[nodiscard]] static std::string GetHighlightText(const char* section) noexcept
		{
			return GetAppendModName(section)
				? fmt::format("{:s}{:s}{:s}"sv,
					GetStringValue(section, "HighlightText"),
					"\nThis Quest is provided by: ",
					GetModName(section))
				: GetStringValue(section, "HighlightText");
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
			return static_cast<CRadiantEnum>(tr > 9 ? 9 : tr);
		};

		[[nodiscard]] static RE::FormID GetBaseFormID(const char* section) noexcept
		{
			return GetFormIDValue(section, "BaseFormID");
		};

		[[nodiscard]] static int GetRadiantStage(const char* section) noexcept
		{
			return GetIntValue(section, "RadiantStage");
		};

		[[nodiscard]] static bool GetRequiresCustomMCMPage(const char* section) noexcept
		{
			return GetBoolValue(section, "RequiresCustomMCMPage");
		};

		[[nodiscard]] static bool GetRequiresCustomMCMPage() noexcept
		{
			return GetBoolValue("Completionist Patch Data", "RequiresCustomMCMPage");
		};

		[[nodiscard]] static RE::FormID GetItemFormID(const char* section) noexcept
		{
			return GetFormIDValue(section, "FormID");
		};

		[[nodiscard]] static int GetItemType(const char* section) noexcept
		{
			return GetIntValue(section, "Type");
		};

		[[nodiscard]] static int GetItemsPatchID() noexcept
		{
			return GetIntValue("Completionist Patch Data", "ItemsPatchID");
		};

		[[nodiscard]] static int GetBooksPatchID() noexcept
		{
			return GetIntValue("Completionist Patch Data", "BooksPatchID");
		};

		[[nodiscard]] static int GetMapMaPatchID() noexcept
		{
			return GetIntValue("Completionist Patch Data", "MapMaPatchID");
		};
	};
};