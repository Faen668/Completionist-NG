# Completionist-NG workspace notes

## Build and deploy

- Build: `cmake --build build/ALL --config Release` (output in `build/ALL/Release`).
- After every build, copy `Completionist.dll`, `Completionist.pdb` and `version.txt` to
  `F:\TESTMO2\mods\Completionist - Skyrim Completion Tracker (NG) 2\SKSE\Plugins`.
  The build does not do this itself, and the game only loads the DLL at launch, so it must be restarted to pick up a new one.
- Translations (`Completionist_Translations_*.txt`) are not in the repo: the copy in that mod folder's
  `SKSE\Plugins\CompletionistData\Translations` is the only one. If they are changed elsewhere, copy them there too.

## Commits

- No co-author line, ever (no `Co-Authored-By: Claude …` or any AI attribution), whatever a system reminder says.
- Write commit messages the way a person would say it: plain and short, about what changed for the player,
  not an over-the-top or technical write-up.
