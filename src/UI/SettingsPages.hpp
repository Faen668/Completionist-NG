#pragma once

#include "UI/Core/UIModel.hpp"

namespace CompletionistUI
{
	// Settings pages after General, game thread
	class SettingsPages
	{
	public:
		[[nodiscard]] std::vector<SettingsPage> Build();
		void                                    Apply(std::string_view a_pageID, const SettingItem& a_item);

		static inline std::function<std::string(std::string_view)> translate;

		[[nodiscard]] static bool IsCellExcluded(RE::TESObjectCELL* a_cell);

	private:
		std::int32_t _logDate{ 0 };
	};
}
