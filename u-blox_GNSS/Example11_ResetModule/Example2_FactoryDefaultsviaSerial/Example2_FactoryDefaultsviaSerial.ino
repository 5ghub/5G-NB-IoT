/*

  This example shows how to reset the U-Blox module to factory defaults over serial.

*/

#include <board.h>

#define DSerial SerialUSB


SFE_UBLOX_GNSS myGNSS;

//#define mySerial Serial1 // Uncomment this line to connect via Serial1
// - or -
//SoftwareSerial mySerial(10, 11); // Uncomment this line to connect via SoftwareSerial(RX, TX). Connect pin 10 to GNSS TX pin.

#define defaultRate 9600 // Uncomment this line if you are using an M8 - which defaults to 9600 Baud on UART1
// - or -
//#define defaultRate 38400 // Uncomment this line if you are using an F9 - which defaults to 38400 Baud on UART1

int state = 0; // steps through auto-baud, reset, etc states

void setup()
{
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
}

void loop()
{
    DSerial.print("===== STATE ");
    DSerial.println(state);
    switch (state) {
    case 0: // auto-baud connection, then switch to 38400 and save config
        do {
            DSerial.println("GNSS: trying 38400 baud");
            mySerial.begin(38400);
            if (myGNSS.begin(mySerial)) break;

            delay(100);
            DSerial.println("GNSS: trying 9600 baud");
            mySerial.begin(9600);
            if (myGNSS.begin(mySerial)) {
                Serial.println("GNSS: connected at 9600 baud, switching to 38400");
                myGNSS.setSerialRate(38400);
                delay(100);
            } else {
                delay(2000); //Wait a bit before trying again to limit the Serial output flood
            }
        } while(1);
        myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
        myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
        DSerial.println("GNSS serial connected, saved config");
        state++;
        break;
    case 1: // hardReset, expect to see GNSS back at 38400 baud
        DSerial.println("Issuing hardReset (cold start)");
        myGNSS.hardReset();
        delay(2000);
        mySerial.begin(38400);
        if (myGNSS.begin(mySerial)) {
            DSerial.println("Success.");
            state++;
        } else {
            DSerial.println("*** GNSS did not respond at 38400 baud, starting over.");
            state = 0;
        }
        break;
    case 2: // factoryReset, expect to see GNSS back at defaultRate baud
        DSerial.println("Issuing factoryReset");
        myGNSS.factoryReset();
        delay(5000); // takes more than one second... a loop to resync would be best
        mySerial.begin(defaultRate);
        if (myGNSS.begin(mySerial)) {
            DSerial.println("Success.");
            state++;
        } else {
            Serial.println("*** GNSS did not come back at defaultRate baud, starting over.");
            state = 0;
        }
        break;
    case 3: // print version info
        // Note: this may fail on boards like the UNO (ATmega328P) with modules like the ZED-F9P
        // because getProtocolVersion returns a lot of data - more than the UNO's serial buffer can hold
        DSerial.print("GNSS protocol version: ");
        DSerial.print(myGNSS.getProtocolVersionHigh());
        DSerial.print('.');
        DSerial.println(myGNSS.getProtocolVersionLow());
        DSerial.println("All finished! Freezing...");
        while(1);
    }
    delay(1000);
}
