#include "Frameworks/FrameworkMaster.hpp"
#include "Events.hpp"
#include "Serialization.hpp"
#include "Variables.hpp"
#include "mainHUD.hpp"

#undef GetModuleHandle

namespace Completionist_MainHUD 
{
	using namespace CVariables;

	using events = CEvents::EventHandler;
	using serial = Serialization::CompletionistData;

	CompletionistRequest s_messagefrommoreHUD{};
	std::vector<std::string> formattedStringHolder{};

	//---------------------------------------------------
	//-- Install Hooks for Main HUD & Inventory Items ---
	//---------------------------------------------------

	void TextnTagsAPI::Register() 
	{
		_OnUpdateCrosshairText = events::RegisterUpdateCrosshairHook(&OnUpdateCrosshairText);
		_OnUpdateInventoryText = events::RegisterUpdateInventoryNameHook(&OnUpdateInventoryText);
		
		//Disable crafting menu hook for linux users to avoid CTD on use.
		if (!VariablesAPI::IsUsingLinux())
		{
			events::RegisterUpdateCraftingMenuHook();
		}

		//Register menu open to periodically clear the formattedStringHolder.
		events::RegisterForEvent_OnMenuOpenCloseEvent(&OnMenuOpenCloseEvent);
		SKSE::GetMessagingInterface()->RegisterListener("Ahzaab's moreHUD Inventory Plugin", moreHUDMessageHandler);
	};

	//---------------------------------------------------
	//-- moreHUD Support --------------------------------
	//---------------------------------------------------

