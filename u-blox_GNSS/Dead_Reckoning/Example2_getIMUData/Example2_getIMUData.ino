/*

	After calibrating the module, also known as "Fusion Mode", you can get
	data directly from the IMU. This data is integrated directly into the GNSS
	output, but is provided by the module as well. 

*/

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

  if (myGNSS.getEsfInfo()){

    Serial.print(F("Fusion Mode: "));
    Serial.println(myGNSS.packetUBXESFSTATUS->data.fusionMode);  

    if (myGNSS.packetUBXESFSTATUS->data.fusionMode == 1){
      Serial.println(F("Fusion Mode is Initialized!"));  
		}
		else {
      Serial.println(F("Fusion Mode is either disabled or not initialized!"));  
			Serial.println(F("Please see the previous example for more information."));
		}
  }
}

void loop()
{
  // ESF data is produced at the navigation rate, so by default we'll get fresh data once per second
  if (myGNSS.getEsfIns()) // Poll new ESF INS data
  {
    Serial.print(F("X Ang Rate: "));
    Serial.print(myGNSS.packetUBXESFINS->data.xAngRate);  
    Serial.print(F(" Y Ang Rate: "));
    Serial.print(myGNSS.packetUBXESFINS->data.yAngRate);  
    Serial.print(F(" Z Ang Rate: "));
    Serial.print(myGNSS.packetUBXESFINS->data.zAngRate);  
    Serial.print(F(" X Accel: "));
    Serial.print(myGNSS.packetUBXESFINS->data.xAccel);  
    Serial.print(F(" Y Accel: "));
    Serial.print(myGNSS.packetUBXESFINS->data.yAccel);  
    Serial.print(F(" Z Accel: "));
    Serial.print(myGNSS.packetUBXESFINS->data.zAccel);
      
		// These values also have "validity checks" that can be provided by the
		// ublox library by reading bitfield0
    Serial.print(F(" Validity: "));
    Serial.print(myGNSS.packetUBXESFINS->data.bitfield0.bits.xAngRateValid);
    Serial.print(myGNSS.packetUBXESFINS->data.bitfield0.bits.yAngRateValid);
    Serial.print(myGNSS.packetUBXESFINS->data.bitfield0.bits.zAngRateValid);
    Serial.print(myGNSS.packetUBXESFINS->data.bitfield0.bits.xAccelValid);
    Serial.print(myGNSS.packetUBXESFINS->data.bitfield0.bits.yAccelValid);
    Serial.println(myGNSS.packetUBXESFINS->data.bitfield0.bits.zAccelValid);
  }

  delay(250);
}
