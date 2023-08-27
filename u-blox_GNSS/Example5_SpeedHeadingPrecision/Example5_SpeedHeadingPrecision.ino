/*
  This example shows how to query a u-blox module for its lat/long/altitude.

  Note: Long/lat are large numbers because they are * 10^7. To convert lat/long
  to something google maps understands simply divide the numbers by 1,000,000. We
  do this so that we don't have to use floating point numbers.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!
*/

#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

void setup()
{
  DSerial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  DSerial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer
    
    long latitude = myGNSS.getLatitude();
    DSerial.print(F("Lat: "));
    DSerial.print(latitude);

    long longitude = myGNSS.getLongitude();
    DSerial.print(F(" Long: "));
    DSerial.print(longitude);

    long speed = myGNSS.getGroundSpeed();
    DSerial.print(F(" Speed: "));
    DSerial.print(speed);
    DSerial.print(F(" (mm/s)"));

    long heading = myGNSS.getHeading();
    DSerial.print(F(" Heading: "));
    DSerial.print(heading);
    DSerial.print(F(" (degrees * 10^-5)"));

    int pDOP = myGNSS.getPDOP();
    DSerial.print(F(" pDOP: "));
    DSerial.print(pDOP / 100.0, 2); // Convert pDOP scaling from 0.01 to 1

    DSerial.println();
  }
}