	void TextnTagsAPI::moreHUDMessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg || a_msg->type != 1 || !a_msg->data) { return; }

		s_messagefrommoreHUD = *static_cast<CompletionistRequest*>(a_msg->data);

		auto* receievedForm = RE::TESForm::LookupByID(s_messagefrommoreHUD.formId);
		if (receievedForm && ItemIsCollectable(receievedForm))
		{
			moreHUDMessage msg{ receievedForm->GetFormID(), ItemIsCollected(receievedForm), V_moreHudEnabled_Menus };
			SKSE::GetMessagingInterface()->Dispatch(2, &msg, sizeof(msg), "Ahzaab's moreHUD Inventory Plugin");
		}
	}

	//---------------------------------------------------
	//-- Events ( Update Variables & Clear Garbage ) ----
	//---------------------------------------------------

	void TextnTagsAPI::OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event) {

		if (!a_event->opening) { formattedStringHolder.clear(); }
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

		return API_GetDisplayNameMerged(_OnUpdateInventoryText(a_this), ItemIsCollected(a_this->object));
	}

	//---------------------------------------------------
	//-- Object Processing For Inventory Items ----------
	//---------------------------------------------------

	const char* TextnTagsAPI::OnUpdateCraftingText(RE::TESForm* a_this)
	{
		if (!a_this || !a_this->GetName())
		{
			return "";
		}

		if (!ItemIsCollectable(a_this))
		{
			return a_this->GetName();
		}

		return API_GetDisplayNameMerged(a_this->GetName(), ItemIsCollected(a_this));

	}

	//---------------------------------------------------
	//-- Prefix Processing For Inventory Items ----------
	//---------------------------------------------------

	std::string TextnTagsAPI::GetPrefix(int32_t a_variable)
	{
		switch (a_variable) {
		case 0: // One Star Prefix
			return "*";
		case 1: // Two Star Prefix
			return "**";
		case 2: // Three Star Prefix
			return "***";
		default:
			return "";
		}
	}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	const char* TextnTagsAPI::API_GetDisplayNamePrefix(const char* a_this, bool a_collected) 
	{
		auto applyPrefix = [](const std::string& prefix, const char* str, int choice) -> std::string {
			switch (choice) {
			case 0: // Append
			case 3: // Append
				return fmt::format("{:s} {:s}", str, prefix);
			case 1: // Prepend
			case 4: // Prepend
				return fmt::format("{:s} {:s}", prefix, str);
			case 2: // Wrap
			case 5: // Wrap
				return fmt::format("{:s} {:s} {:s}", prefix, str, prefix);
			default: // None
				return str;
			}
			};

		const std::string g_prefix = GetPrefix(V_PrefixChoice_G);
		const std::string n_prefix = GetPrefix(V_PrefixChoice_N);

		if (a_collected) {
			return formattedStringHolder.emplace_back(applyPrefix(g_prefix, a_this, V_TextChoice_G)).c_str();
		}
		else {
			return formattedStringHolder.emplace_back(applyPrefix(n_prefix, a_this, V_TextChoice_N)).c_str();
		}
	}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	uint32_t TextnTagsAPI::API_GetDisplayNameColour(RE::FormID a_formID)
	{
		auto newColour = V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
		auto foundColour = V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;

		if (ItemIsCollected(a_formID)) {
			return (V_TextChoice_G >= 3 && V_TextChoice_G <= 6) ? foundColour : -1;
		}
		else {
			return (V_TextChoice_N >= 3 && V_TextChoice_N <= 6) ? newColour : -1;
		}

		return -1; 
	}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	const char* TextnTagsAPI::API_GetDisplayNameMerged(const char* a_this, bool a_collected) 
	{
		auto newColour = V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
		auto foundColour = V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;

		const std::string g_prefix = GetPrefix(V_PrefixChoice_G);
		const std::string n_prefix = GetPrefix(V_PrefixChoice_N);

		auto formatString = [](const std::string& prefix, const char* str, int choice, int color) -> std::string {
			switch (choice) {
			case 0: // Append
				return fmt::format("{:s} {:s}", str, prefix);
			case 1: // Prepend
				return fmt::format("{:s} {:s}", prefix, str);
			case 2: // Wrap
				return fmt::format("{:s} {:s} {:s}", prefix, str, prefix);
			case 3: // Append With Colour
				return fmt::format("{:s} {:s}CompTag{:d}", str, prefix, color);
			case 4: // Prepend With Colour
				return fmt::format("{:s} {:s}CompTag{:d}", prefix, str, color);
			case 5: // Wrap With Colour
				return fmt::format("{:s} {:s} {:s}CompTag{:d}", prefix, str, prefix, color);
			case 6: // Just Colour
				return fmt::format("{:s}CompTag{:d}", str, color);
			default: // None
				return str;
			}
			};

		if (a_collected) {
			return formattedStringHolder.emplace_back(formatString(g_prefix, a_this, V_TextChoice_G, foundColour)).c_str();
		}
		else {
			return formattedStringHolder.emplace_back(formatString(n_prefix, a_this, V_TextChoice_N, newColour)).c_str();
		}
	}

	//---------------------------------------------------
	//-- Crosshair Hook For Main HUD  -------------------
	//---------------------------------------------------

	void TextnTagsAPI::OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data)
	{
		_OnUpdateCrosshairText(a_this, a_menuName, a_type, a_data);

		const auto data = a_data ? static_cast<RE::HUDData*>(a_data) : nullptr;
		if (!data) { return; }

		ProcessCrosshairReference(data);
	}

	//---------------------------------------------------
	//-- Crosshair Ref Processing for Main HUD ----------
	//---------------------------------------------------

	void TextnTagsAPI::ProcessCrosshairReference(RE::HUDData* data) 
	{
		auto CurrentRef = RE::CrosshairPickData::GetSingleton();
		if (!CurrentRef) { return; }

		auto CurrentObj = CurrentRef->target.get();
		if (!CurrentObj) { return; }

		RE::TESForm* Base = CurrentObj.get()->GetBaseObject();
		if (!Base) { return; }

		if (!ItemIsCollectable(Base)) { return; }

		bool PrevCollected = ItemIsCollected(Base);
		if (SKSE::WinAPI::GetModuleHandle(L"AHZmoreHUDPlugin")) {
			moreHUDMessage msg{ Base->GetFormID(), PrevCollected, V_moreHudEnabled_Crosshair };
			SKSE::GetMessagingInterface()->Dispatch(1, &msg, sizeof(msg), "Ahzaab's moreHUD Plugin");
		}

		if (PrevCollected && V_CrosshairTag_Found == "..." || !PrevCollected && V_CrosshairTag_New == "...") { return; }

		auto& newColour = V_HUD_Override_Enabled_New_Crosshair ? V_HUD_CustomColourString_New_Crosshair : V_HUD_ColourString_New_Crosshair;
		auto& foundColour = V_HUD_Override_Enabled_Found_Crosshair ? V_HUD_CustomColourString_Found_Crosshair : V_HUD_ColourString_Found_Crosshair;

		data->text = PrevCollected ? 
			fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, foundColour, V_CrosshairTag_Found) :
			fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, newColour, V_CrosshairTag_New);
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool TextnTagsAPI::ItemIsCollectable(RE::FormID a_formID) 
	{
		return serial::CheckIsCollectable(a_formID);
	}

	bool TextnTagsAPI::ItemIsCollectable(RE::TESForm* a_form) 
	{
		return serial::CheckIsCollectable(a_form->GetFormID());
	}
	RE::TESObjectREFR* my{};

	bool TextnTagsAPI::ItemIsCollected(RE::FormID a_formID) { 
		return CFramework_Master::FoundItemData.HasForm(a_formID) || CFramework_Master::FoundItemData_NoShow.HasForm(a_formID);
	}

	bool TextnTagsAPI::ItemIsCollected(RE::TESForm* a_form) 
	{ 
		return CFramework_Master::FoundItemData.HasForm(a_form->GetFormID()) || CFramework_Master::FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}
}