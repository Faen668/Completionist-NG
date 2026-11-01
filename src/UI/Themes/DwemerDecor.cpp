#include "UI/Themes/DwemerDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::DwemerDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Cut stone, the mortar between, and the Dwemer's gold
		constexpr ImU32 stone = IM_COL32(70, 70, 72, 255);
		constexpr ImU32 stoneLight = IM_COL32(112, 110, 106, 255);
		constexpr ImU32 stoneDark = IM_COL32(34, 34, 36, 255);
		constexpr ImU32 mortar = IM_COL32(14, 14, 15, 255);
		constexpr ImU32 gold = IM_COL32(204, 156, 60, 255);
		constexpr ImU32 goldLight = IM_COL32(250, 214, 130, 255);
		constexpr ImU32 goldDeep = IM_COL32(150, 108, 38, 255);
		constexpr ImU32 lamp = IM_COL32(255, 190, 96, 255);

		//---------------------------------------------------
		//-- Dwemer Decor ( Fade At ) -----------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Stonework ) ---------------------
		//---------------------------------------------------

		// Backdrop: a wall of big dressed blocks, lamplight from the top corners
		void Stonework(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(44, 44, 46, 70), IM_COL32(44, 44, 46, 70), IM_COL32(12, 12, 13, 70), IM_COL32(12, 12, 13, 70) });

			// Courses of blocks, every other one set half a block along
			Random      random{ 0xD3E3u };
			const float course = size * 3.2f;
			int         row = 0;
			for (float y = a_min.y; y < a_max.y; y += course, ++row) {
				const float block = course * random.Range(2.2f, 2.8f);
				for (float x = a_min.x - (row % 2 == 0 ? 0.0f : block * 0.5f); x < a_max.x; x += block) {
					const ImVec2 min(x, y);
					const ImVec2 max(x + block, y + course);
					a_drawList->AddRectFilled(ImVec2(min.x + 2.0f, min.y + 2.0f), ImVec2(max.x - 2.0f, max.y - 2.0f), WithAlpha(random.Next() < 0.5f ? stoneLight : stoneDark, random.Range(0.03f, 0.08f)));
					a_drawList->AddLine(ImVec2(min.x + 2.0f, min.y + 2.0f), ImVec2(max.x - 2.0f, min.y + 2.0f), WithAlpha(stoneLight, 0.07f), 1.0f);
					a_drawList->AddLine(min, ImVec2(max.x, min.y), WithAlpha(mortar, 0.45f), 2.0f);
					a_drawList->AddLine(min, ImVec2(min.x, max.y), WithAlpha(mortar, 0.45f), 2.0f);
				}
			}

			// Lamps out of sight in the upper corners, and their warmth on the stone
			for (const float side : { 0.12f, 0.88f }) {
				SoftSpot(a_drawList, ImVec2(a_min.x + width * side, a_min.y - size), std::max(width * 0.28f, size * 8.0f), WithAlpha(lamp, 0.01f));
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 4, 5, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Gold Strip ) --------------------
		//---------------------------------------------------

		// A strip of worked gold, lit on its upper edge
		void GoldStrip(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			a_drawList->AddRectFilled(a_min, a_max, gold);
			if (a_max.y - a_min.y < a_max.x - a_min.x) {
				a_drawList->AddLine(ImVec2(a_min.x, a_min.y + 0.5f), ImVec2(a_max.x, a_min.y + 0.5f), WithAlpha(goldLight, 0.8f), 1.0f);
			} else {
				a_drawList->AddLine(ImVec2(a_min.x + 0.5f, a_min.y), ImVec2(a_min.x + 0.5f, a_max.y), WithAlpha(goldLight, 0.8f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Brick Band ) --------------------
		//---------------------------------------------------

		// One side of the frame: rough bricks, chipped and uneven, between strips of gold
		void BrickBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, mortar);

			const float trim = std::max(1.5f, depth * 0.1f);
			const float inner = depth - trim;
			Random      random{ a_seed };
			float       along = 0.0f;
			while (along < length) {
				const float brick = depth * random.Range(1.2f, 1.9f);
				const float end = std::min(length, along + brick);
				const ImU32 face = Shade(stone, random.Range(0.72f, 1.12f));
				a_drawList->AddRectFilled(at(along + 1.0f, trim + 1.0f), at(end - 1.0f, inner - 1.0f), face);
				a_drawList->AddLine(at(along + 1.0f, trim + 1.5f), at(end - 1.0f, trim + 1.5f), WithAlpha(stoneLight, 0.55f), 1.0f);
				a_drawList->AddLine(at(along + 1.0f, inner - 1.5f), at(end - 1.0f, inner - 1.5f), WithAlpha(stoneDark, 0.8f), 1.0f);
				// Pits in the face, and a chipped corner now and then
				for (int pit = 0; pit < 3; ++pit) {
					a_drawList->AddCircleFilled(at(along + (end - along) * random.Range(0.15f, 0.85f), trim + (inner - trim) * random.Range(0.25f, 0.75f)), random.Range(0.6f, 1.3f), WithAlpha(stoneDark, 0.7f), 5);
				}
				if (random.Next() < 0.35f) {
					const float  chip = depth * random.Range(0.12f, 0.22f);
					const ImVec2 corner = at(end - 1.0f, trim + 1.0f);
					const ImVec2 chipped[3] = { corner, at(end - 1.0f - chip, trim + 1.0f), at(end - 1.0f, trim + 1.0f + chip) };
					a_drawList->AddTriangleFilled(chipped[0], chipped[1], chipped[2], mortar);
				}
				along = end;
			}

			GoldStrip(a_drawList, at(0.0f, 0.0f), at(length, trim));
			GoldStrip(a_drawList, at(0.0f, inner), at(length, depth));

			// Gold clamps across the bricks, bolted
			const float clamp = depth * 0.55f;
			for (float x = depth * 6.0f; x + clamp < length - depth * 5.0f; x += depth * 21.0f) {
				GoldStrip(a_drawList, at(x, 0.0f), at(x + clamp, depth));
				a_drawList->AddRect(at(x, 0.0f), at(x + clamp, depth), WithAlpha(goldDeep, 0.9f), 0.0f, 0, 1.0f);
				for (const float across : { depth * 0.3f, depth * 0.7f }) {
					a_drawList->AddCircleFilled(at(x + clamp * 0.5f, across), depth * 0.08f, goldLight, 6);
				}
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Key Plate ) ---------------------
		//---------------------------------------------------

		// A square gold plate at each corner with a squared spiral cut in it, as on Dwemer doors
		void KeyPlate(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_sx, float a_sy) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, gold);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), goldLight, 1.5f);
			a_drawList->AddLine(ImVec2(min.x + 1.0f, min.y), ImVec2(min.x + 1.0f, max.y), WithAlpha(goldLight, 0.7f), 1.0f);
			const float inset = a_size * 0.2f;
			a_drawList->AddRectFilled(ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), stoneDark);

			// The spiral turns in from the outer corner of the frame
			const auto at = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * a_size * a_sx, a_centre.y + a_y * a_size * a_sy); };
			const ImVec2 spiral[] = { at(-0.6f, -0.6f), at(0.6f, -0.6f), at(0.6f, 0.6f), at(-0.3f, 0.6f), at(-0.3f, -0.25f), at(0.3f, -0.25f), at(0.3f, 0.3f), at(0.0f, 0.3f) };
			a_drawList->AddPolyline(spiral, static_cast<int>(std::size(spiral)), gold, ImDrawFlags_None, std::max(1.5f, a_size * 0.14f));
			for (const auto& bolt : { at(-0.6f, 0.6f), at(-0.88f, -0.88f), at(0.88f, -0.88f), at(0.88f, 0.88f), at(-0.88f, 0.88f) }) {
				a_drawList->AddCircleFilled(bolt, std::max(1.0f, a_size * 0.07f), goldLight, 6);
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Crest ) -------------------------
		//---------------------------------------------------

		// A gold-bound stone lozenge set into the middle of a beam, a cog at its heart
		void Crest(ImDrawList* a_drawList, ImVec2 a_centre, float a_width, float a_height) {

			const auto diamond = [&](float a_scale, ImU32 a_fill) {
				const ImVec2 points[4] = {
					ImVec2(a_centre.x, a_centre.y - a_height * a_scale),
					ImVec2(a_centre.x + a_width * a_scale, a_centre.y),
					ImVec2(a_centre.x, a_centre.y + a_height * a_scale),
					ImVec2(a_centre.x - a_width * a_scale, a_centre.y),
				};
				a_drawList->AddConvexPolyFilled(points, 4, a_fill);
			};
			diamond(1.0f, gold);
			diamond(0.86f, mortar);
			diamond(0.8f, stone);
			diamond(0.6f, goldDeep);
			diamond(0.54f, stoneDark);
			const ImVec2 top[3] = { ImVec2(a_centre.x - a_width, a_centre.y), ImVec2(a_centre.x, a_centre.y - a_height), ImVec2(a_centre.x + a_width, a_centre.y) };
			a_drawList->AddPolyline(top, 3, goldLight, ImDrawFlags_None, 1.5f);
			Cog(a_drawList, a_centre, a_height * 0.36f, gold, true);
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Stud ) --------------------------
	//---------------------------------------------------

	// In place of the diamond: a gold stud, lit on its upper faces
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
			return;
		}
		a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(IM_COL32(255, 240, 200, 255), 0.35f * alpha));
		a_drawList->AddTriangleFilled(points[1], points[2], a_centre, WithAlpha(IM_COL32(0, 0, 0, 255), 0.22f * alpha));
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Cog ) ---------------------------
	//---------------------------------------------------

	// A Dwemer cog: square teeth round a hub, the axle hole left dark when filled
	void Cog(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		constexpr int teeth = 8;
		const float   step = pi * 2.0f / teeth;
		const float   root = a_radius * 0.76f;
		ImVec2        points[teeth * 4];
		for (int tooth = 0; tooth < teeth; ++tooth) {
			const float base = static_cast<float>(tooth) * step - pi * 0.5f;
			const float angles[4] = { base - step * 0.27f, base - step * 0.17f, base + step * 0.17f, base + step * 0.27f };
			const float radii[4] = { root, a_radius, a_radius, root };
			for (int i = 0; i < 4; ++i) {
				points[tooth * 4 + i] = ImVec2(a_centre.x + std::cos(angles[i]) * radii[i], a_centre.y + std::sin(angles[i]) * radii[i]);
			}
		}
		const float stroke = std::max(1.0f, a_radius * 0.14f);
		if (a_filled) {
			for (const auto& point : points) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(a_colour);
			const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.3f, WithAlpha(mortar, alpha), 16);
			a_drawList->AddCircle(a_centre, a_radius * 0.52f, WithAlpha(IM_COL32(0, 0, 0, 255), 0.3f * alpha), 20, std::max(1.0f, a_radius * 0.08f));
			return;
		}
		a_drawList->AddPolyline(points, teeth * 4, a_colour, ImDrawFlags_Closed, stroke);
		a_drawList->AddCircle(a_centre, a_radius * 0.32f, a_colour, 16, stroke);
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// Needed / found: a cog, cast in gold as it's found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.36f;
		if (a_fill > 0.0f) {
			Cog(a_drawList, a_centre, radius, WithAlpha(a_colour, a_fill), true);
		}
		if (a_fill < 1.0f) {
			Cog(a_drawList, a_centre, radius, WithAlpha(a_colour, 1.0f - a_fill * 0.6f), false);
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Inlay ) -------------------------
	//---------------------------------------------------

	// In place of a rule: a gold band inlaid with square blocks, fading at the ends
	void Inlay(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float half = a_size * 0.22f;
		const float unit = a_size * 0.62f;
		const float block = unit * 0.45f;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		const float step = unit * 0.5f;
		for (float x = a_from.x; x < a_to.x; x += step) {
			const float end = std::min(a_to.x, x + step);
			const float fade = FadeAt((x + end) * 0.5f, a_from.x, length);
			a_drawList->AddLine(ImVec2(x, a_from.y - half), ImVec2(end, a_from.y - half), WithAlpha(a_colour, fade), stroke);
			a_drawList->AddLine(ImVec2(x, a_from.y + half), ImVec2(end, a_from.y + half), WithAlpha(a_colour, fade), stroke);
		}
		for (float x = a_from.x + (std::fmod(length, unit) + unit - block) * 0.5f; x + block <= a_to.x; x += unit) {
			const float fade = FadeAt(x + block * 0.5f, a_from.x, length);
			a_drawList->AddRectFilled(ImVec2(x, a_from.y - half * 0.45f), ImVec2(x + block, a_from.y + half * 0.45f), WithAlpha(a_colour, fade * 0.85f));
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Inlay(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Inlay(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Cog(a_drawList, ImVec2(midX, a_from.y), gap * 0.4f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Rough brick bound in gold, key-cut plates on the corners, a crest set in the top and bottom beams
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Stonework(a_drawList, a_min, a_max);

		BrickBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0xD401u);
		BrickBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0xD402u);
		BrickBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0xD403u);
		BrickBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0xD404u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 2.0f, a_min.y + band + 2.0f), ImVec2(a_max.x - band - 2.0f, a_max.y - band - 2.0f), WithAlpha(gold, 0.22f), 0.0f, 0, 1.0f);

		const float corner = band * 0.95f;
		KeyPlate(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner, 1.0f, 1.0f);
		KeyPlate(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner, -1.0f, 1.0f);
		KeyPlate(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner, -1.0f, -1.0f);
		KeyPlate(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner, 1.0f, -1.0f);

		// Only on the main window; small panels keep plain beams
		if (compact >= 0.8f) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			Crest(a_drawList, ImVec2(midX, a_min.y + band * 0.5f), band * 2.6f, band * 1.3f);
			Crest(a_drawList, ImVec2(midX, a_max.y - band * 0.5f), band * 1.8f, band * 0.9f);
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: brick courses at the edges clearing towards the text, lamplight from above, a great cog in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(lamp, 0.05f), WithAlpha(lamp, 0.05f), WithAlpha(lamp, 0.0f), WithAlpha(lamp, 0.0f));

		// Mortar lines of the brickwork, strongest at the edges
		Random      random{ static_cast<std::uint32_t>(width) * 41u + static_cast<std::uint32_t>(height) };
		const float course = size * 1.5f;
		int         row = 0;
		for (float y = a_min.y + course; y < a_max.y; y += course, ++row) {
			const float brick = course * 2.6f;
			float       x = a_min.x - (row % 2 == 0 ? 0.0f : brick * 0.5f);
			for (; x < a_max.x; x += brick) {
				const float left = std::max(a_min.x, x);
				const float right = std::min(a_max.x, x + brick);
				const float weight = EdgeWeight(ImVec2((left + right) * 0.5f, y - course * 0.5f), a_min, a_max, band);
				if (weight < 0.03f) {
					continue;
				}
				const ImU32 line = WithAlpha(IM_COL32(0, 0, 0, 255), weight * random.Range(0.3f, 0.5f));
				a_drawList->AddLine(ImVec2(left, y), ImVec2(right, y), line, 1.5f);
				if (x > a_min.x) {
					a_drawList->AddLine(ImVec2(x, y - course), ImVec2(x, y), line, 1.5f);
				}
				a_drawList->AddLine(ImVec2(left, y - course + 1.5f), ImVec2(right, y - course + 1.5f), WithAlpha(stoneLight, weight * 0.12f), 1.0f);
			}
		}

		// Grit in the stone
		const int grains = std::clamp(static_cast<int>(width * height / (size * size * 0.5f)), 40, 1600);
		for (int i = 0; i < grains; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.02f) {
				continue;
			}
			const bool light = random.Next() < 0.5f;
			a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.4f), WithAlpha(light ? stoneLight : IM_COL32(0, 0, 0, 255), weight * (light ? 0.2f : 0.35f)), 5);
		}

		// A great cog, large and faint, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.2f, a_max.y - radius * 0.95f);
			Cog(a_drawList, centre, radius, WithAlpha(gold, 0.07f), true);
			Cog(a_drawList, centre, radius, WithAlpha(goldLight, 0.08f), false);
			for (int spoke = 0; spoke < 4; ++spoke) {
				const float angle = static_cast<float>(spoke) * pi * 0.5f + pi * 0.25f;
				a_drawList->AddLine(ImVec2(centre.x + std::cos(angle) * radius * 0.32f, centre.y + std::sin(angle) * radius * 0.32f), ImVec2(centre.x + std::cos(angle) * radius * 0.7f, centre.y + std::sin(angle) * radius * 0.7f), WithAlpha(goldLight, 0.07f), std::max(2.0f, radius * 0.08f));
			}
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Stepped gold brackets on the panel's corners, bolted
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.2f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, gold, ImDrawFlags_None, thick);
			// The step inside the corner
			const float  inset = thick * 1.6f;
			const ImVec2 step[3] = { ImVec2(corner.x + sx * arm * 0.55f, corner.y + sy * inset), ImVec2(corner.x + sx * inset, corner.y + sy * inset), ImVec2(corner.x + sx * inset, corner.y + sy * arm * 0.55f) };
			a_drawList->AddPolyline(step, 3, WithAlpha(gold, 0.8f), ImDrawFlags_None, std::max(1.0f, thick * 0.45f));
			a_drawList->AddLine(ImVec2(corner.x, corner.y - sy * thick * 0.35f), ImVec2(corner.x + sx * arm, corner.y - sy * thick * 0.35f), WithAlpha(goldLight, 0.7f), 1.0f);
			for (const auto& bolt : { ImVec2(corner.x + sx * arm * 0.75f, corner.y), ImVec2(corner.x, corner.y + sy * arm * 0.75f) }) {
				a_drawList->AddCircleFilled(bolt, thick * 0.3f, goldLight, 6);
			}
		}
	}
}

