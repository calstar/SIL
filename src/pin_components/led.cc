#include "led.h"

LED::LED(string name) : PinComponent(name) {}

void LED::activate(int64_t time) {
  isActivated = true;
}

void LED::deactivate(int64_t time) {
  isActivated = false;
}
