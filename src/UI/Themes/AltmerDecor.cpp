#include "UI/Themes/AltmerDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numbers>
#include <utility>

// The one light theme: white Summerset marble, fine gold, the Dominion eagle
namespace CompletionistUI::AltmerDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Gold on white marble
		constexpr ImU32 gold = IM_COL32(206, 164, 66, 255);
		constexpr ImU32 goldLight = IM_COL32(250, 226, 150, 255);
		constexpr ImU32 goldDeep = IM_COL32(172, 128, 38, 255);
		constexpr ImU32 white = IM_COL32(255, 255, 255, 255);
		constexpr ImU32 vein = IM_COL32(168, 166, 176, 255);
		constexpr ImU32 cut = IM_COL32(120, 96, 50, 255);

		//---------------------------------------------------
		//-- Altmer Decor ( Alpha Of ) ----------------------
		//---------------------------------------------------

		float AlphaOf(ImU32 a_colour) {

			return static_cast<float>((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Gem Points ) --------------------
		//---------------------------------------------------

		// A cut gem, tall and pointed top and bottom; a_angle 0 stands it upright
		void GemPoints(ImVec2 (&a_points)[4], ImVec2 a_centre, float a_radius, float a_width, float a_angle) {

			const ImVec2 shape[4] = { { 0.0f, -1.0f }, { a_width, 0.0f }, { 0.0f, 1.0f }, { -a_width, 0.0f } };
			const float  c = std::cos(a_angle);
			const float  s = std::sin(a_angle);
			for (int i = 0; i < 4; ++i) {
				const float x = shape[i].x * a_radius;
				const float y = shape[i].y * a_radius;
				a_points[i] = ImVec2(a_centre.x + x * c - y * s, a_centre.y + x * s + y * c);
			}
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Faceted ) -----------------------
		//---------------------------------------------------

		// A faceted gem: lit on the upper left, shaded on the lower right, a smaller table cut in the middle
		void Faceted(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, float a_width, float a_angle, ImU32 a_colour) {

			ImVec2 points[4];
			GemPoints(points, a_centre, a_radius, a_width, a_angle);
			const float alpha = AlphaOf(a_colour);
			a_drawList->AddConvexPolyFilled(points, 4, a_colour);
			a_drawList->AddTriangleFilled(points[0], a_centre, points[3], WithAlpha(white, 0.5f * alpha));
			a_drawList->AddTriangleFilled(points[1], points[2], a_centre, WithAlpha(cut, 0.35f * alpha));
			if (a_radius > 4.0f) {
				ImVec2 table[4];
				GemPoints(table, a_centre, a_radius * 0.45f, a_width, a_angle);
				a_drawList->AddPolyline(table, 4, WithAlpha(goldLight, 0.8f * alpha), ImDrawFlags_Closed, 1.0f);
			}
			a_drawList->AddPolyline(points, 4, WithAlpha(goldDeep, alpha), ImDrawFlags_Closed, 1.0f);
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Curl ) --------------------------
		//---------------------------------------------------

		// A sweeping Elven stroke from a_from to a_to, bowing out by a_bow and hooking back at its tip
		void Curl(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImVec2 a_bow, ImU32 a_colour, float a_thickness) {

			const ImVec2 first(a_from.x + (a_to.x - a_from.x) * 0.3f + a_bow.x, a_from.y + (a_to.y - a_from.y) * 0.3f + a_bow.y);
			const ImVec2 second(a_from.x + (a_to.x - a_from.x) * 0.8f + a_bow.x * 0.6f, a_from.y + (a_to.y - a_from.y) * 0.8f + a_bow.y * 0.6f);
			a_drawList->AddBezierCubic(a_from, first, second, a_to, a_colour, a_thickness, 20);
			const ImVec2 hook(a_to.x - (a_to.x - second.x) * 0.5f - a_bow.x * 0.3f, a_to.y - (a_to.y - second.y) * 0.5f - a_bow.y * 0.3f);
			a_drawList->AddBezierQuadratic(a_to, ImVec2(a_to.x - a_bow.x * 0.35f, a_to.y - a_bow.y * 0.35f), hook, a_colour, std::max(1.0f, a_thickness * 0.7f), 10);
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Sparkle ) -----------------------
		//---------------------------------------------------

		void Sparkle(ImDrawList* a_drawList, ImVec2 a_centre, float a_length, ImU32 a_colour) {

			a_drawList->AddLine(ImVec2(a_centre.x - a_length, a_centre.y), ImVec2(a_centre.x + a_length, a_centre.y), a_colour, 1.0f);
			a_drawList->AddLine(ImVec2(a_centre.x, a_centre.y - a_length), ImVec2(a_centre.x, a_centre.y + a_length), a_colour, 1.0f);
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Crystal Tower ) -----------------
		//---------------------------------------------------

		// Crystal-Like-Law, drawn in faint white and gold behind the marble
		void CrystalTower(ImDrawList* a_drawList, ImVec2 a_base, float a_height, float a_width, float a_size) {

			const float  half = a_width * 0.5f;
			const float  top = a_base.y - a_height;
			const ImVec2 body[8] = {
				ImVec2(a_base.x - half, a_base.y),
				ImVec2(a_base.x - half * 0.7f, a_base.y - a_height * 0.3f),
				ImVec2(a_base.x - half * 0.82f, a_base.y - a_height * 0.32f),
				ImVec2(a_base.x - half * 0.42f, top + a_height * 0.08f),
				ImVec2(a_base.x + half * 0.42f, top + a_height * 0.08f),
				ImVec2(a_base.x + half * 0.82f, a_base.y - a_height * 0.32f),
				ImVec2(a_base.x + half * 0.7f, a_base.y - a_height * 0.3f),
				ImVec2(a_base.x + half, a_base.y),
			};
			for (const auto& point : body) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(white, 0.1f));
			a_drawList->AddPolyline(body, 8, WithAlpha(gold, 0.18f), ImDrawFlags_None, 1.0f);
			const ImVec2 crown(a_base.x, top);
			SoftSpot(a_drawList, crown, a_size * 4.0f, WithAlpha(white, 0.04f));
			Faceted(a_drawList, crown, a_size * 1.3f, 0.36f, 0.0f, WithAlpha(gold, 0.35f));
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Marble ) ------------------------
		//---------------------------------------------------

		// Backdrop: clouded white marble, faint veins, a rose window's tracery in gold, Crystal-Like-Law behind
		void Marble(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			const bool  full = height > size * 20.0f;
			a_drawList->PushClipRect(a_min, a_max, true);

			// Brighter above, a warmer cream below
			const ImVec2 wash[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wash, { IM_COL32(255, 255, 255, 70), IM_COL32(255, 255, 255, 70), IM_COL32(226, 214, 186, 50), IM_COL32(226, 214, 186, 50) });

			Random random{ 0xA17u };

			// Clouding in the stone
			for (int i = 0; i < 26; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const bool   light = random.Next() < 0.6f;
				SoftSpot(a_drawList, at, size * random.Range(3.0f, 9.0f), light ? IM_COL32(255, 255, 255, 6) : IM_COL32(190, 184, 172, 4));
			}

			// Tracery: rings, spokes and interlocking circles
			const ImVec2 hub((a_min.x + a_max.x) * 0.5f, a_min.y + height * 0.48f);
			const float  ring = std::min(width, height) * 0.34f;
			const ImU32  trace = WithAlpha(gold, 0.07f);
			a_drawList->AddCircle(hub, ring, trace, 96, 1.0f);
			a_drawList->AddCircle(hub, ring * 1.38f, trace, 96, 1.0f);
			a_drawList->AddCircle(hub, ring * 0.42f, trace, 64, 1.0f);
			for (int spoke = 0; spoke < 16; ++spoke) {
				const float  angle = static_cast<float>(spoke) * pi / 8.0f;
				const ImVec2 out(std::cos(angle), std::sin(angle));
				a_drawList->AddLine(ImVec2(hub.x + out.x * ring * 0.42f, hub.y + out.y * ring * 0.42f), ImVec2(hub.x + out.x * ring * 1.38f, hub.y + out.y * ring * 1.38f), trace, 1.0f);
				if (spoke % 2 == 0) {
					a_drawList->AddCircle(ImVec2(hub.x + out.x * ring, hub.y + out.y * ring), ring * 0.38f, trace, 48, 1.0f);
				}
			}

			if (full) {
				CrystalTower(a_drawList, ImVec2(a_min.x + width * 0.8f, a_max.y), height * 0.6f, width * 0.05f, size);
			}

			// Veins wandering through the marble, a few of them gold
			const int veins = std::clamp(static_cast<int>((width + height) / (size * 6.0f)), 6, 40);
			for (int v = 0; v < veins; ++v) {
				ImVec2      point(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				float       heading = random.Range(0.0f, pi * 2.0f);
				const ImU32 colour = random.Next() < 0.25f ? gold : vein;
				for (int s = 0; s < 18; ++s) {
					heading += random.Range(-0.3f, 0.3f);
					const ImVec2 next(point.x + std::cos(heading) * size * 0.7f, point.y + std::sin(heading) * size * 0.7f);
					a_drawList->AddLine(point, next, WithAlpha(colour, 0.13f * (1.0f - static_cast<float>(s) / 18.0f)), 1.0f);
					point = next;
				}
			}

			// Starlight caught in the stone
			const int glints = std::clamp(static_cast<int>(width * height / (size * size * 40.0f)), 8, 60);
			for (int i = 0; i < glints; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				Sparkle(a_drawList, at, size * random.Range(0.12f, 0.3f), WithAlpha(gold, random.Range(0.12f, 0.3f)));
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.12f, IM_COL32(214, 196, 150, 70));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Corner Flourish ) ---------------
		//---------------------------------------------------

		// Gold strokes sweeping out along both edges from a corner, a gem set where they meet
		void CornerFlourish(ImDrawList* a_drawList, ImVec2 a_corner, float a_sx, float a_sy, float a_size) {

			const float reach = a_size * 4.5f;
			for (const bool across : { true, false }) {
				const ImVec2 along = across ? ImVec2(a_sx, 0.0f) : ImVec2(0.0f, a_sy);
				const ImVec2 inward = across ? ImVec2(0.0f, a_sy) : ImVec2(a_sx, 0.0f);
				const ImVec2 from(a_corner.x + along.x * a_size * 0.5f + inward.x * a_size * 0.5f, a_corner.y + along.y * a_size * 0.5f + inward.y * a_size * 0.5f);
				const ImVec2 to(a_corner.x + along.x * reach + inward.x * a_size * 0.2f, a_corner.y + along.y * reach + inward.y * a_size * 0.2f);
				Curl(a_drawList, from, to, ImVec2(inward.x * a_size * 0.9f, inward.y * a_size * 0.9f), gold, 1.8f);
				const ImVec2 shortTo(a_corner.x + along.x * reach * 0.55f + inward.x * a_size * 0.9f, a_corner.y + along.y * reach * 0.55f + inward.y * a_size * 0.9f);
				Curl(a_drawList, from, shortTo, ImVec2(inward.x * a_size * 0.6f, inward.y * a_size * 0.6f), WithAlpha(gold, 0.75f), 1.2f);
			}
			const float angle = std::atan2(a_sy, a_sx) + pi * 0.5f;
			Faceted(a_drawList, ImVec2(a_corner.x + a_sx * a_size * 0.45f, a_corner.y + a_sy * a_size * 0.45f), a_size * 0.7f, 0.55f, angle, gold);
		}

		//---------------------------------------------------
		//-- Altmer Decor ( Crest ) -------------------------
		//---------------------------------------------------

		// Over the title: a gold sunburst round a gem, Elven strokes sweeping away either side
		void Crest(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			for (const float side : { -1.0f, 1.0f }) {
				Curl(a_drawList, ImVec2(a_centre.x + side * a_size * 0.6f, a_centre.y + a_size * 0.1f), ImVec2(a_centre.x + side * a_size * 9.0f, a_centre.y + a_size * 0.2f), ImVec2(0.0f, a_size * 1.1f), gold, 2.0f);
				Curl(a_drawList, ImVec2(a_centre.x + side * a_size * 0.6f, a_centre.y - a_size * 0.1f), ImVec2(a_centre.x + side * a_size * 5.5f, a_centre.y - a_size * 0.15f), ImVec2(0.0f, a_size * 0.45f), WithAlpha(gold, 0.8f), 1.2f);
			}
			for (int ray = 0; ray < 16; ++ray) {
				const float  angle = static_cast<float>(ray) * pi / 8.0f - pi * 0.5f;
				const float  reach = a_size * (ray % 2 == 0 ? 1.15f : 0.7f);
				const float  base = a_size * 0.12f;
				const ImVec2 out(std::cos(angle), std::sin(angle));
				const ImVec2 side(-out.y, out.x);
				a_drawList->AddTriangleFilled(ImVec2(a_centre.x + side.x * base, a_centre.y + side.y * base), ImVec2(a_centre.x + out.x * reach, a_centre.y + out.y * reach), ImVec2(a_centre.x - side.x * base, a_centre.y - side.y * base), ray % 2 == 0 ? gold : goldDeep);
			}
			a_drawList->AddCircleFilled(a_centre, a_size * 0.5f, goldLight, 24);
			a_drawList->AddCircle(a_centre, a_size * 0.5f, goldDeep, 24, 1.2f);
			Faceted(a_drawList, a_centre, a_size * 0.42f, 0.6f, 0.0f, gold);
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Crystal ) -----------------------
	//---------------------------------------------------

	// In place of the diamond: a cut gem
	void Crystal(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		ImVec2 points[4];
		GemPoints(points, a_centre, a_radius * 1.25f, 0.6f, 0.0f);
		if (a_filled) {
			a_drawList->AddConvexPolyFilled(points, 4, a_colour);
			if (a_radius > 3.0f) {
				a_drawList->AddTriangleFilled(points[0], a_centre, points[3], WithAlpha(white, 0.45f * AlphaOf(a_colour)));
			}
		} else {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.18f));
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Shard ) -------------------------
	//---------------------------------------------------

	// Marker: an empty gem's outline, the faceted gem filling it when found
	void Shard(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.42f;
		ImVec2      points[4];
		GemPoints(points, a_centre, radius, 0.62f, 0.0f);
		if (a_fill >= 1.0f) {
			SoftSpot(a_drawList, a_centre, radius * 1.4f, WithAlpha(goldLight, 0.06f));
		}
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.065f));
		if (a_fill > 0.0f) {
			Faceted(a_drawList, a_centre, radius * (0.35f + 0.55f * a_fill), 0.62f, 0.0f, a_colour);
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Eagle ) -------------------------
	//---------------------------------------------------

	// The Aldmeri Dominion's eagle, wings raised, head turned: one outline so faint fills stay even
	void Eagle(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		// Right half from the crown of the head down to the tail's notch
		static constexpr ImVec2 neck[] = { { 0.1f, -0.37f } };
		static constexpr ImVec2 beak[] = { { 0.14f, -0.53f }, { 0.34f, -0.46f }, { 0.13f, -0.43f } };
		static constexpr ImVec2 side[] = {
			{ 0.14f, -0.28f }, { 0.32f, -0.52f }, { 0.62f, -0.8f }, { 1.0f, -0.92f }, { 0.88f, -0.62f }, { 0.96f, -0.56f },
			{ 0.78f, -0.38f }, { 0.86f, -0.3f }, { 0.6f, -0.16f }, { 0.66f, -0.08f }, { 0.3f, 0.02f }, { 0.15f, 0.08f },
			{ 0.14f, 0.16f }, { 0.07f, 0.3f }, { 0.26f, 0.72f }, { 0.0f, 0.6f },
		};
		const auto point = [&](ImVec2 a_unit, float a_side = 1.0f) {
			return ImVec2(a_centre.x + a_unit.x * a_side * a_radius, a_centre.y + a_unit.y * a_radius);
		};
		constexpr int arc = 5;
		const auto    head = [&](int a_step, float a_side) {
			const float angle = -pi * 0.5f + pi * 0.5f * static_cast<float>(a_step) / arc;
			return point({ std::cos(angle) * 0.15f, -0.5f + std::sin(angle) * 0.15f }, a_side);
		};

		// Right side with the beak, then the left side mirrored back up
		for (int step = 0; step < arc; ++step) {
			a_drawList->PathLineTo(head(step, 1.0f));
		}
		for (const auto& unit : beak) {
			a_drawList->PathLineTo(point(unit));
		}
		a_drawList->PathLineTo(point(neck[0]));
		for (const auto& unit : side) {
			a_drawList->PathLineTo(point(unit));
		}
		for (int i = static_cast<int>(std::size(side)) - 2; i >= 0; --i) {
			a_drawList->PathLineTo(point(side[i], -1.0f));
		}
		a_drawList->PathLineTo(point(neck[0], -1.0f));
		for (int step = arc; step > 0; --step) {
			a_drawList->PathLineTo(head(step, -1.0f));
		}
		a_drawList->PathFillConcave(a_colour);

		// Feather lines and an eye once there is room
		if (a_radius > 10.0f) {
			const ImU32 line = WithAlpha(white, 0.6f * AlphaOf(a_colour));
			for (const float wing : { -1.0f, 1.0f }) {
				a_drawList->AddLine(point({ 0.2f, -0.12f }, wing), point({ 0.86f, -0.6f }, wing), line, 1.0f);
				a_drawList->AddLine(point({ 0.2f, -0.02f }, wing), point({ 0.72f, -0.34f }, wing), line, 1.0f);
			}
			a_drawList->AddCircleFilled(point({ 0.05f, -0.52f }), std::max(1.0f, a_radius * 0.03f), line, 6);
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Gem ) ---------------------------
	//---------------------------------------------------

	// Ornament beside headings: a gold gem with a stroke curling off each side
	void Gem(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		for (const float side : { -1.0f, 1.0f }) {
			Curl(a_drawList, ImVec2(a_centre.x + side * a_radius * 0.4f, a_centre.y + a_radius * 0.5f), ImVec2(a_centre.x + side * a_radius * 1.2f, a_centre.y - a_radius * 0.2f), ImVec2(side * a_radius * 0.3f, a_radius * 0.3f), WithAlpha(a_colour, 0.8f), 1.0f);
		}
		Faceted(a_drawList, a_centre, a_radius, 0.55f, 0.0f, a_colour);
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Filigree ) ----------------------
	//---------------------------------------------------

	// A fine gold line fading at its ends, a gem near its start and a curl lifting off it, in place of a rule
	void Filigree(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const auto fadeAt = [&](float a_x) {
			return std::pow(std::sin(std::clamp((a_x - a_from.x) / length, 0.0f, 1.0f) * pi), 0.5f);
		};
		const float step = a_size * 0.4f;
		for (float x = a_from.x; x < a_to.x; x += step) {
			const float next = std::min(a_to.x, x + step);
			a_drawList->AddLine(ImVec2(x, a_from.y), ImVec2(next, a_from.y), WithAlpha(a_colour, fadeAt((x + next) * 0.5f) * 0.8f), 1.0f);
		}
		if (length < a_size * 5.0f) {
			return;
		}
		const float  at = a_from.x + std::min(length * 0.12f, a_size * 2.5f);
		const ImVec2 gem(at, a_from.y);
		ImVec2       points[4];
		GemPoints(points, gem, a_size * 0.22f, 0.9f, pi * 0.5f);
		a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(a_colour, fadeAt(at)));
		Curl(a_drawList, ImVec2(at + a_size * 0.3f, a_from.y), ImVec2(at + a_size * 2.2f, a_from.y - a_size * 0.3f), ImVec2(0.0f, -a_size * 0.35f), WithAlpha(a_colour, 0.7f * fadeAt(at)), 1.0f);
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float  midX = (a_from.x + a_to.x) * 0.5f;
		const float  gap = ImGui::GetFontSize() * 0.9f;
		const ImVec2 centre(midX, a_from.y);
		for (const auto& [from, to] : { std::pair{ a_from.x, midX - gap }, std::pair{ midX + gap, a_to.x } }) {
			const float length = to - from;
			if (length <= 0.0f) {
				continue;
			}
			const float step = gap * 0.3f;
			for (float x = from; x < to; x += step) {
				const float next = std::min(to, x + step);
				const float t = ((x + next) * 0.5f - from) / length;
				const float fade = from < midX ? t : 1.0f - t;
				a_drawList->AddLine(ImVec2(x, a_from.y), ImVec2(next, a_from.y), WithAlpha(a_colour, 0.8f * std::sqrt(fade)), 1.0f);
			}
		}
		Gem(a_drawList, centre, gap * 0.42f, a_colour);
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Fine gold rules round white marble, Elven strokes sweeping from the corners, the crest over the title
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Marble(a_drawList, a_min, a_max);

		// Two gold rules, the outer one heavier
		const float  outer = band * 0.3f;
		const float  inner = band * 0.72f;
		const ImVec2 outerMin(a_min.x + outer, a_min.y + outer);
		const ImVec2 outerMax(a_max.x - outer, a_max.y - outer);
		a_drawList->AddRect(outerMin, outerMax, gold, 0.0f, 0, 2.5f);
		a_drawList->AddRect(ImVec2(outerMin.x + 1.0f, outerMin.y + 1.0f), ImVec2(outerMax.x - 1.0f, outerMax.y - 1.0f), WithAlpha(goldLight, 0.8f), 0.0f, 0, 1.0f);
		a_drawList->AddRect(ImVec2(a_min.x + inner, a_min.y + inner), ImVec2(a_max.x - inner, a_max.y - inner), WithAlpha(gold, 0.7f), 0.0f, 0, 1.0f);

		const float flourish = band * 1.1f;
		CornerFlourish(a_drawList, outerMin, 1.0f, 1.0f, flourish);
		CornerFlourish(a_drawList, ImVec2(outerMax.x, outerMin.y), -1.0f, 1.0f, flourish);
		CornerFlourish(a_drawList, outerMax, -1.0f, -1.0f, flourish);
		CornerFlourish(a_drawList, ImVec2(outerMin.x, outerMax.y), 1.0f, -1.0f, flourish);

		// Gems at the middle of the sides and the foot, the crest at the head
		const float midX = (a_min.x + a_max.x) * 0.5f;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		Faceted(a_drawList, ImVec2(outerMin.x, midY), band * 0.7f, 0.5f, 0.0f, gold);
		Faceted(a_drawList, ImVec2(outerMax.x, midY), band * 0.7f, 0.5f, 0.0f, gold);
		for (const float side : { -1.0f, 1.0f }) {
			Curl(a_drawList, ImVec2(midX + side * band * 0.4f, outerMax.y), ImVec2(midX + side * band * 4.0f, outerMax.y - band * 0.1f), ImVec2(0.0f, -band * 0.6f), gold, 1.5f);
		}
		Faceted(a_drawList, ImVec2(midX, outerMax.y), band * 0.75f, 0.55f, 0.0f, gold);
		if (width > size * 30.0f) {
			Crest(a_drawList, ImVec2(midX, outerMin.y + band * 0.5f), band * 0.75f);
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: whiter marble, faint veins, the Dominion eagle in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}

		// A paler slab of marble, brightest at the top
		a_drawList->AddRectFilledMultiColor(a_min, a_max, IM_COL32(255, 255, 255, 70), IM_COL32(255, 255, 255, 70), IM_COL32(255, 255, 255, 30), IM_COL32(255, 255, 255, 30));

		// Veins creeping in from the edges
		const float band = std::min(width, height) * 0.3f + size * 2.0f;
		Random      random{ static_cast<std::uint32_t>(width) * 41u + static_cast<std::uint32_t>(height) };
		const int   veins = std::clamp(static_cast<int>((width + height) / (size * 3.0f)), 6, 60);
		for (int v = 0; v < veins; ++v) {
			const int edge = static_cast<int>(random.Next() * 4.0f);
			ImVec2    point = edge == 0 ? ImVec2(a_min.x + width * random.Next(), a_min.y) : edge == 1 ? ImVec2(a_max.x, a_min.y + height * random.Next()) : edge == 2 ? ImVec2(a_min.x + width * random.Next(), a_max.y) : ImVec2(a_min.x, a_min.y + height * random.Next());
			float     heading = (edge == 0 ? 0.5f : edge == 1 ? 1.0f : edge == 2 ? 1.5f : 0.0f) * pi + random.Range(-0.7f, 0.7f);
			const ImU32 colour = random.Next() < 0.2f ? gold : vein;
			for (int s = 0; s < 12; ++s) {
				heading += random.Range(-0.35f, 0.35f);
				const ImVec2 next(point.x + std::cos(heading) * size * 0.5f, point.y + std::sin(heading) * size * 0.5f);
				const float  weight = EdgeWeight(next, a_min, a_max, band);
				if (weight < 0.05f) {
					break;
				}
				a_drawList->AddLine(point, next, WithAlpha(colour, 0.2f * weight), 1.0f);
				point = next;
			}
		}

		// The Dominion eagle, large and faint, ringed in gold, in the lower right
		if (a_emblem) {
			const float  radius = std::min(height * 0.42f, width * 0.16f);
			const ImVec2 centre(a_max.x - radius * 1.25f, a_max.y - radius * 0.95f);
			const ImU32  faint = WithAlpha(gold, 0.12f);
			a_drawList->AddCircle(centre, radius * 1.12f, faint, 64, std::max(1.0f, radius * 0.02f));
			a_drawList->AddCircle(centre, radius * 1.02f, WithAlpha(faint, 0.6f), 64, std::max(1.0f, radius * 0.012f));
			Eagle(a_drawList, ImVec2(centre.x, centre.y + radius * 0.08f), radius * 0.85f, faint);
		}
	}

	//---------------------------------------------------
	//-- Altmer Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	// Fine gold brackets on the panel's corners, a small gem in each, the arms curling at their ends
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.2f;
		const float inset = 2.0f;
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float  sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float  sy = outer.y == a_min.y ? 1.0f : -1.0f;
			const ImVec2 corner(outer.x + sx * inset, outer.y + sy * inset);
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, gold, ImDrawFlags_None, 2.0f);
			Curl(a_drawList, points[0], ImVec2(points[0].x + sx * size * 0.5f, points[0].y + sy * size * 0.2f), ImVec2(0.0f, sy * size * 0.2f), gold, 1.2f);
			Curl(a_drawList, points[2], ImVec2(points[2].x + sx * size * 0.2f, points[2].y + sy * size * 0.5f), ImVec2(sx * size * 0.2f, 0.0f), gold, 1.2f);
			Faceted(a_drawList, ImVec2(corner.x + sx * size * 0.35f, corner.y + sy * size * 0.35f), size * 0.3f, 0.6f, std::atan2(sy, sx) + pi * 0.5f, gold);
		}
	}
}
