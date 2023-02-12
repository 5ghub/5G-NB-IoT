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

#define OBD_5GHUB_RST   19  // Reset pin. Does a 10msec reset.
#define OBD_5GHUB_SLP   12  // Sleep Pin.

#define DSerial SerialUSB
#define OBD_5GHUB Serial

void setup() 
{  
  // Define outputs to turn on 
  // power through the port and set Reset/Sleep to outputs.
  pinMode(OBD_5GHUB_RST, OUTPUT);
  pinMode(OBD_5GHUB_SLP, OUTPUT);
  digitalWrite(OBD_5GHUB_RST, HIGH);
  digitalWrite(OBD_5GHUB_SLP, HIGH);

  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the OBD AT debug serial!. Enter AT command:!");
  
  OBD_5GHUB.begin(9600);

  while (OBD_5GHUB.read() >= 0);
  digitalWrite(OBD_5GHUB_RST, LOW);
  delay(100);
  digitalWrite(OBD_5GHUB_RST, HIGH);
  delay(100);

  DSerial.println("STN2100 reset successfully.");
}

void loop() 
{
  if (OBD_5GHUB.available() > 0) 
  {
    char incomingByte = OBD_5GHUB.read();
    DSerial.write(incomingByte);
  }

  if (DSerial.available()) 
  {
    char d = DSerial.read();
    OBD_5GHUB.write(d);
    DSerial.write(d);
  }
}
