/*
  Demonstrate get/setMeasurementRate and get/setNavigationRate

  This example shows how to slow down the measurement and navigation rates.
  This should run on any GNSS module but has only been tested on the ZED_F9P and ZOE_M8Q.

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

unsigned long lastTime = 0; //Simple local timer. Used to calc the message interval.

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

  // Uncomment the next line if you need to completely reset your module
  //myGNSS.factoryDefault(); delay(5000); // Reset everything and wait while the module restarts

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  // Begin by printing the current measurement rate and navigation rate

  uint16_t rate = myGNSS.getMeasurementRate(); //Get the measurement rate of this module
  DSerial.print("Current measurement interval (ms): ");
  DSerial.println(rate);

  rate = myGNSS.getNavigationRate(); //Get the navigation rate of this module
  DSerial.print("Current navigation ratio (cycles): ");
  DSerial.println(rate);

  // The measurement rate is the elapsed time between GNSS measurements, which defines the rate
  // e.g. 100 ms => 10 Hz, 1000 ms => 1 Hz, 10000 ms => 0.1 Hz.
  // Let's set the measurement rate (interval) to 5 seconds = 5000 milliseconds
  if (myGNSS.setMeasurementRate(5000) == false)
  {
    DSerial.println(F("Could not set the measurement rate. Freezing."));
    while (1);
  }

  // setMeasurementRate will set i2cPollingWait to a quarter of the interval
  // Let's override that so we can poll the module more frequently and avoid timeouts
  myGNSS.setI2CpollingWait(25); // Set i2cPollingWait to 25ms

  // The navigation rate is the ratio between the number of measurements and the number of navigation solutions
  // e.g. 5 means five measurements for every navigation solution. Maximum value is 127
  // Let's set the navigation rate (ratio) to 12 to produce a solution every minute
  if (myGNSS.setNavigationRate(12) == false)
  {
    DSerial.println(F("Could not set the navigation rate. Freezing."));
    while (1);
  }

  // Read and print the updated measurement rate and navigation rate

  rate = myGNSS.getMeasurementRate(); //Get the measurement rate of this module
  DSerial.print("New measurement interval (ms): ");
  DSerial.println(rate);

  rate = myGNSS.getNavigationRate(); //Get the navigation rate of this module
  DSerial.print("New navigation ratio (cycles): ");
  DSerial.println(rate);

  lastTime = millis();
}

void loop()
{
  // i2cPollingWait will prevent us from thrashing the I2C bus

  if (myGNSS.getPVT()) //Check for new Position, Velocity, Time data. getPVT returns true if new data is available.
  {    
      long latitude = myGNSS.getLatitude();
      DSerial.print(F("Lat: "));
      DSerial.print(latitude);

      long longitude = myGNSS.getLongitude();
      DSerial.print(F(" Long: "));
      DSerial.print(longitude);

      //Calculate the interval since the last message
      DSerial.print(F(" Interval: "));
      DSerial.print(((float)(millis() - lastTime)) / 1000.0, 2);
      DSerial.print(F("s"));

      DSerial.println();

      lastTime = millis(); //Update lastTime
  }
}
