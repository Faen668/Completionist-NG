#pragma once

#include "Import/CompletionistAPI.h"

class CompletionistAPIServer
{
public:
	CompletionistAPIServer() = delete;
	~CompletionistAPIServer() = delete;
	CompletionistAPIServer(CompletionistAPIServer const&) = delete;
	CompletionistAPIServer(CompletionistAPIServer const&&) = delete;
	CompletionistAPIServer operator=(CompletionistAPIServer&) = delete;
	CompletionistAPIServer operator=(CompletionistAPIServer&&) = delete;

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	struct InterfaceV20
	{
		virtual uint32_t GetNeededItemTextColor();
		virtual uint32_t GetCollectedItemTextColor();
		virtual uint32_t GetOccupiedItemTextColor();
		virtual uint32_t GetDisplayableItemTextColor();
		virtual uint32_t GetDisplayedItemTextColor();

		virtual bool UseNeededItemTextColor();
		virtual bool UseCollectedItemTextColor();
		virtual bool UseDisplayableItemTextColor();
		virtual bool UseDisplayedItemTextColor();
		virtual bool UseOccupiedItemTextColor();

		virtual bool TreatOccupiedAsDisplayed();

		virtual void GetItemInfo(CompletionistAPI::ItemInfo* info, size_t count);
		virtual void GetIconInfo(CompletionistAPI::IconInfo* info, size_t count);

		virtual RE::BSString GetDecoratedItemName(RE::InventoryEntryData* entry);
		virtual uint32_t GetItemTextColor(RE::TESBoundObject* object);
		virtual bool IsItemNeeded(RE::TESBoundObject* object);
		virtual bool IsItemCollected(RE::TESBoundObject* object);
		virtual bool IsItemTracked(RE::TESBoundObject* object);
		virtual bool IsItemDisplayable(RE::TESBoundObject* object);
		virtual bool IsItemDisplayed(RE::TESBoundObject* object, bool& isVariationDisplayedInstead);
	};

	static InterfaceV20* GetInterfaceV20() { return &_interface; }

private:
	static inline InterfaceV20 _interface{};
};
