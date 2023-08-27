/*
  Power Save Mode
  This example shows how to put the u-blox module into power save mode and then
  query its lat/long/altitude. We also turn off the NMEA output on the I2C port.
  This decreases the amount of I2C traffic dramatically.

  ** When it is able to ** the module will reduce its current draw.
  For the ZOE-M8Q with a passive antenna, you should see the current drop
  from (approx.) 25-28mA to (approx.) 9mA when power save mode kicks in.

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
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.saveConfiguration(); //Optional: Uncomment this line to save the current settings to flash and BBR

  DSerial.println("Power save example.");
  DSerial.println("1) Enable power saving");
  DSerial.println("2) Disable power saving");
}

void loop()
{
  if (DSerial.available())
  {
    byte incoming = DSerial.read();

    if (incoming == '1')
    {
      // Put the GNSS into power save mode
      // (If you want to disable power save mode, call myGNSS.powerSaveMode(false) instead)
      // This will fail on the ZED (protocol version >= 27) as UBX-CFG-RXM is not supported
      if (myGNSS.powerSaveMode()) // Defaults to true
        DSerial.println(F("Power Save Mode enabled."));
      else
        DSerial.println(F("*** Power Save Mode FAILED ***"));
    }
    else if (incoming == '2')
    {
      //Go to normal power mode (not power saving mode)
      if (myGNSS.powerSaveMode(false))
        DSerial.println(F("Power Save Mode disabled."));
      else
        DSerial.println(F("*** Power Save Disable FAILED ***"));
    }

    // Read and print the new low power mode
    uint8_t lowPowerMode = myGNSS.getPowerSaveMode();
    if (lowPowerMode == 255)
    {
      DSerial.println(F("*** getPowerSaveMode FAILED ***"));
    }
    else
    {
      DSerial.print(F("The low power mode is: "));
      DSerial.print(lowPowerMode);
      if (lowPowerMode == 0)
      {
        DSerial.println(F(" (Continuous)"));
      }
      else if (lowPowerMode == 1)
      {
        DSerial.println(F(" (Power Save)"));
      }
      else if (lowPowerMode == 4)
      {
        DSerial.println(F(" (Continuous)"));
      }
      else
      {
        DSerial.println(F(" (Unknown!)"));
      }
    }
  }

  //Query module every 10 seconds so it is easier to monitor the current draw
  if (millis() - lastTime > 10000)
  {
    lastTime = millis(); //Update the timer

    byte fixType = myGNSS.getFixType(); // Get the fix type
    DSerial.print(F("Fix: "));
    DSerial.print(fixType);
    if (fixType == 0)
      DSerial.print(F("(No fix)"));
    else if (fixType == 1)
      DSerial.print(F("(Dead reckoning)"));
    else if (fixType == 2)
      DSerial.print(F("(2D)"));
    else if (fixType == 3)
      DSerial.print(F("(3D)"));
    else if (fixType == 4)
      DSerial.print(F("(GNSS + Dead reckoning)"));

    long latitude = myGNSS.getLatitude();
    DSerial.print(F(" Lat: "));
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
