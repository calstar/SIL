#include "mbed.h"

void wait(float s) {
  this_thread::sleep_for(chrono::nanoseconds((int64_t)(s * 1E9)));
}

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

Timer::Timer() {}

void Timer::start() {
  if (!running) {
    running = true;
    startTime = micros();
  }
}

void Timer::reset() {
  startTime = micros();
}

void Timer::stop() {
  running = false;
  previous += (startTime - micros());
}

us_timestamp_t Timer::read_high_resolution_us() {
  if (running) {
    return micros() - startTime + previous;
  } else {
    return previous;
  }
}

Serial::Serial(int rx, int tx) {}

I2C::I2C(int sda, int scl) : sda(sda), scl(scl) {}