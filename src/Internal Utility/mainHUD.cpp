#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "Variables.hpp"
#include "mainHUD.hpp"
#include "QuickLootAPI.h"

#undef GetModuleHandle

namespace Completionist_MainHUD 
{
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	struct moreHUDmessage 
	{
		RE::FormID m_formID;
		bool m_icontype; // false = New, true = Found
		bool m_display;
	};

	struct quickLootMessage 
	{
		RE::FormID m_formId;
		std::string newName;
	};

	struct CompletionistRequest
	{
		RE::FormID m_formId;
	};

	struct CompletionistRequestEE
	{
		RE::FormID m_formId;
	};

	CompletionistRequest s_messagefrommoreHUD{};
	CompletionistRequestEE s_messagefromQuickLootEE{};

	//---------------------------------------------------
	//-- QuickLoot EE Support ---------------------------
	//---------------------------------------------------

	void TextnTagsAPI::RegisterQuickLootEEListener()
	{
		auto messageInterface = SKSE::GetMessagingInterface();
		auto regQLEE = messageInterface->RegisterListener("QuickLootEE", TextnTagsAPI::QuickLootEEMessageHandler);
	}

	void TextnTagsAPI::QuickLootEEMessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		auto* SKSEMessaging = SKSE::GetMessagingInterface();

		std::string basename{};
		RE::FormID  baseform{};
		bool		basetype{};

		if (a_msg->type != 1 || !a_msg->data) { return; }

		s_messagefromQuickLootEE = *static_cast<struct CompletionistRequestEE*>(a_msg->data);

		auto* receievedForm = RE::TESForm::LookupByID(s_messagefromQuickLootEE.m_formId);
		if (receievedForm)
		{
			baseform = receievedForm->GetFormID();
			basename = receievedForm->GetName();
			basetype = TextnTagsAPI::ItemIsCollectable(receievedForm);
		}

		//INFO("Receieved Message From QuickLoot with [formID - {}] - [name - {}] - [Is Collectable - {}]", baseform, basename, basetype);

		if (!baseform || basename == "" || !basetype || !V_quickLoot_Enabled)
		{
			if (SKSE::WinAPI::GetModuleHandle(L"QuickLootEE")) 
			{
				quickLootMessage msg{ s_messagefromQuickLootEE.m_formId, "" };
				SKSEMessaging->Dispatch(2, &msg, sizeof(msg), "QuickLootEE");
				//INFO("Dispatched Invalid Message For - {}", s_messagefromQuickLootEE.m_formId);
				return;
			}
			return;
		}

