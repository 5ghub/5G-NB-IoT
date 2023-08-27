/*
  This example shows how to change the u-blox module's dynamic platform model and then
  query its lat/long/altitude. We also turn off the NMEA output on the I2C port.
  This decreases the amount of I2C traffic dramatically.

  Possible values for the dynamic model are: PORTABLE, STATIONARY, PEDESTRIAN, AUTOMOTIVE,
  SEA, AIRBORNE1g, AIRBORNE2g, AIRBORNE4g, WRIST, BIKE

  Note: Long/lat are large numbers because they are * 10^7. To convert lat/long
  to something google maps understands simply divide the numbers by 10,000,000. We
  do this so that we don't have to use floating point numbers.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!

*/

#include <board.h>

#define DSerial SerialUSB


SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

void setup()
{
  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);

  DSerial.println(F("  u-blox Example"));

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  // If we are going to change the dynamic platform model, let's do it here.
  // Possible values are:
  // PORTABLE, STATIONARY, PEDESTRIAN, AUTOMOTIVE, SEA, AIRBORNE1g, AIRBORNE2g, AIRBORNE4g, WRIST, BIKE

  if (myGNSS.setDynamicModel(DYN_MODEL_PORTABLE) == false) // Set the dynamic model to PORTABLE
  {
    DSerial.println(F("*** Warning: setDynamicModel failed ***"));
  }
  else
  {
    DSerial.println(F("Dynamic platform model changed successfully!"));
  }

  // Let's read the new dynamic model to see if it worked
  uint8_t newDynamicModel = myGNSS.getDynamicModel();
  if (newDynamicModel == DYN_MODEL_UNKNOWN)
  {
    DSerial.println(F("*** Warning: getDynamicModel failed ***"));
  }
  else
  {
    DSerial.print(F("The new dynamic model is: "));
    DSerial.println(newDynamicModel);
  }

  //myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_NAVCONF); //Uncomment this line to save only the NAV settings to flash and BBR
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

    DSerial.println();
  }
}
