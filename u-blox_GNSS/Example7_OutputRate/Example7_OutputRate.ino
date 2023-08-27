#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

unsigned long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
unsigned long startTime = 0; //Used to calc the actual update rate.
unsigned long updateCount = 0; //Used to calc the actual update rate.

void setup()
{
  DSerial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  DSerial.println("  u-blox Example");

  Wire.begin();

  // Increase I2C clock speed to 400kHz to cope with the high navigation rate
  // (We normally recommend running the bus at 100kHz)
  Wire.setClock(400000);
  
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(5); //Set output to 5 times a second

  uint8_t rate = myGNSS.getNavigationFrequency(); //Get the update rate of this module
  DSerial.print("Current update rate: ");
  DSerial.println(rate);

  startTime = millis();
}

void loop()
{
  //Query module every 25 ms. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available. This is defined
  //by the update freq.
  if (millis() - lastTime > 25)
  {
    lastTime = millis(); //Update the timer
    
    long latitude = myGNSS.getLatitude();
    DSerial.print(F("Lat: "));
    DSerial.print(latitude);

    long longitude = myGNSS.getLongitude();
    DSerial.print(F(" Long: "));
    DSerial.print(longitude);

    updateCount++;

    //Calculate the actual update rate based on the sketch start time and the 
    //number of updates we've received.
    DSerial.print(F(" Rate: "));
    DSerial.print( updateCount / ((millis() - startTime) / 1000.0), 2);
    DSerial.print(F("Hz"));

    DSerial.println();
  }
}
