# Recommendations & Achievements — IDE 2.3.10 Upgrade

Running project log, separate from `TESTING_CHECKLIST.md` on purpose: the
checklist is one row per sketch, pass/fail, kept mechanical. This file is for
things that don't belong to any single row — decisions made, milestones
verified, and improvements worth doing beyond strict 2.3.10 compatibility.

## Achievements

Confirmed, dated findings — not aspirations.

- **Boards Manager package installs cleanly on IDE 2.3.10.** All five pinned
  legacy tool versions (`arm-none-eabi-gcc 4.8.3-2014q1`, `bossac 1.7.0`,
  `openocd 0.9.0-arduino5-static`, `CMSIS 4.5.0`, `CMSIS-Atmel 1.1.0`) still
  resolve and download through the Arduino package index — nothing had to be
  re-pinned to get the board package itself installed.
- **Core toolchain compiles on 2.3.10, verified via `arduino-cli` before
  switching to GUI-driven testing:** `BlinkLED`, `TCPClient`, `MQTTClient`,
  `HTTPClient`, `GNSS` all compiled with zero errors. This covers both a
  no-library sketch and several that exercise the BG96 modem library, so the
  core + library combination is sound on the new toolchain, not just the bare
  core.
- **SparkFun u-blox GNSS Arduino Library v2.2.29 installed** (2026-08-25),
  unblocking the `u-blox_GNSS` section of the checklist. Confirmed by matching
  the example folder names already in this repo (`Dead_Reckoning/`,
  `Callbacks/`, `Example23_TimePulseParameters`, up to `Example31_...`) against
  each candidate library's known structure — ruled out "SparkFun u-blox Arduino
  Library" (explicitly marked deprecated, it's the v1.x predecessor) and
  "SparkFun u-blox GNSS v3" (newer major rewrite, restructured examples, not
  backward-compatible with older modules).
- **Batch-compile automation built** (`scripts/test-compile-all.ps1`), so the
  full 127-sketch sweep doesn't require manually opening and clicking Verify
  on every sketch in the GUI. Runs every sketch under the checklist's scope
  against `5G-NB-IoT:samd:5G-NB-IoT` via `arduino-cli` and writes
  `compile_report.csv` / `compile_report.log`.
- **Full `#include` dependency audit completed across all 127 sketches**
  (2026-08-25), cross-checked against every bundled `.zip`, the main
  `5G-NB-IoT_Arduino.zip` library's `board.h` umbrella header, and the board
  core's built-in libraries. All 14 bundled per-sketch/per-lesson `.zip`
  dependencies were opened and confirmed to genuinely contain the header their
  sketch expects (see the "Confirmed dependency zips" list below) — none are
  mismatched or empty. This also turned up 4 real issues and ruled out 2 false
  alarms; see Recommended Enhancements below for each.

**Confirmed dependency zips** (all verified to contain the expected header,
install each via Sketch → Include Library → Add .ZIP Library before compiling
its sketch):

| Zip | Location | Provides | Needed by |
|---|---|---|---|
| `Arduino-FreeRTOS-SAMD21-master.zip` | `ArduinoSketches/examples/TCPContinuoslyFreeRTOS/` | `FreeRTOS_SAMD21.h` | `TCPContinuoslyFreeRTOS` |
| `Servo.zip` | `KitSketches/Lesson 06 Servo/` | `Servo.h` | `servo` |
| `Keypad.zip` | `KitSketches/Lesson 07 Keypad/` | `Keypad.h` | `Keypad` |
| `Adafruit_Sensor-master.zip` | `KitSketches/Lesson 08 .../` | `Adafruit_Sensor.h` | `DHT11` |
| `DHT-sensor-library-master.zip` | `KitSketches/Lesson 08 .../` | `DHT.h`, `DHT_U.h` | `DHT11` |
| `IRLib2-master.zip` | `KitSketches/Lesson 10 IR Receiver Module/` | `IRLibAll.h` | `IR_Receiver_Module` |
| `Rtc-master.zip` | `KitSketches/Lesson 12 Real Time Clock Module/` | `RtcDS1307.h` | `DS1307` |
| `rfid-master.zip` | `KitSketches/Lesson 14 RC522 RFID Module/` | `MFRC522.h` | `MF-RC522_RFID` |
| `LiquidCrystal.zip` | `KitSketches/Lesson 15 LCD Display/` | `LiquidCrystal.h` | `HelloWorld` |
| `Stepper.zip` | `KitSketches/Lesson 20 Stepper Motor/` | `Stepper.h` | `stepper` |
| `Stepper.zip` + `IRremote.zip` | `KitSketches/Lesson 21 .../` | `Stepper.h`, `IRremote.h` | `With_Remote` |
| `SdFat.zip` | `NBIoTPhone/SD_card/` | `SdFat.h` | `SD_card` |
| `TFT_eSPI-master.zip` | `NBIoTPhone/TFTLibrary/` | `TFT_eSPI.h` | every `NBIoTPhone` TFT sketch (plus the `User_Setup.h` swap from the README) |

