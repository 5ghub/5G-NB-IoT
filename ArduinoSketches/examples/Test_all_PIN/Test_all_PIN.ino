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

#include <board.h>

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

GNSS_Work_Mode_t mode = STAND_ALONE;

_5G_NB_IoT_GNSS _5GNBIoT(ATSerial, DSerial);

#define PIN_NUMBER    17

int allPins[] = {PIN_LED_RXL, PIN_LED_TXL, PIN_LED_13, PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4,
                 PIN_A5, 8, 9, 36, 35, 34, 37, 32, 33
                };

// LEDs

void setup()
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetGNSSOutputPort(USBNMEA);

  _5GNBIoT.SetDevCommandEcho(false);
  delay(100);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) {
    DSerial.println(inf);
  }

  while (!_5GNBIoT.TurnOnGNSS(mode, WRITE_MODE)) {
    DSerial.println("\r\nOpen the GNSS Function Fail!");
    if (_5GNBIoT.TurnOnGNSS(mode, READ_MODE)) {
      DSerial.println("\r\nThe GNSS Function is Opened!");
      _5GNBIoT.TurnOffGNSS();
    }
  }
  DSerial.println("\r\nOpen the GNSS Function Success!");

  for (int i = 0; i < PIN_NUMBER; i++)
  {
    pinMode(allPins[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  static int activepin = 0;

  // Light Up all LEDS
  // These two LEDs are active LOW
  digitalWrite(PIN_LED_RXL, LOW);
  digitalWrite(PIN_LED_TXL, LOW);

  for (int i = 2; i < PIN_NUMBER; i++)
  {
    digitalWrite(allPins[i], HIGH);
  }

  if (activepin == 0 || activepin == 1)
    digitalWrite(allPins[activepin], HIGH);
  else
    digitalWrite(allPins[activepin], LOW);

  activepin++;
  if (activepin >= PIN_NUMBER)
    activepin = 0;

  delay(500); // wait for a 0.5 second
}
