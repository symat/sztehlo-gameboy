/* 
   Playing tones in th background.

   This version only works on Arduino! 
   connect the buzzer to PIN 9, with a 200-300 ohm resistor
*/

#ifndef __SZTEHLO_GAMEBOY_SOUND__
#define __SZTEHLO_GAMEBOY_SOUND__

#define NOTE_PAUSE 0


// the following notes are defined for 8MHz clock speed 
// used in the Sztehlo Gameboy (ATtiny85 internal clock)
// using them on Arduino Uno (16MHz) will result every note 
// to sound one octave higher
#define NOTE_0_C 16
#define NOTE_0_CS 17
#define NOTE_0_D 18
#define NOTE_0_DS 19
#define NOTE_0_E 21
#define NOTE_0_F 22
#define NOTE_0_FS 23
#define NOTE_0_G 25
#define NOTE_0_GS 26
#define NOTE_0_A 28
#define NOTE_0_AS 29
#define NOTE_0_B 31
#define NOTE_1_C 33
#define NOTE_1_CS 35
#define NOTE_1_D 37
#define NOTE_1_DS 39
#define NOTE_1_E 41
#define NOTE_1_F 44
#define NOTE_1_FS 46
#define NOTE_1_G 49
#define NOTE_1_GS 52
#define NOTE_1_A 55
#define NOTE_1_AS 58
#define NOTE_1_B 62
#define NOTE_2_C 65
#define NOTE_2_CS 69
#define NOTE_2_D 73
#define NOTE_2_DS 78
#define NOTE_2_E 82
#define NOTE_2_F 87
#define NOTE_2_FS 93
#define NOTE_2_G 98
#define NOTE_2_GS 104
#define NOTE_2_A 110
#define NOTE_2_AS 117
#define NOTE_2_B 123
#define NOTE_3_C 131
#define NOTE_3_CS 139
#define NOTE_3_D 147
#define NOTE_3_DS 156
#define NOTE_3_E 165
#define NOTE_3_F 175
#define NOTE_3_FS 185
#define NOTE_3_G 196
#define NOTE_3_GS 208
#define NOTE_3_A 220
#define NOTE_3_AS 233
#define NOTE_3_B 247
#define NOTE_4_C 262
#define NOTE_4_CS 277
#define NOTE_4_D 294
#define NOTE_4_DS 311
#define NOTE_4_E 330
#define NOTE_4_F 349
#define NOTE_4_FS 370
#define NOTE_4_G 392
#define NOTE_4_GS 415
#define NOTE_4_A 440
#define NOTE_4_AS 466
#define NOTE_4_B 494
#define NOTE_5_C 523
#define NOTE_5_CS 554
#define NOTE_5_D 587
#define NOTE_5_DS 622
#define NOTE_5_E 659
#define NOTE_5_F 698
#define NOTE_5_FS 740
#define NOTE_5_G 784
#define NOTE_5_GS 831
#define NOTE_5_A 880
#define NOTE_5_AS 932
#define NOTE_5_B 988
#define NOTE_6_C 1047
#define NOTE_6_CS 1109
#define NOTE_6_D 1175
#define NOTE_6_DS 1245
#define NOTE_6_E 1319
#define NOTE_6_F 1397
#define NOTE_6_FS 1480
#define NOTE_6_G 1568
#define NOTE_6_GS 1661
#define NOTE_6_A 1760
#define NOTE_6_AS 1865
#define NOTE_6_B 1976
#define NOTE_7_C 2093
#define NOTE_7_CS 2217
#define NOTE_7_D 2349
#define NOTE_7_DS 2489
#define NOTE_7_E 2637
#define NOTE_7_F 2794
#define NOTE_7_FS 2960
#define NOTE_7_G 3136
#define NOTE_7_GS 3322
#define NOTE_7_A 3520
#define NOTE_7_AS 3729
#define NOTE_7_B 3951
#define NOTE_8_C 4186
#define NOTE_8_CS 4435
#define NOTE_8_D 4699
#define NOTE_8_DS 4978
#define NOTE_8_E 5274
#define NOTE_8_F 5588
#define NOTE_8_FS 5920
#define NOTE_8_G 6272
#define NOTE_8_GS 6645
#define NOTE_8_A 7040
#define NOTE_8_AS 7459
#define NOTE_8_B 7902

#ifndef TONE_PAUSE_MS
#define TONE_PAUSE_MS 100
#endif

#ifndef TONE_LENGTH_MULTIPLIER_MS
#define TONE_LENGTH_MULTIPLIER_MS 100
#endif


uint32_t note_until;            // next millisec when anything nees to be changed
uint8_t note_pause;             // weather to pause between notes
uint16_t note_next;             // next tone to play
uint16_t number_of_notes = 0;
const uint16_t* current_tone_counter = 0;   
const uint8_t* current_tone_lengths = 0;   



void soundStop() {
  noTone(9);          
  current_tone_counter = 0;
}

uint8_t soundIsPlaying() {
  return current_tone_counter == 0 ? 0 : 1;
}

void soundInit() {
  pinMode(9, OUTPUT);
}


void soundUpdateTones() {
  if(current_tone_counter == 0 || millis() < note_until) return;

  if(note_pause) {
    note_pause = 0;
    if(note_next >= number_of_notes) {
      soundStop();
      return; // end of the melody
    }
    // playing the next note:
    uint16_t actualCounter = pgm_read_word(current_tone_counter + note_next);
    uint16_t tone_length = TONE_LENGTH_MULTIPLIER_MS * (uint16_t) pgm_read_byte(current_tone_lengths + note_next);
    if(actualCounter == NOTE_PAUSE) {
      noTone(9);
    } else {
      tone(9, actualCounter, tone_length);
    }
    
    note_until = millis() + tone_length;
    note_next++;
  } else {
    note_pause = 1;
    noTone(9);
    note_until = millis() + TONE_PAUSE_MS;   
  }
}


// expecting the arrays to be sitting on flash (PROGMEM)
void soundPlayTones(const uint16_t* tones, const uint8_t* tone_lengths, uint16_t _number_of_notes) {
  current_tone_counter = tones;
  current_tone_lengths = tone_lengths;
  number_of_notes = _number_of_notes;
  note_next = 0;
  note_until = 0;
  note_pause = 1;
  soundUpdateTones();
}




#endif //__SZTEHLO_GAMEBOY_SOUND__