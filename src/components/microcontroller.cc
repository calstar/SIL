#include "microcontroller.h"

Microcontroller::Microcontroller(string name, int id) : PinComponent(name), name(name), id(id) {
  powered = true;
  serial_in = NULL;
}

void Microcontroller::mapPin(int index, PinMapping mapping) {
    assert(pinMap.count(index) == 0);
    pinMap.insert({index, mapping});
}

void Microcontroller::setPin(int64_t time, int index, bool value) {
    if (pinMap.count(index) == 0) {
      cerr << "Pin " << index << " not mapped" << endl;
      assert(false);
    }
    if (value != pinMap[index].value) {
        if (!pinMap[index].component) {
          cerr << "Pin " << index << " missing component" << endl;
          assert(false);
        }
        pinMap[index].value = value;
        if (value) {
            pinMap[index].component->activate(time);
        } else {
            pinMap[index].component->deactivate(time);
        }
    }
}

shared_ptr<PinComponent> Microcontroller::getComponent(int index) {
  if (pinMap.count(index) == 0) ERROR("getComponent() called with nonexistent pin");
  return pinMap[index].component;
}

void Microcontroller::activate(int64_t time) {
  powered = true;
  DEBUG_OUT << "MCU Activated: " << name << endl;
}

void Microcontroller::deactivate(int64_t time) {
  powered = false;
  DEBUG_OUT << "MCU Deactivated: " << name << endl;
}
