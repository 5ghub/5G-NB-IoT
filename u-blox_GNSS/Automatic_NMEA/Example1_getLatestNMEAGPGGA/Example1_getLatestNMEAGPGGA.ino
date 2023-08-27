
#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

void setup()
{

  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println(F("  u-blox Example"));

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  if (myGNSS.begin() == false)
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  //Disable or enable various NMEA sentences over the I2C interface
  myGNSS.setI2COutput(COM_TYPE_NMEA | COM_TYPE_UBX); // Turn on both UBX and NMEA sentences on I2C. (Turn off RTCM and SPARTN)
  myGNSS.disableNMEAMessage(UBX_NMEA_GLL, COM_PORT_I2C); // Several of these are on by default on ublox board so let's disable them
  myGNSS.disableNMEAMessage(UBX_NMEA_GSA, COM_PORT_I2C);
  myGNSS.disableNMEAMessage(UBX_NMEA_GSV, COM_PORT_I2C);
  myGNSS.disableNMEAMessage(UBX_NMEA_RMC, COM_PORT_I2C);
  myGNSS.disableNMEAMessage(UBX_NMEA_VTG, COM_PORT_I2C);
  myGNSS.enableNMEAMessage(UBX_NMEA_GGA, COM_PORT_I2C); // Leave only GGA enabled at current navigation rate

  // Set the Main Talker ID to "GP". The NMEA GGA messages will be GPGGA instead of GNGGA
  myGNSS.setMainTalkerID(SFE_UBLOX_MAIN_TALKER_ID_GP);
  //myGNSS.setMainTalkerID(SFE_UBLOX_MAIN_TALKER_ID_DEFAULT); // Uncomment this line to restore the default main talker ID

  myGNSS.setHighPrecisionMode(true); // Enable High Precision Mode - include extra decimal places in the GGA messages

  //myGNSS.saveConfiguration(VAL_CFG_SUBSEC_IOPORT | VAL_CFG_SUBSEC_MSGCONF); //Optional: Save only the ioPort and message settings to NVM

  DSerial.println(F("Messages configured"));

  //myGNSS.setNMEAOutputPort(Serial); // Uncomment this line to echo all NMEA data to Serial for debugging
}

void loop()
{
  // getLatestNMEAGPGGA calls checkUblox for us. We don't need to do it here

  NMEA_GGA_data_t data; // Storage for the GPGGA data
  uint8_t result = myGNSS.getLatestNMEAGPGGA(&data); // Get the latest GPGGA data (if any)
  
  if (result == 0)
  {
    DSerial.println(F("No GPGGA data available"));
  }
  else if (result == 1)
  {
    DSerial.println(F("GPGGA data is available but is stale"));
  }
  else // if (result == 2)
  {
    // Data contains .length and .nmea
    DSerial.print(F("Latest GPGGA: Length: "));
    DSerial.print(data.length);
    DSerial.print(F("\tData: "));
    DSerial.println((const char *)data.nmea); // .nmea is printable (NULL-terminated)
  }

  result = myGNSS.getLatestNMEAGNGGA(&data); // Get the latest GNGGA data (if any)
  
  if (result == 0)
  {
    DSerial.println(F("No GNGGA data available"));
  }
  else if (result == 1)
  {
    DSerial.println(F("GNGGA data is available but is stale"));
  }
  else // if (result == 2)
  {
    // Data contains .length and .nmea
    DSerial.print(F("Latest GNGGA: Length: "));
    DSerial.print(data.length);
    DSerial.print(F("\tData: "));
    DSerial.println((const char *)data.nmea); // .nmea is printable (NULL-terminated)
  }

  delay(250);
}
