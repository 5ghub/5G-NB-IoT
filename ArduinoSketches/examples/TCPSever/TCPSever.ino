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
char APN[] = "phone";
char LOGIN[] = "";
char PASSWORD[] = "";
#endif
#ifdef MEGAFON_RUSSIA
char APN[] = "internet";
char LOGIN[] = "gdata";
char PASSWORD[] = "gdata";
#endif

char tcps_ip[] = "127.0.0.1";
unsigned int tcp_port = 3030;
unsigned int comm_pdp_index = 3;    // The range is 1 ~ 16
unsigned int comm_socket_index = 3; // The range is 0 ~ 11
Socket_Type_t socket = TCP_SEVER;

_5G_NB_IoT_TCPIP _5GNBIoT(ATSerial, DSerial);

void setup()
{
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
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, "", "", apn_error)) {
    DSerial.println(apn_error);
  }
  DSerial.println(apn_error);

  while (!_5GNBIoT.OpenSocketService(comm_pdp_index, comm_socket_index, socket, tcps_ip, 0, tcp_port, BUFFER_MODE)) {
    DSerial.println("\r\nOpen Socket Service Fail!");
  }
  DSerial.println("\r\nOpen Socket Service Success!");
}

void loop()
{
  char m_event[32];
  char recv_data[128];
  unsigned int index;
  char m_ip[16];
  unsigned int m_port;
  char *sta_buf, *med_buf, *end_buf;
  Socket_Event_t ret = _5GNBIoT.WaitCheckSocketEvent(m_event, 2);
  switch (ret)
  {
    case SOCKET_CLOSE_EVENT:
      index = atoi(m_event);
      if (_5GNBIoT.CloseSocketService(index)) {
        DSerial.println("\r\nClose Socket Success!");
      }
      break;
    case SOCKET_RECV_DATA_EVENT:
      index = atoi(m_event);
      if (_5GNBIoT.SocketRecvData(index, 128, socket, recv_data)) {
        DSerial.println("\r\nSocket Recv Data Success!");
        DSerial.println("");
        DSerial.println(recv_data);
        DSerial.println("");
      }
      break;
    case SOCKET_PDP_DEACTIVATION_EVENT:
      index = atoi(m_event);
      if (_5GNBIoT.DeactivateDevAPN(index)) {
        DSerial.println("\r\nPlease reconfigure APN!");
      }
      break;
    case SOCKET_INCOMING_CONNECTION_EVENT:
      DSerial.println(m_event);
      sta_buf = strchr(m_event, ',');
      *sta_buf = '\0';
      index = atoi(m_event);
      med_buf = strstr(sta_buf + 1, ",\"");
      end_buf = strstr(sta_buf + 1, "\",");
      *end_buf = '\0';
      strcpy(m_ip, med_buf + 2);
      m_port = atoi(end_buf + 2);
      if (_5GNBIoT.SocketSendData(index, socket, "Hello", m_ip, m_port)) {
        DSerial.println("\r\nSocket Send Data successfully");
      }
      break;
    case SOCKET_CONNECTION_FULL_EVENT:
      if (_5GNBIoT.CloseSocketService(comm_socket_index)) {
        DSerial.println("\r\nClose Socket function successfully");
      }
      break;
    default:
      break;
  }
}
