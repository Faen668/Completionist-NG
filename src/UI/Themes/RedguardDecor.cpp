#include "UI/Themes/RedguardDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::RedguardDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Sand-dyed and madder-red cloth, indigo stripes, tent poles, rope, brass and turquoise
		constexpr ImU32 sand = IM_COL32(194, 158, 110, 255);
		constexpr ImU32 sandLight = IM_COL32(234, 206, 158, 255);
		constexpr ImU32 cloth = IM_COL32(166, 72, 44, 255);
		constexpr ImU32 clothDeep = IM_COL32(104, 42, 26, 255);
		constexpr ImU32 indigo = IM_COL32(46, 54, 100, 255);
		constexpr ImU32 wood = IM_COL32(112, 74, 42, 255);
		constexpr ImU32 woodLight = IM_COL32(170, 120, 72, 255);
		constexpr ImU32 woodDark = IM_COL32(52, 33, 20, 255);
		constexpr ImU32 rope = IM_COL32(182, 152, 102, 255);
		constexpr ImU32 brass = IM_COL32(202, 158, 74, 255);
		constexpr ImU32 brassLight = IM_COL32(246, 214, 144, 255);
		constexpr ImU32 turquoise = IM_COL32(58, 164, 158, 255);
		constexpr ImU32 turquoiseLight = IM_COL32(132, 216, 206, 255);
		constexpr ImU32 shadow = IM_COL32(32, 21, 15, 255);
		constexpr ImU32 lamp = IM_COL32(255, 190, 110, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Fade At ) ---------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Sword ) -----------------------
		//---------------------------------------------------

		// A curved Yokudan sword, hilt at a_hilt, the blade sweeping up and out; a_facing is -1 or 1
		void Sword(ImDrawList* a_drawList, ImVec2 a_hilt, float a_length, float a_facing, ImU32 a_colour) {

			const ImVec2 p0 = a_hilt;
			const ImVec2 p1(a_hilt.x + a_facing * a_length * 0.15f, a_hilt.y - a_length * 0.6f);
			const ImVec2 p2(a_hilt.x + a_facing * a_length * 0.55f, a_hilt.y - a_length * 0.92f);
			constexpr int steps = 14;
			std::vector<ImVec2> spine;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / steps;
				const float u = 1.0f - t;
				spine.emplace_back(u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x, u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y);
			}
			// The blade widens a little towards the point, then cuts back to it
			const float width = a_length * 0.07f;
			std::vector<ImVec2> outline;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / steps;
				const float wide = t < 0.85f ? width * (0.8f + t * 0.4f) : width * 1.14f * (1.0f - t) / 0.15f;
				const auto  normal = NormalAt(spine, static_cast<std::size_t>(i));
				outline.emplace_back(spine[i].x + normal.x * wide * a_facing, spine[i].y + normal.y * wide * a_facing);
			}
			for (int i = steps; i >= 0; --i) {
				outline.push_back(spine[i]);
			}
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(a_colour);
			// Guard, grip and pommel
			const auto   normal = NormalAt(spine, 0);
			const ImVec2 along(-normal.y, normal.x);
			a_drawList->AddLine(ImVec2(a_hilt.x - normal.x * width * 2.0f, a_hilt.y - normal.y * width * 2.0f), ImVec2(a_hilt.x + normal.x * width * 2.0f, a_hilt.y + normal.y * width * 2.0f), a_colour, std::max(1.5f, width * 0.6f));
			const ImVec2 pommel(a_hilt.x - along.x * a_length * 0.18f, a_hilt.y - along.y * a_length * 0.18f);
			a_drawList->AddLine(a_hilt, pommel, a_colour, std::max(1.5f, width * 0.8f));
			a_drawList->AddCircleFilled(pommel, std::max(1.5f, width * 0.7f), a_colour, 10);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Tent Cloth ) ------------------
		//---------------------------------------------------

		// Backdrop: the inside of a tent, its cloth hanging in soft vertical folds, a lamp burning low in the middle
		void TentCloth(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(56, 30, 20, 70), IM_COL32(56, 30, 20, 70), IM_COL32(20, 11, 8, 70), IM_COL32(20, 11, 8, 70) });

			// Each fold lit on one side and in shadow on the other
			Random      random{ 0x5E47u };
			float       x = a_min.x;
			const ImU32 dark = IM_COL32(0, 0, 0, 255);
			while (x < a_max.x) {
				const float fold = size * random.Range(2.5f, 4.5f);
				const float middle = x + fold * 0.45f;
				const float depth = random.Range(0.08f, 0.16f);
				a_drawList->AddRectFilledMultiColor(ImVec2(x, a_min.y), ImVec2(middle, a_max.y), WithAlpha(dark, depth), WithAlpha(sandLight, 0.025f), WithAlpha(sandLight, 0.025f), WithAlpha(dark, depth));
				a_drawList->AddRectFilledMultiColor(ImVec2(middle, a_min.y), ImVec2(x + fold, a_max.y), WithAlpha(sandLight, 0.025f), WithAlpha(dark, depth), WithAlpha(dark, depth), WithAlpha(sandLight, 0.025f));
				x += fold;
			}
			// Where the cloth is sewn, a faint indigo stripe
			for (float stripe = a_min.x + size * 12.0f; stripe < a_max.x; stripe += size * 12.0f) {
				a_drawList->AddRectFilled(ImVec2(stripe, a_min.y), ImVec2(stripe + size * 0.3f, a_max.y), WithAlpha(indigo, 0.12f));
			}

			// The lamp, out of sight below
			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_max.y + size * 2.0f), std::max(width * 0.4f, size * 10.0f), WithAlpha(lamp, 0.014f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(6, 3, 2, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Tassel ) ----------------------
		//---------------------------------------------------

		// A brass bead with threads hanging from it
		void Tassel(ImDrawList* a_drawList, ImVec2 a_top, float a_length, ImU32 a_colour) {

			a_drawList->AddCircleFilled(a_top, a_length * 0.18f, brass, 8);
			const ImVec2 neck(a_top.x, a_top.y + a_length * 0.25f);
			const float  spread = a_length * 0.22f;
			const ImVec2 points[4] = { ImVec2(neck.x - spread * 0.4f, neck.y), ImVec2(neck.x + spread * 0.4f, neck.y), ImVec2(neck.x + spread, a_top.y + a_length), ImVec2(neck.x - spread, a_top.y + a_length) };
			a_drawList->AddConvexPolyFilled(points, 4, a_colour);
			a_drawList->AddLine(ImVec2(neck.x - spread * 0.45f, neck.y + 1.0f), ImVec2(neck.x + spread * 0.45f, neck.y + 1.0f), brass, 1.5f);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Canopy ) ----------------------
		//---------------------------------------------------

		// Across the top: a striped valance, scalloped along its lower edge, a tassel where each scallop meets the next
		void Canopy(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float depth = a_max.y - a_min.y;
			const float body = depth * 0.62f;
			// The stripes: madder red, a sand band with an indigo line through it, red again
			a_drawList->AddRectFilled(a_min, ImVec2(a_max.x, a_min.y + body), cloth);
			const float sandTop = a_min.y + body * 0.3f;
			const float sandBottom = a_min.y + body * 0.72f;
			a_drawList->AddRectFilled(ImVec2(a_min.x, sandTop), ImVec2(a_max.x, sandBottom), sand);
			a_drawList->AddRectFilled(ImVec2(a_min.x, (sandTop + sandBottom) * 0.5f - 1.0f), ImVec2(a_max.x, (sandTop + sandBottom) * 0.5f + 1.0f), indigo);
			a_drawList->AddLine(ImVec2(a_min.x, a_min.y + 0.5f), ImVec2(a_max.x, a_min.y + 0.5f), WithAlpha(sandLight, 0.5f), 1.0f);

			// The scallops
			const float scallop = depth * 2.2f;
			const int   count = std::max(1, static_cast<int>(width / scallop));
			const float span = width / static_cast<float>(count);
			const float drop = depth - body;
			const float base = a_min.y + body;
			for (int i = 0; i < count; ++i) {
				const float left = a_min.x + span * static_cast<float>(i);
				a_drawList->PathLineTo(ImVec2(left, base - 0.5f));
				for (int s = 0; s <= 10; ++s) {
					const float t = static_cast<float>(s) / 10.0f;
					a_drawList->PathLineTo(ImVec2(left + span * t, base + std::sin(t * pi) * drop));
				}
				a_drawList->PathFillConvex(cloth);
				for (int s = 0; s < 10; ++s) {
					const float t0 = static_cast<float>(s) / 10.0f;
					const float t1 = static_cast<float>(s + 1) / 10.0f;
					a_drawList->AddLine(ImVec2(left + span * t0, base + std::sin(t0 * pi) * drop), ImVec2(left + span * t1, base + std::sin(t1 * pi) * drop), clothDeep, 1.5f);
				}
				if (i > 0) {
					Tassel(a_drawList, ImVec2(left, base), depth * 0.75f, turquoise);
				}
			}
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Pole ) ------------------------
		//---------------------------------------------------

		// A round tent pole, lit down its middle, lashed with rope near each end
		void Pole(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float middle = a_min.x + width * 0.42f;
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(middle, a_max.y), woodDark, woodLight, woodLight, woodDark);
			a_drawList->AddRectFilledMultiColor(ImVec2(middle, a_min.y), a_max, woodLight, woodDark, woodDark, woodLight);
			// Grain
			for (const float t : { 0.25f, 0.62f }) {
				a_drawList->AddLine(ImVec2(a_min.x + width * t, a_min.y), ImVec2(a_min.x + width * t, a_max.y), WithAlpha(woodDark, 0.4f), 1.0f);
			}
			// Rope wound round it near each end
			const float wrap = width * 0.45f;
			for (const float from : { a_min.y + width * 0.6f, a_max.y - width * 0.6f - wrap * 5.0f }) {
				for (int turn = 0; turn < 5; ++turn) {
					const float y = from + wrap * static_cast<float>(turn);
					a_drawList->AddLine(ImVec2(a_min.x - 1.0f, y + wrap * 0.8f), ImVec2(a_max.x + 1.0f, y), rope, std::max(1.5f, wrap * 0.7f));
					a_drawList->AddLine(ImVec2(a_min.x - 1.0f, y + wrap * 0.8f + wrap * 0.35f), ImVec2(a_max.x + 1.0f, y + wrap * 0.35f), WithAlpha(woodDark, 0.6f), 1.0f);
				}
			}
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Finial ) ----------------------
		//---------------------------------------------------

		// The brass onion dome that caps a tent pole
		void Finial(ImDrawList* a_drawList, ImVec2 a_base, float a_size) {

			std::vector<ImVec2> outline;
			constexpr int steps = 16;
			for (int side = 0; side < 2; ++side) {
				for (int i = 0; i <= steps; ++i) {
					const float t = side == 0 ? static_cast<float>(i) / steps : 1.0f - static_cast<float>(i) / steps;
					// Swelling out from the neck, then drawing in to the point
					const float bulge = std::sin(std::min(1.0f, t * 1.25f) * pi) * (1.0f - t * 0.35f);
					const float x = (side == 0 ? -1.0f : 1.0f) * a_size * 0.5f * (0.25f + bulge * 0.75f) * (1.0f - std::pow(t, 6.0f));
					outline.emplace_back(a_base.x + x, a_base.y - t * a_size * 1.4f);
				}
			}
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(brass);
			a_drawList->AddCircleFilled(ImVec2(a_base.x - a_size * 0.12f, a_base.y - a_size * 0.55f), a_size * 0.12f, WithAlpha(brassLight, 0.8f), 10);
			a_drawList->AddRectFilled(ImVec2(a_base.x - a_size * 0.3f, a_base.y - a_size * 0.08f), ImVec2(a_base.x + a_size * 0.3f, a_base.y + a_size * 0.06f), brassLight);
			a_drawList->AddCircleFilled(ImVec2(a_base.x, a_base.y - a_size * 1.45f), a_size * 0.08f, turquoise, 8);
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Kilim ) -----------------------
		//---------------------------------------------------

		// Along the foot: a woven kilim, stepped red and indigo diamonds on sand, a turquoise eye in each
		void Kilim(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float depth = a_max.y - a_min.y;
			a_drawList->AddRectFilled(a_min, a_max, sand);
			a_drawList->AddRectFilled(a_min, ImVec2(a_max.x, a_min.y + depth * 0.12f), clothDeep);
			a_drawList->AddRectFilled(ImVec2(a_min.x, a_max.y - depth * 0.12f), a_max, clothDeep);
			const float cell = depth * 1.5f;
			const int   count = std::max(1, static_cast<int>(width / cell));
			const float span = width / static_cast<float>(count);
			const float middle = (a_min.y + a_max.y) * 0.5f;
			const float half = depth * 0.34f;
			for (int i = 0; i < count; ++i) {
				const float  x = a_min.x + span * (static_cast<float>(i) + 0.5f);
				const ImU32  colour = i % 2 == 0 ? cloth : indigo;
				// A diamond with stepped edges, as woven
				const float  step = half / 3.0f;
				ImVec2       points[12];
				int          n = 0;
				for (int k = 0; k < 3; ++k) {
					points[n++] = ImVec2(x + step * static_cast<float>(k), middle - half + step * static_cast<float>(k));
				}
				for (int k = 0; k < 3; ++k) {
					points[n++] = ImVec2(x + half - step * static_cast<float>(k), middle + step * static_cast<float>(k));
				}
				for (int k = 0; k < 3; ++k) {
					points[n++] = ImVec2(x - step * static_cast<float>(k), middle + half - step * static_cast<float>(k));
				}
				for (int k = 0; k < 3; ++k) {
					points[n++] = ImVec2(x - half + step * static_cast<float>(k), middle - step * static_cast<float>(k));
				}
				for (const auto& point : points) {
					a_drawList->PathLineTo(point);
				}
				a_drawList->PathFillConvex(colour);
				Tile(a_drawList, ImVec2(x, middle), half * 0.32f, turquoise, true);
			}
		}

		//---------------------------------------------------
		//-- Redguard Decor ( Fringe ) ----------------------
		//---------------------------------------------------

		// The rug's fringe, threads hanging below its edge
		void Fringe(ImDrawList* a_drawList, ImVec2 a_from, float a_width, float a_length) {

			Random random{ static_cast<std::uint32_t>(a_width) * 13u + 7u };
			for (float x = a_from.x + 2.0f; x < a_from.x + a_width - 1.0f; x += 3.0f) {
				const float hang = a_length * random.Range(0.75f, 1.0f);
				a_drawList->AddLine(ImVec2(x, a_from.y), ImVec2(x + random.Range(-1.0f, 1.0f), a_from.y + hang), WithAlpha(sandLight, random.Range(0.6f, 0.9f)), 1.0f);
			}
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Tile ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: a glazed tile, lit on its upper faces
	void Tile(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

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
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(IM_COL32(255, 255, 240, 255), 0.3f * Alpha(a_colour)));
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Star ) ------------------------
	//---------------------------------------------------

	// The eight-pointed star of Yokudan tilework: two squares turned across each other
	void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		ImVec2 squares[2][4];
		for (int square = 0; square < 2; ++square) {
			for (int i = 0; i < 4; ++i) {
				const float angle = static_cast<float>(i) * pi * 0.5f + (square == 0 ? 0.0f : pi * 0.25f);
				squares[square][i] = ImVec2(a_centre.x + std::cos(angle) * a_radius, a_centre.y + std::sin(angle) * a_radius);
			}
		}
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(squares[0], 4, a_colour);
			a_drawList->AddConvexPolyFilled(squares[1], 4, a_colour);
			return;
		}
		// The outline only, round the points
		ImVec2 outline[16];
		for (int i = 0; i < 16; ++i) {
			const float angle = static_cast<float>(i) * pi / 8.0f;
			const float radius = i % 2 == 0 ? a_radius : a_radius * 0.77f;
			outline[i] = ImVec2(a_centre.x + std::cos(angle) * radius, a_centre.y + std::sin(angle) * radius);
		}
		a_drawList->AddPolyline(outline, 16, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.14f));
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Marker ) ----------------------
	//---------------------------------------------------

	// Needed / found: a brass oil lamp, lit when found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  s = a_size;
		const ImVec2 bowl(a_centre.x - s * 0.06f, a_centre.y + s * 0.1f);
		const ImVec2 tip(a_centre.x + s * 0.4f, a_centre.y + s * 0.02f);
		std::vector<ImVec2> outline{ ImVec2(bowl.x - s * 0.26f, bowl.y), tip, ImVec2(tip.x - s * 0.1f, bowl.y + s * 0.05f) };
		constexpr int steps = 10;
		for (int i = 0; i <= steps; ++i) {
			const float angle = static_cast<float>(i) / steps * pi;
			outline.emplace_back(bowl.x + std::cos(angle) * s * 0.26f, bowl.y + std::sin(angle) * s * 0.15f);
		}
		const float stroke = std::max(1.0f, s * 0.07f);
		if (a_fill > 0.0f) {
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(a_colour, a_fill));
			// The flame at the spout
			const ImVec2 flame(tip.x - s * 0.02f, tip.y - s * 0.06f);
			const float  grow = 0.4f + 0.6f * a_fill;
			const ImVec2 points[] = { ImVec2(flame.x, flame.y - s * 0.26f * grow), ImVec2(flame.x + s * 0.07f * grow, flame.y - s * 0.04f), ImVec2(flame.x, flame.y + s * 0.03f), ImVec2(flame.x - s * 0.07f * grow, flame.y - s * 0.04f) };
			a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(a_colour, a_fill));
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, stroke);
		// The lid and the handle
		a_drawList->PathArcTo(ImVec2(bowl.x, bowl.y), s * 0.1f, pi, pi * 2.0f, 8);
		a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
		a_drawList->PathArcTo(ImVec2(bowl.x - s * 0.3f, bowl.y + s * 0.04f), s * 0.07f, pi * 0.5f, pi * 1.5f, 8);
		a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Beads ) -----------------------
	//---------------------------------------------------

	// In place of a rule: a string of beads, turquoise and brass in turn, fading at the ends
	void Beads(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size, bool a_fade) {

		const float dx = a_to.x - a_from.x;
		const float dy = a_to.y - a_from.y;
		const float length = std::hypot(dx, dy);
		if (length < a_size * 1.5f) {
			return;
		}
		const ImVec2 along(dx / length, dy / length);
		const float  unit = a_size * 0.42f;
		const int    count = static_cast<int>(length / unit);
		const float  start = (length - static_cast<float>(count) * unit) * 0.5f;
		const float  alpha = Alpha(a_colour);
		for (int i = 0; i <= count; ++i) {
			const float  at = start + static_cast<float>(i) * unit;
			const float  fade = a_fade ? FadeAt(at, 0.0f, length) : 1.0f;
			const ImVec2 centre(a_from.x + along.x * at, a_from.y + along.y * at);
			if (i % 2 == 0) {
				a_drawList->AddCircleFilled(centre, a_size * 0.13f, WithAlpha(turquoise, fade * alpha), 10);
				a_drawList->AddCircleFilled(ImVec2(centre.x - a_size * 0.04f, centre.y - a_size * 0.04f), a_size * 0.045f, WithAlpha(turquoiseLight, fade * alpha), 6);
			} else {
				a_drawList->AddCircleFilled(centre, a_size * 0.08f, WithAlpha(a_colour, fade), 8);
			}
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Divider ) ---------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Beads(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Beads(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Star(a_drawList, ImVec2(midX, a_from.y), gap * 0.45f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Frame ) -----------------------
	//---------------------------------------------------

	// An Alik'r tent: a striped, scalloped canopy across the top, rope-lashed poles up the sides with brass
	// finials above them, a kilim along the foot with its fringe hanging below
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		TentCloth(a_drawList, a_min, a_max);

		// Behind the window, so only what stands out past its edges shows: the finials and the fringe
		auto* behind = ImGui::GetBackgroundDrawList();
		const float finial = band * (compact >= 0.8f ? 2.3f : 1.6f);
		Finial(behind, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.3f), finial);
		Finial(behind, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.3f), finial);
		Fringe(behind, ImVec2(a_min.x + band, a_max.y - 1.0f), width - band * 2.0f, band * 0.7f);

		Kilim(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band), ImVec2(a_max.x - band * 0.5f, a_max.y));
		const float pole = band * 0.72f;
		for (const float x : { a_min.x + band * 0.5f, a_max.x - band * 0.5f }) {
			Pole(a_drawList, ImVec2(x - pole * 0.5f, a_min.y), ImVec2(x + pole * 0.5f, a_max.y));
		}
		Canopy(a_drawList, ImVec2(a_min.x + band * 0.5f + pole * 0.5f, a_min.y), ImVec2(a_max.x - band * 0.5f - pole * 0.5f, a_min.y + band * 1.25f));
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Panel ) -----------------------
	//---------------------------------------------------

	// Inside a panel: the cloth's folds deepening towards its sides, lamplight from below, crossed swords in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}

		const float glow = std::min(height * 0.35f, size * 6.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(lamp, 0.0f), WithAlpha(lamp, 0.0f), WithAlpha(lamp, 0.06f), WithAlpha(lamp, 0.06f));

		// Folds gathered at each side
		const ImU32 dark = IM_COL32(0, 0, 0, 255);
		const float reach = std::min(width * 0.2f, size * 6.0f);
		for (int fold = 0; fold < 3; ++fold) {
			const float wide = reach * (1.0f - 0.28f * static_cast<float>(fold));
			const float strength = 0.12f - 0.03f * static_cast<float>(fold);
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + wide, a_max.y), WithAlpha(dark, strength), WithAlpha(dark, 0.0f), WithAlpha(dark, 0.0f), WithAlpha(dark, strength));
			a_drawList->AddRectFilledMultiColor(ImVec2(a_max.x - wide, a_min.y), a_max, WithAlpha(dark, 0.0f), WithAlpha(dark, strength), WithAlpha(dark, strength), WithAlpha(dark, 0.0f));
		}
		a_drawList->AddRectFilled(ImVec2(a_min.x, a_min.y), ImVec2(a_max.x, a_min.y + 2.0f), WithAlpha(indigo, 0.35f));

		// Two curved swords crossed, large and faint, in the lower right
		if (a_emblem) {
			const float  length = std::min(height * 0.85f, width * 0.3f);
			const ImVec2 centre(a_max.x - length * 0.6f, a_max.y - length * 0.12f);
			Sword(a_drawList, ImVec2(centre.x - length * 0.25f, centre.y), length, 1.0f, WithAlpha(brassLight, 0.08f));
			Sword(a_drawList, ImVec2(centre.x + length * 0.25f, centre.y), length, -1.0f, WithAlpha(brassLight, 0.08f));
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Panel Corners ) ---------------
	//---------------------------------------------------

	// A cord knotted across each top corner with a tassel hanging from it, a turquoise bead in each lower one
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float reach = size * 1.2f;
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const ImVec2 along(outer.x + sx * reach, outer.y + 2.0f);
			const ImVec2 down(outer.x + sx * 2.0f, outer.y + reach);
			const ImVec2 knot(outer.x + sx * reach * 0.38f, outer.y + reach * 0.38f);
			a_drawList->AddBezierQuadratic(along, ImVec2(outer.x + sx * reach * 0.5f, outer.y + reach * 0.15f), knot, rope, std::max(1.5f, size * 0.12f), 10);
			a_drawList->AddBezierQuadratic(down, ImVec2(outer.x + sx * reach * 0.15f, outer.y + reach * 0.5f), knot, rope, std::max(1.5f, size * 0.12f), 10);
			a_drawList->AddCircle(knot, size * 0.16f, rope, 10, std::max(1.5f, size * 0.1f));
			Tassel(a_drawList, ImVec2(knot.x, knot.y + size * 0.16f), size * 0.8f, turquoise);
		}
		for (const auto& outer : { a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const ImVec2 bead(outer.x + sx * size * 0.45f, outer.y - size * 0.45f);
			a_drawList->AddCircleFilled(bead, size * 0.18f, turquoise, 10);
			a_drawList->AddCircleFilled(ImVec2(bead.x - size * 0.05f, bead.y - size * 0.05f), size * 0.06f, turquoiseLight, 6);
			a_drawList->AddCircle(bead, size * 0.22f, brass, 12, 1.0f);
		}
	}
}

