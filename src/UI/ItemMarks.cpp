#include "UI/ItemMarks.hpp"

#include "Import/CompletionistAPI.h"
#include "Internal Utility/CompletionistAPIServer.hpp"
#include "Internal Utility/Variables.hpp"
#include "Internal Utility/mainHUD.hpp"
#include "UI/SettingsPages.hpp"

namespace CompletionistUI
{
	namespace
	{
		// Icon data from svg_to_icons.py, op: 0 moveTo, 1 lineTo, 2 curveTo
		struct IconCommand
		{
			int   op;
			float a, b, c, d;
		};

		struct IconShape
		{
			int   fill;
			int   stroke;
			float width;
			bool  strokeFirst;
			int   begin;
			int   count;
		};

#include "UI/ItemIcons.inl"

		enum class IconGroup
		{
			kNeeded,
			kFound,
			kMuseum,
		};

		struct Icon
		{
			std::string_view             name;
			IconGroup                    group;
			std::span<const IconShape>   shapes;
			std::span<const IconCommand> commands;
		};

		constexpr Icon icons[] = {
			{ "cmpNew", IconGroup::kNeeded, TrophyPlusShapes, TrophyPlusCommands },
			{ "cmpFound", IconGroup::kFound, TrophyTickShapes, TrophyTickCommands },
			{ "cmpDisplayable", IconGroup::kMuseum, MuseumPlusShapes, MuseumPlusCommands },
			{ "cmpDisplayed", IconGroup::kMuseum, MuseumTickShapes, MuseumTickCommands },
			{ "cmpOccupied", IconGroup::kMuseum, MuseumDiamondShapes, MuseumDiamondCommands },
		};

		// Crafting Menu covers SkyUI's crafting lists and Crafting Categories' constructibleobjectmenu.swf
		constexpr std::string_view listMenus[] = { RE::InventoryMenu::MENU_NAME, RE::ContainerMenu::MENU_NAME, RE::BarterMenu::MENU_NAME, RE::GiftMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME };
		constexpr const char*      skyuiIcons[] = { "bestIcon", "favoriteIcon", "poisonIcon", "stolenIcon", "enchIcon", "readIcon" };

		constexpr const char* markName = "_comp_mark";
		constexpr const char* bandName = "_comp_band";
		constexpr const char* bandMember = "_comp_band";
		constexpr const char* statusMember = "comp_status";
		constexpr const char* originalSetEntry = "_comp_setEntry";
		constexpr const char* originalProcessEntry = "_comp_processEntry";
		constexpr const char* originalApplyFilter = "_comp_applyFilter";
		constexpr const char* craftedMember = "_comp_formId";

		//---------------------------------------------------
		//-- Item Marks ( Helpers ) -------------------------
		//---------------------------------------------------

		double Number(const RE::GFxValue& a_object, const char* a_member, double a_default = 0.0) {

			RE::GFxValue value;
			return a_object.GetMember(a_member, &value) && value.IsNumber() ? value.GetNumber() : a_default;
		}

		bool Visible(const RE::GFxValue& a_clip) {

			RE::GFxValue value;
			return a_clip.GetMember("_visible", &value) && value.IsBool() && value.GetBool();
		}

		const char* GetIconName(const RE::GFxValue& a_entry) {

			// Recipes are marked by the item they make
			auto formID = static_cast<RE::FormID>(Number(a_entry, craftedMember));
			if (!formID) {
				formID = static_cast<RE::FormID>(Number(a_entry, "formId"));
			}
			auto* form = formID ? RE::TESForm::LookupByID(formID) : nullptr;
			if (auto* recipe = form ? form->As<RE::BGSConstructibleObject>() : nullptr) {
				form = recipe->createdItem;
			}
			auto* object = form ? form->As<RE::TESBoundObject>() : nullptr;
			if (!object) { return ""; }

			CompletionistAPI::IconInfo info{};
			info.object = object;
			CompletionistAPIServer::GetInterfaceV20()->GetIconInfo(&info, 1);
			return info.GetRequiredIconName();
		}

