#pragma once
#undef GetObject

namespace Completionist_MainHUD
{
	class TextnTagsAPI 
	{
	public:
		static TextnTagsAPI* GetSingleton() { static TextnTagsAPI singleton; return &singleton; }

		static void Register();

		static void OnMenuOpenCloseEvent(RE::MenuOpenCloseEvent const* a_event);

		static void	ProcessCrosshairReference(RE::HUDData* data);
		static std::string GetPrefix(int32_t a_variable);

		static bool ItemIsCollectable(RE::FormID a_formID);
		static bool ItemIsCollectable(RE::TESForm* a_form);

		static bool ItemIsCollected(RE::FormID a_formID);
		static bool ItemIsCollected(RE::TESForm* a_form);

		static void OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data);
		static const char* OnUpdateInventoryText(RE::InventoryEntryData* a_this);
		static const char* OnUpdateCraftingText(RE::TESForm* a_this);

		static const char*	API_GetDisplayNamePrefix(const char* a_this, bool a_collected, bool a_displayed = false, bool a_variationDisplayed = false);
		static const char*  API_GetDisplayNameMerged(const char* a_this, bool a_collected, RE::TESForm* a_form);

		TextnTagsAPI(TextnTagsAPI const&) = delete;
		TextnTagsAPI(TextnTagsAPI const&&) = delete;
		TextnTagsAPI operator=(TextnTagsAPI&) = delete;
		TextnTagsAPI operator=(TextnTagsAPI&&) = delete;

	private:
		TextnTagsAPI() = default;
		~TextnTagsAPI() = default;

		static inline REL::Relocation<decltype(OnUpdateCrosshairText)> _OnUpdateCrosshairText;
		static inline REL::Relocation<decltype(OnUpdateInventoryText)> _OnUpdateInventoryText;
	};
}
