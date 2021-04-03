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

// Global SIM APN
char APN[] = "super";

unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
_5G_NB_IoT_TCPIP _5GNBIoTTCPIP(ATSerial, DSerial);
_5G_NB_IoT_Common _5GNBIoTCommon(ATSerial, DSerial);


GNSS_Work_Mode_t mode = STAND_ALONE;
_5G_NB_IoT_GNSS _5GNBIoT(ATSerial, DSerial);

#define PIN_NUMBER    21
int allPins[] = {LED1, LED2, A0, A1, A2, A3, A4, A5, SCL, SDA, SCK, MISO, MOSI, SS, PA6, PA7, PA8, PB11, PB22, PB23, RELAY_CTRL};

void setup()
{     
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  /************************************************************************/
  /*                                                                      */
  /*                                                                      */
  /* Turn on the modem AND gnss and register on mobile operator network   */  
  /*                                                                      */
  /*                                                                      */
  /************************************************************************/  
  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  char apn_error[64];
  while(!_5GNBIoTTCPIP.InitAPN(comm_pdp_index, APN, "", "", apn_error))
  {
    DSerial.println(apn_error);
    delay(1000);
  }  
  DSerial.println(apn_error);

  _5GNBIoT.SetGNSSOutputPort(USBNMEA);

  _5GNBIoT.SetDevCommandEcho(false);
  delay(100);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) 
  {
    DSerial.println(inf);
  }

  while (!_5GNBIoT.TurnOnGNSS(mode, WRITE_MODE)) 
  {
    DSerial.println("\r\nOpen the GNSS Function Fail!");
    if (_5GNBIoT.TurnOnGNSS(mode, READ_MODE)) 
    {
      DSerial.println("\r\nThe GNSS Function is Opened!");
      _5GNBIoT.TurnOffGNSS();
    }
  }
  DSerial.println("\r\nOpen the GNSS Function Success!");


  /************************************************************************/
  /*                                                                      */
  /*                                                                      */
  /*     Set up all pins on J2                                            */  
  /*                                                                      */
  /*                                                                      */
  /************************************************************************/  
  pinMode(ONOFF, OUTPUT);

  // This turn off the power supply to the modem
  digitalWrite(ONOFF, LOW);

  for (int i = 0; i < PIN_NUMBER; i++)
  {
    pinMode(allPins[i], OUTPUT);
  }

    //turn off two LEDs
    digitalWrite(LED1, HIGH);
    digitalWrite(LED1, HIGH);
}

// the loop function runs over and over again forever
void loop() 
{
  static int activepin = 0;

  //turn off old pin
  if (activepin == 0 || activepin == 1)
    digitalWrite(allPins[activepin], HIGH);
  else
    digitalWrite(allPins[activepin], LOW);

  activepin++;
  if (activepin >= PIN_NUMBER)
    activepin = 0;


  //turn on new pin
  if (activepin == 0 || activepin == 1)
    digitalWrite(allPins[activepin], LOW);
  else
    digitalWrite(allPins[activepin], HIGH);


  //Keep dispalying the registration status of the modem
  if (_5GNBIoTCommon.DevNetRegistrationStatus() == Net_Status_t::REGISTERED)
  {
      DSerial.println("\r\n_REGISTERED!");
  }

  delay(500); // wait for a 0.5 second
}