		//---------------------------------------------------
		//-- Item Marks ( Get Icon ) ------------------------
		//---------------------------------------------------

		const Icon* GetIcon(std::string_view a_name) {

			if (!CVariables::V_ItemIcons) { return nullptr; }

			for (const auto& icon : icons) {
				if (icon.name != a_name) {
					continue;
				}
				const bool shown = icon.group == IconGroup::kNeeded ? CVariables::V_ItemIcons_Needed : icon.group == IconGroup::kFound ? CVariables::V_ItemIcons_Found : CVariables::V_ItemIcons_Museum;
				return shown ? std::addressof(icon) : nullptr;
			}
			return nullptr;
		}

		//---------------------------------------------------
		//-- Item Marks ( Draw Icon ) -----------------------
		//---------------------------------------------------

		void DrawIcon(RE::GFxValue& a_clip, const Icon& a_icon, double a_size) {

			a_clip.Invoke("clear");

			const auto path = [&](const IconShape& a_shape) {
				for (int i = a_shape.begin; i < a_shape.begin + a_shape.count; ++i) {
					const auto& c = a_icon.commands[static_cast<std::size_t>(i)];
					if (c.op == 0) {
						a_clip.Invoke("moveTo", std::array<RE::GFxValue, 2>{ c.a * a_size, c.b * a_size });
					} else if (c.op == 1) {
						a_clip.Invoke("lineTo", std::array<RE::GFxValue, 2>{ c.a * a_size, c.b * a_size });
					} else {
						a_clip.Invoke("curveTo", std::array<RE::GFxValue, 4>{ c.a * a_size, c.b * a_size, c.c * a_size, c.d * a_size });
					}
				}
			};

			const auto outline = [&](const IconShape& a_shape) {
				if (a_shape.stroke < 0 || a_shape.width <= 0.0f) { return; }
				a_clip.Invoke("lineStyle", std::array<RE::GFxValue, 7>{ a_shape.width * a_size, static_cast<double>(a_shape.stroke), 100.0, false, "normal", "round", "round" });
				path(a_shape);
				a_clip.Invoke("lineStyle");
			};

			const auto fill = [&](const IconShape& a_shape) {
				if (a_shape.fill < 0) { return; }
				a_clip.Invoke("lineStyle");
				a_clip.Invoke("beginFill", std::array<RE::GFxValue, 2>{ static_cast<double>(a_shape.fill), 100.0 });
				path(a_shape);
				a_clip.Invoke("endFill");
			};

			for (const auto& shape : a_icon.shapes) {
				if (shape.strokeFirst) {
					outline(shape);
					fill(shape);
				} else {
					fill(shape);
					outline(shape);
				}
			}
		}

		//---------------------------------------------------
		//-- Item Marks ( Name Field ) ----------------------
		//---------------------------------------------------

		// Name column is the leftmost textField showing text
		RE::GFxValue GetNameField(RE::GFxValue& a_row, double& a_x) {

			RE::GFxValue textField;
			for (int i = 0; i < 10; ++i) {
				RE::GFxValue column;
				RE::GFxValue text;
				if (!a_row.GetMember(fmt::format("textField{}", i).c_str(), &column) || !column.IsDisplayObject() || !Visible(column) ||
					!column.GetMember("text", &text) || !text.IsString() || !*text.GetString()) {
					continue;
				}
				const double x = Number(column, "_x");
				if (textField.IsUndefined() || x < a_x) {
					textField = column;
					a_x = x;
				}
			}
			return textField;
		}

		//---------------------------------------------------
		//-- Item Marks ( Name Style ) ----------------------
		//---------------------------------------------------

		struct NameStyle
		{
			std::int32_t format{ 7 };
			std::int32_t prefix{ 0 };
			std::string  colour;
		};

