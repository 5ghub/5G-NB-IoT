/*

  Copyright 2020, 5G HUB

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
#include <ArduinoJson.h>

#define ATSerial Serial1

char APN[] = "m2mNB16.com.attz";
char LOGIN[] = "";
char PASSWORD[] = "";

char mqtt_server[] = "a3kjt69iibf2h0-ats.iot.us-west-2.amazonaws.com";
unsigned int mqtt_port = 8883;
char mqtt_clientId[] = "basicPubSub";
char mqtt_topicName[] = "MyTopic";
unsigned int mqtt_index = 3;
Mqtt_Qos_t mqtt_qos = AT_MOST_ONCE;
unsigned long pub_time;

// Allocate the documents to serialize and deserialize
StaticJsonDocument<200> docInput;
StaticJsonDocument<200> docOutput;

//IMEI of the modem
char IMEI[20];  

_5G_NB_IoT_MQTT AWSIoT(ATSerial, ATSerial);

// AWSIoT is the object of class _5G_NB_IoT_MQTT 
// DSerial USB serial
// APN name
// APIN login,
// APN password ,
// MQTT server FQDN, 
// MQTT port, number
// MQTT Client Id such as "BasibPubSub",
// MQTT topic name, 
// MQTT QoS  such as AT_MOST_ONCE, 
// MQTT index: The range is 0 ~ 5
// PDP index:  The range is 1 ~ 16
// SSL index:  The range is 0 ~ 5

//char *ModemIMEI  is an output contains the IMEI
bool Init_AWS_MQTT(_5G_NB_IoT_MQTT  &AWSIoT, 
                   char *APN,
                   char *LOGIN,
                   char *PASSWORD,
                   char *MQTTServer, 
                   unsigned int MQTTPort, 
                   char *MQTTClientId,
                   char *mqtt_topicName, 
                   Mqtt_Qos_t MQTT_QoS = AT_MOST_ONCE, 
                   unsigned int MQTTIndex = 3, 
                   unsigned int PDPIndex = 1, 
                   unsigned int SSLIndex = 2,
                   char *ModemIMEI = NULL)
{
  Mqtt_Version_t version = MQTT_V3;

  AWSIoT.InitModule();
  TFTprint("\r\n InitModule() OK!");
  
  AWSIoT.SetDevCommandEcho(false);
  
  char inf[64];
  if (AWSIoT.GetDevInformation(inf))
  {
    TFTprint(inf);
  }
  
  char imei_tmp[64];
  if (AWSIoT.GetDevIMEI(imei_tmp))
  {
    String s = String(imei_tmp);
    s.trim();
    s.toCharArray(ModemIMEI, 64);
    TFTprint(ModemIMEI);
  }

  AWSIoT.DeleteFiles("*");
  
  char apn_error[64];
  while (!AWSIoT.InitAPN(PDPIndex, APN, LOGIN, PASSWORD, apn_error)) 
  {
    TFTprint(apn_error);
  }
  TFTprint(apn_error);
  
  char ssl_error[128];
  while (!AWSIoT.InitSSL(SSLIndex, aws_root_ca_pem, certificate_pem_crt, private_pem_key, ssl_error)) 
  {
    TFTprint(ssl_error);
  }
  TFTprint(ssl_error);
  
  TFTprint("\r\nStart Config the MQTT Parameter!");
  while (!AWSIoT.SetMQTTConfigureParameters(MQTTIndex, PDPIndex, version, 150, SERVER_STORE_SUBSCRIPTIONS)) 
  {
    TFTprint("\r\nConfig the MQTT Parameter Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
    }
  }
  TFTprint("\r\nConfig the MQTT Parameter Success!");
  
  while (!AWSIoT.SetMQTTEnableSSL(MQTTIndex, SSLIndex, true)) 
  {
    TFTprint("\r\SetMQTTEnableSSL the MQTT Parameter Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
    }
  }
  TFTprint("\r\SetMQTTEnableSSL the MQTT Parameter Success!");

  /*while(!AWSIoT.ConfigDNSServer(PDPIndex,"208.67.222.222", "208.67.220.220"))
  {
    DSerial.println("\r\DNS Configuraiton failed!");  
  }*/
  
  while (AWSIoT.OpenMQTTNetwork(MQTTIndex, MQTTServer, MQTTPort) != 0) 
  {
    TFTprint("\r\nSet the MQTT Service Address Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
    }
  }
  TFTprint("\r\nSet the MQTT Service Address Success!");
  
  TFTprint("\r\nConfigure Timeout!");
  while(!AWSIoT.SetMQTTMessageTimeout(MQTTIndex, 10, 5, 1))
  {
      TFTprint("\r\nMQTT Timeout Fail!");
      int e_code;
      if (AWSIoT.returnErrorCode(e_code)) 
      {
        TFTprint("\r\nERROR CODE: ");
        TFTprint(String(e_code));
        TFTprint("Please check the documentation for error details.");
      }
    }
  
  TFTprint("\r\nStart Create a MQTT Client!");
  while (AWSIoT.CreateMQTTClient(MQTTIndex, MQTTClientId, "", "") != 0) 
  {
    TFTprint("\r\nCreate a MQTT Client Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
    }
  }
  TFTprint("\r\nCreate a MQTT Client Success!");
  
  TFTprint("\r\nStart MQTT Subscribe Topic!");
  while (AWSIoT.MQTTSubscribeTopic(MQTTIndex, 1, mqtt_topicName, MQTT_QoS) != 0) 
  {
    TFTprint("\r\nMQTT Subscribe Topic Fail!");
    int e_code;
    if (AWSIoT.returnErrorCode(e_code)) 
    {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
    }
  }
  TFTprint("\r\nMQTT Subscribe Topic Success!");

  return true;
}

