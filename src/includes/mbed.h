#pragma once
#include "common.h" // TODO: Maybe take this out? We don't want code "seeing" anything it won't in the real MCU
#include "components/environment.h"
#include "flatbuffers/flatbuffers.h"

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

void wait(float s);
int64_t micros();

float getAccX();
float getAccY();
float getAccZ();
float getAltitude();

class DigitalOut {
  int pin;
  int current_value;
public:
  DigitalOut(int pin);
  void write(int value);
  int read();
  DigitalOut& operator= (int value);
  operator int();
};

class Timer {
  us_timestamp_t previous;
  us_timestamp_t startTime;
  bool running;

public:
  Timer();
  void start();
  void reset();
  void stop();
  us_timestamp_t read_high_resolution_us();
};

class Serial {
  int rxpin;
  int txpin;
public:
  Serial(int tx, int rx, int baud = 0);
  void baud(int rate);
  void set_blocking(bool blocking);
  void printf(const char* format, ...);
  void write(uint8_t* buf, int len, void* callback);
  ssize_t read(uint8_t* buf, int len, void* callback);
  bool readable();
  char getc();
};

class UARTSerial : public Serial {
public:
  UARTSerial(int tx, int rx, int baud = 0);
  void write(uint8_t* buf, int len);
  ssize_t read(uint8_t* buf, int len);
};

class I2C {
  // Just a skeleton class, sensors should be implemented in their own classes instead of implementing I2C protocols
  int sda;
  int scl;

public:
  I2C(int sda, int scl);
};
