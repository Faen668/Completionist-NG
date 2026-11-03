#include "UI/Themes/DwemerDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <numbers>
#include <utility>
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
		//-- Dwemer Decor ( Rivet ) -------------------------
		//---------------------------------------------------

		// A domed rivet with its shadow, lit from the upper left like everything else here
		void Rivet(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, float a_alpha = 1.0f) {

			a_drawList->AddCircleFilled(ImVec2(a_centre.x + a_radius * 0.3f, a_centre.y + a_radius * 0.4f), a_radius * 1.15f, WithAlpha(mortar, 0.6f * a_alpha), 8);
			a_drawList->AddCircleFilled(a_centre, a_radius, WithAlpha(a_colour, a_alpha), 8);
			a_drawList->AddCircleFilled(ImVec2(a_centre.x - a_radius * 0.3f, a_centre.y - a_radius * 0.35f), a_radius * 0.38f, WithAlpha(goldLight, 0.8f * a_alpha), 6);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Gear ) --------------------------
		//---------------------------------------------------

		struct GearSpec
		{
			ImVec2 centre;
			float  pitch;
			int    teeth;
			double angle;
		};

		// A cut gear that turns: the light stays put on its edges while the teeth go round
		void Gear(ImDrawList* a_drawList, const GearSpec& a_gear, float a_alpha, bool a_shadow) {

			constexpr int most = 48;
			const int     teeth = std::clamp(a_gear.teeth, 5, most);
			const float   angle = static_cast<float>(std::fmod(a_gear.angle, static_cast<double>(pi) * 2.0));
			const float   step = pi * 2.0f / static_cast<float>(teeth);
			const float   module = a_gear.pitch * 2.0f / static_cast<float>(teeth);
			const float   outer = a_gear.pitch + module * 0.55f;
			const float   root = a_gear.pitch - module * 0.7f;
			const ImVec2  c = a_gear.centre;

			ImVec2 points[most * 4];
			for (int tooth = 0; tooth < teeth; ++tooth) {
				const float base = static_cast<float>(tooth) * step + angle;
				const float angles[4] = { base - step * 0.31f, base - step * 0.16f, base + step * 0.16f, base + step * 0.31f };
				const float radii[4] = { root, outer, outer, root };
				for (int i = 0; i < 4; ++i) {
					points[tooth * 4 + i] = ImVec2(c.x + std::cos(angles[i]) * radii[i], c.y + std::sin(angles[i]) * radii[i]);
				}
			}
			const int count = teeth * 4;
			if (a_shadow) {
				const float drop = std::max(1.0f, a_gear.pitch * 0.09f);
				for (int i = 0; i < count; ++i) {
					a_drawList->PathLineTo(ImVec2(points[i].x + drop * 0.6f, points[i].y + drop));
				}
				a_drawList->PathFillConcave(WithAlpha(mortar, 0.7f * a_alpha));
			}
			for (int i = 0; i < count; ++i) {
				a_drawList->PathLineTo(points[i]);
			}
			a_drawList->PathFillConcave(WithAlpha(gold, a_alpha));

			// The edges by which way they face
			const float stroke = std::max(1.0f, a_gear.pitch * 0.045f);
			for (int i = 0; i < count; ++i) {
				const ImVec2& from = points[i];
				const ImVec2& to = points[(i + 1) % count];
				const float   dx = to.x - from.x;
				const float   dy = to.y - from.y;
				const float   length = std::max(0.01f, std::hypot(dx, dy));
				const float   facing = (dy / length) * -0.6f + (-dx / length) * -0.8f;
				a_drawList->AddLine(from, to, WithAlpha(facing > 0.0f ? Mix(gold, goldLight, facing) : Mix(gold, goldDeep, -facing), a_alpha), stroke);
			}

			// The web sunk inside the rim, its near side in shadow
			const float web = root * 0.8f;
			a_drawList->AddCircleFilled(c, web, WithAlpha(goldDeep, a_alpha), 28);
			a_drawList->PathArcTo(c, web, pi * 0.75f + 0.3f, pi * 1.75f + 0.3f, 16);
			a_drawList->PathStroke(WithAlpha(mortar, 0.6f * a_alpha), ImDrawFlags_None, stroke * 1.4f);
			a_drawList->PathArcTo(c, web, pi * -0.25f + 0.3f, pi * 0.75f + 0.3f, 16);
			a_drawList->PathStroke(WithAlpha(goldLight, 0.55f * a_alpha), ImDrawFlags_None, stroke);

			// Lightening holes and bolts only show on the larger ones
			if (a_gear.pitch > ImGui::GetFontSize() * 0.9f) {
				const int holes = teeth >= 18 ? 6 : 4;
				for (int i = 0; i < holes; ++i) {
					const float turn = angle + static_cast<float>(i) * pi * 2.0f / static_cast<float>(holes);
					a_drawList->AddCircleFilled(ImVec2(c.x + std::cos(turn) * web * 0.6f, c.y + std::sin(turn) * web * 0.6f), web * 0.2f, WithAlpha(mortar, 0.85f * a_alpha), 14);
				}
			}
			const float hub = root * 0.32f;
			a_drawList->AddCircleFilled(c, hub, WithAlpha(gold, a_alpha), 16);
			a_drawList->AddCircle(c, hub, WithAlpha(goldLight, 0.7f * a_alpha), 16, stroke * 0.8f);
			a_drawList->AddCircleFilled(c, hub * 0.42f, WithAlpha(mortar, a_alpha), 10);
			if (a_gear.pitch > ImGui::GetFontSize() * 0.9f) {
				for (int i = 0; i < 4; ++i) {
					const float turn = angle + pi * 0.25f + static_cast<float>(i) * pi * 0.5f;
					a_drawList->AddCircleFilled(ImVec2(c.x + std::cos(turn) * hub * 0.72f, c.y + std::sin(turn) * hub * 0.72f), hub * 0.11f, WithAlpha(goldLight, a_alpha), 6);
				}
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Train ) -------------------------
		//---------------------------------------------------

		// Gears set against each other so their teeth really mesh, each link a tooth count and the way to it
		std::vector<GearSpec> Train(ImVec2 a_start, float a_module, int a_teeth, double a_angle, std::initializer_list<std::pair<int, float>> a_links) {

			std::vector<GearSpec> gears;
			gears.reserve(a_links.size() + 1);
			gears.push_back({ a_start, static_cast<float>(a_teeth) * a_module * 0.5f, a_teeth, a_angle });
			for (const auto& [teeth, way] : a_links) {
				const auto& last = gears.back();
				const float pitch = static_cast<float>(teeth) * a_module * 0.5f;
				const float reach = last.pitch + pitch;
				const double turned = -(last.angle - way) * static_cast<double>(last.teeth) / static_cast<double>(teeth) + way + pi + pi / static_cast<float>(teeth);
				gears.push_back({ ImVec2(last.centre.x + std::cos(way) * reach, last.centre.y + std::sin(way) * reach), pitch, teeth, turned });
			}
			return gears;
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Block Length ) ------------------
		//---------------------------------------------------

		float BlockLength(float a_course, int a_row) {

			return a_course * (2.2f + Hash(a_row, 7) * 0.6f);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Cut ) ---------------------------
		//---------------------------------------------------

		// A stroke chiselled into stone: dark in the groove, its lower lip catching the light
		void Cut(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, float a_width, float a_depth) {

			const float lip = std::max(0.8f, a_width * 0.55f);
			a_drawList->AddLine(ImVec2(a_from.x + lip, a_from.y + lip), ImVec2(a_to.x + lip, a_to.y + lip), WithAlpha(stoneLight, 0.3f * a_depth), std::max(1.0f, a_width * 0.7f));
			a_drawList->AddLine(a_from, a_to, WithAlpha(mortar, 0.85f * a_depth), a_width);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Glyph ) -------------------------
		//---------------------------------------------------

		// One letter of an invented hard-edged script: a stem with squared and slanted arms, sometimes a ring, as if cut by machine
		void Glyph(ImDrawList* a_drawList, ImVec2 a_at, float a_height, Random& a_random, float a_stroke, float a_depth) {

			const float wide = a_height * 0.62f;
			const auto  node = [&](int a_column, int a_row) { return ImVec2(a_at.x + wide * static_cast<float>(a_column) * 0.5f, a_at.y + a_height * static_cast<float>(a_row) / 3.0f); };
			const int   stem = static_cast<int>(a_random.Next() * 3.0f) % 3;
			Cut(a_drawList, node(stem, 0), node(stem, 3), a_stroke, a_depth);
			const int arms = 2 + static_cast<int>(a_random.Next() * 2.0f);
			for (int i = 0; i < arms; ++i) {
				const int row = static_cast<int>(a_random.Next() * 4.0f) % 4;
				const int to = (stem + 1 + static_cast<int>(a_random.Next() * 2.0f)) % 3;
				const int fall = a_random.Next() < 0.35f ? std::clamp(row + (row < 2 ? 1 : -1), 0, 3) : row;
				Cut(a_drawList, node(stem, row), node(to, fall), a_stroke, a_depth);
			}
			if (a_random.Next() < 0.28f) {
				const ImVec2 ring = node((stem + 1) % 3, 1 + static_cast<int>(a_random.Next() * 2.0f) % 2);
				a_drawList->AddCircle(ImVec2(ring.x + a_stroke * 0.6f, ring.y + a_stroke * 0.6f), a_height * 0.13f, WithAlpha(stoneLight, 0.3f * a_depth), 10, std::max(1.0f, a_stroke * 0.7f));
				a_drawList->AddCircle(ring, a_height * 0.13f, WithAlpha(mortar, 0.85f * a_depth), 10, a_stroke);
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Carvings ) ----------------------
		//---------------------------------------------------

		// Lines of script, the odd one in a cartouche, and now and then a carved cog or key spiral, chosen block by block
		void Carvings(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float size = ImGui::GetFontSize();
			const float course = size * 3.2f;
			const float stroke = std::max(1.2f, size * 0.09f);
			Random      random{ 0xD7C1u };
			int         row = 0;
			for (float y = a_min.y; y < a_max.y; y += course, ++row) {
				const float block = BlockLength(course, row);
				for (float x = a_min.x - (row % 2 == 0 ? 0.0f : block * 0.5f); x < a_max.x; x += block) {
					const float  left = std::max(a_min.x, x);
					const float  right = std::min(a_max.x, x + block);
					const float  bottom = std::min(a_max.y, y + course);
					const float  pick = random.Next();
					if (right - left < block * 0.8f || bottom - y < course * 0.9f || pick > 0.42f) {
						continue;
					}
					const ImVec2 middle((left + right) * 0.5f, (y + bottom) * 0.5f);
					const float  depth = random.Range(0.5f, 0.75f);
					if (pick < 0.05f) {
						// A carved cog
						const float radius = course * 0.33f;
						Cog(a_drawList, ImVec2(middle.x + stroke * 0.6f, middle.y + stroke * 0.6f), radius, WithAlpha(stoneLight, 0.3f * depth), false);
						Cog(a_drawList, middle, radius, WithAlpha(mortar, 0.85f * depth), false);
						continue;
					}
					if (pick < 0.09f) {
						// A key spiral, as on the doors
						const float unit = course * 0.3f;
						const ImVec2 turns[] = { ImVec2(-1.0f, -1.0f), ImVec2(1.0f, -1.0f), ImVec2(1.0f, 1.0f), ImVec2(-0.5f, 1.0f), ImVec2(-0.5f, -0.4f), ImVec2(0.45f, -0.4f), ImVec2(0.45f, 0.45f), ImVec2(0.0f, 0.45f) };
						for (std::size_t i = 1; i < std::size(turns); ++i) {
							Cut(a_drawList, ImVec2(middle.x + turns[i - 1].x * unit, middle.y + turns[i - 1].y * unit), ImVec2(middle.x + turns[i].x * unit, middle.y + turns[i].y * unit), stroke * 1.2f, depth);
						}
						continue;
					}
					// A line of script, broken into words, centred in the block
					const float height = course * 0.3f;
					const float step = height * 0.86f;
					const int   letters = std::clamp(static_cast<int>((block * random.Range(0.5f, 0.78f)) / step), 3, 12);
					const float total = step * static_cast<float>(letters);
					const float startX = middle.x - total * 0.5f;
					const float top = middle.y - height * 0.5f;
					const int   gap = 2 + static_cast<int>(random.Next() * 3.0f);
					for (int i = 0; i < letters; ++i) {
						if (i % (gap + 2) != gap) {
							Glyph(a_drawList, ImVec2(startX + step * static_cast<float>(i), top), height, random, stroke * 0.85f, depth);
						}
					}
					if (random.Next() < 0.3f) {
						const float roomX = height * 0.5f;
						const float roomY = height * 0.45f;
						const ImVec2 box[4] = { ImVec2(startX - roomX, top - roomY), ImVec2(startX + total + roomX * 0.4f, top - roomY), ImVec2(startX + total + roomX * 0.4f, top + height + roomY), ImVec2(startX - roomX, top + height + roomY) };
						for (int i = 0; i < 4; ++i) {
							Cut(a_drawList, box[i], box[(i + 1) % 4], stroke * 0.8f, depth * 0.8f);
						}
					} else {
						Cut(a_drawList, ImVec2(startX, top + height * 1.35f), ImVec2(startX + total - step * 0.3f, top + height * 1.35f), stroke * 0.7f, depth * 0.7f);
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Stonework ) ---------------------
		//---------------------------------------------------

		// Backdrop: a wall of big dressed blocks, weathered and cracked, lamplight from the top corners
		void Stonework(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();

			const float reach = std::max(width * 0.3f, size * 10.0f);
			const float shade = std::min(width, height) * 0.2f;
			Mesh(a_drawList, a_min, a_max, size * 0.8f, size * 0.8f, [&](float a_x, float a_y) {
				const float tone = Fbm(a_x / (size * 9.0f), a_y / (size * 6.0f), 3) * 0.7f + Fbm(a_x / (size * 1.3f) + 30.0f, a_y / (size * 1.3f), 2) * 0.3f;
				float lit = 0.0f;
				for (const float side : { 0.12f, 0.88f }) {
					lit = std::max(lit, 1.0f - std::hypot(a_x - (a_min.x + width * side), a_y - a_min.y) / reach);
				}
				const ImU32 rock = Mix(IM_COL32(16, 16, 18, 255), IM_COL32(58, 57, 54, 255), Smooth(std::clamp(tone * 1.5f - 0.25f, 0.0f, 1.0f)));
				const ImU32 warm = Mix(rock, IM_COL32(112, 84, 48, 255), std::clamp(lit, 0.0f, 1.0f) * 0.4f);
				return WithAlpha(warm, 0.46f - EdgeWeight(ImVec2(a_x, a_y), a_min, a_max, shade) * 0.3f);
			});

			// Courses of blocks, every other one set half a block along
			Random      random{ 0xD3E3u };
			const float course = size * 3.2f;
			int         row = 0;
			for (float y = a_min.y; y < a_max.y; y += course, ++row) {
				const float block = BlockLength(course, row);
				for (float x = a_min.x - (row % 2 == 0 ? 0.0f : block * 0.5f); x < a_max.x; x += block) {
					const ImVec2 min(std::max(a_min.x, x), y);
					const ImVec2 max(std::min(a_max.x, x + block), std::min(a_max.y, y + course));
					const float  span = max.x - min.x;
					if (span < 4.0f || max.y - min.y < 4.0f) {
						continue;
					}
					// Each block its own colour, lit on the top and left of its dressed edge, sooted at the foot
					const ImU32 face = WithAlpha(random.Next() < 0.5f ? stoneLight : mortar, random.Range(0.04f, 0.16f));
					const ImU32 foot = WithAlpha(mortar, random.Range(0.1f, 0.3f));
					a_drawList->AddRectFilledMultiColor(ImVec2(min.x + 2.0f, min.y + 2.0f), ImVec2(max.x - 2.0f, max.y - 2.0f), face, face, foot, foot);
					a_drawList->AddLine(ImVec2(min.x + 2.0f, min.y + 2.5f), ImVec2(max.x - 2.0f, min.y + 2.5f), WithAlpha(stoneLight, 0.28f), 1.0f);
					a_drawList->AddLine(ImVec2(min.x + 2.5f, min.y + 2.0f), ImVec2(min.x + 2.5f, max.y - 2.0f), WithAlpha(stoneLight, 0.14f), 1.0f);
					a_drawList->AddLine(ImVec2(min.x + 2.0f, max.y - 2.5f), ImVec2(max.x - 2.0f, max.y - 2.5f), WithAlpha(mortar, 0.5f), 1.0f);
					a_drawList->AddLine(min, ImVec2(max.x, min.y), WithAlpha(mortar, 0.75f), 2.0f);
					if (x > a_min.x) {
						a_drawList->AddLine(min, ImVec2(min.x, max.y), WithAlpha(mortar, 0.75f), 2.0f);
					}
					// Pits, and a crack now and then
					for (int pit = 0; pit < 5; ++pit) {
						const ImVec2 at(min.x + span * random.Range(0.06f, 0.94f), min.y + (max.y - min.y) * random.Range(0.1f, 0.9f));
						const float  radius = random.Range(0.6f, 1.6f);
						a_drawList->AddCircleFilled(at, radius, WithAlpha(mortar, 0.55f), 5);
						a_drawList->AddLine(ImVec2(at.x - radius * 0.5f, at.y + radius), ImVec2(at.x + radius * 0.5f, at.y + radius), WithAlpha(stoneLight, 0.16f), 1.0f);
					}
					if (random.Next() < 0.16f) {
						ImVec2 at(min.x + span * random.Range(0.2f, 0.8f), min.y + 2.0f);
						const float way = random.Next() < 0.5f ? -1.0f : 1.0f;
						for (int bit = 0; bit < 5 && at.y < max.y - 3.0f; ++bit) {
							const ImVec2 next(std::clamp(at.x + way * size * random.Range(-0.15f, 0.55f), min.x + 2.0f, max.x - 2.0f), std::min(max.y - 3.0f, at.y + course * random.Range(0.12f, 0.26f)));
							a_drawList->AddLine(at, next, WithAlpha(mortar, 0.7f), 1.2f);
							a_drawList->AddLine(ImVec2(at.x + 1.0f, at.y), ImVec2(next.x + 1.0f, next.y), WithAlpha(stoneLight, 0.1f), 1.0f);
							at = next;
						}
					}
				}
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 4, 5, 210));
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Gold Strip ) --------------------
		//---------------------------------------------------

		// A strip of worked gold, rounded by the light across it
		void GoldStrip(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			if (a_max.y - a_min.y < a_max.x - a_min.x) {
				a_drawList->AddRectFilledMultiColor(a_min, a_max, goldLight, goldLight, goldDeep, goldDeep);
				a_drawList->AddLine(ImVec2(a_min.x, a_min.y + 0.5f), ImVec2(a_max.x, a_min.y + 0.5f), WithAlpha(goldLight, 0.8f), 1.0f);
			} else {
				a_drawList->AddRectFilledMultiColor(a_min, a_max, goldLight, goldDeep, goldDeep, goldLight);
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
				a_drawList->AddRectFilledMultiColor(at(along + 1.0f, trim + 1.0f), at(end - 1.0f, inner - 1.0f), Shade(face, 1.18f), a_horizontal ? Shade(face, 1.18f) : Shade(face, 0.7f), Shade(face, 0.7f), a_horizontal ? Shade(face, 0.7f) : Shade(face, 1.18f));
				a_drawList->AddLine(at(along + 1.0f, trim + 1.5f), at(end - 1.0f, trim + 1.5f), WithAlpha(stoneLight, 0.55f), 1.0f);
				a_drawList->AddLine(at(along + 1.0f, inner - 1.5f), at(end - 1.0f, inner - 1.5f), WithAlpha(stoneDark, 0.8f), 1.0f);
				// Pits in the face, and a chipped corner now and then
				for (int pit = 0; pit < 4; ++pit) {
					a_drawList->AddCircleFilled(at(along + (end - along) * random.Range(0.12f, 0.88f), trim + (inner - trim) * random.Range(0.25f, 0.75f)), random.Range(0.5f, 1.2f), WithAlpha(mortar, 0.7f), 5);
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
					Rivet(a_drawList, at(x + clamp * 0.5f, across), std::max(1.0f, depth * 0.09f), goldDeep);
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
			a_drawList->AddRectFilledMultiColor(min, max, goldLight, gold, goldDeep, gold);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), goldLight, 1.5f);
			a_drawList->AddLine(ImVec2(min.x + 1.0f, min.y), ImVec2(min.x + 1.0f, max.y), WithAlpha(goldLight, 0.7f), 1.0f);
			const float inset = a_size * 0.2f;
			a_drawList->AddRectFilledMultiColor(ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), mortar, stoneDark, stone, stoneDark);

			// The spiral turns in from the outer corner of the frame
			const auto at = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * a_size * a_sx, a_centre.y + a_y * a_size * a_sy); };
			const ImVec2 spiral[] = { at(-0.6f, -0.6f), at(0.6f, -0.6f), at(0.6f, 0.6f), at(-0.3f, 0.6f), at(-0.3f, -0.25f), at(0.3f, -0.25f), at(0.3f, 0.3f), at(0.0f, 0.3f) };
			a_drawList->AddPolyline(spiral, static_cast<int>(std::size(spiral)), gold, ImDrawFlags_None, std::max(1.5f, a_size * 0.14f));
			for (const auto& bolt : { at(-0.6f, 0.6f), at(-0.88f, -0.88f), at(0.88f, -0.88f), at(0.88f, 0.88f), at(-0.88f, 0.88f) }) {
				Rivet(a_drawList, bolt, std::max(1.0f, a_size * 0.08f), goldDeep);
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
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Hatch ) -------------------------
		//---------------------------------------------------

		// An engraver's shading: slanted strokes kept inside the box, since baked art can't clip
		void Hatch(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_gap, ImU32 a_ink, float a_stroke) {

			const float height = a_max.y - a_min.y;
			for (float x = a_min.x - height + a_gap; x < a_max.x; x += a_gap) {
				const float startX = std::max(a_min.x, x);
				const float endX = std::min(a_max.x, x + height);
				if (endX <= startX) {
					continue;
				}
				a_drawList->AddLine(ImVec2(startX, a_max.y - (startX - x)), ImVec2(endX, a_max.y - (endX - x)), a_ink, a_stroke);
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Etching ) -----------------------
		//---------------------------------------------------

		// An engraving of a Dwemer city: a stepped hall and its spire, a great round tower, a bridge on arches, the pipes between
		void Etching(ImDrawList* a_drawList, float a_left, float a_ground, float a_width, float a_height, ImU32 a_ink) {

			const float stroke = std::max(1.0f, ImGui::GetFontSize() * 0.05f);
			const float gap = std::max(2.5f, a_height * 0.035f);
			const auto  at = [&](float a_u, float a_v) { return ImVec2(a_left + a_u * a_width, a_ground - a_v * a_height); };
			const auto  box = [&](float a_u0, float a_u1, float a_v0, float a_v1) { a_drawList->AddRect(at(a_u0, a_v1), at(a_u1, a_v0), a_ink, 0.0f, 0, stroke); };
			const auto  shaded = [&](float a_u0, float a_u1, float a_v0, float a_v1, float a_from) {
				box(a_u0, a_u1, a_v0, a_v1);
				Hatch(a_drawList, at(a_u0 + (a_u1 - a_u0) * a_from, a_v1), at(a_u1, a_v0), gap, a_ink, stroke * 0.8f);
			};
			const auto  line = [&](float a_u0, float a_v0, float a_u1, float a_v1) { a_drawList->AddLine(at(a_u0, a_v0), at(a_u1, a_v1), a_ink, stroke); };
			const auto  dome = [&](float a_u, float a_v, float a_halfWidth, float a_rise) {
				const ImVec2 from = at(a_u - a_halfWidth, a_v);
				const ImVec2 to = at(a_u + a_halfWidth, a_v);
				const ImVec2 top = at(a_u, a_v + a_rise * 1.33f);
				a_drawList->AddBezierCubic(from, ImVec2(from.x, top.y), ImVec2(to.x, top.y), to, a_ink, stroke, 16);
			};

			line(0.0f, 0.0f, 1.0f, 0.0f);

			// The bridge, its deck and arches
			line(0.0f, 0.3f, 0.34f, 0.3f);
			line(0.0f, 0.26f, 0.34f, 0.26f);
			for (const float u : { 0.015f, 0.115f, 0.215f }) {
				const ImVec2 foot = at(u + 0.012f, 0.0f);
				const ImVec2 other = at(u + 0.088f, 0.0f);
				const ImVec2 spring = at(u + 0.012f, 0.13f);
				const ImVec2 spring2 = at(u + 0.088f, 0.13f);
				const ImVec2 crown = at(u + 0.05f, 0.31f);
				a_drawList->AddLine(foot, spring, a_ink, stroke);
				a_drawList->AddLine(other, spring2, a_ink, stroke);
				a_drawList->AddBezierCubic(spring, ImVec2(spring.x, crown.y), ImVec2(spring2.x, crown.y), spring2, a_ink, stroke, 12);
				Hatch(a_drawList, at(u + 0.088f, 0.26f), at(u + 0.112f, 0.0f), gap, a_ink, stroke * 0.8f);
			}

			// The watchtower on the bridge
			shaded(0.24f, 0.3f, 0.3f, 0.55f, 0.55f);
			line(0.23f, 0.55f, 0.31f, 0.55f);
			dome(0.27f, 0.55f, 0.03f, 0.07f);

			// The stepped hall
			shaded(0.34f, 0.66f, 0.0f, 0.24f, 0.72f);
			shaded(0.38f, 0.62f, 0.24f, 0.44f, 0.72f);
			shaded(0.42f, 0.58f, 0.44f, 0.62f, 0.72f);
			a_drawList->AddRectFilled(at(0.47f, 0.2f), at(0.53f, 0.0f), WithAlpha(a_ink, 0.5f));
			dome(0.5f, 0.2f, 0.03f, 0.07f);
			for (const float u : { 0.4f, 0.44f, 0.56f, 0.6f }) {
				a_drawList->AddRectFilled(at(u - 0.006f, 0.38f), at(u + 0.006f, 0.28f), WithAlpha(a_ink, 0.55f));
			}

			// Its dome and spire, a cog at the top
			dome(0.5f, 0.62f, 0.06f, 0.1f);
			line(0.5f, 0.72f, 0.5f, 0.97f);
			line(0.485f, 0.76f, 0.515f, 0.76f);
			Cog(a_drawList, at(0.5f, 0.86f), a_height * 0.075f, a_ink, false);

			// The round tower, banded, and the pipes that feed it
			shaded(0.72f, 0.81f, 0.0f, 0.68f, 0.62f);
			for (const float v : { 0.14f, 0.28f, 0.42f, 0.56f }) {
				line(0.715f, v, 0.815f, v);
			}
			dome(0.765f, 0.68f, 0.05f, 0.1f);
			line(0.765f, 0.78f, 0.765f, 0.88f);
			for (const float v : { 0.33f, 0.18f }) {
				line(0.66f, v, 0.72f, v);
				line(0.66f, v + 0.05f, 0.72f, v + 0.05f);
				box(0.682f, 0.696f, v - 0.015f, v + 0.065f);
			}

			// Out to a gear wheel on the right
			line(0.81f, 0.5f, 0.92f, 0.5f);
			line(0.81f, 0.455f, 0.885f, 0.455f);
			line(0.92f, 0.5f, 0.92f, 0.34f);
			line(0.885f, 0.455f, 0.885f, 0.34f);
			Cog(a_drawList, at(0.9f, 0.2f), a_height * 0.17f, a_ink, false);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Plating ) -----------------------
		//---------------------------------------------------

		// A panel's face: bolted plates of old dark metal, brushed and tarnished in patches, set back into the wall
		void Plating(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const float band = std::min(width, height) * 0.25f + size * 2.0f;
			const float seed = width * 0.013f + height * 0.029f;

			Mesh(a_drawList, a_min, a_max, size * 0.9f, size * 0.9f, [&](float a_x, float a_y) {
				const float patch = Fbm(a_x / (size * 8.0f) + seed, a_y / (size * 5.0f), 3);
				const float streak = Fbm(a_x / (size * 14.0f) + 9.0f, a_y / (size * 0.9f) + seed, 2);
				const ImU32 metal = Mix(IM_COL32(14, 14, 15, 255), IM_COL32(38, 36, 32, 255), Smooth(std::clamp(patch * 1.5f - 0.3f, 0.0f, 1.0f)) * 0.75f + streak * 0.25f);
				return WithAlpha(metal, 0.46f + EdgeWeight(ImVec2(a_x, a_y), a_min, a_max, band) * 0.2f);
			});

			// The brushing, long fine strokes all one way
			Random    random{ static_cast<std::uint32_t>(width) * 41u + static_cast<std::uint32_t>(height) };
			const int strokes = std::clamp(static_cast<int>(width * height / (size * size * 1.4f)), 30, 900);
			for (int i = 0; i < strokes; ++i) {
				const float  y = a_min.y + height * random.Next();
				const float  x = a_min.x + width * random.Next();
				const float  end = std::min(a_max.x, x + size * random.Range(1.5f, 7.0f));
				a_drawList->AddLine(ImVec2(x, y), ImVec2(end, y), WithAlpha(random.Next() < 0.55f ? IM_COL32(150, 144, 130, 255) : mortar, random.Range(0.02f, 0.065f)), 1.0f);
			}

			// Plates lapped like courses of brick, a rivet at each join
			const float tall = size * 7.0f;
			const float wide = size * 13.0f;
			const float bolt = std::max(1.4f, size * 0.12f);
			int         row = 0;
			for (float y = a_min.y + tall; y < a_max.y - size; y += tall, ++row) {
				a_drawList->AddLine(ImVec2(a_min.x, y), ImVec2(a_max.x, y), WithAlpha(mortar, 0.6f), 1.5f);
				a_drawList->AddLine(ImVec2(a_min.x, y + 1.5f), ImVec2(a_max.x, y + 1.5f), WithAlpha(IM_COL32(150, 144, 130, 255), 0.09f), 1.0f);
			}
			row = 0;
			for (float y = a_min.y; y < a_max.y - size; y += tall, ++row) {
				const float bottom = std::min(a_max.y, y + tall);
				for (float x = a_min.x + (row % 2 == 0 ? wide : wide * 0.5f); x < a_max.x - size; x += wide) {
					a_drawList->AddLine(ImVec2(x, y), ImVec2(x, bottom), WithAlpha(mortar, 0.6f), 1.5f);
					a_drawList->AddLine(ImVec2(x + 1.5f, y), ImVec2(x + 1.5f, bottom), WithAlpha(IM_COL32(150, 144, 130, 255), 0.07f), 1.0f);
					for (const float side : { -1.0f, 1.0f }) {
						for (const float down : { y + size * 0.7f, bottom - size * 0.7f }) {
							if (down < a_max.y - size * 0.5f) {
								Rivet(a_drawList, ImVec2(x + side * size * 0.6f, down), bolt, Mix(goldDeep, stoneDark, 0.7f), 0.36f);
							}
						}
					}
				}
			}

			// Set back: a shadow under the top and left edges, a catch of light along the bottom
			const float deep = std::min(size * 0.9f, height * 0.3f);
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + deep), WithAlpha(mortar, 0.6f), WithAlpha(mortar, 0.6f), WithAlpha(mortar, 0.0f), WithAlpha(mortar, 0.0f));
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + deep * 0.7f, a_max.y), WithAlpha(mortar, 0.45f), WithAlpha(mortar, 0.0f), WithAlpha(mortar, 0.0f), WithAlpha(mortar, 0.45f));
			a_drawList->AddLine(ImVec2(a_min.x, a_max.y - 1.0f), ImVec2(a_max.x, a_max.y - 1.0f), WithAlpha(stoneLight, 0.25f), 1.0f);
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Steam ) -------------------------
		//---------------------------------------------------

		// Puffs from a vent, thinning as they rise
		void Steam(ImDrawList* a_drawList, ImVec2 a_vent, float a_side, float a_size, double a_now, float a_strength = 0.035f) {

			constexpr int puffs = 4;
			for (int i = 0; i < puffs; ++i) {
				const double cycle = a_now / 4.5 + static_cast<double>(i) / puffs;
				const float  life = static_cast<float>(cycle - std::floor(cycle));
				const float  drift = std::sin(life * 5.0f + static_cast<float>(i) * 1.9f) * a_size * 0.25f;
				SoftSpot(a_drawList, ImVec2(a_vent.x + a_side * a_size * life * 0.7f + drift, a_vent.y - a_size * life * 3.2f), a_size * (0.25f + life * 0.8f), WithAlpha(IM_COL32(204, 202, 192, 255), a_strength * std::sin(life * pi)));
			}
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Mist ) --------------------------
		//---------------------------------------------------

		// Slow rags of mist, always under the text; a_shape says how thick it may get at a point, 0 to 1
		template <class Shape>
		void Mist(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_seed, double a_now, float a_strength, Shape&& a_shape) {

			const float size = ImGui::GetFontSize();
			if (a_max.x - a_min.x < size || a_max.y - a_min.y < size) {
				return;
			}
			const float drift = static_cast<float>(std::fmod(a_now, 7200.0));
			Mesh(a_drawList, a_min, a_max, size * 1.1f, size * 0.9f, [&](float a_x, float a_y) {
				const float rags = Fbm(a_x / (size * 10.0f) - drift * 0.09f + a_seed, a_y / (size * 3.5f) + drift * 0.035f, 3);
				const float body = Smooth(std::clamp((rags - 0.36f) * 3.0f, 0.0f, 1.0f));
				return WithAlpha(IM_COL32(196, 197, 192, 255), a_strength * body * a_shape(a_x, a_y));
			});
		}

		//---------------------------------------------------
		//-- Dwemer Decor ( Piston ) ------------------------
		//---------------------------------------------------

		// Let into a side of the frame: a brass cylinder whose rod works up and down its slot
		void PistonSlot(ImDrawList* a_drawList, ImVec2 a_centre, float a_band) {

			const float half = a_band * 0.36f;
			const float length = a_band * 3.0f;
			a_drawList->AddRectFilledMultiColor(ImVec2(a_centre.x - half, a_centre.y - length), ImVec2(a_centre.x + half, a_centre.y + length), mortar, stoneDark, stoneDark, mortar);
			a_drawList->AddRect(ImVec2(a_centre.x - half, a_centre.y - length), ImVec2(a_centre.x + half, a_centre.y + length), goldDeep, 0.0f, 0, 1.0f);
			GoldStrip(a_drawList, ImVec2(a_centre.x - half, a_centre.y - length), ImVec2(a_centre.x + half, a_centre.y - length * 0.45f));
			for (const float y : { a_centre.y - length * 0.92f, a_centre.y - length * 0.72f, a_centre.y - length * 0.52f }) {
				a_drawList->AddLine(ImVec2(a_centre.x - half, y), ImVec2(a_centre.x + half, y), goldDeep, 1.0f);
			}
		}

		void PistonRod(ImDrawList* a_drawList, ImVec2 a_centre, float a_band, double a_now) {

			const float half = a_band * 0.36f;
			const float length = a_band * 3.0f;
			const float stroke = (std::sin(static_cast<float>(std::fmod(a_now * 1.1, static_cast<double>(pi) * 2.0))) + 1.0f) * 0.5f;
			const float head = a_centre.y - length * 0.2f + stroke * length * 0.95f;
			a_drawList->AddRectFilledMultiColor(ImVec2(a_centre.x - half * 0.3f, a_centre.y - length * 0.45f), ImVec2(a_centre.x + half * 0.3f, head), goldLight, goldDeep, goldDeep, goldLight);
			a_drawList->AddRectFilledMultiColor(ImVec2(a_centre.x - half * 0.86f, head), ImVec2(a_centre.x + half * 0.86f, head + a_band * 0.34f), goldLight, goldDeep, goldDeep, goldLight);
			a_drawList->AddLine(ImVec2(a_centre.x - half * 0.86f, head + a_band * 0.34f), ImVec2(a_centre.x + half * 0.86f, head + a_band * 0.34f), mortar, 1.0f);
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
	void Cog(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled, float a_angle) {

		constexpr int teeth = 8;
		const float   step = pi * 2.0f / teeth;
		const float   root = a_radius * 0.76f;
		ImVec2        points[teeth * 4];
		for (int tooth = 0; tooth < teeth; ++tooth) {
			const float base = static_cast<float>(tooth) * step - pi * 0.5f + a_angle;
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
	//-- Dwemer Decor ( Ornament ) ----------------------
	//---------------------------------------------------

	// A small cog that keeps turning, neighbours going opposite ways
	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const bool  backwards = (static_cast<int>(std::abs(a_centre.x) / std::max(1.0f, a_radius * 6.0f)) & 1) != 0;
		const float turn = static_cast<float>(std::fmod(ImGui::GetTime() * 0.45, static_cast<double>(pi) * 2.0));
		Cog(a_drawList, a_centre, a_radius, a_colour, true, backwards ? -turn : turn);
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
		Ornament(a_drawList, ImVec2(midX, a_from.y), gap * 0.4f, a_colour);
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Rough brick bound in gold, key-cut plates on the corners, a crest set in the top and bottom beams,
	// and the works let into them: a cog turning in each crest, a piston in each side, steam and mist hanging low
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);
		const bool  full = compact >= 0.8f;
		const float midX = (a_min.x + a_max.x) * 0.5f;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const float topY = a_min.y + band * 0.5f;
		const float footY = a_max.y - band * 0.5f;
		const float crestTop = band * 2.6f;
		const float crestFoot = band * 1.8f;
		const ImVec2 pistons[2] = { ImVec2(a_min.x + band * 0.5f, midY), ImVec2(a_max.x - band * 0.5f, midY) };

		const bool  sideWorks = full && height > size * 16.0f;

		Baked(a_drawList, 0xD100u, a_min, a_max, [&](ImDrawList* a_list) { Stonework(a_list, a_min, a_max); });
		Baked(a_drawList, 0xD102u, a_min, a_max, [&](ImDrawList* a_list) { Carvings(a_list, a_min, a_max); });
		Baked(a_drawList, 0xD101u, a_min, a_max, [&](ImDrawList* a_list) {
			BrickBand(a_list, a_min, ImVec2(a_max.x, a_min.y + band), true, 0xD401u);
			BrickBand(a_list, ImVec2(a_min.x, a_max.y - band), a_max, true, 0xD402u);
			BrickBand(a_list, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0xD403u);
			BrickBand(a_list, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0xD404u);
			a_list->AddRect(ImVec2(a_min.x + band + 2.0f, a_min.y + band + 2.0f), ImVec2(a_max.x - band - 2.0f, a_max.y - band - 2.0f), WithAlpha(gold, 0.22f), 0.0f, 0, 1.0f);

			if (sideWorks) {
				for (const auto& piston : pistons) {
					PistonSlot(a_list, piston, band);
				}
			}

			const float corner = band * 0.95f;
			KeyPlate(a_list, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner, 1.0f, 1.0f);
			KeyPlate(a_list, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner, -1.0f, 1.0f);
			KeyPlate(a_list, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner, -1.0f, -1.0f);
			KeyPlate(a_list, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner, 1.0f, -1.0f);

			// Only on the main window; small panels keep plain beams
			if (full) {
				Crest(a_list, ImVec2(midX, topY), crestTop, band * 1.3f);
				Crest(a_list, ImVec2(midX, footY), crestFoot, band * 0.9f);
			}
		});

		// What moves goes over the top, every frame
		const double now = ImGui::GetTime();
		if (full) {
			Gear(a_drawList, { ImVec2(midX, topY), band * 0.42f, 10, now * 0.5 }, 1.0f, false);
			Gear(a_drawList, { ImVec2(midX, footY), band * 0.29f, 8, -now * 0.5 }, 1.0f, false);
		}
		if (sideWorks) {
			for (const auto& piston : pistons) {
				PistonRod(a_drawList, piston, band, now);
			}
			Steam(a_drawList, ImVec2(pistons[0].x + band * 0.5f, midY - band * 3.0f), 1.0f, size, now);
			Steam(a_drawList, ImVec2(pistons[1].x - band * 0.5f, midY - band * 3.0f), -1.0f, size, now + 1.7);
		}
		if (full) {
			// Lying along the floor, and climbing the walls either side
			const ImVec2 inner(a_min.x + band, a_min.y + band);
			const ImVec2 outer(a_max.x - band, a_max.y - band);
			const float  floor = std::min(height * 0.26f, size * 7.0f);
			Mist(a_drawList, ImVec2(inner.x, outer.y - floor), outer, 3.0f, now, 0.15f, [&](float, float a_y) {
				const float rise = (a_y - (outer.y - floor)) / floor;
				return rise * rise;
			});
			const float wall = size * 3.0f;
			for (const float side : { 0.0f, 1.0f }) {
				const float x0 = side == 0.0f ? inner.x : outer.x - wall;
				Mist(a_drawList, ImVec2(x0, inner.y), ImVec2(x0 + wall, outer.y), 17.0f + side * 11.0f, now, 0.1f, [&](float a_x, float) {
					const float out = side == 0.0f ? (a_x - x0) / wall : 1.0f - (a_x - x0) / wall;
					return (1.0f - out) * (1.0f - out);
				});
			}
		}
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: old metal plating, lamplight from above, and in the empty corner an engraving of a Dwemer city with a train of gears at work beside it
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}

		const float pitch = std::min(height * 0.34f, width * 0.12f);
		const bool  works = a_emblem && pitch > size * 1.5f;
		const float module = pitch * 2.0f / 24.0f;
		const ImVec2 hub(a_max.x - pitch * 1.25f, a_max.y - pitch * 1.0f);

		// The engraving stops short of the gears; its tower gets a wisp of steam below
		const float room = hub.x - pitch * 2.35f - a_min.x - size * 0.6f;
		const float wide = std::min(room, std::min(height * 0.72f, size * 8.5f) * 3.2f);
		const float tall = std::min(std::min(height * 0.72f, size * 8.5f), wide / 2.6f);
		const float left = a_min.x + room - wide + size * 0.6f;
		const float ground = a_max.y - size * 0.45f;

		Baked(a_drawList, a_emblem ? 0xD201u : 0xD200u, a_min, a_max, [&](ImDrawList* a_list) {
			Plating(a_list, a_min, a_max);
			const float glow = std::min(height * 0.3f, size * 5.0f);
			a_list->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(lamp, 0.022f), WithAlpha(lamp, 0.022f), WithAlpha(lamp, 0.0f), WithAlpha(lamp, 0.0f));

			if (works && wide > size * 14.0f) {
				Etching(a_list, left, ground, wide, tall, WithAlpha(goldLight, 0.17f));
			}
		});

		const double now = ImGui::GetTime();
		Mist(a_drawList, ImVec2(a_min.x, a_max.y - std::min(height * 0.4f, size * 6.0f)), a_max, width * 0.011f, now, 0.1f, [&](float, float a_y) {
			const float rise = 1.0f - (a_max.y - a_y) / std::min(height * 0.4f, size * 6.0f);
			return rise * rise;
		});

		// A train of gears, turning slowly
		if (works) {
			if (wide > size * 14.0f) {
				Steam(a_drawList, ImVec2(left + wide * 0.765f, ground - tall * 0.9f), 1.0f, size * 0.8f, now, 0.03f);
			}
			const float up = -pi * 0.73f;
			const float across = pi * 0.96f;
			for (const auto& gear : Train(hub, module, 24, now * 0.14, { { 12, up }, { 16, across } })) {
				Gear(a_drawList, gear, 0.2f, false);
			}
			for (const auto& gear : Train(hub, module, 24, now * 0.14, { { 9, -pi * 0.42f } })) {
				if (gear.teeth == 9) {
					Gear(a_drawList, gear, 0.2f, false);
				}
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
				Rivet(a_drawList, bolt, thick * 0.32f, goldDeep);
			}
		}
	}
}

