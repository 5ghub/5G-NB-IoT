/*
  Configuring the GNSS to automatically send position reports over I2C

  This example shows how to configure the U-Blox GNSS the send navigation reports automatically
  and retrieving the latest one via getPVT. This eliminates the blocking in getPVT while the GNSS
  produces a fresh navigation solution at the expense of returning a slighly old solution.

  This can be used over serial or over I2C, this example shows the I2C use. With serial the GNSS
  simply outputs the UBX_NAV_PVT packet. With I2C it queues it into its internal I2C buffer (4KB in
  size?) where it can be retrieved in the next I2C poll.

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

void setup()
{
  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
 
  Serial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(2); //Produce two solutions per second
  myGNSS.setAutoPVT(true); //Tell the GNSS to "send" each solution
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR
}

void loop()
{
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  // Start the reading only when valid LLH is available
  if (myGNSS.getPVT() && (myGNSS.getInvalidLlh() == false))
  {
    DSerial.println();
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

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

    int PDOP = myGNSS.getPDOP();
    DSerial.print(F(" PDOP: "));
    DSerial.print(PDOP);
    DSerial.print(F(" (10^-2)"));

    int nedNorthVel = myGNSS.getNedNorthVel();
    DSerial.print(F(" VelN: "));
    DSerial.print(nedNorthVel);
    DSerial.print(F(" (mm/s)"));

    int nedEastVel = myGNSS.getNedEastVel();
    DSerial.print(F(" VelE: "));
    DSerial.print(nedEastVel);
    DSerial.print(F(" (mm/s)"));

    int nedDownVel = myGNSS.getNedDownVel();
    DSerial.print(F(" VelD: "));
    DSerial.print(nedDownVel);
    DSerial.print(F(" (mm/s)"));

    int verticalAccEst = myGNSS.getVerticalAccEst();
    DSerial.print(F(" VAccEst: "));
    DSerial.print(verticalAccEst);
    DSerial.print(F(" (mm)"));

    int horizontalAccEst = myGNSS.getHorizontalAccEst();
    DSerial.print(F(" HAccEst: "));
    DSerial.print(horizontalAccEst);
    DSerial.print(F(" (mm)"));

    int speedAccEst = myGNSS.getSpeedAccEst();
    DSerial.print(F(" SpeedAccEst: "));
    DSerial.print(speedAccEst);
    DSerial.print(F(" (mm/s)"));

    int headAccEst = myGNSS.getHeadingAccEst();
    DSerial.print(F(" HeadAccEst: "));
    DSerial.print(headAccEst);
    DSerial.print(F(" (degrees * 10^-5)"));

    if (myGNSS.getHeadVehValid() == true) {
      int headVeh = myGNSS.getHeadVeh();
      DSerial.print(F(" HeadVeh: "));
      DSerial.print(headVeh);
      DSerial.print(F(" (degrees * 10^-5)"));

      int magDec = myGNSS.getMagDec();
      DSerial.print(F(" MagDec: "));
      DSerial.print(magDec);
      DSerial.print(F(" (degrees * 10^-2)"));

      int magAcc = myGNSS.getMagAcc();
      DSerial.print(F(" MagAcc: "));
      DSerial.print(magAcc);
      DSerial.print(F(" (degrees * 10^-2)"));
    }

    DSerial.println();
  } else {
    DSerial.print(".");
    delay(50);
  }
}
