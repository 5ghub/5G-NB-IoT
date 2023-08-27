/*
  Get fix type and RTK fix type if available

  This example shows how to query a u-blox module for fix type and RTK fix type.
  The fix type is as follows:
    0 = no fix
    1 = dead reckoning (requires external sensors)
    2 = 2D (not quite enough satellites in view)
    3 = 3D (the standard fix)
    4 = GNSS + dead reckoning (requires external sensors)
    5 = Time fix only

  Additionally, if we are doing RTK, we can figure out if we have a floating 
  RTK solution or if we have been able to resolve a fixec solution (better precision).

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
  //Wire.setClock(400000); //Optional. Increase I2C clock speed to 400kHz.

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer

    long latitude = myGNSS.getLatitude();
    DSerial.print(F("Lat: "));
    DSerial.print(latitude);

    long longitude = myGNSS.getLongitude();
    DSerial.print(F(" Long: "));
    DSerial.print(longitude);

    long altitude = myGNSS.getAltitude();
    DSerial.print(F(" Alt: "));
    DSerial.print(altitude);

    byte fixType = myGNSS.getFixType();
    DSerial.print(F(" Fix: "));
    if(fixType == 0) DSerial.print(F("No fix"));
    else if(fixType == 1) DSerial.print(F("Dead reckoning"));
    else if(fixType == 2) DSerial.print(F("2D"));
    else if(fixType == 3) DSerial.print(F("3D"));
    else if(fixType == 4) DSerial.print(F("GNSS + Dead reckoning"));
    else if(fixType == 5) DSerial.print(F("Time only"));

    byte RTK = myGNSS.getCarrierSolutionType();
    DSerial.print(" RTK: ");
    DSerial.print(RTK);
    if (RTK == 0) DSerial.print(F(" (No solution)"));
    else if (RTK == 1) DSerial.print(F(" (High precision floating fix)"));
    else if (RTK == 2) DSerial.print(F(" (High precision fix)"));

    DSerial.println();
  }

}
