#include "buttons.h"
#include "display.h"
#include "tones.h"


// Standard Arduino headers
#include <EEPROM.h> 
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h> // needed for the additional interrupt



const unsigned char zero[] PROGMEM = {
  0b00000000,
  0b11111111,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b11111111,
  0b00000000
};


const unsigned char one[] PROGMEM = {
  0b00000000,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b00000000
};


// 'snake_logo', 56x56px
const unsigned char snake_image[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x80, 0x40, 0xe0, 0x18, 0x0c, 0x04, 0x24, 0x64, 0x64, 0x0a, 0xf1, 0x01, 0x19, 
  0x19, 0x9b, 0xc6, 0xbc, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0xc0, 0x30, 0x18, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00, 0x01, 0x03, 0x04, 
  0xc4, 0x04, 0x04, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x83, 0x06, 
  0x1c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xf8, 0x04, 0x02, 0x01, 0x01, 0x02, 0x04, 0x09, 0x0a, 0x0c, 0x0c, 0x08, 0x08, 0x18, 
  0x10, 0xf0, 0x60, 0x88, 0x08, 0x09, 0x04, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x01, 0x06, 0x08, 0x10, 0x20, 0x20, 
  0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x1c, 0x78, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0xc0, 0xc0, 0x40, 0x40, 0x80, 0x81, 
  0x82, 0x82, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x01, 0x01, 
  0x02, 0x02, 0x04, 0x08, 0x30, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x01, 0x06, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x40, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x11, 0x11, 0x21, 0x22, 0x62, 0x46, 
  0x44, 0x64, 0x28, 0x29, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3e, 0x20, 
  0x20, 0x10, 0x10, 0x18, 0x08, 0x04, 0x06, 0x03, 0x00, 0x80, 0xc0, 0x30, 0x0f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x06, 0x04, 0x08, 0x18, 0x10, 0x20, 0x20, 0x40, 
  0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 
  0x60, 0x90, 0x98, 0x8c, 0x42, 0x41, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x0c, 0x06, 
  0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Lawrence of Arabia, main theme
static const uint16_t main_theme_freq[] PROGMEM = {
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_AS, NOTE_5_CS,
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_DS, NOTE_4_CS,
  NOTE_4_D, NOTE_4_A, NOTE_4_G, NOTE_5_C, NOTE_4_AS, NOTE_4_A, 
  NOTE_4_B, NOTE_5_CS
};

static const uint8_t main_theme_lengths[] PROGMEM = {
  8, 12, 4, 2, 2, 2, 
  8, 12, 4, 2, 2, 2, 
  8, 8, 8, 8, 8, 16, 2, 2
};



static const byte music_on[] PROGMEM = {
  B00111100, 
  B00111100, 
  B00111100, 
  B01111110, 
  B11111111,
  B00000000,
  B00000000,
  B00000000
};

static const byte music_off[] PROGMEM = {
  B00111100, 
  B00111100, 
  B00111100, 
  B01000010, 
  B10000001,
  B00100100,
  B00011000,
  B00100100
};


void setup() {
  tones_init();
  display_init();       // initialise the screen
  init_gameboy_inputs();
}


void loop() {
  
 // put your main code here, to run repeatedly:

    display_fillscreen(0x00);
    display_char_f6x8(10, 0, "SW A:");

    display_char_f6x8(10, 2,"SW B:");

  uint8_t music_should_play = 0;
  uint8_t button1_prev = 0;
  uint8_t button2_prev = 0;

  unsigned long current_time = millis();

  while(1) {
  
    button1_prev = button_state[BUTTON_BTN1];
    button2_prev = button_state[BUTTON_BTN2];
    read_all_inputs();
    
    if(!button1_prev && button_state[BUTTON_BTN1]) {
      music_should_play = !music_should_play;
      if(!music_should_play)  silence();
    }
    
    if(!button2_prev && button_state[BUTTON_BTN2]) {
      current_time = millis();
    }

    update_tones();  
    if(!still_playing() && music_should_play) play_tones(main_theme_freq, main_theme_lengths, sizeof(main_theme_lengths));


    draw_barcode(1, btn_right_btn1_btn2);
    draw_barcode(3, btn_up_left_down);
    display_char_f6x8(40, 0, "        ");
    display_char_f6x8(40, 2, "        ");
    doNumber(40, 0, btn_right_btn1_btn2);
    doNumber(40, 2, btn_up_left_down);

    display_draw_bmp(20, 6, 27, 6, button_state[BUTTON_LEFT] ? one : zero);
    display_draw_bmp(48, 6, 55, 6, button_state[BUTTON_RIGHT] ? one : zero);
    display_draw_bmp(34, 5, 41, 5, button_state[BUTTON_UP] ? one : zero);
    display_draw_bmp(34, 7, 41, 7, button_state[BUTTON_DOWN] ? one : zero);


    display_draw_bmp(100, 5, 107, 5, button_state[BUTTON_BTN1] ? one : zero);
    display_draw_bmp(110, 5, 117, 5, button_state[BUTTON_BTN2] ? one : zero);

    display_draw_bmp(80, 5, 87, 5, music_should_play ? music_on : music_off);

    display_char_f6x8(64, 7, "time:           ");
    doNumber(96, 7, current_time);

    delay(10);

  }
  

}

void draw_barcode(uint8_t line, uint16_t number) {

    for(uint8_t seg = 0; seg<16; seg++) {
      uint16_t color = (number << seg) & 0x8000;
      if(color) display_draw_bmp(seg*8, line, seg*8 + 7, line, one);
      else display_draw_bmp(seg*8, line, seg*8 + 7, line, zero);
    }


}


