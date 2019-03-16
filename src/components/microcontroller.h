#pragma once
#include "common.h"
#include "pin_components/pinComponent.h"
#include "pin_components/silserial.h"

struct PinMapping {
  shared_ptr<PinComponent> component;
  bool value;
};

class Microcontroller : public PinComponent {
  private:
    map<int, PinMapping> pinMap;

  public:
    shared_ptr<SILSerial> serial_in;
    bool powered;
    string name;
    int id;

    Microcontroller(string name, int id);

    void mapPin(int index, PinMapping mapping);
    void setPin(int64_t time, int index, bool value);
    shared_ptr<PinComponent> getComponent(int index);

    void activate(int64_t time);
    void deactivate(int64_t time);
};
