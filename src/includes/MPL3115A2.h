#pragma once
#include "components/environment.h"
#include "components/noise.h"
#include "includes/mbed.h"

class Altitude {
    // TODO: Support more of the API?
    // Possibly include unit conversions, etc.
    float _altitude;

public:
    Altitude();

    float altitude();
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
    char offsetAltitude();
    void setOffsetAltitude(const char offset);
};
