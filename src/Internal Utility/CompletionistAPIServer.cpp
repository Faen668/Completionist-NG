#include "CompletionistAPIServer.hpp"
#include "Serialization.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "Internal Utility/Variables.hpp"
#include "Frameworks/FrameworkMaster.hpp"

using hud = Completionist_MainHUD::TextnTagsAPI;
using namespace CVariables;

extern "C" __declspec(dllexport) void* GetCompletionistInterfaceV20()
{
	return CompletionistAPIServer::GetInterfaceV20();
}

uint32_t CompletionistAPIServer::InterfaceV20::GetNeededItemTextColor()
{
	return V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
}

uint32_t CompletionistAPIServer::InterfaceV20::GetCollectedItemTextColor()
{
	return V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;
}

bool CompletionistAPIServer::InterfaceV20::UseNeededItemTextColor()
{
	return V_TextChoice_N >= 3 && V_TextChoice_N <= 6;
}

bool CompletionistAPIServer::InterfaceV20::UseCollectedItemTextColor()
{
	return V_TextChoice_G >= 3 && V_TextChoice_G <= 6;
}

void CompletionistAPIServer::InterfaceV20::GetItemInfo(CompletionistAPI::ItemInfo* info, size_t count)
{
	if (!info || count == 0) return;

	for (auto i = 0; i < count; i++)
	{
		auto& currentItem = info[i];
		if (hud::ItemIsCollectable(currentItem.object)) {
			const bool collected = hud::ItemIsCollected(currentItem.object);
			currentItem.decoratedName = hud::API_GetDisplayNamePrefix(currentItem.object->GetName(), collected);
			currentItem.textColor = collected ? GetCollectedItemTextColor() : GetNeededItemTextColor();
			currentItem.isNeeded = !collected;
			currentItem.isCollected = collected;
		}
		else {
			currentItem.decoratedName = currentItem.object->GetName();
			currentItem.textColor = -1;
			currentItem.isNeeded = false;
			currentItem.isCollected = false;
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
		}
		else {
			currentItem.isNeeded = false;
			currentItem.isCollected = false;
		}
	}
}

RE::BSString CompletionistAPIServer::InterfaceV20::GetDecoratedItemName(RE::TESBoundObject* object)
{
	return !object ? "" : !IsItemTracked(object) ? object->GetName() : hud::API_GetDisplayNamePrefix(object->GetName(), IsItemCollected(object));
}

uint32_t CompletionistAPIServer::InterfaceV20::GetItemTextColor(RE::TESBoundObject* object)
{
	return (!object || !IsItemTracked(object)) ? -1 : IsItemCollected(object) ? GetCollectedItemTextColor() : GetNeededItemTextColor();
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
