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

void setup(void)
{
  SerialUSB.begin(115200);
  SerialUSB.println("Orientation Sensor Test"); 
  SerialUSB.println("");

  /* Initialise the sensor */
  if (!bno.Begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    SerialUSB.print("No BNO055 detected. Check wiring or I2C Address!");
    while (1);
  }

  delay(1000);
}

void loop(void)
{
  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.GetEvent(&orientationData, _5GHUB_BNO055::VECTOR_EULER);
  bno.GetEvent(&angVelocityData, _5GHUB_BNO055::VECTOR_GYROSCOPE);
  bno.GetEvent(&linearAccelData, _5GHUB_BNO055::VECTOR_LINEARACCEL);
  bno.GetEvent(&magnetometerData, _5GHUB_BNO055::VECTOR_MAGNETOMETER);
  bno.GetEvent(&accelerometerData, _5GHUB_BNO055::VECTOR_ACCELEROMETER);
  bno.GetEvent(&gravityData, _5GHUB_BNO055::VECTOR_GRAVITY);

  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  printEvent(&accelerometerData);
  printEvent(&gravityData);

  int8_t boardTemp = bno.GetTemp();
  SerialUSB.println();
  SerialUSB.print(F("temperature: "));
  SerialUSB.println(boardTemp);

  uint8_t system, gyro, accel, mag = 0;
  bno.GetCalibration(&system, &gyro, &accel, &mag);
  SerialUSB.println();
  SerialUSB.print("Calibration: Sys=");
  SerialUSB.print(system);
  SerialUSB.print(" Gyro=");
  SerialUSB.print(gyro);
  SerialUSB.print(" Accel=");
  SerialUSB.print(accel);
  SerialUSB.print(" Mag=");
  SerialUSB.println(mag);

  SerialUSB.println("--");
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void printEvent(sensors_event_t* event) 
{
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) 
  {
    SerialUSB.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) 
  {
    SerialUSB.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) 
  {
    SerialUSB.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) 
  {
    SerialUSB.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) 
  {
    SerialUSB.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) 
  {
    SerialUSB.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) 
  {
    SerialUSB.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else 
  {
    SerialUSB.print("Unk:");
  }

  SerialUSB.print("\tx= ");
  SerialUSB.print(x);
  SerialUSB.print(" |\ty= ");
  SerialUSB.print(y);
  SerialUSB.print(" |\tz= ");
  SerialUSB.println(z);
}
