// ebben a fájlban vannak a kígyós játék állapotát leíró változók 
// és az azokat vezérlő függvények


// a pálya adott darab dobozból áll:
#define NUM_BOXES_X 16
#define NUM_BOXES_Y 10

// egy doboz szélessége / magassága képpontokban:
#define BOX_PIXELS 6

// a pálya köré keretet rajzolunk, az offset jelzi, hogy hol van a pálya bal-felső sarka
#define OFFSET_X 2
#define OFFSET_Y 2



// ha elértük a maximális méretét a kígyónak, akkor nyerünk:
#define MAX_SNAKE_LENGTH 30

// ennyi ezredmásodperc telik el két kígyó mozgás között
// (ezt csökkentsük/növeljük, ha gyorsítani/lassítani akarunk a játékon)
#define STEP_TIME_MILLISEC 500

// a kígyó fejének lehetséges irányai (merre fog tovább menni a következő lépésekben):
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4


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
uint8_t high_score;           // az eddigi legmagasabb elért pontszám az adott pályán
uint8_t running;              // 1 ha még tart a játék és mozog a kígyó, 0 ha nyertünk vagy vesztettünk
uint8_t sound_enabled = 1;    // engedélyezve van-e a hang
uint8_t current_level = 0;    // aktuális pálya
uint8_t current_level_idx = 0;// az aktuális pályához tartozó dobozok kezdő index-e a level_blocks_x/y tömbben
uint8_t new_level = 0;        // 1, ha kiléptünk egy pályáról és új pályát kezdünk (ilyenkor nincs kezdő képernyő)



// ============================================================================
// =  a pálya elemeinek (kígyó, akadály, étel) kirajzolásával kapcsolatos kód =
// ============================================================================

// a kígyó adott pozíciójához (0: eleje, snale_length-1: hátulja) tartozó
// index, amit a snale_x/y_position tömbben használhatunk
uint8_t snakeIndex(uint8_t snakePos) {
  return (MAX_SNAKE_LENGTH + snake_head_index - snakePos) % MAX_SNAKE_LENGTH;
}

