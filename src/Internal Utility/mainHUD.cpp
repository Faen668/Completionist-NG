#include "Frameworks/FrameworkMaster.hpp"
#include "Events.hpp"
#include "Serialization.hpp"
#include "Variables.hpp"
#include "mainHUD.hpp"
#include "MuseumAPI.hpp"

#undef GetModuleHandle

namespace Completionist_MainHUD 
{
	using namespace CVariables;

	using events = CEvents::EventHandler;
	using serial = Serialization::CompletionistData;
	using museum = Completionist::MuseumAPI;

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
	};

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

		return API_GetDisplayNameMerged(_OnUpdateInventoryText(a_this), ItemIsCollected(a_this->object), a_this->object);
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

		return API_GetDisplayNameMerged(a_this->GetName(), ItemIsCollected(a_this), a_this);

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

	const char* TextnTagsAPI::API_GetDisplayNamePrefix(RE::InventoryEntryData* a_object, bool a_collected, bool a_displayed, bool a_variationDisplayed) 
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

		auto displayName = a_object->GetDisplayName();
		if (CVariables::V_MuseumModeEnabled && museum::IsMuseumDisplayable(a_object->GetObject()))
		{
			const std::string displayable_prefix = GetPrefix(V_PrefixChoice_Displayable);
			const std::string displayed_prefix = GetPrefix(V_PrefixChoice_Displayed);
			const std::string occupied_prefix = GetPrefix(V_PrefixChoice_Occupied);

			if (a_displayed) {
				return formattedStringHolder.emplace_back(applyPrefix(displayed_prefix, displayName, V_TextChoice_Displayed)).c_str();
			}

			if (a_variationDisplayed) {
				return formattedStringHolder.emplace_back(applyPrefix(occupied_prefix, displayName, V_TextChoice_Occupied)).c_str();
			}
			else {
				return formattedStringHolder.emplace_back(applyPrefix(displayable_prefix, displayName, V_TextChoice_Displayable)).c_str();
			}
		}
		else
		{
			const std::string g_prefix = GetPrefix(V_PrefixChoice_G);
			const std::string n_prefix = GetPrefix(V_PrefixChoice_N);

			if (a_collected) {
				return formattedStringHolder.emplace_back(applyPrefix(g_prefix, displayName, V_TextChoice_G)).c_str();
			}
			else {
				return formattedStringHolder.emplace_back(applyPrefix(n_prefix, displayName, V_TextChoice_N)).c_str();
			}
		}
	}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	const char* TextnTagsAPI::API_GetDisplayNameMerged(const char* a_this, bool a_collected, RE::TESForm* a_form) 
	{
		int32_t newColour, foundColour, occupiedColour;
		bool isMuseumDisplayable = false;

		if (CVariables::V_MuseumModeEnabled && museum::IsMuseumDisplayable(a_form)) {
			newColour = V_HUD_Override_Enabled_Displayable ? V_HUD_CustomColour_Displayable : V_HUD_Colour_Displayable;
			foundColour = V_HUD_Override_Enabled_Displayed ? V_HUD_CustomColour_Displayed : V_HUD_Colour_Displayed;
			occupiedColour = V_HUD_Override_Enabled_Occupied ? V_HUD_CustomColour_Occupied : V_HUD_Colour_Occupied;
			isMuseumDisplayable = true;
		}
		else {
			newColour = V_HUD_Override_Enabled_New_Menus ? V_HUD_CustomColour_New_Menus : V_HUD_Colour_New_Menus;
			foundColour = V_HUD_Override_Enabled_Found_Menus ? V_HUD_CustomColour_Found_Menus : V_HUD_Colour_Found_Menus;
		}

		const std::string g_prefix = GetPrefix(V_PrefixChoice_G);
		const std::string n_prefix = GetPrefix(V_PrefixChoice_N);
		const std::string displayable_prefix = GetPrefix(V_PrefixChoice_Displayable);
		const std::string displayed_prefix = GetPrefix(V_PrefixChoice_Displayed);
		const std::string occupied_prefix = GetPrefix(V_PrefixChoice_Occupied);

		auto applyFormat = [](const std::string& prefix, const char* str, int choice, int color) -> std::string {
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

		if (isMuseumDisplayable) {
			bool isVariationDisplayedInstead = false;
			if (museum::IsDisplayed(a_form, isVariationDisplayedInstead) || (isVariationDisplayedInstead && V_TreatOccupiedAsDisplayed)) {
				return formattedStringHolder.emplace_back(applyFormat(displayed_prefix, a_this, V_TextChoice_Displayed, foundColour)).c_str();
			}

			if (isVariationDisplayedInstead) {
				return formattedStringHolder.emplace_back(applyFormat(occupied_prefix, a_this, V_TextChoice_Occupied, occupiedColour)).c_str();
			}

			return formattedStringHolder.emplace_back(applyFormat(displayable_prefix, a_this, V_TextChoice_Displayable, newColour)).c_str();
		}
		else {
			if (a_collected) {
				return formattedStringHolder.emplace_back(applyFormat(g_prefix, a_this, V_TextChoice_G, foundColour)).c_str();
			}
			else {
				return formattedStringHolder.emplace_back(applyFormat(n_prefix, a_this, V_TextChoice_N, newColour)).c_str();
			}
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
		if (!CurrentRef || !CurrentRef->target) return;

		auto CurrentObj = CurrentRef->target.get();
		RE::TESForm* Base = CurrentObj->GetBaseObject();
		if (!Base || !ItemIsCollectable(Base)) return;

		const auto formatTag = [](const RE::BSString& originalText, const std::string& color, const std::string& tag) -> std::string {
			return fmt::format("{:s} <font color = '{:s}'>{:s} </font>"sv, originalText.c_str(), color, tag);
			};

		std::string newColour, foundColour, occupiedColour;
		bool isMuseumDisplayable = CVariables::V_MuseumModeEnabled && museum::IsMuseumDisplayable(Base);

		if (isMuseumDisplayable) {
			newColour = V_HUD_Override_Enabled_Displayable ? V_HUD_CustomColourString_Displayable : V_HUD_ColourString_Displayable;
			foundColour = V_HUD_Override_Enabled_Displayed ? V_HUD_CustomColourString_Displayed : V_HUD_ColourString_Displayed;
			occupiedColour = V_HUD_Override_Enabled_Occupied ? V_HUD_CustomColourString_Occupied : V_HUD_ColourString_Occupied;

			bool isVariationDisplayedInstead = false;
			bool isDisplayed = museum::IsDisplayed(Base, isVariationDisplayedInstead);

			if (isVariationDisplayedInstead && V_TreatOccupiedAsDisplayed)
				isDisplayed = true;

			// Skip if appropriate tag is disabled ("...")
			if ((isDisplayed && V_CrosshairTag_Displayed == "...") ||
				(isVariationDisplayedInstead && V_CrosshairTag_Occupied == "...") ||
				(!isVariationDisplayedInstead && !isDisplayed && V_CrosshairTag_Displayable == "...")) {
				return;
			}

			data->text = isDisplayed ?
				formatTag(data->text, foundColour, V_CrosshairTag_Displayed) :
				isVariationDisplayedInstead ?
				formatTag(data->text, occupiedColour, V_CrosshairTag_Occupied) :
				formatTag(data->text, newColour, V_CrosshairTag_Displayable);
		}
		else {
			newColour = V_HUD_Override_Enabled_New_Crosshair ? V_HUD_CustomColourString_New_Crosshair : V_HUD_ColourString_New_Crosshair;
			foundColour = V_HUD_Override_Enabled_Found_Crosshair ? V_HUD_CustomColourString_Found_Crosshair : V_HUD_ColourString_Found_Crosshair;

			bool isCollected = ItemIsCollected(Base);

			// Skip if appropriate tag is disabled ("...")
			if ((isCollected && V_CrosshairTag_Found == "...") ||
				(!isCollected && V_CrosshairTag_New == "...")) {
				return;
			}

			data->text = isCollected ?
				formatTag(data->text, foundColour, V_CrosshairTag_Found) :
				formatTag(data->text, newColour, V_CrosshairTag_New);
		}
	}

	//---------------------------------------------------
	//-- Collectability Functions -----------------------
	//---------------------------------------------------

	bool TextnTagsAPI::ItemIsCollectable(RE::TESForm* a_form) 
	{
		return serial::CheckIsCollectable(a_form->GetFormID());
	}

	bool TextnTagsAPI::ItemIsCollected(RE::TESForm* a_form) 
	{ 
		return CFramework_Master::FoundItemData.HasForm(a_form->GetFormID()) || CFramework_Master::FoundItemData_NoShow.HasForm(a_form->GetFormID());
	}
}