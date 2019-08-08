#pragma once
#include "components/environment.h"
#include "components/noise.h"
#include "includes/mbed.h"

#define MPL3115A2_ADDRESS   0xC0    // Shifted 7-bit I2C address for sensor

class Altitude {
    // TODO: Support more of the API?
    // Possibly include unit conversions, etc.
    float _altitude;

public:
    Altitude();

    float altitude() const;
    void setAltitude(float value); 
};

class MPL3115A2 {
    char offsetAlt;
    Serial* debug;

public:
    MPL3115A2(I2C* sensor, Serial* debug = nullptr);

    char whoAmI();
    void init();
    Altitude* readAltitude(Altitude* a);
    void setOversampleRate(char rate);
    void setModeStandby();
    void setModeAltimeter();
    void setModeActive();
    char offsetAltitude();
    void setOffsetAltitude(const char offset);
};
