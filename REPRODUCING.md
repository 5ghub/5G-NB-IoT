# Reproducing the 2.3.10 build from scratch

This branch compiles **127 / 127** of the repo's own sketches on Arduino IDE
2.3.10. That result depends on two things: the code in this repo, and a set of
libraries that do **not** live in this repo. The code half travels with a
`git clone`; the library half does not, and nothing in the upstream README says
which versions to use.

This file closes that gap. Follow it on a clean machine and you should reach
127 / 127 without applying a single fix by hand — every fix this branch needed
is already committed, including the two repackaged `.zip` libraries.

## What is already fixed in the repo

You do **not** need to re-apply any of these. They are committed, and the two
zip-based ones are inside the very zips you are told to install below, so
installing per these instructions gives you the fixed library:

| Fix | Lives in |
|---|---|
| Adafruit type redeclaration guard (`#ifndef _ADAFRUIT_SENSOR_H`) | `5G-NB-IoT_Arduino.zip` → `5GHUB_Sensor.h` |
| `Rtc` variable vs CMSIS `Rtc` type collision, 11 examples | `Rtc-master.zip` |
| `DHT11.ino` include order | tracked source |
| `Position.ino` / `Rawdata.ino` filename case | tracked source (recorded via `git mv --force`) |
| 4 `u-blox_GNSS` sketch fixes | tracked source |

A fresh `git clone` writes `Position.ino` / `Rawdata.ino` with correct
capitalisation on any OS. The `core.ignorecase` trap only affects renaming in
place, never cloning.

## Toolchain versions this result was produced with

| Component | Version |
|---|---|
| Arduino IDE | 2.3.10 |
| `arduino-cli` (bundled with the IDE) | 1.5.1 |
| Board package `5G-NB-IoT:samd` | 1.0.6 |
| FQBN | `5G-NB-IoT:samd:5G-NB-IoT` |

The board package pins its own legacy toolchain — `arm-none-eabi-gcc
4.8.3-2014q1`, `bossac 1.7.0`, `openocd 0.9.0-arduino5-static`, `CMSIS 4.5.0`,
`CMSIS-Atmel 1.1.0`. These download from Arduino's servers and are outside this
repo's control; see "Known drift risks" at the end.

## Step 1 — board package

In the IDE: File → Preferences → Additional Boards Manager URLs, add

```
https://raw.githubusercontent.com/5ghub/5G-NB-IoT/master/package_5G-NB-IoT_index.json
```

then Tools → Board → Boards Manager → install **5G-NB-IoT SAMD Boards** 1.0.6,
and select board **5G NB-IoT (Native USB Port)**.

Equivalent on the command line:

```bash
arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/5ghub/5G-NB-IoT/master/package_5G-NB-IoT_index.json
arduino-cli core install 5G-NB-IoT:samd@1.0.6 --additional-urls https://raw.githubusercontent.com/5ghub/5G-NB-IoT/master/package_5G-NB-IoT_index.json
```

## Step 2 — libraries

