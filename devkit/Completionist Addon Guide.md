# Making a Completionist Addon

This guide shows you how to add another mod's items, books, places, quests and more to Completionist.
You don't need to write any code. An addon is two text files.

- **New here?** Read sections 1 to 4, then copy `CompletionistAddon_Template.ini` and `CompletionistAddon_Template.txt` and change them.
- **Looking something up?** Sections 5 to 13 list every setting.

## Contents

1. What an addon is
2. Where the files go
3. The quickest start: the xEdit script
4. How the .ini file works
5. The `[Completionist Patch Data]` section
6. Pages, sections and the Completionist window
7. Section settings
8. FormIDs: what to write in the list
9. Settings for one entry
10. The Types, one by one
11. Quests
12. Install conditions
13. The filter drop-down
14. The translation file
15. Testing your addon
16. Common mistakes
17. Old settings you can delete

---

## 1. What an addon is

An addon is two files that have the same name:

| File | What it holds |
|---|---|
| `MyMod.ini` | **What** to track: the FormIDs, how they're grouped, how each one is completed. |
| `MyMod.txt` | **What the player reads**: page names, headings, item names, hint text. |

The .ini never holds the text the player sees. It holds a **key** instead, such as `cTranslate_PageName1`.
Completionist looks that key up in the .txt file and shows the text it finds there.
This way someone can translate your addon by changing only the .txt file.

Give your files a name that no other addon is likely to use, such as `AuthorName_ModName.ini`.

## 2. Where the files go

