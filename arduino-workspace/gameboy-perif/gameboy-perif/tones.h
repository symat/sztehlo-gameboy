#ifndef __STEHLO_GAMEBOY_NOTES__
#define __STEHLO_GAMEBOY_NOTES__

#define NOTE_PAUSE 0
#define NOTE_0_C 0x0BEF
#define NOTE_0_CS 0x0BE2
#define NOTE_0_D 0x0BD5
#define NOTE_0_DS 0x0BC9
#define NOTE_0_E 0x0BBE
#define NOTE_0_F 0x0BB3
#define NOTE_0_FS 0x0BA9
#define NOTE_0_G 0x0B9F
#define NOTE_0_GS 0x0B96
#define NOTE_0_A 0x0B8E
#define NOTE_0_AS 0x0B86
#define NOTE_0_B 0x0B7F
#define NOTE_1_C 0x0AEF
#define NOTE_1_CS 0x0AE1
#define NOTE_1_D 0x0AD5
#define NOTE_1_DS 0x0AC9
#define NOTE_1_E 0x0ABE
#define NOTE_1_F 0x0AB3
#define NOTE_1_FS 0x0AA9
#define NOTE_1_G 0x0A9F
#define NOTE_1_GS 0x0A97
#define NOTE_1_A 0x0A8E
#define NOTE_1_AS 0x0A86
#define NOTE_1_B 0x0A7F
#define NOTE_2_C 0x09EF
#define NOTE_2_CS 0x09E1
#define NOTE_2_D 0x09D5
#define NOTE_2_DS 0x09C9
#define NOTE_2_E 0x09BE
#define NOTE_2_F 0x09B3
#define NOTE_2_FS 0x09A9
#define NOTE_2_G 0x099F
#define NOTE_2_GS 0x0996
#define NOTE_2_A 0x098E
#define NOTE_2_AS 0x0986
#define NOTE_2_B 0x097F
#define NOTE_3_C 0x08EF
#define NOTE_3_CS 0x08E1
#define NOTE_3_D 0x08D5
#define NOTE_3_DS 0x08C9
#define NOTE_3_E 0x08BE
#define NOTE_3_F 0x08B3
#define NOTE_3_FS 0x08A9
#define NOTE_3_G 0x089F
#define NOTE_3_GS 0x0896
#define NOTE_3_A 0x088E
#define NOTE_3_AS 0x0886
#define NOTE_3_B 0x087F
#define NOTE_4_C 0x07EF
#define NOTE_4_CS 0x07E1
#define NOTE_4_D 0x07D5
#define NOTE_4_DS 0x07C9
#define NOTE_4_E 0x07BE
#define NOTE_4_F 0x07B3
#define NOTE_4_FS 0x07A9
#define NOTE_4_G 0x079F
#define NOTE_4_GS 0x0796
#define NOTE_4_A 0x078E
#define NOTE_4_AS 0x0786
#define NOTE_4_B 0x077F
#define NOTE_5_C 0x06EF
#define NOTE_5_CS 0x06E1
#define NOTE_5_D 0x06D5
#define NOTE_5_DS 0x06C9
#define NOTE_5_E 0x06BE
#define NOTE_5_F 0x06B3
#define NOTE_5_FS 0x06A9
#define NOTE_5_G 0x069F
#define NOTE_5_GS 0x0696
#define NOTE_5_A 0x068E
#define NOTE_5_AS 0x0686
#define NOTE_5_B 0x067F
#define NOTE_6_C 0x05EF
#define NOTE_6_CS 0x05E1
#define NOTE_6_D 0x05D5
#define NOTE_6_DS 0x05C9
#define NOTE_6_E 0x05BE
#define NOTE_6_F 0x05B3
#define NOTE_6_FS 0x05A9
#define NOTE_6_G 0x059F
#define NOTE_6_GS 0x0596
#define NOTE_6_A 0x058E
#define NOTE_6_AS 0x0586
#define NOTE_6_B 0x057F
#define NOTE_7_C 0x04EF
#define NOTE_7_CS 0x04E1
#define NOTE_7_D 0x04D5
#define NOTE_7_DS 0x04C9
#define NOTE_7_E 0x04BE
#define NOTE_7_F 0x04B3
#define NOTE_7_FS 0x04A9
#define NOTE_7_G 0x049F
#define NOTE_7_GS 0x0496
#define NOTE_7_A 0x048E
#define NOTE_7_AS 0x0486
#define NOTE_7_B 0x047F
#define NOTE_8_C 0x03EF
#define NOTE_8_CS 0x03E1
#define NOTE_8_D 0x03D5
#define NOTE_8_DS 0x03C9
#define NOTE_8_E 0x03BE
#define NOTE_8_F 0x03B3
#define NOTE_8_FS 0x03A9
#define NOTE_8_G 0x039F
#define NOTE_8_GS 0x0396
#define NOTE_8_A 0x038E
#define NOTE_8_AS 0x0386
#define NOTE_8_B 0x037F

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


void silence() {
  TCCR1 = 0x90;              // stop the TCCR1 counter
  current_tone_counter = 0;
}

uint8_t still_playing() {
  return current_tone_counter == 0 ? 0 : 1;
}

void tones_init() {
  DDRB |= 0b00000010;    // set PB1 as output (for the speaker)  
}

void play_tone(uint16_t tone_counter) {
  uint8_t clock_divider = (tone_counter >> 8);
  uint8_t counter_target_8bit = tone_counter & 0xff;

  TCCR1 = 0x90 | clock_divider; // 0x9 means: clear timer when OCR reached, and also
                                // toggling the output of OC1A = PB1
  OCR1C = counter_target_8bit-1;// set the OCR (number of counts)
}

void update_tones() {
  if(current_tone_counter == 0 || millis() < note_until) return;

  if(note_pause) {
    note_pause = 0;
    if(note_next >= number_of_notes) {
      silence();
      return; // end of the melody
    }
    // playing the next note:
    uint16_t actualCounter = pgm_read_word(current_tone_counter + note_next);
    if(actualCounter == NOTE_PAUSE) {
      TCCR1 = 0x90;              // stop the TCCR1 counter, silencing
    } else {
      play_tone(actualCounter);
    }
    
    note_until = millis() + (TONE_LENGTH_MULTIPLIER_MS * (uint16_t) pgm_read_byte(current_tone_lengths + note_next));
    note_next++;
  } else {
    note_pause = 1;
    TCCR1 = 0x90;              // stop the TCCR1 counter, silencing
    note_until = millis() + TONE_PAUSE_MS;   
  }
}



// expecting the arrays from plash (PROGMEM)
void play_tones(const uint16_t* tones, const uint8_t* tone_lengths, uint16_t _number_of_notes) {
  current_tone_counter = tones;
  current_tone_lengths = tone_lengths;
  number_of_notes = _number_of_notes;
  note_next = 0;
  note_until = 0;
  note_pause = 1;
  update_tones();
}



#endif //__STEHLO_GAMEBOY_NOTES__