26 library folders must end up in `Documents\Arduino\libraries\`. They come
from two places, and the distinction matters:

- **18 are pinned by this repo.** They install from `.zip` files committed
  beside their sketches, so their versions are fixed by the clone itself and
  cannot drift.
- **8 come from Library Manager**, which installs *latest* by default. These
  are the entire drift surface, so install them **by exact version**.

### 2a. From bundled zips (pinned by the repo — 18 folders)

Install each with Sketch → Include Library → **Add .ZIP Library**, except the
two special cases noted below. The version column is what the zip contains; it
is recorded here so you can confirm an install rather than guess.

| Zip (path in repo) | Installs as | Version |
|---|---|---|
| `5G-NB-IoT_Arduino.zip` | `5G-NB-IoT` | 1.0.0 |
| `ArduinoSketches/examples/TCPContinuoslyFreeRTOS/Arduino-FreeRTOS-SAMD21-master.zip` | `FreeRTOS_SAMD21` | 2.3.0 |
| `KitSketches/Lesson 06 Servo/Servo.zip` | `Servo` | 1.1.2 |
| `KitSketches/Lesson 07 Keypad/Keypad.zip` | `Keypad` | (no metadata) |
| `KitSketches/Lesson 08 .../Adafruit_Sensor-master.zip` | `Adafruit_Unified_Sensor` | 1.0.3 |
| `KitSketches/Lesson 08 .../DHT-sensor-library-master.zip` | `DHT_sensor_library` | 1.3.7 |
| `KitSketches/Lesson 10 .../IRLib2-master.zip` | `IRLib2` + 4 more — **see 2c** | (no metadata) |
| `KitSketches/Lesson 12 .../Rtc-master.zip` | `Rtc_by_Makuna` | 2.3.3 |
| `KitSketches/Lesson 14 .../rfid-master.zip` | `MFRC522` | 1.4.5 |
| `KitSketches/Lesson 15 .../LiquidCrystal.zip` | `LiquidCrystal` | 1.0.5 |
| `KitSketches/Lesson 20 .../Stepper.zip` | `Stepper` | 1.1.3 |
| `KitSketches/Lesson 21 .../IRremote.zip` | `IRremote` | (no metadata) |
| `NBIoTPhone/SD_card/SdFat.zip` | `SdFat` | 1.1.4 — **see 2c** |
| `NBIoTPhone/TFTLibrary/TFT_eSPI-master.zip` | `TFT_eSPI` | 2.2.12 — **see 2d** |

The installed folder is named from the zip's internal `library.properties`,
**not** from the zip filename — `Adafruit_Sensor-master.zip` becomes
`Adafruit_Unified_Sensor`, `rfid-master.zip` becomes `MFRC522`,
`Rtc-master.zip` becomes `Rtc_by_Makuna`. That renaming is correct.

### 2b. From Library Manager (NOT pinned by the repo — install by version)

Only the first five are needed for the 127 sketches. The last three are needed
only to compile the vendor examples bundled *inside* the zips (Part 2 of
TESTING_CHECKLIST.md).

| Library | Version | Needed by |
|---|---|---|
| SparkFun u-blox GNSS Arduino Library | **2.2.29** | all 62 `u-blox_GNSS` examples |
| ArduinoJson | 7.4.3 | `NBIoTPhone/AWS_MQTTS_Client_Bare` |
| MicroNMEA | 2.0.6 | `u-blox_GNSS/Example2_NMEAParsing` |
| Keyboard | 1.0.7 | Lessons 10 and 21 — see note |
| Mouse | 1.0.1 | Lessons 10 and 21 — see note |
| SD | 1.3.0 | Part 2 only (SdFat's own examples) |
| JPEGDecoder | 2.0.0 | Part 2 only |
| FlashStorage | 1.0.0 | Part 2 only (`SDU/extras/SDUBoot`) |

```bash
arduino-cli lib install "SparkFun u-blox GNSS Arduino Library@2.2.29"
arduino-cli lib install "ArduinoJson@7.4.3"
arduino-cli lib install "MicroNMEA@2.0.6"
arduino-cli lib install "Keyboard@1.0.7"
arduino-cli lib install "Mouse@1.0.1"
# Part 2 only:
arduino-cli lib install "SD@1.3.0" "JPEGDecoder@2.0.0" "FlashStorage@1.0.0"
```

**Why `Keyboard` and `Mouse` are required, when nothing in the repo mentions
them.** `IRLib2`'s `IRLibProtocols/IRLib_P12_CYKM.h` includes `<Keyboard.h>`
and `<Mouse.h>` under `#if defined(__SAMD21G18A__)`, and `boards.txt` defines
`-D__SAMD21G18A__` unconditionally for this board — so that branch is always
taken. Without them, both `Lesson 10 IR_Receiver_Module` and
`Lesson 21 With_Remote` fail with `fatal error: Keyboard.h: No such file or
directory`. `HID` ships with the core, so only these two are missing.

### 2c. Two zips that Add .ZIP Library cannot handle

