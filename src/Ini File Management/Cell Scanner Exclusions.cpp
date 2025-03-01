#include "Cell Scanner Exclusions.hpp"
#include "Serialization.hpp"
#include "SimpleIni.h"

static std::string filename;

namespace CompletionistExclusions
{
	std::vector<RE::FormID> ExclusionHandler::ExcludedCells;
	std::vector<RE::FormID> ExclusionHandler::ExcludedReferences;

	//---------------------------------------------------
	//-- Function ---------------------------------------
	//---------------------------------------------------

	void ExclusionHandler::init()
	{
		const std::filesystem::path cellPath = L"Data/SKSE/Plugins/CompletionistData/Exclusions/Cell Scanner Exclusions/Cells/";
		const std::filesystem::path contPath = L"Data/SKSE/Plugins/CompletionistData/Exclusions/Cell Scanner Exclusions/References/";

		LoadiniFiles(cellPath, ExcludedCells);
		LoadiniFiles(contPath, ExcludedReferences);
	}

	//---------------------------------------------------
	//-- Function ---------------------------------------
	//---------------------------------------------------

	void ExclusionHandler::LoadiniFiles(const std::filesystem::path& path, std::vector<RE::FormID>& vector)
	{
		std::vector<std::string> files;
		using rdi = std::filesystem::recursive_directory_iterator;

		if (!std::filesystem::exists(path) || std::filesystem::is_empty(path))
		{
			INFO("Exclusion Folder Is Empty Or Does Not Exist.");
			return;
		}

		for (const auto& dirEntry : rdi(path))
		{
			auto file = dirEntry.path().string();

			if (!DKUtil::string::icontains(file, ".ini")) {
				continue;
			}

			files.push_back(file);
		}

		if (files.size() == 0)
		{
			return;
		}

		for (auto& file : files)
		{
			filename = file.substr(file.find_last_of("/") + 1);

			SI_Error rc{};

			ini.Reset();
			ini.SetUnicode();
			ini.SetMultiLine(true);
			rc = ini.LoadFile(file.c_str());

			if (rc < 0)
			{
				INFO("Unable to load useable data from file: {} : [{}]", filename, "File Corrupt Or Wrong Format");
				continue;
			}
			ProcessiniFile(file, vector);
		}
	}

	//---------------------------------------------------
	//-- Function ---------------------------------------
	//---------------------------------------------------

	void ExclusionHandler::ProcessiniFile(std::string& file, std::vector<RE::FormID>& vector)
	{
		CSimpleIniA::TNamesDepend sections;
		ini.GetAllSections(sections);
		for (auto& section : sections)
		{
			auto sec = section.pItem;

			auto Idx = 0;
			for (auto& [formID, raw, pluginFileName] : GetFormIDArray(sec))
			{
				if (!Serialization::CompletionistData::IsModInstalled(pluginFileName))
				{
					//INFO("Unable to exclude {} from {} - [mod not found.]", Serialization::CompletionistData::GetFormIDHexString(formID), pluginFileName);
					Idx++;
					continue;
				};

				INFO("Excluded {} from {}", Serialization::CompletionistData::GetFormIDHexString(formID), pluginFileName);
				vector.push_back(formID);
			}
		}
	}

	//---------------------------------------------------
	//-- Function ---------------------------------------
	//---------------------------------------------------

	std::string ExclusionHandler::GetStringValue(const char* section, const char* key)
	{
		auto val = ini.GetValue(section, key, "ERROR");
		if (DKUtil::string::iequals(val, "ERROR"))
		{
			ERROR("Unable to retrieve string value for key: {} in section {} in file {}", key, section, filename);
		}
		return fmt::format("{:s}", val);
	}
};