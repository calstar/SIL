#include "pinComponent.h"

PinComponent::PinComponent(string name) : name(name) {}

void PinComponent::deactivate(int64_t time) {}

bool PinComponent::activated() {
    return isActivated;
}