#include "pins.h"
#include "mbed.h"
#include "MPL3115A2.h"

DigitalOut myled(STATE_LED_RED);
DigitalOut motor(3);
DigitalOut drogue(6);
DigitalOut main_chute(7);

float max_alt;
bool falling;

I2C i2c_sensors(I2C_SENSOR_SDA, I2C_SENSOR_SCL);
Serial debug(DEBUG_RX, DEBUG_TX);

Altitude altitude;
MPL3115A2 altimeter(&i2c_sensors, &debug);

void start() {
  max_alt = 0;
  falling = false;
  altimeter.init();
}

void loop() {
  motor = 1;
  myled = (micros() / 1000000) & 1; // Flash every second

  altimeter.readAltitude(&altitude);
  float alt = altitude.altitude();
  max_alt = max(max_alt, alt);
  if (alt < max_alt - 50) {
    falling = true;
  }
  if (falling) {
    drogue = 1;
    if (alt < 200) {
      main_chute = 1;
    }
  }
}

int main() {
  start();
  while (1) {
    loop();
  }
}
