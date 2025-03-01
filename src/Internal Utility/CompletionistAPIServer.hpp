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
		virtual bool UseNeededItemTextColor();
		virtual bool UseCollectedItemTextColor();

		virtual void GetItemInfo(CompletionistAPI::ItemInfo* info, size_t count);
		virtual void GetIconInfo(CompletionistAPI::IconInfo* info, size_t count);

		virtual RE::BSString GetDecoratedItemName(RE::TESBoundObject* object);
		virtual uint32_t GetItemTextColor(RE::TESBoundObject* object);
		virtual bool IsItemNeeded(RE::TESBoundObject* object);
		virtual bool IsItemCollected(RE::TESBoundObject* object);
		virtual bool IsItemTracked(RE::TESBoundObject* object);
	};

	static InterfaceV20* GetInterfaceV20() { return &_interface; }

private:
	static inline InterfaceV20 _interface{};
};
