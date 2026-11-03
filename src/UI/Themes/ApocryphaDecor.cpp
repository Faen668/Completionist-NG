#include "UI/Themes/ApocryphaDecor.hpp"
#include "UI/Themes/DecorDraw.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>
#include <vector>

namespace CompletionistUI::ApocryphaDecor
{
	using namespace DecorDraw;

	namespace
	{
		constexpr float pi = std::numbers::pi_v<float>;

		// Black ink, wet hide, sick green, old bone
		constexpr ImU32 ink = IM_COL32(6, 9, 7, 255);
		constexpr ImU32 hide = IM_COL32(22, 30, 23, 255);
		constexpr ImU32 hideLit = IM_COL32(70, 92, 62, 255);
		constexpr ImU32 belly = IM_COL32(112, 126, 80, 255);
		constexpr ImU32 sheen = IM_COL32(196, 220, 164, 255);
		constexpr ImU32 sucker = IM_COL32(138, 150, 96, 255);
		constexpr ImU32 pit = IM_COL32(26, 22, 18, 255);
		constexpr ImU32 vein = IM_COL32(70, 112, 70, 255);
		constexpr ImU32 acid = IM_COL32(150, 204, 92, 255);
		constexpr ImU32 acidDeep = IM_COL32(58, 94, 40, 255);
		constexpr ImU32 ichor = IM_COL32(214, 196, 84, 255);
		constexpr ImU32 bone = IM_COL32(222, 226, 176, 255);
		constexpr ImU32 rule = IM_COL32(84, 108, 72, 255);

		bool groundDrawn = false;

		//---------------------------------------------------
		//-- Apocrypha Decor ( Walk ) -----------------------
		//---------------------------------------------------

		// A line that steers as it goes: a slow weave along its length, then a curl of a_curl radians over its last stretch
		std::vector<ImVec2> Walk(ImVec2 a_start, float a_heading, float a_length, float a_weave, float a_weaveLength, float a_phase, float a_curl) {

			const float step = std::max(2.0f, ImGui::GetFontSize() * 0.3f);
			const int   steps = std::max(3, static_cast<int>(a_length / step));
			std::vector<ImVec2> out;
			out.reserve(static_cast<std::size_t>(steps) + 1);
			ImVec2 at = a_start;
			float  heading = a_heading;
			const float curlFrom = std::max(a_length * 0.55f, a_length - ImGui::GetFontSize() * 4.5f);
			out.push_back(at);
			for (int i = 1; i <= steps; ++i) {
				const float t = static_cast<float>(i) / static_cast<float>(steps);
				const float along = t * a_length;
				const float ramp = std::clamp((along - curlFrom) / std::max(1.0f, a_length - curlFrom), 0.0f, 1.0f);
				const float turn = a_weave * std::cos(along / a_weaveLength * 2.0f * pi + a_phase);
				heading = a_heading + turn + a_curl * ramp * ramp;
				at = ImVec2(at.x + std::cos(heading) * step * a_length / (static_cast<float>(steps) * step), at.y + std::sin(heading) * step * a_length / (static_cast<float>(steps) * step));
				out.push_back(at);
			}
			return out;
		}

		//---------------------------------------------------
		//-- Apocrypha Decor ( Tentacle ) -------------------
		//---------------------------------------------------

		// Shaded as the round, wet thing it is: every vertex is lit from where it would face on a tube. a_side says which edge carries the suckers
		void Tentacle(ImDrawList* a_list, const std::vector<ImVec2>& a_path, float a_root, float a_side, std::uint32_t a_seed, float a_alpha = 1.0f) {

			if (a_path.size() < 3) {
				return;
			}
			std::vector<float> along(a_path.size(), 0.0f);
			for (std::size_t i = 1; i < a_path.size(); ++i) {
				along[i] = along[i - 1] + std::hypot(a_path[i].x - a_path[i - 1].x, a_path[i].y - a_path[i - 1].y);
			}
			const float total = std::max(1.0f, along.back());
			const float seed = static_cast<float>(a_seed % 977u);
			const auto  widthAt = [&](std::size_t a_index) {
				const float t = along[a_index] / total;
				return a_root * std::pow(std::max(0.0f, 1.0f - t), 0.55f) * (0.94f + 0.12f * ValueNoise(along[a_index] / (a_root * 2.2f), seed)) + 0.6f;
			};

			// Its shadow on what's behind
			const ImU32 shadow = WithAlpha(ink, 0.5f * a_alpha);
			const float drop = a_root * 0.13f;
			for (std::size_t i = 1; i < a_path.size(); ++i) {
				a_list->AddLine(ImVec2(a_path[i - 1].x + drop * 0.6f, a_path[i - 1].y + drop), ImVec2(a_path[i].x + drop * 0.6f, a_path[i].y + drop), shadow, widthAt(i));
			}

			constexpr float stops[] = { -1.0f, -0.74f, -0.4f, 0.0f, 0.4f, 0.74f, 1.0f };
			constexpr int   across = 7;
			const auto      uv = ImGui::GetFontTexUvWhitePixel();
			const int       rows = static_cast<int>(a_path.size());
			a_list->PrimReserve((rows - 1) * (across - 1) * 6, rows * across);
			const auto base = a_list->_VtxCurrentIdx;
			for (int row = 0; row < rows; ++row) {
				const auto  index = static_cast<std::size_t>(row);
				const auto  normal = NormalAt(a_path, index);
				const float width = widthAt(index);
				const float bands = 0.86f + 0.14f * std::sin(along[index] / std::max(2.0f, a_root * std::max(0.25f, width / a_root) * 0.34f) + seed);
				for (const float stop : stops) {
					const float nz = std::sqrt(std::max(0.0f, 1.0f - stop * stop));
					const float nx = normal.x * stop;
					const float ny = normal.y * stop;
					// A low light from up and to the left
					const float light = std::max(0.0f, nx * -0.36f + ny * -0.62f + nz * 0.7f);
					const float shine = std::pow(std::max(0.0f, nx * -0.2f + ny * -0.35f + nz * 0.915f), 26.0f);
					ImU32       colour = Mix(hide, hideLit, light * light);
					const float under = stop * a_side;
					if (under > 0.2f) {
						colour = Mix(colour, belly, (under - 0.2f) * 0.9f * light);
					}
					colour = Shade(colour, bands);
					colour = Mix(colour, sheen, shine * 0.62f);
					if (std::abs(stop) > 0.9f) {
						colour = Mix(colour, ink, 0.75f);
					}
					a_list->PrimWriteVtx(ImVec2(a_path[index].x + nx * width * 0.5f, a_path[index].y + ny * width * 0.5f), uv, WithAlpha(colour, a_alpha));
				}
			}
			for (int row = 0; row < rows - 1; ++row) {
				for (int column = 0; column < across - 1; ++column) {
					const auto corner = static_cast<ImDrawIdx>(base + static_cast<unsigned int>(row * across + column));
					const auto next = static_cast<ImDrawIdx>(corner + across);
					a_list->PrimWriteIdx(corner);
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(corner + 1));
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_list->PrimWriteIdx(corner);
					a_list->PrimWriteIdx(static_cast<ImDrawIdx>(next + 1));
					a_list->PrimWriteIdx(next);
				}
			}

