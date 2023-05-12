#include "sztehlo-buttons.h"  // gomb kezelés
#include "sztehlo-video.h"    // képernyő kezelés
#include "sztehlo-eeprom.h"   // eeprom írás / olvasás
#include "sztehlo-sound.h"    // hang kezelés

#include "snake-media.h"      // képek, hangok, pályák definíciói
#include "snake-gameplay.h"   // a játék állapotát leíró változók és az ezeket állító függvények


// a kígyós játékunk program azonosítója:
#define EEPROM_PROGRAM_ID 40

// a legnagyobb pontszám címe az EEPROM-ban:
#define EEPROM_HIGHSCORE_ADDRESS 10


// játék kezdő képernyője
void drawStartScreen() {
  videoFillScreen(BLACK_SCREEN_BYTE);
  videoDrawBitmap(8, 0, 56, 7, snake_image);
  videoPrintText(72, 0, "sztehlo");
  videoPrintText(72, 2, "kigyo");
  videoPrintText(72, 4, "jatek");

  if(sound_enabled) soundPlayTones(wellcome_notes, wellcome_note_lengths, 20);
  updateMusicIcon();
  updateLevelIcon();
  delayAndHandleSoundOrLevels(5000);
}


// játék elvesztése utáni képernyő
void drawGameOverScreen() {
  videoFillScreen(BLACK_SCREEN_BYTE);
  videoDrawBitmap(0, 0, 64, 8, bitmap_tombstone);
  videoPrintText(72, 1, "game over");
  videoPrintText(72, 3, "probald");
  videoPrintText(72, 4, "ujra!");

  updateMusicIcon();
  updateLevelIcon();
  if(sound_enabled) soundPlayTones(game_over_notes, game_over_lengths, 4);
  delayAndHandleSoundOrLevels(2000);
}


// játék megnyerése utáni képernyő
void drawWinningScreen() {
  videoFillScreen(BLACK_SCREEN_BYTE);
  videoDrawBitmap(2, 0, 60, 8, bitmap_trophy);
  videoPrintText(66, 1, "nyertel :)");
  videoPrintText(66, 2, "elerted a");
  videoPrintText(66, 3, "maximalis");
  videoPrintText(66, 4, "pontszamot");

  updateMusicIcon();
  updateLevelIcon();
  if(sound_enabled) soundPlayTones(winning_notes, winning_lengths, 4);
  delayAndHandleSoundOrLevels(4000);
}


// a játék állapotának beállítása a kezdő értékekre: 
//   3 hosszú kígyó, jobbra halad a pálya közepén és 0 pontunk van
void initSnake() { 
  if(!new_level) drawStartScreen();
  drawGameScreen();
  updateMusicIcon();
  updateLevelIcon();

  // a kígyó három doboz hosszú, a helyzete a snake_x/y_positions tömbök 
  // 0, 1, 2. elemében van tárolva, a feje a 2. elem
  snake_head_index = 2;
  snake_current_length = 3;
  snake_x_positions[0] = 4;
  snake_x_positions[1] = 5;
  snake_x_positions[2] = 6;
  snake_y_positions[0] = 5;
  snake_y_positions[1] = 5;
  snake_y_positions[2] = 5;

  // a kígyó jobbra néz, erre fog tovább menni
  last_direction = RIGHT;
  next_direction = RIGHT;

  // bepozícionáljuk magunkat az akruális pályára
  current_level_idx = 0;
  for(uint8_t level=0; level<current_level; level++) current_level_idx += number_of_level_blocks[level];

  // új ételt rajzolunk ki
  drawNewFood();

  // kirajzoljuk a pályát
  for(uint8_t i=current_level_idx; i < current_level_idx + number_of_level_blocks[current_level]; i++) {
    redrawBox(level_blocks_x[i], level_blocks_y[i]);
  }

  // kirajzoljuk a kígyót, az aktuális pontszámot és egy új ételt
  redrawBox(snake_x_positions[0], snake_y_positions[0]);
  redrawBox(snake_x_positions[1], snake_y_positions[1]);
  redrawBox(snake_x_positions[2], snake_y_positions[2]);
  score = 0;
  printScore();
  

  // elindítjuk a játékot, a kígyó mozog
  running = 1;
  new_level = 0;
} 

void setup() {
  videoInit();
  soundInit();
  buttonsInit();
  eepromInit(EEPROM_PROGRAM_ID);
  high_score = eepromRead(EEPROM_HIGHSCORE_ADDRESS + current_level);
}


void loop() {
  initSnake();
  high_score = eepromRead(EEPROM_HIGHSCORE_ADDRESS + current_level);
  uint8_t winning = 0;
  uint8_t game_over = 0;

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

       // ha megdöntöttük a rekordot, akkor elmentjük az eredményt
        if(score > high_score) {
          eepromUpdate(EEPROM_HIGHSCORE_ADDRESS + current_level, score);
          high_score = score;
        }

        printScore();
        if(sound_enabled) soundPlayTones(food_notes, food_lengths, 2);

        // ha elértük a maximális méretet, nyertünk
        if(snake_current_length == MAX_SNAKE_LENGTH) {
          running = 0;
          winning = 1;
        } else {
          drawNewFood();
        }
      }

      if(!isFree(next_x, next_y)) { // ha nekiütköztünk valaminek, akkor vége a játéknak
        if(sound_enabled) soundPlayTones(collision_notes, collision_lengths, 4);
        running = 0;
        game_over = 1;
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
    delay(40);
    handleSound();

    // ha pálya váltás történt, kilépünk az aktuális pályáról, 
    // anélkül, hogy győztünk vagy vesztettünk volna
    if(handleLevelButton()) {
      running = 0;
    }
  }
  // ha kiléptünk a fenti ciklusból, akkor vagy vesztettünk, vagy nyertünk, vagy pályát váltottunk
  // ezután új játékot kezdünk

  // hacsak nem pálya váltás volt, előbb várunk kicsit
  if(!BUTTON_PUSHED_ACTION_2) delayAndHandleSoundOrLevels(1500);
  
  if(game_over) drawGameOverScreen();
  if(winning) drawWinningScreen();
}
