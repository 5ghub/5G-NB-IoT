/*
  Getting Unix Epoch Time and micros using u-blox commands

  This example shows how to query a u-blox module for the current time and date as Unix Epoch uint32_t type to avoid time.h dependency.
  We also turn off the NMEA output on the I2C port. This decreases the amount of I2C traffic dramatically.

  Note: this example works best on modules like the ZED_F9P. Modules like the ZOE_M8Q do not support confirmedTime.

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

  // Uncomment the next line if you need to completely reset your module
  //myGNSS.factoryDefault(); delay(5000); // Reset everything and wait while the module restarts

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.saveConfiguration();        //Optional: Save the current settings to flash and BBR

  DSerial.println(F("Compare Unix Epoch given with reference one from https://www.epochconverter.com/"));

}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer

    // getUnixEpoch marks the PVT data as stale so you will get Unix time and PVT time on alternate seconds

    uint32_t us;  //microseconds returned by getUnixEpoch()
    uint32_t epoch = myGNSS.getUnixEpoch();
    DSerial.print(F("Unix Epoch rounded: "));
    DSerial.print(epoch, DEC);    
    epoch = myGNSS.getUnixEpoch(us);
    DSerial.print(F("  Exact Unix Epoch: "));
    DSerial.print(epoch, DEC);
    DSerial.print(F("  micros: "));
    DSerial.println(us, DEC);

    DSerial.print(myGNSS.getYear());
    DSerial.print(F("-"));
    DSerial.print(myGNSS.getMonth());
    DSerial.print(F("-"));
    DSerial.print(myGNSS.getDay());
    DSerial.print(F(" "));
    DSerial.print(myGNSS.getHour());
    DSerial.print(F(":"));
    DSerial.print(myGNSS.getMinute());
    DSerial.print(F(":"));
    DSerial.print(myGNSS.getSecond());
    
    DSerial.print(F("  Time is "));
    if (myGNSS.getTimeFullyResolved() == false)
    {
      DSerial.print(F("not fully resolved but "));
    } else {
      DSerial.print(F("fully resolved and "));
    }
    if (myGNSS.getTimeValid() == false)
    {
      DSerial.print(F("not "));
    }
    DSerial.print(F("valid "));
    if (myGNSS.getConfirmedTime() == false)
    {
      DSerial.print(F("but not "));
    } else {
      DSerial.print(F("and "));
    }
    DSerial.print(F("confirmed"));

    byte SIV = myGNSS.getSIV();
    DSerial.print(F("  SIV: "));
    DSerial.println(SIV);
  }
}
