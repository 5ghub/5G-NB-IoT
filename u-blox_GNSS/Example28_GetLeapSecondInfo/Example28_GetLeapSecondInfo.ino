/*
  Getting leap second event info as SNTP Leap Indicator, time to a leap second event and the number of leap seconds since GPS epoch

  This example shows how to query a u-blox module for the leap second event info to cast to SNTP Leap Indicator enumeration.
  We also turn off the NMEA output on the I2C port. This decreases the amount of I2C traffic dramatically.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

typedef enum { 
  LI_NO_WARNING,      //Time leaping not scheduled
  LI_LAST_MINUTE_61_SEC,  //Last minute has 61 seconds
  LI_LAST_MINUTE_59_SEC,  //Last minute has 59 seconds
  LI_ALARM_CONDITION    //The NTP server's clock not synchronized
} ntp_LI_e;


long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
  
void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();
  
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  // Uncomment the next line if you need to completely reset your module
  //myGNSS.factoryDefault(); delay(5000); // Reset everything and wait while the module restarts

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfiguration();        //Optional: Save the current settings to flash and BBR

  DSerial.println(F("Compare Unix Epoch given with reference one from https://www.epochconverter.com/"));
  
}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer

    // getUnixEpoch marks the PVT data as stale so you will get Unix time and PVT time on alternate seconds

    uint32_t us;  //microseconds returned by getUnixEpoch()
    uint32_t epoch = myGNSS.getUnixEpoch();
    DSerial.print(F("Unix Epoch rounded: "));
    DSerial.print(epoch, DEC);    
    epoch = myGNSS.getUnixEpoch(us);
    DSerial.print(F("  Exact Unix Epoch: "));
    DSerial.print(epoch, DEC);
    DSerial.print(F("  micros: "));
    DSerial.println(us, DEC);
    int32_t timeToLeapSecEvent;
    ntp_LI_e leapIndicator = (ntp_LI_e)myGNSS.getLeapIndicator(timeToLeapSecEvent);
    DSerial.print(F("NTP LI: "));
    DSerial.print(leapIndicator, DEC);
    switch (leapIndicator){
      case LI_NO_WARNING:
        DSerial.print(F(" - No event scheduled"));
        break;
      case LI_LAST_MINUTE_61_SEC:
        DSerial.print(F(" - last minute will end at 23:60"));
        break;
      case LI_LAST_MINUTE_59_SEC:
        DSerial.print(F(" - last minute will end at 23:58"));
        break; 
      case LI_ALARM_CONDITION:
      default:
        DSerial.print(F(" - Unknown (clock not synchronized)"));
        break; 
    }
    if (timeToLeapSecEvent < 0)
    {
      DSerial.print(F(". Time since the last leap second event: "));
      DSerial.println(timeToLeapSecEvent * -1, DEC);
    }
    else
    {
      DSerial.print(F(". Time to the next leap second event: "));
      DSerial.println(timeToLeapSecEvent, DEC);
    }

    sfe_ublox_ls_src_e leapSecSource;
    DSerial.print(F("Leap seconds since GPS Epoch (Jan 6th, 1980): "));
    DSerial.print(myGNSS.getCurrentLeapSeconds(leapSecSource), DEC);
    switch (leapSecSource){
      case SFE_UBLOX_LS_SRC_DEFAULT:
        DSerial.print(F(" - hardcoded"));
        break;
      case SFE_UBLOX_LS_SRC_GLONASS:
        DSerial.print(F(" - derived from GPS and GLONASS time difference"));
        break;
      case SFE_UBLOX_LS_SRC_GPS:
        DSerial.print(F(" - according to GPS"));
        break; 
      case SFE_UBLOX_LS_SRC_SBAS:
        DSerial.print(F(" - according to SBAS"));
        break;
      case SFE_UBLOX_LS_SRC_BEIDOU:
        DSerial.print(F(" - according to BeiDou"));
        break;
      case SFE_UBLOX_LS_SRC_GALILEO:
        DSerial.print(F(" - according to Galileo"));
        break;
      case SFE_UBLOX_LS_SRC_AIDED:
        DSerial.print(F(" - last minute will end at 23:58"));
        break; 
      case SFE_UBLOX_LS_SRC_CONFIGURED:
        DSerial.print(F(" - as configured)"));
        break;
      case SFE_UBLOX_LS_SRC_UNKNOWN:
      default:
        DSerial.print(F(" - source unknown"));
        break;
    }
    DSerial.println();
  }
  DSerial.println();
}
