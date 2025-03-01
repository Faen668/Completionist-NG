#pragma once
#include "SimpleIni.h"
#include "Serialization.hpp"

namespace CompletionistExclusions
{
	static CSimpleIniA ini;
	

	class ExclusionHandler
	{
	public:
		ExclusionHandler(ExclusionHandler const&) = delete;
		ExclusionHandler(ExclusionHandler const&&) = delete;
		ExclusionHandler operator=(ExclusionHandler&) = delete;
		ExclusionHandler operator=(ExclusionHandler&&) = delete;

		static void init();
		static const std::vector<RE::FormID>& GetExcludedCellsVector() { return ExcludedCells; }
		static const std::vector<RE::FormID>& GetExcludedContainersVector() { return ExcludedReferences; }

	private:
		ExclusionHandler() = default;
		~ExclusionHandler() = default;

		static std::vector<RE::FormID> ExcludedCells;
		static std::vector<RE::FormID> ExcludedReferences;

		static void LoadiniFiles(const std::filesystem::path& path, std::vector<RE::FormID>& vector);
		static void ProcessiniFile(std::string& file, std::vector<RE::FormID>& vector);

		static std::string GetStringValue(const char* section, const char* key);

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

		[[nodiscard]] static std::vector<std::tuple<RE::FormID, std::string, std::string>> GetFormIDArray(const char* section) noexcept
		{
			std::stringstream ss(GetStringValue(section, "FormIDs"));
			std::vector<std::tuple<RE::FormID, std::string, std::string>> formIDs{};
			std::string str;
			RE::FormID formID{};

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

				std::string delimiter = "*";
				std::string rawformID = trim(str.substr(0, str.find(delimiter)));
				RE::FormID formID = static_cast<RE::FormID>(std::stoul(rawformID, nullptr, 16));

				str.erase(0, str.find(delimiter) + delimiter.length());
				formIDs.push_back(std::make_tuple(formID, rawformID, str));
			}
			return formIDs;
		};
	};
}
