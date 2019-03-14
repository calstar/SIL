#pragma once
#include "common.h" // TODO: Maybe take this out? We don't want code "seeing" anything it won't in the real MCU
#include "components/environment.h"
#include "includes/sensors.h"

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
