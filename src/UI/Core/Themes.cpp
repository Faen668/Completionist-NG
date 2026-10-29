#include "UI/Core/Themes.hpp"

#include "UI/Core/Decor.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace CompletionistUI
{
	namespace
	{

		//---------------------------------------------------
		//-- Themes ( RGBA ) --------------------------------
		//---------------------------------------------------

		constexpr ImVec4 RGBA(int a_r, int a_g, int a_b, float a_a = 1.0f) {

			return ImVec4(a_r / 255.0f, a_g / 255.0f, a_b / 255.0f, a_a);
		}

		//---------------------------------------------------
		//-- Themes ( Font Exists ) -------------------------
		//---------------------------------------------------

		bool FontExists(const std::string& a_path) {

			return !a_path.empty() && std::filesystem::exists(a_path);
		}

		//---------------------------------------------------
		//-- Themes ( Add Font ) ----------------------------
		//---------------------------------------------------

		ImFont* AddFont(const std::string& a_path, const std::string& a_fallback, float a_size, const ImWchar* a_ranges) {

			auto&   io = ImGui::GetIO();
			ImFont* font = nullptr;
			if (FontExists(a_path)) {
				font = io.Fonts->AddFontFromFileTTF(a_path.c_str(), a_size, nullptr, a_ranges);
			} else {
				ImFontConfig config;
				config.SizePixels = a_size;
				font = io.Fonts->AddFontDefault(&config);
			}

			if (FontExists(a_fallback) && a_fallback != a_path) {
				ImFontConfig merge;
				merge.MergeMode = true;
				io.Fonts->AddFontFromFileTTF(a_fallback.c_str(), a_size, &merge, a_ranges);
			}
			return font;
		}

		//---------------------------------------------------
		//-- Themes ( Sky UI Palette ) ----------------------
		//---------------------------------------------------

		Palette SkyUIPalette() {

			return {
				.background = RGBA(0, 0, 0, 0.82f),
				.panel = RGBA(12, 12, 12, 0.6f),
				.text = RGBA(255, 255, 255),
				.textDim = RGBA(140, 140, 140),
				.accent = RGBA(255, 255, 255),
				.heading = RGBA(255, 255, 255),
				.count = RGBA(140, 140, 140),
				.bar = RGBA(255, 255, 255),
				.found = RGBA(140, 140, 140),
				.needed = RGBA(255, 255, 255),
				.displayable = RGBA(190, 190, 190),
				.occupied = RGBA(110, 110, 110),
				.barBackground = RGBA(45, 45, 45, 0.9f),
				.separator = RGBA(90, 90, 90),
				.pinned = RGBA(130, 178, 224),
				.stealing = RGBA(238, 112, 150),
			};
		}
	}

	namespace
	{

		//---------------------------------------------------
		//-- Themes ( Completionist Palette ) ---------------
		//---------------------------------------------------

		Palette CompletionistPalette() {

			return {
				.background = RGBA(26, 23, 19, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(226, 218, 200),
				.textDim = RGBA(138, 128, 110),
				.accent = RGBA(198, 162, 98),
				.heading = RGBA(198, 162, 98),
				.count = RGBA(138, 128, 110),
				.bar = RGBA(198, 162, 98),
				.found = RGBA(198, 162, 98),
				.needed = RGBA(226, 218, 200),
				.displayable = RGBA(130, 170, 190),
				.occupied = RGBA(160, 130, 180),
				.barBackground = RGBA(48, 43, 36),
				.separator = RGBA(110, 94, 68),
				.pinned = RGBA(124, 162, 200),
				.stealing = RGBA(238, 112, 150),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Altmer Palette ) ----------------------
		//---------------------------------------------------

		// Summerset, the one light theme: white marble, dark ink, gold
		Palette AltmerPalette() {

			return {
				.background = RGBA(238, 233, 219, 0.95f),
				.panel = RGBA(246, 242, 232, 0.0f),
				.text = RGBA(52, 46, 38),
				.textDim = RGBA(128, 116, 96),
				.accent = RGBA(184, 140, 46),
				.heading = RGBA(36, 32, 26),
				.count = RGBA(112, 96, 66),
				.bar = RGBA(204, 160, 60),
				.found = RGBA(136, 96, 18),
				.needed = RGBA(52, 46, 38),
				.displayable = RGBA(52, 104, 168),
				.occupied = RGBA(146, 70, 124),
				.barBackground = RGBA(224, 214, 192),
				.separator = RGBA(200, 176, 122),
				.pinned = RGBA(44, 104, 170),
				.stealing = RGBA(184, 44, 66),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Argonian Palette ) --------------------
		//---------------------------------------------------

		// Black Marsh: murky olive water, moss-green headings, Hist sap amber for found
		Palette ArgonianPalette() {

			return {
				.background = RGBA(11, 16, 10, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(214, 216, 196),
				.textDim = RGBA(125, 134, 112),
				.accent = RGBA(138, 163, 90),
				.heading = RGBA(164, 179, 106),
				.count = RGBA(170, 166, 128),
				.bar = RGBA(124, 148, 80),
				.found = RGBA(227, 164, 60),
				.needed = RGBA(214, 216, 196),
				.displayable = RGBA(130, 172, 204),
				.occupied = RGBA(170, 130, 170),
				.barBackground = RGBA(31, 40, 30),
				.separator = RGBA(74, 82, 64),
				.pinned = RGBA(120, 182, 214),
				.stealing = RGBA(230, 110, 120),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Dunmer Palette ) ----------------------
		//---------------------------------------------------

		// Ash and ember: charcoal, Red Mountain red, salmon headings, molten gold for found
		Palette DunmerPalette() {

			return {
				.background = RGBA(17, 12, 12, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(224, 212, 200),
				.textDim = RGBA(140, 124, 118),
				.accent = RGBA(194, 72, 46),
				.heading = RGBA(234, 146, 100),
				.count = RGBA(192, 142, 112),
				.bar = RGBA(152, 52, 34),
				.found = RGBA(248, 184, 72),
				.needed = RGBA(224, 212, 200),
				.displayable = RGBA(140, 180, 196),
				.occupied = RGBA(170, 126, 176),
				.barBackground = RGBA(42, 26, 24),
				.separator = RGBA(92, 60, 50),
				.pinned = RGBA(124, 176, 210),
				.stealing = RGBA(236, 112, 132),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Khajiit Palette ) ---------------------
		//---------------------------------------------------

		// Elsweyr night: indigo and plum, brass, saffron headings, moonlight for found
		Palette KhajiitPalette() {

			return {
				.background = RGBA(14, 11, 24, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(234, 220, 196),
				.textDim = RGBA(146, 130, 128),
				.accent = RGBA(204, 148, 66),
				.heading = RGBA(240, 194, 116),
				.count = RGBA(200, 160, 112),
				.bar = RGBA(150, 104, 46),
				.found = RGBA(138, 192, 238),
				.needed = RGBA(234, 220, 196),
				.displayable = RGBA(150, 196, 160),
				.occupied = RGBA(196, 132, 172),
				.barBackground = RGBA(40, 30, 46),
				.separator = RGBA(98, 74, 66),
				.pinned = RGBA(120, 200, 190),
				.stealing = RGBA(232, 112, 122),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Bosmer Palette ) ----------------------
		//---------------------------------------------------

		// Dark forest: moss green, oak bark headings and counts, autumn gold for found
		Palette BosmerPalette() {

			return {
				.background = RGBA(10, 15, 9, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(224, 222, 196),
				.textDim = RGBA(122, 130, 102),
				.accent = RGBA(118, 152, 76),
				.heading = RGBA(240, 188, 120),
				.count = RGBA(196, 154, 104),
				.bar = RGBA(88, 120, 58),
				.found = RGBA(200, 146, 70),
				.needed = RGBA(224, 222, 196),
				.displayable = RGBA(120, 178, 168),
				.occupied = RGBA(176, 128, 150),
				.barBackground = RGBA(34, 44, 29),
				.separator = RGBA(96, 82, 56),
				.pinned = RGBA(124, 172, 196),
				.stealing = RGBA(232, 118, 128),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Nord Palette ) ------------------------
		//---------------------------------------------------

		// A mead hall: dark timber and iron, bone text, hearth amber headings, gold for found
		Palette NordPalette() {

			return {
				.background = RGBA(18, 15, 12, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(230, 220, 200),
				.textDim = RGBA(140, 132, 120),
				.accent = RGBA(216, 154, 74),
				.heading = RGBA(216, 154, 74),
				.count = RGBA(176, 166, 150),
				.bar = RGBA(194, 135, 62),
				.found = RGBA(226, 180, 92),
				.needed = RGBA(230, 220, 200),
				.displayable = RGBA(150, 180, 200),
				.occupied = RGBA(186, 140, 176),
				.barBackground = RGBA(44, 40, 36),
				.separator = RGBA(90, 84, 76),
				.pinned = RGBA(130, 170, 200),
				.stealing = RGBA(232, 112, 122),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Dear Diary Palette ) ------------------
		//---------------------------------------------------

		// Dear Diary Dark Mode's look: white on near-black, bronze edges, red for the chosen, green for the done
		Palette DearDiaryPalette() {

			return {
				.background = RGBA(12, 11, 10, 0.95f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(236, 232, 226),
				.textDim = RGBA(150, 144, 136),
				.accent = RGBA(196, 44, 36),
				.heading = RGBA(242, 238, 230),
				.count = RGBA(170, 164, 156),
				.bar = RGBA(176, 40, 32),
				.found = RGBA(92, 186, 74),
				.needed = RGBA(236, 232, 226),
				.displayable = RGBA(112, 160, 214),
				.occupied = RGBA(176, 128, 196),
				.barBackground = RGBA(36, 32, 30),
				.separator = RGBA(96, 82, 68),
				.pinned = RGBA(118, 162, 214),
				.stealing = RGBA(232, 104, 132),
			};
		}

		//---------------------------------------------------
		//-- Themes ( Nordic Palette ) ----------------------
		//---------------------------------------------------

		// Nordic UI's look: silver on black, pale gold as on the level shield
		Palette NordicPalette() {

			return {
				.background = RGBA(2, 2, 3, 0.93f),
				.panel = RGBA(0, 0, 0, 0.0f),
				.text = RGBA(230, 228, 222),
				.textDim = RGBA(146, 144, 140),
				.accent = RGBA(206, 186, 140),
				.heading = RGBA(242, 240, 234),
				.count = RGBA(170, 168, 162),
				.bar = RGBA(206, 186, 140),
				.found = RGBA(206, 186, 140),
				.needed = RGBA(230, 228, 222),
				.displayable = RGBA(122, 176, 192),
				.occupied = RGBA(176, 150, 210),
				.barBackground = RGBA(34, 34, 36),
				.separator = RGBA(104, 102, 98),
				.pinned = RGBA(126, 170, 214),
				.stealing = RGBA(222, 102, 122),
			};
		}
	}

	//---------------------------------------------------
	//-- Themes ( From Name ) ---------------------------
	//---------------------------------------------------

	// Old names still load: Nordic was Completionist, Woodland was Bosmer
	Theme Themes::FromName(std::string_view a_name) noexcept {

		const auto same = [&](std::string_view a_other) {
			return std::ranges::equal(a_name, a_other, [](char a, char b) { return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b)); });
		};
		if (same("Woodland")) {
			return Theme::kBosmer;
		}
		for (const auto& info : themeList) {
			if (same(info.name)) {
				return info.theme;
			}
		}
		return Theme::kCompletionist;
	}

	//---------------------------------------------------
	//-- Themes ( Name ) --------------------------------
	//---------------------------------------------------

	std::string_view Themes::Name(Theme a_theme) noexcept {

		for (const auto& info : themeList) {
			if (info.theme == a_theme) {
				return info.name;
			}
		}
		return themeList[0].name;
	}

	//---------------------------------------------------
	//-- Themes ( Build Fonts ) -------------------------
	//---------------------------------------------------

	void Themes::BuildFonts(const FontPaths& a_paths, float a_scale) {

		auto& io = ImGui::GetIO();
		io.Fonts->Clear();

		// Latin, Latin Extended-A and Cyrillic
		static ImVector<ImWchar> ranges;
		ranges.clear();
		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
		builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
		static constexpr ImWchar latinExtended[] = { 0x0100, 0x017F, 0x2010, 0x2027, 0 };
		builder.AddRanges(latinExtended);
		builder.BuildRanges(&ranges);

		const auto& bold = a_paths.bold.empty() ? a_paths.regular : a_paths.bold;
		const auto& heading = a_paths.heading.empty() ? bold : a_paths.heading;
		const auto& title = a_paths.title.empty() ? heading : a_paths.title;
		_body = AddFont(a_paths.regular, a_paths.fallback, 18.0f * a_scale, ranges.Data);
		_small = AddFont(a_paths.regular, a_paths.fallback, 15.0f * a_scale, ranges.Data);
		_header = AddFont(heading, a_paths.fallback, 22.0f * a_scale, ranges.Data);
		_title = AddFont(title, a_paths.fallback, 30.0f * a_scale, ranges.Data);

		io.FontDefault = _body;
		io.Fonts->Build();
	}

	//---------------------------------------------------
	//-- Themes ( Apply ) -------------------------------
	//---------------------------------------------------

	void Themes::Apply(Theme a_theme, float a_scale) {

		_theme = a_theme;
		Decor::SetTheme(a_theme);
		switch (a_theme) {
		case Theme::kSkyUI:
			_palette = SkyUIPalette();
			break;
		case Theme::kBosmer:
			_palette = BosmerPalette();
			break;
		case Theme::kArgonian:
			_palette = ArgonianPalette();
			break;
		case Theme::kKhajiit:
			_palette = KhajiitPalette();
			break;
		case Theme::kDunmer:
			_palette = DunmerPalette();
			break;
		case Theme::kNord:
			_palette = NordPalette();
			break;
		case Theme::kAltmer:
			_palette = AltmerPalette();
			break;
		case Theme::kDearDiary:
			_palette = DearDiaryPalette();
			break;
		case Theme::kNordic:
			_palette = NordicPalette();
			break;
		default:
			_palette = CompletionistPalette();
			break;
		}

		auto& style = ImGui::GetStyle();
		style = ImGuiStyle();

		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.FrameBorderSize = 1.0f;
		style.WindowPadding = ImVec2(18.0f, 16.0f);
		style.FramePadding = ImVec2(8.0f, 5.0f);
		style.ItemSpacing = ImVec2(10.0f, 7.0f);
		style.ScrollbarSize = 6.0f;
		style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

		const auto& p = _palette;
		const bool  neutral = a_theme == Theme::kSkyUI;
		auto*       c = style.Colors;
		c[ImGuiCol_Text] = p.text;
		c[ImGuiCol_TextDisabled] = p.textDim;
		c[ImGuiCol_WindowBg] = p.background;
		c[ImGuiCol_ChildBg] = p.panel;
		c[ImGuiCol_PopupBg] = ImVec4(p.panel.x, p.panel.y, p.panel.z, 0.98f);
		c[ImGuiCol_Border] = p.separator;
		c[ImGuiCol_Separator] = p.separator;
		c[ImGuiCol_FrameBg] = neutral ? ImVec4(0, 0, 0, 0.5f) : p.barBackground;
		c[ImGuiCol_FrameBgHovered] = neutral ? ImVec4(1, 1, 1, 0.08f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.18f);
		c[ImGuiCol_FrameBgActive] = neutral ? ImVec4(1, 1, 1, 0.14f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.28f);
		c[ImGuiCol_Header] = neutral ? ImVec4(1, 1, 1, 0.10f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.20f);
		c[ImGuiCol_HeaderHovered] = neutral ? ImVec4(1, 1, 1, 0.16f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.30f);
		c[ImGuiCol_HeaderActive] = neutral ? ImVec4(1, 1, 1, 0.22f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.40f);
		c[ImGuiCol_Button] = neutral ? ImVec4(0, 0, 0, 0) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.15f);
		c[ImGuiCol_ButtonHovered] = neutral ? ImVec4(1, 1, 1, 0.12f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.30f);
		c[ImGuiCol_ButtonActive] = neutral ? ImVec4(1, 1, 1, 0.20f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.45f);
		c[ImGuiCol_CheckMark] = p.accent;
		c[ImGuiCol_SliderGrab] = ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.75f);
		c[ImGuiCol_SliderGrabActive] = p.accent;
		c[ImGuiCol_PlotHistogram] = neutral ? ImVec4(1, 1, 1, 0.5f) : p.bar;
		c[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
		c[ImGuiCol_ScrollbarGrab] = p.separator;
		c[ImGuiCol_ScrollbarGrabHovered] = p.textDim;
		c[ImGuiCol_ScrollbarGrabActive] = p.text;
		c[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
		c[ImGuiCol_TableRowBgAlt] = neutral ? ImVec4(1, 1, 1, 0.03f) : ImVec4(p.accent.x, p.accent.y, p.accent.z, 0.04f);
		c[ImGuiCol_NavCursor] = p.accent;

		style.ScaleAllSizes(a_scale);
		// ImGui rounds the cursor scale down to a whole number: below 1 (720p, a small UI scale) it hit 0
		// and the mouse cursor vanished
		style.MouseCursorScale = std::max(style.MouseCursorScale, 1.0f);
	}
}