		// Format 0-2 prefix, 3-5 prefix and colour, 6 colour only, 7 no edits
		NameStyle GetNameStyle(std::string_view a_icon) {

			using namespace CVariables;
			if (a_icon == "cmpDisplayed") {
				return { V_MenuFormat_Displayed, V_MenuPrefix_Displayed, V_HUD_Override_Enabled_Displayed ? V_HUD_CustomColourString_Displayed : V_HUD_ColourString_Displayed };
			}
			if (a_icon == "cmpOccupied") {
				return { V_MenuFormat_Occupied, V_MenuPrefix_Occupied, V_HUD_Override_Enabled_Occupied ? V_HUD_CustomColourString_Occupied : V_HUD_ColourString_Occupied };
			}
			if (a_icon == "cmpDisplayable") {
				return { V_MenuFormat_Displayable, V_MenuPrefix_Displayable, V_HUD_Override_Enabled_Displayable ? V_HUD_CustomColourString_Displayable : V_HUD_ColourString_Displayable };
			}
			if (a_icon == "cmpFound") {
				return { V_MenuFormat_G, V_MenuPrefix_G, V_MenuColour_G };
			}
			if (a_icon == "cmpNew") {
				return { V_MenuFormat_N, V_MenuPrefix_N, V_MenuColour_N };
			}
			return {};
		}

		//---------------------------------------------------
		//-- Item Marks ( Style Name ) ----------------------
		//---------------------------------------------------

		// Rows are reused, so the colour SkyUI gave the row is kept to put back
		void StyleName(RE::GFxMovie* a_movie, RE::GFxValue& a_field, std::string_view a_icon) {

			const auto style = GetNameStyle(a_icon);

			RE::GFxValue was;
			RE::GFxValue ours;
			const double current = Number(a_field, "textColor");
			const bool   coloured = a_field.GetMember("_comp_colour", &ours) && ours.IsNumber();
			const double base = coloured && current == ours.GetNumber() && a_field.GetMember("_comp_was", &was) && was.IsNumber() ? was.GetNumber() : current;

			if (style.format < 6) {
				RE::GFxValue text;
				if (a_field.GetMember("text", &text) && text.IsString()) {
					const auto   shown = Completionist_MainHUD::TextnTagsAPI::FormatMenuName(text.GetString(), style.format, style.prefix);
					RE::GFxValue value;
					a_movie->CreateString(&value, shown.c_str());
					a_field.SetMember("text", value);
				}
			}

			const auto rgb = style.colour.size() == 7 && style.colour[0] == '#' ? std::strtol(style.colour.c_str() + 1, nullptr, 16) : -1L;
			if (style.format >= 3 && style.format <= 6 && rgb >= 0) {
				a_field.SetMember("_comp_was", base);
				a_field.SetMember("_comp_colour", static_cast<double>(rgb));
				a_field.SetMember("textColor", static_cast<double>(rgb));
			} else if (coloured) {
				a_field.SetMember("textColor", base);
				a_field.SetMember("_comp_colour", RE::GFxValue());
			}
		}

		//---------------------------------------------------
		//-- Item Marks ( Mark Row ) ------------------------
		//---------------------------------------------------

