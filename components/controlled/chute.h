#pragma once
#include "common.h"
#include "pinComponent.h"

class Chute : public PinComponent {
  double drag;

public:
  Chute(double drag, string name);

  double getDrag();
  void activate(int64_t time);
};
