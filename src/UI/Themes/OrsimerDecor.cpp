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

		// Blackened iron, rust, bone, hide and the forge
		constexpr ImU32 iron = IM_COL32(56, 54, 52, 255);
		constexpr ImU32 ironLight = IM_COL32(124, 120, 114, 255);
		constexpr ImU32 ironDark = IM_COL32(20, 19, 18, 255);
		constexpr ImU32 rust = IM_COL32(150, 80, 42, 255);
		constexpr ImU32 bone = IM_COL32(226, 212, 178, 255);
		constexpr ImU32 boneShade = IM_COL32(150, 134, 102, 255);
		constexpr ImU32 hide = IM_COL32(94, 66, 44, 255);
		constexpr ImU32 hideDark = IM_COL32(46, 32, 22, 255);
		constexpr ImU32 forge = IM_COL32(240, 140, 60, 255);

		float Alpha(ImU32 a_colour) {

			return ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Fade At ) ----------------------
		//---------------------------------------------------

		// 0 at a run's ends, 1 through its middle
		float FadeAt(float a_t) {

			return std::pow(std::sin(std::clamp(a_t, 0.0f, 1.0f) * pi), 0.6f);
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Tusk ) -------------------------
		//---------------------------------------------------

		// A tusk rising from a_base and curving out to its point; a_facing is -1 or 1
		std::vector<ImVec2> TuskOutline(ImVec2 a_base, float a_length, float a_facing) {

			const ImVec2 p0 = a_base;
			const ImVec2 p1(a_base.x, a_base.y - a_length * 0.6f);
			const ImVec2 p2(a_base.x + a_facing * a_length * 0.55f, a_base.y - a_length * 0.95f);
			const float  width = a_length * 0.17f;
			constexpr int steps = 12;
			std::vector<ImVec2> spine;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / steps;
				const float u = 1.0f - t;
				spine.emplace_back(u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x, u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y);
			}
			std::vector<ImVec2> outline;
			for (int i = 0; i <= steps; ++i) {
				const auto  normal = NormalAt(spine, static_cast<std::size_t>(i));
				const float half = width * (1.0f - static_cast<float>(i) / steps) * 0.5f;
				outline.emplace_back(spine[i].x + normal.x * half, spine[i].y + normal.y * half);
			}
			for (int i = steps - 1; i >= 0; --i) {
				const auto  normal = NormalAt(spine, static_cast<std::size_t>(i));
				const float half = width * (1.0f - static_cast<float>(i) / steps) * 0.5f;
				outline.emplace_back(spine[i].x - normal.x * half, spine[i].y - normal.y * half);
			}
			return outline;
		}

		// A bone tusk bound at its root with hide
		void Tusk(ImDrawList* a_drawList, ImVec2 a_base, float a_length, float a_facing, float a_alpha) {

			const auto outline = TuskOutline(a_base, a_length, a_facing);
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(bone, a_alpha));
			// Shade down the inner curve, rings of age near the root
			const std::size_t half = outline.size() / 2;
			std::vector<ImVec2> inner(outline.begin() + static_cast<std::ptrdiff_t>(half), outline.end());
			a_drawList->AddPolyline(inner.data(), static_cast<int>(inner.size()), WithAlpha(boneShade, a_alpha), ImDrawFlags_None, std::max(1.0f, a_length * 0.035f));
			const float width = a_length * 0.17f;
			for (int band = 0; band < 3; ++band) {
				const float y = a_base.y - width * (0.25f + band * 0.32f);
				a_drawList->AddLine(ImVec2(a_base.x - width * 0.55f, y), ImVec2(a_base.x + width * 0.55f, y + width * 0.12f), WithAlpha(hide, a_alpha), std::max(1.5f, width * 0.22f));
			}
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Hide ) -------------------------
		//---------------------------------------------------

		// Backdrop: hides stretched across the wall, mottled and stitched, the forge glowing below
		void Hides(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float size = ImGui::GetFontSize();
			a_drawList->PushClipRect(a_min, a_max, true);

			const ImVec2 wall[4] = { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) };
			GradientQuad(a_drawList, wall, { IM_COL32(40, 30, 22, 60), IM_COL32(40, 30, 22, 60), IM_COL32(14, 11, 9, 60), IM_COL32(14, 11, 9, 60) });

			// Mottling in the leather
			Random    random{ 0x0A5Cu };
			const int blotches = std::clamp(static_cast<int>(width * height / (size * size * 6.0f)), 20, 260);
			for (int i = 0; i < blotches; ++i) {
				const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
				const bool   light = random.Next() < 0.4f;
				a_drawList->AddCircleFilled(at, size * random.Range(0.8f, 3.0f), WithAlpha(light ? hide : IM_COL32(0, 0, 0, 255), light ? 0.03f : 0.06f), 16);
			}

			// Seams where the hides meet, wandering, laced across
			if (width > size * 20.0f) {
				for (const float across : { 0.34f, 0.71f }) {
					std::vector<ImVec2> seam;
					float               x = a_min.x + width * across;
					for (float y = a_min.y; y <= a_max.y + size; y += size * 1.5f) {
						seam.emplace_back(x, y);
						x += random.Range(-0.5f, 0.5f) * size;
					}
					a_drawList->AddPolyline(seam.data(), static_cast<int>(seam.size()), WithAlpha(IM_COL32(0, 0, 0, 255), 0.35f), ImDrawFlags_None, 2.0f);
					for (const auto& point : seam) {
						a_drawList->AddLine(ImVec2(point.x - size * 0.35f, point.y - size * 0.2f), ImVec2(point.x + size * 0.35f, point.y + size * 0.2f), WithAlpha(hide, 0.18f), 1.5f);
					}
				}
			}

			// The forge, out of sight below
			SoftSpot(a_drawList, ImVec2(a_min.x + width * 0.5f, a_max.y + size * 3.0f), std::max(width * 0.4f, size * 10.0f), WithAlpha(forge, 0.012f));

			EdgeShadow(a_drawList, a_min, a_max, std::min(width, height) * 0.15f, IM_COL32(4, 3, 2, 210));
			a_drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Iron Band ) --------------------
		//---------------------------------------------------

		// One side of the frame: hammered iron plates, dented, riveted where they meet, lashed with hide
		void IronBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, std::uint32_t a_seed) {

			const float length = a_horizontal ? a_max.x - a_min.x : a_max.y - a_min.y;
			const float depth = a_horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const auto  at = [&](float a_along, float a_across) {
				return a_horizontal ? ImVec2(a_min.x + a_along, a_min.y + a_across) : ImVec2(a_min.x + a_across, a_min.y + a_along);
			};
			a_drawList->AddRectFilled(a_min, a_max, ironDark);

			Random random{ a_seed };
			float  along = 0.0f;
			while (along < length) {
				const float plate = depth * random.Range(3.0f, 4.5f);
				const float end = std::min(length, along + plate);
				a_drawList->AddRectFilled(at(along + 1.0f, 1.0f), at(end - 1.0f, depth - 1.0f), Shade(iron, random.Range(0.8f, 1.1f)));
				a_drawList->AddLine(at(along + 1.0f, 1.5f), at(end - 1.0f, 1.5f), WithAlpha(ironLight, 0.5f), 1.0f);
				// Hammer marks: a dark dent, lit on its lower lip
				for (int dent = 0; dent < 3; ++dent) {
					const ImVec2 spot = at(along + (end - along) * random.Range(0.2f, 0.8f), depth * random.Range(0.3f, 0.7f));
					const float  radius = depth * random.Range(0.12f, 0.2f);
					a_drawList->AddCircleFilled(spot, radius, WithAlpha(ironDark, 0.55f), 10);
					a_drawList->PathArcTo(spot, radius, pi * 0.15f, pi * 0.85f, 6);
					a_drawList->PathStroke(WithAlpha(ironLight, 0.35f), ImDrawFlags_None, 1.0f);
				}
				// Rust bleeding from the seam now and then
				if (random.Next() < 0.4f) {
					a_drawList->AddRectFilledMultiColor(at(end - depth * 0.9f, 1.0f), at(end - 1.0f, depth - 1.0f), WithAlpha(rust, 0.0f), WithAlpha(rust, 0.35f), WithAlpha(rust, 0.35f), WithAlpha(rust, 0.0f));
				}
				// Rivets either side of the seam
				if (end < length) {
					for (const float across : { depth * 0.3f, depth * 0.7f }) {
						const ImVec2 rivet = at(end - depth * 0.22f, across);
						a_drawList->AddCircleFilled(rivet, depth * 0.12f, ironDark, 8);
						a_drawList->AddCircleFilled(ImVec2(rivet.x - depth * 0.025f, rivet.y - depth * 0.025f), depth * 0.08f, ironLight, 8);
					}
				}
				along = end;
			}

			// Hide lashed round it
			for (float x = depth * 7.0f; x + depth < length - depth * 5.0f; x += depth * 17.0f) {
				const float wide = depth * 1.1f;
				a_drawList->AddRectFilled(at(x, -1.0f), at(x + wide, depth + 1.0f), hide);
				for (float wrap = 0.0f; wrap < wide; wrap += depth * 0.3f) {
					a_drawList->AddLine(at(x + wrap, -1.0f), at(std::min(x + wide, x + wrap + depth * 0.25f), depth + 1.0f), hideDark, 1.0f);
				}
			}
		}

		//---------------------------------------------------
		//-- Orsimer Decor ( Corner Plate ) -----------------
		//---------------------------------------------------

		// A thick iron plate at each corner, a spiked boss in the middle
		void CornerPlate(ImDrawList* a_drawList, ImVec2 a_centre, float a_size) {

			const ImVec2 min(a_centre.x - a_size, a_centre.y - a_size);
			const ImVec2 max(a_centre.x + a_size, a_centre.y + a_size);
			a_drawList->AddRectFilled(min, max, iron);
			a_drawList->AddLine(ImVec2(min.x, min.y + 1.0f), ImVec2(max.x, min.y + 1.0f), WithAlpha(ironLight, 0.7f), 1.5f);
			a_drawList->AddRect(min, max, ironDark, 0.0f, 0, 1.5f);
			const float inset = a_size * 0.24f;
			for (const auto& rivet : { ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), ImVec2(min.x + inset, max.y - inset) }) {
				a_drawList->AddCircleFilled(rivet, a_size * 0.1f, ironLight, 8);
			}
			Spike(a_drawList, a_centre, a_size * 0.5f, ironLight);
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Rivet ) ------------------------
	//---------------------------------------------------

	// In place of the diamond: a round rivet head, lit on top
	void Rivet(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const float radius = a_radius * 0.85f;
		if (!a_filled) {
			a_drawList->AddCircle(a_centre, radius, a_colour, 12, std::max(1.0f, a_radius * 0.24f));
			return;
		}
		a_drawList->AddCircleFilled(a_centre, radius, a_colour, 12);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x - radius * 0.25f, a_centre.y - radius * 0.25f), radius * 0.38f, WithAlpha(IM_COL32(255, 244, 220, 255), 0.4f * Alpha(a_colour)), 8);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Spike ) ------------------------
	//---------------------------------------------------

	// A boss with four short spikes, as on Orcish shields
	void Spike(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		for (int spike = 0; spike < 4; ++spike) {
			const float  angle = static_cast<float>(spike) * pi * 0.5f - pi * 0.5f;
			const ImVec2 out(std::cos(angle), std::sin(angle));
			const ImVec2 side(-out.y, out.x);
			const ImVec2 tip(a_centre.x + out.x * a_radius, a_centre.y + out.y * a_radius);
			const ImVec2 left(a_centre.x + (out.x * 0.35f + side.x * 0.24f) * a_radius, a_centre.y + (out.y * 0.35f + side.y * 0.24f) * a_radius);
			const ImVec2 right(a_centre.x + (out.x * 0.35f - side.x * 0.24f) * a_radius, a_centre.y + (out.y * 0.35f - side.y * 0.24f) * a_radius);
			a_drawList->AddTriangleFilled(left, tip, right, a_colour);
		}
		a_drawList->AddCircleFilled(a_centre, a_radius * 0.45f, a_colour, 14);
		a_drawList->AddCircleFilled(ImVec2(a_centre.x - a_radius * 0.12f, a_centre.y - a_radius * 0.12f), a_radius * 0.16f, WithAlpha(IM_COL32(255, 244, 220, 255), 0.35f * Alpha(a_colour)), 8);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Marker ) -----------------------
	//---------------------------------------------------

	// Needed / found: a tusk, outlined until it's found, then whole bone
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float  length = a_size * 0.8f;
		const ImVec2 base(a_centre.x - length * 0.18f, a_centre.y + length * 0.46f);
		const auto   outline = TuskOutline(base, length, 1.0f);
		if (a_fill > 0.0f) {
			for (const auto& point : outline) {
				a_drawList->PathLineTo(point);
			}
			a_drawList->PathFillConcave(WithAlpha(a_colour, a_fill));
		}
		a_drawList->AddPolyline(outline.data(), static_cast<int>(outline.size()), a_colour, ImDrawFlags_Closed, std::max(1.0f, a_size * 0.07f));
		// Its root bound in hide
		a_drawList->AddLine(ImVec2(base.x - length * 0.12f, base.y - length * 0.06f), ImVec2(base.x + length * 0.12f, base.y - length * 0.04f), a_colour, std::max(1.0f, a_size * 0.1f));
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Chain ) ------------------------
	//---------------------------------------------------

	// In place of a rule: forged links, one face on, the next edge on, fading at the ends
	void Chain(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_size, bool a_fade) {

		const float dx = a_to.x - a_from.x;
		const float dy = a_to.y - a_from.y;
		const float length = std::hypot(dx, dy);
		if (length < a_size * 2.0f) {
			return;
		}
		const ImVec2 along(dx / length, dy / length);
		const float  angle = std::atan2(dy, dx);
		const float  link = a_size * 0.62f;
		const float  stroke = std::max(1.0f, a_size * 0.09f);
		const int    count = static_cast<int>(length / link);
		const float  start = (length - static_cast<float>(count) * link) * 0.5f;
		for (int i = 0; i < count; ++i) {
			const float  middle = start + (static_cast<float>(i) + 0.5f) * link;
			const ImVec2 centre(a_from.x + along.x * middle, a_from.y + along.y * middle);
			const ImU32  colour = a_fade ? WithAlpha(a_colour, FadeAt(middle / length)) : a_colour;
			if (i % 2 == 0) {
				a_drawList->AddEllipse(centre, ImVec2(link * 0.68f, a_size * 0.2f), colour, angle, 14, stroke);
			} else {
				const float reach = link * 0.62f;
				a_drawList->AddLine(ImVec2(centre.x - along.x * reach, centre.y - along.y * reach), ImVec2(centre.x + along.x * reach, centre.y + along.y * reach), colour, stroke * 1.6f);
			}
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Divider ) ----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.9f;
		Chain(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour, gap * 0.8f);
		Chain(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour, gap * 0.8f);
		Spike(a_drawList, ImVec2(midX, a_from.y), gap * 0.5f, a_colour);
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Frame ) ------------------------
	//---------------------------------------------------

	// Hammered iron lashed with hide, spiked plates on the corners, tusks rising from the top ones
	void Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float compact = std::clamp(std::min(width, height) / (size * 24.0f), 0.45f, 1.0f);
		const float band = size * 0.72f * std::max(0.6f, compact);

		Hides(a_drawList, a_min, a_max);

		IronBand(a_drawList, a_min, ImVec2(a_max.x, a_min.y + band), true, 0x0C01u);
		IronBand(a_drawList, ImVec2(a_min.x, a_max.y - band), a_max, true, 0x0C02u);
		IronBand(a_drawList, ImVec2(a_min.x, a_min.y + band), ImVec2(a_min.x + band, a_max.y - band), false, 0x0C03u);
		IronBand(a_drawList, ImVec2(a_max.x - band, a_min.y + band), ImVec2(a_max.x, a_max.y - band), false, 0x0C04u);
		a_drawList->AddRect(ImVec2(a_min.x + band + 1.0f, a_min.y + band + 1.0f), ImVec2(a_max.x - band - 1.0f, a_max.y - band - 1.0f), WithAlpha(rust, 0.3f), 0.0f, 0, 1.0f);

		const float corner = band * 1.05f;
		CornerPlate(a_drawList, ImVec2(a_min.x + band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerPlate(a_drawList, ImVec2(a_max.x - band * 0.5f, a_min.y + band * 0.5f), corner);
		CornerPlate(a_drawList, ImVec2(a_max.x - band * 0.5f, a_max.y - band * 0.5f), corner);
		CornerPlate(a_drawList, ImVec2(a_min.x + band * 0.5f, a_max.y - band * 0.5f), corner);

		// Only on the main window: tusks behind the top corners, rising above it, as over a stronghold's gate
		if (compact >= 0.8f) {
			auto*       behind = ImGui::GetBackgroundDrawList();
			const float tusk = band * 5.5f;
			Tusk(behind, ImVec2(a_min.x + band * 1.1f, a_min.y + band * 1.2f), tusk, -1.0f, 1.0f);
			Tusk(behind, ImVec2(a_max.x - band * 1.1f, a_min.y + band * 1.2f), tusk, 1.0f, 1.0f);
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Panel ) ------------------------
	//---------------------------------------------------

	// Inside a panel: pitted iron at the edges clearing towards the text, the forge's light from below, crossed tusks in the empty corner
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		const float band = std::min(width, height) * 0.3f + size * 2.0f;

		const float glow = std::min(height * 0.35f, size * 6.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - glow), a_max, WithAlpha(forge, 0.0f), WithAlpha(forge, 0.0f), WithAlpha(forge, 0.06f), WithAlpha(forge, 0.06f));

		// Pits and hammer marks
		Random    random{ static_cast<std::uint32_t>(width) * 53u + static_cast<std::uint32_t>(height) };
		const int marks = std::clamp(static_cast<int>(width * height / (size * size * 0.6f)), 40, 1400);
		for (int i = 0; i < marks; ++i) {
			const ImVec2 at(a_min.x + width * random.Next(), a_min.y + height * random.Next());
			const float  weight = EdgeWeight(at, a_min, a_max, band);
			if (weight < 0.02f) {
				continue;
			}
			const float radius = random.Range(0.8f, 2.4f);
			a_drawList->AddCircleFilled(at, radius, WithAlpha(IM_COL32(0, 0, 0, 255), weight * 0.35f), 6);
			a_drawList->AddLine(ImVec2(at.x - radius, at.y + radius), ImVec2(at.x + radius, at.y + radius), WithAlpha(ironLight, weight * 0.18f), 1.0f);
		}

		// Two tusks crossed, large and faint, in the lower right
		if (a_emblem) {
			const float  length = std::min(height * 0.8f, width * 0.3f);
			const ImVec2 centre(a_max.x - length * 0.6f, a_max.y - length * 0.08f);
			Tusk(a_drawList, ImVec2(centre.x - length * 0.22f, centre.y), length, 1.0f, 0.055f);
			Tusk(a_drawList, ImVec2(centre.x + length * 0.22f, centre.y), length, -1.0f, 0.055f);
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Panel Corners ) ----------------
	//---------------------------------------------------

	// Rough iron brackets on the panel's corners, a spike on the point
	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float arm = size * 1.1f;
		const float thick = std::max(3.0f, size * 0.24f);
		for (const auto& outer : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = outer.x == a_min.x ? 1.0f : -1.0f;
			const float sy = outer.y == a_min.y ? 1.0f : -1.0f;
			// Inside the panel's edge, so its clip doesn't halve them
			const ImVec2 corner(outer.x + sx * (thick * 0.5f + 1.0f), outer.y + sy * (thick * 0.5f + 1.0f));
			const ImVec2 points[3] = { ImVec2(corner.x + sx * arm, corner.y), corner, ImVec2(corner.x, corner.y + sy * arm) };
			a_drawList->AddPolyline(points, 3, ironDark, ImDrawFlags_None, thick + 2.0f);
			a_drawList->AddPolyline(points, 3, iron, ImDrawFlags_None, thick);
			a_drawList->AddLine(ImVec2(corner.x + sx * thick * 0.3f, corner.y - sy * thick * 0.3f), ImVec2(corner.x + sx * arm * 0.9f, corner.y - sy * thick * 0.3f), WithAlpha(ironLight, 0.6f), 1.0f);
			for (const auto& rivet : { ImVec2(corner.x + sx * arm * 0.7f, corner.y), ImVec2(corner.x, corner.y + sy * arm * 0.7f) }) {
				a_drawList->AddCircleFilled(rivet, thick * 0.3f, ironLight, 8);
			}
			// The spike, pointing in
			const ImVec2 spike[3] = { ImVec2(corner.x + sx * thick * 0.9f, corner.y), ImVec2(corner.x + sx * thick * 2.4f, corner.y + sy * thick * 2.4f), ImVec2(corner.x, corner.y + sy * thick * 0.9f) };
			a_drawList->AddTriangleFilled(spike[0], spike[1], spike[2], ironLight);
		}
	}
}

