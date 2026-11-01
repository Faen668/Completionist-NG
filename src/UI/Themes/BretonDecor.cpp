#include "UI/Themes/BretonDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::BretonDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Pale castle stone, lead and glass, wine and silver from the heraldry
		constexpr ImU32 stone = IM_COL32(128, 124, 126, 255);
		constexpr ImU32 stoneLight = IM_COL32(178, 174, 172, 255);
		constexpr ImU32 stoneDark = IM_COL32(64, 62, 64, 255);
		constexpr ImU32 lead = IM_COL32(30, 28, 32, 255);
		constexpr ImU32 glass = IM_COL32(40, 36, 44, 255);
		constexpr ImU32 wine = IM_COL32(118, 40, 62, 255);
		constexpr ImU32 wineDeep = IM_COL32(58, 20, 32, 255);
		constexpr ImU32 silver = IM_COL32(200, 204, 212, 255);
		constexpr ImU32 silverLight = IM_COL32(240, 242, 246, 255);
		constexpr ImU32 candle = IM_COL32(255, 206, 140, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Breton Decor ( Fade At ) -----------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Breton Decor ( Shield Outline ) ----------------
		//---------------------------------------------------

		// A heater shield: flat top, straight sides, curving in to the point
		std::vector<ImVec2> ShieldOutline(ImVec2 a_centre, float a_half, float a_height) {

			const float top = a_centre.y - a_height;
			const float shoulder = a_centre.y + a_height * 0.05f;
			const ImVec2 point(a_centre.x, a_centre.y + a_height);
			std::vector<ImVec2> outline{ ImVec2(a_centre.x - a_half, top), ImVec2(a_centre.x + a_half, top), ImVec2(a_centre.x + a_half, shoulder) };
			constexpr int steps = 10;
			for (int side = 0; side < 2; ++side) {
				const float  sign = side == 0 ? 1.0f : -1.0f;
				const ImVec2 from = side == 0 ? ImVec2(a_centre.x + a_half, shoulder) : point;
				const ImVec2 control(a_centre.x + sign * a_half, a_centre.y + a_height * 0.72f);
				const ImVec2 to = side == 0 ? point : ImVec2(a_centre.x - a_half, shoulder);
				for (int i = 1; i <= steps; ++i) {
					const float t = static_cast<float>(i) / steps;
					const float u = 1.0f - t;
					outline.emplace_back(u * u * from.x + 2.0f * u * t * control.x + t * t * to.x, u * u * from.y + 2.0f * u * t * control.y + t * t * to.y);
				}
			}
			return outline;
		}

		//---------------------------------------------------
		//-- Breton Decor ( Tapestry ) ----------------------
		//---------------------------------------------------

		// Backdrop: a wine tapestry woven with a lozenge pattern, candlelight from above
		void Tapestry(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(52, 22, 34, 70), IM_COL32(52, 22, 34, 70), IM_COL32(16, 8, 12, 70), IM_COL32(16, 8, 12, 70) });

			// The weave
			for (float y = a_min.y; y < a_max.y; y += 3.0f) {
				a_drawList->AddLine(ImVec2(a_min.x, y), ImVec2(a_max.x, y), WithAlpha(IM_COL32(0, 0, 0, 255), 0.06f), 1.0f);
			}

			// The lozenges woven in silver thread, a stitch where the lines cross
			const float cell = size * 3.0f;
			for (float x = a_min.x - height; x < a_max.x; x += cell) {
				a_drawList->AddLine(ImVec2(x, a_min.y), ImVec2(x + height, a_max.y), WithAlpha(silver, 0.035f), 1.0f);
				a_drawList->AddLine(ImVec2(x + height, a_min.y), ImVec2(x, a_max.y), WithAlpha(silver, 0.035f), 1.0f);
			}
			for (float y = a_min.y; y < a_max.y + cell; y += cell * 0.5f) {
				const int   row = static_cast<int>((y - a_min.y) / (cell * 0.5f));
				const float offset = row % 2 == 0 ? 0.0f : cell * 0.5f;
				for (float x = a_min.x + offset; x < a_max.x; x += cell) {
					a_drawList->AddCircleFilled(ImVec2(x, y), 1.6f, WithAlpha(silver, 0.07f), 6);
				}
			}

			// Candles out of sight above
			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_min.y - size * 2.0f), std::max(width * 0.4f, size * 10.0f), WithAlpha(candle, 0.01f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(6, 3, 4, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Breton Decor ( Leaded Band ) -------------------
		//---------------------------------------------------

		// One side of the frame: diamond panes set in lead, between mouldings of pale stone
		void LeadedBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};

			const float moulding = std::max(2.0f, depth * 0.2f);
			const float inner = depth - moulding;
			const ImVec2 glassMin = at(0.0f, moulding);
			const ImVec2 glassMax = at(length, inner);
			a_drawList->AddRectFilled(glassMin, glassMax, glass);

			// The panes, a few tinted wine, the rest pale
			a_drawList->PushClipRect(ImVec2(std::min(glassMin.x, glassMax.x), std::min(glassMin.y, glassMax.y)), ImVec2(std::max(glassMin.x, glassMax.x), std::max(glassMin.y, glassMax.y)), true);
			Random      random{ a_seed };
			const float pane = inner - moulding;
			const float middle = (moulding + inner) * 0.5f;
			const float stroke = std::max(1.0f, depth * 0.09f);
			for (float along = 0.0f; along < length + pane; along += pane * 0.5f) {
				const int   step = static_cast<int>(along / (pane * 0.5f));
				const float across = step % 2 == 0 ? middle : (step % 4 == 1 ? moulding : inner);
				const ImVec2 points[4] = { at(along, across - pane * 0.5f), at(along + pane * 0.5f, across), at(along, across + pane * 0.5f), at(along - pane * 0.5f, across) };
				const bool   tinted = random.Next() < 0.18f;
				a_drawList->AddConvexPolyFilled(points, 4, tinted ? WithAlpha(wine, random.Range(0.55f, 0.75f)) : WithAlpha(silver, random.Range(0.12f, 0.24f)));
				a_drawList->AddPolyline(points, 4, lead, ImDrawFlags_Closed, stroke);
				if (random.Next() < 0.25f) {
					a_drawList->AddLine(at(along - pane * 0.15f, across - pane * 0.1f), at(along, across - pane * 0.25f), WithAlpha(silverLight, 0.5f), 1.0f);
				}
			}
			a_drawList->PopClipRect();

			// Stone mouldings either side, lit on their upper edge
			for (const auto& [from, to] : { std::pair{ 0.0f, moulding }, std::pair{ inner, depth } }) {
				a_drawList->AddRectFilled(at(0.0f, from), at(length, to), stone);
				a_drawList->AddLine(at(0.0f, from + 0.5f), at(length, from + 0.5f), WithAlpha(stoneLight, 0.8f), 1.0f);
				a_drawList->AddLine(at(0.0f, to - 0.5f), at(length, to - 0.5f), WithAlpha(stoneDark, 0.8f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Breton Decor ( Corner Stone ) ------------------
		//---------------------------------------------------

		// A carved stone block at each corner, a rose cut in it
		void CornerStone(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, stone);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), stoneLight, 1.5f);
			a_drawList->AddLine(ImVec2(min.x + 1.0f, min.y), ImVec2(min.x + 1.0f, max.y), WithAlpha(stoneLight, 0.7f), 1.0f);
			a_drawList->AddLine(ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), stoneDark, 1.5f);
			const float inset = a_size * 0.18f;
			a_drawList->AddRect(ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), WithAlpha(stoneDark, 0.7f), 0.0f, 0, 1.0f);
			Rose(a_drawList, a_centre, a_size * 0.55f, wine, true);
		}

		//---------------------------------------------------
		//-- Breton Decor ( Crest ) -------------------------
		//---------------------------------------------------

		// A heraldic shield hung on the beam: quartered wine and silver, a rose at its heart
		void Crest(ImDrawList* a_drawList, ImVec2 a_centre, float a_half, float a_height) {

			const auto outline = ShieldOutline(a_centre, a_half, a_height);
			const auto border = ShieldOutline(a_centre, a_half + 2.5f, a_height + 2.5f);
			for (const auto& point : border) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(silver);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(wine);
			// Silver in the upper right and lower left quarters
			a_drawList->PushClipRect(ImVec2(a_centre.x, a_centre.y - a_height), ImVec2(a_centre.x + a_half, a_centre.y), true);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(silver);
			a_drawList->PopClipRect();
			a_drawList->PushClipRect(ImVec2(a_centre.x - a_half, a_centre.y), ImVec2(a_centre.x, a_centre.y + a_height), true);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(silver);
			a_drawList->PopClipRect();
			a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), lead, ImDrawFlags_Closed, 1.0f);
			a_drawList->AddCircleFilled(a_centre, a_half * 0.5f, lead, 20);
			Rose(a_drawList, a_centre, a_half * 0.42f, wine, true);
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Lozenge ) -----------------------
	//---------------------------------------------------

	// In place of the diamond: a heraldic lozenge, lit on its upper faces
	void Lozenge(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius * 1.15f),
			ImVec2(a_centre.x + a_radius * 0.8f, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius * 1.15f),
			ImVec2(a_centre.x - a_radius * 0.8f, a_centre.y),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
			return;
		}
		a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(IM_COL32(255, 255, 255, 255), 0.3f * Alpha(a_colour)));
	}

	//---------------------------------------------------
	//-- Breton Decor ( Rose ) --------------------------
	//---------------------------------------------------

	// A heraldic rose: five petals round a seeded heart
	void Rose(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float petal = a_radius * 0.46f;
		const float stroke = std::max(1.0f, a_radius * 0.12f);
		for (int i = 0; i < 5; ++i) {
			const float  angle = static_cast<float>(i) * pi * 0.4f - pi * 0.5f;
			const ImVec2 at(a_centre.x + std::cos(angle) * a_radius * 0.54f, a_centre.y + std::sin(angle) * a_radius * 0.54f);
			if (a_filled) {
				a_drawList->AddCircleFilled(at, petal, a_colour, 14);
			} else {
				a_drawList->PathArcTo(at, petal, angle - pi * 0.62f, angle + pi * 0.62f, 10);
				a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
			}
		}
		const float alpha = Alpha(a_colour);
		if (a_filled) {
			// Barbs between the petals, and the seeds
			for (int i = 0; i < 5; ++i) {
				const float angle = static_cast<float>(i) * pi * 0.4f - pi * 0.3f;
				a_drawList->AddLine(ImVec2(a_centre.x + std::cos(angle) * a_radius * 0.5f, a_centre.y + std::sin(angle) * a_radius * 0.5f), ImVec2(a_centre.x + std::cos(angle) * a_radius * 0.82f, a_centre.y + std::sin(angle) * a_radius * 0.82f), WithAlpha(IM_COL32(0, 0, 0, 255), 0.35f * alpha), 1.0f);
			}
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.3f, WithAlpha(silverLight, alpha), 12);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.12f, WithAlpha(wineDeep, alpha), 8);
		} else {
			a_drawList->AddCircle(a_centre, a_radius * 0.24f, a_colour, 12, stroke);
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// Needed / found: a heater shield, its field filled with a chevron when found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  half = a_size * 0.27f;
		const float  height = a_size * 0.36f;
		const ImVec2 centre(a_centre.x, a_centre.y + a_size * 0.02f);
		const auto   outline = ShieldOutline(centre, half, height);
		if (a_fill > 0.0f) {
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(a_colour, a_fill));
			const ImVec2 chevron[3] = { ImVec2(centre.x - half * 0.75f, centre.y + height * 0.35f), ImVec2(centre.x, centre.y - height * 0.3f), ImVec2(centre.x + half * 0.75f, centre.y + height * 0.35f) };
			a_drawList->AddPolyline(chevron, 3, WithAlpha(wineDeep, a_fill), ImDrawFlags_None, std::max(1.5f, a_size * 0.1f));
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.07f));
	}

	//---------------------------------------------------
	//-- Breton Decor ( Came ) --------------------------
	//---------------------------------------------------

	// In place of a rule: lead came crossing in a run of diamonds, a bead in each, fading at the ends
	void Came(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float half = a_size * 0.2f;
		const float unit = a_size * 0.7f;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		const int   count = std::max(1, static_cast<int>(length / unit));
		const float start = a_from.x + (length - static_cast<float>(count) * unit) * 0.5f;
		for (int i = 0; i < count; ++i) {
			const float x0 = start + static_cast<float>(i) * unit;
			const float x1 = x0 + unit * 0.5f;
			const float x2 = x0 + unit;
			const float fade = FadeAt(x1, a_from.x, length);
			const ImU32 colour = WithAlpha(a_colour, fade);
			const ImVec2 diamond[4] = { ImVec2(x0, a_from.y), ImVec2(x1, a_from.y - half), ImVec2(x2, a_from.y), ImVec2(x1, a_from.y + half) };
			a_drawList->AddPolyline(diamond, 4, colour, ImDrawFlags_Closed, stroke);
			a_drawList->AddCircleFilled(ImVec2(x1, a_from.y), std::max(1.0f, half * 0.3f), WithAlpha(a_colour, fade * 0.9f), 6);
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Came(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Came(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Rose(a_drawList, ImVec2(midX, a_from.y), gap * 0.42f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Breton Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Leaded windows in pale stone, carved roses on the corners, a shield hung on the top beam
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Tapestry(a_drawList, a_min, a_max);

		LeadedBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0xB201u);
		LeadedBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0xB202u);
		LeadedBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0xB203u);
		LeadedBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0xB204u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(silver, 0.2f), 0.0f, 0, 1.0f);

		const float corner = band * 0.95f;
		CornerStone(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerStone(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerStone(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		CornerStone(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);

		// Only on the main window; small panels keep plain beams
		if (compact >= 0.8f) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			// Hung inside the window's top edge, so none of it is clipped
			Crest(a_drawList, ImVec2(midX, a_min.y + band * 0.85f + 3.0f), band * 0.75f, band * 0.85f);
			CornerStone(a_drawList, ImVec2(midX, a_max.y - band * 0.5f), corner);
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: the tapestry's lozenges at the edges clearing towards the text, candlelight from above, a shield in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(candle, 0.045f), WithAlpha(candle, 0.045f), WithAlpha(candle, 0.0f), WithAlpha(candle, 0.0f));

		// Lozenge lines, drawn in short lengths so each fades by how near the edge it is
		const float cell = size * 2.4f;
		const float piece = size * 0.8f;
		a_drawList->PushClipRect(a_min, a_max, true);
		for (float x = a_min.x - height; x < a_max.x; x += cell) {
			for (const float sign : { 1.0f, -1.0f }) {
				const float startX = sign > 0.0f ? x : x + height;
				for (float t = 0.0f; t < height; t += piece) {
					const ImVec2 from(startX + sign * t, a_min.y + t);
					const ImVec2 to(startX + sign * (t + piece), a_min.y + t + piece);
					const float  weight = EdgeWeight(ImVec2((from.x + to.x) * 0.5f, (from.y + to.y) * 0.5f), a_min, a_max, band);
					if (weight > 0.03f) {
						a_drawList->AddLine(from, to, WithAlpha(silver, weight * 0.1f), 1.0f);
					}
				}
			}
		}
		a_drawList->PopClipRect();

		// A shield, large and faint, in the lower right
		if (a_emblem) {
			const float  height2 = std::min(height * 0.42f, width * 0.2f);
			const ImVec2 centre(a_max.x - height2 * 1.2f, a_max.y - height2 * 1.0f);
			const auto   outline = ShieldOutline(centre, height2 * 0.78f, height2);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(wine, 0.12f));
			a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), WithAlpha(silver, 0.1f), ImDrawFlags_Closed, std::max(2.0f, height2 * 0.04f));
			Rose(a_drawList, centre, height2 * 0.4f, WithAlpha(silver, 0.08f), true);
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Silver brackets on the panel's corners, a lozenge on the point
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(2.0f, size * 0.14f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick + 1.0f), outer.y + sy * (thick + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, silver, ImDrawFlags_None, thick);
			const float  inset = thick * 2.2f;
			const ImVec2 inner[3] = { ImVec2(corner.x + sx * arm * 0.6f, corner.y + sy * inset), ImVec2(corner.x + sx * inset, corner.y + sy * inset), ImVec2(corner.x + sx * inset, corner.y + sy * arm * 0.6f) };
			a_drawList->AddPolyline(inner, 3, WithAlpha(silver, 0.6f), ImDrawFlags_None, 1.0f);
			for (const auto& end : { points[0], points[2] }) {
				Lozenge(a_drawList, end, thick * 1.3f, silver, true);
			}
			Lozenge(a_drawList, ImVec2(corner.x + sx * thick * 0.5f, corner.y + sy * thick * 0.5f), thick * 2.0f, wine, true);
			Lozenge(a_drawList, ImVec2(corner.x + sx * thick * 0.5f, corner.y + sy * thick * 0.5f), thick * 2.0f, silver, false);
		}
	}
}

