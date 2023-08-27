#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

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
    while (1);
  }

  DSerial.print(F("Version: "));
  byte versionHigh = myGNSS.getProtocolVersionHigh();
  DSerial.print(versionHigh);
  DSerial.print(".");
  byte versionLow = myGNSS.getProtocolVersionLow();
  DSerial.print(versionLow);
}

void loop()
{
  //Do nothing
}
