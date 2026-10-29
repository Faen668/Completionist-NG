#!/usr/bin/env python3
"""Tidy the four Completionist translation files so translators can read them.

    python format_translations.py <translations folder>            report only
    python format_translations.py <translations folder> --check    exit 1 if a file needs tidying
    python format_translations.py <translations folder> --write    rewrite the files

The folder is SKSE/Plugins/CompletionistData/Translations (D, N, M and UI files).

How the game reads these files (Localisation.cpp, TranslationFile.cpp):
  * blank lines and lines starting with '-' are skipped
  * the key is the text before the FIRST TAB, the value is everything after the first '|'
  * any other line is silently dropped

What this script changes, and nothing else:
  * one header per file with the translator rules, a titled section per group
  * keys padded with TABs so the '|' lines up (tab width 4), Windows line endings
  * D only: runs of spaces/tabs inside a value collapse to one space
  * all files: spaces/tabs directly before a literal \\n are removed
  * UI only (old files): bare text lines that follow an entry are joined on with \\n
Values are otherwise kept exactly, because some end in a space on purpose
(for example M's LogPrefix_* entries are joined to a name in game).

Before writing, every file is re-parsed and compared with the original. The run stops
if a key is lost or added, or a word changes, unless --allow-text-change is given.
"""
import argparse
import os
import re
import sys

BS = chr(92)
NL = BS + "n"  # backslash + n, the line break used inside values
RULE = "-" * 76
TAB_W = 4
FILES = ("D", "N", "M", "UI")

D_TITLES = {
    "MSQ": "Main quest: Skyrim",
    "CW": "Civil War",
    "Main_DG": "Main quest: Dawnguard",
    "Main_DB": "Main quest: Dragonborn",
    "Dawnstar": "Dawnstar", "Falkreath": "Falkreath", "Markarth": "Markarth", "Morthal": "Morthal",
    "Riften": "Riften", "Solitude": "Solitude", "Whiterun": "Whiterun", "Windhelm": "Windhelm",
    "Winterhold": "Winterhold", "RavenRock": "Raven Rock", "SkaalVillage": "Skaal Village",
    "SmallTowns": "Small towns", "TelMithryn": "Tel Mithryn", "Thirsk": "Thirsk Hall",
    "CollegeOfWinterhold": "College of Winterhold", "Companions": "Companions",
    "DarkBrotherhood": "Dark Brotherhood", "ThievesGuild": "Thieves Guild",
    "Dawnguard": "Dawnguard (radiant quests)", "Vampires": "Volkihar vampires (radiant quests)",
    "Dungeons": "Dungeons", "Misc_DB": "Dragonborn: miscellaneous quests",
    "Misc_DG": "Dawnguard: miscellaneous quests", "Side_SK": "Skyrim: side quests",
    "Radi_SK": "Skyrim: radiant quests",
    "Other": "Other entries",
    "LOTD_Main": "Legacy of the Dragonborn: main quests", "LOTD_Exp": "Legacy of the Dragonborn: expeditions",
    "LOTD_Journals": "Legacy of the Dragonborn: journals", "LOTD_Notes": "Legacy of the Dragonborn: notes",
    "LOTD_Misc": "Legacy of the Dragonborn: miscellaneous",
}

# Used only when tidying an old M file that has no section titles yet.
M_TITLES = {
    "V_ShoutText00": "Shout word locations: Skyrim and DLC (V_ShoutText)",
    "MiscDataShout_FSK_Data": "Shouts added by other mods (MiscDataShout)",
    "T_ShoutText00": "Shouts added by Thunderchild (T_ShoutText)",
    "Barenziah_Name00": "Stones of Barenziah: names",
    "Barenziah_Text00": "Stones of Barenziah: where to find them",
    "LogPrefix_Col": "Completion log prefixes (each ends in a space on purpose - keep it)",
    "MapMarkerCleared": "Map markers",
    "ItemType_Scroll": "Item types",
    "MCMText_DeathSentanceBy": "Death messages",
    "DeathArticle_Vowel": "Death messages: articles and locations",
    "UI_Title": "In-game window: titles and categories",
    "UI_Screenshot": "Screenshots",
    "UI_Settings": "In-game window: settings",
}

_RULES = [
    "Change only the text AFTER the '|'. Never change the KEY in front of it.",
    "Keep the TAB between the KEY and the '|': a key ends at its first TAB.",
]
_TAIL = [
    "Lines starting with '-' are comments and are ignored.",
    "The order of the lines does not matter: lines are matched by KEY.",
    "Set your editor's tab width to 4 and the columns line up.",
]
_BREAK = "'" + NL + "' is a line break. Keep each one where it is."


def _head(title, filename, note, per_line, extra):
    lines = [title, "File: " + filename + "   (" + note + ")", "", "Each line is:   KEY <TAB> | " + per_line, "", "HOW TO TRANSLATE"]
    lines += [r if r.startswith("    ") else "  * " + r for r in _RULES + extra + _TAIL]
    return lines


