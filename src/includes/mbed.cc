#include "mbed.h"

int64_t micros() {
  assert(Environment::global_env != nullptr);
  return Environment::global_env->micros();
}

DigitalOut::DigitalOut(int pin) : pin(pin) { }

void DigitalOut::write(int value) {
  assert(Environment::global_env != nullptr);
  Environment::global_env->current_mcu->setPin(Environment::global_env->micros(), pin, value);
}

int DigitalOut::read() {
  assert(Environment::global_env != nullptr);
  ERROR("Read is not implemented");
}

DigitalOut& DigitalOut::operator=(int value) {
  write(value);
  return *this;
}

DigitalOut::operator int() {
  return read();
}

I2C::I2C(int sda, int scl) : sda(sda), scl(scl) {}