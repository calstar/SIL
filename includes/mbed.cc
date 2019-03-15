#include "mbed.h"

int64_t micros() {
  assert(Environment::global_env != nullptr);

  return Environment::global_env->micros();
}

void pinMode(int pin, uint8_t mode) {
  assert(Environment::global_env != nullptr);
  Environment::global_env->pinMode(Environment::current_mcu, pin, mode);
}

vec getAcc() {
  auto r = Environment::curr_roc();
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
  auto r = Environment::curr_roc();
  assert(r->alt != nullptr);
  return r->alt->getData();
}

DigitalOut::DigitalOut(int pin) : pin(pin) { }

void DigitalOut::write(int value) {
  assert(Environment::global_env != nullptr);
  pinMode(pin, OUTPUT);
  Environment::global_env->setPin(Environment::current_mcu, pin, value);
}

int DigitalOut::read() {
  assert(Environment::global_env != nullptr);
  pinMode(pin, OUTPUT);
  return Environment::global_env->getPin(Environment::current_mcu, pin);
}

DigitalOut& DigitalOut::operator=(int value) {
  write(value);
  return *this;
}

DigitalOut::operator int() {
  return read();
}
