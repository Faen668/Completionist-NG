#include "CompletionistAPIServer.hpp"
#include "Serialization.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Internal Utility/Variables.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "Internal Utility/MuseumAPI.hpp"

using hud = Completionist_MainHUD::TextnTagsAPI;
using dbm = Completionist::MuseumAPI;
using namespace CVariables;

extern "C" __declspec(dllexport) void* GetCompletionistInterfaceV20()
{
	return CompletionistAPIServer::GetInterfaceV20();
}

// Item colours and name prefixes are gone, the API keeps its functions so
// mods built against V20 (QuickLootIE) still work: no colour and the plain name
uint32_t CompletionistAPIServer::InterfaceV20::GetNeededItemTextColor() { return static_cast<uint32_t>(-1); }
uint32_t CompletionistAPIServer::InterfaceV20::GetCollectedItemTextColor() { return static_cast<uint32_t>(-1); }
uint32_t CompletionistAPIServer::InterfaceV20::GetOccupiedItemTextColor() { return static_cast<uint32_t>(-1); }
uint32_t CompletionistAPIServer::InterfaceV20::GetDisplayableItemTextColor() { return static_cast<uint32_t>(-1); }
uint32_t CompletionistAPIServer::InterfaceV20::GetDisplayedItemTextColor() { return static_cast<uint32_t>(-1); }

bool CompletionistAPIServer::InterfaceV20::UseNeededItemTextColor() { return false; }
bool CompletionistAPIServer::InterfaceV20::UseCollectedItemTextColor() { return false; }
bool CompletionistAPIServer::InterfaceV20::UseDisplayableItemTextColor() { return false; }
bool CompletionistAPIServer::InterfaceV20::UseDisplayedItemTextColor() { return false; }
bool CompletionistAPIServer::InterfaceV20::UseOccupiedItemTextColor() { return false; }

bool CompletionistAPIServer::InterfaceV20::TreatOccupiedAsDisplayed()
{
	return V_TreatOccupiedAsDisplayed;
}

void CompletionistAPIServer::InterfaceV20::GetItemInfo(CompletionistAPI::ItemInfo* info, size_t count)
{
	if (!info || count == 0) return;

	for (auto i = 0; i < count; i++)
	{
		auto& currentItem = info[i];
		auto* boundObject = currentItem.entry ? currentItem.entry->object : nullptr;

		if (hud::ItemIsCollectable(boundObject))
		{
			const bool collected = hud::ItemIsCollected(boundObject);
			currentItem.isNeeded = !collected;
			currentItem.isCollected = collected;

			if (CVariables::V_MuseumModeEnabled)
			{
				bool isVariationDisplayedInstead = false;
				bool displayed = dbm::IsDisplayed(boundObject, isVariationDisplayedInstead);

				if (isVariationDisplayedInstead && TreatOccupiedAsDisplayed())
					displayed = true;

				currentItem.isDisplayable = dbm::IsMuseumDisplayable(boundObject);;
				currentItem.isDisplayed = displayed;
				currentItem.isOccupied = isVariationDisplayedInstead;
				currentItem.treatOccupiedAsDisplayed = TreatOccupiedAsDisplayed();
				currentItem.decoratedName = GetDecoratedItemName(currentItem.entry);
				currentItem.textColor = static_cast<uint32_t>(-1);
			}
			else
			{
				currentItem.decoratedName = GetDecoratedItemName(currentItem.entry);
				currentItem.textColor = static_cast<uint32_t>(-1);
				currentItem.isDisplayable = false;
				currentItem.isDisplayed = false;
				currentItem.isOccupied = false;
				currentItem.treatOccupiedAsDisplayed = false;
			}
		}
		else {
			currentItem.decoratedName = currentItem.entry ? currentItem.entry->GetDisplayName() : "";
			currentItem.textColor = -1;
			currentItem.isNeeded = false;
			currentItem.isCollected = false;
			currentItem.isDisplayable = false;
			currentItem.isDisplayed = false;
			currentItem.isOccupied = false;
			currentItem.treatOccupiedAsDisplayed = false;
		}
	}
}

void CompletionistAPIServer::InterfaceV20::GetIconInfo(CompletionistAPI::IconInfo* info, size_t count)
{
	if (!info || count == 0) return;

	for (auto i = 0; i < count; i++)
	{
		auto& currentItem = info[i];
		if (hud::ItemIsCollectable(currentItem.object)) {
			const bool collected = hud::ItemIsCollected(currentItem.object);
			currentItem.isNeeded = !collected;
			currentItem.isCollected = collected;

			if (CVariables::V_MuseumModeEnabled)
			{
				bool isVariationDisplayedInstead = false;
				bool displayed = dbm::IsDisplayed(currentItem.object, isVariationDisplayedInstead);

				if (isVariationDisplayedInstead && TreatOccupiedAsDisplayed())
					displayed = true;

				currentItem.isDisplayable = dbm::IsMuseumDisplayable(currentItem.object);;
				currentItem.isDisplayed = displayed;
				currentItem.isOccupied = isVariationDisplayedInstead;
				currentItem.treatOccupiedAsDisplayed = TreatOccupiedAsDisplayed();
			}
			else
			{
				currentItem.isDisplayable = false;
				currentItem.isDisplayed = false;
				currentItem.isOccupied = false;
				currentItem.treatOccupiedAsDisplayed = false;
			}
		}
		else {
			currentItem.isNeeded = false;
			currentItem.isCollected = false;
			currentItem.isDisplayable = false;
			currentItem.isDisplayed = false;
			currentItem.isOccupied = false;
			currentItem.treatOccupiedAsDisplayed = false;
		}
	}
}

RE::BSString CompletionistAPIServer::InterfaceV20::GetDecoratedItemName(RE::InventoryEntryData* entry)
{
	return entry ? entry->GetDisplayName() : "";
}

uint32_t CompletionistAPIServer::InterfaceV20::GetItemTextColor(RE::TESBoundObject*)
{
	return static_cast<uint32_t>(-1);
}

bool CompletionistAPIServer::InterfaceV20::IsItemNeeded(RE::TESBoundObject* object)
{
	return object && hud::ItemIsCollectable(object) && !hud::ItemIsCollected(object);
}

bool CompletionistAPIServer::InterfaceV20::IsItemCollected(RE::TESBoundObject* object)
{
	return object && hud::ItemIsCollected(object);
}

bool CompletionistAPIServer::InterfaceV20::IsItemTracked(RE::TESBoundObject* object)
{
	return object && hud::ItemIsCollectable(object);
}

bool CompletionistAPIServer::InterfaceV20::IsItemDisplayable(RE::TESBoundObject* object)
{
	return object && dbm::IsMuseumDisplayable(object);
}

bool CompletionistAPIServer::InterfaceV20::IsItemDisplayed(RE::TESBoundObject* object, bool& isVariationDisplayedInstead)
{
	if (!object)
		return false;

	bool displayed = dbm::IsDisplayed(object, isVariationDisplayedInstead);

	// Treat variation as displayed if the setting allows it
	if (isVariationDisplayedInstead && TreatOccupiedAsDisplayed()) {
		displayed = true;
	}

	return displayed;
}
