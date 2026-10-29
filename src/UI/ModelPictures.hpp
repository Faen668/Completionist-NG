#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace CompletionistUI
{
	// A picture of an item's own model, as the game loads it with the player's mods: the model
	// is loaded on the game thread, then drawn once on the render thread into a small texture
	// with our own shader, and kept on disk so it's only ever drawn once
	class ModelPictures
	{
	public:
		// Render thread. The texture, or nothing while it's still loading or can't be drawn
		[[nodiscard]] static void* Get(std::uint32_t a_formID);

		// Render thread, every frame the window's open: preparing and tidying up
		static void Tick();

		// Every item that has a picture, from the pages as they're built
		static void SetItems(std::vector<std::uint32_t> a_formIDs);

		// Draw every picture not kept yet, or all of them again
		static void PrepareAll(bool a_redraw);

		// Prepared so far and how many in all; 0 of 0 when nothing's being prepared
		[[nodiscard]] static std::pair<int, int> Progress();

		// Game thread: how many items have no picture kept yet
		[[nodiscard]] static std::size_t Missing();
	};
}
