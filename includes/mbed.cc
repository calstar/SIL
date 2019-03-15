#include "mbed.h"

int64_t micros() {
  assert(Environment::global_env != nullptr);
  return Environment::global_env->micros();
}

vec getAcc() {
  auto r = Environment::global_env->current_rocket;
  assert(r->acc != nullptr);
  return r->acc->getData();
}

float getAccX() {
  return getAcc().x;
}

float getAccY() {
  return getAcc().y;
}

float getAccZ() {
  return getAcc().z;
}

float getAltitude() {
  auto r = Environment::global_env->current_rocket;
  assert(r->alt != nullptr);
  return r->alt->getData();
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