## ✅ MILESTONE: all 127 repo sketches compile on Arduino IDE 2.3.10

**127 / 127 pass, 0 failures** (verified 2026-08-25 via
`scripts/test-compile-all.ps1`; snapshot kept as `compile_report_127_PASS.csv`).

Every sketch this repo owns — `ArduinoSketches` (20), `KitSketches` (30),
`5G OBD` (2), `NBIoTPhone` (13), `u-blox_GNSS` (62) — builds cleanly against
`5G-NB-IoT:samd:5G-NB-IoT` on IDE 2.3.10.

Reaching it required **7 fixes, none of which were caused by the IDE upgrade** —
every one was a latent pre-existing bug that would have failed identically on
2.1.1. Total change to the repo: 9 files, +62 / -6 lines plus two repackaged zips —
see the full delta table in TESTING_CHECKLIST.md.

| # | Sketch | Issue | Fix |
|---|--------|-------|-----|
| 1 | `KitSketches/Lesson 08 .../DHT11` + `5G-NB-IoT_Arduino.zip` | `board.h` pulls in `5GHUB_Sensor.h`, which duplicates Adafruit's type names verbatim -> redeclaration errors when a sketch also uses `DHT_U.h` | **Root-cause fix in the library:** wrapped the duplicated typedefs/enums in `#ifndef _ADAFRUIT_SENSOR_H` (the `_5GHUB_SensorInterface` class stays outside the guard, since the BME680/TSL25911/BNO055 classes inherit from it). DHT11 keeps `#include <board.h>`, with `DHT.h`/`DHT_U.h` moved above it so Adafruit's guard is set first. |
| 2 | `Lesson 26 .../Position` | folder `Position\` vs file `position.ino` -- Arduino needs them to match, and arduino-cli compares case-sensitively even on Windows | renamed file to `Position.ino` |
| 3 | `Lesson 26 .../Rawdata` | same case mismatch | renamed file to `Rawdata.ino` |
| 4 | `u-blox_GNSS/Example1_BasicNMEARead` | missing `#include <board.h>` (only 1 of 62 examples lacking it) | added the include |
| 5 | `u-blox_GNSS/.../Example1_FactoryDefaultviaI2C` | include present but on line 9, *after* the line 7 that used the type | moved include above the declaration |
| 6 | `u-blox_GNSS/.../Example2_FactoryDefaultsviaSerial` | ships with both serial options commented out; cannot compile as-shipped by design | uncommented `#define mySerial Serial1` |
| 7 | `u-blox_GNSS/Example12_UseUart` | needs `SoftwareSerial.h`, which does not exist for SAMD (AVR-only bit-banged timing) | disabled it, used hardware `Serial1`; added a ready-to-enable SERCOM "Option B" block |

## Scope policy: what counts as a failure, and what does not

The repo's bundled `.zip` libraries carry 284 of their own example sketches.
Many are vendor examples written for **other hardware entirely**, and no change
on our side could ever make them compile for the SAMD21. Counting those as
"failures" would misrepresent the result, so they are marked **N/A** in
TESTING_CHECKLIST.md and excluded from the totals. Each N/A row states its
reason, and the rules live in `scripts/build-checklist.py` (`NA_RULES`) so the
classification is reproducible rather than hand-waved.

