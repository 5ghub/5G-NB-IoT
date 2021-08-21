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
int previous_pirStat = 0;     // previous PIR status
int pirStat = 0;              // PIR status

void setup()
{
  DSerial.begin(9600);
  while (DSerial.read() >= 0);
  delay(1000);
  DSerial.println("This is PIR Sensor Test.");
  
  pinMode(LED1, OUTPUT); //initialize the ledPin as an output
  pinMode(LED2, OUTPUT);
  pinMode(ONOFF, OUTPUT);
  pinMode(PA6, INPUT);

  digitalWrite(LED2, HIGH);
  digitalWrite(ONOFF, HIGH); //disable LTE modem
}
/******************************************/
void loop()
{
 pirStat = digitalRead(PA6); 
 if (pirStat == HIGH && previous_pirStat != HIGH) 
 {            
   // if motion detected, turn LED ON
   digitalWrite(LED1, HIGH);
   DSerial.println("Motion Detected");
 } 
 else if (pirStat == LOW && previous_pirStat != LOW) 
 {
   digitalWrite(LED1, LOW); // turn LED OFF if we have no motion
   DSerial.println("No Motion");
 }

  previous_pirStat = pirStat;
  digitalWrite(LED2, HIGH);
}
