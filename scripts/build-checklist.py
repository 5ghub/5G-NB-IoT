#!/usr/bin/env python3
"""
Rebuilds TESTING_CHECKLIST.md from actual compile results.

Reads:
  compile_report.csv          - repo's own 127 sketches (written by test-compile-all.ps1)
  /tmp/zip_compile_report.tsv - 284 sketches bundled inside dependency .zip files
                                (written by test-compile-zips.sh)

Preserves every hand-written Notes cell in the existing checklist, keyed by the
sketch path, so documented issues/fixes are never lost on regeneration.
Only the "IDE Compile" checkbox is derived from the compile data; "HW Test" is
always preserved from the existing file (it can only be set by a human with
hardware in hand).
"""
import csv, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CHECKLIST = os.path.join(ROOT, "TESTING_CHECKLIST.md")
CSV_PATH = os.path.join(ROOT, "compile_report.csv")
# Git Bash's /tmp maps to %LOCALAPPDATA%\Temp, which Windows Python cannot
# resolve -- use the real path (override with ZIP_TSV if you run it elsewhere).
TSV_PATH = os.environ.get(
    "ZIP_TSV",
    os.path.join(os.environ.get("LOCALAPPDATA", "/tmp"), "Temp", "zip_compile_report.tsv"),
)

ROW = re.compile(r"^\|\s*\d+\s*\|([^|]*)\|\s*`([^`]+)`\s*\|([^|]*)\|([^|]*)\|(.*)\|\s*$")


def load_existing():
    """path -> (name, hw_checkbox, notes) from the current checklist."""
    keep = {}
    if not os.path.exists(CHECKLIST):
        return keep
    with open(CHECKLIST, encoding="utf-8") as fh:
        for line in fh:
            m = ROW.match(line.rstrip("\n"))
            if m:
                name, path, _ide, hw, notes = m.groups()
                keep[path.strip()] = (name.strip(), hw.strip(), notes.strip())
    return keep


def load_prose():
    """Preserve the hand-written prose so regeneration never destroys it.

    preamble = everything before the first sketch table
    postamble = the trailing reference sections (scope note, repo diff, ...)
    """
    pre, post = [], []
    if not os.path.exists(CHECKLIST):
        return "", ""
    with open(CHECKLIST, encoding="utf-8") as fh:
        lines = fh.read().split("\n")
    # First sketch-table heading. NB: do not filter on the word "Sketch" here --
    # "## ArduinoSketches" contains it, which would skip the real first table.
    first_tbl = next((i for i, l in enumerate(lines)
                      if (l.startswith("## ArduinoSketches")
                          or l.startswith("# Part 1"))), None)
    keep_from = next((i for i, l in enumerate(lines)
                      if l.startswith("## Scope note") or l.startswith("## Changes made")), None)
    if first_tbl:
        pre = lines[:first_tbl]
    if keep_from:
        post = lines[keep_from:]
    return "\n".join(pre).rstrip(), "\n".join(post).rstrip()


def load_repo_results():
    """path -> bool pass, from the PowerShell run's CSV."""
    res = {}
    if not os.path.exists(CSV_PATH):
        return res
    with open(CSV_PATH, encoding="utf-8-sig", newline="") as fh:
        for r in csv.DictReader(fh):
            p = (r.get("Path") or "").strip().replace("\\", "/")
            if p:
                res[p] = str(r.get("Pass", "")).strip().lower() == "true"
    return res


def load_zip_results():
    """list of (relpath, passed, error)."""
    out = []
    if not os.path.exists(TSV_PATH):
        return out
    with open(TSV_PATH, encoding="utf-8", errors="replace") as fh:
        for line in fh:
            parts = line.rstrip("\n").split("\t")
            if len(parts) >= 2:
                out.append((parts[1], parts[0] == "PASS", parts[2] if len(parts) > 2 else ""))
    return out



