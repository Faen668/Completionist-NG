#include "UI/Themes/DunmerDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace CompletionistUI::DunmerDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Chitin, basalt, ash and fire
		constexpr ImU32 chitin = IM_COL32(92, 58, 32, 255);
		constexpr ImU32 chitinLight = IM_COL32(170, 116, 62, 255);
		constexpr ImU32 chitinDark = IM_COL32(34, 20, 12, 255);
		constexpr ImU32 basalt = IM_COL32(34, 30, 30, 255);
		constexpr ImU32 basaltLight = IM_COL32(70, 62, 60, 255);
		constexpr ImU32 ash = IM_COL32(150, 140, 134, 255);
		constexpr ImU32 ember = IM_COL32(236, 96, 40, 255);
		constexpr ImU32 molten = IM_COL32(255, 186, 84, 255);
		constexpr ImU32 crust = IM_COL32(52, 26, 20, 255);

		//---------------------------------------------------
		//-- Dunmer Decor ( Flame Path ) --------------------
		//---------------------------------------------------

		// Teardrop flame: round base, tapering to a point above
		void FlamePath(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

			const float  round = a_radius * 0.55f;
			const ImVec2 base(a_centre.x, a_centre.y + a_radius * 0.35f);
			const ImVec2 tip(a_centre.x + a_radius * 0.08f, a_centre.y - a_radius);
			a_drawList->PathArcTo(base, round, 0.0f, pi, 12);
			a_drawList->PathBezierQuadraticCurveTo(ImVec2(base.x - round, base.y - a_radius * 0.75f), tip, 8);
			a_drawList->PathBezierQuadraticCurveTo(ImVec2(base.x + round * 1.05f, base.y - a_radius * 0.7f), ImVec2(base.x + round, base.y), 8);
		}

		//---------------------------------------------------
		//-- Dunmer Decor ( Ash Storm ) ---------------------
		//---------------------------------------------------

		// Backdrop: ash haze, ember glow from below, Red Mountain, ash falling, sparks rising
		void AshStorm(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const bool  full = height > size * 20.0f;
			a_drawList->PushClipRect(a_min, a_max, true);

			// Grey haze above, fire's glow below
			const ImVec2 sky[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, sky, { IM_COL32(64, 58, 56, 50), IM_COL32(64, 58, 56, 50), IM_COL32(110, 30, 14, 70), IM_COL32(110, 30, 14, 70) });

			Random random{ 0xA5Bu };

			// Red Mountain, smouldering
			if (full) {
				const float  peakX = a_min.x + width * 0.8f;
				const float  rise = height * 0.3f;
				const float  crater = width * 0.045f;
				const ImVec2 ridge[9] = {
					ImVec2(a_min.x + width * 0.5f, a_max.y),
					ImVec2(peakX - width * 0.2f, a_max.y - rise * 0.35f),
					ImVec2(peakX - width * 0.1f, a_max.y - rise * 0.75f),
					ImVec2(peakX - crater, a_max.y - rise),
					ImVec2(peakX - crater * 0.3f, a_max.y - rise * 0.93f),
					ImVec2(peakX + crater * 0.4f, a_max.y - rise * 0.95f),
					ImVec2(peakX + crater, a_max.y - rise * 0.98f),
					ImVec2(peakX + width * 0.12f, a_max.y - rise * 0.6f),
					ImVec2(a_max.x + width * 0.05f, a_max.y - rise * 0.2f),
				};
				// Smoke plume drifting west
				for (int i = 0; i < 14; ++i) {
					const float  t = static_cast<float>(i) / 13.0f;
					const ImVec2 puff(peakX - width * 0.18f * t * t + random.Range(-1.0f, 1.0f) * size, a_max.y - rise - height * 0.42f * t);
					SoftSpot(a_drawList, puff, size * (1.5f + 4.0f * t), IM_COL32(70, 62, 60, static_cast<int>(5.0f * (1.0f - t * 0.6f))));
				}
				SoftSpot(a_drawList, ImVec2(peakX, a_max.y - rise), size * 4.0f, WithAlpha(ember, 0.02f));
				for (const auto& point : ridge) {
					a_drawList->PathLineTo(point);
				}
				a_drawList->PathLineTo(ImVec2(a_max.x + width * 0.05f, a_max.y));
				a_drawList->PathFillConcave(IM_COL32(22, 14, 14, 235));
				// Lava on the crater rim and running down
				a_drawList->AddPolyline(ridge + 3, 4, WithAlpha(molten, 0.7f), ImDrawFlags_None, 2.0f);
				for (int i = 0; i < 4; ++i) {
					const float  side = i % 2 == 0 ? -1.0f : 1.0f;
					ImVec2       point(peakX + side * crater * random.Range(0.2f, 0.8f), a_max.y - rise * 0.95f);
					const float  run = rise * random.Range(0.25f, 0.55f);
					const float  lean = side * random.Range(0.25f, 0.5f);
					constexpr int segments = 8;
					for (int s = 0; s < segments; ++s) {
						const float  t = static_cast<float>(s) / segments;
						const ImVec2 next(point.x + lean * run / segments + random.Range(-1.5f, 1.5f), point.y + run / segments);
						a_drawList->AddLine(point, next, WithAlpha(ember, 0.55f * (1.0f - t)), 1.5f);
						point = next;
					}
				}
			}

			// Ash falling on the wind
			const int flakes = std::clamp(static_cast<int>(width * height / (size * size * 2.5f)), 30, 500);
			for (int i = 0; i < flakes; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const float  length = size * random.Range(0.08f, 0.22f);
				a_drawList->AddLine(at, ImVec2(at.x - length * 0.6f, at.y + length), WithAlpha(ash, random.Range(0.08f, 0.3f)), random.Range(1.0f, 1.8f));
			}

			// Sparks rising, thicker towards the bottom
			const int sparks = std::clamp(static_cast<int>(width * height / (size * size * 40.0f)), 6, 40);
			for (int i = 0; i < sparks; ++i) {
				const float  v = std::sqrt(random.Next());
				const ImVec2 at(a_min.x + width * random.Range(0.02f, 0.98f), a_min.y + height * (0.25f + v * 0.73f));
				const float  glow = random.Range(0.4f, 1.0f);
				SoftSpot(a_drawList, at, size * 0.5f * glow, WithAlpha(ember, 0.03f * glow));
				a_drawList->AddCircleFilled(at, std::max(1.0f, size * 0.07f * glow), WithAlpha(molten, 0.7f * glow), 8);
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(6, 3, 3, 200));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Dunmer Decor ( Chitin Band ) -------------------
		//---------------------------------------------------

		// One side of the frame: overlapping chitin plates, riveted
		void ChitinBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, chitinDark);
			const float plate = depth * 2.4f;
			int         index = 0;
			for (float along = 0.0f; along < length; along += plate * 0.85f, ++index) {
				const float end = std::min(length, along + plate);
				const float tone = index % 2 == 0 ? 1.0f : 0.85f;
				const auto  min = at(along, depth * 0.08f);
				const auto  max = at(end, depth * 0.92f);
				a_drawList->AddRectFilled(min, max, Shade(chitin, tone), depth * 0.35f);
				// Sheen along the plate
				a_drawList->AddLine(at(along + depth * 0.3f, depth * 0.3f), at(end - depth * 0.3f, depth * 0.3f), WithAlpha(chitinLight, 0.6f), std::max(1.0f, depth * 0.1f));
				a_drawList->AddRect(min, max, chitinDark, depth * 0.35f, 0, 1.0f);
				// Rivets at each end
				for (const float rivet : { along + depth * 0.35f, end - depth * 0.35f }) {
					if (rivet < length) {
						a_drawList->AddCircleFilled(at(rivet, depth * 0.62f), depth * 0.1f, IM_COL32(150, 132, 110, 255), 8);
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Dunmer Decor ( Shrine Corner ) -----------------
		//---------------------------------------------------

		// Basalt block with the Tribunal's triangle glowing in it
		void ShrineCorner(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, basalt);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), basaltLight);
			a_drawList->AddRect(min, max, IM_COL32(10, 8, 8, 255), 0.0f, 0, 1.5f);
			Triangle(a_drawList, ImVec2(a_centre.x, a_centre.y + a_size * 0.08f), a_size * 0.55f, ember);
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Flame ) -------------------------
	//---------------------------------------------------

	// In place of the diamond: a flame
	void Flame(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float radius = a_radius * 1.15f;
		FlamePath(a_drawList, a_centre, radius);
		if (a_filled) {
			a_drawList->PathFillConvex(a_colour);
			if (a_radius > 4.0f) {
				FlamePath(a_drawList, ImVec2(a_centre.x, a_centre.y + radius * 0.25f), radius * 0.5f);
				a_drawList->PathFillConvex(WithAlpha(molten, ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f));
			}
		} else {
			a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.2f));
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Ember ) -------------------------
	//---------------------------------------------------

	// Marker: a ring of cold ash, a flame kindling in it when found
	void Ember(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.36f;
		a_drawList->AddCircle(a_centre, radius, a_colour, 24, std::max(1.0f, a_size * 0.07f));
		if (a_fill > 0.0f) {
			if (a_fill >= 1.0f) {
				SoftSpot(a_drawList, a_centre, radius * 1.6f, WithAlpha(ember, 0.05f));
			}
			const float flame = radius * (0.3f + 0.45f * a_fill);
			FlamePath(a_drawList, ImVec2(a_centre.x, a_centre.y + radius * 0.08f), flame);
			a_drawList->PathFillConvex(a_colour);
			FlamePath(a_drawList, ImVec2(a_centre.x, a_centre.y + radius * 0.08f + flame * 0.25f), flame * 0.5f);
			a_drawList->PathFillConvex(WithAlpha(IM_COL32(255, 236, 170, 255), a_fill));
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Triangle ) ----------------------
	//---------------------------------------------------

	// The Tribunal's triangle, a spark at its heart
	void Triangle(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		const ImVec2 top(a_centre.x, a_centre.y - a_radius);
		const ImVec2 right(a_centre.x + a_radius * 0.87f, a_centre.y + a_radius * 0.5f);
		const ImVec2 left(a_centre.x - a_radius * 0.87f, a_centre.y + a_radius * 0.5f);
		SoftSpot(a_drawList, a_centre, a_radius * 1.8f, WithAlpha(a_colour, 0.03f));
		a_drawList->AddTriangle(top, right, left, a_colour, std::max(1.0f, a_radius * 0.2f));
		a_drawList->AddCircleFilled(a_centre, std::max(1.0f, a_radius * 0.22f), molten, 10);
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Lava ) --------------------------
	//---------------------------------------------------

	// Cracked crust with fire showing through, in place of a rule
	void Lava(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		Random      random{ static_cast<std::uint32_t>(length) * 2654435761u + 7u };
		const float step = a_size * 0.4f;
		ImVec2      point = a_from;
		for (float x = a_from.x + step; x <= a_to.x + 0.5f; x += step) {
			const float  t = (x - a_from.x) / length;
			const float  fade = std::pow(std::sin(std::clamp(t, 0.0f, 1.0f) * pi), 0.7f);
			const ImVec2 next(std::min(x, a_to.x), a_from.y + random.Range(-1.0f, 1.0f) * a_size * 0.1f * fade);
			a_drawList->AddLine(point, next, WithAlpha(crust, fade), std::max(2.0f, a_size * 0.2f));
			a_drawList->AddLine(point, next, WithAlpha(a_colour, fade * random.Range(0.5f, 1.0f)), 1.0f);
			if (random.Next() < 0.12f && fade > 0.4f) {
				SoftSpot(a_drawList, next, a_size * 0.35f, WithAlpha(ember, 0.05f * fade));
				a_drawList->AddCircleFilled(next, 1.2f, WithAlpha(molten, fade), 6);
			}
			point = next;
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Lava(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Lava(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Triangle(a_drawList, ImVec2(midX, a_from.y), gap * 0.45f, a_colour);
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Chitin frame over the ash, Tribunal shrines at the corners
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		AshStorm(a_drawList, a_min, a_max);

		ChitinBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true);
		ChitinBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true);
		ChitinBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false);
		ChitinBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(ember, 0.3f), 0.0f, 0, 1.0f);

		const float corner = band * 1.05f;
		ShrineCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		ShrineCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		ShrineCorner(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		ShrineCorner(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: basalt at the edges clearing towards the text, embers below, the Tribunal in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;
		const auto  edge = [&](ImVec2 a_point) {
			const float distance = std::min({ a_point.x - a_min.x, a_max.x - a_point.x, a_point.y - a_min.y, a_max.y - a_point.y });
			const float t = 1.0f - std::clamp(distance / band, 0.0f, 1.0f);
			return t * t;
		};

		// Ember glow along the bottom
		const float glow = std::min(height * 0.3f, size * 5.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(ember, 0.0f), WithAlpha(ember, 0.0f), WithAlpha(ember, 0.08f), WithAlpha(ember, 0.08f));

		// Basalt grain
		Random    random{ static_cast<std::uint32_t>(width) * 31u + static_cast<std::uint32_t>(height) };
		const int grains = std::clamp(static_cast<int>(width * height / (size * size * 0.35f)), 60, 2400);
		for (int i = 0; i < grains; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = edge(at);
			if (weight < 0.02f) {
				continue;
			}
			const bool  light = random.Next() < 0.55f;
			const ImU32 colour = light ? IM_COL32(130, 116, 110, 255) : IM_COL32(0, 0, 0, 255);
			a_drawList->AddCircleFilled(at, random.Range(0.6f, 1.6f), WithAlpha(colour, weight * (light ? 0.24f : 0.4f)), 5);
		}

		// Cracks climbing from the bottom edge, fire in them
		const int cracks = std::clamp(static_cast<int>(width / (size * 14.0f)), 1, 5);
		for (int c = 0; c < cracks; ++c) {
			ImVec2      point(a_min.x + width * (static_cast<float>(c) + random.Range(0.2f, 0.8f)) / static_cast<float>(cracks), a_max.y);
			float       heading = -pi * 0.5f + random.Range(-0.5f, 0.5f);
			const float reach = std::min(height * 0.45f, size * random.Range(3.0f, 6.0f));
			constexpr int segments = 10;
			for (int i = 0; i < segments; ++i) {
				const float  t = static_cast<float>(i) / segments;
				heading += random.Range(-0.45f, 0.45f);
				const ImVec2 next(point.x + std::cos(heading) * reach / segments, point.y + std::sin(heading) * reach / segments);
				const float  fade = 1.0f - t;
				a_drawList->AddLine(point, next, WithAlpha(IM_COL32(0, 0, 0, 255), 0.5f * fade), 2.5f * fade + 1.0f);
				a_drawList->AddLine(point, next, WithAlpha(ember, 0.65f * fade * fade), 1.2f);
				if (random.Next() < 0.2f && t < 0.6f) {
					const float  side = heading + (random.Next() < 0.5f ? 1.0f : -1.0f) * random.Range(0.6f, 1.1f);
					const ImVec2 twig(next.x + std::cos(side) * reach * 0.12f, next.y + std::sin(side) * reach * 0.12f);
					a_drawList->AddLine(next, twig, WithAlpha(IM_COL32(0, 0, 0, 255), 0.4f * fade), 1.2f);
				}
				point = next;
			}
		}

		// The Tribunal, large and faint, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const ImU32  faint = WithAlpha(IM_COL32(234, 146, 100, 255), 0.09f);
			const ImVec2 top(centre.x, centre.y - radius);
			const ImVec2 right(centre.x + radius * 0.87f, centre.y + radius * 0.5f);
			const ImVec2 left(centre.x - radius * 0.87f, centre.y + radius * 0.5f);
			SoftSpot(a_drawList, centre, radius * 1.2f, WithAlpha(ember, 0.008f));
			a_drawList->AddCircle(centre, radius * 1.12f, WithAlpha(faint, 0.6f), 64, std::max(1.0f, radius * 0.02f));
			a_drawList->AddTriangle(top, right, left, faint, std::max(1.5f, radius * 0.05f));
			a_drawList->AddTriangle(ImVec2(centre.x, centre.y - radius * 0.62f), ImVec2(centre.x + radius * 0.54f, centre.y + radius * 0.31f), ImVec2(centre.x - radius * 0.54f, centre.y + radius * 0.31f), WithAlpha(faint, 0.6f), std::max(1.0f, radius * 0.02f));
			// Red Mountain at its heart
			const ImVec2 peak[5] = {
				ImVec2(centre.x - radius * 0.42f, centre.y + radius * 0.31f),
				ImVec2(centre.x - radius * 0.1f, centre.y - radius * 0.12f),
				ImVec2(centre.x + radius * 0.02f, centre.y - radius * 0.06f),
				ImVec2(centre.x + radius * 0.12f, centre.y - radius * 0.14f),
				ImVec2(centre.x + radius * 0.42f, centre.y + radius * 0.31f),
			};
			a_drawList->AddConvexPolyFilled(peak, 5, WithAlpha(faint, 0.8f));
			for (const auto& point : { top, right, left }) {
				a_drawList->AddCircleFilled(point, std::max(2.0f, radius * 0.05f), faint, 12);
			}
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Chitin brackets on the panel's corners, riveted
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
			a_drawList->AddPolyline(points, 3, chitinDark, ImDrawFlags_None, thick + 2.0f);
			a_drawList->AddPolyline(points, 3, chitin, ImDrawFlags_None, thick);
			a_drawList->AddLine(ImVec2(corner.x + sx * thick * 0.3f, corner.y + sy * thick * 0.3f), ImVec2(corner.x + sx * arm * 0.9f, corner.y + sy * thick * 0.3f), WithAlpha(chitinLight, 0.7f), 1.0f);
			a_drawList->AddCircleFilled(ImVec2(corner.x + sx * thick * 0.1f, corner.y + sy * thick * 0.1f), thick * 0.45f, IM_COL32(150, 132, 110, 255), 8);
		}
	}
}

