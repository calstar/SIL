#include "led.h"

LED::LED(string name) : name(name) {
  activated = false;
}

bool LED::val() {
  return activated;
}

void LED::set(bool val) {
  activated = val;
}