			// The suckers, down the belly and shrinking with it
			if (a_root < 6.0f) {
				return;
			}
			float next = a_root * 0.9f;
			for (std::size_t i = 1; i < a_path.size(); ++i) {
				if (along[i] < next) {
					continue;
				}
				const float width = widthAt(i);
				if (width < 3.0f) {
					break;
				}
				const auto   normal = NormalAt(a_path, i);
				const float  radius = width * 0.135f;
				const ImVec2 at(a_path[i].x + normal.x * a_side * width * 0.3f, a_path[i].y + normal.y * a_side * width * 0.3f);
				a_list->AddCircleFilled(at, radius * 1.25f, WithAlpha(ink, 0.5f * a_alpha), 10);
				a_list->AddCircleFilled(at, radius, WithAlpha(sucker, a_alpha), 10);
				a_list->AddCircleFilled(at, radius * 0.5f, WithAlpha(pit, a_alpha), 8);
				next = along[i] + std::max(width * 0.62f, radius * 3.2f);
			}
		}

		//---------------------------------------------------
		//-- Apocrypha Decor ( Script ) ---------------------
		//---------------------------------------------------

		// Lines of writing nobody should try to read: angular strokes on a small grid, no two letters the same
		void Script(ImDrawList* a_list, ImVec2 a_at, float a_length, float a_height, ImU32 a_colour, std::uint32_t a_seed) {

			Random random{ a_seed | 1u };
			const float width = a_height * 0.72f;
			const float pen = std::max(1.0f, a_height * 0.09f);
			float       x = a_at.x;
			const float end = a_at.x + a_length;
			while (x + width < end) {
				if (random.Next() < 0.14f) {
					x += width * 0.9f;
					continue;
				}
				ImVec2      from(x + width * random.Next(), a_at.y + a_height * random.Next());
				const int   strokes = 2 + static_cast<int>(random.Next() * 3.0f);
				for (int i = 0; i < strokes; ++i) {
					const ImVec2 to(x + width * std::round(random.Next() * 2.0f) * 0.5f, a_at.y + a_height * std::round(random.Next() * 3.0f) / 3.0f);
					a_list->AddLine(from, to, a_colour, pen);
					from = to;
				}
				if (random.Next() < 0.3f) {
					a_list->AddCircleFilled(ImVec2(x + width * 0.5f, a_at.y - a_height * 0.18f), pen * 0.7f, a_colour, 6);
				}
				x += width * 1.28f;
			}
		}

		//---------------------------------------------------
		//-- Apocrypha Decor ( Ink ) ------------------------
		//---------------------------------------------------

		// Ink dropped in water: noise pushed about by noise, which is what pulls the veins out long and folded
		void Marble(ImDrawList* a_list, ImVec2 a_min, ImVec2 a_max, float a_strength, float a_seed) {

			const float size = ImGui::GetFontSize();
			const float area = (a_max.x - a_min.x) * (a_max.y - a_min.y);
			const float cell = std::max(size * 0.42f, std::sqrt(std::max(1.0f, area) / 36000.0f));
			const float scale = size * 13.0f;
			Mesh(a_list, a_min, a_max, cell, cell, [&](float a_x, float a_y) {
				const float x = a_x / scale + a_seed;
				const float y = a_y / scale;
				const float pushX = Fbm(x, y, 3);
				const float pushY = Fbm(x + 5.2f, y + 1.3f, 3);
				const float value = Fbm(x + pushX * 3.4f, y + pushY * 3.4f, 4);
				const float fold = 1.0f - std::min(1.0f, std::abs(value - 0.5f) * 11.0f);
				const float deep = Smooth((0.46f - value) * 5.0f);
				const float glow = fold * fold * (0.32f + 0.68f * pushY);
				if (glow * 0.8f > deep * 0.5f) {
					return WithAlpha(vein, glow * 0.34f * a_strength);
				}
				return WithAlpha(ink, deep * 0.62f * a_strength);
			});
		}

		//---------------------------------------------------
		//-- Apocrypha Decor ( Watching Eye ) ---------------
		//---------------------------------------------------

		// How far the lids are apart this moment: open, with a quick blink every several seconds, each eye on its own clock
		float BlinkAt(std::uint32_t a_seed) {

			const float seed = Hash(static_cast<int>(a_seed), 41);
			const float period = 5.0f + seed * 7.0f;
			const float at = std::fmod(static_cast<float>(ImGui::GetTime()) + seed * 37.0f, period);
			constexpr float shut = 0.2f;
			if (at > shut) {
				return 1.0f;
			}
			return std::abs(at / shut * 2.0f - 1.0f);
		}

