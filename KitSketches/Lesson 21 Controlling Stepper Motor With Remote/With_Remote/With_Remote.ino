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
#include "Stepper.h"
#include <IRLibAll.h>

/*----- Variables, Pins -----*/
const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
int receiver = MISO; // Signal Pin of IR receiver to Arduino Digital Pin 6

/*-----( Declare objects )-----*/
// Setup of proper sequencing for Motor Driver Pins
// In1, In2, In3, In4 in the sequence 1-3-2-4
// IN1 - PA6, IN2 - PA7, IN3 - SS, IN4 - MOSI
Stepper myStepper(stepsPerRevolution, PA6, SS, PA7, MOSI);

IRrecv myReceiver(MISO); //receiver on pin MISO
IRdecode myDecoder;   //Decoder object

void setup()
{
  myStepper.setSpeed(rolePerMinute);
  myReceiver.enableIRIn(); // Start the receiver
}

void loop()
{
  if (myReceiver.getResults()) 
  {
    myDecoder.decode();
    if (myDecoder.protocolNum == NEC) 
    {
      switch (myDecoder.value) 
      {
        case 0xFFA857: // VOL+ button pressed
          myStepper.step(stepsPerRevolution);
          break;

        case 0xFFE01F: // VOL- button pressed
          myStepper.step(-stepsPerRevolution);
          break;
      }

      delay(500); // Do not get immediate repeat

      myReceiver.enableIRIn(); //Restart the receiver
    }
  }
}/* --end main loop -- */
