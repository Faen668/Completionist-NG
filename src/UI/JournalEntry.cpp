#include "UI/JournalEntry.hpp"

#include "UI/GameUI.hpp"
#include "UI/SettingsPages.hpp"

#undef PlaySound	// Windows' PlaySoundW, not RE::PlaySound

namespace CompletionistUI
{
	namespace
	{
		// The System tab is SystemPage in the game's quest_journal.swf and in SkyUI's. Its onLoad fills
		// CategoryList.entryList; a press calls onCategoryButtonPress, looked up by name when it happens.
		// SkyUI finds its entries by text (UpdateIndices), so ours only needs the indices redone. The game's
		// own swf finds them by position, in the press handler and in the three functions below, so those
		// run with our entry taken out and the pressed index moved past it.
		constexpr const char* pagePath = "_root.QuestJournalFader.Menu_mc.SystemFader.Page_mc";
		constexpr const char* prototypePath = "_global.SystemPage.prototype";
		constexpr const char* entryMember = "_comp_journal";
		constexpr const char* originalPress = "_comp_onCategoryButtonPress";
		// Name, where the original is kept, and how the list is redrawn after, as the original does
		struct Positional
		{
			const char* function;
			const char* original;
			const char* redraw;
		};
		constexpr Positional positionalFunctions[] = {
			{ "SetShowMod", "_comp_SetShowMod", "InvalidateData" },
			{ "UpdatePermissions", "_comp_UpdatePermissions", "UpdateList" },
			{ "RefreshSystemButtons", "_comp_RefreshSystemButtons", "UpdateList" },
		};

		std::atomic<bool> openAfterJournal{ false };

		//---------------------------------------------------
		//-- Journal Entry ( Helpers ) ----------------------
		//---------------------------------------------------

		bool GetEntries(RE::GFxValue& a_page, RE::GFxValue& a_list, RE::GFxValue& a_entries) {

			if (!a_page.GetMember("CategoryList", &a_list) || !a_list.IsObject()) { return false; }
			if (a_list.GetMember("entryList", &a_entries) && a_entries.IsArray()) { return true; }
			return a_list.Invoke("__get__entryList", &a_entries) && a_entries.IsArray();
		}

		std::string_view Text(const RE::GFxValue& a_entry) {

			RE::GFxValue text;
			return a_entry.IsObject() && a_entry.GetMember("text", &text) && text.IsString() ? std::string_view(text.GetString()) : std::string_view();
		}

		std::int32_t FindOurs(RE::GFxValue& a_entries) {

			for (std::uint32_t i = 0; i < a_entries.GetArraySize(); ++i) {
				RE::GFxValue entry;
				RE::GFxValue ours;
				if (a_entries.GetElement(i, &entry) && entry.IsObject() && entry.GetMember(entryMember, &ours) && ours.IsBool()) {
					return static_cast<std::int32_t>(i);
				}
			}
			return -1;
		}

		// After SkyUI's Mod Configuration, else just before Settings (after Installed Content and Creations)
		std::uint32_t Placement(RE::GFxValue& a_entries) {

			std::optional<std::uint32_t> settings;
			for (std::uint32_t i = 0; i < a_entries.GetArraySize(); ++i) {
				RE::GFxValue entry;
				a_entries.GetElement(i, &entry);
				const auto text = Text(entry);
				if (text == "$MOD CONFIGURATION") {
					return i + 1;
				}
				if (text == "$SETTINGS" && !settings) {
					settings = i;
				}
			}
			return settings.value_or(a_entries.GetArraySize());
		}

		void Insert(RE::GFxValue& a_entries, const RE::GFxValue& a_entry) {

			const auto at = Placement(a_entries);
			a_entries.PushBack(a_entry);
			for (auto i = a_entries.GetArraySize() - 1; i > at; --i) {
				RE::GFxValue moved;
				a_entries.GetElement(i - 1, &moved);
				a_entries.SetElement(i, moved);
			}
			a_entries.SetElement(at, a_entry);
		}

