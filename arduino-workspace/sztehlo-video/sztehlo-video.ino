#include <avr/pgmspace.h>
#include "sztehlo-video.h"
#include "bitmaps.h"

void resetVideo() {
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  delay(10);
  digitalWrite(11, HIGH);
  delay(50);
}


void setup() {
  // reset-eljük a videót, erre a gameboy-nál nem lesz lehetőség.
  // de ha Arduino-t használunk, akkor van hozzá elég kimenetünk és fontos is.
  resetVideo();

  videoInit();
}


void szovegKiiras() {
  videoPrintText(15, 1, "Hello!");
  delay(1000);
  videoPrintText(64, 3, "ez egy proba szoveg...");
  delay(1500);
  videoPrintText(15, 6, "szam:");
  videoPrintNumber(64, 6, -5323);
}

void bitmapRajzolas() {
  videoDrawBitmap(32, 0, 64, 8, bitmap_sztehlo_logo);
  delay(1500);
  videoDrawBitmap(5, 1, 32, 4, bitmap_smiley_black_bg);
  delay(500);
  videoDrawBitmap(75, 0, 32, 4, bitmap_smiley_white_bg);
  delay(500);
  videoDrawBitmap(44, 2, 32, 4, bitmap_smiley_black_bg);
  delay(500);
  videoDrawBitmap(16, 1, 32, 4, bitmap_smiley_white_bg);
  delay(500);
  videoDrawBitmap(80, 3, 32, 4, bitmap_smiley_black_bg);
  delay(500);
  videoDrawBitmap(2, 3, 32, 4, bitmap_smiley_white_bg);
}

uint8_t kepkockaPixel(uint8_t x, uint8_t y, int16_t csigaOffsetX) {
  uint8_t color = videoPixelColor(x, y, bitmap_mountain, 12, 0, 99, 56, WHITE_PIXEL);
  color &= videoPixelColor(x, y, bitmap_csiga, csigaOffsetX, 42, 16, 16, WHITE_PIXEL);
  color &= videoPixelColor(x, y, bitmap_csiga_vissza, 120 - csigaOffsetX, 48, 16, 16, WHITE_PIXEL);
  return color;
}

void kepkockaRajzolas(int16_t csigaOffsetX) {
  for(uint8_t page=0; page<8; page++) {
    if((page+1) * 8 < 42) continue; // csak azok a "page-ek" lesznek frissítve, ahol a csigák mozognak
    videoStartPixels(page);
    for(uint8_t x=0; x<128; x++) {
      uint8_t byteToDraw = BLACK_SCREEN_BYTE;
      for(uint8_t b=0; b<8; b++) {
        byteToDraw |= kepkockaPixel(x, page*8 + b, csigaOffsetX) << b;
      }
      videoSendByte(byteToDraw);
    }
    videoEndPixels();
  }
}

void animacioRajzolas() {
  videoFillScreen(WHITE_SCREEN_BYTE);
  videoDrawBitmap(12, 0, 99, 7, bitmap_mountain); // háttér egyszer kirajzolva

  for(int16_t csigaOffsetX = -16; csigaOffsetX < 128; csigaOffsetX++) {
    kepkockaRajzolas(csigaOffsetX);
  }
}

void loop() {

  videoFillScreen(BLACK_SCREEN_BYTE);
  szovegKiiras();
  delay(1500);

  videoFillScreen(0b10101010);
  delay(1000);
  
  bitmapRajzolas();
  delay(1000);

  animacioRajzolas();
}
