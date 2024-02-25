#include "DKUtil/Utility.hpp"
#pragma once

namespace Serialization
{
	struct KillData 
	{
		int32_t killCount;
		int32_t assistCount;
		std::string highlight;

		KillData(int32_t kills = 0, int32_t assists = 0, std::string_view h = "")
			: killCount(kills), assistCount(assists), highlight(h) {}
	};

	struct CompletionistDeathSentance final : public ISerializable
	{
		void AddKillOrAssist(const std::string& a_name, const std::string& a_highlight, bool isKill)
		{
			if (a_name.empty())
			{
				return;
			}

			std::string highlightString;

			if (!a_highlight.empty())
			{
				highlightString = a_highlight;

				size_t pos = highlightString.find(':');
				if (pos != std::string::npos)
				{
					highlightString.erase(pos, 1);
				}
			}

			auto it = data.find(a_name);
			if (it != data.end())
			{
				KillData& killData = it->second;
				isKill ? ++killData.killCount : ++killData.assistCount;
				killData.highlight = highlightString;
			}
			else
			{
				data.emplace(a_name, KillData(isKill ? 1 : 0, isKill ? 0 : 1, highlightString));
			}
		}

		void ResetCount(const std::string& a_name)
		{
			auto it = data.find(a_name);
			if (it != data.end())
			{
				KillData& killData = it->second;
				killData.killCount = 0;
				killData.assistCount = 0;
				killData.highlight = "";
			}
		}

		void RemoveEnemy(const std::string& a_name)
		{
			auto it = data.find(a_name);
			if (it != data.end())
			{
				data.erase(it);
			}
		}

		[[nodiscard]] std::string GetRaceName(const RE::Actor* a_actor) const noexcept {
			static const std::unordered_map<std::string, std::string> raceNameMap = {
				{"AlduinRace", "Dragon:"},
				{"ccBGSSSE036_BoneWolfCompanionRace", "Bone Wolf:"},
				{"ccBGSSSE040_GoblinRace", "Goblin:"},
				{"CowRace", "Cow:"},
				{"DefaultRace", ""},
				{"DLC1ChaurusHunterRace", "Chaurus Hunter:"},
				{"DLC1DeathHoundRace", "Death Hound:"},
				{"DLC1UndeadDragonRaceDefaultRace", "Dragon:"},
				{"DLC2BoarRace", "Boar:"},
				{"DLC2MountedRieklingRace", "Riekling:"},
				{"DLC2RieklingRace", "Riekling:"},
				{"DLC2SeekerRace", "Seeker:"},
				{"DragonRaceDefaultRace", "Dragon:"},
				{"ElderRace", "Elder:"},
				{"ElderRaceVampire", "Elder:"},
				{"InvisibleRace", ""},
				{"UndeadDragonRaceDefaultRace", "Dragon:"}
			};

			const auto& race = a_actor->GetRace();
			if (!race) {
				return "";
			}

			const auto& editorID = race->GetFormEditorID();
			const auto& iter = raceNameMap.find(editorID);

			return (iter != raceNameMap.end()) ? iter->second : fmt::format("{}:", race->GetFullName());
		}

		[[nodiscard]] std::vector<std::string> GetKills()  noexcept
		{
			std::vector<std::string> list{};

			for (auto& [name, killData] : data)
			{
				list.push_back(name);
			};

			std::sort(list.begin(), list.end());
			return list;
		};

		[[nodiscard]] std::string GetCombinedKillString(const std::string& a_name) noexcept 
		{
			auto it = data.find(a_name);
			if (it != data.end()) {
				if (it->second.assistCount > 0) {
					return fmt::format("{}/{}", it->second.killCount, it->second.assistCount);
				}
				else {
					return std::to_string(it->second.killCount);
				}
			}
			return "0";
		}

		[[nodiscard]] int32_t GetKillCountFor(const std::string& a_name) noexcept
		{
			auto it = data.find(a_name);
			return (it != data.end()) ? it->second.killCount : 0;
		}

		[[nodiscard]] int32_t GetAssistCountFor(const std::string& a_name) noexcept
		{
			auto it = data.find(a_name);
			return (it != data.end()) ? it->second.assistCount : 0;
		}

		[[nodiscard]] std::string GetDeathStringFor(const std::string& a_name) noexcept
		{
			auto it = data.find(a_name);
			return (it != data.end()) ? it->second.highlight : "";
		}

		//---------------------------------------------------
		//-- Completionist Serialization ( SKSE APIs ) ------
		//---------------------------------------------------

		virtual void Save(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			std::size_t total = data.size();
			if (!a_intfc->WriteRecordData(&total, sizeof(total))) {
				ERROR("Failed to write serialized form data: size");
				return;
			}

			for (const auto& [name, killData] : data) {
				std::size_t nameSize = name.size();
				std::size_t highSize = killData.highlight.size();

				if (!a_intfc->WriteRecordData(&nameSize, sizeof(nameSize)) ||
					!a_intfc->WriteRecordData(name.data(), nameSize) ||
					!a_intfc->WriteRecordData(&highSize, sizeof(highSize)) ||
					!a_intfc->WriteRecordData(killData.highlight.data(), highSize) ||
					!a_intfc->WriteRecordData(&killData.killCount, sizeof(killData.killCount)) ||
					!a_intfc->WriteRecordData(&killData.assistCount, sizeof(killData.assistCount)))
				{
					ERROR("Failed to write serialized form data: {}", name);
					return;
				}
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: size");
				return;
			}

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				std::string name;
				std::string high;
				int32_t kills;
				int32_t assists;

				std::size_t nameSize;
				std::size_t highSize;
				if (!a_intfc->ReadRecordData(&nameSize, sizeof(nameSize))) {
					ERROR("Failed to read serialized form data: kill name size");
					return;
				}

				name.resize(nameSize);
				if (!a_intfc->ReadRecordData(name.data(), nameSize)) {
					ERROR("Failed to read serialized form data: kill name");
					return;
				}

				if (!a_intfc->ReadRecordData(&highSize, sizeof(highSize))) {
					ERROR("Failed to read serialized form data: death sentence size");
					return;
				}

				high.resize(highSize);
				if (highSize && !a_intfc->ReadRecordData(high.data(), highSize)) {
					ERROR("Failed to read serialized form data: death sentence");
					return;
				}

				if (!a_intfc->ReadRecordData(&kills, sizeof(kills))) {
					ERROR("Failed to read serialized form data: kills");
					return;
				}

				if (!a_intfc->ReadRecordData(&assists, sizeof(assists))) {
					ERROR("Failed to read serialized form data: assists");
					return;
				}

				data.emplace(name, KillData(kills, assists, high));
				read++;
			}

			if (read != total) {
				INFO("Lost data while loading {} from co-save... Expected: {} Written: {}", a_name, total, read);
			}

			INFO("Loaded SKSE co-save {} with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		std::unordered_map<std::string, KillData> data;
	};
};