/*
  u-blox Example: ESF RAW (100Hz!)

  This example configures the External Sensor Fusion RAW IMU sensor messages on the NEO-M8U / ZED-F9R and
  shows how to access the ESF data in the loop - without using the callback.

  Notes:
    On the ZED-F9R, each ESF RAW message contains _one_ set of IMU sensor data: seven readings in total (3 x Accel, 3 x Gyro, 1 x Temperature).
    However, on the NEO-M8U, each message contains _ten_ sets of IMU sensor data, seventy readings in total.
    The NEO-M8U data is all timestamped and it is possible to reconstruct the full data stream, you just need to do it
    ten samples at a time...
    Also, note that the sensor data is 24-bit signed (two's complement). You need to be careful when converting to int32_t.
    Data will arrive at 100Hz! (10Hz x 10 on the NEO-M8U)
    400kHz I2C is essential...
    Serial printing needs to be kept short and the baud rate needs to be at least 230400.

  Please make sure your NEO-M8U is running UDR firmware >= 1.31. Please update using u-center if necessary:
  https://www.u-blox.com/en/product/neo-m8u-module#tab-documentation-resources

*/

#include <board.h>

#define DSerial SerialUSB
SFE_UBLOX_GNSS myGNSS;

// Callback: printESFRAWdata will be called when new ESF RAW data arrives
// See u-blox_structs.h for the full definition of UBX_ESF_RAW_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setAutoESFRAWcallback
//        /                  _____  This _must_ be UBX_ESF_RAW_data_t
//        |                 /                   _____ You can use any name you like for the struct
//        |                 |                  /
//        |                 |                  |
void printESFRAWdata(UBX_ESF_RAW_data_t *ubxDataStruct)
{
  Serial.println(F("Hey! The ESF RAW callback has been called!"));
}

void setup()
{
  Serial.begin(230400); // <--- Use >> 100k baud (see notes above)
  
  while (!Serial); //Wait for user to open terminal
  Serial.println(F("  u-blox Example"));

  Wire.begin();
  Wire.setClock(400000); // <-- Use 400kHz I2C (ESSENTIAL)

  //myGNSS.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  myGNSS.setI2CpollingWait(5); //Allow checkUblox to poll I2C data every 5ms to keep up with the ESF RAW messages

  if (myGNSS.setAutoESFRAWcallbackPtr(&printESFRAWdata) == true) // Enable automatic ESF RAW messages with callback to printESFRAWdata
    Serial.println(F("setAutoESFRAWcallback successful"));
}

