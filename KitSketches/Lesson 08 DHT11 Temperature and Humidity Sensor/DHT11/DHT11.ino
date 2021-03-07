/*

  Copyright 2019, 5G HUB

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

/*#include <dht_nonblocking.h>
  #define DHT_SENSOR_TYPE DHT_TYPE_11

  static const int DHT_SENSOR_PIN = 2;
  DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
*/
#include <board.h>
#include <DHT.h>
#include <DHT_U.h>

//#define DSerial Serial
#define DSerial SerialUSB

#define DHTPIN   PA6
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() 
{
  DSerial.begin(9600);
  while (!DSerial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(1000);
  
  // Initialize device.
  dht.begin();
  DSerial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  DSerial.println(F("------------------------------------"));
  DSerial.println(F("Temperature Sensor"));
  DSerial.print  (F("Sensor Type: ")); DSerial.println(sensor.name);
  DSerial.print  (F("Driver Ver:  ")); DSerial.println(sensor.version);
  DSerial.print  (F("Unique ID:   ")); DSerial.println(sensor.sensor_id);
  DSerial.print  (F("Max Value:   ")); DSerial.print(sensor.max_value); DSerial.println(F("°C"));
  DSerial.print  (F("Min Value:   ")); DSerial.print(sensor.min_value); DSerial.println(F("°C"));
  DSerial.print  (F("Resolution:  ")); DSerial.print(sensor.resolution); DSerial.println(F("°C"));
  DSerial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  DSerial.println(F("Humidity Sensor"));
  DSerial.print  (F("Sensor Type: ")); DSerial.println(sensor.name);
  DSerial.print  (F("Driver Ver:  ")); DSerial.println(sensor.version);
  DSerial.print  (F("Unique ID:   ")); DSerial.println(sensor.sensor_id);
  DSerial.print  (F("Max Value:   ")); DSerial.print(sensor.max_value); DSerial.println(F("%"));
  DSerial.print  (F("Min Value:   ")); DSerial.print(sensor.min_value); DSerial.println(F("%"));
  DSerial.print  (F("Resolution:  ")); DSerial.print(sensor.resolution); DSerial.println(F("%"));
  DSerial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() 
{
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
  {
    DSerial.println(F("Error reading temperature!"));
  }
  else 
  {
    DSerial.print(F("Temperature: "));
    DSerial.print(event.temperature);
    DSerial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    DSerial.println(F("Error reading humidity!"));
  }
  else 
  {
    DSerial.print(F("Humidity: "));
    DSerial.print(event.relative_humidity);
    DSerial.println(F("%"));
  }
}
