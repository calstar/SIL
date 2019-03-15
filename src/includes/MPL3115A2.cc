#include "includes/MPL3115A2.h"

Altitude::Altitude() : _altitude(0) {}

float Altitude::altitude() {
    return _altitude;
}

void Altitude::setAltitude(float value) {
    _altitude = value;
}

MPL3115A2::MPL3115A2(I2C* i2c, Serial* debug) : debug(debug) {}

char MPL3115A2::whoAmI() {
    return 0xC4;
}

void MPL3115A2::init() {
    offsetAlt = 0;
}

Altitude* MPL3115A2::readAltitude(Altitude* altitude) {
    double a = Environment::global_env->current_rocket->pos.z;
    altitude->setAltitude(a + offsetAlt);
    return altitude;
}

char MPL3115A2::offsetAltitude() {
    return offsetAlt;
}

void MPL3115A2::setOffsetAltitude(const char offset) {
    offsetAlt = offset;
}