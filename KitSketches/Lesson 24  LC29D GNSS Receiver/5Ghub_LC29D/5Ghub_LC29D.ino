/*

  Copyright 2021, 5G HUB

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
/* Test 3 UARTs. Pins:
 *  UART#1: PA6, PA7
 *  UART#2: MOSI, SCK
 *  UART#3: SDA, SCL 
 
 */
#include <board.h>
#include "wiring_private.h"

#define DSerial SerialUSB

String readString;

void setup() 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);

  // This is UART #1
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

//  digitalWrite(LED1, HIGH);
//  digitalWrite(LED2, HIGH);
}

void loop() 
{
  // This is UART #1   
  Serial.println("This is the Serial #1");
  
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    char incomingByte = Serial.read();
    
    if (incomingByte =='\n') 
    {
      DSerial.print(readString);
      readString="";
    } 
    readString += incomingByte;
  }
}
