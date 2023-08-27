/*
  This example configures the High Navigation Rate on the NEO-M8U and then
  polls and displays the attitude solution, vehicle dynamics information
  and high rate position, velocity and time.
  
  This example polls the high rate data.
  (The next example uses "autoHNR" to receive the HNR data automatically.)
  
  Please make sure your NEO-M8U is running UDR firmware >= 1.31. Please update using u-center if necessary:
  https://www.u-blox.com/en/product/neo-m8u-module#tab-documentation-resources

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

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("Warning! u-blox GPS did not begin correctly."));
    Serial.println(F("(This may be because the I2C port is busy with HNR messages.)"));
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  if (myGNSS.setHNRNavigationRate(10) == true) //Set the High Navigation Rate to 10Hz
    Serial.println(F("setHNRNavigationRate was successful"));
  else
    Serial.println(F("setHNRNavigationRate was NOT successful"));

  myGNSS.setAutoHNRATT(false); //Make sure auto HNR attitude messages are disabled
  myGNSS.setAutoHNRINS(false); //Make sure auto HNR vehicle dynamics messages are disabled
  myGNSS.setAutoHNRPVT(false); //Make sure auto HNR PVT messages are disabled
}

void loop()
{
  // Poll and print selected HNR data
  if (myGNSS.getHNRAtt(125) == true) // Request HNR Att data using a 125ms timeout
  {
    Serial.print(F("Roll: "));
    Serial.print(myGNSS.getHNRroll(), 2); // Use the helper function to get the roll in degrees
    Serial.print(F(" Pitch: "));
    Serial.print(myGNSS.getHNRpitch(), 2); // Use the helper function to get the pitch in degrees
    Serial.print(F(" Heading: "));
    Serial.println(myGNSS.getHNRheading(), 2); // Use the helper function to get the heading in degrees
  }
  if (myGNSS.getHNRDyn(125) == true) // Request HNR Dyn data using a 125ms timeout
  {
    Serial.print(F("xAccel: "));
    Serial.print(myGNSS.packetUBXHNRINS->data.xAccel);
    Serial.print(F(" yAccel: "));
    Serial.print(myGNSS.packetUBXHNRINS->data.yAccel);
    Serial.print(F(" zAccel: "));
    Serial.println(myGNSS.packetUBXHNRINS->data.zAccel);
  }
  if (myGNSS.getHNRPVT(125) == true) // Request HNR PVT data using a 125ms timeout
  {
    Serial.print(F("ns: "));
    Serial.print(myGNSS.packetUBXHNRPVT->data.nano);
    Serial.print(F(" Lat: "));
    Serial.print(myGNSS.packetUBXHNRPVT->data.lat);
    Serial.print(F(" Lon: "));
    Serial.println(myGNSS.packetUBXHNRPVT->data.lon);
  }
}
