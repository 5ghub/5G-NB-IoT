/*
	After calibrating the module and securing it to your vehicle such that it's
  stable within 2 degrees, and the board is oriented correctly with regards to
  the vehicle's frame, you can now read the vehicle's "attitude". The attitude
  includes the vehicle's heading, pitch, and roll.

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
}

void loop()
{
  // PVAT data is produced at the navigation rate, so by default we'll get fresh data once per second
	if (myGNSS.getNAVPVAT()) // Poll new PVAT
  {
    Serial.print(F("Roll: ")); 
    Serial.print((float)myGNSS.getVehicleRoll() / 100000.0, 5); // Use the helper function to get the roll in degrees * 10^-5
    
  	Serial.print(F(" Pitch: ")); 
    Serial.print((float)myGNSS.getVehiclePitch() / 100000.0, 5); // Use the helper function to get the pitch in degrees * 10^-5
    
  	Serial.print(F(" Heading: ")); 
    Serial.print((float)myGNSS.getVehicleHeading() / 100000.0, 5); // Use the helper function to get the heading in degrees * 10^-5

    // We don't have helper functions to extract the roll, pitch and heading valid flags from the PVAT message. But we can do it manually:
    
    Serial.print(F(" Roll Valid: "));
    Serial.print(myGNSS.packetUBXNAVPVAT->data.flags.bits.vehRollValid);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.vehRollValid = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.print(F(" Pitch Valid: "));
    Serial.print(myGNSS.packetUBXNAVPVAT->data.flags.bits.vehPitchValid);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.vehPitchValid = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.print(F(" Heading Valid: "));
    Serial.print(myGNSS.packetUBXNAVPVAT->data.flags.bits.vehHeadingValid);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.vehHeadingValid = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    // We don't have helper functions to extract the roll, pitch and heading accuracy from the PVAT message. But we can do it manually:
    
    Serial.print(F(" Roll Acc: "));
    Serial.print(((float)myGNSS.packetUBXNAVPVAT->data.accRoll) / 100, 2);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried2.bits.accRoll = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.print(F(" Pitch Acc: "));
    Serial.print(((float)myGNSS.packetUBXNAVPVAT->data.accPitch) / 100, 2);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried2.bits.accPitch = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.print(F(" Heading Acc: "));
    Serial.print(((float)myGNSS.packetUBXNAVPVAT->data.accHeading) / 100, 2);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried2.bits.accHeading = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    // We don't have helper functions to extract the lat and lon from the PVAT message. But we can do it manually:
    
    Serial.print(F(" Lat: ")); 
    Serial.print(((float)myGNSS.packetUBXNAVPVAT->data.lat) / 10000000.0, 7);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.lat = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.print(F(" Lon: ")); 
    Serial.print(((float)myGNSS.packetUBXNAVPVAT->data.lon) / 10000000.0, 7);
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.lon = false; // Mark the data as stale
    myGNSS.packetUBXNAVPVAT->moduleQueried.moduleQueried1.bits.all = false;
    
    Serial.println();
  }

  delay(250);
}
