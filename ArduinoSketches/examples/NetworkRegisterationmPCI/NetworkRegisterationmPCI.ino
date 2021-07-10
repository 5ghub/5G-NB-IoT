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

#define DSerial SerialUSB
#define ATSerial Serial1

// Global SIM APN
char APN[] = "super";

// AT&T APN
//char APN[] = "m2mNB16.com.attz";
unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16

_5G_NB_IoT_TCPIP _5GNBIoT(ATSerial, DSerial);

void setup() 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the miniPCIe modems power on and Network registration.");
  
  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  // This must exist for miniPCIe card
  pinMode(RESET_N, OUTPUT);
  digitalWrite(RESET_N, HIGH);

  // Modem of EG9X takes about  15 seconds to power-on
  delay(15000);
  
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  char apn_error[64];
  while(!_5GNBIoT.InitAPN(comm_pdp_index, APN, "", "", apn_error))
  {
    DSerial.println(apn_error);
    delay(1000);
  }  
  DSerial.println(apn_error);
}

void loop() 
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(500); // wait for a 5 second
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  delay(500); // wait for a 5 second
}