		//---------------------------------------------------
		//-- Journal Entry ( Press Hook ) -------------------
		//---------------------------------------------------

		// SystemPage.onCategoryButtonPress
		class CategoryPress final : public RE::GFxFunctionHandler
		{
		public:
			void Call(Params& a_params) override {

				if (!a_params.thisPtr) { return; }

				RE::GFxValue entry;
				RE::GFxValue ours;
				const bool   hasEvent = a_params.argCount > 0 && a_params.args[0].IsObject();
				if (hasEvent && a_params.args[0].GetMember("entry", &entry) && entry.IsObject() && entry.GetMember(entryMember, &ours) && ours.IsBool() && ours.GetBool()) {

					// Only from the tab's list itself, not under a confirm box or another panel
					RE::GFxValue state;
					RE::GFxValue mainState;
					if (a_params.thisPtr->GetMember("iCurrentState", &state) && a_params.movie->GetVariable(&mainState, "_global.SystemPage.MAIN_STATE") &&
						state.IsNumber() && mainState.IsNumber() && state.GetNumber() != mainState.GetNumber()) {
						return;
					}

					RE::PlaySound("UIMenuOK");
					openAfterJournal = true;
					if (auto* queue = RE::UIMessageQueue::GetSingleton()) {
						queue->AddMessage(RE::JournalMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
					}
					INFO("Journal Entry: Pressed, closing the Journal");
					return;
				}

				// The game's own handler counts by position: entries after ours are one further on
				RE::GFxValue index;
				RE::GFxValue list;
				RE::GFxValue entries;
				RE::GFxValue byText;
				std::int32_t ourIndex = -1;
				const bool   positional = !(a_params.thisPtr->GetMember("UpdateIndices", &byText) && !byText.IsUndefined());
				if (positional && hasEvent && a_params.args[0].GetMember("index", &index) && index.IsNumber() && GetEntries(*a_params.thisPtr, list, entries)) {
					ourIndex = FindOurs(entries);
				}
				const bool shift = ourIndex >= 0 && index.GetNumber() > ourIndex;
				if (shift) {
					a_params.args[0].SetMember("index", index.GetNumber() - 1.0);
				}
				a_params.thisPtr->Invoke(originalPress, a_params.retVal, a_params.args, a_params.argCount);
				if (shift) {
					a_params.args[0].SetMember("index", index);
				}
			}
		};

		// SetShowMod, UpdatePermissions, RefreshSystemButtons in the game's swf
		class WithoutOurs final : public RE::GFxFunctionHandler
		{
		public:
			explicit WithoutOurs(const Positional& a_function) :
				function(a_function) {}

			void Call(Params& a_params) override {

				if (!a_params.thisPtr) { return; }

				RE::GFxValue list;
				RE::GFxValue entries;
				RE::GFxValue taken;
				if (GetEntries(*a_params.thisPtr, list, entries)) {
					if (const auto at = FindOurs(entries); at >= 0) {
						entries.GetElement(static_cast<std::uint32_t>(at), &taken);
						entries.RemoveElement(static_cast<std::uint32_t>(at));
					}
				}

				a_params.thisPtr->Invoke(function.original, a_params.retVal, a_params.args, a_params.argCount);

				if (taken.IsObject()) {
					Insert(entries, taken);
					list.Invoke(function.redraw);
				}
			}

		private:
			const Positional& function;
		};

		CategoryPress categoryPress;
		WithoutOurs   withoutOurs[] = { WithoutOurs(positionalFunctions[0]), WithoutOurs(positionalFunctions[1]), WithoutOurs(positionalFunctions[2]) };

		//---------------------------------------------------
		//-- Journal Entry ( Wrap Function ) ----------------
		//---------------------------------------------------