namespace CompletionistUI::DwemerDecor
{
	//---------------------------------------------------
	//-- Dwemer Decor ( Toast Plate ) -------------------
	//---------------------------------------------------

	// A bolted slip of dark bronze with a gold edge, a cog turning in its corner
	void ToastPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha, float a_opacity) {

		const float size = ImGui::GetFontSize();
		const float fill = a_alpha * std::max(a_opacity, 0.6f);
		a_drawList->AddRectFilledMultiColor(a_min, a_max, WithAlpha(IM_COL32(46, 40, 30, 255), fill), WithAlpha(IM_COL32(32, 30, 28, 255), fill), WithAlpha(IM_COL32(16, 16, 17, 255), fill), WithAlpha(IM_COL32(24, 22, 19, 255), fill));
		a_drawList->AddRect(a_min, a_max, WithAlpha(goldDeep, a_alpha), 0.0f, 0, 1.0f);
		const float pen = std::max(2.0f, size * 0.16f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + pen, a_max.y), WithAlpha(goldLight, a_alpha), WithAlpha(goldDeep, a_alpha), WithAlpha(goldDeep, a_alpha), WithAlpha(goldLight, a_alpha));
		const float bolt = std::max(1.2f, size * 0.1f);
		const float in = size * 0.28f;
		for (const auto& at : { ImVec2(a_min.x + pen + in, a_min.y + in), ImVec2(a_min.x + pen + in, a_max.y - in), ImVec2(a_max.x - in, a_max.y - in) }) {
			Rivet(a_drawList, at, bolt, goldDeep, a_alpha);
		}
		Cog(a_drawList, ImVec2(a_max.x - size * 0.55f, a_min.y + size * 0.55f), size * 0.36f, WithAlpha(gold, a_alpha), true, static_cast<float>(std::fmod(ImGui::GetTime() * 0.9, static_cast<double>(pi) * 2.0)));
	}

	//---------------------------------------------------
	//-- Dwemer Decor ( Perk Node ) ---------------------
	//---------------------------------------------------

	// Cogs: cast in gold when taken, an empty cog when next, a bare hex nut when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Cog(a_drawList, a_centre, a_radius * 1.05f, a_colour, true, static_cast<float>(std::fmod(ImGui::GetTime() * 0.4, static_cast<double>(pi) * 2.0)));
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

		// Pressure going along it
		const float first = std::abs(middle - a_from.x);
		const float second = std::abs(a_to.y - a_from.y);
		const float third = std::abs(a_to.x - middle);
		const float total = first + second + third;
		if (total > bore * 4.0f) {
			const double cycle = ImGui::GetTime() * 0.4 + static_cast<double>(a_from.y) * 0.007 + static_cast<double>(a_to.y) * 0.003;
			const float  head = static_cast<float>(cycle - std::floor(cycle)) * total;
			const auto   along = [&](float a_distance) {
				if (a_distance < first) {
					return ImVec2(a_from.x + (middle - a_from.x) * a_distance / std::max(first, 0.01f), a_from.y);
				}
				if (a_distance < first + second) {
					return ImVec2(middle, a_from.y + (a_to.y - a_from.y) * (a_distance - first) / std::max(second, 0.01f));
				}
				return ImVec2(middle + (a_to.x - middle) * (a_distance - first - second) / std::max(third, 0.01f), a_to.y);
			};
			for (int i = 0; i < 6; ++i) {
				const float distance = head - static_cast<float>(i) * bore * 0.7f;
				if (distance > 0.0f) {
					a_drawList->AddCircleFilled(along(distance), bore * 0.42f, WithAlpha(IM_COL32(255, 238, 190, 255), 0.55f * (1.0f - static_cast<float>(i) / 6.0f)), 8);
				}
			}
		}
		if (std::abs(a_to.y - a_from.y) > bore * 3.0f) {
			for (const auto& bend : { points[1], points[2] }) {
				a_drawList->AddRectFilled(ImVec2(bend.x - bore * 0.9f, bend.y - bore * 0.9f), ImVec2(bend.x + bore * 0.9f, bend.y + bore * 0.9f), a_colour);
				a_drawList->AddRect(ImVec2(bend.x - bore * 0.9f, bend.y - bore * 0.9f), ImVec2(bend.x + bore * 0.9f, bend.y + bore * 0.9f), WithAlpha(IM_COL32(255, 238, 190, 255), 0.4f), 0.0f, 0, 1.0f);
			}
		}
	}
}
