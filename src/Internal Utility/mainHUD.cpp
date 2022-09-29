#include "Frameworks/FrameworkMaster.hpp"
#include "Serialization.hpp"
#include "Variables.hpp"
#include "mainHUD.hpp"

inline RE::FormID a_form;
inline RE::TESForm* a_base;

inline std::string replacename;
inline std::vector<std::string*> garbageDump{};

namespace Completionist_MainHUD {
	using namespace CFramework_Master;
	using namespace Serialization;
	using namespace CVariables;

	//---------------------------------------------------
	//-- Crosshair Hook For HUD Tagging -----------------
	//---------------------------------------------------

	void FunctionHolder::InstallHook() {

		auto& trampoline = SKSE::GetTrampoline();
		_OnUpdateCrosshairText = trampoline.write_call<5>(RELOCATION_ID(39535, 40621).address() + REL::Relocate(0x289, 0x280), OnUpdateCrosshairText);
		_OnUpdateInventoryText = trampoline.write_branch<5>(RELOCATION_ID(50926, 51803).address() + REL::Relocate(0x4, 0x4), OnUpdateInventoryText);

		auto UserInterface = RE::UI::GetSingleton();
		UserInterface->AddEventSink(static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(FunctionHolder::GetSingleton()));
	}

	//---------------------------------------------------
	//-- Events ( Clear Garbage On Menu Close ) ---------
	//---------------------------------------------------

	EventResult FunctionHolder::ProcessEvent(RE::MenuOpenCloseEvent const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) {

		if (!a_event || a_event->opening) { return RE::BSEventNotifyControl::kContinue; }

		garbageDump.clear();
		return EventResult::kContinue;
	}

	//---------------------------------------------------
	//-- Inventory Hook For Name Tagging ----------------
	//---------------------------------------------------

	const char* FunctionHolder::OnUpdateInventoryText(RE::InventoryEntryData* a_this) {

		auto baseform = a_this->object;
		if (!baseform || ItemIsCollected(baseform)) { return _OnUpdateInventoryText(a_this); }

		auto basename = baseform->GetName();
		if (!basename) { return _OnUpdateInventoryText(a_this); }

		if (ItemIsCollectable(baseform->GetFormID())) {

			std::string* myString = new std::string(_OnUpdateInventoryText(a_this));

			switch (V_TextChoice) {
			
			case 0: {

				myString->append(std::string(" ***"));
				garbageDump.push_back(myString);
				return myString->c_str();
			}

			case 1: {

				std::string* str = new std::string("*** ");
				str->append(myString->c_str());
				garbageDump.push_back(str);
				return str->c_str();
			}

			case 2: {

				std::string* pr = new std::string("*** ");
				std::string* ap = new std::string(" ***");

				myString->append(ap->c_str());
				pr->append(myString->c_str());

				garbageDump.push_back(pr);
				return pr->c_str();
			}

			default:
				return _OnUpdateInventoryText(a_this);
			}
		}
		return _OnUpdateInventoryText(a_this);
	}

	//---------------------------------------------------
	//-- Crosshair Hook For HUD Tagging -----------------
	//---------------------------------------------------

	void FunctionHolder::OnUpdateCrosshairText(RE::UIMessageQueue * a_this, const RE::BSFixedString & a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData * a_data) {

		_OnUpdateCrosshairText(a_this, a_menuName, a_type, a_data);

		const auto data = a_data ? static_cast<RE::HUDData*>(a_data) : nullptr;
		const auto crossHairRef = data ? data->crossHairRef.get() : RE::TESObjectREFRPtr();

		if (!data) { return; }
		ProcessCrosshairReference(data);
	}

	//---------------------------------------------------
	//-- Crosshair Hook For HUD Tagging -----------------
	//---------------------------------------------------

	void FunctionHolder::ProcessCrosshairReference(RE::HUDData* data) {

		if (auto CurrentRef = RE::CrosshairPickData::GetSingleton(); CurrentRef) {
			if (auto CurrentObj = CurrentRef->target.get(); CurrentObj) {
				a_base = CurrentObj.get()->GetBaseObject();
				a_form = a_base->GetFormID();
			}
		}

		if (!a_base || !a_form) { return; }

		replacename = data->text.c_str();

		switch (a_base->GetFormType()) {
		case RE::FormType::AlchemyItem:
		{
			if (ItemIsCollected(a_form) && V_Alchemy_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Alchemy_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Alchemy_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Alchemy_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Ammo:
		{
			if (ItemIsCollected(a_form) && V_Ammo_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Ammo_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Ammo_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Ammo_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Armor:
		{
			if (ItemIsCollected(a_form) && V_Armor_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Armor_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Armor_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Armor_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Book:
		{
			if (ItemIsCollected(a_form) && V_Books_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Books_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Books_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Books_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Note:
		{
			if (ItemIsCollected(a_form) && V_Books_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Books_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Books_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Books_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Ingredient:
		{
			if (ItemIsCollected(a_form) && V_Alchemy_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Alchemy_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Alchemy_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Alchemy_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		case RE::FormType::Weapon:
		{
			if (ItemIsCollected(a_form) && V_Weapons_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Weapons_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Weapons_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Weapons_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
		default:

			if (ItemIsCollected(a_form) && V_Other_Enabled_Found) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Other_ColourString_Found, V_CrosshairTag_Found);
				return;
			}
			else if (ItemIsCollectable(a_form) && V_Other_Enabled_New) {
				data->text = fmt::format("{:s} <font color='{:s}'>{:s}</font>"sv, replacename, V_Other_ColourString_New, V_CrosshairTag_New);
				return;
			}
			return;
		}
	}

	bool FunctionHolder::ItemIsCollectable(RE::FormID a_formID) {
		return CompletionistData::CheckIsCollectable(a_formID) && !FoundItemData.HasForm(a_formID) && !FoundItemData_NoShow.HasForm(a_formID);
	}

	bool FunctionHolder::ItemIsCollected(RE::FormID a_formID) { return FoundItemData.HasForm(a_formID); }
	bool FunctionHolder::ItemIsCollected(RE::TESForm* a_form) { return FoundItemData.HasForm(a_form->GetFormID()); }
}