namespace CompletionistUI::OrsimerDecor
{
	//---------------------------------------------------
	//-- Orsimer Decor ( Perk Node ) --------------------
	//---------------------------------------------------

	// Spiked bosses when taken, a bare iron ring when next, a rivet when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			SoftSpot(a_drawList, a_centre, a_radius * 1.6f, WithAlpha(a_colour, 0.05f));
			Spike(a_drawList, a_centre, a_radius * 1.15f, a_colour);
			return;
		case PerkState::kNext:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.6f, ironDark, 16);
			a_drawList->AddCircle(a_centre, a_radius * 0.6f, a_colour, 16, std::max(1.5f, a_radius * 0.16f));
			return;
		default:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.32f, ironDark, 10);
			a_drawList->AddCircle(a_centre, a_radius * 0.32f, a_colour, 10, 1.0f);
			return;
		}
	}

	//---------------------------------------------------
	//-- Orsimer Decor ( Perk Link ) --------------------
	//---------------------------------------------------

	// Chain between the perks where they're taken, a thin iron rod where they're not
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const float  middle = (a_from.x + a_to.x) * 0.5f;
		const ImVec2 points[4] = { a_from, ImVec2(middle, a_from.y), ImVec2(middle, a_to.y), a_to };
		if (!a_lit) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_None, 1.5f);
			return;
		}
		const float size = std::max(6.0f, a_radius * 1.1f);
		for (int i = 0; i < 3; ++i) {
			const float run = std::hypot(points[i + 1].x - points[i].x, points[i + 1].y - points[i].y);
			if (run >= size * 2.0f) {
				Chain(a_drawList, points[i], points[i + 1], a_colour, size, false);
			} else if (run > 0.5f) {
				a_drawList->AddLine(points[i], points[i + 1], a_colour, std::max(1.0f, size * 0.12f));
			}
		}
	}
}
