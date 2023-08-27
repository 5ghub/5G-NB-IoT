/*
  Configuring the GNSS to automatically send position reports over Serial

  This example shows how to configure the U-Blox GNSS the send navigation reports automatically
  and retrieving the latest one via getPVT. This eliminates the blocking in getPVT while the GNSS
  produces a fresh navigation solution at the expense of returning a slighly old solution.

  This can be used over serial or over I2C, this example shows the Serial use. With serial the GNSS
  simply outputs the UBX_NAV_PVT packet. With I2C it queues it into its internal I2C buffer (4KB in
  size?) where it can be retrieved in the next I2C poll.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!
*/

#include <board.h>
#define DSerial SerialUSB

SFE_UBLOX_GNSS myGNSS;

Uart mySerial(&sercom1, MOSI, SCK, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM1_Handler()
{
  mySerial.IrqHandler();
}

//#define mySerial Serial2 // Uncomment this line to connect via Serial2
// - or -
//Uart mySerial(10, 11); // Uncomment this line to connect via SoftwareSerial(RX, TX). Connect pin 10 to GNSS TX pin.

//#define baudRate 9600 // Uncomment this line to select 9600 Baud for the M8
// - or -
#define baudRate 38400 // Uncomment this line to select 38400 Baud for the F9

void setup()
{

  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);

  DSerial.println("  u-blox Example");

  mySerial.begin(baudRate); // Start the Serial port

  if (myGNSS.begin(mySerial) == false) //Connect to the u-blox module using Serial
  {
    DSerial.println(F("u-blox GNSS not detected. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART1 port to output UBX only (turn off NMEA noise)
  myGNSS.setNavigationFrequency(2); //Produce two solutions per second
  myGNSS.setAutoPVT(true); //Tell the GNSS to "send" each solution
  //myGNSS.saveConfiguration(); //Optional: Save the current settings to flash and BBR
}

void loop()
{
  // getPVT will return true if there actually is a fresh navigation solution available.
  // Important note: the PVT message is 100 bytes long. We need to call getPVT often enough
  // to prevent serial buffer overflows on boards like the original RedBoard / UNO.
  // At 38400 Baud, the 100 PVT bytes will arrive in 26ms.
  // On the RedBoard, we need to call getPVT every 5ms to keep up.
  if (myGNSS.getPVT())
  {
    DSerial.println();

    long latitude = myGNSS.getLatitude();
    DSerial.print(F("Lat: "));
    DSerial.print(latitude);

    long longitude = myGNSS.getLongitude();
    DSerial.print(F(" Long: "));
    DSerial.print(longitude);
    DSerial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    DSerial.print(F(" Alt: "));
    DSerial.print(altitude);
    DSerial.print(F(" (mm)"));

    byte SIV = myGNSS.getSIV();
    DSerial.print(F(" SIV: "));
    DSerial.print(SIV);

    DSerial.println();
  }
  else
  {
    delay(5); // Delay for 5ms only

    static int counter = 0; // Print a dot every 50ms
    counter++;
    if (counter > 10)
    {
      DSerial.print(".");
      counter = 0;
    }
  }
}