| File | Folder |
|---|---|
| `.ini` | `Data\SKSE\Plugins\CompletionistData\Addons\` |
| `.txt` | `Data\SKSE\Plugins\CompletionistData\Translations\` |

- Completionist reads every `.ini` in the Addons folder, and in any folder inside it.
- The .txt must have the same name as the .ini (`MyMod.ini` → `MyMod.txt`).
  To use a different name, or to share one .txt between several .ini files, see `OverrideLocalisationFileName` in section 5.
- Addons are read when the game starts. After you change a file, restart the game.

## 3. The quickest start: the xEdit script

Completionist Addons comes with an xEdit script, `Completionist_Output_Patch.pas`, that writes a first draft for you.

1. Put the script in xEdit's `Edit Scripts` folder.
2. In xEdit, select the records you want to track (or a whole plugin).
3. Right-click → **Apply Script** → `Completionist_Output_Patch`.
4. It writes `<PluginName>.ini` and `<PluginName>.txt` to `Completionist Patches\` in xEdit's folder.

The script sorts records into weapons, armour, books, keys, map markers, quests and so on.
Then go through the draft:

- Remove anything that shouldn't be tracked.
- Fill in the quest hint text. The script writes `Empty` for every quest.
- Check each quest's `QuestType` and `CompletionType`. The script sets every quest to a side quest that completes the normal way.
- Add display names, hints and variations where they help (section 9).

## 4. How the .ini file works

An .ini file is made of **sections** and **settings**.

```ini
[Section Name]
Setting = value
```

A few rules to know:

- **Comments go on their own line** and start with `;`.
  Don't put a comment after a value: `Type = 0 ; items` makes the value `0 ; items`.
  The only place a comment may follow a value is inside a FormIDs list (section 8).
- **Setting names don't care about upper or lower case.** `EditorID` and `EditorId` are the same.
- **Every section needs its own name.** Two sections with the same name are merged into one.
- **Every section needs `Enabled = true`**, or it is skipped. This includes the top section and each quest.
- **True or false** can be written `true`/`false`, `yes`/`no`, `on`/`off` or `1`/`0`.
- Spaces around `=` don't matter.

## 5. The `[Completionist Patch Data]` section

This section comes first. It describes the whole addon.

| Setting | Needed? | What it does |
|---|---|---|
| `Enabled` | **Yes** | `true` to load the addon. Leaving it out counts as `false`. |
| `PluginFileName` | **Yes** | The plugin most of your FormIDs come from, with its extension, e.g. `MyMod.esp`. If this plugin isn't in the load order, the whole addon is skipped. |
| `MCMPageName` | **Yes** | Translation key for the page's name in the Completionist window's list of pages. |
| `PageName1`, `PageName2`, … | **Yes** (at least `PageName1`) | Translation keys for the tabs on your page. Number them 1, 2, 3 with no gaps: Completionist stops counting at the first missing number. See section 6. |
| `PageName1_InstallCondition`, … | No | Only show that tab when a condition is met. See section 12. |
| `InstallCondition` | No | Only load the addon when a condition is met. See section 12. |
| `Priority` | No | Where your page sits in the list. Lower numbers come first. Pages with the same priority are sorted by name. Default `0`. |
| `Header` | No | Translation key for a heading in the list of pages. Addons with the same heading text are listed together under it. Without a header, your page goes under "Mod Added Patches" (or "Vanilla Tracking", see the next row). |
| `IsVanillaTracking` | No | `true` only for addons that track the base game or Creation Club content. Those pages are listed first, in the Collections group. Leave it `false` for mods. |
| `SortMode` | No | How entries are ordered in each section. `0` = A to Z (default). `1` = A to Z, ignoring a leading "The", "A" or "An". `2` = the order you wrote them in the FormIDs list. |
| `OverrideLocalisationFileName` | No | The name of the translation file to use instead of `<ini name>.txt`, e.g. `MyMods_Shared.txt`. Several .ini files can share one .txt this way, as long as all their keys are different. |
| `Log` | No | `true` writes what happened to each entry to Completionist's log. Use it while you make the addon, and set it back to `false` before you release it. See section 15. |
| `DropDownMenu`, `DropDownMenu_Name`, `DropDownMenu_Highlight`, `DropDownMenu_Option_1`, … | No | A drop-down that filters the page. See section 13. |

## 6. Pages, sections and the Completionist window

Each addon .ini gets **one page** in the Completionist window.

- `MCMPageName` is the page's name in the list on the left.
- `PageName1`, `PageName2`, … are the **tabs** across the top of the page. With only one tab, the page shows no tab bar.
- Every other `[section]` in the .ini is a **list with a heading** on one of those tabs. `DisplayOnPage` says which tab.

```
MCMPageName  →  "Bloodchill Armoury"        (the page)
PageName1    →  "Gear"                      (tab 1)
   [Weapons]     DisplayOnPage = 1  →  "Weapons" list on tab 1
   [Armour]      DisplayOnPage = 1  →  "Armour" list on tab 1
PageName2    →  "Quests"                    (tab 2)
   [Quest list]  DisplayOnPage = 2, Type = 5 →  the quests on tab 2
