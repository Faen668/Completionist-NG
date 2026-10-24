#pragma once

#include "UI/Core/UIModel.hpp"

#include <string_view>

namespace CompletionistUI
{
	class IDataProvider
	{
	public:
		virtual ~IDataProvider() = default;

		[[nodiscard]] virtual std::vector<Category> GetCategories() = 0;
		[[nodiscard]] virtual Page                  GetPage(std::string_view a_id) = 0;

		virtual void SetCompleted(std::string_view a_pageID, const Entry& a_entry, bool a_completed) = 0;

		[[nodiscard]] virtual std::vector<SettingsPage> GetSettingsPages() { return {}; }
		virtual void                                    ApplySetting(std::string_view, const SettingItem&) {}
		[[nodiscard]] virtual AboutInfo                 GetAbout() { return {}; }

		[[nodiscard]] virtual std::uint32_t Revision() { return 0; }

		[[nodiscard]] virtual std::vector<RecentEvent> GetRecentEvents() { return {}; }

		[[nodiscard]] virtual std::vector<KillRecord> GetKills() { return {}; }
		virtual void                                  ResetKills(std::string_view, bool /*a_remove*/) {}

		[[nodiscard]] virtual PinList GetPins() { return {}; }
		virtual void                  SetPin(const Entry&) {}
		virtual void                  ClearPin(std::uint32_t /*a_formID*/) {}
		virtual void                  ClearPins() {}

		[[nodiscard]] virtual std::uint32_t GetLocationMarker() { return 0; }
		virtual void                        SetLocationMarker(const Entry&) {}
		virtual void                        ClearLocationMarker() {}

		[[nodiscard]] virtual CellScan GetCellScan() { return {}; }
		virtual void                   RescanCell() {}
		virtual void                   MarkCellItem(const CellItem&) {}
		virtual void                   ClearCellMarker(const CellItem&) {}
		virtual void                   ClearCellMarkers() {}
		virtual void ExcludeCurrentCell() {}
		virtual void ExcludeCellItem(const CellItem&) {}

		[[nodiscard]] virtual std::string Translate(std::string_view a_key) = 0;
	};
}