namespace CompletionistUI::DunmerDecor
{
	//---------------------------------------------------
	//-- Dunmer Decor ( Perk Node ) ------------------
	//---------------------------------------------------

	// Embers from Red Mountain: burning when taken, banked when next, ash when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		if (a_state == PerkState::kLocked) {
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.45f, IM_COL32(26, 17, 16, 255), 16);
			a_drawList->AddCircle(a_centre, a_radius * 0.45f, a_colour, 16, 1.0f);
			return;
		}
		Ember(a_drawList, a_centre, a_radius * 2.6f, a_colour, a_state == PerkState::kTaken ? 1.0f : 0.0f);
		if (a_state == PerkState::kNext) {
			a_drawList->AddCircleFilled(a_centre, std::max(1.5f, a_radius * 0.2f), a_colour, 10);
		}
	}

	//---------------------------------------------------
	//-- Dunmer Decor ( Perk Link ) ------------------
	//---------------------------------------------------

	// Cracks in the ash, glowing with magma between taken perks
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto curve = PerkCurve(a_from, a_to, a_radius * 0.9f);
		Random     random{ static_cast<std::uint32_t>(a_from.x * 13.0f + a_to.y * 7.0f) | 1u };
		std::vector<ImVec2> crack;
		crack.reserve(curve.size());
		for (std::size_t i = 0; i < curve.size(); ++i) {
			const bool  end = i == 0 || i + 1 == curve.size();
			const float jitter = end ? 0.0f : random.Range(-1.0f, 1.0f) * a_radius * 0.3f;
			const auto  normal = NormalAt(curve, i);
			crack.emplace_back(curve[i].x + normal.x * jitter, curve[i].y + normal.y * jitter);
		}
		const int count = static_cast<int>(crack.size());
		if (!a_lit) {
			a_drawList->AddPolyline(crack.data(), count, a_colour, ImDrawFlags_None, std::max(1.0f, a_radius * 0.12f));
			return;
		}
		a_drawList->AddPolyline(crack.data(), count, WithAlpha(a_colour, 0.25f), ImDrawFlags_None, std::max(3.0f, a_radius * 0.6f));
		a_drawList->AddPolyline(crack.data(), count, a_colour, ImDrawFlags_None, std::max(1.5f, a_radius * 0.18f));
	}
}
