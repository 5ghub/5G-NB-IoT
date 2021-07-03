/*

  Copyright 2021, 5G HUB

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
  associated documentation files (the "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
  following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <board.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// Using I2C Interface
_5GHUB_BME680 bme; 

#define DSerial SerialUSB
#define ATSerial Serial1

void setup() 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println(F("BME680 async test"));

  if (!bme.Begin()) 
  {
    DSerial.println(F("BME680 sensor can not be found. Check again!"));
    while (1);
  }

  // Configure BME680 oversampling and filter
  bme.SetTemperatureOversampling(BME680_OS_8X);
  bme.SetHumidityOversampling(BME680_OS_2X);
  bme.SetPressureOversampling(BME680_OS_4X);
  bme.SetIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.SetGasHeater(320, 150); // 320*C for 150 ms
}

void loop() 
{
  // start BME680 to start doing measurement.
  unsigned long endTime = bme.BeginReading();
  if (endTime == 0) 
  {
    DSerial.println(F("Failed to start reading :("));
    return;
  }
  DSerial.print(F("Reading started at "));
  DSerial.print(millis());
  DSerial.print(F(" and will finish at "));
  DSerial.println(endTime);

  DSerial.println(F("While BME680 do measurements, you can schedule another task."));
  delay(100); // This represents parallel work.

  // Finally get measurement results from BME680. This operation is not
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme.EndReading()) 
  {
    DSerial.println(F("Failed to complete reading :("));
    return;
  }
  DSerial.print(F("Reading finished at "));
  DSerial.println(millis());

  DSerial.print(F("Temperature = "));
  DSerial.print(bme.temperature);
  DSerial.println(F(" C"));

  
  DSerial.print(F("Humidity = "));
  DSerial.print(bme.humidity);
  DSerial.println(F(" %"));
  
  DSerial.print(F("Pressure = "));
  DSerial.print(bme.pressure / 100.0);
  DSerial.println(F(" hPa"));

  DSerial.print(F("Gas = "));
  DSerial.print(bme.gas_resistance / 1000.0);
  DSerial.println(F(" Kohms"));

  DSerial.print(F("Altitude = "));
  DSerial.print(bme.ReadAltitude(SEALEVELPRESSURE_HPA));
  DSerial.println(F(" m"));

  DSerial.println();
  delay(3000);
}
