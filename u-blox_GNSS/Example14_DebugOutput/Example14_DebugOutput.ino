/*
  Debug Output

  This example shows how to configure the debug output from the library.
  Debug shows various packet and status outputs. These prints can be directed
  towards Serial (as in Serial.print) or any other port (Serial1, SerialUSB, etc).

  You can also limit the debug messages to the "critical" ones by adding an extra argument.

  The debug messages can be disabled again by calling disableDebugging()
*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

unsigned long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
int counter = 0; // Disable the debug messages when counter reaches 20

void setup()
{
  // Start the console serial port
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
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR

  myGNSS.enableDebugging(); //Enable all the debug messages over Serial (default)
  
  //myGNSS.enableDebugging(SerialUSB); //Enable debug messages over Serial USB

  //myGNSS.enableDebugging(Serial, true); //Enable only the critical debug messages over Serial
  
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

    DSerial.print(F("   "));
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
    DSerial.println(myGNSS.getSecond());
    
    DSerial.println();

    counter++; // Increment counter
    if (counter == 20)
    {
      myGNSS.disableDebugging(); // Disable the debug messages when counter reaches 20
    }
  }
}