**81 of the 284 are N/A**, by category:

| Reason | Count | Evidence |
|---|---|---|
| ESP32/ESP8266 only | 35 | `FS.h`, `SPIFFS`, bare `<pgmspace.h>` (SAMD ships `<avr/pgmspace.h>`), `D8` |
| AVR only | ~16 | `TCCR2A` (Timer2 registers), `dtostrf`, and two sketches with an explicit `#error This program is only for AVR` |
| Exceeds SAMD21 memory / unsupported DMA | 6 | linker: `region RAM overflowed`, `FLASH overflowed by 19624 bytes`; TFT_eSPI DMA is implemented only for ESP32/STM32/RP2040 |
| Broken upstream | 4 | `lfnTest`, `lfnTestCout`, `TestMkdir`, `TestRmdir` include `SdFatUtil.h`, a header **removed** from SdFat 1.1.4 -- confirmed absent from the zip |
| Not example code | 6 | SdFat's `extras/SdFatTestSuite` harness (the board core's `SDU/extras/SDUBoot` was in this group until `FlashStorage` was installed — it now compiles) |
| Other hardware libs | ~14 | Teensy (`IMXRT_board.h`, `SdFatSdio`), `WiFi101.h`, `epd2in7.h` (Waveshare e-paper), `I2Cdev.h`, display-specific `ILI9341_*`, TFT_eSPI `getTouch` (needs `TOUCH_CS`) |
(35 + 16 + 6 + 4 + 6 + 14 = 81.)

**Result: 203 / 203 in-scope zip sketches compile — zero failures.**

Getting there took 3 library installs and 1 code fix:

