#include "UI/Themes/OrsimerDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>

namespace CompletionistUI::OrsimerDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Blackened iron, bone, and the red the Orcs paint for war
		constexpr ImU32 iron = IM_COL32(42, 42, 44, 255);
		constexpr ImU32 ironLight = IM_COL32(118, 118, 120, 255);
		constexpr ImU32 ironDark = IM_COL32(10, 10, 11, 255);
		constexpr ImU32 bone = IM_COL32(226, 214, 186, 255);
		constexpr ImU32 paint = IM_COL32(168, 34, 28, 255);
		constexpr ImU32 smoke = IM_COL32(120, 116, 112, 255);

		//---------------------------------------------------
		//-- Orsimer Decor ( Fade At ) ----------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_x, float a_from, float a_length) {

			return std::pow(std::sin(std::clamp((a_x - a_from) / a_length, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Fin ) --------------------------
		//---------------------------------------------------

		// A blade-shaped fin, as on Orcish armour: from a_base along a_angle, a bevel down its spine and a bone edge
		void Fin(ImDrawList* a_drawList, ImVec2 a_base, float a_length, float a_angle, float a_width) {

			const ImVec2 along(std::cos(a_angle), std::sin(a_angle));
			const ImVec2 side(-along.y, along.x);
			const ImVec2 tip(a_base.x + along.x * a_length, a_base.y + along.y * a_length);
			const ImVec2 left(a_base.x + side.x * a_width, a_base.y + side.y * a_width);
			const ImVec2 right(a_base.x - side.x * a_width * 0.6f, a_base.y - side.y * a_width * 0.6f);
			// The back edge kinks once, so it reads as forged rather than drawn
			const ImVec2 kink(a_base.x + along.x * a_length * 0.55f + side.x * a_width * 0.75f, a_base.y + along.y * a_length * 0.55f + side.y * a_width * 0.75f);
			const ImVec2 blade[4] = { left, kink, tip, right };
			a_drawList->AddConvexPolyFilled(blade, 4, iron);
			a_drawList->AddTriangleFilled(left, kink, ImVec2((a_base.x + tip.x) * 0.5f, (a_base.y + tip.y) * 0.5f), WithAlpha(ironLight, 0.35f));
			a_drawList->AddLine(right, tip, bone, std::max(1.0f, a_width * 0.18f));
			a_drawList->AddLine(a_base, tip, WithAlpha(ironDark, 0.8f), 1.0f);
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Soot ) -------------------------
		//---------------------------------------------------

		// Backdrop: the soot-black wall of a forge, smoke rising in streaks, cracks running straight through the stone
		void Soot(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(30, 30, 31, 70), IM_COL32(30, 30, 31, 70), IM_COL32(6, 6, 6, 70), IM_COL32(6, 6, 6, 70) });

			Random random{ 0x0A5Cu };
			// Smoke, wide and soft, thinning as it rises
			const int plumes = std::clamp(static_cast<int>(width / (size * 7.0f)), 3, 30);
			for (int i = 0; i < plumes; ++i) {
				const float  x = a_min.x + width * random.Next();
				const float  wide = size * random.Range(1.5f, 3.5f);
				const float  lean = size * random.Range(-2.0f, 2.0f);
				const ImVec2 quad[4] = { ImVec2(x - wide * 0.3f, a_max.y), ImVec2(x + wide * 0.3f, a_max.y), ImVec2(x + lean + wide, a_min.y), ImVec2(x + lean - wide, a_min.y) };
				GradientQuad(a_drawList, quad, { WithAlpha(smoke, 0.06f), WithAlpha(smoke, 0.06f), WithAlpha(smoke, 0.0f), WithAlpha(smoke, 0.0f) });
			}
			// Cracks: straight lengths, turning sharply
			const int cracks = std::clamp(static_cast<int>(width * height / (size * size * 50.0f)), 4, 40);
			for (int i = 0; i < cracks; ++i) {
				ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				float  angle = random.Range(0.0f, pi * 2.0f);
				for (int step = 0; step < 4; ++step) {
					const float  reach = size * random.Range(0.8f, 2.0f);
					const ImVec2 next(at.x + std::cos(angle) * reach, at.y + std::sin(angle) * reach);
					a_drawList->AddLine(at, next, WithAlpha(IM_COL32(0, 0, 0, 255), 0.5f), 1.0f);
					a_drawList->AddLine(ImVec2(at.x + 1.0f, at.y + 1.0f), ImVec2(next.x + 1.0f, next.y + 1.0f), WithAlpha(smoke, 0.06f), 1.0f);
					angle += random.Range(0.5f, 1.1f) * (random.Next() < 0.5f ? -1.0f : 1.0f);
					at = next;
				}
			}

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(2, 2, 2, 220));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Plate Band ) -------------------
		//---------------------------------------------------

		// One side of the frame: angular iron plates, each end cut on a slant, teeth along the inner edge
		void PlateBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, bool a_teethDown, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, ironDark);

			Random random{ a_seed };
			float  along = 0.0f;
			bool   lean = false;
			while (along < length) {
				const float  plate = depth * random.Range(2.6f, 4.0f);
				const float  slant = depth * 0.45f * (lean ? 1.0f : -1.0f);
				const ImVec2 points[4] = { at(along + 1.5f, 1.0f), at(along + plate - 1.5f, 1.0f), at(along + plate - 1.5f + slant, depth - 1.0f), at(along + 1.5f + slant, depth - 1.0f) };
				a_drawList->AddConvexPolyFilled(points, 4, Shade(iron, random.Range(0.85f, 1.15f)));
				a_drawList->AddLine(points[0], points[1], WithAlpha(ironLight, 0.6f), 1.0f);
				a_drawList->AddLine(points[1], points[2], WithAlpha(ironLight, 0.25f), 1.0f);
				along += plate;
				lean = !lean;
			}

			// Teeth along the inner edge, pointing in
			const float edge = a_teethDown ? depth : 0.0f;
			const float point = a_teethDown ? depth * 1.35f : -depth * 0.35f;
			const float tooth = depth * 0.6f;
			for (float x = tooth; x < length - tooth; x += tooth * 2.2f) {
				const ImVec2 left = at(x - tooth * 0.35f, edge);
				const ImVec2 right = at(x + tooth * 0.35f, edge);
				const ImVec2 tip = at(x, point);
				a_drawList->AddTriangleFilled(left, tip, right, iron);
				a_drawList->AddLine(left, tip, WithAlpha(ironLight, 0.5f), 1.0f);
			}
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( War Paint ) --------------------
		//---------------------------------------------------

		// Three slashes of red smeared across the iron, as a clan marks its gear
		void WarPaint(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			for (int slash = -1; slash <= 1; ++slash) {
				const float  x = a_centre.x + static_cast<float>(slash) * a_size * 0.45f;
				const ImVec2 from(x - a_size * 0.2f, a_centre.y - a_size * 0.45f);
				const ImVec2 to(x + a_size * 0.2f, a_centre.y + a_size * 0.45f);
				a_drawList->AddLine(from, to, WithAlpha(paint, 0.9f), std::max(1.5f, a_size * 0.2f));
				a_drawList->AddLine(ImVec2(to.x - a_size * 0.04f, to.y), ImVec2(to.x + a_size * 0.02f, to.y + a_size * 0.15f), WithAlpha(paint, 0.7f), std::max(1.0f, a_size * 0.1f));
			}
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Axe ) --------------------------
		//---------------------------------------------------

		// An Orcish war axe: a long haft, a broad blade swept back into spikes
		void Axe(ImDrawList* a_drawList, ImVec2 a_head, float a_length, ImU32 a_colour) {

			const ImVec2 foot(a_head.x - a_length * 0.35f, a_head.y + a_length);
			a_drawList->AddLine(ImVec2(a_head.x, a_head.y - a_length * 0.08f), foot, a_colour, std::max(2.0f, a_length * 0.04f));
			const float  s = a_length * 0.32f;
			const ImVec2 blade[] = {
				ImVec2(a_head.x, a_head.y - s * 0.35f), ImVec2(a_head.x + s * 0.9f, a_head.y - s * 0.8f), ImVec2(a_head.x + s * 0.75f, a_head.y - s * 0.2f),
				ImVec2(a_head.x + s * 1.0f, a_head.y + s * 0.15f), ImVec2(a_head.x + s * 0.7f, a_head.y + s * 0.5f), ImVec2(a_head.x + s * 0.85f, a_head.y + s * 0.95f),
				ImVec2(a_head.x, a_head.y + s * 0.35f),
			};
			for (const auto& point : blade) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(a_colour);
			// The back spike
			a_drawList->AddTriangleFilled(ImVec2(a_head.x, a_head.y - s * 0.2f), ImVec2(a_head.x - s * 0.6f, a_head.y - s * 0.05f), ImVec2(a_head.x, a_head.y + s * 0.2f), a_colour);
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Shard ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: an angular iron shard, longer below than above
	void Shard(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x + a_radius * 0.15f, a_centre.y - a_radius * 0.9f),
			ImVec2(a_centre.x + a_radius * 0.75f, a_centre.y - a_radius * 0.1f),
			ImVec2(a_centre.x - a_radius * 0.1f, a_centre.y + a_radius * 1.1f),
			ImVec2(a_centre.x - a_radius * 0.75f, a_centre.y - a_radius * 0.05f),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
			return;
		}
		a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		a_drawList->AddTriangleFilled(points[3], points[0], points[1], WithAlpha(IM_COL32(255, 255, 255, 255), 0.25f * (((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f)));
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Blades ) -----------------------
	//---------------------------------------------------

	// Four blades round a square boss, the upright ones longer, as the Orcs cut their marks
	void Blades(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		for (int blade = 0; blade < 4; ++blade) {
			const float  angle = static_cast<float>(blade) * pi * 0.5f - pi * 0.5f;
			const float  reach = a_radius * (blade % 2 == 0 ? 1.0f : 0.7f);
			const ImVec2 out(std::cos(angle), std::sin(angle));
			const ImVec2 side(-out.y, out.x);
			const ImVec2 tip(a_centre.x + out.x * reach, a_centre.y + out.y * reach);
			const ImVec2 left(a_centre.x + side.x * a_radius * 0.22f, a_centre.y + side.y * a_radius * 0.22f);
			const ImVec2 right(a_centre.x + out.x * a_radius * 0.3f - side.x * a_radius * 0.1f, a_centre.y + out.y * a_radius * 0.3f - side.y * a_radius * 0.1f);
			a_drawList->AddTriangleFilled(left, tip, right, a_colour);
		}
		const float  half = a_radius * 0.24f;
		const ImVec2 square[4] = { ImVec2(a_centre.x, a_centre.y - half), ImVec2(a_centre.x + half, a_centre.y), ImVec2(a_centre.x, a_centre.y + half), ImVec2(a_centre.x - half, a_centre.y) };
		a_drawList->AddConvexPolyFilled(square, 4, a_colour);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Anvil ) ------------------------
	//---------------------------------------------------

	// Needed / found: the anvil at the heart of every stronghold, sparks flying when it's found
	void Anvil(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  s = a_size;
		const auto   at = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * s, a_centre.y + a_y * s); };
		const ImVec2 outline[] = {
			at(-0.46f, -0.13f), at(-0.2f, -0.2f), at(0.38f, -0.2f), at(0.38f, -0.06f), at(0.14f, -0.01f), at(0.08f, 0.12f),
			at(0.24f, 0.24f), at(-0.22f, 0.24f), at(-0.06f, 0.12f), at(-0.12f, -0.01f), at(-0.3f, -0.05f),
		};
		const int count = static_cast<int>(std::size(outline));
		if (a_fill > 0.0f) {
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(a_colour, a_fill));
			// Sparks off the face
			for (const auto& [x, y] : { std::pair{ -0.05f, -0.36f }, std::pair{ 0.12f, -0.42f }, std::pair{ 0.26f, -0.33f } }) {
				a_drawList->AddCircleFilled(at(x, y), std::max(1.0f, s * 0.04f * a_fill), WithAlpha(a_colour, a_fill), 6);
			}
		}
		a_drawList->AddPolyline(outline, count, a_colour, ImDrawFlags_Closed, std::max(1.0f, s * 0.065f));
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Serration ) --------------------
	//---------------------------------------------------

	// In place of a rule: a blade's serrated edge, the teeth uneven, fading at the ends
	void Serration(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size) {

		const float length = a_to.x - a_from.x;
		if (length < a_size * 2.0f) {
			return;
		}
		const float tooth = a_size * 0.34f;
		const float base = a_from.y + a_size * 0.12f;
		for (float x = a_from.x; x < a_to.x; x += tooth * 0.5f) {
			const float end = std::min(a_to.x, x + tooth * 0.5f);
			a_drawList->AddLine(ImVec2(x, base), ImVec2(end, base), WithAlpha(a_colour, FadeAt((x + end) * 0.5f, a_from.x, length)), std::max(1.0f, a_size * 0.08f));
		}
		int i = 0;
		for (float x = a_from.x + tooth; x < a_to.x - tooth; x += tooth, ++i) {
			const float  fade = FadeAt(x, a_from.x, length);
			const float  tall = a_size * (i % 3 == 0 ? 0.36f : 0.22f);
			const ImVec2 left(x - tooth * 0.4f, base);
			const ImVec2 right(x + tooth * 0.15f, base);
			const ImVec2 tip(x + tooth * 0.25f, base - tall);
			a_drawList->AddTriangleFilled(left, tip, right, WithAlpha(a_colour, fade * 0.9f));
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Divider ) ----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Serration(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Serration(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Blades(a_drawList, ImVec2(midX, a_from.y), gap * 0.6f, a_colour);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Frame ) ------------------------
	//---------------------------------------------------

	// Jagged iron plates toothed along their inner edge and slashed with war paint, blade fins
	// thrusting out from the corners, and on the main window a helmet's crest of fins over the top
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Soot(a_drawList, a_min, a_max);

		// Behind the window, so only what thrusts out past its edges shows
		auto*       behind = ImGui::GetBackgroundDrawList();
		const float fin = band * (compact >= 0.8f ? 4.6f : 3.0f);
		const float corner = pi * 0.25f;
		Fin(behind, ImVec2(a_min.x + band, a_min.y + band), fin, pi + corner, band * 0.7f);
		Fin(behind, ImVec2(a_max.x - band, a_min.y + band), fin, -corner, band * 0.7f);
		Fin(behind, ImVec2(a_max.x - band, a_max.y - band), fin, corner, band * 0.7f);
		Fin(behind, ImVec2(a_min.x + band, a_max.y - band), fin, pi - corner, band * 0.7f);
		if (compact >= 0.8f) {
			const float midX = (a_min.x + a_max.x) * 0.5f;
			Fin(behind, ImVec2(midX, a_min.y + band), band * 6.0f, -pi * 0.5f, band * 0.8f);
			Fin(behind, ImVec2(midX - band * 1.2f, a_min.y + band), band * 4.6f, -pi * 0.5f - 0.38f, band * 0.65f);
			Fin(behind, ImVec2(midX + band * 1.2f, a_min.y + band), band * 4.6f, -pi * 0.5f + 0.38f, band * 0.65f);
		}

		PlateBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, true, 0x0C01u);
		PlateBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, false, 0x0C02u);
		PlateBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, true, 0x0C03u);
		PlateBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, false, 0x0C04u);

		WarPaint(a_drawList, ImVec2(a_min.x + width * 0.22f, a_min.y + band * 0.5f), band * 0.9f);
		WarPaint(a_drawList, ImVec2(a_max.x - width * 0.22f, a_min.y + band * 0.5f), band * 0.9f);
		WarPaint(a_drawList, ImVec2((a_min.x + a_max.x) * 0.5f, a_max.y - band * 0.5f), band * 0.9f);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Panel ) ------------------------
	//---------------------------------------------------

	// Inside a panel: hammer facets at the edges clearing towards the text, a war axe in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		// Facets: small flat triangles where the hammer struck, catching the light on one face
		Random    random{ static_cast<std::uint32_t>(width) * 53u + static_cast<std::uint32_t>(height) };
		const int marks = std::clamp(static_cast<int>(width * height / (size * size * 1.0f)), 30, 900);
		for (int i = 0; i < marks; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.04f) {
				continue;
			}
			const float  r = size * random.Range(0.25f, 0.6f);
			const float  angle = random.Range(0.0f, pi * 2.0f);
			const ImVec2 a(at.x + std::cos(angle) * r, at.y + std::sin(angle) * r);
			const ImVec2 b(at.x + std::cos(angle + 2.2f) * r, at.y + std::sin(angle + 2.2f) * r);
			const ImVec2 c(at.x + std::cos(angle + 4.1f) * r, at.y + std::sin(angle + 4.1f) * r);
			a_drawList->AddTriangleFilled(a, b, c, WithAlpha(random.Next() < 0.5f ? ironLight : IM_COL32(0, 0, 0, 255), weight * 0.12f));
		}

		// An Orcish war axe, large and faint, in the lower right
		if (a_emblem) {
			const float length = std::min(height * 0.85f, width * 0.28f);
			Axe(a_drawList, ImVec2(a_max.x - length * 0.55f, a_max.y - length * 0.92f), length, WithAlpha(bone, 0.07f));
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Panel Corners ) ----------------
	//---------------------------------------------------

	// A blade thrust in at each corner, its edge in bone
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			Fin(a_drawList, ImVec2(outer.x + sx, outer.y + sy), size * 1.3f, std::atan2(sy, sx), size * 0.28f);
		}
	}
}

