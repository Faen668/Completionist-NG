#include "DKUtil/Utility.hpp"
#pragma once

namespace Serialization
{
	enum class PatchDataEnum {
		kactivePage,
		kdefaultPage,
		kuse_default_page,
		ksearchTerms,
		kcurrentUserOption,
	};

	struct PatchData 
	{
		int32_t activePage;
		int32_t defaultPage;
		bool use_default_page;
		std::string searchTerms;
		int32_t currentUserOption;

		PatchData(int32_t ap = 0, int32_t dp = 0, bool ud = false, const std::string& st = "", int32_t au = 0)
			: activePage(ap), defaultPage(dp), use_default_page(ud), searchTerms(st), currentUserOption(au) {}
	};

	struct CompletionistPatchSettings final : public ISerializable
	{
	private:
		const std::unordered_map<PatchDataEnum, std::function<void(PatchData&, const std::string&)>> updateFunctions = {
			{ PatchDataEnum::kactivePage,		[](PatchData& pd, const std::string& a_value) { pd.activePage =			std::stoi(a_value); } },
			{ PatchDataEnum::kdefaultPage,		[](PatchData& pd, const std::string& a_value) { pd.defaultPage =		std::stoi(a_value); } },
			{ PatchDataEnum::kuse_default_page, [](PatchData& pd, const std::string& a_value) { pd.use_default_page =	std::stoi(a_value) != 0; } },
			{ PatchDataEnum::ksearchTerms,		[](PatchData& pd, const std::string& a_value) { pd.searchTerms =		fmt::format("{}{}", a_value, pd.searchTerms); } },
			{ PatchDataEnum::kcurrentUserOption,[](PatchData& pd, const std::string& a_value) { pd.currentUserOption =	std::stoi(a_value); } },
		};
	public:

		void CreatePageIfRequired(const std::string& a_page) {
			auto it = data.find(a_page);

			if (it == data.end()) {
				it = data.emplace(a_page, PatchData(0, 0, false, "", 0)).first;
			}
		}

		template <PatchDataEnum variable, typename T>
		void UpdateSetting(const std::string& a_page, const T& a_value) {
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			if (it != data.end()) {
				PatchData& patchData = it->second;

				auto updateFunctionIt = updateFunctions.find(variable);
				if (updateFunctionIt == updateFunctions.end()) {
					return;
				}

				using ValueType = typename std::decay<T>::type;

				if constexpr (std::is_same_v<ValueType, std::string>) {
					updateFunctionIt->second(patchData, a_value);
				}
				else {
					updateFunctionIt->second(patchData, std::to_string(a_value));
				}
			}
		}

		void ResetSettings(const std::string& a_page) noexcept {
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			if (it != data.end()) {
				PatchData& patchData = it->second;
				patchData.activePage = 0;
				patchData.defaultPage = 0;
				patchData.use_default_page = 0;
				patchData.searchTerms = "";
				patchData.currentUserOption = 0 ;
			}
		}

		void DeleteSettings(const std::string& a_page) noexcept {
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			if (it != data.end()) {
				data.erase(it);
			}
		}

		void RemoveDuplicateSearchTerms(const std::string& a_page, const std::string& a_term) noexcept
		{
			auto it = data.find(a_page.data());
			if (it != data.end()) {
				PatchData& patchData = it->second;
				patchData.searchTerms = DKUtil::string::replace_nth_occurrence(patchData.searchTerms, 0, a_term, {});
			}
		};

		void ClearSearchHistory(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			if (it != data.end()) {
				PatchData& patchData = it->second;
				patchData.searchTerms = "";
			}
		}

		void AddSearchTerm(const std::string& a_page, const std::string& a_term) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			if (it != data.end()) {
				const auto term = fmt::format("|{}|", a_term);
				RemoveDuplicateSearchTerms(a_page, term);
				UpdateSetting<PatchDataEnum::ksearchTerms>(a_page, term);
			}
		}

		[[nodiscard]] int32_t GetActivePage(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			return (it != data.end()) ? it->second.activePage : 0;
		}

		[[nodiscard]] int32_t GetDefaultPage(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			return (it != data.end()) ? it->second.defaultPage : 0;
		}

