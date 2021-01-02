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
// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

#include "Logo.h"             // Sketch tab header for xbm images

#include <TFT_eSPI.h>        // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();   // Invoke library

void setup()
{
  tft.begin();               // Initialise the display
  tft.fillScreen(TFT_BLACK); // Black screen fill

  tft.drawXBitmap(0, 64, logo, logoWidth, logoHeight, TFT_WHITE);
}

void loop()
{

/*  // Example 1
  // =========
  // Random x and y coordinates
  int x = random(tft.width()  - logoWidth);
  int y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //              x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE);

  delay(500);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK);


  // Example 2
  // =========
  // New random x and y coordinates
  x = random(tft.width()  - logoWidth);
  y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE, TFT_RED);

  delay(500);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK, TFT_BLACK);
*/
}