namespace CompletionistUI::OrsimerDecor
{
	//---------------------------------------------------
	//-- Orsimer Decor ( Perk Node ) --------------------
	//---------------------------------------------------

	// Anvils: struck and sparking when taken, an empty anvil when next, a shard when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Anvil(a_drawList, a_centre, a_radius * 2.6f, a_colour, 1.0f);
			return;
		case PerkState::kNext:
			a_drawList->AddRectFilled(ImVec2(a_centre.x - a_radius * 0.9f, a_centre.y - a_radius * 0.5f), ImVec2(a_centre.x + a_radius * 0.9f, a_centre.y + a_radius * 0.6f), ironDark);
			Anvil(a_drawList, a_centre, a_radius * 2.6f, a_colour, 0.0f);
			return;
		default:
			Shard(a_drawList, a_centre, a_radius * 0.45f, ironDark, true);
			Shard(a_drawList, a_centre, a_radius * 0.45f, a_colour, false);
			return;
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Perk Link ) --------------------
	//---------------------------------------------------

	// Hard angles between the perks, struck in war paint where they're taken
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const float  run = std::min(std::abs(a_to.y - a_from.y), (a_to.x - a_from.x) * 0.6f);
		const float  middle = (a_from.x + a_to.x) * 0.5f;
		const ImVec2 points[4] = { a_from, ImVec2(middle - run * 0.5f, a_from.y), ImVec2(middle + run * 0.5f, a_to.y), a_to };
		if (!a_lit) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_None, 1.0f);
			return;
		}
		a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_None, std::max(2.0f, a_radius * 0.22f));
		a_drawList->AddPolyline(points, 4, WithAlpha(bone, 0.5f), ImDrawFlags_None, 1.0f);
	}
}
