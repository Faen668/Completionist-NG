#include "UI/GameUI.hpp"
#include "Internal Utility/Forms.hpp"
#include "UI/Core/CompletionistUI.hpp"
#include "UI/Core/SweepEffect.hpp"
#include "UI/Core/Themes.hpp"
#include "UI/GameDataProvider.hpp"
#include "Frameworks/FrameworkMaster.hpp"
#include "UI/KeyNames.hpp"
#include "Internal Utility/Variables.hpp"

#include <ScreenGrab.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <wincodec.h>
#include <wrl/client.h>

namespace CompletionistUI
{
	namespace
	{
		using Microsoft::WRL::ComPtr;

		constexpr std::string_view menuName = GameUI::menuName;
		constexpr std::uint32_t    escapeKey = 1;	// DIK_ESCAPE
		constexpr std::uint32_t    gamepadB = 0x2000;

		constexpr std::int32_t captureIdle = -2;
		constexpr std::int32_t captureWaiting = -1;
		constexpr std::int32_t captureCancelled = -3;
		constexpr std::int32_t captureUnbind = -4;
		constexpr std::uint32_t deleteKey = 0xD3;
		constexpr std::uint32_t printScreenKey = 0xB7;	// DIK_SYSRQ

		struct State
		{
			// Render thread
			bool                                  deviceReady{ false };
			UISettings                            settings;
			Menu                                  menu;
			float                                 appliedScale{ 0.0f };
			Theme                                 appliedTheme{ Theme::kCompletionist };
			std::uint32_t                         namedKey{ 0 };
			ID3D11Device*                         device{ nullptr };
			ID3D11DeviceContext*                  context{ nullptr };
			IDXGISwapChain*                       swapChain{ nullptr };
			std::chrono::steady_clock::time_point lastFrame{};

			// Game thread
			ImVec2        mouse{ 0.0f, 0.0f };
			bool          mousePlaced{ false };
			std::uint32_t padHeld{ 0 };
			std::uint32_t padIgnored{ 0 };	// held as the window opened, ignored until let go
			std::uint32_t padHeldBack{ 0 };	// a combo's hold button, passed on when let go unless the combo used it

			// Shared
			GameDataProvider                               provider;
			std::atomic<bool>                              open{ false };
			std::atomic<bool>                              justOpened{ false };
			std::atomic<std::uint32_t>                     openKey{ 199 };
			std::atomic<std::int32_t>                      capture{ captureIdle };
			std::atomic<std::uint32_t>                     openPadButton{ 0 };
			std::atomic<std::uint32_t>                     openPadModifier{ 0 };
			std::atomic<std::int64_t>                      padCapture{ captureIdle };
			std::atomic<std::uint32_t>                     padRejected{ 0 };
			std::uint32_t                                  padPending{ 0 };
			std::atomic<std::uint32_t>                     cellKey{ 0 };
			std::atomic<std::uint32_t>                     cellPadButton{ 0 };
			std::atomic<std::uint32_t>                     cellPadModifier{ 0 };
			std::atomic<std::uint32_t>                     nearestKey{ 0 };
			std::atomic<std::uint32_t>                     nearestPadButton{ 0 };
			std::atomic<std::uint32_t>                     nearestPadModifier{ 0 };
			std::atomic<bool>                              sweepEffect{ true };
			std::atomic<bool>                              seekersWhisper{ false };
			std::atomic<bool>                              sweepActive{ false };
			std::atomic<bool>                              sweepFollowQueued{ false };
			std::vector<std::pair<RE::FormID, RE::FormID>> sweepFinds;	// game thread: what's in sweepWave.items
			std::chrono::steady_clock::time_point          sweepChecked{};	// game thread
			std::atomic<bool>                              shoutWaiting{ false };	// a new game's player has no cell yet
			std::mutex                                     sweepLock;
			SweepEffect::Wave                              sweepWave;
			std::chrono::steady_clock::time_point          sweepStart{};
			SweepEffect                                    sweep;
			std::uint32_t                                  namedNearestKey{ 0xFFFFFFFF };
			std::atomic<bool>                              cellPanel{ false };
			Toasts                                         toasts;
			std::atomic<bool>                              journalEntry{ true };
			double                                         toastClock{ 0.0 };
			std::atomic<bool>                              cellPanelToggle{ false };	// the key was pressed, for the render thread
			std::uint32_t                                  namedCellKey{ 0xFFFFFFFF };
			std::atomic<bool>                              usingGamepad{ false };
			std::atomic<bool>                              printScreen{ false };
			std::atomic<float>                             displayWidth{ 1920.0f };
			std::atomic<float>                             displayHeight{ 1080.0f };
			std::mutex                                     inputLock;
			std::vector<std::function<void(ImGuiIO&)>>     inputQueue;
			std::mutex                                     saveLock;
			std::optional<UISettings>                      pendingSave;
		};

		//---------------------------------------------------
		//-- Game UI ( Get State ) --------------------------
		//---------------------------------------------------

		State& GetState() {

			static State state;
			return state;
		}

		//---------------------------------------------------
		//-- Game UI ( Queue Input ) ------------------------
		//---------------------------------------------------

		void QueueInput(std::function<void(ImGuiIO&)> a_event) {

			auto&            state = GetState();
			std::scoped_lock lock(state.inputLock);
			state.inputQueue.push_back(std::move(a_event));
		}

		//---------------------------------------------------
		//-- Game UI ( Fonts For ) --------------------------
		//---------------------------------------------------

