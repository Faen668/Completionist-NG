#pragma once

#include <span>
#include <string_view>

namespace CompletionistUI
{
	// Icon data from svg_to_icons.py, op: 0 moveTo, 1 lineTo, 2 curveTo
	struct IconCommand
	{
		int   op;
		float a, b, c, d;
	};

	// The first shape takes an icon's primary colour, the second its secondary
	struct IconShape
	{
		int   fill;
		int   stroke;
		float width;
		bool  strokeFirst;
		int   begin;
		int   count;
	};

#include "UI/Core/ItemIcons.inl"

	enum class IconGroup
	{
		kNeeded,
		kFound,
		kMuseum,
	};

	struct ItemIcon
	{
		std::string_view             name;
		IconGroup                    group;
		std::span<const IconShape>   shapes;
		std::span<const IconCommand> commands;
	};

	// In the order of CVariables::V_ItemIconColours
	inline constexpr ItemIcon itemIcons[] = {
		{ "cmpNew", IconGroup::kNeeded, TrophyPlusShapes, TrophyPlusCommands },
		{ "cmpFound", IconGroup::kFound, TrophyTickShapes, TrophyTickCommands },
		{ "cmpDisplayable", IconGroup::kMuseum, MuseumPlusShapes, MuseumPlusCommands },
		{ "cmpDisplayed", IconGroup::kMuseum, MuseumTickShapes, MuseumTickCommands },
		{ "cmpOccupied", IconGroup::kMuseum, MuseumDiamondShapes, MuseumDiamondCommands },
	};
}
