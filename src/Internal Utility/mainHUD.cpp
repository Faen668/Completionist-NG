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

	//---------------------------------------------------
	//-- Install Hooks for Main HUD ---------------------
	//---------------------------------------------------

	void TextnTagsAPI::Register() 
	{
		// Item names are no longer rewritten, UI/ItemMarks does icons instead
		_OnUpdateCrosshairText = events::RegisterUpdateCrosshairHook(&OnUpdateCrosshairText);
	};

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
		// Covers telekinesis targets too
		auto CurrentObj = data->crosshairRef.get();
		if (!CurrentObj)
			return;

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
		return a_form && serial::CheckIsCollectable(a_form->GetFormID());
	}

	bool TextnTagsAPI::ItemIsCollected(RE::TESForm* a_form)
	{
		return a_form && (CFramework_Master::FoundItemData.HasForm(a_form->GetFormID()) || CFramework_Master::FoundItemData_NoShow.HasForm(a_form->GetFormID()));
	}
}