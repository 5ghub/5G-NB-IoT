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

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
_5GHUB_BNO055 bno = _5GHUB_BNO055(-1, 0x28);

#define DSerial SerialUSB
#define ATSerial Serial1

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  SerialUSB.begin(115200);
  SerialUSB.println("Orientation Sensor Raw Data Test"); 
  Serial.println("");

  //delay(5000);

  /* Initialise the sensor */
  if (!bno.Begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    SerialUSB.print("No BNO055 detected. Check wiring or I2C Address!");
    while (1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.GetTemp();
  SerialUSB.print("Current Temperature: ");
  SerialUSB.print(temp);
  SerialUSB.println(" C");
  SerialUSB.println("");

  bno.SetExtCrystalUse(true);

  SerialUSB.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}

/**************************************************************************/
/*
 * 
 */
/**************************************************************************/
void loop(void)
{
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  imu::Vector<3> euler = bno.GetVector(_5GHUB_BNO055::VECTOR_EULER);

  /* Display the floating point data */
  SerialUSB.print("X: ");
  SerialUSB.print(euler.x());
  SerialUSB.print(" Y: ");
  SerialUSB.print(euler.y());
  SerialUSB.print(" Z: ");
  SerialUSB.print(euler.z());
  SerialUSB.print("\t\t");

  /* Display calibration status for each sensor. */
  uint8_t system, gyro, accel, mag = 0;
  bno.GetCalibration(&system, &gyro, &accel, &mag);
  SerialUSB.print("CALIBRATION: Sys=");
  SerialUSB.print(system, DEC);
  SerialUSB.print(" Gyro=");
  SerialUSB.print(gyro, DEC);
  SerialUSB.print(" Accel=");
  SerialUSB.print(accel, DEC);
  SerialUSB.print(" Mag=");
  SerialUSB.println(mag, DEC);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
