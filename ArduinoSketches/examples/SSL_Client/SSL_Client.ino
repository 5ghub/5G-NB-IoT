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

#include "5G-NB-IoT_SSL.h"

//#define DSerial Serial
#define DSerial SerialUSB
#define ATSerial Serial1

#define USE_CERT
#ifdef USE_CERT
char pem_CA[] = "-----BEGIN CERTIFICATE-----\n\
MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzElMCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp\n\
U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQwNjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE\n\
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3\n\
DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN\n\
+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa\n\
K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G\n\
A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fRzt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0\n\
YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w\n\
DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D\n\
eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJlxy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp\n\
VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEYWQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=\n\
-----END CERTIFICATE-----\n";

// "The certificate for this thing"
char pem_cert[] = "-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUWermTFtqBipvIdo9fw0sqlQmiUEwDQYJKoZIhvcNAQELBQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MTEyNTAwMjE1MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALalCvpF6hUcpAhQo+OAsUG28pCsbCpRpqcbw75MI22dD0V26uVH3f90kDUZrrRrfYKiUobc/QBrUi+wL27Z\n\
0+AG+4szfNT1HxTfG5cCp9AGQq/fFViD/d2vFDNwK7hfm3AV0ouIJaHg4qehrCwobvUu0GYoVU72g6wCfUQ9PMyscUBuU/BYa9P9NgUHM2VI46OEqGd+fhKGfLua6wiI\n\
TL+0uMZ102g+ixWnCVbixZg17DbXix2KyzofSj9Zcl2J2dqEyeLlnGI4pJ72vIwsFqm1n3vMSBdN33uDRALH1YTNd2+hxOWbtDKv5L9/dSCayXA9UmK9NUA4Tx1kmc24\n\
q98CAwEAAaNgMF4wHwYDVR0jBBgwFoAUEYvhKrZ37Neoj+bx7Y4QrtrNLDYwHQYDVR0OBBYEFE+2WYlcnvcn/baK2GWrsOOvhqqpMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCe8fojFAgQQWnf9bShNtxLzXLZSOLS6Xar4uSn2LK7UuORw63S6CovKcGSuTkv/Zea64W8xPI71nnWEQMqWwEPJSLP\n\
hxDelOQFMsrdd03HUzhaHxB69g3KqGoK7Rlx4u30w1sf2gYeyz5ajUx7OovpJm+VKCNdotnYmA3LLiG16u6A+0jt3N6aLC1cBYLMumo/0zgJr5jeRiv5jtYlh59SukjQ\n\
h2Yzg7LIgT9uOUzIjZ5hEF4mUT4jYPQz5MO4nTMQtWcL2RBKPgEMHLo6G4MM50xIfV8zUHJ2oh9RRkug6Y3kWdvC3HcO3onLLM9MCtOjOBDUnOsULvhCod4fFop6\n\
-----END CERTIFICATE-----\n";

