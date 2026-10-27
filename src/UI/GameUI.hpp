#pragma once

#include "UI/Core/Toasts.hpp"

namespace CompletionistUI
{
	class GameUI
	{
	public:
		static constexpr std::string_view menuName = "CompletionistMenu";

		static void Install();

		static void Open();
		static void Close();
		static void SetOpen(bool a_open);
		static void OnGameLoaded();

		[[nodiscard]] static bool IsOpen() noexcept;
		[[nodiscard]] static bool CanOpen();
		[[nodiscard]] static bool OverlayAllowed();

		[[nodiscard]] static bool NotificationsEnabled() noexcept;
		static void               Notify(std::string a_label, std::string a_text, ToastKind a_kind, std::optional<ImVec4> a_colour = std::nullopt);

	private:
		static void OnPresent(std::uint32_t a_arg);

		static inline REL::Relocation<decltype(OnPresent)> _OnPresent;
	};
}
