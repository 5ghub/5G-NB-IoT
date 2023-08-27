/*

  This example shows how to reset the U-Blox module to factory defaults over I2C.

*/

SFE_UBLOX_GNSS myGNSS;

#include <board.h>

#define DSerial SerialUSB


void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  while (DSerial.available()) DSerial.read(); //Trash any incoming chars
  DSerial.println("Press a key to reset module to factory defaults");
  while (DSerial.available() == false) ; //Wait for user to send character

  myGNSS.factoryReset(); //Reset everything: baud rate, I2C address, update rate, everything.

  delay(5000); // Wait while the module restarts

  while (myGNSS.begin() == false) //Attempt to re-connect
  {
    delay(1000);
    DSerial.println(F("Attempting to re-connect to u-blox GNSS..."));
  }

  DSerial.println("Unit has now been factory reset. Freezing...");
  while(1); // Do nothing more
}

void loop()
{

}
