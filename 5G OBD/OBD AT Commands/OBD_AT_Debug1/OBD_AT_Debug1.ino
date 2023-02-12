// Simple serial to USB using the Arduino's USB 
// and Software Serial on the pins.
// Also using the gpio to power the adaptor.


#include <board.h>

#define DSerial SerialUSB
#define OBD_5GHUB Serial

#define OBD_5GHUB_RST   19  // Reset pin. Does a 10msec reset.
#define OBD_5GHUB_SLP   12  // Sleep Pin.

void setup() {
  
  // Define outputs to turn on 
  // power through the port and set Reset/Sleep to outputs.
  pinMode(OBD_5GHUB_RST, OUTPUT);
  pinMode(OBD_5GHUB_SLP, OUTPUT);
  digitalWrite(OBD_5GHUB_RST, HIGH);
  digitalWrite(OBD_5GHUB_SLP, HIGH);

  DSerial.begin(115200);
  OBD_5GHUB.begin(9600);
  OBD_5GHUB.setTimeout(5);

  //Do a reset 
  digitalWrite(OBD_5GHUB_RST, LOW);
  delay(100);
  digitalWrite(OBD_5GHUB_RST, HIGH);
  delay(100);

  DSerial.println("Multiprotocol OBD Ready.");
}

void loop() {
   if((OBD_5GHUB.available()))
    DSerial.println(OBD_5GHUB.readString());
    
   if((DSerial.available()))
    OBD_5GHUB.print(DSerial.readString());
}
