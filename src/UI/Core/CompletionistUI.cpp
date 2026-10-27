#include "UI/Core/CompletionistUI.hpp"
#include "UI/Core/Decor.hpp"
#include "UI/Core/SettingsLayout.hpp"
#include "UI/Core/Themes.hpp"

#include <imgui_internal.h>

#include <algorithm>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iterator>
#include <numbers>
#include <ctime>

namespace CompletionistUI
{
	namespace
	{

		//---------------------------------------------------
		//-- Completionist UI ( Sky UI ) --------------------
		//---------------------------------------------------

		// SkyUI theme swaps the gold diamonds for knots and double rules
		bool SkyUI() {

			return Themes::GetTheme() == Theme::kSkyUI;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Fraction ) ------------------
		//---------------------------------------------------

		float Fraction(std::int32_t a_found, std::int32_t a_total) {

			return a_total > 0 ? static_cast<float>(a_found) / static_cast<float>(a_total) : 0.0f;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Lower ) ---------------------
		//---------------------------------------------------

		std::string Lower(std::string_view a_text) {

			std::string lower(a_text);
			std::ranges::transform(lower, lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return lower;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Upper ) ---------------------
		//---------------------------------------------------

		// Upper case for ASCII, Latin-1, Latin Extended-A and Cyrillic
		std::string Upper(std::string_view a_text) {

			std::string out;
			out.reserve(a_text.size());
			const auto put = [&](char32_t a_code) {
				if (a_code < 0x80) {
					out += static_cast<char>(a_code);
				} else if (a_code < 0x800) {
					out += static_cast<char>(0xC0 | (a_code >> 6));
					out += static_cast<char>(0x80 | (a_code & 0x3F));
				} else {
					out += static_cast<char>(0xE0 | (a_code >> 12));
					out += static_cast<char>(0x80 | ((a_code >> 6) & 0x3F));
					out += static_cast<char>(0x80 | (a_code & 0x3F));
				}
			};
			for (std::size_t i = 0; i < a_text.size();) {
				const auto lead = static_cast<unsigned char>(a_text[i]);
				const std::size_t length = lead < 0x80 ? 1 : (lead >> 5) == 0x6 ? 2 : (lead >> 4) == 0xE ? 3 : (lead >> 3) == 0x1E ? 4 : 1;
				if (length > 3 || i + length > a_text.size()) {
					out.append(a_text.substr(i, std::min(length, a_text.size() - i)));
					i += length;
					continue;
				}
				char32_t code = length == 1 ? lead : length == 2 ? (lead & 0x1F) : (lead & 0x0F);
				for (std::size_t k = 1; k < length; ++k) {
					code = (code << 6) | (static_cast<unsigned char>(a_text[i + k]) & 0x3F);
				}
				i += length;
				if (code >= 'a' && code <= 'z') {
					code -= 0x20;
				} else if (code >= 0xE0 && code <= 0xFE && code != 0xF7) {
					code -= 0x20;
				} else if (code >= 0x100 && code <= 0x17F) {
					// Pairs, upper case first except 0x139-0x148 and 0x179-0x17E
					const bool oddUpper = (code >= 0x139 && code <= 0x148) || (code >= 0x179 && code <= 0x17E);
					if (code != 0x131 && code != 0x138 && code != 0x149 && code != 0x17F && (code % 2 == 0) == oddUpper) {
						code -= 1;
					}
				} else if (code >= 0x430 && code <= 0x44F) {
					code -= 0x20;
				} else if (code >= 0x450 && code <= 0x45F) {
					code -= 0x50;
				}
				put(code);
			}
			return out;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Fit Text ) ------------------
		//---------------------------------------------------

		// Trims to fit, with ...
		std::string FitText(const std::string& a_text, float a_width) {

			if (ImGui::CalcTextSize(a_text.c_str()).x <= a_width) {
				return a_text;
			}
			constexpr const char* ellipsis = "\xE2\x80\xA6";
			const float           room = a_width - ImGui::CalcTextSize(ellipsis).x;
			std::size_t           end = a_text.size();
			while (end > 0) {
				do {
					--end;
				} while (end > 0 && (static_cast<unsigned char>(a_text[end]) & 0xC0) == 0x80);
				if (ImGui::CalcTextSize(a_text.c_str(), a_text.c_str() + end).x <= room) {
					break;
				}
			}
			while (end > 0 && a_text[end - 1] == ' ') {
				--end;
			}
			return end > 0 ? a_text.substr(0, end) + ellipsis : std::string{};
		}

		//---------------------------------------------------
		//-- Completionist UI ( Count Text ) ----------------
		//---------------------------------------------------

		std::string CountText(std::int32_t a_found, std::int32_t a_total) {

			char buffer[32];
			std::snprintf(buffer, sizeof(buffer), "%d / %d", a_found, a_total);
			return buffer;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Percent Text ) --------------
		//---------------------------------------------------

		std::string PercentText(std::int32_t a_found, std::int32_t a_total) {

			char buffer[16];
			std::snprintf(buffer, sizeof(buffer), "%d%%", static_cast<int>(Fraction(a_found, a_total) * 100.0f + 0.5f));
			return buffer;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour ) --------------------
		//---------------------------------------------------

		ImU32 Colour(const ImVec4& a_colour, float a_alpha = 1.0f) {

			return ImGui::GetColorU32(ImVec4(a_colour.x, a_colour.y, a_colour.z, a_colour.w * a_alpha));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Inset Colour ) --------------
		//---------------------------------------------------

		// The opacity setting as the open window uses it, so the panels inside go see-through with it
		float windowOpacity = 0.95f;

		// Behind a panel: a bit darker than the window, or a bit whiter on a light theme
		ImVec4 InsetColour() {

			auto       colour = Themes::GetPalette().background;
			const bool light = colour.x * 0.3f + colour.y * 0.59f + colour.z * 0.11f > 0.5f;
			for (float* channel : { &colour.x, &colour.y, &colour.z }) {
				*channel = light ? *channel + (1.0f - *channel) * 0.4f : *channel * 0.72f;
			}
			colour.w = windowOpacity;
			return colour;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Corner Marks ) --------------
		//---------------------------------------------------

		// Diamonds on two corners, not in SkyUI
		void CornerMarks(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_radius, ImU32 a_colour) {

			if (!SkyUI()) {
				Decor::Diamond(a_drawList, a_min, a_radius, a_colour, true);
				Decor::Diamond(a_drawList, a_max, a_radius, a_colour, true);
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( Thin Bar ) ------------------
		//---------------------------------------------------

		void ThinBar(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, float a_fraction) {

			const auto& palette = Themes::GetPalette();
			const auto  fill = a_fraction >= 1.0f ? palette.found : ImGui::GetStyleColorVec4(ImGuiCol_PlotHistogram);
			if (SkyUI()) {
				// SkyUI uses a grey hairline
				a_min.y = std::round(a_max.y) - 1.0f;
				a_max.y = a_min.y + 1.0f;
			}
			const float end = a_min.x + (a_max.x - a_min.x) * a_fraction;
			a_drawList->AddRectFilled(a_min, a_max, Colour(palette.barBackground));
			a_drawList->AddRectFilled(a_min, ImVec2(end, a_max.y), Colour(fill));

			if (!SkyUI() && a_fraction > 0.0f) {
				const float height = a_max.y - a_min.y;
				Decor::Diamond(a_drawList, ImVec2(end, (a_min.y + a_max.y) * 0.5f), std::max(3.0f, height * 1.6f), Colour(fill), true);
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( Progress Bar ) --------------
		//---------------------------------------------------

		void ProgressBar(float a_fraction, float a_height) {

			const float width = ImGui::GetContentRegionAvail().x;
			const float slim = std::max(2.0f, a_height * 0.35f);
			const auto  pos = ImGui::GetCursorScreenPos();
			const float top = pos.y + (a_height - slim) * 0.5f;
			ThinBar(ImGui::GetWindowDrawList(), ImVec2(pos.x + a_height, top), ImVec2(pos.x + width - a_height, top + slim), a_fraction);
			ImGui::Dummy(ImVec2(width, a_height));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Centred Heading ) -----------
		//---------------------------------------------------

		// Title between rules with ornaments, the settings page headings
		void CentredHeading(const std::string& a_text) {

			const auto& palette = Themes::GetPalette();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::PushFont(Themes::HeaderFont());
			const auto  size = ImGui::CalcTextSize(a_text.c_str());
			const auto  pos = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			const float textX = pos.x + (width - size.x) * 0.5f;
			const float lineY = pos.y + size.y * 0.55f;
			const float gap = ImGui::GetFontSize() * 0.8f;
			auto*       drawList = ImGui::GetWindowDrawList();
			drawList->AddText(ImVec2(textX, pos.y), Colour(palette.heading), a_text.c_str());
			ImGui::PopFont();
			const float edge = SkyUI() ? gap * 0.5f : 0.0f;
			Decor::Rule(drawList, ImVec2(pos.x, lineY), ImVec2(textX - gap - edge, lineY), Colour(palette.separator));
			Decor::Rule(drawList, ImVec2(textX + size.x + gap + edge, lineY), ImVec2(pos.x + width, lineY), Colour(palette.separator));
			Decor::Ornament(drawList, ImVec2(textX - gap * 0.5f - edge, lineY), gap * 0.18f, Colour(palette.accent));
			Decor::Ornament(drawList, ImVec2(textX + size.x + gap * 0.5f + edge, lineY), gap * 0.18f, Colour(palette.accent));
			ImGui::Dummy(ImVec2(width, size.y));
			ImGui::Spacing();
		}

		//---------------------------------------------------
		//-- Completionist UI ( Heading With Count ) --------
		//---------------------------------------------------

		void HeadingWithCount(const std::string& a_title, const std::string& a_count, const ImVec4& a_colour) {

			const auto& palette = Themes::GetPalette();
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(a_colour, "%s", a_title.c_str());
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() * 0.15f);
			ImGui::TextColored(palette.textDim, "%s", a_count.c_str());
		}

		//---------------------------------------------------
		//-- Completionist UI ( Draw Marker ) ---------------
		//---------------------------------------------------

		// Theme marker, a_fill 0 to 1 so toggles can animate
		void DrawMarker(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour, float a_fill) {

			Decor::Marker(a_drawList, a_centre, a_size, a_colour, a_fill);
		}

		//---------------------------------------------------
		//-- Completionist UI ( State Colour ) --------------
		//---------------------------------------------------

		ImVec4 StateColour(EntryState a_state) {

			const auto& palette = Themes::GetPalette();
			switch (a_state) {
			case EntryState::kFound:
				return palette.found;
			case EntryState::kDisplayable:
			case EntryState::kDisplayed:
				return palette.displayable;
			case EntryState::kOccupied:
				return palette.occupied;
			default:
				return palette.textDim;
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( State Marker ) --------------
		//---------------------------------------------------

		void StateMarker(EntryState a_state) {

			const float size = ImGui::GetTextLineHeight();
			const float rowHeight = ImGui::GetFrameHeight();
			const auto  pos = ImGui::GetCursorScreenPos();
			const bool  filled = a_state == EntryState::kFound || a_state == EntryState::kDisplayed || a_state == EntryState::kOccupied;
			DrawMarker(ImGui::GetWindowDrawList(), ImVec2(pos.x + size * 0.5f, pos.y + rowHeight * 0.5f), size, Colour(StateColour(a_state)), filled ? 1.0f : 0.0f);
			ImGui::Dummy(ImVec2(size, rowHeight));
		}

		// Controller X arrives as F24, ImGui uses FaceLeft for its own window switching
		constexpr ImGuiKey rowActionKey = ImGuiKey_F24;

		// Records the page controls for controller navigation
		CompletionistUI::Menu* controlRecorder = nullptr;

		//---------------------------------------------------
		//-- Completionist UI ( Note Control ) --------------
		//---------------------------------------------------

		void NoteControl() {

			if (controlRecorder && !(ImGui::GetCurrentWindow()->Flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip))) {
				controlRecorder->RecordControl();
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( Marker Toggle ) -------------
		//---------------------------------------------------

		// Checkbox replacement
		bool MarkerToggle(const char* a_id, bool& a_value, const char* a_label = nullptr, const ImVec4* a_onColour = nullptr) {

			const auto& palette = Themes::GetPalette();
			const float size = ImGui::GetTextLineHeight();
			const float height = ImGui::GetFrameHeight();
			const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			const float labelWidth = a_label ? ImGui::CalcTextSize(a_label, nullptr, true).x + spacing : 0.0f;

			const bool clicked = ImGui::InvisibleButton(a_id, ImVec2(size + labelWidth, height));
			NoteControl();
			if (clicked) {
				a_value = !a_value;
			}

			const bool  hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			const float fill = Decor::Animate(ImGui::GetItemID(), a_value, 14.0f);
			const float glow = Decor::Animate(ImGui::GetItemID() + 1, hovered, 12.0f);
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const auto  centre = ImVec2(min.x + size * 0.5f, min.y + height * 0.5f);
			auto*       drawList = ImGui::GetWindowDrawList();

			// Focused from a controller or the keys: the marker alone is too small to spot, so a band
			// and underline run across the control's space, as the dropdowns and sliders show
			const float navGlow = Decor::Animate(ImGui::GetItemID() + 2, ImGui::IsItemFocused() && ImGui::GetCurrentContext()->NavCursorVisible, 12.0f);
			if (navGlow > 0.0f) {
				const float  reach = std::max(max.x + size, std::min(ImGui::GetCurrentWindow()->WorkRect.Max.x, min.x + size * 12.0f));
				const ImVec2 bandMin(min.x - size * 0.35f, min.y);
				const ImVec2 bandMax(reach, max.y);
				const ImU32  strong = Colour(palette.accent, 0.16f * navGlow);
				const ImU32  clear = Colour(palette.accent, 0.0f);
				drawList->AddRectFilledMultiColor(bandMin, bandMax, strong, clear, clear, strong);
				Decor::FadedLine(drawList, ImVec2(bandMin.x, max.y - 1.0f), ImVec2(bandMax.x, max.y - 1.0f), Colour(palette.accent, navGlow));
			}

			const ImVec4 on = a_onColour ? *a_onColour : palette.accent;
			const ImVec4 off = hovered ? palette.text : palette.textDim;
			if (glow > 0.0f && !SkyUI()) {
				drawList->AddCircleFilled(centre, size * 0.55f, Colour(palette.accent, 0.12f * glow));
			}
			DrawMarker(drawList, centre, size, Colour(a_value ? on : off), fill);

			if (a_label) {
				drawList->AddText(ImVec2(min.x + size + spacing, min.y + (height - ImGui::GetFontSize()) * 0.5f), Colour(hovered ? palette.text : palette.needed), a_label);
			}
			return clicked;
		}

		//---------------------------------------------------
		//-- Completionist UI ( State Key ) -----------------
		//---------------------------------------------------

		const char* StateKey(EntryState a_state) {

			switch (a_state) {
			case EntryState::kFound:
				return "UI_State_Found";
			case EntryState::kDisplayable:
				return "UI_State_Displayable";
			case EntryState::kDisplayed:
				return "UI_State_Displayed";
			case EntryState::kOccupied:
				return "UI_State_Occupied";
			default:
				return "UI_State_Needed";
			}
		}

		//---------------------------------------------------
		// Themed controls
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Completionist UI ( Button Width ) --------------
		//---------------------------------------------------

		float ButtonWidth(const char* a_label) {

			if (SkyUI()) {
				return ImGui::CalcTextSize(Upper(a_label).c_str(), nullptr, true).x + ImGui::GetStyle().FramePadding.x * 3.0f;
			}
			const float ends = ImGui::GetFontSize() * 0.8f;
			return ImGui::CalcTextSize(a_label, nullptr, true).x + ImGui::GetStyle().FramePadding.x * 2.0f + ends;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Themed Button ) -------------
		//---------------------------------------------------

		bool ThemedButton(const char* a_label, float a_width = 0.0f) {

			const auto& palette = Themes::GetPalette();
			const float width = a_width > 0.0f ? a_width : ButtonWidth(a_label);
			const bool  clicked = ImGui::InvisibleButton(a_label, ImVec2(width, ImGui::GetFrameHeight()));
			NoteControl();
			const bool  hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			const float glow = Decor::Animate(ImGui::GetItemID(), hovered || ImGui::IsItemActive(), 12.0f);
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const float midY = (min.y + max.y) * 0.5f;
			const float diamond = ImGui::GetFontSize() * 0.16f;
			auto*       drawList = ImGui::GetWindowDrawList();

			const char* labelEnd = a_label;
			while (*labelEnd && !(labelEnd[0] == '#' && labelEnd[1] == '#')) {
				++labelEnd;
			}

			if (SkyUI()) {
				const float midX = (min.x + max.x) * 0.5f;
				const ImU32 fill = Colour(palette.accent, 0.04f + (ImGui::IsItemActive() ? 0.22f : 0.14f * glow));
				const ImU32 clear = Colour(palette.accent, 0.0f);
				drawList->AddRectFilled(min, max, Colour(ImVec4(0, 0, 0, 0.45f)));
				drawList->AddRectFilledMultiColor(min, ImVec2(midX, max.y), clear, fill, fill, clear);
				drawList->AddRectFilledMultiColor(ImVec2(midX, min.y), max, fill, clear, clear, fill);
				drawList->AddRect(min, max, Colour(glow > 0.0f ? palette.text : palette.separator, 0.6f + 0.4f * glow));
				const auto caption = Upper(std::string_view(a_label, labelEnd));
				const auto size = ImGui::CalcTextSize(caption.c_str());
				drawList->PushClipRect(min, max, true);
				drawList->AddText(ImVec2(std::max(min.x + 2.0f, midX - size.x * 0.5f), midY - size.y * 0.5f), Colour(hovered ? palette.text : palette.needed, hovered ? 1.0f : 0.85f), caption.c_str());
				drawList->PopClipRect();
				return clicked;
			}

			drawList->AddRectFilled(min, max, Colour(palette.accent, 0.05f + (ImGui::IsItemActive() ? 0.18f : 0.10f * glow)));
			drawList->AddRect(min, max, Colour(glow > 0.0f ? palette.accent : palette.separator, 0.55f + 0.45f * glow));
			Decor::Diamond(drawList, ImVec2(min.x, midY), diamond, Colour(palette.accent, 0.6f + 0.4f * glow), true);
			Decor::Diamond(drawList, ImVec2(max.x, midY), diamond, Colour(palette.accent, 0.6f + 0.4f * glow), true);

			const auto textSize = ImGui::CalcTextSize(a_label, labelEnd);
			drawList->AddText(ImVec2((min.x + max.x - textSize.x) * 0.5f, midY - textSize.y * 0.5f), Colour(hovered ? palette.text : palette.needed), a_label, labelEnd);
			return clicked;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Themed Slider ) -------------
		//---------------------------------------------------

		bool ThemedSlider(const char* a_id, ImGuiDataType a_type, void* a_value, const void* a_min, const void* a_max, const char* a_format, float a_width, bool* a_released = nullptr) {

			ImGui::BeginGroup();

			const auto& palette = Themes::GetPalette();
			char        valueText[32];
			if (a_type == ImGuiDataType_Float) {
				std::snprintf(valueText, sizeof(valueText), a_format, *static_cast<const float*>(a_value));
			} else {
				std::snprintf(valueText, sizeof(valueText), a_format, *static_cast<const int*>(a_value));
			}
			const float valueWidth = ImGui::CalcTextSize("100%").x + ImGui::GetStyle().ItemSpacing.x;
			const float handle = ImGui::GetFrameHeight() * 0.62f;

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, handle);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::SetNextItemWidth(a_width - valueWidth);
			const bool changed = ImGui::SliderScalar(a_id, a_type, a_value, a_min, a_max, a_format);
			NoteControl();
			if (a_released) {
				*a_released = ImGui::IsItemDeactivatedAfterEdit();
			}
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(6);

			const bool  active = ImGui::IsItemActive();
			const bool  hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			const float glow = Decor::Animate(ImGui::GetItemID(), hovered || active, 12.0f);
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const float midY = (min.y + max.y) * 0.5f;

			// Same maths as ImGui's grab
			double value = 0.0;
			double low = 0.0;
			double high = 1.0;
			if (a_type == ImGuiDataType_Float) {
				value = *static_cast<const float*>(a_value);
				low = *static_cast<const float*>(a_min);
				high = *static_cast<const float*>(a_max);
			} else {
				value = *static_cast<const int*>(a_value);
				low = *static_cast<const int*>(a_min);
				high = *static_cast<const int*>(a_max);
			}
			const float t = high > low ? static_cast<float>(std::clamp((value - low) / (high - low), 0.0, 1.0)) : 0.0f;
			const float grabPadding = 2.0f;
			const float left = min.x + grabPadding + handle * 0.5f;
			const float right = max.x - grabPadding - handle * 0.5f;
			const float x = left + (right - left) * t;

			auto* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(ImVec2(left, midY - 1.0f), ImVec2(right, midY + 1.0f), Colour(palette.barBackground));
			drawList->AddRectFilled(ImVec2(left, midY - 1.0f), ImVec2(x, midY + 1.0f), Colour(palette.accent, 0.8f + 0.2f * glow));
			if (glow > 0.0f) {
				drawList->AddCircleFilled(ImVec2(x, midY), handle * 0.6f, Colour(palette.accent, 0.12f * glow));
			}
			Decor::Diamond(drawList, ImVec2(x, midY), handle * (0.36f + 0.06f * glow), Colour(palette.accent), true);

			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();
			ImGui::TextColored(hovered ? palette.text : palette.needed, "%s", valueText);
			ImGui::EndGroup();
			return changed;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Dress Popup ) ---------------
		//---------------------------------------------------

		// Inside the current popup: the theme's panel texture and corners, else the accent diamonds
		void DressPopup() {

			const auto& palette = Themes::GetPalette();
			auto*       drawList = ImGui::GetWindowDrawList();
			const auto  min = ImGui::GetWindowPos();
			const auto  max = ImVec2(min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight());
			const float corner = ImGui::GetFontSize() * 0.2f;
			drawList->PushClipRect(min, max, false);
			if (Decor::Panel(drawList, min, max, false)) {
				Decor::PanelCorners(drawList, min, max);
			} else if (!SkyUI()) {
				for (const auto& point : { min, ImVec2(max.x, min.y), ImVec2(min.x, max.y), max }) {
					Decor::Diamond(drawList, point, corner, Colour(palette.accent), true);
				}
			}
			drawList->PopClipRect();
		}

		//---------------------------------------------------
		//-- Completionist UI ( Themed Begin Combo ) --------
		//---------------------------------------------------

		// Combo drawn as text over an underline
		bool ThemedBeginCombo(const char* a_id, const char* a_preview, float a_width) {

			ImGui::SetNextItemWidth(a_width);

			const auto& palette = Themes::GetPalette();
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(palette.accent.x, palette.accent.y, palette.accent.z, 0.06f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(palette.accent.x, palette.accent.y, palette.accent.z, 0.10f));
			// The theme's own ground, near solid so the list reads over anything; light on Altmer
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(palette.background.x, palette.background.y, palette.background.z, 0.98f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(palette.accent.x, palette.accent.y, palette.accent.z, 0.6f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ImGui::GetFontSize() * 0.6f, ImGui::GetFontSize() * 0.5f));
			if (SkyUI()) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
			}
			const bool open = ImGui::BeginCombo(a_id, a_preview, ImGuiComboFlags_NoArrowButton);
			if (SkyUI()) {
				ImGui::PopStyleColor();
			}
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(5);

			// Inside the open list: dressed as the theme dresses its panels
			if (open) {
				DressPopup();
			}

			if (SkyUI()) {
				if (!open) {
					NoteControl();
				}
				const bool  hovered = open || ImGui::IsItemHovered() || ImGui::IsItemFocused();
				const auto  min = ImGui::GetItemRectMin();
				const auto  max = ImGui::GetItemRectMax();
				auto*       drawList = ImGui::GetWindowDrawList();
				const float size = ImGui::GetFontSize();
				const float pad = ImGui::GetStyle().FramePadding.x;
				const auto  value = Upper(a_preview);
				const auto  valueSize = ImGui::CalcTextSize(value.c_str());
				const float textX = std::max(min.x + pad + size, max.x - pad - valueSize.x);
				const float midY = (min.y + max.y) * 0.5f;
				const ImU32 colour = Colour(hovered ? palette.text : palette.needed, hovered ? 1.0f : 0.85f);
				const float arrow = size * 0.22f;
				const float arrowX = textX - size * 0.55f;
				drawList->AddTriangleFilled(ImVec2(arrowX - arrow, midY - arrow * 0.5f), ImVec2(arrowX + arrow, midY - arrow * 0.5f), ImVec2(arrowX, midY + arrow * 0.6f), colour);
				drawList->PushClipRect(min, max, true);
				drawList->AddText(ImVec2(textX, midY - valueSize.y * 0.5f), colour, value.c_str());
				drawList->PopClipRect();
				return open;
			}

			if (!open) {
				NoteControl();
				const bool  hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
				const auto  min = ImGui::GetItemRectMin();
				const auto  max = ImGui::GetItemRectMax();
				auto*       drawList = ImGui::GetWindowDrawList();
				const float size = ImGui::GetFontSize();
				Decor::FadedLine(drawList, ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), Colour(hovered ? palette.accent : palette.separator));
				Decor::Diamond(drawList, ImVec2(max.x - size * 0.5f, (min.y + max.y) * 0.5f), size * 0.2f, Colour(hovered ? palette.accent : palette.textDim), true);
			}
			return open;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Type On Activate ) ----------
		//---------------------------------------------------

		// A / Cross on a focused text box starts typing; ImGui only does that from Triangle, which is ours
		void TypeOnActivate() {

			if (ImGui::IsItemFocused() && !ImGui::IsItemActive() && ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown, false)) {
				ImGui::SetKeyboardFocusHere(-1);
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( Themed Filter Input ) -------
		//---------------------------------------------------

		// Filter box
		void ThemedFilterInput(const char* a_id, const char* a_hint, char* a_buffer, std::size_t a_size, float a_width) {

			const auto& palette = Themes::GetPalette();
			const float size = ImGui::GetFontSize();
			const auto  padding = ImGui::GetStyle().FramePadding;

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding.x + size * 1.1f, padding.y));
			ImGui::SetNextItemWidth(a_width);
			ImGui::InputTextWithHint(a_id, a_hint, a_buffer, a_size);
			NoteControl();
			TypeOnActivate();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor();

			const bool  active = ImGui::IsItemActive();
			const bool  hovered = ImGui::IsItemHovered();
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const auto  colour = Colour(active ? palette.accent : (hovered ? palette.text : palette.textDim));
			auto*       drawList = ImGui::GetWindowDrawList();
			const auto  lens = ImVec2(min.x + padding.x + size * 0.38f, (min.y + max.y) * 0.5f - size * 0.06f);
			const float radius = size * 0.26f;
			drawList->AddCircle(lens, radius, colour, 0, std::max(1.0f, size * 0.07f));
			drawList->AddLine(ImVec2(lens.x + radius * 0.7f, lens.y + radius * 0.7f), ImVec2(lens.x + radius * 1.6f, lens.y + radius * 1.6f), colour, std::max(1.0f, size * 0.09f));
			Decor::FadedLine(drawList, ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), Colour(active ? palette.accent : palette.separator));
		}

		constexpr std::string_view windowTab = "window";

		//---------------------------------------------------
		//-- Completionist UI ( Themed Text Input ) ---------
		//---------------------------------------------------

		bool ThemedTextInput(const char* a_id, char* a_buffer, std::size_t a_size, float a_width) {

			const auto& palette = Themes::GetPalette();
			ImGui::SetNextItemWidth(a_width);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::InputText(a_id, a_buffer, a_size);
			NoteControl();
			TypeOnActivate();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();

			const bool done = ImGui::IsItemDeactivatedAfterEdit();
			const bool active = ImGui::IsItemActive();
			const auto min = ImGui::GetItemRectMin();
			const auto max = ImGui::GetItemRectMax();
			Decor::FadedLine(ImGui::GetWindowDrawList(), ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), Colour(active || ImGui::IsItemHovered() ? palette.accent : palette.separator));
			return done;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour From RGB ) -----------
		//---------------------------------------------------

		ImVec4 ColourFromRGB(std::int32_t a_rgb) {

			const auto rgb = static_cast<std::uint32_t>(std::max(a_rgb, 0));
			return ImVec4(static_cast<float>((rgb >> 16) & 0xFF) / 255.0f, static_cast<float>((rgb >> 8) & 0xFF) / 255.0f, static_cast<float>(rgb & 0xFF) / 255.0f, 1.0f);
		}

		//---------------------------------------------------
		//-- Completionist UI ( RGB From Colour ) -----------
		//---------------------------------------------------

		std::int32_t RGBFromColour(const ImVec4& a_colour) {

			const auto channel = [](float a_value) { return static_cast<std::int32_t>(std::lround(std::clamp(a_value, 0.0f, 1.0f) * 255.0f)); };
			return (channel(a_colour.x) << 16) | (channel(a_colour.y) << 8) | channel(a_colour.z);
		}

		//---------------------------------------------------
		//-- Completionist UI ( Hue Of ) --------------------
		//---------------------------------------------------

		float HueOf(std::int32_t a_rgb) {

			const auto colour = ColourFromRGB(a_rgb);
			float      hue = 0.0f;
			float      saturation = 0.0f;
			float      value = 0.0f;
			ImGui::ColorConvertRGBtoHSV(colour.x, colour.y, colour.z, hue, saturation, value);
			return hue;
		}

		//---------------------------------------------------
		//-- Completionist UI ( HSV Colour ) ----------------
		//---------------------------------------------------

		ImVec4 HSVColour(float a_hue, float a_saturation, float a_value) {

			ImVec4 colour(0.0f, 0.0f, 0.0f, 1.0f);
			ImGui::ColorConvertHSVtoRGB(std::clamp(a_hue, 0.0f, 0.9999f), std::clamp(a_saturation, 0.0f, 1.0f), std::clamp(a_value, 0.0f, 1.0f), colour.x, colour.y, colour.z);
			return colour;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour Swatch ) -------------
		//---------------------------------------------------

		// A cut gem of colour: lit along the top, shaded at the foot, ringed in the accent when picked
		void ColourSwatch(ImDrawList* a_drawList, ImVec2 a_min, ImVec2 a_max, std::int32_t a_rgb, bool a_highlight) {

			const auto& palette = Themes::GetPalette();
			const float height = a_max.y - a_min.y;
			const float rounding = SkyUI() ? 0.0f : std::min(a_max.x - a_min.x, height) * 0.2f;
			if (a_highlight) {
				const float grow = std::max(2.0f, height * 0.1f);
				a_drawList->AddRectFilled(ImVec2(a_min.x - grow, a_min.y - grow), ImVec2(a_max.x + grow, a_max.y + grow), Colour(palette.accent, 0.3f), rounding + grow);
			}
			a_drawList->AddRectFilled(a_min, a_max, Colour(ColourFromRGB(a_rgb)), rounding);
			if (height > 8.0f) {
				a_drawList->AddRectFilled(a_min, ImVec2(a_max.x, a_min.y + height * 0.42f), IM_COL32(255, 255, 255, 34), rounding, ImDrawFlags_RoundCornersTop);
				a_drawList->AddRectFilled(ImVec2(a_min.x, a_max.y - height * 0.28f), a_max, IM_COL32(0, 0, 0, 40), rounding, ImDrawFlags_RoundCornersBottom);
			}
			a_drawList->AddRect(a_min, a_max, Colour(a_highlight ? palette.accent : palette.separator), rounding, 0, a_highlight ? 2.0f : 1.0f);
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour Button ) -------------
		//---------------------------------------------------

		bool ColourButton(const char* a_id, std::int32_t a_rgb, float a_width) {

			const auto& palette = Themes::GetPalette();
			const float height = ImGui::GetFrameHeight();
			const bool  clicked = ImGui::InvisibleButton(a_id, ImVec2(a_width, height));
			NoteControl();
			const bool  hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			auto*       drawList = ImGui::GetWindowDrawList();
			const float inset = height * 0.22f;
			const float swatchWidth = height * 1.8f;
			const auto  padding = ImGui::GetStyle().FramePadding.x;

			ColourSwatch(drawList, ImVec2(min.x + padding, min.y + inset), ImVec2(min.x + padding + swatchWidth, max.y - inset), a_rgb, hovered);
			const auto hex = HexColour(static_cast<std::uint32_t>(std::max(a_rgb, 0)));
			drawList->AddText(ImVec2(min.x + padding * 2.0f + swatchWidth, min.y + (height - ImGui::GetFontSize()) * 0.5f), Colour(hovered ? palette.text : palette.textDim), hex.c_str());
			Decor::FadedLine(drawList, ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), Colour(hovered ? palette.accent : palette.separator));
			return clicked;
		}

		// A focused colour bar uses left and right itself; navigation leaves them alone next frame
		bool sidewaysTaken = false;

		// Just opened: ImGui's focus is put in the popup once, so Back closes it
		bool colourFocusPending = false;

		// The picker's own controller focus: 0-19 the swatches, 20-22 the bars, 23 Done. ImGui's
		// navigation found nothing to move to in the popup and scrolled it instead, so the picker
		// steers itself and cancels ImGui's move while a controller or the arrow keys drive it
		int  colourFocus = 0;
		bool colourPadNav = false;
		// The button press that opened the picker isn't also a pick; armed once it's let go
		bool colourActivateArmed = false;

		constexpr std::int32_t colourSwatches[] = {
			0xFFFFFF, 0xC0C0C0, 0x808080, 0x404040, 0xC6A262, 0xFFD700, 0xEAAB00, 0xFFA500, 0xE37222, 0xBD4F19,
			0xCC3333, 0x990033, 0xB3FF00, 0x9EAB05, 0x13A81C, 0x99FFFF, 0x4398DE, 0x8C6CD0, 0x800080, 0xFF66CC,
		};

		// Where the picker's focus starts: the swatch of the current colour, else the hue bar
		int ColourStartFocus(std::int32_t a_rgb) {

			const auto it = std::ranges::find(colourSwatches, a_rgb);
			return it != std::end(colourSwatches) ? static_cast<int>(it - std::begin(colourSwatches)) : static_cast<int>(std::size(colourSwatches));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour Bar ) ----------------
		//---------------------------------------------------

		// One custom colour bar: 0 hue, 1 saturation, 2 brightness. Drag it, or with it focused
		// nudge it with left and right on the keys, d-pad or stick, held to keep going
		bool ColourBar(const char* a_id, const char* a_label, float& a_value, int a_kind, float a_hue, float a_saturation, float a_brightness, float a_width, bool a_padFocus) {

			const auto& palette = Themes::GetPalette();
			const float size = ImGui::GetFontSize();
			const float barHeight = std::max(size * 0.5f, 6.0f);
			ImGui::PushFont(Themes::SmallFont());
			const float labelHeight = ImGui::GetTextLineHeight();
			ImGui::PopFont();

			ImGui::InvisibleButton(a_id, ImVec2(a_width, labelHeight + barHeight * 2.2f));
			NoteControl();
			const bool  active = ImGui::IsItemActive();
			const bool  focused = a_padFocus || (!colourPadNav && ImGui::IsItemFocused());
			const float glow = Decor::Animate(ImGui::GetItemID(), focused || active || ImGui::IsItemHovered(), 12.0f);
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const ImVec2 barMin(min.x + barHeight * 0.6f, max.y - barHeight * 1.5f);
			const ImVec2 barMax(max.x - barHeight * 0.6f, barMin.y + barHeight);

			bool changed = false;
			if (active) {
				a_value = std::clamp((ImGui::GetIO().MousePos.x - barMin.x) / (barMax.x - barMin.x), 0.0f, 1.0f);
				changed = true;
			}
			if (focused) {
				sidewaysTaken = true;
			}
			if (focused && !active) {
				const auto pressed = [](ImGuiKey a_key, ImGuiKey a_pad, ImGuiKey a_stick) {
					return ImGui::IsKeyPressed(a_key) || ImGui::IsKeyPressed(a_pad) || ImGui::IsKeyPressed(a_stick);
				};
				const bool left = pressed(ImGuiKey_LeftArrow, ImGuiKey_GamepadDpadLeft, ImGuiKey_GamepadLStickLeft);
				const bool right = pressed(ImGuiKey_RightArrow, ImGuiKey_GamepadDpadRight, ImGuiKey_GamepadLStickRight);
				if (left != right) {
					a_value = std::clamp(a_value + (right ? 0.01f : -0.01f), 0.0f, 1.0f);
					changed = true;
				}
			}

			const auto colourAt = [&](float a_t) {
				switch (a_kind) {
				case 0:
					return HSVColour(a_t, 1.0f, 1.0f);
				case 1:
					return HSVColour(a_hue, a_t, a_brightness);
				default:
					return HSVColour(a_hue, a_saturation, a_t);
				}
			};

			auto* drawList = ImGui::GetWindowDrawList();
			ImGui::PushFont(Themes::SmallFont());
			drawList->AddText(ImVec2(barMin.x, min.y), Colour(focused || active ? palette.accent : palette.heading), a_label);
			const auto percent = std::to_string(static_cast<int>(std::lround(a_value * (a_kind == 0 ? 360.0f : 100.0f)))) + (a_kind == 0 ? "\xC2\xB0" : "%");
			const auto percentSize = ImGui::CalcTextSize(percent.c_str());
			drawList->AddText(ImVec2(barMax.x - percentSize.x, min.y), Colour(palette.textDim), percent.c_str());
			ImGui::PopFont();

			constexpr int segments = 24;
			const float   span = barMax.x - barMin.x;
			for (int i = 0; i < segments; ++i) {
				const float t0 = static_cast<float>(i) / segments;
				const float t1 = static_cast<float>(i + 1) / segments;
				const auto  left = Colour(colourAt(t0));
				const auto  right = Colour(colourAt(t1));
				drawList->AddRectFilledMultiColor(ImVec2(barMin.x + span * t0, barMin.y), ImVec2(barMin.x + span * t1, barMax.y), left, right, right, left);
			}
			drawList->AddRect(ImVec2(barMin.x - 1.0f, barMin.y - 1.0f), ImVec2(barMax.x + 1.0f, barMax.y + 1.0f), Colour(glow > 0.0f ? palette.accent : palette.separator, 0.6f + 0.4f * glow));
			const ImVec2 knob(barMin.x + span * a_value, (barMin.y + barMax.y) * 0.5f);
			if (glow > 0.0f) {
				drawList->AddCircleFilled(knob, barHeight * 1.3f, Colour(palette.accent, 0.15f * glow));
			}
			Decor::Diamond(drawList, knob, barHeight * (0.95f + 0.1f * glow), Colour(palette.accent), true);
			Decor::Diamond(drawList, knob, barHeight * 0.55f, Colour(colourAt(a_value)), true);
			return changed;
		}

		struct ColourPanelText
		{
			const char* swatches;
			const char* custom;
			const char* current;
			const char* next;
			const char* done;
			const char* hue;
			const char* saturation;
			const char* brightness;
		};

		//---------------------------------------------------
		//-- Completionist UI ( Begin Colour Popup ) --------
		//---------------------------------------------------

		// The colour picker floats over the page under its button, or above it when the button is low,
		// so the rows don't move and a controller stays inside it until Done or Back
		bool BeginColourPopup(const char* a_id, ImVec2 a_anchorMin, ImVec2 a_anchorMax, float a_width) {

			const auto& palette = Themes::GetPalette();
			const float size = ImGui::GetFontSize();
			const float width = std::max(a_width, size * 20.0f);
			const auto* viewport = ImGui::GetMainViewport();
			const float top = viewport->WorkPos.y;
			const float bottom = top + viewport->WorkSize.y;
			const bool  below = a_anchorMax.y < top + (bottom - top) * 0.5f;
			const float x = std::clamp(a_anchorMin.x, viewport->WorkPos.x + size, viewport->WorkPos.x + viewport->WorkSize.x - width - size);
			// Whole pixels, so there's no fraction of a pixel left for it to scroll through
			ImGui::SetNextWindowPos(ImVec2(std::floor(x), std::floor(below ? a_anchorMax.y + size * 0.3f : a_anchorMin.y - size * 0.3f)), ImGuiCond_Always, ImVec2(0.0f, below ? 0.0f : 1.0f));
			ImGui::SetNextWindowSizeConstraints(ImVec2(width, 0.0f), ImVec2(width, FLT_MAX));

			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(palette.background.x, palette.background.y, palette.background.z, 0.98f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(palette.accent.x, palette.accent.y, palette.accent.z, 0.6f));
			ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(size * 0.9f, size * 0.7f));
			const bool open = ImGui::BeginPopup(a_id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
			if (open) {
				// It fits its contents; the swatches' glow mustn't nudge it into scrolling
				ImGui::SetScrollY(0.0f);
				DressPopup();
			}
			return open;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Colour Panel ) --------------
		//---------------------------------------------------

		// The colour picker's contents, inside its popup. a_hue is kept apart from the colour so
		// greys and black don't lose it
		bool ColourPanel(std::int32_t& a_rgb, float& a_hue, std::int32_t a_original, const ColourPanelText& a_text, bool& a_close) {

			const auto&   swatches = colourSwatches;
			constexpr int columns = 10;
			constexpr int swatchCount = static_cast<int>(std::size(swatches));
			constexpr int firstBar = swatchCount;
			constexpr int doneSpot = firstBar + 3;

			const auto& palette = Themes::GetPalette();
			const auto& style = ImGui::GetStyle();
			const auto& io = ImGui::GetIO();
			auto*       drawList = ImGui::GetWindowDrawList();

			//---------------------------------------------------
			// The picker's own steering
			//---------------------------------------------------

			if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) {
				colourPadNav = false;
			}
			const auto pressed = [](ImGuiKey a_key, ImGuiKey a_pad, ImGuiKey a_stick) {
				return ImGui::IsKeyPressed(a_key) || ImGui::IsKeyPressed(a_pad) || ImGui::IsKeyPressed(a_stick);
			};
			const bool up = pressed(ImGuiKey_UpArrow, ImGuiKey_GamepadDpadUp, ImGuiKey_GamepadLStickUp);
			const bool down = pressed(ImGuiKey_DownArrow, ImGuiKey_GamepadDpadDown, ImGuiKey_GamepadLStickDown);
			const bool left = pressed(ImGuiKey_LeftArrow, ImGuiKey_GamepadDpadLeft, ImGuiKey_GamepadLStickLeft);
			const bool right = pressed(ImGuiKey_RightArrow, ImGuiKey_GamepadDpadRight, ImGuiKey_GamepadLStickRight);
			if (up || down || left || right) {
				colourPadNav = true;
			}

			bool activate = false;
			if (colourPadNav) {
				// ImGui's own move would only scroll the popup
				if (ImGui::GetCurrentContext()->NavMoveSubmitted) {
					ImGui::NavMoveRequestCancel();
				}
				ImGui::SetNavCursorVisible(false);

				static int lastColumn = 0;
				if (colourFocus < swatchCount) {
					const int row = colourFocus / columns;
					const int column = colourFocus % columns;
					if (left && column > 0) {
						--colourFocus;
					} else if (right && column + 1 < columns) {
						++colourFocus;
					} else if (up && row > 0) {
						colourFocus -= columns;
					} else if (down) {
						colourFocus = (row + 1) * columns < swatchCount ? colourFocus + columns : firstBar;
					}
					if (colourFocus < swatchCount) {
						lastColumn = colourFocus % columns;
					}
				} else if (colourFocus < doneSpot) {
					// Left and right belong to the bar itself
					if (up) {
						colourFocus = colourFocus == firstBar ? swatchCount - columns + lastColumn : colourFocus - 1;
					} else if (down) {
						++colourFocus;
					}
				} else if (up) {
					colourFocus = doneSpot - 1;
				}
				activate = colourActivateArmed && (ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown, false) || ImGui::IsKeyPressed(ImGuiKey_Enter, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false));
			}
			if (!ImGui::IsKeyDown(ImGuiKey_GamepadFaceDown) && !ImGui::IsKeyDown(ImGuiKey_Enter) && !ImGui::IsKeyDown(ImGuiKey_KeypadEnter)) {
				colourActivateArmed = true;
			}

			const float inner = ImGui::GetContentRegionAvail().x;
			const float gap = style.ItemSpacing.x;
			const float cell = (inner - gap * (columns - 1)) / columns;
			bool        changed = false;

			const auto heading = [&](const char* a_label) {
				ImGui::PushFont(Themes::SmallFont());
				ImGui::TextColored(palette.heading, "%s", a_label);
				ImGui::PopFont();
			};
			// The picker's focus ring, brighter than the picked one
			const auto padRing = [&](float a_grow) {
				const auto min = ImGui::GetItemRectMin();
				const auto max = ImGui::GetItemRectMax();
				drawList->AddRect(ImVec2(min.x - a_grow, min.y - a_grow), ImVec2(max.x + a_grow, max.y + a_grow), Colour(palette.text), 3.0f, 0, 2.0f);
			};

			heading(a_text.swatches);
			for (int i = 0; i < swatchCount; ++i) {
				if (i % columns != 0) {
					ImGui::SameLine(0.0f, gap);
				}
				ImGui::PushID(i);
				// Keeps ImGui's focus in the popup, so Back closes it
				if (i == 0 && colourFocusPending) {
					ImGui::SetKeyboardFocusHere();
				}
				const bool clicked = ImGui::InvisibleButton("##Swatch", ImVec2(cell, cell * 0.8f));
				if (i == 0) {
					colourFocusPending = false;
				}
				NoteControl();
				const bool padHere = colourPadNav && colourFocus == i;
				if ((clicked && !colourPadNav) || (padHere && activate)) {
					a_rgb = swatches[i];
					a_hue = HueOf(a_rgb);
					changed = true;
				}
				const bool highlight = padHere || swatches[i] == a_rgb || (!colourPadNav && (ImGui::IsItemHovered() || ImGui::IsItemFocused()));
				ColourSwatch(drawList, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), swatches[i], highlight);
				if (padHere) {
					padRing(std::max(3.0f, cell * 0.12f));
				}
				ImGui::PopID();
			}

			// Hue, saturation and brightness, each its own bar
			ImGui::Spacing();
			heading(a_text.custom);
			const auto current = ColourFromRGB(a_rgb);
			float      rgbHue = 0.0f;
			float      saturation = 0.0f;
			float      brightness = 0.0f;
			ImGui::ColorConvertRGBtoHSV(current.x, current.y, current.z, rgbHue, saturation, brightness);
			float hue = a_hue;
			bool  barChanged = false;
			barChanged |= ColourBar("##Hue", a_text.hue, hue, 0, hue, saturation, brightness, inner, colourPadNav && colourFocus == firstBar);
			barChanged |= ColourBar("##Saturation", a_text.saturation, saturation, 1, hue, saturation, brightness, inner, colourPadNav && colourFocus == firstBar + 1);
			barChanged |= ColourBar("##Brightness", a_text.brightness, brightness, 2, hue, saturation, brightness, inner, colourPadNav && colourFocus == firstBar + 2);
			if (barChanged) {
				a_hue = hue;
				a_rgb = RGBFromColour(HSVColour(hue, saturation, brightness));
				changed = true;
			}

			// Before and after, side by side, then Done
			ImGui::Spacing();
			const float chip = cell * 1.6f;
			const auto  previewSwatch = [&](const char* a_label, std::int32_t a_colour) {
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(palette.textDim, "%s", a_label);
				ImGui::SameLine();
				const auto pos = ImGui::GetCursorScreenPos();
				const auto top = pos.y + (ImGui::GetFrameHeight() - cell * 0.7f) * 0.5f;
				ImGui::Dummy(ImVec2(chip, ImGui::GetFrameHeight()));
				ColourSwatch(drawList, ImVec2(pos.x, top), ImVec2(pos.x + chip, top + cell * 0.7f), a_colour, false);
			};
			previewSwatch(a_text.current, a_original);
			ImGui::SameLine(0.0f, cell);
			previewSwatch(a_text.next, a_rgb);
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();
			ImGui::TextColored(palette.textDim, "%s", HexColour(static_cast<std::uint32_t>(std::max(a_rgb, 0))).c_str());

			const float doneWidth = ButtonWidth(a_text.done) + ImGui::GetFontSize() * 2.0f;
			ImGui::SameLine();
			if (ImGui::GetContentRegionAvail().x < doneWidth) {
				ImGui::NewLine();
			}
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max(0.0f, ImGui::GetContentRegionAvail().x - doneWidth));
			const bool doneClicked = ThemedButton(a_text.done, doneWidth);
			const bool padDone = colourPadNav && colourFocus == doneSpot;
			if (padDone) {
				padRing(2.0f);
			}
			if ((doneClicked && !colourPadNav) || (padDone && activate)) {
				a_close = true;
			}
			return changed;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Begin Framed Panel ) --------
		//---------------------------------------------------

		// a_emblem: a detail panel, where a race theme puts its emblem in the empty corner
		void BeginFramedPanel(const char* a_id, ImVec2 a_size, bool a_emblem = false) {

			const auto& palette = Themes::GetPalette();
			ImGui::PushStyleColor(ImGuiCol_ChildBg, InsetColour());
			ImGui::BeginChild(a_id, a_size, ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PopStyleColor();

			auto*       drawList = ImGui::GetWindowDrawList();
			const auto  min = ImGui::GetWindowPos();
			const auto  max = ImVec2(min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight());
			const float corner = ImGui::GetFontSize() * 0.2f;
			drawList->PushClipRect(min, max, false);
			// Race themes dress the panel: texture at the edges, light, emblem, their own corners
			const bool dressed = Decor::Panel(drawList, min, max, a_emblem);
			drawList->AddRect(min, max, Colour(palette.separator, 0.8f));
			for (const auto& point : { min, ImVec2(max.x, min.y), ImVec2(min.x, max.y), max }) {
				if (!SkyUI() && !dressed) {
					Decor::Diamond(drawList, point, corner, Colour(palette.accent), true);
				}
			}
			if (dressed) {
				Decor::PanelCorners(drawList, min, max);
			}
			drawList->PopClipRect();

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::BeginChild("##Scroll", ImVec2(0.0f, 0.0f), ImGuiChildFlags_NavFlattened);
			ImGui::PopStyleColor();
		}

		//---------------------------------------------------
		//-- Completionist UI ( Right Stick Amount ) --------
		//---------------------------------------------------

		float RightStickAmount() {

			const auto& io = ImGui::GetIO();
			const auto  analog = [&](ImGuiKey a_key) { return io.KeysData[a_key - ImGuiKey_NamedKey_BEGIN].AnalogValue; };
			const float amount = analog(ImGuiKey_GamepadRStickDown) - analog(ImGuiKey_GamepadRStickUp);
			return std::abs(amount) > 0.1f ? amount : 0.0f;
		}

		bool rightStickTaken = false;

		//---------------------------------------------------
		//-- Completionist UI ( Scroll With Right Stick ) ---
		//---------------------------------------------------

		// Right stick scrolls a long description, otherwise the list
		void ScrollWithRightStick() {

			const float amount = RightStickAmount();
			if (amount != 0.0f && ImGui::GetScrollMaxY() > 0.0f) {
				ImGui::SetScrollY(ImGui::GetScrollY() + amount * ImGui::GetFontSize() * 25.0f * ImGui::GetIO().DeltaTime);
				rightStickTaken = true;
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( End Framed Panel ) ----------
		//---------------------------------------------------

		void EndFramedPanel() {

			ImGui::EndChild();
			ImGui::EndChild();
		}

		//---------------------------------------------------
		//-- Completionist UI ( Row Selectable ) ------------
		//---------------------------------------------------

		bool RowSelectable(const char* a_id, bool a_selected, ImVec2 a_size, ImGuiSelectableFlags a_flags = 0) {


			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
			// A table cell takes the row's text baseline from the cells before it, which pushes a Selectable
			// down and makes it taller in every column but the first. Rows start from the top of the line.
			ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0.0f;
			const bool clicked = ImGui::Selectable(a_id, a_selected, a_flags, a_size);
			ImGui::PopStyleColor(3);
			NoteControl();

			const bool  focused = ImGui::IsItemFocused() && ImGui::GetIO().NavVisible;
			const float glow = Decor::Animate(ImGui::GetItemID(), a_selected || (ImGui::IsItemHovered() && !ImGui::GetIO().NavVisible) || focused);
			Decor::GlowBar(ImGui::GetWindowDrawList(), ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), Colour(Themes::GetPalette().accent), a_selected ? 1.0f : glow * 0.6f);
			return clicked;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Heading Rule ) --------------
		//---------------------------------------------------

		void HeadingRule(float a_space) {

			const auto  pos = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			const auto& palette = Themes::GetPalette();
			if (SkyUI()) {
				const auto  headingMin = ImGui::GetItemRectMin();
				const auto  headingMax = ImGui::GetItemRectMax();
				const float midY = std::round((headingMin.y + headingMax.y) * 0.5f);
				Decor::HeadingTail(ImGui::GetWindowDrawList(), ImVec2(headingMax.x + ImGui::GetFontSize() * 0.4f, midY), ImVec2(pos.x + width, midY), Colour(palette.text, 0.85f), Colour(palette.separator));
				ImGui::Dummy(ImVec2(width, a_space));
				return;
			}
			Decor::FadedLine(ImGui::GetWindowDrawList(), pos, ImVec2(pos.x + width, pos.y), Colour(palette.separator));
			ImGui::Dummy(ImVec2(width, a_space));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Divider Row ) ---------------
		//---------------------------------------------------

		void DividerRow() {

			const auto  pos = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			Decor::Divider(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y + 4.0f), ImVec2(pos.x + width, pos.y + 4.0f), Colour(Themes::GetPalette().separator));
			ImGui::Dummy(ImVec2(width, 8.0f));
		}
	}

	//---------------------------------------------------
	//-- Menu ( Frame ) ---------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw ) ----------------------
	//---------------------------------------------------

	bool Menu::Draw(IDataProvider& a_provider, UISettings& a_settings) {

		_settings = &a_settings;
		if (!_settingsApplied) {
			_hideCompleted = a_settings.hideCompleted;
			_settingsApplied = true;
		}

		if (const auto revision = a_provider.Revision(); revision != _revision) {
			_revision = revision;
			_dirty = true;
		}
		if (_dirty) {
			_categories = a_provider.GetCategories();
			std::ranges::stable_sort(_categories, {}, [](const Category& a_category) {
				constexpr std::string_view order[] = { "UI_Category_Quests", "UI_Category_Collections", "UI_Category_Locations", "UI_Category_Patches" };
				const auto                 it = std::ranges::find(order, std::string_view(a_category.Group()));
				return static_cast<int>(it - std::begin(order));
			});
			_recent = a_provider.GetRecentEvents();
			_kills = a_provider.GetKills();
			_dragonKills = a_provider.GetDragonKills();
			_pins = a_provider.GetPins();
			_settingsPages = a_provider.GetSettingsPages();
			_about = a_provider.GetAbout();
			// Put the focus back on the same entry after a rebuild
			_restoreFocusKey = _focusKey;
			_page.reset();
			++_pageLoads;
			_findIndexDirty = true;
			if (!_selectedPage.empty() && !_selectedPage.starts_with('#')) {
				_page = a_provider.GetPage(_selectedPage);
			}
			_dirty = false;
		}

		_showScreenshotButton = a_settings.screenshotButton;
		_statusTime = std::max(0.0f, _statusTime - ImGui::GetIO().DeltaTime);
		TrackInputDevice();
		_padActionKey.clear();
		_padTriggerKey.clear();

		rightStickTaken = false;

		// Just opened, focus starts on the sidebar
		if (std::exchange(_justOpened, false)) {
			_openFrames = 3;
			_showSidebarFocus = false;
		}
		if (_openFrames > 0) {
			--_openFrames;
			ImGui::SetNavCursorVisible(false);
			_focusSidebar = true;
			_focusCellList = true;
		}

		if (_placingToasts) {
			DrawToastPlacement(a_provider, a_settings);
			return true;
		}
		if (_cellView) {
			return DrawCellWindow(a_provider, a_settings);
		}

		const auto& io = ImGui::GetIO();
		const auto  size = ImVec2(
			std::clamp(io.DisplaySize.x * a_settings.widthPercent / 100.0f, std::min(io.DisplaySize.x, 640.0f), io.DisplaySize.x),
			std::clamp(io.DisplaySize.y * a_settings.heightPercent / 100.0f, std::min(io.DisplaySize.y, 480.0f), io.DisplaySize.y));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);

		bool open = true;
		ImGui::PushFont(Themes::BodyFont());
		auto background = Themes::GetPalette().background;
		background.w = std::clamp(a_settings.opacity, 0.3f, 1.0f);
		windowOpacity = background.w;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
		const float pad = ImGui::GetFontSize() * 1.4f;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(pad, pad));
		constexpr auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		// The panels scroll, the window never does (controller focus near the bottom nudged it)
		ImGui::SetNextWindowScroll(ImVec2(0.0f, 0.0f));
		const bool     visible = ImGui::Begin("##Completionist", nullptr, flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (visible) {
			const auto min = ImGui::GetWindowPos();
			const auto max = ImVec2(min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight());
			const auto inset = ImGui::GetFontSize() * 0.4f;
			Decor::Frame(ImGui::GetWindowDrawList(), ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), Colour(Themes::GetPalette().separator));

			HandleGamepadShortcuts(open);
			RouteNavigation();
			// Header buttons are controller targets too
			controlRecorder = this;
			_recordingHeader = true;
			DrawHeader(a_provider, open);
			if (_pins.enabled && !_pins.pins.empty()) {
				DrawPinnedBar(a_provider);
			}
			_recordingHeader = false;
			controlRecorder = nullptr;
			const float footer = PadHintsHeight();

			const float fullWidth = ImGui::GetFontSize() * 17.0f;
			const float navWidth = std::floor(fullWidth * UpdateNavigationOpen());
			const auto  bodyTop = ImGui::GetCursorScreenPos();
			if (navWidth >= 1.0f) {
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::BeginChild("##NavigationClip", ImVec2(navWidth, -footer), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);
				ImGui::PopStyleVar();
				ImGui::SetCursorPosX(navWidth - fullWidth);
				ImGui::BeginChild("##Navigation", ImVec2(fullWidth, 0.0f), ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_NavFlattened);
				DrawNavigation(a_provider);
				ImGui::EndChild();
				ImGui::EndChild();
				ImGui::SameLine(0.0f, 0.0f);
			} else {
				ImGui::SetCursorScreenPos(bodyTop);
				_sidebarFocused = false;
				// Folded, the opening focus goes to the page instead
				_focusPage = _focusPage || (_focusSidebar && !_showSidebarFocus);
				_focusSidebar = _focusSidebar && _showSidebarFocus;
			}
			DrawNavigationGrip(ImGui::GetFontSize() * 0.8f);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::BeginChild("##Body", ImVec2(0.0f, -footer), ImGuiChildFlags_NavFlattened);
			controlRecorder = this;
			if (_selectedPage == settingsPage) {
				DrawSettings(a_provider);
			} else if (_selectedPage == killsPage) {
				DrawKills(a_provider, _kills);
			} else if (_selectedPage == findPage) {
				DrawFind(a_provider);
			} else if (_page) {
				DrawPage(a_provider);
			} else {
				DrawOverview(a_provider);
			}
			controlRecorder = nullptr;
			ImGui::EndChild();
			ImGui::PopStyleColor();
			// The page's first entry (or control), hidden until the first press shows it
			if (std::exchange(_focusPage, false)) {
				const auto start = std::ranges::find_if(_entrySpots, &NavSpot::visible);
				const auto control = std::ranges::find_if(_controlSpots, [](const NavSpot& a_spot) { return a_spot.visible && a_spot.section != -1; });
				const NavSpot* spot = start != _entrySpots.end() ? std::addressof(*start) : control != _controlSpots.end() ? std::addressof(*control) : nullptr;
				if (spot) {
					// Placed as the item is drawn next frame, inside its own window
					_pendingFocus = spot->id;
					_pendingHidden = spot->id;
					_revealFocus = spot->id;
				}
			}
			if (PadHintsShown()) {
				DrawPadHints(a_provider);
			}
			ScrollFocusWithRightStick();
			_restoreFocusKey.clear();
		}
		// B / Escape close a dropdown first
		_backHandledByImGui = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel) || ImGui::IsAnyItemActive();
		ImGui::End();
		ImGui::PopFont();

		a_settings.lastPage = _selectedPage;
		return open;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Handle Gamepad Shortcuts ) ---
	//---------------------------------------------------

	void Menu::HandleGamepadShortcuts(bool& a_open) {

		// Triangle is ours (autofit and fold the description). ImGui's navigation reads it as its
		// "input" key, which also presses whatever is focused, so it's given an owner nav won't use
		ImGui::SetKeyOwner(ImGuiKey_GamepadFaceUp, ImGui::GetID("##DescriptionKey"));

		// B / Escape step back to the sidebar, then close
		const bool backPressed = ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false) || ImGui::IsKeyPressed(ImGuiKey_Escape, false);
		if (backPressed && !_backHandledByImGui) {
			if (ImGui::GetIO().NavVisible && !_sidebarFocused) {
				if (_settings && _settings->navigationCollapsed) {
					_settings->navigationCollapsed = false;
					_settingsChanged = true;
				}
				_focusSidebar = true;
				_showSidebarFocus = true;
			} else {
				a_open = false;
			}
		}
		// Y fits the description panel to the text, held collapses or expands it
		// Ahead of the active check, as Y down also activates the focused row
		if (_settings && !ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel)) {
			constexpr float holdTime = 0.4f;
			const auto*     faceUp = ImGui::GetKeyData(ImGuiKey_GamepadFaceUp);
			if (faceUp->Down && faceUp->DownDuration >= holdTime && !_descriptionHeld) {
				_descriptionHeld = true;
				_descriptionFitLines = -1.0f;
				_settings->descriptionCollapsed = !_settings->descriptionCollapsed;
				_settingsChanged = true;
			}
			if (ImGui::IsKeyReleased(ImGuiKey_GamepadFaceUp) && !std::exchange(_descriptionHeld, false)) {
				_fitDescription = true;
				if (_settings->descriptionCollapsed) {
					_settings->descriptionCollapsed = false;
					_settingsChanged = true;
				}
			}
		}

		if (_backHandledByImGui || ImGui::IsAnyItemActive()) {
			return;
		}

		// Bumpers step through the sidebar
		const bool previous = ImGui::IsKeyPressed(ImGuiKey_GamepadL1, false);
		const bool next = ImGui::IsKeyPressed(ImGuiKey_GamepadR1, false);
		if (previous || next) {
			std::vector<std::string> pages{ std::string{} };
			const Category*          lastCollections = nullptr;
			for (const auto& category : _categories) {
				if (category.Group() == "UI_Category_Collections") {
					lastCollections = std::addressof(category);
				}
			}
			for (const auto& category : _categories) {
				for (const auto& page : category.pages) {
					pages.push_back(page.id);
				}
				if (std::addressof(category) == lastCollections) {
					pages.emplace_back(killsPage);
				}
			}
			pages.emplace_back(settingsPage);

			const auto current = std::ranges::find(pages, _selectedPage);
			auto       index = current == pages.end() ? 0 : static_cast<std::ptrdiff_t>(current - pages.begin());
			const auto count = static_cast<std::ptrdiff_t>(pages.size());
			index = (index + (next ? 1 : count - 1)) % count;
			OpenPage(pages[static_cast<std::size_t>(index)]);
		}

		// LS folds the sidebar
		if (_settings && ImGui::IsKeyPressed(ImGuiKey_GamepadL3, false)) {
			_settings->navigationCollapsed = !_settings->navigationCollapsed;
			_settingsChanged = true;
		}

		// View / Back opens Find
		if (ImGui::IsKeyPressed(ImGuiKey_GamepadBack, false)) {
			OpenPage(findPage);
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Keep Focus ) ----------------
	//---------------------------------------------------

	void Menu::KeepFocus(const std::string& a_key) {

		if (ImGui::IsItemFocused()) {
			_focusKey = a_key;
		}
		if (!_restoreFocusKey.empty() && _restoreFocusKey == a_key) {
			auto* window = ImGui::GetCurrentWindow();
			ImGui::SetFocusID(ImGui::GetItemID(), window);
			window->NavRectRel[0] = ImGui::WindowRectAbsToRel(window, ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
			ImGui::ScrollToItem(ImGuiScrollFlags_KeepVisibleEdgeY);
			if (_padActive) {
				ImGui::SetNavCursorVisible(true);
			}
			_focusKey = a_key;
			_restoreFocusKey.clear();
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Record Spot ) ---------------
	//---------------------------------------------------

	void Menu::RecordSpot(std::vector<NavSpot>& a_spots, int a_section, int a_index, int a_columns, int a_count) {

		const auto  id = ImGui::GetItemID();
		const auto  min = ImGui::GetItemRectMin();
		const auto  max = ImGui::GetItemRectMax();
		auto*       window = ImGui::GetCurrentWindow();
		if (&a_spots == &_entrySpots && !_controlSpots.empty() && _controlSpots.back().id == id) {
			_controlSpots.pop_back();
		}
		a_spots.push_back({ id, window, min.x, min.y, max.x, max.y, a_section, a_index, a_columns, a_count, ImGui::IsItemVisible() });
		if (ImGui::IsItemFocused()) {
			_focusedSpot = id;
		}
		if (_pendingFocus != 0 && _pendingFocus == id) {
			ImGui::SetFocusID(id, window);
			window->NavRectRel[0] = ImGui::WindowRectAbsToRel(window, ImRect(min, max));
			ImGui::SetNavCursorVisible(std::exchange(_pendingHidden, 0) != id);
			ImGui::ScrollToItem(ImGuiScrollFlags_KeepVisibleEdgeY);
			_pendingFocus = 0;
			// Held, the repeat can press again next frame while ImGui has dropped the nav id: the router
			// then goes from this, and without it went from the row before or from nothing (the sidebar)
			_focusedSpot = id;
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Describe Controls ) ---------
	//---------------------------------------------------

	std::string Menu::DescribeControls() const {

		std::string text;
		for (const auto& spot : _controlSpots) {
			text += "  control " + std::to_string(spot.index) + " x " + std::to_string(static_cast<int>(spot.minX)) + "-" + std::to_string(static_cast<int>(spot.maxX)) + " y " + std::to_string(static_cast<int>(spot.minY)) + "-" + std::to_string(static_cast<int>(spot.maxY)) + (spot.visible ? "" : " hidden") + "\n";
		}
		return text;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Spots ) ---------------------
	//---------------------------------------------------

	std::vector<Menu::SpotView> Menu::Spots() const {

		std::vector<SpotView> spots;
		const auto add = [&](const std::vector<NavSpot>& a_list, char a_kind) {
			for (const auto& spot : a_list) {
				const char kind = a_kind == 'C' && spot.section == -1 ? 'H' : a_kind;
				spots.push_back({ spot.id, kind, spot.section, spot.index, spot.window, spot.minX, spot.minY, spot.maxX, spot.maxY, spot.visible });
			}
		};
		add(_entrySpots, 'E');
		add(_controlSpots, 'C');
		add(_sidebarSpots, 'S');
		return spots;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Describe Focus ) ------------
	//---------------------------------------------------

	std::string Menu::DescribeFocus(std::uint32_t a_id) const {

		for (const auto& spot : _entrySpots) {
			if (spot.id == a_id) {
				return "entry " + std::to_string(spot.section) + "/" + std::to_string(spot.index) + " of " + std::to_string(spot.count) + " (" + std::to_string(spot.columns) + " cols)" + (spot.visible ? "" : " hidden") + " y " + std::to_string(static_cast<int>(spot.minY)) + "-" + std::to_string(static_cast<int>(spot.maxY)) + [&] { std::string rows; for (const auto& other : _entrySpots) { if (other.section == spot.section && other.index >= 14 && other.index <= 19) rows += " " + std::to_string(other.index) + ":" + std::to_string(static_cast<int>(other.minY)); } return rows; }();
			}
		}
		for (const auto& spot : _sidebarSpots) {
			if (spot.id == a_id) {
				return "sidebar " + std::to_string(spot.index);
			}
		}
		for (const auto& spot : _controlSpots) {
			if (spot.id == a_id) {
				return "control " + std::to_string(spot.index);
			}
		}
		return "- (" + std::to_string(_entrySpots.size()) + " entries recorded)";
	}

	//---------------------------------------------------
	//-- Completionist UI ( Record Control ) ------------
	//---------------------------------------------------

	void Menu::RecordControl() {

		RecordSpot(_controlSpots, _recordingHeader ? -1 : 0, static_cast<int>(_controlSpots.size()), 1, 0);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Route Navigation ) ----------
	//---------------------------------------------------

	void Menu::RouteNavigation() {

		_entrySpotsLast.swap(_entrySpots);
		_entrySpots.clear();
		_sidebarSpotsLast.swap(_sidebarSpots);
		_sidebarSpots.clear();
		_controlSpotsLast.swap(_controlSpots);
		_controlSpots.clear();
		_sidebarSelectedLast = std::exchange(_sidebarSelectedID, 0);
		_focusedSpotLast = std::exchange(_focusedSpot, 0);
		_spotSection = 0;

		auto&      g = *ImGui::GetCurrentContext();
		const bool sideways = std::exchange(sidewaysTaken, false);
		// A popup (a dropdown, the colour picker) keeps the controller inside it; ImGui does that
		// itself, and last frame's focus out on the page must not pull it back out
		if (ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel)) {
			return;
		}
		if (!g.NavMoveSubmitted || g.NavMoveDir == ImGuiDir_None || (g.NavMoveFlags & (ImGuiNavMoveFlags_IsTabbing | ImGuiNavMoveFlags_IsPageMove)) != 0) {
			return;
		}
		const auto dir = g.NavMoveDir;
		if (sideways && (dir == ImGuiDir_Left || dir == ImGuiDir_Right)) {
			ImGui::NavMoveRequestCancel();
			return;
		}
		if (const auto parked = std::exchange(_cellParkedRow, 0); parked != 0) {
			ImGui::NavMoveRequestCancel();
			_pendingFocus = parked;
			ImGui::SetNavCursorVisible(true);
			return;
		}
		// ImGui can drop the nav id, use last frame's
		const auto current = g.NavId != 0 ? g.NavId : _focusedSpotLast;
		// The opening focus on the page: the first press shows it rather than moving off it
		if (const auto reveal = std::exchange(_revealFocus, 0); reveal != 0 && reveal == current && !g.NavCursorVisible) {
			ImGui::NavMoveRequestCancel();
			_pendingFocus = current;	// ImGui drops the hidden focus on the press
			return;
		}
		const auto find = [](const std::vector<NavSpot>& a_spots, std::uint32_t a_id) -> const NavSpot* {
			const auto it = std::ranges::find(a_spots, a_id, &NavSpot::id);
			return it == a_spots.end() ? nullptr : std::addressof(*it);
		};
		const auto at = [&](int a_section, int a_index) -> const NavSpot* {
			const auto it = std::ranges::find_if(_entrySpotsLast, [&](const NavSpot& a_spot) { return a_spot.section == a_section && a_spot.index == a_index; });
			return it == _entrySpotsLast.end() ? nullptr : std::addressof(*it);
		};
		const auto sectionInfo = [&](int a_section) -> const NavSpot* { return at(a_section, 0); };
		// How much of a spot its panel shows, 0 to 1
		const auto shown = [](const NavSpot& a_spot) {
			const auto* window = static_cast<const ImGuiWindow*>(a_spot.window);
			const float height = a_spot.maxY - a_spot.minY;
			if (!window || height <= 0.0f) {
				return a_spot.visible ? 1.0f : 0.0f;
			}
			return std::clamp((std::min(a_spot.maxY, window->ClipRect.Max.y) - std::max(a_spot.minY, window->ClipRect.Min.y)) / height, 0.0f, 1.0f);
		};

		// Scrolled away with the right stick: Up or Down picks up from what's on screen, the row at the edge
		// the focus went past, rather than jumping back to it. Focusing a row scrolls it fully into view.
		if (dir == ImGuiDir_Up || dir == ImGuiDir_Down) {
			for (const auto* list : { &_entrySpotsLast, &_controlSpotsLast, &_sidebarSpotsLast }) {
				const auto* spot = find(*list, current);
				if (!spot || shown(*spot) >= 0.5f) {
					continue;
				}
				const auto* window = static_cast<const ImGuiWindow*>(spot->window);
				const bool  above = window && spot->maxY <= (window->ClipRect.Min.y + window->ClipRect.Max.y) * 0.5f;
				const NavSpot* edge = nullptr;
				for (const auto& candidate : *list) {
					if (candidate.id == current || candidate.window != spot->window || shown(candidate) < 0.5f) {
						continue;
					}
					if (!edge || (above ? candidate.minY < edge->minY : candidate.minY > edge->minY)) {
						edge = std::addressof(candidate);
					}
				}
				if (edge) {
					ImGui::NavMoveRequestCancel();
					ImGui::SetNavCursorVisible(true);
					_pendingFocus = edge->id;
					_lastMoveFrom = _lastMoveTo = 0;
					lastRoute = "routed to the edge on screen";
					return;
				}
			}
		}

		// Nearest item in a direction
		const auto nearest = [&](const NavSpot& a_from, std::initializer_list<const std::vector<NavSpot>*> a_lists) -> const NavSpot* {
			struct Candidate
			{
				const NavSpot* spot;
				float          along;
				float          offset;
				bool           lined;
			};
			std::vector<Candidate> candidates;
			const bool             vertical = dir == ImGuiDir_Up || dir == ImGuiDir_Down;
			for (const auto* list : a_lists) {
				for (const auto& candidate : *list) {
					if (candidate.id == a_from.id) {
						continue;
					}
					if (a_from.section != -1 && candidate.section == -1 && dir != ImGuiDir_Up) {
						continue;
					}
					if (!candidate.visible && candidate.window != a_from.window) {
						continue;
					}
					float along = 0.0f;
					switch (dir) {
					case ImGuiDir_Down: along = candidate.minY - a_from.maxY; break;
					case ImGuiDir_Up: along = a_from.minY - candidate.maxY; break;
					case ImGuiDir_Right: along = candidate.minX - a_from.maxX; break;
					default: along = a_from.minX - candidate.maxX; break;
					}
					if (along < -2.0f) {
						continue;
					}
					const float overlap = vertical ? std::min(a_from.maxX, candidate.maxX) - std::max(a_from.minX, candidate.minX) : std::min(a_from.maxY, candidate.maxY) - std::max(a_from.minY, candidate.minY);
					const float offset = vertical ? std::abs((candidate.minX + candidate.maxX) - (a_from.minX + a_from.maxX)) * 0.5f : std::abs((candidate.minY + candidate.maxY) - (a_from.minY + a_from.maxY)) * 0.5f;
					// Up and down reach items a little to the side; how far is measured edge to edge, so a
					// row further away doesn't win over a nearer one just off to the side
					const float sideGap = vertical ? -overlap : offset;
					const float slack = vertical ? std::max(0.0f, along) + 6.0f * ImGui::GetFrameHeight() : std::max(0.0f, along) + ImGui::GetFrameHeight();
					if (overlap <= 0.0f && sideGap > slack) {
						continue;
					}
					candidates.push_back({ std::addressof(candidate), std::max(0.0f, along), offset, overlap > 0.0f });
				}
			}
			const bool moreThisWay = std::ranges::any_of(candidates, [&](const Candidate& a_candidate) { return !a_candidate.spot->visible && a_candidate.spot->window == a_from.window; });
			const bool onRow = !vertical && std::ranges::any_of(candidates, &Candidate::lined);

			const auto eligible = [&](const Candidate& a_candidate) {
				return !(moreThisWay && a_candidate.spot->window != a_from.window) && !(onRow && !a_candidate.lined);
			};
			const auto leaving = [&](const Candidate& a_candidate) {
				return a_candidate.spot->window != a_from.window ? ImGui::GetFrameHeight() * 4.0f : 0.0f;
			};
			// Up and down only go to the next row
			float nextRow = FLT_MAX;
			if (vertical) {
				for (const auto& candidate : candidates) {
					if (eligible(candidate)) {
						nextRow = std::min(nextRow, candidate.along + leaving(candidate));
					}
				}
			}

			const Candidate* best = nullptr;
			float            bestDistance = FLT_MAX;
			for (const auto& candidate : candidates) {
				if (!eligible(candidate) || (vertical && candidate.along + leaving(candidate) > nextRow + ImGui::GetFrameHeight() * 0.75f)) {
					continue;
				}
				const float distance = candidate.along + candidate.offset * (candidate.lined ? 0.05f : 1.0f) + leaving(candidate);
				if (distance < bestDistance) {
					bestDistance = distance;
					best = std::addressof(candidate);
				}
			}
			return best ? best->spot : nullptr;
		};
		// Closest item up or down when nothing lines up, such as the log's date filter under its tab:
		// the next row that way, then the item in it nearest across
		const auto closest = [&](const NavSpot& a_from, std::initializer_list<const std::vector<NavSpot>*> a_lists) -> const NavSpot* {
			const auto along = [&](const NavSpot& a_candidate) { return dir == ImGuiDir_Down ? a_candidate.minY - a_from.maxY : a_from.minY - a_candidate.maxY; };
			const auto usable = [&](const NavSpot& a_candidate) {
				return a_candidate.id != a_from.id && !(a_candidate.section == -1 && dir != ImGuiDir_Up) && (a_candidate.visible || a_candidate.window == a_from.window) && along(a_candidate) >= -2.0f;
			};
			float nextRow = FLT_MAX;
			for (const auto* list : a_lists) {
				for (const auto& candidate : *list) {
					if (usable(candidate)) {
						nextRow = std::min(nextRow, std::max(0.0f, along(candidate)));
					}
				}
			}
			const NavSpot* best = nullptr;
			float          bestDistance = FLT_MAX;
			for (const auto* list : a_lists) {
				for (const auto& candidate : *list) {
					if (!usable(candidate) || along(candidate) > nextRow + ImGui::GetFrameHeight() * 0.75f) {
						continue;
					}
					const float distance = std::abs((candidate.minX + candidate.maxX) - (a_from.minX + a_from.maxX)) * 0.5f;
					if (distance < bestDistance) {
						bestDistance = distance;
						best = std::addressof(candidate);
					}
				}
			}
			return best;
		};

		const auto sidebarFor = [&](const NavSpot&) -> const NavSpot* {
			// The open page's row while the sidebar shows it, otherwise the top row showing in its scroll
			// (snapped fully into view), rather than scrolling all the way back to the open page
			if (const auto* selected = find(_sidebarSpotsLast, _sidebarSelectedLast); selected && selected->visible) {
				return selected;
			}
			const NavSpot* top = nullptr;
			for (const auto& row : _sidebarSpotsLast) {
				if (row.visible && (!top || row.minY < top->minY)) {
					top = std::addressof(row);
				}
			}
			return top ? top : _sidebarSpotsLast.empty() ? nullptr : std::addressof(_sidebarSpotsLast.front());
		};
		std::vector<NavSpot> topSpots;
		std::vector<NavSpot> pageControls;

		const NavSpot* target = nullptr;
		bool           handled = false;

		if (const auto* spot = find(_entrySpotsLast, current)) {
			handled = true;
			_lastEntryFocus = current;

			const int cols = std::max(1, spot->columns);
			const int row = spot->index / cols;
			const int col = spot->index % cols;
			// Sections carry on in the same list; the quest journal has its Completed list beside the others
			const auto sameList = [&](const NavSpot* a_other) { return a_other && a_other->window == spot->window; };
			// The closest entry in the list beside this one, left or right
			const auto beside = [&]() -> const NavSpot* {
				const NavSpot* best = nullptr;
				float          bestDistance = FLT_MAX;
				for (const auto& candidate : _entrySpotsLast) {
					if (candidate.window == spot->window || !candidate.visible) {
						continue;
					}
					const float gap = dir == ImGuiDir_Right ? candidate.minX - spot->maxX : spot->minX - candidate.maxX;
					if (gap < -2.0f) {
						continue;
					}
					const float distance = gap + std::abs((candidate.minY + candidate.maxY) - (spot->minY + spot->maxY)) * 0.5f;
					if (distance < bestDistance) {
						bestDistance = distance;
						best = std::addressof(candidate);
					}
				}
				return best;
			};
			switch (dir) {
			case ImGuiDir_Down:
				if ((row + 1) * cols + col < spot->count) {
					target = at(spot->section, (row + 1) * cols + col);
				} else if ((row + 1) * cols < spot->count) {
					target = at(spot->section, spot->count - 1);
				} else if (const auto* next = sectionInfo(spot->section + 1); sameList(next)) {
					target = at(next->section, std::min({ col, next->columns - 1, next->count - 1 }));
				}
				break;
			case ImGuiDir_Up:
				if (row > 0) {
					target = at(spot->section, (row - 1) * cols + col);
				} else if (const auto* previous = sectionInfo(spot->section - 1); sameList(previous)) {
					const int lastRow = (previous->count - 1) / std::max(1, previous->columns);
					target = at(previous->section, std::min(lastRow * previous->columns + col, previous->count - 1));
				} else {
					target = nearest(*spot, { &_controlSpotsLast });
				}
				break;
			case ImGuiDir_Left:
				if (col > 0) {
					target = at(spot->section, spot->index - 1);
				} else if (!(target = beside())) {
					target = sidebarFor(*spot);
				}
				break;
			case ImGuiDir_Right:
				if (col + 1 < cols && spot->index + 1 < spot->count) {
					target = at(spot->section, spot->index + 1);
				} else {
					target = beside();
				}
				break;
			default:
				break;
			}
		} else if (const auto* control = find(_controlSpotsLast, current)) {
			handled = true;
			if (control->section == -1 && dir == ImGuiDir_Down) {
				std::ranges::copy_if(_controlSpotsLast, std::back_inserter(topSpots), [](const NavSpot& a_spot) { return a_spot.section == -1; });
				target = nearest(*control, { &topSpots });
				if (target && std::min(control->maxX, target->maxX) - std::max(control->minX, target->minX) <= 0.0f) {
					target = nullptr;
				}
				// Buttons over the sidebar go into it, the rest (the Settings cog) into the page
				const bool overSidebar = !_sidebarSpotsLast.empty() && control->minX < _sidebarSpotsLast.front().maxX;
				if (!target && !overSidebar) {
					target = closest(*control, { &_controlSpotsLast, &_entrySpotsLast });
				}
				if (!target && !_sidebarSpotsLast.empty()) {
					target = std::addressof(_sidebarSpotsLast.front());
				}
			} else {
				target = nearest(*control, { &_controlSpotsLast, &_entrySpotsLast });
				if (!target && (dir == ImGuiDir_Up || dir == ImGuiDir_Down)) {
					target = closest(*control, { &_controlSpotsLast, &_entrySpotsLast });
				}
			}
			if (!target && dir == ImGuiDir_Left) {
				target = sidebarFor(*control);
			}
		} else if (const auto* side = find(_sidebarSpotsLast, current)) {
			handled = true;
			switch (dir) {
			case ImGuiDir_Down:
				if (side->index + 1 < static_cast<int>(_sidebarSpotsLast.size())) {
					target = std::addressof(_sidebarSpotsLast[side->index + 1]);
				}
				break;
			case ImGuiDir_Up:
				if (side->index > 0) {
					target = std::addressof(_sidebarSpotsLast[side->index - 1]);
				} else {
					target = nearest(*side, { &_controlSpotsLast });
				}
				break;
			case ImGuiDir_Right:
				{
					std::ranges::copy_if(_controlSpotsLast, std::back_inserter(pageControls), [](const NavSpot& a_spot) { return a_spot.section != -1; });
					const std::vector<NavSpot>& items = _entrySpotsLast.empty() ? pageControls : _entrySpotsLast;
					if (items.empty()) {
						handled = false;
						break;
					}
					// Back to the last entry, if it's in the list next to the sidebar rather than one further across
					const auto leftmost = std::ranges::min_element(_entrySpotsLast, {}, [](const NavSpot& a_spot) { return a_spot.visible ? a_spot.minX : FLT_MAX; });
					if (const auto* last = find(_entrySpotsLast, _lastEntryFocus); last && last->visible && leftmost != _entrySpotsLast.end() && last->window == leftmost->window) {
						target = last;
						break;
					}
					float best = FLT_MAX;
					for (const auto& candidate : items) {
						const bool level = candidate.minY < side->maxY && candidate.maxY > side->minY;
						if (candidate.visible && level && candidate.minX < best) {
							best = candidate.minX;
							target = std::addressof(candidate);
						}
					}
					if (!target) {
						const auto first = std::ranges::find_if(items, &NavSpot::visible);
						target = std::addressof(first != items.end() ? *first : items.front());
					}
				}
				break;
			default:
				break;
			}
		} else {
			// The focus is on nothing the window placed (or nothing at all): start from the open page's
			// sidebar row, or the page's first entry or control, rather than wherever ImGui lands
			const auto pageControl = std::ranges::find_if(_controlSpotsLast, [](const NavSpot& a_spot) { return a_spot.visible && a_spot.section != -1; });
			const auto entry = std::ranges::find_if(_entrySpotsLast, &NavSpot::visible);
			if (const auto* selected = find(_sidebarSpotsLast, _sidebarSelectedLast); selected && selected->visible) {
				target = selected;
			} else if (entry != _entrySpotsLast.end()) {
				target = std::addressof(*entry);
			} else if (pageControl != _controlSpotsLast.end()) {
				target = std::addressof(*pageControl);
			}
			handled = target != nullptr;
		}

		// Straight back the way it came: Down then Up returns to the same control, whichever is nearest.
		// It may have scrolled out of view in the same panel; focusing it scrolls it back
		const auto opposite = dir == ImGuiDir_Up ? ImGuiDir_Down : dir == ImGuiDir_Down ? ImGuiDir_Up : dir == ImGuiDir_Left ? ImGuiDir_Right : ImGuiDir_Left;
		if (handled && _lastMoveTo == current && _lastMoveDir == static_cast<int>(opposite)) {
			const NavSpot* now = nullptr;
			for (const auto* list : { &_entrySpotsLast, &_controlSpotsLast, &_sidebarSpotsLast }) {
				now = now ? now : find(*list, current);
			}
			for (const auto* list : { &_entrySpotsLast, &_controlSpotsLast, &_sidebarSpotsLast }) {
				if (const auto* back = find(*list, _lastMoveFrom); back && (back->visible || (now && back->window == now->window))) {
					target = back;
				}
			}
		}
		_lastMoveFrom = handled && target ? current : 0;
		_lastMoveTo = handled && target ? target->id : 0;
		_lastMoveDir = static_cast<int>(dir);

		lastRoute = std::string(handled ? "routed" : "imgui") + (target ? " to " + std::to_string(target->section) + "/" + std::to_string(target->index) : " nowhere");
		if (!handled) {
			return;
		}
		ImGui::NavMoveRequestCancel();
		// The press shows the focus box even at the end of a list
		ImGui::SetNavCursorVisible(true);
		if (!target && g.NavId == 0) {
			// End of a list, stay put
			_pendingFocus = current;
		}
		if (target) {
			_pendingFocus = target->id;
			if (find(_entrySpotsLast, target->id)) {
				_lastEntryFocus = target->id;
			}
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Scroll Focus With Right Stick ) ---
	//---------------------------------------------------

	void Menu::ScrollFocusWithRightStick() {

		const float amount = RightStickAmount();
		if (rightStickTaken || amount == 0.0f) {
			return;
		}
		auto&        g = *ImGui::GetCurrentContext();
		ImGuiWindow* window = g.IO.NavVisible && g.NavWindow ? g.NavWindow : g.HoveredWindow;
		while (window && window->ScrollMax.y <= 0.0f && (window->Flags & ImGuiWindowFlags_ChildWindow) && window->ParentWindow) {
			window = window->ParentWindow;
		}
		if (window && window->ScrollMax.y > 0.0f) {
			ImGui::SetScrollY(window, window->Scroll.y + amount * ImGui::GetFontSize() * 35.0f * g.IO.DeltaTime);
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Track Input Device ) --------
	//---------------------------------------------------

	void Menu::TrackInputDevice() {

		// Controller or mouse, whichever was used last
		auto& io = ImGui::GetIO();
		for (int key = ImGuiKey_GamepadStart; key <= ImGuiKey_GamepadRStickDown; ++key) {
			if (ImGui::IsKeyDown(static_cast<ImGuiKey>(key)) || (key == ImGuiKey_GamepadStart && ImGui::IsKeyDown(rowActionKey))) {
				if (!_padActive) {
					io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
					_pinnedEntry.reset();
					_hoveredEntry.reset();
				}
				_padActive = true;
				return;
			}
		}
		if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f || ImGui::IsAnyMouseDown() || io.MouseWheel != 0.0f) {
			if (_padActive) {
				ImGui::SetNavCursorVisible(false);
				_pinnedEntry.reset();
			}
			_padActive = false;
		}
	}

	namespace
	{
		enum class PadButton
		{
			kFaceDown,
			kFaceRight,
			kFaceLeft,
			kFaceUp,
			kLeftBumper,
			kRightBumper,
			kLeftStickPress,
			kRightStick,
			kView,
			kLeftTrigger,
			kRightTrigger,
		};

		//---------------------------------------------------
		//-- Completionist UI ( X Input Mask ) --------------
		//---------------------------------------------------

		constexpr std::uint32_t XInputMask(PadButton a_button) {

			switch (a_button) {
			case PadButton::kFaceDown: return 0x1000;
			case PadButton::kFaceRight: return 0x2000;
			case PadButton::kFaceLeft: return 0x4000;
			case PadButton::kFaceUp: return 0x8000;
			case PadButton::kLeftBumper: return 0x0100;
			case PadButton::kRightBumper: return 0x0200;
			case PadButton::kLeftStickPress: return 0x0040;
			case PadButton::kRightStick: return 0x0080;
			default: return 0x0020;
			}
		}

		//---------------------------------------------------
		//-- Completionist UI ( Is Face ) -------------------
		//---------------------------------------------------

		bool IsFace(PadButton a_button) {

			return a_button <= PadButton::kFaceUp;
		}

		struct Ink
		{
			ImVec2 min;
			ImVec2 max;
		};

		//---------------------------------------------------
		//-- Completionist UI ( Glyph Ink ) -----------------
		//---------------------------------------------------

		// Glyph pixels from the font texture
		bool GlyphInk(const ImFontAtlas* a_atlas, const ImFontGlyph* a_glyph, Ink& a_ink) {

			const int width = a_atlas->TexWidth;
			const int height = a_atlas->TexHeight;
			if (width <= 0 || height <= 0 || (!a_atlas->TexPixelsAlpha8 && !a_atlas->TexPixelsRGBA32)) {
				return false;
			}
			const int left = static_cast<int>(std::lround(a_glyph->U0 * width));
			const int right = static_cast<int>(std::lround(a_glyph->U1 * width));
			const int top = static_cast<int>(std::lround(a_glyph->V0 * height));
			const int bottom = static_cast<int>(std::lround(a_glyph->V1 * height));
			if (right <= left || bottom <= top) {
				return false;
			}
			const auto alpha = [&](int a_x, int a_y) -> unsigned {
				const auto index = static_cast<std::size_t>(a_y) * width + a_x;
				return a_atlas->TexPixelsAlpha8 ? a_atlas->TexPixelsAlpha8[index] : (a_atlas->TexPixelsRGBA32[index] >> IM_COL32_A_SHIFT) & 0xFF;
			};
			int minX = right, minY = bottom, maxX = left - 1, maxY = top - 1;
			for (int y = top; y < bottom; ++y) {
				for (int x = left; x < right; ++x) {
					if (alpha(x, y) >= 128) {
						minX = std::min(minX, x);
						maxX = std::max(maxX, x);
						minY = std::min(minY, y);
						maxY = std::max(maxY, y);
					}
				}
			}
			if (maxX < minX) {
				return false;
			}
			const float scaleX = (a_glyph->X1 - a_glyph->X0) / static_cast<float>(right - left);
			const float scaleY = (a_glyph->Y1 - a_glyph->Y0) / static_cast<float>(bottom - top);
			a_ink.min = ImVec2(a_glyph->X0 + (minX - left) * scaleX, a_glyph->Y0 + (minY - top) * scaleY);
			a_ink.max = ImVec2(a_glyph->X0 + (maxX + 1 - left) * scaleX, a_glyph->Y0 + (maxY + 1 - top) * scaleY);
			return true;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Small Text Ink ) ------------
		//---------------------------------------------------

		Ink SmallTextInk(std::string_view a_text) {

			ImGui::PushFont(Themes::SmallFont());
			ImFont*       font = ImGui::GetFont();
			const float   scale = ImGui::GetFontSize() / font->FontSize;
			Ink           ink{ ImVec2(FLT_MAX, FLT_MAX), ImVec2(-FLT_MAX, -FLT_MAX) };
			float         x = 0.0f;
			for (const char c : a_text) {
				const auto* glyph = font->FindGlyph(static_cast<ImWchar>(static_cast<unsigned char>(c)));
				if (!glyph) {
					continue;
				}
				if (glyph->Visible) {
					Ink glyphInk{ ImVec2(glyph->X0, glyph->Y0), ImVec2(glyph->X1, glyph->Y1) };
					GlyphInk(font->ContainerAtlas, glyph, glyphInk);
					ink.min = ImVec2(std::min(ink.min.x, x + glyphInk.min.x * scale), std::min(ink.min.y, glyphInk.min.y * scale));
					ink.max = ImVec2(std::max(ink.max.x, x + glyphInk.max.x * scale), std::max(ink.max.y, glyphInk.max.y * scale));
				}
				x += glyph->AdvanceX * scale;
			}
			if (ink.min.x > ink.max.x) {
				ink = { ImVec2(0.0f, 0.0f), ImVec2(x, ImGui::GetFontSize()) };
			}
			ImGui::PopFont();
			return ink;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Pad Button Name ) -----------
		//---------------------------------------------------

		std::string PadButtonName(PadButton a_button, PadStyle a_style) {

			if (a_button == PadButton::kLeftTrigger || a_button == PadButton::kRightTrigger) {
				const bool left = a_button == PadButton::kLeftTrigger;
				switch (a_style) {
				case PadStyle::kPlayStation:
				case PadStyle::kSteamDeck:
					return left ? "L2" : "R2";
				case PadStyle::kSwitch:
					return left ? "ZL" : "ZR";
				default:
					return left ? "LT" : "RT";
				}
			}
			return a_button == PadButton::kRightStick ? std::string("RS") : GamepadButtonName(XInputMask(a_button), a_style);
		}

		//---------------------------------------------------
		//-- Completionist UI ( Pad Button Width ) ----------
		//---------------------------------------------------

		float PadButtonWidth(PadButton a_button, PadStyle a_style, float a_height) {

			if (IsFace(a_button)) {
				return a_height;
			}
			const auto  ink = SmallTextInk(PadButtonName(a_button, a_style));
			const float inside = ink.max.y - ink.min.y;
			const float margin = (a_height - inside) * 0.5f + a_height * 0.08f;
			return std::round(std::max(a_height * 1.25f, ink.max.x - ink.min.x + margin * 2.0f));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Draw Pad Button ) -----------
		//---------------------------------------------------

		// Controller button glyph for the button help
		float DrawPadButton(ImDrawList* a_drawList, ImVec2 a_pos, float a_height, PadButton a_button, PadStyle a_style) {

			const auto& palette = Themes::GetPalette();
			const ImU32 colour = Colour(palette.text, 0.9f);
			const float stroke = std::max(1.2f, ImGui::GetFontSize() * 0.075f);
			const float radius = a_height * 0.5f - stroke * 0.5f;
			const float width = PadButtonWidth(a_button, a_style, a_height);
			const auto label = [&](const std::string& a_text, ImVec2 a_centre) {
				const auto ink = SmallTextInk(a_text);
				const auto inkCentre = ImVec2((ink.min.x + ink.max.x) * 0.5f, (ink.min.y + ink.max.y) * 0.5f);
				const auto pen = ImVec2(std::round(a_centre.x - inkCentre.x), std::round(a_centre.y - inkCentre.y));
				ImGui::PushFont(Themes::SmallFont());
				a_drawList->AddText(pen, colour, a_text.c_str());
				ImGui::PopFont();
				return ImVec2(pen.x + inkCentre.x - a_centre.x, pen.y + inkCentre.y - a_centre.y);
			};

			if (!IsFace(a_button)) {
				const auto   name = PadButtonName(a_button, a_style);
				const ImVec2 middle(a_pos.x + width * 0.5f, a_pos.y + a_height * 0.5f);
				ImVec2       shift(0.0f, 0.0f);
				if (name != "-" && name != "+") {
					shift = label(name, middle);
				}
				a_drawList->AddRect(ImVec2(a_pos.x + shift.x + stroke * 0.5f, a_pos.y + shift.y + stroke * 0.5f), ImVec2(a_pos.x + shift.x + width - stroke * 0.5f, a_pos.y + shift.y + a_height - stroke * 0.5f), colour, a_height * 0.3f, 0, stroke);
				if (name == "-" || name == "+") {
					// Switch minus and plus
					const float arm = a_height * 0.22f;
					a_drawList->AddLine(ImVec2(middle.x - arm, middle.y), ImVec2(middle.x + arm, middle.y), colour, stroke);
					if (name == "+") {
						a_drawList->AddLine(ImVec2(middle.x, middle.y - arm), ImVec2(middle.x, middle.y + arm), colour, stroke);
					}
				}
				return width;
			}

			const ImVec2 centre(a_pos.x + a_height * 0.5f, a_pos.y + a_height * 0.5f);
			if (a_style != PadStyle::kPlayStation) {
				const auto shift = label(GamepadButtonName(XInputMask(a_button), a_style), centre);
				a_drawList->AddCircle(ImVec2(centre.x + shift.x, centre.y + shift.y), radius, colour, 32, stroke);
				return width;
			}
			a_drawList->AddCircle(centre, radius, colour, 32, stroke);

			// PlayStation symbols
			const float s = radius * 0.48f;
			switch (a_button) {
			case PadButton::kFaceDown:
				a_drawList->AddLine(ImVec2(centre.x - s, centre.y - s), ImVec2(centre.x + s, centre.y + s), colour, stroke);
				a_drawList->AddLine(ImVec2(centre.x - s, centre.y + s), ImVec2(centre.x + s, centre.y - s), colour, stroke);
				break;
			case PadButton::kFaceRight:
				a_drawList->AddCircle(centre, s * 1.05f, colour, 20, stroke);
				break;
			case PadButton::kFaceLeft:
				a_drawList->AddRect(ImVec2(centre.x - s * 0.9f, centre.y - s * 0.9f), ImVec2(centre.x + s * 0.9f, centre.y + s * 0.9f), colour, 0.0f, 0, stroke);
				break;
			default:
				a_drawList->AddTriangle(ImVec2(centre.x, centre.y - s * 1.05f), ImVec2(centre.x + s * 1.05f, centre.y + s * 0.75f), ImVec2(centre.x - s * 1.05f, centre.y + s * 0.75f), colour, stroke);
				break;
			}
			return width;
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Pad Hints Height ) ----------
	//---------------------------------------------------

	float Menu::PadHintsHeight() const {

		return PadHintsShown() ? ImGui::GetFrameHeight() + ImGui::GetFontSize() * 0.6f : 0.0f;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Pad Hints ) ------------
	//---------------------------------------------------

	void Menu::DrawPadHints(IDataProvider& a_provider, bool a_cellWindow) {

		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();
		const float height = PadHintsHeight();
		auto*       drawList = ImGui::GetWindowDrawList();
		const auto  start = ImGui::GetCursorScreenPos();
		const float width = ImGui::GetContentRegionAvail().x;

		Decor::Rule(drawList, ImVec2(start.x + width * 0.1f, start.y + 1.0f), ImVec2(start.x + width * 0.9f, start.y + 1.0f), Colour(palette.separator));

		const auto style = _settings ? _settings->padStyle : PadStyle::kXbox;
		std::vector<std::pair<std::vector<PadButton>, std::string>> hints;
		hints.push_back({ { PadButton::kFaceDown }, a_provider.Translate("UI_Pad_Select") });
		if (!_padActionKey.empty()) {
			hints.push_back({ { PadButton::kFaceLeft }, a_provider.Translate(_padActionKey) });
		}
		if (!_padTriggerKey.empty()) {
			hints.push_back({ { PadButton::kRightTrigger }, a_provider.Translate(_padTriggerKey) });
		}
		if (a_cellWindow) {
			// Cell Scanner
			hints.push_back({ { PadButton::kFaceRight }, a_provider.Translate("UI_Pad_Close") });
			hints.push_back({ { PadButton::kRightStick }, a_provider.Translate("UI_Pad_Scroll") });
		} else {
		hints.push_back({ { PadButton::kFaceRight }, a_provider.Translate(ImGui::GetIO().NavVisible && !_sidebarFocused ? "UI_Pad_Back" : "UI_Pad_Close") });
		hints.push_back({ { PadButton::kLeftBumper, PadButton::kRightBumper }, a_provider.Translate("UI_Pad_Pages") });
		if (const auto tabKeys = std::exchange(_tabsShown, TabKeys::kNone); tabKeys != TabKeys::kNone) {
			if (tabKeys == TabKeys::kLeft) {
				hints.push_back({ { PadButton::kLeftTrigger }, a_provider.Translate("UI_Pad_Tabs") });
			} else {
				hints.push_back({ { PadButton::kLeftTrigger, PadButton::kRightTrigger }, a_provider.Translate("UI_Pad_Tabs") });
			}
		}
		hints.push_back({ { PadButton::kFaceUp }, a_provider.Translate("UI_Pad_Description") });
		hints.push_back({ { PadButton::kFaceUp }, a_provider.Translate(_settings && _settings->descriptionCollapsed ? "UI_Pad_DescriptionHoldExpand" : "UI_Pad_DescriptionHoldCollapse") });
		hints.push_back({ { PadButton::kLeftStickPress }, a_provider.Translate("UI_Pad_Sidebar") });
		hints.push_back({ { PadButton::kRightStick }, a_provider.Translate("UI_Pad_Scroll") });
		hints.push_back({ { PadButton::kView }, a_provider.Translate("UI_Pad_Find") });
		}

		const float button = std::floor(fontSize * 1.15f);
		const float gap = fontSize * 0.35f;
		const float spacing = fontSize * 1.6f;
		const auto  measure = [&](const std::vector<PadButton>& a_buttons, const std::string& a_label) {
			float total = ImGui::CalcTextSize(a_label.c_str()).x;
			for (const auto name : a_buttons) {
				total += PadButtonWidth(name, style, button) + gap;
			}
			return total;
		};
		float total = 0.0f;
		for (const auto& [buttons, label] : hints) {
			total += measure(buttons, label) + spacing;
		}
		total -= spacing;
		// The least needed go first in a narrow window, rather than running off the edge
		while (total > width && hints.size() > 1) {
			total -= measure(hints.back().first, hints.back().second) + spacing;
			hints.pop_back();
		}

		float      x = start.x + std::max(0.0f, (width - total) * 0.5f);
		const auto centreY = start.y + height * 0.5f + 1.0f;
		drawList->PushClipRect(ImVec2(start.x, start.y), ImVec2(start.x + width, start.y + height), true);
		for (const auto& [buttons, label] : hints) {
			for (const auto name : buttons) {
				x += DrawPadButton(drawList, ImVec2(std::floor(x), std::floor(centreY - button * 0.5f)), button, name, style) + gap;
			}
			drawList->AddText(ImVec2(x, centreY - fontSize * 0.5f), Colour(palette.textDim), label.c_str());
			x += ImGui::CalcTextSize(label.c_str()).x + spacing;
		}
		drawList->PopClipRect();
		ImGui::Dummy(ImVec2(width, height));
	}

	//---------------------------------------------------
	//-- Completionist UI ( Update Navigation Open ) ----
	//---------------------------------------------------

	float Menu::UpdateNavigationOpen() {

		const bool open = !(_settings && _settings->navigationCollapsed);
		if (_navigationOpen < 0.0f) {
			_navigationOpen = open ? 1.0f : 0.0f;
		}
		const float step = ImGui::GetIO().DeltaTime * 5.0f;
		_navigationOpen = open ? std::min(1.0f, _navigationOpen + step) : std::max(0.0f, _navigationOpen - step);
		const float t = _navigationOpen;
		return t * t * (3.0f - 2.0f * t);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Navigation Grip ) ------
	//---------------------------------------------------

	void Menu::DrawNavigationGrip(float a_gap) {

		// Sidebar grabber, double-click to fold it
		const auto& palette = Themes::GetPalette();
		const auto  pos = ImGui::GetCursorScreenPos();
		const float height = ImGui::GetContentRegionAvail().y;
		const float lineX = std::floor(pos.x + a_gap * 0.35f);
		const float centreY = pos.y + height * 0.5f;
		const float gripHeight = ImGui::GetFontSize() * 4.0f;
		auto*       drawList = ImGui::GetWindowDrawList();

		ImGui::SetCursorScreenPos(ImVec2(lineX - a_gap * 0.5f, centreY - gripHeight * 0.5f));
		ImGui::InvisibleButton("##NavigationGrip", ImVec2(a_gap, gripHeight));
		const bool hovered = ImGui::IsItemHovered();
		if (hovered && _settings && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			_settings->navigationCollapsed = !_settings->navigationCollapsed;
			_settingsChanged = true;
		}

		const auto verticalFade = [&](float a_x, float a_top, float a_bottom, ImU32 a_colour, float a_thickness) {
			const float mid = (a_top + a_bottom) * 0.5f;
			const ImU32 clear = a_colour & ~IM_COL32_A_MASK;
			const float left = a_x - a_thickness * 0.5f;
			const float right = a_x + a_thickness * 0.5f;
			drawList->AddRectFilledMultiColor(ImVec2(left, a_top), ImVec2(right, mid), clear, clear, a_colour, a_colour);
			drawList->AddRectFilledMultiColor(ImVec2(left, mid), ImVec2(right, a_bottom), a_colour, a_colour, clear, clear);
		};
		const ImU32 colour = Colour(palette.accent, hovered ? 1.0f : 0.6f);
		if (SkyUI()) {
			drawList->AddRectFilled(ImVec2(lineX, pos.y), ImVec2(lineX + 1.0f, pos.y + height), Colour(palette.separator));
			const float knot = ImGui::GetFontSize() * 0.5f;
			drawList->AddRectFilled(ImVec2(lineX - knot, centreY - knot), ImVec2(lineX + knot + 1.0f, centreY + knot), Colour(ImVec4(0, 0, 0, 1)));
			Decor::Knot(drawList, ImVec2(lineX + 0.5f, centreY), knot, colour);
			ImGui::SetCursorScreenPos(ImVec2(pos.x + a_gap, pos.y));
			return;
		}
		verticalFade(lineX + 0.5f, pos.y, pos.y + height, Colour(palette.separator), 1.0f);
		const float half = gripHeight * 0.5f;
		const float offset = ImGui::GetFontSize() * 0.22f;
		const float thickness = std::max(1.5f, ImGui::GetFontSize() * 0.08f);
		verticalFade(lineX - offset, centreY - half, centreY + half, colour, thickness);
		verticalFade(lineX + offset, centreY - half * 0.6f, centreY + half * 0.6f, colour, thickness);
		Decor::Diamond(drawList, ImVec2(lineX, centreY), ImGui::GetFontSize() * 0.3f, colour, true);

		ImGui::SetCursorScreenPos(ImVec2(pos.x + a_gap, pos.y));
	}

	//---------------------------------------------------
	//-- Completionist UI ( Open Page ) -----------------
	//---------------------------------------------------

	void Menu::OpenPage(std::string_view a_id) {

		_focusKey.clear();
		_restoreFocusKey.clear();
		_selectedPage = a_id;
		_filter.fill('\0');
		_pinnedEntry.reset();
		_hoveredEntry.reset();
		_pendingScale = -1.0f;
		_dirty = true;
		_findFocus = a_id == findPage;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Open Find ) -----------------
	//---------------------------------------------------

	void Menu::OpenFind(std::string_view a_text) {

		OpenPage(findPage);
		_findText.fill('\0');
		a_text.copy(_findText.data(), std::min(a_text.size(), _findText.size() - 1));
	}

	//---------------------------------------------------
	//-- Completionist UI ( Show Status ) ---------------
	//---------------------------------------------------

	void Menu::ShowStatus(std::string a_text, float a_seconds) {

		_status = std::move(a_text);
		_statusTime = a_seconds;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Compute Scale ) -------------
	//---------------------------------------------------

	float Menu::ComputeScale(const UISettings& a_settings, float a_displayHeight) noexcept {

		return a_settings.scale * std::clamp(a_displayHeight / 1080.0f, 0.75f, 4.0f);
	}

	//---------------------------------------------------
	//-- Menu ( Header ) --------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Header ) ---------------
	//---------------------------------------------------

	void Menu::DrawHeader(IDataProvider& a_provider, bool& a_open) {

		const auto& palette = Themes::GetPalette();

		std::int32_t found = 0;
		std::int32_t total = 0;
		for (const auto& category : _categories) {
			found += category.Found();
			total += category.Total();
		}

		const auto title = a_provider.Translate("UI_Title");
		const auto overall = a_provider.Translate("UI_Overall") + "  " + CountText(found, total) + "  (" + PercentText(found, total) + ")";
		auto*       drawList = ImGui::GetWindowDrawList();
		const float width = ImGui::GetContentRegionAvail().x;
		const auto  origin = ImGui::GetCursorScreenPos();

		ImGui::PushFont(Themes::TitleFont());
		const auto  shownTitle = SkyUI() ? Upper(title) : title;
		const auto  titleSize = ImGui::CalcTextSize(shownTitle.c_str());
		const float titleX = origin.x + (width - titleSize.x) * 0.5f;
		const float lineY = origin.y + titleSize.y * 0.55f;
		float edge = 0.0f;
		if (SkyUI()) {
			const float pad = ImGui::GetFontSize() * 0.9f;
			const float top = origin.y - ImGui::GetFontSize() * 0.1f;
			const float bottom = origin.y + titleSize.y + ImGui::GetFontSize() * 0.05f;
			Decor::Banner(drawList, ImVec2(titleX - pad, top), ImVec2(titleX + titleSize.x + pad, bottom), Colour(ImVec4(0.07f, 0.07f, 0.07f, 0.95f)), Colour(palette.separator));
			edge = pad + (bottom - top) * 0.5f;
		}
		drawList->AddText(ImVec2(titleX, origin.y), Colour(palette.text), shownTitle.c_str());
		ImGui::PopFont();
		const float gap = ImGui::GetFontSize() * 1.2f;

		Decor::Rule(drawList, ImVec2(origin.x + width * 0.12f, lineY), ImVec2(titleX - edge - gap, lineY), Colour(palette.separator));
		Decor::Rule(drawList, ImVec2(titleX + titleSize.x + edge + gap, lineY), ImVec2(origin.x + width * 0.88f, lineY), Colour(palette.separator));
		Decor::Ornament(drawList, ImVec2(titleX - edge - gap * 0.5f, lineY), gap * 0.18f, Colour(palette.accent));
		Decor::Ornament(drawList, ImVec2(titleX + titleSize.x + edge + gap * 0.5f, lineY), gap * 0.18f, Colour(palette.accent));

		const auto overallSize = ImGui::CalcTextSize(overall.c_str());
		drawList->AddText(ImVec2(origin.x + (width - overallSize.x) * 0.5f, origin.y + titleSize.y + 2.0f), Colour(palette.textDim), overall.c_str());

		DrawHeaderButtons(a_provider, a_open, origin.x + width, origin.y);

		ImGui::SetCursorScreenPos(origin);
		if (ThemedButton((a_provider.Translate("UI_Find") + "##Find").c_str())) {
			OpenPage(findPage);
		}
		ImGui::SameLine();
		if (ThemedButton((a_provider.Translate("UI_Cell_Button") + "##CellView").c_str())) {
			_cellView = true;
			_focusCellList = true;
		}

		ImGui::SetCursorScreenPos(ImVec2(origin.x, origin.y + titleSize.y + overallSize.y + ImGui::GetFontSize() * 0.5f));
		ImGui::Spacing();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Header Buttons ) -------
	//---------------------------------------------------

	void Menu::DrawHeaderButtons(IDataProvider& a_provider, bool& a_open, float a_right, float a_y) {

		const auto& style = ImGui::GetStyle();
		const auto  closeLabel = a_provider.Translate("UI_Close");
		const auto  shotLabel = a_provider.Translate("UI_Screenshot");
		const float cogSide = ImGui::GetFrameHeight();
		float width = ButtonWidth(closeLabel.c_str()) + cogSide + style.ItemSpacing.x;
		if (_showScreenshotButton) {
			width += ButtonWidth(shotLabel.c_str()) + style.ItemSpacing.x;
		}

		const float statusWidth = _statusTime > 0.0f ? ImGui::CalcTextSize(_status.c_str()).x + style.ItemSpacing.x : 0.0f;
		ImGui::SetCursorScreenPos(ImVec2(a_right - width - statusWidth, a_y));

		if (statusWidth > 0.0f) {
			ImGui::AlignTextToFramePadding();
			ImGui::TextColored(Themes::GetPalette().textDim, "%s", _status.c_str());
			ImGui::SameLine();
		}
		if (_showScreenshotButton) {
			if (ThemedButton(shotLabel.c_str())) {
				_screenshotRequested = true;
			}
			ImGui::SameLine();
		}

		// Settings cog beside Close, lit while Settings is open
		if (ThemedButton("##SettingsCog", cogSide)) {
			OpenPage(settingsPage);
		}
		{
			const auto& palette = Themes::GetPalette();
			const bool  lit = ImGui::IsItemHovered() || ImGui::IsItemFocused() || _selectedPage == settingsPage;
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			Decor::DrawIcon(ImGui::GetWindowDrawList(), Icon::kSettings, ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f), ImGui::GetFontSize() * 0.9f, Colour(lit ? palette.accent : palette.needed));
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("%s", a_provider.Translate("UI_Settings").c_str());
			}
		}
		ImGui::SameLine();
		if (ThemedButton(closeLabel.c_str())) {
			a_open = false;
		}
	}

	//---------------------------------------------------
	//-- Menu ( Navigation ) ----------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Navigation ) -----------
	//---------------------------------------------------

	void Menu::DrawNavigation(IDataProvider& a_provider) {

		const auto& palette = Themes::GetPalette();
		const float rowHeight = ImGui::GetFontSize() * 1.7f;
		const float padding = ImGui::GetStyle().FramePadding.x;
		const float iconSize = ImGui::GetFontSize() * 0.9f;

		bool       sidebarFocused = false;
		int        sidebarIndex = 0;
		const auto noteRow = [&](bool a_selected) {
			RecordSpot(_sidebarSpots, 0, sidebarIndex++, 1, 0);
			if (a_selected) {
				_sidebarSelectedID = ImGui::GetItemID();
			}
			sidebarFocused = sidebarFocused || ImGui::IsItemFocused();
			if (a_selected && _focusSidebar) {
				ImGui::SetFocusID(ImGui::GetItemID(), ImGui::GetCurrentWindow());
				ImGui::GetCurrentWindow()->NavRectRel[0] = ImGui::WindowRectAbsToRel(ImGui::GetCurrentWindow(), ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()));
				ImGui::SetNavCursorVisible(_showSidebarFocus);
				ImGui::SetScrollHereY(0.5f);
				_focusSidebar = false;
				sidebarFocused = true;
			}
		};

		const auto centredRow = [&](const char* a_id, const char* a_key, bool a_selected) {
			const bool  clicked = RowSelectable(a_id, a_selected, ImVec2(0.0f, rowHeight));
			noteRow(a_selected);
			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			auto*       drawList = ImGui::GetWindowDrawList();
			const auto  text = a_provider.Translate(a_key);
			ImGui::PushFont(Themes::HeaderFont());
			const auto  size = ImGui::CalcTextSize(text.c_str());
			const float centreX = (min.x + max.x) * 0.5f;
			const float centreY = (min.y + max.y) * 0.5f;
			drawList->AddText(ImVec2(centreX - size.x * 0.5f, centreY - size.y * 0.5f), Colour(palette.heading), text.c_str());
			ImGui::PopFont();
			const float gap = ImGui::GetFontSize() * 0.8f;
			const float glyph = ImGui::GetFontSize() * 0.2f;
			Decor::Ornament(drawList, ImVec2(centreX - size.x * 0.5f - gap, centreY), glyph, Colour(palette.accent));
			Decor::Ornament(drawList, ImVec2(centreX + size.x * 0.5f + gap, centreY), glyph, Colour(palette.accent));
			return clicked;
		};
		const auto divider = [&]() {
			ImGui::Spacing();
			DividerRow();
			ImGui::Spacing();
		};

		if (centredRow("##Overview", "UI_Overview", _selectedPage.empty())) {
			OpenPage({});
		}
		divider();

		const Category* lastCollections = nullptr;
		for (const auto& category : _categories) {
			if (category.Group() == "UI_Category_Collections") {
				lastCollections = std::addressof(category);
			}
		}

		for (const auto& category : _categories) {
			// Room for the last row's progress bar above the header
			if (std::addressof(category) != std::addressof(_categories.front())) {
				ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize() * 0.6f));
			}
			ImGui::Spacing();
			const auto title = a_provider.Translate(category.title);
			{
				auto*       drawList = ImGui::GetWindowDrawList();
				const auto  pos = ImGui::GetCursorScreenPos();
				const float width = ImGui::GetContentRegionAvail().x;
				const float lineHeight = ImGui::GetFontSize() * 1.5f;
				const float lineY = pos.y + lineHeight * 0.5f;
				ImGui::PushFont(Themes::HeaderFont());
				const auto textSize = ImGui::CalcTextSize(title.c_str());
				if (SkyUI()) {
					const float textX = pos.x + width - rowHeight * 0.5f - padding - textSize.x;
					drawList->AddText(ImVec2(textX, pos.y + (lineHeight - textSize.y) * 0.5f), Colour(palette.text), title.c_str());
					ImGui::PopFont();
					const float knot = ImGui::GetFontSize() * 0.42f;
					Decor::Knot(drawList, ImVec2(textX - padding - knot, lineY), knot, Colour(palette.text, 0.85f));
					Decor::DoubleRule(drawList, ImVec2(pos.x + padding, lineY), ImVec2(textX - padding - knot * 2.4f, lineY), Colour(palette.separator));
				} else {
					const float textX = pos.x + iconSize + padding;
					drawList->AddText(ImVec2(textX, pos.y + (lineHeight - textSize.y) * 0.5f), Colour(palette.heading), title.c_str());
					ImGui::PopFont();
					Decor::DrawIcon(drawList, Decor::IconFor(category.Group()), ImVec2(pos.x + iconSize * 0.5f, lineY), iconSize, Colour(palette.heading));
					Decor::Rule(drawList, ImVec2(textX + textSize.x + padding, lineY), ImVec2(pos.x + width, lineY), Colour(palette.separator));
				}
				ImGui::Dummy(ImVec2(width, lineHeight));
			}

			for (const auto& page : category.pages) {
				ImGui::PushID(page.id.c_str());
				if (RowSelectable("##Page", _selectedPage == page.id, ImVec2(0.0f, rowHeight))) {
					OpenPage(page.id);
				}
				noteRow(_selectedPage == page.id);

				const auto  min = ImGui::GetItemRectMin();
				const auto  max = ImGui::GetItemRectMax();
				auto*       drawList = ImGui::GetWindowDrawList();
				const float textY = min.y + (rowHeight - ImGui::GetFontSize()) * 0.4f;
				const auto  count = CountText(page.found, page.total);
				const auto  countSize = ImGui::CalcTextSize(count.c_str());
				const bool  complete = page.total > 0 && page.found >= page.total;
				const float textX = min.x + padding + (iconSize + padding);
				const auto  name = a_provider.Translate(page.title);

				if (SkyUI()) {
					const float right = max.x - rowHeight * 0.5f - padding;
					const float countRight = min.x + padding + countSize.x;
					const auto  shown = FitText(name, right - countRight - padding * 2.0f);
					drawList->AddText(ImVec2(min.x + padding, textY), Colour(complete ? palette.found : palette.textDim), count.c_str());
					drawList->AddText(ImVec2(right - ImGui::CalcTextSize(shown.c_str()).x, textY), Colour(palette.text), shown.c_str());
					ThinBar(drawList, ImVec2(min.x + padding, max.y - 2.0f), ImVec2(right, max.y - 1.0f), Fraction(page.found, page.total));
				} else {
					drawList->AddText(ImVec2(textX, textY), Colour(palette.text), FitText(name, max.x - countSize.x - padding * 2.0f - textX).c_str());
					drawList->AddText(ImVec2(max.x - countSize.x - padding, textY), Colour(complete ? palette.found : palette.count), count.c_str());
					ThinBar(drawList, ImVec2(textX, max.y - 3.0f), ImVec2(max.x - padding, max.y - 2.0f), Fraction(page.found, page.total));
				}
				ImGui::PopID();
			}

			// Kill Counts has no progress bar, its count is kills
			if (std::addressof(category) == lastCollections) {
				if (RowSelectable("##Kills", _selectedPage == killsPage, ImVec2(0.0f, rowHeight))) {
					OpenPage(killsPage);
				}
				noteRow(_selectedPage == killsPage);
				const auto  min = ImGui::GetItemRectMin();
				const auto  max = ImGui::GetItemRectMax();
				auto*       drawList = ImGui::GetWindowDrawList();
				const float textY = min.y + (rowHeight - ImGui::GetFontSize()) * 0.4f;
				std::int64_t kills = 0;
				for (const auto& record : _kills) {
					kills += record.kills;
				}
				const auto  count = std::to_string(kills);
				const auto  countSize = ImGui::CalcTextSize(count.c_str());
				const float textX = min.x + padding + (iconSize + padding);
				const auto  name = a_provider.Translate("UI_Kills_Title");
				if (SkyUI()) {
					const float right = max.x - rowHeight * 0.5f - padding;
					drawList->AddText(ImVec2(min.x + padding, textY), Colour(palette.textDim), count.c_str());
					drawList->AddText(ImVec2(right - ImGui::CalcTextSize(name.c_str()).x, textY), Colour(palette.text), name.c_str());
				} else {
					drawList->PushClipRect(min, ImVec2(max.x - countSize.x - padding * 2.0f, max.y), true);
					drawList->AddText(ImVec2(textX, textY), Colour(palette.text), name.c_str());
					drawList->PopClipRect();
					drawList->AddText(ImVec2(max.x - countSize.x - padding, textY), Colour(palette.count), count.c_str());
				}
			}
		}

		// The last divider closes the list; Settings is the cog beside Close
		divider();
		if (_focusSidebar && !_sidebarSpots.empty()) {
			if (_showSidebarFocus) {
				_pendingFocus = _sidebarSpots.front().id;
			}
			_focusSidebar = false;
		}
		_sidebarFocused = sidebarFocused;
	}

	//---------------------------------------------------
	//-- Menu ( Overview ) ------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Overview ) -------------
	//---------------------------------------------------

	void Menu::DrawOverview(IDataProvider& a_provider) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();

		//---------------------------------------------------
		// Overview cards
		//---------------------------------------------------

		struct GroupTotal
		{
			std::string  title;
			std::int32_t found{ 0 };
			std::int32_t total{ 0 };
		};
		std::vector<GroupTotal> groups;
		for (const auto& category : _categories) {
			auto it = std::ranges::find(groups, category.Group(), &GroupTotal::title);
			if (it == groups.end()) {
				it = groups.insert(groups.end(), GroupTotal{ category.Group() });
			}
			it->found += category.Found();
			it->total += category.Total();
		}

		const int   cards = std::clamp(static_cast<int>(groups.size()), 1, 4);
		const float cardHeight = fontSize * 5.2f;
		if (ImGui::BeginTable("##Cards", cards, ImGuiTableFlags_SizingStretchSame)) {
			for (const auto& group : groups) {
				ImGui::TableNextColumn();
				const auto  min = ImGui::GetCursorScreenPos();
				const auto  max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + cardHeight);
				const float padding = fontSize * 0.8f;
				auto*       drawList = ImGui::GetWindowDrawList();

				drawList->AddRectFilled(min, max, Colour(palette.accent, 0.04f));
				drawList->AddRect(min, max, Colour(palette.separator, 0.7f));
				const float corner = fontSize * 0.18f;
				CornerMarks(drawList, min, max, corner, Colour(palette.accent, 0.6f));

				float y = min.y + padding * 0.7f;
				ImGui::PushFont(Themes::HeaderFont());
				drawList->AddText(ImVec2(min.x + padding, y), Colour(palette.heading), a_provider.Translate(group.title).c_str());
				y += ImGui::GetFontSize() + fontSize * 0.2f;
				ImGui::PopFont();

				ImGui::PushFont(Themes::TitleFont());
				const auto  percent = PercentText(group.found, group.total);
				const float percentHeight = ImGui::GetFontSize();
				drawList->AddText(ImVec2(min.x + padding, y), Colour(group.total > 0 && group.found >= group.total ? palette.found : palette.text), percent.c_str());
				ImGui::PopFont();

				const auto count = CountText(group.found, group.total);
				const auto countSize = ImGui::CalcTextSize(count.c_str());
				drawList->AddText(ImVec2(max.x - padding - countSize.x, y + percentHeight - countSize.y), Colour(palette.textDim), count.c_str());
				ThinBar(drawList, ImVec2(min.x + padding, max.y - padding * 0.7f - 2.0f), ImVec2(max.x - padding, max.y - padding * 0.7f), Fraction(group.found, group.total));

				ImGui::Dummy(ImVec2(max.x - min.x, cardHeight));
			}
			ImGui::EndTable();
		}

		ImGui::Dummy(ImVec2(0.0f, fontSize * 0.8f));

		//---------------------------------------------------
		// Closest to complete and the latest finds
		//---------------------------------------------------

		std::vector<const PageSummary*> closest;
		for (const auto& category : _categories) {
			for (const auto& page : category.pages) {
				if (page.total > 0 && page.found < page.total) {
					closest.push_back(std::addressof(page));
				}
			}
		}
		std::ranges::sort(closest, [](const PageSummary* a_left, const PageSummary* a_right) {
			const float left = Fraction(a_left->found, a_left->total);
			const float right = Fraction(a_right->found, a_right->total);
			return left != right ? left > right : (a_left->total - a_left->found) < (a_right->total - a_right->found);
		});
		constexpr std::size_t listLength = 10;
		if (closest.size() > listLength) {
			closest.resize(listLength);
		}

		const auto listHeading = [&](const char* a_key) {
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", a_provider.Translate(a_key).c_str());
			ImGui::PopFont();
			HeadingRule(4.0f);
		};
		const float rowHeight = ImGui::GetFrameHeight() + fontSize * 0.35f;

		if (ImGui::BeginTable("##Lists", 3, ImGuiTableFlags_None)) {
			ImGui::TableSetupColumn("##Closest", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("##Gutter", ImGuiTableColumnFlags_WidthFixed, fontSize * 2.5f);
			ImGui::TableSetupColumn("##Recent", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextColumn();
			listHeading("UI_Dashboard_Closest");
			if (closest.empty()) {
				ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Dashboard_AllDone").c_str());
			}
			for (const auto* page : closest) {
				ImGui::PushID(page->id.c_str());
				const bool clicked = RowSelectable("##Closest", false, ImVec2(0.0f, rowHeight));
				const auto min = ImGui::GetItemRectMin();
				const auto max = ImGui::GetItemRectMax();
				ImGui::PopID();

				auto*       drawList = ImGui::GetWindowDrawList();
				const float padding = style.FramePadding.x;
				const auto  count = CountText(page->found, page->total);
				const auto  countWidth = ImGui::CalcTextSize(count.c_str()).x;
				const float textY = min.y + (rowHeight - fontSize) * 0.4f;
				drawList->PushClipRect(min, ImVec2(max.x - countWidth - padding * 2.0f, max.y), true);
				drawList->AddText(ImVec2(min.x + padding, textY), Colour(palette.text), a_provider.Translate(page->title).c_str());
				drawList->PopClipRect();
				drawList->AddText(ImVec2(max.x - countWidth - padding, textY), Colour(palette.textDim), count.c_str());
				ThinBar(drawList, ImVec2(min.x + padding, max.y - 3.0f), ImVec2(max.x - padding, max.y - 1.0f), Fraction(page->found, page->total));
				if (clicked) {
					OpenPage(page->id);
				}
			}

			ImGui::TableNextColumn();
			ImGui::TableNextColumn();
			listHeading("UI_Dashboard_Recent");
			if (_recent.empty()) {
				ImGui::PushTextWrapPos(0.0f);
				ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Dashboard_NoRecent").c_str());
				ImGui::PopTextWrapPos();
			}

			// Today / yesterday instead of the date
			const auto dayText = [](std::int32_t a_daysAgo) {
				const auto time = std::time(nullptr) - static_cast<std::time_t>(a_daysAgo) * 24 * 60 * 60;
				std::tm    local{};
				localtime_s(&local, &time);
				char buffer[16]{};
				std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", &local);
				return std::string(buffer);
			};
			const auto today = dayText(0);
			const auto yesterday = dayText(1);

			for (const auto& event : _recent) {
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, rowHeight));
				const auto min = ImGui::GetItemRectMin();
				const auto max = ImGui::GetItemRectMax();

				const auto  day = event.date == today ? a_provider.Translate("UI_Today") : event.date == yesterday ? a_provider.Translate("UI_Yesterday") : event.date;
				const auto  when = day + "  " + event.time;
				auto*       drawList = ImGui::GetWindowDrawList();
				const float padding = style.FramePadding.x;
				const float whenWidth = ImGui::CalcTextSize(when.c_str()).x;
				const float textY = min.y + (rowHeight - fontSize) * 0.4f;
				drawList->PushClipRect(min, ImVec2(max.x - whenWidth - padding * 2.0f, max.y), true);
				drawList->AddText(ImVec2(min.x + padding, textY), Colour(palette.text), event.text.c_str());
				drawList->PopClipRect();
				drawList->AddText(ImVec2(max.x - whenWidth - padding, textY), Colour(palette.textDim), when.c_str());
				Decor::FadedLine(drawList, ImVec2(min.x, max.y - 1.0f), ImVec2(max.x, max.y - 1.0f), Colour(palette.separator, 0.5f));
			}
			ImGui::EndTable();
		}
	}

	//---------------------------------------------------
	//-- Menu ( Page ) ----------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Tab Row ) -------------------
	//---------------------------------------------------

	// Settings and patch sub-pages, LT / RT to switch
	// Pages keep RT for pinning, so LT cycles through the tabs there
	bool Menu::TabRow(const std::vector<TabItem>& a_tabs, std::string& a_selected, bool a_leftOnly) {

		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();
		const auto  before = a_selected;

		_tabsShown = a_leftOnly ? TabKeys::kLeft : TabKeys::kBoth;
		if (!ImGui::IsAnyItemActive() && !a_tabs.empty()) {
			const bool previous = !a_leftOnly && ImGui::IsKeyPressed(ImGuiKey_GamepadL2, false);
			const bool next = ImGui::IsKeyPressed(a_leftOnly ? ImGuiKey_GamepadL2 : ImGuiKey_GamepadR2, false);
			if (previous || next) {
				const auto current = std::ranges::find(a_tabs, a_selected, &TabItem::id);
				const auto count = static_cast<std::ptrdiff_t>(a_tabs.size());
				const auto index = (static_cast<std::ptrdiff_t>(current - a_tabs.begin()) + (next ? 1 : count - 1)) % count;
				a_selected = a_tabs[static_cast<std::size_t>(index)].id;
			}
		}

		const float countGap = fontSize * 0.4f;
		const auto  width = [&](const TabItem& a_tab) {
			const float count = a_tab.count.empty() ? 0.0f : ImGui::CalcTextSize(a_tab.count.c_str()).x + countGap;
			return ImGui::CalcTextSize(a_tab.title.c_str()).x + count;
		};
		float tabs = 0.0f;
		for (const auto& tab : a_tabs) {
			tabs += width(tab);
		}
		// Closer together when they don't fit, then onto more lines in a narrow window
		const float available = ImGui::GetContentRegionAvail().x;
		const float gaps = static_cast<float>(a_tabs.size() > 0 ? a_tabs.size() - 1 : 0);
		const float tabGap = tabs + gaps * fontSize * 1.6f <= available ? fontSize * 1.6f : fontSize * 0.9f;

		// Each line centred, wrapped ones too: the width of the line each tab starts
		std::vector<float> lineWidths(a_tabs.size(), -1.0f);
		for (std::size_t i = 0, start = 0; i < a_tabs.size(); ++i) {
			const float tab = width(a_tabs[i]);
			if (i > start && lineWidths[start] + tabGap + tab > available) {
				start = i;
			}
			lineWidths[start] = i == start ? tab : lineWidths[start] + tabGap + tab;
		}
		ImGui::Spacing();
		const float lineX = ImGui::GetCursorPosX();
		for (std::size_t i = 0; i < a_tabs.size(); ++i) {
			const auto& tab = a_tabs[i];
			if (lineWidths[i] >= 0.0f) {
				ImGui::SetCursorPosX(lineX + std::max(0.0f, (available - lineWidths[i]) * 0.5f));
			} else {
				ImGui::SameLine(0.0f, tabGap);
			}
			const auto size = ImGui::CalcTextSize(tab.title.c_str());
			ImGui::PushID(tab.id.c_str());
			if (ImGui::InvisibleButton("##Tab", ImVec2(width(tab), ImGui::GetFrameHeight()))) {
				a_selected = tab.id;
			}
			NoteControl();
			const bool selected = tab.id == a_selected;
			const bool focused = ImGui::IsItemFocused() && ImGui::GetIO().NavVisible;
			const bool hovered = ImGui::IsItemHovered() || focused;
			const auto min = ImGui::GetItemRectMin();
			const auto max = ImGui::GetItemRectMax();
			const float textY = min.y + (max.y - min.y - size.y) * 0.5f;
			auto*      drawList = ImGui::GetWindowDrawList();
			if (focused) {
				const float pad = fontSize * 0.4f;
				drawList->AddRect(ImVec2(min.x - pad, min.y), ImVec2(max.x + pad, max.y), Colour(palette.accent, 0.8f), fontSize * 0.2f, 0, 1.5f);
			}
			drawList->AddText(ImVec2(min.x, textY), Colour(selected ? palette.heading : (hovered ? palette.text : palette.textDim)), tab.title.c_str());
			if (!tab.count.empty()) {
				drawList->AddText(ImVec2(min.x + size.x + countGap, textY), Colour(palette.textDim), tab.count.c_str());
			}
			if (selected) {
				if (SkyUI()) {
					Decor::DoubleRule(drawList, ImVec2(min.x - tabGap * 0.25f, max.y), ImVec2(max.x + tabGap * 0.25f, max.y), Colour(palette.accent));
				} else {
					Decor::FadedLine(drawList, ImVec2(min.x - tabGap * 0.25f, max.y), ImVec2(max.x + tabGap * 0.25f, max.y), Colour(palette.accent), 1.5f);
				}
			}
			ImGui::PopID();
		}
		return a_selected != before;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Tab View ) ------------------
	//---------------------------------------------------

	// The page with only one tab's sections, kept until the page or tab changes
	const Page& Menu::TabView(const Page& a_page, std::int32_t a_tab) {

		const auto key = a_page.id + '\x1f' + std::to_string(a_tab) + '\x1f' + std::to_string(_pageLoads);
		if (key != _tabViewKey) {
			_tabViewKey = key;
			_tabView = Page{ a_page.id, a_page.title, {} };
			_tabView.layout = a_page.tabs[a_tab].layout;
			for (const auto& section : a_page.sections) {
				if (section.tab == a_tab) {
					_tabView.sections.push_back(section);
				}
			}
		}
		return _tabView;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Page ) -----------------
	//---------------------------------------------------

	void Menu::DrawPage(IDataProvider& a_provider) {

		const auto& palette = Themes::GetPalette();
		const auto& page = *_page;

		std::int32_t found = 0;
		std::int32_t total = 0;
		for (const auto& section : page.sections) {
			found += section.Found();
			total += section.Total();
		}

		HeadingWithCount(a_provider.Translate(page.title), CountText(found, total) + "  " + PercentText(found, total), palette.heading);
		HeadingRule(2.0f);
		ImGui::Spacing();

		// A kill list tab has its own filter and sort, and nothing to complete
		const auto tab = _pageTabs.find(page.id);
		const auto shownTab = tab != _pageTabs.end() && !tab->second.empty() ? std::atoi(tab->second.c_str()) : 0;
		const bool killsTab = shownTab >= 0 && shownTab < static_cast<int>(page.tabs.size()) && page.tabs[shownTab].layout == PageLayout::kKills;
		if (!killsTab) {
			ThemedFilterInput("##Filter", a_provider.Translate("UI_Filter").c_str(), _filter.data(), _filter.size(), ImGui::GetFontSize() * 14.0f);
			ImGui::SameLine(0.0f, ImGui::GetFontSize());
			MarkerToggle("##HideCompleted", _hideCompleted, a_provider.Translate("UI_HideCompleted").c_str());
			ImGui::SameLine(0.0f, ImGui::GetFontSize());

			const std::string sortLabels[] = {
				a_provider.Translate("UI_SortDefault"),
				a_provider.Translate("UI_SortName"),
				a_provider.Translate("UI_SortNeeded"),
			};
			auto sortIndex = static_cast<int>(_sort);
			if (ThemedBeginCombo("##Sort", sortLabels[sortIndex].c_str(), ImGui::GetFontSize() * 9.0f)) {
				for (int i = 0; i < 3; ++i) {
					if (ImGui::Selectable(sortLabels[i].c_str(), i == sortIndex)) {
						_sort = static_cast<SortMode>(i);
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine(0.0f, ImGui::GetFontSize() * 2.0f);
			{
				const float available = ImGui::GetContentRegionAvail().x;
				const auto  percent = PercentText(found, total);
				const float percentWidth = ImGui::CalcTextSize(percent.c_str()).x;
				const auto  rowStart = ImGui::GetCursorPos();

				// Only for patches with a dropdown, e.g. which faction's quests to show
				const auto& choice = page.choice;
				float       choiceWidth = 0.0f;
				if (!choice.options.empty()) {
					for (const auto& option : choice.options) {
						choiceWidth = std::max(choiceWidth, ImGui::CalcTextSize(option.c_str()).x);
					}
					choiceWidth += ImGui::GetStyle().FramePadding.x * 2.0f + ImGui::GetFontSize() * 1.5f;
				}
				const float choiceSpace = choiceWidth > 0.0f ? choiceWidth + ImGui::GetFontSize() * 1.5f : 0.0f;

				const float barWidth = std::max(0.0f, std::min(available - choiceSpace - percentWidth, std::max(ImGui::GetFontSize() * 10.0f, available * 0.4f)));
				const float barHeight = ImGui::GetFontSize() * 0.45f;
				const float barX = rowStart.x + available - barWidth;
				const float percentX = barX - percentWidth - ImGui::GetFontSize() * 0.6f;

				if (choiceWidth > 0.0f) {
					ImGui::SetCursorPos(ImVec2(percentX - choiceSpace, rowStart.y));
					const auto selected = std::clamp(choice.selected, 0, static_cast<std::int32_t>(choice.options.size()) - 1);
					const bool open = ThemedBeginCombo("##PageChoice", choice.options[selected].c_str(), choiceWidth);
					if (!open && ImGui::IsItemHovered() && !choice.name.empty()) {
						ImGui::SetTooltip("%s\n%s", choice.name.c_str(), choice.info.c_str());
					}
					if (open) {
						for (std::int32_t i = 0; i < static_cast<std::int32_t>(choice.options.size()); ++i) {
							if (ImGui::Selectable(choice.options[i].c_str(), i == selected) && i != selected) {
								a_provider.SetPageChoice(page.id, i);
								_page->choice.selected = i;
							}
						}
						ImGui::EndCombo();
					}
				}

				ImGui::SetCursorPos(ImVec2(percentX, rowStart.y));
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(total > 0 && found >= total ? palette.found : palette.textDim, "%s", percent.c_str());
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(barX, rowStart.y + (ImGui::GetFrameHeight() - barHeight) * 0.5f));
				ProgressBar(Fraction(found, total), barHeight);
			}

			DividerRow();
		}

		// Shown instead of the list, e.g. Civil War before picking a side
		if (!page.notice.empty()) {
			auto text = a_provider.Translate(page.notice);
			for (std::size_t pos; (pos = text.find("\\n")) != std::string::npos;) {
				text.replace(pos, 2, "\n");
			}
			ImGui::Spacing();
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(palette.textDim, "%s", text.c_str());
			ImGui::PopTextWrapPos();
			return;
		}

		// A patch's own sub-pages, e.g. Quests, Gear, Books
		const Page* view = &page;
		if (!page.tabs.empty()) {
			auto& selected = _pageTabs[page.id];

			// Find and pins open the tab holding the entry
			if (!_scrollToKey.empty()) {
				for (const auto& section : page.sections) {
					const bool holds = std::ranges::any_of(section.entries, [&](const Entry& a_entry) {
						return (a_entry.key.empty() ? a_entry.name : a_entry.key) == _scrollToKey || a_entry.name + '\x1f' + a_entry.detail == _scrollToKey;
					});
					if (holds && section.tab >= 0) {
						selected = std::to_string(section.tab);
						break;
					}
				}
			}

			std::vector<TabItem> tabs;
			for (std::size_t i = 0; i < page.tabs.size(); ++i) {
				std::int32_t tabFound = 0;
				std::int32_t tabTotal = 0;
				for (const auto& section : page.sections) {
					if (section.tab == static_cast<std::int32_t>(i)) {
						tabFound += section.Found();
						tabTotal += section.Total();
					}
				}
				const auto& title = page.tabs[i].title;
				// A kill list counts kills, as Kill Counts does in the sidebar
				std::int64_t kills = 0;
				for (const auto& record : _dragonKills) {
					kills += record.kills;
				}
				const auto count = page.tabs[i].layout == PageLayout::kKills ? std::to_string(kills) : CountText(tabFound, tabTotal);
				tabs.push_back({ std::to_string(i), a_provider.Translate(title.empty() ? page.title : title), count });
			}
			if (std::ranges::none_of(tabs, [&](const TabItem& a_tab) { return a_tab.id == selected; })) {
				selected = tabs.front().id;
			}
			TabRow(tabs, selected, true);
			ImGui::Spacing();
			view = &TabView(page, std::stoi(selected));
		}

		if (view->layout == PageLayout::kJournal) {
			DrawJournal(a_provider, *view);
			return;
		}
		if (view->layout == PageLayout::kKills) {
			DrawKills(a_provider, _dragonKills, true);
			return;
		}

		_hoveredEntry.reset();
		const float gap = ImGui::GetFontSize() * 0.75f;

		// Shout and trainer pages show their panel instead of the description
		const auto pageHas = [&](bool (*a_test)(const Entry&)) {
			return std::ranges::any_of(view->sections, [&](const Section& a_section) { return std::ranges::any_of(a_section.entries, a_test); });
		};
		const bool trainerPage = pageHas([](const Entry& a_entry) { return a_entry.IsTrainer(); });
		if (trainerPage) {
			_trainerRows = 1;
			for (const auto& section : view->sections) {
				if (!section.entries.empty() && section.entries.front().IsTrainer()) {
					_trainerRows = std::max(_trainerRows, PerkTreeRows(section.entries.front().trainer.perks));
				}
			}
		}
		const bool dragonPage = pageHas([](const Entry& a_entry) { return a_entry.IsDragon(); });
		if (dragonPage) {
			const float width = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().WindowPadding.x * 2.0f;
			_dragonLore = 0.0f;
			for (const auto& section : view->sections) {
				for (const auto& entry : section.entries) {
					_dragonLore = std::max(_dragonLore, DragonLoreHeight(entry.dragon.lore, width));
				}
			}
		}
		const bool shoutPage = trainerPage || dragonPage || pageHas([](const Entry& a_entry) { return a_entry.placesKnown; });
		float panelHeight = 0.0f;
		UpdateDescriptionPanel(ImGui::GetContentRegionAvail().y, gap);
		if (shoutPage) {
			// Folds with the description
			const auto& style = ImGui::GetStyle();
			ImGui::PushFont(Themes::HeaderFont());
			const float title = ImGui::GetFontSize() + style.ItemSpacing.y * 2.0f;
			ImGui::PopFont();
			const float t = std::clamp(_descriptionOpen, 0.0f, 1.0f);
			// Trainers have no title row, so the padding below matches the padding above
			const float panelWidth = ImGui::GetContentRegionAvail().x - style.WindowPadding.x * 2.0f;
			const float content = trainerPage ? TrainerInfoHeight() + 1.0f
			                    : dragonPage  ? DragonInfoHeight(_dragonLore, panelWidth) + 1.0f
			                                  : title + ShoutWordsHeight() + style.ItemSpacing.y * 2.0f + ImGui::GetFontSize() * 0.8f;
			panelHeight = (style.WindowPadding.y * 2.0f + content) * t * t * (3.0f - 2.0f * t);
			// Leaves the list room for a few rows, as the description panel does, on a short window or a big scale
			panelHeight = std::min(panelHeight, std::max(0.0f, ImGui::GetContentRegionAvail().y - gap - 6.0f * ImGui::GetTextLineHeightWithSpacing()));
		} else {
			panelHeight = DescriptionPanelHeight();
		}

		BeginFramedPanel("##Entries", ImVec2(0.0f, -(panelHeight + gap)));
		bool anyShown = false;
		for (const auto& section : view->sections) {
			const bool showTitle = view->sections.size() > 1 || section.title != page.title;
			anyShown |= DrawSection(a_provider, section, showTitle);
		}
		if (!anyShown) {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_NothingMatches").c_str());
		}
		EndFramedPanel();

		// Folded, the description panel's grabber is all that's left either way
		if (shoutPage && panelHeight >= 1.0f) {
			DrawShoutPanel(a_provider, *view, panelHeight, gap);
		} else {
			DrawDescriptionPanel(a_provider, panelHeight, gap);
		}
	}

	//---------------------------------------------------
	//-- Menu ( Kill Counts ) ---------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Kills ) ----------------
	//---------------------------------------------------

	void Menu::DrawKills(IDataProvider& a_provider, const std::vector<KillRecord>& a_records, bool a_dragons) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();
		_hoveredEntry.reset();

		// Stored as "Draugr:"
		const auto displayName = [](const std::string& a_name) {
			auto name = a_name;
			while (!name.empty() && (name.back() == ':' || name.back() == ' ')) {
				name.pop_back();
			}
			return name;
		};

		std::int64_t      totalKills = 0;
		std::int64_t      totalAssists = 0;
		std::int32_t      mostKills = 0;
		const KillRecord* mostSlain = nullptr;
		for (const auto& record : a_records) {
			totalKills += record.kills;
			totalAssists += record.assists;
			if (!mostSlain || record.kills > mostSlain->kills) {
				mostSlain = std::addressof(record);
			}
			mostKills = std::max(mostKills, record.kills);
		}

		// On a page's tab the page has the heading
		if (!a_dragons) {
			HeadingWithCount(a_provider.Translate("UI_Kills_Title"), std::to_string(a_records.size()), palette.heading);
			HeadingRule(2.0f);
			ImGui::Spacing();
		}

		//---------------------------------------------------
		// Totals
		//---------------------------------------------------

		const std::string cardValues[] = {
			std::to_string(totalKills),
			std::to_string(totalAssists),
			std::to_string(a_records.size()),
			mostSlain && mostSlain->kills > 0 ? displayName(mostSlain->name) : std::string("-"),
		};
		const char* cardTitles[] = { "UI_Kills_Total", "UI_Kills_Assists", a_dragons ? "UI_Dragons_Kinds" : "UI_Kills_Types", "UI_Kills_MostSlain" };
		const float           cardHeight = fontSize * 4.0f;
		if (ImGui::BeginTable("##KillCards", 4, ImGuiTableFlags_SizingStretchSame)) {
			for (int i = 0; i < 4; ++i) {
				ImGui::TableNextColumn();
				const auto  min = ImGui::GetCursorScreenPos();
				const auto  max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + cardHeight);
				const float padding = fontSize * 0.8f;
				auto*       drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(min, max, Colour(palette.accent, 0.04f));
				drawList->AddRect(min, max, Colour(palette.separator, 0.7f));
				const float corner = fontSize * 0.18f;
				CornerMarks(drawList, min, max, corner, Colour(palette.accent, 0.6f));
				drawList->AddText(ImVec2(min.x + padding, min.y + padding * 0.7f), Colour(Decor::Race() ? palette.heading : palette.textDim), a_provider.Translate(cardTitles[i]).c_str());
				ImGui::PushFont(Themes::TitleFont());
				drawList->PushClipRect(min, ImVec2(max.x - padding, max.y), true);
				drawList->AddText(ImVec2(min.x + padding, max.y - padding * 0.7f - ImGui::GetFontSize()), Colour(palette.accent), cardValues[i].c_str());
				drawList->PopClipRect();
				ImGui::PopFont();
				ImGui::Dummy(ImVec2(max.x - min.x, cardHeight));
			}
			ImGui::EndTable();
		}
		ImGui::Spacing();

		//---------------------------------------------------
		// Filter and sort
		//---------------------------------------------------

		ThemedFilterInput("##Filter", a_provider.Translate("UI_Filter").c_str(), _filter.data(), _filter.size(), fontSize * 14.0f);
		ImGui::SameLine(0.0f, fontSize);
		const std::string sortLabels[] = {
			a_provider.Translate("UI_Kills_SortKills"),
			a_provider.Translate("UI_SortName"),
			a_provider.Translate("UI_Kills_SortAssists"),
		};
		_killSort = std::clamp(_killSort, 0, 2);
		if (ThemedBeginCombo("##KillSort", sortLabels[_killSort].c_str(), fontSize * 11.0f)) {
			for (int i = 0; i < 3; ++i) {
				if (ImGui::Selectable(sortLabels[i].c_str(), i == _killSort)) {
					_killSort = i;
				}
			}
			ImGui::EndCombo();
		}

		DividerRow();

		if (a_records.empty()) {
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(a_dragons ? "UI_Dragons_KillsEmpty" : "UI_Kills_Empty").c_str());
			ImGui::PopTextWrapPos();
			return;
		}

		const auto filter = Lower(std::string(_filter.data()));
		std::vector<const KillRecord*> shown;
		for (const auto& record : a_records) {
			if (filter.empty() || Lower(record.name).find(filter) != std::string::npos) {
				shown.push_back(std::addressof(record));
			}
		}
		std::ranges::stable_sort(shown, [&](const KillRecord* a, const KillRecord* b) {
			switch (_killSort) {
			case 1:
				return Lower(a->name) < Lower(b->name);
			case 2:
				return a->assists != b->assists ? a->assists > b->assists : Lower(a->name) < Lower(b->name);
			default:
				return a->kills != b->kills ? a->kills > b->kills : Lower(a->name) < Lower(b->name);
			}
		});

		const KillRecord* selected = nullptr;
		for (const auto* record : shown) {
			if (record->name == _killKey) {
				selected = record;
			}
		}
		if (!selected && !shown.empty()) {
			selected = shown.front();
		}

		//---------------------------------------------------
		// Kill list
		//---------------------------------------------------

		const float gap = fontSize * 0.75f;
		const float lineHeight = ImGui::GetTextLineHeightWithSpacing();
		const float detailHeight = lineHeight * 3.4f + ImGui::GetFrameHeight() + style.WindowPadding.y * 2.0f;
		const float rowHeight = ImGui::GetFrameHeight();

		const std::string headers[] = {
			a_provider.Translate(a_dragons ? "UI_Dragons_Kind" : "UI_Kills_Creature"),
			a_provider.Translate("UI_Kills_Share"),
			a_provider.Translate("UI_Kills_Kills"),
			a_provider.Translate("UI_Kills_Assists"),
		};
		const float numberWidth = std::max({ ImGui::CalcTextSize("00000").x, ImGui::CalcTextSize(headers[2].c_str()).x, ImGui::CalcTextSize(headers[3].c_str()).x });
		const float columnGap = fontSize * 1.5f;

		struct Columns
		{
			float nameRight;
			float shareLeft;
			float shareRight;
			float killsRight;
			float assistsRight;
		};
		const auto columnsFor = [&](float a_left, float a_right) {
			Columns columns{};
			columns.assistsRight = a_right - style.FramePadding.x;
			columns.killsRight = columns.assistsRight - numberWidth - columnGap;
			columns.shareRight = columns.killsRight - numberWidth - columnGap;
			columns.nameRight = a_left + std::clamp((a_right - a_left) * 0.3f, fontSize * 9.0f, fontSize * 16.0f);
			columns.shareLeft = columns.nameRight + columnGap;
			return columns;
		};

		BeginFramedPanel("##KillList", ImVec2(0.0f, -(detailHeight + gap)));
		{
			const auto  pos = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			const bool  scrolls = static_cast<float>(shown.size()) * (rowHeight + style.ItemSpacing.y) > ImGui::GetContentRegionAvail().y - rowHeight - 2.0f;
			const auto  columns = columnsFor(pos.x, pos.x + width - (scrolls ? style.ScrollbarSize : 0.0f));
			auto*       drawList = ImGui::GetWindowDrawList();
			const ImU32 dim = Colour(palette.textDim);
			const float textY = pos.y + (rowHeight - fontSize) * 0.5f;
			drawList->AddText(ImVec2(pos.x + style.FramePadding.x, textY), dim, headers[0].c_str());
			if (columns.shareRight - columns.shareLeft > fontSize * 4.0f) {
				drawList->AddText(ImVec2(columns.shareRight - ImGui::CalcTextSize(headers[1].c_str()).x, textY), dim, headers[1].c_str());
			}
			drawList->AddText(ImVec2(columns.killsRight - ImGui::CalcTextSize(headers[2].c_str()).x, textY), dim, headers[2].c_str());
			drawList->AddText(ImVec2(columns.assistsRight - ImGui::CalcTextSize(headers[3].c_str()).x, textY), dim, headers[3].c_str());
			ImGui::Dummy(ImVec2(width, rowHeight));
			const auto rule = ImGui::GetCursorScreenPos();
			Decor::FadedLine(drawList, rule, ImVec2(rule.x + width, rule.y), Colour(palette.separator));
			ImGui::Dummy(ImVec2(width, 2.0f));
		}

		ImGui::BeginChild("##KillRows", ImVec2(0.0f, 0.0f), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoBackground);
		ScrollWithRightStick();
		if (shown.empty()) {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_NothingMatches").c_str());
		}
		for (const auto* record : shown) {
			ImGui::PushID(record->name.c_str());
			if (RowSelectable("##Kill", record == selected, ImVec2(0.0f, rowHeight))) {
				_killKey = record->name;
				_confirmKey.clear();
			}
			ImGui::PopID();

			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			const auto  columns = columnsFor(min.x, max.x);
			auto*       drawList = ImGui::GetWindowDrawList();
			const float centreY = (min.y + max.y) * 0.5f;
			const float textY = centreY - fontSize * 0.5f;

			drawList->PushClipRect(min, ImVec2(columns.nameRight, max.y), true);
			drawList->AddText(ImVec2(min.x + style.FramePadding.x, textY), Colour(palette.text), displayName(record->name).c_str());
			drawList->PopClipRect();

			if (columns.shareRight - columns.shareLeft > fontSize * 2.0f) {
				const float span = columns.shareRight - columns.shareLeft;
				const float barHeight = std::max(2.0f, fontSize * 0.3f);
				const float fill = span * Fraction(record->kills, mostKills);
				drawList->AddRectFilled(ImVec2(columns.shareLeft, centreY - 0.5f), ImVec2(columns.shareRight, centreY + 0.5f), Colour(palette.separator, 0.35f));
				if (fill > 0.0f) {
					drawList->AddRectFilled(ImVec2(columns.shareRight - fill, centreY - barHeight * 0.5f), ImVec2(columns.shareRight, centreY + barHeight * 0.5f), SkyUI() ? Colour(palette.textDim) : Colour(palette.accent, 0.85f));
					if (!SkyUI()) {
						Decor::Diamond(drawList, ImVec2(columns.shareRight - fill, centreY), barHeight * 1.1f, Colour(palette.accent), true);
					}
				}
			}

			const auto kills = std::to_string(record->kills);
			const auto assists = std::to_string(record->assists);
			drawList->AddText(ImVec2(columns.killsRight - ImGui::CalcTextSize(kills.c_str()).x, textY), Colour(record->kills > 0 ? palette.text : palette.textDim), kills.c_str());
			drawList->AddText(ImVec2(columns.assistsRight - ImGui::CalcTextSize(assists.c_str()).x, textY), Colour(palette.textDim), assists.c_str());
			Decor::FadedLine(drawList, ImVec2(min.x, max.y), ImVec2(max.x, max.y), Colour(palette.separator, 0.35f));
		}
		ImGui::EndChild();
		EndFramedPanel();

		//---------------------------------------------------
		// Selected creature
		//---------------------------------------------------

		ImGui::Dummy(ImVec2(0.0f, gap - style.ItemSpacing.y));
		BeginFramedPanel("##KillDetail", ImVec2(0.0f, 0.0f), true);
		if (selected) {
			const auto name = displayName(selected->name);
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", name.c_str());
			ImGui::PopFont();

			auto lastKill = selected->lastKill;
			for (auto pos = lastKill.find('\n'); pos != std::string::npos; pos = lastKill.find('\n', pos)) {
				lastKill.replace(pos, 1, ", ");
			}
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Kills_LastKill").c_str());
			ImGui::SameLine();
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(lastKill.empty() ? palette.textDim : palette.text, "%s", lastKill.empty() ? a_provider.Translate("UI_Kills_NoLastKill").c_str() : lastKill.c_str());
			ImGui::PopTextWrapPos();

			// Reset and Remove, both need a second click
			const auto  resetText = a_provider.Translate("UI_Kills_Reset");
			const auto  removeText = a_provider.Translate("UI_Remove");
			const auto  confirmText = a_provider.Translate("UI_ConfirmClick");
			const float buttonWidth = std::max({ ButtonWidth(resetText.c_str()), ButtonWidth(removeText.c_str()), ButtonWidth(confirmText.c_str()) }) + fontSize * 2.0f;
			const float bottom = ImGui::GetWindowHeight() - style.WindowPadding.y - ImGui::GetFrameHeight();
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - style.WindowPadding.x - buttonWidth * 2.0f - style.ItemSpacing.x, std::max(ImGui::GetCursorPosY(), bottom)));

			const auto actionButton = [&](const char* a_id, const std::string& a_text, bool a_remove, bool a_enabled) {
				const auto key = std::string(a_id) + '/' + selected->name;
				const bool armed = _confirmKey == key;
				ImGui::BeginDisabled(!a_enabled);
				const auto caption = (armed ? confirmText : a_text) + "##" + a_id;
				if (ThemedButton(caption.c_str(), buttonWidth)) {
					if (armed) {
						_confirmKey.clear();
						a_dragons ? a_provider.ResetDragonKills(selected->name, a_remove) : a_provider.ResetKills(selected->name, a_remove);
					} else {
						_confirmKey = key;
					}
				}
				ImGui::EndDisabled();
			};
			actionButton("reset", resetText, false, selected->kills > 0 || selected->assists > 0);
			ImGui::SameLine();
			actionButton("remove", removeText, true, true);
		}
		EndFramedPanel();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Pin Icon ) -------------
	//---------------------------------------------------

	// Pin
	static void DrawPinIcon(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		const float  radius = a_size * 0.22f;
		const ImVec2 head(a_centre.x, a_centre.y - a_size * 0.18f);
		a_drawList->AddCircleFilled(head, radius, a_colour);
		a_drawList->AddLine(ImVec2(a_centre.x, head.y + radius * 0.6f), ImVec2(a_centre.x, a_centre.y + a_size * 0.38f), a_colour, std::max(1.5f, a_size * 0.09f));
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Steal Icon ) -----------
	//---------------------------------------------------

	// Steal hand
	static void DrawStealIcon(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		constexpr float pi = std::numbers::pi_v<float>;
		const float     width = std::max(1.5f, a_size * 0.13f);
		const ImVec2    palm(a_centre.x + a_size * 0.04f, a_centre.y + a_size * 0.20f);
		a_drawList->AddCircleFilled(palm, a_size * 0.24f, a_colour, 16);
		a_drawList->AddRectFilled(ImVec2(palm.x - a_size * 0.22f, palm.y - a_size * 0.06f), ImVec2(palm.x + a_size * 0.22f, palm.y + a_size * 0.12f), a_colour, a_size * 0.08f);

		constexpr float angles[] = { -0.30f, -0.10f, 0.10f, 0.30f };
		constexpr float lengths[] = { 0.40f, 0.48f, 0.46f, 0.36f };
		for (int i = 0; i < 4; ++i) {
			const ImVec2 base(palm.x + (static_cast<float>(i) - 1.5f) * a_size * 0.12f, palm.y - a_size * 0.14f);
			const ImVec2 tip(base.x + std::sin(angles[i]) * a_size * lengths[i], base.y - std::cos(angles[i]) * a_size * lengths[i]);
			a_drawList->AddLine(base, tip, a_colour, width);
			a_drawList->AddCircleFilled(tip, width * 0.5f, a_colour, 8);
		}

		const ImVec2 thumbBase(palm.x - a_size * 0.16f, palm.y + a_size * 0.04f);
		const float  thumbAngle = -pi * 0.30f;
		const ImVec2 thumbTip(thumbBase.x + std::sin(thumbAngle) * a_size * 0.34f, thumbBase.y - std::cos(thumbAngle) * a_size * 0.34f);
		a_drawList->AddLine(thumbBase, thumbTip, a_colour, width * 1.1f);
		a_drawList->AddCircleFilled(thumbTip, width * 0.55f, a_colour, 8);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Marker Icon ) ----------
	//---------------------------------------------------

	// Location marker
	static void DrawMarkerIcon(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		constexpr float pi = std::numbers::pi_v<float>;
		const float     radius = a_size * 0.26f;
		const ImVec2    head(a_centre.x, a_centre.y - a_size * 0.12f);
		a_drawList->PathArcTo(head, radius, pi * 5.0f / 6.0f, pi * 13.0f / 6.0f, 16);
		a_drawList->PathLineTo(ImVec2(a_centre.x, a_centre.y + a_size * 0.42f));
		a_drawList->PathStroke(a_colour, ImDrawFlags_Closed, std::max(1.2f, a_size * 0.09f));
		a_drawList->AddCircleFilled(head, radius * 0.38f, a_colour);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Reset Icon ) -----------
	//---------------------------------------------------

	// Reset arrow
	static void DrawResetIcon(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		constexpr float pi = std::numbers::pi_v<float>;
		const float     radius = a_size * 0.34f;
		const float     stroke = std::max(1.2f, a_size * 0.09f);
		const float     start = -pi * 0.35f;
		a_drawList->PathArcTo(a_centre, radius, start, start + pi * 1.6f, 24);
		a_drawList->PathStroke(a_colour, ImDrawFlags_None, stroke);
		const ImVec2 tip(a_centre.x + std::cos(start) * radius, a_centre.y + std::sin(start) * radius);
		const float  head = a_size * 0.2f;
		a_drawList->AddTriangleFilled(ImVec2(tip.x + head, tip.y), ImVec2(tip.x - head * 0.2f, tip.y - head), ImVec2(tip.x - head * 0.2f, tip.y + head * 0.6f), a_colour);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Check Icon ) -----------
	//---------------------------------------------------

	// Tick in a ring
	static void DrawCheckIcon(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, ImU32 a_colour) {

		const float stroke = std::max(1.2f, a_size * 0.09f);
		a_drawList->AddCircle(a_centre, a_size * 0.4f, a_colour, 20, stroke);
		const ImVec2 tick[] = {
			ImVec2(a_centre.x - a_size * 0.18f, a_centre.y + a_size * 0.01f),
			ImVec2(a_centre.x - a_size * 0.04f, a_centre.y + a_size * 0.15f),
			ImVec2(a_centre.x + a_size * 0.2f, a_centre.y - a_size * 0.13f),
		};
		a_drawList->AddPolyline(tick, 3, a_colour, ImDrawFlags_None, stroke);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Direction ) ------------
	//---------------------------------------------------

	// Direction arrow, a_angle in radians
	static void DrawDirection(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, float a_angle, ImU32 a_colour) {

		const float sine = std::sin(a_angle);
		const float cosine = std::cos(a_angle);
		const auto  turn = [&](float a_x, float a_y) { return ImVec2(a_centre.x + a_x * cosine - a_y * sine, a_centre.y + a_x * sine + a_y * cosine); };
		a_drawList->AddTriangleFilled(turn(0.0f, -a_size * 0.5f), turn(a_size * 0.36f, a_size * 0.4f), turn(-a_size * 0.36f, a_size * 0.4f), a_colour);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Height Slot Width ) ---------
	//---------------------------------------------------

	static float HeightSlotWidth(IDataProvider& a_provider) {

		const float fontSize = ImGui::GetFontSize();
		return fontSize * 0.81f + ImGui::CalcTextSize(a_provider.Translate("UI_Cell_Metres{99}").c_str()).x;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Height ) ---------------
	//---------------------------------------------------

	// Height above or below, nothing under 2 m
	static void DrawHeight(ImDrawList* a_drawList, IDataProvider& a_provider, float a_left, float a_centreY, float a_height, ImU32 a_colour) {

		const int metres = static_cast<int>(std::lround(std::abs(a_height)));
		if (metres < 2) {
			return;
		}
		const float fontSize = ImGui::GetFontSize();
		const float chevron = fontSize * 0.28f;
		const float x = a_left + chevron;
		const float stroke = std::max(1.2f, fontSize * 0.08f);
		const float up = a_height > 0.0f ? -1.0f : 1.0f;
		a_drawList->AddLine(ImVec2(x - chevron, a_centreY - up * chevron * 0.5f), ImVec2(x, a_centreY + up * chevron * 0.5f), a_colour, stroke);
		a_drawList->AddLine(ImVec2(x, a_centreY + up * chevron * 0.5f), ImVec2(x + chevron, a_centreY - up * chevron * 0.5f), a_colour, stroke);
		const auto text = a_provider.Translate("UI_Cell_Metres{" + std::to_string(metres) + "}");
		const auto size = ImGui::CalcTextSize(text.c_str());
		a_drawList->AddText(ImVec2(a_left + chevron * 2.0f + fontSize * 0.25f, a_centreY - size.y * 0.5f), a_colour, text.c_str());
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Place Glyph ) ----------
	//---------------------------------------------------

	// Chest, person or lying around
	static void DrawPlaceGlyph(ImDrawList* a_drawList, ImVec2 a_centre, float a_size, CellPlace a_place, ImU32 a_colour) {

		const float half = a_size * 0.5f;
		const float stroke = std::max(1.2f, a_size * 0.09f);
		switch (a_place) {
		case CellPlace::kContainer:
			{
				const ImVec2 min(a_centre.x - half, a_centre.y - half * 0.7f);
				const ImVec2 max(a_centre.x + half, a_centre.y + half * 0.75f);
				a_drawList->AddRect(min, max, a_colour, a_size * 0.12f, 0, stroke);
				const float lidY = min.y + (max.y - min.y) * 0.38f;
				a_drawList->AddLine(ImVec2(min.x, lidY), ImVec2(max.x, lidY), a_colour, stroke);
				a_drawList->AddRectFilled(ImVec2(a_centre.x - a_size * 0.08f, lidY - a_size * 0.06f), ImVec2(a_centre.x + a_size * 0.08f, lidY + a_size * 0.14f), a_colour);
			}
			break;
		case CellPlace::kCarried:
			{
				a_drawList->AddCircleFilled(ImVec2(a_centre.x, a_centre.y - half * 0.45f), a_size * 0.2f, a_colour);
				a_drawList->PathArcTo(ImVec2(a_centre.x, a_centre.y + half), half * 0.82f, std::numbers::pi_v<float>, 2.0f * std::numbers::pi_v<float>, 12);
				a_drawList->PathFillConvex(a_colour);
			}
			break;
		default:
			{
				const float groundY = a_centre.y + half * 0.75f;
				a_drawList->AddLine(ImVec2(a_centre.x - half, groundY), ImVec2(a_centre.x + half, groundY), a_colour, stroke);
				Decor::Diamond(a_drawList, ImVec2(a_centre.x, a_centre.y - half * 0.05f), a_size * 0.34f, a_colour, true);
			}
			break;
		}
	}

	//---------------------------------------------------
	//-- Menu ( In Current Cell ) -----------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Cell Window ) ----------
	//---------------------------------------------------

	bool Menu::DrawCellWindow(IDataProvider& a_provider, UISettings& a_settings) {

		const auto& io = ImGui::GetIO();
		ImGui::PushFont(Themes::BodyFont());
		// Grows with the text scale, so the options keep to their columns and the list keeps some rows
		const auto size = ImVec2(
			std::clamp(std::max(io.DisplaySize.x * 0.4f, ImGui::GetFontSize() * 42.0f), std::min(io.DisplaySize.x, 620.0f), io.DisplaySize.x),
			std::clamp(std::max(io.DisplaySize.y * 0.76f, ImGui::GetFontSize() * 45.0f), std::min(io.DisplaySize.y, 460.0f), io.DisplaySize.y));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);

		bool open = true;
		auto background = Themes::GetPalette().background;
		background.w = std::clamp(a_settings.opacity, 0.3f, 1.0f);
		windowOpacity = background.w;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
		const float pad = ImGui::GetFontSize() * 1.4f;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(pad, pad));
		constexpr auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		const bool     visible = ImGui::Begin("##InCurrentCell", nullptr, flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (visible) {
			const auto& palette = Themes::GetPalette();
			const auto& style = ImGui::GetStyle();
			const float fontSize = ImGui::GetFontSize();
			const auto min = ImGui::GetWindowPos();
			const auto max = ImVec2(min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight());
			const auto inset = fontSize * 0.4f;
			Decor::Frame(ImGui::GetWindowDrawList(), ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), Colour(palette.separator));

			const bool backPressed = ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false) || ImGui::IsKeyPressed(ImGuiKey_Escape, false);
			if (backPressed && !_backHandledByImGui) {
				open = false;
			}

			RouteNavigation();
			controlRecorder = this;

			const auto scan = a_provider.GetCellScan();

			const auto  start = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			const auto  backLabel = a_provider.Translate("UI_Cell_Back");
			const auto  closeLabel = a_provider.Translate("UI_Close");
			const float backWidth = ButtonWidth(backLabel.c_str()) + fontSize;
			const float closeWidth = ButtonWidth(closeLabel.c_str()) + fontSize;
			ImGui::PushFont(Themes::TitleFont());
			ImGui::TextColored(palette.heading, "%s", a_provider.Translate("UI_Cell_Title").c_str());
			ImGui::PopFont();
			const auto afterTitle = ImGui::GetCursorScreenPos();
			ImGui::SetCursorScreenPos(ImVec2(start.x + width - backWidth - closeWidth - style.ItemSpacing.x, start.y));
			if (ThemedButton((backLabel + "##CellBack").c_str(), backWidth)) {
				_cellView = false;
			}
			ImGui::SameLine();
			if (ThemedButton((closeLabel + "##CellClose").c_str(), closeWidth)) {
				open = false;
			}
			ImGui::SetCursorScreenPos(afterTitle);

			std::string subtitle = scan.cellName;
			if (scan.scanned && !scan.excluded) {
				subtitle += (subtitle.empty() ? "" : "  \xC2\xB7  ") + a_provider.Translate("UI_Cell_Count{" + std::to_string(scan.items.size()) + "}");
			}
			ImGui::TextColored(palette.textDim, "%s", subtitle.c_str());
			const auto pos = ImGui::GetCursorScreenPos();
			Decor::FadedLine(ImGui::GetWindowDrawList(), pos, ImVec2(pos.x + width, pos.y), Colour(palette.separator));
			ImGui::Dummy(ImVec2(width, 4.0f));

			DrawCellOptions(a_provider, a_settings);

			// Quest marker bar
			const auto markedCount = std::ranges::count_if(scan.items, &CellItem::marked);
			auto       marked = std::ranges::find_if(scan.items, [&](const CellItem& a_item) { return a_item.marked && a_item.refID == _shownMarker; });
			if (marked == scan.items.end()) {
				marked = std::ranges::find_if(scan.items, &CellItem::marked);
			}
			if (marked != scan.items.end()) {
				const auto  stripStart = ImGui::GetCursorScreenPos();
				const float height = ImGui::GetFrameHeight() + fontSize * 0.4f;
				const auto  stripMax = ImVec2(stripStart.x + width, stripStart.y + height);
				auto*       drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(stripStart, stripMax, Colour(palette.accent, 0.05f));
				drawList->AddRect(stripStart, stripMax, Colour(palette.accent, 0.5f));

				const auto  excludeLabel = a_provider.Translate("UI_Cell_ExcludeMarked");
				const auto  clearLabel = a_provider.Translate("UI_Cell_ClearMarker");
				const auto  clearAllLabel = a_provider.Translate("UI_Cell_ClearAllMarkers");
				const auto  more = markedCount - 1;
				const float excludeWidth = ButtonWidth(excludeLabel.c_str()) + fontSize;
				const float clearWidth = ButtonWidth(clearLabel.c_str()) + fontSize;
				const float clearAllWidth = more > 0 ? ButtonWidth(clearAllLabel.c_str()) + fontSize : 0.0f;
				const float buttonsX = stripMax.x - fontSize * 0.5f - excludeWidth - clearWidth - style.ItemSpacing.x - (more > 0 ? clearAllWidth + style.ItemSpacing.x : 0.0f);
				const float rowY = stripStart.y + (height - ImGui::GetFrameHeight()) * 0.5f;

				const float textY = stripStart.y + (height - fontSize) * 0.5f;
				const auto  label = a_provider.Translate(more > 0 ? "UI_Cell_Markers{" + std::to_string(markedCount) + "}" : std::string("UI_Cell_Marker"));
				const auto  where = marked->holder.empty() ? a_provider.Translate("UI_Cell_Loose") : marked->holder;
				drawList->PushClipRect(stripStart, ImVec2(buttonsX - style.ItemSpacing.x, stripMax.y), true);
				float x = stripStart.x + fontSize * 0.7f;
				drawList->AddText(ImVec2(x, textY), Colour(palette.accent), label.c_str());
				x += ImGui::CalcTextSize(label.c_str()).x + fontSize * 0.4f;
				drawList->AddText(ImVec2(x, textY), Colour(palette.text), marked->name.c_str());
				x += ImGui::CalcTextSize(marked->name.c_str()).x;
				drawList->AddText(ImVec2(x, textY), Colour(palette.textDim), FitText("  \xC2\xB7  " + where, buttonsX - style.ItemSpacing.x - x).c_str());
				drawList->PopClipRect();

				const auto shown = *marked;
				ImGui::SetCursorScreenPos(ImVec2(buttonsX, rowY));
				if (ThemedButton((excludeLabel + "##CellExcludeMarked").c_str(), excludeWidth)) {
					a_provider.ExcludeCellItem(shown);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("%s", a_provider.Translate("UI_Cell_ExcludeMarkedHint").c_str());
				}
				ImGui::SameLine();
				if (ThemedButton((clearLabel + "##CellClear").c_str(), clearWidth)) {
					a_provider.ClearCellMarker(shown);
				}
				if (more > 0) {
					ImGui::SameLine();
					if (ThemedButton((clearAllLabel + "##CellClearAll").c_str(), clearAllWidth)) {
						a_provider.ClearCellMarkers();
					}
				}
				ImGui::SetCursorScreenPos(ImVec2(stripStart.x, stripMax.y + style.ItemSpacing.y));
			}

			const float barHeight = ImGui::GetFrameHeight() + style.ItemSpacing.y * 2.0f + PadHintsHeight();
			BeginFramedPanel("##CellList", ImVec2(0.0f, -barHeight));
			ScrollWithRightStick();
			DrawCellList(a_provider, scan);
			EndFramedPanel();

			ImGui::Dummy(ImVec2(0.0f, style.ItemSpacing.y * 0.5f));
			const auto barStart = ImGui::GetCursorScreenPos();
			if (MarkerToggle("##KeepOnScreen", a_settings.cellPanel, a_provider.Translate("UI_Cell_KeepOnScreen").c_str())) {
				_settingsChanged = true;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("%s", a_provider.Translate("UI_SettingHint_CellPanel").c_str());
			}
			ImGui::SameLine(0.0f, fontSize * 1.2f);
			if (MarkerToggle("##ShowHeight", a_settings.cellHeight, a_provider.Translate("UI_Cell_ShowHeight").c_str())) {
				_settingsChanged = true;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("%s", a_provider.Translate("UI_Cell_ShowHeightHint").c_str());
			}
			// Only interiors can be excluded, as in the MCM
			const bool  canExclude = scan.scanned && scan.interior && !scan.excluded;
			const auto  rescan = a_provider.Translate("UI_Cell_Rescan");
			const bool  armed = _confirmKey == "cellwindow/excludeCell";
			const auto  exclude = a_provider.Translate(armed ? "UI_ConfirmClick" : "UI_Cell_ExcludeCell");
			const float rescanWidth = ButtonWidth(rescan.c_str()) + fontSize;
			const float excludeWidth = canExclude ? std::max(ButtonWidth(exclude.c_str()), ButtonWidth(a_provider.Translate("UI_Cell_ExcludeCell").c_str())) + fontSize : 0.0f;
			ImGui::SetCursorScreenPos(ImVec2(barStart.x + width - rescanWidth - (canExclude ? excludeWidth + style.ItemSpacing.x : 0.0f), barStart.y));
			if (canExclude) {
				if (ThemedButton((exclude + "##CellExcludeCell").c_str(), excludeWidth)) {
					if (armed) {
						_confirmKey.clear();
						a_provider.ExcludeCurrentCell();
					} else {
						_confirmKey = "cellwindow/excludeCell";
					}
				}
				ImGui::SameLine();
			}
			if (ThemedButton((rescan + "##CellRescan").c_str(), rescanWidth)) {
				a_provider.RescanCell();
			}
			controlRecorder = nullptr;
			if (PadHintsShown()) {
				DrawPadHints(a_provider, true);
			}
			ScrollFocusWithRightStick();
		}
		_backHandledByImGui = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel) || ImGui::IsAnyItemActive();
		ImGui::End();
		ImGui::PopFont();
		return open;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Cell Options ) ---------
	//---------------------------------------------------

	// Scanner options
	void Menu::DrawCellOptions(IDataProvider& a_provider, UISettings& a_settings) {

		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();

		if (RowSelectable("##CellOptions", false, ImVec2(0.0f, ImGui::GetFrameHeight()))) {
			a_settings.cellOptionsOpen = !a_settings.cellOptionsOpen;
			_settingsChanged = true;
		}
		{
			const auto min = ImGui::GetItemRectMin();
			const auto max = ImGui::GetItemRectMax();
			auto*      drawList = ImGui::GetWindowDrawList();
			const float centreY = (min.y + max.y) * 0.5f;
			Decor::Diamond(drawList, ImVec2(min.x + fontSize * 0.5f, centreY), fontSize * 0.22f, Colour(palette.accent), a_settings.cellOptionsOpen);
			drawList->AddText(ImVec2(min.x + fontSize * 1.2f, centreY - fontSize * 0.5f), Colour(palette.heading), a_provider.Translate("UI_Cell_Options").c_str());
			const auto hint = a_provider.Translate("UI_Cell_OptionsHint");
			const auto hintSize = ImGui::CalcTextSize(hint.c_str());
			drawList->AddText(ImVec2(max.x - hintSize.x - fontSize * 0.4f, centreY - fontSize * 0.5f), Colour(palette.textDim), hint.c_str());
		}
		if (!a_settings.cellOptionsOpen) {
			return;
		}

		auto page = std::ranges::find(_settingsPages, std::string_view("cellwindow"), &SettingsPage::id);
		if (page == _settingsPages.end()) {
			return;
		}

		std::vector<std::pair<std::string, std::vector<SettingItem*>>> sections;
		for (auto& item : page->columns[0]) {
			if (item.kind == SettingKind::kHeader) {
				sections.emplace_back(a_provider.Translate(item.label), std::vector<SettingItem*>{});
			} else if (!sections.empty() && item.kind != SettingKind::kSpacer) {
				sections.back().second.push_back(std::addressof(item));
			}
		}
		if (sections.empty()) {
			return;
		}
		_cellOptionsSection = std::clamp(_cellOptionsSection, 0, static_cast<int>(sections.size()) - 1);

		ImGui::Spacing();
		for (int i = 0; i < static_cast<int>(sections.size()); ++i) {
			if (i > 0) {
				ImGui::SameLine(0.0f, fontSize * 1.6f);
			}
			const auto& name = sections[static_cast<std::size_t>(i)].first;
			const auto  size = ImGui::CalcTextSize(name.c_str());
			const auto  pos = ImGui::GetCursorScreenPos();
			ImGui::PushID(i);
			if (ImGui::InvisibleButton("##Section", ImVec2(size.x, ImGui::GetFrameHeight()))) {
				_cellOptionsSection = i;
			}
			NoteControl();
			const bool selected = i == _cellOptionsSection;
			const bool hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
			auto*      drawList = ImGui::GetWindowDrawList();
			const float textY = pos.y + (ImGui::GetFrameHeight() - size.y) * 0.5f;
			drawList->AddText(ImVec2(pos.x, textY), Colour(selected ? palette.heading : hovered ? palette.text : palette.textDim), name.c_str());
			if (selected) {
				if (SkyUI()) {
					Decor::DoubleRule(drawList, ImVec2(pos.x - fontSize * 0.3f, textY + size.y + 3.0f), ImVec2(pos.x + size.x + fontSize * 0.3f, textY + size.y + 3.0f), Colour(palette.accent));
				} else {
					Decor::FadedLine(drawList, ImVec2(pos.x - fontSize * 0.3f, textY + size.y + 2.0f), ImVec2(pos.x + size.x + fontSize * 0.3f, textY + size.y + 2.0f), Colour(palette.accent), 1.5f);
				}
			}
			ImGui::PopID();
		}

		auto& items = sections[static_cast<std::size_t>(_cellOptionsSection)].second;
		std::vector<std::pair<SettingItem*, SettingItem*>> rows;
		for (std::size_t i = 0; i < items.size(); ++i) {
			const bool pairsWithButton = items[i]->kind == SettingKind::kText && i + 1 < items.size() && items[i + 1]->kind == SettingKind::kButton && items[i + 1]->label.empty();
			rows.emplace_back(items[i], pairsWithButton ? items[i + 1] : nullptr);
			i += pairsWithButton ? 1 : 0;
		}
		const auto half = (rows.size() + 1) / 2;

		const auto apply = [&](SettingItem& a_item) {
			a_provider.ApplySetting("cellwindow", a_item);
			a_provider.RescanCell();
		};
		const auto button = [&](SettingItem& a_item, float a_width) {
			const auto key = "cellwindow/" + a_item.id;
			const bool armed = _confirmKey == key;
			const auto caption = armed ? a_provider.Translate("UI_ConfirmClick") : a_provider.Translate(a_item.value);
			ImGui::BeginDisabled(!a_item.enabled);
			if (ThemedButton((caption + "##Button" + a_item.id).c_str(), a_width)) {
				if (a_item.confirm && !armed) {
					_confirmKey = key;
				} else {
					_confirmKey.clear();
					apply(a_item);
				}
			}
			ImGui::EndDisabled();
		};

		if (ImGui::BeginTable("##CellOptionsTable", 2, ImGuiTableFlags_SizingStretchSame)) {
			for (int column = 0; column < 2; ++column) {
				ImGui::TableNextColumn();
				for (std::size_t r = column == 0 ? 0 : half; r < (column == 0 ? half : rows.size()); ++r) {
					auto& item = *rows[r].first;
					ImGui::PushID(item.id.c_str());
					ImGui::BeginDisabled(!item.enabled);
					const auto  label = a_provider.Translate(item.label);
					const float cellX = ImGui::GetCursorPosX();
					const float cellWidth = ImGui::GetContentRegionAvail().x - fontSize * 0.5f;
					const float controlX = cellX + cellWidth * 0.5f;
					const float controlWidth = cellX + cellWidth - controlX;
					const auto  inlineLabel = [&](const ImVec4& a_colour) {
						ImGui::AlignTextToFramePadding();
						ImGui::TextColored(a_colour, "%s", label.c_str());
						ImGui::SameLine();
						ImGui::SetCursorPosX(controlX);
					};
					switch (item.kind) {
					case SettingKind::kToggle:
						if (MarkerToggle("##Option", item.on, label.c_str())) {
							apply(item);
						}
						break;
					case SettingKind::kSlider:
						{
							inlineLabel(palette.text);
							auto& pending = _pendingCellNumbers.try_emplace(item.id, item.number).first->second;
							bool  released = false;
							ThemedSlider("##Slider", ImGuiDataType_S32, &pending, &item.minimum, &item.maximum, "%d", controlWidth, &released);
							if (released) {
								item.number = pending;
								apply(item);
							}
						}
						break;
					case SettingKind::kChoice:
						{
							inlineLabel(palette.text);
							const auto preview = item.choice >= 0 && item.choice < static_cast<int>(item.options.size()) ? a_provider.Translate(item.options[item.choice]) : std::string{};
							if (ThemedBeginCombo("##Choice", preview.c_str(), controlWidth)) {
								for (int option = 0; option < static_cast<int>(item.options.size()); ++option) {
									if (ImGui::Selectable(a_provider.Translate(item.options[option]).c_str(), option == item.choice)) {
										item.choice = option;
										apply(item);
									}
								}
								ImGui::EndCombo();
							}
						}
						break;
					case SettingKind::kText:
						{
							ImGui::AlignTextToFramePadding();
							ImGui::TextColored(palette.textDim, "%s", label.c_str());
							ImGui::SameLine();
							ImGui::TextColored(palette.text, "%s", a_provider.Translate(item.value).c_str());
							if (rows[r].second) {
								auto&       paired = *rows[r].second;
								const float width = ButtonWidth(a_provider.Translate(paired.value).c_str()) + fontSize * 1.5f;
								ImGui::SameLine();
								ImGui::SetCursorPosX(cellX + cellWidth - width);
								ImGui::EndDisabled();
								button(paired, width);
								ImGui::BeginDisabled(!item.enabled);
							}
						}
						break;
					case SettingKind::kButton:
						if (!label.empty()) {
							inlineLabel(palette.text);
							button(item, controlWidth);
						} else {
							button(item, cellWidth);
						}
						break;
					default:
						break;
					}
					ImGui::EndDisabled();
					if (!item.info.empty() && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
						ImGui::SetTooltip("%s", a_provider.Translate(item.info).c_str());
					}
					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}
		ImGui::Spacing();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Sort Cell Items ) -----------
	//---------------------------------------------------

	// Pinned first, then marked, then by distance
	static void SortCellItems(std::vector<CellItem>& a_items) {

		std::ranges::stable_sort(a_items, [](const CellItem& a_left, const CellItem& a_right) {
			const auto rank = [](const CellItem& a_item) { return a_item.pinned ? 0 : a_item.marked ? 1 : 2; };
			if (rank(a_left) != rank(a_right)) {
				return rank(a_left) < rank(a_right);
			}
			return a_left.distance < a_right.distance;
		});
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Cell List ) ------------
	//---------------------------------------------------

	void Menu::DrawCellList(IDataProvider& a_provider, const CellScan& a_scan) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();

		const auto message = [&](const char* a_key) {
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(a_key).c_str());
			ImGui::PopTextWrapPos();
		};
		if (!a_scan.scanned) {
			message("UI_Cell_Scanning");
			return;
		}
		if (a_scan.excluded) {
			message("UI_Cell_Excluded");
			return;
		}
		if (a_scan.items.empty()) {
			message("UI_Cell_Nothing");
			return;
		}

		auto items = a_scan.items;
		SortCellItems(items);
		const auto tracked = [](const CellItem& a_item) { return a_item.pinned || a_item.marked; };
		struct Group
		{
			const char*                            key;
			std::function<bool(const CellItem&)>  holds;
		};
		const Group groups[] = {
			{ "UI_Cell_Tracking", tracked },
			{ "UI_Cell_Loose", [&](const CellItem& a_item) { return !tracked(a_item) && a_item.place == CellPlace::kLoose; } },
			{ "UI_Cell_Containers", [&](const CellItem& a_item) { return !tracked(a_item) && a_item.place == CellPlace::kContainer; } },
			{ "UI_Cell_Carried", [&](const CellItem& a_item) { return !tracked(a_item) && a_item.place == CellPlace::kCarried; } },
		};
		const float rowHeight = ImGui::GetFrameHeight();
		for (const auto& [key, holds] : groups) {
			const auto count = std::ranges::count_if(items, holds);
			if (count == 0) {
				continue;
			}
			ImGui::Spacing();
			HeadingWithCount(a_provider.Translate(key), std::to_string(count), palette.heading);
			HeadingRule(4.0f);

			for (const auto& item : items) {
				if (!holds(item)) {
					continue;
				}
				char rowID[24];
				std::snprintf(rowID, sizeof(rowID), "%08X:%08X", item.refID, item.formID);
				ImGui::PushID(rowID);
				if (RowSelectable("##CellRow", item.marked, ImVec2(0.0f, rowHeight), ImGuiSelectableFlags_AllowOverlap)) {
					if (item.marked) {
						a_provider.ClearCellMarker(item);
					} else {
						a_provider.MarkCellItem(item);
					}
				}
				if (std::exchange(_focusCellList, false)) {
					_cellParkedRow = ImGui::GetItemID();
				}
				const auto min = ImGui::GetItemRectMin();
				const auto max = ImGui::GetItemRectMax();
				const bool rowHovered = !_padActive && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseHoveringRect(min, max);

				Entry entry;
				entry.name = item.name;
				entry.formID = item.formID;
				const bool pinned = _pins.Has(item.formID);
				if (item.marked && (rowHovered || (ImGui::IsItemFocused() && ImGui::GetIO().NavVisible))) {
					_shownMarker = item.refID;
				}
				if (_pins.enabled && ImGui::IsItemFocused() && ImGui::GetIO().NavVisible) {
					// R2 pins, as on the pages
					_padTriggerKey = pinned ? "UI_Unpin" : "UI_Pin";
					if (ImGui::IsKeyPressed(ImGuiKey_GamepadR2, false)) {
						TogglePin(a_provider, entry);
					}
				}

				auto*       drawList = ImGui::GetWindowDrawList();
				const float centreY = (min.y + max.y) * 0.5f;
				const float textY = centreY - fontSize * 0.5f;
				const float padding = style.FramePadding.x;

				float right = max.x - padding;
				if (_pins.enabled) {
					const float side = rowHeight;
					const auto  pinMin = ImVec2(right - side, min.y);
					bool        pinHovered = false;
					if (rowHovered) {
						const auto after = ImGui::GetCursorScreenPos();
						ImGui::SetCursorScreenPos(pinMin);
						if (ImGui::InvisibleButton("##CellPin", ImVec2(side, side))) {
							TogglePin(a_provider, entry);
						}
						pinHovered = ImGui::IsItemHovered();
						if (pinHovered) {
							drawList->AddRect(pinMin, ImVec2(pinMin.x + side, pinMin.y + side), Colour(palette.accent, 0.6f));
							ImGui::SetTooltip("%s", a_provider.Translate(pinned ? "UI_Unpin" : "UI_Pin").c_str());
						}
						ImGui::SetCursorScreenPos(after);
					}
					if (pinned || rowHovered) {
						DrawPinIcon(drawList, ImVec2(pinMin.x + side * 0.5f, centreY), fontSize, pinned ? Colour(palette.pinned) : pinHovered ? Colour(palette.accent) : Colour(palette.textDim, 0.8f));
					}
					right -= side + padding;
				}
				if (item.distance >= 0.0f) {
					if (_settings && _settings->cellHeight) {
						const float slot = HeightSlotWidth(a_provider);
						DrawHeight(drawList, a_provider, right - slot, centreY, item.height, Colour(palette.textDim));
						right -= slot + fontSize * 0.5f;
					}
					const auto distance = a_provider.Translate("UI_Cell_Metres{" + std::to_string(static_cast<int>(std::lround(item.distance))) + "}");
					const auto size = ImGui::CalcTextSize(distance.c_str());
					DrawDirection(drawList, ImVec2(right - fontSize * 0.4f, centreY), fontSize * 0.75f, item.bearing, Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.textDim));
					right -= fontSize * 1.1f;
					drawList->AddText(ImVec2(right - size.x, textY), Colour(palette.textDim), distance.c_str());
					right -= size.x + fontSize;
				}
				if (!item.holder.empty()) {
					const auto  size = ImGui::CalcTextSize(item.holder.c_str());
					const float x = std::max(min.x + (max.x - min.x) * 0.55f, right - size.x);
					drawList->AddText(ImVec2(x, textY), Colour(palette.textDim), FitText(item.holder, right - x).c_str());
					right = x - fontSize;
				}

				const float marker = ImGui::GetTextLineHeight();
				DrawMarker(drawList, ImVec2(min.x + padding + marker * 0.5f, centreY), marker, Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.needed), item.marked || item.pinned ? 1.0f : 0.0f);
				const float nameX = min.x + padding * 2.0f + marker;
				// Name gives way first
				const float hand = item.stealing ? fontSize * 1.2f : 0.0f;
				const auto  name = FitText(item.name, right - nameX - hand);
				drawList->AddText(ImVec2(nameX, textY), Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.text), name.c_str());
				const float nameRight = nameX + ImGui::CalcTextSize(name.c_str()).x;
				if (item.stealing) {
					DrawStealIcon(drawList, ImVec2(nameRight + hand * 0.5f, centreY), fontSize * 0.9f, Colour(palette.stealing));
					if (rowHovered && ImGui::IsMouseHoveringRect(ImVec2(nameRight, min.y), ImVec2(nameRight + hand, max.y))) {
						ImGui::SetTooltip("%s", a_provider.Translate("UI_Cell_Stealing").c_str());
					}
				}
				if (!item.collection.empty() && name == item.name) {
					const float detailX = nameRight + hand;
					const auto  detail = FitText("  \xC2\xB7  " + item.collection, right - detailX);
					drawList->AddText(ImVec2(detailX, textY), Colour(palette.textDim), detail.c_str());
				}
				ImGui::PopID();
			}
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Toast Placement ) ------
	//---------------------------------------------------

	// Notification placement, hides the main window
	void Menu::DrawToastPlacement(IDataProvider& a_provider, UISettings& a_settings) {

		const auto& io = ImGui::GetIO();
		const auto& palette = Themes::GetPalette();
		if (!_toastSample) {
			_toastSample = std::make_unique<Toasts>();
			_placeFrom = ImVec2(a_settings.toastX, a_settings.toastY);
			const auto entry = a_provider.Translate("UI_Toasts_SampleEntry");
			_toastSample->Push(entry, a_provider.Translate("UI_Toasts_SampleItem1"), ToastKind::kEntry);
			_toastSample->Push(entry, a_provider.Translate("UI_Toasts_SampleItem2"), ToastKind::kEntry);
			_toastSample->Push(a_provider.Translate("UI_Toasts_SampleDiscovery"), a_provider.Translate("UI_Toasts_SampleLocation"), ToastKind::kDiscovery);
		}
		const auto finish = [&](bool a_keep) {
			if (a_keep) {
				_settingsChanged = true;
			} else {
				a_settings.toastX = _placeFrom.x;
				a_settings.toastY = _placeFrom.y;
			}
			_placingToasts = false;
			_toastSample.reset();
		};

		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0.0f, 0.0f), io.DisplaySize, IM_COL32(0, 0, 0, 90));
		ToastOptions options;
		options.x = a_settings.toastX;
		options.y = a_settings.toastY;
		options.newestFirst = a_settings.toastsNewestFirst;
		options.preview = true;
		const auto   bounds = _toastSample->Draw(ImGui::GetTime(), options, {});
		const ImVec2 size(std::max(1.0f, bounds.max.x - bounds.min.x), std::max(1.0f, bounds.max.y - bounds.min.y));
		const auto   travel = Toasts::Travel(size);
		const auto   move = [&](float a_dx, float a_dy) {
			if (travel.x > 0.0f) {
				a_settings.toastX = std::clamp(a_settings.toastX + a_dx / travel.x, 0.0f, 1.0f);
			}
			if (travel.y > 0.0f) {
				a_settings.toastY = std::clamp(a_settings.toastY + a_dy / travel.y, 0.0f, 1.0f);
			}
		};

		ImGui::PushFont(Themes::BodyFont());
		const float fontSize = ImGui::GetFontSize();

		// Mouse drag
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(io.DisplaySize);
		constexpr auto layerFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove |
		                            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		if (ImGui::Begin("##ToastPlacement", nullptr, layerFlags)) {
			ImGui::SetCursorScreenPos(bounds.min);
			ImGui::InvisibleButton("##Stack", size);
			const bool dragging = ImGui::IsItemActive();
			const bool hovered = ImGui::IsItemHovered();
			if (hovered || dragging) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
			}
			if (dragging) {
				move(io.MouseDelta.x, io.MouseDelta.y);
			}
			const float outline = fontSize * 0.4f;
			ImGui::GetForegroundDrawList()->AddRect(ImVec2(bounds.min.x - outline, bounds.min.y - outline), ImVec2(bounds.max.x + outline, bounds.max.y + outline),
				Colour(palette.accent, hovered || dragging ? 1.0f : 0.6f), fontSize * 0.25f, 0, 1.5f);
		}
		ImGui::End();

		// Arrows, d-pad and left stick
		const float speed = io.DeltaTime * 0.5f;
		const auto  held = [](ImGuiKey a_a, ImGuiKey a_b, ImGuiKey a_c) { return ImGui::IsKeyDown(a_a) || ImGui::IsKeyDown(a_b) || ImGui::IsKeyDown(a_c); };
		const float across = (held(ImGuiKey_RightArrow, ImGuiKey_GamepadDpadRight, ImGuiKey_GamepadLStickRight) ? 1.0f : 0.0f) - (held(ImGuiKey_LeftArrow, ImGuiKey_GamepadDpadLeft, ImGuiKey_GamepadLStickLeft) ? 1.0f : 0.0f);
		const float down = (held(ImGuiKey_DownArrow, ImGuiKey_GamepadDpadDown, ImGuiKey_GamepadLStickDown) ? 1.0f : 0.0f) - (held(ImGuiKey_UpArrow, ImGuiKey_GamepadDpadUp, ImGuiKey_GamepadLStickUp) ? 1.0f : 0.0f);
		a_settings.toastX = std::clamp(a_settings.toastX + across * speed, 0.0f, 1.0f);
		a_settings.toastY = std::clamp(a_settings.toastY + down * speed, 0.0f, 1.0f);

		const float panelY = (bounds.min.y + bounds.max.y) * 0.5f < io.DisplaySize.y * 0.5f ? io.DisplaySize.y * 0.68f : io.DisplaySize.y * 0.32f;
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, panelY), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		auto background = palette.background;
		background.w = std::clamp(a_settings.opacity, 0.3f, 1.0f);
		windowOpacity = background.w;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
		const float frameDepth = Decor::FrameDepth();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(fontSize * 1.4f + frameDepth, fontSize * 1.1f + frameDepth));
		constexpr auto panelFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
		bool           keep = false;
		bool           cancel = false;
		if (ImGui::Begin("##ToastPlacementPanel", nullptr, panelFlags)) {
			const auto min = ImGui::GetWindowPos();
			const auto max = ImVec2(min.x + ImGui::GetWindowWidth(), min.y + ImGui::GetWindowHeight());
			const auto inset = fontSize * 0.3f;
			Decor::Frame(ImGui::GetWindowDrawList(), ImVec2(min.x + inset, min.y + inset), ImVec2(max.x - inset, max.y - inset), Colour(palette.separator));

			const float width = fontSize * 24.0f;
			const auto  centred = [&](const std::string& a_text, const ImVec4& a_colour) {
				const float textWidth = ImGui::CalcTextSize(a_text.c_str()).x;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max(0.0f, (width - textWidth) * 0.5f));
				ImGui::TextColored(a_colour, "%s", a_text.c_str());
			};
			ImGui::PushFont(Themes::HeaderFont());
			centred(a_provider.Translate("UI_Toasts_PlaceTitle"), palette.heading);
			ImGui::PopFont();
			ImGui::Dummy(ImVec2(width, fontSize * 0.3f));
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + width);
			ImGui::TextUnformatted(a_provider.Translate("UI_Toasts_PlaceHint").c_str());
			const auto padStyle = a_settings.padStyle;
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Toasts_PlaceKeys{" + GamepadButtonName(0x1000, padStyle) + "}{" + GamepadButtonName(0x2000, padStyle) + "}").c_str());
			ImGui::PopTextWrapPos();
			ImGui::Dummy(ImVec2(width, fontSize * 0.4f));

			const float buttonWidth = (width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
			keep = ThemedButton(a_provider.Translate("UI_Toasts_PlaceConfirm").c_str(), buttonWidth);
			ImGui::SameLine();
			cancel = ThemedButton(a_provider.Translate("UI_Toasts_PlaceCancel").c_str(), buttonWidth);
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopFont();

		keep |= ImGui::IsKeyPressed(ImGuiKey_Enter, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown, false);
		cancel |= ImGui::IsKeyPressed(ImGuiKey_Escape, false) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false);
		if (keep || cancel) {
			finish(keep && !cancel);
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Cell Panel ) -----------
	//---------------------------------------------------

	void Menu::DrawCellPanel(IDataProvider& a_provider, const UISettings& a_settings) {

		// Says why when there's nothing to show
		auto        scan = a_provider.GetCellScan();
		const char* empty = !scan.scanned ? "UI_Cell_Scanning" : scan.excluded ? "UI_Cell_Excluded" : scan.items.empty() ? (scan.interior ? "UI_Cell_Nothing" : "UI_Cell_NothingInRange") : nullptr;
		SortCellItems(scan.items);

		const auto& io = ImGui::GetIO();
		ImGui::PushFont(Themes::BodyFont());
		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();
		const float frameDepth = Decor::FrameDepth();
		const float width = fontSize * 17.0f + frameDepth * 2.0f;
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - width - fontSize * 1.5f, io.DisplaySize.y * 0.14f), ImGuiCond_Always);
		ImGui::SetNextWindowSizeConstraints(ImVec2(width, 0.0f), ImVec2(width, io.DisplaySize.y * 0.6f));

		auto background = palette.background;
		background.w = std::clamp(a_settings.opacity, 0.3f, 1.0f) * 0.85f;
		windowOpacity = background.w;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(fontSize * 0.9f + frameDepth, fontSize * 0.7f + frameDepth));
		constexpr auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings |
		                       ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (ImGui::Begin("##CellPanel", nullptr, flags)) {
			auto*      drawList = ImGui::GetWindowDrawList();
			const auto winMin = ImGui::GetWindowPos();
			const auto winMax = ImVec2(winMin.x + ImGui::GetWindowWidth(), winMin.y + ImGui::GetWindowHeight());
			const float inset = fontSize * 0.25f;
			Decor::Frame(drawList, ImVec2(winMin.x + inset, winMin.y + inset), ImVec2(winMax.x - inset, winMax.y - inset), Colour(palette.separator));

			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", a_provider.Translate("UI_Cell_Title").c_str());
			ImGui::PopFont();
			if (!empty) {
				const auto count = std::to_string(scan.items.size());
				ImGui::SameLine(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize(count.c_str()).x);
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(palette.textDim, "%s", count.c_str());
			}
			const auto line = ImGui::GetCursorScreenPos();
			const float inner = ImGui::GetContentRegionAvail().x;
			Decor::FadedLine(drawList, line, ImVec2(line.x + inner, line.y), Colour(palette.separator));
			ImGui::Dummy(ImVec2(inner, 3.0f));

			if (empty) {
				ImGui::PushTextWrapPos(0.0f);
				ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(empty).c_str());
				ImGui::PopTextWrapPos();
				ImGui::Dummy(ImVec2(inner, fontSize * 0.3f));
			}

			constexpr std::size_t shown = 8;
			const float           rowHeight = ImGui::GetTextLineHeightWithSpacing();
			for (std::size_t i = 0; i < scan.items.size() && i < shown; ++i) {
				const auto& item = scan.items[i];
				const auto  start = ImGui::GetCursorScreenPos();
				const float centreY = start.y + fontSize * 0.5f;
				// Gold for marked, blue for pinned
				const ImU32 colour = Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.text);

				float right = start.x + inner;
				if (a_settings.cellHeight) {
					const float slot = HeightSlotWidth(a_provider);
					DrawHeight(drawList, a_provider, right - slot, centreY, item.height, Colour(palette.textDim));
					right -= slot + fontSize * 0.4f;
				}
				DrawDirection(drawList, ImVec2(right - fontSize * 0.4f, centreY), fontSize * 0.7f, item.bearing, Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.textDim));
				right -= fontSize;
				if (item.distance >= 0.0f) {
					const auto distance = a_provider.Translate("UI_Cell_Metres{" + std::to_string(static_cast<int>(std::lround(item.distance))) + "}");
					const auto size = ImGui::CalcTextSize(distance.c_str());
					drawList->AddText(ImVec2(right - size.x, start.y), Colour(palette.textDim), distance.c_str());
					right -= size.x + fontSize * 0.6f;
				}
				const float glyph = fontSize * 0.8f;
				DrawPlaceGlyph(drawList, ImVec2(start.x + glyph * 0.5f, start.y + fontSize * 0.5f), glyph, item.place, Colour(item.marked ? palette.accent : item.pinned ? palette.pinned : palette.textDim));
				const float nameX = start.x + glyph + fontSize * 0.45f;
				const float hand = item.stealing ? fontSize * 1.1f : 0.0f;
				const auto  name = FitText(item.name, right - fontSize * 0.3f - nameX - hand);
				drawList->AddText(ImVec2(nameX, start.y), colour, name.c_str());
				if (item.stealing) {
					DrawStealIcon(drawList, ImVec2(nameX + ImGui::CalcTextSize(name.c_str()).x + hand * 0.5f, start.y + fontSize * 0.5f), fontSize * 0.75f, Colour(palette.stealing));
				}
				ImGui::Dummy(ImVec2(inner, rowHeight - ImGui::GetStyle().ItemSpacing.y));
			}
			if (scan.items.size() > shown) {
				ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Cell_More{" + std::to_string(scan.items.size() - shown) + "}").c_str());
			}
			ImGui::Dummy(ImVec2(inner, fontSize * 0.2f));
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopFont();
	}

	//---------------------------------------------------
	//-- Menu ( Find ) ----------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Build Find Index ) ----------
	//---------------------------------------------------

	void Menu::BuildFindIndex(IDataProvider& a_provider) {

		_findIndex.clear();
		for (const auto& category : _categories) {
			for (const auto& summary : category.pages) {
				const auto page = a_provider.GetPage(summary.id);
				const auto title = a_provider.Translate(summary.title);
				for (const auto& section : page.sections) {
					for (const auto& entry : section.entries) {
						_findIndex.push_back({ summary.id, title, category.Group(), entry, page.LayoutOf(section) == PageLayout::kJournal });
					}
				}
			}
		}
		_findIndexDirty = false;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Jump To ) -------------------
	//---------------------------------------------------

	void Menu::JumpTo(const FindItem& a_item) {

		OpenPage(a_item.pageID);

		if (_hideCompleted && a_item.entry.IsComplete()) {
			_hideCompleted = false;
		}
		if (a_item.journal) {
			_journalKey = a_item.entry.key.empty() ? a_item.entry.name : a_item.entry.key;
			_scrollToKey = _journalKey;
		} else {
			PinEntry(a_item.entry);
			_scrollToKey = a_item.entry.name + '\x1f' + a_item.entry.detail;
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Find ) -----------------
	//---------------------------------------------------

	void Menu::DrawFind(IDataProvider& a_provider) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();
		_hoveredEntry.reset();
		if (_findIndexDirty) {
			BuildFindIndex(a_provider);
		}

		UISettings  fallback;
		auto&       options = _settings ? *_settings : fallback;
		const int   matchMode = std::clamp(options.findMatch, 0, 2);
		constexpr std::size_t maxResults = 500;

		const auto                     needle = Lower(std::string(_findText.data()));
		std::vector<const FindItem*>   results;
		bool                           truncated = false;
		if (!needle.empty()) {
			for (const auto& item : _findIndex) {
				if (!_findCategory.empty() && item.group != _findCategory) {
					continue;
				}
				if (options.findIgnoreCompleted && item.entry.IsComplete()) {
					continue;
				}
				const auto name = Lower(item.entry.name);
				const bool match = matchMode == 1 ? name.starts_with(needle) : matchMode == 2 ? name == needle : name.find(needle) != std::string::npos;
				if (!match) {
					continue;
				}
				if (results.size() >= maxResults) {
					truncated = true;
					break;
				}
				results.push_back(std::addressof(item));
			}
		}

		HeadingWithCount(a_provider.Translate("UI_Find"), needle.empty() ? std::string{} : std::to_string(results.size()) + (truncated ? "+" : ""), palette.heading);
		HeadingRule(2.0f);
		ImGui::Spacing();

		//---------------------------------------------------
		// Search box and options
		//---------------------------------------------------

		if (_findFocus) {
			if (!_padActive) {
				ImGui::SetKeyboardFocusHere();
			}
			_findFocus = false;
		}
		ThemedFilterInput("##FindText", a_provider.Translate("UI_Find_Hint").c_str(), _findText.data(), _findText.size(), fontSize * 18.0f);

		std::vector<std::string> groups;
		for (const auto& category : _categories) {
			if (std::ranges::find(groups, category.Group()) == groups.end()) {
				groups.push_back(category.Group());
			}
		}
		ImGui::SameLine(0.0f, fontSize);
		const auto allLabel = a_provider.Translate("UI_Find_All");
		if (ThemedBeginCombo("##FindCategory", _findCategory.empty() ? allLabel.c_str() : a_provider.Translate(_findCategory).c_str(), fontSize * 10.0f)) {
			if (ImGui::Selectable(allLabel.c_str(), _findCategory.empty())) {
				_findCategory.clear();
			}
			for (const auto& group : groups) {
				if (ImGui::Selectable(a_provider.Translate(group).c_str(), group == _findCategory)) {
					_findCategory = group;
				}
			}
			ImGui::EndCombo();
		}

		constexpr const char* matchKeys[] = { "$SearchTypeChoice01", "$SearchTypeChoice02", "$SearchTypeChoice03" };
		ImGui::SameLine(0.0f, fontSize);
		if (ThemedBeginCombo("##FindMatch", a_provider.Translate(matchKeys[matchMode]).c_str(), fontSize * 11.0f)) {
			for (int i = 0; i < 3; ++i) {
				if (ImGui::Selectable(a_provider.Translate(matchKeys[i]).c_str(), i == matchMode) && i != matchMode) {
					options.findMatch = i;
					_settingsChanged = true;
				}
			}
			ImGui::EndCombo();
		}
		// On the next line when the window is too narrow for it
		const auto ignoreLabel = a_provider.Translate("UI_Find_IgnoreCompleted");
		const float ignoreWidth = ImGui::GetFrameHeight() * 2.0f + ImGui::CalcTextSize(ignoreLabel.c_str()).x;
		if (ImGui::GetItemRectMax().x + fontSize + ignoreWidth <= ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x) {
			ImGui::SameLine(0.0f, fontSize);
		}
		if (MarkerToggle("##FindIgnoreCompleted", options.findIgnoreCompleted, ignoreLabel.c_str())) {
			_settingsChanged = true;
		}

		DividerRow();

		//---------------------------------------------------
		// Results
		//---------------------------------------------------

		BeginFramedPanel("##FindResults", ImVec2(0.0f, 0.0f));
		ScrollWithRightStick();
		if (needle.empty()) {
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Find_Prompt").c_str());
			ImGui::PopTextWrapPos();
		} else if (results.empty()) {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_NothingMatches").c_str());
		}

		const FindItem* chosen = nullptr;
		const float     rowHeight = ImGui::GetFrameHeight();
		for (std::size_t i = 0; i < results.size(); ++i) {
			const auto& item = *results[i];
			ImGui::PushID(static_cast<int>(i));
			if (RowSelectable("##Result", false, ImVec2(0.0f, rowHeight))) {
				chosen = std::addressof(item);
			}
			ImGui::PopID();

			const auto  min = ImGui::GetItemRectMin();
			const auto  max = ImGui::GetItemRectMax();
			auto*       drawList = ImGui::GetWindowDrawList();
			const float centreY = (min.y + max.y) * 0.5f;
			const float textY = centreY - fontSize * 0.5f;
			const float marker = ImGui::GetTextLineHeight();
			DrawMarker(drawList, ImVec2(min.x + style.FramePadding.x + marker * 0.5f, centreY), marker, Colour(StateColour(item.entry.state)), item.entry.IsComplete() ? 1.0f : 0.0f);

			const auto  where = a_provider.Translate(item.group) + "  \xC2\xB7  " + item.pageTitle;
			const float whereWidth = ImGui::CalcTextSize(where.c_str()).x;
			const float whereX = std::max(min.x + (max.x - min.x) * 0.5f, max.x - style.FramePadding.x - whereWidth);
			drawList->PushClipRect(ImVec2(whereX, min.y), max, true);
			drawList->AddText(ImVec2(whereX, textY), Colour(palette.textDim), where.c_str());
			drawList->PopClipRect();
			if (_pins.Has(item.entry.formID)) {
				DrawPinIcon(drawList, ImVec2(whereX - fontSize, centreY), fontSize, Colour(palette.pinned));
			}

			auto name = item.entry.name;
			if (!item.entry.detail.empty()) {
				name += "  (" + item.entry.detail + ")";
			}
			drawList->PushClipRect(min, ImVec2(whereX - fontSize, max.y), true);
			drawList->AddText(ImVec2(min.x + style.FramePadding.x * 2.0f + marker, textY), Colour(item.entry.IsComplete() ? palette.textDim : palette.text), name.c_str());
			drawList->PopClipRect();
			Decor::FadedLine(drawList, ImVec2(min.x, max.y), ImVec2(max.x, max.y), Colour(palette.separator, 0.35f));
		}
		if (truncated) {
			ImGui::Spacing();
			ImGui::PushTextWrapPos(0.0f);
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Find_Limit").c_str());
			ImGui::PopTextWrapPos();
		}
		EndFramedPanel();

		if (chosen) {
			JumpTo(*chosen);
		}
	}

	//---------------------------------------------------
	//-- Menu ( Journal ) -------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Journal ) --------------
	//---------------------------------------------------

	void Menu::DrawJournal(IDataProvider& a_provider, const Page& a_page) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();
		_hoveredEntry.reset();

		// Kept by key so it survives refreshes; nothing is picked until the player picks it
		const auto   keyOf = [](const Entry& a_entry) { return a_entry.key.empty() ? a_entry.name : a_entry.key; };
		const Entry* selected = nullptr;
		for (const auto& section : a_page.sections) {
			for (const auto& entry : section.entries) {
				if (PassesFilter(entry) && keyOf(entry) == _journalKey) {
					selected = std::addressof(entry);
				}
			}
		}

		// Open quests on the left, completed ones above the description
		const auto completedSection = [](const Section& a_section) { return a_section.title == "UI_QuestSection_Completed"; };
		const auto drawSections = [&](bool a_completed) {
			bool anyShown = false;
			for (const auto& section : a_page.sections) {
				if (completedSection(section) != a_completed) {
					continue;
				}
				std::vector<const Entry*> entries;
				for (const auto& entry : section.entries) {
					if (PassesFilter(entry)) {
						entries.push_back(std::addressof(entry));
					}
				}
				if (entries.empty()) {
					continue;
				}
				if (_sort == SortMode::kName) {
					std::ranges::stable_sort(entries, [](const Entry* a, const Entry* b) { return Lower(a->name) < Lower(b->name); });
				}
				anyShown = true;
				const int journalSection = _spotSection++;
				int       journalIndex = 0;

				ImGui::PushID(section.title.c_str());
				ImGui::Spacing();
				HeadingWithCount(a_provider.Translate(section.title), std::to_string(entries.size()), palette.heading);
				HeadingRule(4.0f);

				for (const auto* entry : entries) {
					ImGui::PushID(keyOf(*entry).c_str());
					const float rowHeight = ImGui::GetFrameHeight();
					if (RowSelectable("##Quest", entry == selected, ImVec2(0.0f, rowHeight), ImGuiSelectableFlags_AllowOverlap)) {
						_journalKey = keyOf(*entry);
					}
					const bool rowFocused = ImGui::IsItemFocused() && ImGui::GetIO().NavVisible;
					RecordSpot(_entrySpots, journalSection, journalIndex++, 1, static_cast<int>(entries.size()));
					KeepFocus("quest\x1f" + keyOf(*entry));
					if (rowFocused && _padActive && keyOf(*entry) != _journalKey) {
						_journalKey = keyOf(*entry);
					}
					if (!_scrollToKey.empty() && _scrollToKey == keyOf(*entry)) {
						ImGui::SetScrollHereY(0.35f);
						ImGui::SetFocusID(ImGui::GetItemID(), ImGui::GetCurrentWindow());
						_scrollToKey.clear();
					}
					const auto min = ImGui::GetItemRectMin();
					const auto max = ImGui::GetItemRectMax();

					// Tick to mark complete for the mouse, Square on a controller
					float right = max.x - style.FramePadding.x;
					if (entry->canToggle) {
						const float side = rowHeight;
						const bool  hovered = !_padActive && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseHoveringRect(min, max);
						const auto  action = RowAction::kComplete;
						if (rowFocused) {
							_padActionKey = RowActionKey(action, entry->IsComplete());
							if (ImGui::IsKeyPressed(rowActionKey, false)) {
								_journalKey = keyOf(*entry);
								DoRowAction(a_provider, *entry, action);
							}
						}
						if (hovered) {
							if (RowActionButton(a_provider, action, entry->IsComplete(), ImVec2(right - side, std::round((min.y + max.y - side) * 0.5f)), side, hovered)) {
								_journalKey = keyOf(*entry);
								DoRowAction(a_provider, *entry, action);
							}
						}
						right -= side;
					}
					ImGui::PopID();

					auto*       drawList = ImGui::GetWindowDrawList();
					const float marker = ImGui::GetTextLineHeight();
					const float centreY = (min.y + max.y) * 0.5f;
					DrawMarker(drawList, ImVec2(min.x + style.FramePadding.x + marker * 0.5f, centreY), marker, Colour(StateColour(entry->state)), entry->IsComplete() ? 1.0f : 0.0f);

					const float textX = min.x + style.FramePadding.x * 2.0f + marker;
					const float textY = centreY - fontSize * 0.5f;
					if (!entry->detail.empty()) {
						const float detailWidth = ImGui::CalcTextSize(entry->detail.c_str()).x;
						right -= detailWidth;
						drawList->AddText(ImVec2(right, textY), Colour(palette.textDim), entry->detail.c_str());
						right -= style.FramePadding.x;
					}
					drawList->PushClipRect(min, ImVec2(right, max.y), true);
					drawList->AddText(ImVec2(textX, textY), Colour(entry->IsComplete() ? palette.textDim : palette.text), entry->name.c_str());
					drawList->PopClipRect();
				}
				ImGui::PopID();
			}
			return anyShown;
		};

		const float gap = fontSize * 0.75f;
		UpdateDescriptionPanel(ImGui::GetContentRegionAvail().y, gap);
		const float panelHeight = DescriptionPanelHeight();

		const float listWidth = std::max(fontSize * 16.0f, ImGui::GetContentRegionAvail().x * 0.42f);
		BeginFramedPanel("##JournalList", ImVec2(listWidth, 0.0f));
		if (!drawSections(false)) {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_NothingMatches").c_str());
		}
		EndFramedPanel();

		ImGui::SameLine(0.0f, gap);
		ImGui::BeginGroup();
		BeginFramedPanel("##JournalCompleted", ImVec2(0.0f, -(panelHeight + gap)));
		if (!drawSections(true)) {
			ImGui::Spacing();
			HeadingWithCount(a_provider.Translate("UI_QuestSection_Completed"), "0", palette.heading);
			HeadingRule(4.0f);
		}
		EndFramedPanel();
		DrawDescriptionPanel(a_provider, panelHeight, gap, &a_page, selected);
		ImGui::EndGroup();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Quest Detail ) ---------
	//---------------------------------------------------

	// Type, page and status, then "Label: text" lines with their own small heading
	void Menu::DrawQuestDetail(IDataProvider& a_provider, const Page& a_page, const Entry& a_entry) {

		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();
		const bool  complete = a_entry.IsComplete();

		std::string subtitle;
		const auto  append = [&](const std::string& a_part) {
			if (!a_part.empty()) {
				subtitle += (subtitle.empty() ? "" : "  \xC2\xB7  ") + a_part;
			}
		};
		append(a_entry.type.empty() ? std::string{} : a_provider.Translate(a_entry.type));
		append(a_provider.Translate(a_page.title));
		append(a_provider.Translate(a_entry.status.empty() ? std::string(StateKey(a_entry.state)) : a_entry.status));
		append(a_entry.detail);
		if (a_entry.marked && a_entry.detail != a_provider.Translate("UI_QuestMarked")) {
			append(a_provider.Translate("UI_QuestMarked"));
		}
		ImGui::TextColored(complete ? palette.found : palette.textDim, "%s", subtitle.c_str());
		ImGui::Spacing();

		std::string_view text = a_entry.description;
		while (!text.empty()) {
			const auto lineEnd = text.find('\n');
			auto       line = text.substr(0, lineEnd);
			text = lineEnd == std::string_view::npos ? std::string_view{} : text.substr(lineEnd + 1);
			while (!line.empty() && line.front() == ' ') {
				line.remove_prefix(1);
			}
			if (line.empty()) {
				continue;
			}

			const auto colon = line.find(": ");
			if (colon != std::string_view::npos && colon <= 24) {
				auto value = line.substr(colon + 2);
				while (!value.empty() && (value.front() == ' ' || value.front() == '-')) {
					value.remove_prefix(1);
				}
				ImGui::PushFont(Themes::SmallFont());
				ImGui::TextColored(palette.textDim, "%.*s", static_cast<int>(colon), line.data());
				ImGui::PopFont();
				ImGui::TextUnformatted(value.data(), value.data() + value.size());
			} else {
				ImGui::TextUnformatted(line.data(), line.data() + line.size());
			}
			ImGui::Dummy(ImVec2(0.0f, fontSize * 0.4f));
		}

		if (!a_entry.canToggle && complete) {
			ImGui::Dummy(ImVec2(0.0f, fontSize * 0.6f));
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_QuestCompletedInGame").c_str());
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Section ) --------------
	//---------------------------------------------------

	bool Menu::DrawSection(IDataProvider& a_provider, const Section& a_section, bool a_showTitle) {

		const auto& palette = Themes::GetPalette();

		std::vector<const Entry*> entries;
		entries.reserve(a_section.entries.size());
		for (const auto& entry : a_section.entries) {
			if (PassesFilter(entry)) {
				entries.push_back(std::addressof(entry));
			}
		}

		if (entries.empty()) {
			return false;
		}

		if (_sort == SortMode::kName) {
			std::ranges::stable_sort(entries, [](const Entry* a, const Entry* b) { return Lower(a->name) < Lower(b->name); });
		} else if (_sort == SortMode::kNeededFirst) {
			std::ranges::stable_sort(entries, [](const Entry* a, const Entry* b) { return !a->IsComplete() && b->IsComplete(); });
		}

		ImGui::PushID(a_section.title.c_str());
		if (a_showTitle) {
			ImGui::Spacing();
			HeadingWithCount(a_provider.Translate(a_section.title), CountText(a_section.Found(), a_section.Total()), palette.heading);
			const auto  pos = ImGui::GetCursorScreenPos();
			const float width = ImGui::GetContentRegionAvail().x;
			Decor::FadedLine(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y), ImVec2(pos.x + width, pos.y), Colour(palette.separator));
			ImGui::Dummy(ImVec2(width, 4.0f));
		}

		const bool  hasDetail = std::ranges::any_of(entries, [](const Entry* a_entry) { return !a_entry->detail.empty(); });
		const float columnWidth = ImGui::GetFontSize() * (hasDetail ? 28.0f : 19.0f);
		const int   autoColumns = std::clamp(static_cast<int>(ImGui::GetContentRegionAvail().x / columnWidth), 1, 4);
		const int   columns = _settings && _settings->columns > 0 ? _settings->columns : autoColumns;
		const auto  tableFlags = ImGuiTableFlags_SizingStretchSame;
		if (ImGui::BeginTable("##Entries", columns, tableFlags)) {
			const int section = _spotSection++;
			int       index = 0;
			for (const auto* entry : entries) {
				ImGui::TableNextColumn();
				ImGui::PushID(index);
				_spotNext.section = section;
				_spotNext.index = index;
				_spotNext.columns = columns;
				_spotNext.count = static_cast<int>(entries.size());
				DrawEntry(a_provider, *entry);
				ImGui::PopID();
				++index;
			}
			ImGui::EndTable();
		}

		ImGui::PopID();
		return true;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Entry ) ----------------
	//---------------------------------------------------

	void Menu::DrawEntry(IDataProvider& a_provider, const Entry& a_entry) {

		const auto& palette = Themes::GetPalette();
		const float cellWidth = ImGui::GetContentRegionAvail().x;
		const auto  start = ImGui::GetCursorScreenPos();
		const auto  key = a_entry.name + '\x1f' + a_entry.detail;

		if (RowSelectable("##Row", _pinnedEntry && _pinnedKey == key, ImVec2(cellWidth, ImGui::GetFrameHeight()), ImGuiSelectableFlags_AllowOverlap)) {
			_pinnedEntry = a_entry;
			_pinnedKey = key;
		}
		// The highlight box, which ImGui sizes per column, so the buttons sit inside it
		const auto boxMin = ImGui::GetItemRectMin();
		const auto boxMax = ImGui::GetItemRectMax();
		RecordSpot(_entrySpots, _spotNext.section, _spotNext.index, _spotNext.columns, _spotNext.count);
		KeepFocus(key);
		if (!_scrollToKey.empty() && _scrollToKey == key) {
			// From Find
			ImGui::SetScrollHereY(0.35f);
			ImGui::SetFocusID(ImGui::GetItemID(), ImGui::GetCurrentWindow());
			_scrollToKey.clear();
		}
		if (_scrollToForm != 0 && a_entry.formID == _scrollToForm) {
			// From the pinned bar
			ImGui::SetScrollHereY(0.35f);
			ImGui::SetFocusID(ImGui::GetItemID(), ImGui::GetCurrentWindow());
			_pinnedEntry = a_entry;
			_pinnedKey = key;
			_scrollToForm = 0;
		}
		const bool focused = ImGui::IsItemHovered() || ImGui::IsItemFocused();
		if (focused) {
			_hoveredEntry = a_entry;
		}

		// Row action, X on a controller
		// Square always marks complete, R2 pins or sets the location marker
		const auto action = RowActionFor(a_entry);
		const auto place = action == RowAction::kMarker || action == RowAction::kPin ? action : RowAction::kNone;
		const bool on = place == RowAction::kMarker ? _pins.Locates(a_entry.MarkerID())
		              : place == RowAction::kPin && _pins.Has(a_entry.formID);
		const bool tick = a_entry.canToggle;
		const bool rowFocused = ImGui::IsItemFocused() && ImGui::GetIO().NavVisible;
		if (rowFocused && tick) {
			_padActionKey = RowActionKey(RowAction::kComplete, a_entry.IsComplete());
			if (ImGui::IsKeyPressed(rowActionKey, false)) {
				DoRowAction(a_provider, a_entry, RowAction::kComplete);
			}
		}
		if (rowFocused && place != RowAction::kNone) {
			_padTriggerKey = RowActionKey(place, on);
			if (ImGui::IsKeyPressed(ImGuiKey_GamepadR2, false)) {
				DoRowAction(a_provider, a_entry, place);
			}
		}
		const auto rowMax = ImVec2(start.x + cellWidth, start.y + ImGui::GetFrameHeight());
		const bool rowHovered = !_padActive && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseHoveringRect(start, rowMax);
		const bool showButtons = rowHovered || (rowFocused && _padActive);
		const bool showPlace = place != RowAction::kNone && (showButtons || on);
		// The tick is for the mouse, a controller has Square
		const bool showTick = tick && rowHovered;
		const float side = ImGui::GetFrameHeight();
		const float buttonsRight = boxMax.x - ImGui::GetStyle().FramePadding.x;
		const float buttonsY = std::round((boxMin.y + boxMax.y - side) * 0.5f);

		// Text stops short of the buttons
		const float buttons = (showPlace ? side : 0.0f) + (showTick ? side : 0.0f);
		ImGui::PushClipRect(start, ImVec2(std::min(rowMax.x, buttonsRight - buttons), rowMax.y), true);
		ImGui::SetCursorScreenPos(start);
		StateMarker(a_entry.state);

		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(a_entry.IsComplete() ? palette.found : palette.needed, "%s", a_entry.name.c_str());
		if (!a_entry.words.empty()) {
			const float spacing = ImGui::GetStyle().ItemSpacing.x;
			ImGui::SameLine();
			ImGui::TextColored(palette.textDim, "(");
			for (std::size_t i = 0; i < a_entry.words.size(); ++i) {
				const auto& word = a_entry.words[i];
				ImGui::SameLine(0.0f, i == 0 ? 0.0f : spacing * 0.5f);
				ImGui::TextColored(word.done ? palette.found : palette.textDim, "%s", word.text.c_str());
				ImGui::SameLine(0.0f, 0.0f);
				ImGui::TextColored(palette.textDim, i + 1 < a_entry.words.size() ? "," : ")");
			}
		}
		if (!a_entry.detail.empty()) {
			ImGui::SameLine();
			ImGui::TextColored(palette.textDim, "%s", a_entry.detail.c_str());
		}
		ImGui::PopClipRect();

		// Pin or marker at the right, the tick beside it
		if (showPlace && RowActionButton(a_provider, place, on, ImVec2(buttonsRight - side, buttonsY), side, rowHovered)) {
			DoRowAction(a_provider, a_entry, place);
		}
		if (showTick) {
			ImGui::PushID("##Tick");
			if (RowActionButton(a_provider, RowAction::kComplete, a_entry.IsComplete(), ImVec2(buttonsRight - side * (showPlace ? 2.0f : 1.0f), buttonsY), side, rowHovered)) {
				DoRowAction(a_provider, a_entry, RowAction::kComplete);
			}
			ImGui::PopID();
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Row Action For ) ------------
	//---------------------------------------------------

	Menu::RowAction Menu::RowActionFor(const Entry& a_entry) const {

		const bool pinned = _pins.Has(a_entry.formID);
		if (a_entry.canMark) {
			return RowAction::kMarker;
		}
		if (_pins.enabled && (a_entry.canPin || pinned)) {
			return RowAction::kPin;
		}
		return a_entry.canToggle ? RowAction::kComplete : RowAction::kNone;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Row Action Key ) ------------
	//---------------------------------------------------

	const char* Menu::RowActionKey(RowAction a_action, bool a_on) {

		switch (a_action) {
		case RowAction::kMarker:
			return a_on ? "UI_Marker_Remove" : "UI_Marker_Place";
		case RowAction::kPin:
			return a_on ? "UI_Unpin" : "UI_Pin";
		case RowAction::kComplete:
			return a_on ? "UI_QuestMarkIncomplete" : "UI_QuestMarkComplete";
		default:
			return "";
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Do Row Action ) -------------
	//---------------------------------------------------

	void Menu::DoRowAction(IDataProvider& a_provider, const Entry& a_entry, RowAction a_action) {

		switch (a_action) {
		case RowAction::kMarker:
			ToggleLocationMarker(a_provider, a_entry);
			break;
		case RowAction::kPin:
			TogglePin(a_provider, a_entry);
			break;
		case RowAction::kComplete:
			a_provider.SetCompleted(_selectedPage, a_entry, !a_entry.IsComplete());
			_dirty = true;
			break;
		default:
			break;
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Row Action Button ) ---------
	//---------------------------------------------------

	bool Menu::RowActionButton(IDataProvider& a_provider, RowAction a_action, bool a_on, ImVec2 a_min, float a_side, bool a_clickable) {

		const auto& palette = Themes::GetPalette();
		auto*       drawList = ImGui::GetWindowDrawList();
		const auto  max = ImVec2(a_min.x + a_side, a_min.y + a_side);
		bool        clicked = false;
		bool        hovered = false;
		if (a_clickable) {
			ImGui::SetCursorScreenPos(a_min);
			clicked = ImGui::InvisibleButton("##RowAction", ImVec2(a_side, a_side));
			hovered = ImGui::IsItemHovered();
			if (hovered) {
				drawList->AddRect(a_min, max, Colour(palette.accent, 0.6f));
				ImGui::SetTooltip("%s", a_provider.Translate(RowActionKey(a_action, a_on)).c_str());
			}
		}
		const bool  lit = (a_on && a_action != RowAction::kComplete) || hovered;
		// Pinned is blue like the Cell Scanner
		const auto  colour = a_action == RowAction::kPin && a_on && !hovered ? Colour(palette.pinned) : lit ? Colour(palette.accent) : a_action == RowAction::kComplete && a_on ? Colour(palette.found, 0.9f) : Colour(palette.textDim, 0.8f);
		const auto  centre = ImVec2((a_min.x + max.x) * 0.5f, (a_min.y + max.y) * 0.5f);
		const float glyph = ImGui::GetFontSize();
		switch (a_action) {
		case RowAction::kMarker:
			DrawMarkerIcon(drawList, centre, glyph, colour);
			break;
		case RowAction::kPin:
			DrawPinIcon(drawList, centre, glyph, colour);
			break;
		case RowAction::kComplete:
			DrawCheckIcon(drawList, centre, glyph, colour);
			break;
		default:
			break;
		}
		return clicked;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Toggle Pin ) ----------------
	//---------------------------------------------------

	void Menu::TogglePin(IDataProvider& a_provider, const Entry& a_entry) {

		if (_pins.Has(a_entry.formID)) {
			a_provider.ClearPin(a_entry.formID);
		} else {
			a_provider.SetPin(a_entry);
		}
		_pins = a_provider.GetPins();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Toggle Location Marker ) ----
	//---------------------------------------------------

	void Menu::ToggleLocationMarker(IDataProvider& a_provider, const Entry& a_entry) {

		if (_pins.Locates(a_entry.MarkerID())) {
			a_provider.ClearLocationMarker(a_entry.MarkerID());
		} else {
			a_provider.SetLocationMarker(a_entry);
		}
		_pins = a_provider.GetPins();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Pinned Bar ) -----------
	//---------------------------------------------------

	void Menu::DrawPinnedBar(IDataProvider& a_provider) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const float fontSize = ImGui::GetFontSize();
		auto*       drawList = ImGui::GetWindowDrawList();

		const auto  start = ImGui::GetCursorScreenPos();
		const float width = ImGui::GetContentRegionAvail().x;
		const float height = ImGui::GetFrameHeight() + fontSize * 0.5f;
		const auto  max = ImVec2(start.x + width, start.y + height);
		drawList->AddRectFilled(start, max, Colour(palette.accent, 0.05f));
		drawList->AddRect(start, max, Colour(palette.accent, 0.55f));
		CornerMarks(drawList, start, max, fontSize * 0.18f, Colour(palette.accent, 0.7f));

		const float rowY = start.y + (height - ImGui::GetFrameHeight()) * 0.5f;
		const float padding = fontSize * 0.7f;
		DrawPinIcon(drawList, ImVec2(start.x + padding + fontSize * 0.4f, start.y + height * 0.5f), fontSize, Colour(palette.pinned));

		// Buttons first so the text knows where to stop
		const auto  pin = _pins.pins.front();
		const auto  more = _pins.pins.size() - 1;
		const auto  unpin = a_provider.Translate("UI_Unpin");
		const auto  unpinAll = a_provider.Translate("UI_UnpinAll");
		const auto  goTo = a_provider.Translate("UI_Pin_GoTo");
		const bool  hasPage = !pin.pageID.empty();
		const float unpinWidth = ButtonWidth(unpin.c_str()) + fontSize;
		const float allWidth = more > 0 ? ButtonWidth(unpinAll.c_str()) + fontSize : 0.0f;
		const float goWidth = hasPage ? ButtonWidth(goTo.c_str()) + fontSize : 0.0f;
		const float buttonsX = max.x - padding - unpinWidth - (hasPage ? goWidth + style.ItemSpacing.x : 0.0f) - (more > 0 ? allWidth + style.ItemSpacing.x : 0.0f);
		ImGui::SetCursorScreenPos(ImVec2(buttonsX, rowY));
		if (hasPage) {
			if (ThemedButton((goTo + "##PinGoTo").c_str(), goWidth)) {
				OpenPage(pin.pageID);
				_scrollToForm = pin.formID;
			}
			ImGui::SameLine();
		}
		if (ThemedButton((unpin + "##PinClear").c_str(), unpinWidth)) {
			a_provider.ClearPin(pin.formID);
			_pins = a_provider.GetPins();
		}
		if (more > 0) {
			ImGui::SameLine();
			if (ThemedButton((unpinAll + "##PinClearAll").c_str(), allWidth)) {
				a_provider.ClearPins();
				_pins = a_provider.GetPins();
			}
		}

		const float textX = start.x + padding + fontSize * 1.2f;
		const float textY = start.y + (height - fontSize) * 0.5f;
		drawList->PushClipRect(start, ImVec2(buttonsX - style.ItemSpacing.x, max.y), true);
		const auto label = a_provider.Translate("UI_Pinned");
		drawList->AddText(ImVec2(textX, textY), Colour(palette.accent), label.c_str());
		float x = textX + ImGui::CalcTextSize(label.c_str()).x + fontSize * 0.4f;
		drawList->AddText(ImVec2(x, textY), Colour(palette.text), pin.name.c_str());
		x += ImGui::CalcTextSize(pin.name.c_str()).x;
		if (more > 0) {
			const auto count = "  " + a_provider.Translate("UI_MoreCount{" + std::to_string(more) + "}");
			drawList->AddText(ImVec2(x, textY), Colour(palette.pinned), count.c_str());
			x += ImGui::CalcTextSize(count.c_str()).x;
		}
		const auto hint = "  \xC2\xB7  " + a_provider.Translate("UI_Pin_Hint");
		drawList->AddText(ImVec2(x, textY), Colour(palette.textDim), hint.c_str());
		drawList->PopClipRect();

		ImGui::SetCursorScreenPos(ImVec2(start.x, max.y));
		ImGui::Dummy(ImVec2(width, style.ItemSpacing.y));
	}

	//---------------------------------------------------
	//-- Menu ( Description Panel ) ---------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Description Panel Height ) ---
	//---------------------------------------------------

	float Menu::DescriptionPanelHeight() const {

		const float lines = _descriptionDragLines >= 0.0f ? _descriptionDragLines : _descriptionFitLines >= 0.0f ? _descriptionFitLines : static_cast<float>(_settings ? _settings->descriptionLines : 5);
		const float open = (std::min(lines, _descriptionMaxLines) + 2.4f) * ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().WindowPadding.y * 2.0f;
		const float t = std::clamp(_descriptionOpen, 0.0f, 1.0f);
		return open * t * t * (3.0f - 2.0f * t);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Update Description Panel ) ---
	//---------------------------------------------------

	void Menu::UpdateDescriptionPanel(float a_available, float a_gap) {

		const float lineHeight = ImGui::GetTextLineHeightWithSpacing();
		const float frame = 2.4f * lineHeight + ImGui::GetStyle().WindowPadding.y * 2.0f;
		_descriptionMaxLines = std::clamp(std::floor((a_available - a_gap - frame) / lineHeight) - 6.0f, 1.0f, static_cast<float>(UISettings::maxDescriptionLines));

		// The setting changed, so it wins over Y's fit
		if (_settings && _settings->descriptionLines != _descriptionFitBase) {
			_descriptionFitLines = -1.0f;
		}

		const bool open = !(_settings && _settings->descriptionCollapsed);
		if (_descriptionOpen < 0.0f) {
			_descriptionOpen = open ? 1.0f : 0.0f;
		}
		const float step = ImGui::GetIO().DeltaTime * 5.0f;
		_descriptionOpen = open ? std::min(1.0f, _descriptionOpen + step) : std::max(0.0f, _descriptionOpen - step);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Panel Grip ) -----------
	//---------------------------------------------------

	// The grabber above the description and shout panels
	static void DrawPanelGrip(ImDrawList* a_drawList, float a_centreX, float a_edgeY, float a_width, bool a_hot) {

		const ImU32 colour = Colour(Themes::GetPalette().accent, a_hot ? 1.0f : 0.6f);
		const float half = a_width * 0.5f;
		const float offset = ImGui::GetFontSize() * 0.22f;
		const float thickness = std::max(1.5f, ImGui::GetFontSize() * 0.08f);
		if (SkyUI()) {
			const float knot = ImGui::GetFontSize() * 0.5f;
			Decor::DoubleRule(a_drawList, ImVec2(a_centreX - half, a_edgeY), ImVec2(a_centreX - knot * 1.3f, a_edgeY), colour);
			Decor::DoubleRule(a_drawList, ImVec2(a_centreX + knot * 1.3f, a_edgeY), ImVec2(a_centreX + half, a_edgeY), colour);
			Decor::Knot(a_drawList, ImVec2(a_centreX, a_edgeY), knot, colour);
			return;
		}
		Decor::FadedLine(a_drawList, ImVec2(a_centreX - half, a_edgeY - offset), ImVec2(a_centreX + half, a_edgeY - offset), colour, thickness);
		Decor::FadedLine(a_drawList, ImVec2(a_centreX - half * 0.6f, a_edgeY + offset), ImVec2(a_centreX + half * 0.6f, a_edgeY + offset), colour, thickness);
		Decor::Diamond(a_drawList, ImVec2(a_centreX, a_edgeY), ImGui::GetFontSize() * 0.3f, colour, true);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Description Panel ) ----
	//---------------------------------------------------

	// Quest pages pass the selected quest, the others show the hovered or pinned entry
	void Menu::DrawDescriptionPanel(IDataProvider& a_provider, float a_height, float a_gap, const Page* a_journal, const Entry* a_quest) {

		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		const auto& io = ImGui::GetIO();
		const auto* entry = a_journal ? a_quest : _hoveredEntry ? std::addressof(*_hoveredEntry) : (_pinnedEntry ? std::addressof(*_pinnedEntry) : nullptr);

		// Grabber, drag to resize, double-click to collapse
		const auto  start = ImGui::GetCursorScreenPos();
		const float gapTop = start.y - style.ItemSpacing.y;
		const float width = ImGui::GetContentRegionAvail().x;
		const float edgeY = gapTop + a_gap;
		const float centreX = start.x + width * 0.5f;
		const float gripWidth = ImGui::GetFontSize() * 4.0f;
		const float lineHeight = ImGui::GetTextLineHeightWithSpacing();

		// Collapsed, the grip sits on the bottom edge and mustn't reach past it
		ImGui::SetCursorScreenPos(ImVec2(centreX - gripWidth * 0.5f, gapTop));
		ImGui::InvisibleButton("##DescriptionGrip", ImVec2(gripWidth, a_gap + (a_height < 1.0f ? 0.0f : ImGui::GetFontSize() * 0.4f)));
		const bool hovered = ImGui::IsItemHovered();
		const bool active = ImGui::IsItemActive();
		if (hovered || active) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		}

		if (_settings) {
			auto& settings = *_settings;
			if (hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				settings.descriptionCollapsed = !settings.descriptionCollapsed;
				_descriptionFitLines = -1.0f;
				_settingsChanged = true;
			} else if (active && io.MouseDelta.y != 0.0f && !settings.descriptionCollapsed) {
				if (!_descriptionDragged) {
					_descriptionDragLines = std::min(_descriptionFitLines >= 0.0f ? _descriptionFitLines : static_cast<float>(settings.descriptionLines), _descriptionMaxLines);
					_descriptionFitLines = -1.0f;
					_descriptionDragged = true;
				}
				_descriptionDragLines = std::clamp(_descriptionDragLines - io.MouseDelta.y / lineHeight, 1.0f, _descriptionMaxLines);
			}
			if (!active && _descriptionDragged) {
				settings.descriptionLines = std::clamp(static_cast<std::int32_t>(std::lround(_descriptionDragLines)), 1, UISettings::maxDescriptionLines);
				_descriptionDragLines = -1.0f;
				_descriptionDragged = false;
				_settingsChanged = true;
			}
		}

		const auto drawGrip = [&](ImDrawList* a_drawList) { DrawPanelGrip(a_drawList, centreX, edgeY, gripWidth, hovered || active); };

		ImGui::SetCursorScreenPos(ImVec2(start.x, edgeY));
		if (a_height < 1.0f) {
			auto* drawList = ImGui::GetWindowDrawList();
			Decor::FadedLine(drawList, ImVec2(start.x, edgeY), ImVec2(start.x + width, edgeY), Colour(palette.separator, 0.6f));
			drawGrip(drawList);
			ImGui::Dummy(ImVec2(width, 0.0f));
			return;
		}

		BeginFramedPanel("##Description", ImVec2(0.0f, a_height), true);
		ScrollWithRightStick();

		auto* gripList = ImGui::GetWindowDrawList();
		gripList->PushClipRect(ImVec2(start.x, gapTop), ImVec2(start.x + width, edgeY + a_height), false);
		drawGrip(gripList);
		gripList->PopClipRect();

		if (!entry) {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(a_journal ? "UI_QuestSelect" : "UI_DescriptionEmpty").c_str());
		} else if (a_journal) {
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", entry->name.c_str());
			ImGui::PopFont();
			ImGui::PushTextWrapPos(0.0f);
			DrawQuestDetail(a_provider, *a_journal, *entry);
			ImGui::PopTextWrapPos();
		} else {
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", entry->name.c_str());
			ImGui::PopFont();

			if (entry->showState) {
				auto status = a_provider.Translate(entry->status.empty() ? StateKey(entry->state) : std::string_view(entry->status));
				if (!entry->detail.empty()) {
					status += "  \xC2\xB7  " + entry->detail;
				}
				ImGui::TextColored(entry->IsComplete() ? palette.found : palette.textDim, "%s", status.c_str());
			}
			ImGui::Spacing();

			// Skip the MCM's ~~~ Name ~~~ banner
			std::string_view text = entry->description;
			if (text.starts_with("~~~")) {
				const auto lineEnd = text.find('\n');
				text = lineEnd == std::string_view::npos ? std::string_view{} : text.substr(lineEnd + 1);
				while (!text.empty() && text.front() == ' ') {
					text.remove_prefix(1);
				}
			}

			ImGui::PushTextWrapPos(0.0f);

			const bool shoutWords = (!entry->words.empty() && entry->placesKnown) || entry->IsTrainer() || entry->IsDragon();
			if (entry->IsDragon()) {
				// The story's in the panel
				DrawDragonInfo(a_provider, *entry, false);
				text = {};
			} else if (entry->IsTrainer()) {
				DrawTrainerInfo(a_provider, *entry, false);
				if (!text.empty()) {
					ImGui::Spacing();
				}
			} else if (shoutWords) {
				DrawShoutWords(a_provider, *entry, true);
				if (!text.empty()) {
					ImGui::Spacing();
				}
			}

			if (!text.empty()) {
				ImGui::TextUnformatted(text.data(), text.data() + text.size());
			} else if (!shoutWords) {
				ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_DescriptionNone").c_str());
			}
			ImGui::PopTextWrapPos();
		}

		// Y: as many lines as the text needs, as DescriptionPanelHeight counts them
		// Until held Y or the grip, the saved height stays as it is
		if (std::exchange(_fitDescription, false)) {
			const float content = ImGui::GetCursorPosY() - ImGui::GetCursorStartPos().y;
			_descriptionFitLines = std::clamp(std::ceil(content / lineHeight - 2.4f), 1.0f, _descriptionMaxLines);
			_descriptionFitBase = _settings ? _settings->descriptionLines : 0;
		}

		EndFramedPanel();
	}

	namespace ShoutLayout
	{
		static float Padding() { return ImGui::GetFontSize() * 0.8f; }
		static float CardGap() { return ImGui::GetFontSize() * 0.6f; }
		static float CardHeight() { return Padding() * 1.2f + ImGui::GetTextLineHeightWithSpacing() * 3.0f; }
		static float MinCard() { return ImGui::GetFontSize() * 12.0f; }

		static float NarrowCard() { return ImGui::GetFontSize() * 8.5f; }

		//---------------------------------------------------
		//-- Completionist UI ( Fit ) -----------------------
		//---------------------------------------------------

		static int Fit(float a_width, float a_cardWidth) {

			return std::max(1, static_cast<int>((a_width + CardGap()) / (a_cardWidth + CardGap())));
		}

		//---------------------------------------------------
		//-- Completionist UI ( Strip Content ) -------------
		//---------------------------------------------------

		static float StripContent() {

			const float fontSize = ImGui::GetFontSize();
			const float line = ImGui::GetTextLineHeight();
			ImGui::PushFont(Themes::TitleFont());
			const float words = ImGui::GetFontSize();
			ImGui::PopFont();
			return words + fontSize * 0.4f + line;
		}
	}

	namespace DragonLayout
	{
		//---------------------------------------------------
		//-- Completionist UI ( Info Content ) --------------
		//---------------------------------------------------

		// Dragon, fate, what the name means, where it's met
		static float InfoContent() {

			ImGui::PushFont(Themes::HeaderFont());
			const float name = ImGui::GetTextLineHeightWithSpacing();
			ImGui::PopFont();
			ImGui::PushFont(Themes::TitleFont());
			const float title = ImGui::GetTextLineHeightWithSpacing();
			ImGui::PopFont();
			return name + title + ImGui::GetTextLineHeightWithSpacing() * 2.0f;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Sizes ) ---------------------
		//---------------------------------------------------

		static bool  SideBySide(float a_width) { return a_width >= ImGui::GetFontSize() * 40.0f; }
		static float InfoWidth(float a_width) { return SideBySide(a_width) ? std::floor(std::clamp(a_width * 0.32f, ImGui::GetFontSize() * 16.0f, ImGui::GetFontSize() * 26.0f)) : a_width; }
		static float LoreWidth(float a_width) { return std::max(ImGui::GetFontSize() * 4.0f, (SideBySide(a_width) ? a_width - InfoWidth(a_width) - ShoutLayout::CardGap() : a_width) - ShoutLayout::Padding() * 2.0f); }

		static float LoreHeight(const std::string& a_lore, float a_width) {

			return a_lore.empty() ? 0.0f : ImGui::CalcTextSize(a_lore.c_str(), nullptr, false, LoreWidth(a_width)).y;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Height ) --------------------
		//---------------------------------------------------

		// Both boxes, side by side or stacked
		static float Height(float a_lore, float a_width) {

			const float padding = ShoutLayout::Padding();
			const float info = padding * 2.0f + InfoContent();
			const float lore = padding * 2.0f + std::max(a_lore, ImGui::GetTextLineHeightWithSpacing());
			return SideBySide(a_width) ? std::max(info, lore) : info + ShoutLayout::CardGap() + lore;
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Shout Words Height ) --------
	//---------------------------------------------------

	float Menu::ShoutWordsHeight() {

		return ShoutLayout::Padding() * 2.0f + ShoutLayout::StripContent() + ShoutLayout::CardGap() + ShoutLayout::CardHeight();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Shout Panel ) ----------
	//---------------------------------------------------

	void Menu::DrawShoutPanel(IDataProvider& a_provider, const Page& a_page, float a_height, float a_gap) {

		const auto& palette = Themes::GetPalette();

		// Hovered, else pinned, else the first
		const auto   hasPanel = [](const Entry& a_entry) { return a_entry.placesKnown || a_entry.IsTrainer() || a_entry.IsDragon(); };
		const Entry* entry = _hoveredEntry && hasPanel(*_hoveredEntry) ? std::addressof(*_hoveredEntry) : _pinnedEntry && hasPanel(*_pinnedEntry) ? std::addressof(*_pinnedEntry) : nullptr;
		for (const auto& section : a_page.sections) {
			for (const auto& candidate : section.entries) {
				if (!entry && hasPanel(candidate) && PassesFilter(candidate)) {
					entry = std::addressof(candidate);
				}
			}
		}

		// Grabber in the gap, double-click folds it like the description
		const auto  start = ImGui::GetCursorScreenPos();
		const float gapTop = start.y - ImGui::GetStyle().ItemSpacing.y;
		const float centreX = start.x + ImGui::GetContentRegionAvail().x * 0.5f;
		const float gripWidth = ImGui::GetFontSize() * 4.0f;
		ImGui::SetCursorScreenPos(ImVec2(centreX - gripWidth * 0.5f, gapTop));
		ImGui::InvisibleButton("##ShoutGrip", ImVec2(gripWidth, a_gap + ImGui::GetFontSize() * 0.4f));
		const bool gripHovered = ImGui::IsItemHovered();
		if (gripHovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && _settings) {
			_settings->descriptionCollapsed = true;
			_settingsChanged = true;
		}
		ImGui::SetCursorScreenPos(ImVec2(start.x, gapTop + a_gap));
		BeginFramedPanel("##Shout", ImVec2(0.0f, a_height));
		auto* gripList = ImGui::GetWindowDrawList();
		gripList->PushClipRect(ImVec2(start.x, gapTop), ImVec2(centreX * 2.0f - start.x, gapTop + a_gap + a_height), false);
		DrawPanelGrip(gripList, centreX, gapTop + a_gap, gripWidth, gripHovered);
		gripList->PopClipRect();
		ScrollWithRightStick();
		if (entry && entry->IsTrainer()) {
			DrawTrainerInfo(a_provider, *entry, true);
		} else if (entry && entry->IsDragon()) {
			DrawDragonInfo(a_provider, *entry, true);
		} else if (entry) {
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", entry->name.c_str());
			ImGui::PopFont();
			DrawShoutWords(a_provider, *entry, false);
		} else {
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_NothingMatches").c_str());
		}
		EndFramedPanel();
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Shout Words ) ----------
	//---------------------------------------------------

	void Menu::DrawShoutWords(IDataProvider& a_provider, const Entry& a_entry, bool a_sideBySide) {

		const auto& palette = Themes::GetPalette();
		const float fontSize = ImGui::GetFontSize();
		const float width = ImGui::GetContentRegionAvail().x;
		auto*       drawList = ImGui::GetWindowDrawList();
		const auto  wordCount = std::min<std::size_t>(a_entry.words.size(), 8);

		const auto panel = [&](ImVec2 a_min, ImVec2 a_max, bool a_lit) {
			drawList->AddRectFilled(a_min, a_max, Colour(palette.accent, 0.04f));
			drawList->AddRect(a_min, a_max, Colour(a_lit ? palette.accent : palette.separator, a_lit ? 0.55f : 0.7f));
			const float corner = fontSize * 0.18f;
			CornerMarks(drawList, a_min, a_max, corner, Colour(palette.accent, 0.6f));
		};

		std::size_t learnt = 0;
		for (std::size_t i = 0; i < wordCount; ++i) {
			learnt += a_entry.words[i].done ? 1 : 0;
		}

		const float padding = ShoutLayout::Padding();
		const float cardGap = ShoutLayout::CardGap();
		const float line = ImGui::GetTextLineHeightWithSpacing();
		const float cardHeight = ShoutLayout::CardHeight();
		const bool  sideBySide = a_sideBySide && width >= fontSize * 40.0f;
		const float stripWidth = sideBySide ? std::floor(width * 0.34f) : width;
		const auto  origin = ImGui::GetCursorScreenPos();

		//---------------------------------------------------
		// Word strip
		//---------------------------------------------------

		const auto measure = [&](float a_gap) {
			float total = 0.0f;
			for (std::size_t i = 0; i < wordCount; ++i) {
				total += ImGui::CalcTextSize(a_entry.words[i].text.c_str()).x + (i > 0 ? a_gap : 0.0f);
			}
			return total;
		};
		ImGui::PushFont(Themes::TitleFont());
		float gap = fontSize * 3.2f;
		bool  large = measure(gap) <= stripWidth - padding * 2.0f;
		ImGui::PopFont();
		if (!large) {
			gap = fontSize * 2.0f;
		}
		ImGui::PushFont(large ? Themes::TitleFont() : Themes::HeaderFont());
		const float wordHeight = ImGui::GetFontSize();
		const float wordsWidth = measure(gap);

		const auto  countText = a_provider.Translate("UI_ShoutWord_Learnt{" + std::to_string(learnt) + "}{" + std::to_string(wordCount) + "}");
		const float contentHeight = wordHeight + fontSize * 0.4f + ImGui::GetTextLineHeight();
		const float stripHeight = std::max(padding * 2.0f + contentHeight, sideBySide ? cardHeight : 0.0f);
		const auto  stripMin = origin;
		const auto  stripMax = ImVec2(stripMin.x + stripWidth, stripMin.y + stripHeight);
		panel(stripMin, stripMax, false);

		float       x = stripMin.x + (stripWidth - wordsWidth) * 0.5f;
		const float wordY = stripMin.y + (stripHeight - contentHeight) * 0.5f;
		for (std::size_t i = 0; i < wordCount; ++i) {
			const auto& word = a_entry.words[i];
			if (i > 0) {
				const bool filled = a_entry.words[i - 1].done;
				Decor::Diamond(drawList, ImVec2(x + gap * 0.5f, wordY + wordHeight * 0.55f), fontSize * 0.3f, Colour(filled ? palette.found : palette.separator), filled);
				x += gap;
			}
			drawList->AddText(ImVec2(x, wordY), Colour(word.done ? palette.found : palette.text), word.text.c_str());
			x += ImGui::CalcTextSize(word.text.c_str()).x;
		}
		ImGui::PopFont();
		const auto countSize = ImGui::CalcTextSize(countText.c_str());
		drawList->AddText(ImVec2(stripMin.x + (stripWidth - countSize.x) * 0.5f, wordY + wordHeight + fontSize * 0.4f), Colour(learnt == wordCount ? palette.found : palette.textDim), countText.c_str());

		//---------------------------------------------------
		// Place cards
		//---------------------------------------------------

		const auto  cardsOrigin = sideBySide ? ImVec2(stripMax.x + cardGap, origin.y) : ImVec2(origin.x, stripMax.y + cardGap);
		const float cardsWidth = sideBySide ? width - stripWidth - cardGap : width;
		if (a_entry.places.empty()) {
			const auto nowhere = a_provider.Translate("UI_ShoutWord_Nowhere");
			drawList->AddText(ImVec2(cardsOrigin.x + (sideBySide ? padding : 0.0f), cardsOrigin.y + (sideBySide ? (stripHeight - fontSize) * 0.5f : 0.0f)), Colour(palette.textDim), nowhere.c_str());
			ImGui::Dummy(ImVec2(width, sideBySide ? stripHeight : stripHeight + cardGap + line));
			return;
		}

		// Max eight cards, quests dropped first when walls teach it
		constexpr int                  maxCards = 8;
		std::vector<const EntryPlace*> shown;
		for (const auto& place : a_entry.places) {
			shown.push_back(std::addressof(place));
		}
		const int  fitNormal = std::min(maxCards, ShoutLayout::Fit(cardsWidth, ShoutLayout::MinCard()));
		const int  capacity = std::min(maxCards, ShoutLayout::Fit(cardsWidth, ShoutLayout::NarrowCard()));
		const bool hasWalls = std::ranges::any_of(shown, [](const EntryPlace* a_place) { return !a_place->quest; });
		if (static_cast<int>(shown.size()) > capacity && hasWalls) {
			std::erase_if(shown, [](const EntryPlace* a_place) { return a_place->quest; });
		}
		const int count = static_cast<int>(shown.size());
		const int columns = count <= fitNormal ? count : std::min(count, capacity);
		if (count > columns) {
			const auto order = [&](const EntryPlace* a_place) {
				return std::pair{ a_place->discovered, std::hash<std::string>{}(a_entry.name + '\x1f' + a_place->name) };
			};
			std::vector<const EntryPlace*> pick = shown;
			std::ranges::sort(pick, {}, order);
			pick.resize(static_cast<std::size_t>(columns));
			std::erase_if(shown, [&](const EntryPlace* a_place) { return std::ranges::find(pick, a_place) == pick.end(); });
		}
		const int cards = static_cast<int>(shown.size());

		const float cardWidth = (cardsWidth - cardGap * static_cast<float>(columns - 1)) / static_cast<float>(columns);
		const auto  allWords = static_cast<std::uint8_t>((1u << wordCount) - 1u);
		const auto  quest = a_provider.Translate("UI_ShoutWord_Quest");
		const auto  all = a_provider.Translate("UI_ShoutWord_AllWords");
		const auto  background = a_provider.Translate("UI_ShoutWord_Background");
		const auto  nameOf = [&](const EntryPlace& a_place) { return a_place.quest && a_place.name.empty() ? background : a_place.name; };
		const auto  cardAt = [&](int a_column) {
			const auto min = ImVec2(cardsOrigin.x + (cardWidth + cardGap) * static_cast<float>(a_column), cardsOrigin.y);
			return std::pair{ min, ImVec2(min.x + cardWidth, min.y + cardHeight) };
		};

		for (int i = 0; i < cards; ++i) {
			const auto& place = *shown[static_cast<std::size_t>(i)];
			const auto [min, max] = cardAt(i);

			// Lit when it teaches a word not learnt yet
			bool teachesNew = false;
			for (std::size_t w = 0; w < wordCount; ++w) {
				teachesNew |= ((place.words >> w) & 1u) && !a_entry.words[w].done;
			}
			panel(min, max, teachesNew);

			drawList->PushClipRect(min, ImVec2(max.x - padding * 0.5f, max.y), true);
			const float textX = min.x + padding;
			float       y = min.y + padding * 0.7f;
			drawList->AddText(ImVec2(textX, y), Colour(palette.textDim), (place.quest ? quest : place.region).c_str());
			y += line;
			drawList->AddText(ImVec2(textX, y), Colour(palette.text), nameOf(place).c_str());
			y += line;

			const float pip = fontSize * 0.22f;
			float       pipX = textX + pip;
			for (std::size_t w = 0; w < wordCount; ++w) {
				const bool taught = (place.words >> w) & 1u;
				Decor::Diamond(drawList, ImVec2(pipX, y + fontSize * 0.5f), pip, Colour(taught ? palette.accent : palette.separator), taught);
				pipX += pip * 3.2f;
			}
			std::string which;
			if ((place.words & allWords) == allWords) {
				which = all;
			} else {
				for (std::size_t w = 0; w < wordCount; ++w) {
					if ((place.words >> w) & 1u) {
						which += (which.empty() ? "" : ", ") + a_entry.words[w].text;
					}
				}
			}
			drawList->AddText(ImVec2(pipX + pip, y), Colour(palette.textDim), which.c_str());
			drawList->PopClipRect();
		}

		ImGui::Dummy(ImVec2(width, sideBySide ? std::max(stripHeight, cardHeight) : stripHeight + cardGap + cardHeight));
	}

	namespace TrainerLayout
	{
		//---------------------------------------------------
		//-- Completionist UI ( Info Content ) --------------
		//---------------------------------------------------

		// Trainer, skill, the skill bar, home, away, then what's left to learn
		static float InfoContent() {

			ImGui::PushFont(Themes::HeaderFont());
			const float name = ImGui::GetTextLineHeightWithSpacing();
			ImGui::PopFont();
			ImGui::PushFont(Themes::TitleFont());
			const float title = ImGui::GetTextLineHeightWithSpacing();
			ImGui::PopFont();
			return name + title + ImGui::GetTextLineHeightWithSpacing() * 4.0f;
		}

		//---------------------------------------------------
		//-- Completionist UI ( Sizes ) ---------------------
		//---------------------------------------------------

		static float Radius() { return ImGui::GetFontSize() * 0.5f; }
		static float Slot() { return Radius() * 2.3f + ImGui::GetTextLineHeight() + ImGui::GetFontSize() * 0.2f; }	// one perk and its label
		static float ColumnGap() { return ImGui::GetFontSize() * 12.0f; }	// at most, so short trees keep short lines
		static float TreeHeight(std::int32_t a_rows) { return Slot() * static_cast<float>(std::max(1, a_rows)) + ShoutLayout::Padding(); }
	}

	//---------------------------------------------------
	//-- Completionist UI ( Perk Tree Layout ) ----------
	//---------------------------------------------------

	// A column per step from the first perk, each perk near the one it comes from; the game's
	// own grid is drawn for its tall Skills menu and crowds when laid on its side. Past three
	// perks a column zigzags, so the tree spreads across rather than down
	PerkTreeLayout LayoutPerkTree(const std::vector<TrainerPerk>& a_perks) {

		PerkTreeLayout layout;
		const auto     count = a_perks.size();
		layout.column.assign(count, 0);
		layout.row.assign(count, 0);
		if (count == 0) {
			return layout;
		}

		// Longest path from a root, so a perk sits after everything it needs
		for (std::size_t pass = 0; pass < count; ++pass) {
			bool moved = false;
			for (std::size_t i = 0; i < count; ++i) {
				for (const auto child : a_perks[i].children) {
					if (layout.column[child] < layout.column[i] + 1) {
						layout.column[child] = layout.column[i] + 1;
						moved = true;
					}
				}
			}
			if (!moved) {
				break;
			}
		}
		const auto columns = *std::ranges::max_element(layout.column) + 1;
		layout.columns = columns;

		// Each column ordered by where its parents sit, the game's left to right breaking ties
		std::vector<std::vector<std::int32_t>> parents(count);
		for (std::size_t i = 0; i < count; ++i) {
			for (const auto child : a_perks[i].children) {
				parents[child].push_back(static_cast<std::int32_t>(i));
			}
		}
		constexpr std::int32_t maxRows = 3;
		layout.x.assign(count, 0.0f);
		layout.labelUnits.assign(count, 1.0f);
		std::vector<float> place(count, 0.0f);
		float              x = 0.0f;
		for (std::int32_t column = 0; column < columns; ++column) {
			std::vector<std::int32_t> members;
			for (std::size_t i = 0; i < count; ++i) {
				if (layout.column[i] == column) {
					members.push_back(static_cast<std::int32_t>(i));
				}
			}
			const auto key = [&](std::int32_t a_index) {
				float sum = 0.0f;
				for (const auto parent : parents[a_index]) {
					sum += place[parent];
				}
				return parents[a_index].empty() ? 0.0f : sum / static_cast<float>(parents[a_index].size());
			};
			std::ranges::sort(members, [&](std::int32_t a_left, std::int32_t a_right) {
				return std::pair(key(a_left), a_perks[a_left].lane) < std::pair(key(a_right), a_perks[a_right].lane);
			});
			const auto  size = static_cast<std::int32_t>(members.size());
			const auto  split = std::max(1, (size + maxRows - 1) / maxRows);
			const auto  span = (size + split - 1) / split;
			const float centre = (static_cast<float>(span) - 1.0f) * 0.5f;
			for (std::int32_t i = 0; i < size; ++i) {
				const auto lane = i % split;
				const auto index = static_cast<std::size_t>(members[i]);
				layout.row[index] = i / split;
				place[index] = static_cast<float>(i / split) + static_cast<float>(lane) / static_cast<float>(split) - centre;
				layout.x[index] = x + 0.5f * static_cast<float>(lane);
				layout.labelUnits[index] = split > 1 ? 0.5f : 1.0f;
			}
			layout.rows = std::max(layout.rows, span);
			x += 1.0f + 0.5f * static_cast<float>(split - 1);
		}
		layout.width = std::max(0.0f, x - 1.0f);
		layout.place = std::move(place);
		return layout;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Perk Tree Rows ) ------------
	//---------------------------------------------------

	std::int32_t PerkTreeRows(const std::vector<TrainerPerk>& a_perks) {

		return std::max(1, LayoutPerkTree(a_perks).rows);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Trainer Info Height ) -------
	//---------------------------------------------------

	float Menu::TrainerInfoHeight() const {

		return std::max(ShoutLayout::Padding() * 2.0f + TrainerLayout::InfoContent(), TrainerLayout::TreeHeight(_trainerRows));
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Trainer Info ) ---------
	//---------------------------------------------------

	// Who and where on the left, the skill's perk tree in the theme's own style on the right;
	// stacked when the window is narrow. The description panel already shows the name
	void Menu::DrawTrainerInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName) {

		const auto& palette = Themes::GetPalette();
		const auto& info = a_entry.trainer;
		const auto& perks = info.perks;
		const float fontSize = ImGui::GetFontSize();
		const float width = ImGui::GetContentRegionAvail().x;
		auto*       drawList = ImGui::GetWindowDrawList();

		const auto panel = [&](ImVec2 a_min, ImVec2 a_max) {
			drawList->AddRectFilled(a_min, a_max, Colour(palette.accent, 0.04f));
			drawList->AddRect(a_min, a_max, Colour(palette.separator, 0.7f));
			CornerMarks(drawList, a_min, a_max, fontSize * 0.18f, Colour(palette.accent, 0.6f));
		};

		const float padding = ShoutLayout::Padding();
		const float gap = ShoutLayout::CardGap();
		const float line = ImGui::GetTextLineHeightWithSpacing();
		const bool  sideBySide = width >= fontSize * 40.0f;
		const float infoWidth = sideBySide ? std::floor(std::clamp(width * 0.28f, fontSize * 15.0f, fontSize * 24.0f)) : width;
		const auto  layout = LayoutPerkTree(perks);
		const auto  rows = std::max(layout.rows, a_withName ? _trainerRows : 1);
		const float infoHeight = padding * 2.0f + TrainerLayout::InfoContent();
		const float boxHeight = sideBySide ? std::max(infoHeight, TrainerLayout::TreeHeight(rows)) : infoHeight;
		const auto  origin = ImGui::GetCursorScreenPos();

		//---------------------------------------------------
		// Trainer and skill
		//---------------------------------------------------

		const auto infoMin = origin;
		const auto infoMax = ImVec2(origin.x + infoWidth, origin.y + boxHeight);
		panel(infoMin, infoMax);
		drawList->PushClipRect(infoMin, ImVec2(infoMax.x - padding * 0.5f, infoMax.y), true);

		const bool  outgrown = info.playerLevel >= info.maxLevel;
		const float textX = infoMin.x + padding;
		const float textWidth = infoWidth - padding * 2.0f;
		float       y = infoMin.y + (boxHeight - TrainerLayout::InfoContent()) * 0.5f;

		ImGui::PushFont(Themes::HeaderFont());
		const float nameLine = ImGui::GetTextLineHeightWithSpacing();
		if (a_withName) {
			drawList->AddText(ImVec2(textX, y), Colour(palette.heading), FitText(a_entry.name, textWidth).c_str());
		}
		ImGui::PopFont();
		y += nameLine;
		ImGui::PushFont(Themes::TitleFont());
		const float titleLine = ImGui::GetTextLineHeightWithSpacing();
		drawList->AddText(ImVec2(textX, y), Colour(a_entry.IsComplete() ? palette.found : palette.text), FitText(info.skillName, textWidth).c_str());
		ImGui::PopFont();
		y += titleLine;

		// Your level, the bar, and what this trainer can take you to
		const auto  levels = std::to_string(info.playerLevel) + " / " + std::to_string(info.maxLevel);
		const auto  levelsSize = ImGui::CalcTextSize(levels.c_str());
		const float barWidth = std::max(fontSize * 3.0f, textWidth - levelsSize.x - fontSize * 0.8f);
		const float barY = y + ImGui::GetTextLineHeight() * 0.5f;
		const float barHeight = std::max(2.0f, std::floor(fontSize * 0.18f));
		const auto  at = [&](std::int32_t a_level) { return textX + barWidth * std::clamp(static_cast<float>(a_level) / 100.0f, 0.0f, 1.0f); };
		drawList->AddRectFilled(ImVec2(textX, barY - barHeight * 0.5f), ImVec2(textX + barWidth, barY + barHeight * 0.5f), Colour(palette.barBackground));
		drawList->AddRectFilled(ImVec2(textX, barY - barHeight * 0.5f), ImVec2(at(info.playerLevel), barY + barHeight * 0.5f), Colour(outgrown ? palette.found : palette.bar));
		for (const auto tick : { 50, 75 }) {
			drawList->AddRectFilled(ImVec2(at(tick), barY - fontSize * 0.3f), ImVec2(at(tick) + 1.0f, barY + fontSize * 0.3f), Colour(palette.separator));
		}
		Decor::Diamond(drawList, ImVec2(at(info.maxLevel), barY), fontSize * 0.28f, Colour(palette.accent), true);
		drawList->AddText(ImVec2(textX + textWidth - levelsSize.x, y), Colour(palette.textDim), levels.c_str());
		y += line;

		// Home, where they are when away, what's left to learn and the perks taken
		std::int32_t taken = 0;
		for (const auto& perk : perks) {
			taken += perk.rank > 0 ? 1 : 0;
		}
		const auto dimLine = [&](const std::string& a_text) {
			drawList->AddText(ImVec2(textX, y), Colour(palette.textDim), FitText(a_text, textWidth).c_str());
			y += line;
		};
		if (!info.location.empty()) {
			dimLine(a_provider.Translate("UI_Trainers_Home{" + info.location + "}"));
		}
		if (!info.now.empty()) {
			dimLine(a_provider.Translate("UI_Trainers_Now{" + info.now + "}"));
		}
		auto learn = a_provider.Translate(outgrown ? std::string("UI_Trainers_Outgrown") : "UI_Trainers_CanTeach{" + std::to_string(info.maxLevel - info.playerLevel) + "}");
		if (!perks.empty()) {
			learn += "  \xC2\xB7  " + a_provider.Translate("UI_Trainers_Perks{" + std::to_string(taken) + "}{" + std::to_string(perks.size()) + "}");
		}
		dimLine(learn);
		drawList->PopClipRect();

		//---------------------------------------------------
		// Perk tree
		//---------------------------------------------------

		const float treeHeight = sideBySide ? boxHeight : TrainerLayout::TreeHeight(rows);
		const auto  treeMin = sideBySide ? ImVec2(infoMax.x + gap, origin.y) : ImVec2(origin.x, infoMax.y + gap);
		const auto  treeMax = ImVec2(origin.x + width, treeMin.y + treeHeight);
		panel(treeMin, treeMax);
		ImGui::Dummy(ImVec2(width, sideBySide ? boxHeight : boxHeight + gap + treeHeight));

		if (perks.empty()) {
			const auto none = a_provider.Translate("UI_Trainers_NoPerks");
			const auto size = ImGui::CalcTextSize(none.c_str());
			drawList->AddText(ImVec2((treeMin.x + treeMax.x - size.x) * 0.5f, (treeMin.y + treeMax.y - size.y) * 0.5f), Colour(palette.textDim), none.c_str());
			return;
		}

		const float labelSize = ImGui::GetTextLineHeight();
		const float radius = TrainerLayout::Radius();
		const float slot = TrainerLayout::Slot();
		const float edge = padding * 0.6f;

		// Next once a perk it leads from is taken, or it starts the tree
		std::vector<bool> hasParent(perks.size(), false);
		std::vector<bool> parentTaken(perks.size(), false);
		for (std::size_t i = 0; i < perks.size(); ++i) {
			for (const auto child : perks[i].children) {
				hasParent[child] = true;
				parentTaken[child] = parentTaken[child] || perks[i].rank > 0;
			}
		}
		const auto stateOf = [&](std::size_t a_index) {
			return perks[a_index].rank > 0 ? PerkState::kTaken : !hasParent[a_index] || parentTaken[a_index] ? PerkState::kNext : PerkState::kLocked;
		};

		std::vector<std::string> labels;
		labels.reserve(perks.size());
		for (std::size_t i = 0; i < perks.size(); ++i) {
			auto label = perks[i].name;
			if (perks[i].ranks > 1) {
				label += " " + std::to_string(perks[i].rank) + "/" + std::to_string(perks[i].ranks);
			}
			if (stateOf(i) != PerkState::kTaken && perks[i].level > 0) {
				label += "  \xC2\xB7  " + std::to_string(perks[i].level);
			}
			labels.push_back(std::move(label));
		}

		// Columns evenly spaced up to a cap and centred, the first and last columns' labels kept inside
		float leftRoom = radius * 1.5f;
		float rightRoom = radius * 1.5f;
		for (std::size_t i = 0; i < perks.size(); ++i) {
			const float half = ImGui::CalcTextSize(labels[i].c_str()).x * 0.5f;
			if (layout.x[i] <= 0.0f) {
				leftRoom = std::max(leftRoom, half);
			}
			if (layout.x[i] >= layout.width) {
				rightRoom = std::max(rightRoom, half);
			}
		}
		const float inner = (treeMax.x - treeMin.x) - edge * 2.0f;
		leftRoom = std::min(leftRoom, inner * 0.25f);
		rightRoom = std::min(rightRoom, inner * 0.25f);
		const float available = inner - leftRoom - rightRoom;
		const float unit = layout.width > 0.0f ? std::min(TrainerLayout::ColumnGap(), available / layout.width) : 0.0f;
		const float treeWidth = unit * layout.width;
		const float firstX = treeMin.x + edge + leftRoom + (available - treeWidth) * 0.5f;
		const float middleY = (treeMin.y + treeMax.y) * 0.5f - labelSize * 0.5f;

		std::vector<ImVec2> spots;
		spots.reserve(perks.size());
		for (std::size_t i = 0; i < perks.size(); ++i) {
			spots.emplace_back(firstX + unit * layout.x[i], middleY + layout.place[i] * slot);
			const float room = layout.width > 0.0f ? std::max(fontSize * 4.0f, unit * layout.labelUnits[i] * (layout.labelUnits[i] < 1.0f ? 2.0f : 1.0f) - radius * 2.0f) : inner;
			labels[i] = FitText(labels[i], room);
		}

		drawList->PushClipRect(treeMin, treeMax, true);
		for (const bool lit : { false, true }) {
			for (std::size_t i = 0; i < perks.size(); ++i) {
				for (const auto child : perks[i].children) {
					if ((perks[i].rank > 0 && perks[child].rank > 0) == lit) {
						Decor::PerkLink(drawList, spots[i], spots[child], radius, lit);
					}
				}
			}
		}

		const auto  mouse = ImGui::GetMousePos();
		const bool  hoverable = ImGui::IsWindowHovered();
		std::size_t hovered = perks.size();
		for (std::size_t i = 0; i < perks.size(); ++i) {
			const auto  state = stateOf(i);
			Decor::PerkNode(drawList, spots[i], radius, state);

			const auto& label = labels[i];
			const auto  size = ImGui::CalcTextSize(label.c_str());
			const float labelX = std::clamp(spots[i].x - size.x * 0.5f, treeMin.x + edge, treeMax.x - edge - size.x);
			const float labelY = spots[i].y + radius * 1.3f;
			const auto  colour = state == PerkState::kTaken ? Colour(palette.text) : state == PerkState::kNext ? Decor::PerkColour(PerkState::kNext) : Colour(palette.textDim);

			// Links pass behind the labels, not through them
			const float pad = fontSize * 0.2f;
			drawList->AddRectFilled(ImVec2(labelX - pad, labelY), ImVec2(labelX + size.x + pad, labelY + size.y), Colour(palette.background, 0.85f), pad);
			drawList->AddText(ImVec2(labelX, labelY), colour, label.c_str());

			const float dx = mouse.x - spots[i].x;
			const float dy = mouse.y - spots[i].y;
			if (hoverable && dx * dx + dy * dy <= radius * radius * 2.2f) {
				hovered = i;
			}
		}
		drawList->PopClipRect();

		if (hovered < perks.size()) {
			const auto& perk = perks[hovered];
			ImGui::BeginTooltip();
			ImGui::TextColored(palette.heading, "%s", perk.name.c_str());
			if (perk.ranks > 1) {
				ImGui::TextUnformatted(a_provider.Translate("UI_Trainers_PerkRank{" + std::to_string(perk.rank) + "}{" + std::to_string(perk.ranks) + "}").c_str());
			}
			if (perk.level > 0) {
				ImGui::TextColored(info.playerLevel >= perk.level ? palette.found : palette.textDim, "%s", a_provider.Translate("UI_Trainers_PerkNeeds{" + std::to_string(perk.level) + "}").c_str());
			}
			ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(perk.rank > 0 ? "UI_Trainers_PerkTaken" : "UI_Trainers_PerkNotTaken").c_str());
			ImGui::EndTooltip();
		}
	}

	//---------------------------------------------------
	//-- Completionist UI ( Dragon Info Height ) --------
	//---------------------------------------------------

	float Menu::DragonInfoHeight(float a_lore, float a_width) {

		return DragonLayout::Height(a_lore, a_width);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Dragon Lore Height ) --------
	//---------------------------------------------------

	float Menu::DragonLoreHeight(const std::string& a_lore, float a_width) {

		return DragonLayout::LoreHeight(a_lore, a_width);
	}

	//---------------------------------------------------
	//-- Completionist UI ( Draw Dragon Info ) ----------
	//---------------------------------------------------

	// Who and what became of them on the left, their story on the right, as the trainers'
	// panel; stacked when the window is narrow. The description panel already shows the name
	void Menu::DrawDragonInfo(IDataProvider& a_provider, const Entry& a_entry, bool a_withName) {

		const auto& palette = Themes::GetPalette();
		const auto& info = a_entry.dragon;
		const float fontSize = ImGui::GetFontSize();
		const float width = ImGui::GetContentRegionAvail().x;
		auto*       drawList = ImGui::GetWindowDrawList();

		const auto panel = [&](ImVec2 a_min, ImVec2 a_max) {
			drawList->AddRectFilled(a_min, a_max, Colour(palette.accent, 0.04f));
			drawList->AddRect(a_min, a_max, Colour(palette.separator, 0.7f));
			CornerMarks(drawList, a_min, a_max, fontSize * 0.18f, Colour(palette.accent, 0.6f));
		};

		const float padding = ShoutLayout::Padding();
		const float gap = ShoutLayout::CardGap();
		const float line = ImGui::GetTextLineHeightWithSpacing();
		const bool  sideBySide = DragonLayout::SideBySide(width);
		const float infoWidth = DragonLayout::InfoWidth(width);
		const float loreWidth = DragonLayout::LoreWidth(width);
		const float lore = a_withName ? std::max(_dragonLore, DragonLayout::LoreHeight(info.lore, width)) : DragonLayout::LoreHeight(info.lore, width);
		const float infoHeight = padding * 2.0f + DragonLayout::InfoContent();
		const float loreHeight = padding * 2.0f + std::max(lore, line);
		const float boxHeight = sideBySide ? std::max(infoHeight, loreHeight) : infoHeight;
		const auto  origin = ImGui::GetCursorScreenPos();

		//---------------------------------------------------
		// Dragon and fate
		//---------------------------------------------------

		const auto infoMin = origin;
		const auto infoMax = ImVec2(origin.x + infoWidth, origin.y + boxHeight);
		panel(infoMin, infoMax);
		drawList->PushClipRect(infoMin, ImVec2(infoMax.x - padding * 0.5f, infoMax.y), true);

		const float textX = infoMin.x + padding;
		const float textWidth = infoWidth - padding * 2.0f;
		float       y = infoMin.y + (boxHeight - DragonLayout::InfoContent()) * 0.5f;

		ImGui::PushFont(Themes::HeaderFont());
		const float nameLine = ImGui::GetTextLineHeightWithSpacing();
		if (a_withName) {
			drawList->AddText(ImVec2(textX, y), Colour(palette.heading), FitText(a_entry.name, textWidth).c_str());
		}
		ImGui::PopFont();
		y += nameLine;
		ImGui::PushFont(Themes::TitleFont());
		const float titleLine = ImGui::GetTextLineHeightWithSpacing();
		drawList->AddText(ImVec2(textX, y), Colour(a_entry.IsComplete() ? palette.found : palette.text), FitText(info.fate, textWidth).c_str());
		ImGui::PopFont();
		y += titleLine;

		const auto dimLine = [&](const std::string& a_text) {
			drawList->AddText(ImVec2(textX, y), Colour(palette.textDim), FitText(a_text, textWidth).c_str());
			y += line;
		};
		if (!info.meaning.empty()) {
			dimLine(a_provider.Translate("UI_Dragons_Means{" + info.meaning + "}"));
		}
		if (!info.place.empty()) {
			dimLine(a_provider.Translate("UI_Dragons_Place{" + info.place + "}"));
		}
		drawList->PopClipRect();

		//---------------------------------------------------
		// Their story
		//---------------------------------------------------

		const float storyHeight = sideBySide ? boxHeight : loreHeight;
		const auto  storyMin = sideBySide ? ImVec2(infoMax.x + gap, origin.y) : ImVec2(origin.x, infoMax.y + gap);
		const auto  storyMax = ImVec2(origin.x + width, storyMin.y + storyHeight);
		panel(storyMin, storyMax);
		ImGui::Dummy(ImVec2(width, sideBySide ? boxHeight : boxHeight + gap + storyHeight));

		const auto& story = info.lore.empty() ? a_provider.Translate("UI_DescriptionNone") : info.lore;
		const float storyY = storyMin.y + (storyHeight - DragonLayout::LoreHeight(story, width)) * 0.5f;
		drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(storyMin.x + padding, std::max(storyMin.y + padding, storyY)), Colour(info.lore.empty() ? palette.textDim : palette.text), story.c_str(), nullptr, loreWidth);
	}

	//---------------------------------------------------
	//-- Menu ( Settings ) ------------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Completionist UI ( Draw Settings ) -------------
	//---------------------------------------------------

	void Menu::DrawSettings(IDataProvider& a_provider) {

		if (!_settings) {
			return;
		}

		auto&       settings = *_settings;
		const auto& palette = Themes::GetPalette();
		const auto& style = ImGui::GetStyle();
		bool        changed = false;
		_hoveredEntry.reset();

		// Applied on release
		if (_pendingScale < 0.0f) {
			_pendingScale = settings.scale;
			_pendingWidth = settings.widthPercent;
			_pendingHeight = settings.heightPercent;
		}

		const float gutter = ImGui::GetFontSize() * 4.0f;
		float       tableTop = 0.0f;
		float       dividerX = 0.0f;
		const auto  beginColumns = [&](const char* a_id) {
			tableTop = ImGui::GetCursorScreenPos().y;
			if (!ImGui::BeginTable(a_id, 3, ImGuiTableFlags_PadOuterX)) {
				return false;
			}
			ImGui::TableSetupColumn("##Left", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("##Gutter", ImGuiTableColumnFlags_WidthFixed, gutter);
			ImGui::TableSetupColumn("##Right", ImGuiTableColumnFlags_WidthStretch);
			return true;
		};
		const auto endColumns = [&]() {
			ImGui::EndTable();
			const float bottom = ImGui::GetCursorScreenPos().y - style.ItemSpacing.y;
			const float middle = (tableTop + bottom) * 0.5f;
			const ImU32 colour = Colour(palette.separator, 0.7f);
			const ImU32 clear = Colour(palette.separator, 0.0f);
			auto*       drawList = ImGui::GetWindowDrawList();
			if (SkyUI()) {
				drawList->AddRectFilled(ImVec2(dividerX, tableTop), ImVec2(dividerX + 1.0f, bottom), colour);
				return;
			}
			drawList->AddRectFilledMultiColor(ImVec2(dividerX, tableTop), ImVec2(dividerX + 1.0f, middle), clear, clear, colour, colour);
			drawList->AddRectFilledMultiColor(ImVec2(dividerX, middle), ImVec2(dividerX + 1.0f, bottom), colour, colour, clear, clear);
		};

		float columnX = 0.0f;
		float labelWidth = 0.0f;
		float controlWidth = 0.0f;
		const auto beginColumn = [&]() {
			ImGui::TableNextColumn();
			if (ImGui::TableGetColumnIndex() == 1) {
				dividerX = ImGui::GetCursorScreenPos().x + gutter * 0.5f;
				ImGui::TableNextColumn();
			}
			columnX = ImGui::GetCursorPosX();
			const float width = ImGui::GetContentRegionAvail().x;
			labelWidth = width * 0.46f;
			controlWidth = width - labelWidth - style.ItemSpacing.x;
		};
		const auto label = [&](const std::string& a_text) {
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(a_text.c_str());
			ImGui::SameLine();
			ImGui::SetCursorPosX(columnX + labelWidth);
		};
		const auto toggleSpot = [&]() {
			if (SkyUI()) {
				ImGui::SetCursorPosX(columnX + labelWidth + controlWidth - ImGui::GetTextLineHeight() - style.FramePadding.x);
			}
		};
		const auto describe = [&](const std::string& a_label, const std::string& a_info) {
			if (!a_info.empty() && (ImGui::IsItemHovered() || ImGui::IsItemFocused())) {
				Entry entry;
				entry.name = a_label;
				entry.description = a_info;
				entry.showState = false;
				_hoveredEntry = entry;
			}
		};
		const auto columnHeading = [&](const std::string& a_text) {
			// Empty headers keep their row, as in the MCM
			ImGui::Spacing();
			ImGui::PushFont(Themes::HeaderFont());
			ImGui::TextColored(palette.heading, "%s", a_text.empty() ? " " : a_text.c_str());
			ImGui::PopFont();
			if (SkyUI()) {
				if (a_text.empty()) {
					ImGui::Dummy(ImVec2(0.0f, 4.0f));
					return;
				}
				const auto  min = ImGui::GetItemRectMin();
				const auto  max = ImGui::GetItemRectMax();
				const float right = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
				const float start = max.x + ImGui::GetFontSize() * 0.4f;
				Decor::HeadingTail(ImGui::GetWindowDrawList(), ImVec2(start, (min.y + max.y) * 0.5f), ImVec2(right, (min.y + max.y) * 0.5f), Colour(palette.text, 0.85f), Colour(palette.separator));
				ImGui::Dummy(ImVec2(0.0f, 4.0f));
				return;
			}
			HeadingRule(6.0f);
		};
		const auto pageHeading = [](const std::string& a_text) { CentredHeading(a_text); };

		ImGui::PushFont(Themes::HeaderFont());
		ImGui::TextColored(palette.heading, "%s", a_provider.Translate("UI_Settings").c_str());
		ImGui::PopFont();
		ImGui::TextColored(palette.textDim, "%s", a_provider.Translate("UI_Setting_SavedHint").c_str());

		const float gap = ImGui::GetFontSize() * 0.75f;
		UpdateDescriptionPanel(ImGui::GetContentRegionAvail().y, gap);
		const float panelHeight = DescriptionPanelHeight();
		BeginFramedPanel("##SettingsBody", ImVec2(0.0f, -(panelHeight + gap)));

		//---------------------------------------------------
		// About
		//---------------------------------------------------

		if (!_about.thanks.empty()) {
			const auto centred = [&](const std::string& a_text, const ImVec4& a_colour) {
				const float width = ImGui::GetContentRegionAvail().x;
				const float textWidth = ImGui::CalcTextSize(a_text.c_str()).x;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max(0.0f, (width - textWidth) * 0.5f));
				ImGui::TextColored(a_colour, "%s", a_text.c_str());
			};

			ImGui::Spacing();
			centred(a_provider.Translate(_about.thanks), palette.text);
			std::string facts;
			for (const auto& fact : _about.facts) {
				if (!facts.empty()) {
					facts += "   \xC2\xB7   ";
				}
				facts += a_provider.Translate(fact);
			}
			centred(facts, palette.textDim);
		}

		//---------------------------------------------------
		// Tabs
		//---------------------------------------------------

		std::vector<TabItem> tabs{ { std::string(windowTab), a_provider.Translate("UI_SettingsSection_Window"), {} } };
		for (const auto& page : _settingsPages) {
			if (page.tab) {
				tabs.push_back({ page.id, a_provider.Translate(page.title), {} });
			}
		}
		if (std::ranges::none_of(tabs, [&](const TabItem& a_tab) { return a_tab.id == _settingsTab; })) {
			_settingsTab = windowTab;
		}
		if (TabRow(tabs, _settingsTab)) {
			_confirmKey.clear();
		}

		if (!settings.captureTarget.empty() && settings.captureResult != UISettings::captureNone) {
			for (auto& page : _settingsPages) {
				for (auto& column : page.columns) {
					for (auto& item : column) {
						if (page.id + '/' + item.id == settings.captureTarget) {
							item.number = settings.captureResult;
							a_provider.ApplySetting(page.id, item);
						}
					}
				}
			}
			settings.captureTarget.clear();
			settings.captureResult = UISettings::captureNone;
		}

		//---------------------------------------------------
		// Window settings (Completionist.ini)
		//---------------------------------------------------

		if (_settingsTab == windowTab) {
			pageHeading(a_provider.Translate("UI_SettingsSection_Window"));
		}
		if (_settingsTab == windowTab && beginColumns("##WindowSettings")) {
			beginColumn();
			columnHeading(a_provider.Translate("UI_SettingsSection_Appearance"));

			const auto current = std::ranges::find(themeList, settings.theme, &ThemeInfo::theme);
			label(a_provider.Translate("UI_Setting_Theme"));
			if (ThemedBeginCombo("##Theme", a_provider.Translate(current != std::end(themeList) ? current->key : themeList[0].key).c_str(), controlWidth)) {
				for (const auto& info : themeList) {
					if (ImGui::Selectable(a_provider.Translate(info.key).c_str(), info.theme == settings.theme)) {
						settings.theme = info.theme;
						changed = true;
					}
				}
				ImGui::EndCombo();
			}

			const auto releasedSlider = [&](const char* a_key, const char* a_hint, const char* a_id, float& a_pending, float& a_target, float a_min, float a_max, const char* a_format) {
				label(a_provider.Translate(a_key));
				bool released = false;
				ThemedSlider(a_id, ImGuiDataType_Float, &a_pending, &a_min, &a_max, a_format, controlWidth, &released);
				if (released) {
					a_target = a_pending;
					changed = true;
				}
				if (a_hint) {
					describe(a_provider.Translate(a_key), a_provider.Translate(a_hint));
				}
			};
			releasedSlider("UI_Setting_Scale", "UI_SettingHint_Scale", "##Scale", _pendingScale, settings.scale, 0.5f, 2.0f, "%.2f");
			releasedSlider("UI_Setting_Width", nullptr, "##Width", _pendingWidth, settings.widthPercent, 40.0f, 100.0f, "%.0f%%");
			releasedSlider("UI_Setting_Height", nullptr, "##Height", _pendingHeight, settings.heightPercent, 40.0f, 100.0f, "%.0f%%");

			auto            opacity = settings.opacity * 100.0f;
			constexpr float opacityMin = 30.0f;
			constexpr float opacityMax = 100.0f;
			label(a_provider.Translate("UI_Setting_Opacity"));
			if (ThemedSlider("##Opacity", ImGuiDataType_Float, &opacity, &opacityMin, &opacityMax, "%.0f%%", controlWidth)) {
				settings.opacity = opacity / 100.0f;
				changed = true;
			}

			const auto toggle = [&](const char* a_key, const char* a_hint, bool& a_value) {
				label(a_provider.Translate(a_key));
				toggleSpot();
				changed |= MarkerToggle(a_key, a_value);
				if (a_hint) {
					describe(a_provider.Translate(a_key), a_provider.Translate(a_hint));
				}
			};

			ImGui::Spacing();
			columnHeading(a_provider.Translate("UI_SettingsSection_Layout"));
			const std::string autoColumns = a_provider.Translate("UI_Setting_ColumnsAuto");
			label(a_provider.Translate("UI_Setting_Columns"));
			if (ThemedBeginCombo("##Columns", settings.columns > 0 ? std::to_string(settings.columns).c_str() : autoColumns.c_str(), controlWidth)) {
				for (int i = 0; i <= 4; ++i) {
					const auto text = i == 0 ? autoColumns : std::to_string(i);
					if (ImGui::Selectable(text.c_str(), i == settings.columns)) {
						settings.columns = i;
						changed = true;
					}
				}
				ImGui::EndCombo();
			}
			describe(a_provider.Translate("UI_Setting_Columns"), a_provider.Translate("UI_SettingHint_Columns"));

			constexpr int linesMin = 1;
			constexpr int linesMax = UISettings::maxDescriptionLines;
			label(a_provider.Translate("UI_Setting_DescriptionLines"));
			changed |= ThemedSlider("##DescriptionLines", ImGuiDataType_S32, &settings.descriptionLines, &linesMin, &linesMax, "%d", controlWidth);
			describe(a_provider.Translate("UI_Setting_DescriptionLines"), a_provider.Translate("UI_SettingHint_DescriptionLines"));
			toggle("UI_Setting_OpenOnLastPage", nullptr, settings.openOnLastPage);
			toggle("UI_Setting_HideCompleted", nullptr, settings.hideCompleted);

			beginColumn();
			columnHeading(a_provider.Translate("UI_SettingsSection_Controls"));

			const UISettings defaults{};
			const auto KeyNameFor = [](std::uint32_t a_scanCode) -> std::string {
				switch (a_scanCode) {
				case 199: return "HOME";
				case 207: return "END";
				default: return std::to_string(a_scanCode);
				}
			};
			const float      resetSide = ImGui::GetFrameHeight();
			const float      bindWidth = controlWidth - resetSide - style.ItemSpacing.x;
			const auto       binding = [&](const char* a_key, const char* a_hint, const std::string& a_text, bool& a_capturing, bool a_isDefault, const std::string& a_defaultText, const std::function<void()>& a_reset) {
				label(a_provider.Translate(a_key));
				if (ThemedButton((a_text + "##" + a_key).c_str(), bindWidth)) {
					a_capturing = true;
				}
				describe(a_provider.Translate(a_key), a_provider.Translate(a_hint));
				ImGui::SameLine();
				ImGui::PushID(a_key);
				const auto min = ImGui::GetCursorScreenPos();
				const bool clicked = ImGui::InvisibleButton("##Reset", ImVec2(resetSide, resetSide));
				NoteControl();
				const bool hovered = ImGui::IsItemHovered() || (ImGui::IsItemFocused() && ImGui::GetIO().NavVisible);
				if (hovered) {
					ImGui::SetTooltip("%s", a_provider.Translate("UI_Setting_ResetKey{" + a_defaultText + "}").c_str());
				}
				const auto colour = a_isDefault ? Colour(palette.textDim, 0.45f) : hovered ? Colour(palette.accent) : Colour(palette.text, 0.85f);
				DrawResetIcon(ImGui::GetWindowDrawList(), ImVec2(min.x + resetSide * 0.5f, min.y + resetSide * 0.5f), ImGui::GetFontSize(), colour);
				if (clicked && !a_isDefault) {
					a_reset();
					changed = true;
				}
				ImGui::PopID();
			};

			// Delete unbinds, Escape / B cancels
			const auto unbound = a_provider.Translate("UI_Setting_Unbound");
			binding("UI_Setting_OpenKey", "UI_SettingHint_OpenKey", settings.capturingOpenKey ? a_provider.Translate("UI_Setting_PressKeyUnbind") : settings.openKey ? settings.openKeyName : unbound, settings.capturingOpenKey,
				settings.openKey == defaults.openKey, KeyNameFor(defaults.openKey), [&]() { settings.openKey = defaults.openKey; });
			binding("UI_Setting_OpenPad", "UI_SettingHint_OpenPad", settings.capturingOpenPad ? a_provider.Translate("UI_Setting_PressButtons") : settings.openPadButton ? GamepadComboName(settings.openPadModifier, settings.openPadButton, settings.padStyle) : unbound, settings.capturingOpenPad,
				settings.openPadButton == defaults.openPadButton && settings.openPadModifier == defaults.openPadModifier, GamepadComboName(defaults.openPadModifier, defaults.openPadButton, settings.padStyle),
				[&]() { settings.openPadButton = defaults.openPadButton; settings.openPadModifier = defaults.openPadModifier; });
			binding("UI_Setting_CellKey", "UI_SettingHint_CellKey", settings.capturingCellKey ? a_provider.Translate("UI_Setting_PressKeyUnbind") : settings.cellKey ? settings.cellKeyName : unbound, settings.capturingCellKey,
				settings.cellKey == defaults.cellKey, KeyNameFor(defaults.cellKey), [&]() { settings.cellKey = defaults.cellKey; });
			binding("UI_Setting_CellPad", "UI_SettingHint_CellPad", settings.capturingCellPad ? a_provider.Translate("UI_Setting_PressButtons") : settings.cellPadButton ? GamepadComboName(settings.cellPadModifier, settings.cellPadButton, settings.padStyle) : unbound, settings.capturingCellPad,
				settings.cellPadButton == defaults.cellPadButton && settings.cellPadModifier == defaults.cellPadModifier, unbound,
				[&]() { settings.cellPadButton = defaults.cellPadButton; settings.cellPadModifier = defaults.cellPadModifier; });

			constexpr const char* padStyleKeys[] = { "UI_Setting_PadXbox", "UI_Setting_PadPlayStation", "UI_Setting_PadSwitch", "UI_Setting_PadSteamDeck" };
			label(a_provider.Translate("UI_Setting_PadStyle"));
			if (ThemedBeginCombo("##PadStyle", a_provider.Translate(padStyleKeys[static_cast<int>(settings.padStyle)]).c_str(), controlWidth)) {
				for (int i = 0; i < 4; ++i) {
					if (ImGui::Selectable(a_provider.Translate(padStyleKeys[i]).c_str(), i == static_cast<int>(settings.padStyle))) {
						settings.padStyle = static_cast<PadStyle>(i);
						changed = true;
					}
				}
				ImGui::EndCombo();
			}
			describe(a_provider.Translate("UI_Setting_PadStyle"), a_provider.Translate("UI_SettingHint_PadStyle"));

			ImGui::Spacing();
			columnHeading(a_provider.Translate("UI_SettingsSection_OnScreen"));
			toggle("UI_Setting_PrintScreen", "UI_SettingHint_PrintScreen", settings.printScreen);
			toggle("UI_Setting_Screenshot", "UI_SettingHint_Screenshot", settings.screenshotButton);
			toggle("UI_Setting_CellPanel", "UI_SettingHint_CellPanel", settings.cellPanel);

			ImGui::Spacing();
			ImGui::SetCursorPosX(columnX + labelWidth);
			if (ThemedButton(a_provider.Translate("UI_Setting_Reset").c_str(), controlWidth)) {
				const auto keyName = settings.openKeyName;
				const auto cellKeyName = settings.cellKeyName;
				settings = UISettings{};
				settings.openKeyName = keyName;
				settings.cellKeyName = cellKeyName;
				_pendingScale = settings.scale;
				_pendingWidth = settings.widthPercent;
				_pendingHeight = settings.heightPercent;
				changed = true;
			}

			endColumns();
		}

		//---------------------------------------------------
		// Settings pages
		//---------------------------------------------------

		for (auto& page : _settingsPages) {
			if (page.id != _settingsTab) {
				continue;
			}
			ImGui::PushID(page.id.c_str());
			pageHeading(a_provider.Translate(page.title));

			if (page.id == "notifications" && beginColumns("##Toasts")) {
				beginColumn();
				columnHeading(a_provider.Translate("UI_SettingsSection_Toasts"));
				label(a_provider.Translate("UI_Setting_Toasts"));
				toggleSpot();
				changed |= MarkerToggle("##Toasts", settings.toasts);
				describe(a_provider.Translate("UI_Setting_Toasts"), a_provider.Translate("UI_SettingHint_Toasts"));

				ImGui::BeginDisabled(!settings.toasts);
				const std::string orders[] = { a_provider.Translate("UI_Setting_ToastsNewestTop"), a_provider.Translate("UI_Setting_ToastsNewestBottom") };
				label(a_provider.Translate("UI_Setting_ToastsOrder"));
				if (ThemedBeginCombo("##ToastsOrder", orders[settings.toastsNewestFirst ? 0 : 1].c_str(), controlWidth)) {
					for (int i = 0; i < 2; ++i) {
						if (ImGui::Selectable(orders[i].c_str(), (i == 0) == settings.toastsNewestFirst)) {
							settings.toastsNewestFirst = i == 0;
							changed = true;
						}
					}
					ImGui::EndCombo();
				}
				describe(a_provider.Translate("UI_Setting_ToastsOrder"), a_provider.Translate("UI_SettingHint_ToastsOrder"));
				ImGui::EndDisabled();

				beginColumn();
				columnHeading({});
				ImGui::BeginDisabled(!settings.toasts);
				constexpr ImVec2      corners[] = { ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 1.0f) };
				constexpr const char* cornerKeys[] = { "UI_Setting_ToastsTopLeft", "UI_Setting_ToastsTopRight", "UI_Setting_ToastsBottomLeft", "UI_Setting_ToastsBottomRight" };
				int                   corner = -1;
				for (int i = 0; i < 4; ++i) {
					if (std::abs(settings.toastX - corners[i].x) < 0.001f && std::abs(settings.toastY - corners[i].y) < 0.001f) {
						corner = i;
					}
				}
				const auto position = corner >= 0 ? a_provider.Translate(cornerKeys[corner]) : a_provider.Translate("UI_Setting_ToastsCustom");
				label(a_provider.Translate("UI_Setting_ToastsPosition"));
				if (ThemedBeginCombo("##ToastsPosition", position.c_str(), controlWidth)) {
					for (int i = 0; i < 4; ++i) {
						if (ImGui::Selectable(a_provider.Translate(cornerKeys[i]).c_str(), i == corner)) {
							settings.toastX = corners[i].x;
							settings.toastY = corners[i].y;
							changed = true;
						}
					}
					ImGui::EndCombo();
				}
				describe(a_provider.Translate("UI_Setting_ToastsPosition"), a_provider.Translate("UI_SettingHint_ToastsPosition"));
				label(a_provider.Translate("UI_Setting_ToastsPlace"));
				if (ThemedButton((a_provider.Translate("UI_Setting_ToastsPlaceButton") + "##ToastsPlace").c_str(), controlWidth)) {
					StartToastPlacement();
				}
				describe(a_provider.Translate("UI_Setting_ToastsPlace"), a_provider.Translate("UI_SettingHint_ToastsPlace"));
				ImGui::EndDisabled();
				endColumns();
				ImGui::Spacing();
			}

			const bool table = !page.singleColumn;
			if (!table || beginColumns("##Columns")) {
				for (auto& column : page.columns) {
					if (table) {
						beginColumn();
					} else if (&column != &page.columns[0]) {
						break;
					} else {
						columnX = ImGui::GetCursorPosX();
						labelWidth = ImGui::GetFontSize() * 10.0f;
						controlWidth = ImGui::GetFontSize() * 14.0f;
					}
					// The completion log, read down then across in up to three columns
					const auto  isLogRow = [&](const SettingItem& a_item) { return !table && a_item.kind == SettingKind::kText && !a_item.value.empty(); };
					std::size_t logSkip = 0;
					for (auto& item : column) {
						if (logSkip > 0) {
							--logSkip;
							continue;
						}
						if (isLogRow(item)) {
							const auto  first = static_cast<std::size_t>(std::addressof(item) - column.data());
							std::size_t count = 0;
							while (first + count < column.size() && isLogRow(column[first + count])) {
								++count;
							}
							logSkip = count - 1;
							const int   logColumns = std::clamp(static_cast<int>(ImGui::GetContentRegionAvail().x / (ImGui::GetFontSize() * 22.0f)), 1, 3);
							const auto  logRows = (count + logColumns - 1) / logColumns;
							const float timeWidth = ImGui::GetFontSize() * 4.5f;
							if (ImGui::BeginTable("##Log", logColumns, ImGuiTableFlags_SizingStretchSame)) {
								for (std::size_t row = 0; row < logRows; ++row) {
									for (int logColumn = 0; logColumn < logColumns; ++logColumn) {
										ImGui::TableNextColumn();
										const auto index = static_cast<std::size_t>(logColumn) * logRows + row;
										if (index >= count) {
											continue;
										}
										const auto& entry = column[first + index];
										const auto  cellX = ImGui::GetCursorPosX();
										const float cellWidth = ImGui::GetContentRegionAvail().x;
										const auto  what = a_provider.Translate(entry.value);
										const auto  shown = FitText(what, cellWidth - timeWidth - style.ItemSpacing.x);
										ImGui::AlignTextToFramePadding();
										ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(entry.label).c_str());
										ImGui::SameLine();
										ImGui::SetCursorPosX(cellX + timeWidth);
										ImGui::TextColored(entry.number > 0 ? ColourFromRGB(entry.number) : palette.text, "%s", shown.c_str());
										if (shown != what && ImGui::IsItemHovered()) {
											ImGui::SetTooltip("%s", what.c_str());
										}
										const auto pos = ImGui::GetCursorScreenPos();
										Decor::FadedLine(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y - style.ItemSpacing.y * 0.5f), ImVec2(pos.x + cellWidth, pos.y - style.ItemSpacing.y * 0.5f), Colour(palette.separator, 0.5f));
									}
								}
								ImGui::EndTable();
							}
							continue;
						}
						ImGui::PushID(item.id.c_str());
						const auto text = a_provider.Translate(item.label);
						const auto key = page.id + '/' + item.id;
						const auto rowY = ImGui::GetCursorPosY();
						bool       edited = false;

						ImGui::BeginDisabled(!item.enabled);
						switch (item.kind) {
						case SettingKind::kHeader:
							columnHeading(text);
							break;
						case SettingKind::kSpacer:
							ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeight() * 0.5f));
							break;
						case SettingKind::kToggle:
							label(text);
							toggleSpot();
							edited = MarkerToggle("##Toggle", item.on);
							break;
						case SettingKind::kChoice:
							{
								label(text);
								const auto preview = item.choice >= 0 && item.choice < static_cast<int>(item.options.size()) ? a_provider.Translate(item.options[item.choice]) : std::string{};
								if (ThemedBeginCombo("##Choice", preview.c_str(), controlWidth)) {
									for (int i = 0; i < static_cast<int>(item.options.size()); ++i) {
										if (ImGui::Selectable(a_provider.Translate(item.options[i]).c_str(), i == item.choice)) {
											item.choice = i;
											edited = true;
										}
									}
									ImGui::EndCombo();
								}
							}
							break;
						case SettingKind::kSlider:
							{
								// Applied on release, like the MCM
								label(text);
								auto pending = _pendingNumbers.try_emplace(key, item.number).first;
								bool released = false;
								ThemedSlider("##Slider", ImGuiDataType_S32, &pending->second, &item.minimum, &item.maximum, "%d", controlWidth, &released);
								if (item.step > 1) {
									pending->second = std::clamp(static_cast<std::int32_t>(std::lround(static_cast<float>(pending->second) / static_cast<float>(item.step))) * item.step, item.minimum, item.maximum);
								}
								if (released) {
									item.number = pending->second;
									edited = true;
								}
								if (!ImGui::IsItemActive() && !released) {
									pending->second = item.number;
								}
							}
							break;
						case SettingKind::kColour:
							{
								label(text);
								constexpr auto popupId = "##ColourPopup";
								if (ColourButton("##Colour", item.number, controlWidth)) {
									_colourKey = key;
									_colourOriginal = item.number;
									_spectrumPosition = HueOf(item.number);
									// The picker's own focus starts once, as it opens: on the current colour's swatch, else the hue bar
									colourFocusPending = true;
									colourFocus = ColourStartFocus(item.number);
									colourPadNav = _padActive;
									colourActivateArmed = false;
									ImGui::OpenPopup(popupId);
								}
								const auto buttonMin = ImGui::GetItemRectMin();
								const auto buttonMax = ImGui::GetItemRectMax();
								// Its picker just closed: ImGui leaves the focus half there, so A wouldn't open it again
								if (_colourReturnKey == key) {
									auto* window = ImGui::GetCurrentWindow();
									ImGui::SetFocusID(ImGui::GetItemID(), window);
									window->NavRectRel[0] = ImGui::WindowRectAbsToRel(window, ImRect(buttonMin, buttonMax));
									if (_padActive) {
										ImGui::SetNavCursorVisible(true);
									}
									_colourReturnKey.clear();
								}
								if (BeginColourPopup(popupId, buttonMin, buttonMax, controlWidth)) {
									const auto texts = std::array{ a_provider.Translate("UI_ColourSwatches"), a_provider.Translate("UI_ColourCustom"), a_provider.Translate("UI_ColourCurrent"), a_provider.Translate("UI_ColourNew"), a_provider.Translate("UI_Done"), a_provider.Translate("UI_ColourHue"), a_provider.Translate("UI_ColourSaturation"), a_provider.Translate("UI_ColourBrightness") };
									bool       close = false;
									edited = ColourPanel(item.number, _spectrumPosition, _colourOriginal, { texts[0].c_str(), texts[1].c_str(), texts[2].c_str(), texts[3].c_str(), texts[4].c_str(), texts[5].c_str(), texts[6].c_str(), texts[7].c_str() }, close);
									if (close) {
										ImGui::CloseCurrentPopup();
									}
									ImGui::EndPopup();
								} else if (_colourKey == key) {
									_colourKey.clear();
									colourFocusPending = false;
									_colourReturnKey = key;
								}
							}
							break;
						case SettingKind::kInput:
							{
								label(text);
								auto& buffer = _pendingTexts[key];
								if (!_editingText.contains(key) && std::string_view(buffer.data()) != item.value) {
									std::snprintf(buffer.data(), buffer.size(), "%s", item.value.c_str());
								}
								if (ThemedTextInput("##Input", buffer.data(), buffer.size(), controlWidth)) {
									item.value = buffer.data();
									edited = true;
								}
								if (ImGui::IsItemActive()) {
									_editingText.insert(key);
								} else {
									_editingText.erase(key);
								}
							}
							break;
						case SettingKind::kKey:
							{
								label(text);
								const bool waiting = settings.captureTarget == key;
								const auto caption = waiting ? a_provider.Translate("UI_Setting_PressKeyUnbind") : (item.number < 0 ? a_provider.Translate("UI_Setting_Unbound") : item.value);
								if (ThemedButton((caption + "##Key").c_str(), controlWidth)) {
									settings.captureTarget = key;
									settings.captureResult = UISettings::captureNone;
								}
							}
							break;
						case SettingKind::kButton:
							{
								// Needs a second click
								if (!text.empty()) {
									label(text);
								} else {
									ImGui::SetCursorPosX(columnX + labelWidth);
								}
								const bool armed = _confirmKey == key;
								const auto caption = armed ? a_provider.Translate("UI_ConfirmClick") : a_provider.Translate(item.value);
								if (ThemedButton((caption + "##Button").c_str(), controlWidth)) {
									if (item.confirm && !armed) {
										_confirmKey = key;
									} else {
										_confirmKey.clear();
										edited = true;
									}
								}
							}
							break;
						case SettingKind::kList:
							{
								if (item.options.empty()) {
									ImGui::AlignTextToFramePadding();
									ImGui::TextColored(palette.textDim, "%s", text.c_str());
									break;
								}
								constexpr std::size_t visibleRows = 6;
								const float           listHeight = ImGui::GetFrameHeightWithSpacing() * static_cast<float>(std::min(item.options.size(), visibleRows)) + style.WindowPadding.y * 2.0f;
								BeginFramedPanel("##List", ImVec2(0.0f, listHeight));
								const bool  scrolls = item.options.size() > visibleRows;
								const float scrollRoom = scrolls ? style.ItemSpacing.x + (ImGui::GetScrollMaxY() <= 0.0f ? style.ScrollbarSize : 0.0f) : 0.0f;
								const float width = ImGui::GetContentRegionAvail().x - scrollRoom;
								const float buttonWidth = std::min(width * 0.35f, ImGui::GetFontSize() * 9.0f);
								for (std::size_t i = 0; i < item.options.size() && i < item.keys.size(); ++i) {
									ImGui::PushID(static_cast<int>(i));
									const auto entryKey = key + '/' + item.keys[i];
									const auto rowStart = ImGui::GetCursorScreenPos();

									ImGui::AlignTextToFramePadding();
									ImGui::PushClipRect(rowStart, ImVec2(rowStart.x + width - buttonWidth - style.ItemSpacing.x, rowStart.y + ImGui::GetFrameHeight()), true);
									ImGui::TextUnformatted(item.options[i].c_str());
									ImGui::PopClipRect();
									ImGui::SameLine();
									ImGui::SetCursorScreenPos(ImVec2(rowStart.x + width - buttonWidth, rowStart.y));

									const bool armed = _confirmKey == entryKey;
									const auto caption = (armed ? a_provider.Translate("UI_ConfirmClick") : a_provider.Translate(item.value)) + "##Entry";
									if (ThemedButton(caption.c_str(), buttonWidth)) {
										if (item.confirm && !armed) {
											_confirmKey = entryKey;
										} else {
											_confirmKey.clear();
											auto entry = item;
											entry.id = item.keys[i];
											a_provider.ApplySetting(page.id, entry);
										}
									}
									ImGui::PopID();
								}
								EndFramedPanel();
							}
							break;
						case SettingKind::kText:
							if (!table && !item.value.empty()) {
								const float timeWidth = ImGui::GetFontSize() * 4.5f;
								ImGui::AlignTextToFramePadding();
								ImGui::TextColored(palette.textDim, "%s", text.c_str());
								ImGui::SameLine();
								ImGui::SetCursorPosX(columnX + timeWidth);
								ImGui::TextColored(item.number > 0 ? ColourFromRGB(item.number) : palette.text, "%s", a_provider.Translate(item.value).c_str());
								const auto  pos = ImGui::GetCursorScreenPos();
								const float width = ImGui::GetContentRegionAvail().x;
								Decor::FadedLine(ImGui::GetWindowDrawList(), ImVec2(pos.x, pos.y - style.ItemSpacing.y * 0.5f), ImVec2(pos.x + width, pos.y - style.ItemSpacing.y * 0.5f), Colour(palette.separator, 0.5f));
							} else if (item.value.empty()) {
								ImGui::AlignTextToFramePadding();
								ImGui::TextColored(palette.textDim, "%s", text.c_str());
							} else {
								ImGui::AlignTextToFramePadding();
								ImGui::TextColored(palette.textDim, "%s", text.c_str());
								ImGui::SameLine();
								ImGui::SetCursorPosX(columnX + labelWidth + style.FramePadding.x);
								ImGui::TextColored(palette.textDim, "%s", a_provider.Translate(item.value).c_str());
							}
							break;
						}
						ImGui::EndDisabled();

						describe(text, a_provider.Translate(item.info));

						if (item.kind != SettingKind::kHeader && item.kind != SettingKind::kSpacer) {
							const float rowEnd = rowY + ImGui::GetFrameHeightWithSpacing();
							if (ImGui::GetCursorPosY() < rowEnd) {
								ImGui::SetCursorPosY(rowEnd - style.ItemSpacing.y);
								ImGui::Dummy(ImVec2(0.0f, 0.0f));
							}
						}
						if (edited) {
							a_provider.ApplySetting(page.id, item);
							_dirty = page.id == "general";
						}
						ImGui::PopID();
					}
				}
				if (table) {
					endColumns();
				}
			}
			ImGui::PopID();
		}

		EndFramedPanel();

		DrawDescriptionPanel(a_provider, panelHeight, gap);

		_settingsChanged |= changed;
	}

	//---------------------------------------------------
	//-- Completionist UI ( Passes Filter ) -------------
	//---------------------------------------------------

	bool Menu::PassesFilter(const Entry& a_entry) const {

		if (_hideCompleted && a_entry.IsComplete()) {
			return false;
		}

		if (_filter[0] == '\0') {
			return true;
		}

		const auto needle = Lower(_filter.data());
		return Lower(a_entry.name).find(needle) != std::string::npos || Lower(a_entry.detail).find(needle) != std::string::npos;
	}
}