		[[nodiscard]] bool GetUseDefault(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			return (it != data.end()) ? it->second.use_default_page : false;
		}

		[[nodiscard]] std::string GetSearchTerms(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			return (it != data.end()) ? it->second.searchTerms : "";
		}

		[[nodiscard]] int32_t GetCurrentUserOption(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			auto it = data.find(a_page.data());
			return (it != data.end()) ? it->second.currentUserOption : 0;
		}

		[[nodiscard]] std::vector<std::string> GetCompiledSearchHistory(const std::string& a_page) noexcept
		{
			CreatePageIfRequired(a_page);

			std::vector<std::string> list{};

			auto it = data.find(a_page.data());
			if (it == data.end()) {
				list.push_back("$NoSearchHistory");
				return list;
			}

			PatchData& patchData = it->second;
			auto raw = DKUtil::string::split(patchData.searchTerms, "|");
			for (auto& r : raw) {
				if (!r.empty() && !DKUtil::string::iequals(r, " ")) {
					list.push_back(r);
				}
			}

			if (list.size() == 0)
			{
				list.push_back("$NoSearchHistory");
			}
			else
			{
				list.insert(list.end(), "$ClearSearchHistory");
			};
			return list;
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
			for (const auto& [name, patchData] : data) {
				std::size_t nameSize{name.size()};
				std::size_t termSize { patchData.searchTerms.size() };

				if (!a_intfc->WriteRecordData(&nameSize, sizeof(nameSize)) ||
					!a_intfc->WriteRecordData(name.data(), nameSize) ||
					!a_intfc->WriteRecordData(&termSize, sizeof(termSize)) ||
					!a_intfc->WriteRecordData(patchData.searchTerms.data(), termSize) ||
					!a_intfc->WriteRecordData(&patchData.activePage, sizeof(patchData.activePage)) ||
					!a_intfc->WriteRecordData(&patchData.defaultPage, sizeof(patchData.defaultPage)) ||
					!a_intfc->WriteRecordData(&patchData.use_default_page, sizeof(patchData.use_default_page)) ||
					!a_intfc->WriteRecordData(&patchData.currentUserOption, sizeof(patchData.currentUserOption)))
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

			auto read = 0;
			for (auto i = 0; i < total; ++i) {
				std::string name;
				std::string term;
				int32_t activePage;
				int32_t defaultPage;
				bool use_default_page;
				int32_t currentUserOption;

				std::size_t nameSize;
				if (!a_intfc->ReadRecordData(&nameSize, sizeof(nameSize))) {
					ERROR("Failed to read serialized form data: Page Name Size");
					return;
				}

				name.resize(nameSize);
				if (!a_intfc->ReadRecordData(name.data(), nameSize)) {
					ERROR("Failed to read serialized form data: Page Name");
					return;
				}

				std::size_t termSize;
				if (!a_intfc->ReadRecordData(&termSize, sizeof(termSize))) {
					ERROR("Failed to read serialized form data: Search Term Size");
					return;
				}

				term.resize(termSize);
				if (termSize && !a_intfc->ReadRecordData(term.data(), termSize)) {
					ERROR("Failed to read serialized form data: Search Term");
					return;
				}

				if (!a_intfc->ReadRecordData(&activePage, sizeof(activePage))) {
					ERROR("Failed to read serialized form data: Active Page");
					return;
				}

				if (!a_intfc->ReadRecordData(&defaultPage, sizeof(defaultPage))) {
					ERROR("Failed to read serialized form data: Default Page");
					return;
				}

				if (!a_intfc->ReadRecordData(&use_default_page, sizeof(use_default_page))) {
					ERROR("Failed to read serialized form data: Use Default Page");
					return;
				}

				if (!a_intfc->ReadRecordData(&currentUserOption, sizeof(currentUserOption))) {
					ERROR("Failed to read serialized form data: Current User Option");
					return;
				}

				data.emplace(name, PatchData(activePage, defaultPage, use_default_page, term, currentUserOption));
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
		std::unordered_map<std::string, PatchData> data;
	};
}  // namespace Serialization