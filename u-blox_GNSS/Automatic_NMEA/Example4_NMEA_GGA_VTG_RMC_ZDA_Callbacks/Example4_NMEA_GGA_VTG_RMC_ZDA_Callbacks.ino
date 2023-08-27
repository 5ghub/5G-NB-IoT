#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

// Callback: printGPGGA will be called when new GPGGA NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_GGA_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGPGGAcallback
//        /               _____  This _must_ be NMEA_GGA_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGPGGA(NMEA_GGA_data_t *nmeaData)
{
    DSerial.print(F("\r\nGPGGA: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGNGGA will be called if new GNGGA NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_GGA_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGNGGAcallback
//        /               _____  This _must_ be NMEA_GGA_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGNGGA(NMEA_GGA_data_t *nmeaData)
{
    DSerial.print(F("\r\nGNGGA: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGPVTG will be called when new GPVTG NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_VTG_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGPVTGcallback
//        /               _____  This _must_ be NMEA_VTG_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGPVTG(NMEA_VTG_data_t *nmeaData)
{
    DSerial.print(F("\r\nGPVTG: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGNVTG will be called if new GNVTG NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_VTG_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGNVTGcallback
//        /               _____  This _must_ be NMEA_VTG_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGNVTG(NMEA_VTG_data_t *nmeaData)
{
    DSerial.print(F("\r\nGNVTG: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGPRMC will be called when new GPRMC NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_RMC_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGPRMCcallback
//        /               _____  This _must_ be NMEA_RMC_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGPRMC(NMEA_RMC_data_t *nmeaData)
{
    DSerial.print(F("\r\nGPRMC: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGNRMC will be called if new GNRMC NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_RMC_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGNRMCcallback
//        /               _____  This _must_ be NMEA_RMC_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGNRMC(NMEA_RMC_data_t *nmeaData)
{
    DSerial.print(F("\r\nGNRMC: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGPZDA will be called when new GPZDA NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_ZDA_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGPZDAcallback
//        /               _____  This _must_ be NMEA_ZDA_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGPZDA(NMEA_ZDA_data_t *nmeaData)
{
    DSerial.print(F("\r\nGPZDA: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

// Callback: printGNZDA will be called if new GNZDA NMEA data arrives
// See u-blox_structs.h for the full definition of NMEA_ZDA_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setNMEAGNZDAcallback
//        /               _____  This _must_ be NMEA_ZDA_data_t
//        |              /           _____ You can use any name you like for the struct
//        |              |          /
//        |              |          |
void printGNZDA(NMEA_ZDA_data_t *nmeaData)
{
    DSerial.print(F("\r\nGNZDA: Length: "));
    DSerial.print(nmeaData->length);
    DSerial.print(F("\tData: "));
    DSerial.print((const char *)nmeaData->nmea); // .nmea is printable (NULL-terminated) and already has \r\n on the end
}

void setup()
{

  DSerial.begin(115200);
  DSerial.println(F("  u-blox GNSS Example"));

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  if (myGNSS.begin() == false)
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  // Disable or enable various NMEA sentences over the I2C interface
  myGNSS.setI2COutput(COM_TYPE_NMEA | COM_TYPE_UBX); // Turn on both UBX and NMEA sentences on I2C. (Turn off RTCM and SPARTN)
  myGNSS.disableNMEAMessage(UBX_NMEA_GLL, COM_PORT_I2C);
  myGNSS.disableNMEAMessage(UBX_NMEA_GSA, COM_PORT_I2C);
  myGNSS.disableNMEAMessage(UBX_NMEA_GSV, COM_PORT_I2C);
  myGNSS.enableNMEAMessage(UBX_NMEA_RMC, COM_PORT_I2C);
  myGNSS.enableNMEAMessage(UBX_NMEA_VTG, COM_PORT_I2C);
  myGNSS.enableNMEAMessage(UBX_NMEA_GGA, COM_PORT_I2C);
  myGNSS.enableNMEAMessage(UBX_NMEA_ZDA, COM_PORT_I2C);

  // Set the Main Talker ID to "GP". The NMEA GGA messages will be GPGGA instead of GNGGA
  myGNSS.setMainTalkerID(SFE_UBLOX_MAIN_TALKER_ID_GP);
  //myGNSS.setMainTalkerID(SFE_UBLOX_MAIN_TALKER_ID_DEFAULT); // Uncomment this line to restore the default main talker ID

  myGNSS.setHighPrecisionMode(true); // Enable High Precision Mode - include extra decimal places in the GGA messages

  //myGNSS.saveConfiguration(VAL_CFG_SUBSEC_IOPORT | VAL_CFG_SUBSEC_MSGCONF); //Optional: Save only the ioPort and message settings to NVM

  DSerial.println(F("Messages configured"));

  //myGNSS.setNMEAOutputPort(Serial); // Uncomment this line to echo all NMEA data to Serial for debugging

  // Set up the callback for GPGGA
  myGNSS.setNMEAGPGGAcallbackPtr(&printGPGGA);

  // Set up the callback for GNGGA
  myGNSS.setNMEAGNGGAcallbackPtr(&printGNGGA);

  // Set up the callback for GPVTG
  myGNSS.setNMEAGPVTGcallbackPtr(&printGPVTG);

  // Set up the callback for GNVTG
  myGNSS.setNMEAGNVTGcallbackPtr(&printGNVTG);

  // Set up the callback for GPRMC
  myGNSS.setNMEAGPRMCcallbackPtr(&printGPRMC);

  // Set up the callback for GNRMC
  myGNSS.setNMEAGNRMCcallbackPtr(&printGNRMC);

  // Set up the callback for GPZDA
  myGNSS.setNMEAGPZDAcallbackPtr(&printGPZDA);

  // Set up the callback for GNZDA
  myGNSS.setNMEAGNZDAcallbackPtr(&printGNZDA);
}

void loop()
{
  myGNSS.checkUblox(); // Check for the arrival of new data and process it.
  myGNSS.checkCallbacks(); // Check if any callbacks are waiting to be processed.

  DSerial.print(".");
  delay(50);
}
