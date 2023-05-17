#include "sztehlo-buttons.h"
#include "sztehlo-video.h"

// a pálya adott darab dobozból áll:
#define NUM_BOXES_X 14
#define NUM_BOXES_Y 8

// egy doboz szélessége / magassága képpontokban:
#define BOX_PIXELS 8

// ha elértük a maximális méretét a kígyónak, akkor nyerünk:
#define MAX_SNAKE_LENGTH 15

// ennyi ezredmásodperc telik el két kígyó mozgás között
// (ezt csökkentsük/növeljük, ha gyorsítani/lassítani akarunk a játékon)
#define STEP_TIME_MILLISEC 500

// a kígyó fejének lehetséges irányai (merre fog tovább menni a következő lépésekben):
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

const uint8_t bitmap_background[] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t bitmap_snake_body[] PROGMEM = {
  0b11111111,
  0b10000001,
  0b10111101,
  0b10111101,
  0b10111101,
  0b10111101,
  0b10000001,
  0b11111111
};

const uint8_t bitmap_food[] PROGMEM = {
  0b00011000,
  0b00100100,
  0b01000010,
  0b10011001,
  0b10011001,
  0b01000010,
  0b00100100,
  0b00011000
};

// a játék aktuális állapotát leíró változók:
uint8_t snake_x_positions[MAX_SNAKE_LENGTH]; // egy tömb, amiben a kígyó által elfoglalt dobozok X kordinátái vannak
uint8_t snake_y_positions[MAX_SNAKE_LENGTH]; // egy tömb, amiben a kígyó által elfoglalt dobozok Y kordinátái vannak
uint8_t snake_current_length; // a kígyó aktuális hossza
uint8_t snake_head_index;     // a snake_x/y_positions tömbökben a kígyó elejéhez (fejéhez) tartozó index
uint8_t last_direction;       // az irány (UP, DOWN, LEFT, RIGHT), amerre a mozgott az előző lépésben
uint8_t next_direction;       // az irány (UP, DOWN, LEFT, RIGHT), amerre a kígyó el fog mozdulni a következő lépésben
uint8_t food_x;               // az ételt tartalmazó doboz X kordinátája
uint8_t food_y;               // az ételt tartalmazó doboz Y kordinátája
uint8_t score;                // az aktuális pontszám
uint8_t running;              // 1 ha még tart a játék és mozog a kígyó, 0 ha nyertünk vagy vesztettünk

// a játék állapotának beállítása a kezdő értékekre: 
//   3 hosszú kígyó, jobbra halad a pálya közepén és 0 pontunk van
void initSnake() {
  // az egész képernyő törlése
  videoFillScreen(BLACK_SCREEN_BYTE); 

  // a kígyó három doboz hosszú, a helyzete a snake_x/y_positions tömbök 
  // 0, 1, 2. elemében van tárolva, a feje a 2. elem
  snake_head_index = 2;
  snake_current_length = 3;
  snake_x_positions[0] = 5;
  snake_x_positions[1] = 6;
  snake_x_positions[2] = 7;
  snake_y_positions[0] = 5;
  snake_y_positions[1] = 5;
  snake_y_positions[2] = 5;

  // a kígyó jobbra néz, erre fog tovább menni
  last_direction = RIGHT;
  next_direction = RIGHT;

  // kirajzoljuk a kígyót, az aktuális pontszámot és egy új ételt
  redrawBox(snake_x_positions[0], snake_y_positions[0]);
  redrawBox(snake_x_positions[1], snake_y_positions[1]);
  redrawBox(snake_x_positions[2], snake_y_positions[2]);
  score = 0;
  printScore();
  drawNewFood();

  // elindítjuk a játékot, a kígyó mozog
  running = 1;
} 

// a kígyó adott pozíciójához (0: eleje, snale_length-1: hátulja) tartozó
// index, amit a snale_x/y_position tömbben használhatunk
uint8_t snakeIndex(uint8_t snakePos) {
  return (MAX_SNAKE_LENGTH + snake_head_index - snakePos) % MAX_SNAKE_LENGTH;
}

// az adott x,y pozícióba mehetünk-e? (0: ha a pálya szélébe vagy a kígyóba ütköznénk)
uint8_t isFree(int8_t x, int8_t y) {
  if(x<0 || y<0 || x>=NUM_BOXES_X ||  y>=NUM_BOXES_Y) {
    return 0;
  }
  for(uint8_t i=0; i<snake_current_length; i++) {
    if(x == snake_x_positions[snakeIndex(i)] && y == snake_y_positions[snakeIndex(i)]) return 0;
  }
  return 1;
}

