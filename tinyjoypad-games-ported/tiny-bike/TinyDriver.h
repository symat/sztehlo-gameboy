//      >>>>>  T-I-N-Y  T-R-I-C-K for ATTINY85  GPL v3 <<<<
//                    Programmer: Daniel C 2019-2021
//              Contact EMAIL: electro_l.i.b@tinyjoypad.com
//                      https://WWW.TINYJOYPAD.COM
//           https://sites.google.com/view/arduino-collection

//  Tiny trick is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//for TINY JOYPAD rev2 (attiny85)
//the code work at 16MHZ internal
//Program the chip with an arduino uno in "Arduino as ISP" mode.


#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)


void Sound(uint8_t freq, uint8_t dur) {
  if (freq == 0) {
    delay(dur);
    goto END;
  }
  for (uint8_t t = 0; t < dur; t++) {
    if (freq != 0) DIGITAL_WRITE_HIGH(1);
    for (uint8_t t = 0; t < (255 - freq); t++) {
      _delay_us(1);
    }
    DIGITAL_WRITE_LOW(1);
    for (uint8_t t = 0; t < (255 - freq); t++) {
      _delay_us(1);
    }
  }
END:;
}



//prototype
void TinyOLED_init(void);
void TinyOLED_Begin(void);
void TinyOLED_End(void);
void TinyOLED_Send(uint8_t byte_);
void TinyOLED_send_command(uint8_t command_);
void TinyOLED_Data_Start(uint8_t Y_);




// Defines for OLED output
#define SSD1306_SCL PORTB2  // SCL
#define SSD1306_SDA PORTB0  // SDA
#define SSD1306_SA 0x78     // Slave address (first 7 bits)


void TinyOLED_init(void) {
  DDRB |= (1 << SSD1306_SDA);  // Set port as output
  DDRB |= (1 << SSD1306_SCL);  // Set port as output

  TinyOLED_send_command(0xAE);  // display off
  TinyOLED_send_command(0x00);  // Set Memory Addressing Mode
  TinyOLED_send_command(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  TinyOLED_send_command(0x40);  // Set Page Start Address for Page Addressing Mode,0-7
  TinyOLED_send_command(0x81);  // Set COM Output Scan Direction
  TinyOLED_send_command(0xCF);  // ---set low column address
  TinyOLED_send_command(0xA1);  // ---set high column address
  TinyOLED_send_command(0xC8);  // --set start line address
  TinyOLED_send_command(0xA6);  // --set contrast control register
  TinyOLED_send_command(0xA8);
  TinyOLED_send_command(0x3F);  // --set segment re-map 0 to 127
  TinyOLED_send_command(0xD3);  // --set normal display
  TinyOLED_send_command(0x00);  // --set multiplex ratio(1 to 64)
  TinyOLED_send_command(0xD5);  //
  TinyOLED_send_command(0x80);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  TinyOLED_send_command(0xD9);  // -set display offset
  TinyOLED_send_command(0xF1);  // -not offset
  TinyOLED_send_command(0xDA);  // --set display clock divide ratio/oscillator frequency
  TinyOLED_send_command(0x12);  // --set divide ratio
  TinyOLED_send_command(0xDB);  // --set pre-charge period
  TinyOLED_send_command(0x40);  //
  TinyOLED_send_command(0x20);  // --set com pins hardware configuration
  TinyOLED_send_command(0x02);
  TinyOLED_send_command(0x8D);  // --set vcomh
  TinyOLED_send_command(0x14);  // 0x20,0.77xVcc
  TinyOLED_send_command(0xA4);  // --set DC-DC enable
  TinyOLED_send_command(0xA6);  //
  TinyOLED_send_command(0xAF);  // --turn on oled panel
}

void TinyOLED_Begin(void) {
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void TinyOLED_End(void) {
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}

void TinyOLED_Send(uint8_t byte_) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    if ((byte_ << i) & 0x80)
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

void TinyOLED_send_command(uint8_t command_) {
  TinyOLED_Begin();
  TinyOLED_Send(SSD1306_SA);  // Slave address, SA0=0
  TinyOLED_Send(0x00);        // write command
  TinyOLED_Send(command_);
  TinyOLED_End();
}

void display_setpos(uint8_t x, uint8_t y) {
  if (y > 7) return;
  TinyOLED_Begin();
  TinyOLED_Send(SSD1306_SA);  //Slave address,SA0=0
  TinyOLED_Send(0x00);        //write command

  TinyOLED_Send(0xb0 + y);
  TinyOLED_Send(((x & 0xf0) >> 4) | 0x10);  // |0x10
  TinyOLED_Send((x & 0x0f) | 0x01);         // |0x01

  TinyOLED_End();
}

void TinyOLED_Data_Start(uint8_t Y_) {
  display_setpos(0, Y_);
  TinyOLED_Begin();
  TinyOLED_Send(SSD1306_SA);
  TinyOLED_Send(0x40);  //write data
}
