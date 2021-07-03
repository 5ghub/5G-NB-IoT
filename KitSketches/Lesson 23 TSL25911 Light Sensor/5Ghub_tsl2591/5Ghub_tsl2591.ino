/* TSL2591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include <board.h>

#define DSerial SerialUSB
#define ATSerial Serial1

_5GHUB_TSL25911 tsl = _5GHUB_TSL25911(25911); // pass in a number for the sensor identifier (for your use later)

//**************************************************************************/
//    Displays basic information 
//**************************************************************************/

void DisplaySensorDetails(void)
{
  sensor_t sensor;
  tsl.GetSensor(&sensor);
  DSerial.println(F("------------------------------------"));
  DSerial.print  (F("Sensor:     ")); DSerial.println(sensor.name);
  DSerial.print  (F("Driver Ver: ")); DSerial.println(sensor.version);
  DSerial.print  (F("Unique ID:  ")); DSerial.println(sensor.sensor_id);
  DSerial.print  (F("Max Value:  ")); DSerial.print(sensor.max_value); DSerial.println(F(" lux"));
  DSerial.print  (F("Min Value:  ")); DSerial.print(sensor.min_value); DSerial.println(F(" lux"));
  DSerial.print  (F("Resolution: ")); DSerial.print(sensor.resolution, 4); DSerial.println(F(" lux"));  
  DSerial.println(F("------------------------------------"));
  DSerial.println(F(""));
  delay(1000);
}

///**************************************************************************/
//
//    Configures the gain and integration time for the TSL2591
//
//**************************************************************************/
void ConfigureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  
  //tsl.SetGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.SetGain(TSL25911_GAIN_MED);      // 25x gain
  //tsl.SetGain(TSL25911_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  // shortest integration time (bright light)
  
  //tsl.SetTiming(TSL25911_INTEGRATIONTIME_100MS);  
  // tsl.SetTiming(TSL25911_INTEGRATIONTIME_200MS);
  tsl.SetTiming(TSL25911_INTEGRATIONTIME_300MS);
  // tsl.SetTiming(TSL25911_INTEGRATIONTIME_400MS);
  // tsl.SetTiming(TSL25911_INTEGRATIONTIME_500MS);
  // tsl.SetTiming(TSL25911_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  // Display the gain and integration time for reference sake 
  DSerial.println(F("------------------------------------"));
  DSerial.print  (F("Gain:         "));
  tsl25911Gain_t gain = tsl.GetGain();
  
  switch(gain)
  {
    case TSL25911_GAIN_LOW:
      DSerial.println(F("1x (Low)"));
      break;
    case TSL25911_GAIN_MED:
      DSerial.println(F("25x (Medium)"));
      break;
    case TSL25911_GAIN_HIGH:
      DSerial.println(F("428x (High)"));
      break;
    case TSL25911_GAIN_MAX:
      DSerial.println(F("9876x (Max)"));
      break;
  }
  DSerial.print  (F("Timing:       "));
  DSerial.print((tsl.GetTiming() + 1) * 100, DEC); 
  DSerial.println(F(" ms"));
  DSerial.println(F("------------------------------------"));
  DSerial.println(F(""));
}

//**************************************************************************/
//    Shows how to perform a basic read on visible, full spectrum or
//    infrared light (returns raw 16-bit ADC values)
//**************************************************************************/
void BasicRead(void)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  
  uint16_t x = tsl.GetLuminosity(TSL25911_VISIBLE);
  //uint16_t x = tsl.GetLuminosity(TSL25911_FULLSPECTRUM);
  //uint16_t x = tsl.GetLuminosity(TSL25911_INFRARED);

  DSerial.print(F("[ ")); DSerial.print(millis()); DSerial.print(F(" ms ] "));
  DSerial.print(F("Luminosity: "));
  DSerial.println(x, DEC);
}

//**************************************************************************/
//    Show how to read IR and Full Spectrum at once and convert to lux
//**************************************************************************/
void AdvancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.GetFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  DSerial.print(F("[ ")); DSerial.print(millis()); DSerial.print(F(" ms ] "));
  DSerial.print(F("IR: ")); DSerial.print(ir);  DSerial.print(F("  "));
  DSerial.print(F("Full: ")); DSerial.print(full); DSerial.print(F("  "));
  DSerial.print(F("Visible: ")); DSerial.print(full - ir); DSerial.print(F("  "));
  DSerial.print(F("Lux: ")); DSerial.println(tsl.CalculateLux(full, ir), 6);
}

void setup(void) 
{
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  
  delay(3000);
  DSerial.println(F("TSL25911 test"));
    
  if (tsl.Begin()) 
  {
    DSerial.println(F("Found a TSL25911 sensor"));
  } 
  else 
  {
    DSerial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
    
  DisplaySensorDetails();
  
  ConfigureSensor();
}

void loop(void) 
{ 
  AdvancedRead();
  
  delay(500);
}
