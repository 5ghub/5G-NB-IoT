#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// The scrolling area must be a integral multiple of TEXT_HEIGHT
#define TEXT_HEIGHT 16 // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0 // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 16 // Number of lines in top fixed area (lines counted from top of screen)
#define YMAX 320 // Bottom of screen area

// The initial y coordinate of the top of the scrolling area
uint16_t yStart = TOP_FIXED_AREA;
// yArea must be a integral multiple of TEXT_HEIGHT
uint16_t yArea = YMAX - TOP_FIXED_AREA - BOT_FIXED_AREA;
// The initial y coordinate of the top of the bottom text line
uint16_t yDraw = YMAX - BOT_FIXED_AREA - TEXT_HEIGHT;

// Keep track of the drawing x coordinate
uint16_t xPos = 0;

// For the byte we read from the serial port
//byte data = 0;

// A few test variables used during debugging
boolean change_colour = 1;
boolean selected = 1;

// We have to blank the top line each time the display is scrolled, but this takes up to 13 milliseconds
// for a full width line, meanwhile the serial buffer may be filling... and overflowing
// We can speed up scrolling of short text lines by just blanking the character we drew
int blank[19]; // We keep all the strings pixel lengths to optimise the speed of the top line blanking

void TFTinit(String title) {
  tft.init();
  tft.setRotation(0); // Must be setRotation(0) for this sketch to work correctly
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.fillRect(0, 0, 240, 16, TFT_BLUE);
  tft.drawCentreString(title, 120, 0, 2);

  // Change colour for scrolling zone text
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Setup scroll area
  setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

  // Zero the array
  for (byte i = 0; i < 18; i++) blank[i] = 0;
}

void TFTprint(String mess) {
  char data;
  for (int i = 0; i < mess.length(); i++) {
    data = mess.charAt(i);
    // If it is a CR or we are near end of line then scroll one line
    if (data == '\r' || xPos > 231) {
      xPos = 0;
      yDraw = scroll_line(); // It can take 13ms to scroll and blank 16 pixel lines
    }
    if (data > 31 && data < 128) {
      xPos += tft.drawChar(data, xPos, yDraw, 2);
      blank[(18 + (yStart - TOP_FIXED_AREA) / TEXT_HEIGHT) % 19] = xPos; // Keep a record of line lengths
    }
    //change_colour = 1; // Line to indicate buffer is being emptied
  }
}

// ##############################################################################################
// Call this function to scroll the display one text line
// ##############################################################################################
int scroll_line() {
  int yTemp = yStart; // Store the old yStart, this is where we draw the next line
  // Use the record of line lengths to optimise the rectangle size we need to erase the top line
  tft.fillRect(0, yStart, blank[(yStart - TOP_FIXED_AREA) / TEXT_HEIGHT], TEXT_HEIGHT, TFT_BLACK);

  // Change the top of the scroll area
  yStart += TEXT_HEIGHT;
  // The value must wrap around as the screen memory is a circular buffer
  if (yStart >= YMAX - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA + (yStart - YMAX + BOT_FIXED_AREA);
  // Now we can scroll the display
  scrollAddress(yStart);
  return  yTemp;
}

// ##############################################################################################
// Setup a portion of the screen for vertical scrolling
// ##############################################################################################
// We are using a hardware feature of the display, so we can only scroll in portrait orientation
void setupScrollArea(uint16_t tfa, uint16_t bfa) {
  tft.writecommand(ST7789_VSCRDEF); // Vertical scroll definition
  tft.writedata(tfa >> 8);           // Top Fixed Area line count
  tft.writedata(tfa);
  tft.writedata((YMAX - tfa - bfa) >> 8); // Vertical Scrolling Area line count
  tft.writedata(YMAX - tfa - bfa);
  tft.writedata(bfa >> 8);           // Bottom Fixed Area line count
  tft.writedata(bfa);
}

// ##############################################################################################
// Setup the vertical scrolling start address pointer
// ##############################################################################################
void scrollAddress(uint16_t vsp) {
  tft.writecommand(0x37); // Vertical scrolling pointer
  tft.writedata(vsp >> 8);
  tft.writedata(vsp);
}
