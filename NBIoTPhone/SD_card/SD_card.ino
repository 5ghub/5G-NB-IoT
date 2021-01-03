/*

  Copyright 2020, 5G HUB

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
  associated documentation files (the "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
  following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

/*
  SD card read/write
  An example of the SdFatSoftSpi template class.
  Use SdFat version 1.1.4 and set ENABLE_SOFTWARE_SPI_CLASS to 1 SdFat/SdFatConfig.h
*/

#include <board.h>
#include <SPI.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS  // Must be set in SdFat/SdFatConfig.h

// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 18;
const uint8_t SOFT_MOSI_PIN = 17;
const uint8_t SOFT_SCK_PIN  = 8;
#define SD_CS_PIN 9

SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;
File myFile;

void setup() {
  TFTinit("SD card test");

  if (!sd.begin(SD_CS_PIN)) {
    TFTprint("\r\nInitialization failed!");
    return;
  }
  TFTprint("\r\nInitialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = sd.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    TFTprint("\rWriting to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    TFTprint("\rdone.");
  } else {
    // if the file didn't open, print an error:
    TFTprint("\rerror opening test.txt");
  }

  // re-open the file for reading:
  myFile = sd.open("test.txt");
  if (myFile) {
    TFTprint("\rtest.txt:");

    // read from the file until there's nothing else in it:
    char buff[128];
    int i = 0;
    while (myFile.available()) {
      buff[i] = myFile.read();
      i++;
    }
    buff[i] = 0;
    TFTprint(String(buff));

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    TFTprint("\r\error opening test.txt");
  }
}

void loop() {
}
#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif  //ENABLE_SOFTWARE_SPI_CLASS
