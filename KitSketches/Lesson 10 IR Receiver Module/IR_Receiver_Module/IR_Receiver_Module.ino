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
#include <IRLibAll.h>

IRrecv myReceiver(MISO);
IRdecode myDecoder;    //Decoder object

//#define DSerial Serial
#define DSerial SerialUSB

void setup()
{
  DSerial.begin(9600);  // Initialize serial communications with the PC
  while (!DSerial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(1000);
  DSerial.println("IR Receiver Button Decode");

  myReceiver.enableIRIn(); // Start the receiver
}/*--(end setup )---*/


void loop() {
  if (myReceiver.getResults()) {
    myDecoder.decode();
    if (myDecoder.protocolNum == NEC) {
      switch (myDecoder.value) {
        case 0xFFA25D: DSerial.println("POWER"); break;
        case 0xFFE21D: DSerial.println("FUNC/STOP"); break;
        case 0xFF629D: DSerial.println("VOL+"); break;
        case 0xFF22DD: DSerial.println("FAST BACK");    break;
        case 0xFF02FD: DSerial.println("PAUSE");    break;
        case 0xFFC23D: DSerial.println("FAST FORWARD");   break;
        case 0xFFE01F: DSerial.println("DOWN");    break;
        case 0xFFA857: DSerial.println("VOL-");    break;
        case 0xFF906F: DSerial.println("UP");    break;
        case 0xFF9867: DSerial.println("EQ");    break;
        case 0xFFB04F: DSerial.println("ST/REPT");    break;
        case 0xFF6897: DSerial.println("0");    break;
        case 0xFF30CF: DSerial.println("1");    break;
        case 0xFF18E7: DSerial.println("2");    break;
        case 0xFF7A85: DSerial.println("3");    break;
        case 0xFF10EF: DSerial.println("4");    break;
        case 0xFF38C7: DSerial.println("5");    break;
        case 0xFF5AA5: DSerial.println("6");    break;
        case 0xFF42BD: DSerial.println("7");    break;
        case 0xFF4AB5: DSerial.println("8");    break;
        case 0xFF52AD: DSerial.println("9");    break;
        case 0xFFFFFFFF: DSerial.println(" REPEAT"); break;

        default:
          DSerial.println(" other button   ");
      }

      delay(500); // Do not get immediate repeat

      myReceiver.enableIRIn(); //Restart the receiver
    }
  }
}
