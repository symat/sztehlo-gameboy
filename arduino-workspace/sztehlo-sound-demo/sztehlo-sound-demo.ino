/*
  Demo program a Sztehlo Gameboy hangszórójának teszteléséhez.
  A sztehlo-sound.h olyan függvényeket tartalmaz, amik segítségével
  zenét játszhatunk le a háttérben.

  Arduino Uno-n csatlakoztassuk a hangszórót a 9-es lábra.
*/


// A 'Lawrence of Arabia' című film zenéje
static const uint16_t lawrence_notes[] PROGMEM = {
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_AS, NOTE_5_CS,
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_DS, NOTE_4_CS,
  NOTE_4_D, NOTE_4_A, NOTE_4_G, NOTE_5_C, NOTE_4_AS, NOTE_4_A, NOTE_4_B, NOTE_5_CS
};
// mindegyik hangjegyhez megadhatjuk, hogy milyen hosszú legyen
// pl. 8 = 800ms
static const uint8_t lawrence_note_lengths[] PROGMEM = {
  8, 12, 4, 2, 2, 2,
  8, 12, 4, 2, 2, 2,
  8, 8, 8, 8, 8, 16, 2, 2
};


void setup() {
  soundInit();
}

void loop() {
    if (!soundIsPlaying()) {
      // ha épp nincs semmi hang, újra kezdjük a zenét az elejéről
      soundPlayTones(lawrence_notes, lawrence_note_lengths, 20);
    } else {
      // különben, ha épp most is tart a zene, akkor meghívjuk a soundUpdateTones függvényt,
      // ami gondoskodik róla, hogy a következő hangjegyre váltsunk, ha már itt az ideje
      soundUpdateTones();
    }

    // itt következne a program többi része, ahol pl a gombokat olvassuk be, számításokat
    // végzünk vagy épp a képernyőt frissítjük. Az aktuális hangjegy közben folyamatosan
    //  szól majd a háttérben, hála a sztehlo-sound-ban beállított timer konfigutációnak
    delay(50);
}
