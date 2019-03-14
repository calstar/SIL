#include "chute.h"

Chute::Chute(double drag, string name) : drag(drag), name(name) {
  activated = false;
}

void Chute::activate() {
  activated = true;
}

double Chute::getDrag() {
  if (activated) {
    return drag;
  }
  return 0;
}
