#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "Variables.hpp"
#include "mainHUD.hpp"

inline std::vector<std::string> garbageDump{};

namespace Completionist_MainHUD {
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	struct moreHUDmessage {

		RE::FormID m_formID;
		bool m_icontype; // false = New, true = Found
		bool m_display;
	};

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

		if (a_event->opening) { VariablesAPI::Update(); }
		else{ garbageDump.clear();}

		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Object Processing For Inventory Items ----------
	//---------------------------------------------------

	const char* TextnTagsAPI::OnUpdateInventoryText(RE::InventoryEntryData* a_this) {

	auto baseform = a_this->object;
	if (!baseform || !baseform->GetName() || !ItemIsCollectable(baseform)) { return _OnUpdateInventoryText(a_this); }

	bool ShouldDisplay = false;
	bool PrevCollected = ItemIsCollected(baseform);

	switch (baseform->GetFormType())
	{

	case RE::FormType::AlchemyItem:
		ShouldDisplay = (PrevCollected && V_Alchemy_Enabled_Found) || (!PrevCollected && V_Alchemy_Enabled_New); break;

	case RE::FormType::Ingredient:
		ShouldDisplay = (PrevCollected && V_Alchemy_Enabled_Found) || (!PrevCollected && V_Alchemy_Enabled_New); break;

	case RE::FormType::Ammo:
		ShouldDisplay = (PrevCollected && V_Ammo_Enabled_Found) || (!PrevCollected && V_Ammo_Enabled_New); break;

	case RE::FormType::Armor:
		ShouldDisplay = (PrevCollected && V_Armor_Enabled_Found) || (!PrevCollected && V_Armor_Enabled_New); break;

	case RE::FormType::Book:
		ShouldDisplay = (PrevCollected && V_Books_Enabled_Found) || (!PrevCollected && V_Books_Enabled_New); break;

	case RE::FormType::Note:
		ShouldDisplay = (PrevCollected && V_Books_Enabled_Found) || (!PrevCollected && V_Books_Enabled_New); break;

	case RE::FormType::Weapon:
		ShouldDisplay = (PrevCollected && V_Weapons_Enabled_Found) || (!PrevCollected && V_Weapons_Enabled_New); break;

	default:
		ShouldDisplay = (PrevCollected && V_Other_Enabled_Found) || (!PrevCollected && V_Other_Enabled_New); break;
	}

	//SKSE Message Here

	if (!ShouldDisplay) { return _OnUpdateInventoryText(a_this); }


	if (V_mainHudEnabled) {
		return OnUpdateInventoryName(_OnUpdateInventoryText(a_this), PrevCollected);
	}
	return _OnUpdateInventoryText(a_this);
}

	//---------------------------------------------------
	//-- Name Processing For Inventory Items ------------
	//---------------------------------------------------

	const char* TextnTagsAPI::OnUpdateInventoryName(const char* a_this, bool a_collected) {

		if (a_collected) {
			return garbageDump.emplace_back(fmt::format("{:s}CompTag{:s}"sv, a_this, std::to_string(V_HUD_Colour_Found))).c_str();
		}

		switch (V_TextChoice) {

		case 0: //Append
			return garbageDump.emplace_back(fmt::format("{:s} ***CompTag{:s}"sv, a_this, std::to_string(V_HUD_Colour_New))).c_str();

		case 1: //Prepend
			return garbageDump.emplace_back(fmt::format("*** {:s}CompTag{:s}"sv, a_this, std::to_string(V_HUD_Colour_New))).c_str();

		case 2: //Wrap
			return garbageDump.emplace_back(fmt::format("*** {:s} ***CompTag{:s}"sv, a_this, std::to_string(V_HUD_Colour_New))).c_str();

		default: //None
			return garbageDump.emplace_back(fmt::format("{:s}CompTag{:s}"sv, a_this, std::to_string(V_HUD_Colour_New))).c_str();
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
		if (!Base || !ItemIsCollectable(Base)) { return; }


		bool ShouldDisplay = false;
		bool PrevCollected = ItemIsCollected(Base);

		switch (Base->GetFormType())
		{
		case RE::FormType::AlchemyItem:
			ShouldDisplay = (PrevCollected && V_Alchemy_Enabled_Found) || (!PrevCollected && V_Alchemy_Enabled_New); break;

		case RE::FormType::Ingredient:
			ShouldDisplay = (PrevCollected && V_Alchemy_Enabled_Found) || (!PrevCollected && V_Alchemy_Enabled_New); break;

		case RE::FormType::Ammo:
			ShouldDisplay = (PrevCollected && V_Ammo_Enabled_Found) || (!PrevCollected && V_Ammo_Enabled_New); break;

		case RE::FormType::Armor:
			ShouldDisplay = (PrevCollected && V_Armor_Enabled_Found) || (!PrevCollected && V_Armor_Enabled_New); break;

		case RE::FormType::Book:
			ShouldDisplay = (PrevCollected && V_Books_Enabled_Found) || (!PrevCollected && V_Books_Enabled_New); break;

		case RE::FormType::Note:
			ShouldDisplay = (PrevCollected && V_Books_Enabled_Found) || (!PrevCollected && V_Books_Enabled_New); break;

		case RE::FormType::Weapon: 
			ShouldDisplay = (PrevCollected && V_Weapons_Enabled_Found) || (!PrevCollected && V_Weapons_Enabled_New); break;

		default:
			ShouldDisplay = (PrevCollected && V_Other_Enabled_Found) || (!PrevCollected && V_Other_Enabled_New); break;
		}

		if (SKSEMessaging) {
			moreHUDmessage msg{ Base->GetFormID(), PrevCollected, (ShouldDisplay && V_moreHudEnabled) };
			SKSEMessaging->Dispatch(1, &msg, sizeof(msg), "Ahzaab's moreHUD Plugin");
		}

		if (ShouldDisplay && V_mainHudEnabled) {
			data->text = PrevCollected ? 
				fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, V_HUD_ColourString_Found, V_CrosshairTag_Found) :
				fmt::format("{:s} <font color = '{:s}'>{:s} < / font>"sv, data->text, V_HUD_ColourString_New, V_CrosshairTag_New);
		}
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