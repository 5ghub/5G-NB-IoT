/*
  Get the jamming state and indication

  This example shows how to enable the jamming / interference monitor and read the
  jamming state and information.

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

void setup()
{

  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);  Serial.println(F("  u-blox GNSS Example"));

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  if (myGNSS.begin() == false)
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  // Enable the jamming / interference monitor
  UBX_CFG_ITFM_data_t jammingConfig; // Create storage for the jamming configuration
  if (myGNSS.getJammingConfiguration(&jammingConfig)) // Read the jamming configuration
  {
    DSerial.print(F("The jamming / interference monitor is "));
    if (jammingConfig.config.bits.enable == 0) // Check if the monitor is already enabled
      DSerial.print(F("not "));
    DSerial.println(F("enabled"));

    if (jammingConfig.config.bits.enable == 0) // Check if the monitor is already enabled
    {
      DSerial.print(F("Enabling the jamming / interference monitor: "));
      (jammingConfig.config.bits.enable = 1); // Enable the monitor
      if (myGNSS.setJammingConfiguration(&jammingConfig)) // Set the jamming configuration
        DSerial.println(F("success"));
      else
        DSerial.println(F("failed!"));
    }
  }

}

void loop()
{
  // Create storage to hold the hardware status
  // See the definition of UBX_MON_HW_data_t in u-blox_structs.h for more details
  UBX_MON_HW_data_t hwStatus;

  if (myGNSS.getHWstatus(&hwStatus)) // Read the hardware status
  {
    DSerial.println(F("Hardware status (UBX_MON_HW):"));

    DSerial.print(F("Jamming state: "));
    DSerial.print(hwStatus.flags.bits.jammingState);
    if (hwStatus.flags.bits.jammingState == 0)
      DSerial.println(F(" = unknown / disabled"));
    else if (hwStatus.flags.bits.jammingState == 1)
      DSerial.println(F(" = ok"));
    else if (hwStatus.flags.bits.jammingState == 2)
      DSerial.println(F(" = warning"));
    else // if (hwStatus.flags.bits.jammingState == 3)
      DSerial.println(F(" = critical!"));

    DSerial.print(F("Noise level: "));
    DSerial.println(hwStatus.noisePerMS);
    
    DSerial.print(F("AGC monitor: "));
    DSerial.println(hwStatus.agcCnt);
    
    DSerial.print(F("CW jamming indicator: "));
    DSerial.println(hwStatus.jamInd);

    DSerial.println();
  }

  // Create storage to hold the RF information from a ZED-F9
  // See the definition of UBX_MON_RF_data_t in u-blox_structs.h for more details
  UBX_MON_RF_data_t rfInformation;

  // Read the RF information from the ZED-F9n. Allow 2 seconds for the data to be returned. Will time out on M8 modules
  if (myGNSS.getRFinformation(&rfInformation, 2000))
  {
    DSerial.print(F("The UBX_MON_RF message contains "));
    DSerial.print(rfInformation.header.nBlocks); // Print how many information blocks were returned. Should be 0, 1 or 2
    DSerial.println(F(" information blocks"));

    for (uint8_t block = 0; block < rfInformation.header.nBlocks; block++)
    {
      DSerial.print(F("Block ID: "));
      DSerial.print(rfInformation.blocks[block].blockId);
      if (rfInformation.blocks[block].blockId == 0)
        DSerial.println(F(" = L1"));
      else // if (rfInformation.blocks[block].blockId == 1)
        DSerial.println(F(" = L2 / L5"));
        
      DSerial.print(F("Jamming state: "));
      DSerial.print(rfInformation.blocks[block].flags.bits.jammingState);
      if (rfInformation.blocks[block].flags.bits.jammingState == 0)
        DSerial.println(F(" = unknown / disabled"));
      else if (rfInformation.blocks[block].flags.bits.jammingState == 1)
        DSerial.println(F(" = ok"));
      else if (rfInformation.blocks[block].flags.bits.jammingState == 2)
        DSerial.println(F(" = warning"));
      else // if (rfInformation.blocks[block].flags.bits.jammingState == 3)
        DSerial.println(F(" = critical!"));

      DSerial.print(F("Noise level: "));
      DSerial.println(rfInformation.blocks[block].noisePerMS);
      
      DSerial.print(F("AGC monitor: "));
      DSerial.println(rfInformation.blocks[block].agcCnt);
      
      DSerial.print(F("CW jamming indicator: "));
      DSerial.println(rfInformation.blocks[block].jamInd);      
    }

    DSerial.println();
  }
}
