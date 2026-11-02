#include "UI/Themes/AnimeDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <ranges>
#include <utility>
#include <vector>

namespace CompletionistUI::AnimeDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Signal yellow, pale panel greys, dark glass
		constexpr ImU32 yellow = IM_COL32(250, 208, 20, 255);
		constexpr ImU32 yellowDeep = IM_COL32(226, 176, 8, 255);
		constexpr ImU32 white = IM_COL32(248, 248, 250, 255);
		constexpr ImU32 pale = IM_COL32(232, 232, 236, 255);
		constexpr ImU32 mist = IM_COL32(214, 214, 218, 255);
		constexpr ImU32 steel = IM_COL32(150, 150, 154, 255);
		constexpr ImU32 graphite = IM_COL32(70, 70, 72, 255);
		constexpr ImU32 glass = IM_COL32(42, 42, 44, 255);
		constexpr ImU32 ink = IM_COL32(24, 24, 26, 255);

		bool openFrame = false;

		//---------------------------------------------------
		//-- Anime Decor ( Slashes ) ------------------------
		//---------------------------------------------------

		// The hazard marks: a row of leaning bars
		void Slashes(ImDrawList* a_list, ImVec2 a_at, int a_count, float a_height, float a_width, ImU32 a_colour) {

			const float lean = a_height * 0.55f;
			const float step = a_width + a_height * 0.28f;
			for (int i = 0; i < a_count; ++i) {
				const float x = a_at.x + step * static_cast<float>(i);
				const ImVec2 bar[4] = { ImVec2(x + lean, a_at.y), ImVec2(x + lean + a_width, a_at.y), ImVec2(x + a_width, a_at.y + a_height), ImVec2(x, a_at.y + a_height) };
				a_list->AddConvexPolyFilled(bar, 4, a_colour);
			}
		}

		//---------------------------------------------------
		//-- Anime Decor ( Slant ) --------------------------
		//---------------------------------------------------

		// A band with its right end cut on the slant, and its left end rounded or cut too
		void Slant(ImDrawList* a_list, ImVec2 a_min, ImVec2 a_max, float a_cut, ImU32 a_colour) {

			const ImVec2 band[4] = { a_min, ImVec2(a_max.x, a_min.y), ImVec2(a_max.x - a_cut, a_max.y), ImVec2(a_min.x, a_max.y) };
			a_list->AddConvexPolyFilled(band, 4, a_colour);
		}

		//---------------------------------------------------
		//-- Anime Decor ( Mascot Styles ) ------------------
		//---------------------------------------------------

		enum class Face
		{
			kSmile,
			kJoy,
			kShy,
		};

		struct Mascot
		{
			ImU32 hair;
			ImU32 hairDark;
			ImU32 hairLight;
			ImU32 eye;
			ImU32 eyeDark;
			ImU32 paw;
			Face  face;
			bool  ears;
			bool  headset;
			bool  tail;
			bool  sprout;
			float turn;	// radians towards +x, as a head on a sphere
			float tilt;	// radians, the whole head
		};

		constexpr Mascot mascots[] = {
			// Silver, golden eyes, a quiet smile
			{ IM_COL32(204, 204, 210, 255), IM_COL32(150, 150, 158, 255), IM_COL32(240, 240, 244, 255), IM_COL32(248, 192, 40, 255), IM_COL32(176, 104, 16, 255), IM_COL32(248, 248, 250, 255), Face::kSmile, true, true, false, false, 0.26f, 0.09f },
			// Charcoal, laughing, a sprout of hair
			{ IM_COL32(74, 74, 80, 255), IM_COL32(44, 44, 50, 255), IM_COL32(128, 128, 136, 255), IM_COL32(248, 192, 40, 255), IM_COL32(176, 104, 16, 255), IM_COL32(44, 44, 48, 255), Face::kJoy, true, true, true, true, -0.22f, -0.08f },
			// Cream, blue eyes, shy
			{ IM_COL32(236, 228, 214, 255), IM_COL32(196, 184, 168, 255), IM_COL32(252, 248, 240, 255), IM_COL32(92, 170, 240, 255), IM_COL32(30, 84, 170, 255), IM_COL32(44, 44, 48, 255), Face::kShy, true, false, false, true, -0.16f, -0.06f },
			// Charcoal again, tied back, no ears
			{ IM_COL32(84, 84, 90, 255), IM_COL32(50, 50, 56, 255), IM_COL32(140, 140, 148, 255), IM_COL32(248, 192, 40, 255), IM_COL32(176, 104, 16, 255), IM_COL32(44, 44, 48, 255), Face::kSmile, false, true, true, false, 0.24f, 0.07f },
			// Cream, tied back, laughing
			{ IM_COL32(232, 222, 208, 255), IM_COL32(190, 176, 160, 255), IM_COL32(252, 246, 238, 255), IM_COL32(92, 170, 240, 255), IM_COL32(30, 84, 170, 255), IM_COL32(248, 248, 250, 255), Face::kJoy, false, true, true, true, -0.24f, -0.09f },
		};

		constexpr ImU32 pen = IM_COL32(36, 30, 34, 255);
		constexpr ImU32 skin = IM_COL32(255, 238, 228, 255);
		constexpr ImU32 skinShade = IM_COL32(244, 212, 200, 255);
		constexpr ImU32 blush = IM_COL32(250, 130, 130, 255);

		//---------------------------------------------------
		//-- Anime Decor ( Paint Mascot ) -------------------
		//---------------------------------------------------

		// One head, a_radius across from its centre at the origin, every shape measured in that radius
		void PaintMascot(ImDrawList* a_list, const Mascot& a_style, float a_radius) {

			const auto  at = [&](float a_x, float a_y) { return ImVec2(a_x * a_radius, a_y * a_radius); };
			const float line = std::max(1.0f, a_radius * 0.04f);
			const auto  smooth = [&](std::initializer_list<ImVec2> a_points, int a_steps = 5) {
				const std::vector<ImVec2> points(a_points);
				return Spline(points.data(), static_cast<int>(points.size()), true, a_steps);
			};
			const auto outlined = [&](const std::vector<ImVec2>& a_shape, ImU32 a_fill, float a_line) {
				FillShape(a_list, a_shape, a_fill);
				a_list->AddPolyline(a_shape.data(), static_cast<int>(a_shape.size()), pen, ImDrawFlags_Closed, a_line);
			};
			const auto stroke = [&](std::initializer_list<ImVec2> a_points, ImU32 a_colour, float a_width) {
				const std::vector<ImVec2> points(a_points);
				const auto                curve = Spline(points.data(), static_cast<int>(points.size()), false, 5);
				a_list->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, a_width);
			};

			// A filled shape shaded from a_top down to a_bottom, so hair isn't one flat colour
			const auto shaded = [&](const std::vector<ImVec2>& a_shape, ImU32 a_top, ImU32 a_bottom, float a_fromY, float a_toY) {
				const int first = a_list->VtxBuffer.Size;
				FillShape(a_list, a_shape, a_top);
				for (int i = first; i < a_list->VtxBuffer.Size; ++i) {
					auto& vertex = a_list->VtxBuffer[i];
					const ImU32 colour = Mix(a_top, a_bottom, (vertex.pos.y - a_fromY * a_radius) / ((a_toY - a_fromY) * a_radius));
					vertex.col = (colour & ~IM_COL32_A_MASK) | (vertex.col & IM_COL32_A_MASK);
				}
			};

			// Where a curve from a_root towards a_tip is, a_t along it
			const auto along = [](ImVec2 a_root, ImVec2 a_bend, ImVec2 a_tip, float a_t) {
				const float u = 1.0f - a_t;
				return ImVec2(u * u * a_root.x + 2.0f * u * a_t * a_bend.x + a_t * a_t * a_tip.x, u * u * a_root.y + 2.0f * u * a_t * a_bend.y + a_t * a_t * a_tip.y);
			};
			const ImU32 hairLine = Mix(a_style.hairDark, pen, 0.55f);
			const float hairFine = std::max(0.7f, a_radius * 0.016f);

			// One lock of hair: leaves its root narrow, swells, and thins away to a point
			const auto lock = [&](ImVec2 a_root, ImVec2 a_bend, ImVec2 a_tip, float a_width, ImU32 a_edge, float a_edgeWidth) {
				constexpr int steps = 12;
				std::vector<ImVec2> spine;
				for (int step = 0; step <= steps; ++step) {
					spine.push_back(along(at(a_root.x, a_root.y), at(a_bend.x, a_bend.y), at(a_tip.x, a_tip.y), static_cast<float>(step) / steps));
				}
				std::vector<ImVec2> outline;
				const auto halfAt = [&](int a_step) {
					const float t = static_cast<float>(a_step) / steps;
					return a_width * a_radius * std::pow(1.0f - t, 0.65f) * (0.55f + 0.45f * std::min(t * 3.0f, 1.0f));
				};
				for (int step = 0; step <= steps; ++step) {
					const auto normal = NormalAt(spine, static_cast<std::size_t>(step));
					outline.emplace_back(spine[static_cast<std::size_t>(step)].x + normal.x * halfAt(step), spine[static_cast<std::size_t>(step)].y + normal.y * halfAt(step));
				}
				for (int step = steps - 1; step >= 0; --step) {
					const auto normal = NormalAt(spine, static_cast<std::size_t>(step));
					outline.emplace_back(spine[static_cast<std::size_t>(step)].x - normal.x * halfAt(step), spine[static_cast<std::size_t>(step)].y - normal.y * halfAt(step));
				}
				shaded(outline, a_style.hair, Mix(a_style.hair, a_style.hairDark, 0.85f), a_root.y, a_tip.y);
				if (a_edgeWidth > 0.0f) {
					a_list->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_edge, ImDrawFlags_Closed, a_edgeWidth);
				}
				// A strand of shadow and one of light running down it
				std::vector<ImVec2> dark;
				std::vector<ImVec2> lit;
				for (int step = 1; step < steps; ++step) {
					const auto normal = NormalAt(spine, static_cast<std::size_t>(step));
					dark.emplace_back(spine[static_cast<std::size_t>(step)].x + normal.x * halfAt(step) * 0.35f, spine[static_cast<std::size_t>(step)].y + normal.y * halfAt(step) * 0.35f);
					if (step < steps * 2 / 3) {
						lit.emplace_back(spine[static_cast<std::size_t>(step)].x - normal.x * halfAt(step) * 0.4f, spine[static_cast<std::size_t>(step)].y - normal.y * halfAt(step) * 0.4f);
					}
				}
				a_list->AddPolyline(dark.data(), static_cast<int>(dark.size()), WithAlpha(a_style.hairDark, 0.7f), ImDrawFlags_None, hairFine);
				a_list->AddPolyline(lit.data(), static_cast<int>(lit.size()), WithAlpha(a_style.hairLight, 0.4f), ImDrawFlags_None, hairFine);
			};

			// Which way the hair is parted, and a drawing's own scatter of fine strands
			const float part = a_style.tail ? -1.0f : 1.0f;
			Random      random{ a_style.hair ^ 0x5bd1e995u };
			for (int i = 0; i < 4; ++i) {
				random.Next();
			}

			// Tied-back hair, behind everything
			if (a_style.tail) {
				outlined(smooth({ at(0.78f, -0.45f), at(1.22f, -0.5f), at(1.5f, -0.1f), at(1.55f, 0.45f), at(1.82f, 0.82f), at(1.3f, 0.78f), at(1.0f, 0.35f) }), a_style.hairDark, line);
				a_list->AddLine(at(1.02f, -0.5f), at(1.14f, -0.16f), yellow, a_radius * 0.09f);
			}

			// Everything from here to the fringe turns with the head
			const int turnFrom = a_list->VtxBuffer.Size;

			// The back of the hair, falling in tufts past the shoulders
			const auto backHair = smooth({ at(0.0f, -1.02f), at(0.74f, -0.84f), at(1.04f, -0.22f), at(1.08f, 0.3f), at(1.2f, 0.72f), at(0.98f, 0.86f), at(0.84f, 1.25f), at(-0.84f, 1.25f), at(-0.98f, 0.86f), at(-1.2f, 0.72f), at(-1.08f, 0.3f), at(-1.04f, -0.22f), at(-0.74f, -0.84f) });
			shaded(backHair, Mix(a_style.hair, a_style.hairDark, 0.55f), Shade(a_style.hairDark, 0.72f), -1.0f, 1.2f);
			a_list->AddPolyline(backHair.data(), static_cast<int>(backHair.size()), pen, ImDrawFlags_Closed, line);

			// Long ends falling past the shoulders, and loose wisps flying from them
			for (const float side : { -1.0f, 1.0f }) {
				lock(ImVec2(side * 0.92f, -0.1f), ImVec2(side * 1.24f, 0.28f), ImVec2(side * 1.42f, 0.82f), 0.15f, pen, line * 0.9f);
				lock(ImVec2(side * 0.96f, 0.1f), ImVec2(side * 1.12f, 0.52f), ImVec2(side * 1.16f, 1.02f), 0.14f, pen, line * 0.9f);
				lock(ImVec2(side * 0.96f, -0.34f), ImVec2(side * 1.2f, -0.16f), ImVec2(side * 1.34f, 0.12f), 0.05f, hairLine, hairFine);
				lock(ImVec2(side * 1.04f, 0.22f), ImVec2(side * 1.28f, 0.42f), ImVec2(side * 1.46f, 0.46f), 0.035f, hairLine, hairFine);
			}

			// Ears, set on top of the head
			if (a_style.ears) {
				for (const float side : { -1.0f, 1.0f }) {
					const std::vector<ImVec2> ear{ at(side * 0.22f, -0.92f), at(side * 0.94f, -1.42f), at(side * 1.0f, -0.42f) };
					outlined(ear, a_style.hair, line);
					FillShape(a_list, { at(side * 0.5f, -0.9f), at(side * 0.88f, -1.2f), at(side * 0.9f, -0.62f) }, IM_COL32(250, 214, 208, 255));
				}
			}

			// Face
			outlined(smooth({ at(-0.8f, -0.2f), at(-0.74f, 0.32f), at(-0.42f, 0.74f), at(0.0f, 0.88f), at(0.42f, 0.74f), at(0.74f, 0.32f), at(0.8f, -0.2f), at(0.5f, -0.6f), at(0.0f, -0.7f), at(-0.5f, -0.6f) }), skin, line);

			// Shadow of the fringe across the brow
			FillShape(a_list, smooth({ at(-0.66f, -0.3f), at(-0.3f, -0.12f), at(0.0f, -0.08f), at(0.3f, -0.12f), at(0.66f, -0.3f), at(0.3f, -0.56f), at(-0.3f, -0.56f) }), skinShade);

			for (const float side : { -1.0f, 1.0f }) {
				SoftSpot(a_list, at(side * 0.56f, 0.5f), a_radius * 0.2f, WithAlpha(blush, 0.06f));
			}

			// Eyes
			for (const float side : { -1.0f, 1.0f }) {
				const float x = side * 0.39f;
				const float y = 0.2f;
				if (a_style.face == Face::kJoy) {
					stroke({ at(x - 0.25f, y + 0.08f), at(x, y - 0.17f), at(x + 0.25f, y + 0.08f) }, pen, a_radius * 0.08f);
					continue;
				}
				// The white, the dark top of the iris, the bright glow rising from under it, then the catchlights
				FillShape(a_list, smooth({ at(x, y - 0.33f), at(x + 0.25f, y - 0.12f), at(x + 0.23f, y + 0.2f), at(x, y + 0.33f), at(x - 0.23f, y + 0.2f), at(x - 0.25f, y - 0.12f) }), white);
				FillShape(a_list, smooth({ at(x, y - 0.31f), at(x + 0.2f, y - 0.1f), at(x + 0.19f, y + 0.16f), at(x, y + 0.31f), at(x - 0.19f, y + 0.16f), at(x - 0.2f, y - 0.1f) }), a_style.eyeDark);
				FillShape(a_list, smooth({ at(x, y - 0.1f), at(x + 0.17f, y + 0.02f), at(x + 0.15f, y + 0.18f), at(x, y + 0.28f), at(x - 0.15f, y + 0.18f), at(x - 0.17f, y + 0.02f) }), a_style.eye);
				FillShape(a_list, smooth({ at(x, y + 0.1f), at(x + 0.1f, y + 0.16f), at(x, y + 0.26f), at(x - 0.1f, y + 0.16f) }), Mix(a_style.eye, white, 0.45f));
				a_list->AddCircleFilled(at(x, y - 0.04f), a_radius * 0.085f, pen, 14);
				a_list->AddCircleFilled(at(x - side * 0.07f, y - 0.15f), a_radius * 0.075f, white, 12);
				a_list->AddCircleFilled(at(x + side * 0.08f, y + 0.14f), a_radius * 0.035f, white, 8);
				// The heavy upper lash, flicking out at the side
				const float droop = a_style.face == Face::kShy ? 0.09f : 0.0f;
				stroke({ at(x - side * 0.23f, y - 0.22f - droop * 0.5f), at(x + side * 0.02f, y - 0.35f + droop), at(x + side * 0.3f, y - 0.15f + droop) }, pen, a_radius * 0.08f);
				a_list->AddLine(at(x + side * 0.29f, y - 0.15f + droop), at(x + side * 0.37f, y - 0.05f + droop), pen, a_radius * 0.04f);
			}

			// Mouth
			switch (a_style.face) {
			case Face::kJoy:
				{
					const auto mouth = smooth({ at(-0.13f, 0.52f), at(0.0f, 0.54f), at(0.13f, 0.52f), at(0.08f, 0.69f), at(0.0f, 0.73f), at(-0.08f, 0.69f) });
					FillShape(a_list, mouth, IM_COL32(170, 50, 62, 255));
					FillShape(a_list, smooth({ at(-0.06f, 0.65f), at(0.0f, 0.62f), at(0.06f, 0.65f), at(0.0f, 0.71f) }), IM_COL32(250, 150, 150, 255));
					a_list->AddPolyline(mouth.data(), static_cast<int>(mouth.size()), pen, ImDrawFlags_Closed, line * 0.8f);
				}
				break;
			case Face::kShy:
				stroke({ at(-0.07f, 0.61f), at(-0.02f, 0.57f), at(0.02f, 0.62f), at(0.07f, 0.57f) }, pen, line);
				break;
			default:
				stroke({ at(-0.08f, 0.58f), at(0.0f, 0.63f), at(0.08f, 0.58f) }, pen, line);
				break;
			}

			// The hair over all of it: the crown first, then lock on lock from the outside in, all falling from the parting
			const ImVec2 parting(part * 0.16f, -1.02f);
			const auto   crown = smooth({ at(-1.0f, -0.06f), at(-0.9f, -0.55f), at(-0.6f, -0.88f), at(-0.25f, -1.02f), at(0.1f, -1.06f), at(0.45f, -0.98f), at(0.8f, -0.72f), at(0.98f, -0.25f), at(1.0f, -0.06f), at(0.72f, -0.16f), at(0.3f, -0.28f), at(-0.3f, -0.28f), at(-0.72f, -0.16f) });
			shaded(crown, a_style.hair, Mix(a_style.hair, a_style.hairDark, 0.55f), -1.05f, -0.1f);

			lock(ImVec2(-part * 0.52f, -0.9f), ImVec2(-part * 1.04f, -0.42f), ImVec2(-part * 0.94f, 0.74f), 0.25f, pen, line);
			lock(ImVec2(part * 0.62f, -0.84f), ImVec2(part * 1.06f, -0.36f), ImVec2(part * 0.96f, 0.64f), 0.23f, pen, line);
			lock(ImVec2(-part * 0.22f, -1.0f), ImVec2(-part * 0.76f, -0.62f), ImVec2(-part * 0.72f, 0.04f), 0.2f, hairLine, hairFine);
			lock(ImVec2(part * 0.44f, -0.96f), ImVec2(part * 0.82f, -0.56f), ImVec2(part * 0.74f, 0.0f), 0.2f, hairLine, hairFine);
			lock(ImVec2(-part * 0.1f, -1.0f), ImVec2(-part * 0.62f, -0.6f), ImVec2(-part * 0.62f, -0.08f), 0.2f, hairLine, hairFine);
			lock(ImVec2(part * 0.3f, -1.0f), ImVec2(part * 0.68f, -0.58f), ImVec2(part * 0.62f, -0.08f), 0.2f, hairLine, hairFine);
			lock(parting, ImVec2(-part * 0.5f, -0.62f), ImVec2(-part * 0.5f, -0.1f), 0.22f, hairLine, hairFine);
			lock(parting, ImVec2(-part * 0.3f, -0.62f), ImVec2(-part * 0.37f, -0.13f), 0.2f, hairLine, hairFine);
			lock(parting, ImVec2(part * 0.36f, -0.56f), ImVec2(part * 0.3f, -0.12f), 0.2f, hairLine, hairFine);
			lock(parting, ImVec2(part * 0.5f, -0.58f), ImVec2(part * 0.48f, -0.09f), 0.2f, hairLine, hairFine);
			lock(parting, ImVec2(-part * 0.14f, -0.58f), ImVec2(-part * 0.24f, -0.1f), 0.17f, hairLine, hairFine);
			lock(parting, ImVec2(part * 0.26f, -0.5f), ImVec2(part * 0.05f, 0.14f), 0.19f, hairLine, hairFine);

			// Fine strands combed down from the parting
			const int strands = a_radius > 28.0f ? 26 : 14;
			for (int i = 0; i < strands; ++i) {
				const float  x = random.Range(-0.92f, 0.92f);
				const ImVec2 root = at(parting.x + (x - parting.x) * 0.12f, -1.0f + std::abs(x) * 0.08f);
				const ImVec2 end = at(x, -0.28f - (1.0f - std::abs(x)) * random.Range(0.02f, 0.22f));
				const ImVec2 bend = at(x * 0.92f, -0.82f + x * x * 0.3f);
				std::vector<ImVec2> strand;
				const float from = random.Range(0.1f, 0.35f);
				const float to = random.Range(0.65f, 1.0f);
				for (int step = 0; step <= 6; ++step) {
					strand.push_back(along(root, bend, end, from + (to - from) * static_cast<float>(step) / 6.0f));
				}
				a_list->AddPolyline(strand.data(), static_cast<int>(strand.size()), WithAlpha(random.Next() < 0.6f ? a_style.hairDark : a_style.hairLight, random.Range(0.25f, 0.5f)), ImDrawFlags_None, hairFine);
			}

			// The sheen: a broken ring of little glints following the round of the head, not a line
			const int glints = a_radius > 28.0f ? 26 : 16;
			for (int i = 0; i < glints; ++i) {
				const float x = -0.82f + 1.64f * (static_cast<float>(i) + random.Range(0.0f, 0.8f)) / static_cast<float>(glints);
				const float y = -0.66f + x * x * 0.42f + random.Range(-0.03f, 0.03f);
				const float length = random.Range(0.05f, 0.15f);
				const float wide = random.Range(0.012f, 0.03f);
				const float lean = (x - parting.x) * 0.3f;
				const std::vector<ImVec2> glint{ at(x - lean * length, y - length), at(x + wide, y), at(x + lean * length, y + length * 0.7f), at(x - wide, y) };
				FillShape(a_list, glint, WithAlpha(a_style.hairLight, random.Range(0.45f, 0.9f)));
			}

			// The ink line round the top of the head, drawn last over the lot
			const std::vector<ImVec2> crownTop{ at(-1.0f, -0.06f), at(-0.9f, -0.55f), at(-0.6f, -0.88f), at(-0.25f, -1.02f), at(0.1f, -1.06f), at(0.45f, -0.98f), at(0.8f, -0.72f), at(0.98f, -0.25f), at(1.0f, -0.06f) };
			const auto crownLine = Spline(crownTop.data(), static_cast<int>(crownTop.size()), false, 5);
			a_list->AddPolyline(crownLine.data(), static_cast<int>(crownLine.size()), pen, ImDrawFlags_None, line);

			if (a_style.sprout) {
				stroke({ at(0.02f, -1.02f), at(0.05f, -1.2f), at(0.2f, -1.26f), at(0.27f, -1.15f) }, pen, a_radius * 0.09f);
				stroke({ at(0.02f, -1.02f), at(0.05f, -1.2f), at(0.2f, -1.26f), at(0.27f, -1.15f) }, a_style.hair, a_radius * 0.045f);
			}

			// Wrap what's drawn round a ball and turn it: the far side crowds up, the near side opens out,
			// easing off towards the head's edge so its outline holds
			constexpr float ball = 1.05f;
			for (int i = turnFrom; i < a_list->VtxBuffer.Size; ++i) {
				auto&       vertex = a_list->VtxBuffer[i];
				const float x = std::clamp(vertex.pos.x / a_radius / ball, -1.0f, 1.0f);
				const float angle = std::asin(x);
				const float ease = 1.0f - x * x * x * x;
				vertex.pos.x += (std::sin(angle + a_style.turn) - x) * ball * a_radius * ease;
			}

			if (a_style.headset) {
				// A cup at each side, a touch of yellow on them
				for (const float side : { -1.0f, 1.0f }) {
					// Only the near one shows, the far one is round the back of the head
					if (side * a_style.turn > 0.0f) {
						continue;
					}
					const ImVec2 cup = at(side * 1.0f, 0.28f);
					a_list->AddCircleFilled(cup, a_radius * 0.27f, pen, 22);
					a_list->AddCircleFilled(cup, a_radius * 0.22f, graphite, 22);
					a_list->AddCircle(cup, a_radius * 0.13f, steel, 18, line);
					a_list->AddCircleFilled(cup, a_radius * 0.05f, yellow, 10);
				}
			}
		}

		//---------------------------------------------------
		//-- Anime Decor ( Mascot ) -------------------------
		//---------------------------------------------------

		// Each head is painted once and its triangles reused wherever it sits
		void DrawMascot(ImDrawList* a_list, ImVec2 a_centre, float a_radius, int a_style) {

			struct Painted
			{
				int      style;
				BakedArt art;
			};
			static std::vector<Painted> painted;
			const auto uv = ImGui::GetFontTexUvWhitePixel();
			const auto cached = std::ranges::find_if(painted, [&](const Painted& a_entry) {
				return a_entry.style == a_style && a_entry.art.size == a_radius && a_entry.art.uv.x == uv.x && a_entry.art.uv.y == uv.y;
			});
			if (cached != painted.end()) {
				Blit(a_list, cached->art, a_centre);
				return;
			}
			if (painted.size() >= 24) {
				painted.clear();
			}
			Painted entry{ a_style, BakedArt{ 0u, ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f), a_radius, uv, 0u } };
			Record(a_list, entry.art, [&](ImDrawList* a_scratch) {
				const auto& style = mascots[static_cast<std::size_t>(a_style) % std::size(mascots)];
				PaintMascot(a_scratch, style, a_radius);
				// The head tips on its neck, low behind the panel's edge
				const float cosine = std::cos(style.tilt);
				const float sine = std::sin(style.tilt);
				const float pivot = a_radius * 0.7f;
				for (auto& vertex : a_scratch->VtxBuffer) {
					const float x = vertex.pos.x;
					const float y = vertex.pos.y - pivot;
					vertex.pos = ImVec2(x * cosine - y * sine, x * sine + y * cosine + pivot);
				}
			});
			Blit(a_list, entry.art, a_centre);
			painted.push_back(std::move(entry));
		}

		// The little burst of excitement beside a head
		void Burst(ImDrawList* a_list, ImVec2 a_at, float a_size, float a_side) {

			const auto wedge = [&](float a_angle, float a_from, float a_to, float a_wide) {
				const float angle = a_side > 0.0f ? a_angle : pi - a_angle;
				const float cosine = std::cos(angle);
				const float sine = std::sin(angle);
				const auto  point = [&](float a_along, float a_across) { return ImVec2(a_at.x + cosine * a_along - sine * a_across, a_at.y + sine * a_along + cosine * a_across); };
				const std::vector<ImVec2> shape{ point(a_from, -a_wide * 0.45f), point(a_to, -a_wide), point(a_to, a_wide), point(a_from, a_wide * 0.45f) };
				FillShape(a_list, shape, yellow);
			};
			wedge(-1.15f, a_size * 0.35f, a_size * 1.05f, a_size * 0.13f);
			wedge(-0.45f, a_size * 0.4f, a_size * 1.1f, a_size * 0.13f);
			wedge(0.18f, a_size * 0.45f, a_size * 0.9f, a_size * 0.1f);
		}

		struct HeaderSpot
		{
			float x;
			int   style;
		};
		HeaderSpot headerSpots[2];
		float      headerEdge = 0.0f;
		float      headerRadius = 0.0f;
		bool       headerShown = false;
	}

	//---------------------------------------------------
	//-- Anime Decor ( Stud ) ---------------------------
	//---------------------------------------------------

	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(yellow, alpha));
			a_drawList->AddPolyline(points, 4, WithAlpha(yellowDeep, alpha), ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
			return;
		}
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.3f));
	}

	//---------------------------------------------------
	//-- Anime Decor ( Marker ) -------------------------
	//---------------------------------------------------

	// Needed / found: a heavy black diamond, filling with yellow as it's found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  radius = a_size * 0.36f;
		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - radius),
			ImVec2(a_centre.x + radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + radius),
			ImVec2(a_centre.x - radius, a_centre.y),
		};
		if (a_fill > 0.0f) {
			a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(yellow, a_fill));
		}
		a_drawList->AddPolyline(points, 4, a_fill >= 1.0f ? yellowDeep : WithAlpha(ink, 1.0f - a_fill * 0.5f), ImDrawFlags_Closed, std::max(1.5f, a_size * 0.1f));
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Anime Decor ( Ornament ) -----------------------
	//---------------------------------------------------

	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const float height = a_radius * 2.4f;
		const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		Slashes(a_drawList, ImVec2(a_centre.x - height * 0.7f, a_centre.y - height * 0.5f), 2, height, std::max(1.5f, height * 0.2f), WithAlpha(yellow, alpha));
	}

	//---------------------------------------------------
	//-- Anime Decor ( Rule ) ---------------------------
	//---------------------------------------------------

	// Three yellow slashes, and a clean line running on from them
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize();
		const float direction = a_to.x >= a_from.x ? 1.0f : -1.0f;
		const float length = (a_to.x - a_from.x) * direction;
		if (length < size * 0.8f) {
			return;
		}
		if (direction < 0.0f || length < size * 2.5f) {
			a_drawList->AddLine(a_from, a_to, a_colour, std::max(1.0f, size * 0.07f));
			return;
		}
		const float height = size * 0.62f;
		const float bar = std::max(1.5f, size * 0.13f);
		Slashes(a_drawList, ImVec2(a_from.x, a_from.y - height * 0.5f), 3, height, bar, yellow);
		a_drawList->AddLine(ImVec2(a_from.x + size * 1.2f, a_from.y), a_to, a_colour, std::max(1.0f, size * 0.07f));
	}

	//---------------------------------------------------
	//-- Anime Decor ( Divider ) ------------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		a_drawList->AddLine(a_from, a_to, a_colour, 1.0f);
	}

	//---------------------------------------------------
	//-- Anime Decor ( Frame ) --------------------------
	//---------------------------------------------------

	void OpenFrame() {

		openFrame = true;
	}

	// A small window: its own pale ground already rounded, a yellow slash on it and the hazard marks at its corner
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		if (std::exchange(openFrame, false)) {
			return;
		}
		const float size = ImGui::GetFontSize();
		if (a_max.x - a_min.x < size * 8.0f) {
			return;
		}
		const float bleed = size * 0.3f;
		const float round = ImGui::GetStyle().WindowRounding;
		a_drawList->AddRect(ImVec2(a_min.x - bleed, a_min.y - bleed), ImVec2(a_max.x + bleed, a_max.y + bleed), WithAlpha(white, 0.9f), round, 0, std::max(1.5f, size * 0.1f));
		Slashes(a_drawList, ImVec2(a_min.x + size * 0.1f, a_min.y - size * 0.05f), 1, size * 0.9f, size * 0.26f, yellow);
		Slashes(a_drawList, ImVec2(a_max.x - size * 2.8f, a_min.y + size * 0.05f), 4, size * 0.3f, size * 0.3f, WithAlpha(graphite, 0.7f));

		// A yellow nick at the far corner, and the fine print of a technical drawing along the foot
		Slashes(a_drawList, ImVec2(a_max.x - size * 1.1f, a_max.y - size * 0.75f), 1, size * 0.7f, size * 0.2f, yellow);
		if (a_max.x - a_min.x > size * 14.0f && a_max.y - a_min.y > size * 8.0f) {
			Random     random{ 0xA11Eu };
			const float foot = a_max.y - size * 0.1f;
			float       x = a_min.x + size * 0.5f;
			const float end = x + size * 3.0f;
			while (x < end) {
				const float bar = size * random.Range(0.04f, 0.14f);
				a_drawList->AddRectFilled(ImVec2(x, foot - size * 0.32f), ImVec2(x + bar, foot), WithAlpha(steel, 0.55f));
				x += bar + size * random.Range(0.04f, 0.12f);
			}
		}
	}

	//---------------------------------------------------
	//-- Anime Decor ( Plates ) -------------------------
	//---------------------------------------------------

	void BodyPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float size = ImGui::GetFontSize();
		const float round = size * 1.1f;
		a_drawList->AddRectFilled(ImVec2(a_min.x - 1.0f, a_min.y + size * 0.14f), ImVec2(a_max.x + 3.0f, a_max.y + size * 0.2f), WithAlpha(ink, 0.3f * a_alpha), round);
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(pale, a_alpha), round);
		a_drawList->AddRect(a_min, a_max, WithAlpha(white, a_alpha), round, 0, std::max(1.5f, size * 0.1f));
		Slashes(a_drawList, ImVec2(a_max.x - size * 3.9f, a_min.y + size * 0.1f), 4, size * 0.34f, size * 0.42f, WithAlpha(graphite, 0.7f));
	}

	void SidebarPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float size = ImGui::GetFontSize();
		const float round = size * 0.9f;
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(IM_COL32(54, 54, 56, 255), std::max(a_alpha * 0.95f, 0.6f)), round);
		a_drawList->AddRect(a_min, a_max, WithAlpha(steel, 0.75f), round, 0, std::max(2.0f, size * 0.16f));
	}

	void StripPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(pale, a_alpha), size * 0.6f);
		a_drawList->AddRect(a_min, a_max, WithAlpha(white, a_alpha), size * 0.6f, 0, std::max(1.0f, size * 0.08f));
	}

	void FooterBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(IM_COL32(46, 46, 48, 255), std::max(a_alpha * 0.9f, 0.75f)), height * 0.3f);

		// A slash on the bar at each end, inside its rounding
		const float tall = height * 0.56f;
		const float top = a_min.y + (height - tall) * 0.5f;
		Slashes(a_drawList, ImVec2(a_min.x + size * 0.8f, top), 1, tall, size * 0.2f, yellow);
		Slashes(a_drawList, ImVec2(a_max.x - size * 0.8f - tall * 0.55f - size * 0.2f, top), 1, tall, size * 0.2f, yellow);
	}

	void TooltipMarks(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float tall = std::min(size * 0.75f, (a_max.y - a_min.y) * 0.6f);
		Slashes(a_drawList, ImVec2(a_min.x + size * 0.1f, a_min.y + size * 0.1f), 1, tall, size * 0.13f, yellow);
		if (a_max.x - a_min.x > size * 6.0f) {
			Slashes(a_drawList, ImVec2(a_max.x - size * 1.1f, a_min.y + size * 0.14f), 3, size * 0.16f, size * 0.16f, WithAlpha(graphite, 0.6f));
		}
	}

	void ToastPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha, float a_opacity) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float round = size * 0.45f;
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(pale, a_alpha * std::max(a_opacity, 0.6f)), round);
		a_drawList->AddRect(a_min, a_max, WithAlpha(white, a_alpha), round, 0, std::max(1.0f, size * 0.08f));
		// Up in the corner, clear of the icon
		Slashes(a_drawList, ImVec2(a_min.x + size * 0.16f, a_min.y + size * 0.12f), 1, std::min(size * 0.8f, height * 0.3f), size * 0.15f, WithAlpha(yellow, a_alpha));
		Slashes(a_drawList, ImVec2(a_max.x - size * 1.35f, a_min.y + size * 0.1f), 3, size * 0.22f, size * 0.2f, WithAlpha(graphite, 0.55f * a_alpha));
	}

	void HeaderTab(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_inward, float a_alpha) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float lean = height * 0.55f;
		const float round = size * 0.7f;
		const ImU32 fill = WithAlpha(IM_COL32(54, 54, 56, 255), std::max(a_alpha * 0.95f, 0.75f));
		const ImU32 rim = WithAlpha(steel, 0.75f);
		const float rimWidth = std::max(1.5f, size * 0.1f);

		// The inner end runs out on the slant, down to the plate
		const float inner = a_inward > 0.0f ? a_max.x : a_min.x;
		const float foot = inner + a_inward * lean;
		const float outer = a_inward > 0.0f ? a_min.x : a_max.x;
		a_drawList->AddRectFilled(a_min, a_max, fill, round, a_inward > 0.0f ? ImDrawFlags_RoundCornersTopLeft : ImDrawFlags_RoundCornersTopRight);
		a_drawList->AddTriangleFilled(ImVec2(inner, a_min.y), a_inward > 0.0f ? ImVec2(foot, a_max.y) : ImVec2(inner, a_max.y), a_inward > 0.0f ? ImVec2(inner, a_max.y) : ImVec2(foot, a_max.y), fill);
		a_drawList->AddLine(ImVec2(outer + a_inward * round, a_min.y), ImVec2(inner, a_min.y), rim, rimWidth);
		a_drawList->AddLine(ImVec2(inner, a_min.y), ImVec2(foot, a_max.y), rim, rimWidth);
		a_drawList->AddLine(ImVec2(outer, a_min.y + round), ImVec2(outer, a_max.y), rim, rimWidth);

		// A loose yellow sliver following the slant
		const float gap = size * 0.3f;
		const float bar = size * 0.32f;
		const float closer = inner + a_inward * gap;
		const float further = closer + a_inward * bar;
		const ImVec2 sliver[4] = { ImVec2(closer, a_min.y), ImVec2(further, a_min.y), ImVec2(further + a_inward * lean, a_max.y), ImVec2(closer + a_inward * lean, a_max.y) };
		FillShape(a_drawList, std::vector<ImVec2>(sliver, sliver + 4), yellow);
	}

	//---------------------------------------------------
	//-- Anime Decor ( Title ) --------------------------
	//---------------------------------------------------

	void TitleBack(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_bottom) {

		const float  size = ImGui::GetFontSize();
		// Feathered off smoothly on every side but the bottom, where the plates cover it, so it has no edge to see
		const float  feather = size * 3.5f;
		const ImVec2 inner(a_titleMin.x - size, a_titleMin.y);
		const ImVec2 outer(a_titleMax.x + size, a_bottom + size);
		Mesh(a_drawList, ImVec2(inner.x - feather * 1.6f, inner.y - feather), ImVec2(outer.x + feather * 1.6f, outer.y), size * 0.7f, size * 0.5f, [&](float a_x, float a_y) {
			const float dx = std::max({ inner.x - a_x, a_x - outer.x, 0.0f }) / (feather * 1.6f);
			const float dy = std::max(inner.y - a_y, 0.0f) / feather;
			return WithAlpha(white, 0.78f * Smooth(1.0f - std::sqrt(dx * dx + dy * dy)));
		});
		const float height = a_titleMax.y - a_titleMin.y;
		Slashes(a_drawList, ImVec2(a_titleMin.x - height * 0.95f, a_titleMin.y - height * 0.1f), 1, height * 1.15f, height * 0.17f, yellow);
	}

	void BoldText(ImDrawList* a_drawList, ImVec2 a_at, ImU32 a_colour, const char* a_text, float a_lean) {

		const int   firstVertex = a_drawList->VtxBuffer.Size;
		const float weight = std::max(0.6f, ImGui::GetFontSize() * 0.035f);
		for (const float offset : { 0.0f, weight, a_lean != 0.0f ? weight * 2.0f : weight }) {
			a_drawList->AddText(ImVec2(a_at.x + offset, a_at.y), a_colour, a_text);
		}
		if (a_lean != 0.0f) {
			const float baseline = a_at.y + ImGui::GetFontSize();
			for (int i = firstVertex; i < a_drawList->VtxBuffer.Size; ++i) {
				auto& vertex = a_drawList->VtxBuffer[i];
				vertex.pos.x += (baseline - vertex.pos.y) * a_lean;
			}
		}
	}

	//---------------------------------------------------
	//-- Anime Decor ( Nav Row ) ------------------------
	//---------------------------------------------------

	// A sidebar row: a soft dark plate; white with a yellow nick when it's the open page, signal yellow under the pointer
	void NavRow(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_chosen, float a_glow) {

		const float size = ImGui::GetFontSize();
		const float round = (a_max.y - a_min.y) * 0.34f;
		if (a_chosen) {
			a_drawList->AddRectFilled(a_min, a_max, white, round);
			const float tall = (a_max.y - a_min.y) * 0.6f;
			Slashes(a_drawList, ImVec2(a_min.x + size * 0.12f, (a_min.y + a_max.y - tall) * 0.5f), 1, tall, size * 0.16f, yellow);
			return;
		}
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(IM_COL32(255, 255, 255, 255), 0.05f), round);
		if (a_glow > 0.0f) {
			a_drawList->AddRectFilled(a_min, a_max, WithAlpha(yellow, a_glow), round);
		}
	}

	//---------------------------------------------------
	//-- Anime Decor ( Controls ) -----------------------
	//---------------------------------------------------

	// A button: dark glass, rounded, lit yellow under the pointer
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed) {

		const float round = ImGui::GetFontSize() * 0.3f;
		a_drawList->AddRectFilled(a_min, a_max, a_pressed ? ink : Mix(WithAlpha(glass, 0.9f), IM_COL32(92, 80, 24, 240), a_glow), round);
		a_drawList->AddRect(a_min, a_max, WithAlpha(Mix(steel, yellow, a_glow), 0.8f), round, 0, 1.0f);
	}

	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(white, 0.92f), size * 0.35f);
		a_drawList->AddRect(a_min, a_max, Mix(steel, yellowDeep, a_glow), size * 0.35f, 0, std::max(1.0f, size * 0.08f));
	}

	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(IM_COL32(240, 240, 243, 255), 0.95f), size * 0.5f);
		a_drawList->AddRect(a_min, a_max, a_lit ? WithAlpha(yellowDeep, 0.55f) : WithAlpha(steel, 0.7f), size * 0.5f, 0, std::max(1.0f, size * 0.07f));
	}

	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_fill) {

		const float  half = a_size * 0.5f;
		const ImVec2 min(a_centre.x - half, a_centre.y - half);
		const ImVec2 max(a_centre.x + half, a_centre.y + half);
		const float  round = a_size * 0.18f;
		a_drawList->AddRectFilled(min, max, WithAlpha(white, 0.9f), round);
		if (a_fill > 0.0f) {
			a_drawList->AddRectFilled(min, max, WithAlpha(yellow, a_fill), round);
			const ImVec2 tick[3] = { ImVec2(a_centre.x - half * 0.5f, a_centre.y), ImVec2(a_centre.x - half * 0.1f, a_centre.y + half * 0.42f), ImVec2(a_centre.x + half * 0.55f, a_centre.y - half * 0.42f) };
			a_drawList->AddPolyline(tick, 3, WithAlpha(ink, a_fill), ImDrawFlags_None, std::max(1.5f, a_size * 0.13f));
		}
		a_drawList->AddRect(min, max, a_fill > 0.5f ? yellowDeep : steel, round, 0, 1.0f);
	}

	// A chosen row: rounded at its head, cut on the slant at its tail with a loose sliver after it
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float  height = a_max.y - a_min.y;
		const float  round = height * 0.4f;
		const float  cut = height * 0.55f;
		const ImU32  colour = WithAlpha(yellow, a_alpha);
		const float  tail = a_max.x - height * 0.5f;
		a_drawList->AddRectFilled(a_min, ImVec2(a_min.x + round * 2.0f, a_max.y), colour, round);
		const ImVec2 body[4] = { ImVec2(a_min.x + round, a_min.y), ImVec2(tail, a_min.y), ImVec2(tail - cut, a_max.y), ImVec2(a_min.x + round, a_max.y) };
		a_drawList->AddConvexPolyFilled(body, 4, colour);
		const float  gap = height * 0.18f;
		const ImVec2 sliver[4] = { ImVec2(tail + gap, a_min.y), ImVec2(tail + gap + height * 0.2f, a_min.y), ImVec2(tail + gap + height * 0.2f - cut, a_max.y), ImVec2(tail + gap - cut, a_max.y) };
		a_drawList->AddConvexPolyFilled(sliver, 4, colour);
	}

	void Meter(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_fraction) {

		const float round = (a_max.y - a_min.y) * 0.5f;
		a_drawList->AddRectFilled(a_min, a_max, steel, round);
		if (a_fraction > 0.0f) {
			const float width = std::max((a_max.y - a_min.y), (a_max.x - a_min.x) * std::min(a_fraction, 1.0f));
			a_drawList->AddRectFilled(a_min, ImVec2(a_min.x + width, a_max.y), yellow, round);
		}
	}

	void Toolbar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(white, 0.28f), size * 0.5f);
		a_drawList->AddRect(a_min, a_max, WithAlpha(steel, 0.35f), size * 0.5f, 0, 1.0f);
	}

	//---------------------------------------------------
	//-- Anime Decor ( Headings ) -----------------------
	//---------------------------------------------------

	float PageHeadingHeight() noexcept {

		return ImGui::GetFontSize() * 3.2f;
	}

	float PageHeadingIndent() noexcept {

		return ImGui::GetFontSize() * 1.6f;
	}

	void PageHeading(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float lean = height * 0.3f;

		// The pale band behind the title, the slash that ends it, the band running on behind the count
		a_drawList->AddRectFilled(ImVec2(a_min.x, a_min.y + size * 0.15f), a_max, WithAlpha(white, 0.32f), size * 0.5f);
		const float slash = a_textEnd + size * 1.4f;
		Slant(a_drawList, ImVec2(a_min.x, a_min.y + size * 0.15f), ImVec2(slash, a_max.y), lean, WithAlpha(mist, 0.75f));
		const ImVec2 bar[4] = { ImVec2(slash + size * 0.12f, a_min.y + size * 0.15f), ImVec2(slash + size * 0.42f, a_min.y + size * 0.15f), ImVec2(slash + size * 0.42f - lean, a_max.y), ImVec2(slash + size * 0.12f - lean, a_max.y) };
		a_drawList->AddConvexPolyFilled(bar, 4, yellow);
	}

	void SectionBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float lean = height * 0.6f;
		const float end = std::min(a_max.x, a_textEnd + size * 2.5f);

		a_drawList->AddRectFilled(ImVec2(end - lean, a_min.y + height * 0.12f), a_max, WithAlpha(steel, 0.4f));
		Slant(a_drawList, a_min, ImVec2(end, a_max.y), lean, graphite);
		const ImVec2 wedge[3] = { a_min, ImVec2(a_min.x + height * 0.6f, a_min.y), ImVec2(a_min.x, a_max.y - height * 0.15f) };
		a_drawList->AddConvexPolyFilled(wedge, 3, yellow);
		const ImVec2 sliver[4] = { ImVec2(end + size * 0.05f, a_min.y), ImVec2(end + size * 0.25f, a_min.y), ImVec2(end + size * 0.25f - lean, a_max.y), ImVec2(end + size * 0.05f - lean, a_max.y) };
		a_drawList->AddConvexPolyFilled(sliver, 4, WithAlpha(graphite, 0.55f));
		if (a_max.x - end > size * 5.0f) {
			Slashes(a_drawList, ImVec2(a_max.x - size * 3.2f, a_min.y + height * 0.26f), 3, height * 0.6f, size * 0.42f, WithAlpha(graphite, 0.65f));
		}
	}

	void TitleBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_textEnd) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float lean = height * 0.6f;
		const float end = std::min(a_max.x, std::max(a_textEnd + size * 2.0f, a_min.x + (a_max.x - a_min.x) * 0.26f));

		a_drawList->AddRectFilled(ImVec2(end - lean, a_min.y + height * 0.1f), ImVec2(a_max.x, a_min.y + height * 0.62f), WithAlpha(steel, 0.32f));
		Slant(a_drawList, a_min, ImVec2(end, a_max.y), lean, yellow);
		const ImVec2 sliver[4] = { ImVec2(end + size * 0.25f, a_min.y), ImVec2(end + size * 0.75f, a_min.y), ImVec2(end + size * 0.75f - lean, a_max.y), ImVec2(end + size * 0.25f - lean, a_max.y) };
		a_drawList->AddConvexPolyFilled(sliver, 4, yellow);
	}

	//---------------------------------------------------
	//-- Anime Decor ( Mascots ) ------------------------
	//---------------------------------------------------

	void Peek(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style, float a_side) {

		// Eyes and mouth clear of the edge, the chin behind it
		DrawMascot(a_drawList, ImVec2(a_x, a_edge - a_radius * 0.92f), a_radius, a_style);
		if (a_side != 0.0f) {
			Burst(a_drawList, ImVec2(a_x + a_side * a_radius * 1.2f, a_edge - a_radius * 1.25f), a_radius * 0.8f, a_side);
		}
	}

	void Paws(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style) {

		const auto& style = mascots[static_cast<std::size_t>(a_style) % std::size(mascots)];
		for (const float side : { -1.0f, 1.0f }) {
			const ImVec2 min(a_x + side * a_radius * 0.72f - a_radius * 0.27f, a_edge - a_radius * 0.17f);
			const ImVec2 max(a_x + side * a_radius * 0.72f + a_radius * 0.27f, a_edge + a_radius * 0.11f);
			a_drawList->AddRectFilled(min, max, style.paw, a_radius * 0.13f);
			a_drawList->AddRect(min, max, pen, a_radius * 0.13f, 0, std::max(1.0f, a_radius * 0.035f));
			for (const float toe : { 0.35f, 0.65f }) {
				const float x = min.x + (max.x - min.x) * toe;
				a_drawList->AddLine(ImVec2(x, max.y - a_radius * 0.13f), ImVec2(x, max.y - 1.0f), pen, 1.0f);
			}
		}
	}

	void PeekOver(ImDrawList* a_drawList, float a_x, float a_edge, float a_radius, int a_style, float a_side) {

		a_drawList->PushClipRect(ImVec2(0.0f, 0.0f), ImVec2(ImGui::GetIO().DisplaySize.x, a_edge), false);
		Peek(a_drawList, a_x, a_edge, a_radius, a_style, a_side);
		a_drawList->PopClipRect();
		Paws(a_drawList, a_x, a_edge, a_radius, a_style);
	}

	void HeaderMascots(ImDrawList* a_drawList, float a_midX, float a_titleHalf, float a_edge) {

		const float size = ImGui::GetFontSize();
		headerRadius = size * 2.25f;
		headerEdge = a_edge;
		headerSpots[0] = { a_midX - a_titleHalf - headerRadius * 2.6f, 0 };
		headerSpots[1] = { a_midX + a_titleHalf + headerRadius * 2.6f, 1 };
		headerShown = true;
		Peek(a_drawList, headerSpots[0].x, headerEdge, headerRadius, headerSpots[0].style, -1.0f);
		Peek(a_drawList, headerSpots[1].x, headerEdge, headerRadius, headerSpots[1].style, 1.0f);
	}

	void HeaderPaws(ImDrawList* a_drawList) {

		if (std::exchange(headerShown, false)) {
			Paws(a_drawList, headerSpots[0].x, headerEdge, headerRadius, headerSpots[0].style);
			Paws(a_drawList, headerSpots[1].x, headerEdge, headerRadius, headerSpots[1].style);
		}
	}

	//---------------------------------------------------
	//-- Anime Decor ( Panel ) --------------------------
	//---------------------------------------------------

	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool) {

		const float size = ImGui::GetFontSize();
		if (a_max.x - a_min.x < size * 4.0f || a_max.y - a_min.y < size * 2.0f) {
			return;
		}
		const ImU32 top = WithAlpha(white, 0.5f);
		const ImU32 clear = WithAlpha(white, 0.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x + size, a_min.y + 1.0f), ImVec2(a_max.x - size, a_min.y + size * 1.5f), top, top, clear, clear);
	}

	// The registration marks of a technical drawing
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float reach = size * 0.28f;
		const ImU32 mark = WithAlpha(steel, 0.6f);
		for (const auto& corner : { ImVec2(a_min.x + size, a_max.y - size), ImVec2(a_max.x - size, a_max.y - size) }) {
			a_drawList->AddLine(ImVec2(corner.x - reach, corner.y), ImVec2(corner.x + reach, corner.y), mark, 1.0f);
			a_drawList->AddLine(ImVec2(corner.x, corner.y - reach), ImVec2(corner.x, corner.y + reach), mark, 1.0f);
		}
	}
}