		void MarkRow(RE::GFxMovie* a_movie, RE::GFxValue& a_row, const RE::GFxValue& a_entry) {

			RE::GFxValue mark;
			const bool hasMark = a_row.GetMember(markName, &mark) && mark.IsDisplayObject();

			double     nameX = 0.0;
			auto       textField = GetNameField(a_row, nameX);
			const auto iconName = a_entry.IsObject() ? std::string_view(GetIconName(a_entry)) : std::string_view{};
			if (!textField.IsUndefined()) {
				StyleName(a_movie, textField, iconName);
			}

			const Icon* icon = GetIcon(iconName);
			if (!icon) {
				if (hasMark) {
					mark.SetMember("_visible", false);
				}
				return;
			}
			if (textField.IsUndefined()) { return; }

			if (!hasMark) {
				RE::GFxValue depth;
				a_row.Invoke("getNextHighestDepth", &depth);
				if (!a_row.Invoke("createEmptyMovieClip", &mark, std::array<RE::GFxValue, 2>{ markName, depth }) || !mark.IsDisplayObject()) {
					return;
				}
			}

			const double textHeight = Number(textField, "textHeight", Number(textField, "_height", 20.0));
			const double size = std::round(textHeight * 1.15 * CVariables::V_ItemIcons_Size / 100.0);

			RE::GFxValue drawn;
			const bool same = mark.GetMember("_comp_icon", &drawn) && drawn.IsString() && icon->name == drawn.GetString() && Number(mark, "_comp_size") == size;
			if (!same) {
				DrawIcon(mark, *icon, size);
				mark.SetMember("_comp_icon", RE::GFxValue(icon->name.data()));
				mark.SetMember("_comp_size", size);
			}

			// SkyUI hides its icons by frame, frame 1 is hidden
			double right = nameX + std::min(Number(textField, "textWidth") + 4.0, Number(textField, "_width"));
			for (const auto* name : skyuiIcons) {
				RE::GFxValue skyuiIcon;
				if (a_row.GetMember(name, &skyuiIcon) && skyuiIcon.IsDisplayObject() && Visible(skyuiIcon) && Number(skyuiIcon, "_currentframe") > 1.0) {
					right = std::max(right, Number(skyuiIcon, "_x") + Number(skyuiIcon, "_width"));
				}
			}

			mark.SetMember("_x", std::round(right + size * 0.2));
			mark.SetMember("_y", std::round(Number(textField, "_y") + (Number(textField, "_height") - size) * 0.5));
			mark.SetMember("_alpha", static_cast<double>(CVariables::V_ItemIcons_Opacity));
			mark.SetMember("_visible", true);
		}

		//---------------------------------------------------
		//-- Item Marks ( Mark Band ) -----------------------
		//---------------------------------------------------

		// 0/1 tracked groups, 2/3 untracked, -1 no band
		void MarkBand(RE::GFxValue& a_row, const RE::GFxValue& a_entry) {

			RE::GFxValue band;
			const bool hasBand = a_row.GetMember(bandName, &band) && band.IsDisplayObject();
			const double group = CVariables::V_Sort_Bands && a_entry.IsObject() ? Number(a_entry, bandMember, -1.0) : -1.0;
			if (group < 0.0) {
				if (hasBand) {
					band.Invoke("clear");
				}
				return;
			}

			// Lowest depth, under SkyUI's own clips
			if (!hasBand && (!a_row.Invoke("createEmptyMovieClip", &band, std::array<RE::GFxValue, 2>{ bandName, -16384.0 }) || !band.IsDisplayObject())) {
				return;
			}

			RE::GFxValue background;
			const bool hasBackground = a_row.GetMember("background", &background) && background.IsDisplayObject();
			const auto& area = hasBackground ? background : a_row;
			// Starts a little left of the row so the edge clears the item icon
			const double x = (hasBackground ? Number(area, "_x") : 0.0) - 5.0;
			const double y = hasBackground ? Number(area, "_y") : 0.0;
			const double width = Number(area, "_width") + 5.0;
			const double height = Number(area, "_height");

			const auto rect = [&](double a_x, double a_width, double a_colour, double a_alpha) {
				band.Invoke("beginFill", std::array<RE::GFxValue, 2>{ a_colour, a_alpha });
				band.Invoke("moveTo", std::array<RE::GFxValue, 2>{ a_x, y });
				band.Invoke("lineTo", std::array<RE::GFxValue, 2>{ a_x + a_width, y });
				band.Invoke("lineTo", std::array<RE::GFxValue, 2>{ a_x + a_width, y + height });
				band.Invoke("lineTo", std::array<RE::GFxValue, 2>{ a_x, y + height });
				band.Invoke("endFill");
			};

			const bool tracked = group < 2.0;
			const bool odd = static_cast<int>(group) % 2 == 1;
			band.Invoke("clear");
			band.Invoke("lineStyle");
			if (width > 0.0 && height > 0.0) {
				rect(x, width, odd ? 0xFFFFFF : 0xC6A262, odd ? 3.0 : 8.0);
				rect(x, 2.0, tracked ? 0xC6A262 : 0x444444, 100.0);
			}
		}

