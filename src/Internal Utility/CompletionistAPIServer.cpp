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

uint32_t CompletionistAPIServer::InterfaceV20::GetNeededItemTextColor()
{
	if (!UseNeededItemTextColor()) return -1;

	return V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
}

uint32_t CompletionistAPIServer::InterfaceV20::GetCollectedItemTextColor()
{
	if (!UseCollectedItemTextColor()) return -1;

	return V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;
}

uint32_t CompletionistAPIServer::InterfaceV20::GetOccupiedItemTextColor()
{
	if (!UseOccupiedItemTextColor()) return -1;

	return V_HUD_Override_Enabled_Occupied ? V_HUD_CustomColour_Occupied : V_HUD_Colour_Occupied;
}

uint32_t CompletionistAPIServer::InterfaceV20::GetDisplayableItemTextColor()
{
	if (!UseDisplayableItemTextColor()) return -1;

	return V_HUD_Override_Enabled_Displayable ? V_HUD_CustomColour_Displayable : V_HUD_Colour_Displayable;
}

uint32_t CompletionistAPIServer::InterfaceV20::GetDisplayedItemTextColor()
{
	if (!UseDisplayedItemTextColor()) return -1;

	return V_HUD_Override_Enabled_Displayed ? V_HUD_CustomColour_Displayed : V_HUD_Colour_Displayed;
}

bool CompletionistAPIServer::InterfaceV20::UseNeededItemTextColor()
{
	return V_TextChoice_N >= 3 && V_TextChoice_N <= 6;
}

bool CompletionistAPIServer::InterfaceV20::UseCollectedItemTextColor()
{
	return V_TextChoice_G >= 3 && V_TextChoice_G <= 6;
}

bool CompletionistAPIServer::InterfaceV20::UseDisplayableItemTextColor()
{
	return V_TextChoice_Displayable >= 3 && V_TextChoice_Displayable <= 6;
}

bool CompletionistAPIServer::InterfaceV20::UseDisplayedItemTextColor()
{
	return V_TextChoice_Displayed >= 3 && V_TextChoice_Displayed <= 6;
}

bool CompletionistAPIServer::InterfaceV20::UseOccupiedItemTextColor()
{
	return V_TextChoice_Occupied >= 3 && V_TextChoice_Occupied <= 6;
}

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
		auto& boundObject = currentItem.entry->object;

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

				if (displayed)
					currentItem.textColor = GetDisplayedItemTextColor();
				else if (isVariationDisplayedInstead)
					currentItem.textColor = GetOccupiedItemTextColor();
				else if (currentItem.isDisplayable)
					currentItem.textColor = GetDisplayableItemTextColor();
				else if (collected)
					currentItem.textColor = GetCollectedItemTextColor();
				else
					currentItem.textColor = GetNeededItemTextColor();
			}
			else
			{
				currentItem.decoratedName = GetDecoratedItemName(currentItem.entry);
				currentItem.textColor = collected ? GetCollectedItemTextColor() : GetNeededItemTextColor();
				currentItem.isDisplayable = false;
				currentItem.isDisplayed = false;
				currentItem.isOccupied = false;
				currentItem.treatOccupiedAsDisplayed = false;
			}
		}
		else {
			currentItem.decoratedName = currentItem.entry->GetDisplayName();
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
	bool isVariationDisplayedInstead = false;

	if (!entry)
		return "";

	bool displayed = dbm::IsDisplayed(entry->object, isVariationDisplayedInstead);

	if (isVariationDisplayedInstead && TreatOccupiedAsDisplayed())
		displayed = true;

	return !IsItemTracked(entry->object)
		? entry->GetDisplayName()
		: hud::API_GetDisplayNamePrefix(entry, IsItemCollected(entry->object), displayed, isVariationDisplayedInstead);
}

uint32_t CompletionistAPIServer::InterfaceV20::GetItemTextColor(RE::TESBoundObject* object)
{
	if (!object || !IsItemTracked(object)) {
		return -1;
	}

	bool isVariationDisplayedInstead = false;
	bool displayed = dbm::IsDisplayed(object, isVariationDisplayedInstead);

	// Treat variation as displayed if the setting allows it
	if (isVariationDisplayedInstead && TreatOccupiedAsDisplayed()) {
		displayed = true;
	}

	if (CVariables::V_MuseumModeEnabled) {
		if (displayed) {
			return GetDisplayedItemTextColor();
		}
		if (isVariationDisplayedInstead) {
			return GetOccupiedItemTextColor();
		}
	}

	if (IsItemCollected(object)) {
		return GetCollectedItemTextColor();
	}

	return GetNeededItemTextColor();
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
