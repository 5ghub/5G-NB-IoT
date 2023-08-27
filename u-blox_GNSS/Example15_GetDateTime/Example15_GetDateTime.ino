

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

void setup()
{
  DSerial.begin(115200);
  while (!Serial)
    ; //Wait for user to open terminal
  DSerial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.saveConfiguration();        //Optional: Save the current settings to flash and BBR
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

    byte SIV = myGNSS.getSIV();
    DSerial.print(F(" SIV: "));
    DSerial.print(SIV);

    DSerial.println();
    DSerial.print(myGNSS.getYear());
    DSerial.print("-");
    DSerial.print(myGNSS.getMonth());
    DSerial.print("-");
    DSerial.print(myGNSS.getDay());
    DSerial.print(" ");
    DSerial.print(myGNSS.getHour());
    DSerial.print(":");
    DSerial.print(myGNSS.getMinute());
    DSerial.print(":");
    DSerial.print(myGNSS.getSecond());

    DSerial.print("  Time is ");
    if (myGNSS.getTimeValid() == false)
    {
      DSerial.print("not ");
    }
    DSerial.print("valid  Date is ");
    if (myGNSS.getDateValid() == false)
    {
      DSerial.print("not ");
    }
    DSerial.print("valid");

    DSerial.println();
  }
}
