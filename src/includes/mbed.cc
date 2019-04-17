#include "mbed.h"
#include <stdarg.h>

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
  current_value = value;
}

int DigitalOut::read() {
  return current_value;
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

Serial::Serial(int tx, int rx, int baud) {
  rxpin = rx;
  txpin = tx;
}

shared_ptr<SILSerial> getSerial(int pin) {
  auto mcu = Environment::global_env->current_mcu;
  auto roc = Environment::global_env->current_rocket;


  shared_ptr<SILSerial> ret = dynamic_pointer_cast<SILSerial>(mcu->getComponent(pin));
  return ret;
}

void Serial::baud(int rate) {

}

void Serial::set_blocking(bool blocking) {

}

void Serial::printf(const char* format, ...) {
  va_list arg;
  char msg[1024];
  va_start (arg, format);
  vsprintf (msg, format, arg);
  va_end (arg);

  getSerial(txpin)->add(msg, strlen(msg));
}

void Serial::write(uint8_t* buf, int len, void* callback) {
  getSerial(txpin)->add((char*)buf, len);
}

bool Serial::readable() {
  return !getSerial(rxpin)->empty();
}

char Serial::getc() {
  return getSerial(rxpin)->getc();
}

I2C::I2C(int sda, int scl) : sda(sda), scl(scl) {}
