/*
  Send Custom Command
  This example shows how you can create and send a custom UBX packet
 

  Previously it was possible to create and send a custom packet
  through the library but it would always appear to timeout as
  some of the internal functions referred to the internal private
  struct packetCfg.
  The most recent version of the library allows sendCommand to
  use a custom packet as if it were packetCfg and so:
  - sendCommand will return a sfe_ublox_status_e enum as if
    it had been called from within the library
  - the custom packet will be updated with data returned by the module
    (previously this was not possible from outside the library)
*/


#include <board.h>

#define DSerial SerialUSB

#define NAV_RATE 5 // The new navigation rate in Hz (measurements per second)

SFE_UBLOX_GNSS myGNSS;

void setup()
{
  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);
  DSerial.println("  u-blox Example");

  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  // Let's configure the module's navigation rate as if we were using setNavigationFrequency

  // Let's create our custom packet
  uint8_t customPayload[MAX_PAYLOAD_SIZE]; // This array holds the payload data bytes. MAX_PAYLOAD_SIZE defaults to 256. The CFG_RATE payload is only 6 bytes!

  // setPacketCfgPayloadSize tells the library how many bytes our customPayload can hold.
  // It is more memory-efficient to call setPacketCfgPayloadSize before .begin (to avoid creating a new buffer, copying across
  // the contents of the old buffer and then deleting the old buffer). But let's call it here just to prove that we can.
  myGNSS.setPacketCfgPayloadSize(MAX_PAYLOAD_SIZE);

  // The next line creates and initialises the packet information which wraps around the payload
  ubxPacket customCfg = {0, 0, 0, 0, 0, customPayload, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};

  // The structure of ubxPacket is:
  // uint8_t cls           : The message Class
  // uint8_t id            : The message ID
  // uint16_t len          : Length of the payload. Does not include cls, id, or checksum bytes
  // uint16_t counter      : Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
  // uint16_t startingSpot : The counter value needed to go past before we begin recording into payload array
  // uint8_t *payload      : The payload
  // uint8_t checksumA     : Given to us by the module. Checked against the rolling calculated A/B checksums.
  // uint8_t checksumB
  // sfe_ublox_packet_validity_e valid            : Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
  // sfe_ublox_packet_validity_e classAndIDmatch  : Goes from NOT_DEFINED to VALID or NOT_VALID when the Class and ID match the requestedClass and requestedID

  // sendCommand will return:
  // SFE_UBLOX_STATUS_DATA_RECEIVED if the data we requested was read / polled successfully
  // SFE_UBLOX_STATUS_DATA_SENT     if the data we sent was writted successfully (ACK'd)
  // Other values indicate errors. Please see the sfe_ublox_status_e enum for further details.

  // Referring to the u-blox M8 Receiver Description and Protocol Specification we see that
  // the navigation rate is configured using the UBX-CFG-RATE message. So let's load our
  // custom packet with the correct information so we can read (poll / get) the current settings.

  customCfg.cls = UBX_CLASS_CFG; // This is the message Class
  customCfg.id = UBX_CFG_RATE; // This is the message ID
  customCfg.len = 0; // Setting the len (length) to zero let's us poll the current settings
  customCfg.startingSpot = 0; // Always set the startingSpot to zero (unless you really know what you are doing)

  // We also need to tell sendCommand how long it should wait for a reply
  uint16_t maxWait = 250; // Wait for up to 250ms (Serial may need a lot longer e.g. 1100)

  // Now let's read the current navigation rate. The results will be loaded into customCfg.
  if (myGNSS.sendCommand(&customCfg, maxWait) != SFE_UBLOX_STATUS_DATA_RECEIVED) // We are expecting data and an ACK
  {
    DSerial.println(F("sendCommand (poll / get) failed! Freezing..."));
    while (1)
      ;
  }

  // Referring to the message definition for UBX-CFG-RATE we see that the measurement rate
  // is stored in payload bytes 0 and 1 as a uint16_t in LSB-first (little endian) format

  uint16_t rate = (customPayload[1] << 8) | customPayload[0]; // Extract the current rate (ms)
  float f_rate = 1000.0 / ((float)rate); // Convert the navigation rate to Hz (measurements per second)

  // Print the current measurement rate
  DSerial.print(F("The current measurement rate is: "));
  DSerial.println(f_rate, 1);

  // Let's change it
  rate = 1000 / NAV_RATE; // Load the new value into rate
  customPayload[0] = rate & 0xFF; // Store it in the payload
  customPayload[1] = rate >> 8;

  // Print the new measurement rate
  DSerial.print(F("The new measurement rate will be: "));
  DSerial.println(NAV_RATE);

  // We don't need to update customCfg.len as it will have been set to 6
  // when sendCommand read the data

  // Now we write the custom packet back again to change the setting
  if (myGNSS.sendCommand(&customCfg, maxWait) != SFE_UBLOX_STATUS_DATA_SENT) // This time we are only expecting an ACK
  {
    DSerial.println(F("sendCommand (set) failed! Freezing."));
    while (1)
      ;
  }
  else
  {
    DSerial.println(F("Navigation rate updated. Here we go..."));
  }

  myGNSS.setAutoPVT(true); // Enable AutoPVT. The module will generate measurements automatically without being polled.

  //myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_NAVCONF); //Uncomment this line to save only the NAV settings to flash and BBR
}

void loop()
{
  //Query the module as fast as possible
    int32_t latitude = myGNSS.getLatitude();
    DSerial.print(F("Lat: "));
    DSerial.print(latitude);

    int32_t longitude = myGNSS.getLongitude();
    DSerial.print(F(" Lon: "));
    DSerial.print(longitude);
    DSerial.print(F(" (degrees * 10^-7)"));

    int32_t altitude = myGNSS.getAltitude();
    DSerial.print(F(" Alt: "));
    DSerial.print(altitude);
    DSerial.print(F(" (mm)"));
    
    uint16_t milliseconds = myGNSS.getMillisecond();
    DSerial.print(F(" Milliseconds: "));
    DSerial.print(milliseconds);
    DSerial.println();
}