		// A real eye in a fold of hide. It looks at the cursor, and stops short of following it round the back of its own head
		void WatchingEye(ImDrawList* a_list, ImVec2 a_centre, float a_radius, std::uint32_t a_seed, float a_alpha = 1.0f) {

			const float open = 0.08f + 0.92f * BlinkAt(a_seed);
			const float high = a_radius * 0.62f;
			const auto  shade = [&](ImU32 a_colour) { return WithAlpha(a_colour, a_alpha); };

			constexpr int points = 22;
			const auto    curve = [&](std::vector<ImVec2>& a_out, float a_width, float a_height, bool a_reverse) {
				for (int i = 0; i <= points; ++i) {
					const float t = static_cast<float>(a_reverse ? points - i : i) / points * 2.0f - 1.0f;
					a_out.emplace_back(a_centre.x + t * a_radius * a_width, a_centre.y + a_height * std::pow(std::max(0.0f, 1.0f - t * t), 0.72f));
				}
			};

			// The fold it sits in
			std::vector<ImVec2> socket;
			curve(socket, 1.34f, -high * 1.62f, false);
			curve(socket, 1.34f, high * 1.42f, true);
			FillShape(a_list, socket, shade(hide));

			// The white of it, gone yellow, its veins, and the iris under the lids
			std::vector<ImVec2> white;
			curve(white, 1.0f, -high, false);
			curve(white, 1.0f, high, true);
			FillShape(a_list, white, shade(Mix(bone, ichor, 0.28f)));
			Random random{ a_seed * 2654435761u | 1u };
			for (int i = 0; i < 9; ++i) {
				const float side = i % 2 == 0 ? -1.0f : 1.0f;
				const float angle = random.Range(-0.5f, 0.5f);
				ImVec2      from(a_centre.x + side * a_radius * 0.96f, a_centre.y + std::sin(angle) * high * 0.2f);
				for (int part = 0; part < 3; ++part) {
					const ImVec2 to(from.x - side * a_radius * random.Range(0.1f, 0.2f), from.y + a_radius * random.Range(-0.09f, 0.09f));
					a_list->AddLine(from, to, shade(IM_COL32(150, 74, 52, 150)), std::max(1.0f, a_radius * 0.022f));
					from = to;
				}
			}

			const auto  mouse = ImGui::GetMousePos();
			const float dx = mouse.x - a_centre.x;
			const float dy = mouse.y - a_centre.y;
			const float distance = std::max(1.0f, std::hypot(dx, dy));
			const float pull = std::min(1.0f, distance / (a_radius * 7.0f));
			const float irisSize = high * 0.98f;
			const ImVec2 iris(a_centre.x + dx / distance * pull * a_radius * 0.34f, a_centre.y + dy / distance * pull * high * 0.2f);
			a_list->AddCircleFilled(iris, irisSize, shade(ink), 28);
			a_list->AddCircleFilled(iris, irisSize * 0.93f, shade(acidDeep), 28);
			a_list->AddCircleFilled(iris, irisSize * 0.74f, shade(Mix(acidDeep, acid, 0.55f)), 28);
			a_list->AddCircleFilled(iris, irisSize * 0.48f, shade(Mix(acid, ichor, 0.7f)), 24);
			for (int i = 0; i < 26; ++i) {
				const float angle = static_cast<float>(i) / 26.0f * 2.0f * pi + random.Range(-0.07f, 0.07f);
				const float inner = irisSize * random.Range(0.3f, 0.42f);
				const float outer = irisSize * random.Range(0.68f, 0.9f);
				a_list->AddLine(ImVec2(iris.x + std::cos(angle) * inner, iris.y + std::sin(angle) * inner), ImVec2(iris.x + std::cos(angle) * outer, iris.y + std::sin(angle) * outer), shade(i % 2 == 0 ? WithAlpha(ink, 0.55f) : WithAlpha(ichor, 0.5f)), std::max(1.0f, a_radius * 0.02f));
			}
			// A slit, as a goat's has
			a_list->AddEllipseFilled(iris, ImVec2(irisSize * 0.17f, irisSize * 0.72f), shade(ink), 0.0f, 16);
			a_list->AddCircleFilled(ImVec2(iris.x - irisSize * 0.36f, iris.y - irisSize * 0.42f), irisSize * 0.14f, shade(IM_COL32(250, 252, 232, 220)), 10);

			// The lids close over all that, whatever the iris was doing near the edge
			std::vector<ImVec2> upper;
			curve(upper, 1.0f, -high * open, false);
			curve(upper, 1.34f, -high * 1.62f, true);
			FillShape(a_list, upper, shade(Mix(hide, hideLit, 0.55f)));
			std::vector<ImVec2> lower;
			curve(lower, 1.0f, high * open, false);
			curve(lower, 1.34f, high * 1.42f, true);
			FillShape(a_list, lower, shade(Mix(hide, hideLit, 0.3f)));

			// Their wet rims and the creases above
			std::vector<ImVec2> rim;
			curve(rim, 1.0f, -high * open, false);
			a_list->AddPolyline(rim.data(), static_cast<int>(rim.size()), shade(ink), ImDrawFlags_None, std::max(1.5f, a_radius * 0.075f));
			rim.clear();
			curve(rim, 1.0f, high * open, false);
			a_list->AddPolyline(rim.data(), static_cast<int>(rim.size()), shade(WithAlpha(sheen, 0.55f)), ImDrawFlags_None, std::max(1.0f, a_radius * 0.03f));
			for (const float crease : { 0.22f, 0.42f }) {
				std::vector<ImVec2> line;
				curve(line, 1.0f + crease * 0.34f, -high * (open + (1.62f - open) * crease), false);
				a_list->AddPolyline(line.data(), static_cast<int>(line.size()), shade(WithAlpha(ink, 0.65f)), ImDrawFlags_None, std::max(1.0f, a_radius * 0.028f));
			}
			std::vector<ImVec2> edge;
			curve(edge, 1.34f, -high * 1.62f, false);
			curve(edge, 1.34f, high * 1.42f, true);
			a_list->AddPolyline(edge.data(), static_cast<int>(edge.size()), shade(ink), ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.05f));
		}

		//---------------------------------------------------
		//-- Apocrypha Decor ( Corner Tip ) -----------------
		//---------------------------------------------------

		// The loose end at a corner. Not baked: this is the part that moves
		void CornerTip(ImDrawList* a_list, ImVec2 a_at, float a_heading, float a_length, float a_root, float a_curl, std::uint32_t a_seed) {

			const float time = static_cast<float>(ImGui::GetTime());
			const float seed = static_cast<float>(a_seed % 89u);
			const float sway = std::sin(time * 0.55f + seed) * 0.28f + std::sin(time * 0.23f + seed * 1.7f) * 0.2f;
			const auto  path = Walk(a_at, a_heading, a_length, 0.3f, a_length * 0.7f, time * 0.8f + seed, a_curl + sway);
			Tentacle(a_list, path, a_root, a_curl > 0.0f ? 1.0f : -1.0f, a_seed);
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Stud ) -----------------------
	//---------------------------------------------------

	void Stud(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		if (a_filled) {
			a_drawList->AddCircleFilled(a_centre, a_radius, a_colour, 14);
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.42f, ink, 10);
		} else {
			a_drawList->AddCircle(a_centre, a_radius, a_colour, 14, std::max(1.0f, a_radius * 0.24f));
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Marker ) ---------------------
	//---------------------------------------------------

	void Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		const float half = a_size * 0.62f;
		const float high = a_size * 0.4f;
		const float pen = std::max(1.0f, a_size * 0.09f);
		const float open = Smooth(a_fill);

		constexpr int points = 12;
		const auto    arc = [&](float a_height) {
			std::vector<ImVec2> out;
			out.reserve(points + 1);
			for (int i = 0; i <= points; ++i) {
				const float t = static_cast<float>(i) / points * 2.0f - 1.0f;
				out.emplace_back(a_centre.x + t * half, a_centre.y + a_height * (1.0f - t * t));
			}
			return out;
		};
		const auto lower = arc(high * 0.72f);
		if (open > 0.02f) {
			auto upper = arc(-high * open);
			std::vector<ImVec2> shape = upper;
			shape.insert(shape.end(), lower.rbegin(), lower.rend());
			FillShape(a_drawList, shape, WithAlpha(bone, 0.95f));
			const ImVec2 iris(a_centre.x, a_centre.y + high * 0.02f);
			a_drawList->PushClipRect(ImVec2(a_centre.x - half, a_centre.y - high * open), ImVec2(a_centre.x + half, a_centre.y + high), true);
			a_drawList->AddCircleFilled(iris, a_size * 0.27f, a_colour, 14);
			a_drawList->AddEllipseFilled(iris, ImVec2(a_size * 0.07f, a_size * 0.2f), ink, 0.0f, 10);
			a_drawList->PopClipRect();
			a_drawList->AddPolyline(upper.data(), static_cast<int>(upper.size()), ink, ImDrawFlags_None, pen);
			a_drawList->AddPolyline(lower.data(), static_cast<int>(lower.size()), ink, ImDrawFlags_None, pen * 0.7f);
		} else {
			// Shut, the lashes hanging
			a_drawList->AddPolyline(lower.data(), static_cast<int>(lower.size()), a_colour, ImDrawFlags_None, pen);
			for (const float t : { -0.55f, 0.0f, 0.55f }) {
				const float x = a_centre.x + t * half;
				const float y = a_centre.y + high * 0.72f * (1.0f - t * t);
				a_drawList->AddLine(ImVec2(x, y), ImVec2(x + t * a_size * 0.1f, y + a_size * 0.13f), a_colour, pen * 0.8f);
			}
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Ornament ) -------------------
	//---------------------------------------------------

	void Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		Marker(a_drawList, a_centre, a_radius * 2.3f, a_colour, 1.0f);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Rule ) -----------------------
	//---------------------------------------------------

	// A stroke of the pen, thin at both ends and never quite straight
	void Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float dx = a_to.x - a_from.x;
		const float dy = a_to.y - a_from.y;
		const float length = std::hypot(dx, dy);
		if (length < 4.0f) {
			return;
		}
		const float size = ImGui::GetFontSize();
		const ImVec2 normal(-dy / length, dx / length);
		const int   parts = std::clamp(static_cast<int>(length / (size * 0.7f)), 6, 80);
		const float seed = a_from.x * 0.013f + a_from.y * 0.021f;
		ImVec2      last = a_from;
		for (int i = 1; i <= parts; ++i) {
			const float t = static_cast<float>(i) / static_cast<float>(parts);
			const float wander = (ValueNoise(t * length / (size * 3.0f) + seed, 3.1f) - 0.5f) * size * 0.28f * std::sin(t * pi);
			const ImVec2 at(a_from.x + dx * t + normal.x * wander, a_from.y + dy * t + normal.y * wander);
			const float thick = 0.6f + std::sin(t * pi) * std::max(0.8f, size * 0.075f);
			a_drawList->AddLine(last, at, a_colour, thick);
			last = at;
		}
	}

	void Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const ImVec2 mid((a_from.x + a_to.x) * 0.5f, (a_from.y + a_to.y) * 0.5f);
		const float  gap = ImGui::GetFontSize() * 0.9f;
		const float  length = std::max(1.0f, std::hypot(a_to.x - a_from.x, a_to.y - a_from.y));
		const float  ux = (a_to.x - a_from.x) / length;
		const float  uy = (a_to.y - a_from.y) / length;
		Rule(a_drawList, a_from, ImVec2(mid.x - ux * gap, mid.y - uy * gap), a_colour);
		Rule(a_drawList, ImVec2(mid.x + ux * gap, mid.y + uy * gap), a_to, a_colour);
		Marker(a_drawList, mid, ImGui::GetFontSize() * 0.72f, acid, 1.0f);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Ground ) ---------------------
	//---------------------------------------------------

	void Ground(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour) {

		a_drawList->AddRectFilled(a_min, a_max, a_colour);
		groundDrawn = true;
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Frame ) ----------------------
	//---------------------------------------------------

	void Frame(ImDrawList* a_drawList, ImVec2 a_inMin, ImVec2 a_inMax) {

		const float  size = ImGui::GetFontSize();
		const bool   margin = std::exchange(groundDrawn, false);
		const float  bleed = margin ? 0.0f : size * 0.4f;
		const ImVec2 a_min(a_inMin.x - bleed, a_inMin.y - bleed);
		const ImVec2 a_max(a_inMax.x + bleed, a_inMax.y + bleed);
		const float  width = a_max.x - a_min.x;
		const float  height = a_max.y - a_min.y;
		const float  compact = std::clamp(std::min(width, height) / (size * 26.0f), 0.34f, 1.0f);
		// The in-game panel has no ground of its own: it keeps them to its corners, thin. A narrow window gets slimmer ones than the main one
		const bool   tiny = !margin;
		const float  slim = std::clamp(width / (size * 60.0f), 0.62f, 1.0f);
		const float  root = tiny ? size * 0.4f : size * 1.4f * compact * slim;

		Baked(a_drawList, 0x0C10u, a_min, a_max, [&](ImDrawList* a_list) { Marble(a_list, a_min, a_max, 1.0f, 3.7f); });
		Baked(a_drawList, 0x0C11u, a_min, a_max, [&](ImDrawList* a_list) {
			// Black pooling in from the edges
			EdgeShadow(a_list, a_min, a_max, size * 3.5f, WithAlpha(ink, 0.75f));

			const float off = root * 0.3f;
			const float weave = 0.1f;
			const float wave = size * 11.0f;
			const float across = tiny ? size * 2.8f : width * 0.36f;
			const float down = tiny ? size * 2.0f : height * 0.48f;
			const float foot = tiny ? size * 2.8f : width * 0.22f;
			const float hook = slim < 1.0f ? 0.65f : 1.0f;	// Not so far in that the side curls reach a narrow window's list
			const float inX = root * 0.42f;
			const float inY = root * 0.42f;

			// Down the sides first, the top and bottom lying over them
			Tentacle(a_list, Walk(ImVec2(a_min.x + inX, a_min.y - off), pi * 0.5f, down, weave, wave, 0.4f, -3.8f * hook), root, -1.0f, 0xC101u);
			Tentacle(a_list, Walk(ImVec2(a_max.x - inX, a_min.y - off), pi * 0.5f, down, weave, wave, 2.1f, 3.8f * hook), root, 1.0f, 0xC102u);
			Tentacle(a_list, Walk(ImVec2(a_min.x + inX, a_max.y + off), -pi * 0.5f, down * 0.92f, weave, wave, 1.2f, 3.8f * hook), root, 1.0f, 0xC103u);
			Tentacle(a_list, Walk(ImVec2(a_max.x - inX, a_max.y + off), -pi * 0.5f, down * 0.92f, weave, wave, 3.3f, -3.8f * hook), root, -1.0f, 0xC104u);

			Tentacle(a_list, Walk(ImVec2(a_min.x - off, a_min.y + inY), 0.0f, across, weave, wave, 1.7f, 3.9f), root, 1.0f, 0xC105u);
			Tentacle(a_list, Walk(ImVec2(a_max.x + off, a_min.y + inY), pi, across, weave, wave, 4.1f, -3.9f), root, -1.0f, 0xC106u);
			Tentacle(a_list, Walk(ImVec2(a_min.x - off, a_max.y - inY), 0.0f, foot, weave, wave, 2.6f, -3.9f), root, -1.0f, 0xC107u);
			Tentacle(a_list, Walk(ImVec2(a_max.x + off, a_max.y - inY), pi, foot, weave, wave, 5.3f, 3.9f), root, 1.0f, 0xC108u);
		});

		// The loose ends at the corners stir
		const float tip = tiny ? size * 1.1f : size * 5.5f * compact * (slim < 1.0f ? slim * 0.7f : 1.0f);
		const float tipRoot = root * 0.8f;
		a_drawList->PushClipRect(a_min, a_max, true);
		CornerTip(a_drawList, a_min, pi * 0.25f, tip, tipRoot, 3.6f, 0xC111u);
		CornerTip(a_drawList, ImVec2(a_max.x, a_min.y), pi * 0.75f, tip, tipRoot, -3.6f, 0xC112u);
		CornerTip(a_drawList, a_max, -pi * 0.75f, tip, tipRoot, 3.6f, 0xC113u);
		CornerTip(a_drawList, ImVec2(a_min.x, a_max.y), -pi * 0.25f, tip, tipRoot, -3.6f, 0xC114u);
		a_drawList->PopClipRect();
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Panel ) ----------------------
	//---------------------------------------------------

	void Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		const float size = ImGui::GetFontSize();
		const float width = a_max.x - a_min.x;
		const float height = a_max.y - a_min.y;
		Baked(a_drawList, 0x0C20u, a_min, a_max, [&](ImDrawList* a_list) {
			EdgeShadow(a_list, a_min, a_max, size * 1.5f, WithAlpha(ink, 0.6f));
			const float in = size * 0.2f;
			const ImU32 line = WithAlpha(rule, 0.9f);
			Rule(a_list, ImVec2(a_min.x + in, a_min.y + in), ImVec2(a_max.x - in, a_min.y + in), line);
			Rule(a_list, ImVec2(a_min.x + in, a_max.y - in), ImVec2(a_max.x - in, a_max.y - in), line);
			Rule(a_list, ImVec2(a_min.x + in, a_min.y + in), ImVec2(a_min.x + in, a_max.y - in), line);
			Rule(a_list, ImVec2(a_max.x - in, a_min.y + in), ImVec2(a_max.x - in, a_max.y - in), line);

			// A marginal line of the script along the foot of the text panels, stopping short of the eye
			if (a_emblem && width > size * 22.0f && height > size * 6.0f) {
				const float stop = width - size * 8.0f;
				Script(a_list, ImVec2(a_min.x + size * 1.0f, a_max.y - size * 1.25f), std::min(stop - size, width * 0.5f), size * 0.62f, WithAlpha(rule, 0.4f), static_cast<std::uint32_t>(a_min.x * 3.0f + a_min.y * 7.0f + width));
			}

			if (a_emblem && width > size * 18.0f && height > size * 5.0f) {
				const float radius = std::min(size * 2.5f, height * 0.26f);
				Tentacle(a_list, Walk(ImVec2(a_max.x + radius * 0.1f, a_max.y - size * 0.6f), pi, radius * 6.4f, 0.1f, size * 6.0f, 0.8f, 3.8f), size * 1.5f, 1.0f, 0xC121u);
				Tentacle(a_list, Walk(ImVec2(a_max.x - size * 0.5f, a_max.y + size * 0.2f), -pi * 0.5f - 0.1f, radius * 3.4f, 0.12f, size * 4.0f, 2.2f, -3.6f), size * 1.1f, -1.0f, 0xC122u);
			}
		});
		PanelCorners(a_drawList, a_min, a_max);

		if (a_emblem && width > size * 18.0f && height > size * 5.0f) {
			const float radius = std::min(size * 2.5f, height * 0.26f);
			WatchingEye(a_drawList, ImVec2(a_max.x - radius * 1.75f, a_max.y - radius * 1.25f), radius, static_cast<std::uint32_t>(a_min.x + a_min.y) | 3u);
		}
	}

	void PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float length = size * 1.7f;
		const float thick = size * 0.3f;
		const auto  curl = [&](ImVec2 a_at, float a_heading, float a_turn) {
			const auto path = Walk(a_at, a_heading, length, 0.0f, length, 0.0f, a_turn);
			for (std::size_t i = 1; i < path.size(); ++i) {
				const float t = static_cast<float>(i) / static_cast<float>(path.size());
				a_drawList->AddLine(path[i - 1], path[i], Mix(hideLit, rule, t), std::max(1.0f, thick * (1.0f - t)));
			}
		};
		curl(a_min, 0.0f, 3.2f);
		curl(a_min, pi * 0.5f, -3.2f);
		curl(ImVec2(a_max.x, a_min.y), pi, -3.2f);
		curl(ImVec2(a_max.x, a_min.y), pi * 0.5f, 3.2f);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Perks ) ----------------------
	//---------------------------------------------------

	void PerkNode(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, PerkState a_state) {

		switch (a_state) {
		case PerkState::kTaken:
			a_drawList->AddCircleFilled(a_centre, a_radius * 1.1f, ink, 20);
			a_drawList->AddCircle(a_centre, a_radius * 1.1f, acid, 20, std::max(1.2f, a_radius * 0.12f));
			Marker(a_drawList, a_centre, a_radius * 1.45f, acid, 1.0f);
			return;
		case PerkState::kNext:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.9f, ink, 20);
			a_drawList->AddCircle(a_centre, a_radius * 0.9f, a_colour, 20, std::max(1.2f, a_radius * 0.1f));
			Marker(a_drawList, a_centre, a_radius * 1.15f, a_colour, 0.55f);
			return;
		default:
			a_drawList->AddCircleFilled(a_centre, a_radius * 0.55f, ink, 14);
			Marker(a_drawList, ImVec2(a_centre.x, a_centre.y - a_radius * 0.15f), a_radius * 0.8f, a_colour, 0.0f);
			return;
		}
	}

	void PerkLink(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, bool a_lit, float a_radius) {

		const auto points = PerkCurve(a_from, a_to, a_radius * 0.5f);
		const float thick = std::max(1.0f, a_radius * (a_lit ? 0.22f : 0.1f));
		const std::size_t count = points.size();
		for (std::size_t i = 1; i < count; ++i) {
			const float t = static_cast<float>(i) / static_cast<float>(count);
			a_drawList->AddLine(points[i - 1], points[i], a_lit ? Mix(hideLit, acid, t) : a_colour, thick * (1.2f - t * 0.6f));
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Plaque ) ---------------------
	//---------------------------------------------------

	// The cover of a Black Book, tooled in green; focused it has to read from across a room, so it floods
	void Plaque(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_glow, bool a_pressed) {

		const float size = ImGui::GetFontSize();
		const float round = size * 0.1f;
		const ImU32 top = Mix(IM_COL32(32, 38, 30, 255), IM_COL32(88, 136, 56, 255), a_glow);
		const ImU32 bottom = Mix(ink, IM_COL32(44, 78, 34, 255), a_glow);
		if (a_pressed) {
			a_drawList->AddRectFilledMultiColor(a_min, a_max, bottom, bottom, top, top);
		} else {
			a_drawList->AddRectFilledMultiColor(a_min, a_max, top, top, bottom, bottom);
		}
		a_drawList->AddRect(a_min, a_max, Mix(rule, acid, a_glow), round, 0, std::max(1.0f, size * (0.07f + 0.05f * a_glow)));
		const float in = std::max(2.0f, size * 0.14f);
		a_drawList->AddRect(ImVec2(a_min.x + in, a_min.y + in), ImVec2(a_max.x - in, a_max.y - in), WithAlpha(Mix(rule, ink, a_glow), 0.55f));
		// Each corner clasped by a small claw
		const float claw = size * 0.3f;
		for (const auto& corner : { ImVec4(a_min.x, a_min.y, 1.0f, 1.0f), ImVec4(a_max.x, a_min.y, -1.0f, 1.0f), ImVec4(a_max.x, a_max.y, -1.0f, -1.0f), ImVec4(a_min.x, a_max.y, 1.0f, -1.0f) }) {
			a_drawList->AddTriangleFilled(ImVec2(corner.x, corner.y), ImVec2(corner.x + corner.z * claw, corner.y), ImVec2(corner.x, corner.y + corner.w * claw), Mix(rule, acid, a_glow));
		}
	}

	void Pill(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRectFilled(a_min, a_max, WithAlpha(ink, 0.85f), size * 0.1f);
		a_drawList->AddRect(a_min, a_max, rule, size * 0.1f, 0, std::max(1.0f, size * 0.06f));
	}

	void Card(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit) {

		const float size = ImGui::GetFontSize();
		const ImU32 top = a_lit ? IM_COL32(40, 66, 32, 235) : IM_COL32(20, 26, 21, 225);
		const ImU32 bottom = a_lit ? IM_COL32(20, 36, 18, 235) : IM_COL32(9, 13, 10, 225);
		a_drawList->AddRectFilledMultiColor(a_min, a_max, top, top, bottom, bottom);
		const ImU32 line = a_lit ? acid : rule;
		a_drawList->AddRect(a_min, a_max, line, size * 0.08f, 0, std::max(1.0f, size * (a_lit ? 0.09f : 0.05f)));
		const float claw = size * 0.42f;
		for (const auto& corner : { ImVec4(a_min.x, a_min.y, 1.0f, 1.0f), ImVec4(a_max.x, a_min.y, -1.0f, 1.0f), ImVec4(a_max.x, a_max.y, -1.0f, -1.0f), ImVec4(a_min.x, a_max.y, 1.0f, -1.0f) }) {
			a_drawList->AddTriangleFilled(ImVec2(corner.x, corner.y), ImVec2(corner.x + corner.z * claw, corner.y), ImVec2(corner.x, corner.y + corner.w * claw), line);
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Check ) ----------------------
	//---------------------------------------------------

	void Check(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_fill) {

		const float half = a_size * 0.46f;
		a_drawList->AddRectFilled(ImVec2(a_centre.x - half, a_centre.y - half), ImVec2(a_centre.x + half, a_centre.y + half), WithAlpha(ink, 0.9f), a_size * 0.08f);
		a_drawList->AddRect(ImVec2(a_centre.x - half, a_centre.y - half), ImVec2(a_centre.x + half, a_centre.y + half), Mix(rule, acid, a_fill), a_size * 0.08f, 0, std::max(1.0f, a_size * 0.08f));
		Marker(a_drawList, ImVec2(a_centre.x, a_centre.y - (a_fill > 0.02f ? 0.0f : a_size * 0.14f)), a_size * 0.68f, a_fill > 0.02f ? acid : rule, a_fill);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Highlight ) ------------------
	//---------------------------------------------------

	// A wet smear of green, its edges bled like ink on bad paper
	void Highlight(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float width = std::max(1.0f, a_max.x - a_min.x);
		const float seed = a_min.y * 0.037f;
		Mesh(a_drawList, a_min, a_max, size * 0.7f, std::max(2.0f, height * 0.25f), [&](float a_x, float a_y) {
			const float along = (a_x - a_min.x) / width;
			const float across = 1.0f - std::abs((a_y - a_min.y) / height * 2.0f - 1.0f);
			const float bleed = ValueNoise(a_x / (size * 1.6f) + seed, a_y / size) * 0.5f;
			const float core = Smooth(across * 2.4f - bleed * 0.4f);
			const float fade = 1.0f - along * along * 0.72f;
			return WithAlpha(Mix(IM_COL32(62, 106, 42, 255), IM_COL32(38, 68, 30, 255), along), a_alpha * core * fade * 0.88f);
		});
		const float pen = std::max(2.0f, size * 0.13f);
		a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + pen, a_max.y), WithAlpha(acid, a_alpha), WithAlpha(acid, a_alpha * 0.5f), WithAlpha(acid, a_alpha * 0.5f), WithAlpha(acid, a_alpha));
		a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, a_max.y - 1.0f), ImVec2(a_min.x + width * 0.75f, a_max.y), WithAlpha(acid, a_alpha * 0.7f), WithAlpha(acid, 0.0f), WithAlpha(acid, 0.0f), WithAlpha(acid, a_alpha * 0.7f));
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Toast Plate ) ----------------
	//---------------------------------------------------

	// A scrap of marbled ink with a tentacle up its side and another feeling along the top
	void ToastPlate(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_alpha, float a_opacity, std::uint32_t a_seed) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float seed = static_cast<float>(a_seed % 977u);
		const float fill = a_alpha * std::max(a_opacity, 0.6f);
		Mesh(a_drawList, a_min, a_max, size * 0.8f, size * 0.6f, [&](float a_x, float a_y) {
			const float warp = Fbm(a_x / (size * 7.0f) + seed, a_y / (size * 4.0f), 2);
			const float band = 1.0f - std::abs(Fbm(a_x / (size * 5.0f) + warp * 2.5f, a_y / (size * 2.5f) + seed, 2) * 2.0f - 1.0f);
			return WithAlpha(Mix(ink, IM_COL32(28, 48, 30, 255), Smooth(band * band * 1.3f - 0.25f)), fill);
		});
		a_drawList->AddRect(a_min, a_max, WithAlpha(rule, a_alpha), size * 0.08f, 0, std::max(1.0f, size * 0.06f));

		const float phase = seed * 0.37f;
		Tentacle(a_drawList, Walk(ImVec2(a_min.x + size * 0.14f, a_max.y + size * 0.1f), -pi * 0.5f, height + size * 1.5f, 0.07f, size * 4.0f, phase, 2.1f), size * 0.62f, 1.0f, a_seed | 1u, a_alpha);
		Tentacle(a_drawList, Walk(ImVec2(a_max.x + size * 0.1f, a_min.y + size * 0.12f), pi, size * 4.2f, 0.06f, size * 3.0f, phase + 1.7f, -2.4f), size * 0.42f, -1.0f, a_seed | 2u, a_alpha);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Swatch Rim ) -----------------
	//---------------------------------------------------

	// The colour inside is the player's own, so only the rim round it is themed
	void SwatchRim(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_lit) {

		const float size = ImGui::GetFontSize();
		const ImU32 line = a_lit ? acid : rule;
		const float grow = std::max(1.0f, size * 0.07f);
		a_drawList->AddRect(a_min, a_max, ink, 0.0f, 0, std::max(1.0f, size * 0.08f));
		a_drawList->AddRect(ImVec2(a_min.x - grow, a_min.y - grow), ImVec2(a_max.x + grow, a_max.y + grow), line, 0.0f, 0, std::max(1.0f, size * 0.05f));
		const float claw = std::min(a_max.y - a_min.y, size) * 0.3f;
		for (const auto& corner : { ImVec4(a_min.x - grow, a_min.y - grow, 1.0f, 1.0f), ImVec4(a_max.x + grow, a_min.y - grow, -1.0f, 1.0f), ImVec4(a_max.x + grow, a_max.y + grow, -1.0f, -1.0f), ImVec4(a_min.x - grow, a_max.y + grow, 1.0f, -1.0f) }) {
			a_drawList->AddTriangleFilled(ImVec2(corner.x, corner.y), ImVec2(corner.x + corner.z * claw, corner.y), ImVec2(corner.x, corner.y + corner.w * claw), line);
		}
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Tooltip ) --------------------
	//---------------------------------------------------

	void TooltipMarks(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		a_drawList->AddRect(a_min, a_max, rule, 0.0f, 0, std::max(1.0f, size * 0.06f));
		PanelCorners(a_drawList, a_min, a_max);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Title Eyes ) -----------------
	//---------------------------------------------------

	void TitleEyes(ImDrawList* a_drawList, ImVec2 a_titleMin, ImVec2 a_titleMax, float a_room) {

		const float height = a_titleMax.y - a_titleMin.y;
		const float radius = std::min(a_room * 0.34f, height * 0.56f);
		const float midY = (a_titleMin.y + a_titleMax.y) * 0.52f;
		WatchingEye(a_drawList, ImVec2(a_titleMin.x - a_room * 0.5f, midY), radius, 0xC131u);
		WatchingEye(a_drawList, ImVec2(a_titleMax.x + a_room * 0.5f, midY), radius, 0xC132u);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Window Header ) --------------
	//---------------------------------------------------

	void WindowHeader(ImDrawList* a_drawList, float a_top, float a_bottom, float a_left, float a_right) {

		const float size = ImGui::GetFontSize();
		const float room = a_right - a_left;
		const float height = a_bottom - a_top;
		if (room < size * 6.0f || height < size) {
			return;
		}
		const ImVec2 min(a_left, a_top);
		const ImVec2 max(a_right, a_bottom);
		const float  mid = (a_left + a_right) * 0.5f;
		Baked(a_drawList, 0x0C30u, min, max, [&](ImDrawList* a_list) {
			const float y = a_bottom - height * 0.34f;
			const float reach = room * 0.42f;
			Tentacle(a_list, Walk(ImVec2(mid - size * 0.8f, y), pi, reach, 0.12f, size * 5.0f, 1.3f, 3.6f), size * 1.1f, 1.0f, 0xC301u);
			Tentacle(a_list, Walk(ImVec2(mid + size * 0.8f, y), 0.0f, reach, 0.12f, size * 5.0f, 2.9f, -3.6f), size * 1.1f, -1.0f, 0xC302u);
		});
		WatchingEye(a_drawList, ImVec2(mid, (a_top + a_bottom) * 0.5f), std::min(height * 0.5f, size * 1.6f), 0xC303u);
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Creep ) ----------------------
	//---------------------------------------------------

	void Creep(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		const float size = ImGui::GetFontSize();
		const float height = a_max.y - a_min.y;
		const float width = a_max.x - a_min.x;
		if (height < size * 11.0f || width < size * 16.0f) {
			return;
		}
		Baked(a_drawList, 0x0C40u, a_min, a_max, [&](ImDrawList* a_list) {
			// Thin and flat along the very foot, under where the rows end, their tips tucked out of sight below it
			const float low = a_max.y - size * 0.2f;
			Tentacle(a_list, Walk(ImVec2(a_max.x + size * 0.3f, low), pi, size * 7.5f, 0.03f, size * 5.0f, 0.6f, -1.1f), size * 0.42f, 1.0f, 0xC401u, 0.8f);
			Tentacle(a_list, Walk(ImVec2(a_min.x - size * 0.3f, low), 0.0f, size * 6.0f, 0.03f, size * 5.0f, 2.4f, 1.1f), size * 0.42f, -1.0f, 0xC402u, 0.8f);
		});
	}

	//---------------------------------------------------
	//-- Apocrypha Decor ( Panel Strip ) ----------------
	//---------------------------------------------------

	void TitleWatcher(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius) {

		WatchingEye(a_drawList, a_centre, a_radius, 0xC504u);
	}
}
