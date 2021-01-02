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

#include <board.h>

#define ATSerial Serial1

GNSS_Work_Mode_t mode = STAND_ALONE;

_5G_NB_IoT_GNSS _5GNBIoT(ATSerial, ATSerial);

void setup() {
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  TFTinit("GNSS test");

  _5GNBIoT.InitModule();
  TFTprint("\r\n_5GNBIoT.InitModule() OK!");

  //turn off GNSS if it is ON
  _5GNBIoT.TurnOffGNSS();

  //_5GNBIoT.SetGNSSOutputPort(UARTNMEA);
  _5GNBIoT.SetGNSSOutputPort(USBNMEA);

  _5GNBIoT.SetDevCommandEcho(false);
  delay(100);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) {
    TFTprint(inf);
  }

  while (!_5GNBIoT.TurnOnGNSS(mode, WRITE_MODE)) {
    TFTprint("\r\nOpen the GNSS Function Fali!");
    if (_5GNBIoT.TurnOnGNSS(mode, READ_MODE)) {
      TFTprint("\r\nThe GNSS Function is Opened!");
      _5GNBIoT.TurnOffGNSS();
    }
  }
  TFTprint("\r\nOpen the GNSS Function Success!");
}

void loop() {
  char gnss_posi[128];
  while (!_5GNBIoT.GetGNSSPositionInformation(gnss_posi)) {
    TFTprint("\r\nGet the GPGSV Fail!");
    delay(5000);
  }
  TFTprint("\r\nGet the GPGSV Success!\r\n");
  TFTprint(gnss_posi);
  delay(1000);
}
