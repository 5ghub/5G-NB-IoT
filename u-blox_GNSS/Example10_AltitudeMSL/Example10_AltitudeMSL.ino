/*
  This example shows how to query a u-blox module for its lat/long/altitude. 

  getAltitude() reports mm above ellipsode model of the globe. There are some
  instances where altitude above Mean Sea Level is better. This example shows how 
  to use getAltitudeMSL(). The difference varies but is ~20m.
  Ellipsoid model: https://www.esri.com/news/arcuser/0703/geoid1of3.html
  Difference between Ellipsoid Model and Mean Sea Level: https://eos-gnss.com/elevation-for-beginners/
  
  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!
*/

#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Tracks the passing of 2000ms (2 seconds)

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
    DSerial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    DSerial.print(F(" Alt: "));
    DSerial.print(altitude);
    DSerial.print(F(" (mm)"));

    long altitudeMSL = myGNSS.getAltitudeMSL();
    DSerial.print(F(" AltMSL: "));
    DSerial.print(altitudeMSL);
    DSerial.print(F(" (mm)"));

    DSerial.println();
  }
}
