#include "UI/Core/Toasts.hpp"

#include "UI/Core/Decor.hpp"
#include "UI/Core/Themes.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>

namespace CompletionistUI
{
	namespace
	{
		constexpr double enterTime = 0.28;
		constexpr double leaveTime = 0.40;
		constexpr double leaveGap = 0.30;
		constexpr std::size_t waitingCap = 200;

		//---------------------------------------------------
		//-- Toasts ( Colour ) ------------------------------
		//---------------------------------------------------

		ImU32 Colour(ImVec4 a_colour, float a_alpha) {

			a_colour.w *= std::clamp(a_alpha, 0.0f, 1.0f);
			return ImGui::ColorConvertFloat4ToU32(a_colour);
		}

		//---------------------------------------------------
		//-- Toasts ( Ease Out ) ----------------------------
		//---------------------------------------------------

		float EaseOut(double a_t) {

			const float t = static_cast<float>(std::clamp(a_t, 0.0, 1.0));
			return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
		}

		//---------------------------------------------------
		//-- Toasts ( Text Size ) ---------------------------
		//---------------------------------------------------

		ImVec2 TextSize(ImFont* a_font, float a_size, const char* a_begin, const char* a_end = nullptr) {

			return a_font->CalcTextSizeA(a_size, FLT_MAX, 0.0f, a_begin, a_end);
		}

		//---------------------------------------------------
		//-- Toasts ( Fit ) ---------------------------------
		//---------------------------------------------------

		// Trims the text to fit, with ...
		std::string Fit(ImFont* a_font, float a_size, const std::string& a_text, float a_width) {

			if (TextSize(a_font, a_size, a_text.c_str()).x <= a_width) {
				return a_text;
			}
			constexpr const char* ellipsis = "\xE2\x80\xA6";
			const float           room = a_width - TextSize(a_font, a_size, ellipsis).x;
			std::size_t           end = a_text.size();
			while (end > 0) {
				do {
					--end;
				} while (end > 0 && (static_cast<unsigned char>(a_text[end]) & 0xC0) == 0x80);
				if (TextSize(a_font, a_size, a_text.c_str(), a_text.c_str() + end).x <= room) {
					break;
				}
			}
			while (end > 0 && a_text[end - 1] == ' ') {
				--end;
			}
			return end > 0 ? a_text.substr(0, end) + ellipsis : std::string{};
		}

		//---------------------------------------------------
		//-- Toasts ( Caps ) --------------------------------
		//---------------------------------------------------

		std::string Caps(std::string a_text) {

			std::ranges::transform(a_text, a_text.begin(), [](unsigned char c) { return static_cast<char>(c < 0x80 ? std::toupper(c) : c); });
			return a_text;
		}

		struct Metrics
		{
			ImFont* body;
			float   fontSize;
			ImFont* smallFont;
			float   smallSize;
			ImVec2  margin;
		};

		//---------------------------------------------------
		//-- Toasts ( Get Metrics ) -------------------------
		//---------------------------------------------------

		Metrics GetMetrics() {

			Metrics metrics{};
			ImGui::PushFont(Themes::BodyFont());
			metrics.body = ImGui::GetFont();
			metrics.fontSize = ImGui::GetFontSize();
			ImGui::PopFont();
			ImGui::PushFont(Themes::SmallFont());
			metrics.smallFont = ImGui::GetFont();
			metrics.smallSize = ImGui::GetFontSize();
			ImGui::PopFont();
			metrics.margin = ImVec2(metrics.fontSize * 1.2f, ImGui::GetIO().DisplaySize.y * 0.05f);
			return metrics;
		}

		//---------------------------------------------------
		//-- Toasts ( Icon For ) ----------------------------
		//---------------------------------------------------

		Icon IconFor(ToastKind a_kind) {

			switch (a_kind) {
			case ToastKind::kEntry:
				return Icon::kCollections;
			case ToastKind::kDiscovery:
				return Icon::kLocations;
			case ToastKind::kCell:
				return Icon::kQuests;
			default:
				return Icon::kOverview;
			}
		}
	}

	//---------------------------------------------------
	//-- Toasts ( Push ) --------------------------------
	//---------------------------------------------------

	void Toasts::Push(std::string a_label, std::string a_text, ToastKind a_kind, std::optional<ImVec4> a_colour) {

		std::scoped_lock lock(_lock);
		// Same message again, bump the count
		const auto same = [&](const Toast& a_toast) { return a_toast.leaving < 0.0 && a_toast.text == a_text && a_toast.label == a_label; };
		if (const auto it = std::ranges::find_if(_shown, same); it != _shown.end()) {
			++it->count;
			it->repeated = true;
			return;
		}
		if (const auto it = std::ranges::find_if(_waiting, same); it != _waiting.end()) {
			++it->count;
			return;
		}
		if (_waiting.size() >= waitingCap) {
			_waiting.pop_front();
		}
		_waiting.push_back({ .label = std::move(a_label), .text = std::move(a_text), .kind = a_kind, .colour = a_colour });
	}

	//---------------------------------------------------
	//-- Toasts ( Active ) ------------------------------
	//---------------------------------------------------

