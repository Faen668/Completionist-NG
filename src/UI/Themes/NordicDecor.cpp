#include "UI/Themes/NordicDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>
#include <utility>

namespace CompletionistUI::NordicDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Silver on black, the nebula's teal
		constexpr ImU32 silver = IM_COL32(196, 194, 188, 255);
		constexpr ImU32 silverDim = IM_COL32(112, 110, 106, 255);
		constexpr ImU32 teal = IM_COL32(70, 150, 160, 255);
		constexpr ImU32 starlight = IM_COL32(236, 240, 255, 255);
		constexpr ImU32 black = IM_COL32(0, 0, 0, 255);

		//---------------------------------------------------
		//-- Nordic Decor ( Alpha Of ) ----------------------
		//---------------------------------------------------

		float AlphaOf(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Rope ) --------------------------
		//---------------------------------------------------

		// Braided rope along a straight edge: slanted twists, each lit on one side and cut dark where the next lies over it
		void Rope(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, float a_thick, ImU32 a_colour) {

			const float dx = a_to.x - a_from.x;
			const float dy = a_to.y - a_from.y;
			const float length = std::hypot(dx, dy);
			if (length < a_thick * 2.0f) {
				return;
			}
			const ImVec2 along(dx / length, dy / length);
			const ImVec2 side(-along.y, along.x);
			const float  pitch = a_thick * 0.9f;
			const float  half = a_thick * 0.5f;
			const float  stroke = std::max(1.0f, a_thick * 0.22f);
			const ImU32  dark = WithAlpha(black, AlphaOf(a_colour));
			for (float t = 0.0f; t + pitch <= length; t += pitch) {
				const float  fade = std::pow(std::sin(std::clamp((t + pitch * 0.5f) / length, 0.0f, 1.0f) * pi), 0.15f);
				const ImVec2 a(a_from.x + along.x * t - side.x * half, a_from.y + along.y * t - side.y * half);
				const ImVec2 b(a_from.x + along.x * (t + pitch * 1.4f) + side.x * half, a_from.y + along.y * (t + pitch * 1.4f) + side.y * half);
				const ImVec2 ctrl(a_from.x + along.x * (t + pitch * 0.2f) + side.x * half * 0.6f, a_from.y + along.y * (t + pitch * 0.2f) + side.y * half * 0.6f);
				a_drawList->AddBezierQuadratic(a, ctrl, b, WithAlpha(dark, fade), stroke + 2.0f, 8);
				a_drawList->AddBezierQuadratic(a, ctrl, b, WithAlpha(a_colour, fade), stroke, 8);
			}
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Chain ) -------------------------
		//---------------------------------------------------

		// A chain of oval links along a horizontal edge, alternate links seen edge-on
		void Chain(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, float a_thick, ImU32 a_colour) {

			const float length = a_to.x - a_from.x;
			if (length < a_thick * 4.0f) {
				return;
			}
			const float link = a_thick * 2.4f;
			const float stroke = std::max(1.0f, a_thick * 0.2f);
			const int   count = std::max(1, static_cast<int>(length / (link * 0.8f)));
			const float step = length / count;
			for (int i = 0; i < count; ++i) {
				const float  x = a_from.x + step * (i + 0.5f);
				const float  fade = std::pow(std::sin((x - a_from.x) / length * pi), 0.15f);
				const ImU32  colour = WithAlpha(a_colour, fade);
				if (i % 2 == 0) {
					const ImVec2 min(x - link * 0.5f, a_from.y - a_thick * 0.45f);
					const ImVec2 max(x + link * 0.5f, a_from.y + a_thick * 0.45f);
					a_drawList->AddRect(min, max, WithAlpha(black, fade * AlphaOf(a_colour)), a_thick * 0.45f, 0, stroke + 2.0f);
					a_drawList->AddRect(min, max, colour, a_thick * 0.45f, 0, stroke);
				} else {
					a_drawList->AddLine(ImVec2(x - link * 0.55f, a_from.y), ImVec2(x + link * 0.55f, a_from.y), WithAlpha(black, fade * AlphaOf(a_colour)), stroke * 2.0f + 2.0f);
					a_drawList->AddLine(ImVec2(x - link * 0.55f, a_from.y), ImVec2(x + link * 0.55f, a_from.y), colour, stroke * 1.8f);
				}
			}
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Dragon Head ) -------------------
		//---------------------------------------------------

		// A carved dragon's head in silver line-work, its neck knotted, facing along a_facing (-1 left, 1 right)
		void DragonHead(ImDrawList* a_drawList, ImVec2 a_nose, float a_size, float a_facing, ImU32 a_colour) {

			const auto  at = [&](float a_x, float a_y) { return ImVec2(a_nose.x + a_x * a_size * a_facing, a_nose.y + a_y * a_size); };
			const float stroke = std::max(1.0f, a_size * 0.07f);
			const ImU32 dark = WithAlpha(black, AlphaOf(a_colour));
			// Snout, brow and the open jaw
			const ImVec2 upper[] = { at(0.0f, -0.05f), at(-0.35f, -0.28f), at(-0.75f, -0.32f), at(-1.05f, -0.48f), at(-1.35f, -0.36f) };
			const ImVec2 lower[] = { at(-0.12f, 0.12f), at(-0.5f, 0.06f), at(-0.8f, 0.22f), at(-1.2f, 0.26f) };
			for (const auto& [colour, thick] : { std::pair{ dark, stroke + 2.0f }, std::pair{ a_colour, stroke } }) {
				a_drawList->AddPolyline(upper, 5, colour, ImDrawFlags_None, thick);
				a_drawList->AddPolyline(lower, 4, colour, ImDrawFlags_None, thick);
				// The neck curling back into a knot
				a_drawList->AddBezierCubic(at(-1.35f, -0.36f), at(-1.9f, -0.2f), at(-1.9f, 0.5f), at(-1.3f, 0.3f), colour, thick, 16);
				a_drawList->AddBezierCubic(at(-1.2f, 0.26f), at(-1.55f, -0.55f), at(-2.2f, -0.35f), at(-2.15f, 0.05f), colour, thick, 16);
				a_drawList->AddBezierCubic(at(-2.15f, 0.05f), at(-2.1f, 0.4f), at(-2.6f, 0.25f), at(-2.9f, 0.05f), colour, thick, 16);
				// Horn swept back
				a_drawList->AddBezierQuadratic(at(-0.95f, -0.42f), at(-1.3f, -0.8f), at(-1.75f, -0.72f), colour, thick, 10);
			}
			a_drawList->AddCircleFilled(at(-0.7f, -0.18f), std::max(1.0f, a_size * 0.06f), a_colour, 8);
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Star ) --------------------------
		//---------------------------------------------------

		// A four-pointed star with a soft halo
		void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, float a_glow) {

			if (a_glow > 0.0f) {
				SoftSpot(a_drawList, a_centre, a_radius * 2.6f, WithAlpha(a_colour, 0.03f * a_glow));
			}
			const float  thin = a_radius * 0.18f;
			const ImVec2 points[8] = {
				ImVec2(a_centre.x, a_centre.y - a_radius), ImVec2(a_centre.x + thin, a_centre.y - thin),
				ImVec2(a_centre.x + a_radius, a_centre.y), ImVec2(a_centre.x + thin, a_centre.y + thin),
				ImVec2(a_centre.x, a_centre.y + a_radius), ImVec2(a_centre.x - thin, a_centre.y + thin),
				ImVec2(a_centre.x - a_radius, a_centre.y), ImVec2(a_centre.x - thin, a_centre.y - thin),
			};
			a_drawList->AddConcavePolyFilled(points, 8, a_colour);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.22f, starlight, 10);
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Night Sky ) ---------------------
		//---------------------------------------------------

		// Behind the window: black, a faint teal nebula low on one side, stars scattered thinly
		void NightSky(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);
			Random random{ 0x4E55u };
			if (height > size * 20.0f) {
				for (int cloud = 0; cloud < 7; ++cloud) {
					const ImVec2 at(a_min.x + width * random.Range(0.55f, 1.0f), a_min.y + height * random.Range(0.55f, 1.0f));
					SoftSpot(a_drawList, at, size * random.Range(5.0f, 11.0f), WithAlpha(teal, 0.006f));
				}
			}
			const int stars = std::clamp(static_cast<int>(width * height / (size * size * 6.0f)), 20, 260);
			for (int i = 0; i < stars; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  bright = random.Next();
				const float  weight = 0.25f + 0.75f * EdgeWeight(at, a_min, a_max, std::min(width, height) * 0.3f);
				a_drawList->AddCircleFilled(at, 0.6f + bright * 1.0f, WithAlpha(bright > 0.85f ? IM_COL32(230, 200, 255, 255) : starlight, weight * (0.08f + bright * 0.22f)), 6);
			}
			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.1f, IM_COL32(0, 0, 0, 200));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Nordic Decor ( Corner Knot ) -------------------
		//---------------------------------------------------

		// Where rope meets chain: a small square knot
		void CornerKnot(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

			const float stroke = std::max(1.0f, a_size * 0.18f);
			a_drawList->AddRectFilled(ImVec2(a_centre.x - a_size, a_centre.y - a_size), ImVec2(a_centre.x + a_size, a_centre.y + a_size), black);
			for (const auto& [colour, thick] : { std::pair{ WithAlpha(black, AlphaOf(a_colour)), stroke + 2.0f }, std::pair{ a_colour, stroke } }) {
				a_drawList->AddCircle(ImVec2(a_centre.x - a_size * 0.35f, a_centre.y), a_size * 0.55f, colour, 12, thick);
				a_drawList->AddCircle(ImVec2(a_centre.x + a_size * 0.35f, a_centre.y), a_size * 0.55f, colour, 12, thick);
			}
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Diamond ) -----------------------
	//---------------------------------------------------

	// The objective bullet: a fine outlined diamond, a smaller one set inside when filled
	void Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const auto shape = [&](float a_r) {
			return std::array<ImVec2, 4>{ ImVec2(a_centre.x, a_centre.y - a_r), ImVec2(a_centre.x + a_r, a_centre.y), ImVec2(a_centre.x, a_centre.y + a_r), ImVec2(a_centre.x - a_r, a_centre.y) };
		};
		const auto outer = shape(a_radius);
		a_drawList->AddPolyline(outer.data(), 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.18f));
		if (a_filled) {
			const auto inner = shape(a_radius * 0.5f);
			a_drawList->AddConvexPolyFilled(inner.data(), 4, a_colour);
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// The diamond bullet, filling in gold and catching starlight as it's found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.3f;
		Diamond(a_drawList, a_centre, radius, a_colour, false);
		if (a_fill > 0.0f) {
			const float r = radius * 0.6f * a_fill;
			const ImVec2 inner[4] = { ImVec2(a_centre.x, a_centre.y - r), ImVec2(a_centre.x + r, a_centre.y), ImVec2(a_centre.x, a_centre.y + r), ImVec2(a_centre.x - r, a_centre.y) };
			a_drawList->AddConvexPolyFilled(inner, 4, a_colour);
			SoftSpot(a_drawList, a_centre, radius * 1.6f, WithAlpha(a_colour, 0.04f * a_fill));
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Shield ) ------------------------
	//---------------------------------------------------

	// The level badge: a pointed shield in pale gold, a second line inside
	void Shield(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const auto outline = [&](float a_r) {
			return std::array<ImVec2, 6>{
				ImVec2(a_centre.x - a_r * 0.8f, a_centre.y - a_r * 0.75f), ImVec2(a_centre.x, a_centre.y - a_r),
				ImVec2(a_centre.x + a_r * 0.8f, a_centre.y - a_r * 0.75f), ImVec2(a_centre.x + a_r * 0.8f, a_centre.y + a_r * 0.15f),
				ImVec2(a_centre.x, a_centre.y + a_r), ImVec2(a_centre.x - a_r * 0.8f, a_centre.y + a_r * 0.15f),
			};
		};
		const float stroke = std::max(1.0f, a_radius * 0.14f);
		const auto  outer = outline(a_radius);
		const auto  inner = outline(a_radius * 0.62f);
		a_drawList->AddConvexPolyFilled(outer.data(), 6, WithAlpha(black, 0.8f * AlphaOf(a_colour)));
		a_drawList->AddPolyline(outer.data(), 6, a_colour, ImDrawFlags_Closed, stroke);
		a_drawList->AddPolyline(inner.data(), 6, a_colour, ImDrawFlags_Closed, std::max(1.0f, stroke * 0.6f));
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Rule ) --------------------------
	//---------------------------------------------------

	// A fine silver line, fading out at both ends
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		if (a_to.x - a_from.x < 4.0f) {
			return;
		}
		Decor::FadedLine(a_drawList, a_from, a_to, WithAlpha(silver, 0.7f * AlphaOf(a_colour)), 1.0f);
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Heading Tail ) ------------------
	//---------------------------------------------------

	// After a heading: a dragon's head facing it, its neck running out into a fading line
	void HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize() * 0.5f;
		const ImU32 colour = WithAlpha(silver, 0.85f * AlphaOf(a_colour));
		if (a_to.x - a_from.x < size * 5.0f) {
			Decor::FadedLine(a_drawList, a_from, a_to, colour, 1.0f);
			return;
		}
		DragonHead(a_drawList, a_from, size, -1.0f, colour);
		const float start = a_from.x + size * 2.9f;
		const ImU32 clear = WithAlpha(colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(start, a_from.y + size * 0.05f - 0.5f), ImVec2(a_to.x, a_from.y + size * 0.05f + 0.5f), colour, clear, clear, colour);
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Divider ) -----------------------
	//---------------------------------------------------

	// Two fading lines meeting at the double peak under the skill menu's description
	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float  midX = (a_from.x + a_to.x) * 0.5f;
		const float  size = ImGui::GetFontSize() * 0.5f;
		const ImU32  colour = WithAlpha(silver, 0.75f);
		const float  y = a_from.y + size * 0.3f;
		Decor::FadedLine(a_drawList, ImVec2(a_from.x, y), ImVec2(midX - size * 1.4f, y), colour, 1.0f);
		Decor::FadedLine(a_drawList, ImVec2(midX + size * 1.4f, y), ImVec2(a_to.x, y), colour, 1.0f);
		for (const float lift : { 0.0f, size * 0.35f }) {
			const ImVec2 peak[5] = { ImVec2(midX - size * 2.0f, y), ImVec2(midX - size * 0.9f, y), ImVec2(midX, y - size * 0.9f + lift), ImVec2(midX + size * 0.9f, y), ImVec2(midX + size * 2.0f, y) };
			a_drawList->AddPolyline(peak, 5, lift == 0.0f ? colour : WithAlpha(colour, 0.6f), ImDrawFlags_None, std::max(1.0f, size * 0.12f));
		}
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Black under a thin night sky, braided rope down the sides, chain along top and bottom, a knot at each corner
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float inset = size * 0.35f;
		const float thick = std::max(4.0f, size * 0.32f);
		NightSky(a_drawList, a_min, a_max);

		const ImVec2 min(a_min.x + inset, a_min.y + inset);
		const ImVec2 max(a_max.x - inset, a_max.y - inset);
		const ImU32 cord = WithAlpha(silver, 0.8f);
		Rope(a_drawList, ImVec2(min.x, min.y + thick), ImVec2(min.x, max.y - thick), thick, cord);
		Rope(a_drawList, ImVec2(max.x, min.y + thick), ImVec2(max.x, max.y - thick), thick, cord);
		Chain(a_drawList, ImVec2(min.x + thick, min.y), ImVec2(max.x - thick, min.y), thick, cord);
		Chain(a_drawList, ImVec2(min.x + thick, max.y), ImVec2(max.x - thick, max.y), thick, cord);
		for (const auto& corner : { min, ImVec2(max.x, min.y), max, ImVec2(min.x, max.y) }) {
			CornerKnot(a_drawList, corner, thick * 0.75f, silver);
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// A blacker inset, a faint constellation in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(black, 0.45f));
		EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(0, 0, 0, 120));

		if (a_emblem) {
			// The Warrior's sword, as a constellation
			const float  span = std::min(height * 0.7f, width * 0.22f);
			const ImVec2 base(a_max.x - span * 0.6f, a_max.y - span * 0.15f);
			const auto   at = [&](float a_x, float a_y) { return ImVec2(base.x + a_x * span, base.y - a_y * span); };
			const ImVec2 stars[] = { at(0.0f, 0.0f), at(0.0f, 0.22f), at(-0.18f, 0.3f), at(0.18f, 0.3f), at(0.0f, 0.55f), at(0.0f, 0.8f), at(0.0f, 0.95f) };
			const ImU32  line = WithAlpha(IM_COL32(200, 170, 255, 255), 0.09f);
			a_drawList->AddLine(stars[0], stars[1], line, 1.0f);
			a_drawList->AddLine(stars[2], stars[3], line, 1.0f);
			a_drawList->AddLine(stars[1], stars[4], line, 1.0f);
			a_drawList->AddLine(stars[4], stars[5], line, 1.0f);
			a_drawList->AddLine(stars[5], stars[6], line, 1.0f);
			for (const auto& star : stars) {
				Star(a_drawList, star, std::max(2.0f, span * 0.025f), WithAlpha(IM_COL32(220, 200, 255, 255), 0.22f), 1.0f);
			}
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Short lengths of rope down a panel's sides, a knot at each corner
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float thick = std::max(3.0f, size * 0.22f);
		const float reach = std::min(size * 2.5f, (a_max.y - a_min.y) * 0.3f);
		const float in = thick * 0.6f + 1.0f;
		for (const float x : { a_min.x + in, a_max.x - in }) {
			Rope(a_drawList, ImVec2(x, a_min.y + in + thick), ImVec2(x, a_min.y + in + reach), thick, silverDim);
			Rope(a_drawList, ImVec2(x, a_max.y - in - reach), ImVec2(x, a_max.y - in - thick), thick, silverDim);
		}
		for (const auto& corner : { ImVec2(a_min.x + in, a_min.y + in), ImVec2(a_max.x - in, a_min.y + in), ImVec2(a_max.x - in, a_max.y - in), ImVec2(a_min.x + in, a_max.y - in) }) {
			CornerKnot(a_drawList, corner, thick * 0.6f, silver);
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Glow Bar ) ----------------------
	//---------------------------------------------------

	// The chosen row: light fading out from the left, the menus' double chevron at the left edge
	void GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha) {

		const float height = a_max.y - a_min.y;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const ImU32 light = WithAlpha(silver, 0.14f * a_alpha);
		const ImU32 clear = WithAlpha(silver, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, a_max, light, clear, clear, light);
		const float arm = std::min(height * 0.3f, ImGui::GetFontSize() * 0.32f);
		for (const float back : { 0.0f, arm * 0.7f }) {
			const ImVec2 chevron[3] = { ImVec2(a_min.x + arm * 0.2f + back, midY - arm), ImVec2(a_min.x + arm * 0.2f + back + arm * 0.7f, midY), ImVec2(a_min.x + arm * 0.2f + back, midY + arm) };
			a_drawList->AddPolyline(chevron, 3, WithAlpha(back == 0.0f ? silverDim : silver, a_alpha), ImDrawFlags_None, std::max(1.0f, arm * 0.22f));
		}
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Perk Node ) ---------------------
	//---------------------------------------------------

	// Stars, as in the skill menu's constellations: bright when taken, dim when next, a speck when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			Star(a_drawList, a_centre, a_radius * 1.3f, a_colour, 1.0f);
			break;
		case PerkState::kNext:
			Star(a_drawList, a_centre, a_radius * 0.9f, WithAlpha(a_colour, 0.8f), 0.4f);
			break;
		default:
			a_drawList->AddCircleFilled(a_centre, std::max(1.5f, a_radius * 0.3f), a_colour, 8);
			break;
		}
	}

	//---------------------------------------------------
	//-- Nordic Decor ( Perk Link ) ---------------------
	//---------------------------------------------------

	// Constellation lines: straight, faint, brighter between taken stars
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		a_drawList->AddLine(a_from, a_to, WithAlpha(a_colour, a_lit ? 0.75f : 0.4f), a_lit ? std::max(1.0f, a_radius * 0.1f) : 1.0f);
	}
}