// visszatérünk azzal az X kocka pozícióval, ahol a kigyó feje a következő lépésben lesz 
uint8_t getNextPositionX() {
  uint8_t next = snake_x_positions[snake_head_index];
  if(next_direction == LEFT) next -= 1;
  else if(next_direction == RIGHT) next += 1;
  return next;
}

// visszatérünk azzal az Y kocka pozícióval, ahol a kigyó feje a következő lépésben lesz 
uint8_t getNextPositionY() {
  uint8_t next = snake_y_positions[snake_head_index];
  if(next_direction == UP) next -= 1;
  else if(next_direction == DOWN) next += 1;
  return next;
}

// a pálya egy adott kockájának újra rajzolása
// (igyekszünk csak azt a részt kirajzolni, ami változik, 
//  így nem "villog" a képernyő és gyorsabb is a program)
void redrawBox(uint8_t x, uint8_t y) {
  // kirajzoljuk a kockát az új helyre, itt kihasználjuk, hogy minden box
  // pontosan egy page-ben van és nem lóg át a page-ek között
  uint8_t* box_bitmap = getBoxBitmap(x, y);
  videoDrawBitmap(x * BOX_PIXELS, y, BOX_PIXELS, 1, box_bitmap);
}

// visszatérünk az adott box pozícióba kirajzolandó bitmap-pel
// pl: kígyó, ennivaló vagy háttér kocka
uint8_t* getBoxBitmap(uint8_t box_position_x, uint8_t box_position_y) {
  for(uint8_t i=0; i<snake_current_length; i++) {
    if(box_position_x == snake_x_positions[snakeIndex(i)] && box_position_y == snake_y_positions[snakeIndex(i)]) return bitmap_snake_body;
  }
  if(box_position_x == food_x && box_position_y == food_y) return bitmap_food;
  return bitmap_background;
}

// az aktuális pontszámot a jobb felső sarokba rajzoljuk
void printScore() {
    videoPrintText(112, 1, "  ");
    videoPrintNumber(112, 1, score);
}

// egy szomorú smiley-t rajzolunk ki, ha vesztettünk
void printLose() {
    videoPrintText(112, 3, ":(");
}

// egy mosolygós smiley-t rajzolunk ki, ha nyertünk
void printWin() {
    videoPrintText(112, 3, ":)");
}

// egy új étel kockát rajzolunk ki, véletlenszerű helyre,
// de figyelünk, hogy nehogy olyan helyre tegyük, amit a kígyó épp elfoglal
void drawNewFood() {
  do {
    food_x = random(NUM_BOXES_X);
    food_y = random(NUM_BOXES_Y);
  } while (!isFree(food_x, food_y));
  redrawBox(food_x, food_y);
}

void setup() {
  videoInit();
  buttonsInit();
}


void loop() {
  initSnake();

  uint32_t next_draw_time = millis() + STEP_TIME_MILLISEC; // legközelebb majd ekkor kell lépnünk
  while(running) {

    // ha elég idő telt el az előző lépés óta, lépünk egyet a kígyóval
    if(millis() >= next_draw_time) {
      next_draw_time = millis() + STEP_TIME_MILLISEC; // legközelebb majd ekkor kell lépnünk

      // az előző lépés óta detektált gombnyomások alapján kiszámoljuk a kígyó fejének következő pozícióját
      uint8_t next_x = getNextPositionX();
      uint8_t next_y = getNextPositionY();
      last_direction = next_direction;


      // ha elértünk egy ennivalót, akkor nő a kígyó hossza és új ennivalót kell kirajzolni
      if(next_x == food_x && next_y == food_y) {
        snake_current_length++;
        score++;
        printScore();

        // ha elértük a maximális méretet, nyertünk
        if(snake_current_length == MAX_SNAKE_LENGTH) {
          printWin();
          running = 0;
        } else {
          drawNewFood();
        }
      }

      if(!isFree(next_x, next_y)) { // ha nekiütköztünk valaminek, akkor vége a játéknak
        printLose();
        running = 0;
      } else { // különben, ha nem ütköztünk, akkor mozgunk előre
        // azt a helyet, ahol a kígyó hátulja volt korábban, újra rajzoljuk
        // és mozgatjuk a kígyót (kirajzoljuk a kockát, ahová a feje lépett)
        uint8_t old_tail_idx = snakeIndex(snake_current_length - 1);
        snake_head_index = (snake_head_index + 1) % MAX_SNAKE_LENGTH;
        snake_x_positions[snake_head_index] = next_x;
        snake_y_positions[snake_head_index] = next_y;
        redrawBox(snake_x_positions[old_tail_idx], snake_y_positions[old_tail_idx]); 
        redrawBox(next_x, next_y); 
      }
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

  // ha itt vagyunk, akkor vagy vesztettünk, vagy nyertünk a játékban
  // hagyunk egy kis időt és utána újra kezdjük
  delay(2000);
}
