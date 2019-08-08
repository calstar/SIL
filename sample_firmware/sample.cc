#include "pins.h"
#include "mbed.h"
#include "MPL3115A2.h"

DigitalOut myled(STATE_LED_RED);

float max_alt;

I2C i2c_sensors(I2C_SENSOR_SDA, I2C_SENSOR_SCL);
Serial debug(DEBUG_RX, DEBUG_TX);

Altitude altitude;
MPL3115A2 altimeter(&i2c_sensors, &debug);

void start() {
    max_alt = 0;
    altimeter.init();
}

void loop() {
    myled = (micros() / 1000000) & 1; // Flash every second

    altimeter.readAltitude(&altitude);
    const float alt = altitude.altitude();

    max_alt = max(max_alt, alt);
}

int main() {
    start();
    while (true) {
        loop();
    }
}
