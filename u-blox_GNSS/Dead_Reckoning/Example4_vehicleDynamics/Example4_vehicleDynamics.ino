/*
  
  
  getEsfAlignment (UBX-ESF-ALG) reports the status and alignment angles of the IMU within the vehicle.
  These define the rotation of the IMU frame within the vehicle (installation frame) - not the heading
  of the vehicle itself. The vehicle attitude solution is reported separately by getNAVATT (UBX-NAV-ATT).

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

  myGNSS.setESFAutoAlignment(true); //Enable Automatic IMU-mount Alignment

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
  if (myGNSS.getEsfAlignment()) // Poll new ESF ALG data
  {
    Serial.print(F("IMU-Mount Alignment: On/Off: ")); 
    Serial.print(myGNSS.packetUBXESFALG->data.flags.bits.autoMntAlgOn);
    Serial.print(F(" Status: ")); 
    Serial.print(myGNSS.packetUBXESFALG->data.flags.bits.status);
    Serial.print(F(" Roll: ")); 
    Serial.print(myGNSS.getESFroll(), 2); // Use the helper function to get the roll in degrees
    Serial.print(F(" Pitch: ")); 
    Serial.print(myGNSS.getESFpitch(), 2); // Use the helper function to get the pitch in degrees
    Serial.print(F(" Yaw: ")); 
    Serial.print(myGNSS.getESFyaw(), 2); // Use the helper function to get the yaw in degrees
    Serial.print(F(" Errors: ")); 
    Serial.print(myGNSS.packetUBXESFALG->data.error.bits.tiltAlgError);
    Serial.print(myGNSS.packetUBXESFALG->data.error.bits.yawAlgError);
    Serial.println(myGNSS.packetUBXESFALG->data.error.bits.angleError);
  }

  if (myGNSS.getNAVATT()) // Poll new NAV ATT data
  {
    Serial.print(F("Vehicle Attitude: Roll: ")); 
    Serial.print(myGNSS.getATTroll(), 2); // Use the helper function to get the roll in degrees
    Serial.print(F(" Pitch: ")); 
    Serial.print(myGNSS.getATTpitch(), 2); // Use the helper function to get the pitch in degrees
    Serial.print(F(" Heading: ")); 
    Serial.println(myGNSS.getATTheading(), 2); // Use the helper function to get the heading in degrees    
  }

  delay(250);
}
