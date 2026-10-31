#pragma once

namespace CompletionistUI
{
	// Quest markers on runtime copies of Completionist_Tracker, one slot each, game thread
	// Pins and cell scanner marks follow the loaded cells, NPCs and places from the pages don't
	class Tracker
	{
	public:
		// Pins and marks, kept in the co-save
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
		[[nodiscard]] static std::string              PinnedNames();

		static void Pin(RE::TESForm* a_form, const std::string& a_name);
		static void Unpin(RE::FormID a_form);
		static void UnpinAll();
		static void PlacePinsFound(const std::vector<RE::FormID>& a_items);
		static void ShowPinObjectives(bool a_shown);
		static void ShowPinGlow(bool a_shown);

		static bool MarkReference(RE::TESObjectREFR* a_ref, RE::TESForm* a_item, const std::string& a_name, const std::string& a_type);
		// a_item 0 clears every marker on the reference
		static void ClearMarker(RE::FormID a_ref, RE::FormID a_item = 0);
		static void ClearMarkers();
		static void DropUnlisted(const std::vector<std::pair<RE::FormID, RE::FormID>>& a_listed);

		// NPCs, placed items and map markers from the pages, as many as there are slots
		[[nodiscard]] static std::vector<RE::FormID> Located();
		// a_text: the objective's translation key with {} for the name, when not the usual one
		static bool                                  Locate(RE::TESObjectREFR* a_ref, const std::string& a_name, bool a_announce = true, const std::string& a_text = {});
		static void                                  Unlocate(RE::FormID a_ref);
		static void                                  UnlocateAll();

		static void Restore();
		static void BeforeSave();
		static void Watch();
	};
}
