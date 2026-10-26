#include "DKUtil/Utility.hpp"
#pragma once

namespace Serialization
{
	// The dropdown choice on each patch page, e.g. which faction's quests to show
	struct CompletionistPatchSettings final : public ISerializable
	{
		void SetCurrentUserOption(const std::string& a_page, int32_t a_value) noexcept
		{
			data[a_page] = a_value;
		}

		[[nodiscard]] int32_t GetCurrentUserOption(const std::string& a_page) const noexcept
		{
			const auto it = data.find(a_page);
			return it != data.end() ? it->second : 0;
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

			auto written = 0;
			for (const auto& [name, option] : data) {
				std::size_t nameSize{ name.size() };
				if (!a_intfc->WriteRecordData(&nameSize, sizeof(nameSize)) ||
					!a_intfc->WriteRecordData(name.data(), nameSize) ||
					!a_intfc->WriteRecordData(&option, sizeof(option)))
				{
					ERROR("Failed to write serialized patch settings data: {}", name);
					return;
				}
				written++;
			}

			if (written != total) {
				ERROR("Lost data during saving co-save!\nExpected: {}\nWritten: {}", total, written);
			}

			INFO("Saved {} to co-save with a size of - {}", a_name, total);
		}

		virtual void Load(SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept override
		{
			std::size_t total;
			if (!a_intfc->ReadRecordData(&total, sizeof(total))) {
				ERROR("Failed to read serialized form data: Patch Settings Total Size");
				return;
			}

			for (std::size_t i = 0; i < total; ++i) {
				std::size_t nameSize;
				if (!a_intfc->ReadRecordData(&nameSize, sizeof(nameSize))) {
					ERROR("Failed to read serialized form data: Page Name Size");
					return;
				}

				std::string name(nameSize, '\0');
				if (!a_intfc->ReadRecordData(name.data(), nameSize)) {
					ERROR("Failed to read serialized form data: Page Name");
					return;
				}

				int32_t option;
				if (!a_intfc->ReadRecordData(&option, sizeof(option))) {
					ERROR("Failed to read serialized form data: Current User Option");
					return;
				}
				data.emplace(std::move(name), option);
			}

			INFO("Loaded SKSE co-save {} with a size of - {}", a_name, data.size());
		}

		virtual void Revert([[maybe_unused]] SKSE::SerializationInterface* a_intfc, std::string_view a_name) noexcept
		{
			INFO("Reverting {} from co-save", a_name);
			data.clear();
		}

		std::unordered_map<std::string, int32_t> data;
	};
}  // namespace Serialization
