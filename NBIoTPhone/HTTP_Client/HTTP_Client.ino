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

#define ATSerial Serial1
#define UART_DEBUG

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

char http_url[] = "http://httpbin.org/ip";
unsigned int comm_pdp_index = 1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  // The range is 1 ~ 16
HTTP_Body_Data_Type_t  http_type = APPLICATION_X_WWW_FORM_URLENCODED;

_5G_NB_IoT_HTTP _5GNBIoT(ATSerial, ATSerial);

void setup() {
  ATSerial.begin(115200);
  while (ATSerial.read() >= 0);
  delay(1000);

  TFTinit("HTTP Client test");

  _5GNBIoT.InitModule();
  TFTprint("\r\n_5GNBIoT.InitModule() OK!");

  _5GNBIoT.SetDevCommandEcho(false);
  delay(100);

  char inf[64];
  if (_5GNBIoT.GetDevInformation(inf)) {
    TFTprint(inf);
  }

  //0 Automatic, 1 GSM only, 3 LTE only
  _5GNBIoT.ScanmodeConfig(1);

  char apn_error[64];
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, LOGIN, PASSWORD, apn_error)) {
    TFTprint(apn_error);
    delay(1000);
  }
  TFTprint(apn_error);

  while (!_5GNBIoT.SetHTTPConfigParameters(comm_pdp_index, false, false, http_type)) {
    TFTprint("\r\nConfig the HTTP Parameter Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
      while (1) {}
    }
  }
  TFTprint("\r\nConfig the HTTP Parameter Success!");

  while (!_5GNBIoT.HTTPURL(http_url, READ_MODE)) {
    TFTprint("\r\Set the HTTP URL Fail!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
      while (1) {}
    }
  }
  TFTprint("\r\nSet the HTTP URL Success!");
}

void loop() {
  char recv_data[128];
  if (!_5GNBIoT.HTTPGET(80)) {
    TFTprint("\r\nHTTP GET Success!");
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      TFTprint("\r\nERROR CODE: ");
      TFTprint(String(e_code));
      TFTprint("Please check the documentation for error details.");
      while (1) {}
    }
  }
  TFTprint("\r\nHTTP GET Success!");

  if (_5GNBIoT.HTTPRead(recv_data, 80)) {
    TFTprint("\r\nHTTP Read Success!");
    TFTprint("");
    TFTprint(recv_data);
    TFTprint("");
  }

  while (1) {}
}
