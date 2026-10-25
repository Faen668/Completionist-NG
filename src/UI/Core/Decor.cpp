#include "UI/Core/Decor.hpp"
#include "UI/Core/Themes.hpp"
#include "UI/Themes/AltmerDecor.hpp"
#include "UI/Themes/ArgonianDecor.hpp"
#include "UI/Themes/BosmerDecor.hpp"
#include "UI/Themes/DecorDraw.hpp"
#include "UI/Themes/DunmerDecor.hpp"
#include "UI/Themes/KhajiitDecor.hpp"
#include "UI/Themes/NordDecor.hpp"
#include "UI/Themes/SkyUIDecor.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <unordered_map>

// Routes each ornament to the current theme (UI/Themes); Completionist's own diamonds and faded lines are the base
namespace CompletionistUI
{
	using namespace DecorDraw;

	namespace
	{

		//---------------------------------------------------
		//-- Decor ( Accent ) -------------------------------
		//---------------------------------------------------

		ImU32 Accent(float a_alpha = 1.0f) {

			return WithAlpha(ImGui::GetColorU32(Themes::GetPalette().accent), a_alpha);
		}

		//---------------------------------------------------
		//-- Decor ( Tail Seed ) ----------------------------
		//---------------------------------------------------

		// From the length, so the pattern stays put while scrolling
		unsigned TailSeed(ImVec2 a_from, ImVec2 a_to) {

			return static_cast<unsigned>(a_to.x - a_from.x) + 1u;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Diamond ) ------------------------------
	//---------------------------------------------------

