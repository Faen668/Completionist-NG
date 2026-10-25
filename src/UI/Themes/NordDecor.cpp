#include "UI/Themes/NordDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

namespace CompletionistUI::NordDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Timber, iron, stone, snow and ice
		constexpr ImU32 timber = IM_COL32(82, 62, 44, 255);
		constexpr ImU32 timberLight = IM_COL32(128, 100, 72, 255);
		constexpr ImU32 timberDark = IM_COL32(30, 23, 18, 255);
		constexpr ImU32 iron = IM_COL32(58, 62, 68, 255);
		constexpr ImU32 ironLight = IM_COL32(138, 146, 156, 255);
		constexpr ImU32 ironDark = IM_COL32(16, 18, 22, 255);
		constexpr ImU32 ironWorn = IM_COL32(96, 102, 112, 255);
		constexpr ImU32 snow = IM_COL32(226, 234, 242, 255);
		constexpr ImU32 frost = IM_COL32(150, 206, 236, 255);
		constexpr ImU32 shadow = IM_COL32(6, 8, 12, 255);

		//---------------------------------------------------
		//-- Nord Decor ( Lerp ) ----------------------------
		//---------------------------------------------------

		ImVec2 Lerp(ImVec2 a_from, ImVec2 a_to, float a_t) {

			return ImVec2(a_from.x + (a_to.x - a_from.x) * a_t, a_from.y + (a_to.y - a_from.y) * a_t);
		}

		//---------------------------------------------------
		//-- Nord Decor ( Range ) ---------------------------
		//---------------------------------------------------

		// A mountain range across the bottom, snow on its peaks
		void Range(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, Random& a_random, float a_rise, ImU32 a_fill, float a_snow, float a_peakAt, float a_peakRise) {

			const float   width = a_max.x - a_min.x;
			constexpr int count = 56;
			ImVec2        ridge[count + 1];
			float         walk = 0.0f;
			const float   phase = a_random.Range(0.0f, pi * 2.0f);
			for (int i = 0; i <= count; ++i) {
				const float t = static_cast<float>(i) / count;
				walk = std::clamp(walk + a_random.Range(-0.2f, 0.2f), -0.45f, 0.45f);
				const float peak = a_peakRise * std::max(0.0f, 1.0f - std::abs(t - a_peakAt) / 0.14f);
				const float height = a_rise * (0.5f + 0.22f * std::sin(t * 9.0f + phase) + 0.3f * walk) + peak;
				ridge[i] = ImVec2(a_min.x + width * t, a_max.y - height);
			}
			for (const auto& point : ridge) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathLineTo(a_max);
			a_drawList->PathLineTo(ImVec2(a_min.x, a_max.y));
			a_drawList->PathFillConcave(a_fill);

			// Snow caps on the peaks, more on the high ones
			for (int i = 1; i < count; ++i) {
				if (ridge[i].y > ridge[i - 1].y || ridge[i].y > ridge[i + 1].y) {
					continue;
				}
				const float  height = (a_max.y - ridge[i].y) / std::max(1.0f, a_rise + a_peakRise);
				const float  reach = std::clamp(height * 0.9f, 0.25f, 0.7f);
				const ImVec2 left = Lerp(ridge[i], ridge[i - 1], reach);
				const ImVec2 right = Lerp(ridge[i], ridge[i + 1], reach);
				const ImVec2 dip(ridge[i].x, std::max(left.y, right.y) + (left.y - ridge[i].y) * 0.4f);
				const ImVec2 cap[4] = { ridge[i], right, dip, left };
				a_drawList->AddConvexPolyFilled(cap, 4, WithAlpha(snow, a_snow * std::clamp(height * 1.4f, 0.3f, 1.0f)));
			}
		}

		//---------------------------------------------------
		//-- Nord Decor ( Snowfall ) ------------------------
		//---------------------------------------------------

		// Backdrop: cold night over dark stone, the Throat of the World, snow drifting down
		void Snowfall(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const bool  full = height > size * 20.0f;
			a_drawList->PushClipRect(a_min, a_max, true);

			// Pale cold sky above, dark stone below
			const ImVec2 sky[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, sky, { IM_COL32(58, 72, 92, 60), IM_COL32(58, 72, 92, 60), IM_COL32(10, 12, 16, 60), IM_COL32(10, 12, 16, 60) });

			Random random{ 0x5C0u };

			if (full) {
				// Mist over the far range, then the depth one
				const float rise = height * 0.16f;
				SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.7f, a_max.y - rise * 2.2f), size * 14.0f, IM_COL32(120, 150, 180, 3));
				Range(a_drawList, a_min, a_max, random, rise * 1.1f, IM_COL32(30, 36, 46, 220), 0.32f, 0.7f, rise * 1.5f);
				a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - rise * 1.2f), a_max, IM_COL32(150, 170, 190, 0), IM_COL32(150, 170, 190, 0), IM_COL32(150, 170, 190, 14), IM_COL32(150, 170, 190, 14));
				Range(a_drawList, a_min, a_max, random, rise * 0.55f, IM_COL32(14, 16, 20, 235), 0.18f, 0.2f, rise * 0.35f);
			}

			// Snow, the large flakes nearer and softer
			const int flakes = std::clamp(static_cast<int>(width * height / (size * size * 3.0f)), 30, 420);
			for (int i = 0; i < flakes; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  depth = random.Next();
				const float  radius = 0.7f + depth * depth * size * 0.12f;
				if (depth > 0.85f) {
					SoftSpot(a_drawList, at, radius * 2.5f, WithAlpha(snow, 0.025f));
				}
				a_drawList->AddCircleFilled(at, radius, WithAlpha(snow, random.Range(0.12f, 0.42f) * (0.5f + depth * 0.5f)), 8);
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 5, 8, 200));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Nord Decor ( Timber Band ) ---------------------
		//---------------------------------------------------

		// One side of the frame: weathered planks bound with iron straps
		void TimberBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, timberDark);

			Random      random{ a_seed };
			const float plank = depth * 9.0f;
			for (float along = 0.0f; along < length; along += plank) {
				const float end = std::min(length, along + plank);
				a_drawList->AddRectFilled(at(along + 1.0f, depth * 0.1f), at(end - 1.0f, depth * 0.9f), Shade(timber, random.Range(0.8f, 1.08f)));
				// Grain running along the plank
				for (int line = 0; line < 3; ++line) {
					const float across = depth * random.Range(0.2f, 0.8f);
					const float from = along + depth * random.Range(0.2f, 1.5f);
					const float to = end - depth * random.Range(0.2f, 1.5f);
					if (to > from) {
						a_drawList->AddLine(at(from, across), at(to, across), WithAlpha(timberDark, random.Range(0.35f, 0.7f)), 1.0f);
					}
				}
				a_drawList->AddLine(at(along + 1.0f, depth * 0.14f), at(end - 1.0f, depth * 0.14f), WithAlpha(timberLight, 0.5f), 1.0f);
				// A knot in the wood now and then
				if (random.Next() < 0.4f) {
					const ImVec2 knot = at(along + (end - along) * random.Range(0.25f, 0.75f), depth * random.Range(0.35f, 0.65f));
					a_drawList->AddCircleFilled(knot, depth * 0.11f, WithAlpha(timberDark, 0.8f), 10);
					a_drawList->AddCircle(knot, depth * 0.2f, WithAlpha(timberDark, 0.45f), 12, 1.0f);
				}
			}

			// Iron straps, riveted
			const float strap = depth * 0.7f;
			for (float along = depth * 3.0f; along + strap < length - depth * 2.0f; along += depth * 7.5f) {
				a_drawList->AddRectFilled(at(along, 0.0f), at(along + strap, depth), iron);
				a_drawList->AddLine(at(along + 1.0f, 0.0f), at(along + 1.0f, depth), WithAlpha(ironLight, 0.6f), 1.0f);
				a_drawList->AddRect(at(along, 0.0f), at(along + strap, depth), ironDark, 0.0f, 0, 1.0f);
				for (const float across : { depth * 0.28f, depth * 0.72f }) {
					a_drawList->AddCircleFilled(at(along + strap * 0.5f, across), depth * 0.09f, ironLight, 8);
				}
			}
		}

		//---------------------------------------------------
		//-- Nord Decor ( Snow Cap ) ------------------------
		//---------------------------------------------------

		// Snow lying along the top of the frame, icicles hanging under it
		void SnowCap(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_band) {

			const float width = a_max.x - a_min.x;
			Random      random{ static_cast<std::uint32_t>(width) * 977u + 3u };
			const float step = a_band * 0.5f;
			float       x = a_min.x;
			float       depth = a_band * 0.3f;
			while (x < a_max.x) {
				const float next = std::min(a_max.x, x + step);
				const float nextDepth = std::clamp(depth + random.Range(-0.08f, 0.08f) * a_band, a_band * 0.18f, a_band * 0.42f);
				const ImVec2 quad[4] = { ImVec2(x, a_min.y), ImVec2(next, a_min.y), ImVec2(next, a_min.y + nextDepth), ImVec2(x, a_min.y + depth) };
				a_drawList->AddConvexPolyFilled(quad, 4, WithAlpha(snow, 0.85f));
				a_drawList->AddLine(quad[3], quad[2], WithAlpha(IM_COL32(150, 170, 190, 255), 0.7f), 1.0f);
				x = next;
				depth = nextDepth;
			}

			// Icicles under the top band, kept short so they clear the header
			for (float at = a_min.x + a_band * 2.5f; at < a_max.x - a_band * 2.5f; at += a_band * random.Range(1.2f, 3.5f)) {
				if (random.Next() < 0.45f) {
					continue;
				}
				const float  length = a_band * random.Range(0.2f, 0.55f);
				const float  half = a_band * random.Range(0.08f, 0.14f);
				const ImVec2 icicle[3] = { ImVec2(at - half, a_max.y), ImVec2(at + half, a_max.y), ImVec2(at + half * 0.2f, a_max.y + length) };
				a_drawList->AddConvexPolyFilled(icicle, 3, WithAlpha(IM_COL32(190, 220, 240, 255), 0.55f));
				a_drawList->AddLine(icicle[0], icicle[2], WithAlpha(snow, 0.6f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Nord Decor ( Iron Corner ) ---------------------
		//---------------------------------------------------

		// Iron plate at each corner, riveted, the star struck into it
		void IronCorner(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, iron);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), WithAlpha(ironLight, 0.7f));
			a_drawList->AddLine(ImVec2(min.x + 1.0f, min.y), ImVec2(min.x + 1.0f, max.y), WithAlpha(ironLight, 0.4f));
			a_drawList->AddRect(min, max, ironDark, 0.0f, 0, 1.5f);
			const float inset = a_size * 0.25f;
			for (const auto& rivet : { ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), ImVec2(min.x + inset, max.y - inset) }) {
				a_drawList->AddCircleFilled(rivet, a_size * 0.09f, ironLight, 8);
			}
			SoftSpot(a_drawList, a_centre, a_size * 0.8f, WithAlpha(frost, 0.04f));
			Star(a_drawList, a_centre, a_size * 0.55f, frost, true);
		}

		//---------------------------------------------------
		//-- Nord Decor ( Frost ) ---------------------------
		//---------------------------------------------------

		// Rime spreading in from a corner: short branches, side shoots at sixty degrees, fading as they go
		void Frost(ImDrawList* a_drawList, ImVec2 a_corner, float a_dirX, float a_dirY, float a_reach, Random& a_random) {

			const float base = std::atan2(a_dirY, a_dirX);
			for (int branch = 0; branch < 7; ++branch) {
				const float  angle = base + a_random.Range(-0.7f, 0.7f);
				const float  reach = a_reach * a_random.Range(0.3f, 1.0f);
				const ImVec2 tip(a_corner.x + std::cos(angle) * reach, a_corner.y + std::sin(angle) * reach);
				constexpr int pieces = 4;
				for (int piece = 0; piece < pieces; ++piece) {
					const float t = static_cast<float>(piece) / pieces;
					a_drawList->AddLine(Lerp(a_corner, tip, t), Lerp(a_corner, tip, t + 1.0f / pieces), WithAlpha(frost, 0.2f * (1.0f - t)), 1.0f);
				}
				for (float t = 0.2f; t < 0.85f; t += 0.16f) {
					const ImVec2 from = Lerp(a_corner, tip, t);
					const float  shoot = reach * 0.2f * (1.0f - t);
					const float  fade = 0.16f * (1.0f - t);
					for (const float side : { -1.0f, 1.0f }) {
						const float  turn = angle + side * pi / 3.0f;
						const ImVec2 to(from.x + std::cos(turn) * shoot, from.y + std::sin(turn) * shoot);
						a_drawList->AddLine(from, to, WithAlpha(frost, fade), 1.0f);
					}
				}
			}
			SoftSpot(a_drawList, a_corner, a_reach * 0.5f, WithAlpha(frost, 0.01f));
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Star ) ----------------------------
	//---------------------------------------------------

	// In place of the diamond: an eight-pointed star, long points to the four winds
	void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float radius = a_radius * 1.25f;
		ImVec2      points[16];
		for (int i = 0; i < 16; ++i) {
			const float angle = static_cast<float>(i) * pi / 8.0f - pi * 0.5f;
			const float reach = i % 2 == 1 ? 0.24f : (i % 4 == 0 ? 1.0f : 0.55f);
			points[i] = ImVec2(a_centre.x + std::cos(angle) * radius * reach, a_centre.y + std::sin(angle) * radius * reach);
		}
		for (const auto& point : points) {
			a_drawList->PathLineTo(point);
		}
		if (a_filled) {
			a_drawList->PathFillConcave(a_colour);
		} else {
			a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.16f));
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Rune Stone ) ----------------------
	//---------------------------------------------------

	// Marker: a standing stone, its rune lit when found
	void RuneStone(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float halfWidth = a_size * 0.24f;
		const float halfHeight = a_size * 0.36f;
		const float top = a_centre.y - halfHeight + halfWidth;
		const float bottom = a_centre.y + halfHeight;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		if (a_fill >= 1.0f) {
			SoftSpot(a_drawList, a_centre, halfHeight * 1.7f, WithAlpha(frost, 0.04f));
		}
		a_drawList->PathLineTo(ImVec2(a_centre.x - halfWidth, bottom));
		a_drawList->PathArcTo(ImVec2(a_centre.x, top), halfWidth, pi, pi * 2.0f, 12);
		a_drawList->PathLineTo(ImVec2(a_centre.x + halfWidth, bottom));
		if (a_fill > 0.0f) {
			ImDrawList& list = *a_drawList;
			list.PathFillConvex(WithAlpha(a_colour, 0.18f * a_fill));
			list.PathLineTo(ImVec2(a_centre.x - halfWidth, bottom));
			list.PathArcTo(ImVec2(a_centre.x, top), halfWidth, pi, pi * 2.0f, 12);
			list.PathLineTo(ImVec2(a_centre.x + halfWidth, bottom));
		}
		a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, stroke);
		// Ground line under the stone
		a_drawList->AddLine(ImVec2(a_centre.x - halfWidth * 1.5f, bottom), ImVec2(a_centre.x + halfWidth * 1.5f, bottom), a_colour, stroke);

		if (a_fill > 0.0f) {
			// The rune: a stave with arms raised, growing in as it's found
			const float  reach = 0.5f + 0.5f * a_fill;
			const ImVec2 head(a_centre.x, a_centre.y - halfHeight * 0.45f * reach);
			const ImVec2 foot(a_centre.x, a_centre.y + halfHeight * 0.6f * reach);
			const ImVec2 left(a_centre.x - halfWidth * 0.6f * reach, head.y + halfHeight * 0.4f * reach);
			const ImVec2 right(a_centre.x + halfWidth * 0.6f * reach, head.y + halfHeight * 0.4f * reach);
			for (const auto& [colour, thick] : { std::pair{ a_colour, stroke * 1.1f }, std::pair{ WithAlpha(snow, a_fill * 0.8f), std::max(1.0f, stroke * 0.45f) } }) {
				a_drawList->AddLine(head, foot, colour, thick);
				a_drawList->AddLine(head, left, colour, thick);
				a_drawList->AddLine(head, right, colour, thick);
			}
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Knot ) ----------------------------
	//---------------------------------------------------

	// Four loops through a ring, cut dark where they cross
	void Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const float stroke = std::max(1.0f, a_radius * 0.14f);
		const ImU32 dark = WithAlpha(shadow, ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f);
		for (const auto& [colour, thick] : { std::pair{ dark, stroke + 2.0f }, std::pair{ a_colour, stroke } }) {
			for (int loop = 0; loop < 4; ++loop) {
				const float  angle = static_cast<float>(loop) * pi * 0.5f - pi * 0.5f;
				const ImVec2 out(std::cos(angle), std::sin(angle));
				const ImVec2 side(-out.y, out.x);
				const ImVec2 first(a_centre.x + (out.x * 1.3f + side.x * 0.8f) * a_radius, a_centre.y + (out.y * 1.3f + side.y * 0.8f) * a_radius);
				const ImVec2 second(a_centre.x + (out.x * 1.3f - side.x * 0.8f) * a_radius, a_centre.y + (out.y * 1.3f - side.y * 0.8f) * a_radius);
				a_drawList->AddBezierCubic(a_centre, first, second, a_centre, colour, thick, 16);
			}
			a_drawList->AddCircle(a_centre, a_radius * 0.55f, colour, 24, thick);
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Knotwork ) ------------------------
	//---------------------------------------------------

	// Two strands braided over and under, in place of a rule
	void Knotwork(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float   wave = a_size * 0.2f;
		const float   unit = a_size * 0.7f;	// one crossing to the next
		const float   stroke = std::max(1.0f, a_size * 0.1f);
		constexpr int steps = 8;
		const auto    fadeAt = [&](float a_x) {
			return std::pow(std::sin(std::clamp((a_x - a_from.x) / length, 0.0f, 1.0f) * pi), 0.6f);
		};
		const auto strand = [&](float a_x, float a_sign) {
			return ImVec2(a_x, a_from.y + a_sign * wave * std::sin((a_x - a_from.x) / unit * pi));
		};

		// Each piece runs peak to peak around one crossing; the strand on top swaps each time
		int crossing = 0;
		for (float start = a_from.x - unit * 0.5f; start < a_to.x; start += unit, ++crossing) {
			const float over = crossing % 2 == 0 ? 1.0f : -1.0f;
			for (const auto& [sign, cut] : { std::pair{ -over, false }, std::pair{ over, true }, std::pair{ over, false } }) {
				for (int s = 0; s < steps; ++s) {
					const float x0 = std::clamp(start + unit * s / steps, a_from.x, a_to.x);
					const float x1 = std::clamp(start + unit * (s + 1) / steps, a_from.x, a_to.x);
					if (x1 <= x0) {
						continue;
					}
					const float fade = fadeAt((x0 + x1) * 0.5f);
					if (cut) {
						a_drawList->AddLine(strand(x0, sign), strand(x1, sign), WithAlpha(shadow, fade), stroke + 2.0f);
					} else {
						a_drawList->AddLine(strand(x0, sign), strand(x1, sign), WithAlpha(a_colour, fade), stroke);
					}
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Divider ) -------------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Knotwork(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Knotwork(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Knot(a_drawList, ImVec2(midX, a_from.y), gap * 0.42f, a_colour);
	}

	//---------------------------------------------------
	//-- Nord Decor ( Frame ) ---------------------------
	//---------------------------------------------------

	// Timber and iron over the snow, snow lying on top, iron plates at the corners
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Snowfall(a_drawList, a_min, a_max);

		TimberBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0x4E01u);
		TimberBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0x4E02u);
		TimberBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0x4E03u);
		TimberBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0x4E04u);
		SnowCap(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), band);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(frost, 0.25f), 0.0f, 0, 1.0f);

		const float corner = band * 1.05f;
		IronCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);
	}

	//---------------------------------------------------
	//-- Nord Decor ( Panel ) ---------------------------
	//---------------------------------------------------

	// Inside a panel: dark stone at the edges clearing towards the text, rime in the corners, the knot in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		// Cold light from above, snow-glow along the bottom
		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + glow), WithAlpha(frost, 0.05f), WithAlpha(frost, 0.05f), WithAlpha(frost, 0.0f), WithAlpha(frost, 0.0f));
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(snow, 0.0f), WithAlpha(snow, 0.0f), WithAlpha(snow, 0.04f), WithAlpha(snow, 0.04f));

		// Stone grain
		Random    random{ static_cast<std::uint32_t>(width) * 37u + static_cast<std::uint32_t>(height) };
		const int grains = std::clamp(static_cast<int>(width * height / (size * size * 0.35f)), 60, 2400);
		for (int i = 0; i < grains; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.02f) {
				continue;
			}
			const bool  light = random.Next() < 0.55f;
			const ImU32 colour = light ? IM_COL32(120, 132, 146, 255) : IM_COL32(0, 0, 0, 255);
			a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.6f), WithAlpha(colour, weight * (light ? 0.22f : 0.4f)), 5);
		}

		// Rime creeping in from the corners
		const float reach = std::min({ width * 0.15f, height * 0.35f, size * 3.5f });
		Frost(a_drawList, a_min, 1.0f, 1.0f, reach, random);
		Frost(a_drawList, ImVec2(a_max.x, a_min.y), -1.0f, 1.0f, reach, random);
		Frost(a_drawList, a_max, -1.0f, -1.0f, reach, random);
		Frost(a_drawList, ImVec2(a_min.x, a_max.y), 1.0f, -1.0f, reach, random);

		// The knot, large and faint, ringed with rune marks, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const ImU32  faint = WithAlpha(IM_COL32(200, 220, 236, 255), 0.08f);
			SoftSpot(a_drawList, centre, radius * 1.2f, WithAlpha(frost, 0.008f));
			a_drawList->AddCircle(centre, radius * 1.12f, faint, 64, std::max(1.0f, radius * 0.02f));
			a_drawList->AddCircle(centre, radius * 0.98f, WithAlpha(faint, 0.6f), 64, std::max(1.0f, radius * 0.015f));
			for (int mark = 0; mark < 24; ++mark) {
				const float  angle = static_cast<float>(mark) * pi / 12.0f;
				const ImVec2 out(std::cos(angle), std::sin(angle));
				const float  tilt = mark % 3 == 0 ? 0.0f : (mark % 3 == 1 ? 0.25f : -0.25f);
				const ImVec2 inner(centre.x + out.x * radius * 1.0f, centre.y + out.y * radius * 1.0f);
				const ImVec2 outer(centre.x + (out.x + -out.y * tilt) * radius * 1.1f, centre.y + (out.y + out.x * tilt) * radius * 1.1f);
				a_drawList->AddLine(inner, outer, faint, std::max(1.0f, radius * 0.02f));
			}
			Knot(a_drawList, centre, radius * 0.55f, WithAlpha(faint, 0.9f));
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Panel Corners ) -------------------
	//---------------------------------------------------

	// Iron brackets on the panel's corners, riveted
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.22f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, ironDark, ImDrawFlags_None, thick + 2.0f);
			a_drawList->AddPolyline(points, 3, ironWorn, ImDrawFlags_None, thick);
			a_drawList->AddLine(ImVec2(corner.x + sx * thick * 0.3f, corner.y - sy * thick * 0.3f), ImVec2(corner.x + sx * arm * 0.9f, corner.y - sy * thick * 0.3f), WithAlpha(ironLight, 0.7f), 1.0f);
			for (const auto& rivet : { corner, ImVec2(corner.x + sx * arm * 0.7f, corner.y), ImVec2(corner.x, corner.y + sy * arm * 0.7f) }) {
				a_drawList->AddCircleFilled(rivet, thick * 0.32f, ironLight, 8);
			}
		}
	}
}
