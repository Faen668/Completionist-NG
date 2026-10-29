# format_translations

Keeps the four Completionist translation files easy to read for translators.

The files live in the mod folder, not in this repo:
`SKSE/Plugins/CompletionistData/Translations/`

| File | Holds |
| --- | --- |
| `Completionist_Translations_D.txt` | quest descriptions |
| `Completionist_Translations_N.txt` | quest names |
| `Completionist_Translations_M.txt` | in-game text: shout hints, Stones of Barenziah, log text, window text |
| `Completionist_Translations_UI.txt` | text of the in-game window |

## Run it

Needs Python 3.8 or newer and nothing else.

```bash
python tools/format_translations/format_translations.py "<Translations folder>"            # report only
python tools/format_translations/format_translations.py "<Translations folder>" --check    # exit 1 if a file needs tidying
python tools/format_translations/format_translations.py "<Translations folder>" --write    # rewrite the files
```

It is safe to run again and again: on files that are already tidy it changes nothing.

Before it writes, it re-reads every file the way the game does and compares with the original. It stops without
writing if a key would be lost or added, or if the words of a value would change.
(`--allow-text-change` is only for old UI files, see below.)

## The layout it keeps

- One header per file with the translator rules.
- A `-- Title` section per group. In D and N the group comes from the key prefix, so a new quest
  such as `Riften_Quest30` is filed under Riften. A new prefix gets its own section named after the prefix; add a
  nicer title to `D_TITLES` in the script. Keys without `Quest<number>` go under "Other entries".
- Each key is padded with TABs so the `|` lines up (tab width 4) within its section.
- Windows (CRLF) line endings.
- Quest text uses `\n` with no spaces around it, and no runs of spaces.

## Rules the game imposes

These come from `Localisation.cpp` and `TranslationFile.cpp`:

- Blank lines and lines starting with `-` are skipped.
- The key is the text before the **first TAB**; the value is everything after the first `|`.
- Any other line is **silently dropped**. A value that needs several lines must use a literal `\n`,
  not a real line break.
- Values in M and UI are never trimmed. Some end in a space on purpose
  (for example `LogPrefix_*` in M, which are joined to a name in game).

## Old UI files

An old UI file may have bare text lines under an entry, left over from the MCM. The game drops them, so
the first run joins them to the entry above with `\n`. That changes what the game shows, so the run needs
`--allow-text-change`. Once the file is tidy this no longer applies.
