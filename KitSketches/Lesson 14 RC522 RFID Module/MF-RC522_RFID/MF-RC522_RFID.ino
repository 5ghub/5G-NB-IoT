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

/*
   --------------------------------------------------------------------------------------------------------------------
   Example to change UID of changeable MIFARE card.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   This sample shows how to set the UID on a UID changeable MIFARE card.
   NOTE: for more informations read the README.rst

   @author Tom Clement
   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522     5G-NB-IoT      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD                 Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin         Pin            Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST         PA6            9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)     SS             10            53        D10        10               10
   SPI MOSI    MOSI        MOSI           11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO        MISO           12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK         SCK            13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <board.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   PA6
#define SS_PIN    SS

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//#define DSerial Serial
#define DSerial SerialUSB

/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;

void setup() 
{
  DSerial.begin(9600);  // Initialize serial communications with the PC
  while (!DSerial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(1000);
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  DSerial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  }
}

// Setting the UID can be as simple as this:
//void loop() {
//  byte newUid[] = NEW_UID;
//  if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
//    Serial.println("Wrote new UID to card.");
//  }
//  delay(1000);
//}

// But of course this is a more proper approach
void loop() 
{
  // Look for new cards, and select one if present
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) 
  {
    delay(50);
    return;
  }

  // Now a card is selected. The UID and SAK is in mfrc522.uid.

  // Dump UID
  DSerial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    DSerial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    DSerial.print(mfrc522.uid.uidByte[i], HEX);
  }
  DSerial.println();

  // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  DSerial.print(F("PICC type: "));
  DSerial.print(mfrc522.PICC_GetTypeName(piccType));
  DSerial.print(F(" (SAK "));
  DSerial.print(mfrc522.uid.sak);
  DSerial.print(")\r\n");
  DSerial.println();
  //  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI
  //    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
  //    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
  //    Serial.println(F("This sample only works with MIFARE Classic cards."));
  //    return;
  //  }

  while (true) {}
  
  /*
    // Set new UID
    byte newUid[] = NEW_UID;
    if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
      DSerial.println(F("Wrote new UID to card."));
    }

    // Halt PICC and re-select it so DumpToSerial doesn't get confused
    mfrc522.PICC_HaltA();
    if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
      return;
    }

    // Dump the new memory contents
    DSerial.println(F("New UID and contents:"));
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      DSerial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      DSerial.print(mfrc522.uid.uidByte[i], HEX);
    }
    DSerial.println();


    delay(2000);*/
}
