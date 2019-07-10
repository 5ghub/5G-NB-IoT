# 5G-NB-IoT Kit

The NB-IoT kit is a cellular and GPS kit that can be used for the new 5G wireless technology. The kit includes a hardware board, LTE&GPS antenna, and USB cables. 

The hardware board is a powerful board that features a microcontroller and a wireless modem. The microcontroller is an Atmel’s SAMD21G18A MCU which features a 32-bit ARM Cortex® M0+ core. The wireless modem is Quectel BG96 which is an embedded IoT (LTE Cat-M1, LTE Cat-NB1 and EGPRS) wireless communication modem. BG96 wireless modem provides a maximum data rate of 375Kbps downlink and 375Kbps uplink. It features ultra-low power consumption, provides data connectivity on LTE-TDD/LTE-FDD/GPRS/EDGE networks, and supports half-duplex operation in LTE networks. It also provides GNSS to meet customers’ specific application demands. 

The board provides rich sets of Internet protocols (TCP/UDP/PPP/SSL/TLS/FTP/HTTP/PING/ MQTT/NITZ/SMS), GNSS technology (GLONASS, BeiDou/Compass, Galileo, QZSS), industry-standard interfaces (USB/UART/I2C/SPI/GPIO/ADC/DAC/PWM/JTAG/Interrupts/LEDs/Status Indicators), Nano USIM slot for a nano SIM card, and USB ports and DC power jack for external power supply. 

The board also supports two modes of operation; the first mode where the MCU controls the wireless modem and second mode where the modem is controlled directly without the need of the MCU. 
The board is ultra-compact size (2.8-inch X 1.6-inch) and fits into many embedded devices and applications.

The board is also compatible with Arduino and Arduino software (IDE). Many Arduino sketches and examples are provided with the kit and additional sketches can be developed and uploaded to the board.

The board supports Windows 7/8/8.1/10, Linux and Android.

The NB-IoT board is a rich board that can be used for a variety of smart and 5G applications. It enables applications such as wireless POS, smart metering, tracking, smart transportation, smart buildings, smart city, and smart homes, navigation, location, and tracking. 


# Software and Tools Included with the Board
-ArduinoSketches: Contains different Arduino sketches to use with the board.

-5G-NB-IoT_Arduino.zip: Contains the library (used with Arduino IDE).

-5G-NB-IoT.zip: Contains the board manager (used with Arduino IDE).

-Document: Contains user manual and how to use the kit and the board.

-Driver: Contains Windows driver for the BG96 chipset and the prolific USB-to-serial cable.

-Tools: Different tools inclduing Quectel QNavigator that can be used with the BG96 wireless modem.

# How to Use Arduino IDE with the Board

1.	Install Arduino IDE for Windows from the following web site:
https://www.arduino.cc/en/Main/Software

2.	Launch Arduino IDE and choose File->Preferences. In the Additional Boards Manager URLs, insert the following URL:
https://raw.githubusercontent.com/5ghub/5G-NB-IoT/master/package_5G-NB-IoT_index.json

3.	In Arduino IDE, choose Tools->Boards Manager and install “5G-NB-IoT SAMD Boards”.

4.	Choose “5G NB-IoT (Native USB Port)”
 
5.	You are ready now to use the Arduino IDE and write the first sketch.