	bool Toasts::Active() const {

		std::scoped_lock lock(_lock);
		return !_shown.empty() || !_waiting.empty();
	}

	//---------------------------------------------------
	//-- Toasts ( Waiting ) -----------------------------
	//---------------------------------------------------

	std::size_t Toasts::Waiting() const {

		std::scoped_lock lock(_lock);
		return _waiting.size();
	}

	//---------------------------------------------------
	//-- Toasts ( Clear ) -------------------------------
	//---------------------------------------------------

	void Toasts::Clear() {

		std::scoped_lock lock(_lock);
		_shown.clear();
		_waiting.clear();
	}

	//---------------------------------------------------
	//-- Toasts ( Advance ) -----------------------------
	//---------------------------------------------------

	void Toasts::Advance(double a_now, const ToastOptions& a_options) {

		// Preview, nothing leaves
		if (a_options.preview) {
			while (!_waiting.empty()) {
				auto toast = std::move(_waiting.front());
				_waiting.pop_front();
				toast.entered = a_now;
				toast.hold = 1.0;
				_shown.push_back(std::move(toast));
			}
			for (auto& toast : _shown) {
				toast.shown = a_now;
			}
			return;
		}

		for (auto& toast : _shown) {
			if (std::exchange(toast.repeated, false)) {
				toast.shown = a_now;
				toast.bumped = a_now;
			}
		}

		std::erase_if(_shown, [&](const Toast& a_toast) { return a_toast.leaving >= 0.0 && a_now - a_toast.leaving > leaveTime; });

		// Shorter time while others are waiting
		const double full = std::max(1.0f, a_options.seconds);
		const double quick = std::max(1.5, full * 0.5);
		auto         live = std::ranges::count_if(_shown, [](const Toast& a_toast) { return a_toast.leaving < 0.0; });
		while (live < std::max(1, a_options.visible) && !_waiting.empty()) {
			auto toast = std::move(_waiting.front());
			_waiting.pop_front();
			toast.entered = toast.shown = a_now;
			toast.hold = full;
			_shown.push_back(std::move(toast));
			++live;
		}
		if (!_waiting.empty()) {
			for (auto& toast : _shown) {
				toast.hold = std::min(toast.hold, quick);
			}
		}

		// One leaves at a time, oldest first
		if (a_now - _lastLeave >= leaveGap) {
			for (auto& toast : _shown) {
				if (toast.leaving < 0.0 && a_now - toast.shown >= toast.hold) {
					toast.leaving = a_now;
					_lastLeave = a_now;
					break;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Toasts ( Travel ) ------------------------------
	//---------------------------------------------------

	ImVec2 Toasts::Travel(ImVec2 a_size) {

		const auto  metrics = GetMetrics();
		const auto& display = ImGui::GetIO().DisplaySize;
		return ImVec2(std::max(0.0f, display.x - metrics.margin.x * 2.0f - a_size.x), std::max(0.0f, display.y - metrics.margin.y * 2.0f - a_size.y));
	}

	//---------------------------------------------------
	//-- Toasts ( Draw ) --------------------------------
	//---------------------------------------------------

	ToastBounds Toasts::Draw(double a_now, const ToastOptions& a_options, const std::string& a_moreText) {

		std::scoped_lock lock(_lock);
		Advance(a_now, a_options);
		const float delta = _lastDraw < 0.0 ? 0.0f : static_cast<float>(std::clamp(a_now - _lastDraw, 0.0, 0.1));
		_lastDraw = a_now;
		if (_shown.empty()) {
			return {};
		}

		const auto& palette = Themes::GetPalette();
		const bool  skyUI = Themes::GetTheme() == Theme::kSkyUI;
		auto*       drawList = ImGui::GetForegroundDrawList();

		const auto  metrics = GetMetrics();
		ImFont*     body = metrics.body;
		const float fontSize = metrics.fontSize;
		ImFont*     smallFont = metrics.smallFont;
		const float smallSize = metrics.smallSize;

		const float width = fontSize * 18.0f;
		const float pad = fontSize * 0.55f;
		const float iconSide = fontSize * 1.7f;
		const float gap = fontSize * 0.35f;
		const float ease = 1.0f - std::exp(-delta * 14.0f);
		const float opacity = std::clamp(a_options.opacity, 0.3f, 1.0f);
		const auto  cardHeight = [&](const Toast& a_toast) { return pad * 2.0f + (a_toast.label.empty() ? fontSize : smallSize + fontSize * 1.05f); };

		const bool more = !a_moreText.empty() && !_waiting.empty();
		float      stackHeight = more ? smallSize + pad * 0.5f : -gap;
		for (const auto& toast : _shown) {
			stackHeight += cardHeight(toast) + gap;
		}
		const auto  travel = Travel(ImVec2(width, stackHeight));
		const float left = metrics.margin.x + std::clamp(a_options.x, 0.0f, 1.0f) * travel.x;
		const float top = metrics.margin.y + std::clamp(a_options.y, 0.0f, 1.0f) * travel.y;
		const float side = a_options.x > 0.5f ? -1.0f : 1.0f;

		// Newest at the top by default
		float y = top;
		for (std::size_t n = 0; n < _shown.size(); ++n) {
			auto&       toast = _shown[a_options.newestFirst ? _shown.size() - 1 - n : n];
			const bool  twoLines = !toast.label.empty();
			const float height = cardHeight(toast);

			if (!toast.placed) {
				toast.y = y;
				toast.placed = true;
			}
			toast.y += (y - toast.y) * ease;
			y += height + gap;

			const float in = EaseOut((a_now - toast.entered) / enterTime);
			const float out = toast.leaving >= 0.0 ? EaseOut((a_now - toast.leaving) / leaveTime) : 0.0f;
			const float alpha = in * (1.0f - out);
			if (alpha <= 0.01f) {
				continue;
			}
			const float  x = left - side * ((1.0f - in) * fontSize * 2.0f + out * fontSize * 1.5f);
			const ImVec2 min(x, toast.y);
			const ImVec2 max(x + width, toast.y + height);
			const ImVec4 accent = toast.colour.value_or(palette.accent);

			auto background = palette.background;
			background.w = opacity;
			if (skyUI) {
				const ImU32 edge = Colour(background, alpha);
				const ImU32 away = Colour(background, alpha * 0.15f);
				const ImU32 l = side > 0.0f ? edge : away;
				const ImU32 r = side > 0.0f ? away : edge;
				drawList->AddRectFilledMultiColor(min, max, l, r, r, l);
			} else {
				drawList->AddRectFilled(min, max, Colour(background, alpha), fontSize * 0.15f);
				drawList->AddRect(min, max, Colour(palette.separator, alpha * 0.9f), fontSize * 0.15f);
				drawList->AddRectFilled(min, ImVec2(min.x + std::max(2.0f, fontSize * 0.12f), max.y), Colour(accent, alpha));
			}

			const ImVec2 iconCentre(min.x + pad + iconSide * 0.4f, (min.y + max.y) * 0.5f);
			Decor::DrawIcon(drawList, IconFor(toast.kind), iconCentre, fontSize * 1.05f, Colour(skyUI ? palette.text : accent, alpha * (skyUI ? 0.8f : 1.0f)));

			// Repeat count
			const float textX = min.x + pad + iconSide;
			float       right = max.x - pad;
			if (toast.count > 1) {
				const auto  count = "\xC3\x97" + std::to_string(toast.count);
				const float pulse = toast.bumped >= 0.0 ? 1.0f - EaseOut((a_now - toast.bumped) / 0.6) : 0.0f;
				const float countWidth = TextSize(body, fontSize, count.c_str()).x;
				const float countY = twoLines ? min.y + pad + smallSize : min.y + pad;
				drawList->AddText(body, fontSize, ImVec2(right - countWidth, countY), Colour(skyUI ? palette.text : accent, alpha * (0.75f + 0.25f * pulse)), count.c_str());
				right -= countWidth + fontSize * 0.5f;
			}

			float textY = min.y + pad;
			if (twoLines) {
				const auto label = Fit(smallFont, smallSize, skyUI ? Caps(toast.label) : toast.label, right - textX);
				drawList->AddText(smallFont, smallSize, ImVec2(textX, textY), Colour(palette.textDim, alpha), label.c_str());
				textY += smallSize + fontSize * 0.05f;
			}
			const auto text = Fit(body, fontSize, toast.text, right - textX);
			drawList->AddText(body, fontSize, ImVec2(textX, textY), Colour(toast.colour.value_or(palette.text), alpha), text.c_str());

			// Time left
			if (toast.leaving < 0.0 && toast.hold > 0.0) {
				const float remaining = 1.0f - static_cast<float>(std::clamp((a_now - toast.shown) / toast.hold, 0.0, 1.0));
				const float inset = skyUI ? 0.0f : fontSize * 0.25f;
				const float barY = max.y - (skyUI ? 1.0f : fontSize * 0.18f);
				const ImVec2 from(textX, barY);
				const ImVec2 to(from.x + (max.x - inset - from.x) * remaining, barY);
				if (skyUI) {
					Decor::FadedLine(drawList, from, to, Colour(palette.text, alpha * 0.45f));
				} else {
					drawList->AddLine(from, to, Colour(accent, alpha * 0.55f), std::max(1.0f, fontSize * 0.06f));
				}
			}
		}

		// +N more
		if (more) {
			const ImVec2 at(left + pad, y);
			const auto   size = TextSize(smallFont, smallSize, a_moreText.c_str());
			auto         background = palette.background;
			background.w = opacity * 0.8f;
			drawList->AddRectFilled(ImVec2(at.x - pad * 0.6f, at.y), ImVec2(at.x + size.x + pad * 0.6f, at.y + size.y + pad * 0.5f), Colour(background, 1.0f), fontSize * 0.15f);
			drawList->AddText(smallFont, smallSize, ImVec2(at.x, at.y + pad * 0.25f), Colour(skyUI ? palette.text : palette.accent, 0.9f), a_moreText.c_str());
		}
		return { ImVec2(left, top), ImVec2(left + width, top + stackHeight) };
	}
}