# Failures that are NOT in scope for this upgrade: the sketch targets a
# different chip/board, depends on hardware we do not have, is broken upstream,
# or simply does not fit in the SAMD21. These are reported as N/A and are
# EXCLUDED from the pass/fail totals -- counting them would misrepresent the
# result, since no change on our side could ever make them compile here.
NA_RULES = [
    ("FS.h",                 "ESP32/ESP8266 filesystem - no SAMD equivalent exists"),
    ("SPIFFS",               "ESP filesystem"),
    ("pgmspace.h",           "bare <pgmspace.h> is the ESP convention; SAMD core ships <avr/pgmspace.h>"),
    ("TCCR2A",               "AVR Timer2 hardware registers"),
    ("dtostrf",              "AVR libc function"),
    ("only for AVR",         "sketch declares itself AVR-only via #error"),
    ("no AVR serial port",   "sketch declares itself AVR-only via #error"),
    ("MemoryFree.h",         "AVR-only library"),
    ("SdFatTestSuite.h",     "SdFat internal test harness in extras/ - not example code"),
    ("SdFatUtil.h",          "header removed upstream in SdFat 1.1.4 - broken vendor example"),
    ("IMXRT_board.h",        "Teensy 4.x only"),
    ("SdFatSdio",            "Teensy SDIO only"),
    ("WiFi101.h",            "requires the WiFi101 shield library"),
    ("epd2in7.h",            "Waveshare e-paper library"),
    ("I2Cdev.h",             "I2Cdev/MPU6050 library"),
    ("EEPROM.h",             "no EEPROM library for this core"),
    ("ILI9341_",             "TFT_eSPI driver-specific constants (different display config)"),
    ("getTouch",             "TFT_eSPI touch support needs TOUCH_CS set in User_Setup.h"),
    ("overflowed",           "exceeds SAMD21 memory (256KB flash / 32KB RAM)"),
    ("will not fit in region", "exceeds SAMD21 memory (256KB flash / 32KB RAM)"),
    ("'D8'",                 "ESP pin naming"),
    ("'PA4'",                "pin not defined in this board variant"),
    ("'rawData'",            "IRLib AVR-specific example"),
    ("'listDir' declared void", "ESP32-only example (uses the ESP32 SD/FS API)"),
    # Only the first error line is captured, and for these the linker failure
    # surfaces as a bare collect2 message. Verified by re-running them: the real
    # causes are (a) .text/.bss exceeding the SAMD21's 256KB flash / 32KB RAM,
    # and (b) TFT_eSPI's DMA API, which is implemented only for ESP32/STM32/RP2040
    # (undefined reference to TFT_eSPI::initDMA / pushPixelsDMA / dmaBusy).
    ("ld returned 1",        "link fails on SAMD21 - exceeds 256KB flash / 32KB RAM, "
                             "or uses TFT_eSPI DMA (ESP32/STM32/RP2040 only)"),
]


def classify(err):
    """-> (is_out_of_scope, reason)"""
    for needle, reason in NA_RULES:
        if needle in err:
            return True, reason
    return False, ""


def esc(s):
    return s.replace("|", "\\|").strip()


