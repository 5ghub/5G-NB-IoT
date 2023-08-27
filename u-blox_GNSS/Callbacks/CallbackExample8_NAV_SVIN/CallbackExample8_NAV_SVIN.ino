/*
  Configuring the GNSS to automatically send NAV SVIN reports over I2C and display them using a callback

  This example shows how to configure the u-blox GNSS to send NAV SVIN reports automatically
  and access the data via a callback. No more polling!
*/

#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

// Callback: newNAVSVIN will be called when new NAV SVIN data arrives
// See u-blox_structs.h for the full definition of UBX_NAV_SVIN_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setAutoNAVSVINcallbackPtr
//        /             _____  This _must_ be UBX_NAV_SVIN_data_t
//        |            /                _____ You can use any name you like for the struct
//        |            |               /
//        |            |               |
void newNAVSVIN(UBX_NAV_SVIN_data_t *ubxDataStruct)
{
  Serial.println();

  Serial.print(F("Survey-in is "));
  if (ubxDataStruct->active == 0)
    Serial.print(F("not "));
  Serial.println(F("in progress"));

  Serial.print(F("Survey-in position is "));
  if (ubxDataStruct->valid == 0)
    Serial.print(F("not "));
  Serial.println(F("valid"));

  Serial.print(F("Survey-in observation time (s): "));
  Serial.println(ubxDataStruct->dur);

  Serial.print(F("ECEF position (cm): "));
  Serial.print(ubxDataStruct->meanX);
  Serial.print(F(" ("));
  if (ubxDataStruct->meanXHP >= 0)
    Serial.print(F("+"));
  Serial.print((float)ubxDataStruct->meanXHP * 0.01); // Convert 0.1mm to cm
  Serial.print(F("), "));
  Serial.print(ubxDataStruct->meanY);
  Serial.print(F(" ("));
  if (ubxDataStruct->meanYHP >= 0)
    Serial.print(F("+"));
  Serial.print((float)ubxDataStruct->meanYHP * 0.01); // Convert 0.1mm to cm
  Serial.print(F("), "));
  Serial.print(ubxDataStruct->meanZ);
  Serial.print(F(" ("));
  if (ubxDataStruct->meanZHP >= 0)
    Serial.print(F("+"));
  Serial.print((float)ubxDataStruct->meanZHP * 0.01); // Convert 0.1mm to cm
  Serial.println(F(")"));  

  Serial.print(F("Mean position accuracy (cm): "));
  Serial.println((float)ubxDataStruct->meanAcc * 0.01); // Convert 0.1mm to cm
}

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println(F("u-blox Base Station example"));

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial
  
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  // Uncomment the next line if you want to reset your module back to the default settings with 1Hz navigation rate
  //myGNSS.factoryDefault(); delay(5000);

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save the communications port settings to flash and BBR

  // Set up the callback for NAV SVIN. This will enable SVIN messages at the navigation rate
  myGNSS.setAutoNAVSVINcallbackPtr(&newNAVSVIN);

  while (Serial.available()) Serial.read(); //Clear the serial buffer
  Serial.println(F("Press any key to begin Survey-In"));
}

void loop()
{
  myGNSS.checkUblox(); //See if new data is available. Process bytes as they come in.
  myGNSS.checkCallbacks(); //Process any waiting callbacks

  if (Serial.available()) // Check if user has pressed a key
  {
    bool success = myGNSS.enableSurveyMode(60, 5.000); //Enable Survey in, 60 seconds, 5.0m
    //bool success = myGNSS.enableSurveyModeFull(86400, 2.000); //Enable Survey in, 24 hours, 2.0m

    Serial.println();

    if (success)
    {
      Serial.println(F("Survey-In started!"));    
    }
    else
    {
      Serial.println(F("Survey start failed!"));
    }
    
    while (Serial.available()) Serial.read(); //Clear the serial buffer
  }
}