namespace CompletionistUI::RedguardDecor
{
	//---------------------------------------------------
	//-- Redguard Decor ( Perk Node ) -------------------
	//---------------------------------------------------

	// Stars: whole when taken, outlined when next, a bare tile when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Star(a_drawList, a_centre, a_radius * 1.1f, a_colour, true);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.3f, WithAlpha(shadow, 0.8f), 12);
			return;
		case PerkState::kNext:
			Star(a_drawList, a_centre, a_radius * 1.0f, shadow, true);
			Star(a_drawList, a_centre, a_radius * 1.0f, a_colour, false);
			return;
		default:
			Tile(a_drawList, a_centre, a_radius * 0.42f, shadow, true);
			Tile(a_drawList, a_centre, a_radius * 0.42f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Redguard Decor ( Perk Link ) -------------------
	//---------------------------------------------------

	// A string of beads between the perks where they're taken, a bare thread where they're not
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, 2.0f);
		a_drawList->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, 1.0f);
		if (!a_lit) {
			return;
		}
		const float bead = std::max(2.0f, a_radius * 0.3f);
		float       travelled = 0.0f;
		float       next = a_radius * 1.4f;
		int         count = 0;
		float       total = 0.0f;
		for (std::size_t i = 1; i < curve.size(); ++i) {
			total += std::hypot(curve[i].x - curve[i - 1].x, curve[i].y - curve[i - 1].y);
		}
		for (std::size_t i = 1; i < curve.size(); ++i) {
			travelled += std::hypot(curve[i].x - curve[i - 1].x, curve[i].y - curve[i - 1].y);
			if (travelled >= next && travelled < total - a_radius * 1.2f) {
				if (count++ % 2 == 0) {
					a_drawList->AddCircleFilled(curve[i], bead, turquoise, 10);
					a_drawList->AddCircleFilled(ImVec2(curve[i].x - bead * 0.3f, curve[i].y - bead * 0.3f), bead * 0.35f, turquoiseLight, 6);
				} else {
					a_drawList->AddCircleFilled(curve[i], bead * 0.65f, a_colour, 8);
				}
				next += bead * 2.6f;
			}
		}
	}
}
