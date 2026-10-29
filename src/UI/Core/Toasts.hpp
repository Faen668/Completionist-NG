#pragma once

#include <imgui.h>

#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>
#include <string>

namespace CompletionistUI
{
	enum class ToastKind : std::uint8_t
	{
		kEntry,
		kDiscovery,
		kCell,
		kInfo,
	};

	struct ToastOptions
	{
		int   visible{ 5 };	// max shown at once
		float seconds{ 4.0f };
		bool  newestFirst{ true };
		float opacity{ 0.85f };
		// Position, 0 to 1 across and down
		float x{ 0.0f };
		float y{ 0.0f };
		bool  preview{ false };	// placing it on screen
		// Joined to the Cell Scanner panel: drawn at dockAt (its bottom left when dockUp), dockWidth wide
		bool   docked{ false };
		ImVec2 dockAt{};
		float  dockWidth{ 0.0f };
		bool   dockUp{ false };
	};

	struct ToastBounds
	{
		ImVec2 min{};
		ImVec2 max{};
	};

	// Completionist notifications drawn by the DLL
	class Toasts
	{
	public:
		void Push(std::string a_label, std::string a_text, ToastKind a_kind, std::optional<ImVec4> a_colour = std::nullopt);

		ToastBounds Draw(double a_now, const ToastOptions& a_options, const std::string& a_moreText);

		[[nodiscard]] static ImVec2 Travel(ImVec2 a_size);
		[[nodiscard]] static float  Width();
		// Top left of a free stack this size at a 0 to 1 position, and back
		[[nodiscard]] static ImVec2 Origin(ImVec2 a_position, ImVec2 a_size);
		[[nodiscard]] static ImVec2 Position(ImVec2 a_origin, ImVec2 a_size);
		// The stack as it was last drawn, for the panel to make room
		[[nodiscard]] ImVec2 StackSize(bool a_more) const;

		[[nodiscard]] bool   Active() const;
		[[nodiscard]] size_t Waiting() const;
		void                 Clear();

	private:
		struct Toast
		{
			std::string           label;
			std::string           text;
			ToastKind             kind{ ToastKind::kInfo };
			std::optional<ImVec4> colour;
			int                   count{ 1 };
			double                entered{ -1.0 };
			double                shown{ -1.0 };
			double                hold{ 0.0 };
			double                bumped{ -1.0 };
			double                leaving{ -1.0 };
			float                 y{ 0.0f };
			bool                  placed{ false };
			bool                  repeated{ false };
		};

		void Advance(double a_now, const ToastOptions& a_options);

		mutable std::mutex _lock;
		std::deque<Toast>  _shown;
		std::deque<Toast>  _waiting;
		double             _lastLeave{ -1.0e9 };
		double             _lastDraw{ -1.0 };
	};
}