HEADERS = {
    "D": _head("COMPLETIONIST - QUEST DESCRIPTIONS", "Completionist_Translations_D.txt",
               "quest names are in Completionist_Translations_N.txt", "description",
               [_BREAK, "Translate the labels too: Quest Giver, Location, Overview, Missable, NOTE."]),
    "N": _head("COMPLETIONIST - QUEST NAMES", "Completionist_Translations_N.txt",
               "quest descriptions are in Completionist_Translations_D.txt", "quest name",
               ["Use the name the quest has in the translated game where there is one."]),
    "M": _head("COMPLETIONIST - IN-GAME TEXT", "Completionist_Translations_M.txt",
               "shout hints, Stones of Barenziah, log text, window text", "text",
               [_BREAK, "Keep '~~~ Name ~~~' headings and placeholders such as {} exactly as they are.",
                "Some texts end in a space on purpose (for example Completed: ). Keep that space,",
                "    because Completionist puts a name straight after it."]),
    "UI": _head("COMPLETIONIST - IN-GAME WINDOW", "Completionist_Translations_UI.txt",
                "lines ported from the MCM keep their MCM key; plain text, no HTML", "text",
                [_BREAK, "{} is filled in by Completionist (a number or a name). Keep every {} in your text.",
                 "Keep '~~~ Name ~~~' headings exactly as they are."]),
}


# --------------------------------------------------------------------------- reading

def parse_entry(line):
    """The game's rule: returns (key, value) or None."""
    if not line or line.startswith("-"):
        return None
    tab, pipe = line.find("\t"), line.find("|")
    if tab < 0 or pipe < 0:
        return None
    return line[:tab], line[pipe + 1:]


def read_lines(path):
    with open(path, encoding="utf-8", newline="") as f:
        return f.read().replace("\r\n", "\n").replace("\r", "\n").split("\n")


def parse_map(lines):
    out = {}
    for ln in lines:
        e = parse_entry(ln)
        if e and e[0] not in out:
            out[e[0]] = e[1]
    return out


def is_formatted(lines):
    return any("HOW TO TRANSLATE" in ln for ln in lines[:40])


def body_start(lines):
    """Index after the header block (the second rule line)."""
    seen = 0
    for i, ln in enumerate(lines):
        if ln.startswith(RULE[:20]):
            seen += 1
            if seen == 2:
                return i + 1
    return 0


# --------------------------------------------------------------------------- value cleaning

def strip_pad(v):
    return re.sub(r"[ \t]+(?=" + re.escape(NL) + ")", "", v)


def clean_d(v):
    v = re.sub(r"[ \t]+", " ", v)
    v = re.sub(r" ?" + re.escape(NL) + r" ?", lambda m: NL, v)
    return v.strip()


# --------------------------------------------------------------------------- writing

