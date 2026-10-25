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

		// Timber, iron, stone, snow and the hearth
		constexpr ImU32 timber = IM_COL32(82, 62, 44, 255);
		constexpr ImU32 timberLight = IM_COL32(128, 100, 72, 255);
		constexpr ImU32 timberDark = IM_COL32(30, 23, 18, 255);
		constexpr ImU32 iron = IM_COL32(58, 62, 68, 255);
		constexpr ImU32 ironLight = IM_COL32(138, 146, 156, 255);
		constexpr ImU32 ironDark = IM_COL32(16, 18, 22, 255);
		constexpr ImU32 ironWorn = IM_COL32(96, 102, 112, 255);
		constexpr ImU32 snow = IM_COL32(222, 222, 218, 255);
		constexpr ImU32 hearth = IM_COL32(214, 122, 52, 255);
		constexpr ImU32 ochre = IM_COL32(192, 71, 46, 255);
		constexpr ImU32 brass = IM_COL32(194, 135, 62, 255);
		constexpr ImU32 shadow = IM_COL32(8, 6, 4, 255);

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

		// Backdrop: a mead hall's dark stone, the hearth glowing below, the range beyond and snow drifting down
		void Snowfall(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const bool  full = height > size * 20.0f;
			a_drawList->PushClipRect(a_min, a_max, true);

			// Warm stone above, darker towards the floor
			const ImVec2 sky[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, sky, { IM_COL32(40, 34, 28, 60), IM_COL32(40, 34, 28, 60), IM_COL32(12, 10, 8, 60), IM_COL32(12, 10, 8, 60) });

			Random random{ 0x5C0u };

			if (full) {
				// Mist over the far range, then the depth one
				const float rise = height * 0.16f;
				Range(a_drawList, a_min, a_max, random, rise * 1.1f, IM_COL32(30, 27, 24, 200), 0.14f, 0.7f, rise * 1.5f);
				Range(a_drawList, a_min, a_max, random, rise * 0.55f, IM_COL32(16, 14, 12, 230), 0.08f, 0.2f, rise * 0.35f);
			}

			// The hearth, out of sight below, warming the room
			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_max.y + size * 2.0f), std::max(width * 0.45f, size * 10.0f), WithAlpha(hearth, 0.012f));

			// Snow: fewer, larger flakes than a sky of stars, the near ones soft
			const int flakes = std::clamp(static_cast<int>(width * height / (size * size * 9.0f)), 16, 160);
			for (int i = 0; i < flakes; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  depth = random.Next();
				const float  radius = 1.2f + depth * size * 0.16f;
				if (depth > 0.7f) {
					SoftSpot(a_drawList, at, radius * 2.0f, WithAlpha(snow, 0.03f));
				}
				a_drawList->AddCircleFilled(at, radius, WithAlpha(snow, random.Range(0.12f, 0.3f) * (0.6f + depth * 0.4f)), 10);
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(6, 4, 2, 200));
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

		// Snow lying along the top of the frame
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
				a_drawList->AddLine(quad[3], quad[2], WithAlpha(IM_COL32(150, 146, 140, 255), 0.7f), 1.0f);
				x = next;
				depth = nextDepth;
			}
		}

		//---------------------------------------------------
		//-- Nord Decor ( Iron Corner ) ---------------------
		//---------------------------------------------------

		// Iron plate at each corner, riveted, a boss in the middle
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
			Boss(a_drawList, a_centre, a_size * 0.42f, brass, true);
		}

		//---------------------------------------------------
		//-- Nord Decor ( Dragon Head ) ---------------------
		//---------------------------------------------------

		// A carved dragon's head on the gable end, as on Dragonsreach and the longhouses; a_facing is -1 or 1
		void DragonHead(ImDrawList* a_drawList, ImVec2 a_base, float a_size, float a_facing) {

			const auto at = [&](float a_x, float a_y) { return ImVec2(a_base.x + a_x * a_size * a_facing, a_base.y + a_y * a_size); };
			// Neck rising from the beam, curling out into the head, the jaw open
			const ImVec2 outline[] = {
				at(-0.10f, 0.00f), at(-0.14f, -0.30f), at(-0.06f, -0.58f), at(0.12f, -0.78f), at(0.36f, -0.86f),
				at(0.58f, -0.80f), at(0.66f, -0.70f), at(0.52f, -0.64f), at(0.70f, -0.56f), at(0.48f, -0.52f),
				at(0.34f, -0.56f), at(0.24f, -0.48f), at(0.30f, -0.36f), at(0.20f, -0.30f), at(0.16f, -0.14f), at(0.18f, 0.00f),
			};
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(timber);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathStroke(timberDark, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.03f));

			// Carved scales down the neck, the horn swept back, an eye
			for (float t = 0.15f; t < 0.8f; t += 0.16f) {
				a_drawList->PathArcTo(at(0.02f, -t * 0.7f), a_size * 0.08f, 0.2f, 2.9f, 6);
				a_drawList->PathStroke(WithAlpha(timberLight, 0.6f), ImDrawFlags_None, 1.0f);
			}
			a_drawList->AddBezierQuadratic(at(0.28f, -0.82f), at(0.10f, -0.98f), at(-0.12f, -0.94f), timberLight, std::max(1.0f, a_size * 0.05f), 10);
			a_drawList->AddCircleFilled(at(0.42f, -0.74f), std::max(1.2f, a_size * 0.035f), brass, 8);
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Boss ) ----------------------------
	//---------------------------------------------------

	// In place of the diamond: an iron shield boss, the dome brass-bright when filled
	void Boss(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float radius = a_radius * 1.05f;
		a_drawList->AddCircleFilled(a_centre, radius, WithAlpha(ironDark, 0.9f), 16);
		a_drawList->AddCircle(a_centre, radius, a_filled ? ironLight : a_colour, 16, std::max(1.0f, radius * 0.2f));
		if (a_filled) {
			a_drawList->AddCircleFilled(a_centre, radius * 0.55f, a_colour, 12);
			a_drawList->AddCircleFilled(ImVec2(a_centre.x - radius * 0.18f, a_centre.y - radius * 0.18f), radius * 0.18f, WithAlpha(IM_COL32(255, 236, 200, 255), 0.6f), 8);
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Rune Stone ) ----------------------
	//---------------------------------------------------

	// Marker: a standing stone, its rune painted in red ochre when found, as the Nords painted theirs
	void RuneStone(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float halfWidth = a_size * 0.24f;
		const float halfHeight = a_size * 0.36f;
		const float top = a_centre.y - halfHeight + halfWidth;
		const float bottom = a_centre.y + halfHeight;
		const float stroke = std::max(1.0f, a_size * 0.07f);
		a_drawList->PathLineTo(ImVec2(a_centre.x - halfWidth, bottom));
		a_drawList->PathArcTo(ImVec2(a_centre.x, top), halfWidth, pi, pi * 2.0f, 12);
		a_drawList->PathLineTo(ImVec2(a_centre.x + halfWidth, bottom));
		if (a_fill > 0.0f) {
			ImDrawList& list = *a_drawList;
			list.PathFillConvex(WithAlpha(IM_COL32(78, 74, 68, 255), a_fill));
			list.PathLineTo(ImVec2(a_centre.x - halfWidth, bottom));
			list.PathArcTo(ImVec2(a_centre.x, top), halfWidth, pi, pi * 2.0f, 12);
			list.PathLineTo(ImVec2(a_centre.x + halfWidth, bottom));
		}
		a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, stroke);
		// Ground line under the stone
		a_drawList->AddLine(ImVec2(a_centre.x - halfWidth * 1.5f, bottom), ImVec2(a_centre.x + halfWidth * 1.5f, bottom), a_colour, stroke);

		if (a_fill > 0.0f) {
			// The rune: a stave with arms raised, painted in as it's found
			const float  reach = 0.5f + 0.5f * a_fill;
			const ImVec2 head(a_centre.x, a_centre.y - halfHeight * 0.45f * reach);
			const ImVec2 foot(a_centre.x, a_centre.y + halfHeight * 0.6f * reach);
			const ImVec2 left(a_centre.x - halfWidth * 0.6f * reach, head.y + halfHeight * 0.4f * reach);
			const ImVec2 right(a_centre.x + halfWidth * 0.6f * reach, head.y + halfHeight * 0.4f * reach);
			for (const auto& [colour, thick] : { std::pair{ WithAlpha(ochre, a_fill), stroke * 1.4f }, std::pair{ WithAlpha(IM_COL32(232, 120, 84, 255), a_fill * 0.5f), std::max(1.0f, stroke * 0.45f) } }) {
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
		Boss(a_drawList, ImVec2(midX, a_from.y), gap * 0.32f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Nord Decor ( Frame ) ---------------------------
	//---------------------------------------------------

	// Timber beams and iron straps, snow lying on top, dragon heads carved on the top corners
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
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(brass, 0.25f), 0.0f, 0, 1.0f);

		const float corner = band * 1.05f;
		IronCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		IronCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);

		// Only on the main window; small panels keep plain corners. Behind the window, so they
		// rise above it and their necks go in behind the top beam
		if (compact >= 0.8f) {
			auto*       behind = ImGui::GetBackgroundDrawList();
			const float head = band * 2.6f;
			DragonHead(behind, ImVec2(a_min.x + band * 1.4f, a_min.y + band * 0.6f), head, -1.0f);
			DragonHead(behind, ImVec2(a_max.x - band * 1.4f, a_min.y + band * 0.6f), head, 1.0f);
		}
	}

	//---------------------------------------------------
	//-- Nord Decor ( Panel ) ---------------------------
	//---------------------------------------------------

	// Inside a panel: dark stone at the edges clearing towards the text, hearth light from below, a round shield in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		// Firelight rising from the bottom edge
		const float glow = std::min(height * 0.35f, size * 6.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(hearth, 0.0f), WithAlpha(hearth, 0.0f), WithAlpha(hearth, 0.07f), WithAlpha(hearth, 0.07f));

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
			const ImU32 colour = light ? IM_COL32(140, 128, 112, 255) : IM_COL32(0, 0, 0, 255);
			a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.6f), WithAlpha(colour, weight * (light ? 0.22f : 0.4f)), 5);
		}

		// A round shield, large and faint, in the lower right: planks, an iron rim and the boss
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const float  alpha = 0.16f;
			a_drawList->AddCircleFilled(centre, radius, WithAlpha(timber, alpha), 48);
			for (int plank = -3; plank <= 3; ++plank) {
				const float x = centre.x + static_cast<float>(plank) * radius * 0.27f;
				const float half = std::sqrt(std::max(0.0f, radius * radius - (x - centre.x) * (x - centre.x)));
				a_drawList->AddLine(ImVec2(x, centre.y - half), ImVec2(x, centre.y + half), WithAlpha(timberDark, alpha * 1.6f), 1.0f);
			}
			a_drawList->AddCircle(centre, radius, WithAlpha(ironLight, alpha * 1.2f), 48, std::max(2.0f, radius * 0.07f));
			for (int rivet = 0; rivet < 12; ++rivet) {
				const float angle = static_cast<float>(rivet) * pi / 6.0f;
				a_drawList->AddCircleFilled(ImVec2(centre.x + std::cos(angle) * radius * 0.9f, centre.y + std::sin(angle) * radius * 0.9f), std::max(1.0f, radius * 0.025f), WithAlpha(ironLight, alpha * 1.5f), 6);
			}
			a_drawList->AddCircleFilled(centre, radius * 0.24f, WithAlpha(iron, alpha * 2.0f), 24);
			a_drawList->AddCircle(centre, radius * 0.24f, WithAlpha(ironLight, alpha * 1.5f), 24, std::max(1.0f, radius * 0.03f));
			a_drawList->AddCircleFilled(centre, radius * 0.1f, WithAlpha(brass, alpha * 2.0f), 16);
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

