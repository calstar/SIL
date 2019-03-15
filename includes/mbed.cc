#include "mbed.h"

int64_t micros() {
  assert(Environment::global_env != NULL);

  return Environment::global_env->micros();
}

void pinMode(int pin, uint8_t mode) {
  assert(Environment::global_env != NULL);
  Environment::global_env->pinMode(Environment::current_mcu, pin, mode);
}

vec getAcc() {
  auto r = Environment::curr_roc();
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
  auto r = Environment::curr_roc();
  assert(r->alt != NULL);
  return r->alt->getData();
}

DigitalOut::DigitalOut(int pin) : pin(pin) { }

void DigitalOut::write(int value) {
  assert(Environment::global_env != NULL);
  pinMode(pin, OUTPUT);
  Environment::global_env->setPin(Environment::current_mcu, pin, value);
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


void Timer::start() {
  start_time = micros();
}

us_timestamp_t Timer::read_high_resolution_us() {
  return micros() - start_time;
}

Serial::Serial(int tx, int rx) {
  rxpin = rx;
  txpin = tx;
}

SILSerial* getSerial(int pin) {
  auto mcu = Environment::curr_mcu_ptr();
  auto roc = Environment::curr_roc();

  if (mcu->pin_map.count(pin) == 0) ERROR("getSerial() called with nonexistent pin");
  auto pmap = mcu->pin_map[pin];
  if (pmap.type != CONNECTION_TYPE::SERIAL_RX && pmap.type != CONNECTION_TYPE::SERIAL_TX) {
    ERROR();
  }
  return &(roc->serials.at(pmap.index));
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
