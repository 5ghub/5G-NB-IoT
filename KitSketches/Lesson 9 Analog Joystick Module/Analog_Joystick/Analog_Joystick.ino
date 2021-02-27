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

// Arduino pin numbers
const int SW_pin = PA6;
const int X_pin = A0;
const int Y_pin = A1;

void setup() {
  DSerial.begin(9600);  // Initialize serial communications with the PC
  while (!DSerial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(1000);

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
}

void loop() {
  DSerial.print("Switch:  ");
  DSerial.print(digitalRead(SW_pin));
  DSerial.print("\n");
  DSerial.print("X-axis: ");
  DSerial.print(analogRead(X_pin));
  DSerial.print("\n");
  DSerial.print("Y-axis: ");
  DSerial.println(analogRead(Y_pin));
  DSerial.print("\n\n");
  delay(500);
}
