#include "includes/BMP388.h"
#include <cmath>

Pressure::Pressure() : 
    _pressure{0}
{

}

float Pressure::pressure() const {
    return _pressure;
}

void Pressure::setPressure(float value) {
    _pressure = value;
}

BMP388::BMP388(I2C *const i2c, Serial *const debug) :
    debug{debug}
{

}

void BMP388::init() {
    initialized = true;
}

Pressure *BMP388::readPressure(Pressure *const p) const {

    // Helps with debugging. Would not occurr with actual hardware, of course.
    assert(initialized == true && "BMP388: not initialized");

    // Sim calculates with feet, so calculate pressure from feet
    const double h_ft = Environment::global_env->current_rocket->measured_pos.z * 3.28084;
    const double p_pa = 101325*std::pow(1.0 - h_ft/145366.45, 5.25530);

    p->setPressure(p_pa);
    return p;
}
