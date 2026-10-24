#pragma once
#undef GetObject

namespace Completionist_MainHUD
{
	class TextnTagsAPI 
	{
	public:
		static TextnTagsAPI* GetSingleton() { static TextnTagsAPI singleton; return &singleton; }

		static void Register();

		static void	ProcessCrosshairReference(RE::HUDData* data);

		static bool ItemIsCollectable(RE::TESForm* a_form);
		static bool ItemIsCollected(RE::TESForm* a_form);

		static void OnUpdateCrosshairText(RE::UIMessageQueue* a_this, const RE::BSFixedString& a_menuName, RE::UI_MESSAGE_TYPE a_type, RE::IUIMessageData* a_data);

		TextnTagsAPI(TextnTagsAPI const&) = delete;
		TextnTagsAPI(TextnTagsAPI const&&) = delete;
		TextnTagsAPI operator=(TextnTagsAPI&) = delete;
		TextnTagsAPI operator=(TextnTagsAPI&&) = delete;

	private:
		TextnTagsAPI() = default;
		~TextnTagsAPI() = default;

		static inline REL::Relocation<decltype(OnUpdateCrosshairText)> _OnUpdateCrosshairText;
	};
}
