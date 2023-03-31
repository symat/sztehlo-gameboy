#ifndef __STEHLO_GAMEBOY_DISPLAY__
#define __STEHLO_GAMEBOY_DISPLAY__


#include "fonts.h"

#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)



// Defines for OLED output
#define SSD1306_SCL   PORTB2  // SCL
#define SSD1306_SDA   PORTB0  // SDA
#define SSD1306_SA    0x78  // Slave address (first 7 bits)

// Function prototypes
void doNumber(int x, int y, int value);
void doNumber16(int x, int y, int value);
void display_init(void);
void display_xfer_start(void);
void display_xfer_stop(void);
void display_send_byte(uint8_t byte);
void display_send_command(uint8_t command);
void display_send_data_start(void);
void display_send_data_stop(void);
void display_setpos(uint8_t x, uint8_t y);
void display_fillscreen(uint8_t fill_Data);
void display_char_f6x8(uint8_t x, uint8_t y, const char ch[]);
void display_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bitmap[]);


void doNumber (int x, int y, int value) {
    char temp[10] = {0,0,0,0,0,0,0,0,0,0};
    itoa(value,temp,10);
    display_char_f6x8(x, y, temp);
}

void doNumber16 (int x, int y, int value) {
    char temp[10] = {0,0,0,0,0,0,0,0,0,0};
    itoa(value,temp,10);
    display_char_f6x8(x, y, temp);
}

void display_init(void){
  DDRB |= (1 << SSD1306_SDA); // Set port as output
  DDRB |= (1 << SSD1306_SCL); // Set port as output

  display_send_command(0xAE); // display off
  display_send_command(0x00); // Set Memory Addressing Mode
  display_send_command(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  display_send_command(0x40); // Set Page Start Address for Page Addressing Mode,0-7
  display_send_command(0x81); // Set COM Output Scan Direction
  display_send_command(0xCF); // ---set low column address
  display_send_command(0xA1); // ---set high column address
  display_send_command(0xC8); // --set start line address
  display_send_command(0xA6); // --set contrast control register
  display_send_command(0xA8);
  display_send_command(0x3F); // --set segment re-map 0 to 127
  display_send_command(0xD3); // --set normal display
  display_send_command(0x00); // --set multiplex ratio(1 to 64)
  display_send_command(0xD5); // 
  display_send_command(0x80); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  display_send_command(0xD9); // -set display offset
  display_send_command(0xF1); // -not offset
  display_send_command(0xDA); // --set display clock divide ratio/oscillator frequency
  display_send_command(0x12); // --set divide ratio
  display_send_command(0xDB); // --set pre-charge period
  display_send_command(0x40); // 
  display_send_command(0x20); // --set com pins hardware configuration
  display_send_command(0x02);
  display_send_command(0x8D); // --set vcomh
  display_send_command(0x14); // 0x20,0.77xVcc
  display_send_command(0xA4); // --set DC-DC enable
  display_send_command(0xA6); // 
  display_send_command(0xAF); // --turn on oled panel 
}

void display_xfer_start(void){
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void display_xfer_stop(void){
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}

void display_send_byte(uint8_t byte){
  uint8_t i;
  for(i=0; i<8; i++)
  {
    if((byte << i) & 0x80)
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

void display_send_command(uint8_t command){
  display_xfer_start();
  display_send_byte(SSD1306_SA);  // Slave address, SA0=0
  display_send_byte(0x00);  // write command
  display_send_byte(command);
  display_xfer_stop();
}

void display_send_data_start(void){
  display_xfer_start();
  display_send_byte(SSD1306_SA);
  display_send_byte(0x40);  //write data
}

void display_send_data_stop(void){
  display_xfer_stop();
}

void display_setpos(uint8_t x, uint8_t y)
{
  if (y>7) return;
  display_xfer_start();
  display_send_byte(SSD1306_SA);  //Slave address,SA0=0
  display_send_byte(0x00);  //write command

  display_send_byte(0xb0+y);
  display_send_byte(((x&0xf0)>>4)|0x10); // |0x10
  display_send_byte((x&0x0f)|0x01); // |0x01

  display_xfer_stop();
}

void display_fillscreen(uint8_t fill_Data){
  uint8_t m,n;
  for(m=0;m<8;m++)
  {
    display_send_command(0xb0+m); //page0-page1
    display_send_command(0x00);   //low column start address
    display_send_command(0x10);   //high column start address
    display_send_data_start();
    for(n=0;n<128;n++)
    {
      display_send_byte(fill_Data);
    }
    display_send_data_stop();
  }
}

void display_char_f6x8(uint8_t x, uint8_t y, const char ch[]){
  uint8_t c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (c >0) c = c - 12;
    if (c >15) c = c - 6;
    if (c>40) c=c-6;
    if(x>126)
    {
      x=0;
      y++;
    }
    display_setpos(x,y);
    display_send_data_start();
    for(i=0;i<6;i++)
    {
      display_send_byte(pgm_read_byte(&displayxled_font6x8[c*6+i]));
    }
    display_send_data_stop();
    x += 6;
    j++;
  }
}

void display_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) { 

  uint8_t width = x1 - x0 + 1;
  uint8_t height = y1 - y0 + 1;
  for (uint8_t i=0; i<height; i++) { 
    display_setpos(x0, i+y0);
    for (uint8_t j=0; j<width; j++) { 
      display_send_data_start();
      display_send_byte(pgm_read_byte(&bitmap[i*width+j]));
      display_send_data_stop();
    }
  }
 
}

#endif //__STEHLO_GAMEBOY_DISPLAY__