void loop()
{
  myGNSS.checkUblox(); // Check for the arrival of new data and process it.

  // Check if new ESF RAW data has arrived:
  // If myGNSS.packetUBXESFRAW->automaticFlags.flags.bits.callbackCopyValid is true, it indicates new ESF RAW data has been received and has been copied.
  // automaticFlags.flags.bits.callbackCopyValid will be cleared automatically when the callback is called.

  if (myGNSS.packetUBXESFRAW->automaticFlags.flags.bits.callbackCopyValid == true)
  {
    // But, we can manually clear the callback flag too. This will prevent the callback from being called!
    myGNSS.packetUBXESFRAW->automaticFlags.flags.bits.callbackCopyValid = false; // Comment this line if you still want the callback to be called

    // myGNSS.packetUBXESFRAW->callbackData->numEsfRawBlocks indicates how many sensor readings the UBX_ESF_RAW_data_t contains.
    // On the ZED-F9R, numEsfRawBlocks will be 7: 3 x Accel, 3 x Gyro, 1 x Temperature.
    // On the NEO-M8U, numEsfRawBlocks will be 70: 10 sets of sensor data. The sensor time tag (sTag)
    //   indicates the timing of each sample.
    // Serial output will be approx. 110 bytes depending on how many digits are in the sensor readings.
    // To keep up, Serial needs to be running at 100k baud minimum. 230400 is recommended.
  
    uint32_t sTag = 0xFFFFFFFF; // Sensor time tag
  
    // Only print the first seven sensor readings (on the NEO-M8U)
    for (uint8_t i = 0; (i < myGNSS.packetUBXESFRAW->callbackData->numEsfRawBlocks) && (i < 7); i++)
    // For fun, and to prove it works, uncomment use this line instead to get the full 100Hz data on the NEO-M8U
    //for (uint8_t i = 0; i < myGNSS.packetUBXESFRAW->callbackData->numEsfRawBlocks; i++)
    {
      // Print sTag the first time - and also if it changes
      if (sTag != myGNSS.packetUBXESFRAW->callbackData->data[i].sTag)
      {
        sTag = myGNSS.packetUBXESFRAW->callbackData->data[i].sTag;
        Serial.print(F("Time:"));
        Serial.println(sTag);
      }
  
      // Print the sensor data type
      // From the M8 interface description:
      //   0: None
      // 1-4: Reserved
      //   5: z-axis gyroscope angular rate deg/s * 2^-12 signed
      //   6: front-left wheel ticks: Bits 0-22: unsigned tick value. Bit 23: direction indicator (0=forward, 1=backward)
      //   7: front-right wheel ticks: Bits 0-22: unsigned tick value. Bit 23: direction indicator (0=forward, 1=backward)
      //   8: rear-left wheel ticks: Bits 0-22: unsigned tick value. Bit 23: direction indicator (0=forward, 1=backward)
      //   9: rear-right wheel ticks: Bits 0-22: unsigned tick value. Bit 23: direction indicator (0=forward, 1=backward)
      //  10: speed ticks: Bits 0-22: unsigned tick value. Bit 23: direction indicator (0=forward, 1=backward)
      //  11: speed m/s * 1e-3 signed
      //  12: gyroscope temperature deg Celsius * 1e-2 signed
      //  13: y-axis gyroscope angular rate deg/s * 2^-12 signed
      //  14: x-axis gyroscope angular rate deg/s * 2^-12 signed
      //  16: x-axis accelerometer specific force m/s^2 * 2^-10 signed
      //  17: y-axis accelerometer specific force m/s^2 * 2^-10 signed
      //  18: z-axis accelerometer specific force m/s^2 * 2^-10 signed
      switch (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType)
      {
        case 5:
          Serial.print(F("Zgyr:"));
          break;
        case 12:
          Serial.print(F("Temp:"));
          break;
        case 13:
          Serial.print(F("Ygyr:"));
          break;
        case 14:
          Serial.print(F("Xgyr:"));
          break;
        case 16:
          Serial.print(F("Xacc:"));
          break;
        case 17:
          Serial.print(F("Yacc:"));
          break;
        case 18:
          Serial.print(F("Zacc:"));
          break;
        default:
          break;
      }
      
      // Gyro data
      if ((myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 5) || (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 13) || (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 14))
      {
        union
        {
          int32_t signed32;
          uint32_t unsigned32;
        } signedUnsigned; // Avoid any ambiguity casting uint32_t to int32_t
        // The dataField is 24-bit signed, stored in the 24 LSBs of a uint32_t
        signedUnsigned.unsigned32 = myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataField  << 8; // Shift left by 8 bits to correctly align the data
        float rate =  signedUnsigned.signed32; // Extract the signed data. Convert to float
        rate /= 256.0; // Divide by 256 to undo the shift
        rate *= 0.000244140625; // Convert from deg/s * 2^-12 to deg/s
        Serial.println(rate);     
      }
      // Accelerometer data
      else if ((myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 16) || (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 17) || (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 18))
      {
        union
        {
          int32_t signed32;
          uint32_t unsigned32;
        } signedUnsigned; // Avoid any ambiguity casting uint32_t to int32_t
        // The dataField is 24-bit signed, stored in the 24 LSBs of a uint32_t
        signedUnsigned.unsigned32 = myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataField  << 8; // Shift left by 8 bits to correctly align the data
        float force =  signedUnsigned.signed32; // Extract the signed data. Convert to float
        force /= 256.0; // Divide by 256 to undo the shift
        force *= 0.0009765625; // Convert from m/s^2 * 2^-10 to m/s^2
        Serial.println(force);     
      }
      // Gyro Temperature
      else if (myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataType == 12)
      {
        union
        {
          int32_t signed32;
          uint32_t unsigned32;
        } signedUnsigned; // Avoid any ambiguity casting uint32_t to int32_t
        // The dataField is 24-bit signed, stored in the 24 LSBs of a uint32_t
        signedUnsigned.unsigned32 = myGNSS.packetUBXESFRAW->callbackData->data[i].data.bits.dataField  << 8; // Shift left by 8 bits to correctly align the data
        float temperature =  signedUnsigned.signed32; // Extract the signed data. Convert to float
        temperature /= 256.0; // Divide by 256 to undo the shift
        temperature *= 0.01; // Convert from C * 1e-2 to C
        Serial.println(temperature);     
      }
    }
  }

  myGNSS.checkCallbacks(); // Check if any callbacks are waiting to be processed. There will not be any in this example, unless you commented the line above  
}