namespace CompletionistUI::DwemerDecor
{
	//---------------------------------------------------
	//-- Dwemer Decor ( Perk Node ) ---------------------
	//---------------------------------------------------

	// Cogs: cast in gold when taken, an empty cog when next, a bare hex nut when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Cog(a_drawList, a_centre, a_radius * 1.05f, a_colour, true);
			return;
		case PerkState::kNext:
			// Over the pipes, so they end at its rim
			Cog(a_drawList, a_centre, a_radius * 0.95f, mortar, true);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.4f, mortar, 16);
			Cog(a_drawList, a_centre, a_radius * 0.95f, a_colour, false);
			return;
		default:
			{
				ImVec2 nut[6];
				for (int i = 0; i < 6; ++i) {
					const float angle = static_cast<float>(i) * pi / 3.0f;
					nut[i] = ImVec2(a_centre.x + std::cos(angle) * a_radius * 0.5f, a_centre.y + std::sin(angle) * a_radius * 0.5f);
				}
				a_drawList->AddConvexPolyFilled(nut, 6, mortar);
				a_drawList->AddPolyline(nut, 6, a_colour, ImDrawFlags_Closed, 1.0f);
				a_drawList->AddCircle(a_centre, a_radius * 0.2f, a_colour, 10, 1.0f);
			}
			return;
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Perk Link ) ---------------------
	//---------------------------------------------------

	// Pipes run at right angles between the perks, bright brass where they're taken, collars at the bends
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const float  middle = (a_from.x + a_to.x) * 0.5f;
		const ImVec2 points[4] = { a_from, ImVec2(middle, a_from.y), ImVec2(middle, a_to.y), a_to };
		if (!a_lit) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_None, 1.5f);
			return;
		}
		const float bore = std::max(2.0f, a_radius * 0.26f);
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_None, bore);
		// The light along the pipe's upper side
		const ImVec2 shine[4] = {
			ImVec2(a_from.x, a_from.y - bore * 0.25f), ImVec2(middle - bore * 0.25f, a_from.y - bore * 0.25f),
			ImVec2(middle - bore * 0.25f, a_to.y - bore * 0.25f), ImVec2(a_to.x, a_to.y - bore * 0.25f),
		};
		a_drawList->AddPolyline(shine, 4, WithAlpha(IM_COL32(255, 238, 190, 255), 0.45f), ImDrawFlags_None, std::max(1.0f, bore * 0.3f));
		if (std::abs(a_to.y - a_from.y) > bore * 3.0f) {
			for (const auto& bend : { points[1], points[2] }) {
				a_drawList->AddRectFilled(ImVec2(bend.x - bore * 0.9f, bend.y - bore * 0.9f), ImVec2(bend.x + bore * 0.9f, bend.y + bore * 0.9f), a_colour);
				a_drawList->AddRect(ImVec2(bend.x - bore * 0.9f, bend.y - bore * 0.9f), ImVec2(bend.x + bore * 0.9f, bend.y + bore * 0.9f), WithAlpha(IM_COL32(255, 238, 190, 255), 0.4f), 0.0f, 0, 1.0f);
			}
		}
	}
}
