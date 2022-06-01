#include <cli.h>

void setup() {
  Serial.begin(19200);
  initFAT();
}

void loop() {
  readFromSerial();
}