		if (SKSE::WinAPI::GetModuleHandle(L"QuickLootEE")) 
		{
			auto name = std::string(OnUpdateInventoryName(receievedForm->GetName(), ItemIsCollected(receievedForm)));
			quickLootMessage msg{ s_messagefromQuickLootEE.m_formId, name };
			SKSEMessaging->Dispatch(2, &msg, sizeof(msg), "QuickLootEE");
			//INFO("Dispatched Valid Messge For - {} with name - {}", s_messagefromQuickLootEE.m_formId, name);
		}
	}

	//---------------------------------------------------
	//-- moreHUD Support --------------------------------
	//---------------------------------------------------

	void TextnTagsAPI::RegistermoreHUDListener()
	{
		auto messageInterface = SKSE::GetMessagingInterface();
		auto regMHUD = messageInterface->RegisterListener("Ahzaab's moreHUD Inventory Plugin", TextnTagsAPI::moreHUDMessageHandler);
	}

	void TextnTagsAPI::moreHUDMessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		auto* SKSEMessaging = SKSE::GetMessagingInterface();

		if (a_msg->type != 1 || !a_msg->data) { return; }

		s_messagefrommoreHUD = *static_cast<CompletionistRequest*>(a_msg->data);
		
		auto* baseform = RE::TESForm::LookupByID(s_messagefrommoreHUD.m_formId);

		if (!baseform || !baseform->GetName() || !TextnTagsAPI::ItemIsCollectable(baseform)) 
		{ 
			if (SKSE::WinAPI::GetModuleHandle(L"AHZmoreHUDInventory")) 
			{
				moreHUDmessage msg{ s_messagefrommoreHUD.m_formId, false, false };
				SKSEMessaging->Dispatch(2, &msg, sizeof(msg), "Ahzaab's moreHUD Inventory Plugin");
				return;
			}
			return;
		}

		if (SKSE::WinAPI::GetModuleHandle(L"AHZmoreHUDInventory")) 
		{
			moreHUDmessage msg{ baseform->GetFormID(), ItemIsCollected(baseform), V_moreHudEnabled_Menus };
			SKSEMessaging->Dispatch(2, &msg, sizeof(msg), "Ahzaab's moreHUD Inventory Plugin");
		}
	}

	//---------------------------------------------------
	//-- Quickloot RE Support  --------------------------
	//---------------------------------------------------

	void TextnTagsAPI::RegisterQuickLootListener()
	{
		auto messageInterface = SKSE::GetMessagingInterface();
		auto isRegistered = messageInterface->RegisterListener("QuickLootRE", TextnTagsAPI::QuickLootMessageHandler);
	}

	void TextnTagsAPI::QuickLootMessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		if ((strcmp(a_msg->sender, "QuickLootRE") == 0) && a_msg->type == QL_RegisterPreprocessListItem) {
			QL_RegisterPreprocessListItem_t DataSet = (QL_RegisterPreprocessListItem_t)a_msg->data;
			DataSet(NULL, (QL_PreprocessListItemCallback_t)TextnTagsAPI::QuickLootMessageCallBack);
		}
	}
	
	void TextnTagsAPI::QuickLootMessageCallBack(void*, RE::GFxValue* gfx, RE::TESForm* form, int32_t count)
	{
		if (!form || !gfx || !form->GetName() || !TextnTagsAPI::ItemIsCollectable(form) || !V_quickLoot_Enabled)
		{
			return;
		}

		gfx->SetMember("displayName", { OnUpdateInventoryName(form->GetName(), ItemIsCollected(form)) } );
	}

	//---------------------------------------------------
	//-- Install Hooks for Main HUD & Inventory Items ---
	//---------------------------------------------------

	void TextnTagsAPI::Register() {

		auto& trampoline = SKSE::GetTrampoline();
		_OnUpdateCrosshairText = trampoline.write_call<5>(RELOCATION_ID(39535, 40621).address() + REL::Relocate(0x289, 0x280), OnUpdateCrosshairText);
		_OnUpdateInventoryText = trampoline.write_branch<5>(RELOCATION_ID(50926, 51803).address() + REL::Relocate(0x4, 0x4), OnUpdateInventoryText);

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(TextnTagsAPI::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Events ( Update Variables & Clear Garbage ) ----
	//---------------------------------------------------

	EventResult TextnTagsAPI::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event) { return RE::BSEventNotifyControl::kContinue; }

		if (!a_event->opening) { garbageDump.clear(); }

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Object Processing For Inventory Items ----------
	//---------------------------------------------------

	const char* TextnTagsAPI::OnUpdateInventoryText(RE::InventoryEntryData* a_this) 
	{
		if (!a_this->object || !a_this->object->GetName() || !ItemIsCollectable(a_this->object))
		{ 
			return _OnUpdateInventoryText(a_this); 
		}

		return OnUpdateInventoryName(_OnUpdateInventoryText(a_this), ItemIsCollected(a_this->object));
	}

	//---------------------------------------------------
	//-- Prefix Processing For Inventory Items ----------
	//---------------------------------------------------

	std::string TextnTagsAPI::GetPrefix(int32_t a_variable)
	{
		switch (a_variable)
		{
		case 0: //One Star Prefix
			return "*";

		case 1: //Two Star Prefix
			return "**";

		case 2: //Three Star Prefix
			return "***";

		default:
			return "***";
		}
	}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	const char* TextnTagsAPI::OnUpdateInventoryName(const char* a_this, bool a_collected) 
	{
		auto newColour = V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
		auto foundColour = V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;
		
		std::string g_prefix = GetPrefix(V_PrefixChoice_G);
		std::string n_prefix = GetPrefix(V_PrefixChoice_N);

		if (a_collected) 
		{
			switch (V_TextChoice_G) 
			{
			case 0: //Append
				return garbageDump.emplace_back(fmt::format("{:s} {:s}"sv, a_this, g_prefix)).c_str();

			case 1: //Prepend
				return garbageDump.emplace_back(fmt::format("{:s} {:s}"sv, g_prefix, a_this)).c_str();

			case 2: //Wrap
				return garbageDump.emplace_back(fmt::format("{:s} {:s} {:s}"sv, g_prefix, a_this, g_prefix)).c_str();

			case 3: //Append With Colour
				return garbageDump.emplace_back(fmt::format("{:s} {:s}CompTag{:s}"sv, a_this, g_prefix, std::to_string(foundColour))).c_str();

			case 4: //Prepend With Colour
				return garbageDump.emplace_back(fmt::format("{:s} {:s}CompTag{:s}"sv, g_prefix, a_this, std::to_string(foundColour))).c_str();

			case 5: //Wrap With Colour
				return garbageDump.emplace_back(fmt::format("{:s} {:s} {:s}CompTag{:s}"sv, g_prefix, a_this, g_prefix, std::to_string(foundColour))).c_str();

			case 6: //Just Colour
				return garbageDump.emplace_back(fmt::format("{:s}CompTag{:s}"sv, a_this, std::to_string(foundColour))).c_str();

			default: //None
				return a_this;
			}
		}

		switch (V_TextChoice_N) 
		{
		case 0: //Append
			return garbageDump.emplace_back(fmt::format("{:s} {:s}"sv, a_this, n_prefix)).c_str();

		case 1: //Prepend
			return garbageDump.emplace_back(fmt::format("{:s} {:s}"sv, n_prefix, a_this)).c_str();

		case 2: //Wrap
			return garbageDump.emplace_back(fmt::format("{:s} {:s} {:s}"sv, n_prefix, a_this, n_prefix)).c_str();

		case 3: //Append With Colour
			return garbageDump.emplace_back(fmt::format("{:s} {:s}CompTag{:s}"sv, a_this, n_prefix, std::to_string(newColour))).c_str();

		case 4: //Prepend With Colour
			return garbageDump.emplace_back(fmt::format("{:s} {:s}CompTag{:s}"sv, n_prefix, a_this, std::to_string(newColour))).c_str();

		case 5: //Wrap With Colour
			return garbageDump.emplace_back(fmt::format("{:s} {:s} {:s}CompTag{:s}"sv, n_prefix, a_this, n_prefix, std::to_string(newColour))).c_str();

		case 6: //Just Colour
			return garbageDump.emplace_back(fmt::format("{:s}CompTag{:s}"sv, a_this, std::to_string(newColour))).c_str();

		default: //None
			return a_this;
		}
	}

	//---------------------------------------------------
	//-- Crosshair Hook For Main HUD  -------------------
	//---------------------------------------------------

	void TextnTagsAPI::OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data) {

		_OnUpdateCrosshairText(a_this, a_menuName, a_type, a_data);

		const auto data = a_data ? static_cast<RE::HUDData*>(a_data) : nullptr;
		const auto crossHairRef = data ? data->crossHairRef.get() : RE::TESObjectREFRPtr();

		if (!data) { return; }
		ProcessCrosshairReference(data);
	}

	//---------------------------------------------------
	//-- Crosshair Ref Processing for Main HUD ----------
	//---------------------------------------------------

	void TextnTagsAPI::ProcessCrosshairReference(RE::HUDData* data) {
		
		auto* SKSEMessaging = SKSE::GetMessagingInterface();

		auto CurrentRef = RE::CrosshairPickData::GetSingleton();
		if (!CurrentRef) { return; }

		auto CurrentObj = CurrentRef->target.get();
		if (!CurrentObj) { return; }

		RE::TESForm* Base = CurrentObj.get()->GetBaseObject();
		if (!Base) { return; }

		bool PrevCollected = ItemIsCollected(Base);

		if (!ItemIsCollectable(Base) || PrevCollected && V_CrosshairTag_Found == "..." || !PrevCollected && V_CrosshairTag_New == "...") { return; }

		if (SKSEMessaging && SKSE::WinAPI::GetModuleHandle(L"AHZmoreHUDPlugin")) {
			moreHUDmessage msg{ Base->GetFormID(), PrevCollected, (V_moreHudEnabled_Crosshair) };
			SKSEMessaging->Dispatch(1, &msg, sizeof(msg), "Ahzaab's moreHUD Plugin");
		}

		auto& newColour = V_HUD_Override_Enabled_New_Crosshair ? V_HUD_CustomColourString_New_Crosshair : V_HUD_ColourString_New_Crosshair;
		auto& foundColour = V_HUD_Override_Enabled_Found_Crosshair ? V_HUD_CustomColourString_Found_Crosshair : V_HUD_ColourString_Found_Crosshair;

		data->text = PrevCollected ? 
			fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, foundColour, V_CrosshairTag_Found) :
			fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, newColour, V_CrosshairTag_New);
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool TextnTagsAPI::ItemIsCollectable(RE::FormID a_formID) {
		return CompletionistData::CheckIsCollectable(a_formID);
	}

	// Override
	bool TextnTagsAPI::ItemIsCollectable(RE::TESForm* a_form) {
		return CompletionistData::CheckIsCollectable(a_form->GetFormID());
	}

	bool TextnTagsAPI::ItemIsCollected(RE::FormID a_formID) { 
		return FoundItemData.HasForm(a_formID) || FoundItemData_NoShow.HasForm(a_formID); 
	}

	// Override
	bool TextnTagsAPI::ItemIsCollected(RE::TESForm* a_form) { 
		return FoundItemData.HasForm(a_form->GetFormID()) || FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}
}