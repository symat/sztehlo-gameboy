#include <avr/pgmspace.h>
#include "sztehlo-video.h"


void resetVideo() {
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  delay(10);
  digitalWrite(11, HIGH);
  delay(50);
}

void setup() {
  // reset-eljük a videót, erre a gameboy-nál nem lesz lehetőség.
  // de ha Arduino-t használunk, akkor van hozzá elég kimenetünk és fontos is.
  resetVideo();

  videoInit();
}

void loop() {
  videoFillScreen(0b10101010);
  delay(1000);
  videoFillScreen(0b11110000);
  delay(1000);
  videoFillScreen(0b11111111);
  delay(1000);
}
