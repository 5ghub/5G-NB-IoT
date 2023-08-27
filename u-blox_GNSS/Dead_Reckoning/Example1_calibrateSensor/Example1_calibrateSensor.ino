

#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println(F("  u-blox Example"));

  Wire.begin();

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  //myGNSS.resetIMUalignment(); // Uncomment this line to reset the IMU alignment
}

void loop()
{
  // ESF data is produced at the navigation rate, so by default we'll get fresh data once per second
  if (myGNSS.getEsfInfo()) // Poll new ESF STATUS data
  {
    Serial.print(F("Fusion Mode: "));  
    Serial.print(myGNSS.packetUBXESFSTATUS->data.fusionMode);  
    if (myGNSS.packetUBXESFSTATUS->data.fusionMode == 0)
      Serial.println(F("  Sensor is initializing..."));  
    else if (myGNSS.packetUBXESFSTATUS->data.fusionMode == 1)
      Serial.println(F("  Sensor is calibrated!"));  
    else if (myGNSS.packetUBXESFSTATUS->data.fusionMode == 2)
      Serial.println(F("  Sensor fusion is suspended!"));  
    else if (myGNSS.packetUBXESFSTATUS->data.fusionMode == 3)
      Serial.println(F("  Sensor fusion is disabled!"));  
  }

  delay(250);
}
