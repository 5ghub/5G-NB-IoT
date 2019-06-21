# 5G-NB-IoT-Hubber

The Hubber board is a powerful board that features a microcontroller and wireless chipset. The microcontroller is an Atmel’s SAMD21 MCU which features a 32-bit ARM Cortex® M0+ core. The wireless module is BG96 which is an embedded IoT (LTE Cat M1, LTE Cat NB1 and EGPRS) wireless communication module. BG96 provides data connectivity on LTE-TDD/LTE-FDD/GPRS/EDGE networks and supports half-duplex operation in LTE networks. It also provides GNSS and voice functionality to meet customers’ specific application demands
The hubber module is a powerful board that can be used for the latest 5G wireless technology and enables a variety of smart and 5G applications for devices, and acts as a great educational tool for learning about 5G and 32-bit application development.  

The board comes with several examples to program and operate the SAMD21 and BG96 wireless IoT module. The examples are written using Arduino IDE. 

# Software and Tools Included with the Board
-ArduinoSketches: Contains different Arduino sketches to use with the board.

-5G-NB-IoT_Arduino.zip: Contains the library (used with Arduino IDE).

-5G-NB-IoT.zip: Contains the board manager (used with Arduino IDE).

-ArduinoCore-samd-master: Bootloader of the board.

-Document: Contains user manual and how to use the board.

-Driver: Contains Windows driver for the BG96 chipset and the prolific USB-to-serial cable.

# How to Use Arduino IDE with the Board

1.	Install Arduino IDE for Windows from the following web site:
https://www.arduino.cc/en/Main/Software

2.	Launch Arduino IDE and choose File->Preferences. In the Additional Boards Manager URLs, insert the following URL:
https://raw.githubusercontent.com/5ghub/5G-NB-IoT-Hubber/master/package_5G-NB-IoT_index.json

3.	In Arduino IDE, choose Tools->Boards Manager and install “5G-NB-IoT SAMD Boards”.

4.	Choose “5G NB-IoT (Native USB Port)”
 
5.	You are ready now to use the Arduino IDE and write the first sketch.