namespace CompletionistUI::NordDecor
{
	//---------------------------------------------------
	//-- Nord Decor ( Perk Node ) --------------------
	//---------------------------------------------------

	// Rune stones: painted when taken, cut but bare when next, rough when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		const float size = a_radius * (a_state == PerkState::kLocked ? 1.7f : 2.5f);
		RuneStone(a_drawList, a_centre, size, a_colour, a_state == PerkState::kTaken ? 1.0f : 0.0f);
	}

	//---------------------------------------------------
	//-- Nord Decor ( Perk Link ) --------------------
	//---------------------------------------------------

	// Carved channels, straight runs and a sharp turn, doubled where the perks are taken
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const float  bend = std::min(std::abs(a_to.y - a_from.y) * 0.5f, (a_to.x - a_from.x) * 0.3f);
		const float  middle = (a_from.x + a_to.x) * 0.5f;
		std::vector<ImVec2> points{ a_from, ImVec2(middle - bend, a_from.y), ImVec2(middle + bend, a_to.y), a_to };
		if (!a_lit) {
			a_drawList->AddPolyline(points.data(), 4, a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		for (const float side : { -1.0f, 1.0f }) {
			const auto offset = OffsetPath(points, side * std::max(1.5f, a_radius * 0.14f));
			a_drawList->AddPolyline(offset.data(), 4, a_colour, ImDrawFlags_None, std::max(1.0f, a_radius * 0.08f));
		}
	}
}
