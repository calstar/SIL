#ifdef SIL
#include "Harness.h"
namespace code0 {
#else
#include "mbed.h"
#endif

#define LED1 10

DigitalOut myled(LED1);
DigitalOut motor(3);
DigitalOut drogue(6);
DigitalOut main(7);

float max_alt = 0;
bool falling = false;

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
      main = 1;
    }
  }
}

#ifdef SIL
} // Close namespace
#else
int main() {
  while(1) {
    loop();
  }
}
#endif
