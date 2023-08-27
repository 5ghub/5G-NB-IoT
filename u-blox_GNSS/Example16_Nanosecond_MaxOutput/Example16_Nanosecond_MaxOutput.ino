/*
  Getting time and date using u-blox commands

  This example shows how to query a u-blox module for the current time and date. We also
  turn off the NMEA output on the I2C port. This decreases the amount of I2C traffic
  dramatically.

  Note: you will need to set your Serial Monitor to 500000 Baud to see the output

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

  DSerial.println("  u-blox Example");

  Wire.begin();
  Wire.setClock(400000); // Increase I2C clock speed to 400kHz

  //myGNSS.enableDebugging(); //Uncomment this line to enable debug messages over Serial

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR

  myGNSS.setNavigationFrequency(5); //Set output to 5 times a second
  
  byte rate = myGNSS.getNavigationFrequency(); //Get the update rate of this module
  DSerial.print("Current update rate: ");
  DSerial.println(rate);
}

void loop()
{
  // Calling getPVT returns true if there actually is a fresh navigation solution available.
  if (myGNSS.getPVT())
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

    DSerial.print(" ");
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
    DSerial.print(" nanoseconds: ");
    DSerial.print(myGNSS.getNanosecond()); // Nanoseconds can be negative

    myGNSS.flushPVT();

    DSerial.println();
  }
}
