#include "fonts.h"

#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)

#define BLACK_PIXEL 0x00
#define WHITE_PIXEL 0x01
#define BLACK_SCREEN_BYTE 0x00
#define WHITE_SCREEN_BYTE 0xff

// Defines for OLED output
#define SSD1306_SCL   PORTB2  // SCL, az Arduino Uno-n ez a 10-es digital pin
#define SSD1306_SDA   PORTB0  // SDA, az Arduino Uno-n ez a 8-as digital pin
#define SSD1306_SA    0x78  // Slave address (first 7 bits)


void videoSendByte(uint8_t byte_){
  uint8_t i;
  for(i=0; i<8; i++)
  {
    if((byte_ << i) & 0x80)
      DIGITAL_WRITE_HIGH(SSD1306_SDA);
    else
      DIGITAL_WRITE_LOW(SSD1306_SDA);
    
    DIGITAL_WRITE_HIGH(SSD1306_SCL);
    DIGITAL_WRITE_LOW(SSD1306_SCL);
  }
  DIGITAL_WRITE_HIGH(SSD1306_SDA);
  DIGITAL_WRITE_HIGH(SSD1306_SCL);
  DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void videoStartCommunication(){
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void videoEndCommunication(){
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}


void videoSendCommand(uint8_t command_){
  videoStartCommunication();
  videoSendByte(SSD1306_SA);  // Slave address, SA0=0
  videoSendByte(0x00);  // write command
  videoSendByte(command_);
  videoEndCommunication();
}

void videoInit(){
  DDRB |= (1 << SSD1306_SDA); // Set port as output
  DDRB |= (1 << SSD1306_SCL); // Set port as output

  videoSendCommand(0xAE); // display off (sleep mode = on)


 
  videoSendCommand(0xA1); // -- set segment re-map 0 to 127 (horizontal flip)
  videoSendCommand(0xC8); // Set COM Output Scan Direction 

  videoSendCommand(0xA8); // Set Multiplex Ratio
  videoSendCommand(0x3F); // Multiplex Ratio for 128x64 (64-1)

  videoSendCommand(0xD3); // --Set vertical shift by COM from 0d~63d
  videoSendCommand(0x00); // no shift

  videoSendCommand(0xD5); // set display clock divide ratio/oscillator frequency
  videoSendCommand(0x80); //    default divide ratio and oscillatorfrequrncy 

  videoSendCommand(0x81); // --set contrast control register
  videoSendCommand(0xAF); //   contrast value (reset: 7f)

  videoSendCommand(0xD9); // --set pre-charge period
  videoSendCommand(0xF1); // Set Pre-Charge Period (0x22 External (default), 0xF1 Internal)
 
  videoSendCommand(0xDA); // --set com pins hardware configuration
  videoSendCommand(0x12); // default com pin HW config

  videoSendCommand(0xDB); // --set vcomh deselect level
  videoSendCommand(0x20); // default, 0.77xVcc

  videoSendCommand(0x20); //Set Memory Addressing Mode
  videoSendCommand(0x02); // page addressing mode (default)

  videoSendCommand(0x40); // Set display start line
 
  videoSendCommand(0x00); // Set Lower Column Start Address (default)
  videoSendCommand(0x10); // Set Higher Column Start Address (default)
  
  videoSendCommand(0xA4); // resume RAM content display
  videoSendCommand(0xA6); // normal mode (not inverse)

  videoSendCommand(0x8D); // Charge pump settings
  videoSendCommand(0x14); // enable charge pump
 
  videoSendCommand(0xAF); // turn on oled panel 
  }




void videoSetPosition(uint8_t x, uint8_t y)
{
  if (y>7) return;
  videoStartCommunication();
  videoSendByte(SSD1306_SA);  //Slave address,SA0=0
  videoSendByte(0x00);  //write command

  videoSendByte(0xb0 + y);  // b0 .. b7 : page
  videoSendByte(((x & 0xf0) >> 4)|0x10); // |0x10
  videoSendByte(x & 0x0f); // |0x01


  videoEndCommunication();
}


void videoStartPixels(uint8_t xPixel, uint8_t yPage){
  videoSetPosition(xPixel, yPage);
  videoStartCommunication();
  videoSendByte(SSD1306_SA);
  videoSendByte(0x40);  //write data
}


inline void videoStartPixels(uint8_t yPage){
  videoStartPixels(0, yPage);
}


inline void videoEndPixels() {
  videoEndCommunication();
} 

// az egész képernyő memóra feltöltése adott bájttal
void videoFillScreen(uint8_t fillData){
  for(uint8_t p=0; p<8; p++)
  {
    videoStartPixels(p);
    for(uint8_t col=0; col<128; col++)
    {
      videoSendByte(fillData);
    }
    videoEndPixels();
  }
}

// egy adott (widthPixel, heightPage) méretű bitmap rajzolása adott (xPixel, yPage) pozícióba
void videoDrawBitmap(uint8_t xPixel, uint8_t yPage, uint8_t widthPixel, uint8_t heightPage, const uint8_t bitmap[]) { 
  for (uint8_t i=0; i<heightPage; i++) { 
    videoStartPixels(xPixel, i+yPage);
    for (uint8_t j=0; j<widthPixel; j++) { 
      videoSendByte(pgm_read_byte(&bitmap[i*widthPixel+j]));
    }
    videoEndPixels();
  }
}

// egy szöveg kiírása adott (xPixel, yPage) pozícióba
void videoPrintText(uint8_t xPixel, uint8_t yPage, const char text[]){
  uint8_t c,i,j=0;
  while(text[j] != '\0')
  {
    c = text[j] - 32;
    if(xPixel>122)
    {
      xPixel=0;
      yPage++;
    }
    videoStartPixels(xPixel, yPage);
    videoSendByte(0x00);
    for(i=0;i<5;i++)
    {
      videoSendByte(pgm_read_byte(&video_font5x8[c*5+i]));
    }
    videoEndPixels();
    xPixel += 6;
    j++;
  }
}

// egy szám kiírása adott (xPixel, yPage) pozícióba
void videoPrintNumber(int xPixel, int yPage, int value) {
    char temp[10] = {0,0,0,0,0,0,0,0,0,0};
    itoa(value, temp, 10);
    videoPrintText(xPixel, yPage, temp);
}

// ez a függvény visszatér egy adott (x,y) koordináta szinével, ha az adott
// méretű (width, height) bitmap a képernyő adott (offsetX, offsetY) koordinátáján
// lenne megjelenítve. Ha a koordináta kívül esik a bitmap-en, akkor defaultColor-ral
// tér vissza
uint8_t videoPixelColor(uint8_t x, uint8_t y, const uint8_t bitmap[], int16_t offsetX, int8_t offsetY, uint8_t width, uint8_t height, uint8_t defaultColor) {
  if(x < offsetX || y < offsetY || x >= offsetX + width || y >= offsetY + height) {
    return defaultColor;
  }
  uint16_t bitmapPage = (y - offsetY) / 8;
  return (pgm_read_byte(&bitmap[bitmapPage * width + (x - offsetX)]) >> ((y - offsetY) % 8)) & 0x01;
}
