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

/*
*/

#include <board.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();

#define Key_LED PIN_A1

/*
  I2C Command Summary
  Setup board (5x10 keypad)
  write to Register, value
            0x1D, 0x1F
            0x1E, 0xFF
            0x1F, 0x03
            0x01, 0xB9
  To get keys
  read 1 byte from 0x04
  To clear interrupt flag
  write to register,value
            0x02, 0x0F

*/
// keyboard Constants
#define KEYBOARD_ADDRESS 0x34
#define KEYBOARD_INT_PIN 14
#define KEYBOARD_STATE_REGISTER 0x04
#define KEYBOARD_FLAG_REGISTER 0x02
#define KEYBOARD_KEY_DOWN 0x80
#define KEYBOARD_KEY_MASK 0x7F

// keycode values for control keys
#define KEYBOARD_CODE_SHIFT 31
#define KEYBOARD_CODE_ALT 41
#define KEYBOARD_CODE_BACKSPACE 30
#define KEYBOARD_CODE_RETURN 40
#define KEYBOARD_CODE_SEND 46
#define KEYBOARD_CODE_LEFT_ARROW 47
#define KEYBOARD_CODE_UP_ARROW 39
#define KEYBOARD_CODE_DOWN_ARROW 48
#define KEYBOARD_CODE_RIGHT_ARROW 49
#define KEYBOARD_CODE_LEFT_SPACE 43
#define KEYBOARD_CODE_RIGHT_SPACE 44
#define KEYBOARD_CODE_UNUSED 42

// configuration commands (configures the 5x5 matrix as inputs with pullups, interrupts ...
// For more information look at the code in the repo linked above
const uint8_t KEYBOARD_CONFIG_COMMANDS[] = {0x1D, 0x1F, 0x1E, 0x1F, 0x1F, 0x00, 0x01, 0xB9, 0x02, 0x0F};

// Labels with placeholder characters for the control buttons
const String KEYBOARD_LABELS[] = {
  "CALL", "DOWN", "UP", "ON/OFF", "SELECT",
  "1", "2", "3", "F1", "LEFT",
  "4", "5", "6", "F2", "OK",
  "7", "8", "9", "F3", "RIGHT",
  "*", "0", "#", "F4", "BACK"
};

// Define our functions before use
void keyboard_configure(void);
void keyboard_clearFlag(void);
uint8_t keyboard_getState(void);

// keyInt allows our event to ultimately be handled in the loop
volatile bool keyInt = false;

void keyboard_configure(void) {
  for (uint8_t i = 0; i < 9; i += 2) {
    Wire.beginTransmission(KEYBOARD_ADDRESS);
    Wire.write(KEYBOARD_CONFIG_COMMANDS[i]);
    Wire.write(KEYBOARD_CONFIG_COMMANDS[i + 1]);
    Wire.endTransmission();
  }
}

uint8_t keyboard_getState(void) {
  uint8_t key;
  Wire.beginTransmission(KEYBOARD_ADDRESS);
  Wire.write(KEYBOARD_STATE_REGISTER);
  Wire.endTransmission();
  Wire.requestFrom(KEYBOARD_ADDRESS, 1);    // request 1 bytes from slave device 0x34
  while (Wire.available()) { // slave may send less than requested
    key = Wire.read(); // receive a byte as character
  }
  return key;
}

void keyboard_clearFlag(void) {
  Wire.beginTransmission(KEYBOARD_ADDRESS);
  Wire.write(KEYBOARD_FLAG_REGISTER);
  Wire.write(0x0F);
  Wire.endTransmission();
}

void keyboard_ISR(void) {
  keyInt = true;
}

void setup() {
  pinMode(Key_LED, OUTPUT);
  digitalWrite(Key_LED, HIGH);
  
  Wire.begin();        // join i2c bus (address optional for master)

  keyboard_configure();
  pinMode(KEYBOARD_INT_PIN, INPUT); // There is an external pullup on the pin

  // Set up a falling edge pin interrupt on pin 2
  attachInterrupt(digitalPinToInterrupt(KEYBOARD_INT_PIN), keyboard_ISR, FALLING);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_NAVY);
  tft.setTextFont(1);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextSize(3);
}

void loop() {
  if (keyInt) {
    uint8_t key_code = keyboard_getState(); // Get first keycode from FIFO
    if (key_code & KEYBOARD_KEY_DOWN) {
      // This is where we can write to a screen or handle control keys
      key_code = (key_code & KEYBOARD_KEY_MASK) - 1;
      keyInt = false;
      keyboard_clearFlag();

      tft.fillScreen(TFT_NAVY);
      tft.setCursor(100, 120, 2);
      tft.println(KEYBOARD_LABELS[(key_code / 10) * 5 + key_code % 10]);
    }
  }

  delay(10);
}
