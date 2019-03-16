#include "pincomponent.h"

PinComponent::PinComponent(string name) : name(name), isActivated(false) {}

void PinComponent::deactivate(int64_t time) {}

bool PinComponent::activated() {
    return isActivated;
}
