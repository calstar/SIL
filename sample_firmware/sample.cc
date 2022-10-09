#include "pins.h"
#include "mbed.h"
#include "BMP388.h"

// TODO Define your pins here
// Syntax: DigitalOut <pin_name>(<pin_number>); (without the angle brackets)

float max_alt;
// TODO What other vars  might it be useful to define? 

I2C i2c_sensors(I2C_SENSOR_SDA, I2C_SENSOR_SCL);
Serial debug(DEBUG_TX, DEBUG_RX);

Pressure pressure;
BMP388 barometer(&i2c_sensors, &debug);


void start() {
    //Assign any other variables here. 
    max_alt = 0;
    barometer.init();
}

void loop() {
    // Read pressure using the BMP388 library
    barometer.readPressure(&pressure);
    float pressure_pascals = pressure.pressure();
    
    // TODO Convert pressure to altitude.
    // TODO Update max altitude 
    // TODO Deploy parachutes at the correct time by setting pins to a boolean true value

}

int main() {
    start();
    while (true) {
        loop();
    }
}