| Was failing | Count | Resolution |
|---|---|---|
| `Rtc` name collision | 11 | **Fixed in `Rtc-master.zip`.** The SAMD21's CMSIS header defines `typedef struct {...} Rtc;` (`component/rtc.h:1057`), and the Rtc-master examples declared a *variable* named `Rtc`. Renamed it to `myRtc` in all 11 examples (`Rtc` word-boundary only, so `RtcDS1302` / `RtcDateTime` type names are untouched) and repackaged the zip with an identical internal structure. The repo's own Lesson 12 `DS1307` sketch was never affected. |
| `SD.h` missing | 8 | installed **SD** 1.3.0 (Library Manager) — `SD.h` is in neither the repo nor the board core |
| `JPEGDecoder.h` missing | 4 | installed **JPEGDecoder** 2.0.0 — this unblocked **0** rows: it only let the compiler reach each sketch's *real* error, and all 4 turned out to be out of scope (bare `<pgmspace.h>`, SPIFFS, ESP32-only) |
| `FlashStorage.h` missing | 1 | installed **FlashStorage** 1.0.0 (needed by the board core's own `SDU/extras/SDUBoot`) |

Three sketches only revealed their *real* error once the first missing header
was installed, and turned out to be out of scope after all (2x bare
`<pgmspace.h>`, 1x an ESP32-only `listDir` example) - they are now N/A, which
is why the N/A count rose from 78 to 81.

## REQUIRED LIBRARIES (complete list)

The board package and `5G-NB-IoT_Arduino.zip` alone are **not** enough. All of
the following must be installed for the full 127 to compile. Bundled `.zip`
files are in the repo; the rest come from Library Manager.

| Library | Source | Needed by |
|---|---|---|
| `5G-NB-IoT` | `5G-NB-IoT_Arduino.zip` (repo root) | almost every sketch (`board.h`) |
| `FreeRTOS_SAMD21` | `Arduino-FreeRTOS-SAMD21-master.zip` | `TCPContinuoslyFreeRTOS` |
| `Servo` | `Lesson 06 Servo/Servo.zip` | Lesson 06 |
| `Keypad` | `Lesson 07 Keypad/Keypad.zip` | Lesson 07 |
| `Adafruit_Unified_Sensor` | `Adafruit_Sensor-master.zip` | Lesson 08 |
| `DHT_sensor_library` | `DHT-sensor-library-master.zip` | Lesson 08 |
| `IRLib2` + `IRLibRecv` + `IRLibRecvPCI` + `IRLibFreq` + `IRLibProtocols` | `IRLib2-master.zip` (**5 libraries in one zip**) | Lessons 10, 21 |
| `Rtc_by_Makuna` | `Rtc-master.zip` | Lesson 12 |
| `MFRC522` | `rfid-master.zip` | Lesson 14 |
| `LiquidCrystal` | `LiquidCrystal.zip` | Lesson 15 |
| `Stepper` | `Lesson 20 .../Stepper.zip` | Lessons 20, 21 |
| `IRremote` | `IRremote.zip` | Lesson 21 |
| `SdFat` | `SdFat.zip` (**no wrapper folder -- install manually**) | `NBIoTPhone/SD_card` |
| `TFT_eSPI` | `TFT_eSPI-master.zip` (+ copy `NBIoTPhone/User_Setup.h` over its own) | all `NBIoTPhone` TFT sketches |
| **`Keyboard`** | **Library Manager** | Lessons 10, 21 -- see note below |
| **`Mouse`** | **Library Manager** | Lessons 10, 21 -- see note below |
| `ArduinoJson` | Library Manager | `NBIoTPhone/AWS_MQTTS_Client_Bare` |
| `MicroNMEA` | Library Manager | `u-blox_GNSS/Example2_NMEAParsing` |

**The `Keyboard` / `Mouse` requirement is non-obvious and worth calling out.**
Nothing in the repo mentions them. `IRLib2`'s `IRLibProtocols/IRLib_P12_CYKM.h`
contains:

```c
#if defined(__AVR_ATmega32U4__) || ... || defined(__SAMD21G18A__) || ...
  #include <Keyboard.h>
  #include <Mouse.h>
  #include <HID.h>
#endif
```

`boards.txt` sets `-D__SAMD21G18A__` unconditionally for this board, so that
branch is always taken. `HID` ships with the board core, but `Keyboard` and
`Mouse` must be installed from Library Manager or **both**
`Lesson 10 IR_Receiver_Module` and `Lesson 21 With_Remote` fail with
`fatal error: Keyboard.h: No such file or directory`.

## REQUIRED: the bundled `.zip` files must be INSTALLED as libraries

This is the single most common cause of "it doesn't compile" in this repo, so
it is stated here in full.

**The rule: a `.zip` sitting next to a sketch does nothing. It must be
installed into the Arduino libraries folder.**

When a sketch says `#include <Servo.h>`, the compiler does **not** look in the
sketch's own folder for a `.zip`, and does not look inside `.zip` files at all.
It searches a fixed set of locations, the important one being:

```
C:\Users\<you>\Documents\Arduino\libraries\
```

Each library must be its own folder directly inside that path — e.g.
`libraries\Servo\src\Servo.h`. The `.zip` shipped beside the `.ino` is there
only for the *human*, so you know which library that sketch needs. The
compiler never reads it. This is why a header can "already exist" in the repo
and still produce `No such file or directory`.

**Two correct ways to install:**

1. **Sketch → Include Library → Add .ZIP Library** (preferred) — the IDE
   extracts it into `libraries\` correctly.
2. **Manually** — extract the zip's *inner* library folder into
   `Documents\Arduino\libraries\`.

**Do NOT simply unzip the file where it sits.** Extracting a `.zip` inside the
repo causes two real problems, both of which we hit:

- **Folder-name collisions corrupt sketches.** `Servo.zip` contains a folder
  `Servo/`, and `KitSketches/Lesson 06 Servo/` already contained a sketch
  folder `servo/`. Windows is case-insensitive, so extracting merged the
  library's `src/`, `examples/`, `library.properties` *into the sketch folder*,
  which then failed to compile. Same happened to `Keypad` and `stepper`.
- **It inflates the test scope.** Extracting `SdFat.zip` into
  `NBIoTPhone/SD_card/` added 58 third-party example sketches to the recursive
  scan, taking the count from 127 to 185 (see TESTING_CHECKLIST.md scope note).

**Two zips need special handling:**

- **`IRLib2-master.zip` is five libraries in one.** Add .ZIP Library will
  nest them one level too deep and the compiler will not find them. Extract it
  and copy these five folders *individually* into `libraries\`:
  `IRLib2`, `IRLibRecv`, `IRLibRecvPCI`, `IRLibFreq`, `IRLibProtocols`.
- **`SdFat.zip` has no wrapper folder.** Its root holds `library.properties`
  and `src/` directly, whereas Add .ZIP Library expects a single top-level
  folder to name the library after — so it is rejected. Extract it and copy
  the contents into a folder you create named `libraries\SdFat\`.

**`TFT_eSPI` needs one extra step** beyond installing: copy
`NBIoTPhone/User_Setup.h` over the installed `libraries\TFT_eSPI\User_Setup.h`,
per the repo README. Without it the TFT sketches build against the wrong
display configuration.

**Verifying an install worked:** the library appears as a folder in
`Documents\Arduino\libraries\`. Note the folder is named from the library's
internal `library.properties`, **not** from the zip filename — e.g.
`Adafruit_Sensor-master.zip` installs as `Adafruit_Unified_Sensor`,
`rfid-master.zip` as `MFRC522`, `Rtc-master.zip` as `Rtc_by_Makuna`,
`DHT-sensor-library-master.zip` as `DHT_sensor_library`. That renaming is
correct and expected, not a mistake.

## Recommended Enhancements

Not required to call this branch "2.3.10 compatible" — worth doing once the
checklist pass is further along, so they don't get lost.

- **`README.md` still states "Arduino IDE 2.1.1"** (line 21). Update once the
  checklist is far enough along to state a real minimum-tested version.
- **Real bug found and fixed (not a 2.3.10 issue): `KitSketches/Lesson 08
  .../DHT11/DHT11.ino` failed to compile** with "redeclaration" errors on
  `sensors_event_t`, `sensor_t`, and every `SENSOR_TYPE_*` constant. Root
  cause: `5GHUB_Sensor.h` inside the main `5G-NB-IoT_Arduino.zip` library
  duplicates Adafruit's `Adafruit_Sensor.h` type/enum names verbatim (same
  names, same values, copy-pasted under a different header filename).
  `DHT11.ino` includes both `<board.h>` (pulls in the 5GHUB copy) and
  `<DHT_U.h>` (pulls in the real Adafruit one) in the same file, so every
  shared name gets declared twice. Would have failed identically on IDE
  2.1.1; this sweep just happened to surface it. Found 2026-08-25.
  **First attempt (abandoned) — do not repeat this approach:** renaming the
  duplicated identifiers inside the shared `5G-NB-IoT_Arduino.zip` library
  (prefixing them `_5GHUB_`) did fix `DHT11`, but broke 2 *other* sketches
  (`Lesson 26 BNO055 .../All_data` and `Lesson 23 .../5Ghub_tsl2591`) that
  independently referenced the same old unprefixed names in their own `.ino`
  code — a regression only caught because those two were spot-checked after
  the change. Since `board.h` is included by nearly every sketch in this
  repo, any edit to the shared library has a blast radius that's easy to
  underestimate; this one specific case is not worth a shared-library
  rename just to fix one sketch. **Actual fix applied (root cause, in the library):** `5GHUB_Sensor.h` inside
  `5G-NB-IoT_Arduino.zip` now wraps its duplicated typedefs/enums in
  `#ifndef _ADAFRUIT_SENSOR_H`, so whichever header arrives first wins and the
  second copy is skipped. `_5GHUB_SensorInterface` stays OUTSIDE the guard,
  because the BME680/TSL25911/BNO055 classes inherit from it. `DHT11.ino` keeps
  `#include <board.h>` and simply moves `DHT.h`/`DHT_U.h` above it, so Adafruit's
  include guard is defined first. Verified with no regression: 127/127 repo
  sketches and every `board.h`-using zip example still compile; DHT11 is
  25116 bytes.
  **Lesson for next time:** when a fix could go in a shared library or in the
  one failing sketch, check the sketch-local option first — it's usually
  available (most `board.h` inclusion in this repo looks like habit, not a
  hard dependency) and it can't regress anything else.
- **Real bug found and fixed: two sketch folders had a case mismatch against
  their own `.ino` filename.** `Lesson 26 BNO055 .../Position/` contained
  `position.ino` (lowercase p), and `Rawdata/` contained `rawdata.ino`.
  Arduino requires a sketch's folder name to match its primary `.ino`
  filename, and `arduino-cli` compares this **case-sensitively even on
  Windows** — so it errored with `Can't open sketch: no valid sketch found
  in ...\Position: missing ...\Position\Position.ino` and never reached the
  compiler at all. Note this is a "can't find the sketch" failure, not a code
  error — worth recognizing the shape, since it looks alarming but is trivial.
  Confirming detail: the sibling `All_data/All_data.ino` always passed,
  because its case already matched. **Fix applied:** capitalized the *files*
  to match their folders — `position.ino` → `Position.ino` and
  `rawdata.ino` → `Rawdata.ino` — chosen over lowercasing the folders so all
  three sketches in this lesson follow one convention (`All_data/All_data.ino`
  was already capitalized). Both now compile clean (29200 and 23372 bytes).
  Not a 2.3.10 issue — would have failed the same on 2.1.1. The last upstream
  commit touching these files is literally titled "change folder name", which
  suggests the mismatch was introduced upstream by a rename.
**Recorded in git** (this needed an explicit step): `core.ignorecase = true` is
  git's default on Windows, so git did not detect the case-only rename and
  `git status` showed nothing. It was forced through with `git mv --force` and
  `git ls-files` now shows the capitalized `Position.ino` / `Rawdata.ino`.
  **General lesson:** on Windows a case-only rename is invisible to git by
  default — any case fix needs `git mv --force` plus a `git ls-files` check, or it
  silently will not be committed.
- **The `u-blox_GNSS` example set has an undocumented external dependency.**
  The repo ships the examples but not the library, and doesn't say which
  version/variant is expected anywhere. Worth adding a line to the top-level
  README (or a short note inside `u-blox_GNSS/`) naming the exact library and
  version, so the next person doesn't have to reverse-engineer it the way we
  just did.
- **This undocumented-dependency pattern isn't unique to `u-blox_GNSS` — it's
  now a repeat offender.** `ArduinoSketches/examples/TCPContinuoslyFreeRTOS/`
  ships `Arduino-FreeRTOS-SAMD21-master.zip` right next to the `.ino`, meant to
  be installed via Add .ZIP Library, but nothing states that — it just fails
  to compile with a missing-header error until you notice the bundled zip.
  Since this is the second sketch found with this exact shape, it's worth a
  README pass (or a per-checklist-row note) flagging every sketch that ships
  its own dependency zip, rather than discovering each one via a compile
  failure.
- **`u-blox_GNSS/Example1_BasicNMEARead` is missing `#include <board.h>`.**
  Every other example in `u-blox_GNSS/` (61 of 62) includes it and gets the
  `SFE_UBLOX_GNSS` class from the main library's bundled copy; this one file
  doesn't, so it won't compile as shipped. One-line fix, matches the sibling
  examples' own pattern.
- **Two sketches need a genuinely external Library Manager install** (not
  bundled anywhere in this repo, same shape as u-blox_GNSS itself): `MicroNMEA`
  for `u-blox_GNSS/Example2_NMEAParsing`, and `ArduinoJson` for
  `NBIoTPhone/AWS_MQTTS_Client_Bare`.
- **`u-blox_GNSS/Example12_UseUart` can't be fixed by installing a library at
  all.** It needs `SoftwareSerial.h`, but every Library Manager entry that
  provides that header declares `avr`/`esp8266`/`esp32` only — none claim
  `samd`. This is a real architecture gap, not a missing-dependency one; the
  actual fix is editing the sketch to use a spare hardware UART (e.g.
  `Serial1`) instead of software-bit-banged serial.
- Ruled out as false alarms during the audit (grep matched text that wasn't
  live code): `dht_nonblocking.h` in `KitSketches/Lesson 08 .../DHT11.ino` is
  inside a `/* comment block */`; `Logo.h` (`NBIoTPhone/TFT_BW_Logo`,
  `TFT_Color_Logo`) and `pitches.h` (`KitSketches/Lesson 04 .../`) are local
  files already sitting in their own sketch folders, not external deps.
