#pragma once

#include "components/environment.h"
#include "components/noise.h"
#include "includes/mbed.h"

class Pressure {
    float _pressure;
    // TODO: like Altitude, potentially support unit conversions, etc
public:
    Pressure();

    float pressure() const;
    void setPressure(float value);
};

class BMP388 {
    Serial *debug;
    bool initialized = false;
public:
    BMP388(I2C *const i2c, Serial *const debug = nullptr);

    void init();
    Pressure *readPressure(Pressure *const p) const;
};