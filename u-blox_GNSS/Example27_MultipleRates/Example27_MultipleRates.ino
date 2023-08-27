/*
  Configuring the GNSS to produce multiple messages at different rates

  This example shows how to configure the U-Blox GNSS to output multiple messages at different rates:
  PVT is output every second;
  POSECEF is output every five seconds;
  VELNED is output every ten seconds.
*/
#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

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

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.setMeasurementRate(1000); //Produce a measurement every 1000ms
  myGNSS.setNavigationRate(1); //Produce a navigation solution every measurement
  
  myGNSS.setAutoPVTrate(1); //Tell the GNSS to send the PVT solution every measurement
  myGNSS.setAutoNAVPOSECEFrate(5); //Tell the GNSS to send each POSECEF solution every 5th measurement
  myGNSS.setAutoNAVVELNEDrate(10); //Tell the GNSS to send each VELNED solution every 10th measurement
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR
}

void loop()
{
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  if (myGNSS.getPVT())
  {
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
    DSerial.println(F(" (mm)"));
  }

  // Calling getNAVPOSECEF returns true if there actually is a fresh position solution available.
  if (myGNSS.getNAVPOSECEF())
  {
    DSerial.print(F("ecefX: "));
    DSerial.print((float)myGNSS.packetUBXNAVPOSECEF->data.ecefX / 100.0, 2); // convert ecefX to m

    DSerial.print(F(" ecefY: "));
    DSerial.print((float)myGNSS.packetUBXNAVPOSECEF->data.ecefY / 100.0, 2); // convert ecefY to m

    DSerial.print(F(" ecefZ: "));
    DSerial.print((float)myGNSS.packetUBXNAVPOSECEF->data.ecefZ / 100.0, 2); // convert ecefY to m
    DSerial.println(F(" (m)"));

    myGNSS.flushNAVPOSECEF(); //Mark all the data as read/stale so we get fresh data next time
  }

  // Calling getNAVVELNED returns true if there actually is fresh velocity data available.
  if (myGNSS.getNAVVELNED())
  {
    DSerial.print(F("velN: "));
    DSerial.print((float)myGNSS.packetUBXNAVVELNED->data.velN / 100.0, 2); // convert velN to m/s

    DSerial.print(F(" velE: "));
    DSerial.print((float)myGNSS.packetUBXNAVVELNED->data.velE / 100.0, 2); // convert velE to m/s

    DSerial.print(F(" velD: "));
    DSerial.print((float)myGNSS.packetUBXNAVVELNED->data.velD / 100.0, 2); // convert velD to m/s
    DSerial.println(F(" (m/s)"));

    myGNSS.flushNAVVELNED(); //Mark all the data as read/stale so we get fresh data next time
  }
}
