#include "UI/Core/SettingsLayout.hpp"

#include <algorithm>
#include <cstdio>
#include <unordered_map>

namespace CompletionistUI
{
	namespace
	{

		//---------------------------------------------------
		//-- Settings Layout ( Header ) ---------------------
		//---------------------------------------------------

		SettingRow Header(std::string a_label = {}) {

			return { .kind = SettingKind::kHeader, .id = "header", .label = std::move(a_label) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Spacer ) ---------------------
		//---------------------------------------------------

		SettingRow Spacer() {

			return { .kind = SettingKind::kSpacer, .id = "spacer" };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Toggle ) ---------------------
		//---------------------------------------------------

		SettingRow Toggle(std::string a_id, std::string a_label, std::string a_value, std::string a_enabledBy = {}, std::string a_action = {}) {

			return { .kind = SettingKind::kToggle, .id = std::move(a_id), .label = std::move(a_label), .value = std::move(a_value), .enabledBy = std::move(a_enabledBy), .action = std::move(a_action) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Choice ) ---------------------
		//---------------------------------------------------

		SettingRow Choice(std::string a_id, std::string a_label, std::string a_value, std::vector<std::string> a_options, std::string a_info = {}, std::string a_enabledBy = {}) {

			return { .kind = SettingKind::kChoice, .id = std::move(a_id), .label = std::move(a_label), .info = std::move(a_info), .value = std::move(a_value), .options = std::move(a_options), .enabledBy = std::move(a_enabledBy) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Slider ) ---------------------
		//---------------------------------------------------

		SettingRow Slider(std::string a_id, std::string a_label, std::string a_value, std::int32_t a_min, std::int32_t a_max, std::int32_t a_step, std::string a_info = {}) {

			return { .kind = SettingKind::kSlider, .id = std::move(a_id), .label = std::move(a_label), .info = std::move(a_info), .value = std::move(a_value), .minimum = a_min, .maximum = a_max, .step = a_step };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Colour ) ---------------------
		//---------------------------------------------------

		SettingRow Colour(std::string a_id, std::string a_label, std::string a_value, std::string a_text, std::string a_info = {}, std::string a_enabledBy = {}, std::string a_action = {}) {

			return { .kind = SettingKind::kColour, .id = std::move(a_id), .label = std::move(a_label), .info = std::move(a_info), .value = std::move(a_value), .text = std::move(a_text), .enabledBy = std::move(a_enabledBy), .action = std::move(a_action) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Hud Colour ) -----------------
		//---------------------------------------------------

		// HUD colours, the picker replaces the typed in custom colour
		SettingRow HudColour(std::string a_id, std::string a_label, std::string a_suffix, std::string a_info) {

			return { .kind = SettingKind::kColour, .id = std::move(a_id), .label = std::move(a_label), .info = std::move(a_info), .value = "State_ColourVal_" + a_suffix, .text = "State_ColourString_" + a_suffix, .colour = std::move(a_suffix) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Input ) ----------------------
		//---------------------------------------------------

		SettingRow Input(std::string a_id, std::string a_label, std::string a_value, std::string a_fallback) {

			return { .kind = SettingKind::kInput, .id = std::move(a_id), .label = std::move(a_label), .value = std::move(a_value), .fallback = std::move(a_fallback) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Button ) ---------------------
		//---------------------------------------------------

		SettingRow Button(std::string a_id, std::string a_label, std::string a_button, std::string a_action, std::string a_enabledBy = {}) {

			return { .kind = SettingKind::kButton, .id = std::move(a_id), .label = std::move(a_label), .enabledBy = std::move(a_enabledBy), .action = std::move(a_action), .button = std::move(a_button) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Dynamic ) --------------------
		//---------------------------------------------------

		SettingRow Dynamic(std::string a_id, std::string a_enabledBy = {}) {

			return { .kind = SettingKind::kText, .id = std::string(dynamicRowPrefix) + a_id, .enabledBy = std::move(a_enabledBy) };
		}

		//---------------------------------------------------
		//-- Settings Layout ( Keys ) -----------------------
		//---------------------------------------------------

		std::vector<std::string> Keys(const char* a_prefix, int a_count) {

			std::vector<std::string> keys;
			for (int i = 0; i < a_count; ++i) {
				keys.push_back(a_prefix + std::to_string(i));
			}
			return keys;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Interface Page ) -------------
		//---------------------------------------------------

		// UI Settings
		SettingsPageLayout InterfacePage() {

			constexpr auto logColours = "CompletionLog_ColoursEnabled";

			SettingsPageLayout page{ "interface", "$MCMPageSettings2", {}, {} };
			page.columns[0] = {
				Header("$State_MapMarkerSettingsHead"),
				Slider("State_MarkerDetection", "$State_MarkerDetectionText", "State_MarkerDetectionVal", 100, 3000, 100, "$State_MarkerDetectionInfo"),
				Spacer(),
				Header("$MCMPageSettingsHeader4"),
				Toggle("CompletionLog_ColoursEnabledState", "$CompletionLog_ColoursEnabledText", logColours),
				Colour("CompletionLog_QuestsTextColourState", "$CompletionLog_QuestsTextColourText", "CompletionLog_QuestsTextColourVal", "CompletionLog_QuestsTextColourString", "$CompletionLog_QuestsTextColourInfo", logColours),
				Colour("CompletionLog_ItemsTextColourState", "$CompletionLog_ItemsTextColourText", "CompletionLog_ItemsTextColourVal", "CompletionLog_ItemsTextColourString", "$CompletionLog_ItemsTextColourInfo", logColours),
			};

			page.columns[1] = {
				Header(),
				Button("State_MarkerDetectionCancel", "$State_MarkerDetectionCancelText", "UI_Setting_RemoveMarker", "markerCancel"),
				Spacer(),
				Header(),
				Toggle("CompletionLog_PrefixEnabledState", "$CompletionLog_PrefixEnabledText", "CompletionLog_PrefixEnabled"),
				Colour("CompletionLog_BooksTextColourState", "$CompletionLog_BooksTextColourText", "CompletionLog_BooksTextColourVal", "CompletionLog_BooksTextColourString", "$CompletionLog_BooksTextColourInfo", logColours),
				Colour("CompletionLog_SpecialTextColourState", "$CompletionLog_SpecialTextColourText", "CompletionLog_SpecialTextColourVal", "CompletionLog_SpecialTextColourString", "$CompletionLog_SpecialTextColourInfo", logColours),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Notifications Page ) ---------
		//---------------------------------------------------

		// Notifications
		SettingsPageLayout NotificationsPage() {

			SettingsPageLayout page{ "notifications", "UI_SettingsSection_Notifications", {}, {} };
			page.columns[0] = {
				Header("$NotificationSettingsText"),
				Toggle("ModNotificationsItems", "$ModNotificationsItems", "NotifyItems"),
				Toggle("ModNotificationsBooks", "$ModNotificationsBooks", "NotifyBooks"),
				Toggle("ModNotificationsColourEnabled", "$ModNotificationsColourEnabled", "NotificationColourEnabled"),
			};
			page.columns[1] = {
				Header(),
				Toggle("ModNotificationsSpecial", "$ModNotificationsSpecial", "NotifySpecial"),
				Toggle("ModNotificationsStartup", "$ModNotificationsStartup", "NotifyStartup"),
				Colour("ModNotificationsColour", "$ModNotificationsColour", "IndexColour", "ColourString", "$ModNotificationsColourInfo", "NotificationColourEnabled"),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Hud Page ) -------------------
		//---------------------------------------------------

		// HUD Settings
		SettingsPageLayout HudPage() {

			const auto side = [](const char* a_tag, const char* a_fallback) {
				const std::string t = a_tag;
				return std::vector<SettingRow>{
					Header("$MainHUDMode_Header_" + t),
					Input("OverRide_" + t + "_Name", "$State_OverRide_" + t + "_Name_Text", "State_OverRide_" + t + "_Name_String", a_fallback),
					HudColour("ColourState_" + t + "_HUD_Crosshair", "$ColourState_" + t + "_Generic_Text_Crosshair", t + "_HUD_Crosshair", "$ColourState_" + t + "_Generic_Info_Crosshair"),
					Spacer(),
					Header("$MenuHUDMode_Header_" + t),
					Choice("InventoryModeOptions_PrAp_" + t, "$InventoryModeOptions_PrAp_Text_" + t, "iMenuFormat_" + t, Keys("$InventoryMode_PrAp_Menu", 8), "$InventoryModeOptions_PrAp_Info_" + t),
					Choice("InventoryModeOptions_PrFx_" + t, "$InventoryModeOptions_PrFx_Text_" + t, "iMenuPrefix_" + t, Keys("$InventoryMode_PrFx_Menu", 3), "$InventoryModeOptions_PrFx_Info_" + t),
					HudColour("ColourState_" + t + "_HUD_Menus", "$ColourState_" + t + "_Generic_Text_Menus", t + "_HUD_Menus", "$ColourState_" + t + "_Generic_Info_Menus"),
				};
			};

			SettingsPageLayout page{ "hud", "$MCMPageSettings3", {}, {} };
			page.columns[0] = side("N", "Need It!");
			page.columns[1] = side("G", "Got It!");

			// Item Icons
			constexpr auto iconsOn = "bItemIcons";
			auto size = Slider("ItemIcons_Size", "UI_ItemIcons_Size", "iItemIcons_Size", 50, 200, 5, "UI_ItemIcons_SizeInfo");
			auto opacity = Slider("ItemIcons_Opacity", "UI_ItemIcons_Opacity", "iItemIcons_Opacity", 20, 100, 5, "UI_ItemIcons_OpacityInfo");
			size.enabledBy = iconsOn;
			opacity.enabledBy = iconsOn;
			page.columns[0].insert(page.columns[0].end(), {
				Spacer(),
				Header("UI_ItemIcons_Header"),
				Toggle("ItemIcons_Show", "UI_ItemIcons_Show", iconsOn),
				size,
				opacity,
			});
			page.columns[1].insert(page.columns[1].end(), {
				Spacer(),
				Header(),
				Toggle("ItemIcons_Needed", "UI_ItemIcons_Needed", "bItemIcons_Needed", iconsOn),
				Toggle("ItemIcons_Found", "UI_ItemIcons_Found", "bItemIcons_Found", iconsOn),
				Toggle("ItemIcons_Museum", "UI_ItemIcons_Museum", "bItemIcons_Museum", iconsOn),
			});

			// Completionist Sort
			const auto priority = [](const char* a_id, const char* a_label, const char* a_value) {
				return Slider(a_id, a_label, a_value, 1, 5, 1, "UI_Sort_PriorityInfo");
			};
			page.columns[0].insert(page.columns[0].end(), {
				Spacer(),
				Header("UI_Sort_Header"),
				priority("Sort_Needed", "UI_Sort_Needed", "iSort_Needed"),
				priority("Sort_Found", "UI_Sort_Found", "iSort_Found"),
				Toggle("Sort_Bands", "UI_Sort_Bands", "bSort_Bands"),
			});
			page.columns[1].insert(page.columns[1].end(), {
				Spacer(),
				Header(),
				priority("Sort_Displayable", "UI_Sort_Displayable", "iSort_Displayable"),
				priority("Sort_Occupied", "UI_Sort_Occupied", "iSort_Occupied"),
				priority("Sort_Displayed", "UI_Sort_Displayed", "iSort_Displayed"),
			});
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Museum Page ) ----------------
		//---------------------------------------------------

		// Museum Mode, Legacy only
		SettingsPageLayout MuseumPage() {

			const auto tag = [](const char* a_name, const char* a_fallback) {
				const std::string n = a_name;
				return std::vector<SettingRow>{
					Input("OverRide_" + n + "_Name", "$State_OverRide_" + n + "_Name_Text", "State_OverRide_" + n + "_Name_String", a_fallback),
					HudColour("ColourState_" + n, "$ColourState_" + n + "_Text", n, "$ColourState_" + n + "_Info"),
					Choice("InventoryModeOptions_PrAp_" + n, "$InventoryModeOptions_PrAp_Text_" + n, "iMenuFormat_" + n, Keys("$InventoryMode_PrAp_Menu", 8), "$InventoryModeOptions_PrAp_Info_" + n),
					Choice("InventoryModeOptions_PrFx_" + n, "$InventoryModeOptions_PrFx_Text_" + n, "iMenuPrefix_" + n, Keys("$InventoryMode_PrFx_Menu", 3), "$InventoryModeOptions_PrFx_Info_" + n),
				};
			};

			SettingsPageLayout page{ "museum", "$MCMPageSettings6", "LegacyoftheDragonborn.esm", {} };
			page.columns[0] = {
				Header("$MenuHUDMode_Header_DB"),
				Toggle("State_MuseumModeEnabled", "$State_MuseumModeEnabled_Text", "MuseumModeEnabled"),
				Toggle("State_TreatOccupiedAsDisplayed", "$State_TreatOccupiedAsDisplayed_Text", "TreatOccupiedAsDisplayed"),
				Spacer(),
				Header("$MenuHUDMode_Header_N_DB"),
			};
			for (auto& row : tag("Displayable", "Displayable!")) {
				page.columns[0].push_back(std::move(row));
			}

			page.columns[1] = { Header("$MenuHUDMode_Header_G_DB") };
			for (auto& row : tag("Displayed", "Displayed!")) {
				page.columns[1].push_back(std::move(row));
			}
			page.columns[1].push_back(Spacer());
			for (auto& row : tag("Occupied", "Occupied!")) {
				page.columns[1].push_back(std::move(row));
			}
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Cell Scanner Page ) ----------
		//---------------------------------------------------

		// Cell Scanner exclusions
		SettingsPageLayout CellScannerPage() {

			SettingsPageLayout page{ "cellscanner", "$MCMPageSettings4", {}, {} };
			page.columns[0] = {
				Header("$CellScanner_ExcludedHeader"),
				Dynamic("excludedCells"),
			};
			page.columns[1] = {
				Header("$CellScanner_ExcludedReferencesHeader"),
				Dynamic("excludedReferences"),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Cell Window Page ) -----------
		//---------------------------------------------------

		// Cell Scanner window options, not a tab
		SettingsPageLayout CellWindowPage() {

			constexpr auto pinning = "bCellScanner_Pinning_Enabled";

			SettingsPageLayout page{ "cellwindow", "UI_Cell_Title", {}, {} };
			page.tab = false;
			page.columns[0] = {
				Header("$CellScanner_SettingsHeader2"),
				Slider("CellScanner_Setting_Range", "UI_Cell_Range", "iCellScanner_Range", 10, 500, 1, "$CellScanner_Setting_Range_Info"),
				Header("$CellScanner_SettingsHeader4"),
				Toggle("CellScanner_Setting_ExcludeAlchemy", "$CellScanner_Setting_ExcludeAlchemy_Text", "bCellScanner_ExcludeAlchemy"),
				Toggle("CellScanner_Setting_ExcludeIngredients", "$CellScanner_Setting_ExcludeIngredients_Text", "bCellScanner_ExcludeIngredients"),
				Toggle("CellScanner_Setting_ExcludeArmor", "$CellScanner_Setting_ExcludeArmor_Text", "bCellScanner_ExcludeArmor"),
				Toggle("CellScanner_Setting_ExcludeBooks", "$CellScanner_Setting_ExcludeBooks_Text", "bCellScanner_ExcludeBooks"),
				Toggle("CellScanner_Setting_ExcludeMiscItems", "$CellScanner_Setting_ExcludeMiscItems_Text", "bCellScanner_ExcludeMiscItems"),
				Toggle("CellScanner_Setting_ExcludeWeapons", "$CellScanner_Setting_ExcludeWeapons_Text", "bCellScanner_ExcludeWeapons"),
				Toggle("CellScanner_Setting_ExcludeOwned", "UI_Cell_ExcludeOwned", "bCellScanner_ExcludeOwned"),
				Toggle("CellScanner_Setting_ExcludeLiving", "UI_Cell_ExcludeLiving", "bCellScanner_ExcludeLiving"),
				Header("$CellScanner_SettingsHeader5"),
				Toggle("CellScanner_Setting_Pinning_Enabled", "$CellScanner_Setting_Pinning_Enabled_Text", pinning, {}, "pinningEnabled"),
				Toggle("CellScanner_Setting_Pinning_Marker", "$CellScanner_Setting_Pinning_Marker_Text", "bCellScanner_Pinning_Marker", pinning, "pinningMarker"),
				Toggle("CellScanner_Setting_Pinning_Effect", "$CellScanner_Setting_Pinning_Effect_Text", "bCellScanner_Pinning_Effect", pinning, "pinningEffect"),
				Toggle("CellScanner_Setting_Pinning_Notify", "UI_Cell_PinNotify", "bCellScanner_Pinning_Notify", pinning),
				Choice("State_CellScanner_Pinning_Sound", "UI_Cell_Sound", "CellScanner_Pinning_Sound_Choice", Keys("$CellScannerSoundList", 3), "$CellScanner_Pinning_Sound_Info", pinning),
				Dynamic("pinnedItem", pinning),
				Button("CellScanner_Setting_Pinning_ClearPin", "", "UI_Cell_ClearPin", "clearPin", pinning),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Add Help Texts ) -------------
		//---------------------------------------------------

		// MCM highlight texts
		void AddHelpTexts(std::vector<SettingsPageLayout>& a_layouts) {

			static const std::unordered_map<std::string_view, std::string_view> help{
			{ "Achievements_Show", "UI_Achievements_SettingShowInfo" },
			{ "Ingredients_Show", "UI_Ingredients_SettingShowInfo" },
			{ "Ingredients_NameUnknown", "UI_Ingredients_SettingNameUnknownInfo" },
			{ "Achievements_Progress", "UI_Achievements_SettingProgressInfo" },
			{ "Achievements_Tips", "UI_Achievements_SettingTipsInfo" },
			{ "Achievements_Rarity", "UI_Achievements_SettingRarityInfo" },
			{ "Achievements_Missable", "UI_Achievements_SettingMissableInfo" },
			{ "CellScanner_Setting_ExcludeAlchemy", "$CellScanner_Setting_ExcludeAlchemy_Info" },
			{ "CellScanner_Setting_ExcludeIngredients", "$CellScanner_Setting_ExcludeIngredients_Info" },
			{ "CellScanner_Setting_ExcludeArmor", "$CellScanner_Setting_ExcludeArmor_Info" },
			{ "CellScanner_Setting_ExcludeBooks", "$CellScanner_Setting_ExcludeBooks_Info" },
			{ "CellScanner_Setting_ExcludeMiscItems", "$CellScanner_Setting_ExcludeMiscItems_Info" },
			{ "CellScanner_Setting_ExcludeWeapons", "$CellScanner_Setting_ExcludeWeapons_Info" },
			{ "CellScanner_Setting_ExcludeOwned", "UI_Cell_ExcludeOwnedInfo" },
			{ "CellScanner_Setting_ExcludeLiving", "UI_Cell_ExcludeLivingInfo" },
			{ "CellScanner_Setting_Pinning_ClearPin", "$CellScanner_Setting_Pinning_ClearPin_Info" },
			{ "CellScanner_Setting_Pinning_Effect", "$CellScanner_Setting_Pinning_Effect_Info" },
			{ "CellScanner_Setting_Pinning_Enabled", "$CellScanner_Setting_Pinning_Enabled_Info" },
			{ "CellScanner_Setting_Pinning_Marker", "$CellScanner_Setting_Pinning_Marker_Info" },
			{ "CellScanner_Setting_Pinning_Notify", "UI_Cell_PinNotifyInfo" },
			{ "CellScanner_Setting_Range", "$CellScanner_Setting_Range_Info" },
			{ "ColourState_Displayable", "$ColourState_Displayable_Info" },
			{ "ColourState_Displayed", "$ColourState_Displayed_Info" },
			{ "ColourState_G_HUD_Crosshair", "$ColourState_G_Generic_Info_Crosshair" },
			{ "ColourState_G_HUD_Menus", "$ColourState_G_Generic_Info_Menus" },
			{ "ColourState_N_HUD_Crosshair", "$ColourState_N_Generic_Info_Crosshair" },
			{ "ColourState_N_HUD_Menus", "$ColourState_N_Generic_Info_Menus" },
			{ "ColourState_Occupied", "$ColourState_Occupied_Info" },
			{ "CompletionLog_BooksTextColourState", "$CompletionLog_BooksTextColourInfo" },
			{ "CompletionLog_ColoursEnabledState", "$CompletionLog_ColoursEnabledInfo" },
			{ "CompletionLog_ItemsTextColourState", "$CompletionLog_ItemsTextColourInfo" },
			{ "CompletionLog_PrefixEnabledState", "$CompletionLog_PrefixEnabledInfo" },
			{ "CompletionLog_QuestsTextColourState", "$CompletionLog_QuestsTextColourInfo" },
			{ "CompletionLog_SpecialTextColourState", "$CompletionLog_SpecialTextColourInfo" },
			{ "ItemIcons_Found", "UI_ItemIcons_FoundInfo" },
			{ "ItemIcons_Museum", "UI_ItemIcons_MuseumInfo" },
			{ "ItemIcons_Needed", "UI_ItemIcons_NeededInfo" },
			{ "ItemIcons_Show", "UI_ItemIcons_ShowInfo" },
			{ "ModNotificationsBooks", "$ModNotificationsBooksInfo" },
			{ "ModNotificationsColour", "$ModNotificationsColourInfo" },
			{ "ModNotificationsColourEnabled", "$ModNotificationsColourEnabledInfo" },
			{ "ModNotificationsItems", "$ModNotificationsItemsInfo" },
			{ "ModNotificationsSpecial", "$ModNotificationsSpecialInfo" },
			{ "ModNotificationsStartup", "$ModNotificationsStartupInfo" },
			{ "OverRide_Displayable_Name", "$State_OverRide_Displayable_Name_Info" },
			{ "OverRide_Displayed_Name", "$State_OverRide_Displayed_Name_Info" },
			{ "OverRide_G_Name", "$State_OverRide_G_Name_Info" },
			{ "OverRide_N_Name", "$State_OverRide_N_Name_Info" },
			{ "OverRide_Occupied_Name", "$State_OverRide_Occupied_Name_Info" },
			{ "Sort_Bands", "UI_Sort_BandsInfo" },
			{ "State_CellScanner_Pinning_Sound", "$CellScanner_Pinning_Sound_Info" },
			{ "State_MarkerDetection", "$State_MarkerDetectionInfo" },
			{ "State_MarkerDetectionCancel", "$State_MarkerDetectionCancelInfo" },
			{ "State_MuseumModeEnabled", "$State_MuseumModeEnabled_Info" },
			{ "State_TreatOccupiedAsDisplayed", "$State_TreatOccupiedAsDisplayed_Info" },
			};
			for (auto& layout : a_layouts) {
				for (auto& column : layout.columns) {
					for (auto& row : column) {
						if (const auto it = help.find(row.id); row.info.empty() && it != help.end()) {
							row.info = it->second;
						}
					}
				}
			}
		}

		//---------------------------------------------------
		//-- Settings Layout ( Achievements Page ) ----------
		//---------------------------------------------------

		// What counts as unlocked, and what the details show
		SettingsPageLayout AchievementsPage() {

			SettingsPageLayout page{ "achievements", "UI_SettingsSection_Achievements", {}, {} };
			page.columns[0] = {
				Header("UI_Achievements_SettingsTracking"),
				Toggle("Achievements_Show", "UI_Achievements_SettingShow", "AchievementsShow"),
			};
			page.columns[1] = {
				Header("UI_Achievements_SettingsDetails"),
				Toggle("Achievements_Progress", "UI_Achievements_SettingProgress", "AchievementsProgress", "AchievementsShow"),
				Toggle("Achievements_Tips", "UI_Achievements_SettingTips", "AchievementsTips", "AchievementsShow"),
				Toggle("Achievements_Rarity", "UI_Achievements_SettingRarity", "AchievementsRarity", "AchievementsShow"),
				Toggle("Achievements_Missable", "UI_Achievements_SettingMissable", "AchievementsMissable", "AchievementsShow"),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Ingredients Page ) -----------
		//---------------------------------------------------

		// The page, and whether effects not learnt yet are named
		SettingsPageLayout IngredientsPage() {

			SettingsPageLayout page{ "ingredients", "UI_SettingsSection_Ingredients", {}, {} };
			page.columns[0] = {
				Header("UI_Ingredients_SettingsTracking"),
				Toggle("Ingredients_Show", "UI_Ingredients_SettingShow", "IngredientsShow"),
			};
			page.columns[1] = {
				Header("UI_Ingredients_SettingsDetails"),
				Toggle("Ingredients_NameUnknown", "UI_Ingredients_SettingNameUnknown", "IngredientsNameUnknown", "IngredientsShow"),
			};
			return page;
		}

		//---------------------------------------------------
		//-- Settings Layout ( Completion Log Page ) --------
		//---------------------------------------------------

		// Completion Log
		SettingsPageLayout CompletionLogPage() {

			SettingsPageLayout page{ "log", "$MCMPageSettings5", {}, {} };
			page.columns[0] = { Dynamic("logDate"), Spacer(), Dynamic("logEntries") };
			page.singleColumn = true;
			return page;
		}
	}

	//---------------------------------------------------
	//-- Settings Layout ( MCM Settings Layouts ) -------
	//---------------------------------------------------

	const std::vector<SettingsPageLayout>& MCMSettingsLayouts() {

		static const std::vector<SettingsPageLayout> layouts = [] {
			std::vector<SettingsPageLayout> pages{ NotificationsPage(), HudPage(), MuseumPage(), InterfacePage(), CellScannerPage(), CompletionLogPage(), CellWindowPage(), AchievementsPage(), IngredientsPage() };
			AddHelpTexts(pages);
			return pages;
		}();
		return layouts;
	}

	//---------------------------------------------------
	//-- Settings Layout ( Find Setting Row ) -----------
	//---------------------------------------------------

	const SettingRow* FindSettingRow(std::string_view a_pageID, std::string_view a_rowID) {

		for (const auto& page : MCMSettingsLayouts()) {
			if (page.id != a_pageID) {
				continue;
			}
			for (const auto& column : page.columns) {
				for (const auto& row : column) {
					if (row.id == a_rowID) {
						return std::addressof(row);
					}
				}
			}
		}
		return nullptr;
	}

	//---------------------------------------------------
	//-- Settings Layout ( Hex Colour ) -----------------
	//---------------------------------------------------

	std::string HexColour(std::uint32_t a_colour) {

		if (!a_colour) {
			return {};
		}
		char buffer[16]{};
		std::snprintf(buffer, sizeof(buffer), "#%X", a_colour);
		return buffer;
	}

	//---------------------------------------------------
	//-- Settings Layout ( Make Setting Item ) ----------
	//---------------------------------------------------

	SettingItem MakeSettingItem(const SettingRow& a_row, SettingValues& a_values) {

		SettingItem item{ .id = a_row.id, .kind = a_row.kind, .label = a_row.label, .info = a_row.info };
		if (!a_row.enabledBy.empty()) {
			const bool inverted = a_row.enabledBy.front() == '!';
			item.enabled = a_values.GetBool(inverted ? a_row.enabledBy.substr(1) : a_row.enabledBy) != inverted;
		}

		switch (a_row.kind) {
		case SettingKind::kToggle:
			item.on = a_values.GetBool(a_row.value);
			break;
		case SettingKind::kChoice:
			item.options = a_row.options;
			item.choice = a_values.IsBool(a_row.value) ? (a_values.GetBool(a_row.value) ? 1 : 0) : a_values.GetInt(a_row.value);
			break;
		case SettingKind::kSlider:
			item.number = a_values.GetInt(a_row.value);
			item.minimum = a_row.minimum;
			item.maximum = a_row.maximum;
			item.step = a_row.step;
			break;
		case SettingKind::kColour:
			// Custom colour wins, as in the MCM
			if (!a_row.colour.empty() && a_values.GetBool("b_CustomColour_" + a_row.colour)) {
				item.number = a_values.GetInt("State_CustomColourVal_" + a_row.colour);
			} else {
				item.number = a_values.GetInt(a_row.value);
			}
			break;
		case SettingKind::kInput:
			item.value = a_values.GetString(a_row.value);
			break;
		case SettingKind::kKey:
			item.number = a_values.GetInt(a_row.value);
			item.value = item.number >= 0 ? a_values.KeyName(item.number) : std::string{};
			break;
		case SettingKind::kButton:
			item.value = a_row.button;
			item.confirm = a_row.confirm;
			break;
		default:
			break;
		}
		return item;
	}

	//---------------------------------------------------
	//-- Settings Layout ( Write Setting Item ) ---------
	//---------------------------------------------------

	void WriteSettingItem(const SettingRow& a_row, const SettingItem& a_item, SettingValues& a_values) {

		switch (a_row.kind) {
		case SettingKind::kToggle:
			a_values.SetBool(a_row.value, a_item.on);
			break;
		case SettingKind::kChoice:
			if (a_values.IsBool(a_row.value)) {
				a_values.SetBool(a_row.value, a_item.choice == 1);
			} else {
				a_values.SetInt(a_row.value, a_item.choice);
			}
			if (!a_row.text.empty() && a_item.choice >= 0 && a_item.choice < static_cast<std::int32_t>(a_row.options.size())) {
				a_values.SetString(a_row.text, a_row.options[a_item.choice]);
			}
			break;
		case SettingKind::kSlider:
			a_values.SetInt(a_row.value, std::clamp(a_item.number, a_row.minimum, a_row.maximum));
			break;
		case SettingKind::kColour:
			a_values.SetInt(a_row.value, a_item.number);
			a_values.SetString(a_row.text, HexColour(static_cast<std::uint32_t>(a_item.number)));
			if (!a_row.colour.empty()) {
				// Picking a colour clears the custom one
				a_values.SetBool("b_CustomColour_" + a_row.colour, false);
				a_values.SetInt("State_CustomColourVal_" + a_row.colour, -1);
				a_values.SetString("State_CustomColourString_" + a_row.colour, "Enter Decimal");
			}
			break;
		case SettingKind::kInput:
			a_values.SetString(a_row.value, a_item.value.empty() ? a_row.fallback : a_item.value);
			break;
		case SettingKind::kKey:
			a_values.SetInt(a_row.value, a_item.number);
			break;
		default:
			break;
		}
	}
}