		//---------------------------------------------------
		//-- Item Marks ( Sort Status ) ---------------------
		//---------------------------------------------------

		double GetSortStatus(std::string_view a_icon) {

			using namespace CVariables;
			const std::pair<std::string_view, std::int32_t> order[] = {
				{ "cmpDisplayable", V_Sort_Displayable }, { "cmpOccupied", V_Sort_Occupied }, { "cmpDisplayed", V_Sort_Displayed },
				{ "cmpNew", V_Sort_Needed }, { "cmpFound", V_Sort_Found },
			};

			for (const auto& [name, rank] : order) {
				if (name == a_icon) {
					return static_cast<double>(rank);
				}
			}
			return 9.0;
		}

		void SetSortStatus(RE::GFxValue& a_entry) {

			a_entry.SetMember(statusMember, GetSortStatus(GetIconName(a_entry)));
		}

		//---------------------------------------------------
		//-- Item Marks ( Tag Recipes ) ---------------------
		//---------------------------------------------------

		// Recipe entries carry no formId, the game's recipe list is in the same order as its entryList
		void TagRecipes(RE::GFxMovie* a_movie) {

			auto* ui = RE::UI::GetSingleton();
			auto  menu = ui ? ui->GetMenu<RE::CraftingMenu>() : nullptr;
			if (!menu || menu->uiMovie.get() != a_movie) { return; }

			auto* subMenu = menu->GetCraftingSubMenu();
			auto* construct = subMenu ? skyrim_cast<RE::CraftingSubMenus::ConstructibleObjectMenu*>(subMenu) : nullptr;
			if (!construct || !construct->entryList.IsArray()) { return; }

			auto&      list = construct->entryList;
			const auto count = std::min<std::uint32_t>(list.GetArraySize(), construct->recipes.size());
			for (std::uint32_t i = 0; i < count; ++i) {
				auto*        recipe = construct->recipes[i].constructibleObject;
				RE::GFxValue entry;
				if (!recipe || !recipe->createdItem || !list.GetElement(i, &entry) || !entry.IsObject()) {
					continue;
				}
				const auto formID = static_cast<double>(recipe->createdItem->GetFormID());
				if (Number(entry, craftedMember) != formID) {
					entry.SetMember(craftedMember, formID);
					SetSortStatus(entry);
				}
			}
		}

		//---------------------------------------------------
		//-- Item Marks ( Set Bands ) -----------------------
		//---------------------------------------------------

		// After the sort, a new band each time the status changes
		void SetBands(RE::GFxValue& a_filter, RE::GFxValue& a_list) {

			RE::GFxValue attributes;
			RE::GFxValue primary;
			const bool ours = a_filter.GetMember("_sortAttributes", &attributes) && attributes.IsArray() && attributes.GetElement(0, &primary) &&
			                  primary.IsString() && std::string_view(primary.GetString()) == statusMember;

			double last = -1.0;
			int    group = -1;
			for (std::uint32_t i = 0; i < a_list.GetArraySize(); ++i) {
				RE::GFxValue entry;
				if (!a_list.GetElement(i, &entry) || !entry.IsObject()) {
					continue;
				}
				if (!ours) {
					entry.SetMember(bandMember, -1.0);
					continue;
				}
				const double status = Number(entry, statusMember, 9.0);
				if (status != last) {
					last = status;
					++group;
				}
				entry.SetMember(bandMember, static_cast<double>((status < 9.0 ? 0 : 2) + group % 2));
			}
		}

