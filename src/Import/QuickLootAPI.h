#pragma once

#include <stdint.h>

enum
{
	// First register a listener for "QuickLootRE" using the SKSE "kMessage_PostLoad" message event
	// On "kMessage_PostPostLoad" QuickLoot will dispatch a method to all listeners which they can use to register their callbacks
	// Here is an example:
	//	extern SKSEMessagingInterface* g_messaging;
	//	extern PluginHandle g_myHandle;
	//	void MyPreprocessListItemCallback(void*, GFxValue* gfx, TESForm* form, uin32_t count);
	//	void MessageListener(SKSEMessagingInterface::Message* msg){
	//		if ((strcmp(msg->sender, "SKSE") == 0) && msg->type == SKSEMessagingInterface::kMessage_PostLoad) {
	//			g_messaging->RegisterListener(g_myHandle, "QuickLootRE", MessageListener);
	//		} else if ((strcmp(msg->sender, "QuickLootRE") == 0) && msg->type == QL_RegisterPreprocessListItem){
	//			QL_RegisterPreprocessListItem_t registrar = (QL_RegisterPreprocessListItem_t)msg->data;
	//			registrar(NULL, MyPreprocessListItemCallback);
	//		}
	//	}
	QL_RegisterPreprocessListItem
};

// This callback is invoked for each gfx item that will be sent to the lootmenu
// after processing on the c++ side, and before invalidation in the actionscript side
typedef void (*QL_PreprocessListItemCallback_t)(
	// User provided context from the point of registration
	void* context,
	// GFxValue*/RE::GFxValue*
	// The gfx value which represents the item
	// Never NULL
	void* gfx,
	// TESForm*/RE::TESForm*
	// The base form of the item
	// 99.99% chance it's never NULL
	void* form,
	// The number of items in the stack
	// (e.g. 7 iron daggers)
	// 99% sure it's always positive
	int32_t count);

typedef void (*QL_RegisterPreprocessListItem_t)(
	// Some sort of context the user may wish to be supplied when the callback is invoked
	// Pass NULL if you don't care
	void* context,
	// The actual callback itself
	QL_PreprocessListItemCallback_t callback);
