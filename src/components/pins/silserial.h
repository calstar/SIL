#pragma once
#include "common.h"
#include "pincomponent.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

enum SERIAL_MODE {
  NORMAL,
  SIL_INPUT,
  SIL_OUTPUT,
  RADIO_INPUT,
  RADIO_OUTPUT
};

class SILSerial : public PinComponent, public enable_shared_from_this<SILSerial> {
  vector<char> buffer;

public:
  SERIAL_MODE mode;

  SILSerial();

  void add(string s);
  void add(const char* buf, int len);
  ssize_t get(char* buf, int len);
  char getc();
  bool empty();
  void activate(int64_t time);
  void deactivate(int64_t time);
};
