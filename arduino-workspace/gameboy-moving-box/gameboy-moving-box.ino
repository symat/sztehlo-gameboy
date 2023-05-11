#include "sztehlo-buttons.h"
#include "sztehlo-video.h"

uint8_t previous_position_x = 7; // 0..15
uint8_t previous_position_y = 3; // 0..7
uint8_t current_position_x = 7;  // 0..15
uint8_t current_position_y = 3;  // 0..7

// egy doboz szélessége / magassága képpontokban:
#define BOX_PIXELS 8

// hány darab doboz fér ki az egész képernyőn:
#define NUM_BOXES_X 16
#define NUM_BOXES_Y 8


// a doboz újra rajzolása
void redraw_box() {
  // feketére állítjuk a korábban kirajzolt kocka helyét
  videoFillRect(previous_position_x * BOX_PIXELS, previous_position_y, BOX_PIXELS, 1, BLACK_SCREEN_BYTE);
  
  // kirajzoljuk a kockát az új helyre
  videoFillRect(current_position_x * BOX_PIXELS, current_position_y, BOX_PIXELS, 1, WHITE_SCREEN_BYTE);
}



void print_coordinates() {
    videoPrintText(0, 0, "  ,   ");
    videoPrintNumber(0, 0, current_position_x);
    videoPrintNumber(24, 0, current_position_y);
}

void setup() {
  videoInit();
  buttonsInit();
  videoFillScreen(BLACK_SCREEN_BYTE);
  print_coordinates();
  redraw_box();
}

void loop() {
  if(previous_position_x != current_position_x || previous_position_y != current_position_y) {
    print_coordinates();
    redraw_box();
    previous_position_x = current_position_x;
    previous_position_y = current_position_y;
  }

  buttonsReadAllInputs();
  delay(50);

  if (BUTTON_PUSHED_UP) current_position_y = (uint8_t)(current_position_y - 1) % NUM_BOXES_Y;
  if (BUTTON_PUSHED_DOWN) current_position_y = (uint8_t)(current_position_y + 1) % NUM_BOXES_Y;
  if (BUTTON_PUSHED_LEFT) current_position_x = (uint8_t)(current_position_x - 1) % NUM_BOXES_X;
  if (BUTTON_PUSHED_RIGHT) current_position_x = (uint8_t)(current_position_x + 1) % NUM_BOXES_X;
}