// az adott x,y pozícióba mehetünk-e? (0: ha a pálya szélébe, akadályba vagy a kígyóba ütköznénk)
uint8_t isFree(int8_t x, int8_t y) {
  if(x<0 || y<0 || x>=NUM_BOXES_X ||  y>=NUM_BOXES_Y) {
    return 0;
  }
  for(uint8_t i=current_level_idx; i < current_level_idx + number_of_level_blocks[current_level]; i++) {
    if(x == level_blocks_x[i] && y == level_blocks_y[i]) return 0;
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


// visszatérünk az adott box pozícióba kirajzolandó bitmap-pel
// pl: kígyó, ennivaló, pálya akarály vagy háttér kocka
uint8_t* getBoxBitmap(uint8_t box_position_x, uint8_t box_position_y) {
  for(uint8_t i=0; i<snake_current_length; i++) {
    if(box_position_x == snake_x_positions[snakeIndex(i)] && box_position_y == snake_y_positions[snakeIndex(i)]) return bitmap_snake_body;
  }
  for(uint8_t i=current_level_idx; i < current_level_idx + number_of_level_blocks[current_level]; i++) {
    if(box_position_x == level_blocks_x[i] && box_position_y == level_blocks_y[i]) return bitmap_level_box;
  }
  if(box_position_x == food_x && box_position_y == food_y) return bitmap_food;
  return bitmap_background;
}

// egy teljes page újra rajzolása, adott kocka x kordinátán
void redrawBoxesInPage(uint8_t x, uint8_t page) {
  videoStartPixels(x * BOX_PIXELS + OFFSET_X, page);
  for(uint8_t i=0; i<BOX_PIXELS; i++) {  // összesen BOX_PIXELS bájtot küldünk el
    uint8_t y = (page * 8 - OFFSET_Y) / BOX_PIXELS;          // melyik y koordinátájú dobozt rajzoljuk épp
    uint8_t y_offset = (page * 8 - OFFSET_Y) % BOX_PIXELS;   // a doboz hányadik y képpontját rajzoljuk épp
    uint8_t* box_bitmap = getBoxBitmap(x, y);     // milyen bitmap-et rajzolunk épp
    uint8_t pixel_byte = 0;
    if(page == 0) pixel_byte = 0b00000001; // a legfelső page-en a felső sorban van egy keret, amit ki kell rajzolnunk
    if(page == 7) pixel_byte = 0b10000000; // a legalsó page-en az alsó sorban van egy keret, amit ki kell rajzolnunk
    // minden 1 pixel széles oszlophoz 8 képpontot küldünk, a legkisebb helyiértékű bit a legfelső pixel
    for(uint8_t j=0; j<8; j++) {  
      if(y_offset == BOX_PIXELS) {   // elértünk egy következő dobozt
        y++;
        y_offset=0;
        box_bitmap = getBoxBitmap(x, y);
      }
      // képpont színe (0/1) a bitmap i oszlopának y_offset sorában:
      uint8_t color_from_bitmap = (box_bitmap[y_offset] >> (7 - i)) & 0x01; 
      pixel_byte |= (color_from_bitmap << j);
      y_offset++;
    }
    videoSendByte(pixel_byte);
  }
  videoEndPixels();
}


// a pálya egy adott kockájának újra rajzolása
// (igyekszünk csak azt a részt kirajzolni, ami változik, 
//  így nem "villog" a képernyő és gyorsabb is a program)
void redrawBox(uint8_t x, uint8_t y) {
  // a képernyőn egy egész 8 pixeles page-et újra kell rajzolni
  uint8_t first_pixel_of_box = y * BOX_PIXELS + OFFSET_Y;
  uint8_t page = first_pixel_of_box / 8;
  redrawBoxesInPage(x, first_pixel_of_box / 8);

  // kocka kisebb mint 8 pixel, viszont átlóghat két page-re is,
  // így lehet hogy a következő page-et is frissíteni kell
  uint8_t lastPixelOfBox = first_pixel_of_box + BOX_PIXELS - 1;
  if(lastPixelOfBox / 8 != page) {
    redrawBoxesInPage(x, page + 1);
  }
}


// ============================================================================
// =        zene és pályaváltó gombok kezelésével kapcsolatos kód             =
// ============================================================================

void updateLevelIcon() {
  // egy extra sor a pálya sorszáma fölé
  videoStartPixels(LEVEL_ICON_X+1, LEVEL_ICON_PAGE-1);
  for (uint8_t j=0; j<5; j++) { 
    videoSendByte(0b10000000);
  }
  videoEndPixels();

  // a pálya számának kiírása (fehér alapon feketével)
  videoStartPixels(LEVEL_ICON_X, LEVEL_ICON_PAGE);
  videoSendByte(0b01111111);
  for (uint8_t j=0; j<5; j++) { 
    videoSendByte(~pgm_read_byte(&video_font5x8[(17 + current_level) * 5 + j]));
  }
  videoSendByte(0b01111111);
  videoEndPixels();
}


// a hangszóró ikonjának kirajzolása
void updateMusicIcon() {
  videoDrawBitmap(MUSIC_ICON_X, MUSIC_ICON_PAGE, MUSIC_ICON_WIDTH, MUSIC_ICON_HEIGHT, sound_enabled ? music_on : music_off);
}


void handleSound() {
  if(BUTTON_PUSHED_ACTION_1) {
    sound_enabled = !sound_enabled;
    updateMusicIcon();
  }

  if (sound_enabled) {
    soundUpdateTones();
  } else {
    if (soundIsPlaying()) soundStop();
  }
}

// a pálya váltó gomb kezelése. 
// visszatérési érték: 1: történt pálya váltás, 0: nem történt váltás
uint8_t handleLevelButton() {
  if(BUTTON_PUSHED_ACTION_2) {
    current_level = (current_level + 1) % NUMBER_OF_LEVELS;
    updateLevelIcon();
    new_level = 1;
    return 1;
  }
  return 0;
}

// adott millisec-ig vár és közben (ha van zene) zenél
// jobbra/balra/le/fel gombnyomásra tovább lép
// kezeli a hang ki/bekapcsoló és a pálya választó gombot
void delayAndHandleSoundOrLevels(uint16_t millisec) {
  uint32_t end_time = millis() + millisec;
  do {
    delay(50);
    buttonsReadAllInputs();
    handleSound();
    handleLevelButton();
  } while(millis() < end_time && !BUTTON_PUSHED_ARROW);
}



// játék közben látható képernyő inícializálása (keret és státusz panel)
void drawGameScreen() {
  // legfelső page
  videoStartPixels(0, 0);
  videoSendByte(0xff);
  uint8_t i=1;
  for(; i<99; i++) {
    videoSendByte(0b00000001);
  }
  videoSendByte(0xff);
  for(; i<126; i++) {
    videoSendByte(0b00000001);
  }
  videoSendByte(0xff);
  videoEndPixels();

  // középső page-ek
  for(uint8_t p=1; p<7; p++) {
    videoStartPixels(0, p);
    videoSendByte(0xff);
    i=1;
    for(; i<99; i++) {
      videoSendByte(0b00000000);
    }
    videoSendByte(0xff);
    for(; i<126; i++) {
      videoSendByte(0b00000000);
    }
    videoSendByte(0xff);   
    videoEndPixels();
  }

  // legalsó page
  videoStartPixels(0, 7);
  videoSendByte(0xff);
  i=1;
  for(; i<99; i++) {
    videoSendByte(0b10000000);
  }
  videoSendByte(0xff);
  for(; i<126; i++) {
    videoSendByte(0b10000000);
  }
  videoSendByte(0xff);
  videoEndPixels();

  videoDrawBitmap(SCORE_ICON_X, SCORE_ICON_PAGE, SCORE_ICON_WIDTH, SCORE_ICON_HEIGHT, bitmap_score);
  videoDrawBitmap(HIGH_SCORE_ICON_X, HIGH_SCORE_ICON_PAGE, HIGH_SCORE_ICON_WIDTH, HIGH_SCORE_ICON_HEIGHT, bitmap_high_score);
}



// az aktuális pontszámot a jobb felső sarokba rajzoljuk
void printScore() {
    videoPrintText(SCORE_VALUE_X, SCORE_VALUE_PAGE, "  ");
    videoPrintNumber(SCORE_VALUE_X, SCORE_VALUE_PAGE, score);

    videoPrintText(HIGH_SCORE_VALUE_X, HIGH_SCORE_VALUE_PAGE, "  ");
    videoPrintNumber(HIGH_SCORE_VALUE_X, HIGH_SCORE_VALUE_PAGE, high_score);
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

