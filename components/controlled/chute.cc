#include "chute.h"

Chute::Chute(double drag, string name) : drag(drag), PinComponent(name) {}

void Chute::activate(int64_t time) {
  isActivated = true;
}

double Chute::getDrag() {
  if (activated) {
    return drag;
  }
  return 0;
}
