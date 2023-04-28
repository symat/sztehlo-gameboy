#include "sztehlo-video.h"

void setup() {
  videoInit();

  videoFillScreen(BLACK_SCREEN_BYTE);
  videoPrintText(10, 4, "hello world!");
}


void loop() {
}
