#include "mbed.h"

#define LED1 10

DigitalOut myled(LED1);
DigitalOut motor(3);
DigitalOut drogue(6);
DigitalOut main_chute(7);

float max_alt;
bool falling;

void start() {
  max_alt = 0;
  falling = false;
}

void loop() {
  motor = 1;
  myled = (micros() / 1000000) & 1; // Flash every second

  float alt = getAltitude();
  max_alt = max(max_alt, alt);
  if (alt < max_alt - 50) {
    falling = true;
  }
  if (falling) {
    drogue = 1;
    if (alt < 200) {
      main_chute = 1;
    }
  }
}

int main() {
  start();
  while (1) {
    loop();
  }
}
