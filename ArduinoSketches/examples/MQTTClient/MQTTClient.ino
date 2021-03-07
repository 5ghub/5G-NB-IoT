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

#include <board.h>

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

char mqtt_server[] = "iot-as-mqtt.cn-shanghai.aliyuncs.com";
unsigned int mqtt_port = 1883;
char mqtt_client_id[] = "mqtt_test";
unsigned int comm_pdp_index = 1;    // The range is 1 ~ 16
unsigned int comm_mqtt_index = 1; // The range is 0 ~ 5
Mqtt_Version_t version = MQTT_V3;
Mqtt_Qos_t mqtt_qos = AT_MOST_ONCE;
char produck_key[] = "a1mQBMY2HI9";
char device_name[] = "D896E0FF00012940";
char device_secret[] = "GVQqGv6Y3hd3q9n8EXLHW95Bpgs8HG3W";

_5G_NB_IoT_MQTT _5GNBIoT(ATSerial, DSerial);

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

  while (!_5GNBIoT.SetMQTTConfigureParameters(comm_mqtt_index, comm_pdp_index, version, 150, SERVER_STORE_SUBSCRIPTIONS)) {
    DSerial.println("\r\nConfig the MQTT Parameter Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nConfig the MQTT Parameter Success!");

  while (!_5GNBIoT.SetMQTTAlibabaCloudDeviceInformation(comm_mqtt_index, produck_key, device_name, device_secret)) {
    DSerial.println("\r\nSet the Alibaba Cloud MQTT Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nSet the Alibaba Cloud MQTT Success!");

  while (_5GNBIoT.OpenMQTTNetwork(comm_mqtt_index, mqtt_server, mqtt_port) != 0) {
    DSerial.println("\r\nSet the MQTT Service Address Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nSet the MQTT Service Address Success!");

  while (_5GNBIoT.CreateMQTTClient(comm_mqtt_index, mqtt_client_id, "", "") != 0) {
    DSerial.println("\r\nCreate a MQTT Client Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nCreate a MQTT Client Success!");

  while (_5GNBIoT.MQTTSubscribeTopic(comm_mqtt_index, 1, "/a1mQBMY2HI9/D896E0FF00012940/update", mqtt_qos) != 0) {
    DSerial.println("\r\nMQTT Subscribe Topic Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    }
  }
  DSerial.println("\r\nMQTT Subscribe Topic Success!");
}

void loop() {
  char mqtt_recv[128];
  char *sta_buf;
  Mqtt_URC_Event_t ret = _5GNBIoT.WaitCheckMQTTURCEvent(mqtt_recv, 2);
  switch (ret)
  {
    case MQTT_RECV_DATA_EVENT:
      sta_buf = strstr(mqtt_recv, "\",\"");
      DSerial.println("\r\nThe MQTT Recv Data");
      DSerial.println(sta_buf + 3);
      break;
    case MQTT_STATUS_EVENT:
      sta_buf = strchr(mqtt_recv, ',');
      if (atoi(sta_buf + 1) == 1) {
        if (_5GNBIoT.CloseMQTTClient(comm_mqtt_index)) {
          DSerial.println("\r\nClose the MQTT Client Success!");
        }
      } else {
        DSerial.print("\r\nStatus cade is :");
        DSerial.println(atoi(sta_buf + 1));
        DSerial.println("Please check the documentation for error details.");
      }
      break;
  }

  /*if (_5GNBIoT.MQTTPublishMessages(comm_mqtt_index, 1, mqtt_qos, "/a1mQBMY2HI9/D896E0FF00012940/get", true, "1234") == 0) {
    DSerial.println("\r\nMQTT Publish Messages Success!");
    delay(10000);
  }*/
}
