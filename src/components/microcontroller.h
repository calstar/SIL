#pragma once
#include "common.h"
#include "pin_components/pinComponent.h"

struct PinMapping {
  shared_ptr<PinComponent> component;
  bool value;
};

class Microcontroller {
  private:
    map<int, PinMapping> pinMap;

  public:
    string name;
    int id;

    Microcontroller(string name, int id);

    void mapPin(int index, PinMapping mapping);
    void setPin(int64_t time, int index, bool value);
};
