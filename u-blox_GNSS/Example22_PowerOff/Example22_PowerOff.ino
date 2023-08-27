/*
  This example shows you how to turn off the ublox module to lower the power consumption.
  There are two functions: one just specifies a duration in milliseconds the other also specifies a pin on the GNSS device to wake it up with.
  By driving a voltage from LOW to HIGH or HIGH to LOW on the chosen module pin you wake the device back up.
  Note: Doing so on the INT0 pin when using the regular powerOff(durationInMs) function will wake the device anyway. (tested on SAM-M8Q)
  Note: While powered off, you should not query the device for data or it might wake up. This can be used to wake the device but is not recommended.
        Works best when also putting your microcontroller to sleep.

*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

// define a digital pin capable of driving HIGH and LOW
#define WAKEUP_PIN 5

// Possible GNSS interrupt pins for powerOffWithInterrupt are:
// VAL_RXM_PMREQ_WAKEUPSOURCE_UARTRX  = uartrx
// VAL_RXM_PMREQ_WAKEUPSOURCE_EXTINT0 = extint0 (default)
// VAL_RXM_PMREQ_WAKEUPSOURCE_EXTINT1 = extint1
// VAL_RXM_PMREQ_WAKEUPSOURCE_SPICS   = spics
// These values can be or'd (|) together to enable interrupts on multiple pins

void wakeUp() {

  DSerial.print("-- waking up module via pin " + String(WAKEUP_PIN));
  DSerial.println(" on your microcontroller --");

  digitalWrite(WAKEUP_PIN, LOW);
  delay(1000);
  digitalWrite(WAKEUP_PIN, HIGH);
  delay(1000);
  digitalWrite(WAKEUP_PIN, LOW);
}


void setup() {

  pinMode(WAKEUP_PIN, OUTPUT);
  digitalWrite(WAKEUP_PIN, LOW);

  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();

  //myGNSS.enableDebugging(); // Enable debug messages

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  // Powering off for 20s, you should see the power consumption drop.
  DSerial.println("-- Powering off module for 20s --");

  myGNSS.powerOff(20000);
  //myGNSS.powerOffWithInterrupt(20000, VAL_RXM_PMREQ_WAKEUPSOURCE_EXTINT0);

  delay(10000);

  // After 10 seconds wake the device via the specified pin on your microcontroller and module.
  wakeUp();
}

void loop() {
  //Do nothing
}
