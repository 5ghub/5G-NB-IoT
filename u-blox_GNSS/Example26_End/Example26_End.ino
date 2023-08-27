/*
  Demonstrating how to use "end"
  This example shows how to use the end function.
  End will stop all automatic message processing and free (nearly) all used RAM.
  The file buffer is deleted (if it exists).
  
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

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
  
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR

  myGNSS.end(); // Call end now just because we can - it won't do much as we haven't used any automatic messages
}

void loop()
{
  // Allocate 128 bytes for file storage - this checks that issue #20 has been resolved
  // Allocating only 128 bytes will let this code run on the ATmega328P
  // If your processor has plenty of RAM, you can increase this to something useful like 16KB
  myGNSS.setFileBufferSize(128);
  
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected. Freezing."));
    while (1);
  }

  DSerial.print(F("The file buffer size is: "));
  DSerial.println(myGNSS.getFileBufferSize());    

  // Request Position, Velocity, Time
  // RAM will be allocated for PVT message processing
  // getPVT will return true is fresh PVT data was received and processed
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

  // Calling end will free the RAM allocated for file storage and PVT processing
  // Calling end is optional. You can comment the next line if you want to.
  myGNSS.end();
}