```

- A list with nothing in it (its plugin is missing, or none of its FormIDs were found) is removed.
  A tab with no lists left is removed too, and the other tabs move up.
- There is no limit on how many entries a list or a tab can hold.
- Quests are shown the same way as Completionist's own quest pages: Main, Side, Radiant and Completed.

## 7. Section settings

Use these in every section except quest sections (quests are in section 11).

| Setting | Needed? | What it does |
|---|---|---|
| `Enabled` | **Yes** | `true` to load this section. |
| `Type` | **Yes** | What kind of thing the list holds, from `0` to `9`. See section 10. One section holds one Type. |
| `DisplayOnPage` | **Yes** | Which tab the list goes on: `1` for `PageName1`, `2` for `PageName2`, and so on. It must be a tab you named. |
| `SectionHeader_L` | **Yes** | Translation key for the list's heading. |
| `SectionHeader_R` | No | Translation key for the right-hand heading in older versions of Completionist ("Found", "Read"). The current window doesn't show it. |
| `FormIDs` | **Yes** (not for Type 5) | The things to track. See section 8. |
| `InstallFrom` | No | The plugin this section's FormIDs come from, if it isn't `PluginFileName`. |
| `InstallCondition` | No | Only load this section when a condition is met. See section 12. |

## 8. FormIDs: what to write in the list

### 8.1 Write the FormID without the load order

In xEdit a FormID looks like `FE000807` or `0A01ABCD`.
The first part is the plugin's place in **your** load order. It's different for every player, so leave it out.
Write `0x` and then the rest:

| Plugin kind | In xEdit | Write |
|---|---|---|
| Normal .esp / .esm | `0A01ABCD` (first 2 digits are the load order) | `0x01ABCD` |
| Light plugin (.esl, or ESL-flagged) | `FE000807` (first 5 digits are the load order) | `0x000807` |

Leading zeros don't matter to Completionist: `0x807` and `0x000807` find the same record.
But write each FormID the **same way everywhere** in the file, because the entry settings in section 9 use it as a name.

Not sure which part is the load order? Select the record in xEdit and read the bar at the top, e.g. `[FE 000]`.
Mod Organizer 2 also shows it in the Plugins tab, to the right of each plugin.

### 8.2 Writing the list

You can write the list on one line:

```ini
FormIDs = 0x000801,0x000802,0x000803,
```

Or as a list, one per line, with a name after each one so you can find it later:

```ini
FormIDs = <<<LIST
0x000801, ; Iron Spear
0x000802, ; Steel Spear
0x000803, ; Elven Spear
LIST
```

How the list works:

- `<<<LIST` starts the list and a line with just `LIST` ends it.
  You can use another word instead of `LIST` (`<<<WEAP` … `WEAP`), as long as both match.
- **End every FormID with a comma**, including the last one.
- After the comma you can add `; a note`. Completionist ignores it.
  Keep `0x` out of these notes, because Completionist treats any `0x` it finds as a FormID.

### 8.3 FormIDs from another plugin: `0xID*Plugin.esp`

If a FormID comes from a different plugin than `PluginFileName` (or `InstallFrom`), add `*` and that plugin's name:

```ini
FormIDs = <<<LIST
0x01579F*HearthFires.esm, ; Lakeview Manor Charter
0x0157A0*HearthFires.esm, ; Windstad Manor Charter
0x000D63*ccbgssse036-petbwolf.esl, ; Bone Wolf
LIST
```

Each FormID without a `*` comes from the section's `InstallFrom`, or from `PluginFileName` if the section has none.
If that plugin isn't in the load order, the entry is skipped and the rest of the list still loads.

### 8.4 Only load an entry when another mod is or isn't installed

Add one condition to the end of the FormID:

| Write | Meaning |
|---|---|
| `0x000800<!OtherMod.esp,` | Load this entry only if `OtherMod.esp` is **not** installed. |
| `0x000800<&OtherMod.esp,` | Load this entry only if `OtherMod.esp` **is** installed. |
| `0x000800*Patch.esp<&OtherMod.esp,` | Load it from `Patch.esp`, only if `OtherMod.esp` is installed. |

One condition fits here. For more than one, use `0x000800_InstallCondition` (section 9 and 12).

Example: the mod Undeath has a spell tome `0x05839E`, but "Undeath Classical Lichdom" removes it.
`0x05839E<!Undeath_ClassicalLichdom.esp,` tracks the tome only when that add-on isn't installed.

## 9. Settings for one entry

These settings belong to one FormID in a section. Put them in the same section as the FormID, after the list.
Each one starts with the FormID **written exactly as in the list**, then `_` and the setting.

| Setting | Works with Types | What it does |
|---|---|---|
| `0x000800 = …` | 0, 1, 6, 9 | **Variations**: other FormIDs that count as this entry. See below. |
| `0x000800_DisplayName` | all | Translation key for the name to show instead of the record's own name. Only changes what Completionist shows. |
| `0x000800_Highlight` | all | Translation key for a hint shown under the entry, e.g. where to find it. |
| `0x000800_Visibility` | all | Which drop-down option shows this entry: `1` for `DropDownMenu_Option_1`, `2` for option 2, and so on. See section 13. |
| `0x000800_InstallCondition` | all | Only load this entry when a condition is met. See section 12. |
| `0x000800_PickupEnabled` | 6 (Fish) | `true`: picking the fish up also completes it, not only catching it. Default `false`. |
| `0x000800_Type` | 7, 8 | How the home or pet is completed. See section 10. |
| `0x000800_Stage` | 7, 8 | The quest stage that completes the home or pet. |
| `0x000800_OptionalStage` | 7 | Read, but not used yet. Set it to `0`. |

If the entry has a plugin, the setting may use the whole entry as its name:
`0x000800*OtherMod.esp_DisplayName = …`. Completionist looks for that first, then for `0x000800_DisplayName`.
You need the long form only when one list has the same FormID from two plugins.
`_PickupEnabled`, `_Type`, `_Stage` and `_OptionalStage` only work in the short form.

### Variations

Some items come in several versions: levelled copies, enchanted copies, a 1-handed and a 2-handed version.
The Amulet of Articulation has seven. Collect one at level 10, find a different one at level 40, and it would look like a new item.

To count every version as the same entry, list the main FormID in `FormIDs` and the others like this:

```ini
FormIDs = <<<LIST
0x09DFF7*Skyrim.esm, ; Amulet of Articulation
LIST
0x09DFF7*Skyrim.esm = 0x0F6904*Skyrim.esm,0x0F690D*Skyrim.esm,0x0F690E*Skyrim.esm,0x0F690F*Skyrim.esm,0x0F6910*Skyrim.esm,0x0F6911*Skyrim.esm,
```

- Only the main FormID goes in `FormIDs`.
- Write the variations on one line, separated by commas.
- Each variation can have its own `*Plugin.esp` and one `<!` or `<&` condition, like in section 8.
- Finding any version completes the entry.

## 10. The Types, one by one

Each section has one `Type`. It decides what the FormIDs are and how an entry is completed.

The player can choose which Types Completionist tracks (`TrackedEntityTypes` in Completionist's settings file, `Documents\My Games\Skyrim Special Edition\SKSE\Completionist.ini`).
Sections of a Type the player turned off are skipped.

### Type 0: Items

- **List:** the item records themselves: weapons, armour, clothing, jewellery, ammo, potions, food, ingredients, keys, misc items, soul gems.
- **Completed when:** the item is added to the player's inventory.
- **Extras:** variations, `_DisplayName`, `_Highlight`, `_Visibility`.
- Items with no name are skipped, because they can't be picked up.
- Items Completionist already tracks itself are skipped, so you don't need to remove them.
  Their variations are added to Completionist's own entry.

```ini
[Weapons]
Enabled = true
Type = 0
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Weapons
FormIDs = <<<LIST
0x000801, ; Frostbite Spear
0x000805, ; Bloodchill Bow
LIST
0x000801 = 0x000802,0x000803,
0x000805_Highlight = cTranslate_Hint_BloodchillBow
```

### Type 1: Books

- **List:** book records: books, notes, journals, spell tomes, skill books.
- **Completed when:** the player reads the book. If the player turned on "count books when picked up", picking it up counts too.
- **Extras:** variations, `_DisplayName`, `_Highlight`, `_Visibility`.
- If the player set `RestrictBooksToTomesAndSkillBooks`, only spell tomes and skill books load.

```ini
[Books]
Enabled = true
Type = 1
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Books
FormIDs = <<<LIST
0x000810, ; The Frozen Vault
0x000811, ; Spell Tome: Ice Lance
LIST
```

### Type 2: Locations

- **List:** the **map marker reference** in the world (a placed `REFR` whose base object is `MapMarker`, `00000010`). Use the reference's own FormID.
- **Completed when:** the marker is discovered, meaning it shows on the map and the player can fast travel to it.
- **Name:** the map marker's name.
- **Extras:** `_Highlight`, `_Visibility`. The player gets a button to put a quest marker on the place.
- Avoid `_DisplayName` here. Completionist matches the discovery message by the marker's name, so a different name means the place is only marked found the next time the map is opened.

```ini
[Locations]
Enabled = true
Type = 2
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Locations
FormIDs = <<<LIST
0x000820, ; Bloodchill Manor
0x000821, ; Frostmoon Crag
LIST
```

### Type 3: Enchantments

- **List:** the playable base enchantment (called "Object Effect", `ENCH`, in xEdit).
- **Completed when:** the player learns it by disenchanting.
- **Extras:** `_DisplayName`, `_Highlight`, `_Visibility`.

```ini
[Enchantments]
Enabled = true
Type = 3
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Enchantments
FormIDs = 0x000830,0x000831,
```

### Type 4: Shouts

- **List:** the shout record (`SHOU`).
- **Completed when:** the player has learned every word. Each word learned shows on the entry.
  One- and two-word shouts are complete once their words are learned.
- **Extras:** `_DisplayName`, `_Highlight`, `_Visibility`.

```ini
[Shouts]
Enabled = true
Type = 4
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Shouts
FormIDs = 0x000840,
```

### Type 5: Quests

A Type 5 section has **no FormIDs**. It marks the tab where your quests appear.
The quests themselves are their own sections, explained in section 11.

```ini
[Quest list]
Enabled = true
Type = 5
DisplayOnPage = 2
SectionHeader_L = cTranslate_Header_Quests
```

### Type 6: Fish

- **List:** the fish item that goes into the player's inventory.
- **Completed when:** the player catches it with the Fishing Creation.
  With `_PickupEnabled = true`, picking it up any other way counts too. Use that for fish that can't be caught, only found.
- **Extras:** variations, `_DisplayName`, `_Highlight`, `_Visibility`, `_PickupEnabled`.

```ini
[Fish]
Enabled = true
Type = 6
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Fish
FormIDs = <<<LIST
0x000850, ; Frost Pike
0x000851, ; Glacier Crab
LIST
0x000850_Highlight = cTranslate_Hint_FrostPike
0x000851_PickupEnabled = true
```

### Type 7: Player Homes

Each home is completed one of two ways. Set it with `_Type`:

| `_Type` | List this | Completed when |
|---|---|---|
| `0` | the **quest** that hands over the house | the quest reaches the stage in `_Stage` |
| `1` | the house's **key** | the key is added to the player's inventory |

A quest's or key's name rarely reads well as a house name, so give each home a `_DisplayName`.

```ini
[Homes]
Enabled = true
Type = 7
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Homes
FormIDs = <<<LIST
0x000860, ; quest: MyModBuyManorQuest
0x000861, ; key: Frostmoon Cabin Key
LIST
0x000860_Type = 0
0x000860_Stage = 200
0x000860_OptionalStage = 0
0x000860_DisplayName = cTranslate_Home_Manor
0x000861_Type = 1
0x000861_DisplayName = cTranslate_Home_Cabin
```

### Type 8: Pets

Each pet is completed one of two ways. Set it with `_Type`:

| `_Type` | List this | Completed when |
|---|---|---|
| `0` | the pet's **actor base** (`NPC_`) | the player activates (talks to) the pet |
| `1` | the **quest** that gives the pet | the quest reaches the stage in `_Stage` |

```ini
[Pets]
Enabled = true
Type = 8
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Pets
FormIDs = <<<LIST
0x000870, ; Snow Fox (actor)
0x000871, ; quest: MyModAdoptWolfQuest
LIST
0x000870_Type = 0
0x000871_Type = 1
0x000871_Stage = 50
0x000871_DisplayName = cTranslate_Pet_Wolf
```

### Type 9: Interactable objects

- **List:** a **placed object in the world** (a `REFR`): a shrine, a lever, a fishing spot, a statue. Use the reference's own FormID, not its base object's.
- **Completed when:** the player activates it.
- **Name:** the object's name. Many objects share one name, so give each a `_DisplayName` such as "Shrine of Talos, Frostmoon Crag".
- **Extras:** variations (other references that count as the same entry), `_DisplayName`, `_Highlight`, `_Visibility`.
- The reference doesn't need to be persistent. Completionist reads non-persistent ones from the plugin itself, so you don't need an extra plugin that makes them persistent.
- **Fishing spots:** a reference placed from one of the Fishing Creation's fishing spot objects (stream, lake, arctic or cave), by any mod, gets a "Find the fishing spot in …" button automatically.
  If the entry is just called "Fishing Spot", the button names the place it's in. There's nothing extra to set.

```ini
[Shrines]
Enabled = true
Type = 9
DisplayOnPage = 1
SectionHeader_L = cTranslate_Header_Shrines
FormIDs = <<<LIST
0x000880, ; Shrine of Talos
0x000881, ; Fishing Spot
LIST
0x000880_DisplayName = cTranslate_Shrine_Crag
0x000880_Highlight = cTranslate_Hint_ShrineCrag
```

## 11. Quests

Quests work differently from the other Types. Two things are needed:

1. **One Type 5 section** on the tab where the quests should appear (section 10).
2. **One section per quest.** A section with an `EditorID` setting is a quest. It needs no `Type`.

Quests Completionist already tracks itself are skipped.

### Settings for every quest

| Setting | Needed? | What it does |
|---|---|---|
| `Enabled` | **Yes** | `true` to load this quest. |
| `DisplayOnPage` | **Yes** | The tab with your Type 5 section. |
| `EditorID` | **Yes** | The quest's Editor ID, as shown in xEdit or the Creation Kit. |
| `UUID` | **Yes** | A name for this quest that no other addon uses, e.g. `AuthorModName_Quest01`. Saves remember completed quests by it, so **never change it** after you release the addon. |
| `QuestName` | **Yes** | Translation key for the quest's name. |
| `HighlightText` | **Yes** | Translation key for the hint, e.g. `Quest giver: Jarl Balgruuf. Needs level 10.` |
| `QuestType` | **Yes** | Which group it goes in: `0` = Main, `1` = Side, `2` = Radiant / repeatable. |
| `CompletionType` | **Yes** | How it's completed: `0`, `1` or `2`. See below. |
| `InstallCondition` | No | Only load this quest when a condition is met (section 12). |
| `Visibility` | No | Which drop-down option shows this quest: `1` for option 1, and so on (section 13). |

### CompletionType 0: the normal way

The quest counts as done when the game marks it completed (or failed). Most quests use this.

```ini
[Quest - The Frozen Vault]
Enabled = true
DisplayOnPage = 2
UUID = Faen_Bloodchill_Quest01
EditorID = MyModVaultQuest
QuestName = cTranslate_QuestName01
HighlightText = cTranslate_QuestText01
QuestType = 0
CompletionType = 0
```

### CompletionType 1: at a quest stage

For quests that never get the "completed" flag, or that are done before they formally end.

| Setting | What it does |
|---|---|
| `StageType` | `1` = done once the quest has gone **past** `Stage`. `2` = done once `Stage` itself is **done**. |
| `Stage` | The stage to check. |
| `OptionalStage` | A second stage that also counts, for quests with two endings. `0` if not needed. |

```ini
[Quest - Ending Either Way]
Enabled = true
DisplayOnPage = 2
UUID = Faen_Bloodchill_Quest02
EditorID = MyModChoiceQuest
QuestName = cTranslate_QuestName02
HighlightText = cTranslate_QuestText02
QuestType = 1
CompletionType = 1
StageType = 2
Stage = 200
OptionalStage = 210
```

### CompletionType 2: repeatable quests

Counts how many times a radiant quest is finished, and completes after a number of times.

| Setting | What it does |
|---|---|
| `RadiantStage` | Each time the quest reaches this stage, it counts once. **Needed** (not for favour quests). |
| `TimesRequired` | How many times it must be done: `1` to `9`. Or one of the player's own radiant settings, so the player decides: `10` fishing, `11` favours, `12` bounties, `13` College, `14` Companions, `15` Dark Brotherhood, `16` Dawnguard, `17` Thieves Guild, `18` Legacy of the Dragonborn, `19` Vigilants, `20` Blades. |

```ini
[Quest - Wolf Hunt]
Enabled = true
DisplayOnPage = 2
UUID = Faen_Bloodchill_Quest03
EditorID = MyModRadiantHunt
QuestName = cTranslate_QuestName03
HighlightText = cTranslate_QuestText03
QuestType = 2
CompletionType = 2
RadiantStage = 100
TimesRequired = 3
```

### Favour quests

Some favours have no quest stage to watch: selling crops, giving a beggar gold, buying someone a drink.
For these, add the settings below to a CompletionType 2 quest. Completionist then watches the NPC instead of the quest.

| Setting | What it does |
|---|---|
| `isGenericFavorQuest` | `true`. |
| `GenericFavorQuestType` | `0` = selling to the NPC (crops, ore, firewood). `1` = giving the NPC gold (beggars). `2` = giving the NPC an item from a list (drinks, gifts). |
| `BaseFormID` | The NPC's actor base FormID, written as in section 8.1. |
| `PluginFileName` | The plugin that NPC comes from. |
| `validItemsFormID` | Type `2` only: the FormID of the form list (`FLST`) of items that count. |
| `validItemsFileName` | Type `2` only: the plugin that form list comes from. |
| `TimesRequired` | As above. |

```ini
[Quest - A Drink for Old Hrolf]
Enabled = true
DisplayOnPage = 2
UUID = Faen_Bloodchill_Quest04
EditorID = MyModFavorDrink
QuestName = cTranslate_QuestName04
HighlightText = cTranslate_QuestText04
QuestType = 2
CompletionType = 2
TimesRequired = 1
isGenericFavorQuest = true
GenericFavorQuestType = 2
BaseFormID = 0x000890
PluginFileName = MyMod.esp
validItemsFormID = 0x000891
validItemsFileName = MyMod.esp
```

Leave `BaseFormID` out of quests that aren't favour quests. Completionist finds the quest by its `EditorID`.

## 12. Install conditions

An install condition loads something only when certain plugins are, or aren't, in the load order.
You can put one on the whole addon, a tab, a section, a quest or a single entry:

| Where | Setting |
|---|---|
| The whole addon | `InstallCondition` in `[Completionist Patch Data]` |
| One tab | `PageName2_InstallCondition` in `[Completionist Patch Data]` (every list on that tab is hidden) |
| One section or one quest | `InstallCondition` in that section |
| One entry | `0x000800_InstallCondition` in that entry's section |

How to write one:

- `<&Plugin.esp` means "this plugin **is** installed".
- `<!Plugin.esp` means "this plugin is **not** installed".
- Write several in a row to need **all** of them: `<&A.esp<!B.esp` = A is installed **and** B isn't.
- Separate groups with a comma to need **any one** of them: `<&A.esp,<&C.esp` = A **or** C is installed.

```ini
; Load if SomeMod1 is missing and SomeMod2 is there, OR if both SomeMod3 and SomeMod4 are there.
InstallCondition = <!SomeMod1.esp<&SomeMod2.esm,<&SomeMod3.esp<&SomeMod4.esm
```

## 13. The filter drop-down

A drop-down at the top of your page lets the player pick which entries to show.
Use it for things like a mod with three questlines where the player can only follow one.

In `[Completionist Patch Data]`:

```ini
DropDownMenu = true
DropDownMenu_Name = cTranslate_Filter_Name
DropDownMenu_Highlight = cTranslate_Filter_Hint
DropDownMenu_Option_1 = cTranslate_Filter_Option1
DropDownMenu_Option_2 = cTranslate_Filter_Option2
DropDownMenu_Option_3 = cTranslate_Filter_Option3
```

- `DropDownMenu_Name` is its label and `DropDownMenu_Highlight` is the tooltip.
- Number the options 1, 2, 3 with no gaps.
- Tie an entry to an option with `0x000800_Visibility = 2`, or a quest with `Visibility = 2`. The numbers match the options: `2` is `DropDownMenu_Option_2`.
- An entry or quest **without** a Visibility setting always shows.
- One with a Visibility setting shows only while its option is picked.

## 14. The translation file

The .txt file turns each key from the .ini into the text the player sees.
Each line is: **the key, a TAB, a `|`, then the text.**

```
- Lines that start with a dash are notes. Completionist skips them.
cTranslate_MCMName	|Bloodchill Armoury 
cTranslate_PageName1	|Gear 
cTranslate_Header_Weapons	|Weapons: 
cTranslate_Hint_BloodchillBow	|Found on the altar in Frostmoon Crag. 
```

- The gap after the key must be a **TAB**, not spaces. Several TABs are fine.
  Many editors turn TABs into spaces, so check that setting.
- The text starts straight after the `|` and runs to the end of the line.
- **Spaces at the end of the text are kept.** Older addons end each line with a space, and that's fine. Take care that your editor doesn't remove or add them where they matter.
- Keys **do** care about upper and lower case, unlike the .ini.
- If a key is missing, the player sees the key itself, e.g. `cTranslate_PageName1`. That makes missing keys easy to spot.
- Trailing `:` and spaces on headings and tab names are removed in the window, so `Weapons: ` shows as "Weapons".
- Save the file as UTF-8. Start the file with a `-` note line: if your editor adds an invisible marker at the very start of the file, it then lands on a line Completionist skips.
- Keys must be unique in the file. If two .ini files share one .txt (`OverrideLocalisationFileName`), their keys must be unique across both.

To translate an addon, copy the .txt and change only the text after the `|`. Never change the keys.

## 15. Testing your addon

1. Set `Log = true` in `[Completionist Patch Data]`.
   Or set `Log_All_Patch_Installs = true` in Completionist's settings file to log every addon.
   The settings file is `Completionist.ini` in `Documents\My Games\Skyrim Special Edition\SKSE\`.
2. Start the game and load a save.
3. Open `Completionist.log` in `Documents\My Games\Skyrim Special Edition\SKSE\`.
   For every entry it says what was added, or why it was skipped:
   plugin not found, condition not met, FormID not found, wrong kind of record, already tracked by Completionist.
4. Open the Completionist window and check your page. Missing text shows as the raw key.
5. Set `Log` back to `false` before you release the addon.

## 16. Common mistakes

| Problem | Likely cause |
|---|---|
| The page doesn't show at all | `Enabled` is missing from `[Completionist Patch Data]`; `PluginFileName` is misspelled or has the wrong extension; the addon's `InstallCondition` failed; or every entry was skipped. |
| A list is missing | That section has no `Enabled = true`; its `Type` is turned off in the player's settings; or none of its FormIDs were found. |
| An entry is missing | The FormID still has the load order in front; it belongs to another plugin (add `*Plugin.esp`); the record is the wrong kind for the Type; or Completionist already tracks it. |
| A setting for one entry does nothing | The FormID in the setting isn't written exactly as in the list, e.g. `0x807_Highlight` for an entry written `0x000807`. |
| A comment changed a value | Comments must be on their own line, except after the comma in a FormIDs list. |
| Text shows as `cTranslate_…` | The key is missing from the .txt, is spelled differently (case matters), or the line uses spaces instead of a TAB. |
| Quests don't appear | There's no Type 5 section on that tab, or the quest's `DisplayOnPage` points to a different tab. |
| A quest's progress was lost | Its `UUID` was changed. |
| A filtered entry never shows | Its `_Visibility` number has no matching `DropDownMenu_Option_` number. Options start at 1. |
| The game crashes or misbehaves at start-up | A quest or Type 5 section has no `DisplayOnPage`, or it points to a tab that has no `PageName`. |

## 17. Old settings you can delete

Older addons, and the xEdit script, still write some settings that Completionist no longer reads. They do no harm, and you can remove them:

- `axPlt`
- `PageCount` (Completionist counts the `PageName` settings instead)
- `PrependPageNumber` (only the old MCM menu used it)