namespace CompletionistUI::BretonDecor
{
	//---------------------------------------------------
	//-- Breton Decor ( Perk Node ) ---------------------
	//---------------------------------------------------

	// Roses: in full colour when taken, outlined when next, a bare lozenge when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Rose(a_drawList, a_centre, a_radius * 1.1f, a_colour, true);
			return;
		case PerkState::kNext:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.95f, glass, 20);
			Rose(a_drawList, a_centre, a_radius * 1.0f, a_colour, false);
			return;
		default:
			Lozenge(a_drawList, a_centre, a_radius * 0.45f, glass, true);
			Lozenge(a_drawList, a_centre, a_radius * 0.45f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Breton Decor ( Perk Link ) ---------------------
	//---------------------------------------------------

	// A banner cord: two strands twisted round each other where the perks are taken
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, 3.0f);
		if (!a_lit) {
			a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		const float twist = std::max(1.5f, a_radius * 0.16f);
		const float stroke = std::max(1.0f, a_radius * 0.1f);
		float       travelled = 0.0f;
		std::vector<ImVec2> strands[2];
		for (std::size_t i = 0; i < curve.size(); ++i) {
			if (i > 0) {
				travelled += std::hypot(curve[i].x - curve[i - 1].x, curve[i].y - curve[i - 1].y);
			}
			const auto  normal = NormalAt(curve, i);
			const float wave = std::sin(travelled / (twist * 2.2f)) * twist;
			strands[0].emplace_back(curve[i].x + normal.x * wave, curve[i].y + normal.y * wave);
			strands[1].emplace_back(curve[i].x - normal.x * wave, curve[i].y - normal.y * wave);
		}
		a_drawList->AddPolyline(strands[0].data(), static_cast<int>(strands[0].size()), WithAlpha(a_colour, 0.7f), ImDrawFlags_None, stroke);
		a_drawList->AddPolyline(strands[1].data(), static_cast<int>(strands[1].size()), a_colour, ImDrawFlags_None, stroke);
	}
}
