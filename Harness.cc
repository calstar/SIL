#include "Harness.h"
#include "Simulator.h"
#include "SIL.h"
#include "common.h"
#include "Sensors.h"

namespace code0 { void loop(); }
// void code1() {}
// void code2() {}
// void code3() {}
// void code4() {}

int64_t micros() {
  assert(global_env != NULL);

  return global_env->micros();
}

void pinMode(int pin, uint8_t mode) {
  assert(global_env != NULL);
  global_env->pinMode(current_mcu, pin, mode);
}

vec getAcc() {
  auto r = curr_roc();
  assert(r->acc != NULL);
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
  auto r = curr_roc();
  assert(r->alt != NULL);
  return r->alt->getData();
}

DigitalOut::DigitalOut(int pin) : pin(pin) { }

void DigitalOut::write(int value) {
  assert(global_env != NULL);
  pinMode(pin, OUTPUT);
  global_env->setPin(current_mcu, pin, value);
}

int DigitalOut::read() {
  assert(global_env != NULL);
  pinMode(pin, OUTPUT);
  return global_env->getPin(current_mcu, pin);
}

DigitalOut& DigitalOut::operator=(int value) {
  write(value);
  return *this;
}

DigitalOut::operator int() {
  return read();
}