		FontPaths FontsFor(Theme a_theme) {

			const std::string folder = "Data/SKSE/Plugins/CompletionistData/Fonts/";
			if (a_theme == Theme::kCompletionist) {
				return { folder + "AlegreyaSans-Regular.ttf", {}, folder + "Cinzel.ttf", {}, folder + "EBGaramond.ttf" };
			}
			// Summerset serif, the title in Cinzel capitals
			if (a_theme == Theme::kAltmer) {
				return { folder + "EBGaramond.ttf", {}, {}, folder + "Cinzel.ttf", folder + "AlegreyaSans-Regular.ttf" };
			}
			// A mapmaker's book hand and his lettered capitals
			if (a_theme == Theme::kOblivion) {
				return { folder + "EBGaramond.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "AlegreyaSans-Regular.ttf", 1.7f, 36.0f };
			}
			// A plain sans, drawn heavy, for a modern game menu
			if (a_theme == Theme::kAnime) {
				return { folder + "AlegreyaSans-Regular.ttf", {}, {}, {}, folder + "EBGaramond.ttf", 1.5f, 40.0f };
			}
			// The Black Books' hand: the same old serif, its capitals pressed in harder
			if (a_theme == Theme::kApocrypha) {
				return { folder + "EBGaramond.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "AlegreyaSans-Regular.ttf", 1.4f, 38.0f };
			}
			// A library's book hand under its carved capitals
			if (a_theme == Theme::kGothic) {
				return { folder + "EBGaramond.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "AlegreyaSans-Regular.ttf", 1.35f, 36.0f };
			}
			// Carved capitals over the Dwemer's doors, a plain face for the text
			if (a_theme == Theme::kDwemer) {
				return { folder + "AlegreyaSans-Regular.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "EBGaramond.ttf" };
			}
			// Roman capitals, as cut over the Imperial City's gates
			if (a_theme == Theme::kImperial) {
				return { folder + "AlegreyaSans-Regular.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "EBGaramond.ttf" };
			}
			// A manor's book hand for the headings
			if (a_theme == Theme::kBreton) {
				return { folder + "AlegreyaSans-Regular.ttf", {}, folder + "EBGaramond.ttf", folder + "EBGaramond.ttf", folder + "Cinzel.ttf" };
			}
			// Nordic UI's carved capitals for the headings, a book serif for the text
			if (a_theme == Theme::kNordic) {
				return { folder + "EBGaramond.ttf", {}, folder + "Cinzel.ttf", folder + "Cinzel.ttf", folder + "AlegreyaSans-Regular.ttf" };
			}
			return { folder + "AlegreyaSans-Regular.ttf", {}, {}, {}, folder + "EBGaramond.ttf" };
		}

		//---------------------------------------------------
		//-- Game UI ( Key From Scan Code ) -----------------
		//---------------------------------------------------

		ImGuiKey KeyFromScanCode(std::uint32_t a_code) {

			switch (a_code) {
			case 0x0E: return ImGuiKey_Backspace;
			case 0x0F: return ImGuiKey_Tab;
			case 0x1C: return ImGuiKey_Enter;
			case 0x9C: return ImGuiKey_KeypadEnter;
			case 0x39: return ImGuiKey_Space;
			case 0xC8: return ImGuiKey_UpArrow;
			case 0xD0: return ImGuiKey_DownArrow;
			case 0xCB: return ImGuiKey_LeftArrow;
			case 0xCD: return ImGuiKey_RightArrow;
			case 0xC7: return ImGuiKey_Home;
			case 0xCF: return ImGuiKey_End;
			case 0xC9: return ImGuiKey_PageUp;
			case 0xD1: return ImGuiKey_PageDown;
			case 0xD3: return ImGuiKey_Delete;
			case 0xD2: return ImGuiKey_Insert;
			case 0x2A: return ImGuiKey_LeftShift;
			case 0x36: return ImGuiKey_RightShift;
			case 0x1D: return ImGuiKey_LeftCtrl;
			case 0x9D: return ImGuiKey_RightCtrl;
			case 0x38: return ImGuiKey_LeftAlt;
			case 0xB8: return ImGuiKey_RightAlt;
			case 0x1E: return ImGuiKey_A;
			case 0x2E: return ImGuiKey_C;
			case 0x2F: return ImGuiKey_V;
			case 0x2D: return ImGuiKey_X;
			case 0x2C: return ImGuiKey_Z;
			case 0x15: return ImGuiKey_Y;
			default: return ImGuiKey_None;
			}
		}

		// XInput masks, triggers are 9 and 10
		constexpr std::uint32_t leftTrigger = 0x0009;
		constexpr std::uint32_t rightTrigger = 0x000A;

		//---------------------------------------------------
		//-- Game UI ( Key From Gamepad ) -------------------
		//---------------------------------------------------

		ImGuiKey KeyFromGamepad(std::uint32_t a_code) {

			switch (a_code) {
			case 0x0001: return ImGuiKey_GamepadDpadUp;
			case 0x0002: return ImGuiKey_GamepadDpadDown;
			case 0x0004: return ImGuiKey_GamepadDpadLeft;
			case 0x0008: return ImGuiKey_GamepadDpadRight;
			case 0x0010: return ImGuiKey_GamepadStart;
			case 0x0020: return ImGuiKey_GamepadBack;
			case 0x0040: return ImGuiKey_GamepadL3;
			case 0x0080: return ImGuiKey_GamepadR3;
			case 0x0100: return ImGuiKey_GamepadL1;
			case 0x0200: return ImGuiKey_GamepadR1;
			case 0x1000: return ImGuiKey_GamepadFaceDown;
			case 0x2000: return ImGuiKey_GamepadFaceRight;
			case 0x4000: return ImGuiKey_F24;	// X, row action
			case 0x8000: return ImGuiKey_GamepadFaceUp;
			case leftTrigger: return ImGuiKey_GamepadL2;
			case rightTrigger: return ImGuiKey_GamepadR2;
			default: return ImGuiKey_None;
			}
		}

		bool IsButtonMask(std::uint32_t a_code) { return a_code != 0 && (a_code & (a_code - 1)) == 0; }

		//---------------------------------------------------
		//-- Game UI ( Modifier For ) -----------------------
		//---------------------------------------------------

		ImGuiKey ModifierFor(ImGuiKey a_key) {

			switch (a_key) {
			case ImGuiKey_LeftShift:
			case ImGuiKey_RightShift:
				return ImGuiMod_Shift;
			case ImGuiKey_LeftCtrl:
			case ImGuiKey_RightCtrl:
				return ImGuiMod_Ctrl;
			case ImGuiKey_LeftAlt:
			case ImGuiKey_RightAlt:
				return ImGuiMod_Alt;
			default:
				return ImGuiKey_None;
			}
		}

		//---------------------------------------------------
		//-- Game UI ( Request Close ) ----------------------
		//---------------------------------------------------

		void RequestClose() {

			SKSE::GetTaskInterface()->AddUITask([]() { GameUI::Close(); });
		}

		//---------------------------------------------------
		//-- Game UI ( Mark Nearest ) -----------------------
		//---------------------------------------------------

		// Game thread. a_shoutWords is how many words of the shout did it, none for the key
		void MarkNearest(int a_shoutWords = 0) {

			auto& state = GetState();
			const auto finds = state.provider.MarkNearestCellItem(a_shoutWords > 0);
			if (finds.cleared) {
				state.sweepActive = false;
				return;
			}
			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player || (a_shoutWords == 0 && !state.sweepEffect)) {	// the shout always sweeps
				return;
			}

			const auto from = player->GetPosition();
			SweepEffect::Wave wave;
			wave.origin = { from.x, from.y, from.z + 20.0f };	// about the ankles, so it spreads over the ground
			wave.firstMarked = finds.marked;
			const float nearest = finds.places.empty() ? 0.0f : finds.places.front().GetDistance(from);
			if (a_shoutWords > 0) {
				// About a room, a cave, then as far as the key goes. Always far enough to light up the find
				constexpr float reach[3] = { 8000.0f, 16000.0f, 40000.0f };
				wave.reach = std::max(reach[std::clamp(a_shoutWords, 1, 3) - 1], nearest * 1.15f);
			} else {
				wave.reach = SweepEffect::ReachFor(nearest);
			}
			const auto count = std::min<std::size_t>(finds.places.size(), SweepEffect::maxItems);
			wave.items.reserve(count);
			for (std::size_t i = 0; i < count; ++i) {
				const auto& place = finds.places[i];
				wave.items.push_back({ place.x, place.y, place.z });
			}
			state.sweepFinds.assign(finds.ids.begin(), finds.ids.begin() + static_cast<std::ptrdiff_t>(count));
			state.sweepChecked = std::chrono::steady_clock::now();

			std::scoped_lock lock(state.sweepLock);
			state.sweepWave = std::move(wave);
			state.sweepStart = std::chrono::steady_clock::now();
			state.sweepActive = true;
		}

		//---------------------------------------------------
		//-- Game UI ( Follow Sweep Finds ) -----------------
		//---------------------------------------------------

		// Game thread. Keeps the gold on the things themselves: it moves with them and goes when they're collected
		void FollowSweepFinds() {

			auto& state = GetState();
			state.sweepFollowQueued = false;
			if (!state.sweepActive || state.sweepFinds.empty()) {
				return;
			}

			const auto now = std::chrono::steady_clock::now();
			const bool rescan = now - state.sweepChecked >= std::chrono::seconds(1);
			if (rescan) {
				state.sweepChecked = now;
			}
			const auto places = state.provider.WhereFindsAre(state.sweepFinds, rescan);

			std::vector<std::pair<RE::FormID, RE::FormID>> finds;
			std::vector<SweepEffect::Point>                items;
			for (std::size_t i = 0; i < places.size(); ++i) {
				if (places[i]) {
					finds.push_back(state.sweepFinds[i]);
					items.push_back({ places[i]->x, places[i]->y, places[i]->z });
				}
			}

			std::scoped_lock lock(state.sweepLock);
			if (!places.front()) {
				state.sweepWave.firstMarked = false;	// the brighter one was the marked find
			}
			state.sweepFinds = std::move(finds);
			state.sweepWave.items = std::move(items);
		}

		//---------------------------------------------------
		//-- Shout ( Shake ) --------------------------------
		//---------------------------------------------------

		// The game's own camera shake, only just there as it's a whisper
		void ShoutShake(int a_words) {

			auto* vm = CForms::VM();
			if (!vm) {
				return;
			}
			const float strength = 0.025f + a_words * 0.015f;
			const float seconds = 0.3f + a_words * 0.1f;

			RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> none;
			auto* camera = RE::MakeFunctionArguments(static_cast<RE::TESObjectREFR*>(nullptr), static_cast<float>(strength), static_cast<float>(seconds));
			vm->DispatchStaticCall("Game", "ShakeCamera", camera, none);
			delete camera;
		}

		//---------------------------------------------------
		//-- Shout ( Give Or Take ) -------------------------
		//---------------------------------------------------

		// Game thread. All three words, free of dragon souls, while the setting is on
		void SyncShout() {

			auto& state = GetState();
			auto* player = RE::PlayerCharacter::GetSingleton();
			auto* shout = CForms::Get<RE::TESShout>(CForms::SeekersWhisper);
			auto* vm = CForms::VM();
			if (!player || !shout || !vm) {
				return;
			}
			if (!player->GetParentCell()) {
				state.shoutWaiting = true;
				return;
			}

			// The game holds these for this session only, and the translation can change them
			const auto name = state.provider.Translate("UI_Shout_Name");
			shout->fullName = name.c_str();
			for (const auto id : CForms::SeekersWhisperSpells) {
				if (auto* spell = CForms::Get<RE::SpellItem>(id)) {
					spell->fullName = name.c_str();
				}
			}
			if (auto* effect = CForms::Get<RE::EffectSetting>(CForms::SeekersWhisperEffect)) {
				effect->fullName = name.c_str();
				effect->magicItemDescription = state.provider.Translate("UI_Shout_Description").c_str();
			}

			RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> none;
			const bool has = player->HasShout(shout);
			if (state.seekersWhisper && !has) {
				player->AddShout(shout);
				for (const auto id : CForms::SeekersWhisperWords) {
					auto* word = CForms::Get<RE::TESWordOfPower>(id);
					if (!word) {
						continue;
					}
					auto* teach = RE::MakeFunctionArguments(static_cast<RE::TESWordOfPower*>(word));
					vm->DispatchStaticCall("Game", "TeachWord", teach, none);
					delete teach;
					auto* unlock = RE::MakeFunctionArguments(static_cast<RE::TESWordOfPower*>(word));
					vm->DispatchStaticCall("Game", "UnlockWord", unlock, none);
					delete unlock;
				}
			} else if (!state.seekersWhisper && has) {
				auto* unequip = RE::MakeFunctionArguments(static_cast<RE::TESShout*>(shout));
				vm->DispatchMethodCall(CForms::HandleFor(player), "Actor", "UnequipShout", unequip, none);
				delete unequip;
				auto* remove = RE::MakeFunctionArguments(static_cast<RE::TESShout*>(shout));
				vm->DispatchMethodCall(CForms::HandleFor(player), "Actor", "RemoveShout", remove, none);
				delete remove;
			}
		}

		//---------------------------------------------------
		//-- Shout ( Heard ) --------------------------------
		//---------------------------------------------------

		class ShoutListener final :
			public RE::BSTEventSink<RE::TESSpellCastEvent>,
			public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>
		{
		public:
			static ShoutListener* Get() {

				static ShoutListener listener;
				return &listener;
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::TESCellFullyLoadedEvent*, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) override {

				if (GetState().shoutWaiting.exchange(false)) {
					SKSE::GetTaskInterface()->AddTask([]() { SyncShout(); });
				}
				return RE::BSEventNotifyControl::kContinue;
			}

			RE::BSEventNotifyControl ProcessEvent(const RE::TESSpellCastEvent* a_event, RE::BSTEventSource<RE::TESSpellCastEvent>*) override {

				if (!a_event || !a_event->spell || a_event->object.get() != RE::PlayerCharacter::GetSingleton()) {
					return RE::BSEventNotifyControl::kContinue;
				}
				for (int i = 0; i < 3; ++i) {
					const auto* spell = CForms::Get<RE::SpellItem>(CForms::SeekersWhisperSpells[i]);
					if (spell && spell->GetFormID() == a_event->spell) {
						const int words = i + 1;
						SKSE::GetTaskInterface()->AddTask([words]() {
							ShoutShake(words);
							MarkNearest(words);
						});
						break;
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};

		//---------------------------------------------------
		//-- Menu ( The window as a game menu ) -----------
		//---------------------------------------------------

		class CompletionistMenu final : public RE::IMenu
		{
		public:
			CompletionistMenu() {

				menuFlags.set(Flag::kUsesMenuContext, Flag::kModal, Flag::kDisablePauseMenu);
				// Made on each open, so a change to the setting applies the next time
				if (GetState().settings.pauseGame) {
					menuFlags.set(Flag::kPausesGame);
				}
				inputContext = Context::kMenuMode;
				depthPriority = 10;
			}

			static RE::IMenu* Create() { return new CompletionistMenu(); }

			RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override {

				switch (a_message.type.get()) {
				case RE::UI_MESSAGE_TYPE::kShow:
					GameUI::SetOpen(true);
					return RE::UI_MESSAGE_RESULTS::kHandled;
				case RE::UI_MESSAGE_TYPE::kHide:
				case RE::UI_MESSAGE_TYPE::kForceHide:
					GameUI::SetOpen(false);
					return RE::UI_MESSAGE_RESULTS::kHandled;
				default:
					return RE::IMenu::ProcessMessage(a_message);
				}
			}
		};

		//---------------------------------------------------
		//-- Input ( Game thread ) --------------------------
		//---------------------------------------------------

		class InputHandler final : public RE::BSTEventSink<RE::InputEvent*>
		{
		public:
			static InputHandler* GetSingleton() {

				static InputHandler singleton;
				return &singleton;
			}

			RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override {

				if (!a_event) {
					return RE::BSEventNotifyControl::kContinue;
				}

				auto& state = GetState();
				for (auto* event = *a_event; event; event = event->next) {
					if (auto* button = event->AsButtonEvent()) {
						HandleButton(state, *button);
					} else if (!state.open) {
						continue;
					} else if (auto* character = event->AsCharEvent()) {
						const auto code = character->keyCode;
						QueueInput([code](ImGuiIO& a_io) { a_io.AddInputCharacter(code); });
					} else if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kMouseMove) {
						state.usingGamepad = false;
						const auto* move = static_cast<RE::MouseMoveEvent*>(event);
						state.mouse.x = std::clamp(state.mouse.x + static_cast<float>(move->mouseInputX), 0.0f, state.displayWidth.load());
						state.mouse.y = std::clamp(state.mouse.y + static_cast<float>(move->mouseInputY), 0.0f, state.displayHeight.load());
						const auto mouse = state.mouse;
						QueueInput([mouse](ImGuiIO& a_io) { a_io.AddMousePosEvent(mouse.x, mouse.y); });
					} else if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kThumbstick) {
						if (state.padCapture == captureWaiting) {
							continue;
						}
						const auto* stick = static_cast<RE::ThumbstickEvent*>(event);
						const float x = stick->xValue;
						const float y = stick->yValue;
						if ((std::abs(x) > 0.3f || std::abs(y) > 0.3f) && !state.usingGamepad.exchange(true)) {
							INFO("In-game UI: controller in use, {} stick {:.2f}, {:.2f}; the mouse cursor hides until the mouse moves", stick->IsLeft() ? "left" : "right", x, y);
						}
						// Left stick as d-pad, right stick scrolls
						const bool left = stick->IsLeft();
						QueueInput([x, y, left](ImGuiIO& a_io) {
							a_io.AddKeyAnalogEvent(left ? ImGuiKey_GamepadLStickLeft : ImGuiKey_GamepadRStickLeft, x < -0.3f, std::max(0.0f, -x));
							a_io.AddKeyAnalogEvent(left ? ImGuiKey_GamepadLStickRight : ImGuiKey_GamepadRStickRight, x > 0.3f, std::max(0.0f, x));
							a_io.AddKeyAnalogEvent(left ? ImGuiKey_GamepadLStickUp : ImGuiKey_GamepadRStickUp, y > 0.3f, std::max(0.0f, y));
							a_io.AddKeyAnalogEvent(left ? ImGuiKey_GamepadLStickDown : ImGuiKey_GamepadRStickDown, y < -0.3f, std::max(0.0f, -y));
						});
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			static void HandleButton(State& a_state, const RE::ButtonEvent& a_button) {

				const auto device = a_button.GetDevice();
				const auto code = a_button.GetIDCode();
				const bool down = a_button.IsPressed();

				const std::uint32_t heldBefore = a_state.padHeld;
				if (device == RE::INPUT_DEVICE::kGamepad && IsButtonMask(code)) {
					a_state.padHeld = down ? (a_state.padHeld | code) : (a_state.padHeld & ~code);
				}
				// Only a real press counts: a pad, or a virtual one like Steam's, can send idle or resting
				// trigger events, which hid the mouse cursor for good
				const bool trigger = code == leftTrigger || code == rightTrigger;
				if (device == RE::INPUT_DEVICE::kGamepad && (trigger ? a_button.Value() > 0.3f : a_button.IsDown())) {
					if (!a_state.usingGamepad.exchange(true) && a_state.open) {
						INFO("In-game UI: controller in use, button {:X} value {:.2f}; the mouse cursor hides until the mouse moves", code, a_button.Value());
					}
				} else if (device == RE::INPUT_DEVICE::kMouse) {
					a_state.usingGamepad = false;
				}

				// Print Screen
				if (device == RE::INPUT_DEVICE::kKeyboard && code == printScreenKey && a_button.IsDown()) {
					a_state.printScreen = true;
				}

				// Open key binding
				if (a_state.open && a_state.capture == captureWaiting && device == RE::INPUT_DEVICE::kKeyboard && a_button.IsDown()) {
					a_state.capture = code == escapeKey ? captureCancelled : static_cast<std::int32_t>(code);
					return;
				}

				// Controller combo binding
				if (a_state.open && a_state.padCapture == captureWaiting) {
					if (device == RE::INPUT_DEVICE::kKeyboard && a_button.IsDown()) {
						if (code == escapeKey) {
							a_state.padCapture = captureCancelled;
						} else if (code == deleteKey) {
							a_state.padCapture = captureUnbind;
						}
						return;
					}
					if (device != RE::INPUT_DEVICE::kGamepad) {
						return;
					}
					if (IsButtonMask(code)) {
						if (a_button.IsDown()) {
							if (a_state.padPending == 0) {
								a_state.padPending = code;
							} else if (code != a_state.padPending) {
								a_state.padCapture = (static_cast<std::int64_t>(a_state.padPending) << 16) | code;
								a_state.padPending = 0;
							}
						} else if (!a_button.IsPressed() && code == a_state.padPending) {
							a_state.padPending = 0;
							if (code == gamepadB) {
								a_state.padCapture = captureCancelled;
							} else if (IsSafePadBinding(0, code)) {
								a_state.padCapture = code;
							} else {
								a_state.padRejected = code;
							}
						}
					}
					return;
				}
				a_state.padPending = 0;

				const bool keyToggle = device == RE::INPUT_DEVICE::kKeyboard && a_state.openKey != 0 && code == a_state.openKey;
				const auto modifier = a_state.openPadModifier.load();
				const bool padToggle = device == RE::INPUT_DEVICE::kGamepad && a_state.openPadButton != 0 && code == a_state.openPadButton && (modifier == 0 || (heldBefore & modifier) != 0);
				if ((keyToggle || padToggle) && a_button.IsDown()) {
					a_state.padHeldBack = 0;
					if (a_state.open) {
						GameUI::Close();
					} else if (GameUI::CanOpen()) {
						a_state.padIgnored = a_state.padHeld;
						GameUI::Open();
					}
					return;
				}

				// Cell Scanner overlay key and combo, while playing
				const auto cellModifier = a_state.cellPadModifier.load();
				const bool cellKeyToggle = device == RE::INPUT_DEVICE::kKeyboard && a_state.cellKey != 0 && code == a_state.cellKey;
				const bool cellPadToggle = device == RE::INPUT_DEVICE::kGamepad && a_state.cellPadButton != 0 && code == a_state.cellPadButton && (cellModifier == 0 || (heldBefore & cellModifier) != 0);
				if ((cellKeyToggle || cellPadToggle) && a_button.IsDown() && !a_state.open && GameUI::OverlayAllowed()) {
					a_state.padHeldBack = 0;
					a_state.cellPanelToggle = true;
					return;
				}

				// Nearest find key and combo, while playing
				const auto nearestModifier = a_state.nearestPadModifier.load();
				const bool nearestKeyPress = device == RE::INPUT_DEVICE::kKeyboard && a_state.nearestKey != 0 && code == a_state.nearestKey;
				const bool nearestPadPress = device == RE::INPUT_DEVICE::kGamepad && a_state.nearestPadButton != 0 && code == a_state.nearestPadButton && (nearestModifier == 0 || (heldBefore & nearestModifier) != 0);
				if ((nearestKeyPress || nearestPadPress) && a_button.IsDown() && !a_state.open && GameUI::OverlayAllowed()) {
					a_state.padHeldBack = 0;
					SKSE::GetTaskInterface()->AddTask([]() { MarkNearest(); });
					return;
				}

				if (!a_state.open) {
					return;
				}

				switch (device) {
				case RE::INPUT_DEVICE::kKeyboard:
					if (code == escapeKey) {
						QueueInput([down](ImGuiIO& a_io) { a_io.AddKeyEvent(ImGuiKey_Escape, down); });
					} else if (const auto key = KeyFromScanCode(code); key != ImGuiKey_None) {
						QueueInput([key, down](ImGuiIO& a_io) {
							a_io.AddKeyEvent(key, down);
							if (const auto modifier = ModifierFor(key); modifier != ImGuiKey_None) {
								a_io.AddKeyEvent(modifier, down);
							}
						});
					}
					break;
				case RE::INPUT_DEVICE::kMouse:
					if (code <= 2) {
						QueueInput([code, down](ImGuiIO& a_io) { a_io.AddMouseButtonEvent(static_cast<int>(code), down); });
					} else if ((code == 8 || code == 9) && a_button.IsDown()) {
						const float wheel = code == 8 ? 1.0f : -1.0f;
						QueueInput([wheel](ImGuiIO& a_io) { a_io.AddMouseWheelEvent(0.0f, wheel); });
					}
					break;
				case RE::INPUT_DEVICE::kGamepad:
					// The open combo's buttons would land as fresh presses (L3 folds the sidebar)
					if (a_state.padIgnored & code) {
						if (!down) {
							a_state.padIgnored &= ~code;
						}
						break;
					}
					// Holding a combo's first button (L3 of L3 + R3) mustn't fold the sidebar on the way to closing:
					// it's held back, and only a tap on its own reaches the window
					if (a_state.padHeldBack & code) {
						if (!down) {
							a_state.padHeldBack &= ~code;
							if (const auto key = KeyFromGamepad(code); key != ImGuiKey_None) {
								QueueInput([key](ImGuiIO& a_io) {
									a_io.AddKeyEvent(key, true);
									a_io.AddKeyEvent(key, false);
								});
							}
						}
						break;
					}
					if (a_button.IsDown()) {
						if (code == a_state.openPadModifier.load() || code == a_state.cellPadModifier.load()) {
							a_state.padHeldBack |= code;
							break;
						}
						// Another button with it held: not a tap, so it's dropped
						a_state.padHeldBack = 0;
					}
					// B as back
					if (const auto key = KeyFromGamepad(code); key != ImGuiKey_None) {
						const float value = code == leftTrigger || code == rightTrigger ? a_button.Value() : (down ? 1.0f : 0.0f);
						QueueInput([key, down, value](ImGuiIO& a_io) { a_io.AddKeyAnalogEvent(key, down, value); });
					}
					break;
				default:
					break;
				}
			}
		};

		//---------------------------------------------------
		//-- Screenshots ( Testing aid ) --------------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Game UI ( Save Screenshot ) --------------------
		//---------------------------------------------------

		std::optional<std::filesystem::path> SaveScreenshot(State& a_state, ID3D11Texture2D* a_backBuffer) {

			auto directory = SKSE::log::log_directory();
			if (!directory) {
				return std::nullopt;
			}
			*directory /= "Completionist/Screenshots";

			std::error_code error;
			std::filesystem::create_directories(*directory, error);

			const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm    local{};
			localtime_s(&local, &time);
			char name[64];
			std::strftime(name, sizeof(name), "Completionist_%Y%m%d_%H%M%S.png", &local);
			const auto path = *directory / name;

			const HRESULT com = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			// forceSRGB or viewers wash it out
			const HRESULT result = DirectX::SaveWICTextureToFile(a_state.context, a_backBuffer, GUID_ContainerFormatPng, path.c_str(), &GUID_WICPixelFormat24bppBGR, nullptr, true);
			if (SUCCEEDED(com)) {
				CoUninitialize();
			}

			if (FAILED(result)) {
				INFO("In-game UI screenshot failed: {:08X}", static_cast<std::uint32_t>(result));
				return std::nullopt;
			}

			INFO("In-game UI screenshot saved: {}", path.string());
			return path;
		}

		//---------------------------------------------------
		//-- Game UI ( Copy Screenshot To Clipboard ) -------
		//---------------------------------------------------

		bool CopyScreenshotToClipboard(State& a_state, ID3D11Texture2D* a_backBuffer) {

			D3D11_TEXTURE2D_DESC desc{};
			a_backBuffer->GetDesc(&desc);
			const bool rgba = desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM || desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			const bool bgra = desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM || desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB || desc.Format == DXGI_FORMAT_B8G8R8X8_UNORM;
			if ((!rgba && !bgra) || desc.SampleDesc.Count != 1) {
				return false;
			}

			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			ComPtr<ID3D11Texture2D> staging;
			if (FAILED(a_state.device->CreateTexture2D(&desc, nullptr, &staging))) {
				return false;
			}
			a_state.context->CopyResource(staging.Get(), a_backBuffer);
			D3D11_MAPPED_SUBRESOURCE mapped{};
			if (FAILED(a_state.context->Map(staging.Get(), 0, D3D11_MAP_READ, 0, &mapped))) {
				return false;
			}

			// Bottom-up 32-bit DIB
			const std::size_t rowBytes = static_cast<std::size_t>(desc.Width) * 4;
			HGLOBAL           memory = GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + rowBytes * desc.Height);
			auto*             header = memory ? static_cast<BITMAPINFOHEADER*>(GlobalLock(memory)) : nullptr;
			if (header) {
				*header = {};
				header->biSize = sizeof(BITMAPINFOHEADER);
				header->biWidth = static_cast<LONG>(desc.Width);
				header->biHeight = static_cast<LONG>(desc.Height);
				header->biPlanes = 1;
				header->biBitCount = 32;
				header->biCompression = BI_RGB;
				auto* pixels = reinterpret_cast<std::uint8_t*>(header + 1);
				for (UINT y = 0; y < desc.Height; ++y) {
					const auto* source = static_cast<const std::uint8_t*>(mapped.pData) + static_cast<std::size_t>(y) * mapped.RowPitch;
					auto*       target = pixels + static_cast<std::size_t>(desc.Height - 1 - y) * rowBytes;
					for (UINT x = 0; x < desc.Width; ++x) {
						target[x * 4 + 0] = source[x * 4 + (rgba ? 2 : 0)];
						target[x * 4 + 1] = source[x * 4 + 1];
						target[x * 4 + 2] = source[x * 4 + (rgba ? 0 : 2)];
						target[x * 4 + 3] = 255;
					}
				}
				GlobalUnlock(memory);
			}
			a_state.context->Unmap(staging.Get(), 0);

			if (!header || !OpenClipboard(nullptr)) {
				if (memory) {
					GlobalFree(memory);
				}
				return false;
			}
			EmptyClipboard();
			const bool copied = SetClipboardData(CF_DIB, memory) != nullptr;
			CloseClipboard();
			if (!copied) {
				GlobalFree(memory);
			}
			return copied;
		}

		//---------------------------------------------------
		//-- Rendering ( Render thread ) --------------------
		//---------------------------------------------------

		//---------------------------------------------------
		//-- Game UI ( Initialise Device ) ------------------
		//---------------------------------------------------

		bool InitialiseDevice(State& a_state) {

			const auto* data = RE::BSGraphics::Renderer::GetRendererDataSingleton();
			if (!data || !data->forwarder || !data->context || !data->renderWindows[0].swapChain) {
				return false;
			}

			a_state.device = reinterpret_cast<ID3D11Device*>(data->forwarder);
			a_state.context = reinterpret_cast<ID3D11DeviceContext*>(data->context);
			a_state.swapChain = reinterpret_cast<IDXGISwapChain*>(data->renderWindows[0].swapChain);
			if (!ImGui_ImplDX11_Init(a_state.device, a_state.context)) {
				return false;
			}

			INFO("In-game UI renderer initialised");
			a_state.deviceReady = true;
			return true;
		}

		//---------------------------------------------------
		//-- Game UI ( Apply Settings Updates ) -------------
		//---------------------------------------------------

		void ApplySettingsUpdates(State& a_state) {

			if (a_state.settings.capturingOpenKey) {
				auto expected = captureIdle;
				a_state.capture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.capture.load();
				if (result >= 0 || result == captureCancelled) {
					if (result >= 0) {
						const auto key = static_cast<std::uint32_t>(result) == deleteKey ? 0 : static_cast<std::uint32_t>(result);
						if (key == 0 && a_state.settings.openPadButton == 0) {
							a_state.menu.ShowStatus(a_state.provider.Translate("UI_Setting_KeepOneOpen"), 6.0f);
						} else {
							a_state.settings.openKey = key;
						}
					}
					a_state.settings.capturingOpenKey = false;
					a_state.capture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}

			// Delete unbinds, Escape cancels
			if (!a_state.settings.captureTarget.empty() && a_state.settings.captureResult == UISettings::captureNone && !a_state.settings.capturingOpenKey) {
				auto expected = captureIdle;
				a_state.capture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.capture.load();
				if (result >= 0) {
					constexpr std::int32_t deleteKey = 0xD3;
					a_state.settings.captureResult = result == deleteKey ? -1 : result;
					a_state.capture = captureIdle;
				} else if (result == captureCancelled) {
					a_state.settings.captureTarget.clear();
					a_state.capture = captureIdle;
				}
			}

			if (a_state.settings.capturingOpenPad) {
				std::int64_t expected = captureIdle;
				a_state.padCapture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.padCapture.load();
				if (result >= 0 || result == captureCancelled || result == captureUnbind) {
					if (result >= 0) {
						a_state.settings.openPadButton = static_cast<std::uint32_t>(result & 0xFFFF);
						a_state.settings.openPadModifier = static_cast<std::uint32_t>(result >> 16);
					} else if (result == captureUnbind && a_state.settings.openKey == 0) {
						a_state.menu.ShowStatus(a_state.provider.Translate("UI_Setting_KeepOneOpen"), 6.0f);
					} else if (result == captureUnbind) {
						a_state.settings.openPadButton = 0;
						a_state.settings.openPadModifier = 0;
					}
					a_state.settings.capturingOpenPad = false;
					a_state.padCapture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}
			a_state.openPadButton = a_state.settings.openPadButton;
			a_state.openPadModifier = a_state.settings.openPadModifier;

			if (const auto rejected = a_state.padRejected.exchange(0); rejected != 0) {
				a_state.menu.ShowStatus(a_state.provider.Translate("UI_Setting_PadSingleUsed{" + GamepadButtonName(rejected, a_state.settings.padStyle) + "}"), 6.0f);
			}

			if (a_state.settings.capturingCellKey) {
				auto expected = captureIdle;
				a_state.capture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.capture.load();
				if (result >= 0 || result == captureCancelled) {
					if (result >= 0) {
						a_state.settings.cellKey = static_cast<std::uint32_t>(result) == deleteKey ? 0 : static_cast<std::uint32_t>(result);
					}
					a_state.settings.capturingCellKey = false;
					a_state.capture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}
			if (a_state.settings.capturingCellPad) {
				std::int64_t expected = captureIdle;
				a_state.padCapture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.padCapture.load();
				if (result >= 0 || result == captureCancelled || result == captureUnbind) {
					if (result >= 0) {
						a_state.settings.cellPadButton = static_cast<std::uint32_t>(result & 0xFFFF);
						a_state.settings.cellPadModifier = static_cast<std::uint32_t>(result >> 16);
					} else if (result == captureUnbind) {
						a_state.settings.cellPadButton = 0;
						a_state.settings.cellPadModifier = 0;
					}
					a_state.settings.capturingCellPad = false;
					a_state.padCapture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}
			if (a_state.settings.capturingNearestKey) {
				auto expected = captureIdle;
				a_state.capture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.capture.load();
				if (result >= 0 || result == captureCancelled) {
					if (result >= 0) {
						a_state.settings.nearestKey = static_cast<std::uint32_t>(result) == deleteKey ? 0 : static_cast<std::uint32_t>(result);
					}
					a_state.settings.capturingNearestKey = false;
					a_state.capture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}
			if (a_state.settings.capturingNearestPad) {
				std::int64_t expected = captureIdle;
				a_state.padCapture.compare_exchange_strong(expected, captureWaiting);

				const auto result = a_state.padCapture.load();
				if (result >= 0 || result == captureCancelled || result == captureUnbind) {
					if (result >= 0) {
						a_state.settings.nearestPadButton = static_cast<std::uint32_t>(result & 0xFFFF);
						a_state.settings.nearestPadModifier = static_cast<std::uint32_t>(result >> 16);
					} else if (result == captureUnbind) {
						a_state.settings.nearestPadButton = 0;
						a_state.settings.nearestPadModifier = 0;
					}
					a_state.settings.capturingNearestPad = false;
					a_state.padCapture = captureIdle;
					std::scoped_lock lock(a_state.saveLock);
					a_state.pendingSave = a_state.settings;
				}
			}
			a_state.cellKey = a_state.settings.cellKey;
			a_state.cellPadButton = a_state.settings.cellPadButton;
			a_state.cellPadModifier = a_state.settings.cellPadModifier;
			a_state.nearestKey = a_state.settings.nearestKey;
			a_state.nearestPadButton = a_state.settings.nearestPadButton;
			a_state.nearestPadModifier = a_state.settings.nearestPadModifier;
			a_state.sweepEffect = a_state.settings.sweepEffect;
			if (a_state.seekersWhisper.exchange(a_state.settings.seekersWhisper) != a_state.settings.seekersWhisper) {
				SKSE::GetTaskInterface()->AddTask([]() { SyncShout(); });
			}
			a_state.cellPanel = a_state.settings.cellPanel;
			a_state.journalEntry = a_state.settings.journalEntry;
			if (a_state.settings.nearestKey != a_state.namedNearestKey) {
				a_state.settings.nearestKeyName = KeyName(a_state.settings.nearestKey);
				a_state.namedNearestKey = a_state.settings.nearestKey;
			}
			if (a_state.settings.cellKey != a_state.namedCellKey) {
				a_state.settings.cellKeyName = KeyName(a_state.settings.cellKey);
				a_state.namedCellKey = a_state.settings.cellKey;
			}

			if (a_state.settings.openKey != a_state.namedKey) {
				a_state.settings.openKeyName = KeyName(a_state.settings.openKey);
				a_state.namedKey = a_state.settings.openKey;
				a_state.openKey = a_state.settings.openKey;
			}

			if (a_state.menu.ConsumeSettingsChanged()) {
				std::scoped_lock lock(a_state.saveLock);
				a_state.pendingSave = a_state.settings;
			}
		}

		//---------------------------------------------------
		//-- Game UI ( Draw Sweep ) -------------------------
		//---------------------------------------------------

		void DrawSweep(State& a_state) {

			SweepEffect::Wave wave;
			{
				std::scoped_lock lock(a_state.sweepLock);
				wave = a_state.sweepWave;
				wave.elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - a_state.sweepStart).count();
			}
			if (wave.elapsed >= SweepEffect::duration || (wave.items.empty() && wave.elapsed >= SweepEffect::waveDuration)) {
				a_state.sweepActive = false;
				return;
			}

			auto* camera = RE::Main::WorldRootCamera();
			auto* renderer = RE::BSGraphics::Renderer::GetSingleton();
			if (!camera || !renderer) {
				a_state.sweepActive = false;
				return;
			}
			// The depth copy the game keeps of the frame's solid geometry
			auto* depth = reinterpret_cast<ID3D11ShaderResourceView*>(renderer->GetDepthStencilData().depthStencils[RE::RENDER_TARGETS_DEPTHSTENCIL::kPOST_ZPREPASS_COPY].depthSRV);

			SweepEffect::View view;
			std::memcpy(view.worldToClip, camera->GetRuntimeData().worldToCam, sizeof(view.worldToClip));
			view.camera = { camera->world.translate.x, camera->world.translate.y, camera->world.translate.z };
			view.nearPlane = camera->GetRuntimeData2().viewFrustum.fNear;
			view.farPlane = camera->GetRuntimeData2().viewFrustum.fFar;

			ComPtr<ID3D11Texture2D>        backBuffer;
			ComPtr<ID3D11RenderTargetView> target;
			if (FAILED(a_state.swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))) || FAILED(a_state.device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target)) ||
				!a_state.sweep.Draw(a_state.device, a_state.context, backBuffer.Get(), target.Get(), depth, view, wave)) {
				INFO("In-game UI: the sweep couldn't be drawn, the marker is still placed");
				a_state.sweepActive = false;
			}
		}

		//---------------------------------------------------
		//-- Game UI ( Render Frame ) -----------------------
		//---------------------------------------------------

		void RenderFrame(State& a_state, bool a_overlay) {

			DXGI_SWAP_CHAIN_DESC desc{};
			if (FAILED(a_state.swapChain->GetDesc(&desc))) {
				return;
			}

			const auto width = static_cast<float>(desc.BufferDesc.Width);
			const auto height = static_cast<float>(desc.BufferDesc.Height);
			a_state.displayWidth = width;
			a_state.displayHeight = height;

			if (!a_overlay && a_state.justOpened.exchange(false)) {
				a_state.menu.Refresh();
				a_state.menu.Opened();
				if (!a_state.settings.openOnLastPage) {
					a_state.menu.OpenPage({});
				}
			}
			const float scale = Menu::ComputeScale(a_state.settings, height);
			if (scale != a_state.appliedScale || a_state.settings.theme != a_state.appliedTheme) {
				Themes::BuildFonts(FontsFor(a_state.settings.theme), scale);
				Themes::Apply(a_state.settings.theme, scale);
				ImGui_ImplDX11_InvalidateDeviceObjects();
				a_state.appliedScale = scale;
				a_state.appliedTheme = a_state.settings.theme;
			}

			const auto now = std::chrono::steady_clock::now();
			auto&      io = ImGui::GetIO();
			io.DisplaySize = ImVec2(width, height);
			io.MouseDrawCursor = !a_overlay && !a_state.usingGamepad;
			io.DeltaTime = std::clamp(std::chrono::duration<float>(now - a_state.lastFrame).count(), 1.0f / 1000.0f, 0.25f);
			a_state.lastFrame = now;

			// What's let go of while the game's out of focus never arrives, Alt after alt-tabbing above all
			// and it would stay held, so ImGui lets go of everything when the game loses focus
			static bool hadFocus = true;
			DWORD       foregroundProcess = 0;
			GetWindowThreadProcessId(GetForegroundWindow(), &foregroundProcess);
			const bool hasFocus = foregroundProcess == GetCurrentProcessId();
			if (hasFocus != hadFocus) {
				hadFocus = hasFocus;
				io.AddFocusEvent(hasFocus);
				INFO("In-game UI: game {} focus, input let go", hasFocus ? "regained" : "lost");
			}

			{
				std::scoped_lock lock(a_state.inputLock);
				if (!a_overlay) {
					for (auto& event : a_state.inputQueue) {
						event(io);
					}
				}
				a_state.inputQueue.clear();
			}

			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();
			if (a_overlay) {
				// Paused while they can't show
				a_state.toastClock += io.DeltaTime;
				ToastOptions options;
				options.x = a_state.settings.toastX;
				options.y = a_state.settings.toastY;
				options.newestFirst = a_state.settings.toastsNewestFirst;
				const auto waiting = a_state.toasts.Waiting();
				// Joined to the panel they sit in its frame; with the panel off, back where they were placed
				if (a_state.cellPanel) {
					const auto join = a_state.settings.toastJoin;
					const auto panel = a_state.menu.DrawCellPanel(a_state.provider, a_state.settings, join, a_state.toasts.StackSize(waiting > 0));
					if (join != ToastJoin::kNone) {
						options.docked = true;
						options.dockAt = panel.dockAt;
						options.dockWidth = panel.dockWidth;
						options.dockUp = panel.dockUp;
					}
				}
				a_state.toasts.Draw(a_state.toastClock, options, waiting > 0 ? a_state.provider.Translate("UI_MoreCount{" + std::to_string(waiting) + "}") : std::string{});
			} else if (!a_state.menu.Draw(a_state.provider, a_state.settings)) {
				RequestClose();
			}
			ImGui::Render();

			ApplySettingsUpdates(a_state);

			ComPtr<ID3D11Texture2D>        backBuffer;
			ComPtr<ID3D11RenderTargetView> target;
			if (FAILED(a_state.swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))) || FAILED(a_state.device->CreateRenderTargetView(backBuffer.Get(), nullptr, &target))) {
				return;
			}

			ComPtr<ID3D11RenderTargetView> previousTarget;
			ComPtr<ID3D11DepthStencilView> previousDepth;
			a_state.context->OMGetRenderTargets(1, &previousTarget, &previousDepth);

			auto* view = target.Get();
			a_state.context->OMSetRenderTargets(1, &view, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			if (a_state.printScreen.exchange(false)) {
				const auto saved = SaveScreenshot(a_state, backBuffer.Get());
				const bool copied = CopyScreenshotToClipboard(a_state, backBuffer.Get());
				auto       message = saved ? a_state.provider.Translate("UI_ScreenshotSaved") + " " + saved->filename().string() : a_state.provider.Translate("UI_ScreenshotFailed");
				if (copied) {
					message += " " + a_state.provider.Translate("UI_ScreenshotCopied");
				}
				if (a_overlay) {
					a_state.toasts.Push({}, message, ToastKind::kInfo);
				} else {
					a_state.menu.ShowStatus(message);
				}
			}

			auto* previous = previousTarget.Get();
			a_state.context->OMSetRenderTargets(1, &previous, previousDepth.Get());
		}
	}

	//---------------------------------------------------
	//-- GameUI ( Registration ) ------------------------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game UI ( Install ) ----------------------------
	//---------------------------------------------------

	void GameUI::Install() {

		auto& state = GetState();
		state.settings = CVariables::VariablesAPI::GetUISettings();
		// Old bindings on a button the window needs go back to default
		if (state.settings.openPadButton != 0 && !IsSafePadBinding(state.settings.openPadModifier, state.settings.openPadButton)) {
			INFO("In-game UI: open buttons {:X}+{:X} clash with the window's own; back to the default", state.settings.openPadModifier, state.settings.openPadButton);
			state.settings.openPadButton = UISettings{}.openPadButton;
			state.settings.openPadModifier = UISettings{}.openPadModifier;
		}
		if (state.settings.cellPadButton != 0 && !IsSafePadBinding(state.settings.cellPadModifier, state.settings.cellPadButton)) {
			INFO("In-game UI: Cell Scanner buttons {:X}+{:X} clash with the window's own; unbound", state.settings.cellPadModifier, state.settings.cellPadButton);
			state.settings.cellPadButton = 0;
			state.settings.cellPadModifier = 0;
		}
		if (state.settings.nearestPadButton != 0 && !IsSafePadBinding(state.settings.nearestPadModifier, state.settings.nearestPadButton)) {
			INFO("In-game UI: nearest find buttons {:X}+{:X} clash with the window's own; unbound", state.settings.nearestPadModifier, state.settings.nearestPadButton);
			state.settings.nearestPadButton = 0;
			state.settings.nearestPadModifier = 0;
		}
		state.openKey = state.settings.openKey;
		state.openPadButton = state.settings.openPadButton;
		state.openPadModifier = state.settings.openPadModifier;
		state.nearestKey = state.settings.nearestKey;
		state.nearestPadButton = state.settings.nearestPadButton;
		state.nearestPadModifier = state.settings.nearestPadModifier;
		state.sweepEffect = state.settings.sweepEffect;
		state.seekersWhisper = state.settings.seekersWhisper;
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESSpellCastEvent>(ShoutListener::Get());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESCellFullyLoadedEvent>(ShoutListener::Get());
		state.cellKey = state.settings.cellKey;
		state.cellPadButton = state.settings.cellPadButton;
		state.cellPadModifier = state.settings.cellPadModifier;
		state.cellPanel = state.settings.cellPanel;
		state.journalEntry = state.settings.journalEntry;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
		io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
		io.MouseDrawCursor = true;

		RE::UI::GetSingleton()->Register(menuName, CompletionistMenu::Create);
		state.provider.WatchCells();
		RE::BSInputDeviceManager::GetSingleton()->AddEventSink(InputHandler::GetSingleton());

		auto& trampoline = SKSE::GetTrampoline();
		_OnPresent = trampoline.write_call<5>(RELOCATION_ID(75461, 77246).address() + 0x9, OnPresent);

		INFO("In-game UI installed, open key {}", state.settings.openKey);
	}

	//---------------------------------------------------
	//-- GameUI ( Open / Close, game thread ) -----------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game UI ( Open ) -------------------------------
	//---------------------------------------------------

	void GameUI::Open() {

		if (auto* queue = RE::UIMessageQueue::GetSingleton()) {
			queue->AddMessage(menuName, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		}
	}

	//---------------------------------------------------
	//-- Game UI ( Close ) ------------------------------
	//---------------------------------------------------

	void GameUI::Close() {

		if (auto* queue = RE::UIMessageQueue::GetSingleton()) {
			queue->AddMessage(menuName, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}
	}

	//---------------------------------------------------
	//-- Game UI ( Open Main ) --------------------------
	//---------------------------------------------------

	// The open key's path, for the Journal's System tab entry
	void GameUI::OpenMain() {

		auto& state = GetState();
		if (state.open || !CanOpen()) {
			return;
		}
		state.padIgnored = state.padHeld;
		Open();
	}

	//---------------------------------------------------
	//-- Game UI ( Journal Entry Shown ) ----------------
	//---------------------------------------------------

	bool GameUI::JournalEntryShown() noexcept {

		return GetState().journalEntry;
	}

	//---------------------------------------------------
	//-- Game UI ( Set Open ) ---------------------------
	//---------------------------------------------------

	void GameUI::SetOpen(bool a_open) {

		auto& state = GetState();
		if (state.open == a_open) {
			return;
		}

		if (a_open) {
			// Before the snapshot, so the window opens on the current state
			CFramework_Master::FrameworkAPI::RefreshCompletion();
			state.provider.Rebuild();
			// The mouse comes back where it was left, the middle the first time
			if (!state.mousePlaced) {
				state.mouse = ImVec2(state.displayWidth * 0.5f, state.displayHeight * 0.5f);
				state.mousePlaced = true;
			}
			state.mouse.x = std::clamp(state.mouse.x, 0.0f, state.displayWidth.load());
			state.mouse.y = std::clamp(state.mouse.y, 0.0f, state.displayHeight.load());
			if (state.usingGamepad) {
				INFO("In-game UI: opened in controller mode, so no mouse cursor until the mouse moves");
			}
			// Opened from a controller, the mouse stays off the window until it moves: parked in the
			// middle it lit up whichever row sat there
			const auto mouse = state.usingGamepad ? ImVec2(-FLT_MAX, -FLT_MAX) : state.mouse;
			QueueInput([mouse](ImGuiIO& a_io) {
				a_io.ClearInputKeys();
				a_io.AddMousePosEvent(mouse.x, mouse.y);
			});
			state.lastFrame = std::chrono::steady_clock::now();
			state.justOpened = true;
			state.open = true;
			return;
		}

		state.open = false;
		state.capture = captureIdle;
		state.padCapture = captureIdle;
		QueueInput([](ImGuiIO& a_io) { a_io.ClearInputKeys(); });

		std::optional<UISettings> save;
		{
			std::scoped_lock lock(state.saveLock);
			save.swap(state.pendingSave);
		}
		if (save) {
			CVariables::VariablesAPI::SaveUISettings(*save);
		}
	}

	//---------------------------------------------------
	//-- Game UI ( Is Open ) ----------------------------
	//---------------------------------------------------

	bool GameUI::IsOpen() noexcept {

		return GetState().open;
	}

	//---------------------------------------------------
	//-- Game UI ( Can Open ) ---------------------------
	//---------------------------------------------------

	bool GameUI::CanOpen() {

		auto*       ui = RE::UI::GetSingleton();
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!ui || !player || !player->Is3DLoaded() || ui->GameIsPaused()) {
			return false;
		}

		for (const auto name : { RE::Console::MENU_NAME, RE::DialogueMenu::MENU_NAME, RE::MainMenu::MENU_NAME, RE::LoadingMenu::MENU_NAME, RE::RaceSexMenu::MENU_NAME }) {
			if (ui->IsMenuOpen(name)) {
				return false;
			}
		}
		return true;
	}

	//---------------------------------------------------
	//-- GameUI ( Frame Hook, render thread ) -----------
	//---------------------------------------------------

	//---------------------------------------------------
	//-- Game UI ( On Present ) -------------------------
	//---------------------------------------------------

	void GameUI::OnPresent(std::uint32_t a_arg) {

		_OnPresent(a_arg);

		auto& state = GetState();
		const bool playing = !state.open && OverlayAllowed();
		const bool sweeping = state.sweepActive && playing;
		if (state.sweepActive && !playing) {
			state.sweepActive = false;	// a menu cut it short
		}
		if (sweeping && !state.sweepFollowQueued.exchange(true)) {
			SKSE::GetTaskInterface()->AddTask([]() { FollowSweepFinds(); });
		}
		if (state.cellPanelToggle.exchange(false)) {
			state.settings.cellPanel = !state.settings.cellPanel;
			state.cellPanel = state.settings.cellPanel;
			// Saved now: pending saves wait for the window to close
			SKSE::GetTaskInterface()->AddTask([settings = state.settings]() { CVariables::VariablesAPI::SaveUISettings(settings); });
		}
		const bool overlay = playing && (state.cellPanel || state.toasts.Active());
		if (!state.open && !overlay && !sweeping) {
			state.printScreen = false;
			return;
		}

		if (!state.deviceReady && !InitialiseDevice(state)) {
			return;
		}

		// Under the panel and notifications
		if (sweeping) {
			DrawSweep(state);
		}
		if (state.open || overlay) {
			RenderFrame(state, overlay);
		}
	}

	//---------------------------------------------------
	//-- Game UI ( On Game Loaded ) ---------------------
	//---------------------------------------------------

	void GameUI::OnGameLoaded() {

		GetState().provider.OnGameLoaded();
		SyncShout();
	}

	//---------------------------------------------------
	//-- Game UI ( Notify ) -----------------------------
	//---------------------------------------------------

	void GameUI::Notify(std::string a_label, std::string a_text, ToastKind a_kind, std::optional<ImVec4> a_colour) {

		GetState().toasts.Push(std::move(a_label), std::move(a_text), a_kind, a_colour);
	}

	//---------------------------------------------------
	//-- Game UI ( Overlay Allowed ) --------------------
	//---------------------------------------------------

	// Normal play only
	bool GameUI::OverlayAllowed() {

		auto*       ui = RE::UI::GetSingleton();
		const auto* player = RE::PlayerCharacter::GetSingleton();
		if (!ui || !player || !player->Is3DLoaded() || ui->GameIsPaused()) {
			return false;
		}
		for (const auto name : { RE::DialogueMenu::MENU_NAME, RE::LoadingMenu::MENU_NAME, RE::MainMenu::MENU_NAME, RE::Console::MENU_NAME, RE::FavoritesMenu::MENU_NAME,
				 RE::LockpickingMenu::MENU_NAME, RE::BookMenu::MENU_NAME, RE::MapMenu::MENU_NAME, RE::RaceSexMenu::MENU_NAME }) {
			if (ui->IsMenuOpen(name)) {
				return false;
			}
		}
		// Any menu the player works in (journal, skills, inventory, crafting, mods' own menus), also when
		// something keeps the game running under it; HUD-style menus that are always open don't count
		using Flag = RE::UI_MENU_FLAGS;
		for (const auto& menu : ui->menuStack) {
			if (menu && !menu->AlwaysOpen() && menu->menuFlags.any(Flag::kPausesGame, Flag::kUsesCursor, Flag::kApplicationMenu, Flag::kInventoryItemMenu, Flag::kModal)) {
				return false;
			}
		}
		return true;
	}
}
