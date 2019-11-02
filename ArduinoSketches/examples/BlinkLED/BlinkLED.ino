/*
* 
*Copyright 2019, 5G HUB
*
*Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
*associated documentation files (the "Software"), to deal in the Software without restriction, including 
*without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
*copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the 
*following conditions:
*
*The above copyright notice and this permission notice shall be included in all copies or substantial 
*portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
*TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
*THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
*CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
*IN THE SOFTWARE.
*
*/

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(PIN_LED_RXL,OUTPUT);
  pinMode(PIN_LED_TXL,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PIN_LED_RXL,LOW);
  digitalWrite(PIN_LED_TXL,HIGH);
  delay(500); // wait for a 5 second
  digitalWrite(PIN_LED_RXL,HIGH);
  digitalWrite(PIN_LED_TXL,LOW);
  delay(500); // wait for a 5 second
}