	void Decor::Diamond(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour, bool a_filled) {

		switch (_theme) {
		case Theme::kArgonian:
			ArgonianDecor::Scale(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		case Theme::kBosmer:
			BosmerDecor::Diamond(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		case Theme::kDunmer:
			DunmerDecor::Flame(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Crescent(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		case Theme::kAltmer:
			AltmerDecor::Crystal(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		case Theme::kNord:
			NordDecor::Star(a_drawList, a_centre, a_radius, a_colour, a_filled);
			return;
		default:
			break;
		}

		const ImVec2 points[4] = {
			ImVec2(a_centre.x, a_centre.y - a_radius),
			ImVec2(a_centre.x + a_radius, a_centre.y),
			ImVec2(a_centre.x, a_centre.y + a_radius),
			ImVec2(a_centre.x - a_radius, a_centre.y),
		};

		if (a_filled) {
			a_drawList->AddConvexPolyFilled(points, 4, a_colour);
		} else {
			a_drawList->AddPolyline(points, 4, a_colour, ImDrawFlags_Closed, std::max(1.0f, a_radius * 0.22f));
		}
	}

	//---------------------------------------------------
	//-- Decor ( Marker ) -------------------------------
	//---------------------------------------------------

	// Needed / found marker, a_fill 0 to 1 so toggles can animate
	void Decor::Marker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::Marker(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Egg(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kBosmer:
			BosmerDecor::Marker(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kDunmer:
			DunmerDecor::Ember(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Phase(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kAltmer:
			AltmerDecor::Shard(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		case Theme::kNord:
			NordDecor::RuneStone(a_drawList, a_centre, a_size, a_colour, a_fill);
			return;
		default:
			break;
		}

		const float radius = a_size * 0.30f;
		Diamond(a_drawList, a_centre, radius, a_colour, false);
		if (a_fill > 0.0f) {
			Diamond(a_drawList, a_centre, radius * 0.62f * a_fill, a_colour, true);
		}
	}

	//---------------------------------------------------
	//-- Decor ( Faded Line ) ---------------------------
	//---------------------------------------------------

	void Decor::FadedLine(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour, float a_thickness) {

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float top = a_from.y - a_thickness * 0.5f;
		const float bottom = a_from.y + a_thickness * 0.5f;
		const ImU32 clear = WithAlpha(a_colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(ImVec2(a_from.x, top), ImVec2(midX, bottom), clear, a_colour, a_colour, clear);
		a_drawList->AddRectFilledMultiColor(ImVec2(midX, top), ImVec2(a_to.x, bottom), a_colour, clear, clear, a_colour);
	}

	//---------------------------------------------------
	//-- Decor ( Divider ) ------------------------------
	//---------------------------------------------------

	void Decor::Divider(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::Divider(a_drawList, a_from, a_to, a_colour);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Divider(a_drawList, a_from, a_to, Accent(0.8f));
			return;
		case Theme::kBosmer:
			BosmerDecor::Divider(a_drawList, a_from, a_to, a_colour);
			return;
		case Theme::kDunmer:
			DunmerDecor::Divider(a_drawList, a_from, a_to, Accent());
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Divider(a_drawList, a_from, a_to, Accent(0.8f));
			return;
		case Theme::kAltmer:
			AltmerDecor::Divider(a_drawList, a_from, a_to, Accent(0.8f));
			return;
		case Theme::kNord:
			NordDecor::Divider(a_drawList, a_from, a_to, Accent(0.8f));
			return;
		default:
			break;
		}

		const float midX = (a_from.x + a_to.x) * 0.5f;
		const float gap = ImGui::GetFontSize() * 0.6f;
		FadedLine(a_drawList, a_from, ImVec2(midX - gap, a_to.y), a_colour);
		FadedLine(a_drawList, ImVec2(midX + gap, a_from.y), a_to, a_colour);
		Diamond(a_drawList, ImVec2(midX, a_from.y), gap * 0.45f, a_colour, true);
	}

	//---------------------------------------------------
	//-- Decor ( Knot ) ---------------------------------
	//---------------------------------------------------

	void Decor::Knot(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		SkyUIDecor::Knot(a_drawList, a_centre, a_radius, a_colour);
	}

	//---------------------------------------------------
	//-- Decor ( Double Rule ) --------------------------
	//---------------------------------------------------

	void Decor::DoubleRule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		SkyUIDecor::DoubleRule(a_drawList, a_from, a_to, a_colour);
	}

	//---------------------------------------------------
	//-- Decor ( Ornament ) -----------------------------
	//---------------------------------------------------

	void Decor::Ornament(ImDrawList* a_drawList, ImVec2 a_centre, float a_radius, ImU32 a_colour) {

		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::Knot(a_drawList, a_centre, a_radius * 2.4f, a_colour);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Eye(a_drawList, a_centre, a_radius * 1.05f, a_colour);
			return;
		case Theme::kBosmer:
			BosmerDecor::Sprig(a_drawList, ImVec2(a_centre.x, a_centre.y + a_radius * 0.4f), a_radius * 1.5f, -std::numbers::pi_v<float> * 0.5f, a_colour);
			return;
		case Theme::kDunmer:
			DunmerDecor::Triangle(a_drawList, a_centre, a_radius * 1.6f, a_colour);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::TwinMoons(a_drawList, a_centre, a_radius * 1.7f);
			return;
		case Theme::kAltmer:
			AltmerDecor::Gem(a_drawList, a_centre, a_radius * 1.5f, a_colour);
			return;
		case Theme::kNord:
			NordDecor::Knot(a_drawList, a_centre, a_radius * 1.15f, a_colour);
			return;
		default:
			Diamond(a_drawList, a_centre, a_radius, a_colour, true);
			return;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Rule ) ---------------------------------
	//---------------------------------------------------

	void Decor::Rule(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_colour) {

		const float size = ImGui::GetFontSize();
		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::DoubleRule(a_drawList, a_from, a_to, a_colour);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Ridge(a_drawList, a_from, a_to, Accent(0.75f), size * 0.6f);
			return;
		case Theme::kBosmer:
			BosmerDecor::Vine(a_drawList, a_from, a_to, a_colour, Accent(), size * 0.55f, TailSeed(a_from, a_to));
			return;
		case Theme::kDunmer:
			DunmerDecor::Lava(a_drawList, a_from, a_to, Accent(), size * 0.6f);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Thread(a_drawList, a_from, a_to, Accent(0.8f), size * 0.6f);
			return;
		case Theme::kAltmer:
			AltmerDecor::Filigree(a_drawList, a_from, a_to, Accent(0.8f), size * 0.6f);
			return;
		case Theme::kNord:
			NordDecor::Knotwork(a_drawList, a_from, a_to, Accent(0.8f), size * 0.6f);
			return;
		default:
			FadedLine(a_drawList, a_from, a_to, a_colour);
			return;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Heading Tail ) -------------------------
	//---------------------------------------------------

	void Decor::HeadingTail(ImDrawList* a_drawList, ImVec2 a_from, ImVec2 a_to, ImU32 a_ornament, ImU32 a_rule) {

		const float size = ImGui::GetFontSize();
		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::HeadingTail(a_drawList, a_from, a_to, a_ornament, a_rule);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Ridge(a_drawList, a_from, a_to, WithAlpha(a_ornament, 0.75f), size * 0.5f);
			return;
		case Theme::kBosmer:
			BosmerDecor::Vine(a_drawList, a_from, a_to, a_rule, a_ornament, size * 0.5f, TailSeed(a_from, a_to));
			return;
		case Theme::kDunmer:
			DunmerDecor::Lava(a_drawList, a_from, a_to, a_ornament, size * 0.5f);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Thread(a_drawList, a_from, a_to, WithAlpha(a_ornament, 0.75f), size * 0.5f);
			return;
		case Theme::kAltmer:
			AltmerDecor::Filigree(a_drawList, a_from, a_to, WithAlpha(a_ornament, 0.75f), size * 0.5f);
			return;
		case Theme::kNord:
			NordDecor::Knotwork(a_drawList, a_from, a_to, WithAlpha(a_ornament, 0.75f), size * 0.5f);
			return;
		default:
			FadedLine(a_drawList, a_from, a_to, a_rule);
			return;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Banner ) -------------------------------
	//---------------------------------------------------

	void Decor::Banner(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_fill, ImU32 a_edge) {

		SkyUIDecor::Banner(a_drawList, a_min, a_max, a_fill, a_edge);
	}

	//---------------------------------------------------
	//-- Decor ( Frame ) --------------------------------
	//---------------------------------------------------

	void Decor::Frame(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour) {

		switch (_theme) {
		case Theme::kSkyUI:
			SkyUIDecor::Frame(a_drawList, a_min, a_max, a_colour);
			return;
		case Theme::kArgonian:
			ArgonianDecor::Frame(a_drawList, a_min, a_max);
			return;
		case Theme::kBosmer:
			BosmerDecor::Frame(a_drawList, a_min, a_max);
			return;
		case Theme::kDunmer:
			DunmerDecor::Frame(a_drawList, a_min, a_max);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::Frame(a_drawList, a_min, a_max);
			return;
		case Theme::kAltmer:
			AltmerDecor::Frame(a_drawList, a_min, a_max);
			return;
		case Theme::kNord:
			NordDecor::Frame(a_drawList, a_min, a_max);
			return;
		default:
			break;
		}

		// Two rules, diamonds in the corners
		const float inset = ImGui::GetFontSize() * 0.3f;
		const float corner = ImGui::GetFontSize() * 0.35f;
		a_drawList->AddRect(a_min, a_max, WithAlpha(a_colour, 0.35f), 0.0f, 0, 1.0f);
		a_drawList->AddRect(ImVec2(a_min.x + inset, a_min.y + inset), ImVec2(a_max.x - inset, a_max.y - inset), a_colour, 0.0f, 0, 1.0f);
		for (const auto& point : { a_min, ImVec2(a_max.x, a_min.y), a_max, ImVec2(a_min.x, a_max.y) }) {
			const float x = point.x == a_min.x ? a_min.x + inset : a_max.x - inset;
			const float y = point.y == a_min.y ? a_min.y + inset : a_max.y - inset;
			Diamond(a_drawList, ImVec2(x, y), corner, a_colour, true);
		}
	}

	//---------------------------------------------------
	//-- Decor ( Frame Depth ) --------------------------
	//---------------------------------------------------

	float Decor::FrameDepth() noexcept {

		const float size = ImGui::GetFontSize();
		switch (_theme) {
		case Theme::kBosmer:
			return size * 0.45f;
		case Theme::kArgonian:
		case Theme::kDunmer:
		case Theme::kKhajiit:
		case Theme::kAltmer:
		case Theme::kNord:
			return size * 0.6f;
		default:
			return 0.0f;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Panel ) --------------------------------
	//---------------------------------------------------

	bool Decor::Panel(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, bool a_emblem) {

		switch (_theme) {
		case Theme::kArgonian:
			ArgonianDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		case Theme::kBosmer:
			BosmerDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		case Theme::kDunmer:
			DunmerDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		case Theme::kKhajiit:
			KhajiitDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		case Theme::kAltmer:
			AltmerDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		case Theme::kNord:
			NordDecor::Panel(a_drawList, a_min, a_max, a_emblem);
			return true;
		default:
			return false;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Panel Corners ) ------------------------
	//---------------------------------------------------

	void Decor::PanelCorners(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max) {

		switch (_theme) {
		case Theme::kArgonian:
			ArgonianDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		case Theme::kBosmer:
			BosmerDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		case Theme::kDunmer:
			DunmerDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		case Theme::kKhajiit:
			KhajiitDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		case Theme::kAltmer:
			AltmerDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		case Theme::kNord:
			NordDecor::PanelCorners(a_drawList, a_min, a_max);
			return;
		default:
			return;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Glow Bar ) -----------------------------
	//---------------------------------------------------

	void Decor::GlowBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, ImU32 a_colour, float a_alpha) {

		if (a_alpha <= 0.01f) {
			return;
		}

		if (SkyUI()) {
			SkyUIDecor::GlowBar(a_drawList, a_min, a_max, a_colour, a_alpha);
			return;
		}

		if (Race()) {
			// Rising out of the shadow, no hard edges
			const ImU32 moss = Altmer() ? WithAlpha(a_colour, 0.45f * a_alpha) : WithAlpha(Shade(a_colour, 0.6f), 0.55f * a_alpha);	// Altmer is light, shading it would turn it brown
			const ImU32 none = WithAlpha(a_colour, 0.0f);
			const float midY = (a_min.y + a_max.y) * 0.5f;
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_max.x, midY), none, none, none, moss);
			a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, midY), a_max, moss, none, none, none);
			const ImU32 edge = WithAlpha(a_colour, 0.7f * a_alpha);
			a_drawList->AddRectFilledMultiColor(a_min, ImVec2(a_min.x + 2.0f, midY), none, none, edge, edge);
			a_drawList->AddRectFilledMultiColor(ImVec2(a_min.x, midY), ImVec2(a_min.x + 2.0f, a_max.y), edge, edge, none, none);
			return;
		}

		const ImU32 strong = WithAlpha(a_colour, 0.22f * a_alpha);
		const ImU32 clear = WithAlpha(a_colour, 0.0f);
		a_drawList->AddRectFilledMultiColor(a_min, a_max, strong, clear, clear, strong);
		a_drawList->AddRectFilled(a_min, ImVec2(a_min.x + 2.0f, a_max.y), WithAlpha(a_colour, a_alpha));
	}

	//---------------------------------------------------
	//-- Decor ( Draw Icon ) ----------------------------
	//---------------------------------------------------

	void Decor::DrawIcon(ImDrawList* a_drawList, Icon a_icon, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		const float r = a_size * 0.5f;
		const float stroke = std::max(1.0f, a_size * 0.08f);

		switch (a_icon) {
		case Icon::kOverview:
			{
				a_drawList->AddCircle(a_centre, r * 0.9f, a_colour, 0, stroke);
				Diamond(a_drawList, a_centre, r * 0.45f, a_colour, true);
			}
			break;
		case Icon::kCollections:
			{
				// Gem
				const ImVec2 points[6] = {
					ImVec2(a_centre.x - r * 0.9f, a_centre.y - r * 0.25f),
					ImVec2(a_centre.x - r * 0.45f, a_centre.y - r * 0.8f),
					ImVec2(a_centre.x + r * 0.45f, a_centre.y - r * 0.8f),
					ImVec2(a_centre.x + r * 0.9f, a_centre.y - r * 0.25f),
					ImVec2(a_centre.x, a_centre.y + r * 0.9f),
					ImVec2(a_centre.x - r * 0.9f, a_centre.y - r * 0.25f),
				};
				a_drawList->AddPolyline(points, 6, a_colour, ImDrawFlags_None, stroke);
				a_drawList->AddLine(ImVec2(a_centre.x - r * 0.9f, a_centre.y - r * 0.25f), ImVec2(a_centre.x + r * 0.9f, a_centre.y - r * 0.25f), a_colour, stroke);
				a_drawList->AddLine(ImVec2(a_centre.x - r * 0.3f, a_centre.y - r * 0.25f), ImVec2(a_centre.x, a_centre.y + r * 0.9f), a_colour, stroke);
				a_drawList->AddLine(ImVec2(a_centre.x + r * 0.3f, a_centre.y - r * 0.25f), ImVec2(a_centre.x, a_centre.y + r * 0.9f), a_colour, stroke);
			}
			break;
		case Icon::kLocations:
			{
				// Map pin
				const ImVec2 head(a_centre.x, a_centre.y - r * 0.25f);
				a_drawList->PathArcTo(head, r * 0.55f, 2.6f, 6.82f);
				a_drawList->PathLineTo(ImVec2(a_centre.x, a_centre.y + r * 0.9f));
				a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, stroke);
				a_drawList->AddCircleFilled(head, r * 0.18f, a_colour);
			}
			break;
		case Icon::kPatches:
			{
				// Scroll
				const float w = r * 0.75f;
				const float h = r * 0.85f;
				a_drawList->AddRect(ImVec2(a_centre.x - w, a_centre.y - h), ImVec2(a_centre.x + w, a_centre.y + h), a_colour, 0.0f, 0, stroke);
				a_drawList->AddLine(ImVec2(a_centre.x - w * 1.25f, a_centre.y - h), ImVec2(a_centre.x + w * 1.25f, a_centre.y - h), a_colour, stroke * 1.6f);
				a_drawList->AddLine(ImVec2(a_centre.x - w * 1.25f, a_centre.y + h), ImVec2(a_centre.x + w * 1.25f, a_centre.y + h), a_colour, stroke * 1.6f);
				for (int line = -1; line <= 1; ++line) {
					const float y = a_centre.y + line * h * 0.4f;
					a_drawList->AddLine(ImVec2(a_centre.x - w * 0.55f, y), ImVec2(a_centre.x + w * 0.55f, y), a_colour, stroke * 0.7f);
				}
			}
			break;
		case Icon::kQuests:
			{
				// Shield
				a_drawList->PathLineTo(ImVec2(a_centre.x - r * 0.75f, a_centre.y - r * 0.8f));
				a_drawList->PathLineTo(ImVec2(a_centre.x + r * 0.75f, a_centre.y - r * 0.8f));
				a_drawList->PathLineTo(ImVec2(a_centre.x + r * 0.75f, a_centre.y));
				a_drawList->PathLineTo(ImVec2(a_centre.x, a_centre.y + r * 0.9f));
				a_drawList->PathLineTo(ImVec2(a_centre.x - r * 0.75f, a_centre.y));
				a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, stroke);
				Diamond(a_drawList, ImVec2(a_centre.x, a_centre.y - r * 0.1f), r * 0.28f, a_colour, true);
			}
			break;
		case Icon::kSettings:
			{
				// Cog
				for (int tooth = 0; tooth < 8; ++tooth) {
					const float angle = tooth * 3.14159265f / 4.0f;
					const ImVec2 inner(a_centre.x + std::cos(angle) * r * 0.55f, a_centre.y + std::sin(angle) * r * 0.55f);
					const ImVec2 outer(a_centre.x + std::cos(angle) * r * 0.9f, a_centre.y + std::sin(angle) * r * 0.9f);
					a_drawList->AddLine(inner, outer, a_colour, stroke * 1.8f);
				}
				a_drawList->AddCircle(a_centre, r * 0.55f, a_colour, 0, stroke);
				a_drawList->AddCircle(a_centre, r * 0.22f, a_colour, 0, stroke);
			}
			break;
		}
	}

	//---------------------------------------------------
	//-- Decor ( Icon For ) -----------------------------
	//---------------------------------------------------

	Icon Decor::IconFor(std::string_view a_categoryKey) noexcept {

		if (a_categoryKey.find("Location") != std::string_view::npos) {
			return Icon::kLocations;
		}
		if (a_categoryKey.find("Patch") != std::string_view::npos) {
			return Icon::kPatches;
		}
		if (a_categoryKey.find("Quest") != std::string_view::npos) {
			return Icon::kQuests;
		}
		return Icon::kCollections;
	}

	//---------------------------------------------------
	//-- Decor ( Animate ) ------------------------------
	//---------------------------------------------------

	float Decor::Animate(ImGuiID a_id, bool a_active, float a_speed) {

		static std::unordered_map<ImGuiID, float> values;
		auto&                                     value = values[a_id];
		const float                               step = ImGui::GetIO().DeltaTime * a_speed;
		value = a_active ? std::min(1.0f, value + step) : std::max(0.0f, value - step);
		return value;
	}
}
