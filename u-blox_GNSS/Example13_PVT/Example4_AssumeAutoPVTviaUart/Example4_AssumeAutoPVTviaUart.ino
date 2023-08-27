/*
  Reading lat and long via UBX binary commands using an RX-only UART

  This example shows how to configure the library for serial port use with a single wire connection using the assumeAutoPVT method.
  Saving your pins for other stuff :-)

  This example only works correctly if the module has already been configured is sending PVT messages via its UART1 TX pin.
  Run the previous example to enable the auto PVT messages if you need to.

  Leave NMEA parsing behind. Now you can simply ask the module for the datums you want!

  Preconditions:
  U-Blox module is configured to send cyclical PVT message
  
  Hardware Connections:
  Connect the U-Blox serial TX pin to Rx of Serial2 (default: GPIO16) on your ESP32
  Or, switch to SoftwareSerial
  Open the serial monitor at 115200 baud to see the output
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
//SoftwareSerial mySerial(10, 11); // Uncomment this line to connect via SoftwareSerial(RX, TX). Connect pin 10 to GNSS TX pin.

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

  //Use any Serial port with at least a RX Pin connected or a receive only version of SoftwareSerial here
  //Assume that the U-Blox GNSS is running at baudRate baud
  mySerial.begin(baudRate);
  // No need to check return value as internal call to isConnected() will not succeed
  myGNSS.begin(mySerial);

  // Tell the library we are expecting the module to send PVT messages by itself to our Rx pin.
  // You can set second parameter to "false" if you want to control the parsing and eviction of the data (need to call checkUblox cyclically)
  myGNSS.assumeAutoPVT(true, true);

}

void loop()
{
  // If implicit updates are enabled, calling getPVT will trigger parsing of the incoming messages
  // and return true once a PVT message has been parsed.
  // In case you want to use explicit updates, wrap this in a timer and call checkUblox()
  // as often as needed, not to overflow your UART buffers.
  //
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
