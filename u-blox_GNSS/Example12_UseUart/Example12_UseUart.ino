/*
  This example shows how to configure the library and U-Blox for serial port use as well as
  switching the module from the default 9600 baud to 38400.

  Note: Long/lat are large numbers because they are * 10^7. To convert lat/long
  to something google maps understands simply divide the numbers by 10,000,000. We 
  do this so that we don't have to use floating point numbers.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!
*/

#include <board.h>
// #include "wiring_private.h"   // Option B only: provides pinPeripheral()

#define DSerial SerialUSB

// ===========================================================================
//  SERIAL PORT SELECTION
// ---------------------------------------------------------------------------
//  WHY THE ORIGINAL CODE BELOW WAS COMMENTED OUT:
//  This example ships from SparkFun written for an Arduino Uno (AVR), where an
//  extra serial port is emulated in software via <SoftwareSerial.h>. That
//  library is AVR-specific -- it bit-bangs the UART using cycle-accurate
//  timing and AVR interrupts -- and does NOT exist for the SAMD21 on this
//  board. No library in the Arduino Library Manager provides SoftwareSerial.h
//  for the "samd" architecture, so the original lines cannot compile here at
//  all (fatal error: SoftwareSerial.h: No such file or directory).
//
//  The SAMD21 does not need it: it has 6 SERCOM units, each of which can be a
//  REAL hardware UART. Two ways to get one -- Option A or Option B below.
// ===========================================================================

// --- OPTION A (ACTIVE) -----------------------------------------------------
//  Use Serial1, a hardware UART the board already provides. Simplest, and
//  correct for this example, which just needs "some" UART to reach the GNSS.
#define mySerial Serial1

// --- OPTION B (INACTIVE -- uncomment to use) -------------------------------
//  A custom hardware UART on a spare SERCOM. Use this ONLY if the GNSS must be
//  on specific pins rather than Serial1's. This is the same pattern already
//  proven in this repo by ArduinoSketches/examples/UART and by the sibling
//  example u-blox_GNSS/Example13_PVT/Example3_AutoPVTviaUart.
//
//  TO ENABLE:
//    1. Comment out the "#define mySerial Serial1" line above.
//    2. Uncomment the #include "wiring_private.h" line at the top of the file.
//    3. Uncomment the two lines immediately below.
//    4. Uncomment the two pinPeripheral() calls inside setup().
//
//  The SERCOM_RX_PAD_1 / UART_TX_PAD_0 values are NOT arbitrary -- they are
//  fixed by the SAMD21 pin-multiplexing table for the MOSI/SCK pins. Do not
//  change them unless you also change the pins and check the datasheet.
//  The SERCOM1_Handler() is required: without it, transmit still works but
//  receive silently returns nothing.
//  Note: while Option B is active, MOSI/SCK are used by the UART, so SPI is
//  not available on those pins.
//
// Uart mySerial(&sercom1, MOSI, SCK, SERCOM_RX_PAD_1, UART_TX_PAD_0);
// void SERCOM1_Handler() { mySerial.IrqHandler(); }

SFE_UBLOX_GNSS myGNSS;

// --- ORIGINAL UNO-ONLY CODE (DISABLED -- see explanation above) ------------
//  Kept for reference only. These cannot compile on SAMD21.
// #include <SoftwareSerial.h>
// SoftwareSerial mySerial(10, 11); // RX, TX. Pin 10 on Uno goes to TX pin on GNSS module.

long lastTime = 0; //Simple local timer. Limits amount of I2C traffic to u-blox module.

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println("  u-blox Example");

  //Assume that the U-Blox GNSS is running at 9600 baud (the default) or at 38400 baud.
  //Loop until we're in sync and then ensure it's at 38400 baud.
  do {
    Serial.println("GNSS: trying 38400 baud");
    mySerial.begin(38400);
    // --- OPTION B only: uncomment the two lines below together with the
    //     Option B block near the top of this file. They re-route the
    //     MOSI/SCK pins away from SPI and onto the SERCOM UART peripheral.
    //     They must be called AFTER mySerial.begin(), because begin()
    //     reconfigures the pins; calling them before has no effect.
    // pinPeripheral(MOSI, PIO_SERCOM);
    // pinPeripheral(SCK, PIO_SERCOM);
    if (myGNSS.begin(mySerial) == true) break;

    delay(100);
    Serial.println("GNSS: trying 9600 baud");
    mySerial.begin(9600);
    if (myGNSS.begin(mySerial) == true) {
        Serial.println("GNSS: connected at 9600 baud, switching to 38400");
        myGNSS.setSerialRate(38400);
        delay(100);
    } else {
        //myGNSS.factoryReset();
        delay(2000); //Wait a bit before trying again to limit the Serial output
    }
  } while(1);
  Serial.println("GNSS serial connected");

  myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer
    
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    byte SIV = myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    Serial.println();
  }
}
