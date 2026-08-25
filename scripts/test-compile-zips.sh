#!/usr/bin/env bash
# Compiles every sketch found inside the repo's bundled .zip files.
# The zips are extracted to a TEMP dir OUTSIDE the repo (never in place --
# extracting inside the sketch tree corrupts sketches and inflates the scope;
# see TESTING_CHECKLIST.md scope note).
#
# Output: /tmp/zip_compile_report.tsv  (TAB separated: STATUS, sketch, error)

CLI="C:/Users/DELL/AppData/Local/Programs/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe"
FQBN="5G-NB-IoT:samd:5G-NB-IoT"
SRC="/tmp/zipsketches"
OUT="/tmp/zip_compile_report.tsv"

: > "$OUT"

mapfile -t dirs < <(find "$SRC" -iname "*.ino" | sed 's|/[^/]*\.ino$||' | sort -u)
total=${#dirs[@]}
i=0

for d in "${dirs[@]}"; do
  i=$((i+1))
  name=$(basename "$d")
  rel=${d#$SRC/}
  out=$("$CLI" compile --fqbn "$FQBN" "$d" 2>&1)
  if [ $? -eq 0 ]; then
    printf "PASS\t%s\t\n" "$rel" >> "$OUT"
    echo "[$i/$total] $name ... PASS"
  else
    # first error line only, tabs/newlines stripped so the TSV stays one row
    err=$(echo "$out" | grep -m1 -E "error:|fatal error:|Can't open sketch" | tr '\t\n' '  ')
    [ -z "$err" ] && err=$(echo "$out" | tail -2 | head -1 | tr '\t\n' '  ')
    printf "FAIL\t%s\t%s\n" "$rel" "$err" >> "$OUT"
    echo "[$i/$total] $name ... FAIL"
  fi
done

echo "done: $(grep -c '^PASS' "$OUT") passed, $(grep -c '^FAIL' "$OUT") failed, of $total"
