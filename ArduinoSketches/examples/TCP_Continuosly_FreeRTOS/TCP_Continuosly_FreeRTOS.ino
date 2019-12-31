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

#include <FreeRTOS_SAMD21.h>  // https://github.com/BriscoeTech/Arduino-FreeRTOS-SAMD21
#include "5G-NB-IoT_TCPIP.h"

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

#define ATT

#ifdef T-MOBILE
char APN[] = "internet2.voicestream.com";
char LOGIN[] = "guest";
char PASSWORD[] = "guest";
#endif
#ifdef ATT
char APN[] = "m2mNB16.com.attz";
char LOGIN[] = "";
char PASSWORD[] = "";
#endif
#ifdef MEGAFON_RUSSIA
char APN[] = "internet";
char LOGIN[] = "gdata";
char PASSWORD[] = "gdata";
#endif

TaskHandle_t Handle_aTask;
TaskHandle_t Handle_monitorTask;

char tcp_ip[] = "5ghub.us";
unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
unsigned int comm_socket_index = 2;  // The range is 0 ~ 11
unsigned int rssi;
unsigned int ping_cnt = 0;

_5G_NB_IoT_TCPIP _5GNBIoT(ATSerial, DSerial);

//**************************************************************************
// Can use these function for RTOS delays
// Takes into account procesor speed
//**************************************************************************
void myDelayUs(int us)
{
  vTaskDelay( us / portTICK_PERIOD_US );
}

void myDelayMs(int ms)
{
  vTaskDelay( (ms * 1000) / portTICK_PERIOD_US );
}

void myDelayMsUntil(TickType_t *previousWakeTime, int ms)
{
  vTaskDelayUntil( previousWakeTime, (ms * 1000) / portTICK_PERIOD_US );
}

//*****************************************************************
// Create a thread that prints out A to the screen every two seconds
// this task will delete its self after printing out afew messages
//*****************************************************************
static void threadA( void *pvParameters )
{
  DSerial.println("Thread A: Started");

  while (1)
  {
    _5GNBIoT.DevPingFunction(comm_socket_index, tcp_ip);
    ping_cnt++;

    /*    if (_5GNBIoT.GetDevNetSignalQuality(rssi)) {
          int _rssi = 0;
          if (rssi <= 31) {
            _rssi = -113 + rssi * 2;

            DSerial.print("\r\nRSSI = ");
            DSerial.print(_rssi, DEC);
            DSerial.print("dBm");
          }
        }*/
  }
}

//*****************************************************************
// Task will periodicallt print out useful information about the tasks running
// Is a useful tool to help figure out stack sizes being used
//*****************************************************************
void taskMonitor(void *pvParameters)
{
  static unsigned int pre_ping_cnt = 0;
  DSerial.println("Task Monitor: Started");

  while (1)
  {
    DSerial.print("Total count: "); DSerial.print(ping_cnt); DSerial.println(" ");
    DSerial.print("Count per sec: "); DSerial.print((ping_cnt - pre_ping_cnt) / 10); DSerial.println(" ");
    pre_ping_cnt = ping_cnt;
    myDelayMs(10000);
  }
}

void setup() {
  DSerial.begin(115200);
  while (DSerial.read() >= 0);
  DSerial.println("This is the _5GNBIoT Debug Serial!");

  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  _5GNBIoT.InitModule();
  DSerial.println("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetDevCommandEcho(false);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) {
    DSerial.println(inf);
  }

  char apn_error[64];
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, LOGIN, PASSWORD, apn_error)) {
    DSerial.println(apn_error);
    delay(1000);
  }
  DSerial.println(apn_error);

  // Create the threads that will be managed by the rtos
  // Sets the stack size and priority of each task
  // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
  xTaskCreate(threadA,     "Task A",       256, NULL, tskIDLE_PRIORITY + 1, &Handle_aTask);
  xTaskCreate(taskMonitor, "Task Monitor", 256, NULL, tskIDLE_PRIORITY + 3, &Handle_monitorTask);

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();
}

void loop() {
  vNopDelayMS(100);
}