		bool WrapFunction(RE::GFxMovieView* a_movie, RE::GFxValue& a_prototype, const char* a_function, const char* a_original, RE::GFxFunctionHandler* a_handler) {

			RE::GFxValue existing;
			if (a_prototype.GetMember(a_original, &existing) && !existing.IsUndefined()) { return true; }

			RE::GFxValue original;
			if (!a_prototype.GetMember(a_function, &original) || original.IsUndefined()) { return false; }

			RE::GFxValue function;
			a_movie->CreateFunction(&function, a_handler);
			a_prototype.SetMember(a_original, original);
			a_prototype.SetMember(a_function, function);
			return true;
		}

		//---------------------------------------------------
		//-- Journal Entry ( Add Entry ) --------------------
		//---------------------------------------------------

		// Retries until the page's onLoad has filled the list, so the game's entries are all there first
		void AddEntry(int a_tries) {

			auto* ui = RE::UI::GetSingleton();
			auto  menu = ui ? ui->GetMenu(RE::JournalMenu::MENU_NAME) : nullptr;
			if (!menu || !menu->uiMovie) { return; }

			auto*        movie = menu->uiMovie.get();
			RE::GFxValue page;
			RE::GFxValue list;
			RE::GFxValue entries;
			if (!movie->GetVariable(&page, pagePath) || !page.IsObject() || !GetEntries(page, list, entries) || entries.GetArraySize() == 0) {
				if (a_tries > 0) {
					SKSE::GetTaskInterface()->AddUITask([a_tries]() { AddEntry(a_tries - 1); });
				} else {
					INFO("Journal Entry: No System tab list in the Journal, so no entry");
				}
				return;
			}
			if (FindOurs(entries) >= 0) { return; }

			RE::GFxValue prototype;
			if (!movie->GetVariable(&prototype, prototypePath) || !prototype.IsObject() || !WrapFunction(movie, prototype, "onCategoryButtonPress", originalPress, &categoryPress)) {
				INFO("Journal Entry: No SystemPage.onCategoryButtonPress in the Journal, so no entry");
				return;
			}
			RE::GFxValue byText;
			const bool   positional = !(prototype.GetMember("UpdateIndices", &byText) && !byText.IsUndefined());
			if (positional) {
				for (std::size_t i = 0; i < std::size(positionalFunctions); ++i) {
					WrapFunction(movie, prototype, positionalFunctions[i].function, positionalFunctions[i].original, &withoutOurs[i]);
				}
			}

			const std::string label = SettingsPages::translate ? SettingsPages::translate("UI_Journal_Entry") : std::string("COMPLETIONIST");
			RE::GFxValue      entry;
			RE::GFxValue      text;
			movie->CreateObject(&entry);
			movie->CreateString(&text, label.c_str());
			entry.SetMember("text", text);
			entry.SetMember(entryMember, true);
			Insert(entries, entry);
			if (!positional) {
				page.Invoke("UpdateIndices");
			}
			list.Invoke("InvalidateData");
			INFO("Journal Entry: Added to the System tab as entry {} of {} ({})", FindOurs(entries), entries.GetArraySize(), positional ? "by position" : "by text");
		}

		//---------------------------------------------------
		//-- Journal Entry ( Menu Watch ) -------------------
		//---------------------------------------------------

		class MenuWatch final : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {

				if (!a_event || a_event->menuName != RE::JournalMenu::MENU_NAME) {
					return RE::BSEventNotifyControl::kContinue;
				}
				if (a_event->opening) {
					openAfterJournal = false;
					if (GameUI::JournalEntryShown()) {
						SKSE::GetTaskInterface()->AddUITask([]() { AddEntry(120); });
					}
				} else if (openAfterJournal.exchange(false)) {
					// Opened once the Journal is gone, as the open key would
					SKSE::GetTaskInterface()->AddTask([]() { GameUI::OpenMain(); });
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};

		MenuWatch menuWatch;
	}

	//---------------------------------------------------
	//-- Journal Entry ( Install ) ----------------------
	//---------------------------------------------------

	void JournalEntry::Install() {

		if (auto* ui = RE::UI::GetSingleton()) {
			ui->AddEventSink<RE::MenuOpenCloseEvent>(&menuWatch);
		}
	}
}