		//---------------------------------------------------
		//-- Item Marks ( SkyUI Function Hooks ) ------------
		//---------------------------------------------------

		// InventoryListEntry.setEntry
		class SetEntry final : public RE::GFxFunctionHandler
		{
		public:
			void Call(Params& a_params) override {

				if (!a_params.thisPtr) { return; }

				a_params.thisPtr->Invoke(originalSetEntry, a_params.retVal, a_params.args, a_params.argCount);
				if (a_params.argCount > 0) {
					MarkBand(*a_params.thisPtr, a_params.args[0]);
					MarkRow(a_params.movie, *a_params.thisPtr, a_params.args[0]);
				}
			}
		};

		// InventoryDataSetter.processEntry
		class ProcessEntry final : public RE::GFxFunctionHandler
		{
		public:
			void Call(Params& a_params) override {

				if (!a_params.thisPtr) { return; }

				a_params.thisPtr->Invoke(originalProcessEntry, a_params.retVal, a_params.args, a_params.argCount);
				if (a_params.argCount > 0 && a_params.args[0].IsObject()) {
					SetSortStatus(a_params.args[0]);
				}
			}
		};

		// SortFilter.applyFilter, catches entries processed before the hook was in
		class ApplyFilter final : public RE::GFxFunctionHandler
		{
		public:
			void Call(Params& a_params) override {

				if (!a_params.thisPtr) { return; }

				TagRecipes(a_params.movie);
				if (a_params.argCount > 0 && a_params.args[0].IsArray()) {
					auto& list = a_params.args[0];
					for (std::uint32_t i = 0; i < list.GetArraySize(); ++i) {
						RE::GFxValue entry;
						RE::GFxValue status;
						if (list.GetElement(i, &entry) && entry.IsObject() && !(entry.GetMember(statusMember, &status) && status.IsNumber())) {
							SetSortStatus(entry);
						}
					}
				}
				a_params.thisPtr->Invoke(originalApplyFilter, a_params.retVal, a_params.args, a_params.argCount);
				if (a_params.argCount > 0 && a_params.args[0].IsArray()) {
					SetBands(*a_params.thisPtr, a_params.args[0]);
				}
			}
		};

		SetEntry     setEntry;
		ProcessEntry processEntry;
		ApplyFilter  applyFilter;

		//---------------------------------------------------
		//-- Item Marks ( Wrap Function ) -------------------
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
		//-- Item Marks ( Add Sort State ) ------------------
		//---------------------------------------------------

		// Adds a state to the name column in SkyUI's loaded config, retries until the config is loaded
		void AddSortState(std::string a_menu, int a_tries) {

			auto* ui = RE::UI::GetSingleton();
			auto menu = ui ? ui->GetMenu(a_menu) : nullptr;
			if (!menu || !menu->uiMovie) { return; }

			auto* movie = menu->uiMovie.get();
			const bool   crafting = a_menu == RE::CraftingMenu::MENU_NAME;
			const auto*  path = crafting ? "_global.skyui.util.ConfigManager._config.ListLayout.columns.craftNameColumn" :
			                               "_global.skyui.util.ConfigManager._config.ListLayout.columns.itemNameColumn";
			RE::GFxValue column;
			if (!movie->GetVariable(&column, path) || !column.IsObject()) {
				if (a_tries > 0) {
					SKSE::GetTaskInterface()->AddUITask([a_menu, a_tries]() { AddSortState(a_menu, a_tries - 1); });
				} else {
					INFO("Item Marks: No SkyUI name column in {}", a_menu);
				}
				return;
			}

			RE::GFxValue added;
			if (column.GetMember("_comp_state", &added) && added.IsNumber()) { return; }

			const auto states = static_cast<int>(Number(column, "states"));
			if (states < 1) { return; }

			const int state = states + 1;
			const std::string title = SettingsPages::translate ? SettingsPages::translate("UI_Sort_Completionist") : std::string("Completionist");

			RE::GFxValue stateData, label, entry, labelText, attributes, options;
			movie->CreateObject(&stateData);
			movie->CreateObject(&label);
			movie->CreateObject(&entry);
			movie->CreateString(&labelText, title.c_str());
			label.SetMember("text", labelText);
			entry.SetMember("text", "@text");

			movie->CreateArray(&attributes);
			attributes.PushBack(statusMember);
			attributes.PushBack("text");

			movie->CreateArray(&options);
			options.PushBack(16.0);	// NUMERIC
			options.PushBack(1.0);	// CASEINSENSITIVE

			stateData.SetMember("label", label);
			stateData.SetMember("entry", entry);
			stateData.SetMember("sortAttributes", attributes);
			stateData.SetMember("sortOptions", options);
			stateData.SetMember("playerOnly", false);

			column.SetMember(fmt::format("state{}", state).c_str(), stateData);
			column.SetMember("states", static_cast<double>(state));
			column.SetMember("_comp_state", static_cast<double>(state));
			INFO("Item Marks: Added sort state {} to {}", state, a_menu);
		}

