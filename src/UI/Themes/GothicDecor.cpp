#include "UI/Themes/GothicDecor.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <ranges>
#include <utility>
#include <vector>

namespace CompletionistUI::GothicDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Night slate, tarnished silver, guild violet, the gilt on the books
		constexpr ImU32 slate = IM_COL32(36, 36, 44, 255);
		constexpr ImU32 slateLight = IM_COL32(74, 72, 84, 255);
		constexpr ImU32 shadow = IM_COL32(6, 6, 9, 255);
		constexpr ImU32 silver = IM_COL32(176, 174, 182, 255);
		constexpr ImU32 silverLight = IM_COL32(236, 234, 238, 255);
		constexpr ImU32 silverDark = IM_COL32(88, 86, 94, 255);
		constexpr ImU32 violet = IM_COL32(74, 44, 118, 255);
		constexpr ImU32 violetLight = IM_COL32(112, 76, 164, 255);
		constexpr ImU32 violetDark = IM_COL32(46, 26, 78, 255);
		constexpr ImU32 sealSilver = IM_COL32(184, 178, 196, 255);
		constexpr ImU32 gilt = IM_COL32(206, 166, 96, 255);
		constexpr ImU32 giltLight = IM_COL32(246, 218, 150, 255);
		constexpr ImU32 giltDeep = IM_COL32(120, 88, 42, 255);

		bool openFrame = false;

		//---------------------------------------------------
		//-- Gothic Decor ( Bar ) ---------------------------
		//---------------------------------------------------

		// A length of tarnished silver moulding, its shine wandering along it, shaded soft so nothing in it has a hard edge
		void Bar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, std::uint32_t a_seed) {

			const bool  horizontal = a_max.x - a_min.x >= a_max.y - a_min.y;
			const float depth = horizontal ? a_max.y - a_min.y : a_max.x - a_min.x;
			const float offset = static_cast<float>(a_seed % 500u);
			const float scale = ImGui::GetFontSize();
			const float along = scale * 0.5f;
			const float across = std::max(1.0f, depth * 0.34f);

			Mesh(a_drawList, a_min, a_max, horizontal ? along : across, horizontal ? across : along, [&](float a_x, float a_y) {
				const float length = horizontal ? a_x : a_y;
				const float t = horizontal ? (a_y - a_min.y) / depth : (a_x - a_min.x) / depth;
				const float ridge = 1.0f - std::abs(t - 0.42f) * 2.0f;
				const float shine = Fbm(length / (scale * 2.2f) + offset, 0.5f, 3);
				const ImU32 metal = Mix(silverDark, silverLight, Smooth((shine - 0.3f) * 2.4f));
				return Mix(shadow, metal, Smooth(ridge * 1.9f));
			});
		}

		// A ruled pair: the heavy line and a fine one inside it
		void RuledEdge(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_horizontal, float a_inward, std::uint32_t a_seed) {

			const float  size = ImGui::GetFontSize();
			const float  heavy = std::max(2.5f, size * 0.17f);
			const float  fine = std::max(1.0f, size * 0.06f);
			const float  gap = size * 0.2f;
			const ImU32  thin = WithAlpha(silver, 0.55f);
			if (a_horizontal) {
				Bar(a_drawList, ImVec2(a_min.x, a_min.y - heavy * 0.5f), ImVec2(a_max.x, a_min.y + heavy * 0.5f), a_seed);
				const float y = a_min.y + a_inward * gap;
				a_drawList->AddRectFilled(ImVec2(a_min.x + size * 0.3f, y), ImVec2(a_max.x - size * 0.3f, y + fine), thin);
			} else {
				Bar(a_drawList, ImVec2(a_min.x - heavy * 0.5f, a_min.y), ImVec2(a_min.x + heavy * 0.5f, a_max.y), a_seed);
				const float x = a_min.x + a_inward * gap;
				a_drawList->AddRectFilled(ImVec2(x, a_min.y + size * 0.3f), ImVec2(x + fine, a_max.y - size * 0.3f), thin);
			}
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Lancet ) ------------------------
		//---------------------------------------------------

		// One long point of a corner's tracery: from its broad base at a_from, narrowing to a_to
		void Lancet(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, float a_width) {

			const float dx = a_to.x - a_from.x;
			const float dy = a_to.y - a_from.y;
			const float length = std::max(0.001f, std::hypot(dx, dy));
			const ImVec2 side(-dy / length * a_width, dx / length * a_width);
			const ImVec2 outerA(a_from.x + side.x * 1.35f, a_from.y + side.y * 1.35f);
			const ImVec2 outerB(a_from.x - side.x * 1.35f, a_from.y - side.y * 1.35f);
			const ImVec2 outerTip(a_to.x + dx / length * a_width, a_to.y + dy / length * a_width);
			a_drawList->AddTriangleFilled(outerA, outerTip, outerB, WithAlpha(shadow, 0.85f));
			const ImVec2 a(a_from.x + side.x, a_from.y + side.y);
			const ImVec2 b(a_from.x - side.x, a_from.y - side.y);
			a_drawList->AddTriangleFilled(a, a_to, a_from, silverLight);
			a_drawList->AddTriangleFilled(a_from, a_to, b, silverDark);
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Star ) --------------------------
		//---------------------------------------------------

		// A four pointed star of silver facets
		void Star(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, float a_turn) {

			for (int point = 0; point < 4; ++point) {
				const float angle = a_turn + static_cast<float>(point) * pi * 0.5f;
				Lancet(a_drawList, a_centre, ImVec2(a_centre.x + std::cos(angle) * a_radius, a_centre.y + std::sin(angle) * a_radius), a_radius * 0.2f);
			}
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Tracery Corner ) ----------------
		//---------------------------------------------------

		// A gothic corner: two tapering points running out along the mouldings, a web between them, a star turned on it
		void TraceryCorner(ImDrawList* a_drawList, ImVec2 a_corner, float a_arm, float a_sx, float a_sy) {

			const auto at = [&](float a_x, float a_y) { return ImVec2(a_corner.x + a_sx * a_x * a_arm, a_corner.y + a_sy * a_y * a_arm); };

			const ImVec2 web[] = { at(0.0f, 0.0f), at(0.72f, 0.0f), at(0.5f, 0.14f), at(0.14f, 0.5f), at(0.0f, 0.72f) };
			a_drawList->AddConvexPolyFilled(web, 5, WithAlpha(shadow, 0.9f));

			const float width = std::max(1.2f, a_arm * 0.085f);
			Lancet(a_drawList, at(0.0f, 0.03f), at(1.0f, 0.02f), width);
			Lancet(a_drawList, at(0.03f, 0.0f), at(0.02f, 1.0f), width);
			Star(a_drawList, at(0.27f, 0.27f), a_arm * 0.27f, pi * 0.25f);
			// The bright L where the mouldings meet
			const ImVec2 elbow[3] = { at(0.3f, 0.0f), at(0.0f, 0.0f), at(0.0f, 0.3f) };
			a_drawList->AddPolyline(elbow, 3, silver, ImDrawFlags_None, std::max(1.2f, a_arm * 0.04f));
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Damask ) ------------------------
		//---------------------------------------------------

		// Backdrop: slate with a faint damask in it, as the library's old wall hangings
		void Damask(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

			const float size = ImGui::GetFontSize();
			const float width = a_max.x - a_min.x;
			const float height = a_max.y - a_min.y;
			const float cell = std::max(size * 0.8f, std::sqrt(width * height / 16000.0f));
			Mesh(a_drawList, a_min, a_max, cell, cell, [&](float a_x, float a_y) {
				const float cloud = Fbm(a_x / (size * 9.0f), a_y / (size * 7.0f), 4);
				const float fine = Fbm(a_x / (size * 1.3f) + 31.0f, a_y / (size * 1.3f), 2);
				const float tone = (cloud - 0.5f) * 0.9f + (fine - 0.5f) * 0.45f;
				return tone > 0.0f ? WithAlpha(slateLight, tone * 0.45f) : WithAlpha(shadow, -tone * 0.7f);
			});

			// The damask's repeat: a lozenge lattice with a curl of leaf in each
			const float step = size * 4.4f;
			const ImU32 line = WithAlpha(slateLight, 0.11f);
			int         row = 0;
			for (float y = a_min.y; y < a_max.y + step; y += step * 0.5f, ++row) {
				for (float x = a_min.x + (row % 2 == 0 ? 0.0f : step * 0.5f); x < a_max.x + step; x += step) {
					const auto at = [&](float a_x, float a_y) { return ImVec2(x + a_x * step, y + a_y * step); };
					const ImVec2 lozenge[] = { at(0.0f, -0.25f), at(0.25f, 0.0f), at(0.0f, 0.25f), at(-0.25f, 0.0f) };
					a_drawList->AddPolyline(lozenge, 4, line, ImDrawFlags_Closed, 1.0f);
					for (const float side : { -1.0f, 1.0f }) {
						const ImVec2 curl[] = { at(0.0f, 0.12f), at(side * 0.08f, 0.0f), at(side * 0.03f, -0.1f), at(side * -0.01f, -0.04f) };
						const auto   stroke = Spline(curl, 4, false, 3);
						a_drawList->AddPolyline(stroke.data(), static_cast<int>(stroke.size()), line, ImDrawFlags_None, 1.0f);
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Square Frame ) ------------------
		//---------------------------------------------------

		void SquareFrame(ImDrawList* a_list, ImVec2 a_min, ImVec2 a_max, std::uint32_t a_seed) {

			const float size = ImGui::GetFontSize();
			RuledEdge(a_list, a_min, ImVec2(a_max.x, a_min.y), true, 1.0f, a_seed + 1u);
			RuledEdge(a_list, ImVec2(a_min.x, a_max.y), a_max, true, -1.0f, a_seed + 2u);
			RuledEdge(a_list, a_min, ImVec2(a_min.x, a_max.y), false, 1.0f, a_seed + 3u);
			RuledEdge(a_list, ImVec2(a_max.x, a_min.y), a_max, false, -1.0f, a_seed + 4u);

			const float room = std::min(a_max.x - a_min.x, a_max.y - a_min.y);
			const float arm = std::min(size * 2.5f, room * 0.3f);
			TraceryCorner(a_list, a_min, arm, 1.0f, 1.0f);
			TraceryCorner(a_list, ImVec2(a_max.x, a_min.y), arm, -1.0f, 1.0f);
			TraceryCorner(a_list, a_max, arm, -1.0f, -1.0f);
			TraceryCorner(a_list, ImVec2(a_min.x, a_max.y), arm, 1.0f, -1.0f);
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Guild Eye ) ---------------------
		//---------------------------------------------------

		// The University's seal, tall on its banner: long horns rising from an arch over an open eye, a bar through the eye
		// out to the cloth's edges, a crescent cradling it, a long shaft below. a_halfWidth and a_halfHeight are its box
		void GuildEye(ImDrawList* a_drawList, ImVec2 a_centre, float a_halfWidth, float a_halfHeight, ImU32 a_colour) {

			const auto  at = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * a_halfWidth, a_centre.y + a_y * a_halfHeight); };
			const float pen = std::max(1.5f, a_halfWidth * 0.15f);
			const auto  stroke = [&](std::initializer_list<ImVec2> a_points, bool a_closed, float a_pen) {
				std::vector<ImVec2> points(a_points);
				const auto          line = Spline(points.data(), static_cast<int>(points.size()), a_closed, 5);
				a_drawList->AddPolyline(line.data(), static_cast<int>(line.size()), a_colour, a_closed ? ImDrawFlags_Closed : ImDrawFlags_None, a_pen);
			};

			// Horns, thick at their foot, with the arch between them
			for (const float side : { -1.0f, 1.0f }) {
				const ImVec2 horn[4] = { at(side * 0.72f, -1.0f), at(side * 0.92f, -0.93f), at(side * 0.5f, -0.3f), at(side * 0.3f, -0.34f) };
				a_drawList->AddQuadFilled(horn[0], horn[1], horn[2], horn[3], a_colour);
			}
			stroke({ at(-0.5f, -0.54f), at(-0.24f, -0.6f), at(0.0f, -0.62f), at(0.24f, -0.6f), at(0.5f, -0.54f) }, false, pen);

			// The eye and its iris
			stroke({ at(-0.66f, -0.14f), at(-0.34f, -0.3f), at(0.0f, -0.36f), at(0.34f, -0.3f), at(0.66f, -0.14f), at(0.34f, -0.03f), at(0.0f, 0.0f), at(-0.34f, -0.03f) }, true, pen);
			const ImVec2 irisCentre = at(0.0f, -0.17f);
			a_drawList->AddCircle(irisCentre, a_halfWidth * 0.2f, a_colour, 20, pen);

			// The bar through it, a double line running out to the cloth's edges
			for (const float side : { -1.0f, 1.0f }) {
				a_drawList->AddLine(at(side * 0.64f, -0.19f), at(side * 1.0f, -0.19f), a_colour, pen * 0.9f);
				a_drawList->AddLine(at(side * 0.64f, -0.09f), at(side * 1.0f, -0.09f), a_colour, pen * 0.9f);
			}

			// The crescent under the eye
			stroke({ at(-0.72f, -0.06f), at(-0.68f, 0.14f), at(-0.46f, 0.32f), at(-0.1f, 0.42f) }, false, pen);
			stroke({ at(0.72f, -0.06f), at(0.68f, 0.14f), at(0.46f, 0.32f), at(0.1f, 0.42f) }, false, pen);

			// The shaft, from under the eye down to the swallowtail's notch
			const ImVec2 shaft[] = { at(-0.07f, 0.02f), at(0.07f, 0.02f), at(0.2f, 0.96f), at(-0.2f, 0.96f) };
			a_drawList->AddConvexPolyFilled(shaft, 4, a_colour);
			const ImVec2 slit[] = { at(0.0f, 0.28f), at(0.055f, 0.56f), at(0.0f, 0.82f), at(-0.055f, 0.56f) };
			a_drawList->AddConvexPolyFilled(slit, 4, violetDark);
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Swallowtail ) -------------------
		//---------------------------------------------------

		// A violet banner hung from the top of the window, its foot cut to a swallowtail, a silver edge and the seal
		void Swallowtail(ImDrawList* a_drawList, float a_midX, float a_top, float a_width, float a_height, std::uint32_t a_seed) {

			const float half = a_width * 0.5f;
			const float bottom = a_top + a_height;
			const float notch = bottom - a_width * 0.42f;

			const auto clothAt = [&](float a_x, float a_y) {
				const float mottle = Fbm(a_x / (a_width * 0.24f) + static_cast<float>(a_seed), a_y / (a_width * 0.5f), 3);
				const float edge = std::abs(a_x - a_midX) / half;
				const float tone = (mottle - 0.5f) * 1.4f - edge * edge * edge * 0.3f;
				return tone >= 0.0f ? Mix(violet, violetLight, tone * 1.5f) : Mix(violet, violetDark, -tone * 1.7f);
			};
			constexpr int strips = 14;
			constexpr int rows = 12;
			const float   step = a_width / strips;
			const auto    footAt = [&](float a_x) { return notch + (bottom - notch) * (std::abs(a_x - a_midX) / half); };
			for (int strip = 0; strip < strips; ++strip) {
				const float left = a_midX - half + step * static_cast<float>(strip);
				const float right = left + step;
				for (int row = 0; row < rows; ++row) {
					const float t0 = static_cast<float>(row) / rows;
					const float t1 = static_cast<float>(row + 1) / rows;
					const ImVec2 points[4] = { ImVec2(left, a_top + (footAt(left) - a_top) * t0), ImVec2(right, a_top + (footAt(right) - a_top) * t0), ImVec2(right, a_top + (footAt(right) - a_top) * t1), ImVec2(left, a_top + (footAt(left) - a_top) * t1) };
					GradientQuad(a_drawList, points, { clothAt(points[0].x, points[0].y), clothAt(points[1].x, points[1].y), clothAt(points[2].x, points[2].y), clothAt(points[3].x, points[3].y) });
				}
			}

			// Silver edges, heavier along the swallowtail
			const float  pen = std::max(1.2f, a_width * 0.036f);
			const ImVec2 left(a_midX - half, a_top);
			const ImVec2 right(a_midX + half, a_top);
			const ImVec2 cut[3] = { ImVec2(a_midX - half, bottom), ImVec2(a_midX, notch), ImVec2(a_midX + half, bottom) };
			a_drawList->AddLine(left, cut[0], WithAlpha(shadow, 0.8f), pen + 2.0f);
			a_drawList->AddLine(right, cut[2], WithAlpha(shadow, 0.8f), pen + 2.0f);
			a_drawList->AddPolyline(cut, 3, WithAlpha(shadow, 0.9f), ImDrawFlags_None, pen * 2.0f);
			a_drawList->AddLine(left, cut[0], sealSilver, pen);
			a_drawList->AddLine(right, cut[2], sealSilver, pen);
			a_drawList->AddPolyline(cut, 3, sealSilver, ImDrawFlags_None, pen * 1.3f);
			const ImVec2 cutInner[3] = { ImVec2(a_midX - half + a_width * 0.11f, bottom - a_width * 0.2f), ImVec2(a_midX, notch - a_width * 0.1f), ImVec2(a_midX + half - a_width * 0.11f, bottom - a_width * 0.2f) };
			a_drawList->AddPolyline(cutInner, 3, WithAlpha(sealSilver, 0.5f), ImDrawFlags_None, 1.0f);

			// A fine line in from the edge
			const float inset = a_width * 0.11f;
			const ImU32 thread = WithAlpha(sealSilver, 0.55f);
			a_drawList->AddLine(ImVec2(left.x + inset, a_top), ImVec2(left.x + inset, bottom - a_width * 0.2f), thread, 1.0f);
			a_drawList->AddLine(ImVec2(right.x - inset, a_top), ImVec2(right.x - inset, bottom - a_width * 0.2f), thread, 1.0f);

			// As tall as the cloth allows: from just under the top to the point of the notch
			const float sealTop = a_top + a_height * 0.1f;
			const float sealBottom = notch - a_width * 0.11f;
			GuildEye(a_drawList, ImVec2(a_midX, (sealTop + sealBottom) * 0.5f), half * 0.86f, (sealBottom - sealTop) * 0.5f, sealSilver);
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Chamfer ) -----------------------
		//---------------------------------------------------

		std::vector<ImVec2> ChamferPath(ImVec2 a_min, ImVec2 a_max, float a_cut) {

			return {
				ImVec2(a_min.x + a_cut, a_min.y), ImVec2(a_max.x - a_cut, a_min.y), ImVec2(a_max.x, a_min.y + a_cut), ImVec2(a_max.x, a_max.y - a_cut),
				ImVec2(a_max.x - a_cut, a_max.y), ImVec2(a_min.x + a_cut, a_max.y), ImVec2(a_min.x, a_max.y - a_cut), ImVec2(a_min.x, a_min.y + a_cut),
			};
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Shelf ) -------------------------
		//---------------------------------------------------

		// Old dark leather, taken from the reference, in the order the books sit
		constexpr ImU32 oxblood = IM_COL32(72, 38, 33, 255);
		constexpr ImU32 blue = IM_COL32(50, 60, 74, 255);
		constexpr ImU32 olive = IM_COL32(64, 66, 40, 255);
		constexpr ImU32 teal = IM_COL32(36, 66, 68, 255);
		constexpr ImU32 tan = IM_COL32(74, 52, 32, 255);
		constexpr ImU32 plum = IM_COL32(64, 38, 54, 255);
		constexpr ImU32 shelf[] = { oxblood, blue, olive, teal, tan, oxblood, blue, plum, olive, tan, teal, tan, blue, olive, plum, tan };

		//---------------------------------------------------
		//-- Gothic Decor ( Soft Grid ) ---------------------
		//---------------------------------------------------

		// A strip shaded vertex by vertex, its top and bottom edges drawn in by a_inset(x), which rounds or points its ends
		template <class Inset, class Shader>
		void SoftGrid(ImDrawList* a_list, const std::vector<float>& a_columns, float a_height, int a_rows, Inset&& a_inset, Shader&& a_shader) {

			const int  columns = static_cast<int>(a_columns.size());
			const auto uv = ImGui::GetFontTexUvWhitePixel();
			a_list->PrimReserve((columns - 1) * a_rows * 6, columns * (a_rows + 1));
			const auto base = a_list->_VtxCurrentIdx;
			for (const float x : a_columns) {
				const float inset = a_inset(x);
				for (int row = 0; row <= a_rows; ++row) {
					const float v = static_cast<float>(row) / static_cast<float>(a_rows);
					a_list->PrimWriteVtx(ImVec2(x, inset + (a_height - inset * 2.0f) * v), uv, a_shader(x, v));
				}
			}
			const int stride = a_rows + 1;
			for (int column = 0; column + 1 < columns; ++column) {
				for (int row = 0; row < a_rows; ++row) {
					const auto corner = static_cast<ImDrawIdx>(base + static_cast<unsigned int>(column * stride + row));
					const auto next = static_cast<ImDrawIdx>(corner + stride);
					a_list->PrimWriteIdx(corner);
					a_list->PrimWriteIdx(next);
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_list->PrimWriteIdx(corner);
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(corner + 1));
				}
			}
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Round Of Spine ) ----------------
		//---------------------------------------------------

		// How much light a rounded spine catches from top (0) to bottom (1)
		float RoundOfSpine(float a_v) {

			struct Stop
			{
				float at;
				float light;
			};
			static constexpr Stop stops[] = { { 0.0f, 0.5f }, { 0.05f, 1.22f }, { 0.14f, 1.1f }, { 0.34f, 0.94f }, { 0.62f, 0.74f }, { 0.84f, 0.54f }, { 0.95f, 0.38f }, { 1.0f, 0.2f } };
			for (std::size_t i = 1; i < std::size(stops); ++i) {
				if (a_v <= stops[i].at) {
					const float t = (a_v - stops[i - 1].at) / (stops[i].at - stops[i - 1].at);
					return stops[i - 1].light + (stops[i].light - stops[i - 1].light) * t;
				}
			}
			return stops[std::size(stops) - 1].light;
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Paint Book ) --------------------
		//---------------------------------------------------

		void PaintBook(ImDrawList* a_list, float a_width, float a_height, ImU32 a_leather, float a_seed) {

			const float round = a_height * 0.2f;
			const auto  inset = [&](float a_x) {
				const float d = std::min(a_x, a_width - a_x);
				if (d >= round) {
					return 0.0f;
				}
				const float off = round - d;
				return round - std::sqrt(std::max(0.0f, round * round - off * off));
			};

			std::vector<float> columns;
			for (int i = 0; i <= 5; ++i) {
				columns.push_back(round * static_cast<float>(i) / 5.0f);
			}
			const float step = std::max(a_height * 0.5f, (a_width - round * 2.0f) / 28.0f);
			for (float x = round + step; x < a_width - round - step * 0.5f; x += step) {
				columns.push_back(x);
			}
			for (int i = 0; i <= 5; ++i) {
				columns.push_back(a_width - round + round * static_cast<float>(i) / 5.0f);
			}

			// The shadow it throws on the shelf below
			a_list->AddRectFilled(ImVec2(1.0f, 2.0f), ImVec2(a_width - 1.0f, a_height + 2.5f), WithAlpha(shadow, 0.85f), round);

			SoftGrid(a_list, columns, a_height, 7, inset, [&](float a_x, float a_v) {
				const float end = std::min(a_x, a_width - a_x) / (a_height * 0.5f);
				const float roll = 0.74f + 0.26f * Smooth(end * 2.0f);
				const float mottle = Fbm(a_x / (a_height * 1.4f) + a_seed * 7.0f, a_v * 1.7f + a_seed, 3);
				// The lit rim is worn through in places
				const float rim = Smooth((a_v - 0.02f) / 0.05f) * (1.0f - Smooth((a_v - 0.14f) / 0.1f));
				const float worn = Fbm(a_x / (a_height * 0.8f) + 40.0f + a_seed, 2.0f, 2);
				const float light = RoundOfSpine(a_v) - rim * (1.0f - Smooth((worn - 0.28f) * 3.5f)) * 0.42f;
				return Shade(a_leather, light * roll * (0.9f + mottle * 0.2f));
			});

			// Pebbled grain
			Random random{ static_cast<std::uint32_t>(a_seed) * 2147u + 91u };
			for (int i = 0; i < 80; ++i) {
				const float v = random.Range(0.08f, 0.92f);
				const float x = a_width * random.Next();
				const float mark = a_height * random.Range(0.025f, 0.07f);
				a_list->AddRectFilled(ImVec2(x, a_height * v), ImVec2(x + mark, a_height * v + 1.0f), WithAlpha(random.Next() < 0.65f ? shadow : Shade(a_leather, 1.6f), random.Range(0.08f, 0.22f)));
			}

			// Tooling round the head and tail in pale old gilt: close rules, a fine hatched band between them
			constexpr ImU32 tool = IM_COL32(224, 196, 142, 255);
			constexpr ImU32 toolDim = IM_COL32(134, 104, 60, 255);
			const auto rule = [&](float a_x, float a_alpha, float a_thickness) {
				const float in = inset(a_x) + a_height * 0.035f;
				a_list->AddRectFilled(ImVec2(a_x + 1.0f, in), ImVec2(a_x + a_thickness + 1.0f, a_height - in), WithAlpha(shadow, 0.5f * a_alpha));
				a_list->AddRectFilledMultiColor(ImVec2(a_x, in), ImVec2(a_x + a_thickness, a_height - in), WithAlpha(tool, a_alpha), WithAlpha(tool, a_alpha), WithAlpha(toolDim, a_alpha), WithAlpha(toolDim, a_alpha));
			};
			const auto hatch = [&](float a_from, float a_to) {
				const float centre = (a_from + a_to) * 0.5f;
				const float half = (a_to - a_from) * 0.3f;
				for (float y = a_height * 0.1f; y < a_height * 0.92f; y += std::max(2.0f, a_height * 0.06f)) {
					a_list->AddLine(ImVec2(centre - half, y), ImVec2(centre + half, y), WithAlpha(tool, 0.42f), 1.0f);
				}
			};
			const float heavy = std::max(1.2f, a_height * 0.036f);
			const float fine = 1.0f;
			hatch(a_height * 0.19f, a_height * 0.33f);
			rule(a_height * 0.07f, 0.95f, heavy);
			rule(a_height * 0.145f, 0.7f, fine);
			rule(a_height * 0.34f, 0.95f, heavy);
			rule(a_height * 0.415f, 0.55f, fine);
			hatch(a_width - a_height * 0.22f, a_width - a_height * 0.1f);
			rule(a_width - a_height * 0.25f, 0.95f, heavy);
			rule(a_width - a_height * 0.075f, 0.8f, fine);
		}

		//---------------------------------------------------
		//-- Gothic Decor ( Paint Case ) --------------------
		//---------------------------------------------------

		// The chosen book: violet in a hexagonal inner rule, a pale silver case round it, dark end caps with scrollwork
		void PaintCase(ImDrawList* a_list, float a_width, float a_height) {

			constexpr ImU32 pale = IM_COL32(222, 198, 206, 255);
			constexpr ImU32 cap = IM_COL32(34, 18, 54, 255);

			const float point = a_height * 0.3f;
			const float midY = a_height * 0.5f;
			const float left = -point * 0.75f;
			const float right = a_width + point * 0.75f;
			const auto  inset = [&](float a_x) {
				const float d = std::min(a_x - left, right - a_x);
				return d < point ? midY * (1.0f - d / point) : 0.0f;
			};

			std::vector<float> columns{ left };
			for (int i = 1; i <= 4; ++i) {
				columns.push_back(left + point * static_cast<float>(i) / 4.0f);
			}
			const float step = std::max(a_height * 0.5f, a_width / 28.0f);
			for (float x = left + point + step; x < right - point - step * 0.5f; x += step) {
				columns.push_back(x);
			}
			for (int i = 0; i <= 4; ++i) {
				columns.push_back(right - point + point * static_cast<float>(i) / 4.0f);
			}

			const float capEnd = point * 0.95f;
			SoftGrid(a_list, columns, a_height, 7, inset, [&](float a_x, float a_v) {
				const float mottle = Fbm(a_x / (a_height * 1.5f) + 5.0f, a_v * 2.0f, 3);
				const float light = RoundOfSpine(a_v * 0.9f + 0.05f);
				const float body = Smooth((std::min(a_x, a_width - a_x) - capEnd + 2.0f) / 3.0f);
				const ImU32 violetTone = Shade(IM_COL32(62, 36, 96, 255), light * (0.8f + mottle * 0.42f));
				return Mix(cap, violetTone, body);
			});

			// The inner hexagon, its ends pointing outward over the caps
			const float pad = a_height * 0.12f;
			const ImVec2 inner[] = {
				ImVec2(capEnd - point * 0.5f, midY), ImVec2(capEnd, pad), ImVec2(a_width - capEnd, pad),
				ImVec2(a_width - capEnd + point * 0.5f, midY), ImVec2(a_width - capEnd, a_height - pad), ImVec2(capEnd, a_height - pad),
			};
			a_list->AddPolyline(inner, 6, WithAlpha(pale, 0.72f), ImDrawFlags_Closed, 1.0f);

			// Scrollwork in the caps, short strokes running to the point
			Random random{ 0xC801u };
			for (const float side : { -1.0f, 1.0f }) {
				const float tip = side < 0.0f ? left : right;
				const float root = side < 0.0f ? capEnd : a_width - capEnd;
				for (const float along : { 0.35f, 0.6f, 0.82f }) {
					const float x = tip + (root - tip) * along;
					const float reach = inset(x) + a_height * 0.09f;
					a_list->AddLine(ImVec2(x, reach), ImVec2(x + side * a_height * 0.09f, midY - a_height * 0.06f), WithAlpha(pale, 0.75f), 1.0f);
					a_list->AddLine(ImVec2(x, a_height - reach), ImVec2(x + side * a_height * 0.09f, midY + a_height * 0.06f), WithAlpha(pale, 0.75f), 1.0f);
				}
				a_list->AddCircleFilled(ImVec2(tip - side * point * 0.34f, midY), std::max(1.0f, a_height * 0.035f), pale, 6);
			}

			// Worn dashes tooled along the inner rule
			for (float x = capEnd + a_height * 0.4f; x < a_width - capEnd - a_height * 0.4f; x += a_height * random.Range(1.2f, 3.0f)) {
				const float length = a_height * random.Range(0.15f, 0.6f);
				a_list->AddLine(ImVec2(x, a_height - pad - 2.0f), ImVec2(std::min(x + length, a_width - capEnd), a_height - pad - 2.0f), WithAlpha(pale, 0.3f), 1.0f);
				a_list->AddLine(ImVec2(x + a_height * 0.3f, pad + 2.0f), ImVec2(std::min(x + a_height * 0.3f + length, a_width - capEnd), pad + 2.0f), WithAlpha(pale, 0.25f), 1.0f);
			}

			// The case: a heavy pale line with a shadow outside and a lit edge
			const ImVec2 outline[] = { ImVec2(left, midY), ImVec2(left + point, 0.0f), ImVec2(right - point, 0.0f), ImVec2(right, midY), ImVec2(right - point, a_height), ImVec2(left + point, a_height) };
			const float  stroke = std::max(1.5f, a_height * 0.055f);
			a_list->AddPolyline(outline, 6, WithAlpha(shadow, 0.9f), ImDrawFlags_Closed, stroke + 2.0f);
			a_list->AddPolyline(outline, 6, pale, ImDrawFlags_Closed, stroke);
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Stud ) --------------------------
	//---------------------------------------------------

	// A cut gilt lozenge, or an empty one in the line's own colour
	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};
		if (!a_filled) {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.2f));
			return;
		}
		const float alpha = ((a_colour >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
		a_drawList->AddConvexPolyFilled(points, 4, WithAlpha(gilt, alpha));
		a_drawList->AddTriangleFilled(points[3], points[0], a_centre, WithAlpha(giltLight, alpha));
		a_drawList->AddTriangleFilled(points[1], points[2], a_centre, WithAlpha(giltDeep, 0.8f * alpha));
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Marker ) ------------------------
	//---------------------------------------------------

	// Needed / found: an open lozenge with a spark in it, cast in gilt as it's found
	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float radius = a_size * 0.34f;
		if (a_fill < 1.0f) {
			const ImU32 line = WithAlpha(a_colour, 1.0f - a_fill * 0.7f);
			Stud(a_drawList, a_centre, radius, line, false);
			const ImVec2 spark[4] = { ImVec2(a_centre.x, a_centre.y - radius * 0.34f), ImVec2(a_centre.x + radius * 0.34f, a_centre.y), ImVec2(a_centre.x, a_centre.y + radius * 0.34f), ImVec2(a_centre.x - radius * 0.34f, a_centre.y) };
			a_drawList->AddConvexPolyFilled(spark, 4, line);
		}
		if (a_fill > 0.0f) {
			Stud(a_drawList, a_centre, radius * 1.08f, WithAlpha(IM_COL32(255, 255, 255, 255), a_fill), true);
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Ornament ) ----------------------
	//---------------------------------------------------

	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		Stud(a_drawList, a_centre, a_radius * 1.3f, a_colour, false);
		Stud(a_drawList, a_centre, a_radius * 0.5f, a_colour, false);
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Rule ) --------------------------
	//---------------------------------------------------

	// A fine silver line ending in a pair of lozenges and a point
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize();
		const float direction = a_to.x >= a_from.x ? 1.0f : -1.0f;
		if ((a_to.x - a_from.x) * direction < size * 1.5f) {
			return;
		}
		const ImU32 line = WithAlpha(silver, 0.8f);
		const float radius = size * 0.13f;
		const float end = a_to.x - direction * size * 0.25f;
		const float second = end - direction * radius * 2.6f;
		a_drawList->AddLine(a_from, ImVec2(a_to.x, a_to.y), line, 1.0f);
		Stud(a_drawList, ImVec2(end, a_to.y), radius, line, false);
		Stud(a_drawList, ImVec2(second, a_to.y), radius * 1.3f, line, false);
		(void)a_colour;
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Divider ) -----------------------
	//---------------------------------------------------

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const ImU32 clear = WithAlpha(a_colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_from, ImVec2(midX, a_from.y + 1.0f), clear, a_colour, a_colour, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(midX, a_from.y), ImVec2(a_to.x, a_from.y + 1.0f), a_colour, clear, clear, a_colour);
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Open Frame ) --------------------
	//---------------------------------------------------

	void OpenFrame() {

		openFrame = true;
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Frame ) -------------------------
	//---------------------------------------------------

	// Slate to the window's edge, and silver tracery round it unless the window frames its own parts
	void Frame(ImDrawList* a_drawList, ImVec2 a_inMin, ImVec2 a_inMax) {

		const float  size = ImGui::GetFontSize();
		const float  bleed = size * 0.4f;
		const ImVec2 min(a_inMin.x - bleed, a_inMin.y - bleed);
		const ImVec2 max(a_inMax.x + bleed, a_inMax.y + bleed);
		const bool   open = std::exchange(openFrame, false);

		Baked(a_drawList, 0x0C10u, min, max, [&](ImDrawList* a_list) { Damask(a_list, min, max); });
		if (!open) {
			Baked(a_drawList, 0x0C11u, a_inMin, a_inMax, [&](ImDrawList* a_list) { SquareFrame(a_list, a_inMin, a_inMax, 0xC100u); });
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Header Band ) -------------------
	//---------------------------------------------------

	// The head of the window has no frame of its own: two banners hung from the very top, and a rule running out either side of them
	void HeaderBand(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_top, float a_bottom) {

		const float  size = ImGui::GetFontSize();
		const float  midX = (a_titleMin.x + a_titleMax.x) * 0.5f;
		const float  offset = (a_titleMax.x - a_titleMin.x) * 0.5f + size * 2.9f;
		const float  width = size * 2.9f;
		const ImVec2 min(a_titleMin.x - size * 14.0f, a_top);
		const ImVec2 max(a_titleMax.x + size * 14.0f, a_bottom + size);
		Baked(a_drawList, 0x0C20u, min, max, [&](ImDrawList* a_list) {
			for (const float side : { -1.0f, 1.0f }) {
				Swallowtail(a_list, midX + side * offset, a_top, width, a_bottom - a_top + size * 0.15f, side < 0.0f ? 7u : 19u);

				const float y = (a_titleMin.y + a_titleMax.y) * 0.5f + size * 0.2f;
				const float inner = midX + side * (offset + width * 0.5f + size * 0.75f);
				const float outer = inner + side * size * 7.0f;
				const ImU32 bright = WithAlpha(silverLight, 0.9f);
				const ImU32 clear = WithAlpha(silverLight, 0.0f);
				if (side < 0.0f) {
					a_list->AddRectFilledMultiColor(ImVec2(outer, y - 1.0f), ImVec2(inner, y + 1.0f), clear, bright, bright, clear);
				} else {
					a_list->AddRectFilledMultiColor(ImVec2(inner, y - 1.0f), ImVec2(outer, y + 1.0f), bright, clear, clear, bright);
				}
				Stud(a_list, ImVec2(inner, y), size * 0.26f, silverLight, false);
				Stud(a_list, ImVec2(inner, y), size * 0.09f, silverLight, false);
			}
		});
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Body Frame ) --------------------
	//---------------------------------------------------

	void BodyFrame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		Baked(a_drawList, 0x0C30u, a_min, a_max, [&](ImDrawList* a_list) { SquareFrame(a_list, a_min, a_max, 0xC200u); });
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Footer Box ) --------------------
	//---------------------------------------------------

	// The controller's hints in a long box drawn out to a point at either end
	void FooterBox(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		Baked(a_drawList, 0x0C40u, a_min, a_max, [&](ImDrawList* a_list) {
			const float midY = (a_min.y + a_max.y) * 0.5f;
			const float point = size * 0.75f;
			const ImVec2 outline[] = { ImVec2(a_min.x + point, a_min.y), ImVec2(a_max.x - point, a_min.y), ImVec2(a_max.x - point * 0.2f, midY), ImVec2(a_max.x - point, a_max.y), ImVec2(a_min.x + point, a_max.y), ImVec2(a_min.x + point * 0.2f, midY) };

			Bar(a_list, ImVec2(a_min.x + point, a_min.y - 1.5f), ImVec2(a_max.x - point, a_min.y + 1.5f), 0xC301u);
			Bar(a_list, ImVec2(a_min.x + point, a_max.y - 1.5f), ImVec2(a_max.x - point, a_max.y + 1.5f), 0xC302u);
			for (const float side : { 0.0f, 1.0f }) {
				const float x = side > 0.5f ? a_max.x : a_min.x;
				const float in = side > 0.5f ? -1.0f : 1.0f;
				Lancet(a_list, ImVec2(x + in * point, a_min.y), ImVec2(x + in * point * 0.2f, midY), size * 0.06f);
				Lancet(a_list, ImVec2(x + in * point, a_max.y), ImVec2(x + in * point * 0.2f, midY), size * 0.06f);
				TraceryCorner(a_list, ImVec2(x + in * point, a_min.y), size * 1.3f, in, 1.0f);
				TraceryCorner(a_list, ImVec2(x + in * point, a_max.y), size * 1.3f, in, -1.0f);
			}
			(void)outline;
		});
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Panel ) -------------------------
	//---------------------------------------------------

	// Inside a panel: slate a little darker than the wall, shadowed at its edges under the moulding
	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool) {

		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		const float size = ImGui::GetFontSize();
		if (width < size * 4.0f || height < size * 2.0f) {
			return;
		}
		Baked(a_drawList, 0x0C50u, a_min, a_max, [&](ImDrawList* a_list) {
			const float band = std::min(width, height) * 0.2f + size;
			Mesh(a_list, a_min, a_max, size * 1.4f, size * 1.4f, [&](float a_x, float a_y) {
				const float cloud = Fbm(a_x / (size * 6.0f) + 13.0f, a_y / (size * 5.0f), 3);
				return WithAlpha(shadow, 0.16f + EdgeWeight(ImVec2(a_x, a_y), a_min, a_max, band) * 0.4f + std::max(0.0f, cloud - 0.55f) * 0.3f);
			});
		});
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Panel Corners ) -----------------
	//---------------------------------------------------

	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		// Clear of the panel's clip, which would halve the moulding
		const float  in = std::max(1.5f, size * 0.09f);
		const ImVec2 min(a_min.x + in, a_min.y + in);
		const ImVec2 max(a_max.x - in, a_max.y - in);
		Baked(a_drawList, 0x0C60u, min, max, [&](ImDrawList* a_list) { SquareFrame(a_list, min, max, 0xC400u); });
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Spine ) -------------------------
	//---------------------------------------------------

	// A sidebar row as a book lying on its shelf, spine out. The chosen one sits in a pale silver case drawn to a point
	// at each end. Each binding is painted once at the origin and its triangles reused at every row that wears it
	void Spine(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, int a_book, bool a_chosen, float a_glow) {

		// The chosen book stands proud of its neighbours, its case lapping over the gaps
		const float  width = a_max.x - a_min.x;
		const float  rowHeight = a_max.y - a_min.y;
		const float  height = rowHeight * (a_chosen ? 1.26f : 1.0f);
		const ImVec2 at(a_min.x, a_min.y - (height - rowHeight) * 0.5f);
		const auto   binding = static_cast<std::size_t>(a_book) % std::size(shelf);
		const ImU32  leather = a_chosen ? 0u : shelf[binding];
		const float  size = ImGui::GetFontSize();
		const auto   uv = ImGui::GetFontTexUvWhitePixel();

		struct Painted
		{
			ImU32    leather;
			BakedArt art;
		};
		static std::vector<Painted> painted;
		const auto cached = std::ranges::find_if(painted, [&](const Painted& a_entry) {
			return a_entry.leather == leather && a_entry.art.max.x == width && a_entry.art.max.y == height && a_entry.art.size == size && a_entry.art.uv.x == uv.x && a_entry.art.uv.y == uv.y;
		});
		if (cached != painted.end()) {
			Blit(a_drawList, cached->art, at);
		} else {
			if (painted.size() >= 32) {
				painted.clear();
			}
			Painted entry{ leather, BakedArt{ 0u, ImVec2(0.0f, 0.0f), ImVec2(width, height), size, uv, 0u } };
			Record(a_drawList, entry.art, [&](ImDrawList* a_list) {
				if (a_chosen) {
					PaintCase(a_list, width, height);
				} else {
					PaintBook(a_list, width, height, leather, static_cast<float>(binding));
				}
			});
			Blit(a_drawList, entry.art, at);
			painted.push_back(std::move(entry));
		}

		if (!a_chosen && a_glow > 0.0f) {
			a_drawList->AddRect(a_min, a_max, WithAlpha(silverLight, a_glow * 0.75f), rowHeight * 0.18f, 0, 1.0f);
		}
	}

	float SpineRight() noexcept {

		return ImGui::GetFontSize() * 0.85f;
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Plaque ) ------------------------
	//---------------------------------------------------

	// A button: slate with its corners cut, a silver line round it and a small tick at each cut
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed) {

		const float size = ImGui::GetFontSize();
		const float cut = size * 0.22f;
		const auto  path = ChamferPath(a_min, a_max, cut);
		a_drawList->AddConvexPolyFilled(path.data(), static_cast<int>(path.size()), a_pressed ? violetDark : Mix(IM_COL32(14, 14, 18, 255), violet, a_glow * 0.75f));
		a_drawList->AddPolyline(path.data(), static_cast<int>(path.size()), Mix(silver, silverLight, a_glow), ImDrawFlags_Closed, std::max(1.0f, size * 0.08f));
		for (const auto& corner : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float sx = corner.x == a_min.x ? 1.0f : -1.0f;
			const float sy = corner.y == a_min.y ? 1.0f : -1.0f;
			a_drawList->AddCircleFilled(ImVec2(corner.x + sx * cut * 0.45f, corner.y + sy * cut * 0.45f), std::max(1.0f, size * 0.05f), silverLight, 6);
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Pill ) --------------------------
	//---------------------------------------------------

	// The filter box: a long lozenge-ended slot
	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow) {

		const float size = ImGui::GetFontSize();
		const float round = (a_max.y - a_min.y) * 0.5f;
		a_drawList->AddRectFilled(a_min, a_max, IM_COL32(10, 10, 13, 255), round);
		a_drawList->AddRect(a_min, a_max, Mix(silver, silverLight, a_glow), round, 0, std::max(1.0f, size * 0.08f));
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Card ) --------------------------
	//---------------------------------------------------

	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit) {

		const float size = ImGui::GetFontSize();
		const auto  path = ChamferPath(a_min, a_max, size * 0.28f);
		a_drawList->AddConvexPolyFilled(path.data(), static_cast<int>(path.size()), WithAlpha(IM_COL32(12, 12, 16, 255), 0.7f));
		a_drawList->AddPolyline(path.data(), static_cast<int>(path.size()), a_lit ? silverLight : silver, ImDrawFlags_Closed, std::max(1.0f, size * 0.07f));
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Check ) -------------------------
	//---------------------------------------------------

	// A toggle: a ruled box, with a pen stroke through it once on
	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float half = a_size * 0.46f;
		const float pen = std::max(1.0f, a_size * 0.08f);
		a_drawList->AddRect(ImVec2(a_centre.x - half, a_centre.y - half), ImVec2(a_centre.x + half, a_centre.y + half), silver, a_size * 0.08f, 0, pen);
		if (a_fill > 0.0f) {
			const ImVec2 tick[3] = { ImVec2(a_centre.x - half * 0.6f, a_centre.y), ImVec2(a_centre.x - half * 0.1f, a_centre.y + half * 0.55f), ImVec2(a_centre.x + half * 0.9f, a_centre.y - half * 0.75f) };
			a_drawList->AddPolyline(tick, 3, WithAlpha(a_colour, a_fill), ImDrawFlags_None, pen * 1.6f);
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Highlight ) ---------------------
	//---------------------------------------------------

	// A chosen row: violet fading to nothing along it, a silver chevron at its head
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float width = a_max.x - a_min.x;
		const float midY = (a_min.y + a_max.y) * 0.5f;
		const ImU32 strong = WithAlpha(violetLight, 0.85f * a_alpha);
		const ImU32 mid = WithAlpha(violet, 0.7f * a_alpha);
		const ImU32 clear = WithAlpha(violet, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + width * 0.35f, a_max.y), strong, mid, mid, strong);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x + width * 0.35f, a_min.y), a_max, mid, clear, clear, mid);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, a_min.y + 1.0f), WithAlpha(silverLight, 0.6f * a_alpha), clear, clear, WithAlpha(silverLight, 0.6f * a_alpha));
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - 1.0f), a_max, WithAlpha(silverLight, 0.6f * a_alpha), clear, clear, WithAlpha(silverLight, 0.6f * a_alpha));
		const float  point = (a_max.y - a_min.y) * 0.3f;
		const ImVec2 chevron[3] = { ImVec2(a_min.x + point * 0.4f, a_min.y + 1.0f), ImVec2(a_min.x - point * 0.2f, midY), ImVec2(a_min.x + point * 0.4f, a_max.y - 1.0f) };
		a_drawList->AddPolyline(chevron, 3, WithAlpha(silverLight, a_alpha), ImDrawFlags_None, std::max(1.0f, ImGui::GetFontSize() * 0.08f));
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Heading Band ) ------------------
	//---------------------------------------------------

	float HeadingHeight() noexcept {

		return ImGui::GetFontSize() * 2.6f;
	}

	void HeadingBand(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		for (const float y : { a_min.y, a_max.y }) {
			Bar(a_drawList, ImVec2(a_min.x + size * 0.5f, y - 1.0f), ImVec2(a_max.x, y + 1.0f), y == a_min.y ? 0xC501u : 0xC502u);
			Stud(a_drawList, ImVec2(a_min.x + size * 0.3f, y), size * 0.2f, silverLight, false);
			Stud(a_drawList, ImVec2(a_min.x + size * 0.3f, y), size * 0.07f, silverLight, false);
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Arcane Circle ) -----------------
	//---------------------------------------------------

	// A summoning circle chalked in the corner of the page: two rings of sigils and a seven pointed star, half under the panel's edge
	void ArcaneCircle(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		if (a_max.x - a_min.x < size * 16.0f || a_max.y - a_min.y < size * 7.0f) {
			return;
		}
		const ImVec2 clipMax(a_max.x - 1.0f, a_max.y - 1.0f);
		a_drawList->PushClipRect(a_min, clipMax, true);
		Baked(a_drawList, 0x0C70u, a_min, a_max, [&](ImDrawList* a_list) {
			const float  radius = std::min(size * 6.0f, (a_max.y - a_min.y) * 0.62f);
			const ImVec2 centre(a_max.x - radius * 0.95f, a_max.y + radius * 0.14f);
			const ImU32  line = WithAlpha(silver, 0.38f);
			const ImU32  faint = WithAlpha(silver, 0.26f);

			a_list->AddCircle(centre, radius, line, 96, 1.2f);
			a_list->AddCircle(centre, radius * 0.8f, line, 96, 1.0f);

			constexpr int sigils = 18;
			Random        random{ 0xC701u };
			for (int i = 0; i < sigils; ++i) {
				const float  angle = static_cast<float>(i) / sigils * pi * 2.0f;
				const ImVec2 at(centre.x + std::cos(angle) * radius * 0.9f, centre.y + std::sin(angle) * radius * 0.9f);
				const float  arm = size * 0.14f;
				const int    strokes = 2 + static_cast<int>(random.Next() * 2.0f);
				for (int stroke = 0; stroke < strokes; ++stroke) {
					const float turn = random.Range(0.0f, pi);
					a_list->AddLine(ImVec2(at.x - std::cos(turn) * arm, at.y - std::sin(turn) * arm), ImVec2(at.x + std::cos(turn) * arm, at.y + std::sin(turn) * arm), line, 1.0f);
				}
			}

			// Stars within stars: seven points joined every third and every second, eleven further in, spokes to the ring
			constexpr int points = 7;
			const auto    point = [&](int a_index, int a_of, float a_reach, float a_turn) {
				const float angle = -pi * 0.5f + a_turn + static_cast<float>(a_index) / static_cast<float>(a_of) * pi * 2.0f;
				return ImVec2(centre.x + std::cos(angle) * radius * a_reach, centre.y + std::sin(angle) * radius * a_reach);
			};
			for (int i = 0; i < points; ++i) {
				a_list->AddLine(point(i, points, 0.78f, 0.0f), point(i + 3, points, 0.78f, 0.0f), line, 1.0f);
				a_list->AddLine(point(i, points, 0.78f, 0.0f), point(i + 2, points, 0.78f, 0.0f), faint, 1.0f);
				a_list->AddLine(point(i, points, 0.56f, 0.45f), point(i + 2, points, 0.56f, 0.45f), faint, 1.0f);
				a_list->AddLine(point(i, points, 0.78f, 0.0f), point(i, points, 0.24f, 0.0f), faint, 1.0f);
				a_list->AddCircleFilled(point(i, points, 0.78f, 0.0f), size * 0.06f, line, 6);
			}
			constexpr int inner = 11;
			for (int i = 0; i < inner; ++i) {
				a_list->AddLine(point(i, inner, 0.42f, 0.0f), point(i + 4, inner, 0.42f, 0.0f), faint, 1.0f);
			}
			a_list->AddCircle(centre, radius * 0.24f, faint, 40, 1.0f);
		});
		a_drawList->PopClipRect();
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Open Book ) ---------------------
	//---------------------------------------------------

	// An open book in silver point, its pages lifting from the gutter, a block of leaves beneath, lines of script on each
	void OpenBook(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		// Free to rise past the top of its space, as it isn't boxed in
		const float room = a_max.y - a_min.y + size * 1.3f;
		if (room < size * 3.0f || a_max.x - a_min.x < size * 24.0f) {
			return;
		}
		Baked(a_drawList, 0x0C80u, a_min, a_max, [&](ImDrawList* a_list) {
			const float  height = std::min(room * 0.92f, size * 5.6f);
			const float  width = height * 1.75f;
			const ImVec2 centre(a_max.x - width * 0.6f, a_max.y - height * 0.55f);
			const auto   at = [&](float a_x, float a_y) { return ImVec2(centre.x + a_x * width, centre.y + a_y * height); };
			const ImU32  line = WithAlpha(silver, 0.62f);
			const ImU32  fine = WithAlpha(silver, 0.4f);
			const auto   path = [&](std::initializer_list<ImVec2> a_points, ImU32 a_colour, float a_pen) {
				std::vector<ImVec2> points(a_points);
				const auto          curve = Spline(points.data(), static_cast<int>(points.size()), false, 5);
				a_list->AddPolyline(curve.data(), static_cast<int>(curve.size()), a_colour, ImDrawFlags_None, a_pen);
			};

			for (const float side : { -1.0f, 1.0f }) {
				// The page: its top sweeping up from the gutter, its outer edge leaning out, its foot back in
				path({ at(0.0f, -0.28f), at(side * 0.1f, -0.42f), at(side * 0.24f, -0.46f), at(side * 0.36f, -0.34f) }, line, 1.3f);
				path({ at(side * 0.36f, -0.34f), at(side * 0.44f, 0.26f) }, line, 1.3f);
				path({ at(side * 0.44f, 0.26f), at(side * 0.24f, 0.28f), at(side * 0.1f, 0.3f), at(0.0f, 0.4f) }, line, 1.3f);

				// Leaves below it, and its cover
				for (const float drop : { 0.05f, 0.1f, 0.15f }) {
					path({ at(side * 0.46f, 0.26f + drop), at(side * 0.24f, 0.3f + drop), at(side * 0.1f, 0.33f + drop), at(0.0f, 0.42f + drop) }, fine, 1.0f);
				}
				path({ at(side * 0.4f, -0.3f), at(side * 0.5f, 0.3f), at(side * 0.5f, 0.42f) }, fine, 1.0f);
				path({ at(side * 0.38f, -0.32f), at(side * 0.47f, 0.28f) }, fine, 1.0f);
				path({ at(side * 0.5f, 0.42f), at(side * 0.26f, 0.46f), at(side * 0.09f, 0.5f) }, line, 1.3f);

				// Script
				constexpr int lines = 7;
				for (int row = 0; row < lines; ++row) {
					const float t = static_cast<float>(row) / (lines - 1);
					const float y = -0.28f + t * 0.46f;
					const float from = 0.06f + t * 0.01f;
					const float to = 0.33f + t * 0.07f;
					path({ at(side * from, y + 0.02f), at(side * (from + to) * 0.5f, y - 0.03f), at(side * to, y) }, fine, 1.0f);
				}
			}
			path({ at(0.0f, -0.28f), at(0.0f, 0.4f) }, line, 1.3f);
			path({ at(-0.07f, 0.47f), at(0.0f, 0.42f), at(0.07f, 0.47f) }, line, 1.2f);
			path({ at(-0.07f, 0.47f), at(0.0f, 0.54f), at(0.07f, 0.47f) }, line, 1.2f);
		});
	}
}

