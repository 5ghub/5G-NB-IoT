/*
  This example shows how to change the I2C address of a u-blox module
*/

#include <board.h>

#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Tracks the passing of 2000ms (2 seconds)

void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();

  byte oldAddress = 0x42; //The default address for u-blox modules is 0x42
  byte newAddress = 0x3F; //Address you want to change to. Valid is 0x08 to 0x77.

  while (DSerial.available()) DSerial.read(); //Trash any incoming chars
  DSerial.print("Press a key to change address to 0x");
  DSerial.println(newAddress, HEX);
  while (DSerial.available() == false) ; //Wait for user to send character

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
  
  if (myGNSS.begin(Wire, oldAddress) == true) //Connect to the u-blox module using Wire port and the old address
  {
    DSerial.print("GNSS found at address 0x");
    DSerial.println(oldAddress, HEX);

    myGNSS.setI2CAddress(newAddress); //Change I2C address of this device
    //Device's I2C address is stored to memory and loaded on each power-on

    delay(2000); // Allow time for the change to take

    if (myGNSS.begin(Wire, newAddress) == true)
    {
      myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
      
      DSerial.print("Address successfully changed to 0x");
      DSerial.println(newAddress, HEX);
      DSerial.print("Now load another example sketch using .begin(Wire, 0x");
      DSerial.print(newAddress, HEX);
      DSerial.println(") to use this GPS module");
      DSerial.println("Freezing...");
      while (1);
    }
  }

  //Something went wrong, begin looking for the I2C device
  DSerial.println("Address change probably failed. Beginning an I2C scan.");

  Wire.begin();
}

void loop() {

  byte address;
  int nDevices;

  DSerial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0)
    {
      DSerial.print("I2C device found at address 0x");
      if (address < 16)
        DSerial.print("0");
      DSerial.print(address, HEX);
      DSerial.println("  !");

      nDevices++;
    }
//    else if (error == 4)
//    {
//      Serial.print("Unknown error at address 0x");
//      if (address < 16)
//        Serial.print("0");
//      Serial.println(address, HEX);
//    }
  }

  if (nDevices == 0)
    DSerial.println("No I2C devices found\n");
  else
    DSerial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