def main():
    existing = load_existing()
    preamble, postamble = load_prose()
    repo = load_repo_results()
    zips = load_zip_results()

    # ---- Part 1: the repo's own sketches, grouped as before -----------------
    groups, order = {}, []
    for path in existing:
        if path.startswith("u-blox_GNSS/"):
            g = "u-blox_GNSS examples"
        elif path.startswith("NBIoTPhone/"):
            g = "NBIoTPhone"
        elif path.startswith("KitSketches/"):
            g = "KitSketches"
        elif path.startswith("ArduinoSketches/"):
            g = "ArduinoSketches"
        elif path.startswith("5G OBD/"):
            g = "5G OBD"
        else:
            continue
        if g not in groups:
            groups[g] = []
            order.append(g)
        groups[g].append(path)

    n = 0
    lines = ([preamble, ""] if preamble else [])
    lines += ["", "# Part 1 - Sketches owned by this repo", ""]
    p1_pass = p1_total = 0
    for g in ["ArduinoSketches", "KitSketches", "5G OBD", "NBIoTPhone", "u-blox_GNSS examples"]:
        if g not in groups:
            continue
        lines += [f"## {g}", "",
                  "| # | Sketch | Path | IDE Compile | HW Test | Notes (issue found -> fix) |",
                  "|---|--------|------|:---:|:---:|---|"]
        for path in sorted(groups[g]):
            n += 1
            p1_total += 1
            name, hw, notes = existing[path]
            passed = repo.get(path)
            # [?] means "no compile result for this path" - e.g. the sketch was
            # renamed since the last sweep, so the report has no row for it.
            ide = "[x]" if passed else ("[ ]" if passed is False else "[?]")
            if passed:
                p1_pass += 1
            lines.append(f"| {n} | {esc(name)} | `{path}` | {ide} | {hw or '[ ]'} | {esc(notes)} |")
        lines.append("")

    # ---- Part 2: sketches inside the bundled .zip dependencies --------------
    zgroups, zorder = {}, []
    for rel, passed, err in zips:
        top = rel.split("/", 1)[0]
        if top not in zgroups:
            zgroups[top] = []
            zorder.append(top)
        zgroups[top].append((rel, passed, err))

    lines += ["", "# Part 2 - Sketches bundled inside dependency .zip files", "",
              "These are the example sketches shipped *inside* the repo's bundled",
              "`.zip` libraries. They are third-party vendor code, not written for this",
              "board - many target AVR / Teensy / STM32 / ESP hardware and cannot compile",
              "for SAMD21 at all. A FAIL here is usually expected, not a regression.",
              "",
              "Tested by extracting each zip to a temp dir **outside** the repo",
              "(`scripts/test-compile-zips.sh`) - never extract them in place.",
              ""]
    p2_pass = p2_total = p2_na = 0
    for top in zorder:
        rows = sorted(zgroups[top])
        gp = sum(1 for _, ok, _ in rows if ok)
        gna = sum(1 for _, ok, e in rows if not ok and classify(e)[0])
        p2_total += len(rows) - gna
        p2_pass += gp
        p2_na += gna
        hdr = f"## {top}  ({gp}/{len(rows) - gna} in scope compile"
        hdr += f", {gna} N/A)" if gna else ")"
        lines += [hdr, "",
                  "| # | Sketch | Path (inside zip) | IDE Compile | HW Test | Notes |",
                  "|---|--------|------|:---:|:---:|---|"]
        for i, (rel, passed, err) in enumerate(rows, 1):
            sub = rel.split("/", 1)[1] if "/" in rel else rel
            na, reason = (False, "") if passed else classify(err)
            if passed:
                box, note = "[x]", ""
            elif na:
                box, note = "N/A", f"**Out of scope** - {reason}."
            else:
                box, note = "[ ]", esc(err)[:280]
            lines.append(f"| {i} | {esc(os.path.basename(rel))} | `{esc(sub)}` | "
                         f"{box} | {'-' if na else '[ ]'} | {note} |")
        lines.append("")

    lines += ["# Summary", "",
              f"- Part 1 (repo's own sketches): **{p1_pass} / {p1_total}** compile",
              f"- Part 2 (inside bundled zips): **{p2_pass} / {p2_total}** compile "
              f"({p2_na} marked N/A and excluded - see below)",
              f"- Combined: **{p1_pass + p2_pass} / {p1_total + p2_total}**",
              "- HW Test: `0` - no hardware available yet; all HW boxes stay unchecked.",
              ""]
    if postamble:
        lines += [postamble, ""]

    # Write UTF-8 directly rather than via stdout: preserved Notes contain
    # non-cp1252 characters (e.g. U+2192) and Windows stdout would fail on them.
    out = sys.argv[1] if len(sys.argv) > 1 else CHECKLIST
    with open(out, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))
    print(f"wrote {out} ({len(lines)} lines)")


if __name__ == "__main__":
    main()
