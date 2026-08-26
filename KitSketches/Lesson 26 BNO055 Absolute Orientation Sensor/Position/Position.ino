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

double xPos = 0, yPos = 0, headingVel = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
uint16_t PRINT_DELAY_MS = 500; // how often to print the data
uint16_t printCount = 0; //counter to avoid printing every 10MS sample

//velocity = accel*dt (dt in seconds)
//position = 0.5*accel*dt^2
double ACCEL_VEL_TRANSITION =  (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;
double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
double DEG_2_RAD = 0.01745329251; //trig functions require radians, BNO055 outputs degrees

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
_5GHUB_BNO055 bno = _5GHUB_BNO055(-1, 0x28);

#define DSerial SerialUSB
#define ATSerial Serial1


void setup(void)
{
  SerialUSB.begin(115200);
  if (!bno.Begin())
  {
    SerialUSB.print("No BNO055 detected");
    while (1);
  }


  delay(1000);
}

void loop(void)
{
  //
  unsigned long tStart = micros();
  sensors_event_t orientationData , linearAccelData;
  bno.GetEvent(&orientationData, _5GHUB_BNO055::VECTOR_EULER);
  //  bno.getEvent(&angVelData, _5GHUB_BNO055::VECTOR_GYROSCOPE);
  bno.GetEvent(&linearAccelData, _5GHUB_BNO055::VECTOR_LINEARACCEL);

  xPos = xPos + ACCEL_POS_TRANSITION * linearAccelData.acceleration.x;
  yPos = yPos + ACCEL_POS_TRANSITION * linearAccelData.acceleration.y;

  // velocity of sensor in the direction it's facing
  headingVel = ACCEL_VEL_TRANSITION * linearAccelData.acceleration.x / cos(DEG_2_RAD * orientationData.orientation.x);

  if (printCount * BNO055_SAMPLERATE_DELAY_MS >= PRINT_DELAY_MS) 
  {
    //enough iterations have passed that we can print the latest data
    SerialUSB.print("Heading: ");
    SerialUSB.println(orientationData.orientation.x);
    SerialUSB.print("Position: ");
    SerialUSB.print(xPos);
    SerialUSB.print(" , ");
    SerialUSB.println(yPos);
    SerialUSB.print("Speed: ");
    SerialUSB.println(headingVel);
    SerialUSB.println("-------");

    printCount = 0;
  }
  else 
  {
    printCount = printCount + 1;
  }



  while ((micros() - tStart) < (BNO055_SAMPLERATE_DELAY_MS * 1000))
  {
    //poll until the next sample is ready
  }
}

void printEvent(sensors_event_t* event) 
{
  SerialUSB.println();
  SerialUSB.print(event->type);
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) 
  {
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) 
  {
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) 
  {
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if ((event->type == SENSOR_TYPE_GYROSCOPE) || (event->type == SENSOR_TYPE_ROTATION_VECTOR)) 
  {
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }

  SerialUSB.print(": x= ");
  SerialUSB.print(x);
  SerialUSB.print(" | y= ");
  SerialUSB.print(y);
  SerialUSB.print(" | z= ");
  SerialUSB.println(z);
}
