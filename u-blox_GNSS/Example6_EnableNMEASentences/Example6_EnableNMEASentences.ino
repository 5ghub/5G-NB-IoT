#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

unsigned long lastGNSSsend = 0;

void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  
  DSerial.println("Example showing how to enable/disable certain NMEA sentences");

  Wire.begin();

  if (myGNSS.begin() == false)
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  //Disable or enable various NMEA sentences over the UART1 interface
  myGNSS.enableNMEAMessage(UBX_NMEA_GLL, COM_PORT_UART1); //Several of these are on by default on ublox board so let's disable them
  myGNSS.enableNMEAMessage(UBX_NMEA_GSA, COM_PORT_UART1);
  myGNSS.enableNMEAMessage(UBX_NMEA_GSV, COM_PORT_UART1);
  myGNSS.enableNMEAMessage(UBX_NMEA_RMC, COM_PORT_UART1);
  myGNSS.enableNMEAMessage(UBX_NMEA_GGA, COM_PORT_UART1); //Only leaving GGA & VTG enabled at current navigation rate
  myGNSS.enableNMEAMessage(UBX_NMEA_VTG, COM_PORT_UART1);

  //Here's the advanced configure method
  //Some of the other examples in this library enable the PVT message so let's disable it
  myGNSS.configureMessage(UBX_CLASS_NAV, UBX_NAV_PVT, COM_PORT_UART1, 0); //Message Class, ID, and port we want to configure, sendRate of 0 (disable).

  myGNSS.setUART1Output(COM_TYPE_NMEA); //Turn off UBX and RTCM sentences on the UART1 interface

  myGNSS.setSerialRate(57600); //Set UART1 to 57600bps.

  //myGNSS.saveConfiguration(); //Optional: Save these settings to NVM

  DSerial.println(F("Messages configured. NMEA now being output over the UART1 port on the u-blox module at 57600bps."));
}

void loop()
{
  if (millis() - lastGNSSsend > 200)
  {
    myGNSS.checkUblox(); //See if new data is available, but we don't want to get NMEA here. Go check UART1.
    lastGNSSsend = millis();
  }
}