void setup()
{
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  TFTinit("AWS MQTT CLIENT");

  Init_AWS_MQTT(AWSIoT, APN, LOGIN, PASSWORD,
                mqtt_server, mqtt_port,
                mqtt_clientId, mqtt_topicName,
                AT_MOST_ONCE, mqtt_index,
                1, 2, IMEI);
}

void loop()
{
  char payload[256];
  char *sta_buf;
  int res;
  DeserializationError error;

  Mqtt_URC_Event_t ret = AWSIoT.WaitCheckMQTTURCEvent(payload, 2);
  switch (ret)
  {
    case MQTT_RECV_DATA_EVENT:
      error = deserializeJson(docOutput, payload);

      if (error == DeserializationError::Ok)
      {
        if (docOutput["Device"] == "Temperature Sensor")
        {
          TFTprint("Device is a Temperature sensor!");

          TFTprint(docOutput["DeviceID"].as<String>());
          //TFTprint(docOutput["Timestamp"].as<String>(), 6);
          TFTprint(docOutput["Timestamp"].as<String>());
          TFTprint(docOutput["Device"].as<String>());
          TFTprint(docOutput["OpCode"].as<String>());
          TFTprint(docOutput["Temperature"].as<String>());
          TFTprint(docOutput["Unit"].as<String>());
        }
        else
        {
          TFTprint("Device is not a Temperature sensor!");
        }
      }
      else
      {
        TFTprint("\r\n Error in  Deserialization!");
        TFTprint(error.c_str());
      }
      

      break;

    case MQTT_STATUS_EVENT:
      sta_buf = strchr(payload, ',');
      if (atoi(sta_buf + 1) == 1)
      {
        if (AWSIoT.CloseMQTTClient(mqtt_index))
        {
          TFTprint("\r\nClose the MQTT Client Success!");
        }
      }
      else
      {
        TFTprint("\r\nStatus code is :");
        TFTprint(String(sta_buf + 1));
        TFTprint("Please check the documentation for error details.");
      }
      break;

    default:
      //TFTprint("\r\nUnknown event from Recv is received");
      //TFTprint(ret);
      break;
  }

  if (millis() - pub_time >= 5000UL) {
    pub_time = millis();

    docInput["DeviceID"] = IMEI;
    docInput["Timestamp"] = millis();
    docInput["Device"] = "Temperature Sensor";
    docInput["OpCode"] = "Read";
    docInput["Temperature"] = 30;
    docInput["Unit"] = "°F";
    serializeJsonPretty(docInput, payload);

    res = AWSIoT.MQTTPublishMessages(mqtt_index, 1, AT_LEAST_ONCE, mqtt_topicName, false, payload);

    if (res == PACKET_SEND_SUCCESS_AND_RECV_ACK ||
        res == PACKET_RETRANSMISSION)
    {
      TFTprint("Publish Succeded!");
    }
    else
    {
      TFTprint("Publish failed!");
    }
  }
  //  delay(5000);   // publish to topic every 5seconds
}