		//---------------------------------------------------
		//-- Item Marks ( Wrap Menu ) -----------------------
		//---------------------------------------------------

		void WrapMenu(std::string a_menu, int a_tries) {

			auto* ui = RE::UI::GetSingleton();
			auto menu = ui ? ui->GetMenu(a_menu) : nullptr;
			if (!menu || !menu->uiMovie) { return; }

			auto* movie = menu->uiMovie.get();
			const bool   crafting = a_menu == RE::CraftingMenu::MENU_NAME;
			RE::GFxValue listEntry;
			if (!movie->GetVariable(&listEntry, crafting ? "_global.CraftingListEntry.prototype" : "_global.InventoryListEntry.prototype") || !listEntry.IsObject()) {
				if (a_tries > 0) {
					SKSE::GetTaskInterface()->AddUITask([a_menu, a_tries]() { WrapMenu(a_menu, a_tries - 1); });
				} else {
					INFO("Item Marks: No SkyUI item list in {}", a_menu);
				}
				return;
			}

			if (!WrapFunction(movie, listEntry, "setEntry", originalSetEntry, &setEntry)) {
				INFO("Item Marks: No setEntry in {}", a_menu);
				return;
			}

			RE::GFxValue dataSetter;
			if (movie->GetVariable(&dataSetter, crafting ? "_global.CraftingDataSetter.prototype" : "_global.InventoryDataSetter.prototype") && dataSetter.IsObject() &&
				WrapFunction(movie, dataSetter, "processEntry", originalProcessEntry, &processEntry)) {

				RE::GFxValue sortFilter;
				if (movie->GetVariable(&sortFilter, "_global.skyui.filter.SortFilter.prototype") && sortFilter.IsObject()) {
					WrapFunction(movie, sortFilter, "applyFilter", originalApplyFilter, &applyFilter);
				}
				AddSortState(a_menu, 600);
			}
			INFO("Item Marks: Installed in {}", a_menu);
		}

		//---------------------------------------------------
		//-- Item Marks ( On Menu Open ) --------------------
		//---------------------------------------------------

		class MenuWatch final : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override {

				if (a_event && a_event->opening && std::ranges::find(listMenus, std::string_view(a_event->menuName.c_str())) != std::end(listMenus)) {
					SKSE::GetTaskInterface()->AddUITask([menu = std::string(a_event->menuName.c_str())]() { WrapMenu(menu, 10); });
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};

		MenuWatch menuWatch;
	}

	//---------------------------------------------------
	//-- Item Marks ( Install ) -------------------------
	//---------------------------------------------------

	void ItemMarks::Install() {

		if (auto* ui = RE::UI::GetSingleton()) {
			ui->AddEventSink<RE::MenuOpenCloseEvent>(&menuWatch);
		}
	}
}
