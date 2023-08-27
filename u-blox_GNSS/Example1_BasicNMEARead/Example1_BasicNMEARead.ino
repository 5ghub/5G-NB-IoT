/*
  Read NMEA sentences over I2C using u-blox module SAM-M8Q, NEO-M8P, ZED-F9P, etc

  This example reads the NMEA setences from the u-blox module over I2c and outputs
  them to the serial port

*/

#include <Wire.h> //Needed for I2C to GNSS

SFE_UBLOX_GNSS myGNSS;

void setup()
{
  Serial.begin(115200);
  Serial.println("  u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false)
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX | COM_TYPE_NMEA); //Set the I2C port to output both NMEA and UBX messages
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  //This will pipe all NMEA sentences to the serial port so we can see them
  myGNSS.setNMEAOutputPort(Serial);
}

void loop()
{
  myGNSS.checkUblox(); //See if new data is available. Process bytes as they come in.

  delay(250); //Don't pound too hard on the I2C bus
}
