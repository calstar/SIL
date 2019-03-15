#include "microcontroller.h"

Microcontroller::Microcontroller(string name, int id) : name(name), id(id) { }

void Microcontroller::mapPin(int index, PinMapping mapping) {
    assert(pinMap.count(index) == 0);
    pinMap.insert({index, mapping});
}

void Microcontroller::setPin(int64_t time, int index, bool value) {
    if (value != pinMap[index].value) {
        pinMap[index].value = value;
        if (value) {
            pinMap[index].component->activate(time);
        } else {
            pinMap[index].component->deactivate(time);
        }
    }
}
