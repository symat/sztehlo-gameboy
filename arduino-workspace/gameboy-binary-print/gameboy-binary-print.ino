#include "sztehlo-buttons.h"
#include "sztehlo-video.h"
#include <avr/sleep.h>

int8_t number = 0;
int8_t previous_number = -1;

void setup() {
  videoInit();
  buttonsInit();

  videoFillScreen(BLACK_SCREEN_BYTE);
  videoPrintText(10, 2, "decimal value: ");
  videoPrintText(10, 5, "binary value: ");
}

void loop() {

  if(previous_number != number) {
    videoPrintText(63, 3, "    ");
    videoPrintNumber(63, 3, number);
    videoPrintBits(63, 6, number);
    previous_number = number;
  }

  buttonsReadAllInputs();
  delay(50);

  if (BUTTON_PUSHED_ACTION_1 || BUTTON_PUSHED_LEFT || BUTTON_PUSHED_DOWN) {
    number -= 1;
  }

  if (BUTTON_PUSHED_ACTION_2 || BUTTON_PUSHED_RIGHT || BUTTON_PUSHED_UP) {
    number += 1;
  }

}