namespace CompletionistUI::GothicDecor
{
	//---------------------------------------------------
	//-- Gothic Decor ( Perk Node ) ---------------------
	//---------------------------------------------------

	// Runes in rings: violet and lit when taken, a ruled ring when next, a pin prick when locked
	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			a_drawList->AddCircleFilled(a_centre, a_radius, silver, 20);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.84f, violet, 20);
			Stud(a_drawList, a_centre, a_radius * 0.5f, a_colour, true);
			return;
		case PerkState::kNext:
			// Over the lines, so they end at its rim
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.85f, slate, 20);
			a_drawList->AddCircle(a_centre, a_radius * 0.85f, a_colour, 20, std::max(1.2f, a_radius * 0.13f));
			Stud(a_drawList, a_centre, a_radius * 0.35f, a_colour, false);
			return;
		default:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.45f, slate, 12);
			a_drawList->AddCircle(a_centre, a_radius * 0.45f, a_colour, 12, 1.0f);
			return;
		}
	}

	//---------------------------------------------------
	//-- Gothic Decor ( Perk Link ) ---------------------
	//---------------------------------------------------

	// A silver line drawn with a straight rule where they're taken, dotted where they aren't
	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto points = PerkCurve(a_from, a_to, std::max(2.0f, a_radius * 0.35f));
		if (a_lit) {
			a_drawList->AddPolyline(points.data(), static_cast<int>(points.size()), a_colour, ImDrawFlags_None, std::max(1.5f, a_radius * 0.18f));
			return;
		}
		for (std::size_t i = 0; i + 1 < points.size(); i += 3) {
			a_drawList->AddLine(points[i], points[std::min(i + 1, points.size() - 1)], a_colour, 1.0f);
		}
	}
}
