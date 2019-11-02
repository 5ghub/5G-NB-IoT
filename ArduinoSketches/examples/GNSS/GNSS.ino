/*

  Copyright 2019, 5G HUB

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

#include "5G-NB-IoT_GNSS.h"

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1
#define UART_DEBUG

GNSS_Work_Mode_t mode = STAND_ALONE;

_5G_NB_IoT_GNSS _5GNBIoT(ATSerial, DSerial);

void setup() {
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  Serial.begin(115200);

  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetGNSSOutputPort(UARTNMEA);

  _5GNBIoT.SetDevCommandEcho(false);
  delay(100);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) {
    DSerial.println(inf);
  }

  while (!_5GNBIoT.TurnOnGNSS(mode, WRITE_MODE)) {
    DSerial.println("\r\nOpen the GNSS Function Fali!");
    if (_5GNBIoT.TurnOnGNSS(mode, READ_MODE)) {
      DSerial.println("\r\nThe GNSS Function is Opened!");
      _5GNBIoT.TurnOffGNSS();
    }
  }
  DSerial.println("\r\nOpen the GNSS Function Success!");
}

void loop() {
  char gnss_posi[128];
  while (!_5GNBIoT.GetGNSSPositionInformation(gnss_posi)) {
    DSerial.println("\r\nGet the GNSS Position Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
    }
    delay(5000);
  }
  DSerial.println("\r\nGet the GNSS Position Success!");
  DSerial.println(gnss_posi);
}