def emit_block(out, entries):
    if not entries:
        return
    col = (max(len(k) for k, _ in entries) // TAB_W + 1) * TAB_W
    for k, v in entries:
        out.append(k + "\t" * (col // TAB_W - len(k) // TAB_W) + "|" + v)


def section(out, title):
    out += ["", RULE, "-- " + title, RULE]


def header(out, which):
    out.append(RULE)
    out += [("-- " + ln).rstrip() for ln in HEADERS[which]]
    out.append(RULE)


def read_sections(lines):
    """Titled sections of an already tidy file: [(title, [(key, value)])]."""
    sections = []
    for ln in lines[body_start(lines):]:
        if ln.startswith(RULE[:20]):
            continue
        if ln.startswith("-"):
            sections.append((ln.lstrip("-").strip(), []))
            continue
        e = parse_entry(ln)
        if e:
            if not sections:
                sections.append(("General", []))
            sections[-1][1].append(e)
    return sections


def prefix_of(key):
    if not re.search(r"Quest\d", key):
        return "Other"
    pre = re.sub(r"\d+$", "", re.sub(r"_?Quest\d+.*$", "", key))
    return "CW" if pre == "CWQuest" else pre


def build_quest_file(which, lines):
    groups, order = {}, []
    for ln in lines:
        e = parse_entry(ln)
        if not e:
            continue
        pre = prefix_of(e[0])
        if pre not in groups:
            groups[pre] = []
            order.append(pre)
        groups[pre].append((e[0], clean_d(e[1]) if which == "D" else strip_pad(e[1])))
    out = []
    header(out, which)
    for pre in order:
        section(out, D_TITLES.get(pre, pre))
        emit_block(out, groups[pre])
    return out


def build_m_legacy(lines):
    blocks, cur = [], None
    for ln in lines:
        e = parse_entry(ln)
        if e:
            if cur is None:
                cur = []
                blocks.append(cur)
            cur.append((e[0], strip_pad(e[1])))
        elif ln.startswith(RULE[:20]):
            cur = None
    out = []
    header(out, "M")
    for blk in blocks:
        if blk[0][0] != "UI_DescriptionEmpty":
            section(out, M_TITLES.get(blk[0][0], blk[0][0]))
            emit_block(out, blk)
            continue
        runs = []
        for k, v in blk:
            parts = k.split("_")
            tok = parts[1] if len(parts) > 2 else None
            if runs and runs[-1][0] == tok:
                runs[-1][1].append((k, v))
            else:
                runs.append([tok, [(k, v)]])
        segs, general = [], []
        for tok, ents in runs:
            if len(ents) >= 12 and tok:
                if general:
                    segs.append(("In-game window: text and settings", general))
                    general = []
                title = "UI: " + tok + " (UI_" + tok + "_*)"
                if segs and segs[-1][0] == title:
                    segs[-1][1].extend(ents)
                else:
                    segs.append((title, ents))
            else:
                general.extend(ents)
        if general:
            segs.append(("In-game window: text and settings", general))
        part = 0
        for title, ents in segs:
            if title == "In-game window: text and settings":
                part += 1
                title += " (part " + str(part) + ")"
            section(out, title)
            emit_block(out, ents)
    return out


def build_ui_legacy(lines):
    sections, cur, last, blanks = [], None, None, 0
    for ln in lines:
        if ln.startswith(RULE[:20]):
            continue
        if ln.startswith("-"):
            title = ln.lstrip("-").strip()
            if title.startswith("Completionist in-game window") or title.startswith("Interface/Translations files keep"):
                continue
            cur = (title, [])
            sections.append(cur)
            last, blanks = None, 0
            continue
        e = parse_entry(ln)
        if e:
            if cur is None:
                cur = ("General", [])
                sections.append(cur)
            cur[1].append([e[0], strip_pad(e[1])])
            last, blanks = cur[1][-1], 0
        elif not ln.strip():
            blanks += 1
        else:  # a bare line the game would drop: it belongs to the entry above
            if last is None:
                raise SystemExit("bare text line with no entry above it: " + ln[:60])
            last[1] += NL * (blanks + 1) + ln.strip()
            blanks = 0
    out = []
    header(out, "UI")
    for title, ents in sections:
        section(out, title)
        emit_block(out, [(k, v) for k, v in ents])
    return out


def build_sectioned(which, lines):
    out = []
    header(out, which)
    for title, ents in read_sections(lines):
        section(out, title)
        emit_block(out, [(k, strip_pad(v)) for k, v in ents])
    return out


def tidy(which, lines):
    if which in ("D", "N"):
        return build_quest_file(which, lines)
    if is_formatted(lines):
        return build_sectioned(which, lines)
    return build_m_legacy(lines) if which == "M" else build_ui_legacy(lines)


# --------------------------------------------------------------------------- main

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("folder", help="the Translations folder")
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--write", action="store_true", help="rewrite the files")
    mode.add_argument("--check", action="store_true", help="exit 1 if any file needs tidying")
    ap.add_argument("--allow-text-change", action="store_true", help="allow a run that changes words (old UI files)")
    args = ap.parse_args()

    problems, pending = 0, []
    for which in FILES:
        path = os.path.join(args.folder, "Completionist_Translations_" + which + ".txt")
        if not os.path.isfile(path):
            print(which + ": missing " + path)
            problems += 1
            continue
        lines = read_lines(path)
        text = "\r\n".join(tidy(which, lines)) + "\r\n"
        before, after = parse_map(lines), parse_map(text.replace("\r\n", "\n").split("\n"))
        lost, added = [k for k in before if k not in after], [k for k in after if k not in before]
        ws = lambda s: re.sub(r"\s+", "", s)
        words = [k for k in before if k in after and ws(before[k]) != ws(after[k])]
        with open(path, encoding="utf-8", newline="") as f:
            same = f.read() == text
        state = "already tidy" if same else "needs tidying"
        print("%-2s %-13s entries %d -> %d, keys lost %d, keys added %d, values with changed words %d"
              % (which, state, len(before), len(after), len(lost), len(added), len(words)))
        if lost or added or (words and not args.allow_text_change):
            for k in (lost + added + words)[:10]:
                print("     ! " + k)
            problems += 1
        elif not same:
            pending.append((path, text))

    if problems:
        print("Nothing written: fix the problems above first.")
        return 2
    if args.check:
        return 1 if pending else 0
    if args.write:
        for path, text in pending:
            with open(path, "w", encoding="utf-8", newline="") as f:
                f.write(text)
        print("Rewrote %d file(s)." % len(pending))
    elif pending:
        print("Run again with --write to rewrite %d file(s)." % len(pending))
    return 0


if __name__ == "__main__":
    sys.exit(main())
