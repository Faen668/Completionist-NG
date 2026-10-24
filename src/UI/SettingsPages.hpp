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

		// Tracking, kept in the co-save
		struct TrackedItem
		{
			RE::FormID  form{ 0 };
			RE::FormID  ref{ 0 };
			std::string name;
			std::string type;
			bool        pinned{ false };
			bool        placed{ false };
		};
		[[nodiscard]] static std::vector<TrackedItem> Tracked();

		static void Pin(RE::TESForm* a_form, const std::string& a_name);
		static void Unpin(RE::FormID a_form);
		static void UnpinAll();
		static void PlacePinsFound(const std::vector<RE::FormID>& a_items);

		static bool MarkReference(RE::TESObjectREFR* a_ref, RE::TESForm* a_item, const std::string& a_name, const std::string& a_type);
		// a_item 0 clears every marker on the reference
		static void ClearMarker(RE::FormID a_ref, RE::FormID a_item = 0);
		static void ClearMarkers();
		static void DropUnlisted(const std::vector<std::pair<RE::FormID, RE::FormID>>& a_listed);

		[[nodiscard]] static bool IsCellExcluded(RE::TESObjectCELL* a_cell);

		// Location marker, kept in the co-save
		[[nodiscard]] static RE::FormID LocationMarker();
		static void                     PlaceLocationMarker(RE::TESObjectREFR* a_mapMarker, const std::string& a_name);
		static void                     ClearLocationMarker(bool a_arrived);
		static void                     CheckLocationMarker();

		static void RestoreTargets();

		static void BeforeSave();

		static void WatchTargets();

	private:
		std::int32_t _logDate{ 0 };
	};
}
