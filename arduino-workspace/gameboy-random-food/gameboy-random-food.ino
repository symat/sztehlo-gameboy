#include "sztehlo-buttons.h"
#include "sztehlo-video.h"

uint8_t previous_position_x = 7; // 0..15
uint8_t previous_position_y = 3; // 0..7
uint8_t current_position_x = 7;  // 0..15
uint8_t current_position_y = 3;  // 0..7
uint8_t score = 0;   // aktuális pontszám
uint8_t food_x = 0;  // az étel helyének aktuális X koordinátája (0..15)
uint8_t food_y = 0;  // az étel helyének aktuális Y koordinátája (0..7)

// egy doboz szélessége / magassága képpontokban:
#define BOX_PIXELS 8

// hány darab doboz fér ki az egész képernyőn:
#define NUM_BOXES_X 16
#define NUM_BOXES_Y 8


// a dobozok újra rajzolása
void redrawBoxes() {
  // feketére állítjuk a korábban kirajzolt kocka helyét
  videoFillRect(previous_position_x * BOX_PIXELS, previous_position_y, BOX_PIXELS, 1, BLACK_SCREEN_BYTE);
  
  // kirajzoljuk a kockát az új helyre
  videoFillRect(current_position_x * BOX_PIXELS, current_position_y, BOX_PIXELS, 1, WHITE_SCREEN_BYTE);
}



void printScore() {
    videoPrintText(0, 0, "   ");
    videoPrintNumber(0, 0, score);
}

void newRandomFood() {
  // újra inicializáljuk a véletlen szám generátort, az indulás óta eltelt mikroszekundumok alapján
  // ha ezt a sort kihagyjuk, akkor ujraindítás után mindíg ugyanott fognak az ételek egymás után megjelenni
  randomSeed(micros());


  // új véletlen koordináták számolása az ételnek:
  // ha az új étel véletlenül pont a régi helyére esne, vagy a bal felső sarokba 
  // a pontszám helyére kerülne, akkor újra próbáljuk
  while((food_x == current_position_x && food_y == current_position_y) || (food_y == 0 && food_x < 2)) {
    food_x = random(NUM_BOXES_X);
    food_y = random(NUM_BOXES_Y);
  }

  // kirajzoljuk az ételt
  videoFillRect(food_x * BOX_PIXELS, food_y, BOX_PIXELS, 1, 0b01010101);
}

void setup() {
  videoInit();
  buttonsInit();
  videoFillScreen(BLACK_SCREEN_BYTE);
  printScore();
  newRandomFood();
  redrawBoxes();
}

void loop() {
  if(previous_position_x != current_position_x || previous_position_y != current_position_y) {
    redrawBoxes();
    previous_position_x = current_position_x;
    previous_position_y = current_position_y;
    if(current_position_x == food_x && current_position_y == food_y) {
      score++;
      printScore();
      newRandomFood();
    }
  }

  buttonsReadAllInputs();
  delay(50);

  if (BUTTON_PUSHED_UP) current_position_y = (uint8_t)(current_position_y - 1) % NUM_BOXES_Y;
  if (BUTTON_PUSHED_DOWN) current_position_y = (uint8_t)(current_position_y + 1) % NUM_BOXES_Y;
  if (BUTTON_PUSHED_LEFT) current_position_x = (uint8_t)(current_position_x - 1) % NUM_BOXES_X;
  if (BUTTON_PUSHED_RIGHT) current_position_x = (uint8_t)(current_position_x + 1) % NUM_BOXES_X;
}