namespace CompletionistUI::AnimeDecor
{
	//---------------------------------------------------
	//-- Anime Decor ( Perk Node ) ----------------------
	//---------------------------------------------------

	// Diamonds, as the markers are: filled yellow when taken, ringed when next, a small hollow one when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		const auto diamond = [&](float a_reach) {
			return std::vector<ImVec2>{ ImVec2(a_centre.x, a_centre.y - a_reach), ImVec2(a_centre.x + a_reach, a_centre.y), ImVec2(a_centre.x, a_centre.y + a_reach), ImVec2(a_centre.x - a_reach, a_centre.y) };
		};
		switch (a_state) {
		case PerkState::kTaken:
			{
				const auto shape = diamond(a_radius * 1.1f);
				FillShape(a_drawList, shape, yellow);
				a_drawList->AddPolyline(shape.data(), 4, ink, ImDrawFlags_Closed, std::max(1.2f, a_radius * 0.14f));
			}
			return;
		case PerkState::kNext:
			{
				const auto shape = diamond(a_radius);
				FillShape(a_drawList, shape, white);
				a_drawList->AddPolyline(shape.data(), 4, ink, ImDrawFlags_Closed, std::max(1.2f, a_radius * 0.14f));
			}
			return;
		default:
			{
				const auto shape = diamond(a_radius * 0.55f);
				FillShape(a_drawList, shape, pale);
				a_drawList->AddPolyline(shape.data(), 4, a_colour, ImDrawFlags_Closed, 1.0f);
			}
			return;
		}
	}

	//---------------------------------------------------
	//-- Anime Decor ( Perk Link ) ----------------------
	//---------------------------------------------------

	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto points = PerkCurve(a_from, a_to, std::max(2.0f, a_radius * 0.35f));
		if (a_lit) {
			a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), yellowDeep, ImDrawFlags_None, std::max(2.0f, a_radius * 0.24f));
			return;
		}
		for (std::size_t i = 0; i + 1 < points.size(); i += 3) {
			a_drawList->AddLine(points[i], points[std::min(i + 1, points.size() - 1)], a_colour, 1.2f);
		}
	}
}
