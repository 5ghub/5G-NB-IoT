/*
  NEO-D9S L-Band receiver example
  This example shows how to display the NEO-D9S's received signal imbalance and magnitude, plus a summary of any received PMP data.

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myLBand; // NEO-D9S

const uint32_t myLBandFreq = 1556290000; // Uncomment this line to use the US SPARTN 1.8 service
//const uint32_t myLBandFreq = 1545260000; // Uncomment this line to use the EU SPARTN 1.8 service

#define OK(ok) (ok ? F("  ->  OK") : F("  ->  ERROR!")) // Convert uint8_t into OK/ERROR

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Callback: printRXMPMP will be called when new PMP data arrives
// See u-blox_structs.h for the full definition of UBX_RXM_PMP_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setRXMPMPcallbackPtr
//        /               _____  This _must_ be UBX_RXM_PMP_data_t
//        |              /              _____ You can use any name you like for the struct
//        |              |             /
//        |              |             |
void printRXMPMP(UBX_RXM_PMP_data_t *pmpData)
{
  DSerial.println(F("New PMP data received:"));

  DSerial.print(F("PMP message version: "));
  DSerial.println(pmpData->version);
  
  DSerial.print(F("numBytesUserData :   "));
  DSerial.println(pmpData->numBytesUserData);
  
  DSerial.print(F("serviceIdentifier:   0x"));
  DSerial.println(pmpData->serviceIdentifier, HEX);
  
  DSerial.print(F("uniqueWordBitErrors: "));
  DSerial.println(pmpData->uniqueWordBitErrors);
  
  DSerial.print(F("fecBits:             "));
  DSerial.println(pmpData->fecBits);
  
  DSerial.print(F("ebno (dB):           "));
  DSerial.println((double)pmpData->ebno / 8, 3); //Convert ebno to dB

  DSerial.println();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");
  DSerial.println(F("NEO-D9S Example"));

  Wire.begin(); //Start I2C

  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Begin and configure the NEO-D9S L-Band receiver

  //myLBand.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  while (myLBand.begin(Wire, 0x43) == false) //Connect to the u-blox NEO-D9S using Wire port. The D9S default I2C address is 0x43 (not 0x42)
  {
    DSerial.println(F("u-blox NEO-D9S not detected at default I2C address. Please check wiring."));
    delay(2000);
  }
  DSerial.println(F("u-blox NEO-D9S connected"));

  uint8_t ok = myLBand.setVal32(UBLOX_CFG_PMP_CENTER_FREQUENCY,   myLBandFreq); // Default 1539812500 Hz
  if (ok) ok = myLBand.setVal16(UBLOX_CFG_PMP_SEARCH_WINDOW,      2200);        // Default 2200 Hz
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_PMP_USE_SERVICE_ID,      0);           // Default 1 
  if (ok) ok = myLBand.setVal16(UBLOX_CFG_PMP_SERVICE_ID,         21845);       // Default 50821
  if (ok) ok = myLBand.setVal16(UBLOX_CFG_PMP_DATA_RATE,          2400);        // Default 2400 bps
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_PMP_USE_DESCRAMBLER,     1);           // Default 1
  if (ok) ok = myLBand.setVal16(UBLOX_CFG_PMP_DESCRAMBLER_INIT,   26969);       // Default 23560
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_PMP_USE_PRESCRAMBLING,   0);           // Default 0
  if (ok) ok = myLBand.setVal64(UBLOX_CFG_PMP_UNIQUE_WORD,        16238547128276412563ull); 
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_MSGOUT_UBX_RXM_PMP_I2C,   1); // Ensure UBX-RXM-PMP is enabled on the I2C port 
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_MSGOUT_UBX_RXM_PMP_UART1, 1); // Output UBX-RXM-PMP on UART1
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_UART2OUTPROT_UBX, 1);         // Enable UBX output on UART2
  if (ok) ok = myLBand.setVal8(UBLOX_CFG_MSGOUT_UBX_RXM_PMP_UART2, 1); // Output UBX-RXM-PMP on UART2
  if (ok) ok = myLBand.setVal32(UBLOX_CFG_UART1_BAUDRATE,         38400); // match baudrate with ZED default
  if (ok) ok = myLBand.setVal32(UBLOX_CFG_UART2_BAUDRATE,         38400); // match baudrate with ZED default

  DSerial.print(F("L-Band: configuration "));
  DSerial.println(OK(ok));

  myLBand.softwareResetGNSSOnly(); // Do a restart

  myLBand.setRXMPMPcallbackPtr(&printRXMPMP); // Call printRXMPMP when new PMP data arrives
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void loop()
{
  myLBand.checkUblox(); // Check for the arrival of new PMP data and process it.
  myLBand.checkCallbacks(); // Check if any LBand callbacks are waiting to be processed.

  UBX_MON_HW2_data_t hwStatus; // Create storage for the HW2 extended hardware status
  if (myLBand.getHW2status(&hwStatus)) // Request the extended hardware status
  {
    // Print the signal imbalance and magnitude
    DSerial.print(F("Signal imbalance and magnitude:  ofsI: "));
    DSerial.print(hwStatus.ofsI);
    DSerial.print(F("  magI: "));
    DSerial.print(hwStatus.magI);
    DSerial.print(F("  ofsQ: "));
    DSerial.print(hwStatus.ofsQ);
    DSerial.print(F("  magQ: "));
    DSerial.println(hwStatus.magQ);
  }
}
