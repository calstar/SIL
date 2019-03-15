#pragma once
#include "common.h" // TODO: Maybe take this out? We don't want code "seeing" anything it won't in the real MCU
#include "components/environment.h"
#include "includes/sensors.h"
#include "includes/flatbuffers.h"

namespace code0 { void start(); }
namespace code1 { void start(); }
namespace code2 { void start(); }
namespace code3 { void start(); }
namespace code4 { void start(); }

namespace code0 { void loop(); }
namespace code1 { void loop(); }
namespace code2 { void loop(); }
namespace code3 { void loop(); }
namespace code4 { void loop(); }

typedef int64_t us_timestamp_t;

int64_t micros();

// void pinMode(int pin, uint8_t mode);
// int digitalRead(int pin);
// void digitalWrite(int pin, bool high);

float getAccX();
float getAccY();
float getAccZ();
float getAltitude();

class DigitalOut {
  int pin;
public:
  DigitalOut(int pin);
  void write(int value);
  int read();
  DigitalOut& operator= (int value);
  operator int();
};

class Timer {
public:
  void start();
  us_timestamp_t read_high_resolution_us();
};

class Serial {
public:
  Serial(int rx, int tx);
  void baud(int rate);
  void set_blocking(bool blocking);
  void printf(char* msg);
};