// "The private key of the thing". The public key is not needed here.
char pem_pkey[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAtqUK+kXqFRykCFCj44CxQbbykKxsKlGmpxvDvkwjbZ0PRXbq5Ufd/3SQNRmutGt9gqJShtz9AGtSL7AvbtnT4Ab7izN81PUfFN8blwKn0AZCr98V\n\
WIP93a8UM3AruF+bcBXSi4gloeDip6GsLChu9S7QZihVTvaDrAJ9RD08zKxxQG5T8Fhr0/02BQczZUjjo4SoZ35+EoZ8u5rrCIhMv7S4xnXTaD6LFacJVuLFmDXsNteL\n\
HYrLOh9KP1lyXYnZ2oTJ4uWcYjiknva8jCwWqbWfe8xIF03fe4NEAsfVhM13b6HE5Zu0Mq/kv391IJrJcD1SYr01QDhPHWSZzbir3wIDAQABAoIBAF+6HwfPjwGjskF9\n\
anh54e2sYnP7OvHvO6aIQc7CfysbT3679h9XzH3wbQi3vj5nvC+0vgZbWutWVIwUDrhNyOO5JSvvP3UqSDEbQo2iYawBLACbrsdm3q7K7Y6MMw3+ZiyX83MYV4kBoinz\n\
7ZZzVryA3c89QoERaS/FWjxi0DQBJutsO0daqpA5sTHlih1i7ULm4jFhP1y6zf/Q/TqX+xU6/xkV/qOgkV8sMUsgWr0W3sR+dkuj+gRG2UPDgftOMOOUatYz8bXxBhRt\n\
RTGWXG9stjyLzf+LrLUOLJTPPdbh10fVcQrFZjmDT/5944eLpIXIBHUyyJknIgiP4JHjLdECgYEA2QbHyejlyDI/BeX3I7oC9D3f3JkQReUbQv0HeEGDn+ABolZjMm+p\n\
ukjrRloGCfPk149W6JHN/C5Wk5HyK4Z7+HKFnIDupu8IyZdkrx1LCcKaS1Yl0/uMVPAjGinJ2VY05XUvsZ6fQEKIDI5pHcPDUCEtVHeGuqRNcipHyE6eHRkCgYEA13Gm\n\
jIGtCnIMwshjKyQZdXQtAyKC0FI/+IyYyrAGQ+ITkhDn5z9QudEgX1rYlxUAC9ed46Gf7IFoKDfGncHdQ4K2k0vtlWuI5x1lwVUgCs9iDzDaBCQH+/UWrjQ3kaJhGr3b\n\
A2DvEy9h0wkptCm+RmrOPZKPA3mKBqp2V3qtt7cCgYAdKUITKU4m1kmNm1bX4BihhObdl1uOaLh/CczFJdhv1QOjMoRltIdT1pR2gYYqNDFdnY8J9P1+kyNi9MtbediS\n\
i6DUZKasm5XMsKEAvqjkiNd+o/a1QymaCCJPU6ohn+m/oz1jWnL/iXEKbuBE9Zb631pd7SmXTphxbKj0i0vHkQKBgQDFcuxf82zCmPnm/j4cyK3ofjvOlq9q+yaTxU82\n\
25IyXpzvDj1rKxTx556SpbDA7kcnk0mp2HO4DnWF/nAyHv2JJN/tKPYqLe6/WarrP/fo0F2fbHcyuk7y9JD/wO14tGfoWT3C3/DcFV3DSq+E5Yj0AmjX91zIo3gDWfCx\n\
hcZ2CwKBgCFUIsL9o4IQOOpCdvw9HDxOu3EOuOuQJfDve4gddCBYGxzHvBc1BJXV7Ogvk2vpebfw1jv/F+CMk+b6kr/zzgb7lrcaCPvc3W7mb8MkuZ4Jw2xHAHRG2zuo\n\
nsfodqU1/jMSKuy477qsnneFRKiHTBymwlQ4dF46SnrSZQEl7lLA-----END RSA PRIVATE KEY-----\n";
#if 0
/* Fill your certificate.pem.root CA wiht LINE ENDING */
char pem_CA[] = {"-----BEGIN CERTIFICATE-----\n\
MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n\
yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n\
ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n\
U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n\
ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n\
aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n\
MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n\
ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n\
biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n\
U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n\
aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n\
nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n\
t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n\
SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n\
BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n\
rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n\
NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n\
BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n\
BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n\
aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n\
MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n\
p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n\
5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n\
WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n\
4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n\
hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n\
-----END CERTIFICATE-----\n"
                };

/* Fill your certificate.pem.crt wiht LINE ENDING */
char pem_cert[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAPO/jH+GepSZxWRtbVnZ5WEjmF1MMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xODAxMjIwNjQ2\n\
MjdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQD0JqFo3Dg5wUj2PBAm\n\
tvCvJLw7E1Nn/J5JrEP5g7M7YCVIv2WCmAXAR5YrPct7h0rOD7h9JG4K93koDK03\n\
0u/WsqddSMedGObRqR7MSCdeAtBGKY9vrThCAQiOvl1LlmCZdyaEv5ukGUHYAGQf\n\
DCfjDvg6K4t9LTocyE2SHISv7zE1E/tHDmjZ7ups7ewE7h+35Ce9bL5tFKun2oEH\n\
4COwJFrHj8RXObXPQYFEQ1x7YbshYkjs4HXT/cKmGXGz/2kAOSPejDFdxC4Larxw\n\
Eoz6xepjjiOXOZ5w7w1M/4/LnwJl/tbGLEU+azU47n4bT3GtscbX+ddisRJ65gN2\n\
7wPHAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDZV+0LrRJWxMz0PBDhwYV57/i65MB0G\n\
A1UdDgQWBBTBStT4wV7A6mkr3o6wLjyhhsmjvDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAxXJMJvQBtbgv+wKisaVXtgph\n\
cuf+jtk5/N04jq7oZDkeH4+jqHmYN+mh8xS7UbnQGRl6B5ftcXIYM/Uxjb46Jhtg\n\
HnQODB6cdbb780nxT5YOQA3EcZBndhe1hBC44qICOPCp3RejGmV43EYgLzJh/wtp\n\
8yNvIEXi4t+rDP+JI7RgrgF2QlYsDYAQlr9a5JFzlLESzFanpN6PNu4/xVIO0LgI\n\
OnccrxQc5ZLcHKkze7OhjGFbmuXI9ljg/FDIXInveTV4OSQa14IfijLiZ9j5RV0o\n\
j0ansu6HbBIlwzq8ltWXfOqpAGm+YW2ukd5kKguvQHU24Tb1k2gXhHvv3mNRBA==\n\
-----END CERTIFICATE-----\n"
                  };

/* Fill your private.pem.key wiht LINE ENDING */
char pem_pkey[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEA9CahaNw4OcFI9jwQJrbwryS8OxNTZ/yeSaxD+YOzO2AlSL9l\n\
gpgFwEeWKz3Le4dKzg+4fSRuCvd5KAytN9Lv1rKnXUjHnRjm0akezEgnXgLQRimP\n\
b604QgEIjr5dS5ZgmXcmhL+bpBlB2ABkHwwn4w74OiuLfS06HMhNkhyEr+8xNRP7\n\
Rw5o2e7qbO3sBO4ft+QnvWy+bRSrp9qBB+AjsCRax4/EVzm1z0GBRENce2G7IWJI\n\
7OB10/3Cphlxs/9pADkj3owxXcQuC2q8cBKM+sXqY44jlzmecO8NTP+Py58CZf7W\n\
xixFPms1OO5+G09xrbHG1/nXYrESeuYDdu8DxwIDAQABAoIBAFWjp0Js3JwFnF4i\n\
u1eZG9uTeyMNqFf1WGBnOICxHZFaLDnUSPsB8CJ9+cK5LkLqNecJxKDAwoi+PZU6\n\
vRezucUJbYNfIAWO0ogAVnk5ZfWn7v5tXY7Cp9UfH61SQM9etAugq7Ysr59j7KCj\n\
onrxfJw8h/1/gLIrxxKJgAwcTL6Y7LoN2oAv7qKCYtkYHVfCPeDLhjbBjuAKKP7Q\n\
t8zzVgc5Gvfe76A12oN//xaZlLI73OxMPtW5MfW718BqFcZLJzScfaY2//YiLN0F\n\
Iiccj+fZDcXPEvRaUludLZNPg2c7LO23Efn5HK66A8KJysOanfh+VBL/qdpFMCGY\n\
BXUwvOECgYEA+u084LcL0WX0TVKue+D0cBuJ4irLd3xxFOdEsVwRaeaFUQ5W9dDP\n\
PMCfgxE3a7gUPCKy7gnWTNvahudK0SqrbguTsYI4jNGpDRPkDI445KD85GAAsFBx\n\
3nbEeG1EB3DAbmIJr1Pfpi+6imXom61Po0oftBBnmyi3G1c74VQF/BsCgYEA+RZS\n\
cVA9OdY722HX2AVVQJIi3mz99QEFIX/A0AJHaNIjlFYtXUG2CQ5WanWW4lUXmyU/\n\
d7rngfACp4qUeLWBn0jiW+ylLi28ZIzzyXLE9ISgeDIA5wK+8cHHDgfmZjPz9T9K\n\
o6u6Wb9H6hJwELJFdP9AgVNPzyiou69x7Zq2OcUCgYEAzoNLdCISHt9LBgxDP/7v\n\
DgkMHNBdrlQDeVGw7ks4pbCsDf7AQQT+WiCp5yPXOddBCBECs/fyoETc/+QBZk44\n\
0pPClPf4dKTk/bdeG5s9ZaS9HXOJfZJndNDId9htgbYhGXekQuDDkzF61AZ1PxKP\n\
FXOezgImUsNal6zewl71ofsCgYAfs6SYgWLZepexqzlCaCNajX7cmjiUNyYFBnJu\n\
xut5vs0Ccxlg9h0J+AYtlHu4oKXZfBsgzOvsELT1x+BvoII32Jneoza+stl4Xqwk\n\
ZnQEvuCBi7OrsyDEOipwcbb0KQaSnxSL0i5sf7jSZLhYl6fZZSc7jnPo5L7c7E5u\n\
tDMfFQKBgQDIhY+A7xWXobtOQyaquT2IdyR3JSFfJwahLUc65nV0s582l8le+xuy\n\
ulCy291puJc1WkGr+vswMD/B2kJv61TdoxxBa2lBdwd+/RZlmuqp9iO9MHsi7oTV\n\
Y51odhXBpRYjDNieOsKEsX/xPUvUtNLJDeurI5KNr+e5dHADwvo2Jg==\n\
-----END RSA PRIVATE KEY-----\n"
                  };
#endif
#endif

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

char ssl_ip[] = "a3kjt69iibf2h0-ats.iot.us-west-2.amazonaws.com";
char ssl_cmd[] = "GET / HTTP/1.1\r\n\r\n";
int ssl_port = 8883;
unsigned int comm_pdp_index = 1;  // The range is 1 ~ 16
unsigned int comm_socket_index = 1;  // The range is 0 ~ 11
unsigned int comm_ssl_index = 1;  // The range if 0 ~ 5

_5G_NB_IoT_SSL _5GNBIoT(ATSerial, DSerial);

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

  _5GNBIoT.DeleteFiles("*");
  char filelist[64];
  /*if (_5GNBIoT.GetFilesList(filelist))
    {
    DSerial.println(filelist);
    }*/

#ifdef USE_CERT
  char ssl_error[128];
  while (!_5GNBIoT.InitSSL(comm_ssl_index, pem_CA, pem_cert, pem_pkey, ssl_error)) {
    DSerial.println(ssl_error);
  }
  DSerial.println(ssl_error);
#else
  char ssl_error[128];
  while (!_5GNBIoT.InitSSL(comm_ssl_index, "", "", "", ssl_error)) {
    DSerial.println(ssl_error);
  }
  DSerial.println(ssl_error);
#endif

  char apn_error[64];
  while (!_5GNBIoT.InitAPN(comm_pdp_index, APN, "", "", apn_error)) {
    DSerial.println(apn_error);
  }
  DSerial.println(apn_error);

  while (!_5GNBIoT.OpenSSLSocket(comm_pdp_index, comm_ssl_index, comm_socket_index, ssl_ip, ssl_port, BUFFER_MODE)) {
    DSerial.println("\r\nConnect the SSL Sever Fail!");
    char ssl_socket_error[64];
    int e_code;
    if (_5GNBIoT.returnErrorCode(e_code)) {
      DSerial.print("\r\nERROR CODE: ");
      DSerial.println(e_code);
      DSerial.println("Please check the documentation for error details.");
      while (1);
    } else if (_5GNBIoT.QueryLastErrorCode(ssl_socket_error)) {
      DSerial.println(ssl_socket_error);
    }
    delay(5000);
  }
  DSerial.println("\r\nConnect the SSL Sever Success!");

  if (_5GNBIoT.SSLSocketSendData(comm_socket_index, ssl_cmd)) {
    DSerial.println("\r\nSSL Socket Send Data Success!");
  }
}

void loop() {
  char ssl_event[16];
  unsigned int index;
  char recv_data[128];
  SSL_Socket_Event_t ret = _5GNBIoT.WaitCheckSSLSocketEvent(ssl_event, 2);
  switch (ret)
  {
    case SSL_SOCKET_CLOSE_EVENT:
      index = atoi(ssl_event);
      if (_5GNBIoT.CloseSSLSocket(index)) {
        DSerial.println("\r\nClose SSL Socket Success!");
      }
      break;
    case SSL_SOCKET_RECV_EVENT:
      index = atoi(ssl_event);
      if (_5GNBIoT.SSLSocketRecvData(index, 128, recv_data)) {
        DSerial.println("\r\nSSL Socket Recv Data Success!");
        DSerial.println("");
        DSerial.println(recv_data);
        DSerial.println("");
      }
      break;
    default:
      break;
  }
}
