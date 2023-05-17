#include "sztehlo-buttons.h"
#include "sztehlo-video.h"


// egy doboz szélessége / magassága képpontokban:
#define BOX_PIXELS 8

// hány darab doboz fér ki az egész képernyőn:
#define NUM_BOXES_X 16
#define NUM_BOXES_Y 8

// a kígyó mérete:
#define SNAKE_LENGTH 7

// ennyi ezredmásodperc telik el két kígyó mozgás között
// (ezt csökkentsük/növeljük, ha gyorsítani/lassítani akarunk a játékon)
#define STEP_TIME_MILLISEC 500

// a kígyó fejének lehetséges irányai (merre fog tovább menni a következő lépésekben):
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4


uint8_t snake_x_positions[SNAKE_LENGTH]; // egy tömb, amiben a kígyó által elfoglalt dobozok X kordinátái vannak
uint8_t snake_y_positions[SNAKE_LENGTH]; // egy tömb, amiben a kígyó által elfoglalt dobozok Y kordinátái vannak
uint8_t snake_head_index;     // a snake_x/y_positions tömbökben a kígyó elejéhez (fejéhez) tartozó index
uint8_t last_direction;       // az irány (UP, DOWN, LEFT, RIGHT), amerre a mozgott az előző lépésben
uint8_t next_direction;       // az irány (UP, DOWN, LEFT, RIGHT), amerre a kígyó el fog mozdulni a következő lépésben
uint32_t next_draw_time;      // legközelebb majd ekkor kell lépnünk

// visszatérünk azzal az X kocka pozícióval, ahol a kigyó feje a következő lépésben lesz 
uint8_t getNextPositionX() {
  uint8_t next = snake_x_positions[snake_head_index];
  if(next_direction == LEFT) next -= 1;
  else if(next_direction == RIGHT) next += 1;
  return next % NUM_BOXES_X;
}

// visszatérünk azzal az Y kocka pozícióval, ahol a kigyó feje a következő lépésben lesz 
uint8_t getNextPositionY() {
  uint8_t next = snake_y_positions[snake_head_index];
  if(next_direction == UP) next -= 1;
  else if(next_direction == DOWN) next += 1;
  return next % NUM_BOXES_Y;
}

// megnézzük, az adott doboz ismétlődik-e a kígyóban ()
// azaz: a kígyó egy adott idx eleme ugyanott van-e mint bármelyik másik eleme
uint8_t snakeOverlap(uint8_t idx) {
  uint8_t x = snake_x_positions[idx];
  uint8_t y = snake_y_positions[idx];
  for(uint8_t i=1; i<SNAKE_LENGTH; i++) {
    uint8_t idx_to_check = (idx + i) % SNAKE_LENGTH;
    if(x == snake_x_positions[idx_to_check] && y == snake_y_positions[idx_to_check]) {
      return 1;
    }
  }
  return 0;
}

void setup() {
  videoInit();
  buttonsInit();

  // az egész képernyő törlése
  videoFillScreen(BLACK_SCREEN_BYTE);

  // a kígyó vízszintesen halad, az y=4-es koordinátájú sorban
  // a feje az utolsó elem
  for(uint8_t i=0; i<SNAKE_LENGTH; i++) {
    snake_x_positions[i] = i+2;
    snake_y_positions[i] = 4;
  }
  snake_head_index = SNAKE_LENGTH - 1;

  // a kígyó jobbra néz, erre fog tovább menni
  last_direction = RIGHT;
  next_direction = RIGHT;

  // kirajzoljuk a kígyót
  for(uint8_t i=0; i<SNAKE_LENGTH; i++) {
    videoFillRect(snake_x_positions[i] * BOX_PIXELS, snake_y_positions[i], BOX_PIXELS, 1, WHITE_SCREEN_BYTE);
  }

  next_draw_time = millis() + STEP_TIME_MILLISEC; // legközelebb majd ekkor kell lépnünk
}


void loop() {

  // ha elég idő telt el az előző lépés óta, lépünk egyet a kígyóval
  if(millis() >= next_draw_time) {
    next_draw_time = millis() + STEP_TIME_MILLISEC; // legközelebb majd ekkor kell lépnünk

    // azt a helyet, ahol a kígyó hátulja volt korábban, letöröljük
    // de csak akkor töröljük le, ha nem része a kígyónak
    // (nehogy beletöröljünk a kígyó közepébe, amikor az átmegy saját magán)
    uint8_t old_tail_idx = (snake_head_index + 1) % SNAKE_LENGTH;
    if(!snakeOverlap(old_tail_idx)) {
      videoFillRect(snake_x_positions[old_tail_idx] * BOX_PIXELS, snake_y_positions[old_tail_idx], BOX_PIXELS, 1, BLACK_SCREEN_BYTE);
    }

    // az előző lépés óta detektált gombnyomások alapján kiszámoljuk a kígyó fejének következő pozícióját
    uint8_t next_x = getNextPositionX();
    uint8_t next_y = getNextPositionY();
    snake_head_index = old_tail_idx;
    snake_x_positions[snake_head_index] = next_x;
    snake_y_positions[snake_head_index] = next_y;
    last_direction = next_direction;

    // kirajzoljuk a kockát, ahová a feje lépett
    videoFillRect(next_x * BOX_PIXELS, next_y, BOX_PIXELS, 1, WHITE_SCREEN_BYTE);    

    // ha ki akarjuk írni, hová léptünk, hogy ellenőrizzük a programot:
    //videoPrintText(0, 0, "  ,   ");
    //videoPrintNumber(0, 0, next_x);
    //videoPrintNumber(24, 0, next_y);
  }

  // a gombok állapota alapján beállítjuk, merre kell majd tovább menni a kígyóval
  // ezt sokszor megnézzük, két kígyó-mozgás között, így mindíg az utolsó lenyomott
  // gomb számít majd
  buttonsReadAllInputs();
  if (BUTTON_PUSHED_UP && last_direction != DOWN) next_direction = UP;
  else if (BUTTON_PUSHED_DOWN && last_direction != UP) next_direction = DOWN;
  else if (BUTTON_PUSHED_LEFT && last_direction != RIGHT) next_direction = LEFT;
  else if (BUTTON_PUSHED_RIGHT && last_direction != LEFT) next_direction = RIGHT;
  delay(50);

}