- **`IRLib2-master.zip` is five libraries in one.** Add .ZIP Library nests them
  one level too deep and the compiler will not find them. Extract it and copy
  these five folders *individually* into `libraries\`: `IRLib2`, `IRLibRecv`,
  `IRLibRecvPCI`, `IRLibFreq`, `IRLibProtocols`.
- **`SdFat.zip` has no wrapper folder.** Its root holds `library.properties`
  and `src/` directly, and Add .ZIP Library expects one top-level folder to
  name the library after, so it rejects the zip. Create `libraries\SdFat\`
  yourself and copy the zip's contents into it.

### 2d. TFT_eSPI needs one extra step

After installing it, copy `NBIoTPhone/User_Setup.h` over the installed
`libraries\TFT_eSPI\User_Setup.h`, per the upstream README. Without this the
TFT sketches build against the wrong display configuration.

### Do not unzip these files where they sit

Extracting a bundled zip inside the repo causes two real problems, both of
which were hit during this work:

- **Folder-name collisions corrupt sketches.** `Servo.zip` contains `Servo/`,
  and `KitSketches/Lesson 06 Servo/` already contains a sketch folder `servo/`.
  Windows is case-insensitive, so extracting merges the library's `src/`,
  `examples/` and `library.properties` *into the sketch folder*, which then
  stops compiling. The same applies to `Keypad` and `stepper`.
- **It inflates the test scope.** Extracting `SdFat.zip` into
  `NBIoTPhone/SD_card/` adds 58 third-party example sketches to any recursive
  scan, taking the sketch count from 127 to 185.

## Step 3 — verify

```powershell
powershell -ExecutionPolicy Bypass -File scripts\test-compile-all.ps1
```

The script locates the repo from its own path, so it works from any clone
directory. It writes `compile_report.csv` and `compile_report.log`.

**Expected result: 127 sketches found, 127 pass, 0 fail.** Anything else means
the environment differs from the one documented here — check library versions
first, since that is by far the most likely cause.

To also compile the 284 vendor examples bundled inside the zips, use
`scripts/test-compile-zips.sh` (Git Bash). It extracts each zip to a temp
directory **outside** the repo. Expected: 203 / 203 in scope, 81 N/A. Every N/A
row in TESTING_CHECKLIST.md quotes the compiler error it was classified from,
so you can check any exclusion against your own output.

## Safe dry run — verify without destroying your setup

You do not need to uninstall anything to test this guide. Park the existing
libraries folder instead; every step is reversible.

```powershell
# 1. Park the current libraries (reversible - nothing is deleted)
Rename-Item "$env:USERPROFILE\Documents\Arduino\libraries" "libraries_backup"

# 2. Clone the branch somewhere scratch
git clone -b IDE_2.3.10 https://github.com/5ghub/5G-NB-IoT.git "$env:TEMP\5G-verify"

# 3. Follow Step 2 above against that clone, then:
powershell -ExecutionPolicy Bypass -File "$env:TEMP\5G-verify\scripts\test-compile-all.ps1"

# 4. Restore your original setup
Remove-Item "$env:USERPROFILE\Documents\Arduino\libraries" -Recurse -Force
Rename-Item "$env:USERPROFILE\Documents\Arduino\libraries_backup" "libraries"
```

Step 4 deletes only the libraries installed during the test, then puts the
original folder back under its own name. Confirm `libraries_backup` exists
before running it.

## Known drift risks

These are the reasons a future rebuild could fail even though nothing in this
repo changed. They are listed in rough order of likelihood.

1. **Library Manager installs *latest* by default.** The eight libraries in 2b
   are not pinned by the repo. The dangerous one is **SparkFun u-blox GNSS**:
   v3 is a rewrite with restructured examples and is not backward compatible
   with v2.2.29, so installing it would break a large share of the 62 u-blox
   sketches in ways unrelated to this branch. Always install that one by
   version.
2. **The DHT11 fix depends on an external library's private include guard.**
   `5GHUB_Sensor.h` skips its duplicate type definitions when
   `_ADAFRUIT_SENSOR_H` is already defined. If Adafruit ever renames that
   guard, the skip silently stops matching and the redeclaration errors return
   — with no warning, because nothing about the change is an error in itself.
   If DHT11 ever breaks again, check that macro name first.
3. **The board package's legacy toolchain is downloaded, not vendored.**
   `arm-none-eabi-gcc 4.8.3-2014q1` dates from 2014. If Arduino retires those
   artifacts from its package servers, the board install fails and no change in
   this repo can fix it.
4. **`SdFat` version changes which examples are out of scope.** 1.1.4 removed
   `SdFatUtil.h`, which is why four of its bundled examples are N/A. A
   different version shifts that set.
