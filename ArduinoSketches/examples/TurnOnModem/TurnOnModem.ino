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

_5G_NB_IoT_TCPIP _5GNBIoT(ATSerial, DSerial);

// the setup function runs once when you press reset or power the board
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

   // This must exist for miniPCIe card
   //pinMode(RESET_N, OUTPUT);
   //digitalWrite(RESET_N, HIGH);
 
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(500); // wait for a 5 second
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  delay(500); // wait for a 5 second
}
